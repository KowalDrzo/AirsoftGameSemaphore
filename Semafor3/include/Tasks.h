#ifndef TASKS_H
#define TASKS_H

#include <Arduino.h>
#include <EEPROM.h>
#include <TM1637.h>

#include "PinDefinitions.h"
#include "Globals.h"
#include "Timer.h"

class Tasks {

    uint16_t sec2minSec(int16_t sec);
    uint8_t requiredOffset(int16_t sec);
    Timer colonTimer;

public:

    void init();
    void setGameMode();
    void setRedTime();
    void setBlueTime();
    void setUpTime();

    void setLed(uint16_t R, uint16_t G, uint16_t B);

    void addPoint(uint8_t team);
    bool checkEnd();

    void updateDataBase();
    void clearMem();
};

extern Tasks tasks;

#endif