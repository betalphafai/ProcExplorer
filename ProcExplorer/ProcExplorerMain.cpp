

#include <QMainWindow>
#include <QWidget>
#include <QStandardItem>
#include <QStringList>

#include "ProcExplorerMain.h"
#include "ProcTableView.h"
#include "ProcModel.h"

ProcExplorerMain::ProcExplorerMain(QWidget *_parent, Qt::WindowFlags _flags)
    : QMainWindow(_parent, _flags)
{
    ui_.setupUi(this);
    proc_tableView_ = new ProcTableView(this);
    ProcModel *_model = new ProcModel(this);
    proc_tableView_->setModel(_model);
    ui_.verticalLayout->addWidget(proc_tableView_);
}

ProcExplorerMain::~ProcExplorerMain(void)
{

}