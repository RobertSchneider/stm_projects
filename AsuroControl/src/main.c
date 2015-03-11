/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_rcc.h"
#include "SSD1289.h"
#include "XPT2046.h"
#include "LCD.h"
#include <stdio.h>
#include <stdlib.h>
#include "Random.h"
#include "RF.c"

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

void adc_configure() {
    ADC_InitTypeDef ADC_init_structure;
    GPIO_InitTypeDef GPIO_initStructre;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOCEN,ENABLE);
    GPIO_initStructre.GPIO_Pin = GPIO_Pin_0;
    GPIO_initStructre.GPIO_Mode = GPIO_Mode_AN;
    GPIO_initStructre.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC,&GPIO_initStructre);
    ADC_DeInit();
    ADC_init_structure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_init_structure.ADC_Resolution = ADC_Resolution_12b;
    ADC_init_structure.ADC_ContinuousConvMode = ENABLE;
    ADC_init_structure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_init_structure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_init_structure.ADC_NbrOfConversion = 1;
    ADC_init_structure.ADC_ScanConvMode = DISABLE;
    ADC_Init(ADC1,&ADC_init_structure);
    ADC_Cmd(ADC1,ENABLE);
    ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_144Cycles);
}
void interrupt_init()
{
    EXTI_Init(&(EXTI_InitTypeDef){
      .EXTI_Line = EXTI_Line0,
      .EXTI_Mode = EXTI_Mode_Interrupt,
      .EXTI_Trigger = EXTI_Trigger_Rising,
      .EXTI_LineCmd = ENABLE
    });
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource0);
    NVIC_Init(&(NVIC_InitTypeDef){
      .NVIC_IRQChannel = EXTI0_IRQn,
      .NVIC_IRQChannelPreemptionPriority = 0x00,
      .NVIC_IRQChannelSubPriority = 0x00,
      .NVIC_IRQChannelCmd = ENABLE
    });
}

void EXTI0_IRQHandler()
{
    //receiveRF();
    EXTI_ClearITPendingBit(EXTI_Line0);
}

#define PIN_RF GPIO_Pin_0

int main(void)
{
  	InitializeSystem();
  	SysTick_Config(SystemCoreClock / 1000000);
  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    EnableDebugOutput(DEBUG_ITM);

    InitializeRandom();

  	InitializeLEDs();
  	lcd_initLCD();
  	DelayMS(100);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    GPIO_Init(GPIOC, &(GPIO_InitTypeDef){
      .GPIO_Speed = GPIO_Speed_50MHz,
      .GPIO_Mode = GPIO_Mode_IN,
      .GPIO_OType = GPIO_OType_PP,
      .GPIO_PuPd = GPIO_PuPd_NOPULL,
      .GPIO_Pin = PIN_RF
    });
    /*interrupt_init();*/

    int last = 1;
    while(1)
    {
        if(!bitAvailible)
        {
            int c = GPIO_ReadInputDataBit(GPIOC, PIN_RF) == Bit_SET ? 1 : 0;
            if(last != c) 
            {
                receiveRF();
            }
            last = c;
            continue;
        }
        //while(!bitAvailible);
        int success = deserialize(&receivedBit);
        printf("sucess %d --> %d len %d\n", success, receivedBit, receivedBitlength);
        bitAvailible = 0;
    }
}