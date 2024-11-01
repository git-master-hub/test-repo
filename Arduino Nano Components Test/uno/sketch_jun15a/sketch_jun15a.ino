
#include <Keypad.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"
#include <stdlib.h>

#define trigPin 8
#define echoPin 9

SoftwareSerial ArduinoUno(7, 6);
int pirPin = 13; 

long duration;
int distance;
static const uint8_t PIN_MP3_TX = 12; // Connects to module's RX 
static const uint8_t PIN_MP3_RX = 11; // Connects to module's TX 
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);
DFRobotDFPlayerMini player;
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

String enteredPassword = ""; 
String orderno= "";
int newPassword = 0;
int orderNo1 = 0;
int orderNo2 = 0;
int orderNo3 = 0;
int orderNo4 = 0;
int orderNo5 = 0;

void setup() {
  Serial.begin(9600);
  ArduinoUno.begin(9600);

  pinMode(pirPin, INPUT);
 ////ultrasonic///
// Set the trigPin as an output
  pinMode(trigPin, OUTPUT);
  
  // Set the echoPin as an input
  pinMode(echoPin, INPUT);
  myservo.attach(10);
  myservo.write(180);
  delay(1000);
  // Init serial port for DFPlayer Mini
  softwareSerial.begin(9600);
  // Start communication with DFPlayer Mini
  if (player.begin(softwareSerial)) {
   Serial.println("OK");  
  } else {
    Serial.println("Connecting to DFPlayer Mini failed!");
  }
 delay(1000); 
}

void security(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;
  Serial.print("Distance: \n");
  Serial.print(distance);
  Serial.println(" cm");
  delay(100);

}
 int a=0; 
 int b=0;
 int c=0;
 int d=0;
void loop() {
  static String inputBuffer = ""; // Buffer to collect incoming data

  while (ArduinoUno.available() > 0) {
    char incomingChar = ArduinoUno.read();

    if (incomingChar == '\n') {
      processInput(inputBuffer); // Process the complete line
      inputBuffer = ""; // Clear the buffer
    } else {
      inputBuffer += incomingChar; // Add character to buffer
    }
  }

  delay(30);

  security();

int pirValue = digitalRead(pirPin);
if(pirValue==HIGH && a==0){
    Serial.print("Motion detected!\n");
     player.volume(30);
     player.play(5);
   }

if (distance>3){
  Serial.print("Security breach detected!");
  int n=1;
  do{
    player.volume(30);
     player.play(9);
         delay(3100);
          player.play(10);
         delay(10000);
  }while(n=1);
}
char key = keypad.getKey();
if (key){
    a=a+1;
    Serial.println(key);
    player.play(11);
    //player.volume(30); 
    //delay(1000);
} 
if (key != NO_KEY) {
    if (key != '#') {
      enteredPassword += key;
    } else {
      if (enteredPassword == newPassword) {
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
          security();
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
            player.play(7);
            delay(2000);
         }

        enteredPassword = ""; // Reset entered password
        if(b==1){
          do{
          if (key){
         Serial.println(key);
         player.play(11);
         //player.volume(30); 
         //delay(1000);
          }
          if (key != NO_KEY) {
          if (key != '#') {
          orderno += key;
          } else {
         if (orderno == orderNo1 || orderNo2 || orderNo3 || orderNo4 || orderNo5) {
         Serial.println("Claim the payment");
         }
        }
       }
      }while(d==0);
    }
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

void processInput(String input) {
  if (input.length() > 1) { // Ensure input is valid
    char type = input.charAt(0);
    int val = input.substring(1).toInt();

    Serial.print("Received: ");
    Serial.print(type);
    Serial.print(val);
    Serial.println();

    switch (type) {
      case 'P':  // New password
        newPassword = val;
        Serial.print("New Password: ");
        Serial.println(newPassword);
        break;
      case '1':  // Order No. 1
        orderNo1 = val;
        Serial.print("Order No. 1: ");
        Serial.println(orderNo1);
        break;
      case '2':  // Order No. 2
        orderNo2 = val;
        Serial.print("Order No. 2: ");
        Serial.println(orderNo2);
        break;
      case '3':  // Order No. 3
        orderNo3 = val;
        Serial.print("Order No. 3: ");
        Serial.println(orderNo3);
        break;
      case '4':  // Order No. 4
        orderNo4 = val;
        Serial.print("Order No. 4: ");
        Serial.println(orderNo4);
        break;
      case '5':  // Order No. 5
        orderNo5 = val;
        Serial.print("Order No. 5: ");
        Serial.println(orderNo5);
        break;
      default:
        Serial.println("Unknown type");
        break;
    }
  }
}
