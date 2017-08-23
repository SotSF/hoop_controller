#ifndef Globals_h
#define Globals_h

#define NUM_LEDS    380

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  300

// Sun consts
#define SUN_SPREAD 16
#define SUN_HUE 70
#define SUN_SAT 255
#define SUN_V 255
// alternate because schimmy hates HSV
#define SUN_R 246
#define SUN_G 230
#define SUN_B 50
#define SUN_SPEED 2 // higher = slower strobe
#define NUM_CHUNKS 12
const int CHUNK_SIZE = floor((float)NUM_LEDS / NUM_CHUNKS);


#endif
