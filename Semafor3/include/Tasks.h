#ifndef TASKS_H
#define TASKS_H

#include <Arduino.h>
#include <EEPROM.h>
#include <TM1637.h>

#include "PinDefinitions.h"
#include "Globals.h"

class Tasks {

public:

    void init();
    void setGameMode();
    void setRedTime();
    void setBlueTime();
    void setUpTime();

    void setLed(uint16_t R, uint16_t G, uint16_t B);

    void updateDataBase();
    void clearMem();
};

extern Tasks tasks;

#endif