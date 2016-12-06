#include "PanelA1U.h"
#include "../Atlantis.h"
#include "../Atlantis_defs.h"


namespace vc
{
	PanelA1U::PanelA1U(Atlantis* _sts):AtlantisPanel(_sts, "A1U")
	{
		/*Add(pSignalStrengthSelect = new StdSwitch3(_sts, "SIG STR"));
		pSignalStrengthSelect->SetLabel(0, "SBAND FM");
		pSignalStrengthSelect->SetLabel(1, "KU");
		pSignalStrengthSelect->SetLabel(2, "SBAND PM");*/

		Add( pSlewAzimuth = new StdSwitch3( _sts, "SLEW AZIMUTH" ) );
		pSlewAzimuth->SetLabel( 0, "R" );
		pSlewAzimuth->SetLabel( 2, "L" );

		Add( pSlewElevation = new StdSwitch3( _sts, "SLEW EL" ) );
		pSlewElevation->SetLabel( 0, "DOWN" );
		pSlewElevation->SetLabel( 2, "UP" );

		Add( pSlewRate = new StdSwitch2( _sts, "SLEW RATE" ) );
		pSlewRate->SetLabel( 0, "SLOW" );
		pSlewRate->SetLabel( 1, "FAST" );

		Add( pControl = new StdSwitch2( _sts, "CONTROL" ) );
		pControl->SetLabel( 0, "PANEL" );
		pControl->SetLabel( 1, "COMMAND" );

		Add( pSteeringMode = new RotaryDemuxSwitch( _sts, "STEERING MODE", 4 ) );
		pSteeringMode->SetLabel( 0, "MAN SLEW" );
		pSteeringMode->SetLabel( 1, "AUTO TRACK" );
		pSteeringMode->SetLabel( 2, "GPC DESIG" );
		pSteeringMode->SetLabel( 3, "GPC" );
		pSteeringMode->SetInitialPosition( 0 );

		Add( pSearch = new StdSwitch2( _sts, "SEARCH" ) );
		pSearch->SetLabel( 1, "SEARCH" );

		Add( pPower = new StdSwitch3( _sts, "POWER" ) );
		pPower->SetLabel( 0, "OFF" );
		pPower->SetLabel( 1, "STBY" );
		pPower->SetLabel( 2, "ON" );

		Add( pMode = new StdSwitch3( _sts, "MODE" ) );
		pMode->SetLabel( 0, "COMM" );
		pMode->SetLabel( 1, "RDR PASSIVE" );
		pMode->SetLabel( 2, "RDR COOP" );

		Add( pRadarOutput = new StdSwitch3( _sts, "RADAR OUTPUT" ) );
		pRadarOutput->SetLabel( 0, "LOW" );
		pRadarOutput->SetLabel( 1, "MED" );
		pRadarOutput->SetLabel( 2, "HIGH" );

		Add( pScanWarnTB = new StandardTalkback_2( _sts, "Scan Warn TB", 1 ) );
		Add( pTrackTB = new StandardTalkback_2( _sts, "Track TB", 1 ) );
		Add( pSearchTB = new StandardTalkback_2( _sts, "Search TB", 1 ) );
	}

	PanelA1U::~PanelA1U()
	{
	}

	void PanelA1U::DefineVC()
	{
		const VECTOR3 switch_rot_vert =  _V( -1, 0, 0 );
		const VECTOR3 switch_rot_horz = _V( 0.0, 1.0, 0.0 );
		const VECTOR3 rotary_switch_rot = _V( 0.0, 0.0, -1.0 );

		AddAIDToMouseEventList( AID_A1U );

		pSlewAzimuth->SetInitialAnimState( 0.5f );
		pSlewAzimuth->DefineSwitchGroup( GRP_A1US2_VC );
		pSlewAzimuth->SetMouseRegion( 0.378565f, 0.285422f, 0.457113f, 0.370123f );
		pSlewAzimuth->SetReference( _V( 1.1198, 2.9729, 12.2231 ), switch_rot_horz );
		pSlewAzimuth->SetOrientation( true );
		pSlewAzimuth->SetSpringLoaded( true, 0 );
		pSlewAzimuth->SetSpringLoaded( true, 2 );

		pSlewElevation->SetInitialAnimState( 0.5f );
		pSlewElevation->DefineSwitchGroup( GRP_A1US3_VC );
		pSlewElevation->SetMouseRegion( 0.505451f, 0.272174f, 0.574339f, 0.377064f );
		pSlewElevation->SetReference( _V( 1.0690, 2.9781, 12.2231 ), switch_rot_vert );
		pSlewElevation->SetSpringLoaded( true, 0 );
		pSlewElevation->SetSpringLoaded( true, 2 );

		pSlewRate->SetInitialAnimState( 0.5f );
		pSlewRate->DefineSwitchGroup( GRP_A1US4_VC );
		pSlewRate->SetMouseRegion( 0.592866f, 0.272507f, 0.656497f, 0.376257f );
		pSlewRate->SetReference( _V( 1.0690, 2.9781, 12.2231 ), switch_rot_vert );

		pControl->SetInitialAnimState( 0.5f );
		pControl->DefineSwitchGroup( GRP_A1US6_VC );
		pControl->SetMouseRegion( 0.549356f, 0.490418f, 0.618453f, 0.598225f );
		pControl->SetReference( _V( 1.0520, 2.9117, 12.2231 ), switch_rot_vert );

		pSteeringMode->SetInitialAnimState( 1.0f );
		pSteeringMode->DefineSwitchGroup( GRP_A1URS7_VC );
		pSteeringMode->SetMouseRegion( 0.165013f, 0.627728f, 0.267503f, 0.750766f );
		pSteeringMode->SetReference( _V( 1.1953, 2.8678, 12.2315 ), rotary_switch_rot );
		pSteeringMode->DefineRotationAngle( 90.0f );
		pSteeringMode->SetOffset( 135.0f );

		pSearch->SetInitialAnimState( 0.5f );
		pSearch->DefineSwitchGroup( GRP_A1US8_VC );
		pSearch->SetMouseRegion( 0.351805f, 0.644865f, 0.415273f, 0.748752f );
		pSearch->SetReference( _V( 1.1282, 2.8656, 12.2231 ), switch_rot_vert );
		pSearch->SetSpringLoaded( true, 1 );

		pPower->SetInitialAnimState( 0.5f );
		pPower->DefineSwitchGroup( GRP_A1US12_VC );
		pPower->SetMouseRegion( 0.030196f, 0.855995f, 0.092852f, 0.958587f );
		pPower->SetReference( _V( 1.2504, 2.8016, 12.2231 ), switch_rot_vert );

		pMode->SetInitialAnimState( 0.5f );
		pMode->DefineSwitchGroup( GRP_A1US13_VC );
		pMode->SetMouseRegion( 0.162602f, 0.855104f, 0.224663f, 0.961070f );
		pMode->SetReference( _V( 1.2504, 2.8016, 12.2231 ), switch_rot_vert );

		pRadarOutput->SetInitialAnimState( 0.5f );
		pRadarOutput->DefineSwitchGroup( GRP_A1US14_VC );
		pRadarOutput->SetMouseRegion( 0.353588f, 0.856309f, 0.416909f, 0.962201f );
		pRadarOutput->SetReference( _V( 1.2504, 2.8016, 12.2231 ), switch_rot_vert );
		
		pScanWarnTB->DefineMeshGroup( STS()->mesh_vc, GRP_A1U_TB_SCANWARN_VC );
		pTrackTB->DefineMeshGroup( STS()->mesh_vc, GRP_A1U_TB_TRACK_VC );
		pSearchTB->DefineMeshGroup( STS()->mesh_vc, GRP_A1U_TB_SEARCH_VC );
	}

	void PanelA1U::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea( AID_A1U, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_A1U, 
			_V( 1.2784, 3.0780, 12.2217 ) + ofs, _V( 0.8986, 3.0780, 12.2217 ) + ofs, 
			_V( 1.2784, 2.7741, 12.2217 ) + ofs, _V( 0.8986, 2.7741, 12.2217 ) + ofs );
	}

	void PanelA1U::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "A1U_SWITCHES_A", 16 );
		pSlewAzimuth->ConnectPort( 1, pBundle, 0 );
		pSlewAzimuth->ConnectPort( 2, pBundle, 1 );
		pSlewElevation->ConnectPort( 1, pBundle, 2 );
		pSlewElevation->ConnectPort( 2, pBundle, 3 );
		pSlewRate->ConnectPort( 1, pBundle, 4 );
		pSearch->ConnectPort( 1, pBundle, 5 );
		pRadarOutput->ConnectPort( 1, pBundle, 6 );
		pRadarOutput->ConnectPort( 2, pBundle, 7 );
		pScanWarnTB->SetInput( 0, pBundle, 8, TB_GRAY );
		pTrackTB->SetInput( 0, pBundle, 9, TB_GRAY );
		pSearchTB->SetInput( 0, pBundle, 10, TB_GRAY );

		pBundle = STS()->BundleManager()->CreateBundle( "A1U_SWITCHES_B", 16 );
		pControl->ConnectPort( 1, pBundle, 0 );
		pSteeringMode->ConnectOutputSignal( 3, pBundle, 1 );
		pSteeringMode->ConnectOutputSignal( 2, pBundle, 2 );
		pSteeringMode->ConnectOutputSignal( 1, pBundle, 3 );
		pSteeringMode->ConnectOutputSignal( 0, pBundle, 4 );
		pPower->ConnectPort( 1, pBundle, 5 );
		pPower->ConnectPort( 2, pBundle, 6 );
		pMode->ConnectPort( 1, pBundle, 7 );
		pMode->ConnectPort( 2, pBundle, 8 );

		AtlantisPanel::Realize();
	}
};