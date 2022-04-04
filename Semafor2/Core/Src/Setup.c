/*
 * Setup.c
 *
 *  Created on: Apr 13, 2020
 *      Author: KowalDrzo
 */

#include "Setup.h"

#define BUTTON_DELAY 500
#define GAME_MODES 2

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

	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);

	for(i = 0; i<250; i++) {
		ledControl(0, i, 0);
		HAL_Delay(3);
	}

	for(; i > 0; i--) {
		ledControl(0, i, 0);
		HAL_Delay(3);
	}
	ledControl(0, 0, 0);

	setBrightness();

	if(!asgClock.gameModeUp) {
		setTime(RED);
		setTime(BLUE);
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

/***************************************************************************************/

/*!
 * \brief Funkcja setTime służy do ustawienia czasu dla poszczególnej drużyny. Jest wywoływana w setupAll
 *
 * \param ifRed - Czy czerwona drużyna, czy może niebieska.
 *
 * Funkcja nie zwraca żadnych wartości.
 */

void setTime(_Bool ifRed) {

	HAL_Delay(250);
	uint16_t stare = TIM2->CNT;

	dispBlink(ifRed);

	uint32_t tickStart = HAL_GetTick();
	while(!Button) {


		int TimerDif = TIM2->CNT - stare;
		stare = TIM2->CNT;

		if(ifRed) asgClock.redTime -= ((int8_t)TimerDif)*10;
		else asgClock.blueTime -= ((int8_t)TimerDif)*10;


		if(asgClock.redTime < 0) asgClock.redTime = 0;
		if(asgClock.redTime > 5940) asgClock.redTime = 5940;

		if(asgClock.blueTime < 0) asgClock.blueTime = 0;
		if(asgClock.blueTime > 5940) asgClock.blueTime = 5940;



		if(HAL_GetTick() - tickStart > 250 * uwTickFreq && HAL_GetTick() - tickStart <= 500 * uwTickFreq) {

			if(ifRed) tm1637DisplayDecimal(int2Time(asgClock.redTime), 0, ifRed);
			else tm1637DisplayDecimal(int2Time(asgClock.blueTime), 0, ifRed);
		}

		else if(HAL_GetTick() - tickStart > 500 * uwTickFreq) {

			if(ifRed) tm1637DisplayDecimal(int2Time(asgClock.redTime), 1, ifRed);
			else tm1637DisplayDecimal(int2Time(asgClock.blueTime), 1, ifRed);
			tickStart = HAL_GetTick();
		}
	}

	buzzerBeep(2);
	HAL_Delay(BUTTON_DELAY);
	Button = 0;
}

/***************************************************************************************/

void setAllTime(void) {

	HAL_Delay(250);
	uint16_t stare = TIM2->CNT;

	uint32_t tickStart = HAL_GetTick();
	while(!Button) {


		int TimerDif = TIM2->CNT - stare;
		stare = TIM2->CNT;

		asgClock.gameModeUp -= ((int8_t)TimerDif)*10;


		if(asgClock.gameModeUp < 10) asgClock.gameModeUp = 10;
		if(asgClock.gameModeUp > 5940) asgClock.gameModeUp = 5940;


		if(HAL_GetTick() - tickStart > 250 * uwTickFreq && HAL_GetTick() - tickStart <= 500 * uwTickFreq) {

			tm1637DisplayDecimal(int2Time(asgClock.gameModeUp), 0, RED);
			tm1637DisplayDecimal(int2Time(asgClock.gameModeUp), 0, BLUE);
		}

		else if(HAL_GetTick() - tickStart > 500 * uwTickFreq) {

			tm1637DisplayDecimal(int2Time(asgClock.gameModeUp), 1, RED);
			tm1637DisplayDecimal(int2Time(asgClock.gameModeUp), 1, BLUE);
			tickStart = HAL_GetTick();
		}
	}

	buzzerBeep(2);
	HAL_Delay(BUTTON_DELAY);
	Button = 0;

	asgClock.redTime = 1;
	asgClock.blueTime = 1;
	asgClock.bright -= 5;
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

		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, B);
		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, R);
		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, G);
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

	HAL_Delay(1);
	if(asgClock.ledGoinBack) asgClock.ledState -= 4;
	else asgClock.ledState += 4;
}

/***************************************************************************************/

void setBrightness(void) {

	uint16_t stare = TIM2->CNT;
	unsigned char Tab[] = {0, 0, 17, 17};

	while(!Button) {


		int TimerDif = TIM2->CNT - stare;
		stare = TIM2->CNT;

		asgClock.bright -= ((int8_t)TimerDif)/4;


		if(asgClock.bright < 1) asgClock.bright = 1;
		if(asgClock.bright > 10) asgClock.bright = 10;


		if(asgClock.bright < 6) {
			tm1637SetBrightness(asgClock.bright);
			asgClock.gameModeUp = 0;
			Tab[0] = asgClock.bright;
			Tab[1] = 10;
		}
		else {
			tm1637SetBrightness(asgClock.bright - 5);
			asgClock.gameModeUp = 1;
			Tab[0] = asgClock.bright - 5;
			Tab[1] = 11;
		}

		tm1637DisplayTab(Tab, 0, RED);
		tm1637DisplayTab(Tab, 0, BLUE);
		HAL_Delay(50);
	}

	buzzerBeep(2);
	HAL_Delay(BUTTON_DELAY);
	Button = 0;
}

/***************************************************************************************/

void buzzerBeep(int value) {

	for(; value > 0; value--) {

		BUZZER_GPIO_Port->ODR |= BUZZER_Pin;
		HAL_Delay(50);
		BUZZER_GPIO_Port->ODR &= ~BUZZER_Pin;
		HAL_Delay(50);
	}

}
