/*
  RATHINAMO - OLED Lyrics Display
  Board: ESP32
  Display: 128x64 I2C OLED (SSD1306, address 0x3C)

  Libraries required (Arduino Library Manager):
    - Adafruit GFX Library
    - Adafruit SSD1306

  Keep rathinamo.ino and animation.h in the same Arduino sketch folder.
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "animation.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET   -1
#define OLED_ADDRESS 0x3C

// Common ESP32 default I2C pins.
// Change these if your wiring uses different GPIOs.
#define OLED_SDA 21
#define OLED_SCL 22

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

struct LyricPage {
  const char* line1;
  const char* line2;
  uint16_t duration;
};

// Each page contains two lyric phrases.
// Long phrases are automatically wrapped to fit the 128x64 display.
const LyricPage lyrics[] = {
  {"Rathinamo", "Vaanam thoorum poo mazhai", 3200},
  {"Adhil aadava", "en thaenirai", 3000},
  {"Paavai paarkkum paarvaiyil", "Irandhen pirandhen", 3400},
  {"Rathinamo muthinamo", "Un kanne mundhi varum", 3400},
  {"Mandhiramo sangeethamo", "Un solle paadi varum", 3400},
  {"Nee chandhirano sooriyano", "En nenjam thindaadum", 3400},
  {"Andha aadhiyanin kattalaiyum", "Un icchai vendraadum", 3600}
};

const uint8_t PAGE_COUNT = sizeof(lyrics) / sizeof(lyrics[0]);

// Wrap a phrase into at most two lines of 21 characters.
// Text size 1 uses a 6-pixel character cell; 21 chars fit in 126 pixels.
void drawWrappedPhrase(const char* phrase, int16_t y, uint8_t maxChars = 21) {
  String remaining = phrase;
  remaining.trim();

  String firstLine = "";
  String secondLine = "";

  if (remaining.length() <= maxChars) {
    firstLine = remaining;
  } else {
    int splitAt = remaining.lastIndexOf(' ', maxChars);
    if (splitAt < 1) splitAt = maxChars;
    firstLine = remaining.substring(0, splitAt);
    remaining = remaining.substring(splitAt);
    remaining.trim();

    if (remaining.length() <= maxChars) {
      secondLine = remaining;
    } else {
      // Safety fallback for unexpectedly long text.
      secondLine = remaining.substring(0, maxChars);
    }
  }

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  int16_t x1, y1;
  uint16_t w, h;

  display.getTextBounds(firstLine.c_str(), 0, y, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, y);
  display.print(firstLine);

  if (secondLine.length() > 0) {
    display.getTextBounds(secondLine.c_str(), 0, y + 11, &x1, &y1, &w, &h);
    display.setCursor((SCREEN_WIDTH - w) / 2, y + 11);
    display.print(secondLine);
  }
}

void showLyricPage(uint8_t pageIndex) {
  uint32_t pageStart = millis();
  const uint32_t pageDuration = lyrics[pageIndex].duration;

  while (millis() - pageStart < pageDuration) {
    display.clearDisplay();

    // Small title/header.
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(3, 3);
    display.print("RATHINAMO");

    // Animated sparkles across the top edge.
    drawSparkleAnimation(display, millis());

    display.drawFastHLine(0, 13, SCREEN_WIDTH, SSD1306_WHITE);

    // Two phrases, each wrapped if necessary.
    drawWrappedPhrase(lyrics[pageIndex].line1, 21);
    drawWrappedPhrase(lyrics[pageIndex].line2, 43);

    display.display();
    delay(70);
  }
}

void setup() {
  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    while (true) {
      delay(100);
    }
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(19, 25);
  display.print("RATHINAMO");
  display.setCursor(25, 39);
  display.print("Starting...");
  display.display();
  delay(1200);
}

void loop() {
  for (uint8_t i = 0; i < PAGE_COUNT; i++) {
    showLyricPage(i);
  }
  delay(900);
}
