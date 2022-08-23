
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
#include "configuration.h"

#define clear() printf("\033[H\033[J")
#define gotoXY(x,y) printf("\033[%u;%uH", (y), (x))

//Optional registration information that will be sent in with
// any license request (full license or trial). The format
// of the XML can be any valid xml snippet.  Normally this info
// would be entered by your end user.
#define		XMLREGINFO		"<telephone>1234567890</telephone>"\
							"<name>ABhijit Patil</name>"



char			*nslVersion = NULL;
char			*nsaVersion = NULL;
char			*compID = NULL;
char			*cert = NULL;
char			*nslHostName = NULL;
char			*nsaHostName = NULL;
char			*authentication = NULL;
char			*specialID = NULL;
char			*licenseInfo = NULL;
char			*expDate = NULL;
char			*expLease = NULL;
int32_t			*licenseStatus = NULL;
char			*tmpStr = NULL;
int				offlineState = 0;

char			*miscStr1 = NULL;
char			*miscStr2 = NULL;


unsigned int			TIMETOCHECKLICENSE = 0;
int			    retVal;
char			*xmlParams;
unsigned int	security;
int				offset;

//Information about type of license and how it was activated
uint32_t				licenseType;
uint32_t				actType;

//Use a NULL license code for a trial license, specify a license
// code for a full license.
char					licensecode[20];

//Example registration data for use with NSL
char					*xmlRegInfo = NULL;

void			*libHandle = NULL;

void
menu()
{

	// while(1){
		int choice=0;
		fprintf(stdout, "1) Set License code\n");

		fprintf(stdout, "2) Get License Status\n");

		fprintf(stdout, "3) Exit\n");

		scanf("%d",&choice);

		switch (choice)
		{
		case 1:
			xmlRegInfo = strdup(XMLREGINFO);

			fprintf(stdout, "Enter the License Code\n");
			// Read License Code
			scanf("%s",licensecode);
			fprintf(stdout, "----------------------------\n");

			//This function checks for errors (return from library call < 0) and
			// invalid license statuses (status <= 0).  If a valid license is
			// not present it attempts to retrieve one from the server.
			// printf("%s\n",licensecode);

			retVal = SetLicenseStatus(&libHandle,  &licensecode[0],
						licenseStatus, &licenseType, &actType, xmlRegInfo);

			if (retVal < 0)
			{
				fprintf(stderr, "Error while setting the License Code\n");

			}
			fprintf(stdout, "===========================================\n");

			break;
		
		case 2:

			fprintf(stdout, "Fetching the License details\n");
			GetLicenseStatus(&libHandle);
			fprintf(stdout, "===========================================\n");
			break;


		default:
			return;
		}
	// }
}

int 
main(
int		argc,
char	*argv[]
)
{
    fprintf(stdout, "Enter into Testing of License Manager\n");

    // Read Cobstant Params and Create XML


    // Initialized Zentitle Library
    setup_licensing_server(libPath, &libHandle, custID, prodID, xauth, yauth, zauth);
    // Display input command prompt

	menu();
    fprintf(stdout, "Exit from Testing of License Manager\n");

    return 0;
}

