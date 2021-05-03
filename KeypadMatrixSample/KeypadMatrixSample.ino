#include <Key.h>
#include <Keypad.h>

const byte ROWS = 2;
const byte COLS = 3;

char keys[ROWS][COLS] = {
  {'a', 'b', 'c'},
  {'d', 'e', 'f'}
};

byte rowPins[ROWS] = {2, 3};
byte colPins[COLS] = {38, 40, 41};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
}

void loop() {
  char key = keypad.getKey();
  if (key != NO_KEY) {
    Serial.println(key);
  }
}
