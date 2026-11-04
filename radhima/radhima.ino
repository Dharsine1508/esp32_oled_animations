#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

// ============================================================
//                    GSNCREATIONS
// ============================================================
// YouTube  : www.youtube.com/@GSNcreation07
//
// About GSNCREATIONS:
// We create Arduino & ESP32 based electronics projects,
// OLED animations, mini games, IoT projects, sensors,
// robotics, DIY circuits and creative embedded systems.
//
// We share project ideas, circuit connections, source code,
// tutorials and experiments to help makers and electronics
// enthusiasts learn and build their own projects.
//
// Follow GSNCREATIONS for more:
// ✓ ESP32 Projects
// ✓ Arduino Projects
// ✓ OLED Animations
// ✓ Mini Games
// ✓ IoT Projects
// ✓ Robotics & Automation
// ✓ Sensors & DIY Electronics
// ✓ Source Code & Tutorials
//
// ============================================================

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "animation.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void playGIF(const AnimatedGIF* gif) {
  // Change loop variable to uint16_t to handle >255 frames
  for (uint16_t frame = 0; frame < gif->frame_count; frame++) {
    display.clearDisplay();

    for (uint16_t y = 0; y < gif->height; y++) {
      for (uint16_t x = 0; x < gif->width; x++) {
        uint16_t byteIndex = y * ((gif->width + 7) / 8) + (x / 8);
        uint8_t bitIndex = 7 - (x % 8);

        uint8_t b = pgm_read_byte(&(gif->frames[frame][byteIndex]));

        if (b & (1 << bitIndex)) {
          display.drawPixel(x, y, SSD1306_WHITE);
        }
      }
    }

    display.display();
    delay(gif->delays[frame]);
  }
}

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
}

void loop() {
  playGIF(&Radhima_gif);
}


// ============================================================
//                    CONNECT WITH US
// ============================================================
//
// Instagram : @GSNCREATIONS
// YouTube   : www.youtube.com/@GSNcreation07
//
// Have a doubt about this project?
// Need help with the circuit, code or connections?
//
// Feel free to contact us on Instagram!
// Send us a DM with your question or project doubt.
// We are happy to help and share ideas with fellow makers.
//
// ============================================================
//                 THANK YOU FOR SUPPORTING
//                     GSNCREATIONS ❤️
// ============================================================
//
// Keep Creating • Keep Learning • Keep Innovating
//
// More exciting ESP32, Arduino, OLED, IoT, Robotics
// and DIY Electronics projects coming soon!
//
// ============================================================

