#ifndef DYNAMICALLOCATION_H
#define DYNAMICALLOCATION_H
#include "memfit.h"
#include "findfirstfit.h"
#include <memory>
/* Singleton and stagedy design pattern */
class Memory
{
    friend class MemFit;
    friend class FindFirstFit;
public:
    unsigned int unusedMem = 0;
    /* get a unique instance */
    static Memory getInstance(void);
    void *mm_malloc(unsigned int size);
    void mm_free(void *bp);
    int mm_init(unsigned int m);
    unsigned int getUnusedMem(void);
    void setMemfit(MemFit *m);
private:
    unsigned int Max_Heap = 0; /* the memory model heap max size */
    char *mem_heap; /*point to first byte of heap */
    char *mem_brk; /* point to last byte of heap */
    char *mem_max_addr; /* max legal heap addr plus 1 */
    char *heap_listp; /* point to prologue block(序言块) */
    static Memory uniqueMemory;
    MemFit *memfit;	/* find mem fit :fistragedy design pattern */
    static const unsigned int ChunkSize; /* extend heap by this amount bytes */
    static const unsigned int wsize; /* word and header/footer size bytes*/
    static const unsigned int dsize; /* double word size bytes*/

    Memory() {}
    bool mem_init(void);
    void *mem_sbrk(unsigned int incr);
    void *extend_heap(unsigned int words);
    void *coalesce(void *bp);
    void place(void *bp, unsigned int size);
    inline unsigned int max(unsigned int x,unsigned int y)
    {
        return x > y?x:y;
    }

    inline unsigned int pack(unsigned int size, unsigned int alloc)
    {
        return size | alloc;
    }

    inline unsigned int get(void *p)
    {
        return *((unsigned int*)p);
    }

    inline void put(void *p,unsigned int val)
    {
        *((unsigned int*)p) = val;
    }

    /*
     *getSize
     *
     * read the size field from addr p,the block size(header+footer+avaliable place)
     *
     * size field:xxxx xxxx xxxx x000
     */
    inline unsigned int getSize(void *p)
    {
        return get(p) & ~0x07;
    }

    /*
     *getAlloc
     *
     *read the alloc field from addr p
     *
     *alloc field:0000 0000 0000 000x
     */
    inline bool getAlloc(void *p)
    {
        return bool(get(p)&0x01);
    }

    /*
     *getBlockFooter
     *
     *return the header addr of the given block addr p
     */
    inline char* getBlockHeader(void *p)
    {
        return (char*)p - wsize;
    }

    /*
     *getBlockFooter
     *
     *return the footer addr of the given block addr p
     */
    inline char* getBlockFooter(void *p)
    {
        return (char*)p +(getSize(getBlockHeader(p)) - dsize);
    }

    /*
     *
     *
     *
     */
    inline char* getNextBlock(void *p)
    {
        unsigned int blocksize = getSize((char*)p - wsize);
        return (char*)p + blocksize;
    }

    inline char* getPrevBlock(void *p)
    {
        unsigned int preblocksize = getSize((char*)p - dsize);
        return (char*)p - preblocksize;
    }
};
#endif
