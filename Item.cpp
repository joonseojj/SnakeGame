#include <ctime>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <curses.h>
#include "map.cpp"

class Item {
public:
    int x;
    int y;
    bool active;
    std::chrono::steady_clock::time_point lastTime;

    Item() {
        x = 0;
        y = 0;
        active = false;
    }  

    void respawn() {
        int new_x = std::rand() % MAP_SIZE;
        int new_y = std::rand() % MAP_SIZE;
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

class GrowthItem : public Item {
public:
    GrowthItem() : Item() { }
};

class PoisonItem : public Item {
public:
    PoisonItem() : Item() { }
};



//
//void checkItemTime(GrowthItem growthItems[], PoisonItem poisonItems[]) {
//    for (int i = 0; i < 3; ++i) {
//        if (growthItems[i].active &&
//            (std::chrono::steady_clock::now() - growthItems[i].lastTime) >= std::chrono::seconds(5)) {
//            growthItems[i].respawn();
//        }
//
//        if (poisonItems[i].active &&
//            (std::chrono::steady_clock::now() - poisonItems[i].lastTime) >= std::chrono::seconds(5)) {
//            poisonItems[i].respawn();
//        }
//    }
//}
//
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
