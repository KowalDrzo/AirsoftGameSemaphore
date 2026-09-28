#include "asg_clock.h"

AirsoftClock asgClock;
SavedTimes savedTimes;

/*!
 * \brief Funkcja structureInit służy do ustawienia początkowych wartości zmiennych z globalnej struktury
 *
 * Funkcja nie przyjmuje ani nie zwraca żadnych wartości.
 */

void structureInit(void) {

	asgClock.bright = 5;
	asgClock.blueTime = 0;
	asgClock.redTime = 0;
	asgClock.matchTime = 0;
	asgClock.potenValue = NEUTRAL;
	asgClock.gameMode = 0;
	asgClock.gameStarted = 0;
}
