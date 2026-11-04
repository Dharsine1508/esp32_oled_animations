/*
  RATHINAMO - Bold Capital Lyrics OLED Display
  ESP32 + 128x64 I2C SSD1306 OLED

  Required libraries:
    - Adafruit GFX Library
    - Adafruit SSD1306

  Keep rathinamo.ino and animation.h in the same sketch folder.
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "animationsong.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET   -1
#define OLED_ADDRESS 0x3C
#define OLED_SDA     21
#define OLED_SCL     22

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Every phrase is converted to CAPITAL LETTERS before it is displayed.
const char* lyrics[] = {
  "RATHINAMO",
  "VAANAM THOORUM POO MAZHAI",
  "ADHIL AADAVA EN THAENIRAI",
  "PAAVAI PAARKKUM PAARVAIYIL",
  "IRANDHEN PIRANDHEN",
  "RATHINAMO MUTHINAMO",
  "UN KANNE MUNDHI VARUM",
  "MANDHIRAMO SANGEETHAMO",
  "UN SOLLE PAADI VARUM",
  "NEE CHANDHIRANO SOORIYANO",
  "EN NENJAM THINDAADUM",
  "ANDHA AADHIYANIN KATTALAIYUM",
  "UN ICCHAI VENDRAADUM"
};

const uint8_t LYRIC_COUNT = sizeof(lyrics) / sizeof(lyrics[0]);

// Bold 5x7 lettering: render each character to a tiny canvas,
// then widen each lit pixel by one column for a thicker pixel-font look.
void drawBoldChar(char c, int16_t x, int16_t y) {
  GFXcanvas1 glyph(6, 8);
  glyph.fillScreen(0);
  glyph.setTextSize(1);
  glyph.setTextColor(1);
  glyph.setCursor(0, 0);
  glyph.write(c);

  for (int16_t gy = 0; gy < 8; gy++) {
    for (int16_t gx = 0; gx < 6; gx++) {
      if (glyph.getPixel(gx, gy)) {
        display.drawPixel(x + gx, y + gy, SSD1306_WHITE);
        display.drawPixel(x + gx + 1, y + gy, SSD1306_WHITE);
      }
    }
  }
}

// Bold uppercase line, centered. Character pitch is 7 pixels.
void drawBoldCentered(const String &line, int16_t y) {
  int16_t x = (SCREEN_WIDTH - (line.length() * 7)) / 2;
  if (x < 0) x = 0;

  for (uint16_t i = 0; i < line.length(); i++) {
    drawBoldChar(line[i], x + (i * 7), y);
  }
}

// Break long lyric phrases at a word boundary.
// 18 characters fit comfortably in the bold font.
void drawWrappedBold(const String &phrase, int16_t y) {
  String first = phrase;
  String second = "";

  if (first.length() > 18) {
    int splitAt = first.lastIndexOf(' ', 18);
    if (splitAt < 1) splitAt = 18;
    second = first.substring(splitAt);
    first = first.substring(0, splitAt);
    second.trim();
  }

  drawBoldCentered(first, y);
  if (second.length() > 0) {
    drawBoldCentered(second, y + 13);
  }
}

void showIntro() {
  display.invertDisplay(false);
  uint32_t startTime = millis();

  // Girl portrait slides in from the right.
  for (int16_t x = SCREEN_WIDTH; x >= 44; x -= 3) {
    display.clearDisplay();
    drawGirlPortrait(display, x, 2);
    drawTwinkles(display, millis() - startTime);
    display.display();
    delay(30);
  }

  // Reveal the title letter by letter under the portrait.
  const char title[] = "RATHINAMO";
  for (uint8_t count = 0; count <= 9; count++) {
    display.clearDisplay();
    drawGirlPortrait(display, 44, 2);
    drawTwinkles(display, millis() - startTime);

    String partial = "";
    for (uint8_t i = 0; i < count; i++) partial += title[i];
    drawBoldCentered(partial, 53);

    display.display();
    delay(160);
  }
  delay(1200);
}

void showLyric(uint8_t index) {
  // Alternate dark-blue-pixel mode and inverted bright-blue-screen mode.
  bool inverted = (index % 2 == 1);
  display.invertDisplay(inverted);

  String phrase = String(lyrics[index]);
  phrase.toUpperCase();

  // Typing reveal, one character at a time.
  for (uint16_t count = 0; count <= phrase.length(); count++) {
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(4, 4);
    display.print("RATHINAMO");
    display.drawFastHLine(4, 14, 120, SSD1306_WHITE);

    String partial = phrase.substring(0, count);
    drawWrappedBold(partial, 25);

    // Animated three-dot indicator.
    uint8_t dots = (millis() / 240) % 4;
    display.setCursor(55, 55);
    for (uint8_t d = 0; d < dots; d++) display.write('.');

    display.display();
    delay(55);
  }

  // Hold the complete lyric on screen.
  delay(1650);
}

void setup() {
  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    while (true) delay(100);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(27, 28);
  display.print("RATHINAMO...");
  display.display();
  delay(900);

  showIntro();
}

void loop() {
  for (uint8_t i = 0; i < LYRIC_COUNT; i++) {
    showLyric(i);
  }

  display.invertDisplay(false);
  delay(1000);
}
