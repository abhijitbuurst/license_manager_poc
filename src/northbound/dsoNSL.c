
#include <stdlib.h>

#if ! defined (WIN32)
#include <unistd.h>
#endif

#include <inttypes.h>

#include "libHelper.h"
#include "NalpeironNSL.h"
#include "dsoNSL.h"

int
dsoNSLValidateLibrary(
uint32_t custID,
uint32_t prodID ,
void *libHandle
)
{
	int				rv;
	NSLValidateLibrary_t	NSLValidateLibrary_ptr = NULL;

	if (!(NSLValidateLibrary_ptr = (NSLValidateLibrary_t)GetSymbol(libHandle, "NSLValidateLibrary")))
	{
		return -1;
	}

	rv = NSLValidateLibrary_ptr(custID, prodID);

	return rv;
}


int
dsoNSLGetVersion(
char **version ,
void *libHandle
)
{
	int				rv;
	NSLGetVersion_t	NSLGetVersion_ptr = NULL;


	if (!(NSLGetVersion_ptr = (NSLGetVersion_t)GetSymbol(libHandle, "NSLGetVersion")))
	{
		return -1;
	}

	rv = NSLGetVersion_ptr(version);

	return rv;
}


int
dsoNSLGetComputerID(
char **computerID ,
void *libHandle
)
{
	int				rv;
	NSLGetComputerID_t	NSLGetComputerID_ptr = NULL;


	if (!(NSLGetComputerID_ptr = (NSLGetComputerID_t)GetSymbol(libHandle, "NSLGetComputerID")))
	{
		return -1;
	}

	rv = NSLGetComputerID_ptr(computerID);

	return rv;
}


int
dsoNSLGetHostName(
char **hostName ,
void *libHandle
)
{
	int				rv;
	NSLGetHostName_t	NSLGetHostName_ptr = NULL;


	if (!(NSLGetHostName_ptr = (NSLGetHostName_t)GetSymbol(libHandle, "NSLGetHostName")))
	{
		return -1;
	}

	rv = NSLGetHostName_ptr(hostName);

	return rv;
}


int
dsoNSLTestConnection(
void *libHandle
)
{
	int				rv;
	NSLTestConnection_t	NSLTestConnection_ptr = NULL;


	if (!(NSLTestConnection_ptr = (NSLTestConnection_t)GetSymbol(libHandle, "NSLTestConnection")))
	{
		return -1;
	}

	rv = NSLTestConnection_ptr();

	return rv;
}


int
dsoNSLTestConnection2(
uint32_t connTimeOut,
uint32_t transTimeOut ,
void *libHandle
)
{
	int				rv;
	NSLTestConnection2_t	NSLTestConnection2_ptr = NULL;


	if (!(NSLTestConnection2_ptr = (NSLTestConnection2_t)GetSymbol(libHandle, "NSLTestConnection2")))
	{
		return -1;
	}

	rv = NSLTestConnection2_ptr(connTimeOut, transTimeOut);

	return rv;
}


int
dsoNSLSetCredentials(
const char *username,
const char *password,
USERAUTH userAuthenticator,
uint32_t inDataSize,
void *inData ,
void *libHandle
)
{
	int				rv;
	NSLSetCredentials_t	NSLSetCredentials_ptr = NULL;


	if (!(NSLSetCredentials_ptr = (NSLSetCredentials_t)GetSymbol(libHandle, "NSLSetCredentials")))
	{
		return -1;
	}

	rv = NSLSetCredentials_ptr(username, password, userAuthenticator, inDataSize, inData);

	return rv;
}


int
dsoNSLSetCredentialsSSO(
const unsigned char *ssoToken,
uint64_t *ssoExpEpoch,
char *inData ,
void *libHandle
)
{
	int				rv;
	NSLSetCredentialsSSO_t	NSLSetCredentialsSSO_ptr = NULL;


	if (!(NSLSetCredentialsSSO_ptr = (NSLSetCredentialsSSO_t)GetSymbol(libHandle, "NSLSetCredentialsSSO")))
	{
		return -1;
	}

	rv = NSLSetCredentialsSSO_ptr(ssoToken, ssoExpEpoch, inData);

	return rv;
}


int
dsoNSLCheckCredentials(
void *libHandle
)
{
	int				rv;
	NSLCheckCredentials_t	NSLCheckCredentials_ptr = NULL;


	if (!(NSLCheckCredentials_ptr = (NSLCheckCredentials_t)GetSymbol(libHandle, "NSLCheckCredentials")))
	{
		return -1;
	}

	rv = NSLCheckCredentials_ptr();

	return rv;
}


int
dsoNSLGetCredentials(
int64_t *lastRet,
char **username,
void **inData,
void **outData ,
void *libHandle
)
{
	int				rv;
	NSLGetCredentials_t	NSLGetCredentials_ptr = NULL;


	if (!(NSLGetCredentials_ptr = (NSLGetCredentials_t)GetSymbol(libHandle, "NSLGetCredentials")))
	{
		return -1;
	}

	rv = NSLGetCredentials_ptr(lastRet, username, inData, outData);

	return rv;
}


int
dsoNSLClearCredentials(
void *libHandle
)
{
	int				rv;
	NSLClearCredentials_t	NSLClearCredentials_ptr = NULL;


	if (!(NSLClearCredentials_ptr = (NSLClearCredentials_t)GetSymbol(libHandle, "NSLClearCredentials")))
	{
		return -1;
	}

	rv = NSLClearCredentials_ptr();

	return rv;
}


int
dsoNSLGetLeaseExpSec(
uint32_t *sec,
uint32_t *expEpoch ,
void *libHandle
)
{
	int				rv;
	NSLGetLeaseExpSec_t	NSLGetLeaseExpSec_ptr = NULL;


	if (!(NSLGetLeaseExpSec_ptr = (NSLGetLeaseExpSec_t)GetSymbol(libHandle, "NSLGetLeaseExpSec")))
	{
		return -1;
	}

	rv = NSLGetLeaseExpSec_ptr(sec, expEpoch);

	return rv;
}


int
dsoNSLGetLeaseExpDate(
char **Date ,
void *libHandle
)
{
	int				rv;
	NSLGetLeaseExpDate_t	NSLGetLeaseExpDate_ptr = NULL;


	if (!(NSLGetLeaseExpDate_ptr = (NSLGetLeaseExpDate_t)GetSymbol(libHandle, "NSLGetLeaseExpDate")))
	{
		return -1;
	}

	rv = NSLGetLeaseExpDate_ptr(Date);

	return rv;
}


int
dsoNSLGetMaintExpSec(
uint32_t *sec,
uint32_t *expEpoch ,
void *libHandle
)
{
	int				rv;
	NSLGetMaintExpSec_t	NSLGetMaintExpSec_ptr = NULL;


	if (!(NSLGetMaintExpSec_ptr = (NSLGetMaintExpSec_t)GetSymbol(libHandle, "NSLGetMaintExpSec")))
	{
		return -1;
	}

	rv = NSLGetMaintExpSec_ptr(sec, expEpoch);

	return rv;
}


int
dsoNSLGetMaintExpDate(
char **Date ,
void *libHandle
)
{
	int				rv;
	NSLGetMaintExpDate_t	NSLGetMaintExpDate_ptr = NULL;


	if (!(NSLGetMaintExpDate_ptr = (NSLGetMaintExpDate_t)GetSymbol(libHandle, "NSLGetMaintExpDate")))
	{
		return -1;
	}

	rv = NSLGetMaintExpDate_ptr(Date);

	return rv;
}


int
dsoNSLGetSubExpSec(
uint32_t *sec,
uint32_t *expEpoch ,
void *libHandle
)
{
	int				rv;
	NSLGetSubExpSec_t	NSLGetSubExpSec_ptr = NULL;


	if (!(NSLGetSubExpSec_ptr = (NSLGetSubExpSec_t)GetSymbol(libHandle, "NSLGetSubExpSec")))
	{
		return -1;
	}

	rv = NSLGetSubExpSec_ptr(sec, expEpoch);

	return rv;
}


int
dsoNSLGetSubExpDate(
char **Date ,
void *libHandle
)
{
	int				rv;
	NSLGetSubExpDate_t	NSLGetSubExpDate_ptr = NULL;


	if (!(NSLGetSubExpDate_ptr = (NSLGetSubExpDate_t)GetSymbol(libHandle, "NSLGetSubExpDate")))
	{
		return -1;
	}

	rv = NSLGetSubExpDate_ptr(Date);

	return rv;
}


int
dsoNSLGetTrialExpSec(
uint32_t *sec,
uint32_t *expEpoch ,
void *libHandle
)
{
	int				rv;
	NSLGetTrialExpSec_t	NSLGetTrialExpSec_ptr = NULL;


	if (!(NSLGetTrialExpSec_ptr = (NSLGetTrialExpSec_t)GetSymbol(libHandle, "NSLGetTrialExpSec")))
	{
		return -1;
	}

	rv = NSLGetTrialExpSec_ptr(sec, expEpoch);

	return rv;
}


int
dsoNSLGetTrialExpDate(
char **Date ,
void *libHandle
)
{
	int				rv;
	NSLGetTrialExpDate_t	NSLGetTrialExpDate_ptr = NULL;


	if (!(NSLGetTrialExpDate_ptr = (NSLGetTrialExpDate_t)GetSymbol(libHandle, "NSLGetTrialExpDate")))
	{
		return -1;
	}

	rv = NSLGetTrialExpDate_ptr(Date);

	return rv;
}


int
dsoNSLGetLicenseStatus(
int32_t *licenseStatus ,
void *libHandle
)
{
	int				rv;
	NSLGetLicenseStatus_t	NSLGetLicenseStatus_ptr = NULL;


	if (!(NSLGetLicenseStatus_ptr = (NSLGetLicenseStatus_t)GetSymbol(libHandle, "NSLGetLicenseStatus")))
	{
		return -1;
	}

	rv = NSLGetLicenseStatus_ptr(licenseStatus);

	return rv;
}


int
dsoNSLGetVMInfo(
uint32_t *isVM,
char **vmType ,
void *libHandle
)
{
	int				rv;
	NSLGetVMInfo_t	NSLGetVMInfo_ptr = NULL;


	if (!(NSLGetVMInfo_ptr = (NSLGetVMInfo_t)GetSymbol(libHandle, "NSLGetVMInfo")))
	{
		return -1;
	}

	rv = NSLGetVMInfo_ptr(isVM, vmType);

	return rv;
}


int
dsoNSLGetLicenseInfo(
uint32_t *licenseType,
uint32_t *actType ,
void *libHandle
)
{
	int				rv;
	NSLGetLicenseInfo_t	NSLGetLicenseInfo_ptr = NULL;


	if (!(NSLGetLicenseInfo_ptr = (NSLGetLicenseInfo_t)GetSymbol(libHandle, "NSLGetLicenseInfo")))
	{
		return -1;
	}

	rv = NSLGetLicenseInfo_ptr(licenseType, actType);

	return rv;
}


int
dsoNSLGetLibraryOptions(
uint64_t *libOptions ,
void *libHandle
)
{
	int				rv;
	NSLGetLibraryOptions_t	NSLGetLibraryOptions_ptr = NULL;


	if (!(NSLGetLibraryOptions_ptr = (NSLGetLibraryOptions_t)GetSymbol(libHandle, "NSLGetLibraryOptions")))
	{
		return -1;
	}

	rv = NSLGetLibraryOptions_ptr(libOptions);

	return rv;
}


int
dsoNSLGetLicenseCode(
char **licenseCode ,
void *libHandle
)
{
	int				rv;
	NSLGetLicenseCode_t	NSLGetLicenseCode_ptr = NULL;


	if (!(NSLGetLicenseCode_ptr = (NSLGetLicenseCode_t)GetSymbol(libHandle, "NSLGetLicenseCode")))
	{
		return -1;
	}

	rv = NSLGetLicenseCode_ptr(licenseCode);

	return rv;
}


int
dsoNSLGetTimeStamp(
uint64_t *timeStamp ,
void *libHandle
)
{
	int				rv;
	NSLGetTimeStamp_t	NSLGetTimeStamp_ptr = NULL;


	if (!(NSLGetTimeStamp_ptr = (NSLGetTimeStamp_t)GetSymbol(libHandle, "NSLGetTimeStamp")))
	{
		return -1;
	}

	rv = NSLGetTimeStamp_ptr(timeStamp);

	return rv;
}


int
dsoNSLGetLicense(
const char *authentication,
int32_t *licenseStatus,
const char *xmlRegInfo ,
void *libHandle
)
{
	int				rv;
	NSLGetLicense_t	NSLGetLicense_ptr = NULL;


	if (!(NSLGetLicense_ptr = (NSLGetLicense_t)GetSymbol(libHandle, "NSLGetLicense")))
	{
		return -1;
	}

	rv = NSLGetLicense_ptr(authentication, licenseStatus, xmlRegInfo);

	return rv;
}


int
dsoNSLObtainLicense(
const char *authentication,
int32_t *licenseStatus,
const char *xmlRegInfo,
const char *specialID ,
void *libHandle
)
{
	int				rv;
	NSLObtainLicense_t	NSLObtainLicense_ptr = NULL;


	if (!(NSLObtainLicense_ptr = (NSLObtainLicense_t)GetSymbol(libHandle, "NSLObtainLicense")))
	{
		return -1;
	}

	rv = NSLObtainLicense_ptr(authentication, licenseStatus, xmlRegInfo, specialID);

	return rv;
}


int
dsoNSLReturnLicense(
const char *authentication,
int32_t *licenseStatus ,
void *libHandle
)
{
	int				rv;
	NSLReturnLicense_t	NSLReturnLicense_ptr = NULL;


	if (!(NSLReturnLicense_ptr = (NSLReturnLicense_t)GetSymbol(libHandle, "NSLReturnLicense")))
	{
		return -1;
	}

	rv = NSLReturnLicense_ptr(authentication, licenseStatus);

	return rv;
}


int
dsoNSLImportCertificate(
const char *authentication,
int32_t *licenseStatus,
char *certContainer ,
void *libHandle
)
{
	int				rv;
	NSLImportCertificate_t	NSLImportCertificate_ptr = NULL;


	if (!(NSLImportCertificate_ptr = (NSLImportCertificate_t)GetSymbol(libHandle, "NSLImportCertificate")))
	{
		return -1;
	}

	rv = NSLImportCertificate_ptr(authentication, licenseStatus, certContainer);

	return rv;
}


int
dsoNSLGetActivationCertReq(
const char *authentication,
const char *xmlRegInfo,
char **cert ,
void *libHandle
)
{
	int				rv;
	NSLGetActivationCertReq_t	NSLGetActivationCertReq_ptr = NULL;


	if (!(NSLGetActivationCertReq_ptr = (NSLGetActivationCertReq_t)GetSymbol(libHandle, "NSLGetActivationCertReq")))
	{
		return -1;
	}

	rv = NSLGetActivationCertReq_ptr(authentication, xmlRegInfo, cert);

	return rv;
}


int
dsoNSLRequestActivationCert(
const char *authentication,
const char *xmlRegInfo,
const char *specialID,
char **cert ,
void *libHandle
)
{
	int				rv;
	NSLRequestActivationCert_t	NSLRequestActivationCert_ptr = NULL;


	if (!(NSLRequestActivationCert_ptr = (NSLRequestActivationCert_t)GetSymbol(libHandle, "NSLRequestActivationCert")))
	{
		return -1;
	}

	rv = NSLRequestActivationCert_ptr(authentication, xmlRegInfo, specialID, cert);

	return rv;
}


int
dsoNSLGetDeactivationCertReq(
const char *authentication,
char **cert ,
void *libHandle
)
{
	int				rv;
	NSLGetDeactivationCertReq_t	NSLGetDeactivationCertReq_ptr = NULL;


	if (!(NSLGetDeactivationCertReq_ptr = (NSLGetDeactivationCertReq_t)GetSymbol(libHandle, "NSLGetDeactivationCertReq")))
	{
		return -1;
	}

	rv = NSLGetDeactivationCertReq_ptr(authentication, cert);

	return rv;
}


int
dsoNSLGetNumbAvailProc(
uint32_t *maxProc,
uint32_t *availProc ,
void *libHandle
)
{
	int				rv;
	NSLGetNumbAvailProc_t	NSLGetNumbAvailProc_ptr = NULL;


	if (!(NSLGetNumbAvailProc_ptr = (NSLGetNumbAvailProc_t)GetSymbol(libHandle, "NSLGetNumbAvailProc")))
	{
		return -1;
	}

	rv = NSLGetNumbAvailProc_ptr(maxProc, availProc);

	return rv;
}


int
dsoNSLCheckoutFeature(
const char *featureName,
int32_t *featureStat,
const char *authentication ,
void *libHandle
)
{
	int				rv;
	NSLCheckoutFeature_t	NSLCheckoutFeature_ptr = NULL;


	if (!(NSLCheckoutFeature_ptr = (NSLCheckoutFeature_t)GetSymbol(libHandle, "NSLCheckoutFeature")))
	{
		return -1;
	}

	rv = NSLCheckoutFeature_ptr(featureName, featureStat, authentication);

	return rv;
}


int
dsoNSLReturnFeature(
const char *featureName,
const char *authentication ,
void *libHandle
)
{
	int				rv;
	NSLReturnFeature_t	NSLReturnFeature_ptr = NULL;


	if (!(NSLReturnFeature_ptr = (NSLReturnFeature_t)GetSymbol(libHandle, "NSLReturnFeature")))
	{
		return -1;
	}

	rv = NSLReturnFeature_ptr(featureName, authentication);

	return rv;
}


int
dsoNSLGetFeatureStatus(
const char *featureName,
int32_t *featureStatus ,
void *libHandle
)
{
	int				rv;
	NSLGetFeatureStatus_t	NSLGetFeatureStatus_ptr = NULL;


	if (!(NSLGetFeatureStatus_ptr = (NSLGetFeatureStatus_t)GetSymbol(libHandle, "NSLGetFeatureStatus")))
	{
		return -1;
	}

	rv = NSLGetFeatureStatus_ptr(featureName, featureStatus);

	return rv;
}


int
dsoNSLCheckoutPool(
const char *poolName,
uint32_t poolAmt,
int32_t *poolStatus,
const char *authentication ,
void *libHandle
)
{
	int				rv;
	NSLCheckoutPool_t	NSLCheckoutPool_ptr = NULL;


	if (!(NSLCheckoutPool_ptr = (NSLCheckoutPool_t)GetSymbol(libHandle, "NSLCheckoutPool")))
	{
		return -1;
	}

	rv = NSLCheckoutPool_ptr(poolName, poolAmt, poolStatus, authentication);

	return rv;
}


int
dsoNSLReturnPool(
const char *poolName,
uint32_t poolAmt,
const char *authentication ,
void *libHandle
)
{
	int				rv;
	NSLReturnPool_t	NSLReturnPool_ptr = NULL;


	if (!(NSLReturnPool_ptr = (NSLReturnPool_t)GetSymbol(libHandle, "NSLReturnPool")))
	{
		return -1;
	}

	rv = NSLReturnPool_ptr(poolName, poolAmt, authentication);

	return rv;
}


int
dsoNSLGetPoolStatus(
const char *poolName,
uint32_t *poolAmt,
int32_t *poolStatus ,
void *libHandle
)
{
	int				rv;
	NSLGetPoolStatus_t	NSLGetPoolStatus_ptr = NULL;


	if (!(NSLGetPoolStatus_ptr = (NSLGetPoolStatus_t)GetSymbol(libHandle, "NSLGetPoolStatus")))
	{
		return -1;
	}

	rv = NSLGetPoolStatus_ptr(poolName, poolAmt, poolStatus);

	return rv;
}


int
dsoNSLGetPoolInfo(
const char *poolName,
uint32_t *poolMax,
uint32_t *poolAmt,
int32_t *poolStatus ,
void *libHandle
)
{
	int				rv;
	NSLGetPoolInfo_t	NSLGetPoolInfo_ptr = NULL;


	if (!(NSLGetPoolInfo_ptr = (NSLGetPoolInfo_t)GetSymbol(libHandle, "NSLGetPoolInfo")))
	{
		return -1;
	}

	rv = NSLGetPoolInfo_ptr(poolName, poolMax, poolAmt, poolStatus);

	return rv;
}


int
dsoNSLCheckoutTokens(
const char *tokenName,
uint32_t tokenAmt,
int32_t *tokenStatus,
const char *authentication ,
void *libHandle
)
{
	int				rv;
	NSLCheckoutTokens_t	NSLCheckoutTokens_ptr = NULL;


	if (!(NSLCheckoutTokens_ptr = (NSLCheckoutTokens_t)GetSymbol(libHandle, "NSLCheckoutTokens")))
	{
		return -1;
	}

	rv = NSLCheckoutTokens_ptr(tokenName, tokenAmt, tokenStatus, authentication);

	return rv;
}


int
dsoNSLConsumeTokens(
const char *tokenName,
uint32_t tokenAmt,
const char *authentication ,
void *libHandle
)
{
	int				rv;
	NSLConsumeTokens_t	NSLConsumeTokens_ptr = NULL;


	if (!(NSLConsumeTokens_ptr = (NSLConsumeTokens_t)GetSymbol(libHandle, "NSLConsumeTokens")))
	{
		return -1;
	}

	rv = NSLConsumeTokens_ptr(tokenName, tokenAmt, authentication);

	return rv;
}


int
dsoNSLReturnTokens(
const char *poolName,
uint32_t poolAmt,
const char *authentication ,
void *libHandle
)
{
	int				rv;
	NSLReturnTokens_t	NSLReturnTokens_ptr = NULL;


	if (!(NSLReturnTokens_ptr = (NSLReturnTokens_t)GetSymbol(libHandle, "NSLReturnTokens")))
	{
		return -1;
	}

	rv = NSLReturnTokens_ptr(poolName, poolAmt, authentication);

	return rv;
}


int
dsoNSLGetTokenInfo(
const char *tokenName,
uint32_t *tokenMax,
uint32_t *tokenAmt,
int32_t *tokenStatus ,
void *libHandle
)
{
	int				rv;
	NSLGetTokenInfo_t	NSLGetTokenInfo_ptr = NULL;


	if (!(NSLGetTokenInfo_ptr = (NSLGetTokenInfo_t)GetSymbol(libHandle, "NSLGetTokenInfo")))
	{
		return -1;
	}

	rv = NSLGetTokenInfo_ptr(tokenName, tokenMax, tokenAmt, tokenStatus);

	return rv;
}


int
dsoNSLGetUDFValue(
const char *udfName,
char **udfValue ,
void *libHandle
)
{
	int				rv;
	NSLGetUDFValue_t	NSLGetUDFValue_ptr = NULL;


	if (!(NSLGetUDFValue_ptr = (NSLGetUDFValue_t)GetSymbol(libHandle, "NSLGetUDFValue")))
	{
		return -1;
	}

	rv = NSLGetUDFValue_ptr(udfName, udfValue);

	return rv;
}


int
dsoNSLReadSecStore(
const char *rawKey,
const char *storeName,
char **storeValue ,
void *libHandle
)
{
	int				rv;
	NSLReadSecStore_t	NSLReadSecStore_ptr = NULL;


	if (!(NSLReadSecStore_ptr = (NSLReadSecStore_t)GetSymbol(libHandle, "NSLReadSecStore")))
	{
		return -1;
	}

	rv = NSLReadSecStore_ptr(rawKey, storeName, storeValue);

	return rv;
}


int
dsoNSLWriteSecStore(
uint32_t storeSize,
const char *rawKey,
const char *storeName,
const char *storeValue ,
void *libHandle
)
{
	int				rv;
	NSLWriteSecStore_t	NSLWriteSecStore_ptr = NULL;


	if (!(NSLWriteSecStore_ptr = (NSLWriteSecStore_t)GetSymbol(libHandle, "NSLWriteSecStore")))
	{
		return -1;
	}

	rv = NSLWriteSecStore_ptr(storeSize, rawKey, storeName, storeValue);

	return rv;
}


int
dsoNSLRemoteCallV(
const char *rpcName,
char **rpcReturn,
char **rpcVarNames,
char **rpcVarVals ,
void *libHandle
)
{
	int				rv;
	NSLRemoteCallV_t	NSLRemoteCallV_ptr = NULL;


	if (!(NSLRemoteCallV_ptr = (NSLRemoteCallV_t)GetSymbol(libHandle, "NSLRemoteCallV")))
	{
		return -1;
	}

	rv = NSLRemoteCallV_ptr(rpcName, rpcReturn, rpcVarNames, rpcVarVals);

	return rv;
}


int
dsoNSLFree(
void *memptr ,
void *libHandle
)
{
	int				rv;
	NSLFree_t	NSLFree_ptr = NULL;


	if (!(NSLFree_ptr = (NSLFree_t)GetSymbol(libHandle, "NSLFree")))
	{
		return -1;
	}

	rv = NSLFree_ptr(memptr);

	return rv;
}


