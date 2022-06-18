#include "Tasks.h"

Tasks tasks;
TM1637 redDisplay(RED_CLK_PIN, RED_DIO_PIN);
TM1637 bluDisplay(BLU_CLK_PIN, BLU_DIO_PIN);

/*********************************************************************/

void Tasks::clearMem() {

    for (uint8_t i = 0; i < RESULTS_IN_MEM; i++) {

        glob.memory.lastResultIndex = 0;
        glob.memory.lastResultNum = 0;
        glob.memory.result[i].num = 0;
        glob.memory.result[i].mode = 0;
        glob.memory.result[i].redTime = 0;
        glob.memory.result[i].bluTime = 0;
    }

    EEPROM.put(0, glob.memory);
    EEPROM.commit();
}

/*********************************************************************/

void Tasks::updateDataBase() {

    glob.memory.lastResultIndex++;
    if (glob.memory.lastResultIndex > RESULTS_IN_MEM) glob.memory.lastResultIndex = 0;
    uint8_t reid = glob.memory.lastResultIndex;

    glob.memory.lastResultNum++;

    glob.memory.result[reid].num = glob.memory.lastResultNum;
    glob.memory.result[reid].mode = 0; // TODO
    glob.memory.result[reid].redTime = 0; // TODO
    glob.memory.result[reid].bluTime = 0; // TODO

    EEPROM.put(0, glob.memory);
    EEPROM.commit();
}