
#include <Wire.h>
#include <PCF8575.h>
#include <Keypad.h>
#include "PCF8575.h"
#include "I2CKeyPad.h"
#define PCF8575_ADDR 0x20

PCF8575 pcf8575(PCF8575_ADDR);

// Define the keypad matrix layout
const int numRows = 4;
const int numCols = 4;
char keys[numRows][numCols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Define the pin mappings for PCF8575
const int rowPins[numRows] = {0, 1, 2, 3};
const int colPins[numCols] = {4, 5, 6, 7};

void setup() {
  Wire.begin();
  pcf8575.begin();
  
  // Set all rows as outputs and columns as inputs with pull-up resistors enabled
  for (int row = 0; row < numRows; row++) {
    pcf8575.pinMode(rowPins[row], OUTPUT);
    pcf8575.digitalWrite(rowPins[row], HIGH);
  }
  for (int col = 0; col < numCols; col++) {
    pcf8575.pinMode(colPins[col], INPUT_PULLUP);
  }
}

char getKey() {
  char key = '\0';
  for (int row = 0; row < numRows; row++) {
    pcf8575.digitalWrite(rowPins[row], LOW);
    for (int col = 0; col < numCols; col++) {
      if (pcf8575.digitalRead(colPins[col]) == LOW) {
        while (pcf8575.digitalRead(colPins[col]) == LOW) {} // Wait for key release
        key = keys[row][col];
      }
    }
    pcf8575.digitalWrite(rowPins[row], HIGH);
  }
  return key;
}

void loop() {

  Serial.println(" WELCOME ");
  char key = getKey();
  if (key != '\0') {
    Serial.println("Pressed: " + String(key));
  }
  delay(100); // Debounce delay
}
