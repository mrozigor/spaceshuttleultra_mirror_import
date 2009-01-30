#ifndef __STBDMPMSYSTEM_H
#define __STBDMPMSYSTEM_H
#pragma once

#include <vector>
#include "MPMSystems.h"

const static char* MPM_MESHNAME = "SSU/OBSSMPMs";

const double MAX_ATTACHMENT_DIST = 0.10;
const double MAX_ATTACHMENT_ANGLE = 0.087266;

const VECTOR3 MPM_MESH_OFFSET = _V(2.81, 1.60, 1.68);
const VECTOR3 OBSS_ATTACHMENT_POINT = _V(0.06, 0.28, 1.457);
//const VECTOR3 OBSS_ATTACHMENT_POINT = _V(2.87, 1.88, 3.137);


class StbdMPMSystem : public MPMSystem
{
public:
	StbdMPMSystem(SubsystemDirector* _director);
	virtual ~StbdMPMSystem();

	virtual void Realize();
	virtual void OnPreStep(double SimT, double DeltaT, double MJD);
	/*virtual void OnPostStep(double SimT, double DeltaT, double MJD);
	virtual bool OnParseLine(const char* line);
	virtual void OnSaveState(FILEHANDLE scn) const;*/
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

	//NOTE: any item in vpOBSS MUST have a corresponding entry in vhOBSSAttach at the same index
	vector<VESSEL*> vpOBSS; //OBSS vessels in scenario
	vector<ATTACHMENTHANDLE> vhOBSSAttach; //attchment point on OBSS(s) that attaches to MPM

	VECTOR3 obss_attach_point[2];
};

#endif //__STBDMPMSYSTEM_H