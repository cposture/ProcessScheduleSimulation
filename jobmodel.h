#ifndef JOBMODEL_H
#define JOBMODEL_H
#include "process.h"
#include <QAbstractTableModel>

class JobModel:public QAbstractTableModel,public JobList
{
public:
    static const unsigned int itemCount;
    JobModel(QObject *parent = 0);
    int rowCount(const QModelIndex &index) const;
    int columnCount(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void updateView(void);

private:
    QString at(const QModelIndex &index) const;
};

#endif // JOBMODEL_H
