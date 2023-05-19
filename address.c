#include "address.h"

void initAddress(Address* address)
{
    address->address = 0;
    address->byte_referenced = 0;
    address->frame_num = 0;

    int i = 0;
    for(i = 0; i < MAX_FRAME_SIZE_; i++)
    {
        address->frame[i] = 0;
    }
}

void initAddresses(Addresses* addresses, uint32_t length)
{
    addresses->num_entries = 0;

    int i = 0;
    for(i = 0; i < length; i++)
    {
        initAddress(&addresses->addresses[i]);
    }
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
    printf("address: %05x | byte_referenced: %-3i | frame_num: %-3i ", address.address, address.byte_referenced, address.frame_num);

    if(printFrame)
        printAddressFrame(address.frame);
    else
        printf("\n");
}

void printAddresses(const Addresses* addresses, uint8_t printFrame)
{
    int i = 0;
    for(i = 0; i < addresses->num_entries; i++)
    {
        printAddress(addresses->addresses[i], printFrame);
    }
}

// masks the 32-bit logical address to get the 16 rightmost bits (= 8-bit page #, 2 8-bit page offset)
uint16_t maskFileInts(const uint32_t logical_address)
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