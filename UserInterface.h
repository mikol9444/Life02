#pragma once
#include "spritelib.h"

struct SpriteInfo {
public:
	CSprite sprite;
	bool isAlive;
};
class UserInterface
{
public:
	UserInterface();	// Standardkonstruktor
	~UserInterface();	// Standaraddestruktor

	static CString DebugMousePosition(CPoint point); //Show mouse position at top of the Window
	static CSprite* LoadSprite(CSprite* sprite,char* path, int sizeX, int sizeY); // Helper Function to load a Sprite with Exception included
	
};

class Board {
public:
	
	virtual void CreateBoard(SpriteInfo grid[36][36]); // Initialize a board 36x36
	virtual bool GetCellStatus(SpriteInfo* info); //Check if cell is alive
	virtual void ToggleCell(SpriteInfo* sprite); // toggle alive status 
	virtual void IterateArray(SpriteInfo grid[36][36], int N_grid[36][36]); //loop through array and call on each element CountNeighbours 
	virtual void CountNeighbours(SpriteInfo grid[36][36], int N_grid[36][36],int k,int l); // Count neighbours at selected position and save it to N_grid
	virtual void UpdateBoard(SpriteInfo grid[36][36], int N_grid[36][36]); // Update board by checking N_grid 
	virtual void ClearBoard(SpriteInfo grid[36][36]); //kill all cells 
private:
	 
};

