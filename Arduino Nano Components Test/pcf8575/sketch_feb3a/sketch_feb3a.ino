#include "Arduino.h"
#include <Keypad.h>
#include <Servo.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "PCF8575.h"
#include "Wire.h"

PCF8575 pcf8575(0x20);


static const uint8_t PIN_MP3_TX = 13; // Connects to module's RX 
static const uint8_t PIN_MP3_RX = 11; // Connects to module's TX 
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);

const int wirePin = 4; 
const int endPin = 5; 

// Create the Player object
DFRobotDFPlayerMini player;
/////////////////////////////////////////////////////////////////////////////
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
///////////////////////////////////////////////////////////////////////////////
Servo myservo;

String password = "1234"; // Set your password here
String enteredPassword = ""; 

void setup(){
  Serial.begin(9600);
  myservo.attach(12);
  myservo.write(0);
  delay(1000);

pcf8575.pinMode(P1,OUTPUT);
pcf8575.begin();
   // put your setup code here, to run once:
  pinMode(wirePin, INPUT);
  pinMode(endPin, INPUT);
 // Init USB serial port for debugging
  Serial.begin(9600);
  // Init serial port for DFPlayer Mini
  softwareSerial.begin(9600);
  // Start communication with DFPlayer Mini
  if (player.begin(softwareSerial)) {
   Serial.println("OK");  
  } else {
    Serial.println("Connecting to DFPlayer Mini failed!");
  }
}

  
void loop(){


pcf8575.digitalWrite(P1,HIGH);
delay(500);
pcf8575.digitalWrite(P1,LOW);
delay(500);         
myservo.write(180);              // tell servo to go to position in variable 'pos'
          delay(500);  
  char key = keypad.getKey();
 
  if (key){
    Serial.println(key);
  }

  if (key != NO_KEY) {
    if (key != '#') {
      enteredPassword += key;
    } else {
      if (enteredPassword == password) {
        // put your main code here, to run repeatedly:
          // Set volume to maximum (0 to 30).
          player.volume(30);
           // Play the first MP3 file on the SD card
          player.play(2);
          delay(2000);
         
          player.play(1);
          delay(1000);
         
        Serial.println("Password Correct");
        // Move servo to unlock position
        // goes from 0 degrees to 90 degrees
          myservo.write(180);              // tell servo to go to position in variable 'pos'
          delay(500);                       // waits 15ms for the servo to reach the position
        
        //delay(1000); // Delay for 1 second
        
        enteredPassword = ""; // Reset entered password
      } else {
        // put your main code here, to run repeatedly:
        // Set volume to maximum (0 to 30).
         player.volume(30);
      // Play the first MP3 file on the SD card
         player.play(3);
         delay(1300);
         
         player.play(4);
         delay(1000);
         myservo.write(0);
         delay(100);

        Serial.println("Incorrect Password");
        enteredPassword = ""; // Reset entered password

      }
    }
  }
  
}
