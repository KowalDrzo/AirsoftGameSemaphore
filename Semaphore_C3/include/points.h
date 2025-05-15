#ifndef POINTS_H
#define POINTS_H

#include <stdbool.h>

_Bool przyciemniony;

void addPoint(_Bool ifRed);

void winGame(_Bool ifRed);

void checkPoint(void);

void displayDark(int *czasJasny);

#endif
