# sleep – embedded white-noise player

> **Note**: ⚠ VIBE CODED ⚠

ESP32-S3 + Adafruit VS1053 Codec/MicroSD breakout.
Embedded version of [falue/sleep](https://github.com/falue/sleep).

## Hardware

| Component | Model |
|---|---|
| MCU | ESP32-S3-DEV-KIT-N16R8-M (Waveshare) |
| Audio/SD | Adafruit VS1053 Codec + MicroSD Breakout |
| Buttons | 3× core + 2× optional volume (momentary NO, to GND) |
| LEDs | 10× LED bar (with current-limiting resistors) |

## Wiring

### VS1053 breakout → ESP32-S3

| VS1053 pin | ESP32-S3 GPIO | Notes |
|---|---|---|
| CLK | 12 | SPI SCK |
| MOSI | 11 | SPI MOSI |
| MISO | 13 | SPI MISO |
| CS | 10 | VS1053 chip select |
| XDCS | 9 | VS1053 data chip select |
| DREQ | 8 | VS1053 data request |
| RST | 14 | VS1053 reset (or tie to EN) |
| SDCS | 7 | SD card chip select |
| (V)CC | 3V3 | "V" is not printed on VS1053 pcb |
| GND | GND | |

> **GPIOs 26-37 are unavailable** on the N16R8 variant (used by octal flash/PSRAM).

### Buttons (active LOW, internal pull-up)

| Function | GPIO | Notes |
|---|---|---|
| Play / Stop | 1 | Toggle: play (with timer) / fade-stop |
| Track | 4 | Cycle to next track |
| Timer | 17 | Cycle timer presets |
| Volume + | 6 | Optional (`ENABLE_VOL_BUTTONS`) |
| Volume - | 15 | Optional (`ENABLE_VOL_BUTTONS`) |

Wire each button between the GPIO pin and **GND**.

### LED bar (10 LEDs, left-to-right)

| LED # | GPIO |
|---|---|
| 1 | 3 |
| 2 | 18 |
| 3 | 21 |
| 4 | 38 |
| 5 | 39 |
| 6 | 40 |
| 7 | 41 |
| 8 | 42 |
| 9 | 47 |
| 10 | 48 |

220 Ω resistor in series to GND per LED.

## Arduino IDE setup

### Board
- **Board**: ESP32S3 Dev Module
- **Flash Size**: 16 MB
- **PSRAM**: OPI PSRAM
- **Partition Scheme**: Default 4 MB with spiffs (or larger OTA-capable scheme)

### Required libraries (install via Library Manager)
| Library | Author |
|---|---|
| Adafruit VS1053 Library | Adafruit |

`SD`, `SPI`, `WiFi`, `HTTPClient`, `Preferences`, `Update` ship with the ESP32 Arduino core.

## SD card contents

Place audio files in the root directory - any filename, any supported format (MP3, WAV, OGG, FLAC, AAC, WMA, MIDI):
```
/rain_loop.mp3
/ocean.wav
/white_noise.ogg
...
```
Files named `audio1.mp3`, `audio2.mp3`, etc. can also be downloaded from `telefabi.ch` by holding PLAY during boot.

Optional: place an `ssid.txt` file on the SD card root to add a WiFi network:
```
my_ssid:my_password
```
This credential is tried first, before the ones hardcoded in `config.h`.

## OTA firmware updates

On boot (if WiFi connects) the device:
1. Fetches `https://telefabi.ch/sleep/embedded/hash.txt` - expects a single line of text (e.g. `abc123`)
2. Compares with locally stored hash (NVS).
3. If different: downloads `https://telefabi.ch/sleep/embedded/recent.bin`, flashes, reboots.

> **Server must serve a raw `.bin` firmware image** (not a `.zip`).
> Compile with *Sketch → Export Compiled Binary* and upload the `.bin` to the server.

## Behaviour

| Action | Effect |
|---|---|
| Boot | Sine-wave LED sweep, plays last track (fade-in). Hold PLAY for WiFi/OTA/download mode |
| Play/Stop btn | Stopped: play last track + auto-start saved timer. Playing: 2 s fade-out -> stop |
| Track btn | Cycle to next track (fade-out/fade-in if playing, wraps around) |
| Timer btn | Cycle: off -> 5 -> 15 -> 30 -> 45 -> 60 min (saved to NVS, auto-starts after 2 s if playing) |
| Vol + / - | Adjust volume, persisted (optional, set `ENABLE_VOL_BUTTONS` in config.h) |
| Timer expires | 60 s fade-out -> stop |
| LED bar (selecting) | Fills L->R proportional to chosen preset |
| LED bar (running) | Single LED sweeps R->L as time elapses |

Tracks loop indefinitely until stopped or timer expires.
Timer preset and last track are persisted across reboots.

## Configuration

Edit `config.h` for user settings:
- WiFi credentials, server URLs
- Timer presets, fade durations
- Volume defaults & step size
- Enable/disable volume buttons

Edit `hardware.h` for system settings:
- SPI, VS1053, SD card pin assignments
- Button and LED pin assignments
- Supported audio extensions, max tracks

## Tools

- **`tools/loopify.py`** - Prepare audio for seamless looping (split, reorder, crossfade). Outputs MP3. Requires `ffmpeg`.
- **`tools/audio2header.py`** - Convert audio to C header for PROGMEM system sounds.
