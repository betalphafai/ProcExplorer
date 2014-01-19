#ifndef MODMODEL_H_
#define MODMODEL_H_

#include <QObject>
#include <QAbstractItemModel>
#include <QVariant>
#include <QModelIndex>
#include <QString>
#include <QStringList>
#include <QList>

#include "GlobalSupport.h"

struct ModuleInfoEntry
{
    QString name_;
    QString path_;
    unsigned char *base_addr_;
    ModuleInfoEntry(void) {};
    ModuleInfoEntry(QString _name, QString _path, unsigned char *_base_addr) :
        name_(_name), path_(_path), base_addr_(_base_addr) {};
    ModuleInfoEntry(const ModuleInfoEntry &_rhs) :
        name_(_rhs.name_), path_(_rhs.path_), base_addr_(_rhs.base_addr_) {};
    ~ModuleInfoEntry(){};
};

struct ModuleInfoPriv
{
    QList<ModuleEntry> *data_;
    QList<ModuleInfoEntry> cached_data_;
    explicit ModuleInfoPriv(QList<ModuleEntry> *_list):
        data_(_list) {};
    void update(void)
    {
        if (data_->size() == 0) return;
        for (int i = 0; i < data_->size(); ++i)
        {
            cached_data_.append(
                ModuleInfoEntry(
                    QString::fromWCharArray(data_->at(i).szModule),
                    QString::fromWCharArray(data_->at(i).szExePath),
                    data_->at(i).modBaseAddr));
        }
    }
    int size(void) const { return cached_data_.size(); }
    const ModuleInfoEntry *index(int _index) const
        { return &cached_data_[_index]; }
    ModuleInfoEntry *index(int _index) { return &cached_data_[_index]; }
    void remove(int _index) { cached_data_.removeAt(_index); }
};

class ModModel : public QAbstractItemModel
{
    Q_OBJECT

    enum ColumnIndex
    {
        NAME = 0,
        BASE_ADDR,
        PATH,
        VERSION,
        COMPANG
    };

 public:
    ModModel(QObject *_parent = NULL);
    ~ModModel(void);

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

public:
    // 重载headerData函数
    QVariant headerData(int section,
        Qt::Orientation orientation, int role /* = Qt::DisplayRole */) const;
 public:
    // 对表数据进行操作的函数
    void update(void);
    bool addRow(const ModuleEntry &_item);
    bool updateRow(const ModuleEntry &_item);
    bool removeRow(const ModuleEntry &_item);

    // 把地址转接成QString
    // 参数_addr：指向的地址
    // 返回：返回地址的QString
    QString toAddr(unsigned char *_addr) const;
public:
    QStringList header_;
    QList<ModuleEntry> *data_;
    ModuleInfoPriv *data_priv_;
};

#endif // MODMODEL_H_