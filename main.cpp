#include "widget.h"

#include <QApplication>
#include "model/manager.h"
#include "config.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile configFile("./config.json");
    Config::set().parse(configFile);
    configFile.close();

    Widget w;
    w.show();

    return a.exec();
}
