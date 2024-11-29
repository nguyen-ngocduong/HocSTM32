#ifndef FIREALARM_H
#define FIREALARM_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void Delay_ms_1(uint16_t _time);
void Input(void);
void Led_Init(void);
void Relay_Init(void);
void DelayInit(void);
void DelayUs(uint32_t us);
void DelayMs(uint32_t ms);
void GPIO_LCD_Config(void);
void LCD_Enable(void);
void LCD_Send4Bit(unsigned char Data);
void LCD_SendCommand(unsigned char command);
void LCD_Clear();
void LCD_Init();
void LCD_Gotoxy(unsigned char x, unsigned char y);
void LCD_PutChar(unsigned char Data);
void LCD_Puts(char *s);

#endif
