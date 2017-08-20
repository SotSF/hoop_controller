#include <FastLED.h>

void runFire(CRGB* leds, int num_leds) {
  for (int i = 0; i < 15; i++){
    leds[i] = CRGB::Green;
  }
  
}
