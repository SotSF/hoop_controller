// 
// 
// 

#include "Chords.h"
#include <stdarg.h>

/* The boring, array based version.
chord enchord(size_t len, int * notes) {
    chord output = 0;
    for (int i = 0; i < len; i++) {
        output |= Notes[notes[i]];
    }
    return output;
} */

// Compose a chord out of notes, the exciting and dangerous variadic version!
Chord enchord(byte numnotes, ...) {
    Chord outchord = 0;
    va_list args;
    va_start(args, numnotes);
    for (byte i = 1; i <= numnotes; i++) {
        int val = va_arg(args, int);
        outchord |= Notes[val];
    }
    va_end(args);
    return outchord;
}

// Apply a function to each note of a chord
void chordmap(Chord input, void (*func)(int note)) {
    byte i = 1;
    long comparison = input;
    while (comparison > 0) {
        if (Notes[i] & comparison) {
            comparison ^= Notes[i];
            func(i);
        }
        i++;
    }
}