
#include <stdlib.h>

#if ! defined (WIN32)
#include <unistd.h>
#endif

// #include <inttypes.h>
// #include "stdint.h"

#include "../utility/libHelper.h"
#include "../NalperionInterface/NalpeironSHAFER.h"
#include "dsoSHAFER.h"

int
dsoNalpLibOpen(
const char *xmlParams ,
void *libHandle
)
{
	int				rv;
	NalpLibOpen_t	NalpLibOpen_ptr = NULL;


	if (!(NalpLibOpen_ptr = (NalpLibOpen_t)GetSymbol(libHandle, "NalpLibOpen")))
	{
		return -1;
	}

	rv = NalpLibOpen_ptr(xmlParams);

	return rv;
}


int
dsoNalpLibClose(
void *libHandle
)
{
	int				rv;
	NalpLibClose_t	NalpLibClose_ptr = NULL;


	if (!(NalpLibClose_ptr = (NalpLibClose_t)GetSymbol(libHandle, "NalpLibClose")))
	{
		return -1;
	}

	rv = NalpLibClose_ptr();

	return rv;
}


int
dsoNalpGetErrorMsg(
int nalpErrorNo,
char **errorMsg ,
void *libHandle
)
{
	int				rv;
	NalpGetErrorMsg_t	NalpGetErrorMsg_ptr = NULL;


	if (!(NalpGetErrorMsg_ptr = (NalpGetErrorMsg_t)GetSymbol(libHandle, "NalpGetErrorMsg")))
	{
		return -1;
	}

	rv = NalpGetErrorMsg_ptr(nalpErrorNo, errorMsg);

	return rv;
}


int
dsoNalpGetFilepath(
const char *extension,
char **filepath ,
void *libHandle
)
{
	int				rv;
	NalpGetFilepath_t	NalpGetFilepath_ptr = NULL;


	if (!(NalpGetFilepath_ptr = (NalpGetFilepath_t)GetSymbol(libHandle, "NalpGetFilepath")))
	{
		return -1;
	}

	rv = NalpGetFilepath_ptr(extension, filepath);

	return rv;
}


