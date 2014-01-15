#ifndef PROCMODEL_H
#define PROCMODEL_H


#include <QAbstractItemModel>
#include <QVariant>
#include <QModelIndex>
#include <QString>
#include <QStringList>
#include <QList>

#include "GlobalSupport.h"


// 进程结构体
struct ProcessInfoEntry
{
    QString proc_name_;
    QString proc_id_;
    QString thread_count_;
    long priority_base_;
    QString parent_proc_id_;
    QString proc_path_;
    QString usage_count_;
    QString block_size_;

    ProcessInfoEntry(void) {};
    ProcessInfoEntry(const ProcessInfoEntry &_rhs):
        proc_name_(_rhs.proc_name_),
        proc_id_(_rhs.proc_id_),
        thread_count_(_rhs.thread_count_),
        priority_base_(_rhs.priority_base_),
        parent_proc_id_(_rhs.parent_proc_id_),
        proc_path_(_rhs.proc_path_),
        usage_count_(_rhs.usage_count_),
        block_size_(_rhs.block_size_) {};
    ProcessInfoEntry(
        const QString &_proc_name,
        const QString &_proc_id,
        const QString &_thread_count,
        long _priority_base,
        const QString &_parent_proc_id,
        const QString &_proc_path,
        const QString &_usage_count,
        const QString &_block_size) :
            proc_name_(_proc_name),
            proc_id_(_proc_id),
            thread_count_(_thread_count),
            priority_base_(_priority_base),
            parent_proc_id_(_parent_proc_id),
            proc_path_(_proc_path),
            usage_count_(_usage_count),
            block_size_(_block_size) {};
    // ~ProcessInfoEntry() {};
};

struct ProcessInfoPriv
{
    // 原始数据的指针
    QList<ProcessEntry> *data_;
    // 处理原始数据后的数据
    QList<ProcessInfoEntry> cached_data_;

    ProcessInfoPriv(void) {};
    explicit ProcessInfoPriv(QList<ProcessEntry> *_data) :
        data_(_data), cached_data_(QList<ProcessInfoEntry>())
        {};
    // ~ProcessInfoPriv(void);

    // 根据原始数据data_，刷新cached_data_
    void update_data(void)
    {
        cached_data_.clear();
        for (int i = 0; i < data_->size(); ++i)
        {
            cached_data_.append(
                ProcessInfoEntry(
                    QString::fromUtf16(data_->at(i).szExeFile),
                    QString::number(data_->at(i).th32ProcessID),
                    QString::number(data_->at(i).cntThreads),
                    data_->at(i).pcPriClassBase,
                    QString::number(data_->at(i).th32ParentProcessID),
                    get_proc_exec_path(data_->at(i).th32ProcessID),
                    QString::number(data_->at(i).cntUsage),
                    QString::number(0)));
        }
    }

    // 获取数据长度
    // 返回：获取数据长度
    int size(void) const { return cached_data_.size(); }

    // 获取指定下标的数据
    // 参数_index：下标
    // 返回：进程结构的ProcessInfoEntry指针
    ProcessInfoEntry *index(int _index)
    {
        // if (cached_data_ == NULL) return NULL;
        if (_index < 0 || _index >= cached_data_.size()) return NULL;
        else return &cached_data_[_index];
    } const

    // 删除指定下标的数据
    // 参数_index：下标
    void delete_item(int _index)
    {
        cached_data_.removeAt(_index);
    }

    // 根据进程id，获取进程的完整路径
    // 参数_proc_id：进程id
    // 返回：进程的完整路径
    QString get_proc_exec_path(DWORD _proc_id)
    {
        // Get the process handle
        HANDLE _hProcess = OpenProcess(
            PROCESS_QUERY_INFORMATION | PROCESS_VM_READ , FALSE, _proc_id);

        // Can't open the Process
        if (_hProcess == NULL)
        {
            if (GetLastError() == 5) return QString("Access is denied.");
            else return QString("Unknown Error.");
        }

        // Get the handle's name
        QString _text;
        wchar_t _path[512] = { L'\0' };
        if (GetModuleFileNameEx(_hProcess, NULL, (LPWSTR)&_path, 512) != 0)
        {
            _text = QString::fromUtf16(_path);
        }

        CloseHandle(_hProcess);
        return _text;
    }
};

class ProcModel : public QAbstractItemModel
{
    Q_OBJECT

 public:
    // 列数据下标
    enum ColumnIndex
    {
        PROC_NAME = 0,
        PROC_ID,
        PARENT_PROC_ID,
        THREAD_COUNT,
        PRIORITY_BASE,
        PROC_PATH,
        USAGE_COUNT,
        BLOCK_SIZE
    };

 public:
    ProcModel(QObject *parent, ProcessInfoPriv *_data = NULL);
    ~ProcModel();

//////////////////////////////////////////////////////////////////////////
    // 继承QAbstractItemModel时，必须重载的函数
 public:
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QModelIndex index(
        int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex & index) const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
//////////////////////////////////////////////////////////////////////////

    // 重载获取表头的函数
    QVariant headerData(
        int section,
        Qt::Orientation orientation,
        int role = Qt::DisplayRole) const;
    // 重载删除行的函数
    bool removeRows(int row, int count,
        const QModelIndex &parent = QModelIndex());
 public:
    // 对表数据进行操作的函数
    void update(void);
    bool addRow(const ProcessEntry &_item);
    bool updateRow(const ProcessEntry &_item);
    bool removeRow(const ProcessEntry &_item);

 private:
    // 获取进程的基本优先级
    // 参数_proc_id：进程的基本优先级
    // 返回：进程的基本优先级名称
    QString proc_base_pri(long _base_pri) const;

 private:
    // 原始数据指针
    QList<ProcessEntry> *data_;
    // 表头数据
    QStringList header_;
    // 数据
    ProcessInfoPriv *data_priv_;
};

#endif // PROCMODEL_H
