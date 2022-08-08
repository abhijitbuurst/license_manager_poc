//
// libHelper.c
//
// Handle symbol lookup across OSes.
//

#if ! defined (WIN32)
#include <unistd.h>
#include <dlfcn.h>
#else
#include <Windows.h>
#endif

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "libHelper.h"

int
OpenLib(
char			*libPath,
void			**libHandle
)
{
    int         retVal;
    struct stat stbuf;


    if (libPath == NULL)
    {
        return -1;
    }

    retVal = stat(libPath, &stbuf);

    if (retVal != 0)
    {
        fprintf(stderr, "The library %s does not exist "
            "or has incorrect permissions\n", libPath);
        return -1;
    }

#if defined (WIN32)
    *libHandle = (void *)LoadLibraryA(libPath);
#else
    *libHandle = dlopen(libPath, RTLD_LOCAL | RTLD_LAZY);
#endif

    if (*libHandle == NULL)
    {
        fprintf(stderr, "libFileMgmt is corrupt\n");
        doDsoError();
        return -1;
    }

    return 0;
}


void *
GetSymbol(
void				*libHandle,
const char  		*symName
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


/*!
 * Return library access error information
 */
void
doDsoError(
)
{
#if ! defined (WIN32)
        fprintf(stderr, "error: %s\n", dlerror());
#else
        fprintf(stderr, "error\n");
#endif
        return;
}


int
CloseLib(
void			*libHandle
)
{
#if ! defined (WIN32)
    dlclose(libHandle);
#else
    FreeLibrary((HINSTANCE)libHandle);
#endif

	return 0;
}


/*
 * vim:tabstop=4
 * vim:shiftwidth=4
 */

