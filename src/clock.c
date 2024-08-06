#include "clock.h"

#include "string.h"

#define TIME_SIZE 6
#define TICKS_PER_SECOND 5
static uint8_t MAX_VALUES[] = {2, 9, 5, 9, 5, 9};

struct clocks_s {
    uint8_t time[TIME_SIZE];
    uint8_t alarm[TIME_SIZE];
    uint16_t ticks_count;
    bool valid;
    bool alarmActive;
    clocks_event_t EventHandler; /* Es una variable puntero que apunta a una funcion*/
};

static struct clocks_s instances;

clocks_t createClock(uint16_t ticks, clocks_event_t event_handler) {
    memset(&instances, 0, sizeof(instances)); /* Setear el array al valor esperado */
    instances.ticks_count = 0;
    instances.valid = false;
    instances.alarmActive = false;
    instances.EventHandler = event_handler;
    return &instances;
}

bool getTime(clocks_t clk, uint8_t time[], int size) {
    memcpy(time, clk->time, size);
    return clk->valid;
}

void setTime(clocks_t clk, const uint8_t time[], int size) {
    memcpy(clk->time, time, size);
    clk->valid = true;
}

void newTick(clocks_t clk) {
    if (clk->ticks_count >= TICKS_PER_SECOND - 1) {
        clk->ticks_count = 0;

        increaseTime(clk);

    } else {
        clk->ticks_count += 1;
    }
}

void increaseTime(clocks_t clk) {
    for (int i = (TIME_SIZE - 1); i >= 0; i--) {
        if (clk->time[i] < MAX_VALUES[i]) {
            clk->time[i] += 1;
            break;
        } else {
            clk->time[i] = 0;
        }
    }
    if (clk->time[0] == 2) {
        MAX_VALUES[1] = 3;
    } else {
        MAX_VALUES[1] = 9;
    }
}

bool getAlarm(clocks_t clk, uint8_t time[], int size) {
    memcpy(time, clk->alarm, size);
    return clk->alarmActive;
}

void setAlarm(clocks_t clk, const uint8_t time[], int size) {
    memcpy(clk->alarm, time, size);
    clk->alarmActive = true;
}

void desactivateAlarm(clocks_t clk) {
    clk->alarmActive = false;
}
