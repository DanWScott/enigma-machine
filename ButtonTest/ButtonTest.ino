#define BUTTONPIN 26

int buttonState = 0;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTONPIN, INPUT);
}

void loop() {
  buttonState = digitalRead(BUTTONPIN);
  Serial.print(buttonState);
  delay(500);
}
