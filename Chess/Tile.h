#ifndef _TILE
#define _TILE

#include "Piece.h"

class Tile {
public:
	Tile();
	Tile(int xCoord, int yCoord);
	Tile(Piece* setPiece, int xCoord, int yCoord);
	~Tile();
	void getCoords(int& xCoord, int& yCoord); // Destructive function, overwrites parameters
	void setCurrentPiece(Piece* setPiece) { currentPiece = setPiece; }
	void setCurrentPiece() { currentPiece->setPiece('z', 2); }
	void setCurrentPiece(char c, int i) { currentPiece->setPiece(c, i); }
	Piece* getPiece() { return currentPiece; }
private:
	Piece* currentPiece; // pointer to piece residing on tile
	int x; // coordinates on Board
	int y;

};

#endif