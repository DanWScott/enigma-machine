#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd1(0x3E,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd2(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  lcd1.init();                      // initialize the lcd 
  lcd1.init();
  // Print a message to the LCD.
  lcd1.backlight();
  lcd1.setCursor(0,0);
  lcd1.print("Screen ONE (1)");
  lcd2.init();                      // initialize the lcd 
  lcd2.init();
  // Print a message to the LCD.
  lcd2.backlight();
  lcd2.setCursor(0,0);
  lcd2.print("Screen TWO (2)");
}


void loop()
{
}
