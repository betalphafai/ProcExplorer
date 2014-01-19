
#include <QTableView>

#include "ModuleDialog.h"
#include "ModModel.h"

ModuleDialog::ModuleDialog(unsigned int _id, QWidget *_parent)
    : QDialog(_parent)
{
    ui.setupUi(this);
    ModModel *_model = new ModModel();

//////////////////////////////////////////////////////////////////////////
    // 获取指定进程的模块信息
    HANDLE _hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, _id);
    MODULEENTRY32W _module_entry;
    _module_entry.dwSize = sizeof(MODULEENTRY32W);
    bool bResult = Module32FirstW(_hSnapShot, &_module_entry);
    int _error = 0;
    if (!bResult) _error = GetLastError();
    while (bResult)
    {
        _model->addRow(_module_entry);
        bResult = Module32NextW(_hSnapShot, &_module_entry);
    }
//////////////////////////////////////////////////////////////////////////

    _model->update();
    ui.tableView->setModel(_model);
    ui.tableView->resizeColumnsToContents();
    ui.tableView->resizeRowsToContents();
}

ModuleDialog::~ModuleDialog(void)
{

}
