#include "Atlantis.h"
#include "MLP.h"

DLLCLBK void InitModule(HINSTANCE hDLL)
{
}

DLLCLBK void ExitModule(HINSTANCE hDLL)
{
}

DLLCLBK VESSEL* ovcInit(OBJHANDLE hVessel, int iFlightModel)
{
	return new MLP(hVessel, iFlightModel);
}

DLLCLBK void ovcExit(VESSEL* pVessel)
{
	delete static_cast<MLP*>(pVessel);
}