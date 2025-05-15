#ifndef KEYBRD_H
#define KEYBRD_H

#include <stdint.h>
#include <stdbool.h>

/* Lista funkcji dostępnych publicznie */

void structureInit(void);

_Bool Button;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif
