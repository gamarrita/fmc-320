#ifndef FMC_BOARD_H
#define FMC_BOARD_H

#include "driver/gpio.h"

uint32_t board_get_inputPulses();
void board_input_pulse_init();

// BOARD_OUTPUT_PULSE = 1:
// -Pulsos de salida, opto-aislados
//
// BOARD_LED_ALIVE:
// - Encendido 1 seg y apagago 1 ssegndo si todo esta OK y no hay señal del front-end
// - Encendido 100 ms y apagago 1900 ms si todo esta OK y hay señal del front-end
// 
// BOARD_OUTPUT_GENERATOR
// - Salida del generado de pulsos internos,programable en frecuencia y cantidad de pulsos
//
enum GPIO_OUTPUT
{
    BOARD_OUTPUT_PULSE = 1,
    BOARD_LED_ALIVE = 2,
    BOARD_OUTPUT_GENERATOR  = 42,
};

enum GPIO_INPUT
{
    BOARD_INPUT_PULSE  = 41,
};

typedef struct
{
    uint32_t factor_calib;
    uint32_t factor_out;
    uint32_t factor_volume;
    uint32_t ttl_pulse;
    uint32_t acm_pulse;
    float rate;
    uint64_t acm_volume;
    uint64_t ttl_volume;
}fmc_data;

void board_update_toalizer(fmc_data *totalizer);

#endif

