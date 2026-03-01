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
 *              Example: 0x00F720DF  (Red button)
 * @return true on success
 */
bool sendIRCode(uint32_t code) {
    uint8_t addr    = (code >> 24) & 0xFF;   // Address byte
    uint8_t addrInv = (code >> 16) & 0xFF;   // ~Address (or non-standard)
    uint8_t cmd     = (code >>  8) & 0xFF;   // Command byte
    uint8_t cmdInv  =  code        & 0xFF;   // ~Command (or non-standard)

    Serial.printf("[IR] Sending 0x%08X  Addr:0x%02X  Cmd:0x%02X\n",
                  code, addr, cmd);

    // Use 16-bit address & command so IRremote sends all 4 bytes
    // exactly as specified — works for both standard and non-standard
    // NEC complements (e.g. address 0x00/0xF7 on many RGB remotes).
    uint16_t address = ((uint16_t)addrInv << 8) | addr;
    uint16_t command = ((uint16_t)cmdInv  << 8) | cmd;
    IrSender.sendNEC(address, command, 2);

    return true;
}
