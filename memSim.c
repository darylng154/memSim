#include "options.h"
#include "safeutil.h"
#include "address.h"
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

void tempInitAddressList(Address* list, int length)
{
    int i = 0;
    for(i = 0; i < length; i++)
    {
        initAddress(&list[i]);
    }
}

int main(int argc, char *argv[]){
    char *filename;                       /* Name of input file*/
    uint8_t num_frames = MAX_FRAMES_;     /* User specified number of frames*/
    Algorithm algorithm = DEFAULT_ALGO_;    /* User specified Algorithm*/
    FILE *file_ptr;                       /* Pointer to file*/
    // FILE *bin_ptr;                        /* Pointer to bin*/
    unsigned int address_count;           /* Number addresses found in file*/
    unsigned int *address_list = NULL;    /* List of addresses*/
    TLBTable* tlb_table = NULL;           // TLB unit
    PageTable* page_table = NULL;         // Page Table Unit
    Address* address = NULL;
    
    /* Get the user input from the terminal and perform checks*/
    parseOptions(argc, argv, &filename, &num_frames, &algorithm);

    file_ptr = safefOpen(filename); /* Open the user input file*/
    /* Read the file and return a list of addresses and address count*/
    readFile(file_ptr, &address_list, &address_count);
    fclose(file_ptr);

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

    uint32_t temp_set[5] = {16916, 62493, 30198, 53683, 18295};
    Address temp_address_list[5];
    tempInitAddressList(temp_address_list, 5);

    // ---------------------------- Testing Page Table ------------------------------------
    int i = 0;
    for(i = 0; i < 5; i++)
    {
        addAddress(&temp_address_list[i], temp_set[i]);
    }

    if(verbosity)
    {
        for(i = 0; i < 5; i++)
        {
            printAddress(temp_address_list[i], 0);
        }
    }

    // 0 is printDetails: for more details in later implementation
    if(verbosity){
        // printTLBTableDebug(tlb_table, 0);
        // printPageTableDebug(page_table, 0);
        // printAddress(*address, 0);
    }

    return 0; 
}
