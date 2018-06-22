/*Platform-Specific Headers*/
#ifdef LINUX
#include "linuxHandling.h"
#else
#include "windowsHandling.h"
#endif

#include "selectServer.h"

int main(int argc, char *argv[])
{
    int opt = 0;

    /*Specifying expected options here*/
    /*Consider moving to header file*/
    static struct option long_options[] =
    {
        {"verbose",     no_argument,    0,  'v' },
    };

    int long_index = 0;

    while ((opt = getopt_long(argc, argv,"v", long_options, &long_index)) != -1)
    {
        switch (opt) 
        {
            case 'v' :
                printf("Verbose mode activated");
                break;
            default: printUsage();
                exit(0);
        }
    return 0;
    }
}

void printUsage()
{
    printf("Select Server Usage: \n");
    printf("-v for verbose mode\n");
    printf("-p with number for port specification\n");
    printf("-i with IP address for specfic IP address\n");
}
