/**
 * fire.js
 *
 * Fire by Mark Kriegsman, originally for the FastLED library
 *
 * see: https://github.com/FastLED/FastLED/blob/master/examples/Fire2012/Fire2012.ino
 *
 * LEDstrip plugin
 *
 * Copyright (c) 2013 Dougal Campbell
 *
 * Distributed under the MIT License
 */

function Fire (ledstrip, opts) {
    opts = opts || {};
    this.ledstrip = ledstrip;
    this.ledstrip.clear();

    this.NUM_LEDS = this.ledstrip.size();
    this.NUM_CHUNKS = 12;
    this.CHUNK_SIZE = this.ledstrip.size() / this.NUM_CHUNKS;

    this.FRAMES_PER_SECOND = 5;

    this.COOLING = 70; // descrease to make white-hot
    this.SPARKING = 210;

    this.heat = new Array(this.NUM_LEDS);

    this.t = 0;                                        // tick counter

    return this;
}

Fire.prototype.init = function() {
    for (var i = 0, l = this.heat.length; i < l; i++) {
        this.heat[i] = 0;
    }

    return this;
}

// Get a timestamp for ms milliseconds from now
Fire.prototype.addTime = function (ms) {
    return (new Date()).valueOf() + ms;
}

// Replicate random8() function
Fire.prototype.random8 = function(min, max) {
    if (min === undefined) {
        min = 0;
        max = 255;
    }
    if (max === undefined) {
        max = min;
        min = 0;
    }
    return (Math.round(Math.random() * (max - min)) + min) & 255;
}

// Replicate random16() function
Fire.prototype.random16 = function(min, max) {
    if (min === undefined) {
        min = 0;
        max = 65535;
    }
    if (max === undefined) {
        max = min;
        min = 0;
    }
    return (Math.round(Math.random() * (max - min)) + min) & 65535;
}

Fire.prototype.qadd8 = function(a, b) {
    var tmp = Math.round(a + b);
    if (tmp > 255) tmp = 255;

    return tmp;
}

Fire.prototype.qsub8 = function(a, b) {
    var tmp = Math.round(a - b);
    if (tmp < 0) tmp = 0;

    return tmp;
}

Fire.prototype.scale8_video = function(val, min, max) {
    if (max === undefined) {
        max = min;
        min = 0;
    }

    return Math.floor(val * (max - min) / 255 + min);
}


Fire.prototype.millis = function() {
    return (new Date()).valueOf();
}

Fire.prototype.SF = function(centroid, i) {
  return (centroid + i) % this.NUM_LEDS 
}

Fire.prototype.RoundFire = function(centroid, num_leds) {
    
    // cool down whole LED strand
    // for each chunk, get centroid
    //   heat drift forwards & backwards
    //   ignite sparks forwards & backwards
    // map whole 

    // Step 1.    Cool down every cell a little
    for( var i = 0; i < this.NUM_LEDS; i++) {
        this.heat[i] = this.qsub8( this.heat[i],    this.random8(0, Math.floor((this.COOLING * 10) / this.CHUNK_SIZE) + 1));
    }

    for (var j = 0; j < this.NUM_CHUNKS; j++) {
      var half_chunk = Math.floor(this.CHUNK_SIZE / 2);
      var cent = (j*2*half_chunk) + half_chunk; // shouldn't need Math.Floor

      // Step 2.    Heat from each cell drifts 'up' and diffuses a little
      for( var k = half_chunk + Math.floor(half_chunk / 3) - 3; k > 0; k--) {
          this.heat[this.SF(cent, k)] = Math.floor((this.heat[this.SF(cent, k - 1)] + this.heat[this.SF(cent, k - 2)] + this.heat[this.SF(cent, k - 3)] ) / 3) || 0;
          this.heat[this.SF(cent, -1*k)] = Math.floor((this.heat[this.SF(cent, -1*(k - 1))] + this.heat[this.SF(cent, -1*(k - 2))] + this.heat[this.SF(cent, -1*(k - 3))] ) / 3) || 0;
      }

      // Step 3.    Randomly ignite new 'sparks' of heat near the bottom
      if( this.random8(j) < this.SPARKING ) {
          var y = this.random8(7 + j);
          this.heat[this.SF(cent, y)] = this.qadd8( this.heat[this.SF(cent, y)], this.random8(100,170) );
          var z = this.random8(8 + j);
          this.heat[this.SF(cent, z)] = this.qadd8( this.heat[this.SF(cent, z)], this.random8(100,170) );
      }
    }

    // Step 4.    Map from heat cells to LED colors
    for( var j = 0; j < this.NUM_LEDS; j++) {
            this.ledstrip.buffer[j] = this.HeatColor( this.heat[j]);
    }
}

Fire.prototype.Fire2012 = function() {
    // Step 1.    Cool down every cell a little
        for( var i = 0; i < this.NUM_LEDS; i++) {
            this.heat[i] = this.qsub8( this.heat[i],    this.random8(0, Math.floor((this.COOLING * 10) / this.NUM_LEDS) + 2));
        }

        // Step 2.    Heat from each cell drifts 'up' and diffuses a little
        for( var k = this.NUM_LEDS - 3; k > 0; k--) {
            this.heat[k] = Math.floor((this.heat[k - 1] + this.heat[k - 2] + this.heat[k - 2] ) / 3) || 0;
        }

        // Step 3.    Randomly ignite new 'sparks' of heat near the bottom
        if( this.random8() < this.SPARKING ) {
            var y = this.random8(7);
            this.heat[y] = this.qadd8( this.heat[y], this.random8(160,255) );
        }

        // Step 4.    Map from heat cells to LED colors
        for( var j = 0; j < this.NUM_LEDS; j++) {
                this.ledstrip.buffer[j] = this.HeatColor( this.heat[j]);
        }
}

//Play with this for different strip colors
Fire.prototype.HeatColor = function(temperature) {
    var heatcolor = [0,0,0];
    if (temperature === undefined) {
        temperature = 0;
    }

    // Scale 'heat' down from 0-255 to 0-191,
    // which can then be easily divided into three
    // equal 'thirds' of 64 units each.
    var t192 = this.scale8_video( temperature, 192);

    // calculate a value that ramps up from
    // zero to 255 in each 'third' of the scale.
    var heatramp = t192 & 0x3F; // 0..63
    heatramp <<= 2; // scale up to 0..252

    // now figure out which third of the spectrum we're in:
    if( t192 & 0x80) {
        // we're in the hottest third
        heatcolor[0] = 255; // full red
        heatcolor[1] = 255; // full green
        heatcolor[2] = heatramp; // ramp up blue

    } else if( t192 & 0x40 ) {
        // we're in the middle third
        heatcolor[0] = 255; // full red
        heatcolor[1] = heatramp; // ramp up green
        heatcolor[2] = 0; // no blue

    } else {
        // we're in the coolest third
        heatcolor[0] = heatramp; // ramp up red
        heatcolor[1] = 0; // no green
        heatcolor[2] = 0; // no blue
    }

    return heatcolor;
}


Fire.prototype.animate = function() {
    animation = requestAnimationFrame(this.animate.bind(this));     // preserve our context

    this.RoundFire();

    this.ledstrip.send(); // display the LED state

    this.t++; // increment tick
}
