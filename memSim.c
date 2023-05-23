#include "options.h"
#include "safeutil.h"
#include "address.h"
#include "tlb.h"
#include "page_table.h"


#define BUFFER_     1024
#define BIN_BUFFER_ 65536
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
        if(address != '\n')
            addresses[running_count++] = address; /* Store address*/
            // running_count++; /* Increment running address counter*/
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

void readBin(FILE *bin_fptr, char **bin_buffer, unsigned int *bin_size){
    char *buffer;
    int size, i, c;
    
    size = BIN_BUFFER_;

    buffer = (char *)safeMalloc(size * sizeof(char));

    errno = 0;
    while(EOF != (c = fgetc(bin_fptr))){
        if(i >= size - 2){/* Reached end of buffer. Get more space*/
            buffer = (char *)safeRealloc(bin_fptr, size + BIN_BUFFER_, size, sizeof(char));
            size += BIN_BUFFER_;
        }
        buffer[i++] = c;
    }

    if(i > 0)
        buffer[i] = '\0';
    else if(i == 0 && c == EOF){
        free(buffer);
        *bin_buffer = NULL;
        return;
    }
    else
        errorout("Something went wrong with readBin.\n");
    if(buffer) /* Shrink buffer if not NULL*/
        buffer = (char *)safeRealloc(buffer, (i + 1), size, sizeof(char));
    
    memcpy(bin_buffer, buffer, i + 1);
    free(buffer);
    
    return;
}


// Addresses *populateAddresses(Address *address, unsigned int *address_list, unsigned int address_count){
//     int i;

    // for(i = 0; i < address_count; i++){
    //     addresses_info[i]. = address_count
    // }

// }


int main(int argc, char *argv[]){
    char *filename;                       /* Name of input file*/
    uint8_t num_frames = MAX_FRAMES_;     /* User specified number of frames*/
    Algorithm algorithm = DEFAULT_ALGO_;  /* User specified Algorithm*/
    FILE *file_ptr;                       /* Pointer to file*/
    FILE *bin_fptr;                        /* Pointer to bin*/
    char *bin_file_path = "bin/BACKING_STORE.bin";
    char *bin_buffer = NULL;              /* Bin buffer*/
    unsigned int bin_size = 0;            /* Size of backing store*/
    unsigned int address_count;           /* Number addresses found in file*/
    unsigned int *address_list = NULL;    /* List of addresses*/
    TLBTable* tlb_table = NULL;           // TLB unit
    PageTable* page_table = NULL;         // Page Table Unit
    Address* address = NULL;
    Addresses *addresses_info = NULL;     /* List of address struct's info*/

    /* Get the user input from the terminal and perform checks*/
    parseOptions(argc, argv, &filename, &num_frames, &algorithm);

    file_ptr = safefOpen(filename); /* Open the user input file*/
    /* Read the file and return a list of addresses and address count*/
    readFile(file_ptr, &address_list, &address_count);
    fclose(file_ptr);

    bin_fptr = safefOpen(bin_file_path);
    readBin(bin_fptr, &bin_buffer, &bin_size);

    printf("Bin: %s", bin_buffer);

    initAddresses(addresses_info, address_count);
    addresses_info->num_entries = address_count;
    // addresses_info->addresses   = populateAddresses(addresses_info, 
                                                    // address_list, 
                                                    // address_count);

    tlb_table = safeMalloc(sizeof(TLBTable));
    initTLBTable(tlb_table, MAX_TLB_SIZE_);

    page_table = safeMalloc(sizeof(PageTable));
    initPageTable(page_table, MAX_FRAME_SIZE_);

    // address = safeMalloc(sizeof(Address));
    // initAddress(address);

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
