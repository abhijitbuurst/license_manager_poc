#ifndef _DSONSA_H_
#define _DSONSA_H_


#include <inttypes.h>


int
dsoNSAValidateLibrary(
uint32_t custID,
uint32_t prodID ,
void *libHandle
);

int
dsoNSAGetVersion(
char **version ,
void *libHandle
);

int
dsoNSAGetComputerID(
char **computerID ,
void *libHandle
);

int
dsoNSAGetHostName(
char **hostName ,
void *libHandle
);

int
dsoNSATestConnection2(
const char *nsaClientData,
uint32_t *transID ,
void *libHandle
);

int
dsoNSATestConnection(
const char *nsaClientData,
uint32_t *transID ,
void *libHandle
);

int
dsoNSAAppStart(
const char *username,
const char *nsaClientData,
uint32_t *transID ,
void *libHandle
);

int
dsoNSAAppStop(
const char *username,
const char *nsaClientData,
uint32_t *transID ,
void *libHandle
);

int
dsoNSALogin(
const char *username,
const char *nsaClientData,
uint32_t *transID ,
void *libHandle
);

int
dsoNSALogout(
const char *username,
const char *nsaClientData,
uint32_t *transID ,
void *libHandle
);

int
dsoNSAFeatureStart(
const char *username,
const char *featureCode,
const char *nsaClientData,
uint32_t *transID ,
void *libHandle
);

int
dsoNSAFeatureStop(
const char *username,
const char *featureCode,
const char *nsaClientData,
uint32_t *transID ,
void *libHandle
);

int
dsoNSAException(
const char *username,
const char *exceptionCode,
const char *description,
const char *nsaClientData,
uint32_t *transID ,
void *libHandle
);

int
dsoNSASysInfo(
const char *username,
const char *applang,
const char *version,
const char *edition,
const char *build,
const char *licenseStat,
const char *nsaClientData,
uint32_t *transID ,
void *libHandle
);

int
dsoNSASendCache(
const char *username,
uint32_t *transID ,
void *libHandle
);

int
dsoNSAGetPrivacy(
void *libHandle
);

int
dsoNSASetPrivacy(
uint16_t nsaPriv ,
void *libHandle
);

int
dsoNSAGetStats(
char **nsaStats ,
void *libHandle
);

int
dsoNSAFree(
void *memptr ,
void *libHandle
);



#endif // _DSONSA_H_
