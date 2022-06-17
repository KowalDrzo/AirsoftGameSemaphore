#include "Tasks.h"

Tasks tasks;


/*********************************************************************/

void Tasks::clearMem() {

    /*for (uint8_t i = 0; i < FLIGHTS_IN_MEM; i++) {

        glob.memory.lastFlightIndex = 0;
        glob.memory.lastFlightNum = 0;
        glob.memory.flight[i].num = 0;
        glob.memory.flight[i].apogee = 0;
        glob.memory.flight[i].maxSpeed = 0;
    }*/

    EEPROM.put(0, glob.memory);
    EEPROM.commit();
}

/*********************************************************************/

void Tasks::updateDataBase() {

    /*glob.memory.lastFlightIndex++;
    if (glob.memory.lastFlightIndex > FLIGHTS_IN_MEM) glob.memory.lastFlightIndex = 0;
    uint8_t flid = glob.memory.lastFlightIndex;

    glob.memory.lastFlightNum++;

    glob.memory.flight[flid].num = glob.memory.lastFlightNum;
    glob.memory.flight[flid].apogee = glob.apogee;
    glob.memory.flight[flid].maxSpeed = glob.maxSpeed;*/

    EEPROM.put(0, glob.memory);
    EEPROM.commit();
}