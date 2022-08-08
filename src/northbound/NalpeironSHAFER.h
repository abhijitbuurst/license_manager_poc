#ifndef _NALPEIRONSHAFER_H_
#define _NALPEIRONSHAFER_H_

#include <inttypes.h>

#if defined(__APPLE__)
// Mac OS X Specific header stuff here
    #include <TargetConditionals.h>
    #define WINAPI
#endif

typedef int (*NalpLibOpen_t)( const char *xmlParams );
typedef int (*NalpLibClose_t)( );
typedef int (*NalpGetErrorMsg_t)( int nalpErrorNo, char **errorMsg );
typedef int (*NalpGetFilepath_t)( const char *extension, char **filepath );


#endif // _NALPEIRONSHAFER_H_
