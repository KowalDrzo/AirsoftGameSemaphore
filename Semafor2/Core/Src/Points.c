/*
 * Points.c
 *
 *  Created on: Apr 15, 2020
 *      Author: KowalDrzo
 */

#include "Points.h"

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

	tm1637DisplayDecimal(int2Time(asgClock.redTime), 1, RED);
	tm1637DisplayDecimal(int2Time(asgClock.blueTime), 1, BLUE);
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

	if(ifRed) ledControl(500, 0, 0);
	else ledControl(0, 0, 500);

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
		if(Button) {

			Button = 0;

			tm1637SetBrightness(5);

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


	if(asgClock.potenValue < 100) addPoint(RED);

	if(asgClock.potenValue > GAME_RANGE - 100) addPoint(BLUE);

	if(asgClock.gameStarted) {

		asgClock.gameModeUp--;

		if(asgClock.gameModeUp == 1) {

			if(asgClock.redTime > asgClock.blueTime) winGame(RED);
			else winGame(BLUE);
		}
	}
}

/***************************************************************************************/

void displayDark(int *czasJasny) {

	if(Button) {
		*czasJasny = 15;
		Button = 0;
	}

	if(*czasJasny > 0) {

		tm1637SetBrightness(asgClock.bright);
		(*czasJasny)--;
	}
	else {

		tm1637SetBrightness(1);
		przyciemniony = 1;
	}
}



