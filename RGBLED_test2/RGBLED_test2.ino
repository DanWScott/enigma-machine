/*
NeoPixel LEDs

modified on 7 May 2019
by Saeed Hosseini @ Electropeak
**This code is based on Adafruit NeoPixel library Example**
https://electropeak.com/learn/

*/

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define PIN      4
#define NUMPIXELS 26


Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

const int RGB_CODES[10][3] = {{0, 255, 0}/*RED*/, {255, 0, 0}/*LIME*/, {0, 0, 255}/*BLUE*/, {135, 235, 41} /*ORANGE*/, {204, 255, 18} /*MUSTARD*/, {0, 204, 204} /*MAGENTA*/, {150, 230, 230} /*PINK*/, {245, 0, 255} /*TURQUOISE*/, {255, 255, 255} /*WHITE*/}; 


void setup() {
pixels.begin();

pixels.clear();
pixels.setBrightness(80);


pixels.setPixelColor(0, pixels.Color(0, 255, 0));
pixels.setPixelColor(1, pixels.Color(255, 0, 0));
pixels.setPixelColor(2, pixels.Color(0, 0, 255));
pixels.setPixelColor(3, pixels.Color(120, 250, 20));
pixels.setPixelColor(4, pixels.Color(255, 255, 0));
pixels.setPixelColor(5, pixels.Color(0, 204, 204));
pixels.setPixelColor(6, pixels.Color(150, 255, 230));
pixels.setPixelColor(7, pixels.Color(245, 0, 255));
pixels.setPixelColor(8, pixels.Color(255, 255, 255));
pixels.setPixelColor(9, pixels.Color(160, 0, 55));

pixels.show();
}

void loop() {


delay(DELAYVAL);
}
