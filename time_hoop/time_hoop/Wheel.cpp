#include <FastLED.h>
#include "Globals.h"
// wheel consts

CRGB MIN_COLOR = CRGB(0, 0, 0);
CRGB MAX_COLOR = CRGB(0, 250, 0);


CRGB scaleColor(int i) {
  int r = ((MAX_COLOR[0] - MIN_COLOR[0]) * ((float)i / CHUNK_SIZE)) + MIN_COLOR[0];
  int g = ((MAX_COLOR[1] - MIN_COLOR[1]) * ((float)i / CHUNK_SIZE)) + MIN_COLOR[1];
  int b = ((MAX_COLOR[2] - MIN_COLOR[2]) * ((float)i / CHUNK_SIZE)) + MIN_COLOR[2];
  return CRGB(round(r), round(g), round(b));
}

void runWheel(CRGB* pBuf, int tick) {
  if (tick == 0) {
    for (int i = 0; i < NUM_CHUNKS; i++) {
      for (int j = 0; j < CHUNK_SIZE; j++) {

        int newPos = i * CHUNK_SIZE + j;
        CRGB col = scaleColor(j);
        pBuf[newPos] = col;
      }
    }
  }

  for ( int i = NUM_LEDS - 1; i >= 0;  i--) {
    int pos = ((i - 1) + NUM_LEDS) % NUM_LEDS;
    pBuf[i] = pBuf[pos];
  }
}


