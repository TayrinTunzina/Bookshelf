#include <FirebaseESP32.h>
#include <Arduino.h>
#include  <WiFi.h>
#include <Stepper.h>
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>

#define FIREBASE_HOST "fir-bookcase-default-rtdb.firebaseio.com"
#define FIREBASE_Authorization_key "zCyCPtQKzdB2QJeMFpjCtG0xi0V6rPy3ZL5biHvV"
#define WIFI_SSID "Endeavour" // Change the name of your WIFI
#define WIFI_PASSWORD "mathematics" // Change the password of your WIFI

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

/* The sign in credentials */
#define AUTHOR_EMAIL "aimersteam007@gmail.com"
#define AUTHOR_PASSWORD "ywvflikecxyphgre"


// ULN2003 Motor Driver Pins
#define IN1 19
#define IN2 18
#define IN3 5
#define IN4 17

FirebaseData firebaseData;
FirebaseData BookAData;
FirebaseData BookBData;
FirebaseData BookCData;
FirebaseData BookDData;
FirebaseData returnData;
FirebaseJson json;

//------------i2c lcd --------------------------------
int totalColumns = 16;
int totalRows = 2;

LiquidCrystal_I2C lcd(0x27, totalColumns, totalRows);
String scrollingMessage = "Welcome to Smart Book Self! Devloped by Team Aimers";

//-------------Stepper 28byz---------------------------------
const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution
int i; //parameter for controlmotor
// initialize the stepper library
Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);

//---------------Servo-----------------------------------------
Servo myservo; 

//==============END OF HEAD=================

void setup() {
  //pinMode(LED_BUILTIN, OUTPUT);
  
  // Begin Serial communication at a baud rate of 9600:
  Serial.begin(9600);
  
  Serial.print("Connecting...");
  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
 
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  } 
  Serial.println("WiFi connected.");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Firebase.begin(FIREBASE_HOST,FIREBASE_Authorization_key);
  Firebase.reconnectWiFi(true);

  //-------------------Stepper-------------------------------
  // Set the speed to 10 rpm:
  myStepper.setSpeed(10);
  
  
  //--------------------i2c lcd-------------------
  lcd.begin();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Robayet Ahasan");

  scrollMessage(1, scrollingMessage, 500, totalColumns);
  //-------------------Servo----------------
  myservo.attach(23);
  
}

//===========FUNCTION for index;STEPPER==========
  //index 
  void steppercontrol(int i)
  {
   // Step i revolution in one direction:
  Serial.println(i);
  Serial.println(" times clockwise");
  myStepper.step(i*(stepsPerRevolution));
  //TODO loop print every sec  
  delay(5000); 
  Serial.println("5 sec wait End");
   // Step i revolution in the other direction:
  Serial.println(i);
  Serial.println(" times counterclockwise");
  myStepper.step(i*(-stepsPerRevolution));
  Serial.println("left steppercontrol funtion");
  }

//===========FUNCTION for i2c scrollMessage==========
  void scrollMessage(int row, String message, int delayTime, int totalColumns) {
  for (int i=0; i < totalColumns; i++) {
    message = " " + message;  
  } 
  message = message + " "; 
  for (int position = 0; position < message.length(); position++) {
    lcd.setCursor(0, row);
    lcd.print(message.substring(position, position + totalColumns));
    delay(delayTime);
  }
}

//==================SET UP END=====================



void loop() {
  
  float done = 2.0;
  // put your main code here, to run repeatedly:
  if (Firebase.getString(BookAData, "/Bookcase_micro/BookA")){
      Serial.println("A=");  
      Serial.println(BookAData.stringData());
    if (BookAData.stringData() == "0") {
      Serial.println("INDEX ONE");
      steppercontrol(1);  // calling funtion for index one
      
      if (Firebase.setFloat(BookAData, "/Bookcase_micro/BookA", done)) //passing done=2.0 to firebase
          {
            Serial.println("A PASSED");
          }
    }
  }
  delay(100);

  if (Firebase.getString(BookBData, "/Bookcase_micro/BookB")){
      Serial.println("B=");
      Serial.println(BookBData.stringData());
    if (BookBData.stringData() == "0") {
      Serial.println("INDEX TWO");
      steppercontrol(2); // calling funtion for index two
      if (Firebase.setFloat(BookBData, "/Bookcase_micro/BookB", done)) //passing done=2.0 to firebase
          {
            Serial.println("B PASSED");
          }
    }
  }
  delay(100);

  if (Firebase.getString(BookCData, "/Bookcase_micro/BookC")){
      Serial.println("C=");
      Serial.println(BookCData.stringData());
    if (BookCData.stringData() == "0") {
      Serial.println("INDEX THREE");
      steppercontrol(3);  // calling funtion for index three    
      if (Firebase.setFloat(BookCData, "/Bookcase_micro/BookC", done)) //passing done=2.0 to firebase
          {
            Serial.println("C PASSED");
          }
    }
  }
  delay(100);

  if (Firebase.getString(BookDData, "/Bookcase_micro/BookD")){
      Serial.println("D=");
      Serial.println(BookDData.stringData());
    if (BookDData.stringData() == "0") {
      Serial.println("INDEX FOUE");
      steppercontrol(4);  // calling funtion for index four   
      if (Firebase.setFloat(BookDData, "/Bookcase_micro/BookD", done)) //passing done=2.0 to firebase
          {
            Serial.println("D PASSED");
          }
    }
  }
  delay(100);

  if (Firebase.getString(returnData, "/Bookcase_micro/return")){
      Serial.println("return=");
      Serial.println(returnData.stringData());
    if (returnData.stringData() == "0") {
      Serial.println("return box open");
      myservo.write(80);  // door open  
      if (Firebase.setFloat(returnData, "/Bookcase_micro/return", done)) //passing done=2.0 to firebase
          {
            Serial.println("return box closed PASSED");
          }
    }
  }
  delay(100);
  
}
