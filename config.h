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

// ── NEC Protocol 24-Key RGB Remote - HEX Codes ────────────
// Format: 0xAABBCCDD  (Address | ~Address | Command | ~Command)
// This remote uses address 0x00 with non-standard complement 0xF7
//
// Row 1: Controls
//   BRIGHT+     0x00F700FF
//   BRIGHT-     0x00F7807F
//   OFF         0x00F740BF
//   ON          0x00F7C03F
//
// Row 2: Primary Colors
//   RED         0x00F720DF
//   GREEN       0x00F7A05F
//   BLUE        0x00F7609F
//   WHITE       0x00F7E01F
//
// Row 3: Color Set 2
//   ORANGE      0x00F710EF
//   LT GREEN    0x00F7906F
//   DK BLUE     0x00F750AF
//   FLASH       0x00F7D02F
//
// Row 4: Color Set 3
//   DK ORANGE   0x00F730CF
//   CYAN        0x00F7B04F
//   PURPLE      0x00F7708F
//   STROBE      0x00F7F00F
//
// Row 5: Color Set 4
//   YELLOW      0x00F708F7
//   LT BLUE     0x00F78877
//   PINK        0x00F748B7
//   FADE        0x00F7C837
//
// Row 6: Color Set 5
//   LT YELLOW   0x00F728D7
//   SKY BLUE    0x00F7A857
//   LT PINK     0x00F76897
//   SMOOTH      0x00F7E817

#endif
