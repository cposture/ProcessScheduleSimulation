/**
  ******************************************************************************
  * @file    findfirst.h
  * @author  cposture
  * @version V1.0
  * @date    2015-1-3
  * @brief   内存空闲块查找
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

#ifndef FINDFIRSTFIT_H
#define FINDFIRSTFIT_H

#include "MemFitInterface.h"
#include "dynamicallocation.h"

class FindFirstFit:public MemFitInterface
{
public:
    FindFirstFit(){}
    ~FindFirstFit(){}
    void *find_fit(Memory &mem, unsigned int size);
};

#endif // FINDFIRSTFIT_H
