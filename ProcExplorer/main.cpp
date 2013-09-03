
#include <QApplication>
#include <QStyleFactory>

#include "ProcExplorerMain.h"

int main(int argc, char *argv[])
{
    QApplication _app(argc, argv);
    QApplication::setStyle(QStyleFactory::create("fusion"));

    ProcExplorerMain _win_main;
    _win_main.show();
    return _app.exec();
}
