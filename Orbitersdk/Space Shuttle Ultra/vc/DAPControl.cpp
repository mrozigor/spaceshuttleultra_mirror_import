#include "DAPControl.h"

extern GDIParams g_Param;

// ==============================================================

namespace vc {
	DAPControl::DAPControl(Atlantis *_sts)
		: sts(_sts)
	{
		for(int i=0;i<24;i++) oldValues[i]=false;
	}

	void DAPControl::Realize()
	{
		oapiWriteLog("DAPControl: Creating Bundle");
		DiscreteBundle* pBundle=sts->BundleManager()->CreateBundle("DAP_PBIS1", 16);
		oapiWriteLog("Bundle created");
		for(int i=0;i<16;i++) {
			oapiWriteLog("Connecting DiscPort");
			input[i].Connect(pBundle, i);
			oapiWriteLog("DiscPort connected");
			output[i].Connect(pBundle, i);
		}

		pBundle=sts->BundleManager()->CreateBundle("DAP_PBIS2", 16);
		for(int i=16;i<24;i++) {
			input[i].Connect(pBundle, i-16);
			output[i].Connect(pBundle, i-16);
		}

		for(int i=0;i<24;i++) {
			oldValues[i]=GetPBIState(i);
			if(oldValues[i]) output[i].SetLine();
			else output[i].ResetLine();
		}
	}

	void DAPControl::OnPostStep(double SimT, double DeltaT, double MJD)
	{
		for(int i=0;i<24;i++) {
			if(input[i] && !oldValues[i]) ButtonPress(i);

			oldValues[i]=GetPBIState(i);
			if(oldValues[i]) output[i].SetLine();
			else output[i].ResetLine();
		}
	}

	bool DAPControl::VCMouseEvent(int block, int nEvent, const VECTOR3 &p)
	{
		bool bRet=false;
		sprintf_s(oapiDebugString(), 255, "DAPControl: %f %f %f", p.x, p.y, p.z);
		
		if(block==1) {
			//A
			if(p.x>=0.003333 && p.x<=0.131725 && p.y>=0.028526 && p.y<=0.849584) {
				//sts->DAPMode[0]=0;
				//sts->UpdateDAP();
				ButtonPress(0);
				bRet=true;
			}
			//B
			if(p.x>=0.176299 && p.x<=0.303113 && p.y>=0.037805 && p.y<=0.871381) {
				//sts->DAPMode[0]=1;
				//sts->UpdateDAP();
				ButtonPress(1);
				bRet=true;
			}
			//AUTO
			if(p.x>=0.351384 && p.x<=0.480228 && p.y>=0.058962 && p.y<=0.899763) {
				//sts->ControlMode=Atlantis::AUTO;
				//add extra initialization
				//InitializeControlMode();
				ButtonPress(2);
				bRet=true;
			}
			//INRTL
			if(p.x>=0.51934 && p.x<=0.646509 && p.y>=0.052969 && p.y<=0.886635) {
				//sts->ControlMode=Atlantis::INRTL;
				//add extra initialization
				//InitializeControlMode();
				ButtonPress(3);
				bRet=true;
			}
			//LVLH
			if(p.x>=0.685939 && p.x<=0.815357 && p.y>=0.041121 && p.y<=0.901168) {
				//sts->ControlMode=Atlantis::LVLH;
				//add extra initialization
				//InitializeControlMode();
				ButtonPress(4);
				bRet=true;
			}
			//FREE
			if(p.x>=0.863466 && p.x<=0.99105 && p.y>=0.037412 && p.y<=0.88139) {
				//sts->ControlMode=Atlantis::FREE;
				//if(sts->PostContactThrusting[1]) sts->TogglePCT();
				//add extra initialization
				//InitializeControlMode();
				ButtonPress(5);
				bRet=true;
			}
		}
		else if(block==2) {
			//PCT
			if(p.x>=0.025329 && p.x<=0.158726 && p.y>=0.02142 && p.y<=0.286954) {
				ButtonPress(6);
				bRet=true;
				/*if(sts->PostContactThrusting[0]) {
					sts->TogglePCT();
				}*/
			}
			//PRI
			else if(p.x>=0.5279445 && p.x<=0.6480245 && p.y>=0.012412 && p.y<=0.261399) {
				ButtonPress(9);
				bRet=true;
				/*if(sts->DAPMode[1]!=0) {
					sts->DAPMode[1]=0;
					sts->UpdateDAP();
				}*/
			}
			//ALT
			else if(p.x>=0.692582 && p.x<=0.82202 && p.y>=0.001188 && p.y<=0.263191) {
				ButtonPress(10);
				bRet=true;
				/*if(sts->DAPMode[1]!=1) {
					sts->DAPMode[1]=1;
					sts->UpdateDAP();
					bRet=true;
				}*/
			}
			//VERN
			else if(p.x>=0.86248 && p.x<=0.994839 && p.y>=0.000951 && p.y<=0.260899) {
				/*if(sts->DAPMode[1]!=2) {
					sts->DAPMode[1]=2;
					sts->UpdateDAP();
				}*/
				ButtonPress(11);
				bRet=true;
			}
			//X NORM
			else if(p.x>=0.003083 && p.x<=0.129235 && p.y>=0.368609 && p.y<=0.628432) {
				ButtonPress(12);
				bRet=true;
			}
			//Y NORM
			else if(p.x>=0.172048 && p.x<=0.301167 && p.y>=0.36579 && p.y<=0.630109) {
				ButtonPress(13);
				bRet=true;
			}
			//Z NORM
			else if(p.x>=0.337985 && p.x<=0.467041 && p.y>=0.373341 && p.y<=0.628341) {
				ButtonPress(14);
				bRet=true;
			}
			//DISC RATE (ROLL)
			else if(p.x>=0.523584 && p.x<=0.656069 && p.y>=0.369307 && p.y<=0.635155) {
				ButtonPress(15);
				bRet=true;
			}
			//DISC RATE (PITCH)
			else if(p.x>=0.694456 && p.x<=0.821347 && p.y>=0.364444 && p.y<=0.631278) {
				ButtonPress(16);
				bRet=true;
			}
			//DISC RATE (YAW)
			else if(p.x>=0.865548 && p.x<=0.996359 && p.y>=0.374009 && p.y<=0.625811) {
				/*if(sts->RotMode[YAW]!=0) {
					sts->RotMode[YAW]=0;
					sts->RotPulseInProg[YAW]=false;
					bRet=true;
				}*/
				ButtonPress(17);
				bRet=true;
			}
			//X PULSE
			else if(p.x>=0.003433 && p.x<=0.126946 && p.y>=0.736736 && p.y<=0.992645) {
				ButtonPress(18);
				bRet=true;
			}
			//Y PULSE
			else if(p.x>=0.169533 && p.x<=0.295239 && p.y>=0.739643 && p.y<=0.989427) {
				ButtonPress(19);
				bRet=true;
			}
			//Z PULSE
			else if(p.x>=0.337445 && p.x<=0.465086 && p.y>=0.735088 && p.y<=0.990598) {
				ButtonPress(20);
				bRet=true;
			}
			//PULSE (ROLL)
			else if(p.x>=0.525904 && p.x<=0.653727 && p.y>=0.73224 && p.y<=0.998566) {
				ButtonPress(21);
				bRet=true;
			}
			//PULSE (PITCH)
			else if(p.x>=0.691348 && p.x<=0.818212 && p.y>=0.723454 && p.y<=0.991530) {
				ButtonPress(22);
				bRet=true;
			}
			//PULSE (YAW)
			else if(p.x>=0.868416 && p.x<=0.994284 && p.y>=0.735267 && p.y<=0.994234) {
				ButtonPress(23);
				bRet=true;
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
			case 6: //PCT
				return (sts->PostContactThrusting[1]);
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

	void DAPControl::ButtonPress(int id)
	{
		switch(id) {
			case 0:
				sts->DAPMode[0]=0;
				sts->UpdateDAP();
				break;
			case 1:
				sts->DAPMode[0]=1;
				sts->UpdateDAP();
				break;
			case 2:
				sts->ControlMode=Atlantis::AUTO;
				InitializeControlMode();
				break;
			case 3:
				sts->ControlMode=Atlantis::INRTL;
				InitializeControlMode();
				break;
			case 4:
				sts->ControlMode=Atlantis::LVLH;
				InitializeControlMode();
				break;
			case 5:
				sts->ControlMode=Atlantis::FREE;
				if(sts->PostContactThrusting[1]) sts->TogglePCT();
				InitializeControlMode();
				break;
			case 6:
				if(sts->PostContactThrusting[0]) {
					sts->TogglePCT();
				}
				break;
			case 9:
				if(sts->DAPMode[1]!=0) {
					sts->DAPMode[1]=0;
					sts->UpdateDAP();
				}
				break;
			case 10:
				if(sts->DAPMode[1]!=1) {
					sts->DAPMode[1]=1;
					sts->UpdateDAP();
				}
				break;
			case 11:
				if(sts->DAPMode[1]!=2) {
					sts->DAPMode[1]=2;
					sts->UpdateDAP();
				}
				break;
			case 12:
				if(sts->TransMode[0]!=0) {
					sts->TransMode[0]=0;
					sts->TransPulseInProg[0]=false;
					sts->TransPulseDV.x=0.0;
				}
				break;
			case 13:
				if(sts->TransMode[1]!=0) {
					sts->TransMode[1]=0;
					sts->TransPulseInProg[1]=false;
					sts->TransPulseDV.y=0.0;
				}
				break;
			case 14:
				if(sts->TransMode[2]!=0) {
					sts->TransMode[2]=0;
					sts->TransPulseInProg[2]=false;
					sts->TransPulseDV.z=0.0;
				}
				break;
			case 15:
				if(sts->RotMode[ROLL]!=0) {
					sts->RotMode[ROLL]=0;
					sts->RotPulseInProg[ROLL]=false;
				}
				break;
			case 16:
				if(sts->RotMode[PITCH]!=0) {
					sts->RotMode[PITCH]=0;
					sts->RotPulseInProg[PITCH]=false;
				}
				break;
			case 17:
				if(sts->RotMode[YAW]!=0) {
					sts->RotMode[YAW]=0;
					sts->RotPulseInProg[YAW]=false;
				}
				break;
			case 18:
				if(sts->TransMode[0]!=1 && (sts->ops==201 || sts->ops==202)) {
					sts->TransMode[0]=1;
				}
				break;
			case 19:
				if(sts->TransMode[1]!=1 && (sts->ops==201 || sts->ops==202)) {
					sts->TransMode[1]=1;
				}
				break;
			case 20:
				if(sts->TransMode[2]!=1 && (sts->ops==201 || sts->ops==202)) {
					sts->TransMode[2]=1;
				}
				break;
			case 21:
				if(sts->RotMode[ROLL]!=1) {
					sts->RotMode[ROLL]=1;
				}
				break;
			case 22:
				if(sts->RotMode[PITCH]!=1) {
					sts->RotMode[PITCH]=1;
				}
				break;
			case 23:
				if(sts->RotMode[YAW]!=1) {
					sts->RotMode[YAW]=1;
				}
				break;
		}

		/*for(int i=0;i<24;i++) {
			oldValues[i]=GetPBIState(i);
			if(oldValues[i]) output[i].SetLine();
			else output[i].ResetLine();
		}*/
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
