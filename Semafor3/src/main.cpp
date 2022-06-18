#include "Loops.h"

#include <Arduino.h>

GlobalAggregator glob;

void setup() {

    tasks.init();
    tasks.setGameMode();

    if (glob.gameMode == 0) {

        tasks.setRedTime();
        tasks.setBlueTime();
    }
    else tasks.setUpTime();
}

void loop() {

}