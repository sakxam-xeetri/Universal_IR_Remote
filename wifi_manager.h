/*
 * =============================================================
 *  WiFi Manager Module - Header
 * =============================================================
 *  Handles WiFi configuration via a captive-portal web UI.
 *  On first boot (or if saved WiFi fails), the ESP32 creates
 *  an AP named "RGB_IR_Setup" where the user picks a network
 *  and enters the password.  Credentials are stored in NVS.
 * =============================================================
 */

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>

/**
 * Attempt to connect using saved credentials.
 * If none are saved, or if connection fails, launch the
 * configuration portal (AP + captive web page).
 *
 * Blocks until WiFi is connected.
 * Returns true once connected, never returns false — if the
 * portal times out it restarts the ESP32.
 */
bool wifiManagerConnect();

/**
 * Erase saved WiFi credentials from NVS.
 * Useful if you want to force the config portal on next boot.
 */
void wifiManagerReset();

#endif
