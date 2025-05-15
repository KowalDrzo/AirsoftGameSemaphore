#include <Arduino.h>
#include <TM1637.h>

#include "pinout.h"
#include "keybrd.h"
#include "points.h"
#include "setup.h"

TM1637 tmRed(RED_CLK_PIN, RED_DIO_PIN);
TM1637 tmBlu(BLU_CLK_PIN, BLU_DIO_PIN);

typedef struct {

	short int redTime;
	short int blueTime;
	int8_t bright;
	short int potenValue;

	short int ledState;
	_Bool ledGoinBack;
	short int gameModeUp;
	_Bool gameStarted;

} airsoftClock;

airsoftClock asgClock;

void setup() {

    delay(100);

    pinMode(BUZZER_PIN, OUTPUT);
    
    pinMode(RED_BUTTON_PIN, INPUT_PULLUP);
    pinMode(BLU_BUTTON_PIN, INPUT_PULLUP);

    pinMode(LED_R_PIN, OUTPUT);
    pinMode(LED_G_PIN, OUTPUT);
    pinMode(LED_B_PIN, OUTPUT);

    tmRed.begin();
    /*tmRed.setBrightnessPercent(30);

    tmRed.display(1234);
    delay(1000);*/

    tmBlu.begin();
    /*tmBlu.setBrightnessPercent(30);

    tmBlu.display(2137);
    delay(1000);*/



    int czasJasny = 0;

    setupAll();
    uint32_t tickStart = HAL_GetTick();
    uint16_t stare = TIM2->CNT;
}

void loop() {

    /*digitalWrite(LED_R_PIN, 1);
    digitalWrite(LED_B_PIN, 0);
    tmBlu.colonOn();
    tmRed.colonOn();
    digitalWrite(BUZZER_PIN, 1);
    delay(500);
    digitalWrite(LED_R_PIN, 0);
    digitalWrite(LED_B_PIN, 1);
    tmBlu.colonOff();
    tmRed.colonOff();
    digitalWrite(BUZZER_PIN, 0);
    delay(2000);*/

    if(HAL_GetTick() - tickStart > 1000 * uwTickFreq) {

        checkPoint(); // Sprawdzanie, która drużyna przejęła cel i reagowanie na to
        displayDark(&czasJasny);

        tickStart = HAL_GetTick();
    }

    sterLed();

    int TimerDif = TIM2->CNT - stare;
    stare = TIM2->CNT;

    if(TimerDif) {

        if(przyciemniony) {

            przyciemniony = 0;
            tm1637SetBrightness(asgClock.bright);
        }
        czasJasny = 20;
    }

    asgClock.potenValue -= (int8_t)TimerDif;


    if(asgClock.potenValue < 0) asgClock.potenValue = 0;
    if(asgClock.potenValue > GAME_RANGE) asgClock.potenValue = GAME_RANGE;
}
