#include <FastLED.h>
#include <TimeLib.h>
#include "Globals.h"




void runBreath(CRGB* pBuf, int tick, int loopCnt, CRGB color) {
  int full = 360;
  int half = 180;
  for (int i = 0; i < NUM_LEDS; i++){
    pBuf[i] = color;
    // if before half, we want to add, otherwise start brightening again
    //Serial.print("before: ");
    //Serial.println(pBuf[i].b);
    //Serial.print("after:");
             Serial.print(loopCnt);

    if ((loopCnt % full) < half) {

      Serial.print("- before half; ");
      Serial.print(255 * (loopCnt % full) / (float)(full));
      Serial.println();
      color.fadeToBlackBy(floor(255 * ((loopCnt % full) / (float)(full))));//.fadeToBlackBy((int)floor(((tick % full) / (float)full) * 255));
      fill_solid(pBuf, NUM_LEDS, color);
      //pBuf[i].fadeToBlackBy((int)floor(((second() % full) / (float)full) * 255));
    } else {
      Serial.print("- after half; ");
      Serial.print(255 * ((full - (loopCnt % full))/(float)full));
      Serial.println();

           // color.fadeToBlackBy((int)floor(((tick % full) / (float)full) * 255));
       color.fadeToBlackBy(floor(255 * ((full - (loopCnt % full))/(float)full)));
      fill_solid(pBuf, NUM_LEDS, color);
      //pBuf[i].fadeToBlackBy((int)floor(((1 - (second() % full)) / (float)full) * 255));
    }
  //      Serial.print(pBuf[i].b);
//
  }
}

void runBlueBreath(CRGB* pBuf, int tick, int loopCnt) {
  //Serial.print("in blue");
  runBreath(pBuf, tick, loopCnt, CRGB::Blue);
}
/*void runGreenBreath(CRGB* pBuf, int tick) {
  runBreath(pBuf, tick,  CRGB::Green);
}
void runPurpleBreath(CRGB* pBuf, int tick) {
  runBreath(pBuf, tick, CRGB::Purple);
}
void runRedBreath(CRGB* pBuf, int tick) {
  runBreath(pBuf, tick, CRGB::Red);
}*/
