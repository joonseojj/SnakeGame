#include "Constants.h"
#include <random>
#include <chrono>
using namespace std;

class Item {
public:
    int x;
    int y; 
    bool active;
    int Item_type;//아이템 종류. 0이면 growth, 1이면 poison

    Item() {
        x = 0;
        y = 0;
        active = false;
        Item_type = std::rand() % 2;
    }
};
