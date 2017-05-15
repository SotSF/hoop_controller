# hoop_controller
Code to run the hoop for the Shrine of the Singing Sun

*Use four-spaces instead of tabs!*

## Simulations
We can simulate the LEDs in javascript, which is a much tighter turn-around time.
Try to code as close to the FastLED output as possible.

To run them, cd into ./simulations and run a server

> cd ./simulations
> python -m SimpleHTTPServer

then just open http://localhost:8000

The fire example is pretty awesome, and you can see how they translated it to code:

- https://github.com/SotSF/hoop_controller/blob/master/simulations/fire.js
to:
- https://github.com/FastLED/FastLED/blob/master/examples/Fire2012/Fire2012.ino
