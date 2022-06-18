#include "position.h"

Position::Position()
{

}

Position::Position(int type, float avgTime, float step)
    : type(type), avgTime(avgTime), step(step)
{

}

Position::~Position()
{

}

void Position::render(QPainter *painter)
{
    for (auto& platform: platforms)
        platform.render(painter);
}

void Position::addPlatform()
{
    const auto number = (QString::number(type)+QString::number(platforms.size()+1)).toInt();
    platforms.emplace_back(number, QPoint(pos.x(), pos.y()+currentHeight));
    currentHeight += Config::get().platform.HEIGHT;
}

Platform* Position::getFreePlatform()
{
//    for (auto& platform: platforms)
//        if  (platform.isFree())
//            return &platform;
    auto rv = std::find_if(platforms.begin(), platforms.end(), [](auto p){ return p.isFree(); });
    return (rv != platforms.end()) ? &*rv: nullptr;

//    return nullptr;
}

float Position::getWaitingTime()
{
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<float> d{avgTime, step};
    return d(gen);
}
