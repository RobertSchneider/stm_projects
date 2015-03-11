/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_rcc.h"
#include "SSD1289.h"
#include "XPT2046.h"
#include "LCD.h"
#include <stdio.h>
#include <stdlib.h>
#include "Random.h"
#include "maths.h"

char* convert_dez(int i)
{
  	int cnt = 0;
  	int icopy = i;
  	while (icopy != 0)
  	{
    	cnt++;
    	icopy /= 10;
  	}
  	char *c = (char*)malloc(cnt+1);
  	int ind = cnt - 1;
  	while (i != 0)
  	{
    	c[ind--] = (i % 10) + (int)'0';
    	i /= 10;
  	}
  	c[cnt] = 0x00;
  	return c;
}

void button_clicked(ButtonInfo *sender)
{
  	lcd_highlightButton(sender);
}

int main(void)
{
  	InitializeSystem();
  	RCC_ClocksTypeDef RCC_Clocks;
  	RCC_GetClocksFreq(&RCC_Clocks);
  	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    InitializeRandom();

  	InitializeLEDs();
  	EnableDebugOutput(DEBUG_ITM);
  	lcd_initLCD();
  	DelayMS(100);

  	char *title = "Sainsmart 3.2 TFT";
  	int xT = MAX_X/2-strlen(title)/2*8;
  	int yT = MAX_Y/2-16/2;
  	SSD1289_CleanTextFont(xT, yT, title, Red, SYSTEM_8x16);
  	DelayMS(1000);
  	SSD1289_Clear(Black);

    while(1)
    {

    }
}