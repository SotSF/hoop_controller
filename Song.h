// Song.h

#ifndef _SONG_h
#define _SONG_h
#include <stdarg.h>
#include <stdio.h>
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif



class SongClass
{
 protected:
	 uint16_t position = 0;

 public:
	uint8_t BPM;
	uint16_t length;
	int b_minor;
	int e_minor;
	void begin();
};

extern SongClass Song;

#endif

