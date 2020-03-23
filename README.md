# ContaclessThermometer
GY-906 Contactless Thermometer using Arduino Pro Mini 3.3v 8MHz

## Release History
* 0001
  * first release with incomplate hardware 

## About the code
The code is for Arduino, consists of 4 main parts:
* main loop:     this is a main code for Arduino Pro-Mini. Upload the code using USB-TTL adapter. The code 
                 is using Arduino deepsleep function and external interrupt from push button to wakup the mcu. 
                 Porting the code to ESP8266/ESP32 must be able to handling this capability.
* push _Button:  this is a code for push button handling. Support Single-Click, Long-Click
* TFT:           this is a code for TFT handling. Support ST7735 SPI TFT.
* Sensor:        this is a code for Sensor handling. Support SHT31d for tempoary and debuging.

## How it works:
* In normal operation, The temperature will continuously measure every 200ms.
* When long-press button abount 1-2 sec, screen start show yellow dot indicate start of measurement.
* If 5 consecutive temperature values are having different less than 0.5 oC, the dot will change 
  to green and buzzer will sound a signle short beep.
* The device will keep measuring as long as the button is pressing. The dot may change between 
  yellow and green according to the last 5 consecutive temperature values.
* If user release the the button, teh average of last 5 consecutive temperature values is displayed.
  If 5 consecutive temperature values are having different more than 0.5 oC, the dot will change 
  to red.
* Long press button again will start measuring and the dot will change to yellow.
* Device will goto deepsleep if no button is press for 10 sec. 

## How to use:
* Device in sleep mode:
  * Single-Click: Wakeup device.
  * Long-Click:   Measure temp
* Device in normal mode:
  * Single-Click: Change profile (Emissivity)
  * Long-Click:   Measure temp 

## Author
Teerapong S. –  [tbdltee@gmail.com]
