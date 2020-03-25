// ==================== Button definitions ====================

const uint8_t buttonPin       = 2;          // button1 - pin 2 (INT0)
const uint32_t debounceDelay  = 50;         // the debounce time 50ms; increase if the output flickers
const uint32_t longPressDelay = 1000;       // long press 1.5 sec.
const uint32_t ClickDelay     = 500;        // next press within 500ms count as double/triple click.

struct buttonType {                         // Data Structure of Key Button
  uint8_t  ButtonState        = 0x30;       // 0b0cba xxxx, xxxx=Keyup counter, a=prevDebounceKeyState (HIGH), b=prevKeyState (HIGH), c=ignore Click
  uint32_t prevKeyTimer       = 0;          // Lastest of millis() since keyUp/keyDown
  uint32_t prevDebounceTime   = 0;          // Lastest of millis() of debounce timer
} btn;

void init_Button() {
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), keyDnISR, FALLING);
}

void keyDnISR (void) {
  state |= 0x01;                            // set wake up flag
}

void Button_multiClick(uint8_t clickCount) {
  if (clickCount == 1) {
    if ((data.state & 0x17) == 0x13) {
      sys.Eidx++;
      sys.Eidx %= (sizeof(sys.E));
      drawEvalue();
      Serial.println ("new E="+String(sys.E[sys.Eidx]));
    } else if ((data.state & 0x17) == 0x03) {
      data.state |= 0x10;
    }
  }
}

void Button_longPress(uint8_t clickCount) {
  if (clickCount == 1) data.state |= 0x90;
}

void Button_keyDown(void) {
  if ((data.state & 0x07) == 0x01) WakeUpSensor();
}

void Button_keyUp(void) {
  data.state &= 0x7F;
}

void scanButton(void) {
  uint8_t reading, i;
  reading = (digitalRead(buttonPin) == LOW)? 0x00: 0x10;           // read the state of the switch into a local variable:
  if (reading == 0x00) sys.tftTimer = millis();
  if (reading != (btn.ButtonState&0x10)) btn.prevDebounceTime = millis();   // reset the debouncing timer if state change
  btn.ButtonState &= 0xEF; btn.ButtonState |= reading;
  if ((millis() - btn.prevDebounceTime) > debounceDelay) {            // if state change is stable (key press more than dbounceDelay time
    if (btn.ButtonState & 0x20) {                                     // prevState == HIGH
      if (reading & 0x10) {                                           // current state == HIGH, idle stat
        if (((millis() - btn.prevKeyTimer) > ClickDelay) && (btn.ButtonState & 0x0F)) {          // Click event
          if ((btn.ButtonState&0x40)==0) Button_multiClick(btn.ButtonState & 0x0F);
          btn.ButtonState &= 0xB0;                                    // reset click counter & ignore click
        }
      } else {                                                        // current state == LOW, keyDown state
        Button_keyDown();
        btn.prevKeyTimer = millis();                                  // start keydown timeer
      }
    } else {                                                          // prevState == LOW
      if (reading & 0x10) {                                           // current state == HIGH, keyUp state
        Button_keyUp();
        btn.prevKeyTimer = millis();                               // start keyUp timeer
        if ((btn.ButtonState & 0x0F) < 0x0F) btn.ButtonState++; // increase keyUp counter
      } else {                                                        // current state == LOW, keyPress state
        if ((millis() - btn.prevKeyTimer) > longPressDelay) {      // longPress event
          if (btn.ButtonState & 0x40) {                            // increase longPress counter
            if ((btn.ButtonState & 0x0F) < 0x0F) btn.ButtonState++;
          } else {
            btn.ButtonState &= 0x30;
            btn.ButtonState |= 0x41;                               // reset longPress counter, set ignore click
          }
          Button_longPress(btn.ButtonState & 0x0F);
          btn.prevKeyTimer = millis();
        }
      }
    }
    btn.ButtonState &= 0xDF; btn.ButtonState |= (reading<<1);
  }
}
