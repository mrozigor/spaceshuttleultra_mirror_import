#include "../Atlantis.h"
#include "MLP.h"
#include <cstdio>

MLP::MLP(OBJHANDLE hVessel, int iFlightModel)
: VESSEL2(hVessel, iFlightModel)
{
	mshMLP = oapiLoadMeshGlobal("SSU\\MLP-2");
	fT_SSSActive = 20.0;	//seconds of water

	fSRBSteam = 0.0;
	fSSMESteam = 0.0;

	bSSS_Active = false;
	bStartSequence = false;
	ahHDP = NULL;
	fCountdown = 8.0;

	dTimer=0.0;
	bPadLightsOn=false;
	vis=NULL;
}

MLP::~MLP()
{
}

bool MLP::clbkLoadVC(int id) {
	switch(id)
	{
	case 0:
		break;
	}
	return true;
}

void MLP::clbkSetClassCaps(FILEHANDLE cfg)
{
	static PARTICLESTREAMSPEC sss_steam = {
		0, 17, 20, 250.0, 0.3, 15, 6, 1.0, PARTICLESTREAMSPEC::DIFFUSE,
		PARTICLESTREAMSPEC::LVL_PSQRT, 0, 0.1,
		PARTICLESTREAMSPEC::ATM_PLOG, 1e-6, 1.0};
	sss_steam.tex = oapiRegisterParticleTexture("contrail4");

	SetSize(25.0);
	SetEmptyMass(4.1957075E6);
	msh_idx=AddMesh(mshMLP);
	DefineAnimations();

	SetTouchdownPoints(_V(0.0, -30.65, 25.0), _V(-25.0, -30.65, -25.0), _V(25.0, -30.65, -25.0)); //080522, DaveS edit: Corrected MLP height so it sits correctly on the MLP stands on the hardstand
	AddParticleStream(&sss_steam, POS_MPS_SMOKE, DIR_MPS_SMOKE, &fSSMESteam);
	AddParticleStream(&sss_steam, POS_MPS_SMOKE, _V(0.0, sin(10.0 * RAD), -cos(10.0 * RAD)), &fSSMESteam);
	AddParticleStream(&sss_steam, POS_SRB_SMOKE, DIR_SRB_SMOKE, &fSRBSteam);

	if(!ahHDP)
	{
		ahHDP = CreateAttachment(false, HDP_POS, _V(0.0, 1.0, 0.0), _V(0.0, 0.0, -1.0), "XHDP");
	}

	
}


void MLP::clbkSaveState(FILEHANDLE scn)
{
	SaveDefaultState(scn);
}

void MLP::clbkLoadStateEx(FILEHANDLE scn, void* vs)
{
	char* line;
	while(oapiReadScenario_nextline(scn, line))
	{
		ParseScenarioLineEx(line, vs);
	}
}

void MLP::clbkPreStep(double fSimT, double fDeltaT, double mjd)
{
	if(dTimer<=fSimT) {
		dTimer=fSimT+300.0;

		OBJHANDLE Sun=NULL;
		int count=(int)oapiGetGbodyCount();
		for(int i=0;i<count;i++) {
			Sun=oapiGetGbodyByIndex(i);
			if(oapiGetObjectType(Sun)==OBJTP_STAR) break;
		}
		if(Sun) {
			VECTOR3 SunPosGlobal, SunPos;
			oapiGetGlobalPos(Sun, &SunPosGlobal);
			Global2Local(SunPosGlobal, SunPos);
			double angle=acos(SunPos.y/length(SunPos))*DEG;
			if(angle>85.0 && !bPadLightsOn) TurnOnPadLights();
			else if(angle<85.0 && bPadLightsOn) TurnOffPadLights();
		}
	}

	if(bStartSequence)
	{
		fCountdown -= fDeltaT;
		if(fCountdown < 7.0 && !bSSS_Active)
		{
			bSSS_Active = true;
		}
		if(fCountdown < -30.0)
		{
			bStartSequence = false;
		}
	}
	if(bSSS_Active)
	{
		fT_SSSActive = max(fT_SSSActive - fDeltaT, 0);
		if(fT_SSSActive == 0.0)
		{
			bSSS_Active = false;
		}
		//Gather incoming exhaust energy
		//Use a general approach instead of only working for the SSU?

		if(oapiGetVesselCount() > 1)
		{
			CalculateSteamProduction(fSimT, fDeltaT);
		}
	}
	/*
	if(bStartSequence)
	{
		sprintf(oapiDebugString(), "MLP STATUS: COUNT: %+5.2f | SSS %3s Tw=%+5.2f Pmps=%f Psrb=%f", 
			fCountdown, bSSS_Active?"ON":"OFF", fT_SSSActive, fSSMESteam, fSRBSteam);
	} else {
		sprintf(oapiDebugString(), "MLP STATUS: SSS %3s Tw=%+5.2f Pmps=%f Psrb=%f", 
			bSSS_Active?"ON":"OFF", fT_SSSActive, fSSMESteam, fSRBSteam);
	}
	*/
}

void MLP::clbkVisualCreated(VISHANDLE _vis, int refcount)
{
	vis=_vis;
}

void MLP::clbkVisualDestroyed(VISHANDLE _vis, int refcount)
{
	if(vis==_vis) vis=NULL;
}

void MLP::OnT0() {
	//Trigger T0 animation
}

void MLP::TurnOnPadLights()
{
	MESHHANDLE mesh=GetMesh(vis, msh_idx);
	IlluminateMesh(mesh);
	Atlantis* sts=GetShuttleOnPad();
	if(sts) sts->TurnOnPadLights();
	bPadLightsOn=true;
}

void MLP::TurnOffPadLights()
{
	MESHHANDLE mesh=GetMesh(vis, msh_idx);
	DisableIllumination(mesh, mshMLP);
	Atlantis* sts=GetShuttleOnPad();
	if(sts) sts->TurnOffPadLights();
	bPadLightsOn=false;
}

Atlantis* MLP::GetShuttleOnPad()
{
	OBJHANDLE Handle=GetAttachmentStatus(ahHDP);
	if(Handle) {
		VESSEL* vessel=oapiGetVesselInterface(Handle);
		if(!strcmp(vessel->GetClassName(), "Atlantis") || !strcmp(vessel->GetClassName(), STD_CLASS_NAME)) {
			Atlantis* sts=(Atlantis*)vessel;
			return sts;
		}
	}
	return NULL;
}

DLLCLBK void InitModule(HINSTANCE hDLL)
{
}

DLLCLBK void ExitModule(HINSTANCE hDLL)
{
}

DLLCLBK VESSEL* ovcInit(OBJHANDLE hVessel, int iFlightModel)
{
	return new MLP(hVessel, iFlightModel);
}

DLLCLBK void ovcExit(VESSEL* pVessel)
{
	delete static_cast<MLP*>(pVessel);
}


void MLP::ActivateSSS()
{
	RecordEvent("MLPGSE", "ACTIVATE SSS");
	bSSS_Active = true;
}

void MLP::TriggerHDP()
{
	VECTOR3 ownRV;
	//Reset Vessel Status if needed
	VESSELSTATUS2 st;
	st.version = 2;
	st.flag = 0;
	VESSELSTATUS2 st2;
	st2.version = 2;
	st2.flag = 0;
	GetStatusEx(&st2);
	OBJHANDLE hShuttle = GetAttachmentStatus(ahHDP);

	
	DetachChild(ahHDP, 0.00001);

	if(hShuttle)
	{
		VESSEL* pV = oapiGetVesselInterface(hShuttle);
		pV->GetStatusEx(&st);
		st.rbody = st2.rbody;
		st.rvel = st2.rvel;
		pV->DefSetStateEx(&st);
	}
	RecordEvent("MLPGSE", "GPC TRIGGER HDP");

}

double MLP::CalculateThrustPower(
		const VECTOR3& vtx_lt,
		const VECTOR3& vtx_rt,
		const VECTOR3& vtx_lb,
		const VECTOR3& vtx_rb) {

	VECTOR3 F, D, DG, B, BG;
	VECTOR3 E1, E2, VP;
	VECTOR3 T, P, Q;
	VECTOR3 TUV;

	double fTotalPower = 0.0;

	

	for(DWORD c=0; c<oapiGetVesselCount(); c++) {
		OBJHANDLE hVessel = oapiGetVesselByIndex(c);
		if(hVessel == GetHandle()) continue;
		//Ignore all vessels further away as 1500m
		GetRelativePos(hVessel, VP);
		if(length(VP) > 1500.0) continue;
		
		VESSEL* pV = oapiGetVesselInterface (hVessel);

		
		
		for(DWORD t = 0; t < pV->GetThrusterCount(); t++)
		{
			THRUSTER_HANDLE thT = pV->GetThrusterHandleByIndex(t);
	
			//Only account powerful thrusters
			if(pV->GetThrusterMax(thT) > 5000.0)
			{
				double Power = pV->GetThrusterMax(thT) * pV->GetThrusterLevel(thT) * pV->GetThrusterIsp(thT);
				pV->GetThrusterDir(thT, D);
				pV->GlobalRot(D, DG);
				pV->GetThrusterRef(thT, B);
				pV->Local2Global(B, BG);
				DG = -DG;

				//First triangle
				E1 = vtx_rt - vtx_lt;
				E2 = vtx_lb - vtx_lt;
				T = BG - vtx_lt;
				P = crossp(DG , E2);
				Q = crossp(T, E1);
				TUV = _V(dotp(Q, E2), dotp(P, T), dotp(Q, DG)) * (1.0/dotp(P, E1));
				if(TUV.x > 0 && TUV.y>= 0.0 && TUV.y < 1.0 && TUV.z>=0 && TUV.z < 1.0)
				{
					//add exhaust energy too flame trench
					fTotalPower += Power;
				}
				

				//Second triangle
				E1 = vtx_lb - vtx_lt;
				E2 = vtx_rb - vtx_lt;
				T = BG - vtx_lt;
				P = crossp(DG , E2);
				Q = crossp(T, E1);
				TUV = _V(dotp(Q, E2), dotp(P, T), dotp(Q, DG)) * (1.0/dotp(P, E1));
				if(TUV.x > 0 && TUV.y>= 0.0 && TUV.y < 1.0 && TUV.z>=0 && TUV.z < 1.0)
				{
					//add exhaust energy too flame trench
					fTotalPower += Power;
				}
			}
		}
	}
	return fTotalPower;

}


void MLP::CalculateSteamProduction(double fSimT, double fDeltaT)
{
	VECTOR3 F, D, DG, B, BG;
	VECTOR3 M0, M1, M2, M3, MP1, MP2;  //MPS Hole
	VECTOR3 L0, L1, L2, L3, LP1, LP2;	//Left SRB Hole
	VECTOR3 R0, R1, R2, R3, RP1, RP2;	//Right SRB Hole
	VECTOR3 E1, E2, VP;
	VECTOR3 T, P, Q;
	VECTOR3 TUV;

	bool bHitSSME = false;
	bool bHitSRB = false;


	Local2Global(_V(-5, -0.8581532, -20), M0);
	Local2Global(_V(5, -0.8581532, -20), M1);
	Local2Global(_V(5, -0.8581532,  -10), M2);
	Local2Global(_V(-5, -0.8581532,  -10), M3);

	Local2Global(_V(-9.764171, -0.8581532, -7.124301), L0);
	Local2Global(_V(-3.252589, -0.8581532, -7.30518), L1);
	Local2Global(_V(-3.343028, -0.8581532,  3.999652), L2);
	Local2Global(_V(-9.854609, -0.8581532,  4.09009), L3);

	Local2Global(_V(9.764171, -0.8581532, -7.124301), R0);
	Local2Global(_V(3.252589, -0.8581532, -7.30518), R1);
	Local2Global(_V(3.343028, -0.8581532,  3.999652), R2);
	Local2Global(_V(9.854609, -0.8581532,  4.09009), R3);


	//for(DWORD c=0; c<oapiGetVesselCount(); c++)
	//{
	//	OBJHANDLE hVessel = oapiGetVesselByIndex(c);
	//	if(hVessel == GetHandle()) continue;
	//	//Ignore all vessels further away as 1500m
	//	GetRelativePos(hVessel, VP);
	//	if(length(VP) > 1500.0) continue;
	//	
	//	VESSEL* pV = oapiGetVesselInterface (hVessel);

	//	
	//	
	//	for(DWORD t = 0; t < pV->GetThrusterCount(); t++)
	//	{
	//		THRUSTER_HANDLE thT = pV->GetThrusterHandleByIndex(t);
	//		
	//		if(pV->GetThrusterMax(thT) > 5000.0)
	//		{
	//			double Power = pV->GetThrusterMax(thT) * pV->GetThrusterLevel(thT) * pV->GetThrusterIsp(thT);
	//			pV->GetThrusterDir(thT, D);
	//			pV->GlobalRot(D, DG);
	//			pV->GetThrusterRef(thT, B);
	//			pV->Local2Global(B, BG);
	//			DG = -DG;

	//			E1 = M1 - M0;
	//			E2 = M2 - M0;
	//			T = BG - M0;
	//			P = crossp(DG , E2);
	//			Q = crossp(T, E1);
	//			TUV = _V(dotp(Q, E2), dotp(P, T), dotp(Q, DG)) * (1.0/dotp(P, E1));
	//			if(TUV.x > 0 && TUV.y>= 0.0 && TUV.y < 1.0 && TUV.z>=0 && TUV.z < 1.0)
	//			{
	//				//add exhaust energy too flame trench
	//				if(Power > 2E6)
	//				{
	//					bHitSSME = true;
	//				}
	//			}
	//			
	//			E1 = M2 - M0;
	//			E2 = M3 - M0;
	//			T = BG - M0;
	//			P = crossp(DG , E2);
	//			Q = crossp(T, E1);
	//			TUV = _V(dotp(Q, E2), dotp(P, T), dotp(Q, DG)) * (1.0/dotp(P, E1));
	//			if(TUV.x > 0 && TUV.y>= 0.0 && TUV.y < 1.0 && TUV.z>=0 && TUV.z < 1.0)
	//			{
	//				//add exhaust energy too flame trench
	//				if(Power > 2E6)
	//				{
	//					bHitSSME = true;
	//				}
	//			}


	//		}
	//	}

	//	
	//}


	double fPowerM = CalculateThrustPower(M0, M1, M2, M3);
	double fPowerL = CalculateThrustPower(L0, L1, L2, L3);
	double fPowerR = CalculateThrustPower(R0, R1, R2, R3);

	if(fPowerM > 2.0E6 && bSSS_Active)
	{
		fSSMESteam = min(1.0, (fPowerM-2.0E6)/10E6);			
	} else
	{
		fSSMESteam = 0.0;
	}

	if(fPowerL + fPowerR > 2.0E6 && bSSS_Active)
	{
		fSRBSteam = min(1.0, (fPowerM-2.0E6)/10E6);			
	} else
	{
		fSRBSteam = 0.0;
	}
}

void MLP::DefineAnimations() {
}

void MLP::SignalGSEStart()
{
	RecordEvent("MLPGSE", "GPC START COMMAND");
	bStartSequence = true;
}
