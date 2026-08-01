#include "points.h"

/*!
 * \brief Funkcja addPoint służy do odjęcia sekundy czasu odpowiedniej drużynie
 *
 * \param ifRed - czy drużyna czerwona, czy może niebieska.
 *
 * Funkcja nie zwraca żadnych wartości.
 */

void addPoint(_Bool ifRed) {

	if(!asgClock.gameModeUp) {

		if(ifRed) {

			if(asgClock.redTime > 0) asgClock.redTime--;
			else winGame(RED);
		}

		else {

			if(asgClock.blueTime > 0) asgClock.blueTime--;
			else winGame(BLUE);
		}
	}

	else {

		if(ifRed) asgClock.redTime++;
		else asgClock.blueTime++;
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

	if(!asgClock.gameModeUp) {
		if(ifRed) asgClock.redTime = 0;
		else asgClock.blueTime = 0;
	}

	if(ifRed) {
		HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, 1);
		HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, 0);
	}
	else {
		HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, 0);
		HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, 1);
	}

	tm1637DisplayDecimal(int2Time(asgClock.redTime), 1, RED);
	tm1637DisplayDecimal(int2Time(asgClock.blueTime), 1, BLUE);

	for(int8_t i = 0; i < 8; i++) {

		HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, 1);
		HAL_Delay(500);
		HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, 0);
		HAL_Delay(500);
	}

	tm1637SetBrightness(1);

	while(1) {

		HAL_Delay(50);
		if(!HAL_GPIO_ReadPin(RED_BUTTON_GPIO_Port, RED_BUTTON_Pin) || !HAL_GPIO_ReadPin(BLUE_BUTTON_GPIO_Port, BLUE_BUTTON_Pin)) {

			tm1637SetBrightness(asgClock.bright);
			HAL_Delay(8000);
			tm1637SetBrightness(1);
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

	asgClock.ledGoinBack = 0;

	if(asgClock.potenValue == RED) addPoint(RED);
	if(asgClock.potenValue == BLUE) addPoint(BLUE);

	//tm1637DisplayDecimal(int2Time(asgClock.redTime), 1, RED);
	//tm1637DisplayDecimal(int2Time(asgClock.blueTime), 1, BLUE);

	if(asgClock.gameStarted) {

		asgClock.gameModeUp--;

		if(asgClock.gameModeUp == 1) {

			if(asgClock.redTime > asgClock.blueTime) winGame(RED);
			else winGame(BLUE);
		}
	}
}

/***************************************************************************************/

void displayDark(int czasJasny) {

	if(czasJasny > 0) tm1637SetBrightness(asgClock.bright);
	else tm1637SetBrightness(1);
}

_Bool colon;

void sterLed(void)
{
    _Bool newColon = (HAL_GetTick() % 1000) >= 500;

    if (newColon != colon)
    {
        colon = newColon;

        if (colon)
        {
            tm1637DisplayDecimal(int2Time(asgClock.redTime), 1, RED);
            tm1637DisplayDecimal(int2Time(asgClock.blueTime), 1, BLUE);

            HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
        }
        else
        {
            if (asgClock.potenValue == RED)
            {
                tm1637DisplayDecimal(int2Time(asgClock.redTime), 0, RED);
                HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);
            }
            else if (asgClock.potenValue == BLUE)
            {
                tm1637DisplayDecimal(int2Time(asgClock.blueTime), 0, BLUE);
                HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_SET);
            }
            else
            {
                HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_SET);
            }
        }
    }
}
