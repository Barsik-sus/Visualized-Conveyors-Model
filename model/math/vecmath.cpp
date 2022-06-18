#include "vecmath.h"


float len(const QPoint &point) {
    const auto x = point.x();
    const auto y = point.y();
    return sqrt(x*x + y*y);
}

QPoint to_direction(QPoint point) {
    return point/len(point);
}
