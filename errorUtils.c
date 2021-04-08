#include "errorUtils.h"

void
errorExit(int retVal,const char* msg)
{
    if( retVal == -1)
    {
        perror(msg);
        exit(EXIT_FAILURE);
    }
}

void
usageErr()
{
    fprintf(stderr,"Usage: (./)split [OPTIONS] src_file\n");
    exit(EXIT_FAILURE);
}