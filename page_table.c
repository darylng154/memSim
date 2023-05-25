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

void printPage(const Page page, uint8_t printDetails, uint8_t printQueue)
{
    if(printQueue)
        printf("page_num: %-3i | valid: %i", page.frame_num, page.valid);
    else
        printf("frame_num: %-3i | valid: %i", page.frame_num, page.valid);

    if(printDetails)
        printf("future stuff (set printDetails = 0 atm)");
    else
        printf("\n");
}

void printPageTable(const Page* list, uint8_t printDetails, uint8_t printQueue)
{
    int i = 0;
    for(i = 0; i < MAX_FRAMES_; i++)
    {
        if(list[i].valid)
        {
            printf("Page[%-3d]: ", i);
            printPage(list[i], printDetails, printQueue);
        }
    }
}

void printPageTableDebug(const PageTable* page_table, uint8_t printDetails, uint8_t printQueue)
{
    if(printQueue)
    {
        printf("\n\n#################################  Queue Table  #################################\n");
        printf("| num_entries: %i \n", page_table->num_entries);
        // printf("| hits: %i | faults: %i \n", page_table->hits, page_table->faults);
    }
    else
        printf("\n\n#################################  Page Table  #################################\n");
    printPageTable(page_table->list, printDetails, printQueue);
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

// returns 1 if TLBTable is Full, else 0; max_entries = <FRAME>
int isQueueFull(PageTable* queue, uint8_t num_frames)
{
    if(queue->num_entries < num_frames)
    {
        if(verbosity)
            printf("Queue is not Full! \n");

        return 0;
    }

    if(verbosity)
        printf("Queue is Full! | num_entries: %i \n", queue->num_entries);
    
    return 1;
}

// inject to front(queue[0]) based on Algorithm
void addToQueue(PageTable* queue, uint8_t page_num)
{
    int i = 0;
    for(i = queue->num_entries; i > 0; i--)
    {
        // swap to make room
        pageSwap(&queue->list[i], &queue->list[i-1]);
    }

    // inject
    setPage(queue->list, 0, page_num, 1);
    queue->num_entries++;

    if(verbosity)
        printf("addToQueue | page_num: %i | num_entries: %i \n", page_num, queue->num_entries);
}