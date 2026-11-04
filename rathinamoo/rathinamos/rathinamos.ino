/*
  RATHINAMO - Animated OLED Lyrics Display
  Board: ESP32
  Display: 128x64 I2C SSD1306 OLED (usually address 0x3C)

  Libraries:
    - Adafruit GFX Library
    - Adafruit SSD1306

  Put rathinamo.ino and animation.h in the same sketch folder.
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "animation.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET   -1
#define OLED_ADDRESS 0x3C
#define OLED_SDA     21
#define OLED_SCL     22

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* lyrics[] = {
  "Rathinamo",
  "Vaanam thoorum poo mazhai",
  "Adhil aadava en thaenirai",
  "Paavai paarkkum paarvaiyil",
  "Irandhen pirandhen",
  "Rathinamo muthinamo",
  "Un kanne mundhi varum",
  "Mandhiramo sangeethamo",
  "Un solle paadi varum",
  "Nee chandhirano sooriyano",
  "En nenjam thindaadum",
  "Andha aadhiyanin kattalaiyum",
  "Un icchai vendraadum"
};

const uint8_t LYRIC_COUNT = sizeof(lyrics) / sizeof(lyrics[0]);

void showIntro() {
  display.invertDisplay(false);
  uint32_t start = millis();

  // Portrait appears first, then title is revealed.
  for (int16_t x = SCREEN_WIDTH; x >= 44; x -= 4) {
    display.clearDisplay();
    drawGirlPortrait(display, x, 3);
    drawTwinkles(display, millis() - start);
    display.display();
    delay(35);
  }

  // Hold the portrait with the title underneath.
  for (uint8_t n = 0; n <= 9; n++) {
    display.clearDisplay();
    drawGirlPortrait(display, 44, 2);
    drawTwinkles(display, millis() - start);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(37, 53);
    const char* title = "RATHINAMO";
    for (uint8_t i = 0; i < n; i++) display.write(title[i]);
    display.display();
    delay(170);
  }
  delay(1400);
}

void drawCenteredWrapped(const char* text, int y) {
  String remaining = String(text);
  remaining.trim();

  // At text size 1, 21 characters fit across 128 pixels.
  const int maxChars = 21;
  String rows[2] = {"", ""};
  if (remaining.length() <= maxChars) {
    rows[0] = remaining;
  } else {
    int split = remaining.lastIndexOf(' ', maxChars);
    if (split < 1) split = maxChars;
    rows[0] = remaining.substring(0, split);
    remaining = remaining.substring(split);
    remaining.trim();
    rows[1] = remaining.substring(0, maxChars);
  }

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  for (uint8_t r = 0; r < 2; r++) {
    if (rows[r].length() == 0) continue;
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(rows[r].c_str(), 0, y + r * 13, &x1, &y1, &w, &h);
    display.setCursor((SCREEN_WIDTH - w) / 2, y + r * 13);
    display.print(rows[r]);
  }
}

void revealLyric(const char* phrase, bool inverted, uint8_t index) {
  display.invertDisplay(inverted);
  String text = String(phrase);
  text.trim();

  // Header and a simple moving underline.
  for (uint8_t count = 0; count <= text.length(); count++) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(4, 4);
    display.print("RATHINAMO");
    display.drawFastHLine(4, 14, (count * 120) / max(1, (int)text.length()), SSD1306_WHITE);

    String partial = text.substring(0, count);
    drawCenteredWrapped(partial.c_str(), 27);

    // Small animated dots at the bottom.
    uint8_t dots = (millis() / 260) % 4;
    display.setCursor(55, 54);
    for (uint8_t d = 0; d < dots; d++) display.write('.');
    display.display();
    delay(45);
  }

  delay(1450);
}

void setup() {
  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    while (true) delay(100);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(24, 27);
  display.print("GETTING READY...");
  display.display();
  delay(900);

  showIntro();
}

void loop() {
  for (uint8_t i = 0; i < LYRIC_COUNT; i++) {
    // Alternate normal and inverted display:
    // normal = blue OLED pixels on dark background;
    // inverted = lit blue background with dark lettering.
    revealLyric(lyrics[i], (i % 2) == 1, i);
  }
  display.invertDisplay(false);
  delay(900);
}
