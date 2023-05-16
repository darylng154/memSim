#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "tlb.h"

void initTLB(TLB* tlb)
{
    tlb->page_num = 0;
    tlb->frame_num = 0;
}

void initTLBTable(TLBTable* tlb_table, uint8_t length)
{
    tlb_table->num_pages = 0;

    int i = 0;
    for(i = 0; i < length; i++)
    {
        initTLB(&tlb_table->list[i]);
    }
}

void printTLB(const TLB tlb, int printDetails)
{
    printf("page_num: %- 2d | frame_num: %- 2i ", tlb.page_num, tlb.frame_num);

    if(printDetails)
        printf("future stuff (set printDetails = 0 atm)");
    else
        printf("\n");
}

void printTLBTable(const TLB* list, uint8_t length, int printDetails)
{
    int i = 0;
    for(i = 0; i < length; i++)
    {
        printf("TLB[%- 2d]: ", i);
        printTLB(list[i], printDetails);
    }
}

void printTLBTableDebug(const TLBTable tlb_table, int printDetails)
{
    printf("\n\n#################################  TLB Table  #################################\n");
    printf("| numtlbs: %d \n", tlb_table.num_pages);
    printTLBTable(tlb_table.list, tlb_table.num_pages, printDetails);
    printf("################################################################################\n\n\n");
}

void tlbSwap(TLB* dest, TLB* src)
{
    TLB temp = *dest;
    *dest = *src;
    *src = temp;
}

void setTLBNumber(TLB* list, uint8_t index, uint8_t page_num, uint8_t frame_num)
{
    list[index].page_num = page_num;
    list[index].frame_num = frame_num;
}