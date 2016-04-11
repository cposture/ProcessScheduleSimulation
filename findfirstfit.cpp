/**
  ******************************************************************************
  * @file    findfirst.cpp
  * @author  cposture
  * @version V1.0
  * @date    2015-1-3
  * @brief   内存空闲块查找
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

#include "findfirstfit.h"
#include "dynamicallocation.h"

void *FindFirstFit::find_fit(Memory &mem,unsigned int size)
{
    unsigned int blocksize;
    char *bp;

    if(size <= 0)
        return 0;

    bp = mem.heap_listp;
    blocksize = mem.getSize(mem.getBlockHeader(bp));
    while(blocksize)
    {
        if(blocksize >= size && !mem.getAlloc(mem.getBlockHeader(bp)))
            return bp;
        bp = mem.getNextBlock(bp);
        blocksize = mem.getSize(mem.getBlockHeader(bp));
    }
    return 0;
}

