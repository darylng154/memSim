#include "options.h"

int main(int argc, char *argv[]){
    char *filename;                       /* Holds the file name*/
    uint8_t num_frames = DEFAULT_FRAMES_; /* Holds the number of frames*/
    uint8_t algorithm = DEFAULT_ALGO_;    /* Holds the alogrithm*/
    
    parseOptions(argc, argv, &filename, &num_frames, &algorithm);
    
    return 0;
}

