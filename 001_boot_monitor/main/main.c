// Este programa fue usado para medir el tiempo de arranque al salir 
// del modo:
//
// Ligth sleep
// Deep sleep
// Hinbernacion
//


#include <stdio.h>
#include "esp_timer.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define BOARD_INPUT_PULSE 41

uint64_t micros_start;
uint64_t micros_end;

static void IRAM_ATTR inputPulse_isr_handle()
{
    static int isOn = 0;
    isOn = gpio_get_level(BOARD_INPUT_PULSE);
    
    if(isOn == 0)
    {
        micros_start = esp_timer_get_time();
        micros_end = micros_start;
    }
    else
    {
        micros_end = esp_timer_get_time();
    }
}

void app_main(void)
{
    int boot_counter = 3;

    gpio_pad_select_gpio(BOARD_INPUT_PULSE);
    gpio_set_direction(BOARD_INPUT_PULSE, GPIO_MODE_INPUT);
    gpio_pulldown_en(BOARD_INPUT_PULSE);

    gpio_set_intr_type(BOARD_INPUT_PULSE, GPIO_INTR_ANYEDGE);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BOARD_INPUT_PULSE, inputPulse_isr_handle, (void *)BOARD_INPUT_PULSE);
   
    while(true)
    {
         while(micros_start == micros_end)
         {
            vTaskDelay(100 / portTICK_PERIOD_MS);
         }
         printf("Tiempo de booteo %d =  %lld mili-segundos\n", boot_counter, (micros_end - micros_start)/1000);
         micros_end  = micros_start;
         boot_counter++;
    }
}
