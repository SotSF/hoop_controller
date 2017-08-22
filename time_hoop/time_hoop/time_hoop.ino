#include <FastLED.h>
#include <TimeLib.h>
#include <math.h>
//#include "Fire.h"
//#include "Night.h"

FASTLED_USING_NAMESPACE

#define DATA_PIN    12
#define CLK_PIN     7

#define LED_TYPE    NEOPIXEL
#define COLOR_ORDER BGR
#define NUM_LEDS    380

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  60

// Sun consts
#define SUN_SPREAD 16
#define SUN_HUE 70
#define SUN_SAT 255
#define SUN_V 255
// alternate because schimmy hates HSV
#define SUN_R 246
#define SUN_G 230
#define SUN_B 50
#define SUN_SPEED 6 // higher = slower strobe


CRGB leds[NUM_LEDS];
CRGB patternBuff[NUM_LEDS];
int heat[NUM_LEDS];

CRGBPalette16 gPal;
bool isInitialized;
int loopCnt; // good for 12 days = 1mil seconds? TODO
#define NUM_CHUNKS 12
const int CHUNK_SIZE = floor((float)NUM_LEDS / NUM_CHUNKS);

// wheel consts

CRGB MIN_COLOR = CRGB(0, 0, 0);
CRGB MAX_COLOR = CRGB(0, 250, 0);


void setup() {
  // set the Time library to use Teensy 3.0's RTC to keep time
  setSyncProvider(getTeensy3Time);

  Serial.begin(115200);
  while (!Serial);  // Wait for Arduino Serial Monitor to open
  delay(3000); // 3 second delay for recovery

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  // for fire pattern
  gPal = HeatColors_p;

  loopCnt = 0;
   isInitialized = false;

}

typedef void (*displayFunc[])(int);

displayFunc funArray = {
  &runBrightOrange,
  &runWheel,
  &runFire
};

// Main function which is looped continuously
void loop()
{

  // display time on LEDs
  //LEDdisplay();
  int numPatterns = 3;
  int numSecondsEachPattern = 10;

  int nowInNumTicks = ((hour() * 60 * 60 ) + (minute() * 60) + second());

  int tick = nowInNumTicks % numSecondsEachPattern;
  if (tick == 0) {
    isInitialized = true;
  }
  int funcIndex = (nowInNumTicks / numSecondsEachPattern) % numPatterns;

  // if we haven't initialized, start off with full orange
  // if loopCnt < numSecondsEachPattern and tick has never been 0
  if (!isInitialized) {
    funcIndex = 0;
  }

  // 20 sec * 60 min * 24 h
  // time = hour * 60 * 24
  // total / num_patterns
  //runFire(tick);
  //runWheel(tick);
  //runNight(leds, NUM_LEDS, tick);
  funArray[funcIndex](tick);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = patternBuff[i];
  }
  OverlaySun((int)floor((float)loopCnt / SUN_SPEED));
  FastLED.show();
  FastLED.delay(1000 / FRAMES_PER_SECOND);
  loopCnt++;
}

void OverlaySun(int tick) {
  if ((tick %3) == 0 ) {
    tick = 0;
  }else if ((tick %3) == 1) {
    tick = 2;
  } else {
    tick = 1;
    }
  CRGB levelSun = CRGB(SUN_R, SUN_G, SUN_B);
  //CRGB levelSun = CRGB(255, 255, 55);
  CRGB midSun = CRGB(80, 80, 10);
  CRGB darkSun = CRGB(5, 5, 5);

  int pos = floor(((hour() + 12) % 24) * ((float)NUM_LEDS / 24));
  // Set the center of the sun
  for ( int i = 0 ; i < SUN_SPREAD; i++) {
    int forwardPos = (pos + i) % NUM_LEDS;
    int rearPos = (pos + NUM_LEDS - i ) % NUM_LEDS;
    leds[forwardPos] = levelSun;
    leds[forwardPos].fadeToBlackBy((int)floor(((float)i /SUN_SPREAD) * 255));
    leds[rearPos] = levelSun;
    leds[rearPos].fadeToBlackBy((int)floor(((float)i /SUN_SPREAD) * 255));
  }

  // spread the sun center
  for (int i = tick + 1; i <= SUN_SPREAD-4; i+=3) {
      for (int j = 0; j < 3; j++) {
        CRGB color = levelSun;
        if (j == 1) {
          color = midSun;
        }
        if (j == 2) {
          color = darkSun;
        }

        int forwardPos = (pos + i + j) % NUM_LEDS;
        int rearPos = (pos + NUM_LEDS - i - j) % NUM_LEDS;

        leds[forwardPos] = color;
        leds[forwardPos].fadeToBlackBy((int)floor(((float)i /SUN_SPREAD) * 255));
        leds[rearPos] = color;
        leds[rearPos].fadeToBlackBy((int)floor(((float)i /SUN_SPREAD) * 255));
      }
  }
}

time_t getTeensy3Time()
{
  return Teensy3Clock.get();
}

void LEDdisplay() {
  if (second() == 0) {
    FastLED.clear();
  } else {
    leds[ second() ] += CRGB::Red;
  }
  leds[minute() + 60] += CRGB::Blue;
}

CRGB average(CRGB* lst, int len) {
  int r = 0;
  int g = 0;
  int b = 0;
  for (int i = 0; i < len; i++) {
    r += lst[i].r;
    g += lst[i].g;
    b += lst[i].b;
  }
  return CRGB(floor((float)r / len), floor((float)g / len), floor((float)b / len));
}

// keeps the pixel safe from going off the edge - TODO there's probably a built-in for ..
int SF(int centroid, int i) {
  return (centroid + i) % NUM_LEDS;
}

void runFire(int tick) {

  int COOLING = 130; // decrease to make white-hot
  int SPARKING = 170;

  if (tick == 0) {
    for (int i = 0; i < NUM_LEDS; i++) {
       heat[i] = 40;
    }
  }
  // cool down whole LED strand
  // for each chunk, get centroid
  //   leds drift forwards & backwards
  //   ignite sparks forwards & backwards
  // map whole

  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8(heat[i], random8(0, floor(((float)COOLING * 10) / CHUNK_SIZE) + 1));
  }

  for (int j = 0; j < NUM_CHUNKS; j++) {
    int half_chunk = floor((float)CHUNK_SIZE / 2);
    int cent = floor(j * 2 * half_chunk) + half_chunk; // shouldn't need floor

    // Step 2.  leds from each cell drifts 'up' and diffuses a little
    for ( int k = half_chunk + floor(half_chunk / 2) - 4; k > 0; k--) {

      int prev[4];
      prev[0] = heat[SF(cent, k - 1)];
      prev[1] = heat[SF(cent, k - 2)];
      prev[2] = heat[SF(cent, k - 3)];
      prev[3] = heat[SF(cent, k - 4)];

      int nxt[4];
      nxt[0] =  heat[SF(cent, -1 * (k - 1))];
      nxt[1] =  heat[SF(cent, -1 * (k - 2))];
      nxt[2] =  heat[SF(cent, -1 * (k - 3))];
      nxt[3] =  heat[SF(cent, -1 * (k - 4))];

      heat[SF(cent, k)] = floor((float)(prev[0] + prev[1] + prev[2] + prev[3]) / 4);
      heat[SF(cent, -1 * k)] = floor((float)(nxt[0] + nxt[1] + nxt[2] + prev[3]) / 4);
    }

    // Step 3.  Randomly ignite new 'sparks' of leds near the bottom
    if ( random8(j) < SPARKING ) {
      int y = random8(7 + j);
      heat[SF(cent, y)] = qadd8(heat[SF(cent, y)], random8(100, 140));
      int z = random8(8 + j);
      heat[SF(cent, z)] = qadd8(heat[SF(cent, z)], random8(100, 140));
    }
  }

  // Step 4.  Map from leds cells to LED colors
  for ( int j = 0; j < NUM_LEDS; j++) {

    // Scale the leds value from 0-255 down to 0-240
    // for best results with color palettes.
    byte colorindex = scale8( heat[j], 240);
    CRGB color = ColorFromPalette( gPal, colorindex);
    patternBuff[j] = color;
  }
}

void runWheel(int tick) {
  if (tick == 0) {
    for (int i = 0; i < NUM_CHUNKS; i++) {
      for (int j = 0; j < CHUNK_SIZE; j++) {

        int newPos = i * CHUNK_SIZE + j;
        CRGB col = scaleColor(j);
        patternBuff[newPos] = col;
      }
    }
  }

  for ( int i = NUM_LEDS - 1; i >= 0;  i--) {
    int pos = ((i - 1) + NUM_LEDS) % NUM_LEDS;
    patternBuff[i] = patternBuff[pos];
  }
}


CRGB scaleColor(int i) {
  int r = ((MAX_COLOR[0] - MIN_COLOR[0]) * ((float)i / CHUNK_SIZE)) + MIN_COLOR[0];
  int g = ((MAX_COLOR[1] - MIN_COLOR[1]) * ((float)i / CHUNK_SIZE)) + MIN_COLOR[1];
  int b = ((MAX_COLOR[2] - MIN_COLOR[2]) * ((float)i / CHUNK_SIZE)) + MIN_COLOR[2];
  return CRGB(round(r), round(g), round(b));
}

void runBrightOrange(int tick) {
  for ( int i = 0; i < NUM_LEDS; i++) {
    patternBuff[i] = CRGB(250, 0, 10);
  }
}

