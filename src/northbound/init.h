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


//Used with alarm to indicate it is time to check license status



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
char            *libPath,
void            **libHandle
);


int
initLib(
char            *libPath,
void            **libHandle
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
int					https,
char				*logLevel,
char				*workDir,
char				*clientName,
int					security,
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
int				offset,
char			**nslVersion,
char			**nsaVersion,
char			**compID,
char			**nslHostName,
char			**nsaHostName,
void			*libHandle
);

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
unsigned int	*startX,
unsigned int	*startY,
int				offlineState,
char			*authentication,
char			*licenseInfo,
char			*licenseStatus,
char			*expDate,
char			*expLease
);

int outInformation(
char            *libPath
);

int 
setConfigParameter(
int				CUSTID,
int				PRODID,
unsigned int	XAUTH,
unsigned int	YAUTH,
unsigned int	ZAUTH
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

