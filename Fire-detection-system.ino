#include <SoftwareSerial.h>  // library for GSM module

SoftwareSerial mySerial(9, 10);  // connected pins are : rx = 10, tx = 9

const int white = 2;
const int red = 4;
const int green = 3;
const int buzzer = 11;

// configuration for smoke sensor
const int smoke = A0;
const int smoke_threshold = 500; // sets threshold value for smoke sensor
float smokeValue; // initialize smoke sensor reading

// configuration for flame sensor
const int flame = A2;
const int flamethreshold = 200; // sets threshold value for flame sensor
int flamesensvalue = 0; // initialize flamesensor reading

void setup() {
  Serial.begin(9600); // sets the serial port to 9600
  Serial.println("Gas sensor warming up!");
  delay(20000); // allow the MQ-2 smoke sensor to warm up

  // defining pin modes
  pinMode(white, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(flame, INPUT);
  pinMode(smoke, INPUT);

  // setting connection with GSM module
  mySerial.begin(9600);   // Setting the baud rate of GSM Module
  delay(100);
}

void loop() {

  // Reading smoke sensor values and displaying in serial monitor
  smokeValue = analogRead(smoke); // reads analog data from smoke sensor
  Serial.println("Gas sensor value : ");
  Serial.println(smokeValue); // print smoke value
  Serial.println("  ");

  // Reading flame sensor values and displaying in serial monitor
  flamesensvalue = analogRead(flame); // reads analog data from flame sensor
  Serial.println("Flame sensor value : ");
  Serial.println(flamesensvalue); // print flame value

  Serial.println("  ");
  Serial.println("  ");
  delay(2000);

  // logic to detect fire
  if (flamesensvalue <= flamethreshold ||  smokeValue >= smoke_threshold)
  { // if fire is detected following commands will run
    digitalWrite(white, HIGH);
    digitalWrite(buzzer, HIGH);
    tone(buzzer, 200); // buzzer start
    digitalWrite(red, HIGH); //turns on led led
    digitalWrite(green, LOW);

    Serial.println("!!!  Fire Detected  !!!");
    delay(1000); //stops program for 1 second
    GSM_message(); // function to send message
    updateSerial();
    delay(10000);
    GSM_call(); // function to call for fire alert
  }
  else {
    digitalWrite(white, HIGH);
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);
    noTone(buzzer);
  }
}

// fuction to send message if fire detected
void GSM_message() {
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  updateSerial();  // Delay of 1 second
  mySerial.println("AT+CMGS=\"+918261961807\"\r");
  updateSerial();
  mySerial.println("ATTENTION !!!! Fire Detected, Urgent action needed !!!");// The SMS text want to send
  updateSerial();
  mySerial.println((char)26);// ASCII code of CTRL+Z for saying the end of sms to  the module
  updateSerial();
}

// fuction to call if fire detected
void GSM_call() {
  mySerial.println("ATD+ +918261961807;"); //  set phone number to call
  updateSerial();
  delay(20000); // wait for 20 seconds...
  mySerial.println("ATH"); //hang up
  updateSerial();
}

void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (mySerial.available())
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
