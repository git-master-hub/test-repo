#include <Wire.h>

// Define PCF8575 I2C address
#define PCF8575_ADDRESS 0x27

// Define DFPlayer Mini commands
#define CMD_STOP_PLAYBACK 0x16
#define CMD_SET_PLAY_MODE 0x12
#define CMD_PLAY_TRACK 0x0F

void setup() {
  Wire.begin(); // Initialize I2C communication
  Serial.begin(9600); // Initialize serial communication
  
  delay(1000); // Allow time for setup

  // Start playing Track 1
  playTrack(1);
}

void loop() {
  // No specific code needed in the loop for this example
}

// Function to send commands to DFPlayer Mini via hardware serial
void sendCommand(uint8_t command, uint8_t param1 = 0, uint8_t param2 = 0) {
  Serial.write(0x7E); // Start byte
  Serial.write(0xFF); // Version
  Serial.write(0x06); // Command length
  Serial.write(command); // Command
  Serial.write(0x00); // Feedback (no)
  Serial.write(param1); // ParameterH
  Serial.write(param2); // ParameterL
  Serial.write(0xEF); // End byte
}

// Function to play a specific track on DFPlayer Mini
void playTrack(uint8_t trackNumber) {
  Serial.print("Playing Track ");
  Serial.println(trackNumber);

  sendCommand(CMD_STOP_PLAYBACK); // Stop any current playback
  delay(500); // Delay for stop command to take effect

  sendCommand(CMD_SET_PLAY_MODE, 0x01); // Set playback mode to single loop
  delay(500); // Delay for mode setting to take effect

  sendCommand(CMD_PLAY_TRACK, 0x00, trackNumber); // Play the specified track
  delay(500); // Delay for play command to take effect
}
