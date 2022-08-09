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
#include <inttypes.h>


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

