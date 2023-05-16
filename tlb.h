#ifndef TLB_H
#define TLB_H

#include "page_table.h"

typedef struct tlb TLB;
typedef struct tlb_table TLBTable;

struct tlb
{
    uint8_t page_num;
    uint8_t frame[MAXFRAMESIZE];
};

struct tlb_table
{
    uint8_t num_pages;
    Page pages[MAXPAGES];
};


#endif