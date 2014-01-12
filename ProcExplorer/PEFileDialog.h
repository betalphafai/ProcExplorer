#ifndef PEFILEDIALOG_H
#define PEFILEDIALOG_H

#include <QDialog>

#include "ui_PEFileDialog.h"

class PEFileDialog : public QDialog
{
    Q_OBJECT

public:
    PEFileDialog(QDialog *parent = 0);
    ~PEFileDialog();

private:
    Ui::PEFileDialog ui;
};

#endif // PEFILEDIALOG_H
