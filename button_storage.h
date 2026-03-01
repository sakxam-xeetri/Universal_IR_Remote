/*
 * =============================================================
 *  Button Storage Module - Header
 * =============================================================
 *  Manages custom buttons stored in ESP32 NVS (Preferences).
 * =============================================================
 */

#ifndef BUTTON_STORAGE_H
#define BUTTON_STORAGE_H

#include <Arduino.h>

struct CustomButton {
    int     id;
    String  name;
    String  hexCode;
};

void   initStorage();
int    getCustomButtonCount();
CustomButton getCustomButton(int index);
bool   addCustomButton(const String& name, const String& hexCode);
bool   editCustomButton(int index, const String& name, const String& hexCode);
bool   deleteCustomButton(int index);
String getCustomButtonsJSON();

#endif
