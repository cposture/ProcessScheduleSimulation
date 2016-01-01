#ifndef MEMFIX_H
#define MEMFIX_H

class Memory;

class MemFit
{
public:
    MemFit(){}
    virtual ~MemFit(){}
    virtual void *find_fit(Memory &mem, unsigned int size){}
};

#endif // MEMFIX_H
