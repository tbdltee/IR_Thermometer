void Init_Sensor(void) {
  mysht31.I2CAddress = 0x44;
  mysht31.begin();
  data.state &= 0xF8;
  if (mysht31.ready == 1) SleepSensor();
}

void Sensor_Handling (void) {
  uint8_t state = data.state & 0x07;
  if (state == 0x00) {          // init state
    Init_Sensor();
  } else if (state == 0x02) {   // wakeup request
    if ((data.timer > 0) && ((millis() - data.timer) > 20L)) {    // wakeup complete
      data.timer = 0;
      data.state &= 0xF8; data.state |= 0x03;
    }
  } else if (state == 0x04) {   // sleep request state
    if ((data.timer > 0) && ((millis() - data.timer) > 20L)) {    // sleep complete
      data.timer = 0;
      data.state &= 0xF8; data.state |= 0x01;
      data.T = 0xFFFF;
      drawTempC ();
    }
  }
}

void WakeUpSensor(void) {      // wakeup request
  // wakeup command
  mysht31.reset();
  data.state &= 0xF8; data.state |= 0x02;
  data.timer = millis();
}

void SleepSensor(void) {      // sleep sensor
  // sleep command
  data.state &= 0xE8; data.state |= 0x04;
  data.timer = millis();
}

void readSensorData(void) {
  data.T = 0xFFFF; data.rh = 0xFFFF;
  if ((data.state & 0x07) == 0x03) {
    float TempC = 0, RH = 0;
    mysht31.getData (TempC, RH);
    if (mysht31.ready == 1) {
      if (isnan(TempC)) data.T = 0xFFFF; 
      else data.T = (uint16_t)(TempC * 10);
      if (isnan(RH)) data.rh = 0xFFFF;
      else data.rh = (uint16_t)(RH * 10);
    }
  }
}
