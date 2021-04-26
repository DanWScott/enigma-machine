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

#define PIN      6
#define NUMPIXELS 27


Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

void setup() {
pixels.begin();

pixels.clear();
pixels.setBrightness(10);

for (int i = 0; i < NUMPIXELS; i++) {
  pixels.setPixelColor(i, pixels.Color(0, 255, 0));
}
pixels.show();
}

void loop() {


delay(DELAYVAL);
}
