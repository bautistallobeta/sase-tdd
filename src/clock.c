#include "clock.h"

#include "string.h"

#define TIME_SIZE 6
#define TICKS_PER_SECOND 5
static uint8_t MAX_VALUES[] = {2, 9, 5, 9, 5, 9};

struct clocks_s {
    /* Variable que guarda la hora actual del reloj */
    uint8_t time[TIME_SIZE];
    /* Variable que guarda la hora seteada para la alarma */
    uint8_t alarm[TIME_SIZE];
    /* Variable que guarda la proxima hora a la que debe sonar (solo es != de alarm[] si se pospone la misma) */
    uint8_t nextAlarm[TIME_SIZE];
    /* Contador de ticks para el avance del tiempo */
    uint16_t ticksCount;
    /* Variable de estado de validez de la hora */
    bool valid;
    /* Variable de estaod de validez de la alarma */
    bool alarmValid;
    /* Variable de estado de activacion de la alarma ( true = sonando ; false = apagada ) */
    bool alarmActivated;
    /* Variable puntero que apunta a una funcion*/
    clocks_event_t EventHandler;
};

static struct clocks_s instances;

clocks_t createClock(uint16_t ticks, clocks_event_t event_handler) {
    /* Setear el array al valor esperado */
    memset(&instances, 0, sizeof(instances));
    /* Inicializacion de ticks */
    instances.ticksCount = 0;
    /* Inicializacion de variables de estado de validez */
    instances.valid = false;
    instances.alarmValid = false;
    instances.alarmActivated = false;
    /* Asignacion del event handler a la funcion definida en el archivo 'test_reloj' */
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
    if (clk->ticksCount >= TICKS_PER_SECOND - 1) {
        clk->ticksCount = 0;

        increaseTime(clk->time);

        checkAlarm(clk);

    } else {
        clk->ticksCount += 1;
    }
}

/**
 * @test @ref test_set_time
 */
void increaseTime(uint8_t time[]) {
    for (int i = (TIME_SIZE - 1); i >= 0; i--) {
        if (time[i] < MAX_VALUES[i]) {
            time[i] += 1;
            break;
        } else {
            time[i] = 0;
        }
    }
    if (time[0] == 2) {
        MAX_VALUES[1] = 3;
    } else {
        MAX_VALUES[1] = 9;
    }
}

void checkAlarm(clocks_t clk) {
    if ((clk->alarmValid == true) && (memcmp(clk->nextAlarm, clk->time, sizeof(clk->alarm)) == 0)) {
        clk->EventHandler(clk, true);
        clk->alarmActivated = true;
    }
}

bool getAlarm(clocks_t clk, uint8_t time[], int size) {
    memcpy(time, clk->alarm, size);
    return clk->alarmValid;
}

void setAlarm(clocks_t clk, const uint8_t time[], int size) {
    memcpy(clk->alarm, time, size);
    memcpy(clk->nextAlarm, time, size);
    clk->alarmValid = true;
}

void desactivateAlarm(clocks_t clk) {
    clk->alarmValid = false;
}

bool shutAlarm(clocks_t clk, int size) {
    clk->alarmActivated = false;
    memcpy(clk->nextAlarm, clk->alarm, size);
    clk->EventHandler(clk, false);
}

void postponeAlarm(clocks_t clk, const uint8_t n_minutes, int size) {
    clk->alarmActivated = false;
    memcpy(clk->nextAlarm, clk->time, size);

    for (int i = 0; i < (n_minutes * 60); i++) {
        increaseTime(clk->nextAlarm);
    }

    clk->EventHandler(clk, false);
}