// ----------------------------------------------------------------------------------------------------
// =========== OLED Display ============ 
// ----------------------------------------------------------------------------------------------------


/* 
 * OLED Display Include and Settings
 */
#include "U8x8lib.h"                                      // include the fast graphics library
#include <Wire.h>                                         // include the Wire protocol (to communicate over I2C)

U8X8_SSD1306_128X32_UNIVISION_HW_I2C oled(U8X8_PIN_NONE); // instantiate the display object "oled"
const int MAX_REFRESH = 1000;                             // limit the screen clearing to at most once per second
unsigned long lastClear = 0;                              // the last time we updated the screen

void setupDisplay() {
  oled.begin();
  oled.setPowerSave(0);
  oled.setFont(u8x8_font_amstrad_cpc_extended_r);
  oled.clearDisplay();
}

/*
 * Write a message on the OLED display.
 */
void writeDisplay(const char *message, int row, bool clearRow) {
  if (clearRow) {
    oled.clearLine(row); // Clear the specific row before writing
  }
  oled.setCursor(0, row);
  oled.print(message);
}

/*
 * Display the current score and remaining lives on the OLED.
 */
void displayScoreAndLives(int score, int lives) {
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "Sc: %d Lv: %d", score, lives); // Abbreviated labels
  writeDisplay(buffer, 0, true); // Display on row 0 and clear the row
}

/*
 * Display the top 3 scores on the OLED.
 */
void displayTopScores(int topScores[3]) {
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "Top: %d %d %d", topScores[0], topScores[1], topScores[2]);
  writeDisplay(buffer, 1, true); // Display on row 1 and clear the row
}

/*
 * Display the current sensitivity setting on the OLED.
 */
void displaySensitivity(int sensitivity) {
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "Sens: %d", sensitivity); // Abbreviated label
  writeDisplay(buffer, 2, true); // Display on row 2 and clear the row
}