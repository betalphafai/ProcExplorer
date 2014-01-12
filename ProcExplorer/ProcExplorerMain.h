#ifndef PROCEXPLORERMAIN_H_
#define PROCEXPLORERMAIN_H_

#include "ui_ProcExplorerMain.h"

class ProcTableView;

class ProcExplorerMain : public QMainWindow
{
    Q_OBJECT

 public:
    ProcExplorerMain(QWidget *_parent = 0, Qt::WindowFlags _flags = 0);
    ~ProcExplorerMain(void);
 private:
    Ui::ProcExplorerMainClass ui_;
    ProcTableView *proc_tableView_;
//     QMap<DWORD, ProcessInfo *> proc_id_;
};

#endif // PROCEXPLORERMAIN_H_