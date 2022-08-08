#ifndef _NALPEIRONNSA_H_
#define _NALPEIRONNSA_H_

#include <inttypes.h>

#if defined(__APPLE__)
// Mac OS X Specific header stuff here
    #include <TargetConditionals.h>
    #define WINAPI
#endif

typedef int (*NSAValidateLibrary_t)( uint32_t custID, uint32_t prodID );
typedef int (*NSAGetVersion_t)( char **version );
typedef int (*NSAGetComputerID_t)( char **computerID );
typedef int (*NSAGetHostName_t)( char **hostName );
typedef int (*NSATestConnection2_t)( const char *nsaClientData, uint32_t *transID );
typedef int (*NSATestConnection_t)( const char *nsaClientData, uint32_t *transID );
typedef int (*NSAAppStart_t)( const char *username, const char *nsaClientData, uint32_t *transID );
typedef int (*NSAAppStop_t)( const char *username, const char *nsaClientData, uint32_t *transID );
typedef int (*NSALogin_t)( const char *username, const char *nsaClientData, uint32_t *transID );
typedef int (*NSALogout_t)( const char *username, const char *nsaClientData, uint32_t *transID );
typedef int (*NSAFeatureStart_t)( const char *username, const char *featureCode, const char *nsaClientData, uint32_t *transID );
typedef int (*NSAFeatureStop_t)( const char *username, const char *featureCode, const char *nsaClientData, uint32_t *transID );
typedef int (*NSAException_t)( const char *username, const char *exceptionCode, const char *description, const char *nsaClientData, uint32_t *transID );
typedef int (*NSASysInfo_t)( const char *username, const char *applang, const char *version, const char *edition, const char *build, const char *licenseStat, const char *nsaClientData, uint32_t *transID );
typedef int (*NSASendCache_t)( const char *username, uint32_t *transID );
typedef int (*NSAGetPrivacy_t)( );
typedef int (*NSASetPrivacy_t)( uint16_t nsaPriv );
typedef int (*NSAGetStats_t)( char **nsaStats );
typedef int (*NSAFree_t)( void *memptr );


#endif // _NALPEIRONNSA_H_
