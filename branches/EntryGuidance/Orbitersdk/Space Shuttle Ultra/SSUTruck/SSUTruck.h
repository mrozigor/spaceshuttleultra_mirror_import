#pragma once
#include <OrbiterAPI.h>
#include <VesselAPI.h>

/**
 * Generic vessel class for the various kinds of transporters used in KSC.
 * Can also be used for general cars.
 */
class SSUTruck: public VESSEL3
{
public:
	SSUTruck(OBJHANDLE hVessel, int iFlightModel = 1);
	virtual ~SSUTruck();
	/**
	 * /see VESSEL2::clbkPreStep
	 */
	virtual void clbkPreStep(double dfSimT, double dfDeltaT, double dfMJD);
	/**
	 * /see VESSEL2::clbkSaveState
	 */
	virtual void clbkSaveState(FILEHANDLE scn);
	/**
	 * /see VESSEL2::clbkSetClassCaps
	 */
	virtual void clbkSetClassCaps(FILEHANDLE cfg);
	
	
};