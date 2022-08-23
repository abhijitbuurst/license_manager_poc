
#include <stdlib.h>

#if ! defined (WIN32)
#include <unistd.h>
#endif

// Todo: Removed the file inttypes
// #include <inttypes.h>
//added in header file
// #include "stdint.h"

#include "../utility/libHelper.h"
#include "../NalperionInterface/NalpeironNSA.h"
#include "dsoNSA.h"

int
dsoNSAValidateLibrary(
uint32_t custID,
uint32_t prodID ,
void *libHandle
)
{
	int				rv;
	NSAValidateLibrary_t	NSAValidateLibrary_ptr = NULL;


	if (!(NSAValidateLibrary_ptr = (NSAValidateLibrary_t)GetSymbol(libHandle, "NSAValidateLibrary")))
	{
		return -1;
	}

	rv = NSAValidateLibrary_ptr(custID, prodID);

	return rv;
}


int
dsoNSAGetVersion(
char **version ,
void *libHandle
)
{
	int				rv;
	NSAGetVersion_t	NSAGetVersion_ptr = NULL;


	if (!(NSAGetVersion_ptr = (NSAGetVersion_t)GetSymbol(libHandle, "NSAGetVersion")))
	{
		return -1;
	}

	rv = NSAGetVersion_ptr(version);

	return rv;
}


int
dsoNSAGetComputerID(
char **computerID ,
void *libHandle
)
{
	int				rv;
	NSAGetComputerID_t	NSAGetComputerID_ptr = NULL;


	if (!(NSAGetComputerID_ptr = (NSAGetComputerID_t)GetSymbol(libHandle, "NSAGetComputerID")))
	{
		return -1;
	}

	rv = NSAGetComputerID_ptr(computerID);

	return rv;
}


int
dsoNSAGetHostName(
char **hostName ,
void *libHandle
)
{
	int				rv;
	NSAGetHostName_t	NSAGetHostName_ptr = NULL;


	if (!(NSAGetHostName_ptr = (NSAGetHostName_t)GetSymbol(libHandle, "NSAGetHostName")))
	{
		return -1;
	}

	rv = NSAGetHostName_ptr(hostName);

	return rv;
}


int
dsoNSATestConnection2(
const char *nsaClientData,
uint32_t *transID ,
void *libHandle
)
{
	int				rv;
	NSATestConnection2_t	NSATestConnection2_ptr = NULL;


	if (!(NSATestConnection2_ptr = (NSATestConnection2_t)GetSymbol(libHandle, "NSATestConnection2")))
	{
		return -1;
	}

	rv = NSATestConnection2_ptr(nsaClientData, transID);

	return rv;
}


int
dsoNSATestConnection(
const char *nsaClientData,
uint32_t *transID ,
void *libHandle
)
{
	int				rv;
	NSATestConnection_t	NSATestConnection_ptr = NULL;


	if (!(NSATestConnection_ptr = (NSATestConnection_t)GetSymbol(libHandle, "NSATestConnection")))
	{
		return -1;
	}

	rv = NSATestConnection_ptr(nsaClientData, transID);

	return rv;
}


int
dsoNSAAppStart(
const char *username,
const char *nsaClientData,
uint32_t *transID ,
void *libHandle
)
{
	int				rv;
	NSAAppStart_t	NSAAppStart_ptr = NULL;


	if (!(NSAAppStart_ptr = (NSAAppStart_t)GetSymbol(libHandle, "NSAAppStart")))
	{
		return -1;
	}

	rv = NSAAppStart_ptr(username, nsaClientData, transID);

	return rv;
}


int
dsoNSAAppStop(
const char *username,
const char *nsaClientData,
uint32_t *transID ,
void *libHandle
)
{
	int				rv;
	NSAAppStop_t	NSAAppStop_ptr = NULL;


	if (!(NSAAppStop_ptr = (NSAAppStop_t)GetSymbol(libHandle, "NSAAppStop")))
	{
		return -1;
	}

	rv = NSAAppStop_ptr(username, nsaClientData, transID);

	return rv;
}


int
dsoNSALogin(
const char *username,
const char *nsaClientData,
uint32_t *transID ,
void *libHandle
)
{
	int				rv;
	NSALogin_t	NSALogin_ptr = NULL;


	if (!(NSALogin_ptr = (NSALogin_t)GetSymbol(libHandle, "NSALogin")))
	{
		return -1;
	}

	rv = NSALogin_ptr(username, nsaClientData, transID);

	return rv;
}


int
dsoNSALogout(
const char *username,
const char *nsaClientData,
uint32_t *transID ,
void *libHandle
)
{
	int				rv;
	NSALogout_t	NSALogout_ptr = NULL;


	if (!(NSALogout_ptr = (NSALogout_t)GetSymbol(libHandle, "NSALogout")))
	{
		return -1;
	}

	rv = NSALogout_ptr(username, nsaClientData, transID);

	return rv;
}


int
dsoNSAFeatureStart(
const char *username,
const char *featureCode,
const char *nsaClientData,
uint32_t *transID ,
void *libHandle
)
{
	int				rv;
	NSAFeatureStart_t	NSAFeatureStart_ptr = NULL;


	if (!(NSAFeatureStart_ptr = (NSAFeatureStart_t)GetSymbol(libHandle, "NSAFeatureStart")))
	{
		return -1;
	}

	rv = NSAFeatureStart_ptr(username, featureCode, nsaClientData, transID);

	return rv;
}


int
dsoNSAFeatureStop(
const char *username,
const char *featureCode,
const char *nsaClientData,
uint32_t *transID ,
void *libHandle
)
{
	int				rv;
	NSAFeatureStop_t	NSAFeatureStop_ptr = NULL;


	if (!(NSAFeatureStop_ptr = (NSAFeatureStop_t)GetSymbol(libHandle, "NSAFeatureStop")))
	{
		return -1;
	}

	rv = NSAFeatureStop_ptr(username, featureCode, nsaClientData, transID);

	return rv;
}


int
dsoNSAException(
const char *username,
const char *exceptionCode,
const char *description,
const char *nsaClientData,
uint32_t *transID ,
void *libHandle
)
{
	int				rv;
	NSAException_t	NSAException_ptr = NULL;


	if (!(NSAException_ptr = (NSAException_t)GetSymbol(libHandle, "NSAException")))
	{
		return -1;
	}

	rv = NSAException_ptr(username, exceptionCode, description, nsaClientData, transID);

	return rv;
}


int
dsoNSASysInfo(
const char *username,
const char *applang,
const char *version,
const char *edition,
const char *build,
const char *licenseStat,
const char *nsaClientData,
uint32_t *transID ,
void *libHandle
)
{
	int				rv;
	NSASysInfo_t	NSASysInfo_ptr = NULL;


	if (!(NSASysInfo_ptr = (NSASysInfo_t)GetSymbol(libHandle, "NSASysInfo")))
	{
		return -1;
	}

	rv = NSASysInfo_ptr(username, applang, version, edition, build, licenseStat, nsaClientData, transID);

	return rv;
}


int
dsoNSASendCache(
const char *username,
uint32_t *transID ,
void *libHandle
)
{
	int				rv;
	NSASendCache_t	NSASendCache_ptr = NULL;


	if (!(NSASendCache_ptr = (NSASendCache_t)GetSymbol(libHandle, "NSASendCache")))
	{
		return -1;
	}

	rv = NSASendCache_ptr(username, transID);

	return rv;
}


int
dsoNSAGetPrivacy(
void *libHandle
)
{
	int				rv;
	NSAGetPrivacy_t	NSAGetPrivacy_ptr = NULL;


	if (!(NSAGetPrivacy_ptr = (NSAGetPrivacy_t)GetSymbol(libHandle, "NSAGetPrivacy")))
	{
		return -1;
	}

	rv = NSAGetPrivacy_ptr();

	return rv;
}


int
dsoNSASetPrivacy(
uint16_t nsaPriv ,
void *libHandle
)
{
	int				rv;
	NSASetPrivacy_t	NSASetPrivacy_ptr = NULL;


	if (!(NSASetPrivacy_ptr = (NSASetPrivacy_t)GetSymbol(libHandle, "NSASetPrivacy")))
	{
		return -1;
	}

	rv = NSASetPrivacy_ptr(nsaPriv);

	return rv;
}


int
dsoNSAGetStats(
char **nsaStats ,
void *libHandle
)
{
	int				rv;
	NSAGetStats_t	NSAGetStats_ptr = NULL;


	if (!(NSAGetStats_ptr = (NSAGetStats_t)GetSymbol(libHandle, "NSAGetStats")))
	{
		return -1;
	}

	rv = NSAGetStats_ptr(nsaStats);

	return rv;
}


int
dsoNSAFree(
void *memptr ,
void *libHandle
)
{
	int				rv;
	NSAFree_t	NSAFree_ptr = NULL;


	if (!(NSAFree_ptr = (NSAFree_t)GetSymbol(libHandle, "NSAFree")))
	{
		return -1;
	}

	rv = NSAFree_ptr(memptr);

	return rv;
}


