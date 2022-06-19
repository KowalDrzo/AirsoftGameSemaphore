#include "Loops.h"

GlobalAggregator glob;

void setup() {

    tasks.init();
    //tasks.clearMem();

    tasks.setGameMode();

    if (glob.memory.gameMode == TIME_DOWN) {

        tasks.setRedTime();
        tasks.setBlueTime();
    }
    else tasks.setUpTime();

    xTaskCreate((TaskFunction_t)Loops::ledLoop, "Led Loop Task", 4098, NULL, 2, NULL);

    loops.semaphoreLoop();
}

void loop() {
    vTaskDelay(1);
}