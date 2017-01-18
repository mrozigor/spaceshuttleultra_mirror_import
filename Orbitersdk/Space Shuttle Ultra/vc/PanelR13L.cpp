#include "PanelR13L.h"
#include "../Atlantis.h"
#include "../Atlantis_defs.h"


namespace vc
{
	PanelR13L::PanelR13L( Atlantis* psts ):AtlantisPanel( psts, "R13L" )
	{
		oapiWriteLog( "(PanelR13L::PanelR13L) Enter constructor." );

		Add( pPLBayDoor[0] = new StdSwitch2( psts, "PL Bay Door SYS 1" ) );
		pPLBayDoor[0]->SetLabel( 0, "DISABLE" );
		pPLBayDoor[0]->SetLabel( 1, "ENABLE" );
		Add( pPLBayDoor[1] = new StdSwitch2( psts, "PL Bay Door SYS 2" ) );
		pPLBayDoor[1]->SetLabel( 0, "DISABLE" );
		pPLBayDoor[1]->SetLabel( 1, "ENABLE" );

		Add( pPLBayMechPWR[0] = new StdSwitch2( psts, "PL Bay Mech PWR SYS 1" ) );
		pPLBayMechPWR[0]->SetLabel( 0, "OFF" );
		pPLBayMechPWR[0]->SetLabel( 1, "ON" );
		Add( pPLBayMechPWR[1] = new StdSwitch2( psts, "PL Bay Mech PWR SYS 2" ) );
		pPLBayMechPWR[1]->SetLabel( 0, "OFF" );
		pPLBayMechPWR[1]->SetLabel( 1, "ON" );

		Add( pPLBayDoorLL = new LockableLever3( psts, "PL Bay Door" ) );
		pPLBayDoorLL->SetLabel( 0, "CLOSE" );
		pPLBayDoorLL->SetLabel( 1, "STOP" );
		pPLBayDoorLL->SetLabel( 2, "OPEN" );

		Add( pLatchControl[0] = new StdSwitch3( psts, "Latch Control SYS A" ) );
		pLatchControl[0]->SetLabel( 0, "LATCH" );
		pLatchControl[0]->SetLabel( 1, "OFF" );
		pLatchControl[0]->SetLabel( 2, "RELEASE" );
		Add( pLatchControl[1] = new StdSwitch3( psts, "Latch Control SYS B" ) );
		pLatchControl[1]->SetLabel( 0, "LATCH" );
		pLatchControl[1]->SetLabel( 1, "OFF" );
		pLatchControl[1]->SetLabel( 2, "RELEASE" );

		Add( pRadiatorControl[0] = new StdSwitch3( psts, "Radiator Control SYS A" ) );
		pRadiatorControl[0]->SetLabel( 0, "STOW" );
		pRadiatorControl[0]->SetLabel( 1, "OFF" );
		pRadiatorControl[0]->SetLabel( 2, "DEPLOY" );
		Add( pRadiatorControl[1] = new StdSwitch3( psts, "Radiator Control SYS B" ) );
		pRadiatorControl[1]->SetLabel( 0, "STOW" );
		pRadiatorControl[1]->SetLabel( 1, "OFF" );
		pRadiatorControl[1]->SetLabel( 2, "DEPLOY" );

		Add( pKUAntennaDirectStow = new LockableLever2( psts, "KU Antenna Direct Stow" ) );
		pKUAntennaDirectStow->SetLabel( 0, "OFF" );
		pKUAntennaDirectStow->SetLabel( 1, "ON" );

		Add( pKUAntenna = new StdSwitch3( psts, "KU Antenna" ) );
		pKUAntenna->SetLabel( 0, "STOW" );
		pKUAntenna->SetLabel( 1, "GND" );
		pKUAntenna->SetLabel( 2, "DEPLOY" );

		Add( pMMUGN2SPLYISOLVLV[0] = new StdSwitch3( psts, "MMU GN2 SPLY ISOL VLV A" ) );
		pMMUGN2SPLYISOLVLV[0]->SetLabel( 0, "CLOSE" );
		pMMUGN2SPLYISOLVLV[0]->SetLabel( 1, "-" );
		pMMUGN2SPLYISOLVLV[0]->SetLabel( 2, "OPEN" );
		Add( pMMUGN2SPLYISOLVLV[1] = new StdSwitch3( psts, "MMU GN2 SPLY ISOL VLV B" ) );
		pMMUGN2SPLYISOLVLV[1]->SetLabel( 0, "CLOSE" );
		pMMUGN2SPLYISOLVLV[1]->SetLabel( 1, "-" );
		pMMUGN2SPLYISOLVLV[1]->SetLabel( 2, "OPEN" );

		Add( pPLBayDoorTB = new StandardTalkback3( psts, "PL Bay Door" ) );

		Add( pLatch[0] = new StandardTalkback3( psts, "Latch STBD" ) );
		Add( pLatch[1] = new StandardTalkback3( psts, "Latch PORT" ) );

		Add( pRadiator[0] = new StandardTalkback3( psts, "Radiator STBD" ) );
		Add( pRadiator[1] = new StandardTalkback3( psts, "Radiator PORT" ) );

		Add( pKUAntennaTB = new StandardTalkback3( psts, "Ku Antenna" ) );

		Add( pMMUGN2SPLYISOLVLVTB[0] = new StandardTalkback3( psts, "MMU GN2 SPLY ISOL VLV A" ) );
		Add( pMMUGN2SPLYISOLVLVTB[1] = new StandardTalkback3( psts, "MMU GN2 SPLY ISOL VLV B" ) );

		oapiWriteLog( "(PanelR13L::PanelR13L) Leave constructor." );
	}

	PanelR13L::~PanelR13L()
	{
	}

	void PanelR13L::DefineVC()
	{
		const VECTOR3 SWITCH_ROT = _V( 0, 0, -1 );
		const VECTOR3 SWITCH_PULL = _V( -0.5, 0.866, 0 );
		AddAIDToMouseEventList( AID_R13L );

		pPLBayDoor[0]->SetInitialAnimState( 0.5f );
		pPLBayDoor[0]->DefineSwitchGroup( GRP_SWITCHR13L_1_VC );
		pPLBayDoor[0]->SetReference( _V( 1.271, 2.259, 12.501 ), SWITCH_ROT );
		pPLBayDoor[0]->SetMouseRegion( 0.163153f, 0.143026f, 0.243193f, 0.224609f );

		pPLBayDoor[1]->SetInitialAnimState( 0.5f );
		pPLBayDoor[1]->DefineSwitchGroup( GRP_SWITCHR13L_2_VC );
		pPLBayDoor[1]->SetReference( _V( 1.271, 2.259, 12.501 ), SWITCH_ROT );
		pPLBayDoor[1]->SetMouseRegion( 0.310684f, 0.137739f, 0.400270f, 0.226850f );

		pPLBayMechPWR[0]->SetInitialAnimState( 0.5f );
		pPLBayMechPWR[0]->DefineSwitchGroup( GRP_SWITCHR13L_3_VC );
		pPLBayMechPWR[0]->SetReference( _V( 1.271, 2.259, 12.501 ), SWITCH_ROT );
		pPLBayMechPWR[0]->SetMouseRegion( 0.464691f, 0.139690f, 0.548855f, 0.223995f );

		pPLBayMechPWR[1]->SetInitialAnimState( 0.5f );
		pPLBayMechPWR[1]->DefineSwitchGroup( GRP_SWITCHR13L_4_VC );
		pPLBayMechPWR[1]->SetReference( _V( 1.271, 2.259, 12.501 ), SWITCH_ROT );
		pPLBayMechPWR[1]->SetMouseRegion( 0.617969f, 0.144364f, 0.706554f, 0.219415f );


		pLatchControl[0]->SetInitialAnimState( 0.5f );
		pLatchControl[0]->DefineSwitchGroup( GRP_SWITCHR13L_6_VC );
		pLatchControl[0]->SetReference( _V( 1.18, 2.199, 12.504 ), SWITCH_ROT );
		pLatchControl[0]->SetMouseRegion( 0.303606f, 0.495669f, 0.389273f, 0.577190f );

		pLatchControl[1]->SetInitialAnimState( 0.5f );
		pLatchControl[1]->DefineSwitchGroup( GRP_SWITCHR13L_7_VC );
		pLatchControl[1]->SetReference( _V( 1.18, 2.199, 12.504 ), SWITCH_ROT );
		pLatchControl[1]->SetMouseRegion( 0.462003f, 0.502695f, 0.547449f, 0.579582f );

		pRadiatorControl[0]->SetInitialAnimState( 0.5f );
		pRadiatorControl[0]->DefineSwitchGroup( GRP_SWITCHR13L_8_VC );
		pRadiatorControl[0]->SetReference( _V( 1.18, 2.199, 12.504 ), SWITCH_ROT );
		pRadiatorControl[0]->SetMouseRegion( 0.621844f, 0.499144f, 0.709090f, 0.575397f );

		pRadiatorControl[1]->SetInitialAnimState( 0.5f );
		pRadiatorControl[1]->DefineSwitchGroup( GRP_SWITCHR13L_9_VC );
		pRadiatorControl[1]->SetReference( _V( 1.18, 2.199, 12.504 ), SWITCH_ROT );
		pRadiatorControl[1]->SetMouseRegion( 0.778255f, 0.494450f, 0.865381f, 0.574690f );
		
		pKUAntenna->SetInitialAnimState( 0.5f );
		pKUAntenna->DefineSwitchGroup( GRP_SWITCHR13L_11_VC );
		pKUAntenna->SetReference( _V( 1.093, 2.142, 12.503 ), SWITCH_ROT );
		pKUAntenna->SetMouseRegion( 0.307785f, 0.828553f, 0.388628f, 0.909387f );

		pMMUGN2SPLYISOLVLV[0]->SetInitialAnimState( 0.5f );
		pMMUGN2SPLYISOLVLV[0]->DefineSwitchGroup( GRP_SWITCHR13L_12_VC );
		pMMUGN2SPLYISOLVLV[0]->SetReference( _V( 1.093, 2.142, 12.503 ), SWITCH_ROT );
		pMMUGN2SPLYISOLVLV[0]->SetMouseRegion( 0.618380f, 0.832279f, 0.705493f, 0.909792f );
		pMMUGN2SPLYISOLVLV[0]->SetSpringLoaded( true, 0 );
		pMMUGN2SPLYISOLVLV[0]->SetSpringLoaded( true, 2 );

		pMMUGN2SPLYISOLVLV[1]->SetInitialAnimState( 0.5f );
		pMMUGN2SPLYISOLVLV[1]->DefineSwitchGroup( GRP_SWITCHR13L_13_VC );
		pMMUGN2SPLYISOLVLV[1]->SetReference( _V( 1.093, 2.142, 12.503 ), SWITCH_ROT );
		pMMUGN2SPLYISOLVLV[1]->SetMouseRegion( 0.779999f, 0.833202f, 0.866269f, 0.908316f );
		pMMUGN2SPLYISOLVLV[1]->SetSpringLoaded( true, 0 );
		pMMUGN2SPLYISOLVLV[1]->SetSpringLoaded( true, 2 );


		pPLBayDoorLL->SetInitialAnimState( 0.5f );
		pPLBayDoorLL->DefineSwitchGroup( GRP_SWITCHR13L_5_VC );
		pPLBayDoorLL->SetReference( _V( 1.18, 2.199, 12.504 ), SWITCH_ROT );
		pPLBayDoorLL->SetMouseRegion( 0.150730f, 0.496063f, 0.234853f, 0.570609f );
		pPLBayDoorLL->SetPullDirection( SWITCH_PULL );

		pKUAntennaDirectStow->SetInitialAnimState( 0.5f );
		pKUAntennaDirectStow->DefineSwitchGroup( GRP_SWITCHR13L_10_VC );
		pKUAntennaDirectStow->SetReference( _V( 1.093, 2.142, 12.503 ), SWITCH_ROT );
		pKUAntennaDirectStow->SetMouseRegion( 0.152062f, 0.833136f, 0.237823f, 0.908989f );
		pKUAntennaDirectStow->SetPullDirection( SWITCH_PULL );


		pPLBayDoorTB->DefineMeshGroup( STS()->mesh_vc, GRP_R13TALK1_VC );

		pLatch[0]->DefineMeshGroup( STS()->mesh_vc, GRP_R13TALK2_VC );

		pLatch[1]->DefineMeshGroup( STS()->mesh_vc, GRP_R13TALK3_VC );

		pRadiator[0]->DefineMeshGroup( STS()->mesh_vc, GRP_R13TALK4_VC );

		pRadiator[1]->DefineMeshGroup( STS()->mesh_vc, GRP_R13TALK5_VC );

		pKUAntennaTB->DefineMeshGroup( STS()->mesh_vc, GRP_R13TALK6_VC );

		pMMUGN2SPLYISOLVLVTB[0]->DefineMeshGroup( STS()->mesh_vc, GRP_R13TALK7_VC );

		pMMUGN2SPLYISOLVLVTB[1]->DefineMeshGroup( STS()->mesh_vc, GRP_R13TALK8_VC );
		return;
	}

	void PanelR13L::RegisterVC()
	{
		AtlantisPanel::RegisterVC();
		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea( AID_R13L, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBPRESSED | PANEL_MOUSE_LBUP );
		oapiVCSetAreaClickmode_Quadrilateral( AID_R13L, 
			_V( 1.32, 2.29, 12.56 ) + ofs, _V( 1.32, 2.29, 12.26 ) + ofs, 
			_V( 1.06, 2.12, 12.56 ) + ofs, _V( 1.06, 2.12, 12.26 ) + ofs );
		return;
	}

	void PanelR13L::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "PayloadBayDoorControl", 6 );
		pPLBayDoor[0]->output.Connect( pBundle, 0 );
		pPLBayDoor[1]->output.Connect( pBundle, 1 );
		pPLBayDoorLL->ConnectPort( 1, pBundle, 2 );
		pPLBayDoorLL->ConnectPort( 2, pBundle, 3 );
		pPLBayDoorTB->SetInput( 0, pBundle, 4, TB_OP );
		pPLBayDoorTB->SetInput( 1, pBundle, 5, TB_CL );

		pBundle = STS()->BundleManager()->CreateBundle( "RadiatorControlSW", 10 );
		pPLBayMechPWR[0]->output.Connect( pBundle, 0 );
		pPLBayMechPWR[1]->output.Connect( pBundle, 1 );
		pLatchControl[0]->ConnectPort( 1, pBundle, 2 );
		pLatchControl[0]->ConnectPort( 2, pBundle, 3 );
		pLatchControl[1]->ConnectPort( 1, pBundle, 4 );
		pLatchControl[1]->ConnectPort( 2, pBundle, 5 );
		pRadiatorControl[0]->ConnectPort( 1, pBundle, 6 );
		pRadiatorControl[0]->ConnectPort( 2, pBundle, 7 );
		pRadiatorControl[1]->ConnectPort( 1, pBundle, 8 );
		pRadiatorControl[1]->ConnectPort( 2, pBundle, 9 );

		pBundle = STS()->BundleManager()->CreateBundle( "RadiatorControlTB", 8 );
		pLatch[0]->SetInput( 0, pBundle, 0, TB_LAT );
		pLatch[0]->SetInput( 1, pBundle, 1, TB_REL );
		pLatch[1]->SetInput( 0, pBundle, 2, TB_LAT );
		pLatch[1]->SetInput( 1, pBundle, 3, TB_REL );
		pRadiator[0]->SetInput( 0, pBundle, 4, TB_STO );
		pRadiator[0]->SetInput( 1, pBundle, 5, TB_DPY );
		pRadiator[1]->SetInput( 0, pBundle, 6, TB_STO );
		pRadiator[1]->SetInput( 1, pBundle, 7, TB_DPY );

		pBundle = STS()->BundleManager()->CreateBundle( "KUAntennaControl", 16 );
		pKUAntennaDirectStow->ConnectPort(1, pBundle, 0 );
		pKUAntenna->ConnectPort( 1, pBundle, 1 );
		pKUAntenna->ConnectPort( 2, pBundle, 2 );
		pKUAntennaTB->SetInput( 0, pBundle, 3, TB_STO );
		pKUAntennaTB->SetInput( 1, pBundle, 4, TB_DPY );

		/*pBundle = STS()->BundleManager()->CreateBundle( "MMUGN2SPLYISOLVLV", 8 );
		pMMUGN2SPLYISOLVLV[0]->ConnectPort( 1, pBundle, 0 );
		pMMUGN2SPLYISOLVLV[0]->ConnectPort( 2, pBundle, 1 );
		pMMUGN2SPLYISOLVLV[1]->ConnectPort( 1, pBundle, 2 );
		pMMUGN2SPLYISOLVLV[1]->ConnectPort( 2, pBundle, 3 );
		pMMUGN2SPLYISOLVLVTB[0]->SetInput( 0, pBundle, 4, TB_OP );
		pMMUGN2SPLYISOLVLVTB[0]->SetInput( 1, pBundle, 5, TB_CLO );
		pMMUGN2SPLYISOLVLVTB[1]->SetInput( 0, pBundle, 6, TB_OP );
		pMMUGN2SPLYISOLVLVTB[1]->SetInput( 1, pBundle, 7, TB_CLO );*/

		AtlantisPanel::Realize();
		return;
	}
};
