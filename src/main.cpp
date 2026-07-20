#include <Arduino.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

TinyGPSPlus gps;

#define GPS_TX_PIN 2  // Connect this to the GPS TX pin
#define GPS_RX_PIN 3  // Connect this to the GPS RX pin

SoftwareSerial GPSSerial(GPS_RX_PIN, GPS_TX_PIN);

void setup() {
  Serial.begin(57600);

  GPSSerial.begin(9600);
}

void loop() {

  while (GPSSerial.available() > 0){
    gps.encode(GPSSerial.read());
  }

  static unsigned long last_serial_print = 0;

  //Only start if the GPS module has already calculated a valid speed
  if(gps.speed.isValid()){

    //Only update speed if last satellite reading was within the past 5 seconds
    if(gps.speed.age() < 5000){

      //Check that new speed reading is available
      if(gps.speed.isUpdated()){
        double raw_speed = gps.speed.kmph();
        Serial.print("Current speed: ");
        Serial.print(raw_speed);
        Serial.println(" km/h");
      }
    }

    else if (millis() - last_serial_print > 5000){
        Serial.println("ERROR: lost satellite connection.");
        last_serial_print = millis();
    }
  }

  else if (millis() - last_serial_print > 2500){
      Serial.println("Waiting to establish satellite connection...");
      last_serial_print = millis();
  }

}