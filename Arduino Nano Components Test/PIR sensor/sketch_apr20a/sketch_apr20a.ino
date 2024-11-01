// Define the pin number for the PIR sensor
int pirPin = 13; // You can change this pin as needed

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  // Set the PIR pin as input
  pinMode(pirPin, INPUT);
}

void loop() {
  // Read the value from the PIR sensor
  int pirValue = digitalRead(pirPin);
  
  // Check if motion is detected

  
  if (pirValue == HIGH) {
    Serial.println("Motion detected!");
  } else {
    Serial.println("No motion detected.");
  }
  
  // Delay for a short time to avoid flooding the serial monitor
  delay(500);
}
