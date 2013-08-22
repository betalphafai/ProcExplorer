

#include <QStandardItem>
#include <QString>
#include <QList>
#include <QTimer>

#include "ProcExplorerMain.h"

ProcExplorerMain::ProcExplorerMain(QWidget *_parent, Qt::WFlags _flags)
    : QMainWindow(_parent, _flags)
{
    ui_.setupUi(this);

    QStandardItemModel *_model = new QStandardItemModel(this);
    QStringList _header;
    _header << "process name" << "process id" << "parent process id "
        << "module id" << "thread id" << "path" << "usage";
    _model->setHorizontalHeaderLabels(_header);
    ui_.tableView_proc_->setModel(_model);
    ui_.tableView_proc_->verticalHeader()->setHidden(true);
    ui_.tableView_proc_->setEditTriggers(QAbstractItemView::NoEditTriggers);
//     ui_.tableView_proc_->setSortingEnabled(true);

    // create a timer to reflush the process id every second
    QTimer *_timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(read_the_process()));
    _timer->start(1000);
}

ProcExplorerMain::~ProcExplorerMain(void)
{

}

void ProcExplorerMain::read_the_process(void)
{
    // get the model
    QStandardItemModel *_model = (QStandardItemModel *)ui_.tableView_proc_->model();
    // remove the model row
    _model->removeRows(0, _model->rowCount());

    // read the process info
    int _proc_id_count = 0;
    PROCESSENTRY32 _proc_entry;
    _proc_entry.dwSize = sizeof(_proc_entry);
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    bool bResult = Process32First(hSnapShot, &_proc_entry);
    while (bResult)
    {
        QList<QStandardItem *> _list;
        QString _exe_file = QString::fromUtf16(_proc_entry.szExeFile);
        _list.append(new QStandardItem(_exe_file));
        _list.append(new QStandardItem(
            QString::number(_proc_entry.th32ProcessID)));
        _list.append(new QStandardItem(
            QString::number(_proc_entry.th32ParentProcessID)));
        _list.append(new QStandardItem(
            QString::number(_proc_entry.th32ModuleID)));
        _list.append(new QStandardItem(
            QString::number(_proc_entry.cntThreads)));

        // get the prcess's exe path
        QString _path;
        int _result = get_proc_exec_path(_proc_entry.th32ProcessID, &_path);
        if (_result == 0)
        {
            _list.append(new QStandardItem(_path));
        }
        else if (_result == 5)
        {
            _list.append(new QStandardItem("Access is denied. "));
        }
        else
        {
            _list.append(new QStandardItem(""));
        }

        _list.append(new QStandardItem(QString::number(_proc_entry.cntUsage)));
        _model->appendRow(_list);

        bResult = Process32Next(hSnapShot, &_proc_entry);
        ++_proc_id_count;
    }

    // show the processes num
    ui_.label_proc_num_->setText(
        "Processes number:" + QString::number(_proc_id_count));
}

int ProcExplorerMain::get_proc_exec_path(DWORD _proc_id, QString *_exe_path)
{
    // get the process handle
    HANDLE _hProcess = OpenProcess(
        PROCESS_QUERY_INFORMATION | PROCESS_VM_READ , FALSE, _proc_id);
    if (_hProcess == 0)
    {
        return GetLastError();
    }

    // get the handle's name
    LPWSTR _path = (LPWSTR)malloc(sizeof(WCHAR) * 512);
    if (GetModuleFileNameEx(_hProcess, NULL, _path, 512) != 0)
    {
        _exe_path->append(QString::fromUtf16(_path));
    }
    return 0;
}