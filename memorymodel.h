#ifndef MEMORYMODEL_H
#define MEMORYMODEL_H
#include <QAbstractTableModel>
#include "dynamicallocation.h"

class MemoryModel:public QAbstractTableModel, private Memory
{
public:
    MemoryModel(QObject *parent = 0);
    int rowCount(const QModelIndex &index) const;
    int columnCount(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
private:
    QString at(const QModelIndex &index) const;
};

#endif // MEMORYMODEL_H
