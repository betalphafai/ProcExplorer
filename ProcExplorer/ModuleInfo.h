#ifndef MODULEINFO_H_
#define MODULEINFO_H_

#include <QObject>

class ModuleInfo
    : public QObject
{
    Q_OBJECT

 public:
    ModuleInfo(QObject *_parent = NULL);
    ModuleInfo(QString _name, QString _path,
        unsigned int _base_addr, QObject *_parent = NULL);
    ~ModuleInfo(void);

 public:
    QString name_;
    QString path_;
    QString base_addr_;
};

#endif // MODULEINFO_H_