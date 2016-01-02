#ifndef PROCESS_H
#define PROCESS_H

#include <list>
#include <string>
#include <iostream>
#include <cstring>
#include <vector>
#include <QAbstractTableModel>
#include "dynamicallocation.h"

extern Memory mem;
class ProcessList;
class Tape;

typedef enum{
    WAIT,READY,FINISH
}ProcessState;

class Tape{
private:
    unsigned int num;
    unsigned int unusedNum;
public:
    std::vector<unsigned int> id;
    void init(unsigned int n);
    unsigned int tapeNum(void);
    unsigned int tapeNum(unsigned int id);
    bool request(unsigned int n,unsigned int pid);
    bool release(unsigned int id);
    unsigned int getUnusedNum(void) const;
};
class PCB{
public:
    PCB(Tape *ptape = NULL,const std::string &pname = std::string("Unified Name"),int super=0,unsigned int ntime=0
            ,unsigned int stime=0,unsigned int tapeNum=0,unsigned int memory=0)
        :super(super),ntime(ntime),stime(stime),tapeNum(tapeNum),memory(memory)
    {
        name = new std::string(pname);
        state = WAIT;
        rtime = 0;
        turntime = 0;
        wtime = 0;
        ftime = 0;
        pid = 0;
        tape = ptape;
        memAddr = NULL;
    }
    PCB(const PCB& other)
    {
        std::memcpy(this, &other, sizeof(PCB));
        name = new std::string(*other.name);
    }
    PCB& operator=(const PCB& other)
    {
        std::memcpy(this, &other, sizeof(PCB));
        name = new std::string(*other.name);
        return *this;
    }
    ~PCB()
    {
        //delete name;
    }
    bool running(ProcessList &p, unsigned int time);
    std::string *name;
    char state;
    unsigned int pid;
    int super;
    unsigned int ntime;
    unsigned int rtime;
    unsigned int stime;
    unsigned int turntime;
    float wtime;
    unsigned int ftime; /* finish time*/
    unsigned int tapeNum; /* the amount of tape that process needs */
    unsigned int memory; /* the amount of memory */
    Tape *tape;
    void *memAddr;
};

class ProcessList:public QAbstractTableModel{
public:
    std::vector<PCB> plist;
    std::vector<PCB> fplist;
    bool schedule(PCB& p);
    void insertJobF(const PCB &t);
    void insertJobB(const PCB &t);
    bool hasProcess(void) const;

    ProcessList(QObject *parent = 0);
    int rowCount(const QModelIndex &index) const;
    int columnCount(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    //bool setData(const QModelIndex &index, const QVariant &value, int role);
    //Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    QString at(const QModelIndex &index) const;
public:
    static int itemCount;
};

class JobList:public QAbstractTableModel
{
public:
    bool schedule(ProcessList& p,unsigned int time);
    void init(std::istream &in,Tape &tape);
    bool hasJob(void) const;

    JobList(QObject *parent = 0);
    int rowCount(const QModelIndex &index) const;
    int columnCount(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    //bool setData(const QModelIndex &index, const QVariant &value, int role);
    //Qt::ItemFlags flags(const QModelIndex &index) const;
private:
    std::vector<PCB> jlist;
    QString at(const QModelIndex &index) const;
public:
    static int itemCount;
};

class Resource:public QAbstractTableModel
{
public:

    Resource(QObject *parent = 0);
    int rowCount(const QModelIndex &index) const;
    int columnCount(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    int getTime(void) const;
    void incTime(void);
    Tape tape;
private:
    unsigned int getUnusedMem(void);
    QString at(const QModelIndex &index) const;
    static int row,column;
    int time;
};

class FreememBlock:public QAbstractTableModel
{
public:
    FreememBlock(QObject *parent = 0);
    int rowCount(const QModelIndex &index) const;
    int columnCount(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
private:
    QString at(const QModelIndex &index) const;
    static int column;
};

#endif

