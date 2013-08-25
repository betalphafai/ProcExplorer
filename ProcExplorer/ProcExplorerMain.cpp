

#include <QStandardItem>
#include <QString>
#include <QList>
#include <QTimer>

#include "ProcExplorerMain.h"

ProcExplorerMain::ProcExplorerMain(QWidget *_parent, Qt::WFlags _flags)
    : QMainWindow(_parent, _flags)
{
    ui_.setupUi(this);
    proc_tableView_ = new ProcTableView(this);

    QStandardItemModel *_model = new QStandardItemModel(this);
    QStringList _header;
    _header << "process name" << "process id" << "parent process id "
        << "thread count" << "path" << "usage" << "block size";
    _model->setHorizontalHeaderLabels(_header);
    proc_tableView_->setModel(_model);
    proc_tableView_->verticalHeader()->setHidden(true);
    proc_tableView_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui_.verticalLayout->addWidget(proc_tableView_);

    // create a timer to reflush the process id every second
    QTimer *_timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()),
        proc_tableView_, SLOT(read_the_process()));
    _timer->start(1000);
}

ProcExplorerMain::~ProcExplorerMain(void)
{

}