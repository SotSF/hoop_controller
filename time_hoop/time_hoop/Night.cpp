#include <FastLED.h>

void runNight(CRGB* leds, int num_leds, int tick) {
  for (int i = 0; i < num_leds; i++){
    leds[i] = CRGB::Blue;
  }
  
}
