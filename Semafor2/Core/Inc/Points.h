/*
 * Points.h
 *
 *  Created on: Apr 15, 2020
 *      Author: KowalDrzo
 */

#include "gpio.h"

#ifndef INC_POINTS_H_
#define INC_POINTS_H_

_Bool przyciemniony;

void addPoint(_Bool ifRed);

void winGame(_Bool ifRed);

void checkPoint(void);

void displayDark(int *czasJasny);

#endif /* INC_POINTS_H_ */
