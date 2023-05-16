#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#define MAXFRAMESIZE 256
#define MAXPAGES 256

typedef struct page Page;
typedef struct page_table PageTable;

struct page
{
    uint8_t page_num;
    uint8_t frame[MAXFRAMESIZE];
    uint8_t valid;
};

struct page_table
{
    uint8_t num_pages;
    Page pages[MAXPAGES];
};


#endif