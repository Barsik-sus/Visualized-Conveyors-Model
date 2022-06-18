#ifndef TRANSBORDER_H
#define TRANSBORDER_H

#include "../config.h"
#include "robject.h"
#include "train.h"
#include "platform.h"
#include "math/vecmath.h"

class Transborder: public VObject
{
public:
    QPoint localePlace; // 30 10

    Train *train = nullptr;
    Platform *from = nullptr;
//    QPoint needed_pos;

public:
    Transborder();
    explicit Transborder(QPoint position);

    void render(QPainter *painter) override;

    bool isFree();
    QPoint getPlacePos();

private:
    int width = 180; // 140
    int height = 50; // 90
};

#endif // TRANSBORDER_H
