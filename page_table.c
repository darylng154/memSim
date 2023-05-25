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
    printf("frame_num: %- 2i | valid: %i", page.frame_num, page.valid);

    if(printDetails)
        printf("future stuff (set printDetails = 0 atm)");
    else
        printf("\n");
}

void printPageTable(const Page* list, uint8_t printDetails)
{
    int i = 0;
    for(i = 0; i < MAX_FRAMES_; i++)
    {
        if(list[i].valid)
        {
            printf("Page[%-3d]: ", i);
            printPage(list[i], printDetails);
        }
    }
}

void printPageTableDebug(const PageTable* page_table, uint8_t printDetails, uint8_t printQueue)
{
    printf("\n\n#################################  Page Table  #################################\n");
    if(printQueue)
    {
        printf("| num_entries: %i \n", page_table->num_entries);
        // printf("| hits: %i | faults: %i \n", page_table->hits, page_table->faults);
    }
    printPageTable(page_table->list, printDetails);
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

// if page is in Page Table == 1, else == 0
int isPageNumValid(PageTable* page_table, uint8_t page_num)
{
    if(page_table->list[page_num].valid == 1)
        return 1;

    return 0;
}

Seek checkPageTable(Address* address, PageTable* page_table)
{
    if(isPageNumValid(page_table, address->page_num))
    {
        // page # is in PageTable => 
        // hit: populate TLB w/ page & put into RAM
        if(verbosity)
            printf("Page Table: Page %i Exists \n", address->page_num);

        return HIT;
    }
    else
    {
        // get page from .bin . Might want to do this in the start sim function
        //  Not sure if we should consider both the TLB and the page table misses somewhat together
        // => no we discussed that we have to keep track of the TLB hit and miss seperate from page faults
        // because of the output
        if(verbosity)
            printf("Page Table: Page %i Doesn't Exist \n", address->page_num);

        return MISS;
    }
}