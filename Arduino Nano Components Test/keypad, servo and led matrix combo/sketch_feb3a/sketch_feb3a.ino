
#include <Keypad.h>
#include <Servo.h>

#define ROW_1 2
#define ROW_2 3
#define  ROW_3 4
#define ROW_4 5
#define ROW_5 6
#define ROW_6 7
#define ROW_7  8
#define ROW_8 9

#define COL_1 10
#define COL_2 11
#define COL_3  12
#define COL_4 13
#define COL_5 A0
#define COL_6 A1
#define COL_7  A2
#define COL_8 A3

const byte rows[] = {
    ROW_1, ROW_2, ROW_3,  ROW_4, ROW_5, ROW_6, ROW_7, ROW_8
};
const byte col[] = {
  COL_1,COL_2,  COL_3, COL_4, COL_5, COL_6, COL_7, COL_8
};
byte  X[] = {B11111111,B10111101,B11011011,B11100111,B11100111,B11011011,B10111101,B11111111};
byte  Y[] = {B00000000,B01000100,B00101000,B00010000,B00010000,B00010000,B00010000,B00000000};
byte  Z[] = {B00000000,B00111100,B00000100,B00001000,B00010000,B00100000,B00111100,B00000000};
byte check[] = {B01111111,B10111111,B11011111,B11101101,B11110101,B11111001,B11111101,B11111111};
float  timeCount = 0;


const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

Servo myservo;

String password = "1234"; // Set your password here
String enteredPassword = ""; 

void setup(){
  Serial.begin(9600);
  myservo.attach(12);
  for (byte  i = 2; i <= 13; i++)
        pinMode(i, OUTPUT);
    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
}
}
  
void loop(){
  myservo.write(0);
  delay(100);
  for(int i=5;0<5;i++){
drawScreen(check);
delay(100);
}
  char key = keypad.getKey();
  int pos;
  if (key){
    Serial.println(key);
  }

  if (key != NO_KEY) {
    if (key != '#') {
      enteredPassword += key;
    } else {
      if (enteredPassword == password) {
        Serial.println("Password Correct");
        // Move servo to unlock position
        // goes from 0 degrees to 90 degrees
          myservo.write(180);              // tell servo to go to position in variable 'pos'
          delay(500);                       // waits 15ms for the servo to reach the position
        
        //delay(1000); // Delay for 1 second
        
        enteredPassword = ""; // Reset entered password
      } else {
        Serial.println("Incorrect Password");
        enteredPassword = ""; // Reset entered password
      }
    }
  }
  
}

void  drawScreen(byte buffer2[])
 { 
   // Turn on each  row in series
    for (byte i = 0; i < 8; i++)        // count next row
     {
        digitalWrite(rows[i], HIGH);    //initiate whole row
        for (byte  a = 0; a < 8; a++)    // count next row
        {
          // if You set  (~buffer2[i] >> a) then You will have positive
          digitalWrite(col[a],  (buffer2[i] >> a) & 0x01); // initiate whole column
          
          delayMicroseconds(100);       // uncoment deley for diferent speed of display
          //delayMicroseconds(1000);
          //delay(10);
          //delay(100);
          
          digitalWrite(col[a],  1);      // reset whole column
        }
        digitalWrite(rows[i], LOW);     // reset whole row
        // otherwise last row will intersect with next  row
    }
}