// include the library code:
#include "Arduino.h"
#include "LiquidCrystal.h"

const int buttonPin = 6;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

uint32_t datahex(uint8_t* string, size_t slength) {

    // must be 8 chars, null pointers bad news.
    if(string == NULL || slength != 8) 
       return NULL;

    uint32_t res = 0xdeadbeef;

    size_t index = 0;
    while (index < slength) {
        char c = string[index];
        int value = 0;
        if(c >= '0' && c <= '9')
          value = (c - '0');
        else if (c >= 'A' && c <= 'F') 
          value = (10 + (c - 'A'));
        else if (c >= 'a' && c <= 'f')
          value = (10 + (c - 'a'));
        else {
          // non-hex digit
          return NULL;
        }

        res = res & (value << ((7 - index) * 8));

        index++;
    }

    return data;
}

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
bool locked = true;

bool correctInput(uint8_t* recvBuf __attribute__((unused)))
{
  return true;
}

void doSerial() {
  //if (c % 100 == 0) {
  //  Serial.println(K, HEX);
  //}

  uint8_t recvBuf[9]; // Eight key chars plus terminator. Should be enough for anybody.
  uint32_t bufLoc = 0; // Where are we writing to in the buffer?

  Serial.print("Enter code: ");
  while (true) {
    if (Serial.available()) {
      recvBuf[bufLoc] = Serial.read();
      if (recvBuf[bufLoc] == '\n') {
        Serial.write(recvBuf, bufLoc);
        Serial.write("\n");
        bufLoc = 0;
        if (correctInput(recvBuf)) {
          locked = false;
          Serial.print("Unlocking...\n");
          lcd.setCursor(0, 0);
          lcd.print("UNLOCKED");
        } else {
          Serial.print("Enter code: ");
        }
      } else {
        bufLoc++;
      }
    }
  }
}

extern "C"
void loop() {
  lcd.setCursor(0, 0);
  if (K) {
    if (locked)
      lcd.print("LOCKED          ");
    else
      lcd.print("UNLOCKED        ");


    doSerial();
  } else {
    unsigned int state = digitalRead(buttonPin);
    lcd.setCursor(0, 1);
    lcd.print("K: ");
    if (state == LOW) {
      randomSeed(millis());
      K = random();
      lcd.print(K, HEX);
    }
  }

  c += 1;
  delay(1);
}

