// ==================== pin definitions ====================
#define TFT_CS            10          // CS,       Chip Select
#define TFT_DC            8           // DC or A0, Data/Command
#define TFT_RST           9           // RST,      Reset
#define TFT_LED           A3          // LED,      Brightness

#define TEMPC_XPOS        76
#define TEMPC_YPOS        50
#define MTEMPC_XPOS       24
#define MTEMPC_YPOS       24
#define SCAN_XPOS         112
#define SCAN_YPOS         28

#define E_XPOS            5
#define E_YPOS            50
#define BATT_XPOS         102
#define BATT_YPOS         4

// ==================== TFT definitions ====================
TFT_ST7735 TFTdisp = TFT_ST7735 (TFT_CS, TFT_DC, TFT_RST);
uint8_t TFT_row = 0;

/* pin definition for the Uno
 * TFT --> Arduino
 * LED --> 3v3 
 * SCK --> CSK (13)
 * SDA --> MOSI (11)
 * A0   --> DC (9)
 * RST --> RST (8)
 * CS   --> CS (10)
 * GND --> GND
 * Vcc --> 3v3
 */
 
void init_TFT (void) {
  TFTdisp.begin();
  TFTdisp.useBacklight(TFT_LED);
  TFTdisp.setRotation(3);
  TFTdisp.setTextScale(1);                                  // set the font scaling to 1
  TFTdisp.clearScreen ();                                   // clear all screen
  TFTdisp.backlight (false);                                // turn-on LED backlight
  TFTdisp.setTextColor(WHITE);                              // set the font color to WHITE
  TFTdisp.drawRect (0, 0, 128, 64, WHITE);
}

void drawMainScreen (void) {
  TFTdisp.setTextScale(2);
  TFTdisp.setCursor (MTEMPC_XPOS+66, MTEMPC_YPOS); TFTdisp.print ("C");          // hold tempC
  TFTdisp.setCursor (MTEMPC_XPOS+12, MTEMPC_YPOS); TFTdisp.print ("--");
  TFTdisp.setCursor (MTEMPC_XPOS+44, MTEMPC_YPOS); TFTdisp.print ("-");
  TFTdisp.fillRect (MTEMPC_XPOS+37, MTEMPC_YPOS+7, 3, 3, WHITE);
  TFTdisp.setTextScale(1);
  TFTdisp.setCursor (MTEMPC_XPOS+58, MTEMPC_YPOS); TFTdisp.print ("o");
  
  TFTdisp.setCursor (TEMPC_XPOS+38, TEMPC_YPOS);     TFTdisp.print ("C");         // tempC
  TFTdisp.setCursor (TEMPC_XPOS+32, TEMPC_YPOS-3); TFTdisp.print ("o");
  TFTdisp.setCursor (TEMPC_XPOS+6, TEMPC_YPOS);          TFTdisp.print ("--");
  TFTdisp.setCursor (TEMPC_XPOS+19, TEMPC_YPOS);     TFTdisp.print (".");
  TFTdisp.setCursor (TEMPC_XPOS+22, TEMPC_YPOS);     TFTdisp.print ("-");
  
  TFTdisp.setCursor (E_XPOS, E_YPOS); TFTdisp.print ("E:");
  TFTdisp.setCursor (BATT_XPOS, BATT_YPOS); TFTdisp.print ("B");
  TFTdisp.drawRect (BATT_XPOS+8, BATT_YPOS+1, 10, 6, WHITE);
  TFTdisp.drawRect (BATT_XPOS+18, BATT_YPOS+3, 2, 2, WHITE);
}

void drawBattvalue (void) {
  TFTdisp.fillRect (BATT_XPOS+9, BATT_YPOS+2, 8, 4, BLACK);
  if (sys.Battlvl == 3) {
    TFTdisp.fillRect (BATT_XPOS+9, BATT_YPOS+2, 8, 4, WHITE);
  } else if (sys.Battlvl == 2) {
    TFTdisp.fillRect (BATT_XPOS+9, BATT_YPOS+2, 6, 4, WHITE);
  } else if (sys.Battlvl == 1) {
    TFTdisp.fillRect (BATT_XPOS+9, BATT_YPOS+2, 4, 4, WHITE);
  } else {
    TFTdisp.fillRect (BATT_XPOS+9, BATT_YPOS+2, 2, 4, WHITE);
  }
}

void drawEvalue (void) {
  String txt = "";
  if (sys.Eidx == 0) {
    txt = "body";
  } else if (sys.Eidx == 1) {
    txt = "water/food";
  }
  TFTdisp.fillRect (E_XPOS+12, E_YPOS-1, 56, 9, BLACK);
  TFTdisp.setCursor (E_XPOS+12, E_YPOS); TFTdisp.print (txt);
}

void drawTempC (void) {
  if (data.T == 0xFFFF) return;
  String txt1 = "   " + String(data.T/10);
         txt1 = txt1.substring(txt1.length() - 3);
  String txt2 = String(data.T%10);
  for (uint8_t i=0; i<3; i++) {
    if (disp.tempC1[i] != txt1.charAt(i)) {
      disp.tempC1[i] = txt1.charAt(i);
      TFTdisp.fillRect (TEMPC_XPOS+(i*6), TEMPC_YPOS-1, 6, 9, BLACK);
      TFTdisp.setCursor (TEMPC_XPOS+(i*6), TEMPC_YPOS);
      TFTdisp.print (disp.tempC1[i]);
    }
  }
  if (disp.tempC1[3] != txt2.charAt(0)) {
    disp.tempC1[3] = txt2.charAt(0);
    TFTdisp.fillRect (TEMPC_XPOS+22, TEMPC_YPOS-1, 6, 9, BLACK);
    TFTdisp.setCursor (TEMPC_XPOS+22, TEMPC_YPOS);
    TFTdisp.print (disp.tempC1[3]);
  }
}

void ClrdrawMeasureTempC (void) {
  strcpy(disp.tempC2, " ---");
  TFTdisp.fillRect (MTEMPC_XPOS-1, MTEMPC_YPOS-1, 56, 15, BLACK);
  TFTdisp.fillRect (MTEMPC_XPOS+37, MTEMPC_YPOS+10, 3, 3, WHITE);
  TFTdisp.fillCircle (SCAN_XPOS, SCAN_YPOS, 5, BLACK);
}

void drawMeasureTempC (uint16_t tempC, uint16_t color) {
  TFTdisp.setTextScale(2);
  String txt1 = "   " + String(tempC/10);
  txt1 = txt1.substring(txt1.length() - 3);
  String txt2 = String(tempC%10);
  
  for (uint8_t i=0; i<3; i++) {
    if (disp.tempC2[i] != txt1.charAt(i)) {
      disp.tempC2[i] = txt1.charAt(i);
      TFTdisp.fillRect (MTEMPC_XPOS+(i*12)-1, MTEMPC_YPOS-1, 12, 15, BLACK);
      TFTdisp.setCursor (MTEMPC_XPOS+(i*12), MTEMPC_YPOS);
      TFTdisp.print (disp.tempC1[i]);
    }
  }
  if (disp.tempC2[3] != txt2.charAt(0)) {
    disp.tempC2[3] = txt2.charAt(0);
    TFTdisp.fillRect (MTEMPC_XPOS+43, MTEMPC_YPOS-1, 12, 15, BLACK);
    TFTdisp.setCursor (MTEMPC_XPOS+44, MTEMPC_YPOS);
    TFTdisp.print (disp.tempC2[3]);
  }
  TFTdisp.fillCircle (SCAN_XPOS, SCAN_YPOS, 5, color);
  TFTdisp.setTextScale(1);
}

void turnoff_TFT (void) { TFTdisp.backlight (true); }
void turnon_TFT (void)  { TFTdisp.backlight (false); }
