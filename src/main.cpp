#include <Arduino.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

TinyGPSPlus gps;

#define GPS_TX_PIN 2  // Connect this to the GPS TX pin
#define GPS_RX_PIN 3  // Connect this to the GPS RX pin

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT  64

#define OLED_DC 6
#define OLED_CS 10
#define OLED_RESET 5
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_DC, OLED_RESET, OLED_CS);

SoftwareSerial GPSSerial(GPS_TX_PIN, GPS_RX_PIN);

void setup() {
  Serial.begin(57600);

  GPSSerial.begin(9600);

  //Invert display for reflection
  display.ssd1306_command(SSD1306_SEGREMAP | 0x0); // Flips left-to-right
  display.ssd1306_command(SSD1306_COMSCANDEC);

  display.clearDisplay();
}

void loop() {

  while (GPSSerial.available() > 0){
    gps.encode(GPSSerial.read());
  }

  static int last_speed_displayed = -1; //Track the last speed sent to OLED display
  static unsigned long last_serial_print = 0;

  //Only start if the GPS module has already calculated a valid speed
  if(gps.speed.isValid()){

    //Only update speed if last satellite reading was within the past 5 seconds
    if(gps.speed.age() < 5000){

      //Check that new speed reading is available
      if(gps.speed.isUpdated()){
        double raw_speed = gps.speed.kmph();
        Serial.print(F("Current speed: "));
        Serial.print(raw_speed);
        Serial.println(F(" km/h"));

        int rounded_speed = round(raw_speed);
        
        display.clearDisplay();
        display.setTextColor(SSD1306_WHITE);

        if(rounded_speed >= 100 && rounded_speed != last_speed_displayed){
          
          //Text calibration for 3 digit speeds
          display.setTextSize(7);
          display.setCursor(2, 10);
          
          //Print and activate display
          display.print(rounded_speed);
          display.display();
        }
        else if(rounded_speed >= 10 && rounded_speed != last_speed_displayed){
          
          //Text calibration for 2 digit speeds
          display.setTextSize(7);
          display.setCursor(20, 10);

          //Print and activate display
          display.print(rounded_speed);
          display.display();
        }
        else if (rounded_speed != last_speed_displayed){
          //Text calibration for 1 digit speeds
          display.setTextSize(7);
          display.setCursor(47, 10);

          //Print and activate display
          display.print(rounded_speed);
          display.display();


        }
      }
    }

    else if (millis() - last_serial_print > 2500){
        Serial.println(F("ERROR: lost satellite connection."));
        
        display.clearDisplay();

        display.setTextSize(4);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);

        //Print and activate display
        display.print("Disconnected");
        display.display();

        last_serial_print = millis();
    }
  }

  else if (millis() - last_serial_print > 2500){

    Serial.println(F("Waiting to establish satellite connection..."));
    last_serial_print = millis();
    
    //Calibrate display for error message
    display.clearDisplay();  
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(5, 23);

    //Print and activate display
    display.print(F("Loading..."));
    display.display();
  }
}