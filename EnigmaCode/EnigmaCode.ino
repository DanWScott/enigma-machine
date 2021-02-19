//DANIEL SCOTT
//ENIGMA MACHINE 2021

//testing testing ---------------------------------------------------- DELETE LATER

//INITIALISATIONS

char alphabet[26] = "abcdefghijklmnopqrstuvwxyz";

//Substitution Arrays
int umkehrwalzeArray[26] = {24, 17, 20, 7, 16, 18, 11, 3, 15, 23, 13, 6, 14, 10, 12, 8, 4, 1, 5, 25, 2, 22, 21, 9, 0, 19};
int walzeIArray[26] = {4, 10, 12, 5, 11, 6, 3, 16, 22, 25, 13, 19, 14, 22, 24, 7, 23, 20, 18, 15, 0, 8, 1, 17, 2, 9};
int walzeIIArray[26] = {0, 9, 3, 10, 18, 8, 17, 20, 23, 1, 11, 7, 22, 19, 12, 2, 16, 6, 25, 13, 15, 24, 5, 21, 14, 4};
int walzeIIIArray[26] = {1, 3, 5, 7, 9, 11, 2, 15, 17, 19, 23, 21, 25, 13, 24, 4, 8, 22, 6, 0, 10, 12, 20, 18, 16, 14};
int walzeIVArray[26] = {4, 18, 14, 21, 15, 25, 9, 0, 24, 16, 20, 8, 17, 7, 23, 11, 13, 5, 19, 6, 10, 3, 2, 12, 22, 1};
int walzeVArray[26] = {21, 25, 1, 17, 6, 8, 19, 24, 20, 15, 18, 3, 13, 7, 11, 23, 0, 22, 12, 9, 16, 14, 5, 4, 2, 11};

//Walze Rotation
int walzenSelected[3] = {0, 0, 0};
char walzenRotationLetters[] = "rfwka";

//Walze Orientation
char walze1Orientation[28] = "abcdefghijklmnopqrstuvwxyz..";
char walze2Orientation[28] = "abcdefghijklmnopqrstuvwxyz..";
char walze3Orientation[28] = "abcdefghijklmnopqrstuvwxyz..";


int encryptedLetter;

//----------------------------------------------------------
//METHODS

void setup() {
  
}

void loop() {
  // put your main code here, to run repeatedly:

}

void rotateWalzen() {
  walze3Orientation[26] = walze3Orientation[0];
  for (int i = 0; i < 27; i++){
    walze3Orientation[i] = walze3Orientation[i + 1];
  }
  if (walze3Orientation[0] == walzenRotationLetters[walzenSelected[2]]) {
    walze2Orientation[26] = walze2Orientation[0];
    for (int i = 0; i < 27; i++) {
      walze2Orientation[i] = walze2Orientation[i + 1];
    }
    if (walze2Orientation[0] == walzenRotationLetters[walzenSelected[1]]) {
      walze1Orientation[26] = walze1Orientation[0];
      for (int i = 0; i < 27; i++) {
        walze1Orientation[i] = walze1Orientation[i + 1];
      }
    }
  }
}

//----------------------------------------------------------
//ENCRYPTION STAGES

void encrypt() {
  walzenForwards();
  umkehrwalze();
  walzenBackwards();
}

void walzenForwards() {
  forwardWalze(2);
  forwardWalze(1);
  forwardWalze(0);
}

void forwardWalze(int walzeNumber) {
  int currentValue;
  char currentLetter;
  if (walzeNumber == 0) currentLetter = walze1Orientation[encryptedLetter];
  else if (walzeNumber == 1) currentLetter = walze2Orientation[encryptedLetter];
  else if (walzeNumber == 2) currentLetter = walze3Orientation[encryptedLetter];
  for (int i = 0; i < 26; i++) {
    if (alphabet[i] == currentLetter) currentValue = i;
  }
  switch(walzenSelected[walzeNumber]) {
    case 0:
      currentValue = walzeIArray[currentValue];
      break;
    case 1:
      currentValue = walzeIIArray[currentValue];
      break;
    case 2:
      currentValue = walzeIIIArray[currentValue];
      break;
    case 3:
      currentValue = walzeIVArray[currentValue];
      break;
    case 4:
      currentValue = walzeVArray[currentValue];
      break;
  }
  currentLetter = alphabet[currentValue];
  for (int i = 0; i < 26; i++) {
    if (walzeNumber == 0 && walze1Orientation[i] == currentLetter) {
      encryptedLetter = i;
    }
    else if (walzeNumber == 1 && walze2Orientation[i] == currentLetter) {
      encryptedLetter = i;
    }
    else if (walzeNumber == 2 && walze3Orientation[i] == currentLetter) {
      encryptedLetter = i;
    }
  }
}

//The umkehrwalze() method runs when the encryption reaches the stage where the letter being encrypted is fed through the umkehrwalze.
void umkehrwalze() {
  //Replaces the letter in a direct substitution with the connected letter in the umkehrwalze.
  encryptedLetter = umkehrwalzeArray[encryptedLetter];
}

void walzenBackwards() {
  backwardWalze(0);
  backwardWalze(1);
  backwardWalze(2);
}

void backwardWalze(int walzeNumber) {
  int currentValue;
  char currentLetter;
  if (walzeNumber == 0) currentLetter = walze1Orientation[encryptedLetter];
  else if (walzeNumber == 1) currentLetter = walze2Orientation[encryptedLetter];
  else if (walzeNumber == 2) currentLetter = walze3Orientation[encryptedLetter];
  for (int i = 0; i < 26; i++) {
    if (alphabet[i] == currentLetter) currentValue = i;
  }
  for (int i = 0; i < 26; i++) {
    if (walzenSelected[walzeNumber] == 0 && walzeIArray[i] == currentValue) currentValue = i;
    else if (walzenSelected[walzeNumber] == 1 && walzeIIArray[i] == currentValue) currentValue = i;
    else if (walzenSelected[walzeNumber] == 2 && walzeIIIArray[i] == currentValue) currentValue = i;
    else if (walzenSelected[walzeNumber] == 3 && walzeIVArray[i] == currentValue) currentValue = i;
    else if (walzenSelected[walzeNumber] == 4 && walzeVArray[i] == currentValue) currentValue = i;
  }
  currentLetter = alphabet[currentValue];
  for (int i = 0; i < 26; i++) {
    if (walzeNumber == 0 && walze1Orientation[currentValue] == currentLetter) encryptedLetter = i;
    else if (walzeNumber == 1 && walze2Orientation[currentValue] == currentLetter) encryptedLetter = i;
    else if (walzeNumber == 2 && walze3Orientation[currentValue] == currentLetter) encryptedLetter = i;
  }
}
