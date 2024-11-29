#include "Button.h"
volatile uint8_t btn_current = 1;
volatile uint8_t btn_last = 1;
volatile uint8_t btn_filter = 1;
volatile uint8_t is_debouncing;
volatile uint32_t time_deboune;
volatile uint32_t time_start_press;
volatile uint8_t is_press_timeout;
uint8_t test;
__weak void btn_pressing_callback()
{
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}
__weak void btn_press_short_callback()
{
	test =3;
}
__weak void btn_release_callback()
{
	test =2;
}
__weak void btn_press_timeout_callback()
{
	test =4;
}


__weak void button_handle()
{
	//------------------ Loc nhieu ------------------------
	uint8_t sta = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
	if(sta != btn_filter)
	{
		btn_filter = sta;
		is_debouncing = 1;
		time_deboune = HAL_GetTick();
	}
	//------------------ Tin hieu da xac lap------------------------
	if(is_debouncing && (HAL_GetTick() - time_deboune >= 15))
	{
		btn_current = btn_filter;
		is_debouncing =0;
	}
	//---------------------Xu li nhan nha------------------------
	if(btn_current != btn_last)
	{
		if(btn_current == 0)//nhan xuong
		{
			is_press_timeout = 1;
			btn_pressing_callback();
			time_start_press = HAL_GetTick();
		}
		else //nha nut
		{
			if(HAL_GetTick() - time_start_press <= 1000)
			{
				btn_press_short_callback();
			}
			btn_release_callback();
		}
		btn_last = btn_current;
	}
	//-------------Xu li nhan giu----------------
	if(is_press_timeout && (HAL_GetTick() - time_start_press >= 3000))
	{
		btn_press_timeout_callback();
		is_press_timeout =0;
	}
}