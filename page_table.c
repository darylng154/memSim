#include "page_table.h"

void initPage(Page* page)
{
    page->frame_num = 0;
    page->valid = 0;

    page->hit = 0;
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
            if(printQueue)
                printf("Queue[%-3d]: ", i);
            else
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
// int isPageNumValid(PageTable* page_table, uint8_t page_num) // changed from
int isPageNumValid(PageTable* page_table, uint8_t page_num, uint8_t *resolved_frame_num) // to
{
    if(page_table->list[page_num].valid == 1){
        *resolved_frame_num = page_table->list[page_num].frame_num; // added
        return 1;
    }

    return 0;
}

// Seek checkPageTable(Address* address, PageTable* page_table) // changed from
Seek checkPageTable(uint8_t page_num, PageTable* page_table, uint8_t *resolved_frame_num) // to
{
    // if(isPageNumValid(page_table, address->page_num)) // change from
    if(isPageNumValid(page_table, page_num, resolved_frame_num)) // to
    {
        // page # is in PageTable => 
        // hit: populate TLB w/ page & put into RAM
        if(verbosity)
            // printf("Page Table: Page %i Exists \n", address->page_num); // changed from
            printf("Page Table: Page %i Exists \n", page_num); // to

        return HIT;
    }
    else
    {
        // get page from .bin . Might want to do this in the start sim function
        //  Not sure if we should consider both the TLB and the page table misses somewhat together
        // => no we discussed that we have to keep track of the TLB hit and miss seperate from page faults
        // because of the output
        if(verbosity)
            // printf("Page Table: Page %i Doesn't Exist \n", address->page_num); // changed from
            printf("Page Table: Page %i Doesn't Exist \n", page_num); // to

        return MISS;
    }
}

// returns 1 if TLBTable is Full, else 0; max_entries = <FRAME>
int isQueueFull(PageTable* queue, uint8_t num_frames)
{
    if(queue->num_entries <= num_frames)
    {
        if(verbosity)
            printf("Queue is not Full! \n");

        return 0;
    }

    if(verbosity)
        printf("Queue is Full! | num_entries: %i \n", queue->num_entries);
    
    return 1;
}

// inject to front(queue[0] = MRU) based on Algorithm
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

// remove last queue entry & make room in queue->list[0]
void removeLastInQueue(PageTable* queue)
{
    setPage(queue->list, queue->num_entries - 1, 0, 0);

    int i = 0;
    for(i = queue->num_entries - 1; i > 0; i--)
    {
        // swap to make room
        pageSwap(&queue->list[i], &queue->list[i-1]);
    }
}

// run QueuePRA - replace page in Queue & page fault++
void runQueuePRA(PageTable* queue, AddressTable* address_table, Algorithm algorithm, uint8_t seek_page_num, Seek TLB_seek_result, Seek PT_seek_result)
{
    switch(algorithm)
    {
        case FIFO:
            if(verbosity)
                printf("runQueuePRA FIFO\n");

            runQueueFIFO(queue, seek_page_num, TLB_seek_result, PT_seek_result);
            break;

        case LRU:
            if(verbosity)
                printf("runQueuePRA LRU\n");
            break;

        case OPT:
            if(verbosity)
                printf("runQueuePRA OPT\n");
            break;

        default:
            perror("#ERROR: runQueuePRA defaulted, does not exist \n");
            exit(EXIT_FAILURE);
            break;
    }

    address_table->page_faults++;
}

void runQueueFIFO(PageTable* queue, uint8_t seek_page_num, Seek TLB_seek_result, Seek PT_seek_result)
{
    if(TLB_seek_result == MISS && PT_seek_result == MISS)
    {
        removeLastInQueue(queue);

        // inject
        setPage(queue->list, 0, seek_page_num, 1);

        if(verbosity)
            printf("runQueueFIFO | page_num: %i | num_entries: %i \n", seek_page_num, queue->num_entries);
    }
}

// update page table (removed & added page)
void updatePageTable(Page* list, uint8_t old_page_num, uint8_t new_page_num, uint8_t frame_num)
{
    // invalidate old page
    setPage(list, old_page_num, frame_num, 0);
    // validate new page
    setPage(list, new_page_num, frame_num, 1);
}


void runQueueLRU(PageTable* queue, Seek TLB_seek_result, Seek PT_seek_result, TLBEntry new_tlb_entry){
    
    uint8_t queue_pos;
    
    queue_pos = getQueuePosition(queue, new_tlb_entry.page_num);

// void addToQueue(PageTable* queue, uint8_t page_num)
// {
    // int i = 0;
    // for(i = queue->num_entries; i > 0; i--)
    // {
        // swap to make room
    //     pageSwap(&queue->list[i], &queue->list[i-1]);
    // }

    if(TLB_seek_result == HIT){
        queue->list[queue_pos];
    }
    else if(PT_seek_result == HIT){

    }
    else if(PT_seek_result == MISS){

    }
}

uint8_t getQueuePosition(PageTable* queue, uint8_t page_num){
    int queue_pos;
    uint8_t curr_page_num; /* Queue page num is stored in frame_num*/

    for(queue_pos = queue->num_entries; queue_pos > 0; queue_pos--){
        curr_page_num = queue->list[queue_pos].frame_num; /* For clarity, queue frame num = page num*/
        if(curr_page_num == page_num){
            if(verbosity){
                printf("Queue[%-3d]: ", queue_pos);
                printPage(queue->list[queue_pos], 0, PRINT_QUEUE_);
            }
            return queue_pos;
        }
    }
    
    errorout("Something went wrong. Can't find page in queue");
    return 0;
}

void reorderQueue(PageTable* queue, uint8_t queue_pos){
    Page temp_page;
    temp_page = queue->list[queue_pos]; /* Store the queue page*/
    slideQueue(queue, queue_pos); /* Slide queue and leave a spot at the front*/
    /* Push to back of queue*/
}

void slideQueue(PageTable* queue, uint8_t popped_pos){
    int queue_pos;

    for(queue_pos = popped_pos; queue_pos > 0; queue_pos--){

    }

    return;
}

void removeNoFutureInQueue(PageTable* queue, AddressTable* address_table, uint32_t current_index)
{
    int i = 0;
    for(i = current_index; i < address_table->num_entries; i++)
    {

    }
}

void removeFurthestInQueue(PageTable* queue, AddressTable* address_table, uint32_t current_index)
{
    
}

void runQueueOPT(PageTable* queue, uint8_t seek_page_num, Seek TLB_seek_result, Seek PT_seek_result)
{
    if(TLB_seek_result == MISS && PT_seek_result == MISS)
    {
        removeLastInQueue(queue);

        // inject
        setPage(queue->list, 0, seek_page_num, 1);

        if(verbosity)
            printf("runQueueFIFO | page_num: %i | num_entries: %i \n", seek_page_num, queue->num_entries);
    }
}