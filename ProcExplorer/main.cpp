#include "ProcExplorerMain.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
    QApplication _app(argc, argv);
    ProcExplorerMain _win_main;
    _win_main.show();
    return _app.exec();
}
