#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#define MAX_FRAME_ 256

typedef struct page Page;
typedef struct page_table PageTable;

struct page
{
    uint8_t frame_num;
    uint8_t valid;
};

struct page_table
{
    uint8_t num_entries;
    Page list[MAX_FRAME_];
};

void initPage(Page* page);
void initPageTable(PageTable* page_table, uint16_t length);
void printPage(const Page page, uint8_t printDetails);
void printPageTable(const Page* list, uint8_t length, uint8_t printDetails);
void printPageTableDebug(const PageTable* tlb_table, uint8_t printDetails);
void pageSwap(Page* dest, Page* src);
void setPage(Page* list, uint8_t index, uint8_t frame_num, uint8_t valid);

#endif