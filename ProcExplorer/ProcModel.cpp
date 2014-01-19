
#include <QtAlgorithms>

#include "ProcModel.h"

ProcModel::ProcModel(QObject *parent, ProcessInfoPriv *_data)
    : QAbstractItemModel(parent),
    data_(new QList<ProcessEntry>()),
    data_priv_(new ProcessInfoPriv(data_))
{
    header_ << CH_STRING("进程名")
        << CH_STRING("进程id")
        << CH_STRING("父进程id")
        << CH_STRING("线程数")
        << CH_STRING("基本优先级")
        << CH_STRING("路径");
}

ProcModel::~ProcModel()
{

}

int ProcModel::columnCount(
    const QModelIndex &parent /*= QModelIndex()*/ ) const
{
    return header_.size();
}

int ProcModel::rowCount(
    const QModelIndex &parent /*= QModelIndex()*/ ) const
{
    return data_priv_->size();
}

QVariant ProcModel::data(
    const QModelIndex &index, int role /*= Qt::DisplayRole*/ ) const
{
    // 判断下标是否合法
    if (!index.isValid()) return QVariant();
    if (role != Qt::DisplayRole) return QVariant();

    ProcessInfoEntry *_item =
        static_cast<ProcessInfoEntry *>(index.internalPointer());

    switch (index.column())
    {
    case PROC_NAME:         return _item->proc_name_;
    case PROC_ID:           return _item->proc_id_;
    case THREAD_COUNT:      return _item->thread_count_;
    case PRIORITY_BASE:     return proc_base_pri(_item->priority_base_);
    case PARENT_PROC_ID:    return _item->parent_proc_id_;
    case PROC_PATH:         return _item->proc_path_;
    case USAGE_COUNT:       return _item->usage_count_;
    case BLOCK_SIZE:        return _item->block_size_;
    default:                break;
    }
    return QVariant();
}

QModelIndex ProcModel::index(
    int row, int column, const QModelIndex &parent /*= QModelIndex()*/ ) const
{
    // 判断下标是否合法
    if (row < 0 || row >= rowCount()) return QModelIndex();
    if (column < 0 || column >= columnCount()) return QModelIndex();

    ProcessInfoEntry *_item = data_priv_->index(row);

    if (_item)
    {
        return createIndex(row, column, data_priv_->index(row));
    }
    else
    {
        return QModelIndex();
    }
}

QModelIndex ProcModel::parent(
    const QModelIndex &index ) const
{
    return QModelIndex();
}

void ProcModel::update(void)
{
    beginResetModel();
    data_priv_->update_data();
    endResetModel();
}

QVariant ProcModel::headerData(
    int section,
    Qt::Orientation orientation,
    int role /*= Qt::DisplayRole*/) const
{
    // 只有行表头，没有列表头
    if (orientation == Qt::Vertical) return QVariant();
    if (role != Qt::DisplayRole) return QVariant();
    if (section > columnCount()) return QVariant();

    switch (section)
    {
    case PROC_NAME:         return header_[PROC_NAME];
    case PROC_ID:           return header_[PROC_ID];
    case THREAD_COUNT:      return header_[THREAD_COUNT];
    case PRIORITY_BASE:     return header_[PRIORITY_BASE];
    case PARENT_PROC_ID:    return header_[PARENT_PROC_ID];
    case PROC_PATH:         return header_[PROC_PATH];
    case USAGE_COUNT:       return header_[USAGE_COUNT];
    case BLOCK_SIZE:        return header_[BLOCK_SIZE];
    default:                return QVariant();
    }
}

bool ProcModel::addRow( const ProcessEntry &_item )
{
//     if (_item == NULL) return false;

    bool _result = true;
    for (int i = 0; i < data_->size(); ++i)
    {
        if (data_->at(i).th32ProcessID == _item.th32ProcessID)
        {
            _result = false;
            break;
        }
    }
    if (_result)
    {
        data_->append(_item);
    }
    return _result;
}

bool ProcModel::removeRow( const ProcessEntry &_item )
{
    if (/*_item == NULL || */data_->size() == 0) return false;

    bool _result = true;
    int _index = -1;
    for (int i = 0; i < data_->size(); ++i)
    {
        if (data_->at(i).th32ProcessID == _item.th32ProcessID)
        {
            _index = i;
            break;
        }
    }
    if (_index != -1) data_->removeAt(_index);
    else _result = false;

    return _result;
}

bool ProcModel::updateRow( const ProcessEntry &_item )
{
    if (/*_item == NULL || */data_->size() == 0) return false;

    bool _result = false;
    for (int i = 0; i < data_->size(); ++i)
    {
        if (data_->at(i).th32ProcessID == _item.th32ProcessID)
        {
            _result = true;
            data_->replace(i, _item);
            break;
        }
    }

    return _result;
}

bool ProcModel::removeRows( int row, int count,
    const QModelIndex &parent /* = QModelIndex */ )
{
    if (row < 0 || row >= rowCount()) return false;
    if (count <= 0 || count > rowCount()) return false;

    if (count == rowCount())
    {
        data_->clear();
    }
    else
    {
        for (int i = row + count - 1; i >= row; --i)
        {
            data_->removeAt(i);
        }
    }

    data_priv_->update_data();
    return true;
}

// 获取进程的基本优先级
// 参数_proc_id：进程的基本优先级
// 返回：进程的基本优先级名称
QString ProcModel::proc_base_pri(long _base_pri) const
{
    if (_base_pri == 0L) return QString("N/A");
    else if (_base_pri == 6L) return QString("Below Normal");
    else if (_base_pri == 8L) return QString("Normal");
    else if (_base_pri == 9L) return QString("Normal");
    else if (_base_pri == 11L) return QString("Normal");
    else if (_base_pri == 13L) return QString("High");
    else return QString::number(_base_pri);
}