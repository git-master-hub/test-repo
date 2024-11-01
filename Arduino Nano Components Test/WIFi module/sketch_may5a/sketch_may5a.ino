//LGU HRMO WiFi_2.4G
//Raged030585
//#include <stdlib.h>
#include <SoftwareSerial.h>

SoftwareSerial ESP8266(7, 6); // RX, TX

char alarm_date[11] = ""; // "YYYY-MM-DD\0"
char alarm_time[6] = "";  // "HH:MM\0"
char ip_address[16] = "";  // "xxx.xxx.xxx.xxx\0"

void setup() {
  Serial.begin(9600);
  ESP8266.begin(9600);

  resetESP8266();
  setWiFiMode();
  connectToWiFi();
  startServer();
  getIPAddress();
}

void loop() {
  handleClientRequests();
}

void resetESP8266() {
  ESP8266.println("AT+RST");
  delay(5000); // Ensure the reset completes
  clearESPBuffer();
}

void setWiFiMode() {
  ESP8266.println("AT+CWMODE=1");
  delay(1000);
}

void connectToWiFi() {
  Serial.println("Connecting to WiFi");
  bool connected = false;
  for (int attempt = 0; attempt < 5; attempt++) {
    ESP8266.println("AT+CWJAP=\"LGU HRMO WiFi_2.4G\",\"Raged030585\"");
    ESP8266.setTimeout(10000); // Increase timeout for connection
    if (ESP8266.find("WIFI GOT IP")) {
      Serial.println("\nWIFI CONNECTED");
      connected = true;
      break;
    }
    delay(2000); // Delay before retrying connection
  }

  if (!connected) {
    Serial.println("\nFailed to connect to WiFi.");
    // Handle retry or other actions here
  }
}

void startServer() {
  ESP8266.println("AT+CIPMUX=1");
  delay(1000);
  ESP8266.println("AT+CIPSERVER=1,80");
  delay(1000);
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
      strncpy(ip_address, response.substring(start, end).c_str(), sizeof(ip_address) - 1);
      ip_address[sizeof(ip_address) - 1] = '\0'; // Ensure null termination
      Serial.print("IP Address: ");
      Serial.println(ip_address);
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

void handleClientRequests() {
  if (ESP8266.available()) {
    String request = ESP8266.readStringUntil('\r');
    Serial.println(request); // Debug: Print each request

    // Parse the connection ID from the request
    int connIdIndex = request.indexOf('+');
    int connId = request.substring(connIdIndex + 5, connIdIndex + 6).toInt();

    // Process the GET request for setting the alarm
    if (request.indexOf("GET /set_alarm?date=") != -1) {
      handleSetAlarmRequest(request, connId);
    } else if (request.indexOf("GET / ") != -1) {
      // Respond with HTML form for setting the alarm
      sendSetAlarmForm(connId);
    }
    clearESPBuffer();
  }
}

void handleSetAlarmRequest(String request, int connId) {
  int date_start = request.indexOf('=') + 1;
  int date_end = request.indexOf('&');
  strncpy(alarm_date, request.substring(date_start, date_end).c_str(), sizeof(alarm_date) - 1);
  alarm_date[sizeof(alarm_date) - 1] = '\0'; // Ensure null termination

  int time_start = request.indexOf("time=") + 5;
  strncpy(alarm_time, request.substring(time_start, request.indexOf(' ', time_start)).c_str(), sizeof(alarm_time) - 1);
  alarm_time[sizeof(alarm_time) - 1] = '\0'; // Ensure null termination

  // Respond with confirmation webpage
  String response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  response += "<html><head><style>";
  response += "body { background-color: #f0f0f0; font-family: Arial, sans-serif; }";
  response += "h1 { color: #333; }";
  response += "form { margin: 20px; }";
  response += "input[type=date], input[type=time] { padding: 8px; }";
  response += "input[type=submit] { padding: 10px 20px; background-color: #4CAF50; color: white; border: none; }";
  response += "</style></head><body>";
  response += "<h1>Alarm Set</h1>";
  response += "<p>Date: ";
  response += alarm_date;
  response += "</p><p>Time: ";
  response += alarm_time;
  response += "</p><a href=\"/\">Set new alarm</a></body></html>";
  sendHTTPResponse(connId, response);

  // Print the set date and time to the serial monitor
  Serial.println("Alarm set:");
  Serial.print("Date: ");
  Serial.println(alarm_date);
  Serial.print("Time: ");
  Serial.println(alarm_time);
}

void sendSetAlarmForm(int connId) {
  // Respond with HTML form for setting the alarm
  String response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  response += "<html><head><style>";
  response += "body { background-color: #f0f0f0; font-family: Arial, sans-serif; }";
  response += "h1 { color: #333; }";
  response += "form { margin: 20px; }";
  response += "input[type=date], input[type=time] { padding: 8px; }";
  response += "input[type=submit] { padding: 10px 20px; background-color: #4CAF50; color: white; border: none; }";
  response += "</style></head><body>";
  response += "<h1>Set Alarm</h1>";
  response += "<form action=\"/set_alarm\">";
  response += "Date: <input type=\"date\" name=\"date\" value=\"";
  response += alarm_date;
  response += "\"><br>Time: <input type=\"time\" name=\"time\" value=\"";
  response += alarm_time;
  response += "\"><br><input type=\"submit\" value=\"Set Alarm\">";
  response += "</form></body></html>";
  sendHTTPResponse(connId, response);
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

void clearESPBuffer() {
  while (ESP8266.available()) {
    ESP8266.read();
  }
}
