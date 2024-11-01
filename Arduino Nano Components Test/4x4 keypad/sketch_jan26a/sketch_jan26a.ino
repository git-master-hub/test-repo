
#include "Wire.h"
#include "PCF8575.h"
#include <Keypad.h>
PCF8575 pcf(0x20);

// Define the pins for the rows and columns of the keypad
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {P0, P1, P2, P3}; // change these to match your wiring
byte colPins[COLS] = {P4, P5, P6, P7}; // change these to match your wiring

void setup() {
  Serial.begin(9600);
  Wire.begin();
  pcf.begin(0x20); // Adjust the address if necessary
  // initialize the keypad
  for (byte r = 0; r < ROWS; r++) {
    pcf.pinMode(rowPins[r], INPUT_PULLUP);
  }
  for (byte c = 0; c < COLS; c++) {
    pcf.pinMode(colPins[c], OUTPUT);
    pcf.digitalWrite(colPins[c], HIGH);
  }
}

void loop() {
  char key = getKey();
  if (key != '\0') {
    Serial.print("Key Pressed: ");
    Serial.println(key);
  }
  delay(100); // Add a small delay to debounce
}

char getKey() {
  for (byte c = 0; c < COLS; c++) {
    pcf.digitalWrite(colPins[c], LOW);
    for (byte r = 0; r < ROWS; r++) {
      if (pcf.digitalRead(rowPins[r]) == LOW) {
        delay(50); // Debounce delay
        while (pcf.digitalRead(rowPins[r]) == LOW); // Wait until key is released
        pcf.digitalWrite(colPins[c], HIGH);
        return keys[r][c];
      }
    }
    pcf.digitalWrite(colPins[c], HIGH);
  }
  return '\0'; // If no key is pressed
}
