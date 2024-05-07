#include <curses.h>
#include <iostream>
using namespace std;

#define MAP_SIZE 20

class MAP {
public:
	int map[MAP_SIZE][MAP_SIZE];

	MAP() {
		init_map();
	}

	void init_map() {
		for (int i = 0; i < MAP_SIZE; i++) {
			for (int j = 0; j < MAP_SIZE; j++) {
				if (i == 0 || i == MAP_SIZE - 1) map[i][j] = 1; //위-아래 벽일때 1
				else if (j == 0 || j == MAP_SIZE - 1) map[i][j] = 1; //양 옆 벽일때 1
				else map[i][j] = 0;
			}
		}
		//immune wall
		map[0][0] = 2;
		map[0][MAP_SIZE - 1] = 2;
		map[MAP_SIZE - 1][0] = 2;
		map[MAP_SIZE - 1][MAP_SIZE - 1] = 2;
	}


	void draw_map(int start_x = 8, int start_y = 8) {

		for (int i = 0; i < MAP_SIZE; i++) {
			for (int j = 0; j < MAP_SIZE; j++) {
				move(start_x + i, start_x + j * 2);
				if (map[i][j] == 1) {
					attron(COLOR_PAIR(1));
					addch(ACS_BLOCK);
					addch(ACS_BLOCK);
					attron(COLOR_PAIR(1));
				}
				else if (map[i][j] == 2) {
					attron(COLOR_PAIR(1)); //immune Wall이지만 일반 Wall과 같은 색상으로 임시 설정
					addch(ACS_BLOCK);
					addch(ACS_BLOCK);
					attron(COLOR_PAIR(1));
				}
				else {
					attron(COLOR_PAIR(2));
					addch(ACS_BLOCK);
					addch(ACS_BLOCK);
					attron(COLOR_PAIR(2));
				}
			}
		}
	}

	void draw_title() {

		int space = 20, space2 = 5;
		attron(COLOR_PAIR(3));

		// S
		mvprintw(1, 0 + space, " ■■■");
		mvprintw(2, 0 + space, "■    ");
		mvprintw(3, 0 + space, " ■■ ");
		mvprintw(4, 0 + space, "     ■");
		mvprintw(5, 0 + space, "■■■ ");

		// N
		mvprintw(1, 8 + space, "■    ■");
		mvprintw(2, 8 + space, "■■  ■");
		mvprintw(3, 8 + space, "■ ■ ■");
		mvprintw(4, 8 + space, "■  ■■");
		mvprintw(5, 8 + space, "■    ■");

		// A
		mvprintw(1, 17 + space, "   ■  ");
		mvprintw(2, 17 + space, " ■ ■ ");
		mvprintw(3, 17 + space, "■   ■");
		mvprintw(4, 17 + space, "■   ■");
		mvprintw(5, 17 + space, "■   ■");

		// K
		mvprintw(1, 25 + space, "■   ■");
		mvprintw(2, 25 + space, "■ ■  ");
		mvprintw(3, 25 + space, "■■   ");
		mvprintw(4, 25 + space, "■  ■ ");
		mvprintw(5, 25 + space, "■   ■");

		// E
		mvprintw(1, 33 + space, "■■■");
		mvprintw(2, 33 + space, "■    ");
		mvprintw(3, 33 + space, "■■■");
		mvprintw(4, 33 + space, "■    ");
		mvprintw(5, 33 + space, "■■■");

		attroff(COLOR_PAIR(3));

		attroff(COLOR_PAIR(4));

		// G
		mvprintw(1, 40 + space + space2, " ■■■");
		mvprintw(2, 40 + space + space2, "■    ");
		mvprintw(3, 40 + space + space2, "■  ■■");
		mvprintw(4, 40 + space + space2, "■    ■");
		mvprintw(5, 40 + space + space2, " ■■■");

		// A
		mvprintw(1, 49 + space + space2, "   ■  ");
		mvprintw(2, 49 + space + space2, " ■ ■ ");
		mvprintw(3, 49 + space + space2, "■   ■");
		mvprintw(4, 49 + space + space2, "■   ■");
		mvprintw(5, 49 + space + space2, "■   ■");

		// M
		mvprintw(1, 57 + space + space2, "■    ■");
		mvprintw(2, 57 + space + space2, "■■■■");
		mvprintw(3, 57 + space + space2, "■ ■ ■");
		mvprintw(4, 57 + space + space2, "■    ■");
		mvprintw(5, 57 + space + space2, "■    ■");

		// E
		mvprintw(1, 66 + space + space2, "■■■");
		mvprintw(2, 66 + space + space2, "■    ");
		mvprintw(3, 66 + space + space2, "■■■");
		mvprintw(4, 66 + space + space2, "■    ");
		mvprintw(5, 66 + space + space2, "■■■");

		attroff(COLOR_PAIR(4));
	}

};

int main() {

	initscr();
	start_color();
	init_pair(1, COLOR_BLUE, COLOR_BLUE);
	init_pair(2, COLOR_WHITE, COLOR_WHITE);

	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_BLACK, COLOR_WHITE);





	MAP snakeMap;
	snakeMap.draw_map();
	snakeMap.draw_title();


	refresh();
	getch();
	endwin();

	return 0;
}