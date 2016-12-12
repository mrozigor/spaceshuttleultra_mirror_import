#include "../Atlantis.h"
#include "../CommonDefs.h"
#include "MLP.h"
#include "meshres_MLP.h"
#include <cstdio>

MLP::MLP(OBJHANDLE hVessel, int iFlightModel)
: VESSEL2(hVessel, iFlightModel)
{
	mshMLP = oapiLoadMeshGlobal("SSU\\MLP-2");
	fT_SSSActive = 35.0;	//seconds of water, this is timed from a video of the May 7 2004 A2202 Flow Test at Pad A

	fSRBSteam = 0.0;
	fSSMESteam = 0.0;

	bSSS_Active = false;
	bStartSequence = false;
	ahHDP = NULL;
	ahBase = NULL;
	fCountdown = 31.0;

	dTimer=0.0;
	bPadLightsOn=false;
	vis=NULL;

	ROFILevel=0.0;
	ROFIStartTime=0.0;

	SSS_SSMELevel = 0;
	SSS_LSRBLevel = 0;
	SSS_RSRBLevel = 0;
	SSS_RainbirdsLevel = 0;
	SRBwaterbagvapor_lvl = 0;

	bSSS_on = false;
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
		0, 5, 50, 500.0, 0.3, 15, 7, 8, PARTICLESTREAMSPEC::DIFFUSE,
		PARTICLESTREAMSPEC::LVL_PSQRT, 0, 0.1,
		PARTICLESTREAMSPEC::ATM_PLOG, 1e-6, 1.0};
	sss_steam.tex = oapiRegisterParticleTexture("contrail4");

	static PARTICLESTREAMSPEC sss_steam_SRB = {
		0, 7, 200, 1000, 0.3, 15, 9, 100, PARTICLESTREAMSPEC::DIFFUSE,
		PARTICLESTREAMSPEC::LVL_PSQRT, 0, 0.1,
		PARTICLESTREAMSPEC::ATM_PLOG, 1e-6, 1.0};
	sss_steam_SRB.tex = oapiRegisterParticleTexture("contrail4");

	static PARTICLESTREAMSPEC ROFI_Stream = {
		0, 0.1, 300.0, 17.5, 0.1, 0.30, 0, 0.5, PARTICLESTREAMSPEC::EMISSIVE,
		PARTICLESTREAMSPEC::LVL_FLAT, 1, 1,
		PARTICLESTREAMSPEC::ATM_FLAT, 1, 1};
	ROFI_Stream.tex = oapiRegisterParticleTexture("contrail3");

	static PARTICLESTREAMSPEC sss_water_SSME = {
		0, 0.05, 15.0, 11.0, 0.1, 0.6, 1.5, 0.8, PARTICLESTREAMSPEC::EMISSIVE,
		PARTICLESTREAMSPEC::LVL_FLAT, 1, 1,
		PARTICLESTREAMSPEC::ATM_FLAT, 1, 1
	};

	static PARTICLESTREAMSPEC sss_water_SRB = {
		0, 0.5, 20.0, 11.0, 0.1, 0.7, 3, 0.8, PARTICLESTREAMSPEC::EMISSIVE,
		PARTICLESTREAMSPEC::LVL_FLAT, 1, 1,
		PARTICLESTREAMSPEC::ATM_FLAT, 1, 1
	};

	static PARTICLESTREAMSPEC sss_water_Rainbirds = {
		0, 0.5, 25.0, 20.0, 0.1, 0.5, 5.5, 2.5, PARTICLESTREAMSPEC::EMISSIVE,
		PARTICLESTREAMSPEC::LVL_FLAT, 1, 1,
		PARTICLESTREAMSPEC::ATM_FLAT, 1, 1
	};

	static PARTICLESTREAMSPEC SRBwaterbagvapor = {
		0, 4, 15.0, 0.5, 0.1, 1, 0.3, 1, PARTICLESTREAMSPEC::EMISSIVE,
		PARTICLESTREAMSPEC::LVL_FLAT, 1, 1,
		PARTICLESTREAMSPEC::ATM_FLAT, 1, 1
	};

	SetSize(25.0);
	SetEmptyMass(4.1957075E6);
	msh_idx=AddMesh(mshMLP);
	DefineAnimations();

	// touchdown points: -30.84 on y for pad, -16.25 for VAB; must set touchdown points to match pad height so shuttle stack has correct touchdown points at liftoff
	DWORD ntdvtx = 4;
	static TOUCHDOWNVTX tdvtx[4] = {
		{_V( 0.0, -30.84, 25.0 ), 1e8, 1e2, 5, 5},
		{_V( -25.0, -30.84, -25.0 ), 1e8, 1e2, 5, 5},
		{_V( 25.0, -30.84, -25.0 ), 1e8, 1e2, 5, 5},
		{_V( 0, 10, 0 ), 1e8, 1e2, 5}
	};
	SetTouchdownPoints( tdvtx, ntdvtx );
	AddParticleStream(&sss_steam, POS_MPS_SMOKE, DIR_MPS_SMOKE, &fSSMESteam);
	AddParticleStream(&sss_steam, POS_MPS_SMOKE, _V(0.0, sin(10.0 * RAD), -cos(10.0 * RAD)), &fSSMESteam);
	AddParticleStream(&sss_steam_SRB, POS_SRB_SMOKE, DIR_SRB_SMOKE, &fSRBSteam);

	AddParticleStream(&ROFI_Stream, FWD_LEFT_ROFI_POS, _V(1, 0, 0), &ROFILevel);
	AddParticleStream(&ROFI_Stream, FWD_RIGHT_ROFI_POS, _V(-1, 0, 0), &ROFILevel);
	AddParticleStream(&ROFI_Stream, AFT_LEFT_ROFI_POS, _V(1, 0, 0), &ROFILevel);
	AddParticleStream(&ROFI_Stream, AFT_RIGHT_ROFI_POS, _V(-1, 0, 0), &ROFILevel);
	AddParticleStream(&ROFI_Stream, AFT_LEFT_ROFI_POS, _V(1, 0, 0), &ROFILevel);
	AddParticleStream(&ROFI_Stream, AFT_RIGHT_ROFI_POS, _V(-1, 0, 0), &ROFILevel);

	// water SSME hole (east/west side)
	for (int i = 0; i < 49; i++)
	{
		AddParticleStream( &sss_water_SSME, _V( 4.8, -0.4, -11.5 - (i * 0.2) ), _V( -0.7071, -0.7071, 0 ), &SSS_SSMELevel );
		AddParticleStream( &sss_water_SSME, _V( -4.8, -0.4, -11.5 - (i * 0.2) ), _V( 0.7071, -0.7071, 0 ), &SSS_SSMELevel );
	}

	// water SSME hole (north/south side)
	for (int i = 0; i < 46; i++)
	{
		AddParticleStream( &sss_water_SSME, _V( -4.6 + (i * 0.2), -0.4, -11 ), _V( 0, -0.7071, -0.7071 ), &SSS_SSMELevel );
		AddParticleStream( &sss_water_SSME, _V( -4.6 + (i * 0.2), -0.4, -21.6 ), _V( 0, -0.7071, 0.7071 ), &SSS_SSMELevel );
	}

	// water SSME hole (TSM)
	for (int i = 0; i < 8; i++)
	{
		AddParticleStream( &sss_water_SSME, _V( 4.25 - (0.025 * i), 0.55, -13.7 - (i * 0.3) ), _V( -0.7071, -0.7071, 0 ), &SSS_SSMELevel );
		AddParticleStream( &sss_water_SSME, _V( -4.25 + (0.025 * i), 0.55, -13.7 - (i * 0.3) ), _V( 0.7071, -0.7071, 0 ), &SSS_SSMELevel );
	}
	
	// water LH SRB hole
	AddParticleStream( &sss_water_SRB, _V( -9, -3.8, -4.9 ), _V(  0.40825, -0.81650, -0.40825 ), &SSS_LSRBLevel );// NW
	AddParticleStream( &sss_water_SRB, _V( -9, -3.8, -7.8 ), _V( 0.40825, -0.81650, 0.40825 ), &SSS_LSRBLevel );// SW
	AddParticleStream( &sss_water_SRB, _V( -3.8, -3.8, -7.5 ), _V( -0.40825, -0.81650, 0.40825 ), &SSS_LSRBLevel );// SE
	AddParticleStream( &sss_water_SRB, _V( -3.8, -3.8, -4.9 ), _V( -0.40825, -0.81650, -0.40825 ), &SSS_LSRBLevel );// NE
	
	// water RH SRB hole
	AddParticleStream( &sss_water_SRB, _V( 9, -3.8, -4.9 ), _V( -0.40825, -0.81650, -0.40825 ), &SSS_RSRBLevel );// NE
	AddParticleStream( &sss_water_SRB, _V( 9, -3.8, -7.8 ), _V( -0.40825, -0.81650, 0.40825 ), &SSS_RSRBLevel );// SE
	AddParticleStream( &sss_water_SRB, _V( 3.8, -3.8, -7.5 ), _V( 0.40825, -0.81650, 0.40825 ), &SSS_RSRBLevel );// SW
	AddParticleStream( &sss_water_SRB, _V( 3.8, -3.8, -4.9 ), _V( 0.40825, -0.81650, -0.40825 ), &SSS_RSRBLevel );// NW

	// water rainbirds
	AddParticleStream( &sss_water_Rainbirds, _V( 0, 3, 20 ), _V( 0, 0, -1 ), &SSS_RainbirdsLevel );// N
	AddParticleStream( &sss_water_Rainbirds, _V( -13, 3, 15 ), _V( 0.7071, 0, -0.7071 ), &SSS_RainbirdsLevel );// NW
	AddParticleStream( &sss_water_Rainbirds, _V( -13, 2.7, 1 ), _V( 0.7071, 0, 0.7071 ), &SSS_RainbirdsLevel );// SW
	AddParticleStream( &sss_water_Rainbirds, _V( 0, 2.7, 1 ), _V( 0, 0, 1 ), &SSS_RainbirdsLevel );// S
	AddParticleStream( &sss_water_Rainbirds, _V( 12.6, 2.7, 1 ), _V( -0.7071, 0, 0.7071 ), &SSS_RainbirdsLevel );// SE
	AddParticleStream( &sss_water_Rainbirds, _V( 12.6, 3, 15 ), _V( -0.7071, 0, -0.7071 ), &SSS_RainbirdsLevel );// NE

	// SRB water bag vapor
	AddParticleStream( &SRBwaterbagvapor, _V( -6.5, 0, 0 ), _V( 0, 0, 1 ), &SRBwaterbagvapor_lvl );// LH
	AddParticleStream( &SRBwaterbagvapor, _V( 6.5, 0, 0 ), _V( 0, 0, 1 ), &SRBwaterbagvapor_lvl );// RH

	if(!ahHDP)	{
		ahHDP = CreateAttachment(false, HDP_POS, _V(0.0, 1.0, 0.0), _V(0.0, 0.0, -1.0), "XHDP");
	}

	if(!ahBase) {
		ahBase = CreateAttachment(true, _V(0, -8.35, 0.325), _V(0, 1, 0), _V(0, 0, 1), "XMLP");
	}
}

void MLP::clbkSaveState(FILEHANDLE scn)
{
	SaveDefaultState(scn);

	WriteScenario_state(scn, "T0_UMB", T0UmbilicalState);
}

void MLP::clbkLoadStateEx(FILEHANDLE scn, void* vs)
{
	char* line;
	while(oapiReadScenario_nextline(scn, line))
	{
		if(!_strnicmp(line, "T0_UMB", 6)) {
			sscan_state(line+6, T0UmbilicalState);
			SetAnimation(anim_t0umb, T0UmbilicalState.pos);
		}
		else ParseScenarioLineEx(line, vs);
	}
}

void MLP::clbkPreStep(double fSimT, double fDeltaT, double mjd)
{
	if(bStartSequence)
	{
		fCountdown -= fDeltaT;
		if(fCountdown < 7.0 && !bSSS_Active)
		{
			bSSS_Active = true;
		}

		if ((fCountdown <= 16.0) && (fCountdown >= -5)) bSSS_on = true;// HACK stop at T+5s
		else bSSS_on = false;

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

	if(ROFILevel>0.01 && (fSimT-ROFIStartTime)>10.0) ROFILevel=0.0;
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

	if (bSSS_on == true)// TODO control from LCC
	{
		SSS_SSMELevel = min( (14 - fCountdown) * 0.5, 1 );
		SSS_LSRBLevel = min( (11 - fCountdown) * 0.5, 1 );
		SSS_RSRBLevel = min( (12.5 - fCountdown) * 0.5, 1 );
		if (T0UmbilicalState.Closed() == false) SSS_RainbirdsLevel = min( -fCountdown, 1 );// HACK using T0UmbilicalState as indication of liftoff
		if (SRBwaterbagvapor_lvl > 0) SRBwaterbagvapor_lvl -= 0.05;
	}
	else
	{
		SSS_SSMELevel = 0;
		SSS_LSRBLevel = 0;
		SSS_RSRBLevel = 0;
		SSS_RainbirdsLevel = 0;
	}

	if(T0UmbilicalState.Moving()) {
		double dp=fDeltaT*TSM_UMBILICAL_RETRACT_SPEED;
		T0UmbilicalState.Move(dp);
		SetAnimation(anim_t0umb, T0UmbilicalState.pos);
	}
}

int MLP::clbkConsumeBufferedKey(DWORD key, bool down, char* keystate)
{
	/*if(down) {
		if(KEYMOD_CONTROL(keystate)) {
			switch(key) {
				//for debugging ROFIs
				case OAPI_KEY_X:
					ROFILevel=1.0-ROFILevel;
					ROFIStartTime=oapiGetSimTime()+1000.0; //let ROFIs rin for 1010.0 seconds
					return 1;
			}
		}
	}*/
	return 0;
}

void MLP::clbkVisualCreated(VISHANDLE _vis, int refcount)
{
	vis=_vis;
	
	if (!T0UmbilicalState.Closed())
	{
		GROUPEDITSPEC grpSpec;
		grpSpec.flags = GRPEDIT_SETUSERFLAG;
		grpSpec.UsrFlag = 0x00000002;
		DEVMESHHANDLE hDevMLP = GetDevMesh( vis, msh_idx );
		oapiEditMeshGroup( hDevMLP, GRP_LSRB_water_bag, &grpSpec );
		oapiEditMeshGroup( hDevMLP, GRP_LSRB_water_bag_water, &grpSpec );
		oapiEditMeshGroup( hDevMLP, GRP_RSRB_water_bag, &grpSpec );
		oapiEditMeshGroup( hDevMLP, GRP_RSRB_water_bag_water, &grpSpec );
	}
}

void MLP::clbkVisualDestroyed(VISHANDLE _vis, int refcount)
{
	if(vis==_vis) vis=NULL;
}

void MLP::OnT0() {
	oapiWriteLog("MLP::OnT0() called");

	TriggerHDP();

	//Trigger T0 animation
	T0UmbilicalState.action=AnimState::OPENING;

	// hide water bags
	GROUPEDITSPEC grpSpec;
	grpSpec.flags = GRPEDIT_SETUSERFLAG;
	grpSpec.UsrFlag = 0x00000002;
	DEVMESHHANDLE hDevMLP = GetDevMesh( vis, msh_idx );
	oapiEditMeshGroup( hDevMLP, GRP_LSRB_water_bag, &grpSpec );
	oapiEditMeshGroup( hDevMLP, GRP_LSRB_water_bag_water, &grpSpec );
	oapiEditMeshGroup( hDevMLP, GRP_RSRB_water_bag, &grpSpec );
	oapiEditMeshGroup( hDevMLP, GRP_RSRB_water_bag_water, &grpSpec );
	// vaporize water
	SRBwaterbagvapor_lvl = 1;
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
//	VECTOR3 ownRV;
	//Reset Vessel Status if needed
	OBJHANDLE hShuttle = GetAttachmentStatus(ahHDP);
	if(hShuttle) {
		VESSELSTATUS2 st;
		st.version = 2;
		st.flag = 0;
		VESSELSTATUS2 st2;
		st2.version = 2;
		st2.flag = 0;
		GetStatusEx(&st2);

		DetachChild(ahHDP, 0.00001);

		VESSEL* pV = oapiGetVesselInterface(hShuttle);
		pV->GetStatusEx(&st);
		st.rbody = st2.rbody;
		st.rvel = st2.rvel;
		pV->DefSetStateEx(&st);
		RecordEvent("MLPGSE", "GPC TRIGGER HDP");
	}
}

void MLP::TriggerROFIs()
{
	ROFILevel=1.0;
	ROFIStartTime=oapiGetSimTime();
}

double MLP::CalculateThrustPower(
		const VECTOR3& vtx_lt,
		const VECTOR3& vtx_rt,
		const VECTOR3& vtx_lb,
		const VECTOR3& vtx_rb) {

	VECTOR3 D, DG, B, BG;
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
	VECTOR3 M0, M1, M2, M3;  //MPS Hole
	VECTOR3 L0, L1, L2, L3;	//Left SRB Hole
	VECTOR3 R0, R1, R2, R3;	//Right SRB Hole

	//bool bHitSSME = false;
	//bool bHitSRB = false;


	Local2Global(_V(-5, -0.8581532, -20), M0);
	Local2Global(_V(5, -0.8581532, -20), M1);
	Local2Global(_V(5, -0.8581532,  -10), M2);
	Local2Global(_V(-5, -0.8581532,  -10), M3);

	Local2Global(_V(-9.764171, -3, -7.124301), L0);
	Local2Global(_V(-3.252589, -3, -7.30518), L1);
	Local2Global(_V(-3.343028, -3,  3.999652), L2);
	Local2Global(_V(-9.854609, -3,  4.09009), L3);

	Local2Global(_V(9.764171, -3, -7.124301), R0);
	Local2Global(_V(3.252589, -3, -7.30518), R1);
	Local2Global(_V(3.343028, -3,  3.999652), R2);
	Local2Global(_V(9.854609, -3,  4.09009), R3);


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

	if(fPowerM > 0.5E8 && bSSS_Active)
	{
		fSSMESteam = min(1.0, (fPowerM-0.5E8)/17E9);			
	} else
	{
		fSSMESteam = 0.0;
	}

	if(fPowerL + fPowerR > 0 && bSSS_Active)
	{
		fSRBSteam = min(1.0, (fPowerM-2.0E6)/10E6);			
	} else
	{
		fSRBSteam = 0.0;
	}
}

void MLP::DefineAnimations()
{
	T0UmbilicalState.Set(AnimState::CLOSED, 0.0);
	anim_t0umb=CreateAnimation(1.0);

	static UINT LeftT0UmbGrp[1] = {GRP_LH_T0_umbilicals};
	static MGROUP_ROTATE LeftT0Umb(msh_idx, LeftT0UmbGrp, 1,
		_V(-5.337, -0.167, -14.805), _V(-0.0871575, 0, 0.996195), (float)(17.0*RAD));
	AddAnimationComponent(anim_t0umb, 0, 0.5, &LeftT0Umb);

	static UINT RightT0UmbGrp[1] = {GRP_RH_T0_umbilicals};
	static MGROUP_ROTATE RightT0Umb(msh_idx, RightT0UmbGrp, 1,
		_V(5.337, -0.167, -14.805), _V(-0.0871575, 0, -0.996195), (float)(17.0*RAD));
	AddAnimationComponent(anim_t0umb, 0, 0.5, &RightT0Umb);

	static UINT LeftT0UmbCoverGrp[1] = {GRP_LH_TSM_bonnet};
	static MGROUP_ROTATE LeftT0UmbCover(msh_idx, LeftT0UmbCoverGrp, 1,
		_V(-6.267, 6.8, -14.216), _V(0.0871575, 0, -0.996195), (float)(90.0*RAD));
	AddAnimationComponent(anim_t0umb, 0.45, 1, &LeftT0UmbCover);

	static UINT RightT0UmbCoverGrp[1] = {GRP_RH_TSM_bonnet};
	static MGROUP_ROTATE RightT0UmbCover(msh_idx, RightT0UmbCoverGrp, 1,
		_V(6.267, 6.8, -14.216), _V(0.0871575, 0, 0.996195), (float)(90.0*RAD));
	AddAnimationComponent(anim_t0umb, 0.45, 1, &RightT0UmbCover);
}

void MLP::GLSAutoSeqStart()
{
	RecordEvent("MLPGSE", "GPC START COMMAND");
	bStartSequence = true;
	fCountdown = 31.0;
}
