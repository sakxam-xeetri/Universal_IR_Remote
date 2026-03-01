/*
 * =============================================================
 *
 *   ESP32 Universal IR Remote Controller
 *   ────────────────────────────────────
 *   A WiFi-based IR blaster that replaces a 24-key RGB LED
 *   strip remote.  Connects as a soft-AP, serves a mobile-
 *   friendly web UI, and transmits NEC 32-bit IR codes.
 *
 *   Hardware:
 *     • ESP32 Dev Module
 *     • IR LED on GPIO 4
 *
 *   Libraries required (install via Arduino Library Manager):
 *     • IRremote  by Armin Joachimsmeyer  (v4.x)
 *
 *   Built-in ESP32 libraries used (no extra install needed):
 *     • WiFi.h
 *     • WebServer.h
 *     • DNSServer.h
 *     • Preferences.h
 *
 *   Author:  Sakshyam Bastakoti
 *   Website: sakshyambastakoti.com.np
 *   License: MIT
 *
 * =============================================================
 */

#include "config.h"

#include <WiFi.h>

#include "ir_sender.h"
#include "button_storage.h"
#include "web_handler.h"

// ═══════════════════════════════════════════════════════════
//  SETUP
// ═══════════════════════════════════════════════════════════
void setup() {
    Serial.begin(115200);
    delay(300);   // Give the serial monitor time to attach

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

    // ── 3. Start WiFi Access Point ──────────────────────────
    WiFi.mode(WIFI_AP);
    WiFi.setSleep(false);                 // Keep radio active

    bool apOk;
    if (strlen(WIFI_PASS) >= 8) {
        apOk = WiFi.softAP(WIFI_SSID, WIFI_PASS);
    } else {
        apOk = WiFi.softAP(WIFI_SSID);   // Open network
    }

    if (!apOk) {
        Serial.println(F("[WiFi] ERROR – Failed to start AP!"));
        Serial.println(F("[WiFi] Restarting in 3 seconds..."));
        delay(3000);
        ESP.restart();
    }

    delay(100);   // Small settle time for soft-AP

    Serial.println(F("[WiFi] Access Point started"));
    Serial.print(F("        SSID : "));  Serial.println(WIFI_SSID);
    Serial.print(F("        IP   : "));  Serial.println(WiFi.softAPIP());
    Serial.print(F("        Pass : "));
    Serial.println(strlen(WIFI_PASS) >= 8 ? WIFI_PASS : "(open)");
    Serial.println();

    // ── 4. Start web server + captive portal ────────────────
    setupWebServer();

    Serial.println();
    Serial.println(F("════════════════════════════════════════════"));
    Serial.println(F("  System ready!  Connect to WiFi and open"));
    Serial.print(F("  http://"));
    Serial.print(WiFi.softAPIP());
    Serial.println(F("/"));
    Serial.println(F("════════════════════════════════════════════"));
    Serial.println();
}

// ═══════════════════════════════════════════════════════════
//  LOOP
// ═══════════════════════════════════════════════════════════
void loop() {
    handleWebServer();
}
