#include "DAPControl.h"

extern GDIParams g_Param;

// ==============================================================

namespace vc {
	DAPControl::DAPControl(Atlantis *_sts)
		: sts(_sts)
	{
		for(int i=0;i<24;i++) oldValues[i]=false;
		//bPitchAuto = true;
		//bRollYawAuto = true;
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

		pBundle=sts->BundleManager()->CreateBundle("CSS_CONTROLS", 4);
		PitchAutoOut.Connect(pBundle, 0);
		PitchAutoIn.Connect(pBundle, 0);
		PitchCSSOut.Connect(pBundle, 1);
		PitchCSSIn.Connect(pBundle, 1);
		RollYawAutoOut.Connect(pBundle, 2);
		RollYawAutoIn.Connect(pBundle, 2);
		RollYawCSSOut.Connect(pBundle, 3);
		RollYawCSSIn.Connect(pBundle, 3);
	}

	void DAPControl::OnPostStep(double SimT, double DeltaT, double MJD)
	{
		for(int i=0;i<24;i++) {
			if(input[i] && !oldValues[i]) ButtonPress(i);

			oldValues[i]=GetPBIState(i);
			if(oldValues[i]) output[i].SetLine();
			else output[i].ResetLine();
		}

		// handle AUTO/CSS PBIs
		switch(sts->ops) {
			case 105:
			case 106:
			case 201:
			case 202:
			case 301:
			case 302:
			case 303:
				PitchAutoOut.ResetLine();
				PitchCSSOut.ResetLine();
				RollYawAutoOut.ResetLine();
				RollYawCSSOut.ResetLine();
				break;
			case 304:
				break;
			case 305:
				// for the moment, force CSS
				PitchCSSOut.SetLine();
				RollYawCSSOut.SetLine();
				PitchAutoOut.ResetLine();
				RollYawAutoOut.ResetLine();
				break;
		}
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
				//sts->GetStatus(sts->Status);
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
