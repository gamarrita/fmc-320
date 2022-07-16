#include "fmc_board.h"

uint32_t global_input_pulses;

// Los pulsos desde el front-end se cuentan uno a uno, en modo de alto consumo
// esto pemite:
// - Tener un señal de pulsos de salida que reacciona instantaneamente con los
// pulsos de entrada, ciclo de trabajo de 50%, es la forma "optima"
// - Se pueden convertir la cantidad de pulsos de entrada a volumen justo al 
// instante posterior de su ingreso, no hay retardo en la comparacion entre
// el volumen de lote programado y el volumena acumulado
// Para el bajo consumo estas funciones no son necesarias, entonces es conveniente
// que estos pulsos tambien ingresen a un timer, luego se puede decidir que 
// dependiendo del modo de alimentacion, bajo consumo con bateria o fuente externa,
// se utilice el timer para acumular pulsos del front-end o se atiendad uno a uno
// con la interrupcion. Si el costo energetico no es significativo se puede usar
// la interrupcion como unico metodo.
static void IRAM_ATTR inputPulse_isr_handle()
{
    global_input_pulses++;
}


void board_input_pulse_init()
{
    gpio_pad_select_gpio(BOARD_INPUT_PULSE);
    gpio_set_direction(BOARD_INPUT_PULSE, GPIO_MODE_INPUT);
    gpio_set_intr_type(BOARD_INPUT_PULSE, GPIO_INTR_NEGEDGE);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BOARD_INPUT_PULSE, inputPulse_isr_handle, (void *)BOARD_INPUT_PULSE);
}

//
//
uint32_t board_pull_inputPulses()
{
    uint32_t new_pulses = global_input_pulses;
    global_input_pulses = 0;
    return new_pulses;
}

//
void board_update_toalizer(fmc_data *totalizer)
{
    uint32_t new_pulses = board_pull_inputPulses();

    if (new_pulses)
    {
        totalizer->ttl_pulse += new_pulses;
        totalizer->acm_pulse += new_pulses;

        totalizer->ttl_volume = totalizer->ttl_pulse;
        totalizer->ttl_volume *= 1000000;
        totalizer->ttl_volume /= totalizer->factor_volume;

        totalizer->acm_volume = totalizer->acm_pulse;
        totalizer->acm_volume *= 1000000;
        totalizer->acm_volume /= totalizer->factor_volume;
        totalizer->rate = new_pulses;
    }
}