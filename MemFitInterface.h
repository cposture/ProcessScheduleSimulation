/**
  ******************************************************************************
  * @file    MemFitInterface.h
  * @author  cposture
  * @version V1.0
  * @date    2015-1-3
  * @brief   内存空闲块查找接口
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

#ifndef MEMFIX_H
#define MEMFIX_H

class Memory;

class MemFitInterface
{
public:
    virtual void *find_fit(Memory &/*mem*/, unsigned int /*size*/) = 0;
};

#endif // MEMFIX_H
