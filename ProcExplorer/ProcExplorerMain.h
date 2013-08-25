#ifndef PROCEXPLORERMAIN_H_
#define PROCEXPLORERMAIN_H_


#include <QtGui/QMainWindow>

#include "ui_ProcExplorerMain.h"
#include "ProcTableView.h"

class ProcExplorerMain : public QMainWindow
{
    Q_OBJECT

 public:
    ProcExplorerMain(QWidget *_parent = 0, Qt::WFlags _flags = 0);
    ~ProcExplorerMain(void);
 private:
    Ui::ProcExplorerMainClass ui_;
    ProcTableView *proc_tableView_;
//     QMap<DWORD, ProcessInfo *> proc_id_;
};

#endif // PROCEXPLORERMAIN_H_