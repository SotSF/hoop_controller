// Notes.h

#ifndef _NOTES_h
#define _NOTES_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "arduino.h"
#else
    #include "WProgram.h"
#endif

const long root = 1;
// Define the 12 notes from C to B plus "beat" which is no note / pause
const typedef enum {
    rest,
    C, Cs, D, Ds, E, F, Fs, G, Gs, A, As, B
} Note;

const long Notes[] = { rest,  
                       root << C, root << Cs, root << D, root << Ds, root << E, root << F, root << Fs, root << G, root << Gs, root << A, root << As, root << B };

#endif

