#define START_X 8
#define START_Y 8

#include <curses.h>
#include "./map.cpp"
#include "./Snake.cpp"

int main() {
	initscr();
	start_color();
	init_pair(1, COLOR_BLUE, COLOR_BLUE);
	init_pair(2, COLOR_WHITE, COLOR_WHITE);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_BLACK, COLOR_WHITE);
	init_pair(5, COLOR_YELLOW, COLOR_YELLOW);
	MAP snakeMap;
	snakeMap.draw_map();
	snakeMap.draw_title();


	Snake snake(3, RIGHT, MAP_SIZE / 2, MAP_SIZE / 2, 3, 10);

	while (true) {
		for (int i = 0; ; ++i) {
			if (snake.getPositionOf(i)[0] == -1 ||
				snake.getPositionOf(i)[1] == -1)
				break;

			move(START_X + snake.getPositionOf(i)[0],
				START_Y + snake.getPositionOf(i)[1]);
			attron(COLOR_PAIR(5));
			addch(ACS_BLOCK);
			addch(ACS_BLOCK);
			attron(COLOR_PAIR(5));
		}
		refresh();

		int keyboardInput = getch();
		switch (keyboardInput)
		{
		case KEY_UP:    snake.setDirectionTo(UP);    break;
		case KEY_DOWN:  snake.setDirectionTo(DOWN);  break;
		case KEY_RIGHT: snake.setDirectionTo(RIGHT); break;
		case KEY_LEFT:  snake.setDirectionTo(LEFT);  break;
		}
		snake.move();
	}


	endwin();
	return 0;
}