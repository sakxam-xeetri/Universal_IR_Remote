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
 * Send a 32-bit NEC IR code.
 *
 * @param code  The NEC code in standard notation:
 *              0xAABBCCDD  where AA=Address, BB=~Address,
 *              CC=Command, DD=~Command
 *              Example: 0x00FF20DF  (Red button)
 * @return true on success
 */
bool sendIRCode(uint32_t code) {
    uint8_t addr    = (code >> 24) & 0xFF;
    uint8_t addrInv = (code >> 16) & 0xFF;
    uint8_t cmd     = (code >>  8) & 0xFF;
    uint8_t cmdInv  =  code        & 0xFF;

    Serial.printf("[IR] Sending 0x%08X  Addr:0x%02X  Cmd:0x%02X\n",
                  code, addr, cmd);

    // Standard NEC: complements match → use clean sendNEC()
    if (addrInv == (uint8_t)(~addr) && cmdInv == (uint8_t)(~cmd)) {
        IrSender.sendNEC(addr, cmd, 2);
    } else {
        // Non-standard frame → byte-swap to raw LSB-first format
        uint32_t raw = __builtin_bswap32(code);
        IrSender.sendNECRaw(raw, 2);
        Serial.println(F("[IR] (sent as raw non-standard NEC)"));
    }

    return true;
}
