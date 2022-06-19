#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define  FIREBASE_HOST "chambre-d-hopital-db-default-rtdb.europe-west1.firebasedatabase.app"
#define  FIREBASE_AUTH "NilDrYsTXu7161XZHSrGa7Kgnig9jcqE7T97A4hC" 
#define DHTTYPE DHT11
#define DHTPIN 2  //--> The pin used for the DHT11 sensor is Pin D4
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht (DHTPIN,DHTTYPE);

//----------------------------------------SSID and Password of WiFi router.
char  ssid[] = "DESKTOP-E9U54LN 7914"; 
char  pass[] = "123456789";
//----------------------------------------

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(500);

  dht.begin();  //--> Start reading DHT11 sensor
  delay(500);
  
  lcd.begin();//--> Start reading lcd display
  lcd.backlight();
  delay(500);
  
  WiFi.begin(ssid,pass); //-->wifi name & password

  //----------------------------------------Wait for connection
  Serial.print("Connecting");
  
  //----------------------------------------If successfully connected to the wifi router, the IP Address that will be visited is displayed in the serial monitor
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //----------------------------------------

  //----------------------------------------Firebase Realtime Database Configuration.
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH); 
  //----------------------------------------
}


void loop() {
  // put your main code here, to run repeatedly:
  
  //----------------------------------------Reading Temperature and Humidity
  float t = dht.readTemperature(); //--> Read temperature as Celsius. 
  
  // Check if any reads failed and exit early (to try again).
  
  if (isnan(t)) {
    Serial.println(" Failed to load DHT sensor !");
    delay(1000);
    return;
  }

  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));

  lcd.setCursor(0,0);
  lcd.print("Temp:");
  lcd.print(t);
  lcd.print("C");
  //----------------------------------------Send Temperature data to the Firebase Realtime Database.
  //float t=5.2;
  //Serial.print(t);
  Firebase.setFloat("temp",t);
  
  //----------------------------------------
  delay(2000);
  // Conditions for handling errors.
  if (Firebase.failed()) { 
      Serial.print("setting Temperature failed :");
      Serial.println(Firebase.error());  
      delay(500);
  else Serial.print("setting Temperature seuccessful !:");
  }
  delay(500);
}
