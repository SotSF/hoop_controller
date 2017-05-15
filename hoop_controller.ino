#include "Chords.h"
#include "Notes.h"
#include "Song2300.h"
#include "Song.h"
#include "FastLED.h"

FASTLED_USING_NAMESPACE

#define DATA_PIN    5
#define CLK_PIN     7
#define LED_TYPE    NEOPIXEL
#define COLOR_ORDER BGR
#define NUM_LEDS    432
    CRGB leds[NUM_LEDS];

#define BRIGHTNESS          30
#define FRAMES_PER_SECOND  120
const char* NoteNames[] = { "rest","c ","cs","d ","ds","e ","f ","fs","g ","gs","a ","as","b ","C ","Cs","D ","Ds","E ","F ","Fs","G ","Gs","A ","As","B " };
void setup() {
    delay(3000); // 3 second delay for recovery

    FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);

    Serial.begin(9600);
    Serial.print("b fifths:");
    Serial.println(b_fifths, BIN);
    Serial.print("e minor:");
    Serial.println(e_minor, BIN);
    Serial.print("e octave:");
    Serial.println(e_octave, BIN);
    Serial.print("a octave:");
    Serial.println(a_octave, BIN);
    Serial.println();
    for (int i = 0; i < 25; i++) {
        Serial.print(NoteNames[i]);
        Serial.print(": ");
        Serial.println(Notes[i], BIN);
    }
}

void loop()
{
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(5, 5, 25);
    }
    chord composite = Song2300.next();
    Serial.print("Inbound note: ");
    Serial.println(composite, BIN);
    int note = root;
    while (composite > 0) {
        if (composite & Notes[note]) {
            Serial.println(NoteNames[note]);
            composite ^= Notes[note];
      int start = 18 * (note - 1);
      for(int i=0; i<18; i++){
          leds[start+i] = CRGB(255, 255, 255); 
      }
        }
        note++;
    }

    FastLED.show();  
    long delay = (10 * 1000L) / Song2300.BPM;
    FastLED.delay(delay);

}
