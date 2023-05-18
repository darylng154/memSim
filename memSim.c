#include "options.h"
#include "safeutil.h"
#include "tlb.h"
#include "page_table.h"

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

    // 0 is printDetails: for more details in later implementation
    if(verbosity){
        printTLBTableDebug(tlb_table, 0);
        printPageTableDebug(page_table, 0);
    }

    return 0; 
}

