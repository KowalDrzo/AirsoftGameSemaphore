#include "Loops.h"

Loops loops;

void Loops::ledLoop() {

    while (1) {

        int16_t i = 0;
        for (; i < 500; i++) {
            
            switch (glob.flagState) {
            case RED_TEAM:
                tasks.setLed(i, 0, 0);
                break;

            case BLUE_TEAM:
                tasks.setLed(0, 0, i);
                break;
            
            default:
                tasks.setLed(i, i, i);
                break;
            }
            
            vTaskDelay(1);
        }

        for (; i >= 0; i--) {
            
            switch (glob.flagState) {
            case RED_TEAM:
                tasks.setLed(i, 0, 0);
                break;

            case BLUE_TEAM:
                tasks.setLed(0, 0, i);
                break;
            
            default:
                tasks.setLed(i, i, i);
                break;
            }

            vTaskDelay(1);
        }

        vTaskDelay(1);
    }
}

/*********************************************************************/

void Loops::semaphoreLoop() {

    glob.encoderVal = GAME_RANGE / 2;
    EEPROM.put(0, glob.memory);
    EEPROM.commit();

    clockTimer.start(1000);
    darkModeTimer.start(10000);

    while(1) {

        int8_t dir = glob.encoder.wasTurned();
        if (dir > 0 && glob.encoderVal < GAME_RANGE)    glob.encoderVal++;
        else if (dir < 0 && glob.encoderVal > 0)        glob.encoderVal--;

        if (clockTimer.check()) {

            if (glob.encoderVal > GAME_RANGE - RANGE_OFFSET) {

                glob.flagState = BLUE_TEAM;
                tasks.addPoint(BLUE_TEAM);
            }

            else if (glob.encoderVal < RANGE_OFFSET) {

                glob.flagState = RED_TEAM;
                tasks.addPoint(RED_TEAM);
            }

            else glob.flagState = NEUTRAL;

            if (tasks.checkEnd()) {
                // TODO
            }
        }

        if (darkModeTimer.check()) {

            
        }

        vTaskDelay(1);
    }
}