#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>

const char* ssid = "LGU HRMO WiFi_2.4G";
const char* password = "Raged030585";

ESP8266WebServer server(80);
SoftwareSerial ArduinoUno(D6, D7);

String passwordValue = "";
String order1 = "", order2 = "", order3 = "", order4 = "", order5 = "";
String message = "";

void setup() {
  Serial.begin(9600);
  ArduinoUno.begin(9600);
  delay(10);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", HTTP_GET, handleRoot);
  server.on("/password", HTTP_GET, handlePassword);
  server.on("/submit1", HTTP_GET, []() { handleFormSubmit(1); });
  server.on("/submit2", HTTP_GET, []() { handleFormSubmit(2); });
  server.on("/submit3", HTTP_GET, []() { handleFormSubmit(3); });
  server.on("/submit4", HTTP_GET, []() { handleFormSubmit(4); });
  server.on("/submit5", HTTP_GET, []() { handleFormSubmit(5); });
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = "<html><head><title>SMART PARCEL BOX</title>";
  html += "<style>body{background-color:#f0f0f0;font-family:Arial,Helvetica,Sans-Serif;margin:0;padding:0}";
  html += ".container{max-width:600px;margin:50px auto;padding:20px;background-color:#ffffff;border-radius:8px;box-shadow:0 0 10px rgba(0,0,0,0.1)}";
  html += "h1{text-align:center;color:#333;font-size:3em;font-weight:bold;text-transform:uppercase;margin-bottom:20px;font-family: 'Arial Black', Gadget, sans-serif;}";
  html += "p.developer{text-align:center;font-size:0.8em;color:#666}";
  html += "form{text-align:center}";
  html += "input[type='text'],input[type='number']{padding:10px;width:100%;margin-bottom:10px;border:1px solid #ccc;border-radius:4px;box-sizing:border-box}";
  html += "input[type='submit']{padding:10px 20px;background-color:#4CAF50;color:white;border:none;border-radius:4px;cursor:pointer}";
  html += "input[type='submit']:hover{background-color:#45a049}";
  html += ".message{color:#4CAF50;text-align:center;display:none}</style></head><body><div class='container'>";
  html += "<h1>SMART PARCEL BOX</h1>";
  html += "<p class='developer'>Developed by: Nel Gonzales</p>";
  html += "<h2>Password Reset</h2><form onsubmit='event.preventDefault();submitPassword();'>Enter 4-digit password:<br><input type='text' id='password' name='password' pattern='\\d{4}' title='Please enter exactly 4 digits' required><br><br><input type='submit' value='Submit'><p id='password-message' class='message'></p></form>";
  html += "<h2>Order Numbers</h2><form onsubmit='event.preventDefault();submitOrder(1);'>Order No. 1:<br><input type='number' id='order1' name='order' min='0' max='99999' value='" + order1 + "' required><br><br><input type='submit' value='Submit'><p id='order1-message' class='message'></p></form>";
  html += "<form onsubmit='event.preventDefault();submitOrder(2);'>Order No. 2:<br><input type='number' id='order2' name='order' min='0' max='99999' value='" + order2 + "' required><br><br><input type='submit' value='Submit'><p id='order2-message' class='message'></p></form>";
  html += "<form onsubmit='event.preventDefault();submitOrder(3);'>Order No. 3:<br><input type='number' id='order3' name='order' min='0' max='99999' value='" + order3 + "' required><br><br><input type='submit' value='Submit'><p id='order3-message' class='message'></p></form>";
  html += "<form onsubmit='event.preventDefault();submitOrder(4);'>Order No. 4:<br><input type='number' id='order4' name='order' min='0' max='99999' value='" + order4 + "' required><br><br><input type='submit' value='Submit'><p id='order4-message' class='message'></p></form>";
  html += "<form onsubmit='event.preventDefault();submitOrder(5);'>Order No. 5:<br><input type='number' id='order5' name='order' min='0' max='99999' value='" + order5 + "' required><br><br><input type='submit' value='Submit'><p id='order5-message' class='message'></p></form>";
  html += "</div><script>function submitPassword(){var xhr=new XMLHttpRequest();var password=document.getElementById('password').value;xhr.open('GET','/password?password='+password);xhr.onload=function(){if(xhr.status===200){document.getElementById('password').value='';document.getElementById('password-message').innerHTML='Password updated successfully!';document.getElementById('password-message').style.display='block';setTimeout(function(){document.getElementById('password-message').style.display='none';}, 3000);}else{document.getElementById('password-message').innerHTML='Failed to update password. Please try again.';document.getElementById('password-message').style.display='block';setTimeout(function(){document.getElementById('password-message').style.display='none';}, 3000);}};xhr.send();}function submitOrder(orderNum){var xhr=new XMLHttpRequest();var order=document.getElementById('order'+orderNum).value;xhr.open('GET','/submit'+orderNum+'?order='+order);xhr.onload=function(){if(xhr.status===200){document.getElementById('order'+orderNum).value='';document.getElementById('order'+orderNum+'-message').innerHTML='Order No. '+orderNum+' updated successfully!';document.getElementById('order'+orderNum+'-message').style.display='block';setTimeout(function(){document.getElementById('order'+orderNum+'-message').style.display='none';}, 3000);}else{document.getElementById('order'+orderNum+'-message').innerHTML='Failed to update Order No. '+orderNum+'. Please try again.';document.getElementById('order'+orderNum+'-message').style.display='block';setTimeout(function(){document.getElementById('order'+orderNum+'-message').style.display='none';}, 3000);}};xhr.send();}</script></body></html>";
  server.send(200, "text/html", html);
}

void handlePassword() {
  String newPassword = server.arg("password");
  if (newPassword.length() == 4 && newPassword.toInt() >= 0 && newPassword.toInt() <= 9999) {
    passwordValue = newPassword;
    ArduinoUno.print("P");
    ArduinoUno.print(passwordValue);
    ArduinoUno.println();
    Serial.print("New password set: ");
    Serial.println(passwordValue);
    server.send(200, "text/plain", "Password updated successfully!");
  } else {
    server.send(400, "text/plain", "Invalid password format!");
  }
}

void handleFormSubmit(int orderNumber) {
  message = "";
  String order = server.arg("order");
  if (order.length() > 0) {
    switch(orderNumber) {
      case 1:
        order1 = order;
        ArduinoUno.print("1");
        ArduinoUno.print(order1);
        ArduinoUno.println();
        break;
      case 2:
        order2 = order;
        ArduinoUno.print("2");
        ArduinoUno.print(order2);
        ArduinoUno.println();
        break;
      case 3:
        order3 = order;
        ArduinoUno.print("3");
        ArduinoUno.print(order3);
        ArduinoUno.println();
        break;
      case 4:
        order4 = order;
        ArduinoUno.print("4");
        ArduinoUno.print(order4);
        ArduinoUno.println();
        break;
      case 5:
        order5 = order;
        ArduinoUno.print("5");
        ArduinoUno.print(order5);
        ArduinoUno.println();
        break;
    }
    Serial.print("Order No. ");
    Serial.print(orderNumber);
    Serial.print(" updated to: ");
    Serial.println(order);
    server.send(200, "text/plain", "Order updated successfully!");
  } else {
    server.send(400, "text/plain", "Invalid order format!");
  }
}
