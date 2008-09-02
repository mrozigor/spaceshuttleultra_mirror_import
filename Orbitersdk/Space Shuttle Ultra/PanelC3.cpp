#include "PanelC3.h"
#include "resource.h"
//#include "meshres.h"
#include "meshres_vc.h"
#include "meshres_vc_additions.h"
#include "DlgCtrl.h"
#include <stdio.h>
#include "AirDataProbeSystem.h"

extern GDIParams g_Param;
extern HELPCONTEXT g_hc;
extern char *ActionString[5];

//bounding box for DAP Block1
const VECTOR3 DAP1ULCorner=_V(0.0721105, 0.4188465, 0); // Upper Left
const VECTOR3 DAP1LRCorner=_V(0.376816, 0.461075, 0); // Lower Right
//bounding box for DAP Block2
const VECTOR3 DAP2ULCorner=_V(0.073854, 0.5236025, 0); // Upper Left
const VECTOR3 DAP2LRCorner=_V(0.376923, 0.6617365, 0); // Lower Right

// ==============================================================

PanelC3::PanelC3(Atlantis *_sts): sts(_sts)
{
	int i;
	for(i=0;i<2;i++)
	{
		Air_Data_Probe[i]=2;
		Air_Data_Stow[i]=1;
		OMS_Eng[i]=2;
		AirDataProbe[i].Set(AnimState::CLOSED, 0.0);
	}
	for(i=0;i<24;i++) PBI_Lights[i]=false;
}

bool PanelC3::VCRedrawEvent(int id, int nEvent, SURFHANDLE surf)
{
	//sprintf_s(oapiDebugString(), 255, "VCRedrawEvent");
	switch(id) {
		case AID_C3_PBI1: //A
			return VCDrawPBILight(surf, id-AID_C3_PBI1, (sts->DAPMode[0]==0));
			break;
		case AID_C3_PBI2: //B
			return VCDrawPBILight(surf, id-AID_C3_PBI1, (sts->DAPMode[0]==1));
			break;
		case AID_C3_PBI3: //AUTO
			return VCDrawPBILight(surf, id-AID_C3_PBI1, (sts->ControlMode==Atlantis::AUTO));
			break;
		case AID_C3_PBI4: //INRTL
			return VCDrawPBILight(surf, id-AID_C3_PBI1, (sts->ControlMode==Atlantis::INRTL));
			break;
		case AID_C3_PBI5: //LVLH
			return VCDrawPBILight(surf, id-AID_C3_PBI1, (sts->ControlMode==Atlantis::LVLH));
			break;
		case AID_C3_PBI6: //FREE
			return VCDrawPBILight(surf, id-AID_C3_PBI1, (sts->ControlMode==Atlantis::FREE));
			break;
	}
	return false;
}

bool PanelC3::VCDrawPBILight(SURFHANDLE surf, int idx, bool bOn)
{
	if(PBI_Lights[idx]==bOn) return false; //nothing to do here
	else {
		if(bOn) oapiBlt(surf, g_Param.pbi_lights, 0, 0, 0, 0, 42, 14);
		else oapiBlt(surf, g_Param.pbi_lights, 0, 0, 0, 14, 42, 14);
		PBI_Lights[idx]=bOn;
	}
	return true;
}

void PanelC3::RegisterVC()
{
	VECTOR3 ofs = sts->orbiter_ofs;
	SURFHANDLE panelc3_tex = oapiGetTextureHandle (sts->hOrbiterVCMesh, TEX_PANELC3_VC);
	oapiVCRegisterArea (AID_C3, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN);

	//-.2732771, 1.76045, 14.35651
	// .2732771 1.69551  13.73551
	oapiVCSetAreaClickmode_Quadrilateral (AID_C3, _V(-0.2732771, 1.76045, 14.35651)+ofs, _V(0.2732771, 1.76045, 14.35651)+ofs, 
		_V(-0.2732771, 1.69551,  13.73551)+ofs, _V(0.2732771, 1.69551,  13.73551)+ofs);
	//oapiVCSetAreaClickmode_Quadrilateral (AID_C3, _V(-0.0804, 1.667, 14.52)+ofs, _V(-0.111, 1.667, 14.52)+ofs, _V(-0.0804, 1.667, 14.52)+ofs, _V(0.276, 1.618, 14.066)+ofs);
	
	//register DAP PBIs
	//A
	oapiVCRegisterArea(AID_C3_PBI1, _R(155, 888, 197, 902), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
	//B
	oapiVCRegisterArea(AID_C3_PBI2, _R(250, 887, 292, 901), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
	//AUTO
	oapiVCRegisterArea(AID_C3_PBI3, _R(344, 888, 386, 902), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
	//INRTL
	oapiVCRegisterArea(AID_C3_PBI4, _R(434, 889, 476, 903), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
	//LVLH
	oapiVCRegisterArea(AID_C3_PBI5, _R(527, 889, 569, 903), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
	//FREE
	oapiVCRegisterArea(AID_C3_PBI6, _R(622, 889, 664, 903), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
	//TRANS X
	//oapiVCRegisterArea(AID_C3_PBI7, _R(157, 1089, 199, 1103), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
	//LOW Z
	//oapiVCRegisterArea(AID_C3_PBI8, _R(247, 1089, 289, 1103), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
	//HIGH Z
	//oapiVCRegisterArea(AID_C3_PBI9, _R(339, 1090, 381, 1104), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
	//PRI
	oapiVCRegisterArea(AID_C3_PBI10, _R(440, 1089, 482, 1103), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
	//ALT
	oapiVCRegisterArea(AID_C3_PBI11, _R(531, 1090, 573, 1104), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
	//VERN
	oapiVCRegisterArea(AID_C3_PBI12, _R(623, 1090, 665, 1104), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
}

bool PanelC3::VCMouseEvent(int id, int nEvent, VECTOR3 &p)
{
	if (id != AID_C3) return false;
	bool action = false;
	int eng=-1;
	sprintf_s(oapiDebugString(), 255, "id %d event %d p %f %f %f",id,nEvent,p.x,p.y,p.z);
	
	if(p.y> 0.753680 && p.y< 0.864232 && p.x>=0.080556 && p.x<=0.208679)
	{
		if(p.y>0.80366)
		{
			if(p.x<=.148883)
			{
				if(Air_Data_Probe[0]<2) 
				{
					Air_Data_Probe[0]++;
					action=true;
				}
			}
			else
			{
				if(Air_Data_Probe[1]<2) 
				{
					Air_Data_Probe[1]++;
					action=true;
				}
			}
		}
		else
		{
			if(p.x<=.148883)
			{
				if(Air_Data_Probe[0]>0) 
				{
					Air_Data_Probe[0]--;
					action=true;
				}
			}
			else
			{
				if(Air_Data_Probe[1]>0) 
				{
					Air_Data_Probe[1]--;
					action=true;
				}
			}
		}
	}
	else if(p.x>=.063720 && p.x<=.189637 && p.y>.255919 && p.y<.321174)
	{
		if(p.y<.292911)
		{
			if(p.x<=.126235)
			{
				Air_Data_Stow[0]=0;
				action=true;
			}
			else
			{
				Air_Data_Stow[1]=0;
				action=true;
			}
		}
		else
		{
			if(p.x<=.126235)
			{
				Air_Data_Stow[0]=1;
				action=true;
			}
			else
			{
				Air_Data_Stow[1]=1;
				action=true;
			}
		}
	}
	else if(p.y>.070910 && p.y<.173581 && p.x>=0.063487 && p.x<=0.179360)
	{
		if(p.y>.126725)
		{
			if(p.x<=.117992)
			{
				if(OMS_Eng[0]<2) 
				{
					OMS_Eng[0]++;
					action=true;
					eng=0;
				}
			}
			else
			{
				if(OMS_Eng[1]<2) 
				{
					OMS_Eng[1]++;
					action=true;
					eng=1;
				}
			}
		}
		else
		{
			if(p.x<=.117992)
			{
				if(OMS_Eng[0]>0) 
				{
					OMS_Eng[0]--;
					action=true;
					eng=0;
				}
			}
			else
			{
				if(OMS_Eng[1]>0) 
				{
					OMS_Eng[1]--;
					action=true;
					eng=1;
				}
			}
		}
	}
	else if(p.x>=DAP1ULCorner.x && p.x<=DAP1LRCorner.x && p.y>=DAP1ULCorner.y && p.y<=DAP1LRCorner.y) {
		//top row of DAP PBIs clicked
		VECTOR3 pos;
		pos.x=(p.x-DAP1ULCorner.x)/(DAP1LRCorner.x-DAP1ULCorner.x);
		pos.y=(p.y-DAP1ULCorner.y)/(DAP1LRCorner.y-DAP1ULCorner.y);
		pos.z=0.0;
		if(sts->dapcontrol->VCMouseEvent(1, nEvent, pos)) action=true;
	}
	else if(p.x>=DAP2ULCorner.x && p.x<=DAP2LRCorner.x && p.y>=DAP2ULCorner.y && p.y<=DAP2LRCorner.y) {
		//second block of DAP PBIs clicked
		VECTOR3 pos;
		pos.x=(p.x-DAP2ULCorner.x)/(DAP2LRCorner.x-DAP2ULCorner.x);
		pos.y=(p.y-DAP2ULCorner.y)/(DAP2LRCorner.y-DAP2ULCorner.y);
		pos.z=0.0;
		if(sts->dapcontrol->VCMouseEvent(2, nEvent, pos)) action=true;
	}

	if(action)
	{
		UpdateVC();
		if(eng>=0) EngControl(eng);
		//else AirDataProbeControl();
	}
	return false;
}

void PanelC3::UpdateVC()
{
	int i;
	for(i=0;i<2;i++)
	{
		sts->SetAnimation(anim_VC_C3[22+i],double(Air_Data_Probe[i])/2.0);
		sts->SetAnimation(anim_VC_C3[11+i],Air_Data_Stow[i]);
		sts->SetAnimation(anim_VC_C3[i],double(OMS_Eng[i])/2.0);
	}

	for(i=0;i<24;i++) oapiVCTriggerRedrawArea(-1, AID_C3_PBI1+i);
	return;
}

void PanelC3::DefineVCAnimations (UINT vcidx)
{
	static VECTOR3 switch_rot = {-1,0,0};

	static UINT VC_C3b1_Grp = GRP_C3b1_VC;
	static MGROUP_ROTATE VC_C3b1 (vcidx, &VC_C3b1_Grp, 1,
		REF_C3S1, switch_rot, (float)(90.0*RAD));
	anim_VC_C3[0]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_C3[0], 0, 1, &VC_C3b1);

	static UINT VC_C3b2_Grp = GRP_C3b2_VC;
	static MGROUP_ROTATE VC_C3b2 (vcidx, &VC_C3b2_Grp, 1,
		REF_C3S2, switch_rot, (float)(90.0*RAD));
	anim_VC_C3[1]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_C3[1], 0, 1, &VC_C3b2);

	static UINT VC_C3b10_Grp = GRP_C3b10_VC;
	static MGROUP_ROTATE VC_C3b10 (vcidx, &VC_C3b10_Grp, 1,
		REF_C3S19, switch_rot, (float)(90.0*RAD));
	anim_VC_C3[11]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_C3[11], 0, 1, &VC_C3b10);

	static UINT VC_C3b11_Grp = GRP_C3b11_VC;
	static MGROUP_ROTATE VC_C3b11 (vcidx, &VC_C3b11_Grp, 1,
		REF_C3S20, switch_rot, (float)(90.0*RAD));
	anim_VC_C3[12]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_C3[12], 0, 1, &VC_C3b11);

	static UINT VC_C3b23_Grp = GRP_C3b23_VC;
	static MGROUP_ROTATE VC_C3b23 (vcidx, &VC_C3b23_Grp, 1,
		REF_C3S8, switch_rot, (float)(90.0*RAD));
	anim_VC_C3[22]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_C3[22], 0, 1, &VC_C3b23);

	static UINT VC_C3b24_Grp = GRP_C3b24_VC;
	static MGROUP_ROTATE VC_C3b24 (vcidx, &VC_C3b24_Grp, 1,
		REF_C3S9, switch_rot, (float)(90.0*RAD));
	anim_VC_C3[23]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_C3[23], 0, 1, &VC_C3b24);
	return;
}

void PanelC3::Step(double t, double dt)
{
	/*
	for(int i=0;i<2;i++) {
		if(AirDataProbe[i].Moving()) {
			double da=dt*AIR_DATA_PROBE_SPEED;
			if(AirDataProbe[i].Opening()) {
				AirDataProbe[i].pos=min(1.0, AirDataProbe[i].pos+da);
				if(AirDataProbe[i].pos>=1.0) AirDataProbe[i].Set(AnimState::OPEN, 1.0);
			}
			else {
				AirDataProbe[i].pos=max(0.0, AirDataProbe[i].pos-da);
				if(AirDataProbe[i].pos<=0.0) AirDataProbe[i].Set(AnimState::CLOSED, 0.0);
			}
		}
	}*/
	//sprintf(oapiDebugString(), "%f %f", AirDataProbe[0].pos, AirDataProbe[1].pos);


	if(Air_Data_Probe[0] <= 1)
	{
		sts->pADPS->SetDeployMode(0, AirDataProbeSystem::ADPS_DEPLOY);
	} else
	{
		sts->pADPS->SetDeployMode(0, AirDataProbeSystem::ADPS_STOW);
	}

	if(Air_Data_Probe[1] <= 1)
	{
		sts->pADPS->SetDeployMode(1, AirDataProbeSystem::ADPS_DEPLOY);
	} else
	{
		sts->pADPS->SetDeployMode(1, AirDataProbeSystem::ADPS_STOW);
	}
}

bool PanelC3::CheckProbesDeployed()
{
	if(sts->pADPS->IsDeployed(0) || sts->pADPS->IsDeployed(1)) return true;
	else return false;
}

void PanelC3::EngControl(int eng)
{
	if(sts->GetPropellantMass(sts->oms_helium_tank[eng])>0.0 && OMS_Eng[eng]<2) {
		sts->SetThrusterResource(sts->th_oms[eng], sts->ph_oms);
	}
	else if(sts->GetPropellantMass(sts->oms_helium_tank[eng])<=0.0 || OMS_Eng[eng]==2) {
		sts->SetThrusterResource(sts->th_oms[eng], NULL);
	}
}

void PanelC3::AirDataProbeControl()
{
	/*
	//sprintf(oapiDebugString(), "AirDataProbeControl()");
	for(int i=0;i<2;i++)
	{
		if(Air_Data_Stow[i]==0) {
			if(Air_Data_Probe[i]!=2 && AirDataProbe[i].action!=AnimState::OPEN && AirDataProbe[i].action!=AnimState::OPENING)
			{
				AirDataProbe[i].action=AnimState::OPENING;
			}
			else if(Air_Data_Probe[i]==2 && AirDataProbe[i].action!=AnimState::CLOSED && AirDataProbe[i].action!=AnimState::CLOSING)
			{
				AirDataProbe[i].action=AnimState::CLOSING;
			}
		}
	}
	*/

	
}

bool PanelC3::CheckOMSArm(int nEng)
{
	if(OMS_Eng[nEng]<2) return true;
	return false;
}

void PanelC3::SaveState(FILEHANDLE scn)
{
	char cbuf[256];
	oapiWriteScenario_int (scn, "LPROBEARM", Air_Data_Stow[0]);
	oapiWriteScenario_int (scn, "LPROBE", Air_Data_Probe[0]);
	oapiWriteScenario_int (scn, "RPROBEARM", Air_Data_Stow[1]);
	oapiWriteScenario_int (scn, "RPROBE", Air_Data_Probe[1]);
	oapiWriteScenario_int (scn, "LOMS", OMS_Eng[0]);
	oapiWriteScenario_int (scn, "ROMS", OMS_Eng[1]);
	sprintf_s(cbuf, 255, "%f %f", AirDataProbe[0].pos, AirDataProbe[1].pos);
	oapiWriteScenario_string (scn, "AIR_DATA_PROBES", cbuf);
}
bool PanelC3::ParseScenarioLine (char *line)
{
	int nNum;
	if (!_strnicmp (line, "LPROBEARM", 9)) 
	{
		sscanf_s (line+9, "%d", &nNum);
		Air_Data_Stow[0]=nNum;
		return true;
	} 
	else if (!_strnicmp (line, "RPROBEARM", 9))
	{
		sscanf_s (line+9, "%d", &nNum);
		Air_Data_Stow[1]=nNum;
		return true;
	} 
	else if (!_strnicmp (line, "LPROBE", 6)) 
	{
		sscanf_s (line+6, "%d", &nNum);
		Air_Data_Probe[0]=nNum;
		return true;
	} 
	else if (!_strnicmp (line, "RPROBE", 6)) 
	{
		sscanf_s (line+6, "%d", &nNum);
		Air_Data_Probe[1]=nNum;
		return true;
	}
	else if (!_strnicmp (line, "LOMS", 4)) 
	{
		sscanf_s (line+4, "%d", &nNum);
		OMS_Eng[0]=nNum;
		return true;
	}
	else if (!_strnicmp (line, "ROMS", 4)) 
	{
		sscanf_s (line+4, "%d", &nNum);
		OMS_Eng[1]=nNum;
		return true;
	}
	else if (!_strnicmp (line, "AIR_DATA_PROBES", 15))
	{
		sscanf_s (line+15, "%lf%lf", &AirDataProbe[0].pos, &AirDataProbe[1].pos);
		AirDataProbeControl();
		return true;
	}
	return false;
}
