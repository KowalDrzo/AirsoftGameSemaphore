#include "Loops.h"

Loops loops;

void Loops::ledLoop() {

    while (1) {

        vTaskDelay(1);
    }
}

/*********************************************************************/

void Loops::semaphoreLoop() {

    EEPROM.put(0, glob.memory);
    EEPROM.commit();

    clockTimer.start(1000);
    darkModeTimer.start(10000);

    while(1) {

        if (clockTimer.check()) {


        }

        if (darkModeTimer.check()) {

            
        }
    }
}