/*
 * Setup.h
 *
 *  Created on: Apr 13, 2020
 *      Author: KowalDrzo
 */

#ifndef INC_SETUP_H_
#define INC_SETUP_H_

#include "tim.h"
#include "Keybrd.h"

#define GAME_RANGE 1000

void setupAll(void);

void ledControl(uint16_t R, uint16_t G, uint16_t B);

void sterLed(void);

void setGameMode(void);

#endif /* INC_SETUP_H_ */
