#include "stdafx.h"
#include "Cursor.h"


Cursor::Cursor()
{
	x = 0;
	y = 0;
	clear();
}


Cursor::~Cursor()
{
	
}

void Cursor::checkCoords() {
	if (x > 7) {
		x = 7;
	}
	if (x < 0) {
		x = 0;
	}
	if (y > 7) {
		y = 7;
	}
	if (y < 0) {
		y = 0;
	}
}

