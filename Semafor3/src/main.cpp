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

    xTaskCreate((TaskFunction_t)Loops::ledLoop, "Led Loop Task", 4098, NULL, 2, NULL);
}

void loop() {

    loops.semaphoreLoop();
    vTaskDelay(1);
}