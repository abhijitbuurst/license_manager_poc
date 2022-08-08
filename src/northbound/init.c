
#include <stdlib.h>

#if ! defined (WIN32)
#include <unistd.h>
#include <signal.h>
#include <dlfcn.h>
#include <sys/time.h>
#else
#include <Windows.h>
#pragma comment(lib, "Winmm.lib")
#endif

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <inttypes.h>

#include "init.h"

//Simple signal handler to catch alarm and set a flag indicating that
// we should check the license status.
#if ! defined (WIN32)
void
sig_handler(
int 		signo
)
{
	if (signo == SIGALRM)
	{
		TIMETOCHECKLICENSE=1;
	}

}
#else
void
CALLBACK
sig_handler(
UINT		uTimerID,
UINT		uMsg,
DWORD_PTR	dwUser,
DWORD_PTR	dw1,
DWORD_PTR	dw2
)
{
		TIMETOCHECKLICENSE=1;
}
#endif


int
SetupLib(
char            *libPath,
void            **libHandle
)
{
    int         retVal;
    struct stat stbuf;


    if (libPath == NULL)
    {
        return -1;
    }

	//Make sure library is present
    retVal = stat(libPath, &stbuf);

    if (retVal != 0)
    {
        fprintf(stderr, "The library %s does not exist "
            "or has incorrect permissions\n", libPath);
        return -1;
    }

	//Load the library
#if defined (WIN32)
    *libHandle = (void *)LoadLibraryA(libPath);
#else
    *libHandle = dlopen(libPath, RTLD_LOCAL | RTLD_LAZY);
#endif

    if (*libHandle == NULL)
    {
        fprintf(stderr, "libFileMgmt is corrupt\n");
        return -1;
    }

    return 0;
}


void *
GetSymbol(
void                *libHandle,
const char          *symName
)
{
    if (libHandle == NULL)
    {
        fprintf(stderr, "Invalild library handle\n");
        return NULL;
    }

#if ! defined (WIN32)
    return dlsym(libHandle, symName);
#else
    return GetProcAddress((HINSTANCE)(libHandle), symName);
#endif
}

//Construct the XML string for NapLibOpen.  Here the XML string is
// constructed by hand to keep the example simple and the
// dependencies small.  The following is horribly inefficient and
// shouldn't be used for real code.  I would recommend using an XML
// library like libxml2.
//
// Any of the following values can be left out in which case the
// following default values are used by the NSLLibrary
//
// logLevel = 0
// logQLen = 300
// cacheQLen = 25
// soapThMin = 10
// soapThMax = 10
// offlineMode = false
// workDir = current working dir on OSX/Linux; selected by OS on Win
// No proxy.
//
// The following parameters are available.
//<LogQLen>			- Length of log queue (300 default)
//<CacheQLen>		- Used only in NSA
//<SoapThreadsMax>	- Used only in NSA
//<SoapThreadsMin>	- Used only in NSA
//<LogLevel>		- debug level (0 default should be set <=4)
//<WorkDir>			- dir where NSA/NSL stores files and info
//<OfflineMode>		- Used only in NSA
//<ProxyIP>			- proxy IP address
//<ProxyPort>		- proxy Port
//<ProxyUsername>	- proxy username
//<ProxyPassword>	- proxy password
//<NSAEnabled>		- Enable NSA functionality
//<NSLEnabled>		- Enable NSL functionality
//<SecurityValue>	- Security value for offsets
//
// You may pass a NULL into NalpLibOpen to use the default values
int
constParams(
char				**xmlParams,
int					security,
char				*WorkDir,
char				*LogLevel
)
{
	int			xmlLen;
	char		secVal[25];

	//Start with enough room for
	// <?xml version=\"1.0\" encoding=\"UTF-8\"?> 42
	// <SHAFERXMLParams> 17
	//+ 1 (NULL Terminator)
	xmlLen = 60;

	*xmlParams = (char *)malloc(xmlLen);

	strcpy(*xmlParams, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	strcat(*xmlParams, "<SHAFERXMLParams>");

	addXMLParam(xmlParams, "NSLEnabled", "1");

	addXMLParam(xmlParams, "NSAEnabled", "1");

	addXMLParam(xmlParams, "WorkDir", WorkDir);

	addXMLParam(xmlParams, "LogLevel", LogLevel);

	//Enable the security offset feature.  That is, all returns from
	// the NSL library will have a security offset added to them.
	sprintf(secVal, "%i", security);
	addXMLParam(xmlParams, "SecurityValue", secVal);

	*xmlParams = (char *)
		realloc(*xmlParams, strlen(*xmlParams) + 18 + 1);	
	strcat(*xmlParams, "</SHAFERXMLParams>");

	return 0;	
}


int
addXMLParam(
char	**xmlParams,
char	*xmlName,
char	*xmlValue
)
{
	int			xmlLen;


	if (xmlValue == NULL)
	{
		return 0;
	}


	xmlLen = strlen(*xmlParams) +
		5 + 2 * strlen(xmlName) + strlen(xmlValue) + 1;

	*xmlParams = (char *)realloc(*xmlParams, xmlLen);

	strcat(*xmlParams, "<");
	strcat(*xmlParams, xmlName);
	strcat(*xmlParams, ">");

	strcat(*xmlParams, xmlValue);

	strcat(*xmlParams, "</");
	strcat(*xmlParams, xmlName);
	strcat(*xmlParams, ">");

	return 0;
}


/*
 * vim:tabstop=4
 * vim:shiftwidth=4
 */

