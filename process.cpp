#include "process.h"

Memory mem = Memory::getInstance();
int JobList::itemCount = 6;

JobList::JobList(QObject *parent):QAbstractTableModel(parent)
{
}

int JobList::rowCount(const QModelIndex &/* index */) const
{
    return jlist.size();
}

int JobList::columnCount(const QModelIndex &/*index*/) const
{
    return itemCount;
}

QString JobList::at(const QModelIndex &index) const
{
    int row = index.row();
    int column = index.column();

    if(row > jlist.size() || column > itemCount)
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
QVariant JobList::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if(role == Qt::TextAlignmentRole)
        return int(Qt::AlignCenter | Qt::AlignRight);
    else if(role == Qt::DisplayRole)
        return at(index);
    return QVariant();
}

QVariant JobList::headerData(int section, Qt::Orientation orientation, int role) const
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

bool JobList::schedule(ProcessList &p,unsigned int time)
{
    if(hasJob() == false)
        return false;
    auto min = jlist.begin();
    bool sign = false;
    for(auto i = min; i != jlist.end(); ++i)
    {
        unsigned int m = mem.getUnusedMem();
        if(i->stime <= time  && i->memory <= m) /* select proper job */
        {
            if(i->ntime <= min->ntime || min->memory > m)
            {
                min = i;
                sign = true;
            }
        }
    }

    if(!sign)
        return false;

    if(NULL == ( min->memAddr = mem.mm_malloc(min->memory)))
        return false;

    p.insertJobB(*min);
    jlist.erase(min);
    endResetModel(); /* update view */
    return true;
}

void JobList::init(std::istream &in, Tape &tape)
{
    PCB t(&tape);
    while(!(in.eof() || in.bad() || in.fail()))
    {
        in >> *(t.name) >> t.pid >> t.stime >> t.ntime >> t.memory >> t.tapeNum >> t.super;
        jlist.push_back(t);
    }
    auto i = jlist.end();
    i--;
    jlist.erase(i);
}

bool JobList::hasJob(void) const
{
    return (bool)(jlist.size());
}

int ProcessList::itemCount = 6;

ProcessList::ProcessList(QObject *parent):QAbstractTableModel(parent)
{
}

int ProcessList::rowCount(const QModelIndex &/*index*/) const
{
    return plist.size();
}

int ProcessList::columnCount(const QModelIndex &index) const
{
    return itemCount;
}

QVariant ProcessList::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if(role == Qt::TextAlignmentRole)
        return int(Qt::AlignCenter | Qt::AlignRight);
    else if(role == Qt::DisplayRole)
        return at(index);
    return QVariant();
}

QString ProcessList::at(const QModelIndex &index) const
{
    int row = index.row();
    int column = index.column();

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

QVariant ProcessList::headerData(int section, Qt::Orientation orientation, int role) const
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
void ProcessList::insertJobF(const PCB &t)
{
    plist.insert(plist.begin(),t);
}

void ProcessList::insertJobB(const PCB &t)
{
    plist.push_back(t);
}

bool ProcessList::hasProcess(void) const
{
    return (bool)(plist.size());
}

bool ProcessList::schedule(PCB& p)
{
    if(hasProcess() == false)
        return false;
    auto min = plist.begin();
    bool sign = false;
    for(auto i = min; i != plist.end(); ++i)
        if(i->state == WAIT && i->super <= min->super)
        {
            min = i;
            sign = true;
        }
    if(!sign)
        return false;

    if(min->tapeNum && !(min->tape->request(min->tapeNum,min->pid)))
        min = plist.begin();
    else
        min->tapeNum = 0;

    p = *min;
    plist.erase(min);
    endResetModel(); /* update view */
    return true;
}

bool PCB::running(ProcessList &p,unsigned int time)
{
    rtime++;
    if(rtime == ntime)
    {
        ftime = time;
        turntime = time - stime;
        wtime = (float)(turntime)/ntime;
        state = FINISH;
        tape->release(pid);
        mem.mm_free(memAddr);
    }
    else
    {
        p.insertJobF(*this);
        state = WAIT;

    }
    return true;
}

void Tape::init(unsigned int n)
{
    unusedNum = n;
    num = n;
    for(unsigned int i = 0; i < n ; ++i)
        id.push_back(0);
}

bool Tape::request(unsigned int n, unsigned int pid)
{
    if(unusedNum < n || pid == 0)
        return false;
    for(unsigned int i = 0, j = 0; i < num && j < n; ++i)
    {
        if(id[i] == 0)
        {
            id[i] = pid;
            j++;
        }
    }
    unusedNum-=n;
    return true;
}

bool Tape::release(unsigned int pid)
{
    bool rtn = false;
    for(unsigned int i = 0; i < num; ++i)
    {
        if(id[i] == pid)
        {
            id[i] = 0;
            rtn = true;
            unusedNum++;
        }
    }
    return rtn;
}

unsigned int Tape::getUnusedNum(void) const
{
    return unusedNum;
}

int Resource::row = 1;
int Resource::column = 3;
Resource::Resource(QObject *parent):QAbstractTableModel(parent),time(0)
{
}

int Resource::rowCount(const QModelIndex &index) const
{
    return row;
}

int Resource::columnCount(const QModelIndex &index) const
{
    return column;
}

QVariant Resource::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if(role == Qt::TextAlignmentRole)
        return int(Qt::AlignCenter | Qt::AlignRight);
    else if(role == Qt::DisplayRole)
        return at(index);
    return QVariant();
}

QVariant Resource::headerData(int section, Qt::Orientation orientation, int role) const
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

QString Resource::at(const QModelIndex &index) const
{
    if(!index.isValid())
        return "";

    switch(index.column())
    {
    case 0:
        return QString("%1").arg(time);
    case 1:
        return QString("%1").arg(mem.unusedMem);
    case 2:
        return QString("%1").arg(tape.getUnusedNum());
    default:
        return "";
    }
}

void Resource::incTime(void)
{
    time++;
    endResetModel(); /* update view */
}

int Resource::getTime(void) const
{
    return time;
}

int FreememBlock::column = 3;

FreememBlock::FreememBlock(QObject *parent):QAbstractTableModel(parent)
{
}

int FreememBlock::rowCount(const QModelIndex &index) const
{
    return 1;
}

int FreememBlock::columnCount(const QModelIndex &index) const
{
    return column;
}

QVariant FreememBlock::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if(role == Qt::TextAlignmentRole)
        return int(Qt::AlignCenter | Qt::AlignRight);
    else if(role == Qt::DisplayRole)
        return at(index);
    return QVariant();
}

QVariant FreememBlock::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == 0x02)
        return QVariant();
    switch(section)
    {
    case 0:
        return "编号";
    case 1:
        return "区间";
    case 2:
        return "状态";
    default:
        return QVariant();
    }
}

QString FreememBlock::at(const QModelIndex &index) const
{
    if(!index.isValid())
        return "";
    int row = index.row();
    int column = index.column();

    for(int i = 0; i < row; ++i)
    {

    }
}
