#include "Song.h"
#ifndef _SONGTEST_H
#define _SONGTEST_h
#if defined(ARDUINO) && ARDUINO >= 100
    #include "arduino.h"
#else
    #include "WProgram.h"
#endif

static const Chord whenTheSaintsScore[] =
{
   rest, Notes[C], Notes[E], Notes[F],
   Notes[G], rest, rest, rest,
   rest, Notes[C], Notes[E], Notes[F],
   Notes[G], rest, rest, rest,
   rest, Notes[C], Notes[E], Notes[F],
   Notes[G],rest,Notes[E],rest,
   Notes[C],rest,Notes[E],rest,
   Notes[D],rest,rest,rest,
   rest,Notes[E],Notes[E],Notes[D],
   Notes[C],rest,rest,Notes[C],
   Notes[E],rest,Notes[G],rest,
   Notes[G],Notes[F],rest,rest,
   rest,Notes[F],Notes[E],Notes[F],
   Notes[G],rest,Notes[E],rest,
   Notes[C],rest,Notes[D],rest,
   Notes[C],rest,rest,rest
};

SongClass SongWhenTheSaints = SongClass(130, sizeof(whenTheSaintsScore)/sizeof(whenTheSaintsScore[0]), whenTheSaintsScore);


#endif

