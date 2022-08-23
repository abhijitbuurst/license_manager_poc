#ifndef __INIT_H__
#define __INIT_H__


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
// #include <inttypes.h>
#include "stdint.h"
#include "utility/stringmapping.h"

#define clear() printf("\033[H\033[J")
#define gotoXY(x,y) printf("\033[%u;%uH", (y), (x))

//These are the security values stamped into your library. They
// should be changed to match your values.
// NOTE When selecting yauth and zauth it is important that they
// NOT be multiples of each other (for instance, yauth=300 zauth=600).
// If they are multiples the resulting security offset will NOT be
// random.

typedef struct
{
    int				custID;
    int				prodID;
    unsigned int	security;
    int				offset;
    unsigned int	xauth;
    unsigned int	yauth;
    unsigned int	zauth;
    char            *libPath;
    void            *libHandle;
}SetupParameter;

typedef struct
{
    int		https;
    char	*logLevel;
    char	*workDir;
    char	*clientName;
    int		security;    
}XmlParameters;

typedef struct
{
    int				offlineState;
    char			*authentication;
    char			*licenseInfo;
    char			*licenseStatus;
    char			*expDate;
    char			*expLease;
    char			*compID;
}LicenseInformation;

//Used with alarm to indicate it is time to check license status

void 
setConfigParameter(
int				CUSTID,
int				PRODID,
unsigned int	XAUTH,
unsigned int	YAUTH,
unsigned int	ZAUTH,
char            *libPath
);

int
GetLibraryInfo(
);

void *
GetSymbol(
void                *libHandle,
const char          *symName
);

int
SetupLib(
);

void 
generateSecurityOffset(
);

void initlicenseInfoStruct(
LicenseInformation** licenseInformation 
);

int
initLib(
);

int
openLibrary(
char				*xmlParams,
char				*libPath,
void				**libHandle
);

int
closeLibrary(
void			*libHandle
);

int
constParams(
XmlParameters       *param,
char				**xmlParams
);

int
addXMLParam(
char	**xmlParams,
char	*xmlName,
char	*xmlValue
);

int
usage(
);

int
cleanup(
);

int
getNSLLibraryInfo(
char			**nslVersion,
char			**nsaVersion,
char			**compID,
char			**nslHostName,
char			**nsaHostName,
void			*libHandle
);

int
getLicenseInfo(
LicenseInformation **licenseInformation
);





void
handleError(
int         nslErrorNo,
char        **nalpMsg,
void        *libHandle
);

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
);

int
outputLicenseInfo(
);

int outInformation(
char            *libPath
);



int
checkLicenseStatus(
void		    **libHandle,
char			*licenseCode,
int32_t			*licenseStatus,
uint32_t		*licenseType,
uint32_t		*actType,
char			*xmlRegInfo
);

int
GetLicenseForCurrentUser(
void            **libHandle
);

#if ! defined (WIN32)
void
sig_handler(
int signo
);
#else
void
CALLBACK
sig_handler(
UINT        uTimerID,
UINT        uMsg,
DWORD_PTR   dwUser,
DWORD_PTR   dw1,
DWORD_PTR   dw2
);
#endif


#endif //__INIT_H__

/*
 * vim:tabstop=4
 * vim:shiftwidth=4
 */

