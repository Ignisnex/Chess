#include "Board.h"
#include <iostream>

/*******************************************
Board layout:
y(0,0)							(7,0)
|WR |WP |	|	|	|	|BP |BR |
|WN |WP |	|	|	|	|BP |BN |
|WB |WP |	|	|	|	|BP |BB |
|WQ |WP |	|	|	|	|BP |BQ |
|WK |WP |	|	|	|	|BP |BK |
|WB |WP |	|	|	|	|BP |BB |
|WN |WP |	|	|	|	|BP |BN |
|WR |WP |	|	|	|	|BP |BR |	
(0,7)---|---|---|---|---|---|---|x(7,7)
*********************************************/
Board::Board() { //Generate board and place pieces on Tiles
	generateBoard();
}

Board::~Board()
{
	
}

void Board::generateBoard() {

	for (int y = 0; y <= 7; y++) {
		for (int x = 0; x <= 7; x++) {
			Piece *piece = new Piece('z', 2);
			Tile *tile = new Tile(piece, x, y);
			playArea.push_back(tile);
		}
	}
	clearBoard();
}

void Board::printBoard() {
	using namespace std;
	int count = 0;
	int tmpx, tmpy, selX, selY = 0;
	bool isCursorCovering = false;
	char cursorInfo[2];
	char previousInfo[2];
	getCursorCoords(tmpx, tmpy);
	getPreviousCursor(selX, selY);
	for (int i = 0; i < playArea.size(); i++) {
		cout << "|";
		Piece *tmp = playArea[i]->getPiece();
		int tileTmpX, tileTmpY = 0;
		playArea[i]->getCoords(tileTmpX, tileTmpY);
		if ((tmpx == tileTmpX) && (tmpy == tileTmpY)) {
			isCursorCovering = true;
			cout << "__";
		}
		if (tmp->getPlayer() == 0) {
			if (isCursorCovering) {
				cursorInfo[0] = 'W';
			}
			else {
				cout << "W";
			}
			if (selX == tileTmpX && selY == tileTmpY) {
				previousInfo[0] = 'W';
			}
		}
		else if (tmp->getPlayer() == 1) {
			if (isCursorCovering) {
				cursorInfo[0] = 'B';
			}
			else {
				cout << "B";
			}
			if (selX == tileTmpX && selY == tileTmpY) {
				previousInfo[0] = 'B';
			}
		}
		else {
			if (!isCursorCovering) {
				cout << " ";
			}
			else {
				cursorInfo[0] = ' ';
			}
			
			
		}

		if (tmp->getType() != 'z') {
			if (isCursorCovering) {
				cursorInfo[1] = (char)((int)tmp->getType() - 32);
				
			}
			else {
				cout << (char)((int)tmp->getType() - 32);//returns char, cast to int, convert to upper, cast back to char
			}
			if (selX == tileTmpX && selY == tileTmpY) {
				previousInfo[1] = (char)((int)tmp->getType() - 32);
			}
		}
		else {
			if (!isCursorCovering) {
				cout << " ";
			}
			else {
				cursorInfo[1] = ' ';
			}
			
		}
		count++;
		if (count == 8) {
			cout << "|" << endl;
			count = 0;
		}
		isCursorCovering = false;
	}
	getPreviousCursor(selX, selY);
	if (selX != -1 || selY != -1) {
		cout << "Selected Tile: (" << selX << "," << selY << ") - " << previousInfo[0] << previousInfo[1] << endl;
	}
	cout << "Cursor : (" << tmpx << "," << tmpy << ") - " << cursorInfo[0] << cursorInfo[1] << endl;

	cout << "Turn: " << (playerTurn == 0 ? "WHITE" : "BLACK") << (isCurrentPlayerInCheck ? " (IS CURRENTLY IN CHECK)" : "") << endl;
	
}

Tile* Board::getPlayAreaTile(const int xCoord, const int yCoord) {
	if (yCoord > 7 || xCoord > 7 || yCoord < 0 || xCoord < 0) {
		return NULL;
	}
	else {
		return playArea[(yCoord * 8) + xCoord];
	}
}

int Board::checkMove(Tile *firstTile, Tile *secondTile) {
	Piece *firstPiece = firstTile->getPiece();
	Piece *secondPiece = secondTile->getPiece();
	int firstX, firstY, secondX, secondY = 0;
	if (firstPiece->getPlayer() == 2) {
		return -1;
	}
	firstTile->getCoords(firstX, firstY);
	secondTile->getCoords(secondX, secondY);
	switch (firstPiece->getType()) {
	case 'p':
		return pawnMoveRule(firstX, secondX, firstY, secondY);
		break;
	case 'r':
		if (firstX == secondX) {
			return lineMoveRule(firstPiece, secondPiece, firstX, firstY, secondY, true);
		}
		else if (firstY == secondY) {
			return lineMoveRule(firstPiece, secondPiece, firstY, firstX, secondX, false);
		}
		break;
	case 'n':
		return knightMoveRule(firstX, secondX, firstY, secondY);
		break;
	case 'b':
		return diagMoveRule(firstX, secondX, firstY, secondY);
		break;
	case 'q':
		int result;
		result = diagMoveRule(firstX, secondX, firstY, secondY);
		if (result == -1) {
			if (firstX == secondX) {
				result = lineMoveRule(firstPiece, secondPiece, firstX, firstY, secondY, true);
			}
			else if (firstY == secondY) {
				result = lineMoveRule(firstPiece, secondPiece, firstY, firstX, secondX, false);
			}
		}
		return result;
		break;
	case 'k':
		return kingMoveRule(firstX, secondX, firstY, secondY);
		break;
	default:
		break;
	}

	return -1; // move, unsuccessful

}

int Board::pawnMoveRule(int firstX, int secondX, int firstY, int secondY) {
	int deltaX = firstX - secondX;
	int deltaY = firstY - secondY;
	if (abs(deltaX) == 0) {
		return -1;
	}
	int incrementX = (deltaX / abs(deltaX)) * -1;
	if (getPlayAreaTile(firstX, firstY)->getPiece()->getPlayer() == 0 && deltaX > 0) {
		return -1;
	}
	if (getPlayAreaTile(firstX, firstY)->getPiece()->getPlayer() == 1 && deltaX < 0) {
		return -1;
	}
	if (abs(deltaY) == 1 && abs(deltaX) == 1) {
		if (getPlayAreaTile(secondX, secondY)->getPiece()->getPlayer() != getPlayAreaTile(firstX, firstY)->getPiece()->getPlayer()) {
			if (getPlayAreaTile(secondX, secondY)->getPiece()->getPlayer() == 2) {
				return -1;
			}
			return 0;
		}
	}
	else if (abs(deltaX) == 2 && deltaY == 0) {
		if (firstX != 1 && firstX != 6) {
			return -1;
		}
		else {
			if (getPlayAreaTile(firstX + incrementX, firstY)->getPiece()->getPlayer() == 2 &&
				getPlayAreaTile(firstX + incrementX + incrementX, firstY)->getPiece()->getPlayer() == 2) {
				return 1;
			}
		}
	}
	else if (abs(deltaX) == 1 && deltaY == 0) {
		if (getPlayAreaTile(secondX, secondY)->getPiece()->getPlayer() == 2) {
			return 1;
		}
	}
	return -1;
	
}
bool Board::checkPawnPromotion(Tile* pawnTile) {
	int x, y;
	pawnTile->getCoords(x, y);
	if (pawnTile->getPiece()->getPlayer() == 0) {
		if (x == 7) {
			return true;
		}
	}
	else if(pawnTile->getPiece()->getPlayer() == 1) {
		if (x == 0) {
			return true;
		}
	}
	
	return false;
}
void Board::promotePawn(Tile *pawnTile, char selection) {
	pawnTile->setCurrentPiece(selection, pawnTile->getPiece()->getPlayer());
}

int Board::lineMoveRule(Piece *firstPiece, Piece *secondPiece, int AxisCoord, int fromCoord, int toCoord, bool isXAxis) {

	if (fromCoord == toCoord) {
		return -1;
	}
	int delta = fromCoord - toCoord;
	int increment = (delta / abs(delta)) * -1;
	if (secondPiece->getPlayer() == firstPiece->getPlayer() && secondPiece->getType() == 'k' && firstPiece->getType() == 'r') {
		if (rookCastleRule(firstPiece, secondPiece, AxisCoord, fromCoord, toCoord, isXAxis, increment)) {
			makeCastleMove(AxisCoord, fromCoord);
			if (isInCheck(playerTurn)) {
				undoCastleMove(AxisCoord, fromCoord);
				return -1;
			}
			return 1;
		}
	}
	for (int i = fromCoord + increment; i != toCoord; i += increment) {
		if (isXAxis) {
			if (getPlayAreaTile(AxisCoord, i)->getPiece()->getPlayer() != 2) {
				return -1;
			}
		}
		else {
			if (getPlayAreaTile(i, AxisCoord)->getPiece()->getPlayer() != 2) {
				return -1;
			}
		}
	}
	if (secondPiece->getPlayer() == 2) {
		return 1;
	}
	else if (secondPiece->getPlayer() != firstPiece->getPlayer()) {
		return 0;
	}
	return -1;
}

int Board::diagMoveRule(int firstX, int secondX, int firstY, int secondY) {
	int deltaX = firstX - secondX;
	int deltaY = firstY - secondY;
	if (deltaX == 0 || deltaY == 0) {
		return -1;
	}
	int incrementX = (deltaX / abs(deltaX)) * -1;
	int incrementY = (deltaY / abs(deltaY)) * -1;
	if (abs(deltaX) != abs(deltaY)) {
		return -1;
	}
	int y = firstY + incrementY;
	for (int x = firstX + incrementX; x != secondX; x += incrementX) {
		if (getPlayAreaTile(x, y)->getPiece()->getPlayer() != 2) {
			return -1;
		}
		y += incrementY;
	}
	if (getPlayAreaTile(secondX, secondY)->getPiece()->getPlayer() == 2) {
		return 1;
	}
	else if (getPlayAreaTile(secondX, secondY)->getPiece()->getPlayer() != getPlayAreaTile(firstX, firstY)->getPiece()->getPlayer()) {
		return 0;
	}
	return -1;
}

int Board::kingMoveRule(int firstX, int secondX, int firstY, int secondY) {
	int deltaX = firstX - secondX;
	int deltaY = firstY - secondY;
	if (abs(deltaX) > 1 || abs(deltaY) > 1) {
		return -1;
	}
	if (abs(deltaX) != 1 && abs(deltaY) != 1) {
		return -1;
	}
	if (getPlayAreaTile(secondX, secondY)->getPiece()->getPlayer() == 2) {
		return 1;
	}
	else if (getPlayAreaTile(secondX, secondY)->getPiece()->getPlayer() != getPlayAreaTile(firstX, firstY)->getPiece()->getPlayer()) {
		return 0;
	}

	return -1;
}

bool Board::rookCastleRule(Piece *firstPiece, Piece *secondPiece, int AxisCoord, int fromCoord, int toCoord, bool isXAxis, int increment) {
	if (!isXAxis) {
		return false;
	}
	if (firstPiece->getPlayer() == 0 && AxisCoord != 0) {
		return false;
	}
	if (firstPiece->getPlayer() == 1 && AxisCoord != 7) {
		return false;
	}
	if ((fromCoord != 0 && fromCoord != 7) || toCoord != 4) {
		return false;
	}

	for (int i = fromCoord + increment; i != toCoord; i += increment) {
		if (getPlayAreaTile(AxisCoord, i)->getPiece()->getPlayer() != 2) {
			return false;
		}
	}
	return true;
}
void Board::makeCastleMove(int x, int y) {
	if (x == 0) {
		if (y == 0) {
			playArea[24]->getPiece()->setPiece('r', 0);
			playArea[16]->getPiece()->setPiece('k', 0);
			playArea[32]->getPiece()->setPiece('z', 2);
			playArea[0]->getPiece()->setPiece('z', 2);
		}
		else if(y == 7){
			playArea[40]->getPiece()->setPiece('r', 0);
			playArea[48]->getPiece()->setPiece('k', 0);
			playArea[32]->getPiece()->setPiece('z', 2);
			playArea[56]->getPiece()->setPiece('z', 2);
		}
	}
	if (x == 7) {
		if (y == 0) {
			playArea[31]->getPiece()->setPiece('r', 1);
			playArea[23]->getPiece()->setPiece('k', 1);
			playArea[39]->getPiece()->setPiece('z', 2);
			playArea[7]->getPiece()->setPiece('z', 2);
		}
		else if (y == 7) {
			playArea[47]->getPiece()->setPiece('r', 1);
			playArea[55]->getPiece()->setPiece('k', 1);
			playArea[39]->getPiece()->setPiece('z', 2);
			playArea[63]->getPiece()->setPiece('z', 2);
		}
	}
}
void Board::undoCastleMove(int x, int y) {
	if (x == 0) {
		if (y == 0) {
			playArea[24]->getPiece()->setPiece('z', 2);
			playArea[16]->getPiece()->setPiece('z', 2);
			playArea[32]->getPiece()->setPiece('k', 0);
			playArea[0]->getPiece()->setPiece('r', 0);
		}
		else if (y == 7) {
			playArea[40]->getPiece()->setPiece('z', 2);
			playArea[48]->getPiece()->setPiece('z', 2);
			playArea[32]->getPiece()->setPiece('k', 0);
			playArea[56]->getPiece()->setPiece('r', 0);
		}
	}
	if (x == 7) {
		if (y == 0) {
			playArea[31]->getPiece()->setPiece('z', 2);
			playArea[23]->getPiece()->setPiece('z', 2);
			playArea[39]->getPiece()->setPiece('k', 1);
			playArea[7]->getPiece()->setPiece('r', 1);
		}
		else if (y == 7) {
			playArea[47]->getPiece()->setPiece('z', 2);
			playArea[55]->getPiece()->setPiece('z', 2);
			playArea[39]->getPiece()->setPiece('k', 1);
			playArea[63]->getPiece()->setPiece('r', 1);
		}
	}
}

int Board::knightMoveRule(int firstX, int secondX, int firstY, int secondY) {
	int deltaX = abs(firstX - secondX);
	int deltaY = abs(firstY - secondY);

	if (deltaX != 1 && deltaX != 2) {
		return -1;
	}
	else if (deltaY != 1 && deltaY != 2) {
		return -1;
	}
	else if (deltaY == deltaX) {
		return -1;
	}
	if (getPlayAreaTile(secondX, secondY)->getPiece()->getPlayer() == 2) {
		return 1;
	}
	else if (getPlayAreaTile(secondX, secondY)->getPiece()->getPlayer() != getPlayAreaTile(firstX, firstY)->getPiece()->getPlayer()) {
		return 0;
	}
		
	return -1;
}

bool Board::isInCheck(int player) {
	Tile *kingTile = playArea[24];
	int result;
	for (int i = 0; i < playArea.size();i++) {
		if (playArea[i]->getPiece()->getType() == 'k' && playArea[i]->getPiece()->getPlayer() == player) {
			kingTile = playArea[i];
		}
	}
	for (int i = 0; i < playArea.size();i++) {
		if (playArea[i]->getPiece()->getPlayer() != player) {
			result = checkMove(playArea[i], kingTile);
			if (result == 0) {
				return true;
			}
		}
	}
	return false;
}

bool Board::isInCheckMate(int player) {
	Tile *kingTile = playArea[36];

	char tmpToType;
	int tmpToPlayer;
	for (int i = 0; i < playArea.size(); i++) {
		if (playArea[i]->getPiece()->getPlayer() == player && playArea[i]->getPiece()->getType() == 'k') {
			kingTile = playArea[i];
		}
	}
	for (int i = 0; i < playArea.size(); i++) {
		int checkedMove = -1;
		checkedMove = checkMove(kingTile, playArea[i]);
			
		if (checkedMove == 1 || checkedMove == 0) {
			tmpToType = playArea[i]->getPiece()->getType();
			tmpToPlayer = playArea[i]->getPiece()->getPlayer();
			playArea[i]->setCurrentPiece(kingTile->getPiece()->getType(), kingTile->getPiece()->getPlayer());
			kingTile->setCurrentPiece();
			if (isInCheck(playerTurn)) {
				kingTile->setCurrentPiece(playArea[i]->getPiece()->getType(), playArea[i]->getPiece()->getPlayer());
				playArea[i]->setCurrentPiece(tmpToType, tmpToPlayer);
			}
			else {
				kingTile->setCurrentPiece(playArea[i]->getPiece()->getType(), playArea[i]->getPiece()->getPlayer());
				playArea[i]->setCurrentPiece(tmpToType, tmpToPlayer);
				return false;
			}
			
		}
	}
	return isCheckMateHelper(player);
	
}

bool Board::isCheckMateHelper(int player) {
	char tmpToType;
	int tmpToPlayer;
	int checkedMove = -1;
	for (int i = 0; i < playArea.size(); i++) {
		if (playArea[i]->getPiece()->getPlayer() == player) {
			for (int j = 0; j < playArea.size(); j++) {
				if (playArea[i]->getPiece()->getType() == 'r' && playArea[j]->getPiece()->getType() == 'k') {
					if (checkMove(playArea[i], playArea[j]) == 1) {
						int x, y;
						playArea[i]->getCoords(x, y);
						undoCastleMove(x, y);
						return false;
					}
				}
				checkedMove = checkMove(playArea[i], playArea[j]);
				if (checkedMove == 1 || checkedMove == 0) {
					tmpToType = playArea[j]->getPiece()->getType();
					tmpToPlayer = playArea[j]->getPiece()->getPlayer();
					playArea[j]->setCurrentPiece(playArea[i]->getPiece()->getType(), playArea[i]->getPiece()->getPlayer());
					playArea[i]->setCurrentPiece();
					if (isInCheck(playerTurn)) {
						playArea[i]->setCurrentPiece(playArea[j]->getPiece()->getType(), playArea[j]->getPiece()->getPlayer());
						playArea[j]->setCurrentPiece(tmpToType, tmpToPlayer);
					}
					else {
						playArea[i]->setCurrentPiece(playArea[j]->getPiece()->getType(), playArea[j]->getPiece()->getPlayer());
						playArea[j]->setCurrentPiece(tmpToType, tmpToPlayer);
						return false;
					}

				}
			}
		}
	}
	return true;
}

void Board::makeMove(Tile *fromTile, Tile *toTile) {
	char toTileTmpType = toTile->getPiece()->getType();
	int toTileTmpPlayer = toTile->getPiece()->getPlayer();

	int moveChecked = checkMove(fromTile, toTile);

	if (moveChecked == 1 || moveChecked == 0) {
		toTile->setCurrentPiece(fromTile->getPiece()->getType(), fromTile->getPiece()->getPlayer());
		fromTile->setCurrentPiece();
		if (isInCheck(playerTurn)) {
			fromTile->setCurrentPiece(toTile->getPiece()->getType(), toTile->getPiece()->getPlayer());
			toTile->setCurrentPiece(toTileTmpType, toTileTmpPlayer);
		}
		else {
			changeTurn();
			clearCursor();
		}
		isCurrentPlayerInCheck = isInCheck(playerTurn);
		if (isCurrentPlayerInCheck) {
			if (isInCheckMate(playerTurn)) {
				clearBoard();
			}
		}
		if (toTileTmpType == 'k' && moveChecked == 0) {
			clearBoard();
		}
	}
}

void Board::clearBoard(){
	for (int i = 0; i < playArea.size(); i++) {
		if (i%8 == 1) {
			playArea[i]->getPiece()->setPiece('p', 0); //White Pawn
		}
		else if (i%8 == 6) {
			playArea[i]->getPiece()->setPiece('p', 1); // Black Pawn
		}
		else if (i%8 == 0) { // White Placement
			if (i == 0 || i == 56) {
				playArea[i]->getPiece()->setPiece('r', 0); // Rook
			}
			else if (i == 8 || i == 48) {
				playArea[i]->getPiece()->setPiece('n', 0); // Knight
			}
			else if (i == 16 || i == 40) {
				playArea[i]->getPiece()->setPiece('b', 0); // Bishop
			}
			else if (i == 32) {
				playArea[i]->getPiece()->setPiece('k', 0); // King
			}
			else if (i == 24) {
				playArea[i]->getPiece()->setPiece('q', 0); // Queen
			}
		}
		else if (i % 8 == 7) { // Black Placement
			if (i == 7 || i == 63) {
				playArea[i]->getPiece()->setPiece('r', 1); // Rook
			}
			else if (i == 15 || i == 55) {
				playArea[i]->getPiece()->setPiece('n', 1); // Knight
			}
			else if (i == 23 || i == 47) {
				playArea[i]->getPiece()->setPiece('b', 1); // Bishop
			}
			else if (i == 39) {
				playArea[i]->getPiece()->setPiece('k', 1); // King
			}
			else if (i == 31) {
				playArea[i]->getPiece()->setPiece('q', 1); // Queen
			}
		}
		else {
			playArea[i]->getPiece()->setPiece('z', 2); // Blank
		}
	}
	playerTurn = 0;
	isCurrentPlayerInCheck = false;
	cursor.clear();
}