#ifndef MODULEDIALOG_H_
#define MODULEDIALOG_H_

#include <QDialog>

#include "ui_ModuleDialog.h"

class ModuleDialog : public QDialog
{
    Q_OBJECT

public:
    ModuleDialog(QWidget *_parent = 0);
    ~ModuleDialog(void);

private:
    Ui::ModuleDialog ui;
};

#endif // MODULEDIALOG_H_
