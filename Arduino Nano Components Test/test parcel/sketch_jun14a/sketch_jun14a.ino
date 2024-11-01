#include <SoftwareSerial.h>

SoftwareSerial ESP8266(7, 6); // RX, TX
unsigned char check_connection = 0;
unsigned char times_check = 0;
int newPassword = 0; // Global variable to store the new password

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

    // Handle any GET request
    if (request.indexOf("GET / ") != -1) {
      // Handle password reset request
      handlePasswordReset(connId);
    } else if (request.indexOf("GET /reset_password?password=") != -1) {
      // Handle the form submission with the new password
      int passwordIndex = request.indexOf("password=") + 9;
      newPassword = request.substring(passwordIndex).toInt();
      
      // Print the received password to Serial Monitor
      Serial.print("New Password Received: ");
      Serial.println(newPassword);
      
      // Respond with a confirmation message and keep the input form
      String response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
      response += "<html><body>";
      response += "<h1>Password Reset Successful</h1>";
      response += "<p>New password set: " + String(newPassword) + "</p>";
      response += "<p>Enter new 4-digit password: <input type=\"number\" name=\"password\" min=\"0\" max=\"9999\" required></p>";
      response += "<form action=\"/reset_password\">";
      response += "<input type=\"submit\" value=\"Reset\">";
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
}

void handlePasswordReset(int connId) {
  // Respond with HTML form for password reset
  String response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  response += "<html><body>";
  response += "<h1>Password Reset</h1>";
  response += "<form action=\"/reset_password\">";
  response += "<p>Enter new 4-digit password: <input type=\"number\" name=\"password\" min=\"0\" max=\"9999\" required></p>";
  response += "<input type=\"submit\" value=\"Reset\">";
  response += "</form>";
  response += "</body></html>";
  sendHTTPResponse(connId, response);
}
