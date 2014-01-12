

#include <QMainWindow>
#include <QWidget>
#include <QStandardItem>
#include <QStringList>

#include "ProcExplorerMain.h"
#include "ProcTableView.h"

ProcExplorerMain::ProcExplorerMain(QWidget *_parent, Qt::WindowFlags _flags)
    : QMainWindow(_parent, _flags)
{
    ui_.setupUi(this);
    proc_tableView_ = new ProcTableView(this);
    QStandardItemModel *_model = new QStandardItemModel(this);
    QStringList _header;
    _header << "Process Name" << "Process Id" << "Parent Process Id "
        << "Thread Count" << "Path" << "Usage" << "Block Size";
    _model->setHorizontalHeaderLabels(_header);
    proc_tableView_->setModel(_model);
    ui_.verticalLayout->addWidget(proc_tableView_);
}

ProcExplorerMain::~ProcExplorerMain(void)
{

}