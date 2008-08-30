#include "DAPControl.h"

// ==============================================================

namespace vc {
	DAPControl::DAPControl(Atlantis *_sts)
		: sts(_sts)
	{
	}

	bool DAPControl::VCMouseEvent(int block, int nEvent, const VECTOR3 &p)
	{
		bool bRet=false;
		sprintf_s(oapiDebugString(), 255, "DAPControl: %f %f %f", p.x, p.y, p.z);
		
		if(block==1) {
		}
		else if(block==2) {
			//PRI
			if(p.x>=0.5279445 && p.x<=0.6480245 && p.y>=0.012412 && p.y<=0.261399) {
				if(sts->DAPMode[1]!=0) {
					sts->DAPMode[1]=0;
					sts->UpdateDAP();
					bRet=true;
				}
			}
			//ALT
			else if(p.x>=0.692582 && p.x<=0.82202 && p.y>=0.001188 && p.y<=0.263191) {
				if(sts->DAPMode[1]!=1) {
					sts->DAPMode[1]=1;
					sts->UpdateDAP();
					bRet=true;
				}
			}
			//VERN
			else if(p.x>=0.86248 && p.x<=0.994839 && p.y>=0.000951 && p.y<=0.260899) {
				if(sts->DAPMode[1]!=2) {
					sts->DAPMode[1]=2;
					sts->UpdateDAP();
					bRet=true;
				}
			}
			//DISC RATE (ROLL)
			else if(p.x>=0.523584 && p.x<=0.656069 && p.y>=0.369307 && p.y<=0.635155) {
				if(sts->RotMode[2]!=0) {
					sts->RotMode[2]=0;
					sprintf_s(oapiDebugString(), 255, "Update RCS settings");
					bRet=true;
				}
			}
			//DISC RATE (PITCH)
			else if(p.x>=0.694456 && p.x<=0.821347 && p.y>=0.364444 && p.y<=0.631278) {
				if(sts->RotMode[0]!=0) {
					sts->RotMode[0]=0;
					sprintf_s(oapiDebugString(), 255, "Update RCS settings");
					bRet=true;
				}
			}
			//DISC RATE (YAW)
			else if(p.x>=0.865548 && p.x<=0.996359 && p.y>=0.374009 && p.y<=0.625811) {
				if(sts->RotMode[1]!=0) {
					sts->RotMode[1]=0;
					sprintf_s(oapiDebugString(), 255, "Update RCS settings");
					bRet=true;
				}
			}
			//PULSE (ROLL)
			else if(p.x>=0.525904 && p.x<=0.653727 && p.y>=0.73224 && p.y<=0.998566) {
				if(sts->RotMode[2]!=1) {
					sts->RotMode[2]=1;
					sprintf_s(oapiDebugString(), 255, "Update RCS settings");
					bRet=true;
				}
			}
			//PULSE (PITCH)
			else if(p.x>=0.691348 && p.x<=0.818212 && p.y>=0.723454 && p.y<=0.991530) {
				if(sts->RotMode[0]!=1) {
					sts->RotMode[0]=1;
					sprintf_s(oapiDebugString(), 255, "Update RCS settings");
					bRet=true;
				}
			}
			//PULSE (YAW)
			else if(p.x>=0.868416 && p.x<=0.994284 && p.y>=0.735267 && p.y<=0.994234) {
				if(sts->RotMode[1]!=1) {
					sts->RotMode[1]=1;
					sprintf_s(oapiDebugString(), 255, "Update RCS settings");
					bRet=true;
				}
			}
		}

		return bRet;
	}
};
