/*
  Simple sparkle animation for the RATHINAMO OLED lyrics project.
  This is a lightweight animation helper, not a pre-rendered GIF.
*/

#ifndef RATHINAMO_ANIMATION_H
#define RATHINAMO_ANIMATION_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

inline void drawTinySparkle(Adafruit_SSD1306 &screen, int x, int y, bool bright) {
  if (bright) {
    screen.drawPixel(x, y, SSD1306_WHITE);
    screen.drawFastHLine(x - 2, y, 5, SSD1306_WHITE);
    screen.drawFastVLine(x, y - 2, 5, SSD1306_WHITE);
  } else {
    screen.drawPixel(x, y, SSD1306_WHITE);
  }
}

inline void drawSparkleAnimation(Adafruit_SSD1306 &screen, uint32_t elapsedMs) {
  // The sparkle positions stay fixed; their brightness alternates.
  const int sparkleX[] = {76, 91, 108, 121};
  const int sparkleY[] = {6, 9, 4, 8};

  uint8_t phase = (elapsedMs / 180) % 4;

  for (uint8_t i = 0; i < 4; i++) {
    bool bright = ((i + phase) % 4 == 0);
    drawTinySparkle(screen, sparkleX[i], sparkleY[i], bright);
  }
}

#endif
