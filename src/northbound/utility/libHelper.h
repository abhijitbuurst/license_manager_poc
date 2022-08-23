//
// libHelper.h
//
// Handle symbol lookup across OSes.
//

#if ! defined (WIN32)
#include <unistd.h>
#else
#include <Windows.h>
#endif

int
OpenLib(
char			*libPath,
void			**libHandle
);

void *
GetSymbol(
void				*libHandle,
const char  		*symName
);

/*!
 * Return library access error information
 */
void
doDsoError(
);

int
CloseLib(
void			*libHandle
);

/*
 * vim:tabstop=4
 * vim:shiftwidth=4
 */

