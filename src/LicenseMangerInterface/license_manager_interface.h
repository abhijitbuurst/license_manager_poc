#ifndef __LM_MANAGER_INTERFACE_H__
#define __LM_MANAGER_INTERFACE_H__

// LM Wrapper


int 
setup_licensing_server(
char            *libPath,
int				custID,
int				prodID,
unsigned int	xauth,
unsigned int	yauth,
unsigned int	zauth
);

int
SetLicenseStatus(
char			*licenseCode
);

int
GetLicenseStatus(
);

int
GetFeature(
);

int
CloseApplication(
);
#endif //__LM_MANAGER_INTERFACE_H__
