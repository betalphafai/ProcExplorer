#ifndef PEFILEDIALOG_H
#define PEFILEDIALOG_H

#include <QWidget>
#include "ui_PEFileDialog.h"

class PEFileDialog : public QWidget
{
    Q_OBJECT

public:
    PEFileDialog(QWidget *parent = 0);
    ~PEFileDialog();

private:
    Ui::PEFileDialog ui;
};

#endif // PEFILEDIALOG_H
