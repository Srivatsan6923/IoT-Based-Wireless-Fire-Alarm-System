//for smoke sensor 
int buzzer = 7;
int smoke_detector = A0;
int safety_lim = 500;

//for DHT11 sensor 
#include "DHT.h"
#define DHTPIN A2
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);


//for flame sensor 
int flame_sensor_pin = 8 ;
int flame_pin = HIGH;

//FOR GSM
#include <SoftwareSerial.h>
 
SoftwareSerial mySerial(10,9);

//for LCD Display
#include "LiquidCrystal.h"
LiquidCrystal lcd(12,11,6,5,4,3);


void setup() {
  // put your setup code here, to run once:
  //for DHT11
  Serial.begin(9600);
  dht.begin();
  
  //for smoke sensor 
  pinMode(smoke_detector,INPUT);
  pinMode(2,OUTPUT);
 
  //for flame sensor 
  pinMode (flame_sensor_pin,INPUT); // declaring sensor pin as input pin for Arduino
  pinMode (2,OUTPUT);

  //for GSM Module
  mySerial.begin(9600); // Setting the baud rate of GSM Module
  Serial.begin(9600); // Setting the baud rate of Serial Monitor (Arduino)

 

  //for LCD Display
  lcd.begin(16,2);
}

void loop() {
  // put your main code here, to run repeatedly:
  //DHT11
  float h = dht.readHumidity();
  float t = dht.readTemperature(); 
  float f = dht.readTemperature(true);
  
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read data from DHT sensor!"));
    return;
  }
  
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  //Temperature and Humidity Serial Monitor Display
  Serial.print(F(" Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("C "));
  Serial.print(f);
  Serial.print(F("F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("C "));
  Serial.print(hif);
  Serial.println(F("F"));
  delay(500);

  //Temperature and Humidity LCD Display  
  

  if(t > 45.0){
    digitalWrite(2,LOW);
    Serial.println("HIGH TEMP ALERT!");
    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("HIGH TEMP ALERT!");
    SendMessage();
  }
  
  else{
    digitalWrite(2,HIGH);
    Serial.println("Temperature Normal");
    lcd.setCursor(0,0);  
    lcd.print("Temp: ");
    lcd.print(t);
    lcd.write(0xdf); //used to display degree symbol on display            
    lcd.print("C ");
    lcd.setCursor(0,1);
    lcd.print("Humidity: ");
    lcd.print(h);
    lcd.print("%");
    delay(500);
  }
 
  //FOR SMOKE SENSOR 
  int sensor_read = analogRead(smoke_detector); 
    //reads and stores the reading from the detector in sensor_read

  Serial.print("Smoke Density: ");
  Serial.println(sensor_read);
  
  if (sensor_read >= safety_lim){ // Checking if reading is beyond safety limit
   digitalWrite(2,LOW);
   Serial.println("Smoke Level Exceeded!!");
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("SmokeDensity:");
   lcd.print(sensor_read);
   lcd.setCursor(0,1);
   lcd.print("SmokeLevelExceed");
   SendMessage();
  }
  
  else {
    digitalWrite(2,HIGH);
    Serial.println("Smoke Level Normal");
  }
  delay(500);


//FOR FLAME SENSOR 
  flame_pin = digitalRead( flame_sensor_pin ) ; // reading from the sensor
if (flame_pin == LOW) // applying condition
 {
  Serial.println("FIRE!! FIRE!! FIRE!!") ;
  digitalWrite(2,LOW);
  lcd.clear();
  lcd.setCursor(1,1);
  lcd.print("Flame!!Flame!!");  
  SendMessage();
 }
 
 else {
  Serial.println ("No Flame") ;
  digitalWrite(2,HIGH);
  
 }
}

void SendMessage()
{
Serial.println("FIRE DETECTED!!SENDING MSG ");
mySerial.println("AT+CMGF=1"); //Sets the GSM Module in Text Mode
delay(1000); // Delay of 1000 milli seconds or 1 second
mySerial.println("AT+CMGS=\"+918925279996\"\r"); // Replace x with mobile number
delay(1000);
mySerial.println("FIRE DETECTED!CALL FIRE SERVICE");// The SMS text you want to send
delay(100);
mySerial.println((char)26);// ASCII code of CTRL+Z
delay(1000);
}



 


 
