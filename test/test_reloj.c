
#include <stdint.h>

#include "clock.h"
#include "unity.h"

#define TIME_SIZE 6
#define TICKS_PER_SECOND 5
#define TICKS_PER_MINUTE 5 * 60
#define TICKS_PER_HOUR 5 * 60 * 60
#define TICKS_PER_DAY 5 * 60 * 60 * 24

clocks_t reloj;
static bool alarmaActivada;

void simulateTicks(uint32_t tiks) {
    for (int i = 0; i <= tiks; i++) {
        newTick(reloj);
    }
}

void alarmEvent(clocks_t reloj) {
    alarmaActivada = true;
}

void setUp() {
    static const uint8_t INICIAL[] = {1, 2, 3, 4, 0, 0};

    alarmaActivada = false;

    reloj = createClock(TICKS_PER_SECOND, alarmEvent);
    setTime(reloj, INICIAL, sizeof(INICIAL));
}

/* Test de inicializacion del reloj */
void test_start_up(void) {
    static const uint8_t ESPERADO[] = {0, 0, 0, 0, 0, 0}; /*Array que representa la hora esperada*/
    uint8_t hora[TIME_SIZE] = {0xFF};                     /* Almacenará la hora que retorna la funcion getTime*/

    reloj = createClock(TICKS_PER_SECOND, alarmEvent); /* Inicializacion del reloj: Recibe por parametro cuantos ticks por segundo recibe el reloj*/

    TEST_ASSERT_FALSE(getTime(reloj, hora, TIME_SIZE));       /* Test de la validez de la hora*/
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, TIME_SIZE); /* Test del resultado de getTime */

    TEST_ASSERT_FALSE(getAlarm(reloj, hora, TIME_SIZE));
}

/* Test de seteo de hora */
void test_set_time(void) {
    static const uint8_t ESPERADO[] = {1, 2, 3, 4, 0, 0}; /*Array que representa la hora esperada*/

    uint8_t hora[TIME_SIZE] = {0xFF}; /* Almacenará la hora que retorna la funcion getTime*/

    TEST_ASSERT_TRUE(getTime(reloj, hora, TIME_SIZE));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, TIME_SIZE); /* Test del resultado de getTime */
}

void test_one_second(void) {
    static const uint8_t ESPERADO[] = {1, 2, 3, 4, 0, 1}; /*Array que representa la hora esperada*/
    uint8_t hora[TIME_SIZE] = {0xFF};                     /*Almacenará la hora que retorna la funcion getTime*/

    simulateTicks(TICKS_PER_SECOND);

    getTime(reloj, hora, TIME_SIZE);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, TIME_SIZE); /* Test del resultado de getTime */
}

void test_ten_seconds(void) {
    static const uint8_t ESPERADO[] = {1, 2, 3, 4, 1, 0}; /*Array que representa la hora esperada*/
    uint8_t hora[TIME_SIZE] = {0xFF};                     /*Almacenará la hora que retorna la funcion getTime*/

    simulateTicks(TICKS_PER_SECOND * 10);

    getTime(reloj, hora, TIME_SIZE);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, TIME_SIZE); /* Test del resultado de getTime */
}

void test_one_minute(void) {
    static const uint8_t ESPERADO[] = {1, 2, 3, 5, 0, 0}; /*Array que representa la hora esperada*/
    uint8_t hora[TIME_SIZE] = {0xFF};                     /*Almacenará la hora que retorna la funcion getTime*/

    simulateTicks(TICKS_PER_MINUTE);

    getTime(reloj, hora, TIME_SIZE);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, TIME_SIZE);
}

void test_ten_minutes(void) {
    static const uint8_t ESPERADO[] = {1, 2, 4, 4, 0, 0}; /*Array que representa la hora esperada*/
    uint8_t hora[TIME_SIZE] = {0xFF};                     /*Almacenará la hora que retorna la funcion getTime*/

    simulateTicks(TICKS_PER_MINUTE * 10);

    getTime(reloj, hora, TIME_SIZE);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, TIME_SIZE);
}

void test_one_hour(void) {
    static const uint8_t ESPERADO[] = {1, 3, 3, 4, 0, 0}; /*Array que representa la hora esperada*/
    uint8_t hora[TIME_SIZE] = {0xFF};                     /*Almacenará la hora que retorna la funcion getTime*/

    simulateTicks(TICKS_PER_HOUR);

    getTime(reloj, hora, TIME_SIZE);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, TIME_SIZE);
}

void test_ten_hours(void) {
    static const uint8_t ESPERADO[] = {2, 2, 3, 4, 0, 0}; /*Array que representa la hora esperada*/
    uint8_t hora[TIME_SIZE] = {0xFF};                     /*Almacenará la hora que retorna la funcion getTime*/

    simulateTicks(TICKS_PER_HOUR * 10);

    getTime(reloj, hora, TIME_SIZE);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, TIME_SIZE);
}

void test_one_day(void) {
    static const uint8_t ESPERADO[] = {1, 2, 3, 4, 0, 0};
    uint8_t hora[TIME_SIZE] = {0xFF}; /* Almacenará la hora que retorna la funcion getTime*/
    simulateTicks(TICKS_PER_DAY);

    getTime(reloj, hora, TIME_SIZE);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, TIME_SIZE);
}

void test_set_alarm(void) {
    static const uint8_t ESPERADO[] = {1, 2, 3, 4, 0, 5}; /*Array que representa la alarma esperada*/

    uint8_t hora[TIME_SIZE] = {0xFF}; /* Almacenará la hora que retorna la funcion getAlarm*/

    setAlarm(reloj, ESPERADO, TIME_SIZE);

    TEST_ASSERT_TRUE(getAlarm(reloj, hora, TIME_SIZE));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, TIME_SIZE); /* Test del resultado de getAlarm */
}

void test_desactivate_alarm(void) {
    static const uint8_t ESPERADO[] = {1, 2, 3, 4, 0, 5}; /*Array que representa la alarma esperada*/

    uint8_t hora[TIME_SIZE] = {0xFF}; /* Almacenará la hora que retorna la funcion getAlarm*/

    setAlarm(reloj, ESPERADO, TIME_SIZE);

    TEST_ASSERT_TRUE(getAlarm(reloj, hora, TIME_SIZE));

    desactivateAlarm(reloj);

    TEST_ASSERT_FALSE(getAlarm(reloj, hora, TIME_SIZE));
}

void test_alarm_on(void) {
    static const uint8_t ALARMA[] = {1, 2, 3, 4, 0, 5}; /*Array que representa la alarma esperada*/

    setAlarm(reloj, ALARMA, TIME_SIZE);

    simulateTicks(TICKS_PER_MINUTE * 5);

    TEST_ASSERT_TRUE(alarmaActivada);
}