#define SYS_MIN_LENGTH 1
#define SYS_MAX_LENGTH 30

#define UP    0
#define DOWN  1
#define RIGHT 2
#define LEFT  3

#include <stdexcept>
#include "map.cpp"

class Snake {
private:
    int minLength;
    int goalLength;

    int length;
    int direction;
    int position[SYS_MAX_LENGTH][2];

    int tailTrace[2];

    void initializeLengthTo(int length_) {
        if (length_ >= SYS_MIN_LENGTH &&
            length_ <= SYS_MAX_LENGTH)
            length = length_;
        else throw out_of_range("INVALID LENGTH");
    }

    void initializeTailTrace() {
        tailTrace[0] = tailTrace[1] = -1;
    }

public:
    Snake(int length, int direction, int x, int y, int minLength, int goalLength) {
        initializeLengthTo(length);
        setDirectionTo(direction);
        for (int i = 0; i < SYS_MAX_LENGTH; ++i)
            if (i < length)
                switch (direction) {
                case UP:    setPositionTo(x, y - i, i); break;
                case DOWN:  setPositionTo(x, y + i, i); break;
                case RIGHT: setPositionTo(x - i, y, i); break;
                case LEFT:  setPositionTo(x + i, y, i); break;
                }
            else setPositionTo(-1, -1, i);
        setLimitsTo(minLength, goalLength);
        initializeTailTrace();
    }

    void increaseLength() {
        ++length;
        if (length <= goalLength) {
            position[length][0] = tailTrace[0];
            position[length][1] = tailTrace[1];
        }
    }

    void decreaseLength() {
        --length;
        if (length >= minLength)
            position[length][0] = position[length][1] = -1;
    }

    void move() {
        tailTrace[0] = position[length - 1][0];
        tailTrace[1] = position[length - 1][1];

        for (int i = length - 1; i > 0; --i) {
            position[i][0] = position[i - 1][0];
            position[i][1] = position[i - 1][1];
        }

        switch (direction) {
        case UP:    ++position[0][1]; break;
        case DOWN:  --position[0][1]; break;
        case RIGHT: ++position[0][0]; break;
        case LEFT:  --position[0][0]; break;
        }
    }

    int getLength() {
        return length;
    }

    int getDirection() {
        return direction;
    }

    int* getPositionOf(int index = 0) {
        return position[index];
    }

    void setDirectionTo(int direction_) {
        if (0 <= direction_ <= 3)
            direction = direction_;
        else throw out_of_range("INVALID DIRECTION");
    }

    void setPositionTo(int x, int y, int index = 0) {
        if (0 <= x && x < MAP_SIZE &&
            0 <= y && y < MAP_SIZE) {
            position[index][0] = x;
            position[index][1] = y;
        }
        else throw out_of_range("INVALID POSITION");
    }

    void setLimitsTo(int minLength_, int goalLength_) {
        if (minLength_ >= SYS_MIN_LENGTH &&
            minLength_ <= goalLength_ &&
            goalLength_ <= SYS_MAX_LENGTH) {
            minLength = minLength_;
            goalLength = goalLength_;
        }
        else throw out_of_range("INVALID LENGTH");
    }
};