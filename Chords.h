// Chords.h

#ifndef _CHORDS_h
#define _CHORDS_h
#include "Notes.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

typedef long chord;
// Creates a "chord" via bitmasking together the individual notes
chord enchord(int numnotes, ...);

// Decomposes a chord into its individual notes
Note * dischord(chord input);

const chord a_octave = enchord(2, a, A);
const chord b_fifths = enchord(3, b, b+7, b-7);
const chord e_octave = enchord(2, e, E);
const chord c_octave = enchord(2, c, C);
const chord e_minor = enchord(6, e, g, b, E, G, B);
//etc
#endif

