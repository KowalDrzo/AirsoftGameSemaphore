#include "pinout.h"
#include "keybrd.h"
#include "points.h"
#include "setup.h"

TM1637 tmRed(RED_CLK_PIN, RED_DIO_PIN);
TM1637 tmBlu(BLU_CLK_PIN, BLU_DIO_PIN);
AirsoftClock asgClock;
uint32_t tickStart;
uint16_t stare;
int czasJasny = 0;

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

    setupAll();
    tickStart = millis();
    stare = 0; // TODO
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

    if(millis() - tickStart > 1000) {

        checkPoint(); // Sprawdzanie, która drużyna przejęła cel i reagowanie na to
        displayDark(&czasJasny);

        tickStart = millis();
    }

    sterLed();

    int TimerDif = 0; // TODO
    stare = 0; // TODO

    if(TimerDif) {

        if(przyciemniony) {

            przyciemniony = 0;
            tmRed.setBrightness(asgClock.bright);
            tmBlu.setBrightness(asgClock.bright);
        }
        czasJasny = 20;
    }

    asgClock.potenValue -= (int8_t)TimerDif;

    if(asgClock.potenValue < 0) asgClock.potenValue = 0;
    if(asgClock.potenValue > GAME_RANGE) asgClock.potenValue = GAME_RANGE;
}
