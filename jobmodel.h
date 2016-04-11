/**
  ******************************************************************************
  * @file    jobmodel.h
  * @author  cposture
  * @version V1.0
  * @date    2015-1-3
  * @brief
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

#ifndef JOBMODEL_H
#define JOBMODEL_H
#include "process.h"
#include <QAbstractTableModel>

class JobModel:public QAbstractTableModel,public JobList
{
public:

    JobModel(QObject *parent = 0);
    void insertJob(const PCB &p);
    void updateView(void);
    int rowCount(const QModelIndex &index) const;
    int columnCount(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    static const int itemCount;

private:

    QString at(const QModelIndex &index) const;
};

#endif // JOBMODEL_H
