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
#define GAME_MODES 3
#define TIME_STEP 30
#define TIME_MIN 30
#define TIME_MAX 5940
#define MODE_C_BUZZER_PERIOD_S 60
#define MODE_C_BUZZER_ON_MS 3000
#define MODE_C_SCORE_MAX 9999
#define MODE_C_LED_FLASH_MS 80

void readSavedTimesFromFlash(void);

void writeSavedTimesToFlash(void);

void setupAll(void);

#endif /* INC_SETUP_H_ */
