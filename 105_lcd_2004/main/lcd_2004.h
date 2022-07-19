#ifndef I2C_LCD_2004_H
#define I2C_LCD_2004_H

// Los siguientes defines hacen uso de los recursos de microctontrolador,
// en el proyecto final es buena idea moverlos a un archivo donde esten
// todos juntos los recuersos del microcontrolador
#define BOARD_I2C_SDA 5
#define BOARD_I2C_SCL 4
#define BOARD_I2C__NUM_LCD_2004 I2C_NUM_0


void lcd_2004_init (void);   // initialize lcd
void lcd_send_cmd (char cmd);  // send command to the lcd
void lcd_send_data (char data);  // send data to the lcd
void lcd_send_string (char *str);  // send string to the lcd
void lcd_put_cur(int row, int col);  // put cursor at the entered position row (0 or 1), col (0-15);
void lcd_clear (void);

#endif