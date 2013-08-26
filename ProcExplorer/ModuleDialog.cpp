
#include <QTableView>

#include "ModuleDialog.h"

ModuleDialog::ModuleDialog(QWidget *_parent)
    : QDialog(_parent)
{
    ui.setupUi(this);
}

ModuleDialog::~ModuleDialog(void)
{

}

void ModuleDialog::set_data(ProcessInfo *_item)
{
    ui.tableView->setModel(_item->toStandardModuleItemList());
}