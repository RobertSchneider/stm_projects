#ifndef _GAME_H_
#define _GAME_H_

typedef	struct TTile
{
	int x;
	int y;
	int r;
	int pixel[4][4];
	int lenX, lenY;
	int color;
}TTile;

void game_draw();
void game_update();
void drawTile(TTile t, int clear);
void game_spawnTile();
void game_rotateTile();
void game_moveTile(int x, int y);

#endif