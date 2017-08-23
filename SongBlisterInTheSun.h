// Here comes the sun

#include "Song.h"
#ifndef _SONGBlisterInTheSun_h
#define _SONGBlisterInTheSun_h
#if defined(ARDUINO) && ARDUINO >= 100
    #include "arduino.h"
#else
    #include "WProgram.h"
#endif

// these should all be eighth notes
static const Chord blisterInTheSunScore[] = {
Notes[Ds],rest,Notes[G],Notes[G],Notes[Gs],rest,Notes[G],Notes[Ds],
rest,Notes[G],rest,Notes[Ds],Notes[Gs],rest,Notes[G],rest,
Notes[Ds],rest,Notes[G],rest,Notes[Gs],rest,Notes[Fs],rest,
Notes[Ds],rest,rest,rest,rest,rest,Notes[Ds],rest,
Notes[Ds],rest,Notes[G],Notes[G],Notes[Gs],rest,Notes[G],Notes[Ds],
rest,Notes[G],rest,rest,Notes[Gs],rest,Notes[G],rest,
Notes[Ds],rest,Notes[G],rest,Notes[Gs],rest,Notes[G],Notes[Ds],
rest,rest,rest,rest,Notes[As],Notes[As],Notes[As],rest,
Notes[As],rest,Notes[G],rest,rest,rest,rest,rest,
rest,rest,rest,Notes[As],rest,rest,Notes[As],rest,
Notes[Ds],rest,Notes[Ds],rest,Notes[Ds],Notes[G],rest,Notes[C],
rest,rest,rest,rest,Notes[Ds],Notes[F],Notes[G],rest,
Notes[G],rest,Notes[Ds],rest,Notes[C],rest,rest,rest,
rest,rest,rest,rest,rest,Notes[Fs],rest,Notes[G],
rest,rest,Notes[F],Notes[Fs],rest,Notes[F],Notes[Ds],Notes[F],
rest,rest,rest,rest,rest,rest,rest,rest
};

SongClass SongBlisterInTheSun = SongClass(2*110, sizeof(blisterInTheSunScore)/sizeof(blisterInTheSunScore[0]), blisterInTheSunScore);
#endif
