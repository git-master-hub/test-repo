#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>

const char* ssid = "Lyra's Wifi 2.4 GHz";
const char* password = "lyamnel01232020";

ESP8266WebServer server(80);
SoftwareSerial ArduinoUno(D2, D3);

String dateValue;
String timeValue = "";

void setup() {
    Serial.begin(9600);
    ArduinoUno.begin(9600);
    delay(10);

    WiFi.begin(ssid, password);
    Serial.print("Connecting to ");
    Serial.print(ssid);

    unsigned long startAttemptTime = millis();
    const unsigned long timeout = 30000; // 30 seconds timeout

    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout) {
        delay(500);
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("");
        Serial.print("WiFi connected. IP address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("");
        Serial.println("Failed to connect to WiFi. Restarting...");
        ESP.restart();
    }

    server.on("/", HTTP_GET, handleRoot);
    server.on("/submitDateTime", HTTP_GET, handleDateTimeSubmit);
    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();
}

void handleRoot() {
    String html = R"=====(
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>SMART SCHOOL BELL</title>
        <style>
            body { background-color: #f0f0f0; font-family: Arial, Helvetica, Sans-Serif; margin: 0; padding: 0; }
            .container { max-width: 600px; margin: 50px auto; padding: 20px; background-color: #ffffff; border-radius: 8px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); }
            h1 { text-align: center; color: #333333; }
            form { text-align: center; }
            input[type='date'], input[type='text'] { padding: 10px; width: calc(100% - 22px); margin-bottom: 10px; border: 1px solid #ccc; border-radius: 4px; box-sizing: border-box; }
            input[type='submit'] { padding: 10px 20px; background-color: #4CAF50; color: white; border: none; border-radius: 4px; cursor: pointer; }
            input[type='submit']:hover { background-color: #45a049; }
            .message { color: #4CAF50; text-align: center; }
            .clock-icon { width: 100px; height: 100px; margin: 20px auto; position: relative; }
            .clock-icon:before { content: ''; display: block; width: 100%; height: 100%; border: 5px solid #333; border-radius: 50%; position: absolute; top: 0; left: 0; box-sizing: border-box; }
            .clock-hand { position: absolute; width: 2px; height: 40%; background-color: #333; top: 10%; left: 50%; transform-origin: bottom center; animation: rotate 60s linear infinite; }
            .clock-hand.second { background-color: red; height: 45%; animation-duration: 60s; }
            .clock-hand.minute { height: 35%; animation-duration: 3600s; }
            .clock-hand.hour { height: 25%; animation-duration: 43200s; }
            @keyframes rotate { from { transform: rotate(0deg); } to { transform: rotate(360deg); } }
            .school-theme { background-color: #f9fafb; }
            .header { background-color: #6c7ae0; color: white; padding: 10px; text-align: center; border-radius: 8px 8px 0 0; }
            @keyframes fadeIn { from { opacity: 0; } to { opacity: 1; } }
            .container { animation: fadeIn 1s ease-in; }
            @media (max-width: 600px) {
                .container { margin: 20px; padding: 15px; }
                h1 { font-size: 1.5em; }
                .clock-icon { width: 80px; height: 80px; }
                .clock-hand { width: 1.5px; }
                .clock-hand.second { height: 40%; }
                .clock-hand.minute { height: 30%; }
                .clock-hand.hour { height: 20%; }
            }
        </style>
        <script>
            function submitDateTime() {
                var xhr = new XMLHttpRequest();
                var date = document.getElementById('date').value;
                var time = document.getElementById('time').value;
                xhr.open('GET', '/submitDateTime?date=' + date + '&time=' + time);
                xhr.onload = function() {
                    if (xhr.status === 200) {
                        document.getElementById('time').value = '';
                        document.getElementById('datetime-message').innerHTML = 'Time updated successfully!';
                        setTimeout(function() {
                            document.getElementById('datetime-message').innerHTML = '';
                        }, 3000);
                    } else {
                        document.getElementById('datetime-message').innerHTML = 'Failed to update Time. Please try again.';
                    }
                };
                xhr.send();
            }
        </script>
    </head>
    <body class='school-theme'>
        <div class='container'>
            <div class='header'>
                <h1>SMART SCHOOL BELL</h1>
            </div>
            <div class='clock-icon'>
                <div class='clock-hand second'></div>
                <div class='clock-hand minute'></div>
                <div class='clock-hand hour'></div>
            </div>
            <form onsubmit='event.preventDefault(); submitDateTime();'>
                Date:<br><input type='date' id='date' name='date' required><br><br>
                Time:<br><input type='text' id='time' name='time' placeholder='HH:MM' pattern='[0-9]{1,2}:[0-9]{1,2}' required><br><br>
                <input type='submit' value='Set Time'>
                <p id='datetime-message' class='message'></p>
            </form>
        </div>
    </body>
    </html>
    )=====";

    server.send(200, "text/html", html);
}

void handleDateTimeSubmit() {
    dateValue = server.arg("date");
    timeValue = server.arg("time");

    // Split date into year, month, and day
    int year, month, day;
    sscanf(dateValue.c_str(), "%d-%d-%d", &year, &month, &day);

    // Format date without leading zeros in month and day
    String formattedDate = String(year) + "-" + String(month) + "-" + String(day);

    // Remove leading zeros from time
    if (timeValue.length() > 0 && timeValue[0] == '0') {
        timeValue = timeValue.substring(1); // Remove leading zero
    }

    String dateTime = "Date: " + formattedDate + " Time: " + timeValue;

    // Send to Arduino
    ArduinoUno.println(dateTime);

    // Print to serial monitor for debugging
    Serial.println(dateTime);

    server.send(200, "text/plain", "Date and Time updated successfully!");
}
