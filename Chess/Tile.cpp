#include "Tile.h"


Tile::Tile() {
	currentPiece = new Piece();
	// Default Constructor
}
Tile::Tile(int xCoord, int yCoord) {
	currentPiece = new Piece();
	x = xCoord;
	y = yCoord;
}
Tile::Tile(Piece* setPiece, int xCoord, int yCoord) {
	currentPiece = setPiece;
	x = xCoord;
	y = yCoord;
}
Tile::~Tile()
{
	//delete currentPiece;
}
void Tile::getCoords(int& xCoord, int& yCoord) {
	xCoord = x;
	yCoord = y;
}// Destructive function, overwrites parameters