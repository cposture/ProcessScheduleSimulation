#ifndef PROCESSINMEMMODEL_H
#define PROCESSINMEMMODEL_H
#include "process.h"
#include <QAbstractTableModel>

class ProcessInMemModel:public QAbstractTableModel,public ProcessList
{
public:
    static const unsigned int itemCount;

    ProcessInMemModel(QObject *parent = 0);
    int rowCount(const QModelIndex &index) const;
    int columnCount(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void updateView(void);


private:
    QString at(const QModelIndex &index) const;
};

#endif // PROCESSINMEMMODEL_H
