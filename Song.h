// Song.h

#ifndef _SONG_h
#define _SONG_h
#include "Notes.h"
#include "Chords.h"
#if defined(ARDUINO) && ARDUINO >= 100
    #include "arduino.h"
#else
    #include "WProgram.h"
#endif
class SongClass {

    uint16_t position;
    uint16_t length;

public:
    uint8_t BPM;
    const Chord * score;
    Chord next();
    SongClass(const uint8_t BPM, const uint16_t length, const Chord* song);
};

extern SongClass Song;

#endif

