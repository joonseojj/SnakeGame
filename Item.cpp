#include <cstdlib>
#include <ctime> //난수 생성용 
#include <iostream>
#include <thread>
#include <chrono> // 시간 계산용 
#include <curses.h>

int MAP_SIZE = 20;

class Item {
public:
    int x=0;
    int y=0;
    bool active = false;
    std::chrono::steady_clock::time_point lastTime;

    Item() :x(0),y(0),active(false) { }
    //Item(int _x, int _y) : x(_x), y(_y), active(true) {}

    void respawn() {
        int new_x = 9 + std::rand() % MAP_SIZE;
        int new_y = 9 + std::rand() % MAP_SIZE;
        x = new_x;
        y = new_y;
        active = true;
        lastTime = std::chrono::steady_clock::now();
    }

    void onCollision() {
        active = false;
        lastTime = std::chrono::steady_clock::now();
    }
};

class GrothItem : public Item {
public:
    GrothItem() {
        respawn();
    }
};

class PoisonItem : public Item {
public:
    PoisonItem() {
        respawn();
    }
};

void checkItemTime(GrothItem grothItems[], PoisonItem poisonItems[]) { //각 아이템마다 5초가 지나면 이동 
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (int i = 0; i < 3; ++i) {
        if (grothItems[i].active &&
            (std::chrono::steady_clock::now() - grothItems[i].lastTime) >= std::chrono::seconds(5)) {
            grothItems[i].respawn();
        }

        if (poisonItems[i].active &&
            (std::chrono::steady_clock::now() - poisonItems[i].lastTime) >= std::chrono::seconds(5)) {
            poisonItems[i].respawn();
        }

    }
}

//void checkItemCollision(GrothItem grothItems[], PoisonItem poisonItems[],Snake snake) { //스네이크와 충돌 검사
//    for (int i = 0; i < 3; ++i) {
//        if (grothItems[i].active &&
//            (grothItems[i].x == snake.getPositionOf()[0] && grothItems[i].y == snake.getPositionOf()[1]) {
//            grothItems[i].respawn();
//        }
//
//        if (grothItems[i].active &&
//            (grothItems[i].x == snake.getPositionOf()[0] && grothItems[i].y == snake.getPositionOf()[1]) {
//            poisonItems[i].respawn();
//        }
//    }
//}


