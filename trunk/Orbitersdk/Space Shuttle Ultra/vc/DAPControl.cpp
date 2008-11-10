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
				InitializeControlMode();
				bRet=true;
			}
			//INRTL
			if(p.x>=0.51934 && p.x<=0.646509 && p.y>=0.052969 && p.y<=0.886635) {
				sts->ControlMode=Atlantis::INRTL;
				sprintf_s(oapiDebugString(), 255, "INRTL");
				//add extra initialization
				InitializeControlMode();
				/*sts->TargetAttOrbiter=sts->InertialOrientationRad;
				sts->TargetAttM50=sts->CurrentAttitude;
				sts->REQD_ATT=sts->CurrentAttitude*DEG;
				sts->ManeuverinProg=true;
				sts->ManeuverStatus=Atlantis::MNVR_COMPLETE; //(check value set here)*/
				bRet=true;
			}
			//LVLH
			if(p.x>=0.685939 && p.x<=0.815357 && p.y>=0.041121 && p.y<=0.901168) {
				sts->ControlMode=Atlantis::LVLH;
				//add extra initialization
				InitializeControlMode();
				/*sts->GetStatus(sts->Status);
				sts->LVLHOrientationReqd=sts->CalcLVLHAttitude()*DEG;
				sts->ReqdAttMatrix=sts->ConvertLVLHAnglesToM50Matrix(sts->LVLHOrientationReqd*RAD);
				sts->ManeuverinProg=true;
				sts->ManeuverStatus=Atlantis::MNVR_COMPLETE;*/
				bRet=true;
			}
			//FREE
			if(p.x>=0.863466 && p.x<=0.99105 && p.y>=0.037412 && p.y<=0.88139) {
				sts->ControlMode=Atlantis::FREE;
				//add extra initialization
				InitializeControlMode();
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
			//X NORM
			else if(p.x>=0.003083 && p.x<=0.129235 && p.y>=0.368609 && p.y<=0.628432) {
				if(sts->TransMode[0]!=0) {
					sts->TransMode[0]=0;
					sts->TransPulseInProg[0]=false;
					sts->TransPulseDV.x=0.0;
					bRet=true;
				}
			}
			//Y NORM
			else if(p.x>=0.172048 && p.x<=0.301167 && p.y>=0.36579 && p.y<=0.630109) {
				if(sts->TransMode[1]!=0) {
					sts->TransMode[1]=0;
					sts->TransPulseInProg[1]=false;
					sts->TransPulseDV.y=0.0;
					bRet=true;
				}
			}
			//Z NORM
			else if(p.x>=0.337985 && p.x<=0.467041 && p.y>=0.373341 && p.y<=0.628341) {
				if(sts->TransMode[2]!=0) {
					sts->TransMode[2]=0;
					sts->TransPulseInProg[2]=false;
					sts->TransPulseDV.z=0.0;
					bRet=true;
				}
			}
			//DISC RATE (ROLL)
			else if(p.x>=0.523584 && p.x<=0.656069 && p.y>=0.369307 && p.y<=0.635155) {
				if(sts->RotMode[ROLL]!=0) {
					sts->RotMode[ROLL]=0;
					sts->RotPulseInProg[ROLL]=false;
					bRet=true;
				}
			}
			//DISC RATE (PITCH)
			else if(p.x>=0.694456 && p.x<=0.821347 && p.y>=0.364444 && p.y<=0.631278) {
				if(sts->RotMode[PITCH]!=0) {
					sts->RotMode[PITCH]=0;
					sts->RotPulseInProg[PITCH]=false;
					bRet=true;
				}
			}
			//DISC RATE (YAW)
			else if(p.x>=0.865548 && p.x<=0.996359 && p.y>=0.374009 && p.y<=0.625811) {
				if(sts->RotMode[YAW]!=0) {
					sts->RotMode[YAW]=0;
					sts->RotPulseInProg[YAW]=false;
					bRet=true;
				}
			}
			//X PULSE
			else if(p.x>=0.003433 && p.x<=0.126946 && p.y>=0.736736 && p.y<=0.992645) {
				if(sts->TransMode[0]!=1 && (sts->ops==201 || sts->ops==202)) {
					sts->TransMode[0]=1;
					bRet=true;
				}
			}
			//Y PULSE
			else if(p.x>=0.169533 && p.x<=0.295239 && p.y>=0.739643 && p.y<=0.989427) {
				if(sts->TransMode[1]!=1 && (sts->ops==201 || sts->ops==202)) {
					sts->TransMode[1]=1;
					bRet=true;
				}
			}
			//Z PULSE
			else if(p.x>=0.337445 && p.x<=0.465086 && p.y>=0.735088 && p.y<=0.990598) {
				if(sts->TransMode[2]!=1 && (sts->ops==201 || sts->ops==202)) {
					sts->TransMode[2]=1;
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

	bool DAPControl::GetPBIState(int id)
	{
		switch(id) {
			case 0: //A
				return (sts->DAPMode[0]==0);
				break;
			case 1: //B
				return (sts->DAPMode[0]==1);
				break;
			case 2: //AUTO
				return (sts->ControlMode==Atlantis::AUTO);
				break;
			case 3: //INRTL
				return (sts->ControlMode==Atlantis::INRTL);
				break;
			case 4: //LVLH
				return (sts->ControlMode==Atlantis::LVLH);
				break;
			case 5: //FREE
				return (sts->ControlMode==Atlantis::FREE);
				break;
			case 6: //TRANS X
				break;
			case 7: //LOW Z
				break;
			case 8: //HIGH Z
				break;
			case 9: //PRI
				return (sts->DAPMode[1]==0);
				break;
			case 10: //ALT
				return (sts->DAPMode[1]==1);
				break;
			case 11: //VERN
				return (sts->DAPMode[1]==2);
				break;
			case 12: //TRANS X NORM
				return (sts->TransMode[0]==0);
				break;
			case 13: //TRANS Y NORM
				return (sts->TransMode[1]==0);
				break;
			case 14: //TRANS Z NORM
				return (sts->TransMode[2]==0);
				break;
			case 15: //ROT ROLL DISC RATE
				return (sts->RotMode[ROLL]==0);
				break;
			case 16: //ROT PITCH DISC RATE
				return (sts->RotMode[PITCH]==0);
				break;
			case 17: //ROT YAW DISC RATE
				return (sts->RotMode[YAW]==0);
				break;
			case 18: //TRANS X PULSE
				return (sts->TransMode[0]==1);
				break;
			case 19: //TRANS Y PULSE
				return (sts->TransMode[1]==1);
				break;
			case 20: //TRANS Z PULSE
				return (sts->TransMode[2]==1);
				break;
			case 21: //ROT ROLL PULSE
				return (sts->RotMode[ROLL]==1);
				break;
			case 22: //ROT PITCH PULSE
				return (sts->RotMode[PITCH]==1);
				break;
			case 23: //ROT YAW PULSE
				return (sts->RotMode[YAW]==1);
				break;
		}
		return false;
	}

	void DAPControl::InitializeControlMode()
	{
		switch(sts->ControlMode) {
			case Atlantis::AUTO:
				sts->StartAttManeuver();
				break;
			case Atlantis::INRTL:
				sts->TargetAttOrbiter=sts->InertialOrientationRad;
				sts->TargetAttM50=sts->CurrentAttitude;
				sts->REQD_ATT=sts->CurrentAttitude*DEG;
				sts->ManeuverinProg=true;
				sts->ManeuverStatus=Atlantis::MNVR_COMPLETE; //(check value set here)
				break;
			case Atlantis::LVLH:
				sts->GetGlobalPos(sts->GVesselPos);
				sts->GetStatus(sts->Status);
				sts->LVLHOrientationReqd=sts->CalcLVLHAttitude()*DEG;
				sts->ReqdAttMatrix=sts->ConvertLVLHAnglesToM50Matrix(sts->LVLHOrientationReqd*RAD);
				sts->ManeuverinProg=true;
				sts->ManeuverStatus=Atlantis::MNVR_COMPLETE;
				break;
			case Atlantis::FREE:
				for(int i=0;i<3;i++) sts->RotMode[i]=1;
				break;
		}
	}
};
