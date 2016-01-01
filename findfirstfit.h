#ifndef FINDFIRSTFIT_H
#define FINDFIRSTFIT_H
#include "memfit.h"
#include "dynamicallocation.h"

class FindFirstFit:public MemFit
{
public:
    FindFirstFit(){}
    ~FindFirstFit(){}
    void *find_fit(Memory &mem, unsigned int size);

};

#endif // FINDFIRSTFIT_H
