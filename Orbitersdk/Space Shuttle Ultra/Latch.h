#ifndef __LATCH_H
#define __LATCH_H
#pragma once

#include "AtlantisSubsystem.h"
//#include "discsignals/discsignals.h"
#include "Atlantis.h"

/**
 * This class essentially acts as a wrapper for a single ATTACHMENTHANDLE.
 */
class LatchSystem : public AtlantisSubsystem
{
public:
	LatchSystem(SubsystemDirector* _director, const string& _ident, const string& _attachID);
	virtual ~LatchSystem();

	virtual void OnPreStep(double SimT, double DeltaT, double MJD);

	/**
	 * Creates ATTACHMENTHANDLE associated with latch(es)
	 * Should be called from Atlantis class at appropriate time.
	 */
	virtual void CreateAttachment() = 0;

	/**
	 * If vessel is NULL or same as attached payload,
	 * attachment between hAttach and hPayloadAttachment is destroyed.
	 * Remains logically 'attached' to payload
	 */
	//void Detach(VESSEL* vessel);
protected:
	void AttachPayload(VESSEL* vessel, ATTACHMENTHANDLE attachment);
	void DetachPayload();

	ATTACHMENTHANDLE FindPayload(VESSEL** pVessel=NULL) const;
	bool PayloadIsFree() const;

	VESSEL* attachedPayload;
	ATTACHMENTHANDLE hPayloadAttachment;
	ATTACHMENTHANDLE hAttach;

	string AttachID;
private:
	void CheckForAttachedObjects();

	bool firstStep;
	//bool detached;
};

#endif //__LATCH_H