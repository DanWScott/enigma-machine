#include <Key.h>
#include <Keypad.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

const char ALPHABET[26] = "abcdefghijklmnopqrstuvwxyz"; //An array to establish the order of the alphabet within code.

#define RGB_LED_DIO 6 //Define the RGB LEDs' Digital pin.

const char LED_STECKER_ORDER[26] = "pyxcvbnmlkjhgfdsaqwertzuio";

const byte ROWS = 2;
const byte COLS = 13;

char KEYS[ROWS][COLS] = {
  {'q', 'w', 'e', 'r', 't', 'z', 'u', 'i', 'o', 'k', 'j', 'h', 'g'},
  {'a', 's', 'd', 'f', 'p', 'y', 'x', 'c', 'v', 'l', 'm', 'n', 'b'}
};

byte colPins[COLS] = {50, 51, 52, 53, 44, 45, 46, 47, 42, 39, 41, 40, 43};
byte rowPins[ROWS] = {48, 49};

Keypad steckerbrettInput = Keypad(makeKeymap(KEYS), rowPins, colPins, ROWS, COLS);

const int RGB_LED_COUNT = 26; //There are 26 RGB LEDs in the LED array.
Adafruit_NeoPixel ledArray(RGB_LED_COUNT, RGB_LED_DIO, NEO_GRB + NEO_KHZ800); //Establishes an array of 26 RGB LEDs.
bool steckerColoursUsed[10]; //An array to store which colours have and have not been used to indicate steckered pairs.
int coloursByStecker[26]; //An array that stores which colour each plug's LED is, so that when they are unsteckered the colour may be identified and reused.
int rgbLedValues[26][3]; //A 2D array to store the values of R, G and B for each of the 26 RGB LEDs.

//RGB Codes for Colours
/*const int RED[3] = {0, 255, 0}; //Basic Red.
const int LIME[3] = {255, 0, 0}; //Lime Green.
const int BLUE[3] = {0, 0, 255}; //Basic Blue.
const int ORANGE[3];
const int MUSTARD[3] = {204, 255, 18}; //Mustard yellow.
const int MAGENTA[3] = {0, 204, 204}; //Magenta/purple.
const int PINK[3];
const int TURQUOISE[3] = {245, 0, 255}; //Turquoise blue.
const int VIOLET[3];
const int WHITE[3] = {255, 255, 255}; //Basic White.*/ 

//An array that stores the RGB values of 10 different colours that can be used to represent steckered pairs.
const int RGB_CODES[10][3] = {{0, 255, 0}/*RED*/, {255, 0, 0}/*LIME*/, {0, 0, 255}/*BLUE*/, {0, 0, 0} /*ORANGE*/, {204, 255, 18} /*MUSTARD*/, {0, 204, 204} /*MAGENTA*/, {0, 0, 0} /*PINK*/, {245, 0, 255} /*TURQUOISE*/, {255, 255, 255} /*WHITE*/}; 

int steckerbrettArray[26]; //An array to establish the substitution pattern of the plugboard.

int oldLedLightup = -1;
int newLedLightup = -1;

bool usedColours[10];

void setup() {
  for (int i = 0; i < 26; i++) steckerbrettArray[i] = i;
  for (int i = 0; i < 10; i++) usedColours[i] = false;
}

void loop() {
  // put your main code here, to run repeatedly:

  char steckerPress = steckerbrettInput.getKey();
  if (steckerPress != NO_KEY)
  {




    
  }
  
}
