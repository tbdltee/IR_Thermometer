// ==================== pin definitions ====================
#define TFT_CS            10          // CS,       Chip Select
#define TFT_DC            8           // DC or A0, Data/Command
#define TFT_RST           9           // RST,      Reset
#define TFT_LED           A3          // LED,      Brightness

#define TEMPC_XPOS        80
#define TEMPC_YPOS        50
#define MTEMPC_XPOS       24
#define MTEMPC_YPOS       24
#define SCAN_XPOS         112
#define SCAN_YPOS         32

#define E_XPOS            5
#define E_YPOS            50
#define BATT_XPOS         102
#define BATT_YPOS         4

// ==================== TFT definitions ====================
Adafruit_ST7735 TFTdisp = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
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
  pinMode (TFT_LED, OUTPUT);
  TFTdisp.initR(INITR_BLACKTAB);
  TFTdisp.setRotation(1);
  TFTdisp.setTextSize(1);                                  // set the font scaling to 1
  TFTdisp.fillScreen(ST77XX_BLACK);                         // clear all screen
  turnon_TFT();
  TFTdisp.setTextColor(ST77XX_WHITE);                              // set the font color to WHITE
  TFTdisp.drawRect (0, 0, 128, 64, ST77XX_WHITE);
}

void drawMainScreen (void) {
  TFTdisp.setTextSize(2);
  TFTdisp.setCursor (MTEMPC_XPOS+66, MTEMPC_YPOS); TFTdisp.print ("C");          // hold tempC
  TFTdisp.setCursor (MTEMPC_XPOS+12, MTEMPC_YPOS); TFTdisp.print ("--");
  TFTdisp.setCursor (MTEMPC_XPOS+44, MTEMPC_YPOS); TFTdisp.print ("-");
  TFTdisp.fillRect (MTEMPC_XPOS+37, MTEMPC_YPOS+7, 3, 3, ST77XX_WHITE);
  TFTdisp.setTextSize(1);
  TFTdisp.drawCircle (MTEMPC_XPOS+61, MTEMPC_YPOS, 2, ST77XX_WHITE);
  
  TFTdisp.setCursor (TEMPC_XPOS+36, TEMPC_YPOS);  TFTdisp.print ("C");         // tempC
  TFTdisp.drawCircle (TEMPC_XPOS+32, TEMPC_YPOS-1, 1, ST77XX_WHITE);
  TFTdisp.setCursor (TEMPC_XPOS+6, TEMPC_YPOS);   TFTdisp.print ("--");
  TFTdisp.setCursor (TEMPC_XPOS+16, TEMPC_YPOS);  TFTdisp.print (".");
  TFTdisp.setCursor (TEMPC_XPOS+22, TEMPC_YPOS);  TFTdisp.print ("-");
  
  TFTdisp.setCursor (E_XPOS, E_YPOS); TFTdisp.print ("E:");
  TFTdisp.drawRect (BATT_XPOS+8, BATT_YPOS+1, 10, 6, ST77XX_WHITE);
  TFTdisp.drawRect (BATT_XPOS+18, BATT_YPOS+3, 2, 2, ST77XX_WHITE);
}

void drawBattvalue (void) {
  TFTdisp.fillRect (BATT_XPOS+9, BATT_YPOS+2, 8, 4, ST77XX_BLACK);
  if (sys.Battlvl == 3) {
    TFTdisp.fillRect (BATT_XPOS+9, BATT_YPOS+2, 8, 4, ST77XX_WHITE);
  } else if (sys.Battlvl == 2) {
    TFTdisp.fillRect (BATT_XPOS+9, BATT_YPOS+2, 6, 4, ST77XX_WHITE);
  } else if (sys.Battlvl == 1) {
    TFTdisp.fillRect (BATT_XPOS+9, BATT_YPOS+2, 4, 4, ST77XX_WHITE);
  } else {
    TFTdisp.fillRect (BATT_XPOS+9, BATT_YPOS+2, 2, 4, ST77XX_WHITE);
  }
}

void drawEvalue (void) {
  String txt = "";
  if (sys.Eidx == 0) {
    txt = "body";
  } else if (sys.Eidx == 1) {
    txt = "water/food";
  }
  TFTdisp.fillRect (E_XPOS+12, E_YPOS-1, 60, 9, ST77XX_BLACK);
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
      TFTdisp.fillRect (TEMPC_XPOS+(i*6), TEMPC_YPOS-1, 6, 9, ST77XX_BLACK);
      TFTdisp.setCursor (TEMPC_XPOS+(i*6), TEMPC_YPOS);
      TFTdisp.print (disp.tempC1[i]);
    }
  }
  if (disp.tempC1[3] != txt2.charAt(0)) {
    disp.tempC1[3] = txt2.charAt(0);
    TFTdisp.fillRect (TEMPC_XPOS+22, TEMPC_YPOS-1, 6, 9, ST77XX_BLACK);
    TFTdisp.setCursor (TEMPC_XPOS+22, TEMPC_YPOS);
    TFTdisp.print (disp.tempC1[3]);
  }
}

void ClrdrawMeasureTempC (void) {
  strcpy(disp.tempC2, " ---");
  TFTdisp.fillRect (MTEMPC_XPOS-1, MTEMPC_YPOS-1, 56, 15, ST77XX_BLACK);
  TFTdisp.fillRect (MTEMPC_XPOS+37, MTEMPC_YPOS+10, 3, 3, ST77XX_WHITE);
  drawMeasureCondition(0);
}

void drawMeasureCondition (uint8_t value) {   // 0:black, 1:white
  if (value) TFTdisp.fillCircle (SCAN_XPOS, SCAN_YPOS, 5, ST77XX_WHITE);
  else TFTdisp.fillCircle (SCAN_XPOS, SCAN_YPOS, 5, ST77XX_BLACK);
}
void drawMeasureTempC (uint16_t tempC, uint8_t sts) { //sts: 0-error, 1-ok, 2-measuring
  TFTdisp.setTextSize(2);
  String txt1 = "   " + String(tempC/10);
  txt1 = txt1.substring(txt1.length() - 3);
  String txt2 = String(tempC%10);
  
  for (uint8_t i=0; i<3; i++) {
    if (disp.tempC2[i] != txt1.charAt(i)) {
      disp.tempC2[i] = txt1.charAt(i);
      TFTdisp.fillRect (MTEMPC_XPOS+(i*12)-1, MTEMPC_YPOS-1, 12, 15, ST77XX_BLACK);
      TFTdisp.setCursor (MTEMPC_XPOS+(i*12), MTEMPC_YPOS);
      TFTdisp.print (disp.tempC1[i]);
    }
  }
  if (disp.tempC2[3] != txt2.charAt(0)) {
    disp.tempC2[3] = txt2.charAt(0);
    TFTdisp.fillRect (MTEMPC_XPOS+43, MTEMPC_YPOS-1, 12, 15, ST77XX_BLACK);
    TFTdisp.setCursor (MTEMPC_XPOS+44, MTEMPC_YPOS);
    TFTdisp.print (disp.tempC2[3]);
  }
  drawMeasureCondition (sts);
  TFTdisp.setTextSize(1);
}

void turnoff_TFT (void) { digitalWrite (TFT_LED, LOW); }
void turnon_TFT (void)  { digitalWrite (TFT_LED, HIGH); }
