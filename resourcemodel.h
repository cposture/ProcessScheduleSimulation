#ifndef RESOURCEMODEL_H
#define RESOURCEMODEL_H
#include "process.h"
#include <QAbstractTableModel>

class ResourceModel:public QAbstractTableModel,public Resource
{
public:
    ResourceModel(QObject *parent = 0);
    int rowCount(const QModelIndex &index) const;
    int columnCount(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void updateView(void);
private:
    QString at(const QModelIndex &index) const;
    static const int row,column;

};

#endif // RESOURCEMODEL_H
