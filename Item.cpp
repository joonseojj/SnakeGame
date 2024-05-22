
#include <cstdlib>
#include <chrono>
#include <thread>
#include <curses.h>

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

