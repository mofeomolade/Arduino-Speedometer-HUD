# Arduino GPS Heads-Up Display (HUD) Speedometer

A real-time digital speedometer that safely integrates with a car's electrical system and has a low-latency HUD.

## Project Outline
1. **Hardware Prototype:** Solder the Arduino, GPS, and SPI OLED screen on a breadboard.
2. **Power Calibration:** Adjust the buck converter to step down car voltage to a safe 5V.
3. **Software Implementation:** Code the NMEA parsing, serial communication, and hardware screen inversion.

---

## Hardware Architecture

### Components
- **Microcontroller:** Arduino Nano
- **GPS Module:** GY-NEO6MV2 module with Ceramic Antenna
- **Display:** 128x64 SPI OLED Display
- **Power Regulator:** MP1584EN Buck Converter

### Wiring Overview
* **Power:** Car 12V-14.4V $\rightarrow$ MP1584EN Input $\rightarrow$ Regulated 10V Output $\rightarrow$ to Arduino Vin Pin. Power peripherals using Arduino 5V and 3v3 pins.
* **GPS Data:** GY-NEO6MV2 TX $\rightarrow$ Arduino D2 (SoftwareSerial RX).
* **OLED Display:** Connected via hardware SPI pins (D13 for Clock, D11 for Data) along with CS, DC, and Reset pins.

---

## Software & Signal Processing Plan

* **NMEA Parsing:** Extract raw speed from `$GPRMC` or `$GPVTG` sentences using the TinyGPS++ library. Round these raw speeds to the nearest integer and send to display over SPI.
* **Windshield Reflection Mirroring:** Send hardware commands directly to the OLED controller during setup to invert the image horizontally so it reads correctly when reflected off the glass.
