// 
// 
// 

#include "Song.h"


// Define the 24 notes from low c to high B plus "beat" which is no note / pause
int root = 1;
typedef enum {
	beat,
	c, cs, d, ds, e, f, fs, g, gs, a, as, b,
	C, Cs, D, Ds, E, F, Fs, G, Gs, A, As, B
} Note;

int Notes[] = { root << c, root << cs, root << d, root << ds, root << e, root << f, root << fs, root << g, root << gs, root << a, root << as, root << b,
				root << C, root << Cs, root << D, root << Ds, root << E, root << F, root << Fs, root << G, root << Gs, root << A, root << As, root << B };

/*/ F-F-F-FILTHY
int e_minor = Notes[e] & Notes[g] & Notes[b] & Notes[E] & Notes[G] & Notes[B];
int b_minor = Notes[b] & Notes[d] & Notes[fs] & Notes[B] & Notes[D] & Notes[Fs];*/

typedef int chord;
int enchord(int num, ...) {
	va_list arguments;
	chord output = 0;
	va_start(arguments, num);
	for (int x = 0; x < num; x++) {
		output &= Notes[va_arg(arguments, int)];
	}
	va_end(arguments);
	return output;
}

Note * dischord(chord input) {
	Note * outnotes = (Note *) malloc(sizeof(Note));
	for (uint8_t i = 1; i < 25; i++) {
		if (Notes[i] & input) {

		}
	}
}

chord e_minor = enchord(e, g, b, E, G, B);
chord b_minor = enchord(b, d, fs, B, D, Fs);



void SongClass::begin() {
	position = 0;
}

SongClass Song;

