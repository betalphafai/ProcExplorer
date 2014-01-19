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
    Ui::ProcExplorerMain ui_;
    ProcTableView *proc_tableView_;
 public slots:
    void set_proc_num(int _num);
};

#endif // PROCEXPLORERMAIN_H_