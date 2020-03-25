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

bool avgTempC (void) {
  uint16_t minVal = data.avgT[0], maxVal = 0;
  data.avgTval = 0;
  for (uint8_t i=0; i<5; i++) {
    data.avgTval += data.avgT[i];
    if (data.avgT[i] < minVal) minVal = data.avgT[i];
    if (data.avgT[i] > maxVal) maxVal = data.avgT[i];
  }
  data.avgTval /= 5;
  if ((maxVal - minVal) < 3) return true;
  return false;
}

// ============================= Batterry Function ===============================
void getBatt(void) {
  uint16_t A11Value = readADC11();                                        // read ADC @1.1v reference  
  uint16_t mVcc = map(1023, 0, A11Value, 0, 1100);                        // calculate Vcc
  
  if (mVcc > 3200) sys.Battlvl = 3;
  else if (mVcc > 3100) sys.Battlvl = 2;
  else if (mVcc > 3000) sys.Battlvl = 1;
  else sys.Battlvl = 0;
}

uint16_t readADC11() {              // set ADC ref. to Vcc and read ADC of internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  

  delay(2);                         // Wait for Vref to settle
  ADCSRA |= _BV(ADSC);              // Start conversion
  while (bit_is_set(ADCSRA,ADSC));  // measuring

  uint8_t low  = ADCL;              // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH;              // unlocks both
  uint16_t result = (high<<8) | low;
  return result;
}
