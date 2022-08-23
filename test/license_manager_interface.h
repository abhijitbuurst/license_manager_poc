#ifndef __LM_MANAGER_INTERFACE_H__
#define __LM_MANAGER_INTERFACE_H__

// LM Wrapper


int 
setup_licensing_server(
char            *libPath,
void            **libHandle,
int				custID,
int				prodID,
unsigned int	xauth,
unsigned int	yauth,
unsigned int	zauth
);

int
SetLicenseStatus(
void		    **libHandle,
char			*licenseCode,
int32_t			*licenseStatus,
uint32_t		*licenseType,
uint32_t		*actType,
char			*xmlRegInfo
);

int
GetLicenseStatus(
void		    **libHandle
);
#endif //__LM_MANAGER_INTERFACE_H__
