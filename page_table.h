#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#include "address.h"
#include "tlb.h"

typedef struct page Page;
typedef struct page_table PageTable;

struct page
{
    uint8_t frame_num;
    uint8_t valid;
};

struct page_table
{
    // page table doesn't use this, only queue
    uint8_t num_entries;

    Page list[MAX_FRAME_SIZE_];
};

void initPage(Page* page);
void initPageTable(PageTable* page_table, uint16_t length);
void printPage(const Page page, uint8_t printDetails);
void printPageTable(const Page* list, uint8_t printDetails);
void printPageTableDebug(const PageTable* page_table, uint8_t printDetails, uint8_t printQueue);
void pageSwap(Page* dest, Page* src);
void setPage(Page* list, uint8_t index, uint8_t frame_num, uint8_t valid);
int isPageNumValid(PageTable* page_table, uint8_t page_num);
Seek checkPageTable(Address* address, PageTable* page_table);

#endif