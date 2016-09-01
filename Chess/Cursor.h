#ifndef _CURSOR
#define _CURSOR
#include "Tile.h"

/*******************************************
Cursor Class

Description:
Cursor is how the player or AI interact with
the game. It highlights a Tile on the board
and allows the user to select it. The selected
Tile has a pointer stored in the Cursor class
and uses that information to act upon a 
second Tile that the user selects.

The first Tile to be selected must not be 
without a piece.
********************************************/

class Cursor
{
public:
	Cursor();
	~Cursor();
	void move(int deltaX, int deltaY) { x += deltaX; y += deltaY; checkCoords(); }
	void getCursorCoords(int &xCoord, int &yCoord) { xCoord = x; yCoord = y; } // Destructive Assignment
	void getPreviousCoords(int &xCoord, int &yCoord) { xCoord = previousX; yCoord = previousY; } // Destructive Assignment
	void setPreviousCoords(int xCoord, int yCoord) { previousX = xCoord; previousY = yCoord; } 
	void checkCoords(); // Safety net to avoid OOB errors
	void clear() { previousX = -1; previousY = -1; }
private:
	int x;
	int y;
	int previousX;
	int previousY;
};

#endif