#include "processinmemmodel.h"
#include <QColor>
const unsigned int ProcessInMemModel::itemCount = 6;

ProcessInMemModel::ProcessInMemModel(QObject *parent):QAbstractTableModel(parent)
{
}

int ProcessInMemModel::rowCount(const QModelIndex &/*index*/) const
{
    return plist.size();
}

int ProcessInMemModel::columnCount(const QModelIndex &/*index*/) const
{
    return itemCount;
}

QVariant ProcessInMemModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if(role == Qt::TextAlignmentRole)
        return int(Qt::AlignCenter | Qt::AlignRight);
    else if(role == Qt::BackgroundRole)
        if(index.column() == 2 && index.row() == 0)
            return QColor(Qt::red);
        else
            return QColor();
    else if(role == Qt::DisplayRole)
        return at(index);
    return QVariant();
}

QString ProcessInMemModel::at(const QModelIndex &index) const
{
    unsigned int row = index.row();
    unsigned int column = index.column();

    if(row > plist.size() || column > itemCount)
        return "";
    PCB temp = plist[row];
    switch (column) {
    case 0:
        return QString(temp.name->c_str());
    case 1:
        return QString("%1").arg(temp.stime);
    case 2:
        return QString("%1").arg(temp.rtime);
    case 3:
        return QString("%1").arg(temp.memory);
    case 4:
        return QString("%1").arg(temp.tapeNum);
    case 5:
        return QString("%1").arg(temp.pid);
    default:
        return "";
    }
}

QVariant ProcessInMemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole || orientation == 0x02)
        return QVariant();
    switch (section) {
    case 0:
        return "作业名";
    case 1:
        return "到达时间";
    case 2:
        return "运行时间";
    case 3:
        return "内存需要";
    case 4:
        return "磁带机需要";
    case 5:
        return "优先级";
    default:
        return "";
    }
}

void ProcessInMemModel::updateView(void)
{
    endResetModel();
}


