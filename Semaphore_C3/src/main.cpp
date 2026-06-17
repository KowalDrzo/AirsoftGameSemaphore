#include "pinout.h"
#include "keybrd.h"
#include "points.h"
#include "setup.h"
#include "EEPROM.h"

TM1637 tmRed(RED_CLK_PIN, RED_DIO_PIN);
TM1637 tmBlu(BLU_CLK_PIN, BLU_DIO_PIN);
AirsoftClock asgClock;
SavedTimes savedTimes;
uint32_t timer1 = 0;
int czasJasny = 3;

void setup() {

    delay(100);
    
    EEPROM.begin(sizeof(SavedTimes));
    EEPROM.get(0, savedTimes);

    pinMode(BUZZER_PIN, OUTPUT);
    
    pinMode(RED_BUTTON_PIN, INPUT_PULLUP);
    pinMode(BLU_BUTTON_PIN, INPUT_PULLUP);

    pinMode(LED_R_PIN, OUTPUT);
    pinMode(LED_G_PIN, OUTPUT);
    pinMode(LED_B_PIN, OUTPUT);

    ledcSetup(LED_R_CHANNEL, 1000, 9);
    ledcSetup(LED_G_CHANNEL, 1000, 9);
    ledcSetup(LED_B_CHANNEL, 1000, 9);

    ledcAttachPin(LED_R_PIN, LED_R_CHANNEL);
    ledcAttachPin(LED_G_PIN, LED_G_CHANNEL);
    ledcAttachPin(LED_B_PIN, LED_B_CHANNEL);

    tmRed.begin();
    tmBlu.begin();

    setupAll();

    EEPROM.put(0, savedTimes);
    EEPROM.commit();
}

void loop() {

    if(millis() - timer1 >= 1000) {

        checkPoint(); // Sprawdzanie, która drużyna przejęła cel i reagowanie na to
        displayDark(czasJasny);
        czasJasny--;

        timer1 = millis();
    }

    sterLed();

    if (!digitalRead(RED_BUTTON_PIN)) {
        asgClock.potenValue = 0;
        czasJasny = 15;
    }
    if (!digitalRead(BLU_BUTTON_PIN)) {
        asgClock.potenValue = GAME_RANGE;
        czasJasny = 15;
    }

    vTaskDelay(1);
}
