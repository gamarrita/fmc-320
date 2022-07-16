/*
Mejoras regitradas en
https://stackoverflow.com/questions/72974115/esp32-s3-booting-time/73006528#73006528
*/



#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_sleep.h"
#include "driver/gpio.h"
#include "esp_log.h"


#define BOARD_LED_ALIVE 2


void app_main(void)
{
   esp_log_level_set("LOG", ESP_LOG_INFO); // no tiene efecto no controla lo del boot

  gpio_pad_select_gpio(BOARD_LED_ALIVE);
  gpio_set_direction(BOARD_LED_ALIVE, GPIO_MODE_OUTPUT);
  //gpio_deep_sleep_hold_en();
  gpio_set_level(BOARD_LED_ALIVE, 1);
 	//gpio_hold_en((gpio_num_t)BOARD_LED_ALIVE);

  vTaskDelay(3000 / portTICK_PERIOD_MS);
            
  esp_sleep_enable_timer_wakeup(0);
  esp_deep_sleep_start();
}




 