#include <QColor>
#include "resourcemodel.h"
#include "dynamicallocation.h"

const int ResourceModel::row = 1;
const int ResourceModel::column = 3;
ResourceModel::ResourceModel(QObject *parent):QAbstractTableModel(parent)
{
    time = 0;
}

int ResourceModel::rowCount(const QModelIndex &/*index*/) const
{
    return row;
}

int ResourceModel::columnCount(const QModelIndex &/*index*/) const
{
    return column;
}

QVariant ResourceModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if(role == Qt::TextAlignmentRole)
        return int(Qt::AlignCenter | Qt::AlignRight);
    else if(role == Qt::BackgroundRole)
        if(index.row() == 0 && index.column() == 0)
            return QColor(0x3d8ec9);
        else
            return QColor(0X302F2F);
    else if(role == Qt::DisplayRole)
        return at(index);
    return QVariant();
}

QVariant ResourceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == 0x02 || role != Qt::DisplayRole)
        return QVariant();
    switch (section) {
    case 0:
        return "系统时间";
    case 1:
        return "内存剩余量";
    case 2:
        return "磁带机剩余量";
    default:
        return QVariant();
    }
}

QString ResourceModel::at(const QModelIndex &index) const
{
    if(!index.isValid())
        return "";

    switch(index.column())
    {
    case 0:
        return QString("%1").arg(time);
    case 1:
        return QString("%1").arg(Memory::getInstance()->unusedMem);
    case 2:
        return QString("%1").arg(tape.getUnusedNum());
    default:
        return "";
    }
}

void ResourceModel::updateView(void)
{
    endResetModel();
}
