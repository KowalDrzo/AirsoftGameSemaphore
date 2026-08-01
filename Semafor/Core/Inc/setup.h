#ifndef INC_SETUP_H_
#define INC_SETUP_H_

#include "asg_clock.h"
#include "stm32f0xx_hal.h"
#include "gpio.h"
#include "Tm1637.h"

#include <string.h>

#define FLASH_DATA_ADDR 0x0800FC00
#define SETTINGS_MAGIC 2137
#define BUTTON_DELAY 500
#define GAME_MODES 2

void readSavedTimesFromFlash();

void writeSavedTimesToFlash();

void setupAll();


#endif /* INC_SETUP_H_ */
