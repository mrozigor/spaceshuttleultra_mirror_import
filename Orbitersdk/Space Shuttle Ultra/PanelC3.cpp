#include "PanelC3.h"
#include "resource.h"
#include "meshres.h"
#include "meshres_vc.h"
#include "DlgCtrl.h"
#include <stdio.h>
#include "AirDataProbeSystem.h"

extern GDIParams g_Param;
extern HELPCONTEXT g_hc;
extern char *ActionString[5];

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
}

bool PanelC3::VCRedrawEvent(int id, int event, SURFHANDLE surf)
{
	//sprintf(oapiDebugString(), "VCRedrawEvent");
	return false;
}

void PanelC3::RegisterVC()
{
	VECTOR3 ofs = sts->orbiter_ofs;
	//SURFHANDLE tkbk_tex = oapiGetTextureHandle (sts->hOrbiterVCMesh, 5);
	oapiVCRegisterArea (AID_C3, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN);

	//-.2732771, 1.76045, 14.35651
	// .2732771 1.69551  13.73551
	oapiVCSetAreaClickmode_Quadrilateral (AID_C3, _V(-0.2732771, 1.76045, 14.35651)+ofs, _V(0.2732771, 1.76045, 14.35651)+ofs, 
		_V(-0.2732771, 1.69551,  13.73551)+ofs, _V(0.2732771, 1.69551,  13.73551)+ofs);
	//oapiVCSetAreaClickmode_Quadrilateral (AID_C3, _V(-0.0804, 1.667, 14.52)+ofs, _V(-0.111, 1.667, 14.52)+ofs, _V(-0.0804, 1.667, 14.52)+ofs, _V(0.276, 1.618, 14.066)+ofs);
	//sprintf(oapiDebugString(), "PanelC3 Registered");
}

bool PanelC3::VCMouseEvent(int id, int event, VECTOR3 &p)
{
	if (id != AID_C3) return false;
	bool action = false;
	int eng=-1;
	//sprintf(oapiDebugString(), "id %d event %d p %f %f %f",id,event,p.x,p.y,p.z);
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
	if(action)
	{
		UpdateVC();
		if(eng>=0) EngControl(eng);
		else AirDataProbeControl();
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