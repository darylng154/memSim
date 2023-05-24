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
        if(!feof(file_ptr)){
            if(verbosity)
                printf("Address: %u\n", address);
            addresses[running_count++] = address; /* Store address*/
        }
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
    char *buffer = NULL;
    int size, i, c;
    
    size = BIN_BUFFER_;

    buffer = (char *)safeMalloc(size * sizeof(char));

    errno = 0;
    while(EOF != (c = fgetc(bin_fptr))){
        if(i >= size - 2){/* Reached end of buffer. Get more space*/
            buffer = (char *)safeRealloc(buffer, size + BIN_BUFFER_, size, sizeof(char));
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
    
    *bin_buffer = (char *)safeMalloc(sizeof(char) * i);

    memcpy(*bin_buffer, buffer, sizeof(char) * i);
    *bin_size = i;
    if(verbosity)
        printf("Bin size: %d\n", *bin_size);
    free(buffer);
    
    return;
}

int main(int argc, char *argv[]){
    char *filename;                       /* Name of input file*/
    uint8_t num_frames = MAX_FRAMES_;     /* User specified number of frames*/
    Algorithm algorithm = DEFAULT_ALGO_;  /* User specified Algorithm*/
    FILE *file_ptr;                       /* Pointer to file*/
    FILE *bin_fptr;                        /* Pointer to bin*/
    char *bin_file_path = "BACKING_STORE.bin";
    char *bin_buffer = NULL;              /* Bin buffer*/
    unsigned int bin_size = 0;            /* Size of backing store*/
    unsigned int address_count;           /* Number addresses found in file*/
    unsigned int *address_list = NULL;    /* List of addresses*/
    TLBTable* tlb_table = NULL;           // TLB unit
    PageTable* page_table = NULL;         // Page Table Unit
    AddressTable* address_table = NULL;

    /* Get the user input from the terminal and perform checks*/
    parseOptions(argc, argv, &filename, &num_frames, &algorithm);

    file_ptr = safefOpen(filename); /* Open the user input file*/
    /* Read the file and return a list of addresses and address count*/
    readFile(file_ptr, &address_list, &address_count);
    fclose(file_ptr);

    bin_fptr = safefOpen(bin_file_path);
    readBin(bin_fptr, &bin_buffer, &bin_size);
    int j;

    int count = 0;
    /* For testing*/
    printf("BIN: \n");
    // for(j = 0; j < 1023; j++, count++){
    //     if(count == 255){
    //         printf("\n\n");
    //         count = 0;
    //     }
    //     printf("%08x ", bin_buffer[j]);
    // }

    printf("\n");
    tlb_table = safeMalloc(sizeof(TLBTable));
    initTLBTable(tlb_table, MAX_TLB_SIZE_);

    page_table = safeMalloc(sizeof(PageTable));
    initPageTable(page_table, MAX_FRAME_SIZE_);

    // address = safeMalloc(sizeof(Address));
    // initAddress(address);

    initAddressTable(&address_table, address_count);
    parseToAddressTable(address_table, address_list, address_count);

    // 0 is printDetails: for more details in later implementation
    if(verbosity){
        // printTLBTableDebug(tlb_table, 0);
        // printPageTableDebug(page_table, 0);
        // printAddress(*address, 0);
        printAddressTable(address_table, 0);
    }

    fclose(bin_fptr);
    free(bin_buffer);
    free(address_list);
    free(tlb_table);
    free(page_table);
    free(address_table->list);
    free(address_table);
    return 0; 
}
