// include the library code:
#include "Arduino.h"
#include "LiquidCrystal.h"

const int buttonPin = 6;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

extern "C"
void setup() {
  
  pinMode(buttonPin, INPUT);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Stack Erase demo");

  Serial.begin(9600);
}

int c = 0;
uint32_t K = 0;

extern "C"
void loop() {
  unsigned int state = digitalRead(buttonPin);
  lcd.setCursor(0, 1);
  lcd.print("K: ");
  if (state == LOW) {
    randomSeed(millis());
    K = random();
    lcd.print(K);
  }

  if (c % 100 == 0) {
    Serial.println(K);
  }

  c += 1;
  delay(1);
}
