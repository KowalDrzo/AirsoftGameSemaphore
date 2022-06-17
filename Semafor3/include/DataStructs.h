#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#define SEMAFOR3_ID 1
#define RESULTS_IN_MEM 30

#include <stdint.h>

enum GameMode {

    TIME_DOWN = 0,
    TIME_UP
};

struct GameResult {
    
    uint16_t num;
    uint8_t mode;
    uint16_t redTime;
    uint16_t bluTime;
};

struct Memory {

    uint8_t lastResultIndex;
    uint16_t lastResultNum;
    GameResult result[RESULTS_IN_MEM];

    uint8_t initGameMode;
    uint16_t initRedTime;
    uint16_t initBluTime;
};

#endif