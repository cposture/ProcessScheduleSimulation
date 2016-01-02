#ifndef MEMFIX_H
#define MEMFIX_H

class Memory;

class MemFit
{
public:
    MemFit(){}
    virtual ~MemFit(){}
    virtual void *find_fit(Memory &/*mem*/, unsigned int /*size*/)
    {
        return 0;
    }
};

#endif // MEMFIX_H
