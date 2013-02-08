/****************************************************************************
  This file is part of Space Shuttle Ultra

  Starboard MPM system



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
#ifndef __STBDMPMSYSTEM_H
#define __STBDMPMSYSTEM_H
#pragma once

#include <vector>
#include "MPMSystems.h"

const static char* MPM_MESHNAME = "SSU/OBSSMPMs";

const double MAX_ATTACHMENT_DIST = 0.10;
const double MAX_ATTACHMENT_ANGLE = 0.087266;

//const VECTOR3 MPM_MESH_OFFSET = _V(2.75, 1.65, 1.98);
const VECTOR3 MPM_MESH_OFFSET = _V(2.75, 1.65, 1.9);
const VECTOR3 OBSS_ATTACHMENT_POINT = _V(0.11, 0.3, 1.457);
//const VECTOR3 OBSS_ATTACHMENT_POINT = _V(2.87, 1.88, 3.137);


class StbdMPMSystem : public MPMSystem
{
public:
	StbdMPMSystem(AtlantisSubsystemDirector* _director);
	virtual ~StbdMPMSystem();

	virtual void Realize();
	virtual void OnPreStep(double SimT, double DeltaT, double MJD);
	virtual void OnPostStep(double SimT, double DeltaT, double MJD);
	/*virtual bool OnParseLine(const char* line);*/
	virtual void OnSaveState(FILEHANDLE scn) const;

	virtual void CreateAttachment();
protected:
	virtual void OnMRLLatched();
	virtual void OnMRLReleased();
private:
	void AttachOBSS();
	void FindOBSSAttachments();
	bool CheckRTL() const;
	/**
	 * Returns index (in vpOBSS and vhOBSSAttach vectors) of first OBSS found that can be attached to the MPMs.
	 * Returns -1 if no attachment is within range.
	 */
	int FindOBSS() const;

	//NOTE: any item in vhOBSS MUST have a corresponding entry in vhOBSSAttach at the same index
	vector<OBJHANDLE> vhOBSS; // OBSS vessels in scenario
	vector<ATTACHMENTHANDLE> vhOBSSAttach; //attchment point on OBSS(s) that attaches to MPM

	VECTOR3 obss_attach_point[2];
};

#endif //__STBDMPMSYSTEM_H
