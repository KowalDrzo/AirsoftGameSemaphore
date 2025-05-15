#ifndef SETUP_H
#define SETUP_H

#include <Arduino.h>
#include "keybrd.h"

extern _Bool przyciemniony;

void setupAll(void);

void ledControl(uint16_t R, uint16_t G, uint16_t B);

void sterLed(void);

void setGameMode(void);

uint16_t int2Time(uint16_t integer);

#endif
