/*
 * =============================================================
 *
 *   ESP32 Universal IR Remote Controller
 *   ────────────────────────────────────
 *   A WiFi-based IR blaster that replaces a 24-key RGB LED
 *   strip remote.  Connects to your home WiFi so every device
 *   on the network can control the RGB strip via a browser.
 *
 *   Hardware:
 *     • ESP32 Dev Module
 *     • IR LED on GPIO 4
 *
 *   Libraries required (install via Arduino Library Manager):
 *     • IRremote  by Armin Joachimsmeyer  (v4.x)
 *     • ESPmDNS   (built-in)
 *
 *   Built-in ESP32 libraries used (no extra install needed):
 *     • WiFi.h
 *     • WebServer.h
 *     • Preferences.h
 *     • ESPmDNS.h
 *
 *   Author:  Sakshyam Bastakoti
 *   Website: sakshyambastakoti.com.np
 *   License: MIT
 *
 * =============================================================
 */

#include "config.h"

#include <WiFi.h>
#include <ESPmDNS.h>

#include "wifi_manager.h"
#include "ir_sender.h"
#include "button_storage.h"
#include "web_handler.h"

// ═══════════════════════════════════════════════════════════
//  SETUP
// ═══════════════════════════════════════════════════════════
void setup() {
    Serial.begin(115200);
    delay(300);

    Serial.println();
    Serial.println(F("╔══════════════════════════════════════════╗"));
    Serial.println(F("║   ESP32 Universal IR Remote Controller   ║"));
    Serial.println(F("║   NEC 32-bit  •  24-Key RGB LED Remote   ║"));
    Serial.println(F("╚══════════════════════════════════════════╝"));
    Serial.println();

    // ── 1. Initialise IR transmitter ────────────────────────
    initIR();

    // ── 2. Initialise NVS button storage ────────────────────
    initStorage();

    // ── 3. Check for WiFi reset (hold BOOT button) ──────────
    pinMode(WIFI_RESET_PIN, INPUT_PULLUP);
    delay(100);
    if (digitalRead(WIFI_RESET_PIN) == LOW) {
        Serial.println(F("[WiFi] BOOT button held — erasing WiFi credentials"));
        wifiManagerReset();
        delay(1000);
    }

    // ── 4. Connect via WiFi Manager ─────────────────────────
    //    Uses saved credentials, or launches config portal.
    wifiManagerConnect();

    Serial.println();
    Serial.println(F("[WiFi] Connected!"));
    Serial.print(F("        SSID : "));  Serial.println(WiFi.SSID());
    Serial.print(F("        IP   : "));  Serial.println(WiFi.localIP());
    Serial.print(F("        RSSI : "));  Serial.print(WiFi.RSSI());
    Serial.println(F(" dBm"));
    Serial.println();

    // ── 5. Start mDNS (http://irremote.local) ──────────────
    if (MDNS.begin(WIFI_HOSTNAME)) {
        MDNS.addService("http", "tcp", 80);
        Serial.print(F("[mDNS] http://"));
        Serial.print(WIFI_HOSTNAME);
        Serial.println(F(".local"));
    } else {
        Serial.println(F("[mDNS] Failed to start (use IP instead)"));
    }

    // ── 6. Start web server ─────────────────────────────────
    setupWebServer();

    Serial.println();
    Serial.println(F("════════════════════════════════════════════"));
    Serial.println(F("  System ready! Open in any browser:"));
    Serial.print(F("  http://"));
    Serial.println(WiFi.localIP());
    Serial.print(F("  http://"));
    Serial.print(WIFI_HOSTNAME);
    Serial.println(F(".local"));
    Serial.println(F("════════════════════════════════════════════"));
    Serial.println();
}

// ═══════════════════════════════════════════════════════════
//  LOOP
// ═══════════════════════════════════════════════════════════
void loop() {
    handleWebServer();

    // Auto-reconnect if WiFi drops
    static unsigned long lastCheck = 0;
    if (millis() - lastCheck > 10000) {
        lastCheck = millis();
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println(F("[WiFi] Connection lost – reconnecting..."));
            WiFi.reconnect();
        }
    }
}
