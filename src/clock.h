#include "stdbool.h"
#include "stdint.h"

typedef struct clocks_s *clocks_t;

typedef void (*clocks_event_t)(clocks_t clock);

clocks_t createClock(uint16_t tiks, clocks_event_t event_handler);

bool getTime(clocks_t clk, uint8_t time[], int size);

void setTime(clocks_t clk, const uint8_t time[], int size);

void newTick();

void increaseTime(clocks_t clk);

bool getAlarm(clocks_t clk, uint8_t time[], int size);

void setAlarm(clocks_t clk, const uint8_t time[], int size);

void desactivateAlarm(clocks_t clk);