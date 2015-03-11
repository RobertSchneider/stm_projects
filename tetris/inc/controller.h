#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#define SNES_B		(1<<11)
#define SNES_Y 		(1<<10)
#define SNES_SELECT (1<<9)
#define SNES_START	(1<<8)
#define SNES_UP		(1<<7)
#define SNES_DOWN	(1<<6)
#define SNES_LEFT	(1<<5)
#define SNES_RIGHT	(1<<4)
#define SNES_A		(1<<3)
#define SNES_X		(1<<2)
#define SNES_L		(1<<1)
#define SNES_R		(1<<0)

void controller_init();
void controller_main();
int controller_checkButton(int b);

#endif