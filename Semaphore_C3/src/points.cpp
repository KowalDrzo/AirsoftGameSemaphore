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

	tmRed.display(val2Str(int2Time(asgClock.redTime)));
	tmRed.colonOn();
	tmBlu.display(val2Str(int2Time(asgClock.blueTime)));
	tmBlu.colonOn();

	for(int8_t i = 0; i < 8; i++) {
		digitalWrite(BUZZER_PIN, 1);
		vTaskDelay(500);
		digitalWrite(BUZZER_PIN, 0);
		vTaskDelay(500);
	}

	tmRed.setBrightness(1);
	tmBlu.setBrightness(1);
	tmRed.colonOn();
	tmBlu.colonOn();
	while(1) {

		vTaskDelay(50);
		if(!digitalRead(RED_BUTTON_PIN) || !digitalRead(BLU_BUTTON_PIN)) {

			tmRed.setBrightness(5);
			tmBlu.setBrightness(5);
			tmRed.colonOn();
			tmBlu.colonOn();

			vTaskDelay(8000);
			tmRed.setBrightness(1);
			tmBlu.setBrightness(1);
			tmRed.colonOn();
			tmBlu.colonOn();
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

	tmRed.display(val2Str(int2Time(asgClock.redTime)));
	tmRed.colonOn();
	tmBlu.display(val2Str(int2Time(asgClock.blueTime)));
	tmBlu.colonOn();

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

	if(czasJasny > 0) {

		tmRed.setBrightness(asgClock.bright);
		tmBlu.setBrightness(asgClock.bright);
		przyciemniony = 0;
	}
	else {

		tmRed.setBrightness(1);
		tmBlu.setBrightness(1);
		przyciemniony = 1;
	}
}
