#ifndef POSITION_H
#define POSITION_H

#include <random>
#include "platform.h"

class Position: public VObject
{
public:
    int type = -1;
    float avgTime = 20.0;
    float step = 5.0;
    int currentHeight = 0;

public:
    Position();
    explicit Position(int type, float avgTime, float step);
    ~Position() override;

    void render(QPainter *painter) override;

    void addPlatform();
    Platform *getFreePlatform();
    float getWaitingTime();

private:
    std::vector<Platform> platforms;
};

#endif // POSITION_H
