#ifndef PROCTABLEVIEW_H_
#define PROCTABLEVIEW_H_


#include "ui_ProcTableView.h"

class ProcessInfo;

class ProcTableView : public QTableView
{
    Q_OBJECT

 public:
    ProcTableView(QWidget *parent = 0);
    ~ProcTableView(void);
 private slots:
    void read_the_process(void);
    void show_menu(const QPoint &_point);
    void injection(void);
    void read_the_module(void);
    void read_the_pe_file(void);
    void open_the_file(void);

 private:
    Ui::ProcTableView ui;
    int focus_index_;
    int focus_proc_id_;
    QList<ProcessInfo *> proc_info_;
};

#endif // PROCTABLEVIEW_H_