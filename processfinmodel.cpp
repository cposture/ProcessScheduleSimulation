#include "processfinmodel.h"

const unsigned int ProcessFinModel::itemCount = 6;

ProcessFinModel::ProcessFinModel(QObject *parent):QAbstractTableModel(parent)
{
}

void ProcessFinModel::setProcess(std::vector<PCB> *p)
{
    fplist = p;
}

int ProcessFinModel::rowCount(const QModelIndex &/*index*/) const
{
    return fplist->size();
}

int ProcessFinModel::columnCount(const QModelIndex &/*index*/) const
{
    return itemCount;
}

QVariant ProcessFinModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if(role == Qt::TextAlignmentRole)
        return int(Qt::AlignCenter | Qt::AlignRight);
    else if(role == Qt::DisplayRole)
        return at(index);
    return QVariant();
}

QString ProcessFinModel::at(const QModelIndex &index) const
{
    unsigned int row = index.row();
    unsigned int column = index.column();

    if(row >= fplist->size() || column > itemCount)
        return "";
    PCB temp = (*fplist)[row];
    switch (column) {
    case 0:
        return QString(temp.name->c_str());
    case 1:
        return QString("%1").arg(temp.stime);
    case 2:
        return QString("%1").arg(temp.rtime);
    case 3:
        return QString("%1").arg(temp.ftime);
    case 4:
        return QString("%1").arg(temp.ftime - temp.stime + 1);
    case 5:
        return QString("%1").arg(temp.memory);
    case 6:
        return QString("%1").arg(temp.pid);
    default:
        return "";
    }
}

QVariant ProcessFinModel::headerData(int section, Qt::Orientation orientation, int role) const
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
        return "完成时间";
    case 4:
        return "周转时间";
    case 5:
        return "内存需要";
    case 6:
        return "优先级";
    default:
        return "";
    }
}

void ProcessFinModel::updateView(void)
{
    endResetModel();
}

