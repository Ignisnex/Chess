// Chess.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include "Board.h"
#include "Tile.h"
#include <iostream>


int getInput(Board *theBoard);
void selection(Board *theBoard);



int main()
{
	Board board;

	board.printBoard();

	while (getInput(&board) != -1) {
		system("cls");
		board.printBoard();
	}
    return 0;
}


int getInput(Board *theBoard) {
	char input = _getch();
	if ((int)input == -32) {//-32 = arrow keys
		input = _getch();
		switch ((int)input)	
		{					//72 = up, 80 = down, 75 = left, 77 = right
			case 72:
				theBoard->moveCursor(0, -1);
				break;
			case 80:
				theBoard->moveCursor(0, 1);
				break;
			case 75:
				theBoard->moveCursor(-1, 0);
				break;
			case 77:
				theBoard->moveCursor(1, 0);
				break;
			default:
				break;
		}
	}
	else if (input == 13) { // 13 = Enter
		selection(theBoard);
	}
	else if (input == 27) { // 27 = Esc
		int x, y;
		theBoard->getPreviousCursor(x, y);
		if (x == -1 || y == -1) {
			return -1;
		}
		else {
			theBoard->clearCursor();	
		}
	}
	else if (input == 114) { // 114 = r
		theBoard->clearBoard();
	}
	return 0;	
}

void selection(Board *theBoard) {
	int x, y = 0;
	int firstX, firstY = 0;
	theBoard->getCursorCoords(x, y);
	Tile *fromTile;
	Tile *toTile = theBoard->getPlayAreaTile(x, y);
	theBoard->getPreviousCursor(firstX, firstY);
	if (firstX == -1 || firstY == -1) {
		if (toTile->getPiece()->getPlayer() == theBoard->getPlayerTurn()) {
			theBoard->setPreviousCursor(x, y);
		}
	}
	else {
		fromTile = theBoard->getPlayAreaTile(firstX, firstY);
		theBoard->makeMove(fromTile, toTile);
		if (toTile->getPiece()->getType() == 'p') {
			if (theBoard->checkPawnPromotion(toTile)) {
				std::cout << "Promote Pawn to (Q)ueen, K(n)ight, (B)ishop, or (R)ook" << std::endl;
				char input = '1';
				while (input != 'q' && input != 'n' && input != 'b' && input != 'r') {
					input = _getch();
				}
				theBoard->promotePawn(toTile, input);
			}
			
		}
	}

}


