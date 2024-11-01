#include <SoftwareSerial.h>
#include <Keypad.h>
#include <Servo.h>
#define trigPin 8
#define echoPin 9

SoftwareSerial ArduinoUno(7, 6);
int pirPin = 13; 
long duration;
int distance;
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A0, A1, A2, A3}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
/////servo//////////////////////////////////////////////////////////////////////////
Servo myservo;
Servo secservo;
String enteredPassword = ""; 
String orderno= "";

String newPassword = "";    // String to store password
String orderNo1 = "";       // Strings to store orders
String orderNo2 = "";
String orderNo3 = "";
String orderNo4 = "";
String orderNo5 = "";

void setup() {
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(trigPin, OUTPUT);
  // Set the echoPin as an input
  pinMode(echoPin, INPUT);
  myservo.attach(10);
  myservo.write(180);
  secservo.attach(12);
  secservo.write(40);
  delay(1000);
  ArduinoUno.begin(9600);

}
void security(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm\n");

}
 int a=0; 
 int d=0;
void loop() {
  static String inputBuffer = ""; // Buffer to collect incoming data

  while (ArduinoUno.available() > 0) {
    char incomingChar = ArduinoUno.read();

    if (incomingChar == '\n') {
      processInput(inputBuffer); // Process the complete line
      inputBuffer = ""; // Clear the buffer
    } else {
      inputBuffer += incomingChar; // Add character to buffer
    }
  }

  //delay(30);
  security();

  int pirValue = digitalRead(pirPin);
if(pirValue==HIGH && a==0){
    Serial.print("Motion detected!\n");
   }

else if (distance>4){
  int n=1;
  do{
  Serial.print("Security breach detected!\n");
  }while(n=1);
}

char key = keypad.getKey();

if (key){
    a=a+1;
   Serial.println(key);
  enteredPassword+= key;
  orderno+= key; 
}
 
if (key != NO_KEY) {
    if (key == '#') {
     
     if (enteredPassword == newPassword) {
        Serial.print("Password Correct");
        Serial.print("newpassword\n");
    Serial.println(newPassword);
    Serial.print("enteredpassword\n");
    Serial.println(enteredPassword);
        // put your main code here, to run repeatedly:
          // Set volume to maximum (0 to 30).
          myservo.write(0);
          delay(2000);              // tell servo to go to position in variable 'pos'
          if(distance>=0){
            int f=0;
            do{ 
            delay(5000);
            security();
           if(distance<=2){
            myservo.write(180);
            delay(2000);
            f=f+1;
           }
            }while(f==0);
          Serial.print("Parcel delivered with No cash");
         }
        enteredPassword = ""; // Reset entered password
        orderno="";
  }
   else{
    Serial.print("newpassword\n");
    Serial.println(newPassword);
    Serial.print("enteredpassword\n");
    Serial.println(enteredPassword);
    Serial.print("Incorrect Password. Try Again\n");
    enteredPassword = "";
    orderno="";
   } 
    } 
   else if (key == '*'){
    
    if((orderno==orderNo1) || (orderno==orderNo2) || (orderno==orderNo3) || (orderno==orderNo4) || (orderno==orderNo5)==1){
     secservo.write(160);
     delay(2000); 
     security();
     if(distance>=4){
      int n=0;
      do{
     Serial.print("Security breach");
      }while(n==0);
     }
     int e=0;
     do{
      Serial.print("Waiting\n");
      char key = keypad.getKey();
      if (key){
      Serial.println(key);
      }
      if (key != NO_KEY) {
      if (key == '*') {
      secservo.write(40);
      delay(1000); 
      Serial.print("Parcel recieved with Cash");
      orderno="";
      enteredPassword = "";
      key="";
      e=e+1;
      }
      } 
     }while(e==0);
   }
   else{
    Serial.print("Wrong order number\n");
    orderno="";
    enteredPassword = "";
   }
   }
  
}
 
else if((a==1) || (d==1) && (distance==2)==1){
  a=0;
  d=0;
}
}

void processInput(String input) {
  input.trim(); // Trim any leading or trailing whitespace

  if (input.length() > 1) { // Ensure input is valid
    char type = input.charAt(0);
    String val = input.substring(1);
    val.trim(); // Trim any leading or trailing whitespace from the value

    Serial.print("Received: ");
    Serial.print(type);
    Serial.print(val);
    Serial.println();

    switch (type) {
      case 'P':  // New password
        newPassword = val + "#"; // Set new password with '#' appended
        Serial.print("New Password:");
        Serial.println(newPassword);
        break;
      case '1':  // Order No. 1
        orderNo1 = val+ "*"; // Set new order number 1
        Serial.print("Order No. 1:");
        Serial.println(orderNo1);
        break;
      case '2':  // Order No. 2
        orderNo2 = val+ "*"; // Set new order number 2
        Serial.print("Order No. 2:");
        Serial.println(orderNo2);
        break;
      case '3':  // Order No. 3
        orderNo3 = val+ "*"; // Set new order number 3
        Serial.print("Order No. 3:");
        Serial.println(orderNo3);
        break;
      case '4':  // Order No. 4
        orderNo4 = val+ "*"; // Set new order number 4
        Serial.print("Order No. 4:");
        Serial.println(orderNo4);
        break;
      case '5':  // Order No. 5
        orderNo5 = val+ "*"; // Set new order number 5
        Serial.print("Order No. 5:");
        Serial.println(orderNo5);
        break;
      default:
        Serial.println("Unknown type");
        break;
    }
  }
}
