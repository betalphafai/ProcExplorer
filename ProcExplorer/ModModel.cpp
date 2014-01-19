
#include "ModModel.h"

ModModel::ModModel(QObject *_parent)
    : QAbstractItemModel(_parent),
    data_(new QList<ModuleEntry>),
    data_priv_(new ModuleInfoPriv(data_))
{
    header_ << CH_STRING("模块名")
        << CH_STRING("基地址")
        << CH_STRING("路径");
}

ModModel::~ModModel()
{
    
}

int ModModel::columnCount(
    const QModelIndex &parent /*= QModelIndex()*/ ) const
{
    return header_.size();
}

int ModModel::rowCount(
    const QModelIndex & parent /*= QModelIndex()*/ ) const
{
    return data_priv_->size();
}

QVariant ModModel::data(
    const QModelIndex &index, int role /*= Qt::DisplayRole*/ ) const
{
    if (!index.isValid()) return QVariant();
    if (role != Qt::DisplayRole) return QVariant();

    ModuleInfoEntry *_item =
        static_cast<ModuleInfoEntry *>(index.internalPointer());
    
    switch (index.column())
    {
    case NAME:      return _item->name_;
    case BASE_ADDR: return toAddr(_item->base_addr_);
    case PATH:      return _item->path_;
    default:        return QVariant();
    }
}

QModelIndex ModModel::parent( const QModelIndex & index ) const
{
    return QModelIndex();
}

QModelIndex ModModel::index( int row, int column,
    const QModelIndex &parent /*= QModelIndex()*/ ) const
{
    if (row < 0 || row >= rowCount()) return QModelIndex();
    if (column < 0 || column >= columnCount()) return QModelIndex();

    ModuleInfoEntry *_item = data_priv_->index(row);
    if (_item != NULL)
    {
        return createIndex(row, column, data_priv_->index(row));
    }
    return QModelIndex();
}

QVariant ModModel::headerData( int section,
    Qt::Orientation orientation, int role /* = Qt::DisplayRole */ ) const
{
    if (section < 0 | section >= columnCount()) return QVariant();
    if (orientation == Qt::Vertical) return QVariant();
    if (role != Qt::DisplayRole) return QVariant();

    switch (section)
    {
    case NAME:      return header_[NAME];
    case BASE_ADDR: return header_[BASE_ADDR];
    case PATH:      return header_[PATH];
    default:        return QVariant();
    }
}

void ModModel::update( void )
{
    beginResetModel();
    data_priv_->update();
    endResetModel();
}

bool ModModel::addRow( const ModuleEntry &_item )
{
    bool _result = true;
    for (int i = 0; i < data_->size(); ++i)
    {
        if (data_->at(i).szModule == _item.szModule)
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

bool ModModel::updateRow( const ModuleEntry &_item )
{
    if (/*_item == NULL || */data_->size() == 0) return false;

    bool _result = false;
    for (int i = 0; i < data_->size(); ++i)
    {
        if (data_->at(i).szModule == _item.szModule)
        {
            _result = true;
            data_->replace(i, _item);
            break;
        }
    }

    return _result;
}

bool ModModel::removeRow( const ModuleEntry &_item )
{
    if (/*_item == NULL || */data_->size() == 0) return false;

    bool _result = true;
    int _index = -1;
    for (int i = 0; i < data_->size(); ++i)
    {
        if (data_->at(i).szModule == _item.szModule)
        {
            _index = i;
            break;
        }
    }
    if (_index != -1) data_->removeAt(_index);
    else _result = false;

    return _result;
}

// 把地址转接成QString
// 参数_addr：指向的地址
// 返回：返回地址的QString
QString ModModel::toAddr( unsigned char *_addr ) const
{
    QString _text = QString::number((unsigned int)_addr, 16).toUpper();
    // 不足8位的在前补0
    while (_text.size() != 8)
    {
        _text.push_front('0');
    }

    // 在最前面补0x
    _text.push_front("0x");
    return _text;
}
