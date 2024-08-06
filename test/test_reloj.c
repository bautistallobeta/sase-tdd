
#include <stdint.h>

#include "clock.h"
#include "unity.h"

/* Tamano del array de tiempo ( ej: si es 6, entonces [H, H, M, M, S, S]) */
#define TIME_SIZE 6
/* Ticks por segundo que recibe el clock */
#define TICKS_PER_SECOND 5
/* Ticks por minuto */
#define TICKS_PER_MINUTE TICKS_PER_SECOND * 60
/* Ticks por hora */
#define TICKS_PER_HOUR TICKS_PER_MINUTE * 60
/* Ticks por dia */
#define TICKS_PER_DAY TICKS_PER_HOUR * 24

/* Puntero a instancia de reloj */
clocks_t reloj;
/* Variable de estado de activacion de la alarma (true = sonando ; false = apagada) */
static bool alarmaActivada;

/* Funcion auxiliar para simular el paso de n ticks */
void simulateTicks(uint32_t ticks) {
    for (int i = 0; i < ticks; i++) {
        newTick(reloj);
    }
}

/* Funcion que llama el reloj para indicar el encendido/apagado de la alarma */
void alarmEvent(clocks_t reloj, bool state) {
    alarmaActivada = state;
}

/* Funcion de setup para inicializar el reloj en un estado predeterminado para las pruebas */
void setUp() {
    static const uint8_t INICIAL[] = {1, 2, 3, 4, 0, 0};

    alarmaActivada = false;

    reloj = createClock(TICKS_PER_SECOND, alarmEvent);

    setTime(reloj, INICIAL, sizeof(INICIAL));
}

/* Test de inicializacion del reloj */
void test_start_up(void) {
    /*Array con la hora esperada*/
    static const uint8_t ESPERADO[] = {0, 0, 0, 0, 0, 0};
    /* Variable para almacenar la hora seteada*/
    uint8_t hora[TIME_SIZE] = {0xFF};

    /* Inicializacion del reloj*/
    reloj = createClock(TICKS_PER_SECOND, alarmEvent);

    /* Test de la validez de la hora*/
    TEST_ASSERT_FALSE(getTime(reloj, hora, TIME_SIZE));
    /* Test del resultado de getTime */
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, TIME_SIZE);
}

/* Test de seteo de hora */
void test_set_time(void) {
    /*Array que con la hora esperada*/
    static const uint8_t ESPERADO[] = {1, 2, 3, 4, 0, 0};
    /* Variable parar almacenar la hora seteada */
    uint8_t hora[TIME_SIZE] = {0xFF};

    /* Test de validez de la hora */
    TEST_ASSERT_TRUE(getTime(reloj, hora, TIME_SIZE));
    /* Test del resultado de getTime */
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, TIME_SIZE);
}

/* Test del paso de un segundo */
void test_one_second(void) {
    /*Array que con la hora esperada*/
    static const uint8_t ESPERADO[] = {1, 2, 3, 4, 0, 1};
    /* Variable parar almacenar la hora seteada */
    uint8_t hora[TIME_SIZE] = {0xFF};

    /* Simulacion de un segundo */
    simulateTicks(TICKS_PER_SECOND);

    getTime(reloj, hora, TIME_SIZE);

    /* Test del resultado de getTime */
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, TIME_SIZE);
}

/* Test del paso de 10 segundos */
void test_ten_seconds(void) {
    /*Array que con la hora esperada*/
    static const uint8_t ESPERADO[] = {1, 2, 3, 4, 1, 0};
    /* Variable parar almacenar la hora seteada */
    uint8_t hora[TIME_SIZE] = {0xFF};

    simulateTicks(TICKS_PER_SECOND * 10);

    getTime(reloj, hora, TIME_SIZE);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, TIME_SIZE);
}

/* Test del paso de 1 minuto */
void test_one_minute(void) {
    /*Array que con la hora esperada*/
    static const uint8_t ESPERADO[] = {1, 2, 3, 5, 0, 0};
    /* Variable parar almacenar la hora seteada */
    uint8_t hora[TIME_SIZE] = {0xFF};

    simulateTicks(TICKS_PER_MINUTE);

    getTime(reloj, hora, TIME_SIZE);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, TIME_SIZE);
}

/* Test del paso de 10 minutos */
void test_ten_minutes(void) {
    /*Array que con la hora esperada*/
    static const uint8_t ESPERADO[] = {1, 2, 4, 4, 0, 0};
    /* Variable parar almacenar la hora seteada */
    uint8_t hora[TIME_SIZE] = {0xFF};

    simulateTicks(TICKS_PER_MINUTE * 10);

    getTime(reloj, hora, TIME_SIZE);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, TIME_SIZE);
}

/* Test del paso de 1 hora */
void test_one_hour(void) {
    /*Array que con la hora esperada*/
    static const uint8_t ESPERADO[] = {1, 3, 3, 4, 0, 0};
    /* Variable parar almacenar la hora seteada */
    uint8_t hora[TIME_SIZE] = {0xFF};

    simulateTicks(TICKS_PER_HOUR);

    getTime(reloj, hora, TIME_SIZE);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, TIME_SIZE);
}

/* Test del paso de 10 horas */
void test_ten_hours(void) {
    /*Array que con la hora esperada*/
    static const uint8_t ESPERADO[] = {2, 2, 3, 4, 0, 0};
    /* Variable parar almacenar la hora seteada */
    uint8_t hora[TIME_SIZE] = {0xFF};

    simulateTicks(TICKS_PER_HOUR * 10);

    getTime(reloj, hora, TIME_SIZE);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, TIME_SIZE);
}

/* Test del paso de 1 dia */
void test_one_day(void) {
    /*Array que con la hora esperada*/
    static const uint8_t ESPERADO[] = {1, 2, 3, 4, 0, 0};
    /* Variable parar almacenar la hora seteada */
    uint8_t hora[TIME_SIZE] = {0xFF};

    simulateTicks(TICKS_PER_DAY);

    getTime(reloj, hora, TIME_SIZE);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, TIME_SIZE);
}

/* Test de seteo y activacion de alarma*/
void test_set_alarm(void) {
    /*Array que con la alarma esperada*/
    static const uint8_t ESPERADO[] = {1, 2, 4, 0, 0, 0};
    uint8_t hora[TIME_SIZE] = {0xFF};

    setAlarm(reloj, ESPERADO, TIME_SIZE);

    TEST_ASSERT_TRUE(getAlarm(reloj, hora, TIME_SIZE));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, TIME_SIZE);
}

/* Test de desactivacion de alarma (ACLARACION: no es el apagado o posposición de la alarma)*/
void test_desactivate_alarm(void) {
    /*Array que con la alarma esperada*/
    static const uint8_t ESPERADO[] = {1, 2, 4, 0, 0, 5};

    uint8_t hora[TIME_SIZE] = {0xFF};

    setAlarm(reloj, ESPERADO, TIME_SIZE);

    desactivateAlarm(reloj);

    TEST_ASSERT_FALSE(getAlarm(reloj, hora, TIME_SIZE));
}

/*Test de alarma activada, no suena hasta llegar la hora seteada*/
void test_alarm_activated_on(void) {
    /*Array que con la alarma esperada*/
    static const uint8_t ALARMA[] = {1, 2, 4, 0, 0, 0};

    setAlarm(reloj, ALARMA, TIME_SIZE);

    simulateTicks(TICKS_PER_MINUTE * 6 - 1);
    TEST_ASSERT_FALSE(alarmaActivada);

    simulateTicks(1);
    TEST_ASSERT_TRUE(alarmaActivada);
}

/*Test de alarma, el reloj llega a la hora seteada pero la alarma esta desactivada*/
void test_alarm_desactivated_off(void) {
    /*Array que con la alarma esperada*/
    static const uint8_t ESPERADO[] = {1, 2, 4, 0, 0, 0};

    uint8_t hora[TIME_SIZE] = {0xFF};

    setAlarm(reloj, ESPERADO, TIME_SIZE);

    desactivateAlarm(reloj);

    simulateTicks(TICKS_PER_MINUTE * 7);

    TEST_ASSERT_FALSE(alarmaActivada);
}

/*Test de alarma, la alarma suena y pasan 24hs sin pararse ni posponerse (debe seguir sonando)*/
void test_alarm_already_on(void) {
    /*Array que con la alarma esperada*/
    static const uint8_t ALARMA[] = {1, 2, 4, 0, 0, 0};

    setAlarm(reloj, ALARMA, TIME_SIZE);

    /*Suena la alarma*/
    simulateTicks(TICKS_PER_MINUTE * 6);
    /*Pasan 24hs*/
    simulateTicks(TICKS_PER_DAY);
    /*Test para comprobar si sigue sonando*/
    TEST_ASSERT_TRUE(alarmaActivada);
}

/* Test de alarma, se apaga la alarma cuando SI esta sonando (ACLARACIÓN: no es la desactivacion de la alarma) */
void test_alarm_shut(void) {
    /*Array que con la alarma esperada*/
    static const uint8_t ALARMA[] = {1, 2, 4, 0, 0, 0};

    setAlarm(reloj, ALARMA, TIME_SIZE);

    simulateTicks(TICKS_PER_MINUTE * 7);
    shutAlarm(reloj, TIME_SIZE);
    TEST_ASSERT_FALSE(alarmaActivada);
}

/*Test de alarma, se apaga de la alarma cuando NO esta sonando (no debe suceder nada)*/
void test_alarm_shut_when_off(void) {
    shutAlarm(reloj, TIME_SIZE);
    TEST_ASSERT_FALSE(alarmaActivada);
}

/* Test de posposicion de alarma cuando SI esta sonando*/
void test_alarm_postponed(void) {
    /*Array que con la alarma esperada*/
    static const uint8_t ALARMA[] = {1, 2, 4, 0, 0, 0};
    /* Constante con la cantidad de minutos que se desea posponer */
    static const uint8_t N_MINUTOS = 5;

    setAlarm(reloj, ALARMA, TIME_SIZE);

    simulateTicks(TICKS_PER_MINUTE * 5);

    postponeAlarm(reloj, N_MINUTOS, TIME_SIZE);

    TEST_ASSERT_FALSE(alarmaActivada);
}

/* Test de alarma, se pospone pero no estaba sonando (no deberia ocurrir nada)*/
void test_alarm_postponed_off(void) {
    /* Constante con la cantidad de minutos que se desea posponer */
    static const uint8_t N_MINUTOS = 5;

    postponeAlarm(reloj, TIME_SIZE, TIME_SIZE);
    simulateTicks(TICKS_PER_MINUTE * N_MINUTOS);
    TEST_ASSERT_FALSE(alarmaActivada);
}

/* Test de alarma,  es pospone n minutos, y no suena hasta pasarse n minutos*/
void test_alarm_postponed_on(void) {
    /*Array que con la alarma esperada*/
    static const uint8_t ALARMA[] = {1, 2, 4, 0, 0, 0};
    /* Constante con la cantidad de minutos que se desea posponer */
    static const uint8_t N_MINUTOS = 5;

    setAlarm(reloj, ALARMA, TIME_SIZE);

    simulateTicks(TICKS_PER_MINUTE * 6);

    postponeAlarm(reloj, N_MINUTOS, TIME_SIZE);

    simulateTicks(TICKS_PER_MINUTE * 5 - 1);
    TEST_ASSERT_FALSE(alarmaActivada);

    simulateTicks(1);
    TEST_ASSERT_TRUE(alarmaActivada);
}

/* Test de alarma, se pospone dos veces, n minutos la primera vez, y m minutos la segunda*/
void test_alarm_postponed_on_twice(void) {
    /*Array que con la alarma esperada*/
    static const uint8_t ALARMA[] = {1, 2, 4, 0, 0, 0};
    /* Constante con la cantidad de minutos que se desea posponer */
    static const uint8_t N_MINUTOS = 5;
    /* Constante con la cantidad de minutos que se desea posponer */
    static const uint8_t M_MINUTOS = 15;

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

/* Test de alarma, suena, se pospone 5 minutos y luego se apaga. No debe volver a sonar hasta que pasan 23HS 55 minutos */
void test_alarm_postponed_and_shut(void) {
    /*Array que con la alarma esperada*/
    static const uint8_t ALARMA[] = {1, 2, 4, 0, 0, 0};
    /* Constante con la cantidad de minutos que se desea posponer */
    static const uint8_t N_MINUTOS = 5;

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

/* Test de alarma, se pospone para un horario posterior a las 23:59 */
void test_alarm_postponed_midnight(void) {
    /*Array con la hora a setear*/
    static const uint8_t HORA[] = {2, 3, 5, 0, 0, 0};
    /*Array con la alarma esperada*/
    static const uint8_t ALARMA[] = {2, 3, 5, 4, 0, 0};
    /* Constante con la cantidad de minutos que se desea posponer */
    static const uint8_t N_MINUTOS = 5;

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
    /*Array con la alarma esperada*/
    static const uint8_t ALARMA[] = {1, 2, 4, 0, 0, 0};
    /* Constante con la cantidad de minutos que se desea posponer */
    static const uint8_t N_MINUTOS = 5;

    setAlarm(reloj, ALARMA, TIME_SIZE);

    simulateTicks(TICKS_PER_MINUTE * 10);

    postponeAlarm(reloj, N_MINUTOS, TIME_SIZE);

    simulateTicks(TICKS_PER_MINUTE * 5 - 1);
    TEST_ASSERT_FALSE(alarmaActivada);

    simulateTicks(1);
    TEST_ASSERT_TRUE(alarmaActivada);
}