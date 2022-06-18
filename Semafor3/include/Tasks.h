#ifndef TASKS_H
#define TASKS_H

#include <Arduino.h>
#include <EEPROM.h>
#include <TM1637.h>

#include "PinDefinitions.h"
#include "Timer.h"
#include "Globals.h"

class Tasks {

    Timer clockTimer;
    Timer darkModeTimer;

public:

    void setGameMode();
    void setRedTime();
    void setBlueTime();
    void setUpTime();

    void updateDataBase();
    void clearMem();
};

extern Tasks tasks;

#endif