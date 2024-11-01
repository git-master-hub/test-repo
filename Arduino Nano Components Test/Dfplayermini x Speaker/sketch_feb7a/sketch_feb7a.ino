#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

static const uint8_t PIN_MP3_TX = 12; // Connects to module's RX 
static const uint8_t PIN_MP3_RX = 11; // Connects to module's TX 
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);


// Create the Player object
DFRobotDFPlayerMini player;

void setup() {
  // put your setup code here, to run once:

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

void loop() {
  // put your main code here, to run repeatedly:
  // Set volume to maximum (0 to 30).
    player.volume(30);
    // Play the first MP3 file on the SD card
    player.play(1);
    delay(1000);
    player.play(2);
    delay(1000);
     player.play(3);
    delay(4000);
     player.play(4);
    delay(4000);

}
