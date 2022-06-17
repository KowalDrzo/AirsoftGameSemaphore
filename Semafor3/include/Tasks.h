#ifndef TASKS_H
#define TASKS_H

#include <Arduino.h>
#include <EEPROM.h>
#include <TM1637.h>

#include "PinDefinitions.h"
#include "Timer.h"
#include "Globals.h"

class Tasks {

public:


  
    void updateDataBase();
    void clearMem();
};

extern Tasks tasks;

#endif