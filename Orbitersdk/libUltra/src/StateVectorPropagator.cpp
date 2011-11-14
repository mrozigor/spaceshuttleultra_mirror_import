#include "..\include\StateVectorPropagator.h"
#include "..\include\UltraMath.h"

//StateVectorPropagator::StateVectorPropagator(const VECTOR3& pos, const VECTOR3& vel, OBJHANDLE hPlanet, double _vesselMass, double currentMET)
	//: curPos(pos), curVel(vel), curMET(currentMET), vesselMass(_vesselMass)
StateVectorPropagator::StateVectorPropagator(double propagationStepLength, int _iterationsPerStep, double _maxPropagationTime)
	: stepLength(propagationStepLength), iterationsPerStep(_iterationsPerStep), maxPropagationTime(_maxPropagationTime),
	lastSaveMET(-1000.0), maxPropMET(-1000.0), propMET(0)
{
}

StateVectorPropagator::~StateVectorPropagator()
{
}

void StateVectorPropagator::SetParameters(double vesselMass, double planetMass, double planetRadius, double J2Coeff)
{
	GM = planetMass*GGRAV;
	mu = GGRAV*(planetMass+vesselMass);
	radius = planetRadius;
	J2 = J2Coeff;
}

void StateVectorPropagator::UpdateStateVector(const VECTOR3& equPos, const VECTOR3& equVel, double met)
{
	// only update vectors if last set of state vectors has been propagated to limit
	if(propMET >= maxPropMET) {
		propPos = equPos;
		propVel = equVel;
		propMET = met;
		maxPropMET = met+maxPropagationTime;
		lastSaveMET = propMET-1000.0; // force data to be saved
		lastStateVectorUpdateMET = met;

		if(stateVectors.size()==0) {
			kostStateVector state;
			state.pos = ConvertBetweenLHAndRHFrames(equPos);
			state.vel = ConvertBetweenLHAndRHFrames(equVel);
			stateVectors.insert(std::make_pair(met, state));
		}
	}
	/*if(stateVectors.size() > 0) {
		VECTOR3 pos, vel;
		GetStateVectors(met, pos, vel);
		//sprintf_s(oapiDebugString(), 255, "Pos error: %f %f %f Vel error: %f %f %f total pos error: %f total vel error: %f",
			//propPos.x-pos.x, propPos.y-pos.y, propPos.z-pos.z, propVel.x-vel.x, propVel.y-vel.y, propVel.z-vel.z, length(propPos-pos), length(propVel-vel));
	}*/
}

/*void StateVectorPropagator::GetState(VECTOR3& pos, VECTOR3& vel) const
{
	pos = curPos;
	vel = curVel;
}*/
void StateVectorPropagator::Step(double currentMET, double DeltaT)
{
	// delete any elements older than current time
	while(stateVectors.size()>1 && stateVectors.begin()->first < currentMET) stateVectors.erase(stateVectors.begin());

	if(propMET<maxPropMET && DeltaT<(stepLength*iterationsPerStep)) { // don't bother propagating if time acceleration is too fast
		Propagate(iterationsPerStep, stepLength);

		if((propMET-lastSaveMET) > 60.0) {
			// clean out values from old state vectors
			MapConstIterator start = stateVectors.upper_bound(lastSaveMET);
			if(start != stateVectors.end()) {
				MapConstIterator end = stateVectors.lower_bound(propMET);
				stateVectors.erase(start, end);
				//if(start != end) stateVectors.erase(start, end);
				//else stateVectors.erase(start);
			}
			// save state vectors
			kostStateVector state;
			state.pos=ConvertBetweenLHAndRHFrames(propPos);
			state.vel=ConvertBetweenLHAndRHFrames(propVel);
			stateVectors.insert(std::make_pair(propMET, state));
			lastSaveMET = propMET;
		}
	}
	//sprintf_s(oapiDebugString(), 255, "SVP propMET: %f maxMET: %f", propMET, maxPropMET);
}

double StateVectorPropagator::GetElements(double MET, ELEMENTS& el, kostOrbitParam* oparam) const
{
	//std::deque<double>::iterator it = std::lower_bound(stateVectorTimestamps.
	//std::map<double, kostStateVector>::const_iterator it = stateVectors.lower_bound(MET);
	MapConstIterator it = stateVectors.lower_bound(MET);
	if(it == stateVectors.end()) --it; // set iterator to last element in map
	kostStateVector2Elements(mu, &(it->second), &el, oparam);
	return it->first;
}

void StateVectorPropagator::GetStateVectors(double MET, VECTOR3& pos, VECTOR3& vel) const
{
	ELEMENTS el;
	double epoch = GetElements(MET, el);
	kostStateVector state;
	kostElements2StateVectorAtTime(mu, &el, &state, MET-epoch, 1e-5, 50, radius, 0.0);
	pos = ConvertBetweenLHAndRHFrames(state.pos);
	vel = ConvertBetweenLHAndRHFrames(state.vel);
}

void StateVectorPropagator::GetApogeeData(double currentMET, double& ApD, double& METAtApogee) const
{
	if(stateVectors.size() == 0) return; // no data

	bool bIncreasingAlt = false;
	MapConstIterator it=stateVectors.begin();
	double previousAlt = length(it->second.pos);
	++it;
	while(it!=stateVectors.end()) {
		double alt = length(it->second.pos);
		// state vectors closest to apogee are when altitude stops increasing and starts decreasing
		if(alt > previousAlt) {
			bIncreasingAlt = true;
		}
		else if(alt < previousAlt && bIncreasingAlt) {
			--it; // go back to previous set of state vectors
			ELEMENTS el;
			kostOrbitParam oparam;
			kostStateVector2Elements(mu, &(it->second), &el, &oparam);
			ApD = oparam.ApD;
			METAtApogee = oparam.ApT + it->first;
			if(METAtApogee < currentMET) METAtApogee +=oparam.T;
			if((METAtApogee-currentMET) > oparam.T) METAtApogee-= oparam.T;
			return;
		}
		previousAlt = alt;
		++it;
	}
	//ApD = 0.0;
	//METAtApogee = 0.0;
	//return;
	// could not find apogee; use earliest set of data to calculate elements
	it = stateVectors.begin();
	//MapConstIterator it = stateVectors.begin();
	ELEMENTS el;
	kostOrbitParam oparam;
	kostStateVector2Elements(mu, &(it->second), &el, &oparam);
	ApD = oparam.ApD;
	METAtApogee = oparam.ApT + it->first;
	if(METAtApogee < currentMET) METAtApogee +=oparam.T;
	if((METAtApogee-currentMET) > oparam.T) METAtApogee-= oparam.T;
}

void StateVectorPropagator::GetPerigeeData(double currentMET, double& PeD, double& METAtPerigee) const
{
	if(stateVectors.size() == 0) return; // no data

	bool bDecreasingAlt = false;
	MapConstIterator it=stateVectors.begin();
	double previousAlt = length(it->second.pos);
	++it;
	while(it!=stateVectors.end()) {
		double alt = length(it->second.pos);
		// state vectors closest to perigee are when altitude stops decreasing and starts increasing
		if(alt < previousAlt) {
			bDecreasingAlt = true;
		}
		else if(alt > previousAlt && bDecreasingAlt) {
			--it; // go back to previous set of state vectors
			ELEMENTS el;
			kostOrbitParam oparam;
			kostStateVector2Elements(mu, &(it->second), &el, &oparam);
			PeD = oparam.PeD;
			METAtPerigee = oparam.PeT + it->first;
			if(METAtPerigee < currentMET) METAtPerigee +=oparam.T;
			if((METAtPerigee-currentMET) > oparam.T) METAtPerigee-= oparam.T;
			return;
		}
		previousAlt = alt;
		++it;
	}
	//PeD = 0.0;
	//METAtPerigee = 0.0;
	//return;
	// could not find apogee; use earliest set of data to calculate elements
	it = stateVectors.begin();
	//MapConstIterator it = stateVectors.begin();
	ELEMENTS el;
	kostOrbitParam oparam;
	kostStateVector2Elements(mu, &(it->second), &el, &oparam);
	PeD = oparam.PeD;
	METAtPerigee = oparam.PeT + it->first;
	if(METAtPerigee < currentMET) METAtPerigee +=oparam.T;
	if((METAtPerigee-currentMET) > oparam.T) METAtPerigee-= oparam.T;
}

void StateVectorPropagator::Propagate(unsigned int iterationCount, double DeltaT)
{
	for(unsigned int i=0;i<iterationCount;i++) {
		VECTOR3 initialGrav;
		CalculateAccelerationVector(initialGrav);
		propPos += (propVel + initialGrav*(0.5*DeltaT))*DeltaT;
		VECTOR3 finalGrav;
		CalculateAccelerationVector(finalGrav);
		propVel += (initialGrav+finalGrav)*0.5*DeltaT;
	}
	propMET += iterationCount*DeltaT;
}

void StateVectorPropagator::CalculateAccelerationVector(VECTOR3& grav) const
{
	double lat = asin(propPos.y/length(propPos));
	double r = length(propPos);
	// acceleration magnitudes in r and theta directions
	double a_r = -GM/(r*r) + 1.5*(GM*radius*radius*J2/pow(r, 4))*(3*pow(sin(lat), 2) - 1);
	double a_theta = -3*(GM*radius*radius*J2/pow(r, 4))*sin(lat)*cos(lat);
	// unit vectors
	VECTOR3 r_hat = propPos/r;
	VECTOR3 phi = crossp(_V(0, 1, 0), r_hat);
	VECTOR3 theta_hat = crossp(r_hat, phi);
	theta_hat/=length(theta_hat);

	grav = r_hat*a_r + theta_hat*a_theta;
}
