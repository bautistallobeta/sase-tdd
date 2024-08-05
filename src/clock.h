#include "stdbool.h"
#include "stdint.h"
#define TIME_SIZE 6
#define TICKS_PER_SECOND 5
typedef struct clocks_s *clocks_t;

clocks_t createClock(uint16_t tiks);

bool getTime(clocks_t clk, uint8_t time[], int size);

void setTime(clocks_t clk, uint8_t time[], int size);

void newTick();

void increaseSeconds(clocks_t clk);