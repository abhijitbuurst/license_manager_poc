#ifndef _NALPEIRONNSL_H_
#define _NALPEIRONNSL_H_

#include <inttypes.h>

#if defined(__APPLE__)
// Mac OS X Specific header stuff here
    #include <TargetConditionals.h>
    #define WINAPI
#endif

typedef int (*NSLValidateLibrary_t)( uint32_t custID, uint32_t prodID );
typedef int (*NSLGetVersion_t)( char **version );
typedef int (*NSLGetComputerID_t)( char **computerID );
typedef int (*NSLGetHostName_t)( char **hostName );
typedef int (*NSLTestConnection_t)( );
typedef int (*NSLTestConnection2_t)( uint32_t connTimeOut, uint32_t transTimeOut );
typedef int (*NSLSetCredentials_t)( const char *username, const char *password, int64_t (*userAuthenticator)(char *uname, char *pwd, void *inData, char **authToken, uint32_t *outDataSize, void **outData), uint32_t inDataSize, void *inData );
typedef int (*NSLSetCredentialsSSO_t)( const unsigned char *ssoToken, uint64_t *ssoExpEpoch, char *inData );
typedef int (*NSLCheckCredentials_t)( );
typedef int (*NSLGetCredentials_t)( int64_t *lastRet, char **username, void **inData, void **outData );
typedef int (*NSLClearCredentials_t)( );
typedef int (*NSLGetLeaseExpSec_t)( uint32_t *sec, uint32_t *expEpoch );
typedef int (*NSLGetLeaseExpDate_t)( char **Date );
typedef int (*NSLGetMaintExpSec_t)( uint32_t *sec, uint32_t *expEpoch );
typedef int (*NSLGetMaintExpDate_t)( char **Date );
typedef int (*NSLGetSubExpSec_t)( uint32_t *sec, uint32_t *expEpoch );
typedef int (*NSLGetSubExpDate_t)( char **Date );
typedef int (*NSLGetTrialExpSec_t)( uint32_t *sec, uint32_t *expEpoch );
typedef int (*NSLGetTrialExpDate_t)( char **Date );
typedef int (*NSLGetLicenseStatus_t)( int32_t *licenseStatus );
typedef int (*NSLGetVMInfo_t)( uint32_t *isVM, char **vmType );
typedef int (*NSLGetLicenseInfo_t)( uint32_t *licenseType, uint32_t *actType );
typedef int (*NSLGetLibraryOptions_t)( uint64_t *libOptions );
typedef int (*NSLGetLicenseCode_t)( char **licenseCode );
typedef int (*NSLGetTimeStamp_t)( uint64_t *timeStamp );
typedef int (*NSLGetLicense_t)( const char *authentication, int32_t *licenseStatus, const char *xmlRegInfo );
typedef int (*NSLObtainLicense_t)( const char *authentication, int32_t *licenseStatus, const char *xmlRegInfo, const char *specialID );
typedef int (*NSLReturnLicense_t)( const char *authentication, int32_t *licenseStatus );
typedef int (*NSLImportCertificate_t)( const char *authentication, int32_t *licenseStatus, char *certContainer );
typedef int (*NSLGetActivationCertReq_t)( const char *authentication, const char *xmlRegInfo, char **cert );
typedef int (*NSLRequestActivationCert_t)( const char *authentication, const char *xmlRegInfo, const char *specialID, char **cert );
typedef int (*NSLGetDeactivationCertReq_t)( const char *authentication, char **cert );
typedef int (*NSLGetNumbAvailProc_t)( uint32_t *maxProc, uint32_t *availProc );
typedef int (*NSLCheckoutFeature_t)( const char *featureName, int32_t *featureStat, const char *authentication );
typedef int (*NSLReturnFeature_t)( const char *featureName, const char *authentication );
typedef int (*NSLGetFeatureStatus_t)( const char *featureName, int32_t *featureStatus );
typedef int (*NSLCheckoutPool_t)( const char *poolName, uint32_t poolAmt, int32_t *poolStatus, const char *authentication );
typedef int (*NSLReturnPool_t)( const char *poolName, uint32_t poolAmt, const char *authentication );
typedef int (*NSLGetPoolStatus_t)( const char *poolName, uint32_t *poolAmt, int32_t *poolStatus );
typedef int (*NSLGetPoolInfo_t)( const char *poolName, uint32_t *poolMax, uint32_t *poolAmt, int32_t *poolStatus );
typedef int (*NSLCheckoutTokens_t)( const char *tokenName, uint32_t tokenAmt, int32_t *tokenStatus, const char *authentication );
typedef int (*NSLConsumeTokens_t)( const char *tokenName, uint32_t tokenAmt, const char *authentication );
typedef int (*NSLReturnTokens_t)( const char *poolName, uint32_t poolAmt, const char *authentication );
typedef int (*NSLGetTokenInfo_t)( const char *tokenName, uint32_t *tokenMax, uint32_t *tokenAmt, int32_t *tokenStatus );
typedef int (*NSLGetUDFValue_t)( const char *udfName, char **udfValue );
typedef int (*NSLReadSecStore_t)( const char *rawKey, const char *storeName, char **storeValue );
typedef int (*NSLWriteSecStore_t)( uint32_t storeSize, const char *rawKey, const char *storeName, const char *storeValue );
typedef int (*NSLRemoteCall_t)( const char *rpcName, char **rpcReturn, ... );
typedef int (*NSLRemoteCallV_t)( const char *rpcName, char **rpcReturn, char **rpcVarNames, char **rpcVarVals );
typedef int (*NSLFree_t)( void *memptr );


#endif // _NALPEIRONNSL_H_
