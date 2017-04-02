#include "../Atlantis.h"
#include "PanelO6.h"
#include "..\CommonDefs.h"
#include "../Atlantis_defs.h"
#include "..\meshres_vc_o6.h"

namespace vc {

	PanelO6::PanelO6(Atlantis* _sts):AtlantisPanel(_sts, "O6")
	{
		oapiWriteLog("(PanelO6::PanelO6) Enter constructor.");

		hPanelMesh = oapiLoadMeshGlobal( DEFAULT_MESHNAME_PANELO6 );
		mesh_index = MESH_UNDEFINED;

		Add(pLGlareShieldFlood	= new StdSwitch3(_sts, "L GLRSHLD FLOOD"));	
		pLGlareShieldFlood->SetLabel(0, "OFF");
		pLGlareShieldFlood->SetLabel(1, "VAR");
		pLGlareShieldFlood->SetLabel(2, "BRIGHT");

		Add(pSTYDoorPosition = new StandardTalkback3(_sts, "STAR TRACKER -Y DOOR POSITION (tb)"));
		Add(pSTZDoorPosition = new StandardTalkback3(_sts, "STAR TRACKER -Z DOOR POSITION (tb)"));

		Add( pSTRKDoorControlSys1Cover = new StandardSwitchCover( _sts, "S TRK DR CNTL SYS1 Cover" ) );

		Add( pSTRKDoorControlSys2Cover = new StandardSwitchCover( _sts, "S TRK DR CNTL SYS2 Cover" ) );

		Add(pSTRKDoorControlSys1 = new StdSwitch3(_sts, "S TRK DR CNTL SYS1"));		 
		pSTRKDoorControlSys1->SetLabel(0, "CLOSE");
		pSTRKDoorControlSys1->SetLabel(1, "OFF");
		pSTRKDoorControlSys1->SetLabel(2, "OPEN");
		
		Add(pSTRKDoorControlSys2 = new StdSwitch3(_sts, "S TRK DR CNTL SYS2"));		 
		pSTRKDoorControlSys2->SetLabel(0, "CLOSE");
		pSTRKDoorControlSys2->SetLabel(1, "OFF");
		pSTRKDoorControlSys2->SetLabel(2, "OPEN");
		
		Add( pAnnunciatorLampTest = new StdSwitch3( _sts, "Annunciator Lamp Test" ) );

		Add( pAnnunciatorBusSelectACA1 = new StdSwitch3( _sts, "Annunciator Bus Select ACA 1" ) );
		pAnnunciatorBusSelectACA1->SetLabel( 0, "MN B" );
		pAnnunciatorBusSelectACA1->SetLabel( 1, "OFF" );
		pAnnunciatorBusSelectACA1->SetLabel( 2, "MN A" );

		Add( pAnnunciatorBusSelectACA23 = new StdSwitch3( _sts, "Annunciator Bus Select ACA 2/3" ) );
		pAnnunciatorBusSelectACA23->SetLabel( 0, "MN C" );
		pAnnunciatorBusSelectACA23->SetLabel( 1, "OFF" );
		pAnnunciatorBusSelectACA23->SetLabel( 2, "MN B" );
		
		/*		
		StdSwitch2* pSTRKPowerNY;
		StdSwitch2* pSTRKPowerNZ;
		StdSwitch3* pLeftSeatCenterFlood;
		StdSwitch2* pUHFXmitFreq;
		StdSwitch2* pUHFSPLXPowerAmp;
		StdSwitch2* pUHFSquelch;
		StdSwitch3* pMTU;
		StdSwitch2* pIDP1;
		StdSwitch2* pIDP2;
		StdSwitch2* pIDP3;
		StdSwitch2* pIDP4;
		StdSwitch2* pMDMPL1;
		StdSwitch2* pMDMPL2;
		StdSwitch2* pMDMPL3;
		StdSwitch2* pMDMFA1;
		StdSwitch2* pMDMFA2;
		StdSwitch2* pMDMFA2;
		StdSwitch2* pMDMFA4;
		StdSwitch2* pMDMFF2;
		StdSwitch2* pMDMFF2;
		StdSwitch2* pMDMFF2;
		StdSwitch2* pMDMFF2;
		*/

		Add(pGPCPowerCover[0] = new StandardSwitchCover(_sts, "GPC_POWER_1_COVER"));
		
		Add(pGPCPowerCover[1] = new StandardSwitchCover(_sts, "GPC_POWER_2_COVER"));
		
		Add(pGPCPowerCover[2] = new StandardSwitchCover(_sts, "GPC_POWER_3_COVER"));
		
		Add(pGPCPowerCover[3] = new StandardSwitchCover(_sts, "GPC_POWER_4_COVER"));

		Add(pGPCPowerCover[4] = new StandardSwitchCover(_sts, "GPC_POWER_5_COVER"));

		Add(pGPC1Pwr = new StdSwitch2(_sts, "GPC POWER 1"));
		pGPC1Pwr->SetLabel(0, "OFF");
		pGPC1Pwr->SetLabel(1, "ON");

		Add(pGPC2Pwr = new StdSwitch2(_sts, "GPC POWER 2"));
		pGPC2Pwr->SetLabel(0, "OFF");
		pGPC2Pwr->SetLabel(1, "ON");

		Add(pGPC3Pwr = new StdSwitch2(_sts, "GPC POWER 3"));
		pGPC3Pwr->SetLabel(0, "OFF");
		pGPC3Pwr->SetLabel(1, "ON");

		Add(pGPC4Pwr = new StdSwitch2(_sts, "GPC POWER 4"));
		pGPC4Pwr->SetLabel(0, "OFF");
		pGPC4Pwr->SetLabel(1, "ON");

		Add(pGPC5Pwr = new StdSwitch2(_sts, "GPC POWER 5"));
		pGPC5Pwr->SetLabel(0, "OFF");
		pGPC5Pwr->SetLabel(1, "ON");

		Add( pGPCOutput[0] = new StandardTalkback2( _sts, "GPC Output 1 (tb)" ) );
		
		Add( pGPCOutput[1] = new StandardTalkback2( _sts, "GPC Output 2 (tb)" ) );

		Add( pGPCOutput[2] = new StandardTalkback2( _sts, "GPC Output 3 (tb)" ) );

		Add( pGPCOutput[3] = new StandardTalkback2( _sts, "GPC Output 4 (tb)" ) );

		Add( pGPCOutput[4] = new StandardTalkback2( _sts, "GPC Output 5 (tb)" ) );

		Add(pGPCOutputCover[0] = new StandardSwitchCover(_sts, "GPC_OUTPUT_1_COVER"));
		
		Add(pGPCOutputCover[1] = new StandardSwitchCover(_sts, "GPC_OUTPUT_2_COVER"));
		
		Add(pGPCOutputCover[2] = new StandardSwitchCover(_sts, "GPC_OUTPUT_3_COVER"));
		
		Add(pGPCOutputCover[3] = new StandardSwitchCover(_sts, "GPC_OUTPUT_4_COVER"));

		Add(pGPCOutputCover[4] = new StandardSwitchCover(_sts, "GPC_OUTPUT_5_COVER"));

		Add(pGPC1Output = new StdSwitch3(_sts, "GPC OUTPUT 1"));
		pGPC1Output->SetLabel(0, "TERMINATE");
		pGPC1Output->SetLabel(1, "NORMAL");
		pGPC1Output->SetLabel(2, "BACKUP");

		Add(pGPC2Output = new StdSwitch3(_sts, "GPC OUTPUT 2"));
		pGPC2Output->SetLabel(0, "TERMINATE");
		pGPC2Output->SetLabel(1, "NORMAL");
		pGPC2Output->SetLabel(2, "BACKUP");

		Add(pGPC3Output = new StdSwitch3(_sts, "GPC OUTPUT 3"));
		pGPC3Output->SetLabel(0, "TERMINATE");
		pGPC3Output->SetLabel(1, "NORMAL");
		pGPC3Output->SetLabel(2, "BACKUP");

		Add(pGPC4Output = new StdSwitch3(_sts, "GPC OUTPUT 4"));
		pGPC4Output->SetLabel(0, "TERMINATE");
		pGPC4Output->SetLabel(1, "NORMAL");
		pGPC4Output->SetLabel(2, "BACKUP");

		Add(pGPC5Output = new StdSwitch3(_sts, "GPC OUTPUT 5"));
		pGPC5Output->SetLabel(0, "TERMINATE");
		pGPC5Output->SetLabel(1, "NORMAL");
		pGPC5Output->SetLabel(2, "BACKUP");

		Add(pIPLSource = new StdSwitch3(_sts, "IPL SOURCE"));
		pIPLSource->SetLabel(0, "MMU2");
		pIPLSource->SetLabel(1, "OFF");
		pIPLSource->SetLabel(2, "MMU1");

		Add(pIPL1 = new PushButton(_sts, "IPL1"));

		Add(pIPL2 = new PushButton(_sts, "IPL2"));

		Add(pIPL3 = new PushButton(_sts, "IPL3"));

		Add(pIPL4 = new PushButton(_sts, "IPL4"));

		Add(pIPL5 = new PushButton(_sts, "IPL5"));

		Add( pGPCMode[0] = new StandardTalkback3( _sts, "GPC MODE 1 (tb)" ) );

		Add( pGPCMode[1] = new StandardTalkback3( _sts, "GPC MODE 2 (tb)" ) );

		Add( pGPCMode[2] = new StandardTalkback3( _sts, "GPC MODE 3 (tb)" ) );

		Add( pGPCMode[3] = new StandardTalkback3( _sts, "GPC MODE 4 (tb)" ) );

		Add( pGPCMode[4] = new StandardTalkback3( _sts, "GPC MODE 5 (tb)" ) );

		Add(pGPC1Mode = new LockableLever3(_sts, "GPC MODE 1"));
		pGPC1Mode->SetLabel(0, "HALT");
		pGPC1Mode->SetLabel(1, "STBY");
		pGPC1Mode->SetLabel(2, "RUN");

		Add(pGPC2Mode = new LockableLever3(_sts, "GPC MODE 2"));
		pGPC2Mode->SetLabel(0, "HALT");
		pGPC2Mode->SetLabel(1, "STBY");
		pGPC2Mode->SetLabel(2, "RUN");

		Add(pGPC3Mode = new LockableLever3(_sts, "GPC MODE 3"));
		pGPC3Mode->SetLabel(0, "HALT");
		pGPC3Mode->SetLabel(1, "STBY");
		pGPC3Mode->SetLabel(2, "RUN");

		Add(pGPC4Mode = new LockableLever3(_sts, "GPC MODE 4"));
		pGPC4Mode->SetLabel(0, "HALT");
		pGPC4Mode->SetLabel(1, "STBY");
		pGPC4Mode->SetLabel(2, "RUN");

		Add(pGPC5Mode = new LockableLever3(_sts, "GPC MODE 5"));
		pGPC5Mode->SetLabel(0, "HALT");
		pGPC5Mode->SetLabel(1, "STBY");
		pGPC5Mode->SetLabel(2, "RUN");

		oapiWriteLog("(PanelO6::PanelO6) Leave constructor.");
	}

	PanelO6::~PanelO6()
	{
	}

	void PanelO6::AddMeshes( const VECTOR3 &ofs )
	{
		SetHasOwnVCMesh();

		if (mesh_index == MESH_UNDEFINED)
		{
			mesh_index = STS()->AddMesh( hPanelMesh, &ofs );
			STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		}
		return;
	}

	void PanelO6::SetMeshVisibility( bool visible )
	{
		if (visible) STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		else STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_NEVER );
		return;
	}

	UINT PanelO6::GetVCMeshIndex( void ) const
	{
		return mesh_index;
	}

	void PanelO6::DefineVC()
	{
		oapiWriteLog("(PanelO6::Define VC)");
		
		VECTOR3 switch_rot = _V( 1, 0, 0 );
		VECTOR3 SWITCH_VERTICAL = _V( 1.0, 0.0, 0.0 );
		
		AddAIDToMouseEventList(AID_O6);

		pSTYDoorPosition->DefineMeshGroup( STS()->mesh_vc, GRP_DS1_O6_VC );
		
		pSTZDoorPosition->DefineMeshGroup( STS()->mesh_vc, GRP_DS2_O6_VC );

		pSTRKDoorControlSys1Cover->SetMouseRegion( 0, 0.656091f, 0.175973f, 0.700474f, 0.227679f );
		pSTRKDoorControlSys1Cover->SetMouseRegion( 1, 0.656091f, 0.236236f, 0.700474f, 0.251363f );
		pSTRKDoorControlSys1Cover->DefineCoverGroup( GRP_COVER2_O6_VC );
		pSTRKDoorControlSys1Cover->SetReference( _V( -0.3814, 3.1445, 13.7718 ), -SWITCH_VERTICAL );

		pSTRKDoorControlSys2Cover->SetMouseRegion( 0, 0.724671f, 0.175973f, 0.764189f, 0.227679f );
		pSTRKDoorControlSys2Cover->SetMouseRegion( 1, 0.724671f, 0.236236f, 0.764189f, 0.251363f );
		pSTRKDoorControlSys2Cover->DefineCoverGroup( GRP_COVER3_O6_VC );
		pSTRKDoorControlSys2Cover->SetReference( _V( -0.3814, 3.1445, 13.7718 ), -SWITCH_VERTICAL );

		pSTRKDoorControlSys1->DefineSwitchGroup( GRP_S2_O6_VC );
		pSTRKDoorControlSys1->SetMouseRegion(0.64f, 0.1767f, 0.7078f, 0.2315f);
		pSTRKDoorControlSys1->SetReference(_V(-0.3818, 3.1533, 13.7537), SWITCH_VERTICAL);
		pSTRKDoorControlSys1->SetInitialAnimState(0.5f);

		pSTRKDoorControlSys2->DefineSwitchGroup( GRP_S3_O6_VC );
		pSTRKDoorControlSys2->SetMouseRegion(0.7267f, 0.1767f, 0.7706f, 0.2315f);
		pSTRKDoorControlSys2->SetReference(_V(-0.3818, 3.1533, 13.7537), SWITCH_VERTICAL);
		pSTRKDoorControlSys2->SetInitialAnimState(0.5f);

		pAnnunciatorLampTest->SetInitialAnimState( 0.5f );
		pAnnunciatorLampTest->DefineSwitchGroup( GRP_S14_O6_VC );
		pAnnunciatorLampTest->SetReference( _V( -0.6506, 3.0979, 13.9709 ), switch_rot );
		pAnnunciatorLampTest->SetMouseRegion( 0.109378f, 0.497310f, 0.155600f, 0.532509f );
		pAnnunciatorLampTest->SetSpringLoaded( true, 0 );
		pAnnunciatorLampTest->SetSpringLoaded( true, 2 );

		pAnnunciatorBusSelectACA1->SetInitialAnimState( 0.5f );
		pAnnunciatorBusSelectACA1->DefineSwitchGroup( GRP_S12_O6_VC );
		pAnnunciatorBusSelectACA1->SetReference( _V( -0.6506, 3.0979, 13.9709 ), switch_rot );
		pAnnunciatorBusSelectACA1->SetMouseRegion( 0.228373f, 0.496051f, 0.271396f, 0.531983f );
		pAnnunciatorBusSelectACA1->SetInitialPosition( 2 );

		pAnnunciatorBusSelectACA23->SetInitialAnimState( 0.5f );
		pAnnunciatorBusSelectACA23->DefineSwitchGroup( GRP_S13_O6_VC );
		pAnnunciatorBusSelectACA23->SetReference( _V( -0.6506, 3.0979, 13.9709 ), switch_rot );
		pAnnunciatorBusSelectACA23->SetMouseRegion( 0.292820f, 0.496975f, 0.334038f, 0.531239f );
		pAnnunciatorBusSelectACA23->SetInitialPosition( 2 );

		pGPCPowerCover[0]->SetMouseRegion(0, 0.4798f, 0.5008f, 0.5473f, 0.5570f);
		pGPCPowerCover[0]->SetMouseRegion(1, 0.483795f, 0.561731f, 0.535098f, 0.576666f);
		pGPCPowerCover[0]->DefineCoverGroup( GRP_COVER30_O6_VC );
		pGPCPowerCover[0]->SetReference(_V( -0.4652, 3.0908, 13.9972 ), -SWITCH_VERTICAL);

		pGPCPowerCover[1]->SetMouseRegion(0, 0.5587f, 0.5008f, 0.6032f, 0.5570f);
		pGPCPowerCover[1]->SetMouseRegion(1, 0.551317f, 0.561731f, 0.606547f, 0.576666f);
		pGPCPowerCover[1]->DefineCoverGroup( GRP_COVER31_O6_VC );
		pGPCPowerCover[1]->SetReference(_V( -0.4652, 3.0908, 13.9972 ), -SWITCH_VERTICAL);

		pGPCPowerCover[2]->SetMouseRegion(0, 0.6212f, 0.5008f, 0.6657f, 0.5570f);
		pGPCPowerCover[2]->SetMouseRegion(1, 0.619886f, 0.561731f, 0.675091f, 0.576666f);
		pGPCPowerCover[2]->DefineCoverGroup( GRP_COVER32_O6_VC );
		pGPCPowerCover[2]->SetReference(_V( -0.4652, 3.0908, 13.9972 ), -SWITCH_VERTICAL);

		pGPCPowerCover[3]->SetMouseRegion(0, 0.6828f, 0.5008f, 0.7375f, 0.5570f);
		pGPCPowerCover[3]->SetMouseRegion(1, 0.688901f, 0.561731f, 0.743673f, 0.576666f);
		pGPCPowerCover[3]->DefineCoverGroup( GRP_COVER33_O6_VC );
		pGPCPowerCover[3]->SetReference(_V( -0.4652, 3.0908, 13.9972 ), -SWITCH_VERTICAL);

		pGPCPowerCover[4]->SetMouseRegion(0, 0.7538f, 0.5008f, 0.8021f, 0.5570f);
		pGPCPowerCover[4]->SetMouseRegion(1, 0.759426f, 0.561731f, 0.811709f, 0.576666f);
		pGPCPowerCover[4]->DefineCoverGroup( GRP_COVER34_O6_VC );
		pGPCPowerCover[4]->SetReference(_V( -0.4652, 3.0908, 13.9972 ), -SWITCH_VERTICAL);

		pGPC1Pwr->SetMouseRegion(0.4798f, 0.5008f, 0.5473f, 0.5570f);
		pGPC1Pwr->DefineSwitchGroup( GRP_S30_O6_VC );
		pGPC1Pwr->SetReference(_V(0.0, 3.09556, 13.98215), SWITCH_VERTICAL);
		pGPC1Pwr->SetInitialAnimState(0.5f);

		pGPC2Pwr->SetMouseRegion(0.5587f, 0.5008f, 0.6032f, 0.5570f);
		pGPC2Pwr->DefineSwitchGroup( GRP_S31_O6_VC );
		pGPC2Pwr->SetReference(_V(0.0, 3.09556, 13.98215), SWITCH_VERTICAL);
		pGPC2Pwr->SetInitialAnimState(0.5f);

		pGPC3Pwr->SetMouseRegion(0.6212f, 0.5008f, 0.6657f, 0.5570f);
		pGPC3Pwr->DefineSwitchGroup( GRP_S32_O6_VC );
		pGPC3Pwr->SetReference(_V(0.0, 3.09556, 13.98215), SWITCH_VERTICAL);
		pGPC3Pwr->SetInitialAnimState(0.5f);

		pGPC4Pwr->SetMouseRegion(0.6828f, 0.5008f, 0.7375f, 0.5570f);
		pGPC4Pwr->DefineSwitchGroup( GRP_S33_O6_VC );
		pGPC4Pwr->SetReference(_V(0.0, 3.09556, 13.98215), SWITCH_VERTICAL);
		pGPC4Pwr->SetInitialAnimState(0.5f);

		pGPC5Pwr->SetMouseRegion(0.7538f, 0.5008f, 0.8021f, 0.5570f);
		pGPC5Pwr->DefineSwitchGroup( GRP_S34_O6_VC );
		pGPC5Pwr->SetReference(_V(0.0, 3.09556, 13.98215), SWITCH_VERTICAL);
		pGPC5Pwr->SetInitialAnimState(0.5f);

		pGPCOutput[0]->DefineMeshGroups( STS()->mesh_vc, GRP_DS3_U_O6_VC, GRP_DS3_L_O6_VC );
		
		pGPCOutput[1]->DefineMeshGroups( STS()->mesh_vc, GRP_DS4_U_O6_VC, GRP_DS4_L_O6_VC );
		
		pGPCOutput[2]->DefineMeshGroups( STS()->mesh_vc, GRP_DS5_U_O6_VC, GRP_DS5_L_O6_VC );
		
		pGPCOutput[3]->DefineMeshGroups( STS()->mesh_vc, GRP_DS6_U_O6_VC, GRP_DS6_L_O6_VC );
		
		pGPCOutput[4]->DefineMeshGroups( STS()->mesh_vc, GRP_DS7_U_O6_VC, GRP_DS7_L_O6_VC );

		pGPCOutputCover[0]->SetMouseRegion(0, 0.4798f, 0.6438f, 0.5473f, 0.7007f);
		pGPCOutputCover[0]->SetMouseRegion(1, 0.483795f, 0.705872f, 0.535098f, 0.723074f);
		pGPCOutputCover[0]->DefineCoverGroup( GRP_COVER35_O6_VC );
		pGPCOutputCover[0]->SetReference(_V( -0.4653, 3.0668, 14.0986 ), -SWITCH_VERTICAL);

		pGPCOutputCover[1]->SetMouseRegion(0, 0.5587f, 0.6438f, 0.6032f, 0.7007f);
		pGPCOutputCover[1]->SetMouseRegion(1, 0.551317f, 0.705872f, 0.606547f, 0.723074f);
		pGPCOutputCover[1]->DefineCoverGroup( GRP_COVER36_O6_VC );
		pGPCOutputCover[1]->SetReference(_V( -0.4653, 3.0668, 14.0986 ), -SWITCH_VERTICAL);

		pGPCOutputCover[2]->SetMouseRegion(0, 0.6212f, 0.6438f, 0.6657f, 0.7007f);
		pGPCOutputCover[2]->SetMouseRegion(1, 0.619886f, 0.705872f, 0.675091f, 0.723074f);
		pGPCOutputCover[2]->DefineCoverGroup( GRP_COVER37_O6_VC );
		pGPCOutputCover[2]->SetReference(_V( -0.4653, 3.0668, 14.0986 ), -SWITCH_VERTICAL);

		pGPCOutputCover[3]->SetMouseRegion(0, 0.6828f, 0.6438f, 0.7375f, 0.7007f);
		pGPCOutputCover[3]->SetMouseRegion(1, 0.688901f, 0.705872f, 0.743673f, 0.723074f);
		pGPCOutputCover[3]->DefineCoverGroup( GRP_COVER38_O6_VC );
		pGPCOutputCover[3]->SetReference(_V( -0.4653, 3.0668, 14.0986 ), -SWITCH_VERTICAL);

		pGPCOutputCover[4]->SetMouseRegion(0, 0.7538f, 0.6438f, 0.8021f, 0.7007f);
		pGPCOutputCover[4]->SetMouseRegion(1, 0.759426f, 0.705872f, 0.811709f, 0.723074f);
		pGPCOutputCover[4]->DefineCoverGroup( GRP_COVER39_O6_VC );
		pGPCOutputCover[4]->SetReference(_V( -0.4653, 3.0668, 14.0986 ), -SWITCH_VERTICAL);

		pGPC1Output->SetMouseRegion(0.4798f, 0.6438f, 0.5473f, 0.7007f);
		pGPC1Output->DefineSwitchGroup( GRP_S35_O6_VC );
		pGPC1Output->SetReference(_V(0.0,  3.07197,  14.08016), SWITCH_VERTICAL);
		pGPC1Output->SetInitialAnimState(0.5f);

		pGPC2Output->SetMouseRegion(0.5587f, 0.6438f, 0.6032f, 0.7007f);
		pGPC2Output->DefineSwitchGroup( GRP_S36_O6_VC );
		pGPC2Output->SetReference(_V(0.0,  3.07197,  14.08016), SWITCH_VERTICAL);
		pGPC2Output->SetInitialAnimState(0.5f);

		pGPC3Output->SetMouseRegion(0.6212f, 0.6438f, 0.6657f, 0.7007f);
		pGPC3Output->DefineSwitchGroup( GRP_S37_O6_VC );
		pGPC3Output->SetReference(_V(0.0,  3.07197,  14.08016), SWITCH_VERTICAL);
		pGPC3Output->SetInitialAnimState(0.5f);

		pGPC4Output->SetMouseRegion(0.6828f, 0.6438f, 0.7375f, 0.7007f);
		pGPC4Output->DefineSwitchGroup( GRP_S38_O6_VC );
		pGPC4Output->SetReference(_V(0.0,  3.07197,  14.08016), SWITCH_VERTICAL);
		pGPC4Output->SetInitialAnimState(0.5f);

		pGPC5Output->SetMouseRegion(0.7538f, 0.6438f, 0.8021f, 0.7007f);
		pGPC5Output->DefineSwitchGroup( GRP_S39_O6_VC );
		pGPC5Output->SetReference(_V(0.0,  3.07197,  14.08016), SWITCH_VERTICAL);
		pGPC5Output->SetInitialAnimState(0.5f);

		pIPLSource->SetMouseRegion(0.8518f, 0.7391f, 0.8936f, 0.7720f);
		pIPLSource->DefineSwitchGroup( GRP_S45_O6_VC );
		pIPLSource->SetReference(_V(0.0, 3.056848, 14.14088), SWITCH_VERTICAL);
		pIPLSource->SetInitialAnimState( 0.5f );

		pIPL1->SetMouseRegion(0.4949f, 0.7365f, 0.5439f, 0.7741f);
		
		pIPL2->SetMouseRegion(0.5599f, 0.7365f, 0.6079f, 0.7741f);
		
		pIPL3->SetMouseRegion(0.6268f, 0.7365f, 0.6727f, 0.7741f);
		
		pIPL4->SetMouseRegion(0.6905f, 0.7365f, 0.7396f, 0.7741f);

		pIPL5->SetMouseRegion(0.7554f, 0.7365f, 0.8026f, 0.7741f);

		pGPCMode[0]->DefineMeshGroup( STS()->mesh_vc, GRP_DS8_O6_VC );
		
		pGPCMode[1]->DefineMeshGroup( STS()->mesh_vc, GRP_DS9_O6_VC );
		
		pGPCMode[2]->DefineMeshGroup( STS()->mesh_vc, GRP_DS10_O6_VC );
		
		pGPCMode[3]->DefineMeshGroup( STS()->mesh_vc, GRP_DS11_O6_VC );
		
		pGPCMode[4]->DefineMeshGroup( STS()->mesh_vc, GRP_DS12_O6_VC );

		pGPC1Mode->SetMouseRegion(0.4798f, 0.8718f, 0.5473f, 0.9117f);
		pGPC1Mode->DefineSwitchGroup( GRP_S46_O6_VC );
		pGPC1Mode->SetReference(_V(0.0,  3.033531,  14.23673), SWITCH_VERTICAL);
		pGPC1Mode->SetPullDirection( _V( 0.0, -0.971961, -0.235142 ) );
		pGPC1Mode->SetInitialAnimState(0.5f);

		pGPC2Mode->SetMouseRegion(0.5587f, 0.8718f, 0.6032f, 0.9117f);
		pGPC2Mode->DefineSwitchGroup( GRP_S47_O6_VC );
		pGPC2Mode->SetReference(_V(0.0,  3.033531,  14.23673), SWITCH_VERTICAL);
		pGPC2Mode->SetPullDirection( _V( 0.0, -0.971961, -0.235142 ) );
		pGPC2Mode->SetInitialAnimState(0.5f);

		pGPC3Mode->SetMouseRegion(0.6212f, 0.8718f, 0.6657f, 0.9117f);
		pGPC3Mode->DefineSwitchGroup( GRP_S48_O6_VC );
		pGPC3Mode->SetReference(_V(0.0,  3.033531,  14.23673), SWITCH_VERTICAL);
		pGPC3Mode->SetPullDirection( _V( 0.0, -0.971961, -0.235142 ) );
		pGPC3Mode->SetInitialAnimState(0.5f);

		pGPC4Mode->SetMouseRegion(0.6828f, 0.8718f, 0.7375f, 0.9117f);
		pGPC4Mode->DefineSwitchGroup( GRP_S49_O6_VC );
		pGPC4Mode->SetReference(_V(0.0,  3.033531,  14.23673), SWITCH_VERTICAL);
		pGPC4Mode->SetPullDirection( _V( 0.0, -0.971961, -0.235142 ) );
		pGPC4Mode->SetInitialAnimState(0.5f);

		pGPC5Mode->SetMouseRegion(0.7538f, 0.8718f, 0.8021f, 0.9117f);
		pGPC5Mode->DefineSwitchGroup( GRP_S50_O6_VC );
		pGPC5Mode->SetReference(_V(0.0,  3.033531,  14.23673), SWITCH_VERTICAL);
		pGPC5Mode->SetPullDirection( _V( 0.0, -0.971961, -0.235142 ) );
		pGPC5Mode->SetInitialAnimState(0.5f);
		return;
	}

	void PanelO6::DefineVCAnimations( UINT vcidx )
	{
		AtlantisPanel::DefineVCAnimations( mesh_index );
		return;
	}

	void PanelO6::Realize()
	{
		//Warning: This is not correct, panel should talk to FMCA
		discsignals::DiscreteBundle* pBundle = 
			STS()->BundleManager()->CreateBundle("FMCA_STARTRACKER", 16);

		pSTRKDoorControlSys1->outputA.Connect(pBundle, 0);
		pSTRKDoorControlSys1->outputB.Connect(pBundle, 1);

		pSTRKDoorControlSys2->outputA.Connect(pBundle, 4);
		pSTRKDoorControlSys2->outputB.Connect(pBundle, 5);

		pSTYDoorPosition->SetInput( 0, pBundle, 6, TB_CL );
		pSTYDoorPosition->SetInput( 1, pBundle, 7, TB_OP );
		pSTZDoorPosition->SetInput( 0, pBundle, 8, TB_CL );
		pSTZDoorPosition->SetInput( 1, pBundle, 9, TB_OP );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA", 16 );
		pAnnunciatorBusSelectACA1->ConnectPort( 1, pBundle, 0 );
		pAnnunciatorBusSelectACA1->ConnectPort( 2, pBundle, 1 );
		pAnnunciatorBusSelectACA23->ConnectPort( 1, pBundle, 2 );
		pAnnunciatorBusSelectACA23->ConnectPort( 2, pBundle, 3 );
		pAnnunciatorLampTest->ConnectPort( 2, pBundle, 6 );
		pAnnunciatorLampTest->ConnectPort( 1, pBundle, 7 );

		pBundle = STS()->BundleManager()->CreateBundle("O6_GPC1", 16);
		pGPC1Pwr->output.Connect(pBundle, 0);
		pGPC1Output->outputA.Connect(pBundle, 1);
		pGPC1Output->outputB.Connect(pBundle, 2);
		pIPL1->output.Connect(pBundle, 3);
		pGPC1Mode->ConnectPort( 1, pBundle, 4 );
		pGPC1Mode->ConnectPort( 2, pBundle, 5 );
		pGPCOutput[0]->SetInput( pBundle, 6, TB_GRAY );
		pGPCMode[0]->SetInput( 0, pBundle, 7, TB_RUN );
		pGPCMode[0]->SetInput( 1, pBundle, 8, TB_IPL );
		
		pBundle = STS()->BundleManager()->CreateBundle("O6_GPC2", 16);
		pGPC2Pwr->output.Connect(pBundle, 0);
		pGPC2Output->outputA.Connect(pBundle, 1);
		pGPC2Output->outputB.Connect(pBundle, 2);
		pIPL2->output.Connect(pBundle, 3);
		pGPC2Mode->ConnectPort( 1, pBundle, 4 );
		pGPC2Mode->ConnectPort( 2, pBundle, 5 );
		pGPCOutput[1]->SetInput( pBundle, 6, TB_GRAY );
		pGPCMode[1]->SetInput( 0, pBundle, 7, TB_RUN );
		pGPCMode[1]->SetInput( 1, pBundle, 8, TB_IPL );
		
		pBundle = STS()->BundleManager()->CreateBundle("O6_GPC3", 16);
		pGPC3Pwr->output.Connect(pBundle, 0);
		pGPC3Output->outputA.Connect(pBundle, 1);
		pGPC3Output->outputB.Connect(pBundle, 2);
		pIPL3->output.Connect(pBundle, 3);
		pGPC3Mode->ConnectPort( 1, pBundle, 4 );
		pGPC3Mode->ConnectPort( 2, pBundle, 5 );
		pGPCOutput[2]->SetInput( pBundle, 6, TB_GRAY );
		pGPCMode[2]->SetInput( 0, pBundle, 7, TB_RUN );
		pGPCMode[2]->SetInput( 1, pBundle, 8, TB_IPL );

		pBundle = STS()->BundleManager()->CreateBundle("O6_GPC4", 16);
		pGPC4Pwr->output.Connect(pBundle, 0);
		pGPC4Output->outputA.Connect(pBundle, 1);
		pGPC4Output->outputB.Connect(pBundle, 2);
		pIPL4->output.Connect(pBundle, 3);
		pGPC4Mode->ConnectPort( 1, pBundle, 4 );
		pGPC4Mode->ConnectPort( 2, pBundle, 5 );
		pGPCOutput[3]->SetInput( pBundle, 6, TB_GRAY );
		pGPCMode[3]->SetInput( 0, pBundle, 7, TB_RUN );
		pGPCMode[3]->SetInput( 1, pBundle, 8, TB_IPL );
		
		pBundle = STS()->BundleManager()->CreateBundle("O6_GPC5", 16);
		pGPC5Pwr->output.Connect(pBundle, 0);
		pGPC5Output->outputA.Connect(pBundle, 1);
		pGPC5Output->outputB.Connect(pBundle, 2);
		pIPL5->output.Connect(pBundle, 3);
		pGPC5Mode->ConnectPort( 1, pBundle, 4 );
		pGPC5Mode->ConnectPort( 2, pBundle, 5 );
		pGPCOutput[4]->SetInput( pBundle, 6, TB_GRAY );
		pGPCMode[4]->SetInput( 0, pBundle, 7, TB_RUN );
		pGPCMode[4]->SetInput( 1, pBundle, 8, TB_IPL );

		pBundle = STS()->BundleManager()->CreateBundle("O6_IPL", 16);
		pIPLSource->outputA.Connect(pBundle, 0);
		pIPLSource->outputB.Connect(pBundle, 1);
		
		AtlantisPanel::Realize();
	}

	void PanelO6::RegisterVC()
	{
		oapiWriteLog("(PanelO6::RegisterVC) Begin registration.");
		AtlantisPanel::RegisterVC();
		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea(AID_O6, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral (AID_O6, 
			_V(-0.726224, 3.18582, 13.6082)+ofs, _V(-0.217782, 3.18582, 13.6082)+ofs, 
			_V(-0.726224, 3.01496, 14.3133) + ofs, _V(-0.217782, 3.01496, 14.3133)+ofs);
	}

};
