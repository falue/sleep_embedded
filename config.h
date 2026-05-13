#pragma once
// ============================================================
//  config.h - User-editable settings
//  For pin assignments and hardware config, see hardware.h
// ============================================================

// ============================================================
//  Timer presets (minutes).  Index 0 = no timer.
// ============================================================
const int TIMER_PRESETS[]    = {0, 5, 15, 30, 45, 60};
const int TIMER_PRESET_COUNT = sizeof(TIMER_PRESETS) / sizeof(TIMER_PRESETS[0]);
const unsigned long TIMER_CONFIRM_MS = 2000;  // ms before selection auto-starts timer

// ============================================================
//  Audio / fade constants
// ============================================================
#define FADE_STOP_MS       2000    // fade-out on STOP button (ms)
#define FADE_SLEEP_MS     60000    // fade-out when sleep timer expires (ms)
#define FADE_MS            2000    // fade out/in on track change (ms)

// VS1053 volume: 0x00 = loudest, 0xFE = near-silent
#define VOL_MAX              0
#define VOL_MIN             60     // practical silence
#define VOL_DEFAULT         16     // ~75% loudness
#define VOL_STEP             2

// ============================================================
//  Volume buttons (set to 0 to disable)
// ============================================================
#define ENABLE_VOL_BUTTONS  1

// ============================================================
//  WiFi credentials (tried in order)
// ============================================================
struct WifiCred { const char* ssid; const char* pass; };
const WifiCred WIFI_CREDS[] = {
  //{ "atelier",    "filmwerkstatt" },
  { "FL_hotspot", "filmwerkstatt" }
};
const int WIFI_CRED_COUNT = sizeof(WIFI_CREDS) / sizeof(WIFI_CREDS[0]);
const unsigned long WIFI_CONNECT_TIMEOUT_MS = 8000;  // per SSID

// ============================================================
//  Remote server URLs
// ============================================================
const char* UPDATE_HASH_URL  = "https://telefabi.ch/sleep/embedded/hash.txt";
const char* UPDATE_FW_URL    = "https://telefabi.ch/sleep/embedded/recent.bin";
const char* AUDIO_BASE_URL   = "https://telefabi.ch/sleep/static/audio/audio";
// Audio files expected as audio1.mp3, audio2.mp3, ... on the server

// ============================================================
//  Hardware config (pin assignments, supported formats, etc.)
// ============================================================
#include "hardware.h"
