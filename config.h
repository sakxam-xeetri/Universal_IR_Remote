/*
 * =============================================================
 *  ESP32 Universal IR Remote Controller - Configuration
 * =============================================================
 *  Edit this file to change WiFi settings, IR pin, or defaults.
 * =============================================================
 */

#ifndef CONFIG_H
#define CONFIG_H

// ── WiFi Settings ───────────────────────────────────────────
// Credentials are configured via the WiFi Manager portal.
// On first boot, connect to the "RGB_IR_Setup" WiFi network
// and pick your home network from the list.
#define WIFI_CONNECT_TIMEOUT  15    // Seconds to wait for connection
#define WIFI_HOSTNAME   "irremote"  // mDNS hostname (http://irremote.local)
#define WIFI_RESET_PIN  0           // Hold BOOT button at startup to reset WiFi

// ── IR Transmitter Settings ─────────────────────────────────
#define IR_SEND_PIN     4           // GPIO pin connected to IR LED

// ── Web Server Settings ─────────────────────────────────────
#define WEB_SERVER_PORT 80

// ── Storage Settings ────────────────────────────────────────
#define MAX_CUSTOM_BUTTONS  50      // Maximum number of custom buttons
#define PREF_NAMESPACE      "ir_btns"

// ── NEC Protocol 24-Key RGB Remote - Standard HEX Codes ────
// Format: 0xAABBCCDD  (Address | ~Address | Command | ~Command)
//
// Row 1: Controls
//   BRIGHT+     0x00FF00FF
//   BRIGHT-     0x00FF807F
//   OFF         0x00FF40BF
//   ON          0x00FFC03F
//
// Row 2: Primary Colors
//   RED         0x00FF20DF
//   GREEN       0x00FFA05F
//   BLUE        0x00FF609F
//   WHITE       0x00FFE01F
//
// Row 3: Color Set 2
//   ORANGE      0x00FF10EF
//   LT GREEN    0x00FF906F
//   DK BLUE     0x00FF50AF
//   FLASH       0x00FFD02F
//
// Row 4: Color Set 3
//   DK ORANGE   0x00FF30CF
//   CYAN        0x00FFB04F
//   PURPLE      0x00FF708F
//   STROBE      0x00FFF00F
//
// Row 5: Color Set 4
//   YELLOW      0x00FF08F7
//   LT BLUE     0x00FF8877
//   PINK        0x00FF48B7
//   FADE        0x00FFC837
//
// Row 6: Color Set 5
//   LT YELLOW   0x00FF28D7
//   SKY BLUE    0x00FFA857
//   LT PINK     0x00FF6897
//   SMOOTH      0x00FFE817

#endif
