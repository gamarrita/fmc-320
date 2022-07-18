/*
 Objetivo: Determinar el tiempo de arranque, booteo, del ESP32 al salir
 del modo deep sleep.
 
 Circuito: ver imagen, se usaron dos ESP32-s3,uno para merdir el retardo
 el otro que entra y sale del modo sleep
  
 Explicacion: El chip bootea, inmediatamente luego de eso se pasa GPIO 2 a estado alto
duranto 5 segundos, luego se lo manda a sleep durante durante cero  microsegundos,
al salir del sleep, casi instantaneamente, se produce el reset, el GPIO 2
queda flotando pero se conecto un un pulldown, esta es la señal que recibe el esp32,
programado con el codigo 001_boot_monitor, el timer del esp32 que actua como moitor
cuenta hasta que este esp32 termina el booteo y pasa GPIO 2 nuevamente a uno. Todas
las lecturas e ven en el ESP32 monitor.
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_sleep.h"
#include "driver/gpio.h"

#define BOARD_LED_ALIVE 2

void app_main(void)
{
  gpio_pad_select_gpio(BOARD_LED_ALIVE);
  gpio_set_direction(BOARD_LED_ALIVE, GPIO_MODE_OUTPUT);
  //gpio_deep_sleep_hold_en();
  gpio_set_level(BOARD_LED_ALIVE, 1);
 	//gpio_hold_en((gpio_num_t)BOARD_LED_ALIVE);

  vTaskDelay(5000 / portTICK_PERIOD_MS);
            
  esp_sleep_enable_timer_wakeup(0);
  esp_deep_sleep_start();
}




 