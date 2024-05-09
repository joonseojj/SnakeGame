#include <cstdlib>
#include <ctime> //난수 생성용 
#include <iostream>
#include <thread>
#include <chrono> // 시간 계산용 
#include <curses.h>
#include "./map.cpp"

//int MAP_SIZE = 20;

class Item {
public:
    int x;
    int y;
    bool active = false;
    std::chrono::steady_clock::time_point lastTime;

    Item() {}
    Item(int _x, int _y) : x(_x), y(_y), active(true) {}

    void respawn() { 
        int new_x = 9 + std::rand() % MAP_SIZE;
        int new_y = 9 + std::rand() % MAP_SIZE;
        x = new_x;
        y = new_y;
        active = true;
        lastTime = std::chrono::steady_clock::now();
    }

    void onCollision()  {
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

GrothItem grothItems[3];
PoisonItem poisonItems[3];

void checkItemCollision() {
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


/*
int main() {
    // 프로그램이 처음 실행될 때만 시드를 설정
    std::srand(static_cast<unsigned int>(std::time(nullptr))); 

    while (true) {
        checkItemCollision();
        for (int i = 0; i < 3; ++i) {
            std::cout << grothItems[i].x << ' ' << grothItems[i].y << "\n";
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "\n";
    }
}
*/


/*
class Game {
private:
    GrothItem grothItems[3];
    PoisonItem poisonItems[3];
    Snake snake;

    // 아이템이 스네이크의 머리와 충돌하거나, 
    // 현재 시간 - 아이템의 마지막 시간 >= 5일 때
    // 해당 아이템을 respawn 하는 메소드. 
    // 이 메소드는 1초마다 실행해야 한다. 
    void checkItemCollision() {
        for (int i = 0; i < 3; ++i) {
            // Growth 아이템과의 충돌 or 시간 확인 
            if (grothItems[i].active &&
                (grothItems[i].x == snake.getPositionOf()[0] && grothItems[i].y == snake.getPositionOf()[1]) ||
                (std::chrono::steady_clock::now() - grothItems[i].lastTime) >= std::chrono::seconds(5)) {
                grothItems[i].respawn();
            }

            // Poison 아이템과의 충돌 or 시간 확인
            if (poisonItems[i].active &&
                (poisonItems[i].x == snake.getPositionOf()[0] && poisonItems[i].y == snake.getPositionOf()[1]) ||
                (std::chrono::steady_clock::now() - poisonItems[i].lastTime) >= std::chrono::seconds(5)) {
                poisonItems[i].respawn();
            }
        }
    }
};
*/
