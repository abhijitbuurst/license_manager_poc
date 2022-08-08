#ifndef __INIT_H__
#define __INIT_H__

//Used with alarm to indicate it is time to check license status
unsigned int			TIMETOCHECKLICENSE = 0;

int
OpenLibrary(
char				*xmlParams,
uint32_t			custID,
uint32_t			prodID
);

int
GetLibraryInfo(
);

int
checkLicenseStatus(
char			*licenseCode,
int32_t			*licenseStatus,
uint32_t		*licenseType,
uint32_t		*actType,
char			*xmlRegInfo
);

int
updateLicense(
);

int
doFeature(
char			*featcode,
char			*nsaUsername
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

int
SetupLib(
char            *libPath,
void            **libHandle
);

void *
GetSymbol(
void                *libHandle,
const char          *symName
);

int
constParams(
char				**xmlParams,
int					security,
char				*WorkDir,
char				*LogLevel
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


#endif //__INIT_H__

/*
 * vim:tabstop=4
 * vim:shiftwidth=4
 */

