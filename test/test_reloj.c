
#include <stdint.h>

#include "clock.h"
#include "unity.h"

clocks_t reloj;

void simulateTicks(uint32_t tiks) {
    for (int i = 0; i <= tiks; i++) {
        newTick(reloj);
    }
}

void setUp() {
    static const uint8_t INICIAL[] = {1, 2, 3, 4, 0, 0};

    reloj = createClock(TICKS_PER_SECOND);
    setTime(reloj, INICIAL, sizeof(INICIAL));
}

/* Test de inicializacion del reloj */
void test_start_up(void) {
    static const uint8_t ESPERADO[] = {0, 0, 0, 0, 0, 0}; /*Array que representa la hora esperada*/
    uint8_t hora[TIME_SIZE] = {0xFF};                     /* Almacenará la hora que retorna la funcion getTime*/

    reloj = createClock(TICKS_PER_SECOND); /* Inicializacion del reloj: Recibe por parametro cuantos ticks por segundo recibe el reloj*/

    TEST_ASSERT_FALSE(getTime(reloj, hora, TIME_SIZE));       /* Test de la validez de la hora*/
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, TIME_SIZE); /* Test del resultado de getTime */
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

void test_10_second(void) {
    static const uint8_t ESPERADO[] = {1, 2, 3, 4, 1, 0}; /*Array que representa la hora esperada*/
    uint8_t hora[TIME_SIZE] = {0xFF};                     /*Almacenará la hora que retorna la funcion getTime*/

    simulateTicks(TICKS_PER_SECOND * 12);

    getTime(reloj, hora, TIME_SIZE);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, TIME_SIZE); /* Test del resultado de getTime */
}