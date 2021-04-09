#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

void errorCheck(int retVal)
{
    if(retVal == -1)
    {
        perror("Error:");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char* argv[] , char* environp[])
{
        int opt;
            puts("Options:");
           while ((opt = getopt(argc, argv, "nt:")) != -1) {
               switch (opt)
               {
               case 'n':
                   printf("%c\n",opt);
                   break;
               case 't':
                   printf("%c = %s\n",opt,optarg);
                   break;
               
               default:/* ? */ //Invalid option or an option with a missing argument
                   break;
               }
           }
           puts("Remaining cmdline args");
           for(int i = optind ; i < argc ; ++i  )
           {
               puts(argv[i]);
           }
    
    return EXIT_SUCCESS;
}