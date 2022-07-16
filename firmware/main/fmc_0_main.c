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

// Con SIMUL_AFTER_FLASH_TRUE se simulan pulsos de entrada al front-end
// luego de cada reset.
// Con SIMUL_AFTER_FLASH_FALSE la simulacion esta desactivada.
// 
#define SIMUL_AFTER_FLASH_TRUE 
#define SIMUL_AFTER_FLASH_FREQ  10 
#define SIMUL_AFTER_FLASH_PULSES  100 // GENERATOR_CYCLES_PERPETUAL // Un entero o GENERATOR_CYCLES_PERPETUAL par asiempre

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