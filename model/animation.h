#ifndef ANIMATION_H
#define ANIMATION_H

#include "robject.h"
#include "math/vecmath.h"


struct Animation
{
    VObject *obj = nullptr;
    float time = 0.0;

    explicit Animation(VObject *obj);
    Animation(VObject *obj, float time);
    virtual ~Animation() = default;

    virtual void update(float deltaSeconds);

    void updateTime(float time);
    bool isAlive();
};

struct AMove: public Animation
{
     AMove(VObject *obj, QPoint to);
     AMove(VObject *obj, QPoint to, float time);
     AMove(VObject *obj, QPoint to, float speed, bool);
    virtual ~AMove() = default;

    void update(float deltaSeconds) override;

private:
    QPoint to;
};

#endif // ANIMATION_H
