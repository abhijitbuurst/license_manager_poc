#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <ctype.h>

#if defined (WIN32)
#include <windows.h>
#else
#include <unistd.h>
#include <dlfcn.h>
#endif

#include "license_manager_interface.h"
#include "../northbound/init.h"


int 
main(
int		argc,
char	*argv[]
)
{
    fprintf(stdout, "Enter into Testing of License Manager");

    // Read Cobstant Params and Create XML
    char			libPath[] = "/root/poc/license_manager_poc/ShaferFilechck.so";

    void			*libHandle = NULL;

    // Initialized Zentitle Library
    setup_licensing_server(libPath, &libHandle);
    // Display input command prompt
    fprintf(stdout, "Exit from Testing of License Manager\n");

}

int 
setup_licensing_server(
char            *libPath,
void            **libHandle
)
{
    int			retVal =0;
    fprintf(stdout, "Enter into Testing of Setup License Manager\n");

	retVal = SetupLib(libPath, libHandle);
    fprintf(stdout, "Exit from Testing of Setup License Manager\n");

    return retVal;
}