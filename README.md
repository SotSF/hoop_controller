# hoop_controller
Code to run the hoop for the Shrine of the Singing Sun

## Design & rationale
The Arduino platform, as a lightweight embedded system, has no real OS or filesystem. All data must be baked into the executable, or provided through some interface (eg a storage breakout board - see 'The storage problem' below).

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
- An understanding of the 24 hours, advancing through the album of 24 songs, etc needs to be added. This may require a real-time clock module, see eg http://www.makeuseof.com/tag/how-and-why-to-add-a-real-time-clock-to-arduino/
- Background / sun animation could be included underneath the notes, for a multi-layered approach to the display
- We may not have enough memory to store all our songs on the current platform. See "The storage problem" below.

## The storage problem
We may not have enough memory (SRAM) on a Genuine™ Arduino™ platform™ to store all our songs. This is a multifaceted issue with several parts and potential approaches.
#### Constraints and problem statement
- An Arduino Micro or Mini only has 2k main memory (SRAM, where the stack/heap live), and ~28k program memory (flash, where the program instructions live).
- A song is composed of notes & chords. Given that we have to address an effectively arbitrary combination of 24 notes, a single 32-bit value is about as small as we can go for a single score-element. That's 4 bytes per element.
- If a song is 100 BPM and 1 minute long, and only has on-beat notes (8th notes?), that's 100 score-elements, or 400 bytes minimum.
- If we have 12 songs, that's in the neighborhood of 4800 bytes, which is larger than the small form-factor Arduinos' main memory (2k), not even counting all the actual memory we need for variables (like the LED array).
#### Potential ameliorations
- Figure out the `PROGMEM` storage approach. Arduino [has a way](https://www.arduino.cc/en/Reference/PROGMEM) to store data in the controller's Flash, which is usually used for the program code. However, making it actually work in the context of the existing code is, how you say... complex. I've done some testing and managed to make it... sort of work? in certain restricted cases? But haven't validated it for our actual use case.
- Include a storage breakout board with a microSD card. This gives us all the storage we could want, but will increase the complexity of the hardware (more soldering) as well as the physical size. Will also require code changes to read off the SD card, and possibly song notation changes.
- Use a beefier microcontroller. The [teensy](https://www.pjrc.com/teensy/techspecs.html) family is Arduino-compatible (ie you can use all the same libraries), the same form factor as the Arduino Micro/Mini, costs ~$20 for a board, and has much more RAM and Flash. This would allow use to use just the controller and the current code, but has some minor complexities for development (it uses a plugin for the Arduino IDE, may not play nice with Visual Studio, etc.)
- Have a "docking" approach where a larger controller (even up to a whole Raspberry Pi?) is mounted on the shrine and the hoop interfaces with it once in place. This approach could take multiple forms, eg
  - Have the hoop unlit while being used in the ritual. Once it's slotted in place, it turns on, yay! 
  - Have the hoop be a fire hoop as well as an LED hoop. It can be on fire for the ritual then plug in once it's placed. Raina has demonstrated this approach with quick wicks on her smarthoop
  - Have the hoop be a "dumb strip" during the ritual and then wise up once placed. The Neopixel (WS2812b) strip we're using right now will latch the last value for the LEDs even without a controller attached as long as there is power, so we could set the hoop to eg orange, white, etc and then disconnect it from the controller for the ritual. Then once it's plugged in it would revert to the song-playing program.

Of these, the `PROGMEM` approach requires the least money/hardware in that it could theoretically be done with the existing hardware only, as long as our songs aren't too long. We currently use in the neighborhood of 10k, out of 28k available Flash. So we could spend an average of 1500 bytes per song, for 12 songs. However, the software engineering side is a substantial pain. I think just getting an appropriately sized Teensy is a very reasonable approach, but am open to ideas.

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