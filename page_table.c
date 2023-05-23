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

// if page is in Page Table == 1, else == 0
int isPageNumValid(PageTable* page_table, uint8_t page_num)
{
    int i = 0;
    for(i = 0; i < page_table->num_entries; i++)
    {
        if(page_table->list[i].valid == 1)
            return 1;
    }

    return 0;
}

void checkPageTable(AddressTable* address_table, PageTable* page_table)
{
    int i = 0;
    for(i = 0; i < address_table->num_entries; i++)
    {
        if(isPageNumValid(page_table, address_table->list[i].page_num))
        {
            // page # is in PageTable => 
            // w/ TLB: populate TLB w/ page
            // w/o TLB: make physical address - get frame # & page_offset = frame_offset?
            if(verbosity)
                printf("Page %i Exists \n", address_table->list[i].page_num);
        }
        else
        {
            // get page from .bin
            if(verbosity)
                printf("Page %i Doesn't Exists \n", address_table->list[i].page_num);
        }
    }
}