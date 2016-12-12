#include "PanelO3.h"
#include "../Atlantis.h"
#include "../Atlantis_defs.h"


namespace vc
{
	PanelO3::PanelO3( Atlantis *_sts ):AtlantisPanel( _sts, "O3" )
	{
		Add( pRCSOMS_PRESS = new RotaryDemuxSwitch( _sts, "RCS/OMS/PRESS", 3 ) );
		pRCSOMS_PRESS->SetLabel( 0, "RCS He X10" );
		pRCSOMS_PRESS->SetLabel( 1, "RCS PRPLT" );
		pRCSOMS_PRESS->SetLabel( 2, "OMS PRPLT" );

		Add( pRCSOMS_PRPLTQTY = new RotaryDemuxSwitch( _sts, "RCS/OMS PRPLT QTY", 5 ) );
		pRCSOMS_PRPLTQTY->SetLabel( 0, "RCS OXID" );
		pRCSOMS_PRPLTQTY->SetLabel( 1, "RCS FUEL" );
		pRCSOMS_PRPLTQTY->SetLabel( 2, "LOWEST" );
		pRCSOMS_PRPLTQTY->SetLabel( 3, "OMS OXID" );
		pRCSOMS_PRPLTQTY->SetLabel( 4, "OMS FUEL" );

		Add( pMissionTimer = new StdSwitch3( _sts, "Mission Timer" ) );
		pMissionTimer->SetLabel( 0, "TEST" );
		pMissionTimer->SetLabel( 1, "MET" );
		pMissionTimer->SetLabel( 2, "GMT" );

		Add( pRCSOMS_PRPLT_QTY = new _7SegDisp_RCSOMS_PRPLT_QTY( _sts, "RCS/OMS PRPLT QTY" ) );
		Add( pMissionTime = new _7SegDisp_MissionTime( _sts, "Mission Time" ) );
	}

	PanelO3::~PanelO3()
	{
	}

	void PanelO3::DefineVC()
	{
		VECTOR3 switch_rot = _V( 1, 0, 0 );
		VECTOR3 rotary_switch_rot = _V( 0, 0.1984, 0.9801 );

		AddAIDToMouseEventList( AID_O3 );

		pRCSOMS_PRESS->DefineSwitchGroup( GRP_O3S10_VC );
		pRCSOMS_PRESS->DefineRotationAngle( 64.0f );
		pRCSOMS_PRESS->SetOffset( -32.0f );
		pRCSOMS_PRESS->SetInitialAnimState( 1.0f );
		pRCSOMS_PRESS->SetReference( _V( 0.2147, 2.7921, 14.3509 ), rotary_switch_rot );
		pRCSOMS_PRESS->SetMouseRegion( 0.142551f, 0.704583f, 0.202544f, 0.829816f );

		pRCSOMS_PRPLTQTY->DefineSwitchGroup( GRP_O3S11_VC );
		pRCSOMS_PRPLTQTY->DefineRotationAngle( 130.0f );
		pRCSOMS_PRPLTQTY->SetOffset( -65.0f );
		pRCSOMS_PRPLTQTY->SetInitialAnimState( 0.75f );
		pRCSOMS_PRPLTQTY->SetReference( _V( 0.3653, 2.7950, 14.3503 ), rotary_switch_rot );
		pRCSOMS_PRPLTQTY->SetMouseRegion( 0.398181f, 0.692892f, 0.454926f, 0.814155f );

		pMissionTimer->DefineSwitchGroup( GRP_O3S12_VC );
		pMissionTimer->SetInitialAnimState( 0.5f );
		pMissionTimer->SetReference( _V( 0.5121638,  2.799416,  14.35993 ), switch_rot );
		pMissionTimer->SetMouseRegion( 0.655579f, 0.695221f, 0.687731f, 0.794568f );
		pMissionTimer->SetSpringLoaded( true, 0 );

		pRCSOMS_PRPLT_QTY->DefineMesh( STS()->mesh_vc );
		pRCSOMS_PRPLT_QTY->DefineComponent( GRP_O3_PRPLT_RIGHT2_VC, true, false, false, _7SD_STATE_NUM0 );
		pRCSOMS_PRPLT_QTY->DefineComponent( GRP_O3_PRPLT_RIGHT1_VC, true, false, false, _7SD_STATE_NUM0 );
		pRCSOMS_PRPLT_QTY->DefineComponent( GRP_O3_PRPLT_FWD2_VC, true, false, false, _7SD_STATE_NUM0 );
		pRCSOMS_PRPLT_QTY->DefineComponent( GRP_O3_PRPLT_FWD1_VC, true, false, false, _7SD_STATE_NUM0 );
		pRCSOMS_PRPLT_QTY->DefineComponent( GRP_O3_PRPLT_LEFT2_VC, true, false, false, _7SD_STATE_NUM0 );
		pRCSOMS_PRPLT_QTY->DefineComponent( GRP_O3_PRPLT_LEFT1_VC, true, false, false, _7SD_STATE_NUM0 );

		pMissionTime->DefineMesh( STS()->mesh_vc );
		pMissionTime->DefineComponent( GRP_O3_MET_S2_VC, true, false, false, _7SD_STATE_NUM0 );
		pMissionTime->DefineComponent( GRP_O3_MET_S1_VC, true, false, false, _7SD_STATE_NUM0 );
		pMissionTime->DefineComponent( GRP_O3_MET_M2_VC, true, false, false, _7SD_STATE_NUM0 );
		pMissionTime->DefineComponent( GRP_O3_MET_M1_VC, true, false, false, _7SD_STATE_NUM0 );
		pMissionTime->DefineComponent( GRP_O3_MET_H2_VC, true, false, false, _7SD_STATE_NUM0 );
		pMissionTime->DefineComponent( GRP_O3_MET_H1_VC, true, false, false, _7SD_STATE_NUM0 );
		pMissionTime->DefineComponent( GRP_O3_MET_DAY3_VC, true, false, false, _7SD_STATE_NUM0 );
		pMissionTime->DefineComponent( GRP_O3_MET_DAY2_VC, true, false, false, _7SD_STATE_NUM0 );
		pMissionTime->DefineComponent( GRP_O3_MET_DAY1_VC, true, false, false, _7SD_STATE_NUM0 );
		pMissionTime->SetLocation( true );
		return;
	}

	void PanelO3::RegisterVC()
	{
		AtlantisPanel::RegisterVC();
		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea( AID_O3, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP );
		oapiVCSetAreaClickmode_Quadrilateral( AID_O3, 
			_V( 0.110078, 3.0096, 14.3151 ) + ofs, _V( 0.713058, 3.0096, 14.3151 ) + ofs, 
			_V( 0.110078, 2.72832, 14.3729 ) + ofs , _V( 0.713058, 2.72832, 14.3729 ) + ofs );
		return;
	}

	void PanelO3::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "O3_Timer_OMSRCS", 10 );
		pMissionTimer->ConnectPort( 1, pBundle, 0 );
		pMissionTimer->ConnectPort( 2, pBundle, 1 );

		pRCSOMS_PRESS->ConnectOutputSignal( 0, pBundle, 2 );
		pRCSOMS_PRESS->ConnectOutputSignal( 1, pBundle, 3 );
		pRCSOMS_PRESS->ConnectOutputSignal( 2, pBundle, 4 );
		pRCSOMS_PRESS_RCSHeX10.Connect( pBundle, 2 );
		pRCSOMS_PRESS_RCSPRPLT.Connect( pBundle, 3 );
		pRCSOMS_PRESS_OMSPRPLT.Connect( pBundle, 4 );

		pRCSOMS_PRPLTQTY->ConnectOutputSignal( 0, pBundle, 5 );
		pRCSOMS_PRPLTQTY->ConnectOutputSignal( 1, pBundle, 6 );
		pRCSOMS_PRPLTQTY->ConnectOutputSignal( 2, pBundle, 7 );
		pRCSOMS_PRPLTQTY->ConnectOutputSignal( 3, pBundle, 8 );
		pRCSOMS_PRPLTQTY->ConnectOutputSignal( 4, pBundle, 9 );

		AtlantisPanel::Realize();
		return;
	}
}
