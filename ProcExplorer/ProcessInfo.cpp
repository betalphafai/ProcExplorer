
#include "ProcessInfo.h"

ProcessInfo::ProcessInfo(QObject *parent)
    : QObject(parent)
{

}

ProcessInfo::~ProcessInfo(void)
{

}

void ProcessInfo::init(PROCESSENTRY32 *_item)
{
    proc_name_ = QString::fromUtf16(_item->szExeFile);
    proc_id_ = QString::number(_item->th32ProcessID);
    thread_count_ = QString::number(_item->cntThreads);
    priority_base_ = QString::number(_item->pcPriClassBase);
    parent_proc_id_ = QString::number(_item->th32ParentProcessID);
    get_proc_exec_path(_item->th32ProcessID, &proc_path_);
    usage_count_ = QString::number(_item->cntUsage);
}

void ProcessInfo::init(HEAPENTRY32 *_item)
{
    block_size_ = QString::number(_item->dwBlockSize);
}

int ProcessInfo::get_proc_exec_path(DWORD _proc_id, QString *_exe_path)
{
    // get the process handle
    HANDLE _hProcess = OpenProcess(
        PROCESS_QUERY_INFORMATION | PROCESS_VM_READ , FALSE, _proc_id);
    if (_hProcess == NULL)
    {
        if (GetLastError() == 5)
        {
            _exe_path->append("Access is denied.");
        }
    }

    // get the handle's name
    LPWSTR _path = (LPWSTR)malloc(sizeof(WCHAR) * 512);
    if (GetModuleFileNameEx(_hProcess, NULL, _path, 512) != 0)
    {
        _exe_path->append(QString::fromUtf16(_path));
    }
    return 0;
}

QList<QStandardItem *> ProcessInfo::toStandardItemList(void)
{
//     << "process name" << "process id" << "parent process id "
//     << "thread id" << "path" << "usage" << "block size";
    QList<QStandardItem *> _list;
    _list << new QStandardItem(proc_name_)
        << new QStandardItem(proc_id_)
        << new QStandardItem(parent_proc_id_)
        << new QStandardItem(thread_count_)
        << new QStandardItem(proc_path_)
        << new QStandardItem(usage_count_)
        << new QStandardItem(block_size_);
    return _list;
}

void ProcessInfo::set_block_size(unsigned long _size)
{
    block_size_ = QString::number(_size);
}

int ProcessInfo::get_proc_id(void)
{
    return proc_id_.toInt();
}
