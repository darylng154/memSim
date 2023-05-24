#ifndef ADDRESS_H
#define ADDRESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "options.h"

typedef struct address Address;
typedef struct address_table AddressTable;

struct address
{
    uint32_t address;
    uint8_t page_num;
    uint8_t offset;

    int byte_referenced;
    uint8_t frame_num;
    uint8_t frame[MAX_FRAME_SIZE_];
};

struct address_table
{
    uint32_t page_faults;
    
    uint32_t num_entries;
    Address* list;
};

void initAddress(Address* address);
void initAddressTable(AddressTable** address_table, uint32_t length);
void printBuffer(char* buffer, int length);
void printAddressFrame(const uint8_t* frame);
void printAddress(const Address address, uint8_t printFrame);
void printAddressTable(const AddressTable* address_table, uint8_t printFrame);
void addAddress(Address* address, const uint32_t logical_address);
void addAddressToTable(AddressTable* address_table, const uint32_t logical_address);
void parseToAddressTable(AddressTable* address_table, const unsigned int* address_list, int length);
uint16_t maskLogicalAddress(const uint32_t logical_address);
uint8_t maskPageNum(const uint16_t right_most_bits);
uint8_t maskOffset(const uint16_t right_most_bits);
void runAlgorithm(AddressTable* address_table, Algorithm algorithm);

#endif