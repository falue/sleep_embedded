#pragma once

// ============================================================
//  SPI bus  (shared by VS1053 + SD card)
//  ESP32-S3-WROOM-1-N16R8: GPIOs 26-37 are reserved for
//  octal flash/PSRAM - do NOT use them.
// ============================================================
#define PIN_SPI_SCK   12
#define PIN_SPI_MOSI  11
#define PIN_SPI_MISO  13

// ============================================================
//  VS1053 control pins
// ============================================================
#define PIN_VS_CS     10
#define PIN_VS_DCS     9
#define PIN_VS_DREQ    8
#define PIN_VS_RESET  14   // set -1 to tie RESET to EN

// ============================================================
//  SD card chip-select (directly on the VS1053 breakout)
// ============================================================
#define PIN_SD_CS      7

// ============================================================
//  Buttons - active LOW, using INPUT_PULLUP
//  3 core buttons + 2 optional volume buttons
// ============================================================
#define BTN_PLAY_STOP     1    // toggle: play (with timer) / fade-stop
#define BTN_TRACK         4    // cycle to next track
#define BTN_TIMER        17    // cycle timer presets

#define BTN_VOL_UP        6
#define BTN_VOL_DOWN     15

#if ENABLE_VOL_BUTTONS
const uint8_t ALL_BTNS[] = {
  BTN_PLAY_STOP, BTN_TRACK, BTN_TIMER, BTN_VOL_UP, BTN_VOL_DOWN
};
#else
const uint8_t ALL_BTNS[] = {
  BTN_PLAY_STOP, BTN_TRACK, BTN_TIMER
};
#endif
const int BTN_COUNT = sizeof(ALL_BTNS) / sizeof(ALL_BTNS[0]);
const unsigned long DEBOUNCE_MS = 200;

// ============================================================
//  LED bar (10 LEDs, left-to-right)
//  Selection: fills L->R proportional to chosen preset
//  Countdown: single LED sweeps R->L as time elapses
// ============================================================
#define LED_COUNT 10
const uint8_t LED_PINS[LED_COUNT] = {
  3, 18, 21, 38, 39, 40, 41, 42, 47, 48
};

// ============================================================
//  Track scanning (system)
// ============================================================
#define MAX_TRACKS  99
const char* AUDIO_EXTENSIONS[] = { ".mp3", ".wav", ".ogg", ".flac", ".aac", ".wma", ".mid", ".midi" };
const int AUDIO_EXT_COUNT = sizeof(AUDIO_EXTENSIONS) / sizeof(AUDIO_EXTENSIONS[0]);
