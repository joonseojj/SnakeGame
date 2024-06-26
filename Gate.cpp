#include "Constants.h"
#include <cstdlib>

class Gate {
public:
	int x;
	int y;

	Gate(int x, int y) : x(x), y(y) {}
	Gate() : x(0), y(0) {}

	int getX() const {
		return x;
	}

	int getY() const {
		return y;
	}

	int checkWall() const {
		if (x == 0) return 0; // 위 벽
		else if (x == MAP_SIZE - 1) return 1; // 아래 벽
		else if (y == MAP_SIZE - 1) return 2; // 오른쪽 벽
		else if (y == 0) return 3; // 왼쪽 벽
		else return 4; //중앙 벽 
	}


};
