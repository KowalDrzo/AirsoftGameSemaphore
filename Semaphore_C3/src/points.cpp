#include "points.h"

/*!
 * \brief Funkcja int2Time służy do konwersji liczby sekund na liczbę w formacie MMSS
 *
 * \param integer - liczba sekund.
 *
 * \return min*100+sec - czas w formacie MMSS.
 */

uint16_t int2Time(uint16_t integer) {

	uint16_t min, sec;
	min = integer / 60;
	sec = integer % 60;

	return min*100 + sec;
}

/***************************************************************************************/

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

	tmRed.display(int2Time(asgClock.redTime));
	tmRed.colonOn();
	tmBlu.display(int2Time(asgClock.blueTime));
	tmBlu.colonOn();
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

	tmRed.display(int2Time(asgClock.redTime));
	tmRed.colonOn();
	tmBlu.display(int2Time(asgClock.blueTime));
	tmBlu.colonOn();

	for(int8_t i = 0; i < 8; i++) {
		digitalWrite(BUZZER_PIN, 1);
		vTaskDelay(500);
		digitalWrite(BUZZER_PIN, 0);
		vTaskDelay(500);
	}

	tmRed.setBrightness(1);
	tmBlu.setBrightness(1);
	while(1) {

		vTaskDelay(50);
		if(Button) {

			Button = 0;

			tmRed.setBrightness(5);
			tmBlu.setBrightness(5);

			vTaskDelay(8000);
			tmRed.setBrightness(1);
			tmBlu.setBrightness(1);
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

		tmRed.setBrightness(asgClock.bright);
		tmBlu.setBrightness(asgClock.bright);
		(*czasJasny)--;
	}
	else {

		tmRed.setBrightness(1);
		tmBlu.setBrightness(1);
		przyciemniony = 1;
	}
}
