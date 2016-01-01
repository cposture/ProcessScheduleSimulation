#ifndef DYNAMICALLOCATION_H
#define DYNAMICALLOCATION_H

#include <malloc.h>


#define FIRST_FIT 1
#define Malloc malloc
static unsigned int Max_Heap = 0; /* the memory model heap max size */
static const unsigned int wsize = 4; /* word and header/footer size bytes*/
static const unsigned int dsize = 8; /* double word size bytes*/
extern unsigned int unusedMem;


extern void *mm_malloc(unsigned int size);
extern void mm_free(void *bp);
extern int mm_init(unsigned int m);
extern unsigned int getUnusedMem(void);

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
#endif
