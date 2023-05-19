#include "options.h"
#include "safeutil.h"
#include "tlb.h"
#include "page_table.h"


#define BUFFER_ 1024

/**
 * @fn	   : readFile
 * @brief  : 
 * @retval : void
 * @parms  : FILE *file_ptr, unsigned int *address_list, unsigned int *num_addresses
 */
void readFile(FILE *file_ptr, unsigned int **address_list, unsigned int *address_count){
    //int i;
    unsigned int address;
    uint16_t running_count = 0;
    size_t buffer_size = BUFFER_;
    unsigned int *addresses = NULL;

    addresses = (unsigned int *)safeMalloc(sizeof(unsigned int) * buffer_size); /* Make space*/
    // addresses = (unsigned int *)calloc(buffer_size, buffer_size)

    errno = 0;
    while(!feof(file_ptr)){ /* While not at EOF*/
        fscanf(file_ptr, "%u", &address); /* Scan for addresses*/
        if(verbosity)
            printf("Address: %u\n", address);
        addresses[running_count] = address; /* Store address*/
        running_count++; /* Increment running address counter*/
        if(running_count > 1248)
            printf("Near broken spot");
        if(running_count >= (buffer_size - 1)){ /* Need more space*/
            addresses = (unsigned int *)safeRealloc(addresses, buffer_size + BUFFER_, buffer_size, sizeof(unsigned int));
            buffer_size += BUFFER_; /* Increase buffer size*/
        }
    }
    
    if(errno) /* Something went wrong*/
        errorout("ReadFile failed.\n");

    if(verbosity)
        printf("Total number of addresses: %u\n", running_count);
    *address_count = running_count; /* Save job count*/

    *address_list = (unsigned int *)safeMalloc(sizeof(unsigned int) * running_count);

    memcpy(*address_list, addresses, sizeof(unsigned int) * running_count);

    free(addresses);
    return;
}

// masks the 32-bit logical address to get the 16 rightmost bits (= 8-bit page #, 2 8-bit page offset)
uint16_t maskFileInts(uint32_t logical_address)
{
    uint16_t result = 0;
    uint32_t mask = 0x0000FFFF;

    result = logical_address & mask;

    return result;
}

// masks the 16 rightmost bits to get the left 8-bits = page #
uint8_t maskPageNum(uint16_t right_most_bits)
{
    uint8_t result = 0;
    uint16_t mask = 0x0000FF00;

    result = (right_most_bits & mask) >> 8;

    return result;
}

// masks the 16 rightmost bits to get the left 8-bits = page #
uint8_t maskOffset(uint16_t right_most_bits)
{
    uint8_t result = 0;
    uint16_t mask = 0x000000FF;

    result = right_most_bits & mask;

    return result;
}

void runAlgorithm(uint16_t* right_most_bits, uint8_t algorithm)
{
    switch(algorithm)
    {
        case 0:
            if(verbosity)
                printf("Running FIFO\n");
            break;

        case 1:
            if(verbosity)
                printf("Running LRU\n");
            break;

        case 2:
            if(verbosity)
                printf("Running OPT\n");
            break;

        default:
            perror("#ERROR: runAlgorithm defaulted, %u does not exist\n");
            exit(EXIT_FAILURE);
            break;
    }
}

int main(int argc, char *argv[]){
    char *filename;                       /* Name of input file*/
    uint8_t num_frames = MAX_FRAMES_;     /* User specified number of frames*/
    uint8_t algorithm = DEFAULT_ALGO_;    /* User specified Algorithm*/
    FILE *file_ptr;                       /* Pointer to file*/
    unsigned int address_count;           /* Number addresses found in file*/
    unsigned int *address_list = NULL;    /* List of addresses*/
    TLBTable* tlb_table = NULL;           // TLB unit
    PageTable* page_table = NULL;         // Page Table Unit
    
    /* Get the user input from the terminal and perform checks*/
    parseOptions(argc, argv, &filename, &num_frames, &algorithm);

    file_ptr = safefOpen(filename);
    /* Read the file and return a list of addresses and address count*/
    readFile(file_ptr, &address_list, &address_count);

    tlb_table = safeMalloc(sizeof(TLBTable));
    initTLBTable(tlb_table, MAX_TLB_SIZE_);

    page_table = safeMalloc(sizeof(PageTable));
    initPageTable(page_table, MAX_FRAME_);

    // tlb_table->num_entries = 10;
    // page_table->num_entries = 10;

    // tlb_table->num_entries = MAX_TLB_SIZE_;
    // page_table->num_entries = MAX_FRAME_;

    uint32_t test = 0x12345678;
    uint16_t right_most_bits = maskFileInts(test);
    printf("mask right_most_bits: %0x | page #: %0x | offset: %0x \n", right_most_bits, 
                                                                    maskPageNum(right_most_bits), 
                                                                    maskOffset(right_most_bits));

    uint16_t set[3] = {0x1122, 0x2233, 0x3344};
    runAlgorithm(set, algorithm);

    // 0 is printDetails: for more details in later implementation
    if(verbosity){
        printTLBTableDebug(tlb_table, 0);
        printPageTableDebug(page_table, 0);
    }

    return 0; 
}
