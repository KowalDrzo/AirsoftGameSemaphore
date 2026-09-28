#ifndef INC_ASG_CLOCK_H_
#define INC_ASG_CLOCK_H_

#include <stdint.h>

#define RED 1
#define BLUE 0
#define NEUTRAL -1

#define GAME_MODE_A 0
#define GAME_MODE_B 1
#define GAME_MODE_C 2

typedef struct {
	int16_t redTime;
	int16_t blueTime;
	int16_t matchTime;
	int8_t bright;
	int8_t potenValue;
	int8_t gameMode;
	_Bool gameStarted;
} AirsoftClock;

typedef struct {
	int16_t redTime;
	int16_t blueTime;
	int16_t bothTime;
	int16_t magic;
} SavedTimes;

void structureInit(void);

extern AirsoftClock asgClock;
extern SavedTimes savedTimes;

#endif /* INC_ASG_CLOCK_H_ */
