#include "setup.h"

#define BUTTON_DELAY 500
#define GAME_MODES 2

_Bool przyciemniony;

void setTime(_Bool ifRed);
void setAllTime(void);
void buzzerBeep(int value);

/*!
 * \brief Funkcja setupAll służy do ustawienia stanu pinów oraz czasu dla drużyn
 *
 * Funkcja nie przyjmuje ani nie zwraca żadnych wartości.
 */

void setupAll(void) {

	uint8_t i;
	structureInit();

	/*
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);*/
	// TODO

	for(i = 0; i<250; i++) {
		ledControl(0, i, 0);
		vTaskDelay(3);
	}

	for(; i > 0; i--) {
		ledControl(0, i, 0);
		vTaskDelay(3);
	}
	ledControl(0, 0, 0);

	setGameMode();

	if(!asgClock.gameModeUp) {
		setTime(RED);
		setTime(BLUE);
	}
	else setAllTime();

	if(!asgClock.redTime) asgClock.redTime++;
	if(!asgClock.blueTime) asgClock.blueTime++;

	digitalWrite(BUZZER_PIN, 1);
	vTaskDelay(500);
	digitalWrite(BUZZER_PIN, 0);

	tmRed.display(int2Time(asgClock.redTime));
	tmRed.colonOn();
	tmBlu.display(int2Time(asgClock.blueTime));
	tmBlu.colonOn();
}

/***************************************************************************************/

/*!
 * \brief Funkcja dispBlink służy do migania wyświetlaczem
 *
 * \param ifRed - czy funkcja ma dotyczyć wyświetlacza czerwonego, czy może niebieskiego.
 *
 * Funkcja nie zwraca żadnych wartości.
 */

void dispBlink(_Bool ifRed) {

	if (ifRed) {

		tmRed.setBrightness(0);
		delay(100);
		tmRed.setBrightness(asgClock.bright);
		delay(250);
	}
	else {

		tmBlu.setBrightness(0);
		delay(100);
		tmBlu.setBrightness(asgClock.bright);
		delay(250);
	}
}

/***************************************************************************************/

bool both_buttons_pressed() {

	if (!digitalRead(RED_BUTTON_PIN) || !digitalRead(BLU_BUTTON_PIN)) {

		for (uint8_t i = 0; i < 20; i++) {
			vTaskDelay(10);
			if (!digitalRead(RED_BUTTON_PIN) && !digitalRead(BLU_BUTTON_PIN)) {
				return true;
			}
		}
	}

	return false;
}

/***************************************************************************************/

String val2Str(uint16_t value) {

	if (value < 10) return String(" 00") + String(value);
	if (value < 100) return String(" 0") + String(value);
	if (value < 1000) return String(" ") + String(value);

	return String(value);
}

/***************************************************************************************/

/*!
 * \brief Funkcja setTime służy do ustawienia czasu dla poszczególnej drużyny. Jest wywoływana w setupAll
 *
 * \param ifRed - Czy czerwona drużyna, czy może niebieska.
 *
 * Funkcja nie zwraca żadnych wartości.
 */

void setTime(_Bool ifRed) {

	vTaskDelay(250);
	dispBlink(ifRed);

	bool colon;
	while (!both_buttons_pressed()) {

		if (!digitalRead(RED_BUTTON_PIN)) {

			if(ifRed) asgClock.redTime -= 30;
			else asgClock.blueTime -= 30;
		}
		else if (!digitalRead(BLU_BUTTON_PIN)) {

			if(ifRed) asgClock.redTime += 30;
			else asgClock.blueTime += 30;
		}

		if(asgClock.redTime < 30) asgClock.redTime = 30;
		if(asgClock.redTime > 5940) asgClock.redTime = 5940;
		if(asgClock.blueTime < 30) asgClock.blueTime = 30;
		if(asgClock.blueTime > 5940) asgClock.blueTime = 5940;

		if(ifRed) {
			tmRed.display(val2Str(int2Time(asgClock.redTime)));

			if (colon) {
				tmRed.colonOff();
				colon = false;
			}
			else {
				tmRed.colonOn();
				colon = true;
			}
		}
		else {
			tmBlu.display(val2Str(int2Time(asgClock.blueTime)));

			if (colon) {
				tmBlu.colonOff();
				colon = false;
			}
			else {
				tmBlu.colonOn();
				colon = true;
			}
		}
		vTaskDelay(100);
	}

	buzzerBeep(2);
	vTaskDelay(BUTTON_DELAY);
}

/***************************************************************************************/

void setAllTime(void) {

	vTaskDelay(250);

	bool colon;
	while (!both_buttons_pressed()) {

		if (!digitalRead(RED_BUTTON_PIN)) {

			asgClock.gameModeUp -= 30;
		}
		else if (!digitalRead(BLU_BUTTON_PIN)) {

			asgClock.gameModeUp += 30;
		}

		if(asgClock.gameModeUp < 30) asgClock.gameModeUp = 30;
		if(asgClock.gameModeUp > 5940) asgClock.gameModeUp = 5940;

		tmRed.display(val2Str(int2Time(asgClock.gameModeUp)));
		tmBlu.display(val2Str(int2Time(asgClock.gameModeUp)));

		if (colon) {
			tmRed.colonOff();
			tmBlu.colonOff();
			colon = false;
		}
		else {
			tmRed.colonOn();
			tmBlu.colonOn();
			colon = true;
		}
		vTaskDelay(100);
	}

	buzzerBeep(2);
	vTaskDelay(BUTTON_DELAY);

	asgClock.redTime = 1;
	asgClock.blueTime = 1;
}

/***************************************************************************************/

/*!
 * \brief Funkcja ledControl służy do ustawienia koloru i mocy świecenia diody LED za pomocą PWMu
 *
 * \param R - kolor czerwony,
 * \param G - kolor zielony,
 * \param B - kolor niebieski.
 *
 * Funkcja nie zwraca żadnych wartości.
 */

void ledControl(uint16_t R, uint16_t G, uint16_t B) {

	if(R <= 1000 && G <= 1000 && B <= 1000) {

		ledcWrite(LED_R_CHANNEL, R);
		ledcWrite(LED_G_CHANNEL, G);
		ledcWrite(LED_B_CHANNEL, B);
	}
}

/***************************************************************************************/

/*!
 * \brief Funkcja sterLed służy do sterowania diodą LED w zależności od pozycji potencjometru
 *
 * Funkcja nie przyjmuje ani nie zwraca żadnych wartości.
 */

void sterLed(void) {

	if(asgClock.ledState > 999 && !asgClock.ledGoinBack) asgClock.ledGoinBack = 1;
	if(asgClock.ledState <= 0 && asgClock.ledGoinBack) return;

	if(asgClock.potenValue < 100) ledControl(asgClock.ledState, 0, 0);
	else if(asgClock.potenValue > GAME_RANGE - 100) ledControl(0, 0, asgClock.ledState);
	else {
		int ledB = asgClock.ledState;
		if(przyciemniony) ledB /= 20;

		ledControl(ledB, ledB, ledB);
	}

	vTaskDelay(2);
	if(asgClock.ledGoinBack) asgClock.ledState -= 4;
	else asgClock.ledState += 4;
}

/***************************************************************************************/

void setGameMode(void) {

	uint16_t stare = 0; // TODO
	unsigned char Tab[] = "  A ";
	tmRed.setBrightness(asgClock.bright);
	tmBlu.setBrightness(asgClock.bright);

	tmRed.display("  A ");
	tmBlu.display("  b ");

	while (1) {

		if (!digitalRead(RED_BUTTON_PIN)) {

			asgClock.gameModeUp = 0;
			break;
		}
		else if (!digitalRead(BLU_BUTTON_PIN)) {

			asgClock.gameModeUp = 1;
			break;
		}
	}

	buzzerBeep(2);
	vTaskDelay(BUTTON_DELAY);
	asgClock.bright = 5;
}

/***************************************************************************************/

void buzzerBeep(int value) {

	for(; value > 0; value--) {

		digitalWrite(BUZZER_PIN, 1);
		vTaskDelay(50);
		digitalWrite(BUZZER_PIN, 0);
		vTaskDelay(50);
	}

}
