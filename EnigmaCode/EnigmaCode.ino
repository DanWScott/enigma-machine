//DANIEL SCOTT
//ENIGMA MACHINE 2021
//AS91900/AS91907

//Fix all of the pin definitions once I figure out where they all go -----------------------------------------------------------------------------------

//INITIALISATIONS

//Libraries
#include <Wire.h> //Needed to include Wire.h to send signals down CLK pins.
#include <LiquidCrystal_I2C.h> //Included LiquidCrystal_I2C.h in order to run the LCD screens.
#include <TM1637Display.h> //Included TM1637Display.h to run the 4-digit 7-segment display with an HW-069 backpack.
#include <Adafruit_NeoPixel.h> //Included Adafruit_Neopixel.h to run the addressable RGB LEDs.
#ifdef __AVR__
#include <avr/power.h> 
#endif
#include <Key.h> //Included Key.h and Keypad.h to run the keypad matrices.
#include <Keypad.h>

//Setup Keyboards
const byte ROWS = 2;
const byte COLS = 13; //Stores the number of rows and columns in the keyboard keypad matrices being established.

char KEYS[ROWS][COLS] = {
  {'q', 'w', 'e', 'r', 't', 'z', 'u', 'i', 'o', 'k', 'j', 'h', 'g'},
  {'a', 's', 'd', 'f', 'p', 'y', 'x', 'c', 'v', 'l', 'm', 'n', 'b'}
}; //Defines what each of the keys in the keyboard keypad matrices corresponds to.

byte kbColPins[COLS] = {50, 51, 52, 53, 44, 45, 46, 47, 42, 39, 41, 40, 43};
byte kbRowPins[ROWS] = {48, 49}; //Defines the pins of the keyboard matrix.

byte sbColPins[COLS] = {50, 51, 52, 53, 44, 45, 46, 47, 42, 39, 41, 40, 43};
byte sbRowPins[ROWS] = {48, 49}; //Defines the pins of the steckerbrett matrix.

Keypad keyboard = Keypad(makeKeymap(KEYS), kbRowPins, kbColPins, ROWS, COLS); //Sets up the keyboard matrix.
Keypad steckerbrettKb = Keypad(makeKeymap(KEYS), sbRowPins, sbColPins, ROWS, COLS); //Sets up the steckerbrett matrix.

//Setup Setting Buttons
const byte DIMENSION = 3;

char SETTING_BUTTONS[DIMENSION][DIMENSION] = {
  {0, 1, 2},
  {3, 4, 5},
  {6, 7, 8}
}; //Creates a number which correllates to each of my nine buttons used to change settings.

byte settingColPins[DIMENSION] = {11, 12, 13};
byte settingRowPins[DIMENSION] = {14, 15, 16}; //Defines the pins of the setting button matrix.

Keypad settingButtons = Keypad(makeKeymap(SETTING_BUTTONS), settingRowPins, settingColPins, DIMENSION, DIMENSION); //Sets up the matrix which runs the setting buttons.

//Define Connection Pins
#define CLK 23 //Define the 7-segment's Clock pin
#define DIO 22 //Define the 7-segment's Digital Input/Output pin
#define RGB_LED_DIO 7 //Define the RGB LEDs' Digital pin.

//LCD Setup
LiquidCrystal_I2C walzenLCD(0x3F, 16, 2); //Establishes an LCD Screen/I2C module with the address 0x3E (A0 connected)
LiquidCrystal_I2C outputLCD(0x3E, 16, 2); //Establishes an LCD Screen/I2C module with the address 0x3F (No solder pad connections)

//7-segment Setup
TM1637Display display = TM1637Display(CLK, DIO); //Established a TM1637 1x4 7-segment display with HW-069 backpack.

//7-segment display readings
const uint8_t DAN[] {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G, //d
  SEG_A | SEG_B | SEG_C | SEG_G | SEG_E | SEG_F, //A
  SEG_C | SEG_E | SEG_G, //n
  0x00
}; //Prints the word "dAn" on the 7-segment.

const uint8_t DTE[] {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G, //d
  SEG_D | SEG_E | SEG_F | SEG_G, //t
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G, //E
  0x00
}; //Prints the word "dtE" on the 7-segment.

//Constants
const char ALPHABET[26] = "abcdefghijklmnopqrstuvwxyz"; //An array to establish the order of the alphabet within code.

//RGB LED Setup
const int RGB_LED_COUNT = 26; //There are 26 RGB LEDs in the LED array.
const int LED_ORDER[26] = {16, 5, 3, 14, 19, 13, 12, 11, 24, 10, 9, 8, 7, 6, 25, 0, 17, 20, 15, 21, 23, 4, 18, 2, 1, 22};
Adafruit_NeoPixel ledArray(RGB_LED_COUNT, RGB_LED_DIO, NEO_GRB + NEO_KHZ400); //Establishes an array of 26 RGB LEDs.
bool steckerColoursUsed[10]; //An array to store which colours have and have not been used to indicate steckered pairs.
int coloursByStecker[26]; //An array that stores which colour each plug's LED is, so that when they are unsteckered the colour may be identified and reused.
int rgbLedValues[26][3]; //A 2D array to store the values of R, G and B for each of the 26 RGB LEDs.
//An array that stores the RGB values of 10 different colours that can be used to represent steckered pairs.
const int RGB_CODES[10][3] = {{0, 255, 0}/*RED*/, {255, 0, 0}/*LIME*/, {0, 0, 255}/*BLUE*/, {120, 250, 50} /*ORANGE*/, {255, 255, 0} /*MUSTARD*/, {0, 204, 204} /*MAGENTA*/, {150, 255, 230} /*PINK*/, {245, 0, 255} /*TURQUOISE*/, {255, 255, 255} /*WHITE*/, {160, 0, 55} /*MINT GREEN*/};

//Substitution Arrays
int steckerbrettArray[26]; //An array to establish the substitution pattern of the plugboard.
int umkehrwalzeArray[26] = {24, 17, 20, 7, 16, 18, 11, 3, 15, 23, 13, 6, 14, 10, 12, 8, 4, 1, 5, 25, 2, 22, 21, 9, 0, 19}; //Establishes the substitution pattern of the reflector board.
int walzeIArray[26] = {4, 10, 12, 5, 11, 6, 3, 16, 21, 25, 13, 19, 14, 22, 24, 7, 23, 20, 18, 15, 0, 8, 1, 17, 2, 9}; //Arrays to establish the single-way substitution of the 5 rotors.
int walzeIIArray[26] = {0, 9, 3, 10, 18, 8, 17, 20, 23, 1, 11, 7, 22, 19, 12, 2, 16, 6, 25, 13, 15, 24, 5, 21, 14, 4};
int walzeIIIArray[26] = {1, 3, 5, 7, 9, 11, 2, 15, 17, 19, 23, 21, 25, 13, 24, 4, 8, 22, 6, 0, 10, 12, 20, 18, 16, 14};
int walzeIVArray[26] = {4, 18, 14, 21, 15, 25, 9, 0, 24, 16, 20, 8, 17, 7, 23, 11, 13, 5, 19, 6, 10, 3, 2, 12, 22, 1};
int walzeVArray[26] = {21, 25, 1, 17, 6, 8, 19, 24, 20, 15, 18, 3, 13, 7, 11, 23, 0, 22, 12, 9, 16, 14, 5, 4, 2, 10};

//Walze Rotation
int walzenSelected[3] = {0, 1, 2}; //An array to store the currently selected rotors in each of the three positions.
const char walzenRotationLetters[] = "rfwka"; //An array to store which letter causes the subsequent rotor to rotate for each of the five rotors. 

//Walze Orientation
char walze1Orientation[28] = "abcdefghijklmnopqrstuvwxyz.."; //Arrays to show the current orientations of the three rotors.
char walze2Orientation[28] = "abcdefghijklmnopqrstuvwxyz..";
char walze3Orientation[28] = "abcdefghijklmnopqrstuvwxyz..";

//Variables
char message[20]; //A character array to store the latest output.
int encryptedLetter; //An integer character to store the position of the encrypted letetr within the alphabet array (easier to work with than characters).
int positionUnoccupied = -1; //Ensures the message population code works.
bool settingSteckerPair = false; //A boolean to store whether or not a new steckered pair is being established.
int lastLedPressed; //An integer to store which LED was the last one to be illuminated.
int lastLetterPressed; //An integer to store which letter was the last one to be steckered.


//----------------------------------------------------------
//METHODS

//Setup runs once when the code is started.
void setup() {
  for (int i = 0; i < 26; i++) steckerbrettArray[i] = i; //Populates the plugboard array such that there is no substitution originally.
  for (int i = 0; i < 10; i++) steckerColoursUsed[i] = false; //Populates the array to indicate that no plugs are currently steckered.
  for (int i1 = 0; i1 < 26; i1++) for (int i2 = 0; i2 < 3; i2++) rgbLedValues[i1][i2] = 0; //Initialises the RGB LEDs so that they are all turned off.
  for (int i = 0; i < 26; i++) coloursByStecker[i] = 10; //Populate the coloursByStecker array with ints that are invalid but can still be used for operations if required.

  ledArray.begin(); //Initialise the RGB LEDs.
  ledArray.clear();
  ledArray.setBrightness(80);
  refreshLeds(); //Initialise the RGB LEDs.
  
  introduction(); //Runs the introduction code to introduce the project.
  
  for (int i = 0; i < 3; i++) display.showNumberDec(walzenSelected[i] + 1, false, 1, i + 1); //Prints the currently selected rotors on the 7-segment.
  walzenScreenRefresh(); //Shows the current orientations of the three rotors.
}

//Loop runs each time the code is refreshed.
void loop() {
  char key = keyboard.getKey(); //The below variables are used to check if any buttons on any of the keypad matrices are being pressed at any given time.
  char sbInput = steckerbrettKb.getKey();
  char settingChange = settingButtons.getKey();

  if (key != NO_KEY) { //Runs the encryption algorithm on a new letter if a key on the keyboard is pressed.
    for (int i = 0; i < 26; i++) {
      if (key == ALPHABET[i]) encryptedLetter = i;
    }
    
    encrypt(); //Encrypts the new input.
  }

    if (sbInput != NO_KEY) { //Goes through the process for altering the steckerbrett and the LEDs which function as a user interface when a button on the steckerbrett is pressed.
    int letterPressed; //letterPressed stores the letter that was pressed as an integer.
    
    for (int i = 0; i < 26; i++) {
      if (sbInput == ALPHABET[i]) letterPressed = i;
    }
    
    changeLeds(letterPressed); //Changes the steckerbrett.
  }

  if (settingChange != NO_KEY) { //Checks to see if one of the settings is being changed.
    if (settingChange <= 2) { //Substitutes a rotor if one of the buttons on the rotor substitution panel is selected.
      augmentRotors(settingChange);
    }
    
    else { //Rotates a rotor if a button on the rotor rotation panel was selected.
      int rotorToTurn; //rotorToTurn will store which rotor the user has indicated they wish to turn.
      int orientation; //Orientation will store whether the user wishes to rotate the rotor forwards or backwards.

      if (settingChange <= 5) { //Rotates the rotors backward if the bottom row of buttons is pressed.
        rotorToTurn = settingChange - 3;
        orientation = false;
      }
      else { //Rotates the rotors forward if the top row of buttons is pressed.
        rotorToTurn = settingChange - 6;
        orientation = true;
      }

      rotorSettings(rotorToTurn, orientation);
    }
  }
}

//Introduction introduces the project by printing on the LCD Screens/7-segment.
void introduction() { 
  walzenLCD.init(); //Initialise the rotor LCD.
  walzenLCD.backlight();
  
  walzenLCD.setCursor(0,0);
  walzenLCD.print("M3 FUNKSCHLÜSSEL");
  walzenLCD.setCursor(1,1);
  walzenLCD.print("ENIGMA MACHINE"); //Prints "M3 FUNKSCHLUSSEL/ENIGMA MACHINE" on the LCD Screen.
  
  outputLCD.init();
  outputLCD.backlight(); //Initialise the output LCD.
  
  outputLCD.setCursor(3,0);
  outputLCD.print("DAN SCOTT");
  outputLCD.setCursor(3,1);
  outputLCD.print("DT PROJECT"); //Prints "DAN SCOTT/DT PROJECT" on the LCD Screen.
  
  display.setBrightness(7); //Initialise the 7-segment.
  
  display.setSegments(DAN); //Print "dAn" on the 7-segment.
  
  delay(900);
  display.clear();
  delay(100); //Pauses for 1 second.
  
  display.setSegments(DTE); //Print "dtE" on the 7-segment.
  
  delay(900);
  display.clear();
  delay(100); //Pauses for 1 second.
  
  display.showNumberDec(2021, false, 4, 0); //Print "2021" on the 7-segment.
  
  delay(900);
  display.clear();
  delay(100);//Pauses for 1 second.
  
  outputLCD.init();
  outputLCD.backlight(); //Clears the output screen.
}

//RotateWalzen rotates the rotors each time a key is pressed.
void rotateWalzen() {
  walze3Orientation[26] = walze3Orientation[0]; //Stores the current first position of the third rotor.
  
  for (int i = 0; i < 27; i++) walze3Orientation[i] = walze3Orientation[i + 1]; //Rotates through the third rotor by one position.
  
  if (walze3Orientation[0] == walzenRotationLetters[walzenSelected[2]]) { //Rotates rotor 2 if rotor 3 has reached the relevant position.
    
    walze2Orientation[26] = walze2Orientation[0]; //Rotates the second rotor in the same way as the first.
    for (int i = 0; i < 27; i++) walze2Orientation[i] = walze2Orientation[i + 1];
    
    if (walze2Orientation[0] == walzenRotationLetters[walzenSelected[1]]) {
      
      walze1Orientation[26] = walze1Orientation[0]; //Rotates the first rotor in the same way as the 2nd and 3rd.
      for (int i = 0; i < 27; i++) walze1Orientation[i] = walze1Orientation[i + 1];
    }
  }
  
  walzenScreenRefresh(); //Refreshes the rotors screen to display the new rotor orientations.
}

//WalzenScreenRefresh refreshes what is displayed on the rotor LCD screen.
void walzenScreenRefresh() {
  
  char letter1 = toUpperCase(walze1Orientation[0]);
  char letter2 = toUpperCase(walze2Orientation[0]);
  char letter3 = toUpperCase(walze3Orientation[0]); //Capitalises the rotor orientations.
  
  walzenLCD.init(); //Initialises the rotor LCD.
  walzenLCD.backlight(); 
  
  walzenLCD.setCursor(3,0);
  walzenLCD.print("|1||2||3|"); //Prints the ordinances of the three rotors.
  
  walzenLCD.setCursor(3,1);
  walzenLCD.print("|");
  walzenLCD.print(letter1);
  walzenLCD.print("||");
  walzenLCD.print(letter2);
  walzenLCD.print("||");
  walzenLCD.print(letter3);
  walzenLCD.print("|"); //Prints the three rotors' orientations.
}

//Changes the output shown on the output screen.
void outputScreen() {
  
  positionUnoccupied++; //Checks how many letters are in the output array.
  
  if (positionUnoccupied == 19) { //Rearranges the array if the screen is about to be full, such that the second line is moved to the top, and the top line is taken off screen.
    for (int i = 0; i < 10; i++) {
      message[i] = message[i + 10];
      message[i + 10] = " ";
    }
    positionUnoccupied -= 10;
  }
  
  message[positionUnoccupied] = toUpperCase(ALPHABET[encryptedLetter]); //Puts the encrypted letter into the first free character in the array.
  
  outputLCD.init(); //Initialises the LCD.
  outputLCD.backlight(); 
  outputLCD.setCursor(2,0); 
  
  if (positionUnoccupied > 9) { //If the message is sufficiently long that it would fully occupy the first row, the first row is filled.
    for (int i = 0; i < 10; i++) {
      outputLCD.print(message[i]);
      if (i == 4) outputLCD.print(" "); //A space is inserted to seperate the output into 5-letter words.
    }
    
    outputLCD.setCursor(2,1);
    
    for (int i = 10; i < positionUnoccupied + 1; i++) { //Prints the second line of the output.
      outputLCD.print(message[i]);
      if (i == 14) outputLCD.print(" "); //A space is inserted to seperate the output into 5-letter words.
    }
  }
  
  else { 
    for (int i = 0; i < positionUnoccupied + 1; i++) { //If the output is not sufficiently long to fill the first row, only part of the top row is filled.
      outputLCD.print(message[i]);
      if (i == 4) outputLCD.print(" "); //A space is inserted to seperate the output into 5-letter words.
    }
  }
}

//RotorSettings allows the user to manually change the rotations of a selected rotor.
//RotorChosen indicates which rotor is being rotated, and isForward indicates whether it is being rotated forwards or backwards.
void rotorSettings(int rotorChosen, bool isForward) {
  switch (rotorChosen) { //Figures out which rotor is being turned.
    case 0:
      if (isForward) { //Rotates through to the next letter if required.
        walze1Orientation[26] = walze1Orientation[0];
        for (int i = 0; i < 27; i++) walze1Orientation[i] = walze1Orientation[i + 1];
      }
      
      else { //Rotates to the previous letter if required.
        for (int i = 26; i > 0; i--) walze1Orientation[i] = walze1Orientation[i - 1];
        walze1Orientation[0] = walze1Orientation[26];
        walze1Orientation[26] = walze1Orientation[27];
      }
      break;
      
    case 1:
      if (isForward) {
        walze2Orientation[26] = walze2Orientation[0];
        for (int i = 0; i < 27; i++) walze2Orientation[i] = walze2Orientation[i + 1];
      }
      
      else {
        for (int i = 26; i > 0; i--) walze2Orientation[i] = walze2Orientation[i - 1];
        walze2Orientation[0] = walze2Orientation[26];
        walze2Orientation[26] = walze2Orientation[27];
      }
      break;
      
    case 2:
      if (isForward) {
        walze3Orientation[26] = walze3Orientation[0];
        for (int i = 0; i < 27; i++) walze3Orientation[i] = walze3Orientation[i + 1];
      }
      
      else {
        for (int i = 26; i > 0; i--) walze3Orientation[i] = walze3Orientation[i - 1];
        walze3Orientation[0] = walze3Orientation[26];
        walze3Orientation[26] = walze3Orientation[27];
      }
      break;
  }
  
  resetOutput(); //Changing the settings clears the output.
  walzenScreenRefresh(); //Refreshes the screen displaying rotor orientations to show the change.
}

//AugmentRotors changes the rotor in the selected position such that different encryptions are available.
//RotorChosen indicates which rotor will be replaced.
void augmentRotors(int rotorChosen) {
  int newRotor = walzenSelected[rotorChosen]; //Figures out the rotor currently in the selected position.
  bool retry = false; //A boolean to check whether the new rotor is already in use so that rotors cannot be duplicated.
  
  if (newRotor < 4) newRotor++; //Rotates through to another rotor.
  else newRotor = 0;
  
  for (int i1 = 0; i1 < 3; i1++) if (walzenSelected[i1] == newRotor) { //Checks to make sure rotors are not duplicated.
    if (newRotor < 4) newRotor++;
    else newRotor = 0;
    retry = true;
  }
  
  if (retry) for (int i2 = 0; i2 < 3; i2++) if (walzenSelected[i2] == newRotor) { //Changes the rotor again if a rotor is still being duplicated.
    if (newRotor < 4) newRotor++;
    else newRotor = 0;
  }
  
  walzenSelected[rotorChosen] = newRotor; //Substitutes the rotor in the selected position for the new rotor.
  for (int i3 = 0; i3 < 3; i3++) display.showNumberDec(walzenSelected[i3] + 1, false, 1, i3 + 1); //Prints the currently selected rotors on the 7-segment.
  resetOutput(); //Changing the settings resets the output.
}

//ResetOutput resets the machine's output to be null.
void resetOutput() {
  for (int i = 0; i < 20; i++) message[i] = " "; //Sets the output string to be blank.
  positionUnoccupied = -1;
  
  outputScreen(); //Updates the output screen.
}

//RefreshLeds resends a signal to the array of RGB LEDs so that they are displaying the most recently decided colours.
void refreshLeds() {
  ledArray.clear(); //Re-initialises the LED array.
  ledArray.setBrightness(80);
  
  for (int i = 0; i < 26; i++) ledArray.setPixelColor(i, ledArray.Color(rgbLedValues[i][0], rgbLedValues[i][1], rgbLedValues[i][2])); //Goes through the rgbLedValues[] array and changes the LEDs colour to match the RGB values allocated to each LED.
  
  ledArray.show(); //Refreshes the LEDs.
} 

//ChangeLeds runs each time the steckerbrett is altered, and changes the LEDs' colours while programming the steckerbrett.
//ledChanging is the letter corresponding to the LED we wish to change.
void changeLeds(int ledChanging) {

  int ledPos; //ledPos stores the addressable position in the array of the LED whose colour is being altered.
  int colourToUse; //colourToUse stores a number which correlates with the colour that the new LED is being illuminated as.
  //allColoursUsed checks to see if there is already the maximum allowed number of steckered pairs created. 
  bool allColoursUsed = true; //This is true by default as the way we find out which colour to use is to check whether there are any colours unassigned, so we assume by default that there are already none available.

  ledPos = LED_ORDER[ledChanging]; //Checks what position in the array of LEDs the one we want to change the colour of is.

  for (int i = 0; i < 10; i++) { //Checks the "most basic/preferred" colour remaining, so that this is what is used for the new steckered pair.
    if (!steckerColoursUsed[i]) {
      if (allColoursUsed) colourToUse = i;
      allColoursUsed = false;
    }
  }

  if (rgbLedValues[ledPos][0] == 0 && rgbLedValues[ledPos][1] == 0 && rgbLedValues[ledPos][2] == 0 && !allColoursUsed) { //Runs what the code should do if creating a new steckered pair (the LED we are changing is not already on and we are able to make more pairs).

    for (int i = 0; i < 3; i++) rgbLedValues[ledPos][i] = RGB_CODES[colourToUse][i]; //Changes the RGB values assigned to the LED we are changing.

    coloursByStecker[ledPos] = colourToUse; //Changes the colour assigned to the LED.

    if (settingSteckerPair) { //Changes the steckerbrettArray and alters the encryption algorithm when a steckered pair is completed.
      steckerColoursUsed[colourToUse] = true; //Ensures the colour cannot be reused on another steckered pair while the current one in question exists.
      steckerbrettArray[ledChanging] = lastLetterPressed;
      steckerbrettArray[lastLetterPressed] = ledChanging;
    }

    settingSteckerPair = !settingSteckerPair; //Toggles whether or not a new pair is currently being established.
  }
  
  else { //Runs what the code should do if the button we pressed corresponds to a LED that is already illuminated.
    int colourRemove; //colourRemove stores a number representing the colour to be removed.
    int correspondingLed; //correspondingLed keeps track of the LED that is paired with that which is being unsteckered.
    int correspondingLetter; //correspondingLetter keeps track of the letter that is steckered to the letter being unsteckered.

    colourRemove = coloursByStecker[ledPos]; //The colour to remove is the colour of the LED corresponding to the button pressed.
    steckerColoursUsed[colourRemove] = false; //Makes the colour available to be used in future steckers.

    correspondingLetter = steckerbrettArray[ledChanging]; 
    correspondingLed = LED_ORDER[correspondingLetter];

    for (int i = 0; i < 3; i++) { //Turns off the LEDs for the two letters which are being unpaired.
      rgbLedValues[ledPos][i] = 0;
      rgbLedValues[correspondingLed][i] = 0;
    }

    steckerbrettArray[ledChanging] = ledChanging; //Resets the steckerbrett array so that the unsteckered letters no longer map to one another through the plugboard.
    steckerbrettArray[correspondingLetter] = correspondingLetter;

    if (settingSteckerPair) for (int i = 0; i < 3; i++) rgbLedValues[lastLedPressed][i] = 0; //If a pair was already being established, it is reset.
    settingSteckerPair = false;    
  }
  lastLetterPressed = ledChanging; //Keeps track of the last button pressed and its corresponding letter.
  lastLedPressed = ledPos;

  resetOutput(); //Changing the steckerbrett settings resets the output.
  refreshLeds(); //Refreshes the LEDs being displayed.
}

//SteckerReset finds any plugs that are halfway through establishing a new steckered pair and unselects them.
void steckerReset() {
  if (settingSteckerPair) {
      for (int i = 0; i < 3; i++) rgbLedValues[lastLedPressed][i] = 0; //Any pairs halfway through being established are reset.
      settingSteckerPair = false;
      
      refreshLeds(); //Refreshes the LEDs being displayed.
    }
}

//----------------------------------------------------------
//ENCRYPTION STAGES

//Encrypt sends the user input through each stage of the digitalised encryption algorithm.
void encrypt() {
  steckerReset(); //Stops any stecker pairs halfway through being established from being established.
  
  rotateWalzen(); //Rotates the walzen.
  
  steckerbrett(); //Sends the letter through the plugboard.
  walzenForwards(); //Feeds the letter through three rotors from right to left.
  umkehrwalze(); //Sends the letter through the reflector.
  walzenBackwards(); //Feeds the letter through three rotors from left to right.
  steckerbrett(); //Sends the letter through the plugboard.
}

//Steckerbrett simulates the electrical signal being sent through the enigma machine's plugboard.
void steckerbrett() {
  encryptedLetter = steckerbrettArray[encryptedLetter]; //The plugboard is a straight substitution where one input will be replaced with another.
}

//WalzenForwards runs the encryption through each of the three rotors, one after the other.
void walzenForwards() {
  forwardWalze(2);
  forwardWalze(1);
  forwardWalze(0);
}

//ForwardWalze simulates the letter's encryption travelling from the steckerbrett to the umkehrwalze.
//WalzeNumber is the number of the rotor (in terms of the order in which the code passes through them).
void forwardWalze(int walzeNumber) {
  char entryLetter; 
  int positionOrder;
  int newPosition;
  char newLetter;
  
  switch (walzeNumber) { //EntryLetter is the letter that the encryption corresponds to on entering the rotor.
    
    case 0:
      entryLetter = walze1Orientation[encryptedLetter];
      break;
      
    case 1:
      entryLetter = walze2Orientation[encryptedLetter];
      break;
      
    case 2:
      entryLetter = walze3Orientation[encryptedLetter];
      break;
  }
  
  for (int i = 0; i < 26; i++) if (ALPHABET[i] == entryLetter) positionOrder = i; //PositionOrder determines what position in the alphabet entryLetter occupies.
  
  switch (walzenSelected[walzeNumber]) { //NewPosition is the position of the alphabet of the rotor's output value.
    
    case 0:
      newPosition = walzeIArray[positionOrder];
      break;
      
    case 1:
      newPosition = walzeIIArray[positionOrder];
      break;
      
    case 2:
      newPosition = walzeIIIArray[positionOrder];
      break;
      
    case 3:
      newPosition = walzeIVArray[positionOrder];
      break;
      
    case 4:
      newPosition = walzeVArray[positionOrder];
      break;
  }
  
  newLetter = ALPHABET[newPosition]; //NewLetter is the letter output of the rotor's encryption.
  
  switch (walzeNumber) { //Finds the position within the machine's cross-section that the encryption is mapped to.
    
    case 0: 
      for (int i = 0; i < 26; i++) if (newLetter == walze1Orientation[i]) encryptedLetter = i;
      break;
      
    case 1:
      for (int i = 0; i < 26; i++) if (newLetter == walze2Orientation[i]) encryptedLetter = i;
      break;
      
    case 2:
      for (int i = 0; i < 26; i++) if (newLetter == walze3Orientation[i]) encryptedLetter = i;
      break;
  }
}

//The umkehrwalze() method runs when the encryption reaches the stage where the letter being encrypted is fed through the umkehrwalze.
void umkehrwalze() {
  encryptedLetter = umkehrwalzeArray[encryptedLetter]; //Replaces the letter in a direct substitution with the connected letter in the umkehrwalze.
}

//WalzenBackwards runs the encryption through each of the three rotors, one after the other.
void walzenBackwards() {
  backwardWalze(0);
  backwardWalze(1);
  backwardWalze(2);
}

//BackwardWalze simulates the letter's encryption travelling from the umkehrwalze back to the steckerbrett.
void backwardWalze(int walzeNumber) {
  char letterIn;
  int exitPos;
  int entryPos;
  char letterOut;
  
  switch (walzeNumber) { //LetterIn determines the letter that the encryption corresponds to on entering the rotor.
    
    case 0:
      letterIn = walze1Orientation[encryptedLetter];
      break;
      
    case 1:
      letterIn = walze2Orientation[encryptedLetter];
      break;
      
    case 2:
      letterIn = walze3Orientation[encryptedLetter];
      break;
  }
  
  for (int i = 0; i < 26; i++) if (ALPHABET[i] == letterIn) exitPos = i; //Finds the position of letterIn in the alphabet.
  
  switch (walzenSelected[walzeNumber]) { //Finds out what input letter would map to the output "letterIn" (using the same wiring as forwardWalze but in reverse).
    
    case 0:
      for (int i = 0; i < 26; i++) if (walzeIArray[i] == exitPos) entryPos = i;
      break;
      
    case 1:
      for (int i = 0; i < 26; i++) if (walzeIIArray[i] == exitPos) entryPos = i;
      break;
      
    case 2:
      for (int i = 0; i < 26; i++) if (walzeIIIArray[i] == exitPos) entryPos = i;
      break;
      
    case 3: 
      for (int i = 0; i < 26; i++) if (walzeIVArray[i] == exitPos) entryPos = i;
      break;
      
    case 4:
      for (int i = 0; i < 26; i++) if (walzeVArray[i] == exitPos) entryPos = i;
      break;
  }
  
  letterOut = ALPHABET[entryPos]; //Finds the letter corresponding to the final position the rotor provides.
  
  switch (walzeNumber) { //Determines the position in the machine's cross-section that the encryption maps to.
    
    case 0:
      for (int i = 0; i < 26; i++) if (letterOut == walze1Orientation[i]) encryptedLetter = i;
      break;
      
    case 1:
      for (int i = 0; i < 26; i++) if (letterOut == walze2Orientation[i]) encryptedLetter = i;
      break;
      
    case 2:
      for (int i = 0; i < 26; i++) if (letterOut == walze3Orientation[i]) encryptedLetter = i;
      break;
  }
}
