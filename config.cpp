#include "config.h"
#include <QDebug>

Config::Config()
{

}

void Config::parse(QFile& file)
{
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject JON = doc.object();
    if (JON["Position"].toObject().value("NUMBER").isArray())
    {
        position.MIN_NUMBER = JON["Position"].toObject().value("NUMBER")[0].toInt();
        position.MAX_NUMBER = JON["Position"].toObject().value("NUMBER")[1].toInt();
    }
    if (JON["Position"].toObject().value("COUNT_MODULES").isArray())
    {
        position.MIN_COUNT_MODULES = JON["Position"].toObject().value("COUNT_MODULES")[0].toInt();
        position.MAX_COUNT_MODULES = JON["Position"].toObject().value("COUNT_MODULES")[1].toInt();
    }
    if (JON["Position"].toObject().value("AVG_TIME").isArray())
    {
        position.MIN_AVG_TIME = JON["Position"].toObject().value("AVG_TIME")[0].toDouble();
        position.MAX_AVG_TIME = JON["Position"].toObject().value("AVG_TIME")[1].toDouble();
    }
    if (JON["Position"].toObject().value("STEP_TIME").isArray())
    {
        position.MIN_STEP_TIME = JON["Position"].toObject().value("STEP_TIME")[0].toDouble();
        position.MAX_STEP_TIME = JON["Position"].toObject().value("STEP_TIME")[1].toDouble();
    }

    if (JON["Module"].toObject().value("SIZE").isArray())
    {
        platform.WIDTH = JON["Module"].toObject().value("SIZE")[0].toInt();
        platform.HEIGHT = JON["Module"].toObject().value("SIZE")[1].toInt();
    }

    if (JON["Transborder"].toObject().value("SIZE").isArray())
    {
        transborder.WIDTH = JON["Transborder"].toObject().value("SIZE")[0].toInt();
        transborder.HEIGHT = JON["Transborder"].toObject().value("SIZE")[1].toInt();
    }

    if (JON["Train"].toObject().value("SIZE").isArray())
    {
        train.WIDTH = JON["Train"].toObject().value("SIZE")[0].toInt();
        train.HEIGHT = JON["Train"].toObject().value("SIZE")[1].toInt();
    }

    if (JON["MAX_COUNT_TRAINS"].isDouble())
        MAX_COUNT_TRAINS = JON["MAX_COUNT_TRAINS"].toInt();
}

Config& Config::set()
{
    static Config instance;
    return instance;
}

Config const& Config::get()
{
    return set();
}
