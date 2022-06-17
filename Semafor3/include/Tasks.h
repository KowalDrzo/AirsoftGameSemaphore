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

    TM1637 redDisplay(RED_CLK_PIN, RED_DIO_PIN);
    TM1637 bluDisplay(BLU_CLK_PIN, BLU_DIO_PIN);
  
    void updateDataBase();
    void clearMem();
};

extern Tasks tasks;

#endif