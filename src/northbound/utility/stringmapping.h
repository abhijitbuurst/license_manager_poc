#ifndef __STRINGMAPPING_H__
#define __STRINGMAPPING_H__

// #include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include "stdint.h"
#include <stdlib.h>
#include "../dso/dsoNSL.h"

void
licStat2Str(
int			licStat,
char		**licenseStatus
);

void
licType2Str(
uint32_t	licType,
char		**licenseType,
char		**expDate,
void		*libHandle,
int			offset
);

void
licAct2Str(
uint32_t	actType,
char		**activationType
);

void
featStat2Str(
int			featStat,
char		**featureStatus
);

#endif