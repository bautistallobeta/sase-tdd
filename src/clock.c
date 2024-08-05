#include "clock.h"

#include "string.h"

struct clocks_s {
    uint8_t time[TIME_SIZE];
    uint16_t ticks_count;
    bool valid;
};

static struct clocks_s instances;

clocks_t createClock(uint16_t ticks) {
    memset(&instances, 0, sizeof(instances)); /* Setear el array al valor esperado */
    instances.ticks_count = 0;
    instances.valid = false;
    return &instances;
}

bool getTime(clocks_t clk, uint8_t time[], int size) {
    memcpy(time, clk->time, size);
    return clk->valid;
}

void setTime(clocks_t clk, uint8_t time[], int size) {
    memcpy(clk->time, time, size);
    clk->valid = true;
}

void newTick(clocks_t clk) {
    if (clk->ticks_count >= TICKS_PER_SECOND) {
        clk->ticks_count = 0;

        increaseSeconds(clk);

    } else {
        printf("\nAA % i", clk->ticks_count);
        clk->ticks_count += 1;
    }
}

void increaseSeconds(clocks_t clk) {
    if (clk->time[5] < 9) {
        clk->time[5] += 1;
    } else {
        clk->time[5] = 0;
        if (clk->time[4] < 9) {
            clk->time[4] += 1;
        } else {
            clk->time[4] = 0;
        }
    }
}