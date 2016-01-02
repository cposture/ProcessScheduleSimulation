#ifndef PROCESS_H
#define PROCESS_H

#include <list>
#include <string>
#include <iostream>
#include <cstring>
#include <vector>
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

class ProcessList{
public:
    std::vector<PCB> plist;
    std::vector<PCB> fplist;
    bool schedule(PCB& p);
    void insertJobF(const PCB &t);
    void insertJobB(const PCB &t);
    bool hasProcess(void) const;
};

class JobList
{
public:
    bool schedule(ProcessList& p,unsigned int time);
    void init(std::istream &in,Tape &tape);
    bool hasJob(void) const;

protected:
    std::vector<PCB> jlist;
};

class Resource
{
public:

    int getTime(void) const;
    void incTime(void);
    Tape tape;
protected:
    unsigned int getUnusedMem(void);
    int time;
};

#endif

