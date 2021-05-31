#include <Key.h>
#include <Keypad.h>

const byte ROWS = 2;
const byte COLS = 13;

char KEYS[ROWS][COLS] = {
  {'q', 'w', 'e', 'r', 't', 'z', 'u', 'i', 'o', 'k', 'j', 'h', 'g'},
  {'a', 's', 'd', 'f', 'p', 'y', 'x', 'c', 'v', 'l', 'm', 'n', 'b'}
};

byte colPins[COLS] = {50, 51, 52, 53, 44, 45, 46, 47, 42, 39, 41, 40, 43};
byte rowPins[ROWS] = {48, 49};

Keypad keyboard = Keypad(makeKeymap(KEYS), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
}

void loop() {
  char key = keyboard.getKey();
  if (key != NO_KEY) {
    Serial.println(key);
  }
}
