
#include "ModuleInfo.h"

ModuleInfo::ModuleInfo(QObject *_parent)
    : QObject(_parent)
{

}

ModuleInfo::ModuleInfo(
    QString _name, QString _path, unsigned int _base_addr, QObject *_parent)
    : QObject(_parent)
{
    name_ = QString(_name);
    path_ = QString(_path);
    base_addr_ = QString::number(_base_addr, 16).toUpper();
}

ModuleInfo::~ModuleInfo()
{

}