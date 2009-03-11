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

	virtual void OnAttach() = 0;
	virtual void OnDetach() = 0;

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

/**
 * Class for PLBD latches that can be operated on-orbit
 */
class ActiveLatch : public LatchSystem
{
public:
	ActiveLatch(SubsystemDirector* _director, const string& _ident, const VECTOR3& _pos, const VECTOR3& _dir, const VECTOR3& _rot);
	virtual ~ActiveLatch();

	virtual void CreateAttachment();

	void Latch();
	void Release();
protected:
	virtual void OnAttach();
	virtual void OnDetach();
private:
	VECTOR3 pos, dir, rot;
};

#endif //__LATCH_H