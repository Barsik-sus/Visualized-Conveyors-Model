#include "train.h"

Train::Train(QImage image) :
    image(image)
{
    width = Config::get().train.WIDTH;
    height = Config::get().train.HEIGHT;

    pos = QPoint(-width-10,10); // -200 20
}

void Train::render(QPainter *painter)
{
    painter->drawImage(QRect(pos.x(), pos.y(), width, height), image);
}
