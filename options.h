#ifndef OPT_H
#define OPT_H

#include <stdint.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define MIN_ARGS_       3
#define PROG_NAME_LOC_  0
#define FRAME_NUM_LOC_  2
#define VERBOSITY_LOC_  4
#define FILE_NAME_LOC_  1
#define DEFAULT_FRAMES_ (256 - 1)
#define DEFAULT_ALGO_   FIFO

typedef enum {FIFO, LRU, OPT, INVALID = 255} algorithm;

extern uint8_t verbosity;

void parseOptions(int argc, char * const argv[], char **filename, uint8_t *num_frames, uint8_t *algorithm);
uint8_t getFrameNum(char *frame_num);
void usage(char *prog_name);
void errorout(char *error);

#endif