#include "Tasks.h"

Tasks tasks;
TM1637 redDisplay(RED_CLK_PIN, RED_DIO_PIN);
TM1637 bluDisplay(BLU_CLK_PIN, BLU_DIO_PIN);

uint16_t sec2minSec(int16_t sec) {

    return sec/60*100 + sec%60;
}

/*********************************************************************/

void Tasks::init() {

    ledcSetup(LED_R_PWM_CHANNEL, 1000, 10);
    ledcSetup(LED_G_PWM_CHANNEL, 1000, 10);
    ledcSetup(LED_B_PWM_CHANNEL, 1000, 10);

    ledcAttachPin(LED_R_PIN, LED_R_PWM_CHANNEL);
    ledcAttachPin(LED_G_PIN, LED_G_PWM_CHANNEL);
    ledcAttachPin(LED_B_PIN, LED_B_PWM_CHANNEL);

    int16_t i = 0;
    for (; i < 512; i++) {
        setLed(0, i, 0);
        vTaskDelay(2);
    }

    for (; i >= 0; i--) {
        setLed(0, i, 0);
        vTaskDelay(1);
    }

    ESP32Encoder::useInternalWeakPullResistors=UP;
    glob.encoder.attachHalfQuad(ENCODER_A_PIN, ENCODER_B_PIN);
    glob.encoder.attachButtonPin(ENCODER_SW_PIN);

    redDisplay.begin();
    bluDisplay.begin();
    redDisplay.setBrightness(BRIGHTNESS);
    bluDisplay.setBrightness(BRIGHTNESS);

    EEPROM.begin(sizeof(Memory));
    EEPROM.get(0, glob.memory);
}

/*********************************************************************/

void Tasks::setGameMode() {

    int8_t choice = glob.memory.gameMode;
    uint8_t buff[4] = {0, 0, 0, 0};

    while (!glob.encoder.wasClicked()) {

        int8_t dir = glob.encoder.wasTurned();
        if (dir > 0)       choice = TIME_UP;
        else if (dir < 0)  choice = TIME_DOWN;

        switch (choice) {
        case TIME_DOWN:

            buff[2] = 119; // A on 7-segment
            break;
        
        case TIME_UP:

            buff[2] = 124; // B on 7-segment
            break;
        }

        redDisplay.displayRawBytes(buff, 4);
        bluDisplay.displayRawBytes(buff, 4);

        vTaskDelay(10);
    }

    setLed(0, 100, 0);
    glob.memory.gameMode = choice;
    vTaskDelay(500);
    setLed(0, 0, 0);
}

/*********************************************************************/

void Tasks::setRedTime() {

    glob.redTime = glob.memory.initRedTime;

    while (!glob.encoder.wasClicked()) {

        int8_t dir = glob.encoder.wasTurned();
        if (dir > 0 && glob.redTime < 6039)     glob.redTime += 10;
        else if (dir < 0 && glob.redTime > 10)   glob.redTime -= 10;

        redDisplay.display(glob.redTime);

        vTaskDelay(10);
    }

    setLed(100, 0, 0);
    glob.memory.initRedTime = glob.redTime;
    vTaskDelay(500);
    setLed(0, 0, 0);
}

/*********************************************************************/

void Tasks::setBlueTime() {

}

/*********************************************************************/

void Tasks::setUpTime() {


}

/*********************************************************************/

void Tasks::setLed(uint16_t R, uint16_t G, uint16_t B) {

    ledcWrite(LED_R_PWM_CHANNEL, R);
    ledcWrite(LED_G_PWM_CHANNEL, G);
    ledcWrite(LED_B_PWM_CHANNEL, B);
}

/*********************************************************************/

void Tasks::clearMem() {

    memset(&glob.memory, 0, sizeof(Memory));

    EEPROM.put(0, glob.memory);
    EEPROM.commit();
}

/*********************************************************************/

void Tasks::updateDataBase() {

    glob.memory.lastResultIndex++;
    if (glob.memory.lastResultIndex > RESULTS_IN_MEM) glob.memory.lastResultIndex = 0;
    uint8_t reid = glob.memory.lastResultIndex;

    glob.memory.lastResultNum++;

    glob.memory.result[reid].num = glob.memory.lastResultNum;
    glob.memory.result[reid].mode = 0; // TODO
    glob.memory.result[reid].redTime = 0; // TODO
    glob.memory.result[reid].bluTime = 0; // TODO

    EEPROM.put(0, glob.memory);
    EEPROM.commit();
}