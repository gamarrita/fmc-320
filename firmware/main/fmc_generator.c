#include "fmc_board.h"
#include "fmc_generator.h"

// La funcion esp_timer_create tiene como parametro el retardo en micro-segundos,
// si T es el retardo se necesitan 2T para un ciclo completo, para convertir a
// frecuencia enbtonces:
// T = 500.000 -> frecuencia del generador igual a 1Hz
// T = 500.000 / f > frecuencia del generador igual f
#define HALF_CYCLE_MICRO_SECONDS 500000

#define FREQ_MAX 3000 // Frecuencia maxima que se puede generar 1000 Hz

// Se puede trabajar en modo perpetuo o generar una cantidad determinada de clicos
#define CYCLES_MAX 0X7FFFFFFF // Limite de ciclos en modo no perpetuo


uint32_t globalCyclesCounter;


void timer_callback(void *args)
{
    static uint32_t outputOne = 0;
    if(globalCyclesCounter == GENERATOR_CYCLES_PERPETUAL) {
        outputOne = !outputOne;
        gpio_set_level(BOARD_OUTPUT_GENERATOR, outputOne);
        return;
    }
   
    if(globalCyclesCounter) {
        outputOne = !outputOne;
        gpio_set_level(BOARD_OUTPUT_GENERATOR, outputOne);
    
        // Un ciclo completo son dos llamadas, on y off entonces resto un clico a los pendientes
        if(outputOne == false){
                globalCyclesCounter--;
        }
    }
    else{
        // Detener el timer
    }
}


esp_timer_handle_t generator_init()
{
    gpio_pad_select_gpio(BOARD_OUTPUT_GENERATOR);
    gpio_set_direction(BOARD_OUTPUT_GENERATOR, GPIO_MODE_OUTPUT);

        const esp_timer_create_args_t esp_timer_create_args = {
        .callback = timer_callback,
        .name = "pulse generator"
    };

    esp_timer_handle_t esp_timer_handle;
    esp_timer_create(&esp_timer_create_args, &esp_timer_handle);
    
    return esp_timer_handle;
}



void generator_start(esp_timer_handle_t esp_timer_handle, uint32_t freq, uint32_t run_cycles)
{

    if(run_cycles < CYCLES_MAX)
    {
        globalCyclesCounter =  run_cycles; 
    }
    else
    {
        // La cantidad maxima de ciclos es el limite de uint64_t
        // para cantidades mayores se toma como infinito ciclos
        globalCyclesCounter = GENERATOR_CYCLES_PERPETUAL;
    }

    if(freq == 0)
    {
        freq = 1;
    }

    if(freq > FREQ_MAX)
    {
        freq = FREQ_MAX;
    }
    esp_timer_start_periodic(esp_timer_handle, HALF_CYCLE_MICRO_SECONDS / freq);
}

