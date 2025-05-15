#ifndef SETUP_H
#define SETUP_H

#include "keybrd.h"

#define GAME_RANGE 1000

void setupAll(void);

void ledControl(uint16_t R, uint16_t G, uint16_t B);

void sterLed(void);

void setGameMode(void);

#endif
