#ifndef KEYBRD_H
#define KEYBRD_H

#include <Arduino.h>
#include <TM1637.h>

#include "pinout.h"

#define RED 1
#define BLUE 0

#define GAME_RANGE 1000

struct AirsoftClock {

	short int redTime;
	short int blueTime;
	int8_t bright;
	short int potenValue;

	short int ledState;
	_Bool ledGoinBack;
	short int gameModeUp;
	_Bool gameStarted;

};

/* Lista funkcji dostępnych publicznie */

void structureInit(void);

extern _Bool Button;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

extern AirsoftClock asgClock;
extern TM1637 tmRed;
extern TM1637 tmBlu;

#endif
