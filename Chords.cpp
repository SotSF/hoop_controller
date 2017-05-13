// 
// 
// 

#include "Chords.h"
#include <stdarg.h>

/* The boring, array based versaion.
chord enchord(size_t len, int * notes) {
	chord output = 0;
	for (int i = 0; i < len; i++) {
		output |= Notes[notes[i]];
	}
	return output;
} */

// The exciting and dangerous variadic version!
chord enchord(int numnotes, ...) {
	chord outchord = 0;
	va_list args;
	va_start(args, numnotes);
	for (int i = 1; i <= numnotes; i++) {
		int val = va_arg(args, int);
		outchord |= Notes[val];
	}
	va_end(args);
	return outchord;
}

Note * dischord(chord input) {
	Note * outnotes = (Note *)malloc(sizeof(Note));
	for (uint8_t i = 1; i < 25; i++) {
		if (Notes[i] & input) {

		}
	}
}