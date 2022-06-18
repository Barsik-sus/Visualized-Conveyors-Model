#ifndef CONFIG_H
#define CONFIG_H

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>

struct Config
{
public:
    struct Position {
        int MIN_NUMBER = 1;
        int MAX_NUMBER = 10;
        int MIN_COUNT_MODULES = 1;
        int MAX_COUNT_MODULES = 10;
        float MIN_AVG_TIME = 1.0f;
        float MAX_AVG_TIME = 1000.0f;
        float MIN_STEP_TIME = 1.0f;
        float MAX_STEP_TIME = 1000.0f;
    } position;

    struct Platform {
        int WIDTH = 180;
        int HEIGHT = 60;
    } platform;

    struct Transborder {
        int WIDTH = 180;
        int HEIGHT = 50;
    } transborder;

    struct Train {
        int WIDTH = 120;
        int HEIGHT = 40;
    } train;

    int MAX_COUNT_TRAINS = 1000;

public:
    void parse(QFile& file);
public:
    static Config& set();
    static Config const& get();

private:
    Config();
    Config(const Config&);
    Config& operator=(const Config&) = delete;

};


#endif // CONFIG_H
