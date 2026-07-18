# Arduino GPS Heads-Up Display (HUD) Speedometer

I am currently building a real-time digital speedometer. The goal is to create a low-latency, readable HUD that safely integrates with a car's electrical system. 

## Project Status & Plan
I am currently in the building phase. The project layout follows this plan:
1. **Hardware Prototype:** Solder the Arduino, GPS, and SPI OLED screen on a breadboard.
2. **Power Calibration:** Adjust the buck converter to step down car voltage to a safe 5V.
3. **Software Implementation:** Code the NMEA parsing, moving average filter, and hardware screen inversion.
4. **Enclosure Design:** 3D print a heat-resistant dashboard housing and mount the final unit.

---

## Hardware Architecture

### Components
- **Microcontroller:** Arduino Nano
- **GPS Module:** GY-NEO6MV2 module with Ceramic Antenna
- **Display:** 128x64 SPI OLED Display
- **Power Regulator:** MP1584EN Buck Converter

### Wiring Overview
* **Power:** Car 12V-14.4V $\rightarrow$ MP1584EN Input $\rightarrow$ Regulated 5V Output $\rightarrow$ Arduino 5V Pin & Peripherals.
* **GPS Data:** GY-NEO6MV2 TX $\rightarrow$ Arduino D2 (SoftwareSerial RX).
* **OLED Display:** Connected via hardware SPI pins (D13 for Clock, D11 for Data) along with CS, DC, and Reset pins.

---

## Software & Signal Processing Plan

* **NMEA Parsing:** Extract raw speed from `$GPRMC` or `$GPVTG` sentences using the TinyGPS++ library.
* **Data Smoothing:** Pass raw speed data through a 5-sample moving average filter ($Speed_{filtered} = \frac{1}{N} \sum Speed_{raw}$) to eliminate minor GPS jitter without causing lag.
* **Windshield Reflection Mirroring:** Send hardware commands (`0xA0` and `0xC0`) directly to the OLED controller during setup to flip the image horizontally and vertically so it reads correctly when reflected off the glass.

---

## Enclosure Requirements

* **Display Orientation:** Recessed cowl pointing the OLED straight up to avoid nighttime cabin glare.
* **Antenna Cutout:** Top-facing slot ensuring the ceramic GPS antenna has an unshielded view of the sky.
* **Material Selection:** Must be printed in **PETG or ABS** because standard PLA will warp under the intense heat of a summer dashboard.
