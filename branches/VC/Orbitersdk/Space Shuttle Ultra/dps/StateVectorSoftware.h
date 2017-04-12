#ifndef STATEVECTORSOFTWARE_H_13BE0480_DDF8_40EF_87D9_3B260431940F
#define STATEVECTORSOFTWARE_H_13BE0480_DDF8_40EF_87D9_3B260431940F

#include "SimpleGPCSoftware.h"
#include "StateVectorPropagator.h"

namespace dps
{

/**
 * GPC code for propagating state vectors.
 * Will eventually implemented SPEC 33 REL NAV display.
 */
class StateVectorSoftware : public SimpleGPCSoftware
{
	OBJHANDLE hEarth;
	double lastUpdateSimTime;
	StateVectorPropagator propagator, targetPropagator;
	VECTOR3 lastVelocity;

	VECTOR3 t0Pos;

	std::string targetVesselName;
	VESSEL* pTargetVessel;
public:
	StateVectorSoftware(SimpleGPCSystem* _gpc);
	~StateVectorSoftware();

	virtual void Realize();

	virtual void OnPreStep(double SimT, double DeltaT, double MJD);

	virtual bool OnMajorModeChange(unsigned int newMajorMode);
	virtual bool ItemInput(int spec, int item, const char* Data, bool &IllegalEntry );
	virtual bool OnPaint(int spec, vc::MDU* pMDU) const;

	virtual bool OnParseLine(const char* keyword, const char* value);
	virtual void OnSaveState(FILEHANDLE scn) const;

	/**
	 * Sets target vessel for tracking. This is the vessel that will be used for rendezvous burn targeting, etc.
	 * In real life, MCC will just upload state vectors for target. Ku/star tracker data will be used (REL NAV display) to refine shuttle's orbit data.
	 * In Orbiter, vessel name will be specified and we will track vessel.
	 * NOTE: code assumes target vessel is not deleted at any point
	 */
	void SetTargetVessel(char* vesselName);

	double GetMETAtAltitude(double altitude) const;
	void GetPropagatedStateVectors(double met, VECTOR3& pos, VECTOR3& vel) const;
	void GetCurrentStateVectorsM50(VECTOR3& pos, VECTOR3& vel) const;
	void GetApogeeData(double& ApD, double& ApT) const;
	void GetPerigeeData(double& PeD, double& PeT) const;
	VECTOR3 GetPositionAtT0() const;

	void GetTargetStateVectors(double met, VECTOR3& pos, VECTOR3& vel) const;
private:
	bool UpdatePropagatorStateVectors();
	void UpdateTargetStateVectors();
};

};

#endif // STATEVECTORSOFTWARE_H_13BE0480_DDF8_40EF_87D9_3B260431940F
