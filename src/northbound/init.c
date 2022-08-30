
#include <stdlib.h>

#include "init.h"
#include "dso/dsoNSA.h"
#include "dso/dsoSHAFER.h"
#include "dso/dsoNSL.h"

SetupParameter* setupParameter = NULL;
LicenseInformation *licenseInformation = NULL;

char			*nslVersion = NULL;
char			*nsaVersion = NULL;
char			*compID = NULL;
char			*cert = NULL;
char			*nslHostName = NULL;
char			*nsaHostName = NULL;
char			*specialID = NULL;
char			*tmpStr = NULL;



unsigned int			TIMETOCHECKLICENSE = 0;
int			    retVal;
char			*xmlParams;



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

void 
setConfigParameter(
int				custID,
int				prodID,
unsigned int	xauth,
unsigned int	yauth,
unsigned int	zauth,
char            *libPath
)
{
	setupParameter = (SetupParameter*)malloc(sizeof(SetupParameter));

	setupParameter->custID = custID;
	setupParameter->prodID = prodID;
	setupParameter->xauth = xauth;
	setupParameter->yauth = yauth;
	setupParameter->zauth = zauth;
	setupParameter->libPath = libPath;
	setupParameter->libHandle = NULL;
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
	// fprintf(stdout, "Input %p %s\n", libHandle, symName);

#if ! defined (WIN32)
    return dlsym(libHandle, symName);
#else
    return GetProcAddress((HINSTANCE)(libHandle), symName);
#endif
}


int
SetupLib(
)
{
    // fprintf(stdout, "Setting up the Library\n");

	generateSecurityOffset();
	// Flooded the XML Param as per the requirement
	// TODO: Take this as input
	XmlParameters *param = (XmlParameters *)malloc(sizeof(XmlParameters));
	param->clientName = "LicenseManagerPOC";
	param->https = 0;
	param->logLevel = "4";
	param->workDir = "/tmp";
	param->security = setupParameter->security;


	retVal = constParams(param, &xmlParams);

	free(param);
	if (retVal < 0)
	{
		fprintf(stdout, "constParams failed: %d\n", retVal);
		return retVal;
	}

	// Till this point
	// We have constants XML and 
	// generated security and offset from securiry constants

	retVal = initLib();

	if (retVal < 0)
	{
		return retVal;
	}
    // fprintf(stdout, "Library setup done\n");

	return retVal;
}

void 
generateSecurityOffset(
)
{
	// fprintf(stdout, "Generating the Security offset\n");

	if ((setupParameter->yauth != 0) && (setupParameter->zauth != 0))
	{
		srand((unsigned)time(NULL));
		setupParameter->security = 1 + (unsigned int)(500.0 * rand() / (RAND_MAX + 1.0));
		setupParameter->offset = setupParameter->xauth + ((setupParameter->security * setupParameter->yauth) % setupParameter->zauth);
	}
	else
	{
		setupParameter->security = 0;
		setupParameter->offset = 0;
	}
	// fprintf(stdout, "Generated Security offset %d\n", setupParameter->security);

}


void initlicenseInfoStruct(
LicenseInformation** licenseInformation 
)
{
	(*licenseInformation)->offlineState   = 0;
	(*licenseInformation)->authentication = NULL;
	(*licenseInformation)->licenseInfo    = NULL;
	(*licenseInformation)->licenseStatus  = NULL;
	(*licenseInformation)->expDate        = NULL;
	(*licenseInformation)->expLease       = NULL;
	(*licenseInformation)->compID         = NULL;
}

int
initLib(
)
{
	int			retVal =-1;

    // fprintf(stdout, "Enter: initLib method");

	retVal = openLibrary(xmlParams, setupParameter->libPath, &(setupParameter->libHandle));
    // fprintf(stdout, "Enter: aaaa method \n");

	free(xmlParams);

	if (retVal < 0)
	{
		fprintf(stdout, "Library open failed: %d\n", retVal);
		return retVal;
	}
	// TODO: Read from 
    // fprintf(stdout, "Enter: dsoNSLValidateLibrary method\n");

	retVal = dsoNSLValidateLibrary(setupParameter->custID, setupParameter->prodID, setupParameter->libHandle);
	retVal = retVal - (setupParameter->offset);

	if (retVal != 0)
	{
		fprintf(stdout, "Library validation failed: %d\n", retVal);
		return retVal;
	}
    // fprintf(stdout, "Enter: getNSLLibraryInfo method");

	retVal = getNSLLibraryInfo(&nslVersion, &nsaVersion,
				&(compID), &nslHostName, &nsaHostName, setupParameter->libHandle);

	if (retVal < 0)
	{
		fprintf(stdout, "Library Information failed: %d\n", retVal);
		return retVal;
	}


	licenseInformation = (LicenseInformation*)malloc(sizeof(LicenseInformation));
	initlicenseInfoStruct(&licenseInformation);
	retVal = getLicenseInfo(&licenseInformation);
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
    // fprintf(stdout, "Enter: initLib method %s \n", libPath);

	retVal = stat(libPath, &stbuf);

	if (retVal != 0)
	{
		fprintf(stderr, "Library %s does not exist or has incorrect "
				"permissions.\n", libPath);
		return -1;
	}
    // fprintf(stdout, "opening the library method %s\n", libPath);

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
    // fprintf(stdout, "libHandle %p\n", *libHandle);

	retVal = dsoNalpLibOpen(xmlParams, *libHandle);

	if (retVal < 0)
	{
		fprintf(stderr, "Open library failed\n");
		return retVal;
	}
    // fprintf(stdout, "Exit:  openLibrary method %p\n", xmlParams);
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
XmlParameters 		*param,
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

	addXMLParam(xmlParams, "WorkDir", param->workDir);

	addXMLParam(xmlParams, "ClientHostname", param->clientName);

	addXMLParam(xmlParams, "LogLevel", param->logLevel);

	if (param->https == 1)
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
	sprintf(secVal, "%i", param->security);
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
	retVal = retVal - setupParameter->offset;

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
	retVal = retVal - setupParameter->offset;

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
	retVal = retVal - setupParameter->offset;

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
LicenseInformation **license
)
{
	int			retVal;

	char		*tempVal;


	retVal = dsoNSLGetComputerID(&tempVal, setupParameter->libHandle);
	retVal = retVal - (setupParameter->offset);
    // fprintf(stdout, "\nlibHandle %d %p %p\n", retVal, tempVal, (*license)->compID);

	if (retVal != 0)
	{
		handleError(retVal, &((*license)->compID), setupParameter->libHandle);
	}
	else
	{
		if ((*license)->compID != NULL)
		{
			free((*license)->compID);
			(*license)->compID = NULL;
		}

		(*license)->compID = strdup(tempVal);
		dsoNSLFree(tempVal, setupParameter->libHandle);
	}
	int32_t		licStat;
	if ((*license)->authentication == NULL)
	{
		retVal = dsoNSLGetLicenseStatus(&licStat, setupParameter->libHandle);
	}else{
		retVal = dsoNSLObtainLicense((*license)->authentication,
					&licStat, NULL, NULL, setupParameter->libHandle);
	}

	retVal = retVal - setupParameter->offset;

	if (retVal != 0)
	{
		handleError(retVal, &((*license)->licenseStatus), setupParameter->libHandle);
	}

	//Call with certContainer = NULL to get offline state.  Libraries
	// prior to 3.7.52 DO NOT have this ability and will error out if
	// licensestatus is NULL.  In libraries 3.7.52 and newer, temp may
	// be NULL as well as *licenseNo and *certContainer.
	int32_t		tempp;
	retVal = dsoNSLImportCertificate(NULL, &tempp, NULL, setupParameter->libHandle);
	retVal = retVal - setupParameter->offset;

	if (retVal != 0)
	{
		handleError(retVal, &((*license)->licenseStatus), setupParameter->libHandle);
	}

	//1 - activation request generated and awaiting cert impomrt
	//0 - Offline activation is not underway.
	(*license)->offlineState = retVal;
	licStat2Str(licStat, &((*license)->licenseStatus));

	if ((*license)->authentication != NULL)
	{
		free((*license)->authentication);
		(*license)->authentication = NULL;
	}

	retVal = dsoNSLGetLicenseCode(&tempVal, setupParameter->libHandle);
	retVal = retVal - setupParameter->offset;

	if (retVal == 0)
	{
		(*license)->authentication = strdup(tempVal);
	}

	if (tempVal != NULL)
			dsoNSLFree(tempVal, setupParameter->libHandle);


	if ((*license)->expLease != NULL)
	{
		free((*license)->expLease);
		(*license)->expLease = NULL;
	}

	retVal = dsoNSLGetLeaseExpDate(&tempVal, setupParameter->libHandle);
	retVal = retVal - setupParameter->offset;

	//Don't display error assume we just don't have a license code.
	if (retVal == 0)
	{
		(*license)->expLease = strdup(tempVal);
	}

	if (tempVal != NULL)
			dsoNSLFree(tempVal, setupParameter->libHandle);

	uint32_t	licType;
	uint32_t	actType;

	retVal = dsoNSLGetLicenseInfo(&licType, &actType, setupParameter->libHandle);
	retVal = retVal - setupParameter->offset;

	if (retVal != 0)
	{
		handleError(retVal, &((*license)->licenseInfo), setupParameter->libHandle);
	}

	char		*typeInfo = NULL;
	char		*actInfo = NULL;
	licType2Str(licType, &typeInfo, &((*license)->expDate), setupParameter->libHandle, setupParameter->offset);
	licAct2Str(actType, &actInfo);

	if ((*license)->licenseInfo != NULL)
		free((*license)->licenseInfo);

	(*license)->licenseInfo = strdup(typeInfo);

	free(typeInfo);
	free(actInfo);

	return 0;
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
)
{

	//License code may be unset
	// gotoXY(x, y);
	
	if (licenseInformation->authentication == NULL)
	{
		fprintf(stdout, "License code: Unknown\n");
	}
	else
	{
		fprintf(stdout, "License code: %s\n", licenseInformation->authentication);
	}

	// gotoXY(x, ++y);
	fprintf(stdout, "ComputerID: %s\n", licenseInformation->compID);

	// gotoXY(x, ++y);
	fprintf(stdout, "License status: %s\n", licenseInformation->licenseStatus);

	if (licenseInformation->offlineState == 1)
	{
		// gotoXY(x, ++y);
		fprintf(stdout, "Library awaiting a certificate import\n");
	}

	// gotoXY(x, ++y);
	fprintf(stdout, "License Type: %s\n", licenseInformation->licenseInfo);

	// gotoXY(x, ++y);
	fprintf(stdout, "License Exp Date: %s\n", licenseInformation->expDate);

	// gotoXY(x, ++y);
	fprintf(stdout, "Lease Exp Date: %s\n", licenseInformation->expLease);

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

	outputNSLLibraryInfo(&x, &y, libPath, setupParameter->custID, setupParameter->prodID,
			nslVersion, nsaVersion, compID, nslHostName, nsaHostName);

	fprintf(stdout, "-----------------------------------\n");

	outputLicenseInfo();
	fprintf(stdout, "===================================\n");

	return 0;
}


int
checkLicenseStatus(
char			*licenseCode,
int32_t			*licenseStatus,
uint32_t		*licenseType,
uint32_t		*actType
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
	int32_t		licStat;
	retVal = dsoNSLGetLicenseStatus(&licStat, setupParameter->libHandle);
	retVal = retVal - setupParameter->offset;

	//An error return (< 0)from the library indicates that something
	// is wrong. On the other hand, a status value of < 0 indicates
	// that the current license is invalid.
	//
	//If no error and a good status we are good to continue
	//Many error returns can be sucessfully handled simply by requesting
	// a new license. So, if we have either an error or an invalid license
	// status, try to get a new license from Nalpeiron.
	if ((retVal < 0) || (licStat <= 0))
	{
		//The getlicense call is where registration information is
		// passed into Nalpeiron if it is present.
		fprintf(stdout, "Getting New License\n");
		retVal = dsoNSLObtainLicense(licenseCode,
					&licStat, NULL, NULL, setupParameter->libHandle);
		retVal = retVal - setupParameter->offset;

		//We've failed with an error.  Output error information and exit
		if (retVal != 0)
		{
			fprintf(stderr, "Get license failed\n");
			dsoNalpGetErrorMsg(retVal, &errMsg, setupParameter->libHandle);

			if (errMsg != NULL)
			{
				fprintf(stderr, "%d: %s\n", retVal, errMsg);
				dsoNSLFree(errMsg, setupParameter->libHandle);
				errMsg = NULL;
			}

			fprintf(stdout, "==================================================\n");
			fprintf(stdout, "\n\n");
			return retVal;
		}
		
		retVal = dsoNSLGetComputerID(&compID, setupParameter->libHandle);
		retVal = retVal - setupParameter->offset;
		
		if (retVal != 0)
		{
			fprintf(stderr, "GetComputerID after NSLObtainLicense() failed\n");
			dsoNalpGetErrorMsg(retVal, &errMsg, setupParameter->libHandle);

			if (errMsg != NULL)
			{
				fprintf(stderr, "%d: %s\n", retVal, errMsg);
				dsoNSLFree(errMsg, setupParameter->libHandle);
				errMsg = NULL;
			}
		}
		else
		{
			fprintf(stdout, "After NSLObtainLicense(), Comuter ID: %s\n", compID);
		}
		
		if (compID != NULL) dsoNSLFree(compID, setupParameter->libHandle);

		//We sucessfully contacted Nalpeiron but no valid license is
		// available to us.  Print an error message and exit.  Possible
		// values of the product status can be found in prodStatus.h
		if (&licStat < 0)
		{
			fprintf(stderr, "Invalid product status of %d\n", licStat);
			fprintf(stdout, "==================================================\n");
			fprintf(stdout, "\n\n");
			return -1;
		}
	}

	//We have a license of some sort.  Get info on type and activation
	// method (activation method will be online as we just got it
	// from NSLGetLicense).
	retVal = dsoNSLGetLicenseInfo(licenseType, actType, setupParameter->libHandle);
	retVal = retVal - setupParameter->offset;

	if (retVal < 0)
	{
		// fprintf(stdout, "Error while Getting license info\n");
		return retVal;
	}

	fprintf(stdout, "Current license status is %d\n", licStat);
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
	retVal = getLicenseInfo(&licenseInformation);

	outputLicenseInfo();
	return retVal;
}

int
GetFeatureStatus(
char			*featcode,
char**			featureS
)
{
	int					retVal;
	char				*errMsg = NULL;
	// fprintf(stdout, "Check for feature %s\n", featcode);
	int32_t			featureStatus;


	//This should be a good feature
	retVal = dsoNSLGetFeatureStatus(featcode, &featureStatus, setupParameter->libHandle);
	retVal = retVal - setupParameter->offset;
	featStat2Str(featureStatus, featureS);
	if (retVal < 0)
	{
		dsoNalpGetErrorMsg(retVal, &errMsg, setupParameter->libHandle);

		if (errMsg != NULL)
		{
			// fprintf(stderr, "%d: %s\n", retVal, errMsg);
			dsoNSLFree(errMsg, setupParameter->libHandle);
			errMsg = NULL;
		}

		return retVal;
	}
	else if (featureStatus <= 0)
	{
		// fprintf(stderr, "Invalid feature:- %d\n",featureStatus);
		return -1;
	}
	// fprintf(stdout, "Status for feature %s :- %d\n", featcode, featureStatus);

	return 0;
}


int GetLicense(
)
{

	char *errMsg = NULL;
	int32_t licenseStatus;
	retVal = dsoNSLGetLicense(NULL, &licenseStatus, NULL, setupParameter->libHandle);
	retVal = retVal - setupParameter->security;

	if (retVal != 0)
	{
		handleError(retVal, &errMsg, setupParameter->libHandle);
	}
	licStat2Str(licenseStatus, &(licenseInformation->licenseStatus));

	return 0;
}