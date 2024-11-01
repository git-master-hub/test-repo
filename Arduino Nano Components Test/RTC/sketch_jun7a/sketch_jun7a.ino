#include <Wire.h>
#include <I2C_RTC.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

static DS3231 RTC;
LiquidCrystal_I2C lcd(0x27, A4, A5); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7)
SoftwareSerial ArduinoUno(2, 3);
int buzzer =10;
int receivedYear = 0;
int receivedMonth = 0;
int receivedDay = 0;
int receivedHour = 0;
int receivedMinute = 0;
int year,day,month,hour,minute;
void setup() {
Serial.begin(9600);
ArduinoUno.begin(9600);
    //Buzzer
    pinMode(10, OUTPUT);
    Serial.begin(9600);
    RTC.begin();
    lcd.begin(16, 2);

    //RTC.setHourMode(CLOCK_H12);
    RTC.setHourMode(CLOCK_H24);
    //if (RTC.getHourMode() == CLOCK_H12) {
        //RTC.setMeridiem(HOUR_PM);
    //}
    RTC.setDay(10);
    RTC.setMonth(2);
    RTC.setYear(2024);
    RTC.setHours(1);
    RTC.setMinutes(59);
    RTC.setSeconds(56);
    RTC.setWeek(7);
    //RTC.setDate(24,10,2);
    //RTC.setTime(24,59,56);
}

void loop() {
   
       // digitalWrite(buzzer, HIGH);
      //  delayMicroseconds(2000);
       // digitalWrite(buzzer, LOW);
       // delayMicroseconds(2000);
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
    }
    switch (RTC.getWeek()) {
        case 1:
            Serial.print("SUN");
            lcd.setCursor(10, 1);
            lcd.print("SUN");
            break;
        case 2:
            Serial.print("MON");
            lcd.setCursor(10, 1);
            lcd.print("MON");
            break;
        case 3:
            Serial.print("TUE");
            lcd.setCursor(10, 1);
            lcd.print("TUE");
            break;
        case 4:
            Serial.print("WED");
            lcd.setCursor(10, 1);
            lcd.print("WED");
            break;
        case 5:
            Serial.print("THU");
            lcd.setCursor(10, 1);
            lcd.print("THU");
            break;
        case 6:
            Serial.print("FRI");
            lcd.setCursor(10, 1);
            lcd.print("FRI");
            break;
        case 7:
            Serial.print("SAT");
            lcd.setCursor(12, 1);
            lcd.print("SAT");
            break;
    }
    Serial.print(" ");
    day= RTC.getDay();
    Serial.print(RTC.getDay());
    Serial.print("-");
    month = RTC.getMonth();
    Serial.print(RTC.getMonth());
    Serial.print("-");
    year = RTC.getYear();
    Serial.print(RTC.getYear());
    Serial.print(" ");
    hour = RTC.getHours();
    Serial.print(RTC.getHours());
    Serial.print(":");
    minute = RTC.getMinutes();
    Serial.print(RTC.getMinutes());
    Serial.print(":");
    Serial.print(RTC.getSeconds());
    Serial.print(" \n");

    lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.print(RTC.getDay());
    lcd.print("-");
    lcd.print(RTC.getMonth());
    lcd.print("-");
    lcd.print(RTC.getYear());
    lcd.setCursor(0, 0);
    lcd.print(" ");
    lcd.print(RTC.getHours());
    lcd.print(":");
    lcd.print(RTC.getMinutes());
    lcd.print(":");
    lcd.print(RTC.getSeconds());
    lcd.print(" ");
if((receivedYear==year) && (receivedMonth==month) && (receivedDay==day) && (receivedHour==hour) && (receivedMinute==minute)==1){
      digitalWrite(buzzer, HIGH);
       delayMicroseconds(2000);
       digitalWrite(buzzer, LOW);
       delayMicroseconds(2000);
       digitalWrite(buzzer, HIGH);
       delayMicroseconds(2000);
       digitalWrite(buzzer, LOW);
       delayMicroseconds(2000);
       digitalWrite(buzzer, HIGH);
       delayMicroseconds(2000);
       digitalWrite(buzzer, LOW);
       delayMicroseconds(2000);


}

    
}
void parseDateTime(String dateTime) {
    int dateIndex = dateTime.indexOf("Date: ");
    int timeIndex = dateTime.indexOf(" Time: ");
    
    if (dateIndex != -1 && timeIndex != -1) {
        // Extract date
        String receivedDate = dateTime.substring(dateIndex + 6, timeIndex);
        receivedDate.trim(); // Remove any leading/trailing spaces
        
        // Extract time
        String receivedTime = dateTime.substring(timeIndex + 7);
        receivedTime.trim(); // Remove any leading/trailing spaces
        receivedYear="";
        receivedMonth="";
        receivedDay="";
        receivedHour="";
        receivedMinute="";
        // Convert date and time strings to integers after trimming
        receivedYear = receivedDate.substring(0, 4).toInt();
        receivedMonth = receivedDate.substring(5, 7).toInt();
        receivedDay = receivedDate.substring(8, 10).toInt();
        receivedHour = receivedTime.substring(0, 2).toInt();
        receivedMinute = receivedTime.substring(3, 5).toInt();
    }
}
