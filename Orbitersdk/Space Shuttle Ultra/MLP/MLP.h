#ifndef ___ATLANTIS_MLP_H___
#define ___ATLANTIS_MLP_H___

#include "OrbiterAPI.h"
#include "VesselAPI.h"

class MLP: public VESSEL2
{
	MESHHANDLE mshMLP;
public:
	MLP(OBJHANDLE hVessel, int iFlightModel);
	virtual ~MLP();

	virtual void clbkSetClassCaps(FILEHANDLE cfg);
	virtual void clbkLoadStateEx(FILEHANDLE scn, void* vs);
	virtual void clbkSaveState(FILEHANDLE scn);
};

#endif