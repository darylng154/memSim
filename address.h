#ifndef ADDRESS_H
#define ADDRESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "options.h"

typedef struct address Address;
typedef struct addresses Addresses;

struct address
{
    uint32_t address;
    uint8_t page_num;
    uint8_t offset;

    int byte_referenced;
    uint8_t frame_num;
    uint8_t frame[MAX_FRAME_SIZE_];
};

struct addresses
{
    uint32_t num_entries;
    Address* addresses;
};

void initAddress(Address* address);
void initAddresses(Addresses* addresses, uint32_t length);
void printAddressFrame(const uint8_t* frame);
void printAddress(const Address address, uint8_t printFrame);
void printAddresses(const Addresses* addresses, uint8_t printFrame);
uint16_t maskFileInts(const uint32_t logical_address);
uint8_t maskPageNum(const uint16_t right_most_bits);
uint8_t maskOffset(const uint16_t right_most_bits);
void runAlgorithm(uint16_t* right_most_bits, uint8_t algorithm);

#endif