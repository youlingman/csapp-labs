/*
 * mm-testbook-method.c - Use code from textbook, shrink brk in condition to improve space utilization.
 * 
 * In this approach, code from textbook are using directly.  
 * A block include header, footer and payload, while header and 
 * footer both with 4 byte size. Blocks will be coalesced after free.
 * The heap can be shrink after coalescing free block.
 * Realloc is implemented directly using mm_malloc and mm_free.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "cyl",
    /* First member's full name */
    "cyl",
    /* First member's email address */
    "cyl@cyl.me",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* $begin mallocmacros */
/* size of word / header / footer */
#define WSIZE 4

/* size of doubleword */
#define DSIZE 8

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc)  ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p)       (*(unsigned int *)(p))
#define PUT(p, val)  (*(unsigned int *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)  (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)       ((char *)(bp) - WSIZE)
#define FTRP(bp)       ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp)  ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp)  ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))
/* $end mallocmacros */

/* Global variables */
static char *first_block_brk;

/* Function prototypes for internal helper routines */
//static void *extend_heap(size_t words);
static void *place_block(void *bp, size_t alloc_size);
static void *find_fit_block(size_t alloc_size);
static void coalesce(void *bp);
static void printblock(void *bp); 
static void checkheap(int verbose);
static void checkblock(void *bp);

/* 
 * mm_init - initialize the malloc package. Add the epilogue header.
 */
int mm_init(void)
{
    void *heap_start_brk =  mem_sbrk(ALIGN(DSIZE));

    if (heap_start_brk == (void *)-1) return -1;
    // set the initial header for the whole heap
    PUT((char *)heap_start_brk + WSIZE, PACK(0, 0));
    // set the first block address
    first_block_brk = heap_start_brk + DSIZE;
    return 0;
}

/* 
 * mm_malloc - Travel through the block chain to find the first fit block to allocate.
 *     Allocate the block by incrementing the brk pointer if there's no fit block exist.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    // the allocate size is content size plus header and footer size
    size_t newsize = ALIGN(size + DSIZE);
    char *bp = find_fit_block(newsize);
    if(bp != NULL) {
        char * tmp = place_block(bp, newsize);
        //printf("checkheap after malloc\n");
        //checkheap(1);
        return (void *)tmp;
    }
    else return NULL;
}

/*
 * mm_free - Freeing a block, coalesce the adjacent free block.
 */
void mm_free(void *ptr)
{
    if(ptr == NULL) return;

    size_t size = GET_SIZE(HDRP(ptr));
    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));
    coalesce(ptr);
    //printf("checkheap after free\n");
    //checkheap(1);
}

/*
 * coalesce - Boundary tag coalescing, shrink the heap after coalescing if possible.
 *    Return ptr to coalesced block
*/
static void coalesce(void *bp) 
{
    size_t size = GET_SIZE(HDRP(bp));
    size_t prev_alloc, next_alloc;

	// try the backward coalescing
    while(bp != first_block_brk) {
        prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
        if (prev_alloc) break;
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }

	// try the forward coalescing
    while(GET(HDRP(NEXT_BLKP(bp))) != 0x0) {
        next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
        if (next_alloc) break;
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    }

    // shrink the heap if possible
    if(GET(HDRP(NEXT_BLKP(bp))) == 0x0) {
        // release the last free block
        PUT(HDRP(bp), PACK(0, 0));
        // shrink the brk
        mem_reset_brk();
        mem_sbrk((char *) bp - (char *)mem_heap_lo());
    }

    return;
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    /* If size == 0 then this is just free, and we return NULL. */
    if (size == 0) {
        mm_free(ptr);
        return 0;
    }

    /* If oldptr is NULL, then this is just malloc. */
    if (ptr == NULL) return mm_malloc(size);

    /* Copy the old data. */
    size_t oldsize = GET_SIZE(HDRP(ptr));
    if(size < oldsize) oldsize = size;

    void * newptr = mm_malloc(size);
    /* If realloc() fails the original block is left untouched  */
    if(!newptr) return 0;

    memcpy(newptr, ptr, oldsize);

    /* Free the old block. */
    mm_free(ptr);

    return newptr;
}

/*
 * find_fit_block - Find the fit block for alloc_size size
 */
static void *find_fit_block(size_t alloc_size)
{
    char *bp = first_block_brk;

    // travel through the block chain
    for ( bp = first_block_brk; GET(HDRP(bp)) != 0x0; bp = NEXT_BLKP(bp)) {
        if(!GET_ALLOC(HDRP(bp)) && alloc_size <= GET_SIZE(HDRP(bp))) return (void *)bp;
    }
    if(GET(HDRP(bp)) == 0x0) return (void *)bp;
    else return NULL;
}

/*
 * place_block - Place a alloc_size size block at the 
 */
static void *place_block(void *bp, size_t alloc_size)
{
    size_t block_size = GET_SIZE(HDRP(bp));

    // extend the heap to get more space
    if(block_size == 0) {
        void *p = mem_sbrk(alloc_size);
        if(p == (void *)-1) return NULL;
        else {
            // set header and footer for current block
            PUT(HDRP(bp), PACK(alloc_size, 1));
            PUT(FTRP(bp), PACK(alloc_size, 1));
            // set header for the unuse heap space
            PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 0));
        }
    }
    // split current block to allocate the new block
    else if (block_size - alloc_size >= 2 * DSIZE) {
        // set header and footer for the new block
        PUT(HDRP(bp), PACK(alloc_size, 1));
        PUT(FTRP(bp), PACK(alloc_size, 1));
        // set header and footer for the remain block
        PUT(HDRP(NEXT_BLKP(bp)), PACK(block_size - alloc_size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(block_size - alloc_size, 0));
    }
	// can't get enough space for two block after split, use the whole block without split
    else {
        PUT(HDRP(bp), PACK(block_size, 1));
        PUT(FTRP(bp), PACK(block_size, 1));
    }
    return bp;
}

static void printblock(void *bp) 
{
    size_t hsize, halloc, fsize, falloc;

    checkheap(0);
    hsize = GET_SIZE(HDRP(bp));
    halloc = GET_ALLOC(HDRP(bp));  
    fsize = GET_SIZE(FTRP(bp));
    falloc = GET_ALLOC(FTRP(bp));  

    if (hsize == 0) {
    printf("%p: EOL\n", bp);
    return;
    }

      printf("%p: header: [%d:%c] footer: [%d:%c]\n", bp, 
            hsize, (halloc ? 'a' : 'f'), 
            fsize, (falloc ? 'a' : 'f'));
}

static void checkblock(void *bp) 
{
    if (GET(HDRP(bp)) != GET(FTRP(bp)))
    printf("Error: header does not match footer\n");
}

/* 
 * checkheap - Minimal check of the heap for consistency 
*/
void checkheap(int verbose) 
{
    char *bp = first_block_brk;

    for (; GET(HDRP(bp)) != 0x0; bp = NEXT_BLKP(bp)) {
    if (verbose) 
        printblock(bp);
    checkblock(bp);
    }

    if ((GET(HDRP(bp)) != 0x0))
    printf("Bad epilogue header\n");
}








