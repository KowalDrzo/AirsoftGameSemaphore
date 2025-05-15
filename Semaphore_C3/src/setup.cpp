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

	/*HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
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

/*!
 * \brief Funkcja setTime służy do ustawienia czasu dla poszczególnej drużyny. Jest wywoływana w setupAll
 *
 * \param ifRed - Czy czerwona drużyna, czy może niebieska.
 *
 * Funkcja nie zwraca żadnych wartości.
 */

void setTime(_Bool ifRed) {

	vTaskDelay(250);
	uint16_t stare = 0; // TODO

	dispBlink(ifRed);

	uint32_t tickStart = millis();
	while(!Button) {


		int TimerDif = 0; // TODO
		stare = 0; // TODO

		if(ifRed) asgClock.redTime -= ((int8_t)TimerDif)*10;
		else asgClock.blueTime -= ((int8_t)TimerDif)*10;


		if(asgClock.redTime < 0) asgClock.redTime = 0;
		if(asgClock.redTime > 5940) asgClock.redTime = 5940;

		if(asgClock.blueTime < 0) asgClock.blueTime = 0;
		if(asgClock.blueTime > 5940) asgClock.blueTime = 5940;



		if(millis() - tickStart > 250 && millis() - tickStart <= 500) {

			if(ifRed) {
				tmRed.display(int2Time(asgClock.redTime));
				tmRed.colonOff();
			}
			else {
				tmBlu.display(int2Time(asgClock.blueTime));
				tmBlu.colonOff();
			}
		}

		else if(millis() - tickStart > 500) {

			if(ifRed) {
				tmRed.display(int2Time(asgClock.redTime));
				tmRed.colonOn();
			}
			else {
				tmBlu.display(int2Time(asgClock.blueTime));
				tmBlu.colonOn();
			}
			tickStart = millis();
		}
	}

	buzzerBeep(2);
	vTaskDelay(BUTTON_DELAY);
	Button = 0;
}

/***************************************************************************************/

void setAllTime(void) {

	vTaskDelay(250);
	uint16_t stare = 0; // TODO

	uint32_t tickStart = millis();
	while(!Button) {


		int TimerDif = 0; // TODO
		stare = 0; // TODO

		asgClock.gameModeUp -= ((int8_t)TimerDif)*10;


		if(asgClock.gameModeUp < 10) asgClock.gameModeUp = 10;
		if(asgClock.gameModeUp > 5940) asgClock.gameModeUp = 5940;


		if(millis() - tickStart > 250 && millis() - tickStart <= 500) {

			tmRed.display(int2Time(asgClock.gameModeUp));
			tmRed.colonOff();
			tmBlu.display(int2Time(asgClock.gameModeUp));
			tmBlu.colonOff();
		}

		else if(millis() - tickStart > 500) {

			tmRed.display(int2Time(asgClock.gameModeUp));
			tmRed.colonOn();
			tmBlu.display(int2Time(asgClock.gameModeUp));
			tmBlu.colonOn();
			tickStart = millis();
		}
	}

	buzzerBeep(2);
	vTaskDelay(BUTTON_DELAY);
	Button = 0;

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

		/*__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, B);
		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, R);
		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, G);*/
		// TODO
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

	vTaskDelay(1);
	if(asgClock.ledGoinBack) asgClock.ledState -= 4;
	else asgClock.ledState += 4;
}

/***************************************************************************************/

void setGameMode(void) {

	uint16_t stare = 0; // TODO
	unsigned char Tab[] = {17, 0, 17, 17};
	tmRed.setBrightness(asgClock.bright);
	tmBlu.setBrightness(asgClock.bright);

	while(!Button) {


		int TimerDif = 0; // TODO
		stare = 0; // TODO

		asgClock.bright -= ((int8_t)TimerDif)/4;


		if(asgClock.bright < 1) asgClock.bright = 1;
		if(asgClock.bright > 2) asgClock.bright = 2;


		if(asgClock.bright == 1) {
			asgClock.gameModeUp = 0;
			Tab[1] = 10;
		}
		else {
			asgClock.gameModeUp = 1;
			Tab[1] = 11;
		}

		tmRed.displayRawBytes(Tab, 4);
		tmBlu.displayRawBytes(Tab, 4);
		vTaskDelay(50);
	}

	buzzerBeep(2);
	vTaskDelay(BUTTON_DELAY);
	asgClock.bright = 5;
	Button = 0;
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
