#include "jobmodel.h"

const unsigned int JobModel::itemCount = 6;

JobModel::JobModel(QObject *parent):QAbstractTableModel(parent)
{
}

int JobModel::rowCount(const QModelIndex &/* index */) const
{
    return jlist.size();
}

int JobModel::columnCount(const QModelIndex &/*index*/) const
{
    return itemCount;
}

QString JobModel::at(const QModelIndex &index) const
{
    unsigned int row = index.row();
    unsigned int column = index.column();

    if(row >= jlist.size() || column > itemCount)
        return "";
    PCB temp = jlist[row];

    switch (column) {
    case 0:
        return QString(temp.name->c_str());
    case 1:
        return QString("%1").arg(temp.stime);
    case 2:
        return QString("%1").arg(temp.ntime);
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
QVariant JobModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if(role == Qt::TextAlignmentRole)
        return int(Qt::AlignCenter | Qt::AlignRight);
    else if(role == Qt::DisplayRole)
        return at(index);
    return QVariant();
}

QVariant JobModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == 0x02)
        return QVariant();
    if(role != Qt::DisplayRole)
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
        return QVariant();
    }
}

void JobModel::updateView(void)
{
    endResetModel();
}
