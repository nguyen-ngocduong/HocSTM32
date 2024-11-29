#include "FireAlarm.h"



int main(void)
{
		Input();
		Led_Init();
		Relay_Init();
		DelayInit();
		LCD_Init();
		int gasdetected = 0;
		int flamedetected = 0;
		int lcd_clear = 1;
		int check = 0;
    while(1)
    {		
			
        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET && gasdetected == 0)	// Flame xuat tin hieu Low(phat hien lua)
        {		
						flamedetected = 1;
						check = 1;
						if(lcd_clear)
						{
							LCD_SendCommand(0x01); // xoa toan bo khung hinh
							lcd_clear = 0;
						}
						LCD_Gotoxy(0,0);
						LCD_Puts("WARNING");
						LCD_Gotoxy(0,1);
						LCD_Puts("FLAME DETECTED");
						GPIOC->BRR = (1<<14);	// kich hoat coi
						GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_RESET); // kich hoat relay
						while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET)
						{	// Led nhay canh bao
							GPIOC->BRR = (1<<13);
							Delay_ms_1(50);
							GPIOC->BSRR = (1<<13);
							Delay_ms_1(50);
							if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == Bit_RESET)
							{
									gasdetected = 1;
									lcd_clear = 1;
									break;
							}
						}
        }
				else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == Bit_RESET && flamedetected == 0) // MQ2 xuat tin hieu Low(phat hien khoi)
				{			
							gasdetected = 1;
							check = 1;
							if(lcd_clear)
							{
								LCD_SendCommand(0x01); // xoa toan bo khung hinh
								lcd_clear = 0;
							}
							LCD_Gotoxy(0, 0);
							LCD_Puts("WARNING");
							LCD_Gotoxy(0, 1);
							LCD_Puts("GAS DETECTED");
							GPIOC->BRR = (1<<14);	// kich hoat coi
							while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == Bit_RESET)
							{	// Led nhay canh bao
								GPIOC->BRR = (1<<13);
								Delay_ms_1(50);
								GPIOC->BSRR = (1<<13);
								Delay_ms_1(50);
								if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET)
								{
										flamedetected = 1;
										lcd_clear = 1;
										break;
								}
							}
				}
				else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET && GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == Bit_RESET)
				{		
						check = 1;
						if(lcd_clear)
						{
							LCD_SendCommand(0x01); // xoa toan bo khung hinh
							lcd_clear = 0;
						}
            LCD_Gotoxy(0, 0);
            LCD_Puts("GAS DETECTED");
            LCD_Gotoxy(0, 1);
            LCD_Puts("FLAME DETECTED");
						GPIOC->BRR = (1<<14);	// kich hoat coi
						GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_RESET); // kich hoat relay
						while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET && GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == Bit_RESET)
						{	// Led nhay canh bao
							GPIOC->BRR = (1<<13);
							Delay_ms_1(50);
							GPIOC->BSRR = (1<<13);
							Delay_ms_1(50);
						}
				}
        else
        {
            GPIOC->BSRR = (1<<13); // Tat led
						GPIOC->BSRR = (1<<14);// Tat coi
						GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_SET); // Ngat relay
						gasdetected = 0;
						flamedetected = 0;
						lcd_clear = 1;
						if(check){
							LCD_SendCommand(0x01); // Clear the LCD screen
							check = 0;
						}
            LCD_Gotoxy(0, 0);
            LCD_Puts("BTL NHOM_7");
            LCD_Gotoxy(0, 1);
            LCD_Puts("NO DETECTED");
        }
    }
}

