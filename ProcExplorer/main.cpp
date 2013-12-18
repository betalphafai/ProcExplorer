
#include <QApplication>
#include <QStyleFactory>
#include <QMessageBox>

#include "ProcExplorerMain.h"
#include "GlobalSupport.h"

int main(int argc, char *argv[])
{
    QApplication _app(argc, argv);
    QApplication::setStyle(QStyleFactory::create("fusion"));

    if (!GlobalSupport::enable_debug_privilege(true))
    {
        QMessageBox::warning(
            NULL, "Warning", "Elevation Privilege Error");
        return 1;
    }

    ProcExplorerMain _win_main;
    _win_main.show();
    return _app.exec();
}
