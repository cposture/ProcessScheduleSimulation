#include "process.h"

Memory mem = Memory::getInstance();
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
        p.fplist.push_back(*this);
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


void Resource::incTime(void)
{
    time++;
}

int Resource::getTime(void) const
{
    return time;
}
