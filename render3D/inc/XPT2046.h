/* -----------------------------------------------------------------------------

XPT2046 based touchscreen driver

Copyright (C) 2013  Fabio Angeletti - fabio.angeletti89@gmail.com

Part of this code is an adaptation from souce code provided by
		WaveShare - http://www.waveshare.net

I'm not the owner of the whole code

------------------------------------------------------------------------------*/

#ifndef _TOUCHPANEL_H_
#define _TOUCHPANEL_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "SSD1289.h"
#include <System.h>

/* Private typedef -----------------------------------------------------------*/
typedef	struct POINT 
{
   uint16_t x;
   uint16_t y;
}Coordinate;


typedef struct Matrix 
{						
long double An,  
            Bn,     
            Cn,   
            Dn,    
            En,    
            Fn,     
            Divider ;
} Matrix ;

/* Private variables ---------------------------------------------------------*/
extern Coordinate ScreenSample[3];
extern Coordinate DisplaySample[3];
extern Matrix matrix ;
extern Coordinate  display ;

/* Private define ------------------------------------------------------------*/

#define	CHX 	0x90
#define	CHY 	0xD0

#define XPT2046_PORT		     GPIOC
#define XPT2046_MISO_PIN	  GPIO_Pin_11
#define XPT2046_MOSI_PIN	  GPIO_Pin_12
#define XPT2046_SCK_PIN		  GPIO_Pin_10
#define XPT2046_CS_PIN		  GPIO_Pin_9
#define XPT2046_IRQ_PIN		  GPIO_Pin_8

/* Private function prototypes -----------------------------------------------*/				
	// controller initialization
void XPT2046_GetAdXY(int *x,int *y) ;
void XPT2046_Init(void);
	// calibration routine	
void TouchPanel_Calibrate(void);
	// returns if a pressure is present
uint16_t XPT2046_Press(void);
	// returns coordinates of the pressure
Coordinate *Read_XPT2046(void);

	// function to calibrate touchscreen and use calibration matrix
FunctionalState setCalibrationMatrix(Coordinate * displayPtr,Coordinate * screenPtr,Matrix * matrixPtr);
FunctionalState getDisplayPoint(Coordinate * displayPtr, Coordinate * screenPtr);

	// controller specific functions - should not be used outside
static void set_CS(void);
static void reset_CS(void);
static uint16_t read_IRQ(void);
static void WR_CMD (uint16_t cmd);
static uint16_t RD_AD(void);

#endif
