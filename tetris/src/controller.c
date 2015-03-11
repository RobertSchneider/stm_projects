#include "controller.h"
#include <System.h>
#include <Accelerometer.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define PIN_CLOCK	GPIO_Pin_0
#define PIN_LATCH	GPIO_Pin_1
#define PIN_DATA	GPIO_Pin_2

enum SNES_CTRL_STATE{ START_LATCH = 0, END_LATCH, SHIFT1, SHIFT1_END, SHIFT2, SHIFT2_END, SHIFT3, SHIFT3_END, SHIFT4, SHIFT4_END, SHIFT5, SHIFT5_END, SHIFT6, SHIFT6_END, SHIFT7, SHIFT7_END, SHIFT8, SHIFT8_END, SHIFT9, SHIFT9_END, SHIFT10, SHIFT10_END, SHIFT11, SHIFT11_END, SHIFT12, SHIFT12_END, SHIFT13, SHIFT13_END, SHIFT14, SHIFT14_END, SHIFT15, SHIFT15_END, SHIFT16, SHIFT16_END };

enum SNES_CTRL_STATE state_ctrl = START_LATCH;
int controller_now = 0;
int controller_save = 0;

void SetBit(GPIO_TypeDef *bus, int p, int t)
{
	uint16_t gpioData = GPIO_ReadOutputData(bus);
	if(!t) gpioData &= ~p;
	else gpioData |= p;
	GPIO_Write(bus, gpioData);
}

int controller_checkButton(int b)
{
	if(!(controller_save & b)) return 1;
	return 0;
}
const char* getButtonName(int b)
{
	if (b == SNES_B) return "B";
	if (b == SNES_Y) return "Y";
	if (b == SNES_SELECT) return "SELECT";
	if (b == SNES_START) return "START";
	if (b == SNES_UP) return "UP";
	if (b == SNES_DOWN) return "DOWN";
	if (b == SNES_LEFT) return "LEFT";
	if (b == SNES_RIGHT) return "RIGHT";
	if (b == SNES_A) return "A";
	if (b == SNES_X) return "X";
	if (b == SNES_L) return "L";
	if (b == SNES_R) return "R";
	return "NULL";
}

char* convert_bin(long i)
{
	int cnt = 0;
	long icopy = i;
	while (icopy != 0) 
	{
		cnt++;
		icopy >>= 1;
	}
	char *c = (char*)malloc(cnt+1);
	int ind = cnt-1;
	while (i != 0)
	{
		c[ind--] = i & 1 ? '1' : '0';
		i >>= 1;
	}
	c[cnt] = 0x00;
	return c;
}
int handleController()
{
	switch (state_ctrl)
	{
	case START_LATCH:
		SetBit(GPIOB, PIN_LATCH, 1);
		break;
	case END_LATCH:
		SetBit(GPIOB, PIN_LATCH, 0);
		break;
	case SHIFT1:
	case SHIFT2:
	case SHIFT3:
	case SHIFT4:
	case SHIFT5:
	case SHIFT6:
	case SHIFT7:
	case SHIFT8:
	case SHIFT9:
	case SHIFT10:
	case SHIFT11:
	case SHIFT12:
	case SHIFT13:
	case SHIFT14:
	case SHIFT15:
	case SHIFT16:
		SetBit(GPIOB, PIN_CLOCK, 0);
		break;
	case SHIFT1_END:
	case SHIFT2_END:
	case SHIFT3_END:
	case SHIFT4_END:
	case SHIFT5_END:
	case SHIFT6_END:
	case SHIFT7_END:
	case SHIFT8_END:
	case SHIFT9_END:
	case SHIFT10_END:
	case SHIFT11_END:
	case SHIFT12_END:
	{
			int dat = GPIO_ReadInputDataBit(GPIOB, PIN_DATA);
			controller_now = (controller_now << 1) | dat;
	}
	case SHIFT13_END:
	case SHIFT14_END:
	case SHIFT15_END:
	case SHIFT16_END:
		SetBit(GPIOB, PIN_CLOCK, 1);
		break;
	}
	if (state_ctrl == SHIFT16_END)	{
		state_ctrl = START_LATCH;
		controller_save = controller_now;

		return 1;
	}
	else
		state_ctrl = (enum SNES_CTRL_STATE)((int)state_ctrl + 1);
	return 0;
}

void controller_main()
{
	int controller = controller_save;
	controller_now = 0xFF;
	while(handleController() == 0);
}


void controller_init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_Init(GPIOB, &(GPIO_InitTypeDef){
		.GPIO_Speed = GPIO_Speed_50MHz,
		.GPIO_Mode = GPIO_Mode_OUT,
		.GPIO_OType = GPIO_OType_PP,
		.GPIO_PuPd = GPIO_PuPd_UP,
		.GPIO_Pin = PIN_CLOCK | PIN_LATCH
	});

	GPIO_Init(GPIOB, &(GPIO_InitTypeDef){
		.GPIO_Speed = GPIO_Speed_50MHz,
		.GPIO_Mode = GPIO_Mode_IN,
		.GPIO_OType = GPIO_OType_PP,
		.GPIO_PuPd = GPIO_PuPd_UP,
		.GPIO_Pin = PIN_DATA
	});
}