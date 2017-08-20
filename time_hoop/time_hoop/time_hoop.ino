#include <FastLED.h>
#include <TimeLib.h>
#include <math.h>
#include "Fire.h"
#include "Night.h"

FASTLED_USING_NAMESPACE

#define DATA_PIN    12
#define CLK_PIN     7

#define LED_TYPE    NEOPIXEL
#define COLOR_ORDER BGR
#define NUM_LEDS    144

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

// Sun consts
#define SUN_SPREAD 5
#define SUN_HUE 40
#define SUN_SAT 255
#define SUN_V 255
CRGB leds[NUM_LEDS];


void setup() {
  // set the Time library to use Teensy 3.0's RTC to keep time
  setSyncProvider(getTeensy3Time);

  Serial.begin(115200);
  while (!Serial);  // Wait for Arduino Serial Monitor to open
  delay(3000); // 3 second delay for recovery

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}


typedef void (*displayFunc[])(CRGB*, int);

displayFunc funArray = {
  &runNight,
  &runFire
};

// Main function which is looped continuously
void loop()
{

  // display time on LEDs
  //LEDdisplay();
  int numPatterns = 2;
  int funcIndex = getDisplayFuncIndex(numPatterns);
      Serial.print("ind: ");
    Serial.print(funcIndex);
    Serial.println();
  // 20 sec * 60 min * 24 h
  // time = hour * 60 * 24
  // total / num_patterns
  //runFire(leds, NUM_LEDS);
  //runNight(leds, NUM_LEDS);
  funArray[funcIndex](leds, NUM_LEDS);
  OverlaySun();
  FastLED.show();
  FastLED.delay(1000/FRAMES_PER_SECOND);

  delay(1000);
}

int getDisplayFuncIndex(int numPatterns) {
  int sliceNum = ((hour() * 60 *60 ) + (minute() *60) + second()) / 20;
  return ((int)floor(sliceNum) % numPatterns);
}

void OverlaySun() {
    int pos = floor(hour() * (NUM_LEDS / 24));
    // Set the center of the sun
    Serial.print("hour: ");
    Serial.print(SUN_SPREAD);

    Serial.print(", center: ");
    Serial.print(pos);
    Serial.println();

    leds[pos] = CHSV(SUN_HUE, SUN_SAT, SUN_V);

    // spread the sun center
    if (SUN_SPREAD > 0) {

        for (int i = 1; i <= SUN_SPREAD; i++) {
            int forwardPos = (pos + i) % NUM_LEDS;
            int rearPos = (pos + NUM_LEDS - i) % NUM_LEDS;

            leds[forwardPos] = CHSV(SUN_HUE, SUN_SAT, floor(SUN_V * ((float)(SUN_SPREAD + 1 - i) / (SUN_SPREAD + 1))));
            leds[rearPos] = CHSV(SUN_HUE, SUN_SAT, floor(SUN_V * ((float)(SUN_SPREAD + 1 - i) / (SUN_SPREAD + 1))));;
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
