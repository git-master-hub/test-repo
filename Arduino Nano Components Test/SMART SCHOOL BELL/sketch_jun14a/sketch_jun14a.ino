
//#include <stdlib.h>
#include <SoftwareSerial.h>

SoftwareSerial ESP8266(3, 2); // RX, TX
unsigned char check_connection = 0;
unsigned char times_check = 0;
String alarm_date = "";
String alarm_time = "";

void setup() {
  Serial.begin(9600);
  ESP8266.begin(9600);

  ESP8266.println("AT+RST");
  delay(5000); // Increased delay to ensure the reset completes

  clearESPBuffer();

  ESP8266.println("AT+CWMODE=1");
  delay(1000);
  Serial.println("Connecting to WiFi");

  while (check_connection == 0) {
    Serial.print(".");
    ESP8266.println("AT+CWJAP=\"LGU HRMO WiFi_2.4G\",\"Raged030585\"");
    ESP8266.setTimeout(10000); // Increase timeout to allow sufficient connection time

    if (ESP8266.find("WIFI GOT IP")) {
      Serial.println("\nWIFI CONNECTED");
      check_connection = 1;
      break;
    }

    times_check++;
    if (times_check > 3) {
      times_check = 0;
      Serial.println("\nTrying to Reconnect..");
    }
  }

  // Get and print IP address
  Serial.println("Getting IP address...");
  getIPAddress();

  // Start the ESP8266 as a server
  ESP8266.println("AT+CIPMUX=1");
  delay(1000);
  ESP8266.println("AT+CIPSERVER=1,80");
  delay(1000);
}

void loop() {
  if (ESP8266.available()) {
    String request = ESP8266.readStringUntil('\r');
    Serial.println(request); // Debug: Print each request

    // Parse the connection ID from the request
    int connIdIndex = request.indexOf('+');
    int connId = request.substring(connIdIndex + 5, connIdIndex + 6).toInt();

    // Process the GET request for setting the alarm
    if (request.indexOf("GET /set_alarm?date=") != -1) {
      int date_start = request.indexOf('=') + 1;
      int date_end = request.indexOf('&');
      alarm_date = request.substring(date_start, date_end);

      int time_start = request.indexOf("time=") + 5;
      alarm_time = request.substring(time_start, request.indexOf(' ', time_start));

      // Respond with confirmation webpage
      String response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
      response += "<html><body>";
      response += "<h1>Alarm Set</h1>";
      response += "<p>Date: " + alarm_date + "</p>";
      response += "<p>Time: " + alarm_time + "</p>";
      response += "</body></html>";
      sendHTTPResponse(connId, response);

      // Print the set date and time to the serial monitor
      Serial.println("Alarm set:");
      Serial.print("Date: ");
      Serial.println(alarm_date);
      Serial.print("Time: ");
      Serial.println(alarm_time);
    } else if (request.indexOf("GET / ") != -1) {
      // Respond with HTML form for setting the alarm
      String response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
      response += "<html><body>";
      response += "<h1>SMART SCHOOL BELL</h1>";
      response += "<form action=\"/set_alarm\">";
      response += "Date: <input type=\"date\" name=\"date\"><br>";
      response += "Time: <input type=\"time\" name=\"time\"><br>";
      response += "<input type=\"submit\" value=\"Set Alarm\">";
      response += "</form>";
      response += "</body></html>";
      sendHTTPResponse(connId, response);
    }
    clearESPBuffer();
  }
}

void clearESPBuffer() {
  while (ESP8266.available()) {
    ESP8266.read();
  }
}

void getIPAddress() {
  ESP8266.println("AT+CIFSR");
  delay(1000);
  bool gotIP = false;
  while (ESP8266.available()) {
    String response = ESP8266.readStringUntil('\n');
    Serial.println(response); // Debug: Print each line of the response

    if (response.indexOf("STAIP,") != -1) {
      int start = response.indexOf("\"") + 1;
      int end = response.lastIndexOf("\"");
      String ip = response.substring(start, end);
      Serial.print("IP Address: ");
      Serial.println(ip);
      gotIP = true;
      break;
    }
  }

  if (!gotIP) {
    Serial.println("Failed to obtain IP address. Retrying...");
    delay(2000);
    clearESPBuffer();
    getIPAddress();
  }
}

void sendHTTPResponse(int connId, String response) {
  String cipSend = "AT+CIPSEND=" + String(connId) + "," + String(response.length());
  ESP8266.println(cipSend);
  delay(100);
  ESP8266.print(response);
  delay(100);
  String cipClose = "AT+CIPCLOSE=" + String(connId);
  ESP8266.println(cipClose);
  delay(100);
}
