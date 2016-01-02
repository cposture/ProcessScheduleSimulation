#ifndef PROCESSFINMODEL_H
#define PROCESSFINMODEL_H
#include "process.h"
#include <QAbstractTableModel>

class ProcessFinModel:public QAbstractTableModel
{
public:
    static const unsigned int itemCount;

    ProcessFinModel(QObject *parent = 0);
    int rowCount(const QModelIndex &index) const;
    int columnCount(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void updateView(void);
    void setProcess(std::vector<PCB>* p);
private:
    std::vector<PCB> *fplist;
    QString at(const QModelIndex &index) const;
};

#endif // PROCESSFINMODEL_H
