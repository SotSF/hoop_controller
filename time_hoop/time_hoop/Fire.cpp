#include <FastLED.h>

/*
//Play with for different strip colors
CRGB HeatColor(int temperature) {
  CRGB heatcolor = {0,0,0};
  // which can then be easily divided into three
  // equal 'thirds' of 64 units each.
  int t192 = scale8_video( temperature, 192);

  // calculate a value that ramps up from
  // zero to 255 in each 'third' of the scale.
  int heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252

  // now figure out which third of the spectrum we're in:
  if( t192 & 0x80) {
    // we're in the hottest third
    heatcolor[0] = 255; // full red
    heatcolor[1] = 255; // full green
    heatcolor[2] = heatramp; // ramp up blue

  } else if( t192 & 0x40 ) {
    // we're in the middle third
    heatcolor[0] = 255; // full red
    heatcolor[1] = heatramp; // ramp up green
    heatcolor[2] = 0; // no blue

  } else {
    // we're in the coolest third
    heatcolor[0] = heatramp; // ramp up red
    heatcolor[1] = 0; // no green
    heatcolor[2] = 0; // no blue
  }

  return heatcolor;
}
*/

// keeps the pixel safe from going off the edge - TODO there's probably a built-in for ..
int SF(int NUM_LEDS, int centroid, int i) {
  return (centroid + i) % NUM_LEDS;
}

void runFire(CRGB* leds, int NUM_LEDS, int tick) {

  int buffer[NUM_LEDS];
  // if first time through, reset the strip
  if (tick == 0) {
    for( int i = 0; i < NUM_LEDS; i++) {
      buffer[i] = 0;
    }
  } else {
    for( int i = 0; i < NUM_LEDS; i++) {
      buffer[i] = leds[i];
    }
  }

  int NUM_CHUNKS = 12;
  int CHUNK_SIZE = NUM_LEDS / NUM_CHUNKS; 

  int COOLING = 70; // decrease to make white-hot
  int SPARKING = 210;

  for (int i = 0; i < NUM_LEDS; i++){
    buffer[i] = CRGB(40, 255, 55);
  }
  // cool down whole LED strand
  // for each chunk, get centroid
  //   heat drift forwards & backwards
  //   ignite sparks forwards & backwards
  // map whole

  // Step 1.  Cool down every cell a little
  for( int i = 0; i < NUM_LEDS; i++) {
    buffer[i] = qsub8(buffer[i], random8(0, floor((COOLING * 10) / CHUNK_SIZE) + 1));
  }

  for (int j = 0; j < NUM_CHUNKS; j++) {
    int half_chunk = floor(CHUNK_SIZE / 2);
    int cent = (j*2*half_chunk) + half_chunk; // shouldn't need floor

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k = half_chunk + floor(half_chunk / 3) - 3; k > 0; k--) {
      Serial.print("location: ");
      Serial.print(SF(NUM_LEDS, cent, k));
      Serial.println();

      Serial.print("val at -1: ");
      Serial.print(buffer[SF(NUM_LEDS, cent, k - 1)]);
      Serial.println();

      Serial.print("pre divide: ");
      Serial.print((buffer[SF(NUM_LEDS, cent, k - 1)] + buffer[SF(NUM_LEDS, cent, k - 2)] + buffer[SF(NUM_LEDS, cent, k - 3)]));
      Serial.println();

      Serial.print("val: ");
      Serial.print(floor((buffer[SF(NUM_LEDS, cent, k - 1)] + buffer[SF(NUM_LEDS, cent, k - 2)] + buffer[SF(NUM_LEDS, cent, k - 3)] ) / 3));
      Serial.println();

      buffer[SF(NUM_LEDS, cent, k)] = max(floor((buffer[SF(NUM_LEDS, cent, k - 1)] + buffer[SF(NUM_LEDS, cent, k - 2)] + buffer[SF(NUM_LEDS, cent, k - 3)] ) / 3), 0);
      buffer[SF(NUM_LEDS, cent, -1*k)] = max(floor((buffer[SF(NUM_LEDS, cent, -1*(k - 1))] + buffer[SF(NUM_LEDS, cent, -1*(k - 2))] + buffer[SF(NUM_LEDS, cent, -1*(k - 3))] ) / 3), 0);
    }

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8(j) < SPARKING ) {
      int y = random8(7 + j);
      buffer[SF(NUM_LEDS, cent, y)] = qadd8(buffer[SF(NUM_LEDS, cent, y)], random8(100,170));
      int z = random8(8 + j);
      buffer[SF(NUM_LEDS, cent, z)] = qadd8(buffer[SF(NUM_LEDS, cent, z)], random8(100,170));
    }
  }

  // Step 4.  Map from heat cells to LED colors
  for( int j = 0; j < NUM_LEDS; j++) {
    leds[j] = HeatColor(buffer[j]);
  }
}
