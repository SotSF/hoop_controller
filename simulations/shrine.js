/**
 * shrine.js
 *
 * LEDstrip plugin
 *
 */


function Shrine (ledstrip, opts) {
    opts = opts || {};
    this.ledstrip = ledstrip;
    this.ledstrip.clear();
    this.direction = 1;
    this.color = opts.color || [255,0,0]; // default to red
    this.speed = opts.speed || 3; // run every Nth tick? 1 == full speed
    this.spread = opts.spread || 3; // spread N pixels on either side
    // tick counter
    this.t = 0;

    this.position = 0;

    this.NUM_LEDS = 100;
    return this;
}

Shrine.prototype.init = function() {return this;}

Shrine.prototype.setColor = function(color) { this.color = color; return this; }
Shrine.prototype.setSpeed = function(speed) { this.speed = speed; return this; }
Shrine.prototype.setSpread = function(spread) { this.spread = spread; return this; }
Shrine.prototype.setPosition = function(pos) { this.position = pos; return this; }

Shrine.prototype.setDirection = function(dir) {
    if (dir >= 0) {
        this.direction = 1;
    } else {
        this.direction = -1;
    }
    return this;
}

/*
The sun function creates a sun for a pixel, with a spread fading away.
We calculate the position of the sun from the hour
 */
Shrine.prototype.sun = function(hour, sun_color, tick) {
    if (!(tick % this.speed == 0)) return; // speed control

    var pos = Math.floor(hour * (this.NUM_LEDS / 12));
    // Set the center of the sun
    this.ledstrip.buffer[pos] = sun_color;    

    // number of pixels to spread the sun out on either side
    var spread_size = 10;
    // spread the sun center
    if (spread_size > 0) {

        for (i = 1; i <= spread_size; i++) {
            scol = [
                Math.floor(sun_color[0] * ((spread_size + 1 - i) / (spread_size + 1))),
                Math.floor(sun_color[1] * ((spread_size + 1 - i) / (spread_size + 1))),
                Math.floor(sun_color[2] * ((spread_size + 1 - i) / (spread_size + 1)))
            ];

            this.ledstrip.buffer[(pos + i) % this.NUM_LEDS] = scol;
            this.ledstrip.buffer[(pos + this.NUM_LEDS - i) % this.NUM_LEDS] = scol;
        }
    }
    this.ledstrip.send();

    return this;
}

Shrine.prototype.animate = function() {
    animation = requestAnimationFrame(this.animate.bind(this)); // preserve our context

    var hour = 3;
    var sun_color = [253, 184, 1];
    this.sun(hour, sun_color, this.t++);

    //this.scan(this.t++); // calculate waves and increment tick

    this.ledstrip.send(); // update strip
}
