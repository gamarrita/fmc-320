#include <stdio.h>
#include <string.h>
#include "freertos/FreeRtos.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_system.h"
#include "esp_timer.h"
#include "esp_log.h"

#include "fmc_board.h"
#include "fmc_debug.h"
#include "fmc_generator.h"

// Este caudalimetro tiene un generador de pulsos internos que se podria activar en culaquier momento
// Convenientemente durante el desarrollo y posereor debuger se dejan los siguientes #definesc que
// controlaran la simulacion de pulsos luego de cada reset, la acciones de los siguientes defines son:
//
// - #define SIMUL_AFTER_FLASH_TRUE -> habilita o desabilita la generacion del pulsos luego de cada reset,
//   termina en _TRUE si queremos simular pulsos, terminado _FALSE, o cualquier otra cosa queda deshabilitado.
//
// - #SIMUL_AFTER_FLASH_FREQ -> un entero de 1 a 2000 para representar la frecuencia en Hz
//
// - #SIMUL_AFTER_FLASH_PULSES -> un entero de 1 a 2000 millones para la cantidad de pulsos, o GENERATOR_CYCLES_PERPETUAL para infinitos
#define SIMUL_AFTER_FLASH_TRUE 
#define SIMUL_AFTER_FLASH_FREQ  10 
#define SIMUL_AFTER_FLASH_PULSES  50 // GENERATOR_CYCLES_PERPETUAL // Un entero o GENERATOR_CYCLES_PERPETUAL par asiempre

fmc_data totalizer;

void task_alive()
{
    // El siguiente codigo comentado se puede usar para medir el tiempo e ejecucon de codigo
    // uint64_t micros_start = esp_timer_get_time();
    // uint64_t micros_end = esp_timer_get_time();
    // printf("Tiempo de calculos = %lld micro-segundos\n", micros_end - micros_start);
    while (true)
    {
        board_update_toalizer(&totalizer);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        debug_log(&totalizer);
    }
}

void app_main(void)
{
    totalizer.factor_calib = 1000;
    totalizer.factor_out = 1000;
    totalizer.factor_volume = 1000;
    totalizer.ttl_pulse = 0;
    totalizer.acm_pulse = 0;
    totalizer.rate = 0.0;
    totalizer.acm_volume = 0;
    totalizer.ttl_volume = 0;

    xTaskCreate(&task_alive, "TaskAlive", 2048, NULL, 1, NULL);
    board_input_pulse_init();
   
    esp_timer_handle_t esp_timer_handle = generator_init();

#ifdef SIMUL_AFTER_FLASH_TRUE
    generator_start(esp_timer_handle, SIMUL_AFTER_FLASH_FREQ, SIMUL_AFTER_FLASH_PULSES);
#endif

}