// Chords.h

#ifndef _CHORDS_h
#define _CHORDS_h
#include "Notes.h"

#if defined(ARDUINO) && ARDUINO >= 100
    #include "arduino.h"
#else
    #include "WProgram.h"
#endif

typedef long Chord;
// Creates a "chord" via bitmasking together the individual notes
Chord enchord(byte numnotes, ...);

// Applies a function to each note of a chord
void chordmap(Chord input, void (*func)(int note));

/*const Chord a_octave = enchord(2, a, A);
const Chord b_fifths = enchord(3, b, b+7, b-7);
const Chord e_octave = enchord(2, e, E);
const Chord c_octave = enchord(2, c, C);
const Chord e_minor = enchord(6, e, g, b, E, G, B);
*/
//etc
#endif

