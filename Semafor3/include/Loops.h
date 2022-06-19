#ifndef LOOPS_H
#define LOOPS_H

#include "Tasks.h"

class Loops {

    Timer clockTimer;
    Timer darkModeTimer;

public:

    static void ledLoop();
    void semaphoreLoop();
};

extern Loops loops;

#endif