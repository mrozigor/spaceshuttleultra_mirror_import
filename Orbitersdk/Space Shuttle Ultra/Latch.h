/****************************************************************************
  This file is part of Space Shuttle Ultra

  Latch definition



  Space Shuttle Ultra is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Space Shuttle Ultra is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Space Shuttle Ultra; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See http://spaceshuttleultra.sourceforge.net/license/ for more details.

  **************************************************************************/
#ifndef __LATCH_H
#define __LATCH_H
#pragma once

#include <vector>
#include "AtlantisSubsystem.h"
#include "vesselapi.h"
#include "discsignals.h"

const double MAX_GRAPPLING_DIST = 0.5;
// max distance between RMS tip and grappling point for successful grappling
const double MAX_GRAPPLING_ANGLE = 0.087266;
// max angle between EE and grapple for successful grappling (radians)


const double LATCH_CLOSE_TIME = 30.0;
// time(sec) for latch to open/close

/**
 * This class essentially acts as a wrapper for a single ATTACHMENTHANDLE.
 */
class LatchSystem : public AtlantisSubsystem
{
public:
	LatchSystem(AtlantisSubsystemDirector* _director, const string& _ident, const string& _attachID);
	virtual ~LatchSystem();

	virtual void OnPreStep(double SimT, double DeltaT, double MJD);
	virtual bool OnParseLine(const char* line);
	virtual void OnSaveState(FILEHANDLE scn) const;
	virtual bool SingleParamParseLine() const {return true;};

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

	/**
	 * Finds vessel that can be attached to this latch
	 * @param pVessel Optional pointer to vessel pointer which will point to payload vessel returned
	 * @returns ATTACHMENTHANDLE to attachment point on payload which can be attached to latch
	 */
	virtual ATTACHMENTHANDLE FindPayload(VESSEL** pVessel=NULL) const;
	/**
	 * @param hV Handle to vessel
	 * @param glatchpos Position of latch in global coordindates
	 * @param glatchdir Direction of latch attachment point in global coordinates
	 * @returns Handle to attachment point which can be used by latch
	 * @returns NULL if vessel cannot be attached
	 */
	ATTACHMENTHANDLE CanAttach(OBJHANDLE hV, const VECTOR3& glatchpos, const VECTOR3& glatchdir) const;
	bool PayloadIsFree() const;

	inline bool IsLatched() { return attachedPayload != NULL; };

	inline bool IsFirstStep() { return firstStep; };

	VESSEL* attachedPayload;
	ATTACHMENTHANDLE hPayloadAttachment;
	ATTACHMENTHANDLE hAttach;

	string AttachID;
private:
	/**
	 * Called during first timestep to handle any objects attached when scn starts
	 */
	void CheckForAttachedObjects();

	bool firstStep;

	// used only for loading attachments from scenario
	std::string payloadName;
	int attachmentIndex;
};

/**
 * Class for PLBD latches that can be operated on-orbit
 */
class ActiveLatchGroup : public LatchSystem
{
public:
	ActiveLatchGroup(AtlantisSubsystemDirector* _director, const string& _ident, const VECTOR3& _pos, const VECTOR3& _dir, const VECTOR3& _rot);
	virtual ~ActiveLatchGroup();

	virtual void OnPreStep(double SimT, double DeltaT, double MJD);
	virtual void Realize();

	virtual bool OnParseLine(const char* line);
	virtual void OnSaveState(FILEHANDLE scn) const;

	virtual void CreateAttachment();

	void SetAttachmentParams(const VECTOR3 &_pos, const VECTOR3 &_dir, const VECTOR3 &_rot);

	void Latch();
	void Release();
protected:
	virtual void OnAttach();
	virtual void OnDetach();

	virtual ATTACHMENTHANDLE FindPayload(VESSEL** pVessel=NULL) const;
private:
	void PopulatePayloadList();
	bool CheckRTL() const;
	bool AllLatchesOpen() const;

	VECTOR3 pos, dir, rot;
	unsigned short usLatchNum;

	DiscInPort LatchSignal[5], ReleaseSignal[5];
	DiscOutPort Latched[5], Released[5];
	DiscOutPort ReadyToLatch[5];

	AnimState LatchState[5];

	// list of payloads which are within grapple range
	// updated every 10 seconds by PopulatePayloadList
	vector<OBJHANDLE> vhPayloads;
	double lastUpdateTime; //SimT at which last update occurred
};

#endif //__LATCH_H