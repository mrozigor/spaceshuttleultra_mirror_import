#ifndef __RMSSUBSYSTEM_H
#define __RMSSUBSYSTEM_H
#pragma once

#include "AtlantisSubsystem.h"
#include "Atlantis.h"

//for the moment, this class handles grappling 
class RMSSystem : public AtlantisSubsystem
{
public:
	RMSSystem(SubsystemDirector* _director);
	virtual ~RMSSystem();

	virtual void Realize();
	virtual void OnPostStep(double fSimT, double fDeltaT, double fMJD);
	virtual bool OnParseLine(const char* line);
	virtual void OnSaveState(FILEHANDLE scn) const;

	virtual double GetSubsystemEmptyMass() const {return 0.0;};

	//temporary function: used only until RMSSystem class is responsible for EE attachment creation
	void DefineEndEffector(ATTACHMENTHANDLE attachment) {end_effector=attachment;};

	void Grapple(VESSEL* vessel, ATTACHMENTHANDLE attachment);
	void Ungrapple();
	void Detach(VESSEL* target=NULL);

	bool Grappled() const {return (grapple!=NULL);};
	/**
	 * Returns true if arm is free to move.
	 * Returns false if arm is grappled to payload which is attached to something else.
	 */
	bool Movable() const {return (grapple==NULL || PayloadIsFree());};
private:
	bool PayloadIsFree() const;

	VESSEL* payload;
	ATTACHMENTHANDLE grapple, end_effector;
	//true if RMS is still grappled, but Orbiter should not fromally attach the RMS to the payload.
	bool detached;
};

#endif //__RMSSUBSYSTEM_H