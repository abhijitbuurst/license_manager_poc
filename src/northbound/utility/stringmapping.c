
#include "stringmapping.h"


void
licStat2Str(
int			licStat,
char		**licenseStatus
)
{
	if (*licenseStatus != NULL)
		free(*licenseStatus);

	switch (licStat)
	{
	case -201:
		*licenseStatus = strdup("(-201) License revoked by daemon\n");
		break;

	case -200:
		*licenseStatus = strdup("(-200) Client missed too many heartbeats\n");
		break;

	case -117:
		*licenseStatus = strdup("(-117) Duplicate device id\n");
		break;

	case -116:
		*licenseStatus = strdup("(-116) Subscription expired\n");
		break;

	case -115:
		*licenseStatus = strdup("(-115) Number of allowed activations exceeded\n");
		break;

	case -114:
		*licenseStatus = strdup("(-114) License code is inactive\n");
		break;

	case -113:
		*licenseStatus = strdup("(-113) Trial has expired\n");
		break;

	case -112:
		*licenseStatus = strdup("(-112) Computer ID already activated\n");
		break;

	case -111:
		*licenseStatus = strdup("(-111) Invalid trial period\n");
		break;

	case -110:
		*licenseStatus = strdup("(-110) Product is inactive\n");
		break;

	case -70:
		*licenseStatus = strdup("(-70) Cached ABL awaiting authentication\n");
		break;

	case -60:
		*licenseStatus = strdup("(-60) Disabled passive license\n");
		break;

	case -52:
		*licenseStatus = strdup("(-52) Daemon ID failure\n");
		break;

	case -51:
		*licenseStatus = strdup("(-51) Daemon failed verify\n");
		break;

	case -50:
		*licenseStatus = strdup("(-50) No available license\n");
		break;

	case -9:
		*licenseStatus = strdup("(-9) License is activation receipt\n");
		break;

	case -8:
		*licenseStatus = strdup("(-8) Product has invalid date\n");
		break;

	case -7:
		*licenseStatus = strdup("(-7) Incorrect system date\n");
		break;

	case -6:
		*licenseStatus = strdup("(-6) License returned to server\n");
		break;

	case -5:
		*licenseStatus = strdup("(-5) Invalid license\n");
		break;

	case -4:
		*licenseStatus = strdup("(-4) License missing\n");
		break;

	case -3:
		*licenseStatus = strdup("(-3) Not authorized\n");
		break;

	case -2:
		*licenseStatus = strdup("(-2) Computer has been backdated\n");
		break;

	case -1:
		*licenseStatus = strdup("(-1) License expired\n");
		break;

	case 1:
		*licenseStatus = strdup("(1) Valid license\n");
		break;

	case 2:
		*licenseStatus = strdup("(2) Trial license\n");
		break;

	case 3:
		*licenseStatus = strdup("(3) Online concurrent mode\n");
		break;

	case 4:
		*licenseStatus = strdup("(4) Account based licensing\n");
		break;

	case 5:
		*licenseStatus = strdup("(5) Concurrent account based licensing\n");
		break;

	case 14:
		*licenseStatus = strdup("(14) Daemon network license\n");
		break;

	case 15:
		*licenseStatus = strdup("(15) Daemon LTCO\n");
		break;

	case 16:
		*licenseStatus = strdup("(16) Daemon OEM\n");
		break;

	case 51:
		*licenseStatus = strdup("(51) LTCO\n");
		break;

	case 53:
		*licenseStatus = strdup("(53) Concurrent LTCO\n");
		break;

	case 54:
		*licenseStatus = strdup("(54) ABL LTCO\n");
		break;

	case 55:
		*licenseStatus = strdup("(55) ABL Concurrent LTCO\n");
		break;

	default:
		*licenseStatus = strdup("Unknown license status\n");
		break;
	}

	return;
}


void
licType2Str(
uint32_t	licType,
char		**licenseType,
char		**expDate,
void		*libHandle,
int			offset
)
{
	char		*tempVal;
	int			retVal;


	if (*expDate != NULL)
		free(*expDate);

	if (*licenseType != NULL)
		free(*licenseType);

	switch (licType)
	{
	case 2:
		*licenseType = strdup("Trial");

		retVal = dsoNSLGetTrialExpDate(&tempVal, libHandle);
		retVal = retVal - offset;

		if (retVal != 0)
		{
			*expDate = NULL;
		}
		else
		{
			*expDate = strdup(tempVal);
			dsoNSLFree(tempVal, libHandle);
		}

		break;

	case 3:
		*licenseType = strdup("Permanent");
		*expDate = strdup("NA");
		break;

	case 4:
		*licenseType = strdup("Permanent concurrent");
		*expDate = strdup("NA");
		break;

	case 5:
		*licenseType = strdup("Subscription");

		retVal = dsoNSLGetSubExpDate(&tempVal, libHandle);
		retVal = retVal - offset;

		if (retVal != 0)
		{
			*expDate = NULL;
		}
		else
		{
			*expDate = strdup(tempVal);
			dsoNSLFree(tempVal, libHandle);
		}

		break;

	case 6:
		*licenseType = strdup("Subscription concurrent");

		retVal = dsoNSLGetSubExpDate(&tempVal, libHandle);
		retVal = retVal - offset;

		if (retVal != 0)
		{
			*expDate = NULL;
		}
		else
		{
			*expDate = strdup(tempVal);
			dsoNSLFree(tempVal, libHandle);
		}

		break;

	case 7:
		*licenseType = strdup("Network");
		*expDate = strdup("NA");
		break;

	case 8:
		*licenseType = strdup("Subscription network");

		retVal = dsoNSLGetSubExpDate(&tempVal, libHandle);
		retVal = retVal - offset;

		if (retVal != 0)
		{
			*expDate = NULL;
		}
		else
		{
			*expDate = strdup(tempVal);
			dsoNSLFree(tempVal, libHandle);
		}

		break;

	case 9:
		*licenseType = strdup("Long term checkout (ltco) from daemon");
		*expDate = strdup("NA");
		break;

	case 10:
		*licenseType = strdup("OEM");
		*expDate = strdup("NA");
		break;

	case 11:
		*licenseType = strdup("Account based");
		*expDate = strdup("NA");
		break;

	case 12:
		*licenseType = strdup("Subscription Account based");

		retVal = dsoNSLGetSubExpDate(&tempVal, libHandle);
		retVal = retVal - offset;

		if (retVal != 0)
		{
			*expDate = NULL;
		}
		else
		{
			*expDate = strdup(tempVal);
			dsoNSLFree(tempVal, libHandle);
		}

		break;

	case 13:
		*licenseType = strdup("Concurrent account based");
		*expDate = strdup("NA");
		break;

	case 14:
		*licenseType = strdup("Subscription concurrent account based");

		retVal = dsoNSLGetSubExpDate(&tempVal, libHandle);
		retVal = retVal - offset;

		if (retVal != 0)
		{
			*expDate = NULL;
		}
		else
		{
			*expDate = strdup(tempVal);
			dsoNSLFree(tempVal, libHandle);
		}

		break;

	case 15:
		*licenseType = strdup("Long term checkout (ltco)");
		*expDate = strdup("NA");
		break;

	case 16:
		*licenseType = strdup("Concurrent Long term checkout (ltco)");
		*expDate = strdup("NA");
		break;

	case 17:
		*licenseType = strdup("Account based LTCO");
		*expDate = strdup("NA");
		break;

	case 18:
		*licenseType = strdup("Concurrent account based LTCO");
		*expDate = strdup("NA");
		break;

	default:
		*licenseType = strdup("Unknown");
		*expDate = strdup("Unknown");
		break;
	}

	return;
}


void
licAct2Str(
uint32_t	actType,
char		**activationType
)
{
	if (*activationType != NULL)
		free(*activationType);

	switch (actType)
	{
	case 1:
		*activationType = strdup(" activated online\n");
		break;

	case 2:
		*activationType = strdup(" activated with certificate\n");
		break;

	case 3:
		*activationType = strdup(" activated via daemon\n");
		break;

	default:
		*activationType = strdup(" with unknown activation method\n");
		break;
	}

	return;
}

void
feat2Str(
uint32_t featureStatusCode,
char **featureStatus 
)
{
	if (*featureStatus != NULL)
		free(*featureStatus);

	switch (featureStatusCode)
	{
	case -6:// 	FEATSTATUS_EMPTY 	-6 Feature pool empty

		*featureStatus = strdup("Feature pool empty\n");
		break;

	case -5:// FEATSTATUS_EXPIRED 	-5 Feature requested but license expired

		*featureStatus = strdup("Feature requested but license expired\n");
		break;

	case -4:// FEATSTATUS_UNAUTH    -4 Feature not authorized for use

		*featureStatus = strdup("Feature not authorized for use\n");
		break;

	case -3:// FEATSTATUS_DENIED 	-3 Feature request denied

		*featureStatus = strdup("Feature request denied\n");
		break;

	case -2:// FEATSTATUS_UNKNOWN 	-2 Unknown Feature requested

		*featureStatus = strdup("Unknown Feature requested\n");
		break;

	case -1:// FEATSTATUS_ERROR 	-1 Error check function return

		*featureStatus = strdup("Error check function return\n");
		break;

	case 0:// FEATSTATUS_UNSET 	0 Status Undefined

		*featureStatus = strdup(" activated via daemon\n");
		break;

	case 1:// FEATSTATUS_AUTH 	    1 Feature authorized for use
		*featureStatus = strdup("Feature authorized for use\n");
		break;


	default:
		*featureStatus = strdup(" With Unknown feature status\n");
		break;
	}

	return;
}