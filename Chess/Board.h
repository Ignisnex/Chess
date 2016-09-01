#ifndef _BOARD
#define _BOARD

#include "Tile.h"
#include "Piece.h"
#include "Cursor.h"
#include <vector>

class Board {

public:
	Board();
	~Board();
	void generateBoard();
	void printBoard();
	void moveCursor(int deltaX, int deltaY) { cursor.move(deltaX, deltaY); }
	void getCursorCoords(int &xCoord, int &yCoord) { cursor.getCursorCoords(xCoord, yCoord); } // Destructive Assignment
	int checkMove(Tile *firstTile, Tile *secondTile);
	Tile* getPlayAreaTile(const int xCoord, const int yCoord);
	int getPlayerTurn() { return playerTurn; }
	void changeTurn(){ if (playerTurn) { playerTurn = 0; } else { playerTurn++; } }
	void getPreviousCursor(int &xCoord, int &yCoord) { cursor.getPreviousCoords(xCoord, yCoord); } // Destructive Assignment
	void setPreviousCursor(int xCoord, int yCoord) { cursor.setPreviousCoords(xCoord, yCoord); }
	void clearBoard();
	void clearCursor() { cursor.clear(); }
	int pawnMoveRule(int firstX, int secondX, int firstY, int secondY);
	bool checkPawnPromotion(Tile *pawnTile);
	void promotePawn(Tile *pawnTile, char selection);
	int lineMoveRule(Piece *firstPiece, Piece *secondPiece, int AxisCoord, int fromCoord, int toCoord, bool isXAxis);
	int diagMoveRule(int firstX, int secondX, int firstY, int secondY);
	int kingMoveRule(int firstX, int secondX, int firstY, int secondY);
	bool rookCastleRule(Piece *firstPiece, Piece *secondPiece, int AxisCoord, int fromCoord, int toCoord, bool isXAxis, int increment);
	bool isInCheck(int player);
	bool isInCheckMate(int player);
	bool isCheckMateHelper(int player);
	void makeMove(Tile *fromTile, Tile *toTile);
	void makeCastleMove(int x, int y);
	void undoCastleMove(int x, int y);
	int knightMoveRule(int firstX, int secondX, int firstY, int secondY);
private:
	std::vector<Tile*> playArea;
	int playerTurn = 0; //0 white, 1 black
	bool isCurrentPlayerInCheck = false;
	Cursor cursor;
};

#endif