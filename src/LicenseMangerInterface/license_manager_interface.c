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
#include "src/northbound/init.h"


// int 
// main(
// int		argc,
// char	*argv[]
// )
// {
//     fprintf(stdout, "Enter into Testing of License Manager");

//     // Read Cobstant Params and Create XML
//     char			libPath[] = "/root/poc/license_manager_poc/ShaferFilechck.so";

//     void			*libHandle = NULL;

//     // Initialized Zentitle Library
//     setup_licensing_server(libPath, &libHandle);
//     // Display input command prompt
//     fprintf(stdout, "Exit from Testing of License Manager\n");

// }

int 
setup_licensing_server(
char            *libPath,
void            **libHandle,
int				custID,
int				prodID,
unsigned int	xauth,
unsigned int	yauth,
unsigned int	zauth
)
{
    int			retVal =0;
    fprintf(stdout, "Enter into Testing of Setup License Manager\n");

    // Set Config parameter
    setConfigParameter(custID, prodID, xauth, yauth, zauth, libPath);

	retVal = SetupLib();
    if (retVal < 0)
	{
		return -1;
	}
    retVal = outInformation(libPath);
    fprintf(stdout, "Exit from Testing of Setup License Manager\n");
    return retVal;
}

int
SetLicenseStatus(
void		    **libHandle,
char			*licenseCode,
int32_t			*licenseStatus,
uint32_t		*licenseType,
uint32_t		*actType,
char			*xmlRegInfo
)
{
    int retVal;
	// fprintf(stdout, "Input %p %s\n", *libHandle, licenseCode);


    retVal = checkLicenseStatus(libHandle, licenseCode,
            licenseStatus, licenseType, actType, xmlRegInfo);

    return retVal;
}


int
GetLicenseStatus(
void		    **libHandle
)
{
    int retVal;
	// fprintf(stdout, "Input %p\n", *libHandle);

	retVal = GetLicenseForCurrentUser(libHandle);

    return retVal;
}
