#include "setup.h"

void readSavedTimesFromFlash() {

	memcpy(&savedTimes,
	       (const void *)FLASH_DATA_ADDR,
	       sizeof(savedTimes));

	if(savedTimes.magic != SETTINGS_MAGIC) {

		savedTimes.redTime = 210;
	    savedTimes.blueTime = 210;
	    savedTimes.bothTime = 210;
	    savedTimes.magic = SETTINGS_MAGIC;
	}
}

void writeSavedTimesToFlash() {

	HAL_FLASH_Unlock();

	FLASH_EraseInitTypeDef erase;

	erase.TypeErase = FLASH_TYPEERASE_PAGES;
	erase.PageAddress = FLASH_DATA_ADDR;
	erase.NbPages = 1;

	uint32_t error;

	HAL_FLASHEx_Erase(&erase, &error);

	uint32_t *ptr = (uint32_t *)&savedTimes;

	for(uint32_t i=0; i<sizeof(savedTimes)/4; i++)
	{
	    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,
	                      FLASH_DATA_ADDR + i*4,
	                      ptr[i]);
	}

	HAL_FLASH_Lock();
}

_Bool both_buttons_pressed() {

	if (!HAL_GPIO_ReadPin(RED_BUTTON_GPIO_Port, RED_BUTTON_Pin) || !HAL_GPIO_ReadPin(BLUE_BUTTON_GPIO_Port, BLUE_BUTTON_Pin)) {

		for (uint8_t i = 0; i < 20; i++) {
			HAL_Delay(10);
			if (!HAL_GPIO_ReadPin(RED_BUTTON_GPIO_Port, RED_BUTTON_Pin) && !HAL_GPIO_ReadPin(BLUE_BUTTON_GPIO_Port, BLUE_BUTTON_Pin)) {
				return 1;
			}
		}
	}

	return 0;
}

void buzzerBeep(int value) {

	for(; value > 0; value--) {

		HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, 1);
		HAL_Delay(50);
		HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, 0);
		HAL_Delay(50);
	}

}

/*!
 * \brief Funkcja setTime służy do ustawienia czasu dla poszczególnej drużyny. Jest wywoływana w setupAll
 *
 * \param ifRed - Czy czerwona drużyna, czy może niebieska.
 *
 * Funkcja nie zwraca żadnych wartości.
 */

void setTime(_Bool ifRed) {

	HAL_Delay(250);
	dispBlink(asgClock.bright, ifRed);

	_Bool colon;
	while (!both_buttons_pressed()) {

		if (!HAL_GPIO_ReadPin(RED_BUTTON_GPIO_Port, RED_BUTTON_Pin)) {

			if(ifRed) asgClock.redTime -= 30;
			else asgClock.blueTime -= 30;
		}
		else if (!HAL_GPIO_ReadPin(BLUE_BUTTON_GPIO_Port, BLUE_BUTTON_Pin)) {

			if(ifRed) asgClock.redTime += 30;
			else asgClock.blueTime += 30;
		}

		if(asgClock.redTime < 30) asgClock.redTime = 30;
		if(asgClock.redTime > 5940) asgClock.redTime = 5940;
		if(asgClock.blueTime < 30) asgClock.blueTime = 30;
		if(asgClock.blueTime > 5940) asgClock.blueTime = 5940;

		if(ifRed) {

			if (colon) {
				tm1637DisplayDecimal(int2Time(asgClock.redTime), colon, ifRed);
				colon = 0;
			}
			else {
				tm1637DisplayDecimal(int2Time(asgClock.redTime), colon, ifRed);
				colon = 1;
			}
		}
		else {

			if (colon) {
				tm1637DisplayDecimal(int2Time(asgClock.blueTime), colon, ifRed);
				colon = 0;
			}
			else {
				tm1637DisplayDecimal(int2Time(asgClock.blueTime), colon, ifRed);
				colon = 1;
			}
		}
		HAL_Delay(100);
	}

	buzzerBeep(2);
	HAL_Delay(BUTTON_DELAY);
}

void setAllTime(void) {

	HAL_Delay(250);
	if (savedTimes.bothTime > 0 && savedTimes.bothTime < 6000) {
		asgClock.gameModeUp = savedTimes.bothTime;
	}

	_Bool colon;
	while (!both_buttons_pressed()) {

		if (!HAL_GPIO_ReadPin(RED_BUTTON_GPIO_Port, RED_BUTTON_Pin)) {

			asgClock.gameModeUp -= 30;
		}
		else if (!HAL_GPIO_ReadPin(BLUE_BUTTON_GPIO_Port, BLUE_BUTTON_Pin)) {

			asgClock.gameModeUp += 30;
		}

		if(asgClock.gameModeUp < 30) asgClock.gameModeUp = 30;
		if(asgClock.gameModeUp > 5940) asgClock.gameModeUp = 5940;

		if (colon) {
			tm1637DisplayDecimal(int2Time(asgClock.gameModeUp), colon, RED);
			tm1637DisplayDecimal(int2Time(asgClock.gameModeUp), colon, BLUE);
			colon = 0;
		}
		else {
			tm1637DisplayDecimal(int2Time(asgClock.gameModeUp), colon, RED);
			tm1637DisplayDecimal(int2Time(asgClock.gameModeUp), colon, BLUE);
			colon = 1;
		}
		HAL_Delay(100);
	}

	buzzerBeep(2);
	HAL_Delay(BUTTON_DELAY);

	asgClock.redTime = 1;
	asgClock.blueTime = 1;

	savedTimes.bothTime = asgClock.gameModeUp;
}

void setGameMode(void) {

	unsigned char TabA[] = {16, 10, 16, 16};
	unsigned char TabB[] = {16, 11, 16, 16};

	tm1637SetBrightness(asgClock.bright);

	tm1637DisplayTab(TabA, 0, RED);
	tm1637DisplayTab(TabB, 0, BLUE);

	while (1) {

		if (!HAL_GPIO_ReadPin(RED_BUTTON_GPIO_Port, RED_BUTTON_Pin)) {

			asgClock.gameModeUp = 0;
			break;
		}
		else if (!HAL_GPIO_ReadPin(BLUE_BUTTON_GPIO_Port, BLUE_BUTTON_Pin)) {

			asgClock.gameModeUp = 1;
			break;
		}
	}

	buzzerBeep(2);
	HAL_Delay(BUTTON_DELAY);
	asgClock.bright = 5;
}

/*!
 * \brief Funkcja setupAll służy do ustawienia stanu pinów oraz czasu dla drużyn
 *
 * Funkcja nie przyjmuje ani nie zwraca żadnych wartości.
 */

void setupAll(void) {

	uint8_t i;
	structureInit();

	HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, 1);
	HAL_Delay(750);
	HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, 0);

	/*for(i = 0; i<250; i++) {
		ledControl(0, i, 0);
		vTaskDelay(3);
	}

	for(; i > 0; i--) {
		ledControl(0, i, 0);
		vTaskDelay(3);
	}
	ledControl(0, 0, 0);*/

	setGameMode();

	if(!asgClock.gameModeUp) {

		if (savedTimes.redTime > 0 && savedTimes.redTime < 6000
			&& savedTimes.blueTime > 0 && savedTimes.blueTime < 6000) {

			asgClock.redTime = savedTimes.redTime ;
			asgClock.blueTime = savedTimes.blueTime;
		}

		setTime(RED);
		setTime(BLUE);

		savedTimes.redTime = asgClock.redTime;
		savedTimes.blueTime = asgClock.blueTime;
	}
	else setAllTime();

	if(!asgClock.redTime) asgClock.redTime++;
	if(!asgClock.blueTime) asgClock.blueTime++;

	HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, 1);
	HAL_Delay(500);
	HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, 0);

	tm1637DisplayDecimal(int2Time(asgClock.redTime), 1, RED);
	tm1637DisplayDecimal(int2Time(asgClock.blueTime), 1, BLUE);
}

