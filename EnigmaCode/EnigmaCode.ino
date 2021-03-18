//DANIEL SCOTT
//ENIGMA MACHINE 2021

//INITIALISATIONS

//Libraries
#include <Wire.h> //Needed to include Wire.h to send signals down CLK pins.
#include <LiquidCrystal_I2C.h> //Included LiquidCrystal_I2C.h in order to run the LCD screens.
#include <TM1637Display.h> //Included TM1637Display.h to run the 4-digit 7-segment display with an HW-069 backpack.

//Define Connection Pins
#define CLK 23 //Define the 7-segment's Clock pin
#define DIO 22 //Define the 7-segment's Digital Input/Output pin

//LCD Setup
LiquidCrystal_I2C walzenLCD(0x3F, 16, 2); //Establishes an LCD Screen/I2C module with the address 0x3E (A0 connected)
LiquidCrystal_I2C outputLCD(0x3E, 16, 2); //Establishes an LCD Screen/I2C module with the address 0x3F (No solder pad connections)

//7-segment Setup
TM1637Display display = TM1637Display(CLK, DIO); //Established a TM1637 1x4 7-segment display with HW-069 backpack.

//7-segment display readings
const uint8_t dan[] {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G, //d
  SEG_A | SEG_B | SEG_C | SEG_G | SEG_E | SEG_F, //A
  SEG_C | SEG_E | SEG_G, //n
  0x00
}; //Prints the word "dAn" on the 7-segment.

const uint8_t dte[] {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G, //d
  SEG_D | SEG_E | SEG_F | SEG_G, //t
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G, //E
  0x00
}; //Prints the word "dtE" on the 7-segment.

//Constants
const char alphabet[26] = "abcdefghijklmnopqrstuvwxyz"; //An array to establish the order of the alphabet within code.

//Substitution Arrays
int steckerbrettArray[26]; //An array to establish the substitution pattern of the plugboard.
int umkehrwalzeArray[26] = {24, 17, 20, 7, 16, 18, 11, 3, 15, 23, 13, 6, 14, 10, 12, 8, 4, 1, 5, 25, 2, 22, 21, 9, 0, 19}; //Establishes the substitution pattern of the reflector board.
int walzeIArray[26] = {4, 10, 12, 5, 11, 6, 3, 16, 22, 25, 13, 19, 14, 22, 24, 7, 23, 20, 18, 15, 0, 8, 1, 17, 2, 9}; //Arrays to establish the single-way substitution of the 5 rotors.
int walzeIIArray[26] = {0, 9, 3, 10, 18, 8, 17, 20, 23, 1, 11, 7, 22, 19, 12, 2, 16, 6, 25, 13, 15, 24, 5, 21, 14, 4};
int walzeIIIArray[26] = {1, 3, 5, 7, 9, 11, 2, 15, 17, 19, 23, 21, 25, 13, 24, 4, 8, 22, 6, 0, 10, 12, 20, 18, 16, 14};
int walzeIVArray[26] = {4, 18, 14, 21, 15, 25, 9, 0, 24, 16, 20, 8, 17, 7, 23, 11, 13, 5, 19, 6, 10, 3, 2, 12, 22, 1};
int walzeVArray[26] = {21, 25, 1, 17, 6, 8, 19, 24, 20, 15, 18, 3, 13, 7, 11, 23, 0, 22, 12, 9, 16, 14, 5, 4, 2, 11};

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

//----------------------------------------------------------
//METHODS

//Setup runs once when the code is started.
void setup() {
  Serial.begin(9600); //Starts the Serial Monitor for testing.
  for (int i = 0; i < 26; i++) steckerbrettArray[i] = i; //Populates the plugboard array such that there is no substitution originally.
  introduction(); //Runs the introduction code to introduce the project.
  for (int i = 0; i < 3; i++) display.showNumberDec(walzenSelected[i] + 1, false, 1, i + 1); //Prints the currently selected rotors on the 7-segment.
  walzenScreenRefresh(); //Shows the current orientations of the three rotors.
}

//Loop runs each time the code is refreshed.
void loop() {
  
}

//Introduction introduces the project by printing on the LCD Screens/7-segment.
void introduction() { 
  walzenLCD.init(); //Initialise the rotor LCD.
  walzenLCD.backlight();
  walzenLCD.setCursor(4,0);
  walzenLCD.print("AS91907");
  walzenLCD.setCursor(1,1);
  walzenLCD.print("ENIGMA MACHINE"); //Prints "AS91907/ENIGMA MACHINE" on the LCD Screen.
  outputLCD.init();
  outputLCD.backlight(); //Initialise the output LCD.
  outputLCD.setCursor(3,0);
  outputLCD.print("DAN SCOTT");
  outputLCD.setCursor(2,1);
  outputLCD.print("DTE PROJECT"); //Prints "DAN SCOTT/DTE PROJECT" on the LCD Screen.
  display.setBrightness(7); //Initialise the 7-segment.
  display.setSegments(dan); //Print "dAn" on the 7-segment.
  delay(900);
  display.clear();
  delay(100); //Pauses for 1 second.
  display.setSegments(dte); //Print "dtE" on the 7-segment.
  delay(900);
  display.clear();
  delay(100); //Pauses for 1 second.
  display.showNumberDec(2021, false, 4, 0); //Print "2021" on the 7-segment.
  delay(900);
  display.clear();
  delay(100);//Pauses for 1 second.
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
  message[positionUnoccupied] = toUpperCase(alphabet[encryptedLetter]); //Puts the encrypted letter into the first free character in the array.*/
  outputLCD.init(); //Initialises the LCD.
  outputLCD.backlight(); //Turns on the LCD's backlight.
  outputLCD.setCursor(2,0); //Starts printing in the third column of the first row.
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

//AugmentRotors changes the rotor in the selected position such that different encryptions are available.
void augmentRotors(int rotorChosen) {
  int newRotor = walzenSelected[rotorChosen];
  bool retry = false;
  if (newRotor < 4) newRotor++;
  else newRotor = 0;
  for (int i1 = 0; i1 < 3; i1++) if (walzenSelected[i1] == newRotor) {
    if (newRotor < 4) newRotor++;
    else newRotor = 0;
    retry = true;
  }
  if (retry) for (int i2 = 0; i2 < 3; i2++) if (walzenSelected[i2] == newRotor) {
    if (newRotor < 4) newRotor++;
    else newRotor = 0;
  }
  walzenSelected[rotorChosen] = newRotor;
  for (int i3 = 0; i3 < 3; i3++) display.showNumberDec(walzenSelected[i3] + 1, false, 1, i3 + 1); //Prints the currently selected rotors on the 7-segment.
}

//----------------------------------------------------------
//ENCRYPTION STAGES

//Encrypt sends the user input through each stage of the digitalised encryption algorithm.
void encrypt() {
  rotateWalzen();
  steckerbrett(); //Sends the letter through the plugboard.
  walzenForwards(); //Feeds the letter through three rotors from right to left.
  umkehrwalze(); //Sends the letter through the reflector.
  walzenBackwards(); //Feeds the letter through three rotors from left to right.
  steckerbrett(); //Sends the letter through the plugboard.
  outputScreen(); //Prints the output.
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
  for (int i = 0; i < 26; i++) if (alphabet[i] == entryLetter) positionOrder = i; //PositionOrder determines what position in the alphabet entryLetter occupies.
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
  newLetter = alphabet[newPosition]; //NewLetter is the letter output of the rotor's encryption.
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
  //Replaces the letter in a direct substitution with the connected letter in the umkehrwalze.
  encryptedLetter = umkehrwalzeArray[encryptedLetter];
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
  for (int i = 0; i < 26; i++) if (alphabet[i] == letterIn) exitPos = i; //Finds the position of letterIn in the alphabet.
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
  letterOut = alphabet[entryPos]; //Finds the letter corresponding to the final position the rotor provides.
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
