#ifndef MANAGER_H
#define MANAGER_H

#include "../config.h"
#include "train.h"
#include "platform.h"
#include "position.h"
#include "transborder.h"
#include "animation.h"

struct Task
{
    Train train;
    std::vector<int> tasks;
    int currentTask = -1;
    Platform* currentPlatform = nullptr;
};

class Manager: public RObject
{
public:
    Manager();
    ~Manager() override;

    void update(float deltaSeconds) override;
    void render(QPainter *painter) override;

    void addTrain(QImage image, std::vector<int> tasks);
    void addPlatform(int type, float avgTime, float step, bool leftColumn);

    int getCountAll();
    int getCountOnStart();
    int getCountActive();

private:
    const int x_leftColumn = 0;
    const int x_rightColumn = 0+180+180;

private:
    // start trains
    std::vector<Task*> startTP;
    // active trains
    std::vector<Task*> activeTP;
    // all trains
    std::vector<Task*> trains;

    Platform deadPlatform;
    std::map<int, Position> positions;
    int leftColumnHeight = 0;
    int rightColumnHeight = 0;

    Transborder transborder; // 140, 0

    std::vector<Animation*> animations;

private:
    void addMoveAnimation(VObject* obj, QPoint pos);
    void addWaitAnimation(VObject* obj, int position);

    void updateTasks();
    void updateTrainTask(Task *train, const int task);

    void tryAddMoreTrains();
};

#endif // MANAGER_H
