/*
 * Keybrd.h
 *
 *  Created on: Apr 12, 2020
 *      Author: KowalDrzo
 */

#ifndef INC_KEYBRD_H_
#define INC_KEYBRD_H_

#include "gpio.h"

/* Lista funkcji dostępnych publicznie */

void structureInit(void);

_Bool Button;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif /* INC_KEYBRD_H_ */
