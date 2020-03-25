//#define BUZZER_PIN              5   // buzzer pin, active HIGH

#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Adafruit_SHT31.h"
Adafruit_SHT31 mysht31 = Adafruit_SHT31();

struct Sensor_type {
  uint8_t   state   = 0x00;       // 0Babcd 0zzz  a:keyDn, b:start measure, c: measure complete, d:E change allow, z:sensor state (0:init/1:sleep/2:wakeup/3:ready/4:sleep req
  uint16_t  T       = 0xFFFF;     // tempC x10
  uint16_t  rh      = 0xFFFF;     // rh x10
  uint16_t avgT[5]  = {0,0,0,0,0};
  uint16_t avgTval  = 0;
  uint8_t  avgTidx  = 0x00;       // avgT index
  uint32_t timer    = 0;          // general sensor timer
} data;

struct sys_type {
  uint8_t Battlvl = 0;            // % battery. 0:<3v, 1:3.0..3.1v, 2:3.1..3.2, 3:3.2v
  uint8_t E[2] = {1,2};
  uint8_t Eidx = 0;
  uint32_t readSensorTime = 0;
  uint32_t battTimer = 0;
  uint32_t tftTimer   = 0;          // display timer
} sys;

volatile uint8_t state = 0x00;

void setup() {
  Serial.begin (57600);
#ifdef BUZZER_PIN
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
#endif
  init_Button ();
  init_TFT (); drawMainScreen(); drawEvalue();
  Serial.println ("[SYSTEM] Init ok. E="+String(sys.E[sys.Eidx]));
  Init_Sensor();
  getBatt(); drawBattvalue();
  if ((data.state & 0x07) == 0x01) Serial.println ("[Sensor] Init ok");
  else Serial.println ("[Sensor] Init error");
  sys.readSensorTime = millis();
  sys.battTimer = millis();
  sys.tftTimer = millis();
}

void loop() {
  scanButton();
  Sensor_Handling();
  if ((millis() - sys.battTimer) > 60000L) {
    getBatt(); drawBattvalue();
    sys.battTimer = millis();
  }
  if ((millis() - sys.readSensorTime) > 500L) {
    readSensorData();
    drawTempC ();
    sys.readSensorTime = millis();
    if (data.state & 0x80) {                // long Press
      if ((data.state & 0x40) == 0x00) {    // start measuring
        ClrdrawMeasureTempC ();
        data.avgTidx &= 0xF8;               // reset avgTidx
        data.state |= 0x40;                 // set measuring flag
        for (uint8_t i=0; i<5; i++) data.avgT[i] = 0;
      }
      data.avgT[data.avgTidx & 0x07] = data.T;
      if (avgTempC()) {                     // result OK
        drawMeasureTempC (data.T, 1);
        if ((data.state & 0x20) == 0) {     // 1st time measurement complete
#ifdef BUZZER_PIN
            analogWrite(BUZZER_PIN, 220); delay (100); digitalWrite(BUZZER_PIN, LOW);
#endif
        }
        data.state |= 0x20;                 // measurement completed
      } else {                              // result not OK
        drawMeasureTempC (data.T, 2);
        data.state &= 0xDF;                 // clear measurement completed flag
      }
      data.avgTidx++;
      if ((data.avgTidx & 0x07) > 0x04) data.avgTidx &= 0xF8;
    } else {                                // key Up
      if (data.state & 0x40) {
        getBatt(); drawBattvalue();
        sys.battTimer = millis();
        data.state &= 0xBF;
        drawMeasureTempC (data.avgTval, ((data.state & 0x20) == 0)?0:1);
      }
    }
  }
  if ((sys.tftTimer > 0) && ((millis() - sys.tftTimer) > 10000L)) {
    sys.tftTimer = 0;
    sleep();
  }
  if ((data.state & 0x60) == 0x40) {                // measuring state
    if ((millis() - sys.readSensorTime) > 200L) {   // clear measuring text
      ClrdrawMeasureTempC();
    }
  }
}

void sleep(void) {
  SleepSensor();
  turnoff_TFT();
  state = 0x00;
  delay (10);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  ADCSRA &= ~(1 << ADEN);
  power_all_disable();
  while ((state & 0x01) == 0x00) {      // while in sleep stage
    sleep_mode();                       // MCU is now sleep and execution completely halt! execution will resume at this point
  }
  sleep_disable();                      // When awake, disable sleep mode
  power_all_enable();                   // put everything on again
  ADCSRA |= (1 << ADEN);                // startup ADC
  delay (10);                           // ensure everything is power up
  turnon_TFT();
}
