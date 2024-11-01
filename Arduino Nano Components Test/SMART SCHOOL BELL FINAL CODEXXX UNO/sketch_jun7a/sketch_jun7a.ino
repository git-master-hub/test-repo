#include <Wire.h>
#include <I2C_RTC.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

static DS3231 RTC;
LiquidCrystal_I2C lcd(0x27, A4, A5); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7)
SoftwareSerial ArduinoUno(2, 3);
int buzzer = 10;
int receivedYear;
int receivedMonth;
int receivedDay;
int receivedHour;
int receivedMinute;
int year, day, month, hour, minute;

void setup() {
    Serial.begin(9600);
    ArduinoUno.begin(9600);
    // Buzzer
    pinMode(buzzer, OUTPUT);
    RTC.begin();
    lcd.begin(16, 2);

    RTC.setHourMode(CLOCK_H24);
    RTC.setDay(22);
    RTC.setMonth(2);
    RTC.setYear(2024);
    RTC.setHours(21);
    RTC.setMinutes(5);
    RTC.setSeconds(10);
    RTC.setWeek(5);
    lcd.backlight();
   
}

void loop() {

    if (ArduinoUno.available()) {
        String dateTime = ArduinoUno.readStringUntil('\n');
        Serial.print("Received Date and Time: ");
        Serial.println(dateTime);

        parseDateTime(dateTime);

        // Print parsed values for verification
        Serial.print("Received Year: ");
        Serial.println(receivedYear);
        Serial.print("Received Month: ");
        Serial.println(receivedMonth);
        Serial.print("Received Day: ");
        Serial.println(receivedDay);
        Serial.print("Received Hour: ");
        Serial.println(receivedHour);
        Serial.print("Received Minute: ");
        Serial.println(receivedMinute);

        // Check conditions and perform actions
        checkAndAct();
    }

    // Other operations as needed
    switch (RTC.getWeek()) {
        case 1: displayDayOfWeek("SUN"); break;
        case 2: displayDayOfWeek("MON"); break;
        case 3: displayDayOfWeek("TUE"); break;
        case 4: displayDayOfWeek("WED"); break;
        case 5: displayDayOfWeek("THU"); break;
        case 6: displayDayOfWeek("FRI"); break;
        case 7: displayDayOfWeek("SAT"); break;
    }

    day = RTC.getDay();
    month = RTC.getMonth();
    year = RTC.getYear();
    hour = RTC.getHours();
    minute = RTC.getMinutes();

    displayDateTime();

    if ((receivedYear == year) && (receivedMonth == month) && (receivedDay == day) &&
        (receivedHour == hour) && (receivedMinute == minute)) {
        activateBuzzer();
    }

    displayMeridiem(hour);
}

void parseDateTime(String dateTime) {
    int dateIndex = dateTime.indexOf("Date: ");
    int timeIndex = dateTime.indexOf(" Time: ");

    if (dateIndex != -1 && timeIndex != -1) {
        // Extract and trim date
        String receivedDate = dateTime.substring(dateIndex + 6, timeIndex);
        receivedDate.trim();

        // Extract and trim time
        String receivedTime = dateTime.substring(timeIndex + 7);
        receivedTime.trim();

        // Parse year, month, day from date string
        sscanf(receivedDate.c_str(), "%d-%d-%d", &receivedYear, &receivedMonth, &receivedDay);

        // Handle single-digit day and month (if applicable)
        if (receivedMonth > 0 && receivedMonth < 10 && receivedDate.charAt(6) == '-') {
            receivedMonth = receivedDate.substring(5, 6).toInt();
        }
        if (receivedDay > 0 && receivedDay < 10 && receivedDate.charAt(8) == '-') {
            receivedDay = receivedDate.substring(7, 8).toInt();
        }

        // Parse hour and minute from time string
        sscanf(receivedTime.c_str(), "%d:%d", &receivedHour, &receivedMinute);

        // Handle single-digit hour and minute (if applicable)
        if (receivedHour >= 0 && receivedHour < 10 && receivedTime.charAt(1) == ':') {
            receivedHour = receivedTime.substring(0, 1).toInt();
        }
        if (receivedMinute >= 0 && receivedMinute < 10 && receivedTime.charAt(4) == ':') {
            receivedMinute = receivedTime.substring(3, 4).toInt();
        }
    }
}



void checkAndAct() {
    // Implement your logic here based on receivedYear, receivedMonth, receivedDay, receivedHour, receivedMinute
    // Example: Activate buzzer if time matches
    if ((receivedYear == year) && (receivedMonth == month) && (receivedDay == day) &&
        (receivedHour == hour) && (receivedMinute == minute)) {
        activateBuzzer();
    }
}

void displayDayOfWeek(String day) {
    Serial.print(day);
    lcd.setCursor(13, 1);
    lcd.print(day);
}

void displayDateTime() {
    Serial.print(day);
    Serial.print("-");
    Serial.print(month);
    Serial.print("-");
    Serial.print(year);
    Serial.print(" ");
    Serial.print(hour);
    Serial.print(":");
    Serial.print(minute);
    Serial.print(":");
    Serial.print(RTC.getSeconds());
    Serial.print(" \n");

    lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.print(day);
    lcd.print("-");
    lcd.print(month);
    lcd.print("-");
    lcd.print(year);
    lcd.setCursor(0, 0);
    lcd.print(" ");
    lcd.print(hour);
    lcd.print(":");
    lcd.print(minute);
    lcd.print(":");
    lcd.print(RTC.getSeconds());
    lcd.print(" ");
    lcd.print(" ");
}

void activateBuzzer() {
 
    for (int i = 0; i < 10; i++) {
        digitalWrite(buzzer, HIGH);
        delayMicroseconds(10000);
        digitalWrite(buzzer, LOW);
        delayMicroseconds(10000);
    }
}

void displayMeridiem(int hour) {
    if (hour > 12) {
        lcd.setCursor(11, 0);
        lcd.print("PM");
    } else {
        lcd.setCursor(11, 0);
        lcd.print("AM");
    }
}
