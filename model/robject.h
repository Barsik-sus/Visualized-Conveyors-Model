#ifndef ROBJECT_H
#define ROBJECT_H

#include <QPainter>

// Renderable Object
struct RObject
{
    RObject();
    virtual ~RObject() = default;

    virtual void update(float deltaTime);
    virtual void render(QPainter *painter) = 0;
};

// Visible Object
struct VObject: public RObject
{
    QPoint pos;
    // if it isn't 0, it means that it is animating
    int isAnimating = 0;
};

#endif // ROBJECT_H
