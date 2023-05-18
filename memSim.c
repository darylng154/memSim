#include "options.h"
#include "safeutil.h"
#include "tlb.h"
#include "page_table.h"

// masks the 32-bit logical address to get the 16 rightmost bits (= 8-bit page #, 2 8-bit page offset)
uint16_t maskFileInts(uint32_t logical_address)
{
    uint16_t result = 0;
    uint32_t mask = 0x0000FFFF;

    result = logical_address & mask;

    return result;
}

int main(int argc, char *argv[]){
    char *filename;                       /* Holds the file name*/
    uint8_t num_frames = MAX_FRAMES_;     /* Holds the number of frames*/
    uint8_t algorithm = DEFAULT_ALGO_;    /* Holds the alogrithm*/
    TLBTable* tlb_table = NULL;           // TLB unit
    PageTable* page_table = NULL;         // Page Table Unit
    
    parseOptions(argc, argv, &filename, &num_frames, &algorithm);
    
    tlb_table = safeMalloc(sizeof(TLBTable));
    initTLBTable(tlb_table, MAX_TLB_SIZE_);

    page_table = safeMalloc(sizeof(PageTable));
    initPageTable(page_table, MAX_FRAME_);

    // tlb_table->num_entries = 10;
    // page_table->num_entries = 10;

    // tlb_table->num_entries = MAX_TLB_SIZE_;
    // page_table->num_entries = MAX_FRAME_;

    uint32_t test = 0x12345678;
    printf("mask test: %0x", maskFileInts(test));

    // 0 is printDetails: for more details in later implementation
    if(verbosity){
        printTLBTableDebug(tlb_table, 0);
        printPageTableDebug(page_table, 0);
    }

    return 0; 
}

