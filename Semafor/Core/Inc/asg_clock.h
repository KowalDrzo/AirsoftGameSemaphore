#ifndef INC_ASG_CLOCK_H_
#define INC_ASG_CLOCK_H_

#include <stdint.h>

#define RED 1
#define BLUE 0
#define NEUTRAL -1

typedef struct {

	int16_t redTime;
	int16_t blueTime;
	int8_t bright;
	int8_t potenValue;

	int16_t ledState;
	_Bool ledGoinBack;
	int16_t gameModeUp;
	_Bool gameStarted;

} AirsoftClock;

typedef struct {

	int16_t redTime;
	int16_t blueTime;
	int16_t bothTime;
	int16_t magic;
} SavedTimes;

/* Lista funkcji dostępnych publicznie */

void structureInit(void);

extern AirsoftClock asgClock;
extern SavedTimes savedTimes;


#endif /* INC_ASG_CLOCK_H_ */
