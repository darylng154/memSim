#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#include "address.h"

typedef struct page Page;
typedef struct page_table PageTable;

struct page
{
    uint8_t frame_num;
    uint8_t valid;
};

struct page_table
{
    uint16_t num_entries;
    Page list[MAX_FRAME_SIZE_];
};

void initPage(Page* page);
void initPageTable(PageTable* page_table, uint16_t length);
void printPage(const Page page, uint8_t printDetails);
void printPageTable(const Page* list, uint16_t length, uint8_t printDetails);
void printPageTableDebug(const PageTable* page_table, uint8_t printDetails);
void pageSwap(Page* dest, Page* src);
void setPage(Page* list, uint8_t index, uint8_t frame_num, uint8_t valid);
int isPageNumValid(PageTable* page_table, uint8_t page_num);
void checkPageTable(AddressTable* address_table, PageTable* page_table);

#endif