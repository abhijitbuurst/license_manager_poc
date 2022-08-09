
#include <stdlib.h>

#include "init.h"
#include "dsoSHAFER.h"
#include "dsoNSL.h"
#include "dsoNSA.h"


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
char			*licenseStatus = NULL;
char			*tmpStr = NULL;
int				offlineState = 0;



unsigned int			TIMETOCHECKLICENSE = 0;
int			    retVal;
char			*xmlParams;
unsigned int	security;
int				offset;
int				custID = 5381;
int				prodID = 100;

//An optional name that may be used to identify the client.  This name
// will appear in DataStream information.
char			clientName[] = "LicenseManagerPOC";


//If not using the Nalpeiron daemon use these values
char			daemonIP[] = "";
char			daemonPort[] = "";
char			daemonUser[] = "";
char			daemonPass[] = "";

//If using the Nalpeiron daemon set these to your local values
/*
char			daemonIP[] = "127.0.0.1";
char			daemonPort[] = "16700";
char			daemonUser[] = "admin";
char			daemonPass[] = "nalpeiron";
*/




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
	fprintf(stdout, "Input %p %s\n", libHandle, symName);

#if ! defined (WIN32)
    return dlsym(libHandle, symName);
#else
    return GetProcAddress((HINSTANCE)(libHandle), symName);
#endif
}


int
SetupLib(
char            *libPath,
void            **libHandle
)
{
    fprintf(stdout, "Enter into Testing of SetupLib\n");

	//These are the security values stamped into your library. They
	// should be changed to match your values.
	// NOTE When selecting yauth and zauth it is important that they
	// NOT be multiples of each other (for instance, yauth=300 zauth=600).
	// If they are multiples the resulting security offset will NOT be
	// random.
	unsigned int	xauth = 198;
	unsigned int	yauth = 819;
	unsigned int	zauth = 333;

	if ((yauth != 0) && (zauth != 0))
	{
		srand((unsigned)time(NULL));
		security = 1 + (unsigned int)(500.0 * rand() / (RAND_MAX + 1.0));
		offset = xauth + ((security * yauth) % zauth);
	}
	else
	{
		security = 0;
		offset = 0;
	}

	retVal = constParams(0, "4", "/tmp", clientName, security, &xmlParams);

	if (retVal < 0)
	{
		fprintf(stdout, "constParams failed: %d\n", retVal);
		return retVal;
	}

	retVal = initLib(libPath, libHandle);

	if (retVal < 0)
	{
		return retVal;
	}
    fprintf(stdout, "Exit: Testing of SetupLib\n");

	return retVal;
}

int
initLib(
char            *libPath,
void            **libHandle
)
{
	int			retVal =-1;

    fprintf(stdout, "Enter: initLib method");

	retVal = openLibrary(xmlParams, libPath, libHandle);

	free(xmlParams);

	if (retVal < 0)
	{
		fprintf(stdout, "Library open failed: %d\n", retVal);
		return retVal;
	}
    fprintf(stdout, "Enter: initLib method %d %d %p \n", custID, prodID, libHandle);

	retVal = dsoNSLValidateLibrary(custID, prodID, libHandle);
	retVal = retVal - offset;

	if (retVal != 0)
	{
		fprintf(stdout, "Library validation failed: %d\n", retVal);
		return retVal;
	}

	// retVal = getNSLLibraryInfo(offset, &nslVersion, &nsaVersion,
	// 			&compID, &nslHostName, &nsaHostName, libHandle);

	// if (retVal < 0)
	// {
	// 	fprintf(stdout, "Library Information failed: %d\n", retVal);
	// 	return retVal;
	// }

	// retVal = getLicenseInfo(offset, &offlineState, &authentication,
	// 			&licenseInfo, &licenseStatus, &expDate, &expLease,
	// 			&compID, libHandle);
    fprintf(stdout, "Exit:  initLib method");

	return 0;
}


int
openLibrary(
char				*xmlParams,
char				*libPath,
void				**libHandle
)
{
	int			retVal;
	struct stat	stbuf;

    fprintf(stdout, "Enter: openLibrary method\n");

	retVal = stat(libPath, &stbuf);

	if (retVal != 0)
	{
		fprintf(stderr, "Library %s does not exist or has incorrect "
				"permissions.\n", libPath);
		return -1;
	}

#if defined (WIN32)
    *libHandle = (void *)LoadLibraryA(libPath);
#else
    *libHandle = dlopen(libPath, RTLD_LOCAL | RTLD_LAZY);
#endif

	if (*libHandle == NULL)
	{
#if defined (WIN32)
		fprintf(stderr, "%s is corrupt: %d\n", libPath, GetLastError());
#else
		fprintf(stderr, "%s is corrupt: %s\n", libPath, dlerror());
#endif
		return -1;
	}

	retVal = dsoNalpLibOpen(xmlParams, *libHandle);

	if (retVal < 0)
	{
		fprintf(stderr, "Open library failed\n");
		return retVal;
	}
    fprintf(stdout, "Exit:  openLibrary method\n");

	return 0;
}


int
closeLibrary(
void			*libHandle
)
{
	int			retVal;


	retVal = dsoNalpLibClose(libHandle);

#if ! defined (WIN32)
    dlclose(libHandle);
#else
    FreeLibrary((HINSTANCE)libHandle);
#endif

	return retVal;
}

//More information about NalpLibOpen and its parameters may be found at 
// Nalpeiron or in the codekit at html/index.html.
//
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
//<LogLevel>		- Verbosity of logging (0 - 4)
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
//<HTTPSEnabled>	- Forces the library to use HTTPS to connect to Nalpeiron
//<WorkGroup>		- Group ownership for the WorkDir (log and licensing files, etc)
//<MaxLogSize>		- Max length in bytes of the log file
//<LicenseCode>		- The passive license "client id" for importation
//<SoapThreadsMin>	- Min Number of threads in the transaction pool for NSA
//<SoapThreadsMax>	- Min Number of threads in the transaction pool for NSA
//<ClientHostname>	- An optional identifier for the end user's system
//<NSLConnTO>		- Connection timeout in ms for NSL connections
//<NSLTransTO>		- Connection timeout in ms for NSL transactions
//<NSAConnTO>		- Connection timeout in ms for NSA connections
//<NSATransTO>		- Connection timeout in ms for NSA transactions
//<DaemonIP>		- Daemon IP address
//<DaemonPort>		- Daemon listen port
//<DaemonUser>		- Daemon username
//<DaemonPass>		- Daemon password
//
// You may pass a NULL into NalpLibOpen to use the default values
int
constParams(
int					https,
char				*logLevel,
char				*workDir,
char				*clientName,
int					security,
char				**xmlParams
)
{
	int			xmlLen;
	char		secVal[25];
    fprintf(stdout, "Enter:  constParams method\n");

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

	addXMLParam(xmlParams, "WorkDir", workDir);

	addXMLParam(xmlParams, "ClientHostname", clientName);

	addXMLParam(xmlParams, "LogLevel", logLevel);

	if (https == 1)
	{
		addXMLParam(xmlParams, "HTTPSEnabled", "1");
	}

	if (strcmp(daemonIP, "") != 0)
	{
		addXMLParam(xmlParams, "DaemonIP", daemonIP);
	}

	if (strcmp(daemonPort, "") != 0)
	{
		addXMLParam(xmlParams, "DaemonPort", daemonPort);
	}

	if (strcmp(daemonUser, "") != 0)
	{
		addXMLParam(xmlParams, "DaemonUser", daemonUser);
	}

	if (strcmp(daemonPass, "") != 0)
	{
		addXMLParam(xmlParams, "DaemonPass", daemonPass);
	}

	//Enable the security offset feature.  That is, all returns from
	// the NSL library will have a security offset added to them.
	sprintf(secVal, "%i", security);
	addXMLParam(xmlParams, "SecurityValue", secVal);

	*xmlParams = (char *)
		realloc(*xmlParams, strlen(*xmlParams) + 18 + 1);	

	strcat(*xmlParams, "</SHAFERXMLParams>");

	/* Other possible parameters that aren't used here.
	 *
	addXMLParam(xmlParams, "OfflineMode", comArgs->offlineMode);
	addXMLParam(xmlParams, "LogQLen", comArgs->logQLen);
	addXMLParam(xmlParams, "CacheQLen", comArgs->cacheQLen);
	addXMLParam(xmlParams, "SoapThreadsMin", comArgs->soapThMin);
	addXMLParam(xmlParams, "SoapthreadsMax", comArgs->soapThMax);
	addXMLParam(xmlParams, "ProxyIP", comArgs->proxyIP);
	addXMLParam(xmlParams, "ProxyPort", comArgs->proxyPort);
	addXMLParam(xmlParams, "ProxyUsername", comArgs->proxyUser);
	addXMLParam(xmlParams, "ProxyPassword", comArgs->proxyPass);
	addXMLParam(xmlParams, "WorkGroup", comArgs->workGroup);
	*/
    fprintf(stdout, "Exit:  constParams method\n");

	return 0;	
}


int
addXMLParam(
char    **xmlParams,
char    *xmlName,
char    *xmlValue
)
{
    int         xmlLen;


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

