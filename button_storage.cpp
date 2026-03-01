/*
 * =============================================================
 *  Button Storage Module - Implementation
 * =============================================================
 *  Custom buttons are stored in ESP32 NVS (Non-Volatile Storage)
 *  using the Preferences library.
 *
 *  Storage layout (namespace "ir_btns"):
 *      "count"   → int,    number of custom buttons
 *      "n0".."nN"→ String, button name
 *      "c0".."cN"→ String, button HEX code
 * =============================================================
 */

#include "button_storage.h"
#include "config.h"
#include <Preferences.h>

// ── Initialise NVS namespace ────────────────────────────────
void initStorage() {
    Preferences p;
    p.begin(PREF_NAMESPACE, false);
    if (!p.isKey("count")) {
        p.putInt("count", 0);
    }
    int count = p.getInt("count", 0);
    p.end();
    Serial.printf("[Storage] Initialised – %d custom button(s) saved\n", count);
}

// ── Read helpers ────────────────────────────────────────────
int getCustomButtonCount() {
    Preferences p;
    p.begin(PREF_NAMESPACE, true);        // read-only
    int count = p.getInt("count", 0);
    p.end();
    return count;
}

CustomButton getCustomButton(int index) {
    CustomButton btn;
    btn.id = index;
    Preferences p;
    p.begin(PREF_NAMESPACE, true);
    btn.name    = p.getString(("n" + String(index)).c_str(), "");
    btn.hexCode = p.getString(("c" + String(index)).c_str(), "");
    p.end();
    return btn;
}

// ── Add a new custom button ─────────────────────────────────
bool addCustomButton(const String& name, const String& hexCode) {
    Preferences p;
    p.begin(PREF_NAMESPACE, false);
    int count = p.getInt("count", 0);

    if (count >= MAX_CUSTOM_BUTTONS) {
        p.end();
        Serial.println(F("[Storage] Cannot add – storage full"));
        return false;
    }

    p.putString(("n" + String(count)).c_str(), name);
    p.putString(("c" + String(count)).c_str(), hexCode);
    p.putInt("count", count + 1);
    p.end();

    Serial.printf("[Storage] Added button #%d: %s → 0x%s\n",
                  count, name.c_str(), hexCode.c_str());
    return true;
}

// ── Edit an existing custom button ──────────────────────────
bool editCustomButton(int index, const String& name, const String& hexCode) {
    Preferences p;
    p.begin(PREF_NAMESPACE, false);
    int count = p.getInt("count", 0);

    if (index < 0 || index >= count) {
        p.end();
        return false;
    }

    p.putString(("n" + String(index)).c_str(), name);
    p.putString(("c" + String(index)).c_str(), hexCode);
    p.end();

    Serial.printf("[Storage] Edited button #%d: %s → 0x%s\n",
                  index, name.c_str(), hexCode.c_str());
    return true;
}

// ── Delete a custom button (shifts remaining entries down) ──
bool deleteCustomButton(int index) {
    Preferences p;
    p.begin(PREF_NAMESPACE, false);
    int count = p.getInt("count", 0);

    if (index < 0 || index >= count) {
        p.end();
        return false;
    }

    // Shift subsequent buttons down by one
    for (int i = index; i < count - 1; i++) {
        String srcN = "n" + String(i + 1);
        String srcC = "c" + String(i + 1);
        String dstN = "n" + String(i);
        String dstC = "c" + String(i);
        p.putString(dstN.c_str(), p.getString(srcN.c_str(), ""));
        p.putString(dstC.c_str(), p.getString(srcC.c_str(), ""));
    }

    // Remove the last (now-duplicate) entry
    p.remove(("n" + String(count - 1)).c_str());
    p.remove(("c" + String(count - 1)).c_str());
    p.putInt("count", count - 1);
    p.end();

    Serial.printf("[Storage] Deleted button #%d – %d remaining\n",
                  index, count - 1);
    return true;
}

// ── Return all custom buttons as a JSON array string ────────
String getCustomButtonsJSON() {
    Preferences p;
    p.begin(PREF_NAMESPACE, true);
    int count = p.getInt("count", 0);

    String json = "[";
    for (int i = 0; i < count; i++) {
        if (i > 0) json += ",";
        String name = p.getString(("n" + String(i)).c_str(), "");
        String code = p.getString(("c" + String(i)).c_str(), "");
        // Escape double-quotes in button name
        name.replace("\"", "\\\"");
        json += "{\"id\":" + String(i)
              + ",\"name\":\"" + name
              + "\",\"code\":\"" + code + "\"}";
    }
    json += "]";
    p.end();
    return json;
}
