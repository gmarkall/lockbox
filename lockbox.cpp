// include the library code:
#include "Arduino.h"
#include "LiquidCrystal.h"

const int buttonPin = 6;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// For parsing user input back into a value.
int conversion_error;

uint32_t datahex(const char* string, size_t slength) {

    conversion_error = 0;

    // must be 8 chars, null pointers bad news.
    if(string == NULL || slength != 8) {
      conversion_error = 1;
      return 0;
    }

    uint32_t res = 0;

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
          conversion_error = 1;
          return 0;
        }

        uint32_t shift =  ((7 - index) * 4);
        uint32_t addition = value << shift;
        res = res | addition;

        index++;
    }

    return res;
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

// Uncomment to protect function with stack erase
//__attribute__((stack_erase))
bool correctInput(const char* recvBuf, size_t buflen)
{
  uint32_t K_try = datahex(recvBuf, buflen);
  uint32_t K_correct = K;

  if (conversion_error)
    return false;

  return K_correct == K_try;
}

char * readSerialBuf() {
  char buf[8];
  uint32_t bufLoc = 0;
  while (true) {
    if (Serial.available()) {
      lcd.setCursor(10,0);
      lcd.print(bufLoc);
      buf[bufLoc] = Serial.read();
      if (buf[bufLoc] == '\n') {
        Serial.write(buf, bufLoc+1);
        char * retBuf = (char *) malloc(8*sizeof(char));
        strncpy(retBuf, buf, (8*sizeof(char)));
        return retBuf;
      }
      bufLoc++;
    }
  }
}


void doSerial() {
  Serial.print("Enter code: ");
  char * recvBuf = readSerialBuf();
  if (correctInput(recvBuf, 8)) {
    locked = false;
    Serial.print("Unlocking...\n");
    lcd.setCursor(0, 0);
    lcd.print("UNLOCKED             ");
  } else {
    Serial.println("Incorrect code...");
    return;
  }
}

extern "C"
void loop() {
  unsigned int state = digitalRead(buttonPin);
  if (state == LOW) {
    randomSeed(millis());
    K = random(); //K has type uint32_t
    lcd.setCursor(0, 1);
    lcd.print(K, HEX);

    lcd.setCursor(0, 0);
    lcd.print("LOCKED          ");
    while (true) { doSerial(); }
  }
}

