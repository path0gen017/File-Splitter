#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <getopt.h>
#include "errorUtils.h"

static char buffer[BUFSIZ];
#define BUF_SIZE        1024
#define EXTENSION_LEN   4





void
openDestFiles(char* baseName , int * dest_fds, int filesNum);

void 
closeDestFiles(int * dest_fds , int filesNum);

int main(int argc , char* argv[] , char* environp[])
{
    int src_fd ,filesNum, *dest_fds, opt;
    struct stat srcfile_stat;
    ssize_t rbytes , wbytes;
    size_t fileLen = BUFSIZ;

    if(argc < 2)
        usageErr();

    while( (opt = getopt(argc , argv , "s:")) != -1)
    {
        switch (opt)
        {
        case 's':
            fileLen = (size_t) atol(optarg);
            break;
        
        default:
            usageErr();
            break;
        }
    }

    errorExit( stat(argv[optind] , &srcfile_stat) , "stat()" );


    /**
     * The number of files is computed as file_size/buffer_size + 1 extra file 
     * to hold the remaining bytes if there are any 
     */
    filesNum = ( srcfile_stat.st_size / fileLen ) +
               ( srcfile_stat.st_size % fileLen == 0 ? 0 : 1 );
    
    dest_fds = (int*)malloc(filesNum * sizeof(int) );
    errorExit(dest_fds == NULL ? FAILED : SUCCESS , "malloc(filesNum)");


    src_fd = open(argv[optind] , O_RDONLY );
    errorExit(src_fd , "open(src_fd):");

    openDestFiles(argv[optind] , dest_fds , filesNum);                   //Open all the filesNum destination files

    int index = 0;                                                  //A variable to be used to index into the file descriptors array
    while( (rbytes = read(src_fd , buffer , fileLen)) )
    {
        errorExit(rbytes, "read(src_fd):");

        wbytes = write(dest_fds[index++] , buffer , rbytes);
        errorExit(wbytes != rbytes ? FAILED : SUCCESS , "write():");        

    }
    


    //House-keeping
    errorExit( close( src_fd ) , "close(src_fd):");
    closeDestFiles(dest_fds,filesNum);
    free(dest_fds);
    
    return EXIT_SUCCESS;
}

void
openDestFiles(char* baseName, int * dest_fds, int filesNum)
{
    
    char buf[BUF_SIZE];
    sprintf(buf,"%d",filesNum);             //Convert the int filesNum to a string and store it in buf

    int baseNameLen = strlen(baseName) , fileDigits = strlen(buf) ,fd;

    char* name = (char*)malloc( baseNameLen + fileDigits + EXTENSION_LEN + 1); //Holds the string that will represent a particular filename
    errorExit(name == NULL ? FAILED : SUCCESS , "malloc(strlen(baseName) + strlen(buf)):");

    strcpy(name,baseName);

    for(int i = 0 ; i < filesNum ; ++i)
    {
        sprintf(buf,"%d",i) ;               //Convert index i to a string

        //The two function calls below construct the file name
        strcat( name , buf) ;
        strcat( name , ".prt");

        fd = open(name , O_CREAT | O_TRUNC | O_WRONLY , S_IRUSR | S_IWUSR );
        errorExit(fd,"open(.prt):");

        *( dest_fds + i ) = fd;

        //Reset the constructed name to its initial state i.e. a '\0' following the baseName
        name[baseNameLen] = '\0';
    }

    free(name);
}

void 
closeDestFiles(int * dest_fds , int filesNum)
{
    for (int i = 0; i < filesNum; i++)
        errorExit( close(dest_fds[i]) , "close(destination files):");
}