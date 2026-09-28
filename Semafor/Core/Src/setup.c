#include "setup.h"

_Static_assert(sizeof(SavedTimes) % 4 == 0, "SavedTimes must be programmed in 32-bit words");

void readSavedTimesFromFlash(void) {

	memcpy(&savedTimes,
	       (const void *)FLASH_DATA_ADDR,
	       sizeof(savedTimes));

	if (savedTimes.magic != SETTINGS_MAGIC) {
		savedTimes.redTime = 210;
		savedTimes.blueTime = 210;
		savedTimes.bothTime = 210;
		savedTimes.magic = SETTINGS_MAGIC;
	}
}

void writeSavedTimesToFlash(void) {

	if (memcmp(&savedTimes, (const void *)FLASH_DATA_ADDR, sizeof(savedTimes)) == 0) {
		return;
	}

	HAL_FLASH_Unlock();

	FLASH_EraseInitTypeDef erase;

	erase.TypeErase = FLASH_TYPEERASE_PAGES;
	erase.PageAddress = FLASH_DATA_ADDR;
	erase.NbPages = 1;

	uint32_t error;

	HAL_FLASHEx_Erase(&erase, &error);

	uint32_t *ptr = (uint32_t *)&savedTimes;

	for (uint32_t i = 0; i < sizeof(savedTimes) / 4; i++) {
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,
		                  FLASH_DATA_ADDR + i * 4,
		                  ptr[i]);
	}

	HAL_FLASH_Lock();
}

static _Bool both_buttons_pressed(void) {

	if (HAL_GPIO_ReadPin(RED_BUTTON_GPIO_Port, RED_BUTTON_Pin)
		|| HAL_GPIO_ReadPin(BLUE_BUTTON_GPIO_Port, BLUE_BUTTON_Pin)) {
		return 0;
	}

	HAL_Delay(20);
	return !HAL_GPIO_ReadPin(RED_BUTTON_GPIO_Port, RED_BUTTON_Pin)
		&& !HAL_GPIO_ReadPin(BLUE_BUTTON_GPIO_Port, BLUE_BUTTON_Pin);
}

static void buzzerBeep(int value) {

	for (; value > 0; value--) {
		HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, 1);
		HAL_Delay(50);
		HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, 0);
		HAL_Delay(50);
	}
}

static void clampTime(int16_t *time) {

	if (*time < TIME_MIN) {
		*time = TIME_MIN;
	}
	if (*time > TIME_MAX) {
		*time = TIME_MAX;
	}
}

static void adjustTimeLoop(int16_t *time, _Bool showRed, _Bool showBlue) {

	_Bool colon = 0;

	while (!both_buttons_pressed()) {
		if (!HAL_GPIO_ReadPin(RED_BUTTON_GPIO_Port, RED_BUTTON_Pin)) {
			*time -= TIME_STEP;
		} else if (!HAL_GPIO_ReadPin(BLUE_BUTTON_GPIO_Port, BLUE_BUTTON_Pin)) {
			*time += TIME_STEP;
		}

		clampTime(time);

		uint16_t shown = int2Time((uint16_t)*time);
		if (showRed) {
			tm1637DisplayDecimal(shown, colon, RED);
		}
		if (showBlue) {
			tm1637DisplayDecimal(shown, colon, BLUE);
		}

		colon = !colon;
		HAL_Delay(100);
	}

	buzzerBeep(2);
	HAL_Delay(BUTTON_DELAY);
}

/*!
 * \brief Funkcja setTime służy do ustawienia czasu dla poszczególnej drużyny. Jest wywoływana w setupAll
 *
 * \param ifRed - Czy czerwona drużyna, czy może niebieska.
 *
 * Funkcja nie zwraca żadnych wartości.
 */

static void setTime(_Bool ifRed) {

	HAL_Delay(250);
	dispBlink(asgClock.bright, ifRed);
	adjustTimeLoop(ifRed ? &asgClock.redTime : &asgClock.blueTime, ifRed, !ifRed);
}

static void setAllTime(void) {

	HAL_Delay(250);
	if (savedTimes.bothTime > 0 && savedTimes.bothTime < 6000) {
		asgClock.matchTime = savedTimes.bothTime;
	}

	adjustTimeLoop(&asgClock.matchTime, 1, 1);

	asgClock.redTime = 1;
	asgClock.blueTime = 1;

	savedTimes.bothTime = asgClock.matchTime;
}

static void setGameMode(void) {

	unsigned char tabA[] = {16, 10, 16, 12};
	unsigned char tabB[] = {16, 11, 16, 12};

	tm1637SetBrightness(asgClock.bright);

	tm1637DisplayTab(tabA, 0, RED);
	tm1637DisplayTab(tabB, 0, BLUE);

	while (1) {
		_Bool redDown = !HAL_GPIO_ReadPin(RED_BUTTON_GPIO_Port, RED_BUTTON_Pin);
		_Bool blueDown = !HAL_GPIO_ReadPin(BLUE_BUTTON_GPIO_Port, BLUE_BUTTON_Pin);

		if (!redDown && !blueDown) {
			continue;
		}

		HAL_Delay(80);
		redDown = !HAL_GPIO_ReadPin(RED_BUTTON_GPIO_Port, RED_BUTTON_Pin);
		blueDown = !HAL_GPIO_ReadPin(BLUE_BUTTON_GPIO_Port, BLUE_BUTTON_Pin);

		if (redDown && blueDown) {
			asgClock.gameMode = GAME_MODE_C;
			break;
		}
		if (redDown) {
			asgClock.gameMode = GAME_MODE_A;
			break;
		}
		if (blueDown) {
			asgClock.gameMode = GAME_MODE_B;
			break;
		}
	}

	buzzerBeep(2);
	HAL_Delay(BUTTON_DELAY);
	while (!HAL_GPIO_ReadPin(RED_BUTTON_GPIO_Port, RED_BUTTON_Pin)
		|| !HAL_GPIO_ReadPin(BLUE_BUTTON_GPIO_Port, BLUE_BUTTON_Pin)) {
	}
	asgClock.bright = 5;
}

/*!
 * \brief Funkcja setupAll służy do ustawienia stanu pinów oraz czasu dla drużyn
 *
 * Funkcja nie przyjmuje ani nie zwraca żadnych wartości.
 */

void setupAll(void) {

	structureInit();

	HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, 1);
	HAL_Delay(750);
	HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, 0);

	setGameMode();

	if (asgClock.gameMode == GAME_MODE_A) {
		if (savedTimes.redTime > 0 && savedTimes.redTime < 6000
			&& savedTimes.blueTime > 0 && savedTimes.blueTime < 6000) {

			asgClock.redTime = savedTimes.redTime;
			asgClock.blueTime = savedTimes.blueTime;
		}

		setTime(RED);
		setTime(BLUE);

		savedTimes.redTime = asgClock.redTime;
		savedTimes.blueTime = asgClock.blueTime;
	} else if (asgClock.gameMode == GAME_MODE_B) {
		setAllTime();
	}

	if (asgClock.gameMode != GAME_MODE_C) {
		if (!asgClock.redTime) {
			asgClock.redTime++;
		}
		if (!asgClock.blueTime) {
			asgClock.blueTime++;
		}
	}

	HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, 1);
	HAL_Delay(500);
	HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, 0);

	if (asgClock.gameMode == GAME_MODE_C) {
		tm1637DisplayDecimal(0, 0, RED);
		tm1637DisplayDecimal(0, 0, BLUE);
		GPIOA->BSRR = (uint32_t)(LED_R_Pin | LED_B_Pin | LED_G_Pin) << 16;
	} else {
		tm1637DisplayDecimal(int2Time((uint16_t)asgClock.redTime), 1, RED);
		tm1637DisplayDecimal(int2Time((uint16_t)asgClock.blueTime), 1, BLUE);
	}
}
