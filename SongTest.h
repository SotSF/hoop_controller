// SongTest.h

#include "Song.h"
#ifndef _SONGTEST_H
#define _SONGTEST_h
#if defined(ARDUINO) && ARDUINO >= 100
    #include "arduino.h"
#else
    #include "WProgram.h"
#endif

static const Chord testScore[] =
{
   Notes[C], Notes[Cs], Notes[D], Notes[Ds], Notes[E], Notes[F], 
   Notes[Fs], Notes[G], Notes[Gs], Notes[A], Notes[As], Notes[B]
};

SongClass SongTest = SongClass(120, testScore, testScore);


#endif

