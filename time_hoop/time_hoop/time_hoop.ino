#include <FastLED.h>
#include <TimeLib.h>
#include <math.h>

#include "Fire.h"
#include "Night.h"
#include "Wheel.h"

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
//int heat[NUM_LEDS];


bool isInitialized;
int loopCnt; // good for 12 days = 1mil seconds? TODO





void setup() {
  // set the Time library to use Teensy 3.0's RTC to keep time
  setSyncProvider(getTeensy3Time);

  //Serial.begin(115200);
  //while (!Serial);  // Wait for Arduino Serial Monitor to open
  delay(3000); // 3 second delay for recovery

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  
  loopCnt = 0;
   isInitialized = false;

}

typedef void (*displayFunc[])(int);

displayFunc funArray = {
  &runBrightOrange//,
  //&runFire
};

typedef void (*displayFuncClass[])(CRGB*, int);

displayFuncClass funArray2 = {
  &runFire,
  &runNight,
  &runFire,
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
  //funArray[funcIndex](tick);
  funArray2[funcIndex](patternBuff, tick);
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


void runBrightOrange(int tick) {
  for ( int i = 0; i < NUM_LEDS; i++) {
    patternBuff[i] = CRGB(250, 0, 10);
  }
}

