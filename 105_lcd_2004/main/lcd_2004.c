#include "lcd_2004.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "unistd.h"

#define SLAVE_ADDRESS_LCD 0x27 // change this according to ur setup

esp_err_t err;

static const char *TAG = "LCD_2004_C";

void lcd_send_cmd(char cmd)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd & 0xf0);
	data_l = ((cmd << 4) & 0xf0);
	data_t[0] = data_u | 0x0C; // en=1, rs=0
	data_t[1] = data_u | 0x08; // en=0, rs=0
	data_t[2] = data_l | 0x0C; // en=1, rs=0
	data_t[3] = data_l | 0x08; // en=0, rs=0
	err = i2c_master_write_to_device(BOARD_I2C__NUM_LCD_2004, SLAVE_ADDRESS_LCD, data_t, 4, 1000);
	if (err != 0)
	{
		ESP_LOGI(TAG, "Error in void lcd_send_cmd(char cmd)");
	}
}

void lcd_send_data(char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data & 0xf0);
	data_l = ((data << 4) & 0xf0);
	data_t[0] = data_u | 0x0D; // en=1, rs=0
	data_t[1] = data_u | 0x09; // en=0, rs=0
	data_t[2] = data_l | 0x0D; // en=1, rs=0
	data_t[3] = data_l | 0x09; // en=0, rs=0
	err = i2c_master_write_to_device(BOARD_I2C__NUM_LCD_2004, SLAVE_ADDRESS_LCD, data_t, 4, 1000);
	if (err != 0)
	{
		ESP_LOGI(TAG, "Error in void lcd_send_data(char data)");
	}
}

void lcd_clear(void)
{
	lcd_send_cmd(0x01);
	usleep(5000);
}

void lcd_put_cur(int row, int col)
{
	switch (row)
	{
	case 0:
		col |= 0x80;
		break;
	case 1:
		col |= 0xC0;
		break;
	}

	lcd_send_cmd(col);
}

/**
 * @brief i2c master initialization
 */
static esp_err_t init_i2c(void)
{
	int i2c_master_port = I2C_NUM_0;
	esp_err_t my_err;

	i2c_config_t conf = {
		.mode = I2C_MODE_MASTER,
		.sda_io_num = GPIO_NUM_5,
		.scl_io_num = GPIO_NUM_4,
		.sda_pullup_en = GPIO_PULLUP_ENABLE,
		.scl_pullup_en = GPIO_PULLUP_ENABLE,
		.master.clk_speed = 100000,
	};

	i2c_param_config(i2c_master_port, &conf);

	my_err = i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);

	return my_err;
}

void lcd_send_string(char *str)
{
	while (*str)
		lcd_send_data(*str++);
}

void lcd_2004_init(void)
{

	ESP_ERROR_CHECK(init_i2c());

	// 4 bit initialisation
	usleep(50000); // wait for >40ms
	lcd_send_cmd(0x30);
	usleep(5000); // wait for >4.1ms
	lcd_send_cmd(0x30);
	usleep(200); // wait for >100us
	lcd_send_cmd(0x30);
	usleep(10000);
	lcd_send_cmd(0x20); // 4bit mode
	usleep(10000);

	// dislay initialisation
	lcd_send_cmd(0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	usleep(1000);
	lcd_send_cmd(0x08); // Display on/off control --> D=0,C=0, B=0  ---> display off
	usleep(1000);
	lcd_send_cmd(0x01); // clear display
	usleep(1000);
	usleep(1000);
	lcd_send_cmd(0x06); // Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	usleep(1000);
	lcd_send_cmd(0x0C); // Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
	usleep(1000);

	lcd_clear();
}