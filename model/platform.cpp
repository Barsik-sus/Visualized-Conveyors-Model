#include "platform.h"

Platform::Platform()
{
}

Platform::Platform(int number, QPoint pos)
{
    VObject::pos = pos;
    this->number = number;
    width = Config::get().platform.WIDTH;
    height = Config::get().platform.HEIGHT;
    localePlace.setX((Config::get().platform.WIDTH-Config::get().train.WIDTH)/2);
    localePlace.setY((Config::get().platform.HEIGHT-Config::get().train.HEIGHT)/2);
}

void Platform::render(QPainter *painter)
{
    painter->setBrush(QBrush(QColor(100, 100, 100)));
    painter->drawRect(QRect(pos.x(), pos.y(), width, height));
    // Рельсы
    const auto magicNumber = Config::get().platform.HEIGHT/3;
    painter->drawRect(QRect(pos.x(), pos.y()+magicNumber, width, 2));
    painter->drawRect(QRect(pos.x(), pos.y()+magicNumber*2, width, 2));

    // Сигнал
    if (isWorking)
        painter->setBrush(QBrush(Qt::red));
    else
        painter->setBrush(QBrush(Qt::green));

    painter->drawRect(
                pos.x()+width/2, pos.y(),
                10, 10
    );
    painter->setBrush(QBrush(Qt::black));

    // Номер
    painter->setPen(Qt::white);
    painter->drawText(
                pos.x()+width/2-10,pos.y()+height/2,
                QString::number(number)
    );
    painter->setPen(Qt::black);
}

bool Platform::set(Train *train)
{
    if (!isFree())
        return false;
    this->train = train;
    return true;
}

Train* Platform::get()
{
    return train;
}

void Platform::free()
{
    train = nullptr;
}

bool Platform::isFree()
{
    return train == nullptr;
}

QPoint Platform::getPlacePos()
{
    return QPoint(pos.x()+localePlace.x(), pos.y()+localePlace.y());
}
