// ==================== pin definitions ====================
#define TEMPC_XPOS        78
#define TEMPC_YPOS        50
#define MTEMPC_XPOS       20
#define MTEMPC_YPOS       24
#define E_XPOS            5
#define E_YPOS            4
#define BATT_XPOS         102
#define BATT_YPOS         4

// ==================== TFT definitions ====================
Adafruit_SSD1306 tft(128, 64, &Wire, -1);

void init_TFT (void) {
  if(!tft.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println("SSD1306 allocation failed");
  }
  tft.clearDisplay();
  turnon_TFT();
  tft.setTextColor(SSD1306_WHITE);
}

void drawMainScreen (void) {
  ClrdrawMeasureTempC();
  tft.setTextSize(2);
  tft.setCursor (MTEMPC_XPOS, MTEMPC_YPOS); tft.print (" --.-");
  tft.setCursor (MTEMPC_XPOS+70, MTEMPC_YPOS); tft.print ("C");
  tft.drawCircle (MTEMPC_XPOS+65, MTEMPC_YPOS, 2, SSD1306_WHITE);
  tft.setTextSize(1);

  tft.setCursor (TEMPC_XPOS+6, TEMPC_YPOS);   tft.print ("--.-");
  tft.setCursor (TEMPC_XPOS+38, TEMPC_YPOS);  tft.print ("C");         // tempC
  tft.drawCircle (TEMPC_XPOS+34, TEMPC_YPOS-1, 1, SSD1306_WHITE);
  
  tft.drawRect (BATT_XPOS+8, BATT_YPOS+1, 10, 6, SSD1306_WHITE);
  tft.drawRect (BATT_XPOS+18, BATT_YPOS+3, 2, 2, SSD1306_WHITE);
  tft.display();
}

void drawBattvalue (void) {
  tft.fillRect (BATT_XPOS+9, BATT_YPOS+2, 8, 4, SSD1306_BLACK);
  if (sys.Battlvl == 3) {
    tft.fillRect (BATT_XPOS+9, BATT_YPOS+2, 8, 4, SSD1306_WHITE);
  } else if (sys.Battlvl == 2) {
    tft.fillRect (BATT_XPOS+9, BATT_YPOS+2, 6, 4, SSD1306_WHITE);
  } else if (sys.Battlvl == 1) {
    tft.fillRect (BATT_XPOS+9, BATT_YPOS+2, 4, 4, SSD1306_WHITE);
  } else {
    tft.fillRect (BATT_XPOS+9, BATT_YPOS+2, 2, 4, SSD1306_WHITE);
  }
  tft.display();
}

void drawEvalue (void) {
  String txt = "";
  if (sys.Eidx == 0) {
    txt = "body";
  } else if (sys.Eidx == 1) {
    txt = "water/food";
  }
  tft.fillRect (E_XPOS, E_YPOS-1, 60, 9, SSD1306_BLACK);
  tft.setCursor (E_XPOS, E_YPOS); tft.print (txt);
  tft.display();
}

void drawTempC (void) {
  if (data.T == 0xFFFF) return;
  tft.fillRect (TEMPC_XPOS, TEMPC_YPOS, 30, 9, SSD1306_BLACK);
  String txt = "   " + String(data.T/10) + "." + String(data.T%10);
         txt = txt.substring(txt.length() - 5);
  tft.setCursor (TEMPC_XPOS, TEMPC_YPOS); tft.print (txt);
  tft.display();
}

void ClrdrawMeasureTempC (void) {
  tft.fillRect (MTEMPC_XPOS, MTEMPC_YPOS, 58, 16, SSD1306_BLACK);
  tft.display();
}

void drawMeasureTempC (uint16_t tempC, uint8_t sts) { //sts: 0-error, 1-ok, 2-measuring
  tft.fillRect (MTEMPC_XPOS, MTEMPC_YPOS, 58, 16, SSD1306_BLACK);
  tft.setTextSize(2);
  String txt = "   " + String(tempC/10) + "." + String(tempC%10);
  txt = txt.substring(txt.length() - 5);
  tft.setCursor (MTEMPC_XPOS, MTEMPC_YPOS); tft.print (txt);
  tft.setTextSize(1);
  tft.display();
}

void turnoff_TFT (void) { tft.ssd1306_command (0xAE); delay(100); }
void turnon_TFT (void)  { tft.ssd1306_command (0xAF); delay(100); }
