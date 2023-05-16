#include "options.h"
#include "safeutil.h"

uint8_t verbosity = 0;

/**
 * @fn	   : parseOptions
 * @brief  : This function parse the user input options. It checks for usage
 *            errors and will populate the filename, number of frams and the
 *            specified algorithm. It can also set a verbosity flag.
 *            
 * @retval : void
 * @parms  : int argc, char * const argv[], char **filename, 
 *           uint8_t *num_frames, uint8_t *algorithm
 */
void parseOptions(int argc, 
                  char * const argv[], 
                  char **filename, 
                  uint8_t *num_frames, 
                  uint8_t *algorithm)
{
    char *prog_name = argv[PROG_NAME_LOC_];
    /* Check for usage errors*/
    if(argc < MIN_ARGS_)
        usage(prog_name);

    if(argc > MIN_ARGS_ && (tolower(argv[VERBOSITY_LOC_][0]) == 'v' || 
                            tolower(argv[VERBOSITY_LOC_][1]) == 'v')) /* Verbosity flag might be set*/
        verbosity = 1;
    else if (argc > MIN_ARGS_)
        usage(prog_name);
    
    *filename = argv[FILE_NAME_LOC_];
    *num_frames = getFrameNum(argv[FRAME_NUM_LOC_]);
}


/**
 * @fn	   : getFrameNum
 * @brief  : This function will check and convert the user input frame number
 *            from a string to a uint8_t.
 *            
 * @retval : void
 * @parms  : char *frame_num
 */
uint8_t getFrameNum(char *frame_num)
{
    long ret = 0;
    
    if(!frame_num) /* Somehow frame_num string is null*/
        errorout("Set Frame Number: Empty frame_num string.\n");
    
    ret = safeStrtol(frame_num);
        
    if(ret == 0 || ret > UINT8_MAX){/* Choose not to error. Use default on input error*/
        printf("Frame Number defaulted to 256\n");
        return DEFAULT_FRAMES_;
    } /* No valid Frame Number found*/
    
    return (uint8_t)ret;
}

/**
 * @fn	   : usage
 * @brief  : This function prints usage information and exits the program.
 *            
 * @retval : void
 * @parms  : char *prog_name
 */
void usage(char *prog_name)
{
    fprintf(stderr,
            "Usage: %s <reference-sequence-file.txt> <FRAMES> <PRA> [-v]\n",
            prog_name);
    exit(EXIT_FAILURE);
}


/**
 * @fn	   : errorout
 * @brief  : This function prints error information and exits the program.
 *            
 * @retval : void
 * @parms  : char *error
 */
void errorout(char *error)
{
    if(errno)
        perror(error);
    else
        fprintf(stderr, "%s", error);
    
    exit(EXIT_FAILURE);
}