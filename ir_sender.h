/*
 * =============================================================
 *  IR Sender Module - Header
 * =============================================================
 */

#ifndef IR_SENDER_H
#define IR_SENDER_H

#include <Arduino.h>

void initIR();
bool sendIRCode(uint32_t code);

#endif
