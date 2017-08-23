// Zelda's Sun Song

#include "Song.h"
#ifndef _SONGZeldaSun_h
#define _SONGZeldaSun_h
#if defined(ARDUINO) && ARDUINO >= 100
    #include "arduino.h"
#else
    #include "WProgram.h"
#endif

static const Chord zeldaSunScore[] = {
Notes[E],rest,Notes[C],rest,Notes[A],rest,rest,rest,rest,rest,
Notes[E],rest,Notes[C],rest,Notes[A],rest,rest,rest,rest,rest,
Notes[E],Notes[G],Notes[A],Notes[B],Notes[C],Notes[D],rest,rest
};

SongClass SongZeldaSun = SongClass(250, sizeof(zeldaSunScore)/sizeof(zeldaSunScore[0]), zeldaSunScore);

#endif
