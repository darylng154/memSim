#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#include "address.h"
#include "tlb.h"

typedef struct page Page;
typedef struct page_table PageTable;

struct page
{
    // if its PageTable* Queue->frame_num = page num
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
void printPage(const Page page, uint8_t printDetails, uint8_t printQueue);
void printPageTable(const Page* list, uint8_t printDetails, uint8_t printQueue);
void printPageTableDebug(const PageTable* page_table, uint8_t printDetails, uint8_t printQueue);
void pageSwap(Page* dest, Page* src);
void setPage(Page* list, uint8_t index, uint8_t frame_num, uint8_t valid);
// if page is in Page Table == 1, else == 0
// int isPageNumValid(PageTable* page_table, uint8_t page_num); // changed from
int isPageNumValid(PageTable* page_table, uint8_t page_num, uint8_t *resolved_frame_num); // to
// Seek checkPageTable(Address* address, PageTable* page_table); // changed from
Seek checkPageTable(uint8_t page_num, PageTable* page_table, uint8_t *resolved_frame_num); // to
// returns 1 if TLBTable is Full, else 0; max_entries = <FRAME>
int isQueueFull(PageTable* queue, uint8_t max_entries);
// inject to front(queue[0]) based on Algorithm
void addToQueue(PageTable* queue, uint8_t page_num);

#endif