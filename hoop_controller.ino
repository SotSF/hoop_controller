#include "Chords.h"
#include "Notes.h"
#include "Song.h"
#include "SongHereComesTheSun.h"
#include "SongZeldaSun.h"
#include "SongBlisterInTheSun.h"
#include "SongWhenTheSaints.h"
#include "FastLED.h"
#include "TimeLib.h"

FASTLED_USING_NAMESPACE

#define DATA_PIN    11
#define CLK_PIN     13
#define LED_TYPE    APA102
#define COLOR_ORDER BGR
#define NUM_LEDS    216
#define BLOCKSIZE   NUM_LEDS/12
#define OFFSET(note) (BLOCKSIZE * (note - 1))
#define BRIGHTNESS  50
#define FPS         30


#define DIRTY_TWEEK_OFFSET    -4

CRGB leds[NUM_LEDS];
SongClass currentSong = SongHereComesTheSun;
float lastSongSet = 0;
CRGB background_light_color = CRGB(0, 0, 0);
CRGB note_color = CRGB(255, 0, 0);
time_t previous_minute = minute(now());
void setup() {
    delay(3000); // 3 second delay for recovery
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLK_PIN, COLOR_ORDER,DATA_RATE_MHZ(10)>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = background_light_color;
    }
}



Chord upcoming = 0;
Chord fadingout = 0;

// Begins a note's lead-in with two red dots at opposite ends
void beginnote(Note note) {
  Serial.println(note);
    upcoming |= Notes[note];
  Serial.println(upcoming);
    leds[OFFSET(note)+ DIRTY_TWEEK_OFFSET] = CRGB(250,6,0);
    leds[OFFSET(note)+BLOCKSIZE-1+ DIRTY_TWEEK_OFFSET] = CRGB(250,6,0);
}

// Repaints the background for a given note-block
void fillbg(Note note) {
    for (byte i = 0; i < BLOCKSIZE; i++) {
        leds[OFFSET(note) + i + DIRTY_TWEEK_OFFSET] = background_light_color;
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
            leds[start + i+DIRTY_TWEEK_OFFSET] = note_color;
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

void determineSongToPlay(){
  time_t current_minute = minute(now());  
  if ( current_minute > previous_minute+3 ) {
    switch(random(0,4)) {
      case 0:
        currentSong = SongWhenTheSaints;
        note_color = CRGB(255, 100, 180);
        break;
      case 1:
        currentSong = SongHereComesTheSun;
        note_color = CRGB(255, 0, 0);
        break;
      case 2:
        currentSong = SongZeldaSun;
        note_color = CRGB(40, 255, 0);
        break;      
      case 3:
        currentSong = SongBlisterInTheSun;
        note_color = CRGB(255, 255, 0);
    }
    previous_minute = current_minute;
  }
}

// Main function which is looped continuously
void loop() {
  determineSongToPlay();        

    chordmap(upcoming, (void (*)(int)) leadnote);
    // Fetch a new note every beat, apply the beginnote function
    EVERY_N_MILLISECONDS(60000L / currentSong.BPM) {
        Chord composite = currentSong.next();
        Serial.print("Inbound composite: ");
        Serial.println(composite, BIN);
        chordmap(composite, (void (*)(int)) beginnote);
    }
    //Take action on the notes which are leading in and fading out

    chordmap(fadingout, (void (*)(int)) fadenote);
    FastLED.show();
    FastLED.delay(1000/FPS);
}
