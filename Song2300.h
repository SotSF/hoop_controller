// Song2300.h

#include "Song.h"
#ifndef _SONG2300_h
#define _SONG2300_h
#if defined(ARDUINO) && ARDUINO >= 100
    #include "arduino.h"
#else
    #include "WProgram.h"
#endif

static const uint8_t BPM = 120;
static const chord score[] =
{
    e_minor, Notes[e], Notes[a], Notes[b], rest,
    e_minor, Notes[e], Notes[b], Notes[b], rest,
    c_octave, Notes[c], Notes[C], c_octave, rest,
    Notes[c], Notes[cs], Notes[d], Notes[ds], rest,
    Notes[C], Notes[D], Notes[Ds], Notes[F], rest
};

SongClass Song2300 = SongClass(BPM, sizeof(score)/sizeof(score[0]), score);


#endif
