#ifndef __MPMSUBSYSTEMS_H
#define __MPMSUBSYSTEMS_H
#pragma once

#include "AtlantisSubsystem.h"
#include "discsignals/discsignals.h"
#include "Atlantis.h"

const double MPM_DEPLOY_SPEED = 0.0294117647;
const double MRL_LATCH_SPEED = 0.11765;
// speed of MPM latches (8.5 seconds)
const double MRL_MAX_ANGLE_ERROR = 0.1;
// max angular misalignment (in degrees) between MPM and OBSS/RMS to allow latching

using namespace discsignals;

class MPMSystem : public AtlantisSubsystem
{
public:
	MPMSystem(SubsystemDirector* _director, const string& _ident, const char* _meshname);
	virtual ~MPMSystem();

	virtual void Realize();
	virtual void OnPreStep(double SimT, double DeltaT, double MJD);
	//virtual void OnPostStep(double SimT, double DeltaT, double MJD);
	virtual bool OnParseLine(const char* line);
	virtual void OnSaveState(FILEHANDLE scn) const;

	bool Deployed() const {return MPMRollout.Open();};
	bool Stowed() const {return MPMRollout.Closed();};
	bool Released() const {return MRLLatches.Open();};
	bool Latched() const {return MRLLatches.Closed();};
protected:
	virtual void OnMRLLatched();
	virtual void OnMRLReleased();

	UINT mesh_index;
	// all animations should be added by derived classes
	UINT anim_mpm;

	AnimState MPMRollout, MRLLatches;
	DiscOutPort MRL_Rel_Microswitches[3], MRL_Lat_Microswitches[3], MRL_RTL_Microswitches[3];
	DiscOutPort MRL_Latched, MRL_Released;

	DiscOutPort MPM_Stowed, MPM_Deployed;
private:
	void AddMesh();
	
	MESHHANDLE hMesh;

	DiscInPort Release, Latch;
	DiscInPort Deploy, Stow;
};

#endif //__MPMSUBSYSTEMS_H