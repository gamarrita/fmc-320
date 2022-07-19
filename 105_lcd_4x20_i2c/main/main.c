/*
LCD 4x20 con conexion I2C
*/
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "lcd_2004.h"

const char TAG[] = "MAIN_C";

char buffer[10];
float num = 12.34;

void app_main(void)
{
    
    ESP_LOGI(TAG, "I2C initialized successfully");

    lcd_2004_init();
    

    lcd_send_cmd(0x80 | 0x00);
    lcd_send_string("TTL 12345678.123 Lt");

    lcd_send_cmd(0x80 | 0x40);
    lcd_send_string("ACM  12345.123 Lt");

    lcd_send_cmd(0x80 | 0x14);
    lcd_send_string("23.1 } 1234.12 Lt/m");

    lcd_send_cmd(0x80 | 0x54);
    lcd_send_string("18/7/2022 19:46:33");
    printf("Hola mundo\n");
}
