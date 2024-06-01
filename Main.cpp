#include "Constants.h"
#include "Snake.cpp"
#include "Map.cpp"
#include "Item.cpp"
#include "Gate.cpp"

#include <windows.h>
#include <string>
#include <curses.h>
#include <fstream>
#include <chrono>

using namespace std;
// 최종적으로 화면에 출력할 내용을 배열로 구현
int screen[MAP_SIZE][MAP_SIZE];

void printb(const int row, const int column, const int color_pair, WINDOW* win) {
	wmove(win, row, column * 2);
	wattron(win, COLOR_PAIR(color_pair));
	waddch(win, ACS_BLOCK);
	waddch(win, ACS_BLOCK);

}

// 맵 파일에서 맵 불러오기
Map map;
Gate getRandomGate1() {
	int x, y;
	do {
		x = rand() % MAP_SIZE;
		y = rand() % MAP_SIZE;
	} while (map.map[x][y] != 1);

	return Gate(x, y);
}

Gate getRandomGate2(int avoidX, int avoidY) {
	int x, y;
	do {
		x = rand() % MAP_SIZE;
		y = rand() % MAP_SIZE;
	} while ((x == avoidX && y == avoidY) || map.map[x][y] != 1);

	return Gate(x, y);
}

void Item_respawn(Item& item) {
	int new_x, new_y;
	while(1) {
		new_x = std::rand() % MAP_SIZE;
		new_y = std::rand() % MAP_SIZE;
		if(screen[new_x][new_y] == 0) break;
	} 
	item.x = new_x;
	item.y = new_y;
	item.Item_type = std::rand() % 2;
	item.active = true;
}

bool can_move(int m) {
	if (m == 0 || m == 5 || m == 6) return true;
	else return false;
}

void setConsoleSize(int width, int height) {

	string command = "mode con: cols=" + std::to_string(width) + " lines=" + std::to_string(height);
	system(command.c_str());
}

int main() {
	initscr();

	/*
	mvaddch(0, 1, ACS_BLOCK);
	mvaddch(0, 2, ACS_BLOCK);
	mvaddch(0, 3, ACS_BLOCK);
	mvaddch(0, 4, ACS_BLOCK);
	mvaddch(1, 0, ACS_BLOCK);
	mvaddch(1, 1, ACS_BLOCK);
	mvaddch(2, 1, ACS_BLOCK);
	mvaddch(2, 2, ACS_BLOCK);
	mvaddch(2, 3, ACS_BLOCK);
	mvaddch(2, 4, ACS_BLOCK);
	mvaddch(3, 4, ACS_BLOCK);
	mvaddch(3, 5, ACS_BLOCK);
	mvaddch(4, 1, ACS_BLOCK);
	mvaddch(4, 2, ACS_BLOCK);
	mvaddch(4, 3, ACS_BLOCK);
	mvaddch(4, 4, ACS_BLOCK);
	refresh();
	getch();
	*/

	start_color();

	init_color(98, 1000, 0, 0);			// red
	init_color(99, 500, 500, 500);		// gray
	init_color(100, 1000, 500, 0);		// orange
	init_color(101, 1000, 1000, 0);		// yellow
	init_color(102, 800, 0, 800);		// purple
	init_color(103, 0, 1000, 0);		// green
	init_color(104, 1000, 1000, 1000);	// white
	init_color(105, 0, 0, 0);			// black

	init_pair(1, 105, 105);
	init_pair(2, 104, 104);
	init_pair(3, 99, 99);
	init_pair(4, 101, 101);
	init_pair(5, 100, 100);
	init_pair(6, 103, 103);
	init_pair(7, 98, 98);
	init_pair(8, 102, 102);
	init_pair(9, 105, 104);
	// 필요하면 컬러페어 수정하거나 추가해주세요!
	curs_set(0);
	int itemNum;
	int itemTick;
	int snakeTick;
	int goal;

	bool gateActive;
	bool victory;
	int tick;
	int keyboardInput;

	// 게임 난이도 변수
	// 추후 플레이어가 메인화면에서 바꿀 수 있도록 할 예정
	goal = 15;
	snakeTick = 5;
	itemTick = 80; 
	itemNum = 2;




	int stdscr_height = getmaxy(stdscr);
	int stdscr_width = getmaxx(stdscr);
	int title_height = 10;
	int title_width = 20;
	int menu_height = 5;
	int menu_width = 24;
	int gap = 2;
	int title_x = (stdscr_width - title_width) / 2;
	int title_y = (stdscr_height - (title_height + menu_height + gap)) / 2;
	int menu_x = (stdscr_width - menu_width) / 2;
	int menu_y = title_y + title_height + gap;

	WINDOW* title = newwin(title_height, title_width, title_y, title_x);
	WINDOW* menu = newwin(menu_height, menu_width, menu_y, menu_x);

	box(title, '|', '-');
	printw("SELECT using ARROW KEYS.\nCONFIRM using SPACEBAR.");
	mvwprintw(title, 0, 5, "TITLE HERE");
	mvwprintw(menu, 0, 0, "Snake Speed");
	mvwprintw(menu, 1, 0, "Item Respawn Rate");
	mvwprintw(menu, 2, 0, "Item Number");

	cbreak();
	keypad(stdscr, TRUE);
	noecho();
	nodelay(stdscr, TRUE);

	refresh();
	wrefresh(title);
	wrefresh(menu);

	bool end = false;
	int menu_cursor = 0;
	while (true)
	{
		if (menu_cursor == 0) wattron(menu, COLOR_PAIR(9));
		mvwprintw(menu, 0, menu_width - 2, "%2d", snakeTick);
		wattroff(menu, COLOR_PAIR(9));
		if (menu_cursor == 1) wattron(menu, COLOR_PAIR(9));
		mvwprintw(menu, 1, menu_width - 2, "%2d", itemTick);
		wattroff(menu, COLOR_PAIR(9));
		if (menu_cursor == 2) wattron(menu, COLOR_PAIR(9));
		mvwprintw(menu, 2, menu_width - 2, "%2d", itemNum);
		wattroff(menu, COLOR_PAIR(9));
		if (menu_cursor == 3) wattron(menu, COLOR_PAIR(9));
		mvwprintw(menu, 4, (menu_width - 2) / 2, "OK");
		wattroff(menu, COLOR_PAIR(9));

		keyboardInput = getch();
		switch (keyboardInput)
		{
		case KEY_DOWN:
			if (menu_cursor < 3)
				++menu_cursor;
			break;

		case KEY_UP:
			if (menu_cursor > 0)
				--menu_cursor;
			break;

		case KEY_RIGHT:
			switch (menu_cursor)
			{
			case 0:
				++snakeTick;
				break;

			case 1:
				++itemTick;
				break;

			case 2:
				if (itemNum < 3)
					++itemNum;
				break;
			}
			break;

		case KEY_LEFT:
			switch (menu_cursor)
			{
			case 0:
				if (snakeTick > 1)
					--snakeTick;
				break;
			case 1:
				if (itemTick > 1)
					--itemTick;
				break;
			case 2:
				if (itemNum > 1)
					--itemNum;
				break;
			}
			break;

		case ' ':
			if (menu_cursor == 3)
				end = true;
			break;
		}

		if (end) break;

		refresh();
		wrefresh(menu);
	}

	delwin(title);
	delwin(menu);
	clear();


	int game_width = MAP_SIZE * 2;
	int game_height = MAP_SIZE;
	int score_width = 8;
	int score_height = 8;
	int game_x = (stdscr_width - MAP_SIZE * 2) / 2;
	int game_y = (stdscr_height - MAP_SIZE) / 2;
	int score_x = game_x + game_width + gap;
	int score_y = game_y;
	WINDOW* game = newwin(game_height, game_width, game_y, game_x);
	WINDOW* score = newwin(score_height, score_width, score_y, score_x);



	// 스테이지 클리어하면 stage 변수가 1씩 증가
	// victory가 true인 상태로 반복문 탈출하면 게임 최종 승리, 아니면 실패
	for (int stage = 1; ; ++stage) {
		// 더 불러올 맵이 없으면 승리 처리 및 반복문 탈출
		std::ifstream fin("map" + to_string(stage) + ".txt");
		if (!fin) {
			victory = true;
			break;
		}

		

		for (int row = 0; row < MAP_SIZE; ++row)
			for (int column = 0; column < MAP_SIZE; ++column)
				fin >> map.map[row][column];

		// 길이 3칸, 왼쪽, 맵 중앙으로 지렁이 초기화
		Snake snake(3, LEFT, MAP_SIZE / 2, MAP_SIZE / 2);

		//items 객체 생성 및 초기화 
		Item items[3];
		for (int i = 0; i < itemNum; i++) {
			Item_respawn(items[i]);
		}

		Gate gates[2];

		srand(time(nullptr));

		Gate randomGate1 = getRandomGate1();
		int x1 = randomGate1.getX();
		int y1 = randomGate1.getY();
		gates[0] = Gate(x1, y1);

		Gate randomGate2 = getRandomGate2(x1, y1);
		int x2 = randomGate2.getX();
		int y2 = randomGate2.getY();
		gates[1] = Gate(x2, y2);



		gateActive = false;
		victory = false;
		tick = 0;

		// victory가 true인 상태로 반복문 탈출하면 스테이지 성공, 아니면 실패
		while (true) {
			// 목표 길이 도달하면 스테이지 클리어
			if (snake.getLength() >= goal) {
				victory = true;
				break;
			}

			// 최소 길이보다 짧아지면 실패
			if (snake.getLength() < MIN_LENGTH)
				break;


			// 지렁이가 움직일 때
			if (tick % snakeTick == 0) {
				keyboardInput = getch();

				// 관리자용
				if (keyboardInput == 'q')
				{
					victory = true;
					break;
				}


				// 반대방향 입력하면 실패
				if ((keyboardInput == KEY_UP && snake.getDirection() == DOWN) ||
					(keyboardInput == KEY_DOWN && snake.getDirection() == UP) ||
					(keyboardInput == KEY_RIGHT && snake.getDirection() == LEFT) ||
					(keyboardInput == KEY_LEFT && snake.getDirection() == RIGHT))
					break;

				// 이동
				switch (keyboardInput) {
				case KEY_UP:    snake.setDirectionTo(UP);  break;
				case KEY_DOWN:  snake.setDirectionTo(DOWN); break;
				case KEY_RIGHT: snake.setDirectionTo(RIGHT); break;
				case KEY_LEFT:  snake.setDirectionTo(LEFT);  break;
				}
				snake.move();

				// 자기 몸체에 충돌하면 실패
				bool crashed = false;
				for (int index = 1; index < snake.getLength(); ++index)
					if (snake.getPositionOf(0)[0] == snake.getPositionOf(index)[0] &&
						snake.getPositionOf(0)[1] == snake.getPositionOf(index)[1]) {
						crashed = true;
						break;
					}
				if (crashed) break;

				// 벽에 부딪히면 실패
				if ((map.map[snake.getPositionOf(0)[0]][snake.getPositionOf(0)[1]] == WALL ||
					map.map[snake.getPositionOf(0)[0]][snake.getPositionOf(0)[1]] == IMMUNE_WALL) &&
					!((snake.getPositionOf(0)[0] == gates[0].x && snake.getPositionOf(0)[1] == gates[0].y) ||
						(snake.getPositionOf(0)[0] == gates[1].x && snake.getPositionOf(0)[1] == gates[1].y))) {
					break;
				}

				// 아이템 획득
				for (int index = 0; index < itemNum; ++index) {
					// items[index]와 위치가 겹치면
					if (snake.getPositionOf(0)[0] == items[index].x &&
						snake.getPositionOf(0)[1] == items[index].y) {
						if (items[index].Item_type == 1) //growthItem일 경우 길이 증가
							snake.increaseLength();
						else //poisonItem일 경우 길이 감소
							snake.decreaseLength();


						Item_respawn(items[index]);
						//아이템이 먹힌 경우 리스폰 뿐만 아니라, 아이템을 화면에서 잠시 숨겨야 함.
						items[index].active = false;

						break;
					}
				}

				int dx[4] = { -1,1,0,0 };
				int dy[4] = { 0,0,1,-1 };

				int clockwise[4] = { 2,3,1,0 }; //시계방향으로 바꾼 방향
				int c_clockwise[4] = { 3,2,0,1 };//반시계방향으로
				int oppwise[4] = { 1,0,3,2 };//반대방향으로 

				for (int i = 0; i < 2; ++i) {
					if (snake.getPositionOf(0)[0] == gates[i].x && snake.getPositionOf(0)[1] == gates[i].y) {
						int oppositeGateIndex = (i == 0) ? 1 : 0;
						int wall = gates[oppositeGateIndex].checkWall();
						int XgoTo = 0, YgoTo = 0;

						if (wall == -1) { //중앙 벽일 경우 
							int current_dir = snake.getDirection();
							int new_dir;
							if (can_move(screen[gates[oppositeGateIndex].x + dx[current_dir]][gates[oppositeGateIndex].y + dy[current_dir]]))
								new_dir = current_dir; //현재 진행방향
							else if (can_move(screen[gates[oppositeGateIndex].x + dx[clockwise[current_dir]]][gates[oppositeGateIndex].y + dy[clockwise[current_dir]]]))
								new_dir = clockwise[current_dir];//시계방향
							else if (can_move(screen[gates[oppositeGateIndex].x + dx[c_clockwise[current_dir]]][gates[oppositeGateIndex].y + dy[c_clockwise[current_dir]]]))
								new_dir = c_clockwise[current_dir];//반시계방향
							else
								new_dir = oppwise[current_dir];//반대방향

							snake.setDirectionTo(new_dir);
							XgoTo = gates[oppositeGateIndex].x + dx[new_dir];
							YgoTo = gates[oppositeGateIndex].y + dy[new_dir];
						}
						else { //게이트가 가장자리에 있는 경우, 반대방향으로 
							XgoTo = gates[oppositeGateIndex].x + dx[oppwise[wall]];
							YgoTo = gates[oppositeGateIndex].y + dy[oppwise[wall]];
							snake.setDirectionTo(oppwise[wall]);
						}

						snake.setPositionTo(XgoTo, YgoTo);
						break;
					}
				}
			}

			if (tick % itemTick == 0) {
				//아이템 재출현
				for (int i = 0; i < itemNum; i++)  Item_respawn(items[i]);
			}

			// screen에 map 올리기
			for (int row = 0; row < MAP_SIZE; ++row)
				for (int column = 0; column < MAP_SIZE; ++column)
					screen[row][column] = map.map[row][column];

			// screen에 item 올리기
			for (int index = 0; index < itemNum; ++index) {
				if (items[index].active) {
					screen[items[index].x][items[index].y]
					= (items[index].Item_type == 1) ? GROWTH_ITEM : POISON_ITEM;
				}
				
			}

			// screen에 gate 올리기
			// 정은: 아래 코드를 완성해주세요.
			for (Gate gate : gates) {
				// gate의 위치에 해당하는 위치를 screen에서 GATE(7)로 설정
				screen[gate.x][gate.y] = 7;

			}

			// screen에 snake 올리기
			for (int index = 0; index < snake.getLength(); ++index) {
				screen[snake.getPositionOf(index)[0]][snake.getPositionOf(index)[1]]
					= (index == 0) ? SNAKE_HEAD : SNAKE_BODY;
			}

			// screen을 화면에 출력
			for (int row = 0; row < MAP_SIZE; ++row)
				for (int column = 0; column < MAP_SIZE; ++column)
					printb(row, column, screen[row][column] + 1, game);
			refresh();
			wrefresh(game);
			wrefresh(score);

			if (tick == 0)
				Sleep(500);


			Sleep(TICK);
			++tick;
		}

		// victory가 false인 상태로 반복문 탈출했으면
		// 실패 처리하고 게임 종료
		if (!victory)
			break;
	}

	endwin();
	if (victory)
		cout << "contratulations!" << endl;
	else
		cout << "failed!" << endl;

	return 0;
}
