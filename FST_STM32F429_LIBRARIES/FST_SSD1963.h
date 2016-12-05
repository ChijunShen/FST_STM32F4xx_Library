#ifndef FST__SSD1963_H
#define FST__SSD1963_H

#include "main.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

#define SSD_BACKLIT GPIO_Pin_12



void FST_SSD1963_Port_Init(void);
uint8_t FST_SSD1963_SLOW_Init(void);
uint8_t FST_SSD1963_FAST_Init(void);

extern void FST_SSD1963_Init(void);

void FST_SSD1963_Write_cmd(uint16_t value);
void FST_SSD1963_Write_data(uint16_t value);

void FST_SSD1963_Clear(uint16_t color);

void FST_SSD1963_SetPos(uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye);
/*************************************************/
/*______### CONVERT 24bit to 565 color  ###______*/
/*_____________###    SSD 1963   ###_____________*/
/*                                               */
/*************************************************/
uint16_t FST_SSD1963_ConvertColor(uint32_t color);

void delay_time(unsigned int i);

#endif
