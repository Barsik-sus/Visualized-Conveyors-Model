#include "widget.h"
#include "./ui_widget.h"
#include "config.h"

#define repeat(what, howManyTimes) do {    \
    for (int i = 0; i < howManyTimes; i++)  \
        what;				 			    \
} while (false)


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setupConfiguration();
    auto file = QUrl::fromLocalFile(QDir::currentPath()+"/music.mp3");
    playlist = new QMediaPlaylist();
    playlist->addMedia(file);
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    player = new QMediaPlayer();
    player->setPlaylist(playlist);
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, ui->openGLW, &GLWidget::animate);
    connect(timer, &QTimer::timeout, this, &Widget::updateCounters);
    timer->start(50);
}

Widget::~Widget()
{
    delete ui;
    delete player;
    delete playlist;
    if (world != nullptr)
        delete world;
}

void Widget::setupConfiguration()
{
    auto delegator = new Delegate;
    ui->tableWidget->item(0,0)->setText(QString::number(Config::get().position.MIN_NUMBER));
    ui->tableWidget->item(1,0)->setText(QString::number(Config::get().position.MIN_NUMBER+1));
    delegator->columnMinAndMax.push_back({
                                             Config::get().position.MIN_NUMBER,
                                             Config::get().position.MAX_NUMBER
                                         });
    delegator->columnMinAndMax.push_back({
                                             Config::get().position.MIN_COUNT_MODULES,
                                             Config::get().position.MAX_COUNT_MODULES
                                         });
    ui->tableWidget->item(0,1)->setText(QString::number(Config::get().position.MIN_COUNT_MODULES));
    ui->tableWidget->item(1,1)->setText(QString::number(Config::get().position.MIN_COUNT_MODULES));
    delegator->columnMinAndMax.push_back({
                                             Config::get().position.MIN_AVG_TIME,
                                             Config::get().position.MAX_AVG_TIME
                                         });
    ui->tableWidget->item(0,2)->setText(QString::number(Config::get().position.MIN_AVG_TIME));
    ui->tableWidget->item(1,2)->setText(QString::number(Config::get().position.MIN_AVG_TIME));
    delegator->columnMinAndMax.push_back({
                                             Config::get().position.MIN_STEP_TIME,
                                             Config::get().position.MAX_STEP_TIME
                                         });
    ui->tableWidget->item(0,3)->setText(QString::number(Config::get().position.MIN_STEP_TIME));
    ui->tableWidget->item(1,3)->setText(QString::number(Config::get().position.MIN_STEP_TIME));
    ui->tableWidget->setItemDelegate(std::move(delegator));
    ui->trainsCount->setMaximum(Config::get().MAX_COUNT_TRAINS);
}

void Widget::setWorld(Manager *world)
{
    this->world = world;
    ui->openGLW->world = world;
}

void Widget::generateTrains(int count, const std::vector<int>& tasks)
{
    for (int i = 0; i < count; i++)
    {
        QImage type;
        switch(QRandomGenerator::global()->bounded(9))
        {
        case 0:
            type = QImage(":/images/tanker.png");
            break;
        case 1:
            type = QImage(":/images/tankerR.png");
            break;
        case 2:
            type = QImage(":/images/tankerG.png");
            break;
        case 3:
            type = QImage(":/images/half.png");
            break;
        case 4:
            type = QImage(":/images/halfB.png");
            break;
        case 5:
            type = QImage(":/images/halfG.png");
            break;
        case 6:
            type = QImage(":/images/hopper.png");
            break;
        case 7:
            type = QImage(":/images/hopperB.png");
            break;
        case 8:
            type = QImage(":/images/hopperG.png");
            break;
        }
        world->addTrain(type, tasks);
    }
}

void Widget::parseTable(std::vector<positionInitList>& input)
{
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        const auto type = ui->tableWidget->item(i, 0)->text().toInt();
        const auto count = ui->tableWidget->item(i, 1)->text().toInt();
        const auto avg = ui->tableWidget->item(i, 2)->text().toFloat();
        const auto step = ui->tableWidget->item(i, 3)->text().toFloat();
        input.push_back({type, count, avg, step});
    }
}

void Widget::run()
{
    if (ui->tableWidget->rowCount() < 2)
        return;
    // TODO: Убрать на релизе
    QFile configFile("./config.json");
    Config::set().parse(configFile);

    isMusicPlaying = false;
    switchMusicStatement();

    if (world != nullptr)
        delete world;
    setWorld(new Manager());

    std::vector<positionInitList> input;
    parseTable(input);

    std::sort(input.begin()+1, input.end()-1,
            [](const auto &l, const auto &r)
            {
                if (l.count != r.count) {
                    return l.count > r.count;
                }

                return l.type > r.type;
            });

    // Добавляю первый модуль в модель
    repeat(world->addPlatform(input[0].type, input[0].avg, input[0].step, true), input[0].count);

    // Распределяю модули поровну между двумя половинами
    int weight = input[input.size()-1].count-input[0].count;
    for (auto position = input.begin()+1; position != input.end()-1; ++position) {
        if(weight > 0) {
            repeat(world->addPlatform((*position).type, (*position).avg, (*position).step, true), (*position).count);
            weight -= (*position).count;
        } else {
            repeat(world->addPlatform((*position).type, (*position).avg, (*position).step, false), (*position).count);
            weight += (*position).count;
        }
    }

    // Добавляю последний модуль
    repeat(world->addPlatform(input[input.size()-1].type, input[input.size()-1].avg, input[input.size()-1].step, false), input[input.size()-1].count);

    std::vector<int> allPositions;
    std::reverse(input.begin()+1, input.end()-1);
    std::transform(input.begin(), input.end(), std::back_inserter(allPositions),
                   [](const positionInitList& p) -> int { return p.type; });


    generateTrains(ui->trainsCount->text().toInt(), allPositions);
}

void Widget::speedChanged(int speed)
{
    ui->openGLW->speed = float(speed)/10.0;
}

void Widget::addRowToTable()
{
    const auto count = ui->tableWidget->rowCount();
    if (Config::get().position.MIN_NUMBER+count > Config::get().position.MAX_NUMBER)
        return;
    ui->tableWidget->insertRow(count);
    ui->tableWidget->setItem(count, 0, new QTableWidgetItem(QString::number(Config::get().position.MIN_NUMBER+count)));
    ui->tableWidget->setItem(count, 1, new QTableWidgetItem(QString::number(Config::get().position.MIN_COUNT_MODULES)));
    ui->tableWidget->setItem(count, 2, new QTableWidgetItem(QString::number(Config::get().position.MIN_AVG_TIME)));
    ui->tableWidget->setItem(count, 3, new QTableWidgetItem(QString::number(Config::get().position.MIN_STEP_TIME)));
    auto item = ui->tableWidget->item(count, 0);
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
}

void Widget::removeRowFromTable()
{
    QModelIndexList selection = ui->tableWidget->selectionModel()->selectedRows();
    if (selection.size() > 0)
    {
        const auto row = selection[0].row();
        ui->tableWidget->removeRow(row);
        for (int i = row; i < ui->tableWidget->rowCount(); ++i)
        {
            auto item = ui->tableWidget->item(i,0);
            item->setText(QString::number(item->text().toInt()-1));
        }
    }
    else
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Не вдалося видалити рядок","Для видалення рядку, його необхідно виділити за допомогою миші, після чого натиснути кнопку «Видалити рядок»");
    }
}

void Widget::updateCounters()
{
    if (world != nullptr)
    {
        // TODO: заменить текст(должно быть что-то типа Надійшло до ремонту и Відремонтовано или типа того
        ui->comeIn->setText("Надійшло: "+QString::number(world->getCountAll()-world->getCountOnStart()));
        ui->comeOut->setText("Відремонтовано: "+QString::number(world->getCountAll()-world->getCountActive()-world->getCountOnStart()));
    }
}

void Widget::switchMusicStatement()
{
        if (isMusicPlaying)
        {
            player->stop();
            ui->playOrStopBtn->setText("►");
        }
        else
        {
            player->play();
            ui->playOrStopBtn->setText("| |");
        }
        isMusicPlaying = !isMusicPlaying;
}
