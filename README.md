# hoop_controller
Code to run the hoop for the Shrine of the Singing Sun

## Design and rationale
The Arduino platform, as embedded system, has no real OS or filesystem. All data must be baked into the executable, or provided through some interface (eg a storage breakout board).

Thus, the simplest approach is to encode songs as big C++ objects in the program source. To achieve this, we use header files defining Song objects implementing a common `SongClass`.
Notes and Chords are encoded as a bitmask of the 24 possible notes, and decomposed during animation.

The LED animation code uses the provided Song objects to know where to apply its effects (eg, leading in a note, flashing the note, background coloring, etc)

## Code layout
The code is currently organized into 6 main cpp source files:

- `Notes.h`: defines the 24 notes of our 2 octaves as an `enum`, and then bitmasks for each note ranging from 2 to 2^25 in `Notes[]`. Note that there is no accompanying cpp file as the definitions are all in the header.
- `Chords.h`: creates a convenience/legibility type `Chord` which is just a `long`, and defines the chording functions `enchord` (create a chord from notes) and `chordmap` (apply a function to each note of a chord). Also predefines some chords for testing, eg `e_minor` - for release we should predefine all desired chords.
- `Chords.cpp`: accompanying source file for Chord.h
- `Song.h`: Defines the `SongClass` which each hour's song will implement. A `SongClass` has a `score` which is a big static array of `Notes`/`Chords`, a `BPM`, and an accessor function for getting the next score element.
- `Song.cpp`: accompanying source file for Song.h
- `hoop_controller.ino`: The main LED controller code which uses references to `SongClass` objects to display the desired notes.

The songs will be defined in the format `Song[hour].h`, as seen in `Song2300.h`.

## Goals & ongoing work
- The musical timing system needs better granularity. Note that there are still 7 bits left in the Chord definition, which we could use to encode some kind of timing information
- The note lead-in animation length could be decoupled from frame rate. Currently it plays faster or slower depending on how fast the LEDs are running. This could be changed to be dependent on a time value, allowing easier tweaking.
- An understanding of the 24 hours, advancing through the album of 24 songs, etc needs to be added
- Background / sun animation could be included underneath the notes, for a multi-layered approach to the display

## Simulations
We can simulate the LEDs in javascript, which is a much tighter turn-around time.
Try to code as close to the FastLED output as possible.

To run them, cd into ./simulations and run a server

```
> cd ./simulations
> python -m SimpleHTTPServer
```

then just open http://localhost:8000

The fire example is pretty awesome, and you can see how they translated it to code:

- https://github.com/SotSF/hoop_controller/blob/master/simulations/fire.js
to:
- https://github.com/FastLED/FastLED/blob/master/examples/Fire2012/Fire2012.ino


*Use four-spaces instead of tabs!*