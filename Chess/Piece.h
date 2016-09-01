#ifndef _PIECE
#define _PIECE

class Piece {
	
public:
	Piece() { type = 'z'; player = 2; } // Default constructor. No owner or piece specified. Shouldn't be used
	~Piece() { type = 'z'; }
	Piece(char c, int i) { type = c; player = i; }
	char getType() { return type; }
	int getPlayer() { return player; }
	void setType(char c) { type = c; }
	void setPlayer(int i) { player = i; }
	void setPiece(char c, int i){ type = c; player = i; }

private:
	char type; // p = pawn, r = rook, n = knight, b = bishop, k = king, q = queen, z = none
	int player; //0 is white, 1 is black, anything else is empty

};

#endif
