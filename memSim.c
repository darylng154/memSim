#include "options.h"
#include "safeutil.h"
#include "address.h"
#include "tlb.h"
#include "page_table.h"

int main(int argc, char *argv[]){
    char *filename;                       /* Holds the file name*/
    uint8_t num_frames = MAX_FRAMES_;     /* Holds the number of frames*/
    uint8_t algorithm = DEFAULT_ALGO_;    /* Holds the alogrithm*/
    TLBTable* tlb_table = NULL;           // TLB unit
    PageTable* page_table = NULL;         // Page Table Unit
    Address* address = NULL;
    
    parseOptions(argc, argv, &filename, &num_frames, &algorithm);
    
    tlb_table = safeMalloc(sizeof(TLBTable));
    initTLBTable(tlb_table, MAX_TLB_SIZE_);

    page_table = safeMalloc(sizeof(PageTable));
    initPageTable(page_table, MAX_FRAME_SIZE_);

    address = safeMalloc(sizeof(Address));
    initAddress(address);

    // tables limit testing
    // tlb_table->num_entries = 10;
    // page_table->num_entries = 10;

    // tlb_table->num_entries = MAX_TLB_SIZE_;
    // page_table->num_entries = MAX_FRAME_SIZE_;

    // uint32_t test = 0x12345678;
    uint32_t test = 64815;
    uint16_t right_most_bits = maskFileInts(test);
    printf("mask right_most_bits: %0x | page #: %0x | offset: %0x \n", right_most_bits, 
                                                                    maskPageNum(right_most_bits), 
                                                                    maskOffset(right_most_bits));

    printf("mask right_most_bits (unmodified): %0x \n", right_most_bits);

    uint16_t set[3] = {0x1122, 0x2233, 0x3344};
    runAlgorithm(set, algorithm);

    // 0 is printDetails: for more details in later implementation
    if(verbosity){
        printTLBTableDebug(tlb_table, 0);
        printPageTableDebug(page_table, 0);
        printAddress(*address, 0);
    }

    return 0; 
}

