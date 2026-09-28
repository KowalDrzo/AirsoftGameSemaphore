#include "points.h"

#define LED_ALL_PINS (LED_R_Pin | LED_B_Pin | LED_G_Pin)

static int8_t lastBrightness = -1;
static int16_t lastRedTime = -1;
static int16_t lastBlueTime = -1;
static uint16_t redMmss;
static uint16_t blueMmss;

static void ledsOff(void) {
	GPIOA->BSRR = (uint32_t)LED_ALL_PINS << 16;
}

static void setBrightnessCached(int8_t brightness) {
	if (brightness == lastBrightness) {
		return;
	}
	lastBrightness = brightness;
	tm1637SetBrightness(brightness);
}

static uint16_t mmssRed(void) {
	if (asgClock.redTime != lastRedTime) {
		lastRedTime = asgClock.redTime;
		redMmss = int2Time((uint16_t)asgClock.redTime);
	}
	return redMmss;
}

static uint16_t mmssBlue(void) {
	if (asgClock.blueTime != lastBlueTime) {
		lastBlueTime = asgClock.blueTime;
		blueMmss = int2Time((uint16_t)asgClock.blueTime);
	}
	return blueMmss;
}

/*!
 * \brief Funkcja addPoint służy do odjęcia sekundy czasu odpowiedniej drużynie
 *
 * \param ifRed - czy drużyna czerwona, czy może niebieska.
 *
 * Funkcja nie zwraca żadnych wartości.
 */

void addPoint(_Bool ifRed) {

	int16_t *time = ifRed ? &asgClock.redTime : &asgClock.blueTime;

	if (!asgClock.gameMode) {
		if (*time > 0) {
			(*time)--;
		} else {
			winGame(ifRed);
		}
	} else {
		(*time)++;
		asgClock.gameStarted = 1;
	}
}

/***************************************************************************************/

/*!
 * \brief Funkcja winGame służy do zakończenia gry
 *
 * \param ifRed - czy drużyna czerwona, czy może niebieska.
 *
 * Funkcja nie zwraca żadnych wartości.
 */

void winGame(_Bool ifRed) {

	if (!asgClock.gameMode) {
		if (ifRed) {
			asgClock.redTime = 0;
		} else {
			asgClock.blueTime = 0;
		}
	}

	if (ifRed) {
		GPIOA->BSRR = LED_R_Pin | ((uint32_t)(LED_B_Pin | LED_G_Pin) << 16);
	} else {
		GPIOA->BSRR = LED_B_Pin | ((uint32_t)(LED_R_Pin | LED_G_Pin) << 16);
	}

	tm1637DisplayDecimal(mmssRed(), 1, RED);
	tm1637DisplayDecimal(mmssBlue(), 1, BLUE);

	for (int8_t i = 0; i < 8; i++) {
		HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, 1);
		HAL_Delay(500);
		HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, 0);
		HAL_Delay(500);
	}

	setBrightnessCached(1);

	while (1) {
		HAL_Delay(50);
		if (!HAL_GPIO_ReadPin(RED_BUTTON_GPIO_Port, RED_BUTTON_Pin)
			|| !HAL_GPIO_ReadPin(BLUE_BUTTON_GPIO_Port, BLUE_BUTTON_Pin)) {

			setBrightnessCached(asgClock.bright);
			HAL_Delay(8000);
			setBrightnessCached(1);
		}
	}
}

/***************************************************************************************/

/*!
 * \brief Funkcja checkPoint służy do sprawdzenia, która drużyna kontroluje cel (pozycja potencjometru)
 *
 * Funkcja nie przyjmuje ani nie zwraca żadnych wartości.
 */

void checkPoint(void) {

	if (asgClock.potenValue == RED) {
		addPoint(RED);
	} else if (asgClock.potenValue == BLUE) {
		addPoint(BLUE);
	}

	if (asgClock.gameStarted) {
		asgClock.matchTime--;

		if (asgClock.matchTime == 1) {
			if (asgClock.redTime > asgClock.blueTime) {
				winGame(RED);
			} else {
				winGame(BLUE);
			}
		}
	}
}

/***************************************************************************************/

void displayDark(int czasJasny) {

	setBrightnessCached((czasJasny > 0) ? asgClock.bright : 1);
}

void sterLed(void) {

	static uint32_t lastFlip;
	static _Bool colon;

	uint32_t now = HAL_GetTick();
	if ((now - lastFlip) < 500u) {
		return;
	}
	lastFlip = now;
	colon = !colon;

	if (colon) {
		tm1637DisplayDecimal(mmssRed(), 1, RED);
		tm1637DisplayDecimal(mmssBlue(), 1, BLUE);
		ledsOff();
	} else if (asgClock.potenValue == RED) {
		tm1637DisplayDecimal(mmssRed(), 0, RED);
		GPIOA->BSRR = LED_R_Pin;
	} else if (asgClock.potenValue == BLUE) {
		tm1637DisplayDecimal(mmssBlue(), 0, BLUE);
		GPIOA->BSRR = LED_B_Pin;
	} else {
		GPIOA->BSRR = LED_ALL_PINS;
	}
}
