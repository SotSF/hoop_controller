/**
 * wheel_smooth.js
 *
 * LEDstrip plugin
 *
 * This just has a bunch of segments rotating clockwise, split into twelve
 *
 * Distributed under the MIT License
 */

function Wheel(ledstrip, opts) {
    opts = opts || {};
    this.ledstrip = ledstrip;
    this.ledstrip.clear();
 
    this.NUM_LEDS = this.ledstrip.size();
    this.NUM_CHUNKS = 12;
    this.CHUNK_SIZE = this.ledstrip.size() / this.NUM_CHUNKS;

    this.FRAMES_PER_SECOND = 1;

    this.MIN_COLOR = [0, 000, 000];
    this.MAX_COLOR = [0, 250, 250];

    this.wheel = new Array(this.NUM_LEDS);

    this.t = 0;                    // tick counter

    return this;
}

// given and i in the chunk size, gives a color
// that corresponds to the intensity
Wheel.prototype.scaleColor = function(i) {
  var r = (this.MAX_COLOR[0] - this.MIN_COLOR[0]) * (i / this.CHUNK_SIZE) + this.MIN_COLOR[0];
  var g = (this.MAX_COLOR[1] - this.MIN_COLOR[1]) * (i / this.CHUNK_SIZE) + this.MIN_COLOR[1];
  var b = (this.MAX_COLOR[2] - this.MIN_COLOR[2]) * (i / this.CHUNK_SIZE) + this.MIN_COLOR[2];
  //console.log("in: ", i, " rbg: ", Math.round(r), Math.round(g), Math.round(b));
  return [Math.round(r), Math.round(g), Math.round(b)];
}

Wheel.prototype.init = function() {
    // set sectors alternatively bright and dim
    for (var i = 0; i <= this.NUM_CHUNKS; i++) {
        for (var j = 0; j < this.CHUNK_SIZE; j++) {
          //console.log("foo: ", i, ", pos: ", i*j,  this.wheel[i*j]);
          this.ledstrip.buffer[i*this.CHUNK_SIZE + j] = this.scaleColor(j);
          this.wheel[i*this.CHUNK_SIZE + j] = this.scaleColor(j);
        }
    }

    return this;
}

Wheel.prototype.Run = function() {
    
    for( var i = this.NUM_LEDS; i >=0;  i--) {
      var pos = ((i-1) + this.NUM_LEDS) % this.NUM_LEDS;
     
      this.ledstrip.buffer[i] = this.wheel[pos];
      this.wheel[i] = this.wheel[pos];
    }
}

Wheel.prototype.animate = function() {
    animation = requestAnimationFrame(this.animate.bind(this));     // preserve our context

    this.Run();

    this.ledstrip.send(); // display the LED state

    this.t++; // increment tick
}
