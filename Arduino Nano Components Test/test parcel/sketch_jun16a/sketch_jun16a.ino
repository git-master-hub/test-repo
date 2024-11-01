
#define BLYNK_TEMPLATE_ID "TMPL64FTf3ioP"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "pfSDiJhYFEerkA0P35cHdWSiQd9N6vcC"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
#define trigPin 8
#define echoPin 9

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include "DFRobotDFPlayerMini.h"
#include <Keypad.h>
#include <Servo.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "LGU HRMO WiFi_2.4G";
char pass[] = "Raged030585";
int pirPin = 13;
long duration;
int distance;
static const uint8_t PIN_MP3_TX = 12; // Connects to module's RX 
static const uint8_t PIN_MP3_RX = 11; // Connects to module's TX 
// Hardware Serial on Mega, Leonardo, Micro...
//#define EspSerial Serial1

// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(7, 6); // RX, TX
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);
// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600
#define trigPin 8
#define echoPin 9
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
Servo myservo;
//DFRobotDFPlayerMini player;
String password = "1234"; // Set your password here
String enteredPassword = "";
ESP8266 wifi(&EspSerial);
DFRobotDFPlayerMini player;
int sliderValue = 0;
int setvalue= 0;
BlynkTimer timer;

void setup()
{
  // Debug console
  Serial.begin(9600);
  softwareSerial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  myservo.attach(10);
  myservo.write(180);
  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  //Blynk.begin(auth, wifi, ssid, pass);
  // You can also specify server:
  
  //Blynk.begin(auth, wifi, ssid, pass, IPAddress(192,168,1,100), 8080);
 Blynk.begin(auth, wifi, ssid, pass, "blynk.cloud", 80);
  // This function will be called every time the slider widget in Blynk app changes value
}
BLYNK_WRITE(V2) {
  sliderValue = param.asInt(); // Assign incoming value from slider to sliderValue
}
BLYNK_WRITE(V0) {
  setvalue = param.asInt(); // Assign incoming value from slider to sliderValue
}

int a=0; 
 int b=0;
 int c=0;
void loop() {
  // Run Blynk functions and timers
  Blynk.run();
  timer.run();
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;
  // Monitor sensors or inputs
  int pirValue = digitalRead(pirPin);
  if (pirValue == HIGH) {
    Serial.println("Motion detected!");
    // Perform actions
  }
  if (distance>3){
  int n=1;
  do{

     player.play(9);   
          player.play(10);
  }while(n=1);
  }
  // Other non-blocking tasks
  // ...

  // Ensure to keep the loop running efficiently
}
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!

