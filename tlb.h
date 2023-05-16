#ifndef TLB_H
#define TLB_H

#define MAX_TLB_SIZE_ 16

typedef struct tlb TLB;
typedef struct tlb_table TLBTable;

struct tlb
{
    uint8_t page_num;
    uint8_t frame_num;
};

struct tlb_table
{
    uint8_t num_entries;
    TLB list[MAX_TLB_SIZE_];
};

void initTLB(TLB* tlb);
void initTLBTable(TLBTable* tlb_table, uint8_t length);
void printTLB(const TLB tlb, uint8_t printDetails);
void printTLBTable(const TLB* list, uint8_t length, uint8_t printDetails);
void printTLBTableDebug(const TLBTable* tlb_table, uint8_t printDetails);
void tlbSwap(TLB* dest, TLB* src);
void setTLB(TLB* list, uint8_t index, uint8_t page_num, uint8_t frame_num);


#endif