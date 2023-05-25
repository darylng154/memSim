#include "tlb.h"
#include "page_table.h"

void initTLB(TLBEntry* tlb)
{
    tlb->page_num = 0;
    tlb->frame_num = 0;
}

void initTLBTable(TLBTable* tlb_table, uint8_t length, uint8_t num_frames)
{
    tlb_table->faults = 0;
    tlb_table->hits = 0;

    if(num_frames >= MAX_TLB_ENTRIES_) /* If there are at least 16 frames*/
        tlb_table->max_entries = MAX_TLB_ENTRIES_; /* Set max entries to 16*/
    else
        tlb_table->max_entries = num_frames; /* Else TLB will only hold < 16*/

    tlb_table->num_entries = 0;

    int i = 0;
    for(i = 0; i < length; i++)
    {
        initTLB(&tlb_table->list[i]);
    }
}

void printTLB(const TLBEntry tlb, uint8_t printDetails)
{
    printf("page_num: %- 2i | frame_num: %- 3i ", tlb.page_num, tlb.frame_num);

    if(printDetails)
        printf("future stuff (set printDetails = 0 atm)");
    else
        printf("\n");
}

void printTLBTable(const TLBEntry* list, uint8_t length, uint8_t printDetails)
{
    int i = 0;
    for(i = 0; i < length; i++)
    {
        printf("TLBEntry[%- 2i]: ", i);
        printTLB(list[i], printDetails);
    }
}

void printTLBTableDebug(const TLBTable* tlb_table, uint8_t printDetails)
{
    printf("\n\n#################################  TLB Table  #################################\n");
    printf("| num_entries: %i | hits: %i | faults: %i \n", tlb_table->num_entries, tlb_table->hits, tlb_table->faults);
    printTLBTable(tlb_table->list, tlb_table->num_entries, printDetails);
    printf("###############################################################################\n\n\n");
}

void tlbSwap(TLBEntry* dest, TLBEntry* src)
{
    TLBEntry temp = *dest;
    *dest = *src;
    *src = temp;
}

void setTLB(TLBEntry* list, uint8_t index, uint8_t page_num, uint8_t frame_num)
{
    list[index].page_num = page_num;
    list[index].frame_num = frame_num;
}

void addPageToTLBTable(TLBTable* tlb_table, uint8_t page_num, uint8_t frame_num)
{
    setTLB(tlb_table->list, tlb_table->num_entries, page_num, frame_num);
    tlb_table->num_entries++;
}

Seek checkTLB(TLBTable* tlb_table, Algorithm algorithm, uint8_t page_num, uint8_t *frame_num)
{
    int TLB_entry;

    /* If there are no entries in the tlb_table then it is a miss*/
    if(tlb_table->num_entries <= 0){
        if(verbosity)
            printf("TLB empty; Page %i Doesn't Exist \n", page_num);
        tlb_table->faults++;
        return MISS;
    }
    
    for(TLB_entry = 0; TLB_entry < tlb_table->num_entries; TLB_entry++){ /* Search tlb_table for page_num*/
        if(tlb_table->list[TLB_entry].page_num == page_num){ /* Entry found*/
            *frame_num = tlb_table->list[TLB_entry].frame_num;
            if(verbosity)
                printf("TLB: Page %i Exists. Its mapped to frame %i\n", page_num, *frame_num);
            // if(algorithm == LRU)
            //     ; /* Adjust queue location. Put to front of queue, like it's the newest entry*/
            // else if(algorithm == OPT)
            //     ; /* Do magic*/
            // else /* FIFO*/
            //     ; /* Do nothing*/
            tlb_table->hits++; /* Page was found*/
            return HIT;
        }
    }
    /* No hit*/
    tlb_table->faults++;
    return MISS;
}


void testCheckTLB(TLBTable* tlb_table){
    verbosity = 1;
    // TLBTable* tlb_table;
    Algorithm algorithm = FIFO;
    uint8_t page_num;
    uint8_t *frame_num;

    Seek TLB_seek_result = MISS;

    tlb_table->list[0].frame_num = 0;
    tlb_table->list[0].page_num  = 3;
    tlb_table->num_entries++;
    tlb_table->list[1].frame_num = 1;
    tlb_table->list[1].page_num  = 7;
    tlb_table->num_entries++;
    tlb_table->list[2].frame_num = 2;
    tlb_table->list[2].page_num  = 0;
    tlb_table->num_entries++;
    
    /* Find frame 0. Should HIT*/
    page_num = 3;
    TLB_seek_result = checkTLB(tlb_table, FIFO, page_num, frame_num);
    if(TLB_seek_result == MISS)
        printf("TLB MISS: Page Number %i Not Found!\n", page_num);
    else
        printf("TLB HIT: Page Number %i Found!\n", page_num);
    
    /* Find frame 1. Should HIT*/
    page_num = 7;
    TLB_seek_result = checkTLB(tlb_table, FIFO, page_num, frame_num);
    if(TLB_seek_result == MISS)
        printf("TLB MISS: Page Number %i Not Found!\n", page_num);
    else
        printf("TLB HIT: Page Number %i Found!\n", page_num);

    /* Find frame 2. Should HIT*/
    page_num = 0;
    TLB_seek_result = checkTLB(tlb_table, FIFO, page_num, frame_num);
    if(TLB_seek_result == MISS)
        printf("TLB MISS: Page Number %i Not Found!\n", page_num);
    else
        printf("TLB HIT: Page Number %i Found!\n", page_num);

    /* Find frame 3. Should MISS*/
    page_num = 8;
    TLB_seek_result = checkTLB(tlb_table, FIFO, page_num, frame_num);
    if(TLB_seek_result == MISS)
        printf("TLB MISS: Page Number %i Not Found!\n", page_num);
    else
        printf("TLB HIT: Page Number %i Found!\n", page_num);
    return;
}

// returns 1 if TLBTable is Full, else 0
int isTLBFull(TLBTable* tlb_table)
{
    if(tlb_table->num_entries >= MAX_TLB_ENTRIES_)
        return 1;
    
    return 0;
}

// run the page replacement algorithm for the TLB
void runTLBPRA(TLBTable* tlb_table, uint8_t frame_num, uint8_t )
{

}