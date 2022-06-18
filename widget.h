#ifndef WIDGET_H
#define WIDGET_H

#include <QDir>
#include <QPainter>
#include <QWidget>
#include <QMessageBox>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QRandomGenerator>

#include "model/manager.h"
#include "delegate.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void setWorld(Manager *world);

public slots:
    void run();
    void speedChanged(int value);
    void addRowToTable();
    void removeRowFromTable();
    void updateCounters();
    void switchMusicStatement();

private:
    Ui::Widget *ui;
    Manager *world = nullptr;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    bool isMusicPlaying = false;

    struct positionInitList{
        int type;
        int count;
        float avg;
        float step;
    };

private:
    void setupConfiguration();
    void generateTrains(int count, const std::vector<int>& tasks);
    void parseTable(std::vector<positionInitList>& input);
};
#endif // WIDGET_H
