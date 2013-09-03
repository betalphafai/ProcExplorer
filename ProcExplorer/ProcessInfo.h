#ifndef PROCESSINFO_H_
#define PROCESSINFO_H_

#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

#include <QList>
#include <QStandardItem>

#include "ModuleInfo.h"

class ProcessInfo
    : public QObject
{
    Q_OBJECT

 public:
    ProcessInfo(QObject *parent = 0);
    ~ProcessInfo(void);
    void init(PROCESSENTRY32 *_item);
    void init(HEAPENTRY32 *_item);
    void set_block_size(unsigned long _size);
    int get_proc_id(void);
    void add_module(ModuleInfo *_item) { modules.push_back(_item); };
    inline QString get_proc_path(void) { return proc_path_; }
    QStandardItemModel *toStandardModuleItemList(void);
    QList<QStandardItem *> toStandardItemList(void);
 private:
    int get_proc_exec_path(DWORD _proc_id, QString *_exe_path);
    QString proc_name_;
    QString proc_id_;
    QString thread_count_;
    QString priority_base_;
    QString parent_proc_id_;
    QString proc_path_;
    QString usage_count_;
    QString block_size_;

    QList<ModuleInfo *> modules;
};

#endif // PROCESSINFO_H_
