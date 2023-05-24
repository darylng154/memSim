#include "tlb.h"

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

Seek checkTLB(TLBTable* tlb_table, Algorithm algorithm, uint8_t page_num, uint8_t *frame_num)
{
    int i;

    /* If there are no entries in the tlb_table then it is a miss*/
    if(tlb_table->num_entries <= 0){
        tlb_table->faults++;
        return MISS;
    }
    
    for(i = 0; i < tlb_table->num_entries; i++){ /* Search tlb_table for page_num*/
        if(tlb_table->list[i].page_num == page_num){
            *frame_num = tlb_table->list[i].frame_num;
            if(algorithm == LRU)
                ; /* Adjust queue location. Put to front of queue, like it's the newest entry*/
            else if(algorithm == OPT)
                ; /* Do magic*/
            else /* FIFO*/
                ; /* Do nothing*/
            tlb_table->hits++; /* Page was found*/
            return HIT;
        }
    }
    /* No hit*/
    tlb_table->faults++;
    return MISS;
}


