#ifndef PROCEXPLORERMAIN_H
#define PROCEXPLORERMAIN_H

#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

#include <QtGui/QMainWindow>

#include "ui_ProcExplorerMain.h"

class ProcExplorerMain : public QMainWindow
{
    Q_OBJECT

 public:
    ProcExplorerMain(QWidget *_parent = 0, Qt::WFlags _flags = 0);
    ~ProcExplorerMain(void);

 private:
    int get_proc_exec_path(DWORD _proc_id, QString *_exe_path);
 signals:
    void send_read_the_process(void);
 private slots:
    void read_the_process(void);
 private:
    Ui::ProcExplorerMainClass ui_;
};

#endif // PROCEXPLORERMAIN_H
