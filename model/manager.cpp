#include "manager.h"

Manager::Manager():
    x_rightColumn{ Config::get().platform.WIDTH+Config::get().transborder.WIDTH },
    transborder(QPoint(Config::get().platform.WIDTH, 0))
{

}

Manager::~Manager()
{
    for(size_t i = 0; i < trains.size(); i++)
        delete trains[i];

    // Должен быть пуст, но если было экстренное завершение
    for(size_t i = 0; i < animations.size(); i++)
        delete animations[i];
}

void Manager::update(float deltaSeconds)
{
    updateTasks();
    for (auto& animation: animations)
    {
        animation->update(deltaSeconds);
        if (!animation->isAlive()) {
            animation->update(deltaSeconds);
            animation->obj->isAnimating--;
            delete animation;
            animation = nullptr;
        }
    }
    animations.erase(std::remove(animations.begin(), animations.end(), nullptr),
                    animations.end());
}

void Manager::render(QPainter *painter)
{
    for (auto& [_, position]: positions)
        position.render(painter);

    // Рельсы для трансбордера
    const auto& x_transborder = transborder.pos.x();
    for (int i = 1; i < 6; i++)
        painter->drawRect(QRect(
                              x_transborder+i*Config::get().transborder.WIDTH/6, 0,
                              1, leftColumnHeight>rightColumnHeight?leftColumnHeight:rightColumnHeight));

    transborder.render(painter);

    for (auto& train: activeTP)
        train->train.render(painter);

}

void Manager::addPlatform(int type, float avgTime, float step, bool leftColumn)
{
    // Если этот модуль ещё не создан
    if (positions.emplace(type, Position(type, avgTime, step)).second)
        // emplace returns pair<iter,bool> where bool means whether the element was created or not
    {

        if (leftColumn)
        {
            positions[type].pos.setX(x_leftColumn);
            positions[type].pos.setY(leftColumnHeight);
        }
        else
        {
            positions[type].pos.setX(x_rightColumn);
            positions[type].pos.setY(rightColumnHeight);
        }
    }
    if (leftColumn)
        leftColumnHeight += Config::get().platform.HEIGHT;
    else
        rightColumnHeight += Config::get().platform.HEIGHT;

    positions[type].addPlatform();
}

void Manager::addTrain(QImage image, std::vector<int> tasks)
{
    std::reverse(tasks.begin(), tasks.end());
    auto train = new Task{ Train(image), tasks };
    trains.push_back(train);
    startTP.push_back(train);
}

int Manager::getCountAll()
{
    return static_cast<int>(trains.size());
}

int Manager::getCountOnStart()
{
    return static_cast<int>(startTP.size());
}

int Manager::getCountActive()
{
    return static_cast<int>(activeTP.size());
}

void Manager::addMoveAnimation(VObject* obj, QPoint pos)
{
    // magic number 20.0
    animations.push_back(new AMove(obj, pos, 20.0, true));
    obj->isAnimating++;
}

void Manager::addWaitAnimation(VObject* obj, int position)
{
    animations.push_back(new Animation(obj, positions[position].getWaitingTime()));
    obj->isAnimating++;
}

// если у поезда task:
// 		-1: он находится в неопределенном состоянии(ожидает)
//		-2: он завершил выполнение всех задач(свободен)
void Manager::updateTrainTask(Task *train, const int task)
{
    const auto& trainOnTransborder = transborder.train;
    // 1
    if (&train->train == trainOnTransborder && train->currentPlatform != nullptr) {
        // 2
        if (transborder.from != nullptr /*&& &train->train == transborder.from->get()*/) {
            transborder.from->free();
            transborder.from = nullptr;
            addMoveAnimation(&train->train, transborder.getPlacePos());
            // 3
        } else if (&train->train == train->currentPlatform->get()) {
            transborder.train = nullptr;
            // время ожидания на платформе(кроме первой)
            addWaitAnimation(&train->train, train->currentTask);
            // isWorking true
            train->currentPlatform->isWorking = true;
            // 4
            if (task == -1)
                train->tasks.push_back(-2);
        } else {
            auto transborderPos = QPoint(transborder.pos.x(), train->currentPlatform->pos.y());
            // 5
            if (transborder.pos == transborderPos) {
                train->currentPlatform->set(&train->train);
                addMoveAnimation(&train->train, train->currentPlatform->getPlacePos());
            } else {
                addMoveAnimation(transborder.train, QPoint(transborderPos.x()+transborder.localePlace.x(),
                                                       transborderPos.y()+transborder.localePlace.y()));
                addMoveAnimation(&transborder, transborderPos);
            }
        }
        // 6
    } else if (task == Config::get().position.MIN_NUMBER) {
        train->tasks.pop_back();
        addWaitAnimation(&train->train, task);
        // isWorking true
        train->currentPlatform->isWorking = true;
        // 7
    } else if (transborder.isFree() && train->currentPlatform != nullptr) {
        auto platformToGo = positions[task].getFreePlatform();
        // 8
        if (platformToGo != nullptr)
        {
            transborder.train = &train->train;
            transborder.from = train->currentPlatform;
            addMoveAnimation(&transborder, QPoint(transborder.pos.x(), train->currentPlatform->pos.y()));
            train->currentTask = task;
            train->currentPlatform = platformToGo;
            train->tasks.pop_back();
        }
    }
}

void Manager::tryAddMoreTrains()
{
    Platform *startPosition;
    while (!startTP.empty() && (startPosition = positions[Config::get().position.MIN_NUMBER].getFreePlatform()) != nullptr)
    {
        auto& currentTrain = startTP.back();
        activeTP.push_back(currentTrain);
        startTP.pop_back();

        currentTrain->train.pos.setY(startPosition->getPlacePos().y());
        startPosition->set(&currentTrain->train);
        currentTrain->currentPlatform = startPosition;
        addMoveAnimation(&currentTrain->train, startPosition->getPlacePos());
    }
}

// если у поезда task:
// 		-1: он находится в неопределенном состоянии(ожидает)
//		-2: он завершил выполнение всех задач(свободен)
void Manager::updateTasks()
{
    // 1
    if (positions.empty())
        return;

    tryAddMoreTrains();

    // освободить уже отработавшие вагоны
    activeTP.erase(std::remove_if(activeTP.begin(), activeTP.end(),
                       [&](Task *t) {
                           return t->currentPlatform == &deadPlatform && t->train.isAnimating == 0;
                       }), activeTP.end());

    // 2
    for (auto& train: activeTP) {
        int task = train->tasks.empty() ? -1 : train->tasks.back();
        // если анимируется либо тележка, либо данный вагон
        // 3
        if (train->train.isAnimating != 0 || transborder.isAnimating != 0) {
            continue;
            // 4
        } else if (task == -2)
            // если вагон уже завершил задачи и не анимируется
        {
            // isWorking false
            train->currentPlatform->isWorking = false;
            train->currentPlatform->free();
            train->currentPlatform = &deadPlatform;
            const auto widthScene = Config::get().platform.WIDTH*2+Config::get().transborder.WIDTH+Config::get().train.WIDTH;
            addMoveAnimation(&train->train, QPoint(widthScene, train->train.pos.y()));
        } else {
            // isWorking false
            train->currentPlatform->isWorking = false;
            updateTrainTask(train, task);
        }
    }
}
