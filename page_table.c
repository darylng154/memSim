#include "page_table.h"

void initPage(Page* page)
{
    page->frame_num = 0;
    page->valid = 0;
}

void initPageTable(PageTable* page_table, uint16_t length)
{
    page_table->num_entries = 0;

    int i = 0;
    for(i = 0; i < length; i++)
    {
        initPage(&page_table->list[i]);
    }
}

void printPage(const Page page, uint8_t printDetails)
{
    printf("frame_num: %- 2i | valid: %i", page.frame_num, page.frame_num);

    if(printDetails)
        printf("future stuff (set printDetails = 0 atm)");
    else
        printf("\n");
}

void printPageTable(const Page* list, uint16_t length, uint8_t printDetails)
{
    int i = 0;
    for(i = 0; i < length; i++)
    {
        printf("Page[%- 2d]: ", i);
        printPage(list[i], printDetails);
    }
}

void printPageTableDebug(const PageTable* page_table, uint8_t printDetails)
{
    printf("\n\n#################################  Page Table  #################################\n");
    printf("| num_entries: %i \n", page_table->num_entries);
    printPageTable(page_table->list, page_table->num_entries, printDetails);
    printf("################################################################################\n\n\n");
}

void pageSwap(Page* dest, Page* src)
{
    Page temp = *dest;
    *dest = *src;
    *src = temp;
}

void setPage(Page* list, uint8_t index, uint8_t frame_num, uint8_t valid)
{
    list[index].frame_num = frame_num;
    list[index].valid = valid;
}

void pageTableFIFO(PageTable* page_table, uint16_t* right_most_bits)
{
    int i = 0;
    for(i = 0; i < page_table->num_entries; i++)
    {
        
    }
}