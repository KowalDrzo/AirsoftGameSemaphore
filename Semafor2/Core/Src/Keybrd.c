/*
 * Keybrd.c
 *
 *  Created on: Apr 12, 2020
 *      Author: KowalDrzo
 */

#include "Keybrd.h"


/***************************************************************************************/

/*!
 * \brief Funkcja structureInit służy do ustawienia początkowych wartości zmiennych z globalnej struktury
 *
 * Funkcja nie przyjmuje ani nie zwraca żadnych wartości.
 */

void structureInit(void) {

	asgClock.bright = 1;
	asgClock.blueTime = 0;
	asgClock.redTime = 0;
	asgClock.ledState = 0;
	asgClock.ledGoinBack = 0;

	asgClock.potenValue = 1500;
	asgClock.gameModeUp = 0;
	asgClock.gameStarted = 0;
}

/***************************************************************************************/

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	if(GPIO_Pin == Przycisk_Pin) Button = 1;
}
