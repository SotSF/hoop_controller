#include "FastLED.h"
#include <TimeLib.h>

FASTLED_USING_NAMESPACE

#define DATA_PIN    12
#define CLK_PIN     7

#define LED_TYPE    NEOPIXEL
#define COLOR_ORDER BGR
#define NUM_LEDS    144

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

CRGB leds[NUM_LEDS];


void setup() {
  // set the Time library to use Teensy 3.0's RTC to keep time
  setSyncProvider(getTeensy3Time);

  delay(3000); // 3 second delay for recovery

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}


// Main function which is looped continuously
void loop()
{

  // display time on LEDs
  LEDdisplay();
  FastLED.show();
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  delay(1000);
}

void LEDdisplay() {
  if (second() == 0) {
      FastLED.clear();
  } else {
      leds[ second() ] += CRGB::Red;
   }
   leds[minute() + 60] += CRGB::Blue;
 }
