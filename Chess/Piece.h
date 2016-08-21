#ifndef _PIECE
#define _PIECE

class Piece {
	Piece();
	Piece(char c, int i) { type = c; player = i; }
public:
	char getType() { return type; }
	int getPlayer() { return player; }
	void setType(char c) { type = c; }
	void setPlayer(int i) { player = i; }
private:
	char type; // p = pawn, r = rook, n = knight, b = bishop, k = king, q = queen
	int player; //0 is white

};

#endif
