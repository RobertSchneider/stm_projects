#include "SSD1289.h"
#include "game.h"
#include "Random.h"

TTile ttileI = {0, 0, 0, {{1.0},{1.0},{1.0},{1.0}}, 4, 1, RGB565CONVERT(0, 255, 255)};
TTile ttileL = {0, 0, 0, {{0,1},{0,1},{1,1}}, 3, 2, RGB565CONVERT(0, 0, 255)};
TTile ttileJ = {0, 0, 0, {{1,1},{0,1},{0,1}}, 3, 2, RGB565CONVERT(255, 100, 0)};
TTile ttileZ = {0, 0, 0, {{1,0},{1,1},{0,1}}, 3, 2, RGB565CONVERT(255, 0, 0)};
TTile ttileS = {0, 0, 0, {{0,1},{1,1},{1,0}}, 3, 2, RGB565CONVERT(0, 255, 0)};
TTile ttileO = {0, 0, 0, {{1,1},{1,1}}, 2, 2, RGB565CONVERT(255, 255, 0)};
TTile ttileT = {0, 0, 0, {{0,1},{1,1},{0,1}}, 3, 2, RGB565CONVERT(255, 0, 255)};	

int tileFallDelay = 0;
TTile actualTile = {};

#define BLOCK_RED Red
#define BLOCK_GREEN Green
#define BLOCK_BLUE Blue
#define BLOCK_YELLOW Yellow

#define cubeW 10

int grid[10][20];

void drawCube(int x, int y, int color)
{
	SSD1289_FillRect(MAX_X/2-5*cubeW+x*cubeW+1, MAX_Y/2-10*cubeW+y*cubeW+1, cubeW-2, cubeW-2, color);
}
void setTile(TTile t)
{
	actualTile.x = 5;
	actualTile.y = 1;
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			actualTile.pixel[x][y] = t.pixel[x][y];
		}
	}
	actualTile.color = t.color;
	actualTile.lenX = t.lenX;
	actualTile.lenY = t.lenY;
	actualTile.r = 0;
}
void game_spawnTile()
{
	int i = GetRandomInteger() % 7;
	if(i == 0) setTile(ttileI);
	if(i == 1) setTile(ttileL);
	if(i == 2) setTile(ttileJ);
	if(i == 3) setTile(ttileZ);
	if(i == 4) setTile(ttileS);
	if(i == 5) setTile(ttileO);
	if(i == 6) setTile(ttileT);
}
void drawTile(TTile t, int solid)
{
	int color = solid ? t.color : 0;
	for (int x = 0; x < t.lenX; ++x)
	{
		for (int y = 0; y < t.lenY; ++y)
		{
			switch(t.r)
			{
				case 0: if(t.pixel[x][y]) drawCube(x+t.x, y+t.y, color); break;
				case 1: if(t.pixel[x][y]) drawCube(t.lenY-1-y+t.x, x+t.y, color); break;
				case 2: if(t.pixel[x][y]) drawCube(t.lenX-1-x+t.x, t.lenY-1-y+t.y, color); break;
				case 3: if(t.pixel[x][y]) drawCube(y+t.x, t.lenX-1-x+t.y, color); break;
			}
		}
	}
}
void game_draw()
{
	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			int color = grid[x][y];
			if(color) drawCube(x, y, color);
		}
	}
	SSD1289_DrawRect(MAX_X/2-5*cubeW-1, MAX_Y/2-10*cubeW-1, 10*cubeW+2, 20*cubeW+2, Red);
}
int collide_P(int x, int y)
{
	if(grid[x][y] || x < 0 || y < 0 || x >= 10 || y >= 20) return 1;
	return 0;
}
int collide(TTile t)
{
	for (int x = 0; x < t.lenX; x++)
	{
		for (int y = 0; y < t.lenY; y++)
		{
			switch(t.r)
			{
				case 0: if(t.pixel[x][y] > 0 && collide_P(x+t.x,y+t.y)) return 1; break;
				case 1: if(t.pixel[x][y] > 0 && collide_P(t.lenY-1-y+t.x,x+t.y)) return 1; break;
				case 2: if(t.pixel[x][y] > 0 && collide_P(t.lenX-1-x+t.x,t.lenY-1-y+t.y)) return 1; break;
				case 3: if(t.pixel[x][y] > 0 && collide_P(y+t.x,t.lenX-1-x+t.y)) return 1; break;
			}
		}
	}
	return 0;
}
void addTile(TTile t)
{
	for (int x = 0; x < t.lenX; ++x)
	{
		for (int y = 0; y < t.lenY; ++y)
		{
			switch(t.r)
			{
				case 0: if(t.pixel[x][y]) grid[x+t.x][y+t.y] = t.color; break;
				case 1: if(t.pixel[x][y]) grid[t.lenY-1-y+t.x][x+t.y] = t.color; break;
				case 2: if(t.pixel[x][y]) grid[t.lenX-1-x+t.x][t.lenY-1-y+t.y] = t.color; break;
				case 3: if(t.pixel[x][y]) grid[y+t.x][t.lenX-1-x+t.y] = t.color; break;
			}
		}
	}
}
void game_rotateTile()
{
	drawTile(actualTile, 0);
	actualTile.r = (actualTile.r+1)%4;
	drawTile(actualTile, 1);
}
void game_moveTile(int x, int y)
{
	drawTile(actualTile, 0);
	actualTile.x += x;
	actualTile.y += y;
	if(collide(actualTile))
	{
		actualTile.y-=1;
		addTile(actualTile);
		game_spawnTile();
	}
	drawTile(actualTile, 1);
	game_draw();
}
void game_update()
{
	if(tileFallDelay > 0) tileFallDelay--;
	else 
	{
		drawTile(actualTile, 0);
		actualTile.y+=1;

		if(collide(actualTile))
		{
			actualTile.y-=1;
			addTile(actualTile);
			game_spawnTile();
		}

		drawTile(actualTile, 1);
		tileFallDelay = 10;
		game_draw();
	}
}