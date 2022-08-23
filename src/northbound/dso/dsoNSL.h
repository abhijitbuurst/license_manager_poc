#ifndef _DSONSL_H_
#define _DSONSL_H_


// #include <inttypes.h>
#include "stdint.h"

typedef int64_t (*USERAUTH)(char *uname, char *pword, void *inData, char **authToken, uint32_t *outDataSize, void **outData);


int
dsoNSLValidateLibrary(
uint32_t custID,
uint32_t prodID ,
void *libHandle
);

int
dsoNSLGetVersion(
char **version ,
void *libHandle
);

int
dsoNSLGetComputerID(
char **computerID ,
void *libHandle
);

int
dsoNSLGetHostName(
char **hostName ,
void *libHandle
);

int
dsoNSLTestConnection(
void *libHandle
);

int
dsoNSLTestConnection2(
uint32_t connTimeOut,
uint32_t transTimeOut ,
void *libHandle
);

int
dsoNSLSetCredentials(
const char *username,
const char *password,
USERAUTH userAuthenticator,
uint32_t inDataSize,
void *inData ,
void *libHandle
);

int
dsoNSLSetCredentialsSSO(
const unsigned char *ssoToken,
uint64_t *ssoExpEpoch,
char *inData ,
void *libHandle
);

int
dsoNSLCheckCredentials(
void *libHandle
);

int
dsoNSLGetCredentials(
int64_t *lastRet,
char **username,
void **inData,
void **outData ,
void *libHandle
);

int
dsoNSLClearCredentials(
void *libHandle
);

int
dsoNSLGetLeaseExpSec(
uint32_t *sec,
uint32_t *expEpoch ,
void *libHandle
);

int
dsoNSLGetLeaseExpDate(
char **Date ,
void *libHandle
);

int
dsoNSLGetMaintExpSec(
uint32_t *sec,
uint32_t *expEpoch ,
void *libHandle
);

int
dsoNSLGetMaintExpDate(
char **Date ,
void *libHandle
);

int
dsoNSLGetSubExpSec(
uint32_t *sec,
uint32_t *expEpoch ,
void *libHandle
);

int
dsoNSLGetSubExpDate(
char **Date ,
void *libHandle
);

int
dsoNSLGetTrialExpSec(
uint32_t *sec,
uint32_t *expEpoch ,
void *libHandle
);

int
dsoNSLGetTrialExpDate(
char **Date ,
void *libHandle
);

int
dsoNSLGetLicenseStatus(
int32_t *licenseStatus ,
void *libHandle
);

int
dsoNSLGetVMInfo(
uint32_t *isVM,
char **vmType ,
void *libHandle
);

int
dsoNSLGetLicenseInfo(
uint32_t *licenseType,
uint32_t *actType ,
void *libHandle
);

int
dsoNSLGetLibraryOptions(
uint64_t *libOptions ,
void *libHandle
);

int
dsoNSLGetLicenseCode(
char **licenseCode ,
void *libHandle
);

int
dsoNSLGetTimeStamp(
uint64_t *timeStamp ,
void *libHandle
);

int
dsoNSLGetLicense(
const char *authentication,
int32_t *licenseStatus,
const char *xmlRegInfo ,
void *libHandle
);

int
dsoNSLObtainLicense(
const char *authentication,
int32_t *licenseStatus,
const char *xmlRegInfo,
const char *specialID ,
void *libHandle
);

int
dsoNSLReturnLicense(
const char *authentication,
int32_t *licenseStatus ,
void *libHandle
);

int
dsoNSLImportCertificate(
const char *authentication,
int32_t *licenseStatus,
char *certContainer ,
void *libHandle
);

int
dsoNSLGetActivationCertReq(
const char *authentication,
const char *xmlRegInfo,
char **cert ,
void *libHandle
);

int
dsoNSLRequestActivationCert(
const char *authentication,
const char *xmlRegInfo,
const char *specialID,
char **cert ,
void *libHandle
);

int
dsoNSLGetDeactivationCertReq(
const char *authentication,
char **cert ,
void *libHandle
);

int
dsoNSLGetNumbAvailProc(
uint32_t *maxProc,
uint32_t *availProc ,
void *libHandle
);

int
dsoNSLCheckoutFeature(
const char *featureName,
int32_t *featureStat,
const char *authentication ,
void *libHandle
);

int
dsoNSLReturnFeature(
const char *featureName,
const char *authentication ,
void *libHandle
);

int
dsoNSLGetFeatureStatus(
const char *featureName,
int32_t *featureStatus ,
void *libHandle
);

int
dsoNSLCheckoutPool(
const char *poolName,
uint32_t poolAmt,
int32_t *poolStatus,
const char *authentication ,
void *libHandle
);

int
dsoNSLReturnPool(
const char *poolName,
uint32_t poolAmt,
const char *authentication ,
void *libHandle
);

int
dsoNSLGetPoolStatus(
const char *poolName,
uint32_t *poolAmt,
int32_t *poolStatus ,
void *libHandle
);

int
dsoNSLGetPoolInfo(
const char *poolName,
uint32_t *poolMax,
uint32_t *poolAmt,
int32_t *poolStatus ,
void *libHandle
);

int
dsoNSLCheckoutTokens(
const char *tokenName,
uint32_t tokenAmt,
int32_t *tokenStatus,
const char *authentication ,
void *libHandle
);

int
dsoNSLConsumeTokens(
const char *tokenName,
uint32_t tokenAmt,
const char *authentication ,
void *libHandle
);

int
dsoNSLReturnTokens(
const char *poolName,
uint32_t poolAmt,
const char *authentication ,
void *libHandle
);

int
dsoNSLGetTokenInfo(
const char *tokenName,
uint32_t *tokenMax,
uint32_t *tokenAmt,
int32_t *tokenStatus ,
void *libHandle
);

int
dsoNSLGetUDFValue(
const char *udfName,
char **udfValue ,
void *libHandle
);

int
dsoNSLReadSecStore(
const char *rawKey,
const char *storeName,
char **storeValue ,
void *libHandle
);

int
dsoNSLWriteSecStore(
uint32_t storeSize,
const char *rawKey,
const char *storeName,
const char *storeValue ,
void *libHandle
);

int
dsoNSLRemoteCallV(
const char *rpcName,
char **rpcReturn,
char **rpcVarNames,
char **rpcVarVals ,
void *libHandle
);

int
dsoNSLFree(
void *memptr ,
void *libHandle
);



#endif // _DSONSL_H_
