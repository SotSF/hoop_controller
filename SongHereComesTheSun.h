// Here comes the sun

#include "Song.h"
#ifndef _SONGHereComesTheSun_h
#define _SONGHereComesTheSun_h
#if defined(ARDUINO) && ARDUINO >= 100
    #include "arduino.h"
#else
    #include "WProgram.h"
#endif

static const Chord hereComesTheSunScore[] = {
Notes[D],rest,Notes[As],rest,Notes[G],Notes[A],Notes[As],rest,Notes[G],
rest,rest,Notes[As],Notes[G],Notes[A],Notes[As],rest,rest,
Notes[Ds],rest,Notes[As],rest,Notes[As],rest,Notes[G],
rest,Notes[G],Notes[A],rest,Notes[G],rest,rest,
rest,rest,Notes[As],rest,Notes[G],rest,Notes[D],
Notes[Ds],Notes[G],Notes[A],Notes[D],Notes[G],Notes[A],Notes[C],Notes[G],
Notes[A],Notes[D],Notes[G],Notes[A],Notes[G],Notes[F],Notes[Ds],Notes[F]
};

SongClass SongHereComesTheSun = SongClass(120, sizeof(hereComesTheSunScore)/sizeof(hereComesTheSunScore[0]), hereComesTheSunScore);

#endif
