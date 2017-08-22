#include <FastLED.h>
#include "Globals.h"

int heat[NUM_LEDS];

// used for cool fire pattern
/*DEFINE_GRADIENT_PALETTE( coolfire_gp ) {
  0,     0,  0,  0,   //black
128,     63,191, 138,   //red
224,     65, 63, 191,   //bright yellow
255,   63,127,  191}; //full white */


CRGBPalette16 gPal;
// keeps the pixel safe from going off the edge - TODO there's probably a built-in for ..
int SF(int centroid, int i) {
  return (centroid + i) % NUM_LEDS;
}

void runFire(CRGB* pBuff, int tick) {
// for fire pattern
  gPal = HeatColors_p;

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
    CRGB color = ColorFromPalette( (CRGBPalette16&) gPal, colorindex);
    pBuff[j] = color;
  }
}
