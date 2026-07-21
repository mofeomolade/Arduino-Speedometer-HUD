#include <Arduino.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

TinyGPSPlus gps;

#define GPS_TX_PIN 2  // Connect to GPS TX
#define GPS_RX_PIN 3  // Connect to GPS RX

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

  // Initialize the OLED display 
  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Stop execution if display fails
  }

  // Invert display for HUD/reflection
  display.ssd1306_command(SSD1306_SEGREMAP | 0x0); // Flips horizontally
  display.ssd1306_command(SSD1306_COMSCANDEC); //

  display.clearDisplay();
  display.display();
}

void loop() {
  // Feed incoming serial data to TinyGPS++
  while (GPSSerial.available() > 0) {
    gps.encode(GPSSerial.read());
  }

  static int last_speed_displayed = -1; // Track the last speed sent to OLED display
  static unsigned long last_serial_print = 0;

  // Only start if the GPS module has a satellite lock
  if (gps.speed.isValid()) {

    // Check if the signal is new
    if (gps.speed.age() < 5000) {

      // Parse new NMEA redings
      if (gps.speed.isUpdated()) {
        double raw_speed = gps.speed.kmph();
        int rounded_speed = round(raw_speed);

        //Print to serial monitor for debugging
        Serial.print(F("Current speed: "));
        Serial.print(raw_speed);
        Serial.println(F(" km/h"));

        // Only send to display if the speed value changed
        if (rounded_speed != last_speed_displayed) {
          
          display.clearDisplay();
          display.setTextColor(SSD1306_WHITE);
          display.setTextSize(7);

          // Adjust horizontal cursor position based on digit count
          if (rounded_speed >= 100) {
            display.setCursor(2, 10);   // 3 digits
          } else if (rounded_speed >= 10) {
            display.setCursor(25, 10);  // 2 digits
          } else {
            display.setCursor(47, 10);  // 1 digit
          }

          display.print(rounded_speed);
          display.display();

          last_speed_displayed = rounded_speed; 
        }
      }
    } 
    else if (millis() - last_serial_print > 5000) {
      // GPS signal lost (>5000 ms since last data received)

      Serial.println(F("ERROR: lost satellite connection.")); //Serial debug message
      
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 20);
      display.print(F("Disconnected"));
      display.display();

      last_speed_displayed = -1; // Reset memory state is signal is lost
      last_serial_print = millis();
    }
  } 
  else if (millis() - last_serial_print > 2500) {
    // Waiting for 
    Serial.println(F("Waiting to establish satellite connection..."));
    
    display.clearDisplay();  
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(5, 23);
    display.print(F("Loading..."));
    display.display();

    last_speed_displayed = -1; // Reset memory state while waiting
    last_serial_print = millis();
  }
}