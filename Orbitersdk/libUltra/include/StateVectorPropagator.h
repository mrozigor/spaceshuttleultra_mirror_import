#ifndef STATEVECTORPROPAGATOR_128F62F4_83F4_4BDD_967B_7E2CD4016D3F
#define STATEVECTORPROPAGATOR_128F62F4_83F4_4BDD_967B_7E2CD4016D3F
#pragma once

#include "orbiterapi.h"
#include "..\KOST\kost.h"
#include <map>

class StateVectorPropagator
{
	typedef std::map<double, kostStateVector> StateVectorMap;
	typedef StateVectorMap::iterator MapIterator;
	typedef StateVectorMap::const_iterator MapConstIterator;

	//std::map<double, kostStateVector> stateVectors;
	StateVectorMap stateVectors;

	double stepLength;
	int iterationsPerStep;
	double maxPropagationTime; // total time since last state vector update we will propagate vector
	//double currentTimestamp;
	double lastSaveMET;
	double lastStateVectorUpdateMET;
	//double maxTimestamp;
	double maxPropMET;

	VECTOR3 propPos, propVel;
	double propMET;

	double mu, GM, radius, J2;
	//double vesselMass;
public:
	//StateVectorPropagator(const VECTOR3& pos, const VECTOR3& vel, OBJHANDLE hPlanet, double _vesselMass, double currentMET);
	StateVectorPropagator(double propagationStepLength, int _iterationsPerStep, double _maxPropagationTime);
	~StateVectorPropagator();

	void SetParameters(double vesselMass, double planetMass, double planetRadius, double J2Coeff);
	void UpdateStateVector(const VECTOR3& equPos, const VECTOR3& equVel, double met);

	//void SetVesselMass(double newMass);

	//void GetState(VECTOR3& pos, VECTOR3& vel) const;
	void Step(double currentMET, double DeltaT);

	double GetElements(double MET, ELEMENTS& el, kostOrbitParam* oparam=NULL) const;
	void GetStateVectors(double MET, VECTOR3& pos, VECTOR3& vel) const;

	//double GetLastUpdateTime() const;

	//void GetApogeeData(double& ApD, double& METAtApogee) const;
	//void GetPerigeeData(double& PeD, double& METAtPerigee) const;
	void GetApogeeData(double currentMET, double& ApD, double& METAtApogee) const;
	void GetPerigeeData(double currentMET, double& PeD, double& METAtPerigee) const;
private:
	void Propagate(unsigned int iterationCount, double DeltaT);
	void CalculateAccelerationVector(VECTOR3& grav) const;
};

#endif //STATEVECTORPROPAGATOR_128F62F4_83F4_4BDD_967B_7E2CD4016D3F