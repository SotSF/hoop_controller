#include <FastLED.h>
#include "Globals.h"

void runNight(CRGB* pBuf, int tick, int loopCnt) {
  for (int i = 0; i < NUM_LEDS; i++){
    pBuf[i] = CRGB::Blue;
  }
  
}
