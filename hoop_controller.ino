#include "Chords.h"
#include "Notes.h"
#include "Song.h"
#include "Songs.h"
#include "FastLED.h"

FASTLED_USING_NAMESPACE

#define DATA_PIN    5
#define CLK_PIN     7
#define LED_TYPE    NEOPIXEL
#define COLOR_ORDER BGR
#define NUM_LEDS    432
#define BLOCKSIZE   NUM_LEDS/12
#define OFFSET(note) (BLOCKSIZE * (note - 1))
#define BRIGHTNESS  5
#define FPS         30

CRGB leds[NUM_LEDS];
SongClass currentSong = SongTest;
float lastSongSet = 0;

const char* NoteNames[] = { "rest","C ","Cs","D ","Ds","E ","F ","Fs","G ","Gs","A ","As","B " };
void setup() {
    delay(3000); // 3 second delay for recovery

    FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);

    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(5, 5, 25);
    }
}

Chord upcoming = 0;
Chord fadingout = 0;

// Begins a note's lead-in with two red dots at opposite ends
void beginnote(Note note) {
    Serial.println(NoteNames[note]);
    upcoming |= Notes[note];
    leds[OFFSET(note)] = CRGB(100,6,0);
    leds[OFFSET(note)+BLOCKSIZE-1] = CRGB(100,6,0);
}

// Repaints the background for a given note-block
void fillbg(Note note) {
    for (byte i = 0; i < BLOCKSIZE; i++) {
        leds[OFFSET(note) + i] = CRGB(5, 5, 25);
    }
}

// Animates the converging red dots before a note is to be played
void leadnote(Note note) {
    int start = OFFSET(note);
    bool finished = true;
    for (byte i = 0; i < BLOCKSIZE / 2; i++) {
        if (leds[start + i].r == 5) {
            // Found red pulse, advance it one LED, growing in brightness
            byte red = 100 + 20 * i < 255 ? (100 + 20 * i) : 255;
            leds[start + i] = CRGB(red, 20 * i, 0);
            leds[start + BLOCKSIZE - 1 - i] = CRGB(red, 20 * i, 0);
            finished = false;
            break;
        }
        else {
            leds[start + i] -= CRGB(20, 10, 0);
            leds[start + BLOCKSIZE - 1 - i] -= CRGB(20, 10, 0);
        }
    }
    // If dots have converged, play the white pulse, remove note from upcoming
    // and add to fading out
    if (finished) {
        for (byte i = 0; i < BLOCKSIZE; i++) {
            leds[start + i] = CRGB(155, 155, 155);
        }
        upcoming ^= Notes[note];
        fadingout |= Notes[note];
    }
}

void fadenote(Note note) {
    // Fade white burst
    int start = OFFSET(note);
    if (leds[start].r >= 25) {
        fadeToBlackBy(&leds[start], BLOCKSIZE, 50);
    } else {
        // Fade is finished, repaint the background and remove this note
        fillbg(note);
        fadingout ^= Notes[note];
    }
}

// Main function which is looped continuously
void loop()
{
    if (millis() - lastSongSet >= 60000) {
        if (currentSong.getPosition() == currentSong.getLength()){
          // TODO: set Next Song
          lastSongSet = millis();
        }
    }
    // Fetch a new note every beat, apply the beginnote function
    EVERY_N_MILLISECONDS(60000L / currentSong.BPM) {
        Chord composite = currentSong.next();
        Serial.print("Inbound composite: ");
        Serial.println(composite, BIN);
        chordmap(composite, beginnote);
    }
    //Take action on the notes which are leading in and fading out
    chordmap(upcoming, leadnote);
    chordmap(fadingout, fadenote);
    FastLED.show();
    FastLED.delay(1000/FPS);
}
