#ifndef TLB_H
#define TLB_H

#include "page_table.h"

typedef struct tlb TLB;
typedef struct tlb_table TLBTable;

struct tlb
{
    uint8_t page_num;
    uint8_t frame;
    uint8_t valid;
};

struct tlb_table
{
    uint8_t num_pages;
    Page pages[MAXFRAME];
};


#endif