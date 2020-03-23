// ==================== pin definitions ====================
#define TFT_CS            10          // CS,       Chip Select
#define TFT_DC            8           // DC or A0, Data/Command
#define TFT_RST           9           // RST,      Reset
#define TFT_LED           A3          // LED,      Brightness

// ==================== TFT Color definitions ====================
#define BLACK             0x0000
#define BLUE              0x001F
#define RED               0xF800
#define GREEN             0x07E0
#define CYAN              0x07FF
#define MAGENTA           0xF81F
#define YELLOW            0xFFE0
#define WHITE             0xFFFF

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
  TFTdisp.fillRect (0, 0, 160, 15, GREEN);                  // draw rectangle (x, y, width, height, background_color)
  TFTdisp.setTextColor(BLACK);                              // set the font color to black
  TFTdisp.setCursor (2, 4);
  TFTdisp.print(F("Digital Thermometer"));
  TFTdisp.backlight (false);                                // turn-on LED backlight
  TFTdisp.setTextColor(WHITE);                              // set the font color to WHITE
}

void drawMainScreen (void) {
  TFTdisp.setTextScale(2);
  TFTdisp.setCursor (3, 27);   TFTdisp.print ("T:");
  TFTdisp.setCursor (110, 27); TFTdisp.print ("C");
  TFTdisp.fillRect (81, 37, 3, 3, WHITE);
  TFTdisp.setCursor (110, 47); TFTdisp.print ("C");
  TFTdisp.setTextScale(1);
  TFTdisp.setCursor (102, 27); TFTdisp.print ("o");
  TFTdisp.setCursor (102, 47); TFTdisp.print ("o");
  TFTdisp.setCursor (3, 67); TFTdisp.print ("E:");
  InitdrawTempC();
}

void drawEvalue (void) {
  String txt = "";
  TFTdisp.setTextScale(1);
  if (sys.Eidx == 0) {
    txt = "body";
  } else if (sys.Eidx == 1) {
    txt = "water/food";
  }
  TFTdisp.fillRect (15, 66, 80, 9, BLACK);
  TFTdisp.setCursor (15, 67); TFTdisp.print (txt);
}

void drawTempC (void) {
  if (data.T == 0xFFFF) return;
  TFTdisp.setTextScale(2);
  String txt1 = "   " + String(data.T/10);
         txt1 = txt1.substring(txt1.length() - 3);
  String txt2 = String(data.T%10);
  for (uint8_t i=0; i<3; i++) {
    if (disp.tempC1[i] != txt1.charAt(i)) {
      disp.tempC1[i] = txt1.charAt(i);
      TFTdisp.fillRect (43+(i*12), 26, 12, 15, BLACK);
      TFTdisp.setCursor (44+(i*12), 27);
      TFTdisp.print (disp.tempC1[i]);
    }
  }
  if (disp.tempC1[3] != txt2.charAt(0)) {
    disp.tempC1[3] = txt2.charAt(0);
    TFTdisp.fillRect (87, 26, 12, 15, BLACK);
    TFTdisp.setCursor (88, 27);
    TFTdisp.print (disp.tempC1[3]);
  }
}

void InitdrawTempC (void) {
  TFTdisp.setTextScale(2);
  TFTdisp.fillRect (43, 26, 12, 15, BLACK);
  TFTdisp.setCursor (55, 27);
  TFTdisp.print ("--");
  TFTdisp.fillRect (87, 26, 12, 15, BLACK);
  TFTdisp.setCursor (88, 27);
  TFTdisp.print ("-");
}

void initdrawMeasureTempC (void) {
  TFTdisp.fillRect (81, 57, 3, 3, WHITE);
}

void ClrdrawMeasureTempC (void) {
  strcpy(disp.tempC2, "    ");
  TFTdisp.fillRect (43, 46, 56, 15, BLACK);
  TFTdisp.fillCircle (130, 54, 5, BLACK);
}

void drawMeasureTempC (uint16_t tempC, uint16_t color) {
  TFTdisp.setTextScale(2);
  String txt1 = "   " + String(tempC/10);
  txt1 = txt1.substring(txt1.length() - 3);
  String txt2 = String(tempC%10);
  
  for (uint8_t i=0; i<3; i++) {
    if (disp.tempC2[i] != txt1.charAt(i)) {
      disp.tempC2[i] = txt1.charAt(i);
      TFTdisp.fillRect (43+(i*12), 46, 12, 15, BLACK);
      TFTdisp.setCursor (44+(i*12), 47);
      TFTdisp.print (disp.tempC1[i]);
    }
  }
  if (disp.tempC2[3] != txt2.charAt(0)) {
    disp.tempC2[3] = txt2.charAt(0);
    TFTdisp.fillRect (87, 46, 12, 15, BLACK);
    TFTdisp.setCursor (88, 47);
    TFTdisp.print (disp.tempC2[3]);
  }
  TFTdisp.fillCircle (130, 54, 5, color);
}

void drawHoldTempC (uint16_t tempC) {
  TFTdisp.setTextScale(2);
  String txt1 = "   " + String(tempC/10);
  txt1 = txt1.substring(txt1.length() - 3);
  String txt2 = String(tempC%10);
  
  for (uint8_t i=0; i<3; i++) {
    if (disp.tempC2[i] != txt1.charAt(i)) {
      disp.tempC2[i] = txt1.charAt(i);
      TFTdisp.fillRect (43+(i*12), 46, 12, 15, BLACK);
      TFTdisp.setCursor (44+(i*12), 47);
      TFTdisp.print (disp.tempC1[i]);
    }
  }
  if (disp.tempC2[3] != txt2.charAt(0)) {
    disp.tempC2[3] = txt2.charAt(0);
    TFTdisp.fillRect (87, 46, 12, 15, BLACK);
    TFTdisp.setCursor (88, 47);
    TFTdisp.print (disp.tempC2[3]);
  }
  if ((data.state & 0x20) == 0) TFTdisp.fillCircle (130, 54, 5, RED);
}

void turnoff_TFT (void) { TFTdisp.backlight (true); }
void turnon_TFT (void)  { TFTdisp.backlight (false); }
