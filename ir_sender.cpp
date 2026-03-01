/*
 * =============================================================
 *  IR Sender Module - Implementation
 * =============================================================
 *  Uses IRremote library by Armin Joachimsmeyer (v4.x)
 *  NEC 32-bit protocol
 * =============================================================
 */

// config.h MUST be included first — it defines IR_SEND_PIN
// before IRremote.hpp reads it during preprocessing.
#include "config.h"
#include "ir_sender.h"

#include <IRremote.hpp>

void initIR() {
    IrSender.begin(DISABLE_LED_FEEDBACK);
    Serial.print(F("[IR] Sender ready on GPIO "));
    Serial.println(IR_SEND_PIN);
}

/**
 * Send a 32-bit NEC IR code using the raw/old-style API.
 *
 * @param code  Full NEC code, e.g. 0xF720DF (Red button)
 *              Leading zeros are ignored (0x00F720DF == 0xF720DF).
 * @return true on success
 */
bool sendIRCode(uint32_t code) {
    Serial.printf("[IR] Sending 0x%X\n", code);

    // Use the old-style raw API: sendNEC(rawData, bits)
    // This is the same call as: IrSender.sendNEC(0xF720DF, 32)
    // which the user confirmed works with their RGB strip.
    IrSender.sendNEC(code, 32);

    return true;
}
