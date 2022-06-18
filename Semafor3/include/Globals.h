#ifndef GLOBALS_H
#define GLOBALS_H

#include "DataStructs.h"
#include "EncoredWithButton.h"

struct GlobalAggregator {

    Memory memory;
    uint8_t flagState = NEUTRAL;
    uint8_t gameMode = TIME_DOWN;
    bool darkMode = false;

    int16_t redTime;
    int16_t blueTime;
    int16_t encoderVal;

    EncoredWithButton encoder;
};

extern GlobalAggregator glob;

#endif