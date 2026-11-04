# esp32_oled_animations 🎵

Tamil and English song lyric animations on a 0.96" SSD1306 OLED, driven by an ESP32. Each song is a ready-to-flash Arduino sketch that plays a frame-by-frame monochrome animation in a loop.

A Python tool (Pillow) converts any GIF or lyrics text file into the Arduino code, so a new song takes minutes to add.

## Demo
Demo videos are in this repo (see the `WhatsApp Video ...` files), and on LinkedIn: <linkedin.com/in/dharsine>

## Songs
| Folder | Song | Language |
|---|---|---|
| `radhima/` | Radhima | Tamil |
| `rathinamo/` | <song name> | Tamil |
| `rathinamoo/` | <song name> | Tamil |
| `song/` | <song name> | English |

## Hardware
- ESP32 DevKit
- 0.96" SSD1306 OLED, 128×64, I2C (address `0x3C`)
- Breadboard, jumper wires, USB cable

## Wiring
| OLED | ESP32 |
|---|---|
| GND | GND |
| VDD | 3V3 |
| SCK (SCL) | GPIO 22 |
| SDA | GPIO 21 |

## Software
- Arduino IDE with ESP32 board support
- Libraries: `Adafruit GFX`, `Adafruit SSD1306`
- Python 3 + `pillow` (only to create new animations)

## How to run
1. Open a song folder's `.ino` file in Arduino IDE.
2. Select your ESP32 board and port.
3. Click Upload. The animation starts and loops automatically.

## Make your own animation
```bash
pip install pillow
python lyrics_to_gif.py lyrics.txt song.gif --image art.png
python gif_to_oled.py song.gif MySong --template radhima/radhima.ino
```
Options: `--threshold`, `--dither`, `--invert`, `--step`, `--delay`.

## How it works
- GIF frames are resized to 128×64 and converted to 1-bit.
- Each frame is packed into 1024 bytes and stored in flash with `PROGMEM`, so the ESP32's RAM stays free.
- One `playGIF()` function plays any song through the `Adafruit_SSD1306` library over I2C.

## What I learned
- Getting started with the ESP32 and Arduino IDE (boards, ports, flashing)
- I2C communication and wiring an OLED display
- Storing large data in flash with PROGMEM
- Encoding frames as bitmaps for monochrome displays
- Automating the workflow with a Python (Pillow) tool

## Roadmap
- [ ] Wi-Fi control: choose the song from a web page
- [ ] Button to switch songs
- [ ] Sync the animation with audio

