#include "DAPControl.h"

extern GDIParams g_Param;

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
			//A
			if(p.x>=0.003333 && p.x<=0.131725 && p.y>=0.028526 && p.y<=0.849584) {
				sts->DAPMode[0]=0;
				sts->UpdateDAP();
				bRet=true;
			}
			//B
			if(p.x>=0.176299 && p.x<=0.303113 && p.y>=0.037805 && p.y<=0.871381) {
				sts->DAPMode[0]=1;
				sts->UpdateDAP();
				bRet=true;
			}
			//AUTO
			if(p.x>=0.351384 && p.x<=0.480228 && p.y>=0.058962 && p.y<=0.899763) {
				sts->ControlMode=Atlantis::AUTO;
				//add extra initialization
				bRet=true;
			}
			//INRTL
			if(p.x>=0.51934 && p.x<=0.646509 && p.y>=0.052969 && p.y<=0.886635) {
				sts->ControlMode=Atlantis::INRTL;
				sprintf_s(oapiDebugString(), 255, "INRTL");
				//add extra initialization
				bRet=true;
			}
			//LVLH
			if(p.x>=0.685939 && p.x<=0.815357 && p.y>=0.041121 && p.y<=0.901168) {
				sts->ControlMode=Atlantis::LVLH;
				//add extra initialization
				bRet=true;
			}
			//FREE
			if(p.x>=0.863466 && p.x<=0.99105 && p.y>=0.037412 && p.y<=0.88139) {
				sts->ControlMode=Atlantis::FREE;
				//add extra initialization
				for(int i=0;i<3;i++) sts->RotMode[i]=1;
				bRet=true;
			}
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
				if(sts->RotMode[ROLL]!=0) {
					sts->RotMode[ROLL]=0;
					sprintf_s(oapiDebugString(), 255, "Update RCS settings");
					bRet=true;
				}
			}
			//DISC RATE (PITCH)
			else if(p.x>=0.694456 && p.x<=0.821347 && p.y>=0.364444 && p.y<=0.631278) {
				if(sts->RotMode[PITCH]!=0) {
					sts->RotMode[PITCH]=0;
					sprintf_s(oapiDebugString(), 255, "Update RCS settings");
					bRet=true;
				}
			}
			//DISC RATE (YAW)
			else if(p.x>=0.865548 && p.x<=0.996359 && p.y>=0.374009 && p.y<=0.625811) {
				if(sts->RotMode[YAW]!=0) {
					sts->RotMode[YAW]=0;
					sprintf_s(oapiDebugString(), 255, "Update RCS settings");
					bRet=true;
				}
			}
			//PULSE (ROLL)
			else if(p.x>=0.525904 && p.x<=0.653727 && p.y>=0.73224 && p.y<=0.998566) {
				if(sts->RotMode[ROLL]!=1) {
					sts->RotMode[ROLL]=1;
					sprintf_s(oapiDebugString(), 255, "Update RCS settings");
					bRet=true;
				}
			}
			//PULSE (PITCH)
			else if(p.x>=0.691348 && p.x<=0.818212 && p.y>=0.723454 && p.y<=0.991530) {
				if(sts->RotMode[PITCH]!=1) {
					sts->RotMode[PITCH]=1;
					sprintf_s(oapiDebugString(), 255, "Update RCS settings");
					bRet=true;
				}
			}
			//PULSE (YAW)
			else if(p.x>=0.868416 && p.x<=0.994284 && p.y>=0.735267 && p.y<=0.994234) {
				if(sts->RotMode[YAW]!=1) {
					sts->RotMode[YAW]=1;
					sprintf_s(oapiDebugString(), 255, "Update RCS settings");
					bRet=true;
				}
			}
		}

		return bRet;
	}

	bool DAPControl::UpdatePBI(SURFHANDLE surf, int id, bool &bState)
	{
		switch(id) {
			case 0: //A
				return DrawPBILight(surf, bState, (sts->DAPMode[0]==0));
				break;
			case 1: //B
				return DrawPBILight(surf, bState, (sts->DAPMode[0]==1));
				break;
			case 2: //AUTO
				return DrawPBILight(surf, bState, (sts->ControlMode==Atlantis::AUTO));
				break;
			case 3: //INRTL
				return DrawPBILight(surf, bState, (sts->ControlMode==Atlantis::INRTL));
				break;
			case 4: //LVLH
				return DrawPBILight(surf, bState, (sts->ControlMode==Atlantis::LVLH));
				break;
			case 5: //FREE
				return DrawPBILight(surf, bState, (sts->ControlMode==Atlantis::FREE));
				break;
		}
		return false;
	}

	bool DAPControl::DrawPBILight(SURFHANDLE surf, bool &bState, bool bOn)
	{
		if(bState==bOn) return false; //nothing to do here
		else {
			if(bOn) oapiBlt(surf, g_Param.pbi_lights, 0, 0, 0, 0, 42, 14);
			else oapiBlt(surf, g_Param.pbi_lights, 0, 0, 0, 14, 42, 14);
			bState=bOn;
		}
		return true;
	}
};
