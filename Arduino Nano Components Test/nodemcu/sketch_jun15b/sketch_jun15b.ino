#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

SoftwareSerial NodeMCU(D6, D7);
const char* ssid = "LGU HRMO WiFi_2.4G";
const char* password = "Raged030585";

void setup() {
 
 Serial.begin(9600);
 NodeMCU.begin(9600);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

 pinMode(D6, INPUT);
 pinMode(D7, OUTPUT);

}

void loop() {
  //int i=10;
  //NodeMCU.print(i);
 // NodeMCU.println("\n");
 // delay(30);

}
