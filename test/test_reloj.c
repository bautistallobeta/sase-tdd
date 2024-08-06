
#include <stdint.h>

#include "clock.h"
#include "unity.h"

#define TIME_SIZE 6
#define TICKS_PER_SECOND 5
#define TICKS_PER_MINUTE TICKS_PER_SECOND * 60
#define TICKS_PER_HOUR TICKS_PER_MINUTE * 60
#define TICKS_PER_DAY TICKS_PER_HOUR * 24

clocks_t reloj;
static bool alarmaActivada;

void simulateTicks(uint32_t tiks) {
    for (int i = 0; i < tiks; i++) {
        newTick(reloj);
    }
}

void alarmEvent(clocks_t reloj, bool state) {
    alarmaActivada = state;
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

/* Test de seteo y activacion de alarma*/
void test_set_alarm(void) {
    static const uint8_t ESPERADO[] = {1, 2, 4, 0, 0, 0}; /*Array que representa la alarma esperada*/

    uint8_t hora[TIME_SIZE] = {0xFF}; /* Almacenará la hora que retorna la funcion getAlarm*/

    setAlarm(reloj, ESPERADO, TIME_SIZE);

    TEST_ASSERT_TRUE(getAlarm(reloj, hora, TIME_SIZE));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, TIME_SIZE); /* Test del resultado de getAlarm */
}

/* Test de desactivacion de alarma (ACLARACION: no es el apagado o pospuesta de la alarma)*/
void test_desactivate_alarm(void) {
    static const uint8_t ESPERADO[] = {1, 2, 4, 0, 0, 5}; /*Array que representa la alarma esperada*/

    uint8_t hora[TIME_SIZE] = {0xFF}; /* Almacenará la hora que retorna la funcion getAlarm*/

    setAlarm(reloj, ESPERADO, TIME_SIZE);

    TEST_ASSERT_TRUE(getAlarm(reloj, hora, TIME_SIZE));

    desactivateAlarm(reloj);

    TEST_ASSERT_FALSE(getAlarm(reloj, hora, TIME_SIZE));
}

/*Test de alarma activada pero aun no se llego a la hora (aun no suena)*/
void test_alarm_activated_off(void) {
    static const uint8_t ALARMA[] = {1, 2, 4, 0, 0, 0}; /*Array que representa la alarma esperada*/

    setAlarm(reloj, ALARMA, TIME_SIZE);

    simulateTicks(TICKS_PER_MINUTE * 4);

    TEST_ASSERT_FALSE(alarmaActivada);
}

/*Test de alarma activada cuando se llega a la hora seteada de la alarma (debe empezar a sonar)*/
void test_alarm_activated_on(void) {
    static const uint8_t ALARMA[] = {1, 2, 4, 0, 0, 0}; /*Array que representa la alarma esperada*/

    setAlarm(reloj, ALARMA, TIME_SIZE);

    simulateTicks(TICKS_PER_MINUTE * 6 - 1);
    TEST_ASSERT_FALSE(alarmaActivada);

    simulateTicks(1);
    TEST_ASSERT_TRUE(alarmaActivada);
}

/*Test de alarma, cuando se llega a la hora seteada, pero la alarma esta desactivada*/
void test_alarm_desactivated_off(void) {
    static const uint8_t ESPERADO[] = {1, 2, 4, 0, 0, 0}; /*Array que representa la alarma esperada*/

    uint8_t hora[TIME_SIZE] = {0xFF}; /* Almacenará la hora que retorna la funcion getAlarm*/

    setAlarm(reloj, ESPERADO, TIME_SIZE);

    TEST_ASSERT_TRUE(getAlarm(reloj, hora, TIME_SIZE));

    desactivateAlarm(reloj);
    simulateTicks(TICKS_PER_MINUTE * 7);

    TEST_ASSERT_FALSE(alarmaActivada);
}

/*Test de alarma, la alarma suena, pasan 24hs y no se paro ni pospuso (debe seguir sonando)*/
void test_alarm_already_on(void) {
    static const uint8_t ALARMA[] = {1, 2, 4, 0, 0, 0}; /*Array que representa la alarma esperada*/

    setAlarm(reloj, ALARMA, TIME_SIZE);

    simulateTicks(TICKS_PER_MINUTE * 6); /*Suena la alarma*/
    TEST_ASSERT_TRUE(alarmaActivada);

    simulateTicks(TICKS_PER_DAY);     /*Pasab 24hs*/
    TEST_ASSERT_TRUE(alarmaActivada); /*Sigue sonando*/
}

/* Test de apagado de la alarma cuando esta sonando (ACLARACIÓN: no es la desactivacion de la alarma) */
void test_alarm_shut(void) {
    static const uint8_t ALARMA[] = {1, 2, 4, 0, 0, 0}; /*Array que representa la alarma esperada*/

    setAlarm(reloj, ALARMA, TIME_SIZE);

    simulateTicks(TICKS_PER_MINUTE * 7);
    shutAlarm(reloj, TIME_SIZE);
    TEST_ASSERT_FALSE(alarmaActivada);
}

/*Test de apagado de la alarma cuando no esta sonando (no debe suceder nada)*/
void test_alarm_shut_when_off(void) {
    TEST_ASSERT_FALSE(alarmaActivada);
    shutAlarm(reloj, TIME_SIZE);
    TEST_ASSERT_FALSE(alarmaActivada);
}

/* Test de posposicion de alarma cuando esta sonando*/
void test_alarm_postponed(void) {
    /*Array que representa la alarma esperada*/
    static const uint8_t ALARMA[] = {1, 2, 4, 0, 0, 0};
    /* Constante con la cantidad de minutos que se desea posponer */
    static const uint8_t N_MINUTOS = 5;

    setAlarm(reloj, ALARMA, TIME_SIZE);

    simulateTicks(TICKS_PER_MINUTE * 5);

    postponeAlarm(reloj, N_MINUTOS, TIME_SIZE);

    TEST_ASSERT_FALSE(alarmaActivada);
}

/* Test, la alarma es pospuesta n minutos, y no suena hasta pasar n minutos*/
void test_alarm_postponed_on(void) {
    static const uint8_t ALARMA[] = {1, 2, 4, 0, 0, 0}; /*Array que representa la alarma esperada*/
    static const uint8_t N_MINUTOS = 5;                 /* Constante con la cantidad de minutos que se desea posponer */

    setAlarm(reloj, ALARMA, TIME_SIZE);

    simulateTicks(TICKS_PER_MINUTE * 6);

    postponeAlarm(reloj, N_MINUTOS, TIME_SIZE);

    simulateTicks(TICKS_PER_MINUTE * 5 - 1);
    TEST_ASSERT_FALSE(alarmaActivada);

    simulateTicks(1);
    TEST_ASSERT_TRUE(alarmaActivada);
}

/* Test, la la alarma es pospuesta dos veces, n minutos la primera vez, y m minutos la segunda*/
void test_alarm_postponed_on_twice(void) {
    static const uint8_t ALARMA[] = {1, 2, 4, 0, 0, 0}; /*Array que representa la alarma esperada*/
    static const uint8_t N_MINUTOS = 5;                 /* Constante con la cantidad de minutos que se desea posponer */
    static const uint8_t M_MINUTOS = 15;                /* Constante con la cantidad de minutos que se desea posponer */

    setAlarm(reloj, ALARMA, TIME_SIZE);

    simulateTicks(TICKS_PER_MINUTE * 7);

    postponeAlarm(reloj, N_MINUTOS, TIME_SIZE);

    simulateTicks(TICKS_PER_MINUTE * N_MINUTOS);
    TEST_ASSERT_TRUE(alarmaActivada);

    postponeAlarm(reloj, M_MINUTOS, TIME_SIZE);
    TEST_ASSERT_FALSE(alarmaActivada);

    simulateTicks(TICKS_PER_MINUTE * M_MINUTOS);
    TEST_ASSERT_TRUE(alarmaActivada);
}

/* Test, la alarma es pospuesta 5 minutos , luego apagada. Y no vuelve a sonar hasta que pasan 23HS 55 minutos */
void test_alarm_postponed_and_shut(void) {
    static const uint8_t ALARMA[] = {1, 2, 4, 0, 0, 0}; /*Array que representa la alarma esperada*/
    static const uint8_t N_MINUTOS = 5;                 /* Constante con la cantidad de minutos que se desea posponer */

    setAlarm(reloj, ALARMA, TIME_SIZE);

    simulateTicks(TICKS_PER_MINUTE * 6);

    postponeAlarm(reloj, N_MINUTOS, TIME_SIZE);

    simulateTicks(TICKS_PER_MINUTE * 5);

    shutAlarm(reloj, TIME_SIZE);

    simulateTicks(TICKS_PER_HOUR * 23 + TICKS_PER_MINUTE * 54);
    TEST_ASSERT_FALSE(alarmaActivada);

    simulateTicks(TICKS_PER_MINUTE);
    TEST_ASSERT_TRUE(alarmaActivada);
}

/* Test, la alarma es pospuesta pero no estaba sonando (no deberia ocurrir nada)*/
void test_alarm_postponed_off(void) {
    static const uint8_t N_MINUTOS = 5; /* Constante con la cantidad de minutos que se desea posponer */

    postponeAlarm(reloj, TIME_SIZE, TIME_SIZE);
    simulateTicks(TICKS_PER_MINUTE * N_MINUTOS);
    TEST_ASSERT_FALSE(alarmaActivada);
}

/* Test, la alarma se pospone para un horario posterior a las 23:59 */
void test_alarm_postponed_midnight(void) {
    static const uint8_t HORA[] = {2, 3, 5, 0, 0, 0};   /*Array que representa la alarma esperada*/
    static const uint8_t ALARMA[] = {2, 3, 5, 4, 0, 0}; /*Array que representa la alarma esperada*/
    static const uint8_t N_MINUTOS = 5;                 /* Constante con la cantidad de minutos que se desea posponer */

    setTime(reloj, HORA, TIME_SIZE);
    setAlarm(reloj, ALARMA, TIME_SIZE);

    simulateTicks(TICKS_PER_MINUTE * 4);

    postponeAlarm(reloj, N_MINUTOS, TIME_SIZE);

    simulateTicks(TICKS_PER_MINUTE * 5 - 1);
    TEST_ASSERT_FALSE(alarmaActivada);

    simulateTicks(1);
    TEST_ASSERT_TRUE(alarmaActivada);
}

/* Test, la alarma se debe posponer 5 minutos DESDE que se pospone */
void test_alarm_postponed_5_min(void) {
    static const uint8_t ALARMA[] = {1, 2, 4, 0, 0, 0}; /*Array que representa la alarma esperada*/
    static const uint8_t N_MINUTOS = 5;                 /* Constante con la cantidad de minutos que se desea posponer */

    setAlarm(reloj, ALARMA, TIME_SIZE);

    simulateTicks(TICKS_PER_MINUTE * 10);

    postponeAlarm(reloj, N_MINUTOS, TIME_SIZE);

    simulateTicks(TICKS_PER_MINUTE * 5 - 1);
    TEST_ASSERT_FALSE(alarmaActivada);

    simulateTicks(1);
    TEST_ASSERT_TRUE(alarmaActivada);
}