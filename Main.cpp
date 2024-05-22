#include <curses.h>
#include <chrono> 
#include <ctime>
#include <thread>
#include "map.cpp"
#include "Snake.cpp"
#include "Item.cpp"

std::chrono::steady_clock::time_point last_move_time;

void printBlockAt(int x, int y, int colorPair) {
    move(x, y);
    attron(COLOR_PAIR(colorPair));
    addch(ACS_BLOCK);
    addch(ACS_BLOCK);
    attron(COLOR_PAIR(colorPair));
}

void Item_respawn(Item item) {
    int new_x = 9 + std::rand() % MAP_SIZE;
    int new_y = 9 + std::rand() % MAP_SIZE;

    item.x = new_x;
    item.y = new_y;
    item.active = true;
    item.lastTime = std::chrono::steady_clock::now();
}

void checkItemTime(GrowthItem growthItems[], PoisonItem poisonItems[]) {
    for (int i = 0; i < 3; ++i) {
        if (growthItems[i].active &&
            (std::chrono::steady_clock::now() - growthItems[i].lastTime) >= std::chrono::seconds(5)) {
            Item_respawn(growthItems[i]);
        }

        if (poisonItems[i].active &&
            (std::chrono::steady_clock::now() - poisonItems[i].lastTime) >= std::chrono::seconds(5)) {
            Item_respawn(poisonItems[i]);
        }
    }
}
///*
//void checkItemCollision(GrowthItem growthItems[], PoisonItem poisonItems[], Snake& snake) {
//    for (int i = 0; i < 3; ++i) {
//        if (growthItems[i].active &&
//            growthItems[i].x == snake.getPositionOf(0)[0] &&
//            growthItems[i].y == snake.getPositionOf(0)[1]) {
//            growthItems[i].onCollision();
//            snake.increaseLength();
//        }
//
//        if (poisonItems[i].active &&
//            poisonItems[i].x == snake.getPositionOf(0)[0] &&
//            poisonItems[i].y == snake.getPositionOf(0)[1]) {
//            poisonItems[i].onCollision();
//            snake.decreaseLength();
//        }
//    }
//}
//*/

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    initscr();

    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLUE);
    init_pair(2, COLOR_WHITE, COLOR_WHITE);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLACK, COLOR_WHITE);
    init_pair(5, COLOR_YELLOW, COLOR_YELLOW);

    keypad(stdscr, true);
    noecho();

    MAP map;
    map.draw_map();
    Snake snake(3, RIGHT, MAP_SIZE / 2, MAP_SIZE / 2);


    GrowthItem growthItems[3];
    PoisonItem poisonItems[3];

    for (int i = 0; i < 3; ++i) {
        Item_respawn(growthItems[i]);
        Item_respawn(poisonItems[i]);
    }
    last_move_time = std::chrono::steady_clock::now();

    while (true) {
        map.draw_map();

        // Snake Visualization
        for (int index = 0; ; ++index) {
            if (snake.getPositionOf(index)[0] == -1 ||
                snake.getPositionOf(index)[1] == -1)
                break;

            printBlockAt(START_X + snake.getPositionOf(index)[0], START_Y + snake.getPositionOf(index)[1] * 2, 5);
        }
        refresh();

        // Snake Update
        int keyboardInput = getch();
        if ((std::chrono::steady_clock::now() - last_move_time) >= std::chrono::milliseconds(500)) {
            switch (keyboardInput) {
            case KEY_UP:    snake.setDirectionTo(LEFT);  break;
            case KEY_DOWN:  snake.setDirectionTo(RIGHT); break;
            case KEY_RIGHT: snake.setDirectionTo(UP);    break;
            case KEY_LEFT:  snake.setDirectionTo(DOWN);  break;
            }
            snake.move();
            last_move_time = std::chrono::steady_clock::now();
        }

        checkItemTime(growthItems, poisonItems); // 매번 각 아이템의 리스폰 시간이 5초 지났는지 확인 
        //checkItemCollision(grothItems, poisonItems);//매번 각 아이템이 스네이크와 충돌하는지 확인 
        //std::this_thread::sleep_for(std::chrono::milliseconds(500)); //0.5초 대기 
    }

    endwin();
    return 0;
}
