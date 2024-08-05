
#include <stdint.h>

#include "clock.h"
#include "unity.h"

/* Test de inicializacion del reloj */
void test_start_up(void) {
    static const uint8_t ESPERADO[] = {0, 0, 0, 0, 0, 0}; /*Array que representa la hora esperada*/
    uint8_t hora[6] = {0xFF};                             /* Almacenará la hora que retorna la funcion getTime*/

    createClock(1);   /* Recibe por parametro cuantos ticks por segundo recibe el reloj*/
    getTime(hora, 6); /* Funcion que devuelve la hora */

    TEST_ASSERT_FALSE(getTime(hora, 6)); /* Test de la validez de la hora*/

    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, 6); /* Test del resultado de getTime */
}
