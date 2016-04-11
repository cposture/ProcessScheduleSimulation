/**
  ******************************************************************************
  * @file    dynamicallocation.cpp
  * @author  cposture
  * @version V1.0
  * @date    2015-1-3
  * @brief   动态内存类——分配、释放、初始化
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

#include "dynamicallocation.h"
#include <malloc.h>

#define Malloc malloc

Memory* volatile Memory::uniqueMemory = nullptr;
const unsigned int Memory::wsize = 4;
const unsigned int Memory::dsize = 8;
const unsigned int Memory::ChunkSize = 1 << 12;

Memory *Memory::getInstance(void)
{
    // mutex 必须放在函数内部，如果放在类声明里会造成错误
    static QMutex mutex;
    // 检查实例，如果不存在就进入同步区域
    if(uniqueMemory == nullptr)
    {
        mutex.lock();
        // 进入同步区域后，再检查一次，如果为nullptr才创建实例
        if(uniqueMemory == nullptr)
            uniqueMemory = new Memory();
        mutex.unlock();
    }
    return uniqueMemory;
}

void Memory::setMemfit(MemFitInterface *m)
{
    memfit = m;
}

/*
 *mem_init
 *
 * init the memory model
 *
 */
bool Memory::mem_init(void)
{
    if(Max_Heap <= 0)
        return false;
    mem_heap = (char*)malloc(Max_Heap);
    mem_brk = mem_heap;
    mem_max_addr = mem_heap + Max_Heap;
    return true;
}

unsigned int Memory::getUnusedMem(void)
{
    return unusedMem;
}

/*
 *mem_sbrk
 *
 * function:extend the heap by incr bytes and return the new start addr of new area
 *
 */

void *Memory::mem_sbrk(unsigned int incr)
{
    char *old_brk = mem_brk;

if(incr <= 0 || mem_brk + incr > mem_max_addr)
        return (void*)-1;
    mem_brk += incr;
    return (void*)old_brk;
}

void *Memory::extend_heap(unsigned int words)
{
    char *bp;
    unsigned int size;

    /* allocate an even number of words to maintain alignment */
    size = (words % 2)?(words+1)*wsize:words*wsize;
    if((bp = (char*)mem_sbrk(size)) == (void*)-1) /* mem_sbrk return the old mem_brk */
        return NULL;
    /* inintialize free block header/footer and epilogue header*/
    put(getBlockHeader(bp),pack(size,0));
    put(getBlockFooter(bp),pack(size,0));
    put(getBlockHeader(getNextBlock(bp)),pack(0,1)); /* ??? */
    return coalesce(bp);
}

int Memory::mm_init(unsigned int m)
{
    Max_Heap = m;
    unusedMem = m;
    mem_init();
    /* create the initial empty heap for memory model */
    heap_listp = (char *)mem_sbrk(4*wsize); /* size == 4*wsize bytes */
    if(heap_listp == (void*)-1)
        return -1;
    put(heap_listp, 0);
    put(heap_listp + 1*wsize, pack(dsize,1));
    put(heap_listp + 2*wsize, pack(dsize,1));
    put(heap_listp + 3*wsize, pack(dsize,1));
    heap_listp += (2*wsize);

    if(extend_heap(ChunkSize/wsize) == NULL)
        return -1;
    return 0;
}

void Memory::mm_free(void *bp)
{
    unsigned int size = getSize(getBlockHeader(bp));

    put(getBlockHeader(bp),pack(size,0));
    put(getBlockFooter(bp),pack(size,0));
    coalesce(bp);
    unusedMem += size;
}

void *Memory::coalesce(void *bp)
{
    unsigned int prev_allocfield = getAlloc(getBlockFooter(getPrevBlock(bp)));
    unsigned int next_allocfield = getAlloc(getBlockHeader(getNextBlock(bp)));
    unsigned int size = getSize(getBlockHeader(bp));

    if(prev_allocfield && next_allocfield)
        return bp;

    if(prev_allocfield && !next_allocfield)
    {
        size += getSize(getBlockHeader(getNextBlock(bp)));
        put(getBlockHeader(bp),pack(size,0));
        put(getBlockFooter(bp),pack(size,0));
    }
    else if(!prev_allocfield && next_allocfield)
    {
        size += getSize(getBlockHeader(getPrevBlock(bp)));
        put(getBlockHeader(getPrevBlock(bp)),pack(size,0));
        put(getBlockFooter(bp),pack(size,0));

    }
    else
    {
        size += getSize(getBlockHeader(getPrevBlock(bp))) + getSize(getBlockHeader(getNextBlock(bp)));
        put(getBlockHeader(getPrevBlock(bp)),pack(size,0));
        put(getBlockFooter(getNextBlock(bp)),pack(size,0));
        bp = getPrevBlock(bp);
    }
    return bp;
}

void *Memory::mm_malloc(unsigned int size)
{
    unsigned int asize; /* adjusted block size */
    unsigned int extendsize; /* amount to extend heap if no fit */
    char *bp;

    if(size <= 0)
        return NULL;

    /* adjusted block size to include overhead and alignment reqs*/
    if(size <= dsize)
        asize = 2 * dsize;
    else
        asize = dsize * ((size + dsize + dsize - 1)/dsize);

    /* search the free list for a fit */
    if((bp = (char*)memfit->find_fit(*this,asize)) != NULL)
    {
        place(bp,asize);
        unusedMem -= asize;
        return bp;
    }

    /* no fit found,get more memory and place the block */
    extendsize = max(asize,ChunkSize);
    if((bp = (char*)extend_heap(extendsize/wsize)) == NULL)
        return NULL;
    place(bp,asize);
    unusedMem -= asize;
    return bp;
}

void Memory::place(void *bp, unsigned int size)
{
    char *spiltblock_addr;
    unsigned int spiltblock_size;
    unsigned int blocksize;

    blocksize = getSize(getBlockHeader(bp));
    /* if the block size == size,then change the block header/footer alloc field simply*/
    /* the block size > size,spilt the old block */
    if(blocksize > size)
    {
        /* inintial the new spilted block(last) header and footer */
        spiltblock_addr = (char*)bp + size;
        spiltblock_size = blocksize - size;
        put(getBlockHeader(spiltblock_addr),pack(spiltblock_size,0));
        put(getBlockFooter(spiltblock_addr),pack(spiltblock_size,0));
    }
    /* inintial the new block header and footer */
    put(getBlockHeader(bp),pack(size,1));
    put(getBlockFooter(bp),pack(size,1));
}
