// include the library code:
#include "LiquidCrystal.h"
#include "WMath.h"
#include "wiring.h"
#include "wiring_constants.h"
#include "wiring_digital.h"

const int buttonPin = 6;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  
  pinMode(buttonPin, INPUT);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Stack Erase demo");


}

void loop() {
  unsigned int state = digitalRead(buttonPin);
  lcd.setCursor(0, 1);
  lcd.print("K: ");
  if (state == LOW) {
    randomSeed(millis());
    lcd.print(random()); 
  }
  delay(1);
}