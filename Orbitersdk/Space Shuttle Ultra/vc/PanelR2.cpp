#include "PanelR2.h"
#include "../Atlantis.h"
#include <UltraMath.h>
#include "../Atlantis_defs.h"
#include "..\CommonDefs.h"
#include "..\meshres_vc_r2.h"


namespace vc
{
	PanelR2::PanelR2(Atlantis* psts)
		: AtlantisPanel(psts, "R2")
	{

		hPanelMesh = oapiLoadMeshGlobal( DEFAULT_MESHNAME_PANELR2 );
		mesh_index = MESH_UNDEFINED;

		Add(pBlrN2Supply[0] = new StdSwitch2(psts, "Boiler1 N2 Supply"));
		Add(pBlrN2Supply[1] = new StdSwitch2(psts, "Boiler2 N2 Supply"));
		Add(pBlrN2Supply[2] = new StdSwitch2(psts, "Boiler3 N2 Supply"));

		Add(pBlrPower[0] = new StdSwitch2(psts, "Boiler1 Power"));
		Add(pBlrPower[1] = new StdSwitch2(psts, "Boiler2 Power"));
		Add(pBlrPower[2] = new StdSwitch2(psts, "Boiler3 Power"));

		Add(pBlrCntlrHtr[0] = new StdSwitch3(psts, "Boiler1 Cntlr/Htr"));
		Add(pBlrCntlrHtr[1] = new StdSwitch3(psts, "Boiler2 Cntlr/Htr"));
		Add(pBlrCntlrHtr[2] = new StdSwitch3(psts, "Boiler3 Cntlr/Htr"));

		Add(pAPUOperate[0] = new LockableLever3(psts, "APU1 Operate"));
		Add(pAPUOperate[1] = new LockableLever3(psts, "APU2 Operate"));
		Add(pAPUOperate[2] = new LockableLever3(psts, "APU3 Operate"));

		Add(pHydPumpPress[0] = new LockableLever2(psts, "Hyd Main Pump Press 1"));
		Add(pHydPumpPress[1] = new LockableLever2(psts, "Hyd Main Pump Press 2"));
		Add(pHydPumpPress[2] = new LockableLever2(psts, "Hyd Main Pump Press 3"));

		Add(pAPUCntlrPwr[0] = new LockableLever2(psts, "APU1 Cntlr Pwr"));
		Add(pAPUCntlrPwr[1] = new LockableLever2(psts, "APU2 Cntlr Pwr"));
		Add(pAPUCntlrPwr[2] = new LockableLever2(psts, "APU3 Cntlr Pwr"));

		Add(pAPUFuelTkVlv[0] = new LockableLever2(psts, "APU1 Fuel Tank Valve"));
		Add(pAPUFuelTkVlv[1] = new LockableLever2(psts, "APU2 Fuel Tank Valve"));
		Add(pAPUFuelTkVlv[2] = new LockableLever2(psts, "APU3 Fuel Tank Valve"));

		Add( pAPUReadyToStart[0] = new StandardTalkback2( psts, "APU1 Ready to Start" ) );
		Add( pAPUReadyToStart[1] = new StandardTalkback2( psts, "APU2 Ready to Start" ) );
		Add( pAPUReadyToStart[2] = new StandardTalkback2( psts, "APU3 Ready to Start" ) );

		Add(pCenterlineLatch = new StdSwitch2(psts, "ET Umb Centerline Latch"));
		Add(pLeftUmbDoor = new StdSwitch3(psts, "ET Umb Left Door"));
		Add(pLeftUmbDoorLatch = new StdSwitch3(psts, "ET Umb Left Door Latch"));
		Add(pRightUmbDoor = new StdSwitch3(psts, "ET Umb Right Door"));
		Add(pRightUmbDoorLatch = new StdSwitch3(psts, "ET Umb Right Door Latch"));

		Add( pETUmbLDoor = new StandardTalkback3( psts, "ET Umb Left Door Tb" ) );
		Add( pETUmbRDoor = new StandardTalkback3( psts, "ET Umb Right Door Tb" ) );
		Add( pETUmbDoorCLLatch = new StandardTalkback3( psts, "ET Umb Door CL Latch Tb" ) );
		Add( pETUmbLLatch = new StandardTalkback3( psts, "ET Umb Left Door Latch Tb" ) );
		Add( pETUmbRLatch = new StandardTalkback3( psts, "ET Umb Right Door Latch Tb" ) );

		Add(pMPSPower[0][0] = new StdSwitch2(psts, "MPS Pwr Left AC2"));
		Add(pMPSPower[0][1] = new StdSwitch2(psts, "MPS Pwr Ctr AC1"));
		Add(pMPSPower[0][2] = new StdSwitch2(psts, "MPS Pwr Right AC3"));
		Add(pMPSPower[1][0] = new StdSwitch2(psts, "MPS Pwr Left AC3"));
		Add(pMPSPower[1][1] = new StdSwitch2(psts, "MPS Pwr Ctr AC2"));
		Add(pMPSPower[1][2] = new StdSwitch2(psts, "MPS Pwr Right AC1"));

		Add(pMPSHeIsolA[0] = new StdSwitch3(psts, "MPS He Isol A Left"));
		Add(pMPSHeIsolA[1] = new StdSwitch3(psts, "MPS He Isol A Ctr"));
		Add(pMPSHeIsolA[2] = new StdSwitch3(psts, "MPS He Isol A Right"));
		Add(pMPSHeIsolB[0] = new StdSwitch3(psts, "MPS He Isol B Left"));
		Add(pMPSHeIsolB[1] = new StdSwitch3(psts, "MPS He Isol B Ctr"));
		Add(pMPSHeIsolB[2] = new StdSwitch3(psts, "MPS He Isol B Right"));

		Add( pMPSPRPLTDumpSequence = new StdSwitch3( psts, "MPS PRPLT Dump Sequence" ) );
		Add( pMPSPRPLTBackupLH2vlv = new StdSwitch3( psts, "MPS PRPLT Backup LH2 vlv" ) );

		Add( pPneuLEngHeXOVR = new StdSwitch3( psts, "Pneu L Eng He XOVR" ) );
		Add( pPneuHeIsol = new StdSwitch3( psts, "Pneu He Isol" ) );

		Add( pHeInterconnect[0] = new StdSwitch3( psts, "He Interconnect Left" ) );
		Add( pHeInterconnect[1] = new StdSwitch3( psts, "He Interconnect Ctr" ) );
		Add( pHeInterconnect[2] = new StdSwitch3( psts, "He Interconnect Right" ) );

		Add( pLH2UllagePress = new LockableLever2( psts, "LH2 Ullage Press" ) );

		for(int i=0;i<3;i++) {
			pAPUOperate[i]->SetLabel(0, "INJECTOR COOL");
			pAPUOperate[i]->SetLabel(1, "OFF");
			pAPUOperate[i]->SetLabel(2, "START/RUN");
			pHydPumpPress[i]->SetLabel(0, "LOW");
			pHydPumpPress[i]->SetLabel(1, "NORM");
			pAPUCntlrPwr[i]->SetLabel(0, "OFF");
			pAPUCntlrPwr[i]->SetLabel(1, "ON");
			pAPUFuelTkVlv[i]->SetLabel(0, "CLOSE");
			pAPUFuelTkVlv[i]->SetLabel(1, "OPEN");

			pMPSHeIsolA[i]->SetLabel(0, "CLOSE");
			pMPSHeIsolA[i]->SetLabel(1, "GPC");
			pMPSHeIsolA[i]->SetLabel(2, "OPEN");
			pMPSHeIsolB[i]->SetLabel(0, "CLOSE");
			pMPSHeIsolB[i]->SetLabel(1, "GPC");
			pMPSHeIsolB[i]->SetLabel(2, "OPEN");

			pBlrCntlrHtr[i]->SetLabel(2, "A");
			pBlrCntlrHtr[i]->SetLabel(1, "OFF");
			pBlrCntlrHtr[i]->SetLabel(0, "B");
			pBlrPower[i]->SetLabel(1, "ON");
			pBlrPower[i]->SetLabel(0, "OFF");
			pBlrN2Supply[i]->SetLabel(1, "ON");
			pBlrN2Supply[i]->SetLabel(0, "OFF");

			pHeInterconnect[i]->SetLabel(0, "OUT OPEN");
			pHeInterconnect[i]->SetLabel(1, "GPC");
			pHeInterconnect[i]->SetLabel(2, "IN OPEN");
		}

		pCenterlineLatch->SetLabel(0, "STOW");
		pCenterlineLatch->SetLabel(1, "GND");
		pCenterlineLatch->SetInitialPosition(1);

		pLeftUmbDoor->SetLabel(0, "CLOSE");
		pLeftUmbDoor->SetLabel(1, "OFF");
		pLeftUmbDoor->SetLabel(2, "OPEN");
		pLeftUmbDoorLatch->SetLabel(0, "LATCH");
		pLeftUmbDoorLatch->SetLabel(1, "OFF");
		pLeftUmbDoorLatch->SetLabel(2, "RELEASE");

		pRightUmbDoor->SetLabel(0, "CLOSE");
		pRightUmbDoor->SetLabel(1, "OFF");
		pRightUmbDoor->SetLabel(2, "OPEN");
		pRightUmbDoorLatch->SetLabel(0, "LATCH");
		pRightUmbDoorLatch->SetLabel(1, "OFF");
		pRightUmbDoorLatch->SetLabel(2, "RELEASE");

		pMPSPRPLTDumpSequence->SetLabel( 0, "STOP" );
		pMPSPRPLTDumpSequence->SetLabel( 1, "GPC" );
		pMPSPRPLTDumpSequence->SetLabel( 2, "START" );

		pMPSPRPLTBackupLH2vlv->SetLabel( 0, "CLOSE" );
		pMPSPRPLTBackupLH2vlv->SetLabel( 1, "GPC" );
		pMPSPRPLTBackupLH2vlv->SetLabel( 2, "OPEN" );

		pPneuLEngHeXOVR->SetLabel( 0, "CLOSE" );
		pPneuLEngHeXOVR->SetLabel( 1, "GPC" );
		pPneuLEngHeXOVR->SetLabel( 2, "OPEN" );

		pPneuHeIsol->SetLabel( 0, "CLOSE" );
		pPneuHeIsol->SetLabel( 1, "GPC" );
		pPneuHeIsol->SetLabel( 2, "OPEN" );

		pLH2UllagePress->SetLabel( 0, "OPEN" );
		pLH2UllagePress->SetLabel( 1, "AUTO" );
	}

	PanelR2::~PanelR2()
	{
	}

	void PanelR2::AddMeshes( const VECTOR3 &ofs )
	{
		SetHasOwnVCMesh();

		if (mesh_index == MESH_UNDEFINED)
		{
			mesh_index = STS()->AddMesh( hPanelMesh, &ofs );
			STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		}
		return;
	}

	void PanelR2::SetMeshVisibility( bool visible )
	{
		if (visible) STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		else STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_NEVER );
		return;
	}

	UINT PanelR2::GetVCMeshIndex( void ) const
	{
		return mesh_index;
	}

	void PanelR2::RegisterVC()
	{
		oapiWriteLog("Registering PanelR2");
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea (AID_R2, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBPRESSED | PANEL_MOUSE_LBUP);
		oapiVCSetAreaClickmode_Quadrilateral (AID_R2, _V(0.9524, 1.8417, 14.6602)+ofs, _V(1.4477, 1.9868, 14.6602)+ofs,
			_V(.9524, 1.7286, 13.8048)+ofs, _V(1.4477, 1.8755, 13.8048)+ofs);
		
		return;
	}

	void PanelR2::DefineVC()
	{
		const static VECTOR3 switch_rot = _V(0.95, 0.31225, 0.0);
		const static VECTOR3 switch_pull = Normalize(crossp(_V(0, 0, 1), switch_rot));

		AddAIDToMouseEventList(AID_R2);

		/**** APU 1 ****/
		pAPUOperate[0]->SetMouseRegion(0.347421f, 0.382021f, 0.412748f, 0.424506f);
		pAPUOperate[0]->SetReference(_V(1.1416, 1.8473, 14.307), switch_rot);
		pAPUOperate[0]->SetPullDirection(switch_pull);
		pAPUOperate[0]->DefineSwitchGroup( GRP_S16_R2_VC );
		pAPUOperate[0]->SetInitialAnimState(0.5f);

		pHydPumpPress[0]->SetMouseRegion(0.364955f, 0.534468f, 0.454298f, 0.568684f);
		pHydPumpPress[0]->SetReference(_V(1.1496, 1.8333, 14.184), switch_rot);
		pHydPumpPress[0]->SetPullDirection(switch_pull);
		pHydPumpPress[0]->DefineSwitchGroup( GRP_S26_R2_VC );
		pHydPumpPress[0]->SetInitialAnimState(0.5f);

		pAPUCntlrPwr[0]->SetMouseRegion(0.495306f, 0.606395f, 0.553809f, 0.649438f);
		pAPUCntlrPwr[0]->SetReference(_V(1.2106, 1.8433, 14.122), switch_rot);
		pAPUCntlrPwr[0]->SetPullDirection(switch_pull);
		pAPUCntlrPwr[0]->DefineSwitchGroup( GRP_S32_R2_VC );
		pAPUCntlrPwr[0]->SetInitialAnimState(0.5f);

		pAPUFuelTkVlv[0]->SetMouseRegion(0.226394f, 0.675783f, 0.271294f, 0.707931f);
		pAPUFuelTkVlv[0]->SetReference(_V(1.0712, 1.7933, 14.0694), switch_rot);
		pAPUFuelTkVlv[0]->SetPullDirection(switch_pull);
		pAPUFuelTkVlv[0]->DefineSwitchGroup( GRP_S35_R2_VC );
		pAPUFuelTkVlv[0]->SetInitialAnimState(0.5f);

		/**** APU 2 ****/
		pAPUOperate[1]->SetMouseRegion(0.412748f, 0.382021f, 0.472092f, 0.424506f);
		pAPUOperate[1]->SetReference(_V(1.1706, 1.8563, 14.306), switch_rot);
		pAPUOperate[1]->SetPullDirection(switch_pull);
		pAPUOperate[1]->DefineSwitchGroup( GRP_S17_R2_VC );
		pAPUOperate[1]->SetInitialAnimState(0.5f);

		pHydPumpPress[1]->SetMouseRegion(0.454298f, 0.534468f, 0.549506f, 0.568684f);
		pHydPumpPress[1]->SetReference(_V(1.1986, 1.8473, 14.182), switch_rot);
		pHydPumpPress[1]->SetPullDirection(switch_pull);
		pHydPumpPress[1]->DefineSwitchGroup( GRP_S27_R2_VC );
		pHydPumpPress[1]->SetInitialAnimState(0.5f);

		pAPUCntlrPwr[1]->SetMouseRegion(0.553809f, 0.606395f, 0.617025f, 0.649438f);
		pAPUCntlrPwr[1]->SetReference(_V(1.2416, 1.8523, 14.121), switch_rot);
		pAPUCntlrPwr[1]->SetPullDirection(switch_pull);
		pAPUCntlrPwr[1]->DefineSwitchGroup( GRP_S33_R2_VC );
		pAPUCntlrPwr[1]->SetInitialAnimState(0.5f);

		pAPUFuelTkVlv[1]->SetMouseRegion(0.318000f, 0.677248f, 0.363841f, 0.707553f);
		pAPUFuelTkVlv[1]->SetReference(_V(1.1164, 1.8064, 14.0684), switch_rot);
		pAPUFuelTkVlv[1]->SetPullDirection(switch_pull);
		pAPUFuelTkVlv[1]->DefineSwitchGroup( GRP_S36_R2_VC );
		pAPUFuelTkVlv[1]->SetInitialAnimState(0.5f);

		/**** APU 3 ****/
		pAPUOperate[2]->SetMouseRegion(0.472092f, 0.382021f, 0.535220f, 0.424506f);
		pAPUOperate[2]->SetReference(_V(1.2006, 1.8643, 14.305), switch_rot);
		pAPUOperate[2]->SetPullDirection(switch_pull);
		pAPUOperate[2]->DefineSwitchGroup( GRP_S18_R2_VC );
		pAPUOperate[2]->SetInitialAnimState(0.5f);

		pHydPumpPress[2]->SetMouseRegion(0.549506f, 0.534468f, 0.625087f, 0.568684f);
		pHydPumpPress[2]->SetReference(_V(1.2446, 1.8613, 14.181), switch_rot);
		pHydPumpPress[2]->SetPullDirection(switch_pull);
		pHydPumpPress[2]->DefineSwitchGroup( GRP_S28_R2_VC );
		pHydPumpPress[2]->SetInitialAnimState(0.5f);

		pAPUCntlrPwr[2]->SetMouseRegion(0.617025f, 0.606395f, 0.682484f, 0.649438f);
		pAPUCntlrPwr[2]->SetReference(_V(1.2726, 1.8613, 14.12), switch_rot);
		pAPUCntlrPwr[2]->SetPullDirection(switch_pull);
		pAPUCntlrPwr[2]->DefineSwitchGroup( GRP_S34_R2_VC );
		pAPUCntlrPwr[2]->SetInitialAnimState(0.5f);

		pAPUFuelTkVlv[2]->SetMouseRegion(0.406730f, 0.679643f, 0.451296f, 0.710359f);
		pAPUFuelTkVlv[2]->SetReference(_V(1.1611, 1.8193, 14.0668), switch_rot);
		pAPUFuelTkVlv[2]->SetPullDirection(switch_pull);
		pAPUFuelTkVlv[2]->DefineSwitchGroup( GRP_S37_R2_VC );
		pAPUFuelTkVlv[2]->SetInitialAnimState(0.5f);

		pAPUReadyToStart[0]->DefineMeshGroups( mesh_index, GRP_DS1_U_R2_VC, GRP_DS1_L_R2_VC );
		pAPUReadyToStart[1]->DefineMeshGroups( mesh_index, GRP_DS2_U_R2_VC, GRP_DS2_L_R2_VC );
		pAPUReadyToStart[2]->DefineMeshGroups( mesh_index, GRP_DS3_U_R2_VC, GRP_DS3_L_R2_VC );

		/**** ET Umbilical Doors ****/
		pCenterlineLatch->SetMouseRegion(0.193928f, 0.884566f, 0.253453f, 0.916514f);
		pCenterlineLatch->SetReference(_V(1.062, 1.773, 13.888), switch_rot);
		pCenterlineLatch->DefineSwitchGroup( GRP_S48_R2_VC );
		pCenterlineLatch->SetInitialAnimState(0.5f);
		pCenterlineLatch->SetOnPosition(0);

		pLeftUmbDoor->SetMouseRegion(0.292763f, 0.885751f, 0.351700f, 0.920420f);
		pLeftUmbDoor->SetReference(_V(1.11, 1.787, 13.885), switch_rot);
		pLeftUmbDoor->DefineSwitchGroup( GRP_S49_R2_VC );
		pLeftUmbDoor->SetInitialAnimState(0.5f);

		pLeftUmbDoorLatch->SetMouseRegion(0.375219f, 0.887672f, 0.433632f, 0.922404f);
		pLeftUmbDoorLatch->SetReference(_V(1.151, 1.799, 13.884), switch_rot);
		pLeftUmbDoorLatch->DefineSwitchGroup( GRP_S50_R2_VC );
		pLeftUmbDoorLatch->SetInitialAnimState(0.5f);

		pRightUmbDoor->SetMouseRegion(0.455335f, 0.889641f, 0.513106f, 0.923580f);
		pRightUmbDoor->SetReference(_V(1.19, 1.811, 13.883), switch_rot);
		pRightUmbDoor->DefineSwitchGroup( GRP_S51_R2_VC );
		pRightUmbDoor->SetInitialAnimState(0.5f);

		pRightUmbDoorLatch->SetMouseRegion(0.53287f, 0.890511f, 0.590665f, 0.925481f);
		pRightUmbDoorLatch->SetReference(_V(1.228, 1.822, 13.881), switch_rot);
		pRightUmbDoorLatch->DefineSwitchGroup( GRP_S52_R2_VC );
		pRightUmbDoorLatch->SetInitialAnimState(0.5f);

		pETUmbDoorCLLatch->DefineMeshGroup( mesh_index, GRP_DS4_R2_VC );

		pETUmbLDoor->DefineMeshGroup( mesh_index, GRP_DS5_R2_VC );

		pETUmbLLatch->DefineMeshGroup( mesh_index, GRP_DS6_R2_VC );

		pETUmbRDoor->DefineMeshGroup( mesh_index, GRP_DS7_R2_VC );

		pETUmbRLatch->DefineMeshGroup( mesh_index, GRP_DS8_R2_VC );

		/**** MPS He Isol A ****/
		pMPSHeIsolA[0]->SetMouseRegion(0.081538f, 0.322382f, 0.157321f, 0.352672f);
		pMPSHeIsolA[0]->SetReference(_V(1.008, 1.822, 14.372), switch_rot);
		pMPSHeIsolA[0]->DefineSwitchGroup( GRP_S56_R2_VC );
		pMPSHeIsolA[0]->SetInitialAnimState(0.5f);

		pMPSHeIsolA[1]->SetMouseRegion(0.157321f, 0.323196f, 0.239897f, 0.354704f);
		pMPSHeIsolA[1]->SetReference(_V(1.049, 1.834, 14.371), switch_rot);
		pMPSHeIsolA[1]->DefineSwitchGroup( GRP_S55_R2_VC );
		pMPSHeIsolA[1]->SetInitialAnimState(0.5f);

		pMPSHeIsolA[2]->SetMouseRegion(0.239897f, 0.323704f, 0.312041f, 0.354271f);
		pMPSHeIsolA[2]->SetReference(_V(1.089, 1.845, 14.37), switch_rot);
		pMPSHeIsolA[2]->DefineSwitchGroup( GRP_S57_R2_VC );
		pMPSHeIsolA[2]->SetInitialAnimState(0.5f);

		/**** MPS He Isol B ****/
		pMPSHeIsolB[0]->SetMouseRegion(0.085931f, 0.391285f, 0.159351f, 0.422357f);
		pMPSHeIsolB[0]->SetReference(_V(1.008, 1.814, 14.313), switch_rot);
		pMPSHeIsolB[0]->DefineSwitchGroup( GRP_S13_R2_VC );
		pMPSHeIsolB[0]->SetInitialAnimState(0.5f);

		pMPSHeIsolB[1]->SetMouseRegion(0.159351f, 0.389469f, 0.238773f, 0.424157f);
		pMPSHeIsolB[1]->SetReference(_V(1.048, 1.825, 14.312), switch_rot);
		pMPSHeIsolB[1]->DefineSwitchGroup( GRP_S12_R2_VC );
		pMPSHeIsolB[1]->SetInitialAnimState(0.5f);

		pMPSHeIsolB[2]->SetMouseRegion(0.238773f, 0.390728f, 0.310743f, 0.425303f);
		pMPSHeIsolB[2]->SetReference(_V(1.088, 1.837, 14.31), switch_rot);
		pMPSHeIsolB[2]->DefineSwitchGroup( GRP_S14_R2_VC );
		pMPSHeIsolB[2]->SetInitialAnimState(0.5f);

		/**** MPS Engine Power ****/
		pMPSPower[0][0]->SetMouseRegion(0.091011f, 0.177120f, 0.159388f, 0.207124f);
		pMPSPower[0][0]->SetReference(_V(1.009, 1.838, 14.495), switch_rot);
		pMPSPower[0][0]->DefineSwitchGroup( GRP_S4_R2_VC );
		pMPSPower[0][0]->SetInitialAnimState(0.5f);

		pMPSPower[0][1]->SetMouseRegion(0.159388f, 0.178765f, 0.239655f, 0.208946f);
		pMPSPower[0][1]->SetReference(_V(1.05, 1.85, 14.494), switch_rot);
		pMPSPower[0][1]->DefineSwitchGroup( GRP_S3_R2_VC );
		pMPSPower[0][1]->SetInitialAnimState(0.5f);

		pMPSPower[0][2]->SetMouseRegion(0.239655f, 0.179134f, 0.313464f, 0.208574f);
		pMPSPower[0][2]->SetReference(_V(1.088, 1.861, 14.493), switch_rot);
		pMPSPower[0][2]->DefineSwitchGroup( GRP_S5_R2_VC );
		pMPSPower[0][2]->SetInitialAnimState(0.5f);

		pMPSPower[1][0]->SetMouseRegion(0.089520f, 0.244445f, 0.159198f, 0.272745f);
		pMPSPower[1][0]->SetReference(_V(1.008, 1.83, 14.439), switch_rot);
		pMPSPower[1][0]->DefineSwitchGroup( GRP_S7_R2_VC );
		pMPSPower[1][0]->SetInitialAnimState(0.5f);

		pMPSPower[1][1]->SetMouseRegion(0.159198f, 0.246158f, 0.240562f, 0.276445f);
		pMPSPower[1][1]->SetReference(_V(1.05, 1.843, 14.437), switch_rot);
		pMPSPower[1][1]->DefineSwitchGroup( GRP_S6_R2_VC );
		pMPSPower[1][1]->SetInitialAnimState(0.5f);

		pMPSPower[1][2]->SetMouseRegion(0.240562f, 0.247841f, 0.316475f, 0.276847f);
		pMPSPower[1][2]->SetReference(_V(1.091, 1.855, 14.435), switch_rot);
		pMPSPower[1][2]->DefineSwitchGroup( GRP_S8_R2_VC );
		pMPSPower[1][2]->SetInitialAnimState(0.5f);

		pMPSPRPLTDumpSequence->SetMouseRegion( 0.089324f, 0.098690f, 0.150280f, 0.130199f );
		pMPSPRPLTDumpSequence->SetReference( _V( 1.0107, 1.8482, 14.5611 ), switch_rot );
		pMPSPRPLTDumpSequence->DefineSwitchGroup( GRP_S1_R2_VC );
		pMPSPRPLTDumpSequence->SetInitialAnimState( 0.5f );

		pMPSPRPLTBackupLH2vlv->SetMouseRegion( 0.163286f, 0.101039f, 0.222317f, 0.132316f );
		pMPSPRPLTBackupLH2vlv->SetReference( _V( 1.0475, 1.859, 14.5595 ), switch_rot );
		pMPSPRPLTBackupLH2vlv->DefineSwitchGroup( GRP_S2_R2_VC );
		pMPSPRPLTBackupLH2vlv->SetInitialAnimState( 0.5f );

		pPneuLEngHeXOVR->SetMouseRegion( 0.083889f, 0.460774f, 0.151326f, 0.493669f );
		pPneuLEngHeXOVR->SetReference( _V( 1.009, 1.8065, 14.2516 ), switch_rot );
		pPneuLEngHeXOVR->DefineSwitchGroup( GRP_S54_R2_VC );
		pPneuLEngHeXOVR->SetInitialAnimState( 0.5f );

		pPneuHeIsol->SetMouseRegion( 0.183902f, 0.463183f, 0.250303f, 0.495449f );
		pPneuHeIsol->SetReference( _V( 1.058, 1.821, 14.2497 ), switch_rot );
		pPneuHeIsol->DefineSwitchGroup( GRP_S15_R2_VC );
		pPneuHeIsol->SetInitialAnimState( 0.5f );

		pHeInterconnect[0]->SetMouseRegion( 0.086966f, 0.535918f, 0.150105f, 0.566381f );
		pHeInterconnect[0]->SetReference( _V( 1.0114, 1.7986, 14.187 ), switch_rot );
		pHeInterconnect[0]->DefineSwitchGroup( GRP_S10_R2_VC );
		pHeInterconnect[0]->SetInitialAnimState( 0.5f );

		pHeInterconnect[1]->SetMouseRegion( 0.190284f, 0.539667f, 0.237936f, 0.567023f );
		pHeInterconnect[1]->SetReference( _V( 1.0574, 1.8125, 14.1854 ), switch_rot );
		pHeInterconnect[1]->DefineSwitchGroup( GRP_S9_R2_VC );
		pHeInterconnect[1]->SetInitialAnimState( 0.5f );

		pHeInterconnect[2]->SetMouseRegion( 0.271724f, 0.542303f, 0.316761f, 0.568987f );
		pHeInterconnect[2]->SetReference( _V( 1.0967, 1.8236, 14.1845 ), switch_rot );
		pHeInterconnect[2]->DefineSwitchGroup( GRP_S11_R2_VC );
		pHeInterconnect[2]->SetInitialAnimState( 0.5f );

		pLH2UllagePress->SetMouseRegion( 0.285254f, 0.465080f, 0.334198f, 0.496738f );
		pLH2UllagePress->SetReference( _V( 1.1025, 1.8270, 14.2502 ), switch_rot );
		pLH2UllagePress->SetPullDirection( switch_pull );
		pLH2UllagePress->DefineSwitchGroup( GRP_S53_R2_VC );
		pLH2UllagePress->SetInitialAnimState( 0.5f );

		/**** WSB 1 ****/
		pBlrCntlrHtr[0]->SetMouseRegion(0.547575f, 0.754219f, 0.625464f, 0.788508f);
		pBlrCntlrHtr[0]->SetReference(_V(1.237, 1.84, 13.998), switch_rot);
		pBlrCntlrHtr[0]->DefineSwitchGroup( GRP_S41_R2_VC );
		pBlrCntlrHtr[0]->SetInitialAnimState(0.5f);

		pBlrPower[0]->SetMouseRegion(0.633620f, 0.821740f, 0.697079f, 0.862008f);
		pBlrPower[0]->SetReference(_V(1.281, 1.845, 13.938), switch_rot);
		pBlrPower[0]->DefineSwitchGroup( GRP_S38_R2_VC );
		pBlrPower[0]->SetInitialAnimState(0.5f);

		pBlrN2Supply[0]->SetMouseRegion(0.671214f, 0.890886f, 0.741418f, 0.934839f);
		pBlrN2Supply[0]->SetReference(_V(1.303, 1.844, 13.88), switch_rot);
		pBlrN2Supply[0]->DefineSwitchGroup( GRP_S44_R2_VC );
		pBlrN2Supply[0]->SetInitialAnimState(0.5f);

		/**** WSB 2 ****/
		pBlrCntlrHtr[1]->SetMouseRegion(0.625464f, 0.754219f, 0.693698f, 0.788508f);
		pBlrCntlrHtr[1]->SetReference(_V(1.276, 1.851, 13.997), switch_rot);
		pBlrCntlrHtr[1]->DefineSwitchGroup( GRP_S42_R2_VC );
		pBlrCntlrHtr[1]->SetInitialAnimState(0.5f);

		pBlrPower[1]->SetMouseRegion(0.697079f, 0.821740f, 0.755268f, 0.862008f);
		pBlrPower[1]->SetReference(_V(1.308, 1.853, 13.937), switch_rot);
		pBlrPower[1]->DefineSwitchGroup( GRP_S39_R2_VC );
		pBlrPower[1]->SetInitialAnimState(0.5f);

		pBlrN2Supply[1]->SetMouseRegion(0.741418f, 0.890886f, 0.798522f, 0.934839f);
		pBlrN2Supply[1]->SetReference(_V(1.33, 1.851, 13.878), switch_rot);
		pBlrN2Supply[1]->DefineSwitchGroup( GRP_S45_R2_VC );
		pBlrN2Supply[1]->SetInitialAnimState(0.5f);

		/**** WSB 3 ****/
		pBlrCntlrHtr[2]->SetMouseRegion(0.693698f, 0.754219f, 0.769339f, 0.788508f);
		pBlrCntlrHtr[2]->SetReference(_V(1.314, 1.862, 13.997), switch_rot);
		pBlrCntlrHtr[2]->DefineSwitchGroup( GRP_S43_R2_VC );
		pBlrCntlrHtr[2]->SetInitialAnimState(0.5f);

		pBlrPower[2]->SetMouseRegion(0.755268f, 0.821740f, 0.816670f, 0.862008f);
		pBlrPower[2]->SetReference(_V(1.337, 1.861, 13.936), switch_rot);
		pBlrPower[2]->DefineSwitchGroup( GRP_S40_R2_VC );
		pBlrPower[2]->SetInitialAnimState(0.5f);

		pBlrN2Supply[2]->SetMouseRegion(0.798522f, 0.890886f, 0.875479f, 0.934839f);
		pBlrN2Supply[2]->SetReference(_V(1.358, 1.86, 13.877), switch_rot);
		pBlrN2Supply[2]->DefineSwitchGroup( GRP_S46_R2_VC );
		pBlrN2Supply[2]->SetInitialAnimState(0.5f);
	}

	void PanelR2::DefineVCAnimations( UINT vcidx )
	{
		AtlantisPanel::DefineVCAnimations( mesh_index );
		return;
	}

	void PanelR2::Realize()
	{
		char cbuf[255];

		for(int i=0;i<3;i++) {
			sprintf_s(cbuf, 255, "R2_To_APU%d", i+1);
			DiscreteBundle *pBundle=STS()->BundleManager()->CreateBundle(cbuf, 16);
			
			/*APU_Run[i].Connect(pBundle, 0);
			APU_CntlrPwr[i].Connect(pBundle, 1);
			APU_FuelTankValves[i].Connect(pBundle, 2);
			APU_HydPumpPress[i].Connect(pBundle, 3);
			APU_HydraulicPressure[i].Connect(pBundle, 4);
			APU_ReadyToStart[i].Connect(pBundle, 5);*/
			pAPUOperate[i]->ConnectPort(1, pBundle, 0);
			pAPUOperate[i]->ConnectPort(2, pBundle, 0); // temporary; THIS IS NOT CORRECT
			pAPUCntlrPwr[i]->ConnectPort(1, pBundle, 1);
			pAPUFuelTkVlv[i]->ConnectPort(1, pBundle, 2);
			pHydPumpPress[i]->ConnectPort(1, pBundle, 3);
			APU_HydraulicPress[i].Connect(pBundle, 4);
			pAPUReadyToStart[i]->SetInput( pBundle, 5, TB_GRAY );

			sprintf_s(cbuf, 255, "WSB%d", i+1);
			pBundle=STS()->BundleManager()->CreateBundle(cbuf, 16);
			pBlrPower[i]->output.Connect(pBundle, 1);
			pBlrCntlrHtr[i]->outputA.Connect(pBundle, 2);
			pBlrCntlrHtr[i]->outputB.Connect(pBundle, 3);
			pBlrN2Supply[i]->output.Connect(pBundle, 4);
		}

		DiscreteBundle* pBundle=STS()->BundleManager()->CreateBundle("ETUmbDoors", 16);
		pLeftUmbDoor->outputA.Connect(pBundle, 0); // close door
		pLeftUmbDoor->outputB.Connect(pBundle, 1); // open door
		pLeftUmbDoorLatch->outputA.Connect(pBundle, 2); // latch
		pLeftUmbDoorLatch->outputB.Connect(pBundle, 3); // release
		pRightUmbDoor->outputA.Connect(pBundle, 4); // close door
		pRightUmbDoor->outputB.Connect(pBundle, 5); // open door
		pRightUmbDoorLatch->outputA.Connect(pBundle, 6); // latch
		pRightUmbDoorLatch->outputB.Connect(pBundle, 7); // release
		pCenterlineLatch->output.Connect(pBundle, 8); // stow

		pBundle=STS()->BundleManager()->CreateBundle("ETUmbDoor microswitches", 16);
		pETUmbLDoor->SetInput(0, pBundle, 0, TB_CL);
		pETUmbLDoor->SetInput(1, pBundle, 1, TB_OP);
		pETUmbLLatch->SetInput(0, pBundle, 2, TB_LAT);
		pETUmbLLatch->SetInput(1, pBundle, 3, TB_REL);
		pETUmbRDoor->SetInput(0, pBundle, 4, TB_CL);
		pETUmbRDoor->SetInput(1, pBundle, 5, TB_OP);
		pETUmbRLatch->SetInput(0, pBundle, 6, TB_LAT);
		pETUmbRLatch->SetInput(1, pBundle, 7, TB_REL);
		pETUmbDoorCLLatch->SetInput(0, pBundle, 8, TB_STO);

		pBundle=STS()->BundleManager()->CreateBundle("SSMEC_R2_SWITCHES", 8);
		pMPSPower[0][1]->output.Connect(pBundle, 0);
		pMPSPower[1][1]->output.Connect(pBundle, 1);
		pMPSHeIsolA[1]->outputA.Connect(pBundle, 2);// CL
		pMPSHeIsolA[1]->outputB.Connect(pBundle, 3);// OP
		pMPSHeIsolB[1]->outputA.Connect(pBundle, 4);// CL
		pMPSHeIsolB[1]->outputB.Connect(pBundle, 5);// OP
		pHeInterconnect[1]->outputA.Connect(pBundle, 6);// OUT OPEN
		pHeInterconnect[1]->outputB.Connect(pBundle, 7);// IN OPEN

		pBundle=STS()->BundleManager()->CreateBundle("SSMEL_R2_SWITCHES", 8);
		pMPSPower[0][0]->output.Connect(pBundle, 0);
		pMPSPower[1][0]->output.Connect(pBundle, 1);
		pMPSHeIsolA[0]->outputA.Connect(pBundle, 2);// CL
		pMPSHeIsolA[0]->outputB.Connect(pBundle, 3);// OP
		pMPSHeIsolB[0]->outputA.Connect(pBundle, 4);// CL
		pMPSHeIsolB[0]->outputB.Connect(pBundle, 5);// OP
		pHeInterconnect[0]->outputA.Connect(pBundle, 6);// OUT OPEN
		pHeInterconnect[0]->outputB.Connect(pBundle, 7);// IN OPEN

		pBundle=STS()->BundleManager()->CreateBundle("SSMER_R2_SWITCHES", 8);
		pMPSPower[0][2]->output.Connect(pBundle, 0);
		pMPSPower[1][2]->output.Connect(pBundle, 1);
		pMPSHeIsolA[2]->outputA.Connect(pBundle, 2);// CL
		pMPSHeIsolA[2]->outputB.Connect(pBundle, 3);// OP
		pMPSHeIsolB[2]->outputA.Connect(pBundle, 4);// CL
		pMPSHeIsolB[2]->outputB.Connect(pBundle, 5);// OP
		pHeInterconnect[2]->outputA.Connect(pBundle, 6);// OUT OPEN
		pHeInterconnect[2]->outputB.Connect(pBundle, 7);// IN OPEN

		pBundle=STS()->BundleManager()->CreateBundle( "PNEU_R2_SWITCHES", 4 );
		pPneuLEngHeXOVR->outputA.Connect( pBundle, 0 );// CLOSE
		pPneuLEngHeXOVR->outputB.Connect( pBundle, 1 );// OPEN
		pPneuHeIsol->outputA.Connect( pBundle, 2 );// CLOSE
		pPneuHeIsol->outputB.Connect( pBundle, 3 );// OPEN

		pBundle=STS()->BundleManager()->CreateBundle( "MPSDUMP_LH2UP_R2_SWITCHES", 5 );
		pMPSPRPLTDumpSequence->outputA.Connect( pBundle, 0 );// STOP
		pMPSPRPLTDumpSequence->outputB.Connect( pBundle, 1 );// START
		pMPSPRPLTBackupLH2vlv->outputA.Connect( pBundle, 2 );// CLOSE
		pMPSPRPLTBackupLH2vlv->outputB.Connect( pBundle, 3 );// OPEN
		pLH2UllagePress->ConnectPort( 1, pBundle, 4 );// AUTO

		AtlantisPanel::Realize();
	}
};
