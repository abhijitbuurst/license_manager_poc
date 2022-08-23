#ifndef _DSOSHAFER_H_
#define _DSOSHAFER_H_


// #include <inttypes.h>
#include "stdint.h"


int
dsoNalpLibOpen(
const char *xmlParams ,
void *libHandle
);

int
dsoNalpLibClose(
void *libHandle
);

int
dsoNalpGetErrorMsg(
int nalpErrorNo,
char **errorMsg ,
void *libHandle
);

int
dsoNalpGetFilepath(
const char *extension,
char **filepath ,
void *libHandle
);



#endif // _DSOSHAFER_H_
