#ifndef POINTS_H
#define POINTS_H

#include <Arduino.h>
#include "setup.h"

void addPoint(_Bool ifRed);

void winGame(_Bool ifRed);

void checkPoint(void);

void displayDark(int czasJasny);

#endif
