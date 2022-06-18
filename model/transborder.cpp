#include "transborder.h"

Transborder::Transborder()
{

}

Transborder::Transborder(QPoint position)
{
    VObject::pos = position;

    width = Config::get().transborder.WIDTH;
    height = Config::get().transborder.HEIGHT;

    localePlace.setX((Config::get().transborder.WIDTH-Config::get().train.WIDTH)/2);
    localePlace.setY((Config::get().platform.HEIGHT-Config::get().train.HEIGHT)/2);
}

void Transborder::render(QPainter *painter)
{
    painter->setBrush(QBrush(QColor(80, 80, 80)));
    painter->drawRect(QRect(pos.x(), pos.y()+localePlace.y()/2, width, height));

    // Рельсы
    const auto magicNumber = Config::get().platform.HEIGHT/3;
    painter->drawRect(QRect(pos.x(), pos.y()+magicNumber, width, 2));
    painter->drawRect(QRect(pos.x(), pos.y()+magicNumber*2, width, 2));
}

bool Transborder::isFree()
{
    return train == nullptr;
}

QPoint Transborder::getPlacePos()
{
    return QPoint(pos.x()+localePlace.x(), pos.y()+localePlace.y());
}
