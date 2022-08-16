
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
int				custID;
int				prodID;

//These are the security values stamped into your library. They
// should be changed to match your values.
// NOTE When selecting yauth and zauth it is important that they
// NOT be multiples of each other (for instance, yauth=300 zauth=600).
// If they are multiples the resulting security offset will NOT be
// random.
unsigned int	xauth;
unsigned int	yauth;
unsigned int	zauth;

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
	// fprintf(stdout, "Input %p %s\n", libHandle, symName);

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
    fprintf(stdout, "Setting up the Library\n");

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
    fprintf(stdout, "Library setup done\n");

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
	// TODO: Read from 
    // fprintf(stdout, "Enter: initLib method %d %d %p \n", custID, prodID, *libHandle);

	retVal = dsoNSLValidateLibrary(custID, prodID, *libHandle);
	retVal = retVal - offset;

	if (retVal != 0)
	{
		fprintf(stdout, "Library validation failed: %d\n", retVal);
		return retVal;
	}

	retVal = getNSLLibraryInfo(offset, &nslVersion, &nsaVersion,
				&compID, &nslHostName, &nsaHostName, *libHandle);

	if (retVal < 0)
	{
		fprintf(stdout, "Library Information failed: %d\n", retVal);
		return retVal;
	}

	retVal = getLicenseInfo(offset, &offlineState, &authentication,
				&licenseInfo, &licenseStatus, &expDate, &expLease,
				&compID, *libHandle);
    // fprintf(stdout, "Exit:  initLib method");

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

    // fprintf(stdout, "Enter: openLibrary method\n");

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
    // fprintf(stdout, "Exit:  openLibrary method\n");
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
    // fprintf(stdout, "Enter:  constParams method\n");

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
    // fprintf(stdout, "Exit:  constParams method\n");

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

int
getNSLLibraryInfo(
int				offset,
char			**nslVersion,
char			**nsaVersion,
char			**compID,
char			**nslHostName,
char			**nsaHostName,
void			*libHandle
)
{	
	char		*tempVal;
	int			retVal;


	retVal = dsoNSLGetVersion(&tempVal, libHandle);
	retVal = retVal - offset;

	if (retVal != 0)
	{
		handleError(retVal, nslVersion, libHandle);
	}
	else
	{
		//Windows maintains separate heaps for shared libraries
		*nslVersion = strdup(tempVal);
		dsoNSLFree(tempVal, libHandle);
	}

	//Note NSA Functions do not use security offsets
	retVal = dsoNSAGetVersion(&tempVal, libHandle);

	if (retVal < 0)
	{
		handleError(retVal, nsaVersion, libHandle);
	}
	else
	{
		*nsaVersion = strdup(tempVal);
		dsoNSAFree(tempVal, libHandle);
	}

	retVal = dsoNSLGetComputerID(&tempVal, libHandle);
	retVal = retVal - offset;

	if (retVal != 0)
	{
		handleError(retVal, compID, libHandle);
	}
	else
	{
		if (*compID != NULL)
		{
			free(*compID);
			*compID = NULL;
		}

		*compID = strdup(tempVal);
		dsoNSLFree(tempVal, libHandle);
	}

	retVal = dsoNSLGetHostName(&tempVal, libHandle);
	retVal = retVal - offset;

	if (retVal != 0)
	{
		handleError(retVal, nslHostName, libHandle);
	}
	else
	{
		*nslHostName = strdup(tempVal);
		dsoNSLFree(tempVal, libHandle);
	}

	retVal = dsoNSAGetHostName(&tempVal, libHandle);

	if (retVal != 0)
	{
		handleError(retVal, nsaHostName, libHandle);
	}
	else
	{
		*nsaHostName = strdup(tempVal);
		dsoNSAFree(tempVal, libHandle);
	}

	return 0;
}


int
getLicenseInfo(
int				offset,
int				*offlineState,
char			**authentication,
char			**licenseInfo,
char			**licenseStatus,
char			**expDate,
char			**expLease,
char			**compID,
void			*libHandle
)
{
	int			retVal;
	int32_t		licStat;
	int32_t		tempp;
	uint32_t	licType;
	uint32_t	actType;
	char		*typeInfo = NULL;
	char		*actInfo = NULL;
	char		*tempVal;


	retVal = dsoNSLGetComputerID(&tempVal, libHandle);
	retVal = retVal - offset;

	if (retVal != 0)
	{
		handleError(retVal, compID, libHandle);
	}
	else
	{
		if (*compID != NULL)
		{
			free(*compID);
			*compID = NULL;
		}

		*compID = strdup(tempVal);
		dsoNSLFree(tempVal, libHandle);
	}

	retVal = dsoNSLGetLicenseStatus(&licStat, libHandle);
	retVal = retVal - offset;

	if (retVal != 0)
	{
		handleError(retVal, licenseStatus, libHandle);
	}

	//Call with certContainer = NULL to get offline state.  Libraries
	// prior to 3.7.52 DO NOT have this ability and will error out if
	// licensestatus is NULL.  In libraries 3.7.52 and newer, temp may
	// be NULL as well as *licenseNo and *certContainer.
	retVal = dsoNSLImportCertificate(NULL, &tempp, NULL, libHandle);
	retVal = retVal - offset;

	if (retVal != 0)
	{
		handleError(retVal, licenseStatus, libHandle);
	}

	//1 - activation request generated and awaiting cert impomrt
	//0 - Offline activation is not underway.
	*offlineState = retVal;

	licStat2Str(licStat, licenseStatus);

	if (*authentication != NULL)
	{
		free(*authentication);
		*authentication = NULL;
	}

	retVal = dsoNSLGetLicenseCode(&tempVal, libHandle);
	retVal = retVal - offset;

	if (retVal == 0)
	{
		*authentication = strdup(tempVal);
	}

	if (tempVal != NULL)
			dsoNSLFree(tempVal, libHandle);


	if (*expLease != NULL)
	{
		free(*expLease);
		*expLease = NULL;
	}

	retVal = dsoNSLGetLeaseExpDate(&tempVal, libHandle);
	retVal = retVal - offset;

	//Don't display error assume we just don't have a license code.
	if (retVal == 0)
	{
		*expLease = strdup(tempVal);
	}

	if (tempVal != NULL)
			dsoNSLFree(tempVal, libHandle);
		
	retVal = dsoNSLGetLicenseInfo(&licType, &actType, libHandle);
	retVal = retVal - offset;

	if (retVal != 0)
	{
		handleError(retVal, licenseInfo, libHandle);
	}

	licType2Str(licType, &typeInfo, expDate, libHandle);
	licAct2Str(actType, &actInfo);

	if (*licenseInfo != NULL)
		free(*licenseInfo);

	*licenseInfo = strdup(typeInfo);

	free(typeInfo);
	free(actInfo);

	return 0;
}

void
licStat2Str(
int			licStat,
char		**licenseStatus
)
{
	if (*licenseStatus != NULL)
		free(*licenseStatus);

	switch (licStat)
	{
	case -201:
		*licenseStatus = strdup("(-201) License revoked by daemon\n");
		break;

	case -200:
		*licenseStatus = strdup("(-200) Client missed too many heartbeats\n");
		break;

	case -117:
		*licenseStatus = strdup("(-117) Duplicate device id\n");
		break;

	case -116:
		*licenseStatus = strdup("(-116) Subscription expired\n");
		break;

	case -115:
		*licenseStatus = strdup("(-115) Number of allowed activations exceeded\n");
		break;

	case -114:
		*licenseStatus = strdup("(-114) License code is inactive\n");
		break;

	case -113:
		*licenseStatus = strdup("(-113) Trial has expired\n");
		break;

	case -112:
		*licenseStatus = strdup("(-112) Computer ID already activated\n");
		break;

	case -111:
		*licenseStatus = strdup("(-111) Invalid trial period\n");
		break;

	case -110:
		*licenseStatus = strdup("(-110) Product is inactive\n");
		break;

	case -70:
		*licenseStatus = strdup("(-70) Cached ABL awaiting authentication\n");
		break;

	case -60:
		*licenseStatus = strdup("(-60) Disabled passive license\n");
		break;

	case -52:
		*licenseStatus = strdup("(-52) Daemon ID failure\n");
		break;

	case -51:
		*licenseStatus = strdup("(-51) Daemon failed verify\n");
		break;

	case -50:
		*licenseStatus = strdup("(-50) No available license\n");
		break;

	case -9:
		*licenseStatus = strdup("(-9) License is activation receipt\n");
		break;

	case -8:
		*licenseStatus = strdup("(-8) Product has invalid date\n");
		break;

	case -7:
		*licenseStatus = strdup("(-7) Incorrect system date\n");
		break;

	case -6:
		*licenseStatus = strdup("(-6) License returned to server\n");
		break;

	case -5:
		*licenseStatus = strdup("(-5) Invalid license\n");
		break;

	case -4:
		*licenseStatus = strdup("(-4) License missing\n");
		break;

	case -3:
		*licenseStatus = strdup("(-3) Not authorized\n");
		break;

	case -2:
		*licenseStatus = strdup("(-2) Computer has been backdated\n");
		break;

	case -1:
		*licenseStatus = strdup("(-1) License expired\n");
		break;

	case 1:
		*licenseStatus = strdup("(1) Valid license\n");
		break;

	case 2:
		*licenseStatus = strdup("(2) Trial license\n");
		break;

	case 3:
		*licenseStatus = strdup("(3) Online concurrent mode\n");
		break;

	case 4:
		*licenseStatus = strdup("(4) Account based licensing\n");
		break;

	case 5:
		*licenseStatus = strdup("(5) Concurrent account based licensing\n");
		break;

	case 14:
		*licenseStatus = strdup("(14) Daemon network license\n");
		break;

	case 15:
		*licenseStatus = strdup("(15) Daemon LTCO\n");
		break;

	case 16:
		*licenseStatus = strdup("(16) Daemon OEM\n");
		break;

	case 51:
		*licenseStatus = strdup("(51) LTCO\n");
		break;

	case 53:
		*licenseStatus = strdup("(53) Concurrent LTCO\n");
		break;

	case 54:
		*licenseStatus = strdup("(54) ABL LTCO\n");
		break;

	case 55:
		*licenseStatus = strdup("(55) ABL Concurrent LTCO\n");
		break;

	default:
		*licenseStatus = strdup("Unknown license status\n");
		break;
	}

	return;
}


void
licType2Str(
uint32_t	licType,
char		**licenseType,
char		**expDate,
void		*libHandle
)
{
	char		*tempVal;
	int			retVal;


	if (*expDate != NULL)
		free(*expDate);

	if (*licenseType != NULL)
		free(*licenseType);

	switch (licType)
	{
	case 2:
		*licenseType = strdup("Trial");

		retVal = dsoNSLGetTrialExpDate(&tempVal, libHandle);
		retVal = retVal - offset;

		if (retVal != 0)
		{
			*expDate = NULL;
		}
		else
		{
			*expDate = strdup(tempVal);
			dsoNSLFree(tempVal, libHandle);
		}

		break;

	case 3:
		*licenseType = strdup("Permanent");
		*expDate = strdup("NA");
		break;

	case 4:
		*licenseType = strdup("Permanent concurrent");
		*expDate = strdup("NA");
		break;

	case 5:
		*licenseType = strdup("Subscription");

		retVal = dsoNSLGetSubExpDate(&tempVal, libHandle);
		retVal = retVal - offset;

		if (retVal != 0)
		{
			*expDate = NULL;
		}
		else
		{
			*expDate = strdup(tempVal);
			dsoNSLFree(tempVal, libHandle);
		}

		break;

	case 6:
		*licenseType = strdup("Subscription concurrent");

		retVal = dsoNSLGetSubExpDate(&tempVal, libHandle);
		retVal = retVal - offset;

		if (retVal != 0)
		{
			*expDate = NULL;
		}
		else
		{
			*expDate = strdup(tempVal);
			dsoNSLFree(tempVal, libHandle);
		}

		break;

	case 7:
		*licenseType = strdup("Network");
		*expDate = strdup("NA");
		break;

	case 8:
		*licenseType = strdup("Subscription network");

		retVal = dsoNSLGetSubExpDate(&tempVal, libHandle);
		retVal = retVal - offset;

		if (retVal != 0)
		{
			*expDate = NULL;
		}
		else
		{
			*expDate = strdup(tempVal);
			dsoNSLFree(tempVal, libHandle);
		}

		break;

	case 9:
		*licenseType = strdup("Long term checkout (ltco) from daemon");
		*expDate = strdup("NA");
		break;

	case 10:
		*licenseType = strdup("OEM");
		*expDate = strdup("NA");
		break;

	case 11:
		*licenseType = strdup("Account based");
		*expDate = strdup("NA");
		break;

	case 12:
		*licenseType = strdup("Subscription Account based");

		retVal = dsoNSLGetSubExpDate(&tempVal, libHandle);
		retVal = retVal - offset;

		if (retVal != 0)
		{
			*expDate = NULL;
		}
		else
		{
			*expDate = strdup(tempVal);
			dsoNSLFree(tempVal, libHandle);
		}

		break;

	case 13:
		*licenseType = strdup("Concurrent account based");
		*expDate = strdup("NA");
		break;

	case 14:
		*licenseType = strdup("Subscription concurrent account based");

		retVal = dsoNSLGetSubExpDate(&tempVal, libHandle);
		retVal = retVal - offset;

		if (retVal != 0)
		{
			*expDate = NULL;
		}
		else
		{
			*expDate = strdup(tempVal);
			dsoNSLFree(tempVal, libHandle);
		}

		break;

	case 15:
		*licenseType = strdup("Long term checkout (ltco)");
		*expDate = strdup("NA");
		break;

	case 16:
		*licenseType = strdup("Concurrent Long term checkout (ltco)");
		*expDate = strdup("NA");
		break;

	case 17:
		*licenseType = strdup("Account based LTCO");
		*expDate = strdup("NA");
		break;

	case 18:
		*licenseType = strdup("Concurrent account based LTCO");
		*expDate = strdup("NA");
		break;

	default:
		*licenseType = strdup("Unknown");
		*expDate = strdup("Unknown");
		break;
	}

	return;
}


void
licAct2Str(
uint32_t	actType,
char		**activationType
)
{
	if (*activationType != NULL)
		free(*activationType);

	switch (actType)
	{
	case 1:
		*activationType = strdup(" activated online\n");
		break;

	case 2:
		*activationType = strdup(" activated with certificate\n");
		break;

	case 3:
		*activationType = strdup(" activated via daemon\n");
		break;

	default:
		*activationType = strdup(" with unknown activation method\n");
		break;
	}

	return;
}



//errorMsg must be freed by caller.
void
handleError(
int         nslErrorNo,
char        **nalpMsg,
void        *libHandle
)
{
    char        *errorMsg;
    int         retVal;


    errorMsg = NULL;
    retVal = dsoNalpGetErrorMsg(nslErrorNo, &errorMsg, libHandle);

    if ((retVal < 0) || (errorMsg == NULL))
    {
        *nalpMsg = (char *)malloc(33 + 12 + 1);
        sprintf(*nalpMsg, "Error Handling routines failed: %i\n", retVal);
        return;
    }

	//We've got to make a copy of the error message for Windows systems.  There
	// the errorMsg was alloced in the lib so it must be freed in the lib as it
	// is very likely to have a different heap than the calling program.
    *nalpMsg = (char *)malloc(12 + strlen(errorMsg) + 1);
    sprintf(*nalpMsg, "%d:%s\n", nslErrorNo, errorMsg);

    dsoNSLFree(errorMsg, libHandle);

    return;
}

int
outputNSLLibraryInfo(
unsigned int	*startX,
unsigned int	*startY,
char			*libPath,
int				custID,
int				prodID,
char			*nslVersion,
char			*nsaVersion,
char			*compID,
char			*nslHostName,
char			*nsaHostName
)
{
	unsigned	x, y;
	char		*nsaVer;


	x = *startX;
	y = *startY;

	// gotoXY(x, y);
	fprintf(stdout, "Library path: %s\n", libPath);

	// gotoXY(x, ++y);
	fprintf(stdout, "CustID: %d\tProdID: %d\n", custID, prodID);

	//The version info is something like
	//  Library Version: 3.0.93 NSL: 13.0.93
	//  Library Version: 3.0.93 NSA: 3.0.93
	// Combine the two so as not to waste screen space
	nsaVer = strrchr(nsaVersion, ':');
	nsaVer = nsaVer - 3;

	// gotoXY(x, ++y);

	if (nsaVer != NULL)
	{
		fprintf(stdout, "%s %s\n", nslVersion, nsaVer);
	}
	else
	{
		fprintf(stdout, "%s\n", nslVersion);
	}

	// gotoXY(x, ++y);
	fprintf(stdout, "NSL Host: %s\n", nslHostName);

	// gotoXY(x, ++y);
	fprintf(stdout, "NSA Host: %s\n", nsaHostName);

	*startX = x;
	*startY = y;

	return 0;
}


int
outputLicenseInfo(
unsigned int	*startX,
unsigned int	*startY,
int				offlineState,
char			*authentication,
char			*licenseInfo,
char			*licenseStatus,
char			*expDate,
char			*expLease
)
{
	unsigned x, y;


	x = *startX;
	y = *startY;

	//License code may be unset
	// gotoXY(x, y);
	
	if (authentication == NULL)
	{
		fprintf(stdout, "License code: Unknown\n");
	}
	else
	{
		fprintf(stdout, "License code: %s\n", authentication);
	}

	// gotoXY(x, ++y);
	fprintf(stdout, "ComputerID: %s\n", compID);

	// gotoXY(x, ++y);
	fprintf(stdout, "License status: %s\n", licenseStatus);

	if (offlineState == 1)
	{
		// gotoXY(x, ++y);
		fprintf(stdout, "Library awaiting a certificate import\n");
	}

	// gotoXY(x, ++y);
	fprintf(stdout, "License Type: %s\n", licenseInfo);

	// gotoXY(x, ++y);
	fprintf(stdout, "License Exp Date: %s\n", expDate);

	// gotoXY(x, ++y);
	fprintf(stdout, "Lease Exp Date: %s\n", expLease);

	*startX = x;
	*startY = y;

	return 0;
}


int outInformation(
char            *libPath
)
{
	unsigned int	x,y;

	// clear();

	x = 3;
	y = 2;
	fprintf(stdout, "===================================\n");

	outputNSLLibraryInfo(&x, &y, libPath, custID, prodID,
			nslVersion, nsaVersion, compID, nslHostName, nsaHostName);

	fprintf(stdout, "-----------------------------------\n");

	outputLicenseInfo(&x, &y, offlineState,
			authentication, licenseInfo, licenseStatus, expDate, expLease);
	fprintf(stdout, "===================================\n");

	return 0;
}

int 
setConfigParameter(
int				CUSTID,
int				PRODID,
unsigned int	XAUTH,
unsigned int	YAUTH,
unsigned int	ZAUTH
)
{
	custID = CUSTID;
	prodID = PRODID;
	xauth = XAUTH;
	yauth = YAUTH;
	zauth = ZAUTH;
	return 0;
}


int
checkLicenseStatus(
void			**libHandle,
char			*licenseCode,
int32_t			*licenseStatus,
uint32_t		*licenseType,
uint32_t		*actType,
char			*xmlRegInfo
)
{
	char			*compID = NULL;
	char			*errMsg = NULL;
	int				retVal;

	// fprintf(stdout, "Input %p %s\n", *libHandle, licenseCode);

	fprintf(stdout, "\n\n");
	fprintf(stdout, "Checking current license status\n");
	fprintf(stdout, "==================================================\n");

	//Check on the license and get a new one if needed.  If
	// we don't get a valid license quit.
	retVal = dsoNSLGetLicenseStatus(licenseStatus, *libHandle);
	retVal = retVal - offset;

	//An error return (< 0)from the library indicates that something
	// is wrong. On the other hand, a status value of < 0 indicates
	// that the current license is invalid.
	//
	//If no error and a good status we are good to continue
	//Many error returns can be sucessfully handled simply by requesting
	// a new license. So, if we have either an error or an invalid license
	// status, try to get a new license from Nalpeiron.
	if ((retVal < 0) || (*licenseStatus <= 0))
	{
		//The getlicense call is where registration information is
		// passed into Nalpeiron if it is present.
		fprintf(stdout, "Getting New License\n");
		retVal = dsoNSLObtainLicense(licenseCode,
					licenseStatus, xmlRegInfo, NULL, *libHandle);
		retVal = retVal - offset;

		//We've failed with an error.  Output error information and exit
		if (retVal != 0)
		{
			fprintf(stderr, "Get license failed\n");
			dsoNalpGetErrorMsg(retVal, &errMsg, *libHandle);

			if (errMsg != NULL)
			{
				fprintf(stderr, "%d: %s\n", retVal, errMsg);
				dsoNSLFree(errMsg, *libHandle);
				errMsg = NULL;
			}

			fprintf(stdout, "==================================================\n");
			fprintf(stdout, "\n\n");
			return retVal;
		}
		
		retVal = dsoNSLGetComputerID(&compID, *libHandle);
		retVal = retVal - offset;
		
		if (retVal != 0)
		{
			fprintf(stderr, "GetComputerID after NSLObtainLicense() failed\n");
			dsoNalpGetErrorMsg(retVal, &errMsg, *libHandle);

			if (errMsg != NULL)
			{
				fprintf(stderr, "%d: %s\n", retVal, errMsg);
				dsoNSLFree(errMsg, *libHandle);
				errMsg = NULL;
			}
		}
		else
		{
			fprintf(stdout, "After NSLObtainLicense(), Comuter ID: %s\n", compID);
		}
		
		if (compID != NULL) dsoNSLFree(compID, *libHandle);

		//We sucessfully contacted Nalpeiron but no valid license is
		// available to us.  Print an error message and exit.  Possible
		// values of the product status can be found in prodStatus.h
		if (licenseStatus < 0)
		{
			fprintf(stderr, "Invalid product status of %d\n", *licenseStatus);
			fprintf(stdout, "==================================================\n");
			fprintf(stdout, "\n\n");
			return -1;
		}
	}

	//We have a license of some sort.  Get info on type and activation
	// method (activation method will be online as we just got it
	// from NSLGetLicense).
	retVal = dsoNSLGetLicenseInfo(licenseType, actType, *libHandle);
	retVal = retVal - offset;

	if (retVal < 0)
	{
		// fprintf(stdout, "Error while Getting license info\n");
		return retVal;
	}

	fprintf(stdout, "Current license status is %d\n", *licenseStatus);
	fprintf(stdout, "==================================================\n");
	fprintf(stdout, "\n\n");
	fflush(stdout);
	return 0;
}


int
GetLicenseForCurrentUser(
void            **libHandle
)
{
	retVal = getLicenseInfo(offset, &offlineState, &authentication,
			&licenseInfo, &licenseStatus, &expDate, &expLease,
			&compID, *libHandle);

	unsigned int	x,y;

	// clear();

	x = 3;
	y = 2;
	outputLicenseInfo(&x, &y, offlineState,
		authentication, licenseInfo, licenseStatus, expDate, expLease);
	return retVal;
}