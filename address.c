#include "address.h"
#include "safeutil.h"

void initAddress(Address* address)
{
    address->address = 0;
    address->page_num = 0;
    address->offset = 0;

    address->byte_referenced = 0;
    address->frame_num = 0;

    int i = 0;
    for(i = 0; i < MAX_FRAME_SIZE_; i++)
    {
        address->frame[i] = 0;
    }
}

void initAddressTable(AddressTable** address_table, uint32_t length)
{
    *address_table = (AddressTable*) safeMalloc(sizeof(AddressTable));
    (*address_table)->list = (Address*) safeMalloc(sizeof(Address) * length);
    (*address_table)->num_entries = 0;

    int i = 0;
    for(i = 0; i < length; i++)
    {
        initAddress(&(*address_table)->list[i]);
    }
}

void printBuffer(char* buffer, int length)
{
    int i;

    if(buffer == NULL)
    {
        perror("#ERROR: buffer empty!!!!");
        exit(1);
    }

    for(i = 0; i < length; i++)
    {
        if(i % 32 == 0  && i != 0)
        {
            printf("\n");

            if(i % 256 == 0)   // for spacing octets
                printf("\n");
        }

        // printf("[%02d] 0x%0X ('%c')\t", i, buffer[i], (char)buffer[i]);
        printf("%02X ", buffer[i]);
    }
    printf("\n");
}

void printAddressFrame(const uint8_t* frame)
{
    int i = 0;
    for(i = 0; i < MAX_FRAME_SIZE_; i++)
    {
        if(i % 5 == 0)
            printf("\n");

        printf("frame[%-3i]: %u | ", i, frame[i]);
    }
    printf("\n");
}

void printAddress(const Address address, uint8_t printFrame)
{
    printf("address: 0x%04x (%-4i) | page_num: 0x%02x (%-3i) | offset: 0x%02x (%-3i) | byte_referenced: 0x%02x (%-3i) | frame_num: 0x%02x (%-3i) ", 
    address.address, address.address, address.page_num, address.page_num, address.offset, address.offset, address.byte_referenced, address.byte_referenced, address.frame_num, address.frame_num);

    if(printFrame)
        printAddressFrame(address.frame);
    else
        printf("\n");
}

void printAddressTable(const AddressTable* address_table, uint8_t printFrame)
{
    int i = 0;
    for(i = 0; i < address_table->num_entries; i++)
    {
        printf("AddressTable[%-3i] | ", i);
        printAddress(address_table->list[i], printFrame);
    }
}

void addAddress(Address* address, const uint32_t logical_address)
{
    uint16_t right_most_bits = maskLogicalAddress(logical_address);

    address->address = logical_address;
    address->page_num = maskPageNum(right_most_bits);
    address->offset = maskOffset(right_most_bits);
}

void addAddressToTable(AddressTable* address_table, const uint32_t logical_address)
{
    addAddress(&address_table->list[address_table->num_entries], logical_address);
    address_table->num_entries++;
}

void parseToAddressTable(AddressTable* address_table, const unsigned int* address_list, int length)
{
    int i = 0;
    for(i = 0; i < length; i++)
    {
        addAddressToTable(address_table, address_list[i]);
    }
}

// masks the 32-bit logical address to get the 16 rightmost bits (= 8-bit page #, 2 8-bit page offset)
uint16_t maskLogicalAddress(const uint32_t logical_address)
{
    uint16_t result = 0;
    uint32_t mask = 0x0000FFFF;

    result = logical_address & mask;

    return result;
}

// masks the 16 rightmost bits to get the left 8-bits = page #
uint8_t maskPageNum(const uint16_t right_most_bits)
{
    uint8_t result = 0;
    uint16_t mask = 0x0000FF00;

    result = (right_most_bits & mask) >> 8;

    return result;
}

// masks the 16 rightmost bits to get the left 8-bits = page #
uint8_t maskOffset(const uint16_t right_most_bits)
{
    uint8_t result = 0;
    uint16_t mask = 0x000000FF;

    result = right_most_bits & mask;

    return result;
}

void runAlgorithm(AddressTable* address_table, Algorithm algorithm)
{
    switch(algorithm)
    {
        case FIFO:
            if(verbosity)
                printf("Running FIFO\n");
            break;

        case LRU:
            if(verbosity)
                printf("Running LRU\n");
            break;

        case OPT:
            if(verbosity)
                printf("Running OPT\n");
            break;

        default:
            perror("#ERROR: runAlgorithm defaulted, %u does not exist\n");
            exit(EXIT_FAILURE);
            break;
    }
}

/* I think this function is a bit confused on what we are wanting to do. 
    Here we want to populate the frame but the frame number doesn't get set
    until the page is added to the page table. It would seem wrong to add 
    every address entry with a frame number as the address might not have one
    until its it is added to the page or swapped in.

    => yeah we need to update the code so we set the frame num when we change the page_table

   My thoughts are change the name of address_table's frame[] variable
    to page_data[]. This would make sense since the page data comes from the
    bin file and we wont have to worry about the frame_num. Plus we are using the
    page number to parse into the bin buffer.
    Also we might conside changing the function name to populate PageData 
    */
void populateFrames(AddressTable* address_table, char* bin_buffer)
{
    // implement reading data from bin for the specified 
    int i = 0;
    // int index = 0;
    for(i = 0; i < address_table->num_entries; i++)
    {
        address_table->list[i].frame_num = i;
        memcpy(address_table->list[i].frame, 
               bin_buffer + (address_table->list[i].page_num * MAX_FRAME_SIZE_), 
               MAX_FRAME_SIZE_);

        if(verbosity)
        {
            printf("Address frame: %u | Address Data: \n",
                   address_table->list[i].frame_num);
            printBuffer(address_table->list[i].frame, MAX_FRAME_SIZE_);
            printf("Bin Data: ");
            printBuffer(bin_buffer[address_table->list[i].page_num * MAX_FRAME_SIZE_], 
                        MAX_FRAME_SIZE_);

            printAddressTable(address_table, address_table->num_entries);
        }
    }
    return;
}