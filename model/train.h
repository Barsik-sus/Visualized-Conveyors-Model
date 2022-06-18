#ifndef TRAIN_H
#define TRAIN_H

#include "../config.h"
#include "robject.h"
#include "math/vecmath.h"

class Train : public VObject
{
public:
    Train(){};
    explicit Train(QImage image);
    ~Train() = default;

    void render(QPainter *painter) override;

private:
    QImage image;

    int width = 120; // 100
    int height = 40; // 50
};

#endif // TRAIN_H
