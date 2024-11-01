
#include <Keypad.h>
#include <Servo.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <stdlib.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#define BLYNK_PRINT Serial
#define BLYNK_AUTH_TOKEN ""
char auth[] = BLYNK_AUTH_TOKEN;
SoftwareSerial ESP8266(7, 6); // RX, TX
unsigned char check_connection=0;                                      
unsigned char times_check=0;
/////PIR sensor///
int pirPin = 13; // You can change this pin as needed
///ultrasonic/////
#define trigPin 8
#define echoPin 9
long duration;
int distance;
/////////////////////////////////
static const uint8_t PIN_MP3_TX = 12; // Connects to module's RX 
static const uint8_t PIN_MP3_RX = 11; // Connects to module's TX 
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);

// Create the Player object
DFRobotDFPlayerMini player;
////////keypad/////////////////////////////////////////////////////////////////////
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A0, A1, A2, A3}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
/////servo//////////////////////////////////////////////////////////////////////////
Servo myservo;

String password = "1234"; // Set your password here
String enteredPassword = ""; 

void setup(){
//////PIR sensor//////
// Set the PIR pin as input
  pinMode(pirPin, INPUT);
 ////ultrasonic///
// Set the trigPin as an output
  pinMode(trigPin, OUTPUT);
  
  // Set the echoPin as an input
  pinMode(echoPin, INPUT);

///////////////////////////////////
 
  Serial.begin(9600);
  myservo.attach(10);
  myservo.write(180);
  delay(1000);

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

 int a=0; 
 int b=0;
 int c=0;
void loop(){
if (distance>2){
  Serial.print("Security breach detected!");
    player.volume(30);
     player.play(9);
         delay(3100);
          player.play(10);
         delay(10000);

}
  char key = keypad.getKey();
  int pirValue = digitalRead(pirPin);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(100);
  Serial.print(a);
  do{
 if ((pirValue == HIGH) && (c==0) && (a==0)==1){
    Serial.println("Motion detected!");
    player.volume(30);
    player.play(5);
         delay(5000);

  } 
  
  }while(key==1);
if (key){
    a=a+1;
    Serial.println(key);
} 
if (key != NO_KEY) {
    if (key != '#') {
      enteredPassword += key;
    } else {
      if (enteredPassword == password) {
        Serial.println("Password Correct");
        // put your main code here, to run repeatedly:
          // Set volume to maximum (0 to 30).
          myservo.write(90);              // tell servo to go to position in variable 'pos'
          delay(500);
          player.volume(30);
           // Play the first MP3 file on the SD card
          player.play(2);
          delay(2000);
         
          player.play(1);
          delay(2500);
          player.play(6);
          delay(4000);
          while(distance==2){
            
            digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(100);
            Serial.print(b);
          }
          while(distance>=3 || distance ==1){
            b=1;
            delay(5000);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2; 
  }    
         if((distance==2) && (b==1)){ 
            myservo.write(180);
         }

        enteredPassword = ""; // Reset entered password
      } else {
        // put your main code here, to run repeatedly:
    
         player.play(3);
         delay(1300);
         
         player.play(4);
         delay(1000);
         myservo.write(180);
         delay(100);

        Serial.println("Incorrect Password");
        enteredPassword = ""; // Reset entered password
        c=1;
      }
    }
  }
if((b==1) && (distance==2)==1){
  b=0;
  a=0;
  c=0;
}
}