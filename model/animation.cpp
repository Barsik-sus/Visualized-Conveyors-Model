#include "animation.h"

Animation::Animation(VObject *obj) : obj(obj)
{

}

Animation::Animation(VObject *obj, float time) : obj(obj), time(time)
{

}

void Animation::update(float deltaSeconds)
{
    time -= deltaSeconds;
}

void Animation::updateTime(float time)
{
    this->time = time;
}

bool Animation::isAlive()
{
    return obj != nullptr && time > 0.0;
}

AMove::AMove(VObject *obj, QPoint to) : Animation(obj), to(to)
{

}

AMove::AMove(VObject *obj, QPoint to, float time) : Animation(obj, time), to(to)
{

}

AMove::AMove(VObject *obj, QPoint to, float speed, bool):AMove(obj, to)
{
    const auto x = to.x() - obj->pos.x();
    const auto y = to.y() - obj->pos.y();
    this->time = sqrt(x*x + y*y)/speed;
}

void AMove::update(float deltaSeconds)
{
    auto to_obj = to-obj->pos;
    if (time <= 0.0)
        obj->pos = to;
    else
        obj->pos += to_obj*(deltaSeconds/time);

    Animation::update(deltaSeconds);
}
