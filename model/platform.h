#ifndef PLATFORM_H
#define PLATFORM_H

#include "../config.h"
#include "robject.h"
#include "train.h"

class Platform : public VObject
{
public:
    bool isWorking = false;
    QPoint localePlace; // 30 10

public:
    Platform();
    explicit Platform(int number, QPoint position);

    void render(QPainter *painter) override;

    void free();
    bool isFree();

    bool set(Train *train);
    Train *get();

    QPoint getPlacePos();

private:
    Train *train = nullptr;
    int number = -1;

    int width = 180; // 140
    int height = 60; // 90
};

#endif // PLATFORM_H
