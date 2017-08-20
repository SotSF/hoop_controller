#include <FastLED.h>
#include <TimeLib.h>
#include <math.h>

FASTLED_USING_NAMESPACE

#define DATA_PIN    12
#define CLK_PIN     7

#define LED_TYPE    NEOPIXEL
#define COLOR_ORDER BGR
#define NUM_LEDS    144

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

// Sun consts
// #define SUN_COLOR  {200, 50, 50};
#define SUN_SPREAD 10

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


// Main function which is looped continuously
void loop()
{

  // display time on LEDs
  LEDdisplay();
  OverlaySun();
  FastLED.show();
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  delay(1000);
}

void OverlaySun() {
    CRGB SUN_COLOR = CHSV(68, 65, 100);
    int pos = floor(hour() * (NUM_LEDS / 24));
    // Set the center of the sun
    Serial.print("hour: ");
    Serial.print(SUN_SPREAD);

    Serial.print(", center: ");
    Serial.print(pos);
    Serial.println();
    
    leds[pos] = SUN_COLOR;    

    // spread the sun center
    if (SUN_SPREAD > 0) {

        for (int i = 1; i <= SUN_SPREAD; i++) {
            /*CRGB scol = CHSV(
                SUN_COLOR[0], 
                SUN_COLOR[1], 
                floor(SUN_COLOR[2] * ((SUN_SPREAD + 1 - i) / (SUN_SPREAD + 1)))
            );*/
            int forwardPos = (pos + i) % NUM_LEDS;
            int rearPos = (pos + NUM_LEDS - i) % NUM_LEDS;
            leds[forwardPos] = SUN_COLOR;
            leds[forwardPos].setHue(floor(100 * ((SUN_SPREAD + 1 - i) / (SUN_SPREAD + 1))));
            leds[rearPos] = SUN_COLOR;
            leds[rearPos].setHue(floor(100 * ((SUN_SPREAD + 1 - i) / (SUN_SPREAD + 1))));;
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
