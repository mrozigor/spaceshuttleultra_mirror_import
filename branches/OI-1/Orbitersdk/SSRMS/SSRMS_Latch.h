/****************************************************************************
 Modified from Space Shuttle Ultra project

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

#include "SSRMS.h"
#include <string>

const double MAX_GRAPPLING_DIST = 0.5;
// max distance between RMS tip and grappling point for successful grappling
const double MAX_GRAPPLING_ANGLE = 0.087266;
// max angle between EE and grapple for successful grappling (radians)

/**
 * This class essentially acts as a wrapper for a single ATTACHMENTHANDLE.
 * In SSU, this class is derived from AtlantisSubsystem class
 */
class LatchSystem
{
public:
	//LatchSystem(SubsystemDirector* _director, const string& _ident, const std::string& _attachID);
	LatchSystem(SSRMS* _ssrms, const std::string& _ident, const std::string& _attachID);
	virtual ~LatchSystem();

	/**
	 * Sets attachment controlled by class
	 * @param active bool indicating if this LEE is active (can grapple payload) or passive (grappled to base PDGF)
	 */
	void SetAttachment(ATTACHMENTHANDLE ah, bool active);
	void SetAttachmentParams(const VECTOR3& pos, const VECTOR3& dir, const VECTOR3& rot) const;

	OBJHANDLE Grapple();
	void Ungrapple();
	bool Grappled() const {return (hPayloadAttachment!=NULL);};
	bool GrappledToBase() const {return (Grappled() && isGrappledToBase);};

	virtual void OnPreStep(double SimT, double DeltaT, double MJD);
	virtual bool OnParseLine(const char* line);
	virtual void OnSaveState(FILEHANDLE scn) const;
protected:
	SSRMS* GetSSRMS() const { return ssrms; };
	const std::string& GetIdentifier() const { return identifier; };
	
	void CheckForAttachedObjects();
	bool CanAttach(VESSEL* v, ATTACHMENTHANDLE ah, const VECTOR3& glatchpos, const VECTOR3& glatchdir) const;

	void AttachPayload(VESSEL* vessel, ATTACHMENTHANDLE attachment);
	void DetachPayload();

	//virtual void OnAttach() = 0;
	//virtual void OnDetach() = 0;

	ATTACHMENTHANDLE FindPayload(bool checkBaseAttachment, VESSEL** pVessel=NULL) const;
	bool PayloadIsFree() const;
	
	SSRMS* ssrms;
	std::string identifier;

	VESSEL* attachedPayload;
	ATTACHMENTHANDLE hPayloadAttachment;
	ATTACHMENTHANDLE hAttach;
	
	bool isActive;
	bool isGrappledToBase;

	std::string AttachID;

	bool firstStep;

	// used only for loading attachments from scenario
	std::string payloadName;
	int attachmentIndex;
};

#endif //__LATCH_H