#include <cstdlib>
#include <ctime>
#include <chrono> // 시간 측정을 위한 헤더 파일
#include <curses.h>
#include "./map.cpp"
#include "./Snake.cpp"

class Item {
public:
    int x;
    int y;
    bool active = false; // 아이템이 활성화되어 있는지
    std::chrono::steady_clock::time_point lastTime; // 마지막 아이템 초기화시간 저장 

    Item() {}
    Item(int _x, int _y) : x(_x), y(_y), active(true) {} // 새로 생성할때는 active를 true로 시작 

    
    // 아이템이 스네이크의 머리와 충돌했을 때 
    // active = false , lastTime = now 하는 메서드
    virtual void onCollision(Snake& snake) = 0;

    
    //아이템을 랜덤 위치로 이동시키고, 숨겨져 있는 경우 다시 꺼내는 (생성) 메소드 
    void respawn() { 
        int new_x = rand() % MAP_SIZE;
        int new_y = rand() % MAP_SIZE;
        //스네이크와 닿지 않는 위치로 움직이는 코드 추가
        x = new_x;
        y = new_y;
        active = true; //활성화
        lastTime = std::chrono::steady_clock::now(); // 마지막 생성 시간 갱신
    }
};

class GrothItem : public Item {//상속
public:
    GrothItem() {
        respawn();
    }

    void onCollision(Snake& snake)  {
        snake.increaseLength();// 스네이크 길이 증가
        active = false;
        lastTime = std::chrono::steady_clock::now(); // 현재 시간으로 초기화
    }

};

class PoisonItem : public Item {

public:
    PoisonItem() {
        respawn();
    }

    void onCollision(Snake& snake) {
        snake.decreaseLength();// 스네이크 길이 감소 
        active = false;
        lastTime = std::chrono::steady_clock::now(); // 현재 시간으로 초기화
    }

};





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