#include "PanelA1U.h"
#include "../meshres_vc_additions.h"
#include "../Atlantis.h"
#include "../Atlantis_defs.h"


namespace vc
{
	PanelA1U::PanelA1U(Atlantis* _sts):AtlantisPanel(_sts, "A1U")
	{
		Add(pSignalStrengthSelect = new StdSwitch3(_sts, "SIG STR"));
		pSignalStrengthSelect->SetLabel(0, "SBAND FM");
		pSignalStrengthSelect->SetLabel(1, "KU");
		pSignalStrengthSelect->SetLabel(2, "SBAND PM");

		Add( pSlewAzimuth = new StdSwitch3( _sts, "SLEW AZIMUTH" ) );
		pSlewAzimuth->SetLabel( 0, "L" );
		pSlewAzimuth->SetLabel( 2, "R" );

		Add( pSlewElevation = new StdSwitch3( _sts, "SLEW EL" ) );
		pSlewElevation->SetLabel( 0, "DOWN" );
		pSlewElevation->SetLabel( 2, "UP" );

		Add( pSlewRate = new StdSwitch2( _sts, "SLEW RATE" ) );
		pSlewRate->SetLabel( 0, "SLOW" );
		pSlewRate->SetLabel( 1, "FAST" );

		Add( pSteeringMode = new RotaryDemuxSwitch( _sts, "STEERING MODE", 4 ) );
		pSteeringMode->SetLabel( 3, "MAN SLEW" );
		pSteeringMode->SetLabel( 2, "AUTO TRACK" );
		pSteeringMode->SetLabel( 1, "GPC DESIG" );
		pSteeringMode->SetLabel( 0, "GPC" );

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
	}

	PanelA1U::~PanelA1U()
	{
	}

	void PanelA1U::OnPreStep(double SimT, double DeltaT, double MJD)
	{
		return;
	}

	void PanelA1U::DefineVC()
	{
		const VECTOR3 switch_rot_vert =  _V( -1, 0, 0 );
		const VECTOR3 switch_rot_horz = _V( 0.0, -0.9499, 0.3126 );
		const VECTOR3 rotary_switch_rot = _V( 0.0, -0.3126, -0.9499 );
		//VECTOR3 switch_pull = _V( 0, 0.3126, 0.9499 );

		AddAIDToMouseEventList( AID_A1U1 );
		//AddAIDToMouseEventList( AID_A1U2 );

		pSlewAzimuth->SetInitialAnimState( 0.5f );
		pSlewAzimuth->DefineSwitchGroup( GRP_A1US2_VC );
		pSlewAzimuth->SetMouseRegion( 0.170449f, 0.618925f, 0.226497f, 0.651137f );//////
		pSlewAzimuth->SetReference( _V( 1.1198, 2.9729, 12.2231 ), switch_rot_horz );
		pSlewAzimuth->SetOrientation( false );
		pSlewAzimuth->SetSpringLoaded( true, 0 );
		pSlewAzimuth->SetSpringLoaded( true, 2 );

		pSlewElevation->SetInitialAnimState( 0.5f );
		pSlewElevation->DefineSwitchGroup( GRP_A1US3_VC );
		pSlewElevation->SetMouseRegion( 0.170449f, 0.618925f, 0.226497f, 0.651137f );//////
		pSlewElevation->SetReference( _V( 1.0690, 2.9781, 12.2231 ), switch_rot_vert );
		pSlewElevation->SetSpringLoaded( true, 0 );
		pSlewElevation->SetSpringLoaded( true, 2 );

		pSlewRate->SetInitialAnimState( 0.5f );
		pSlewRate->DefineSwitchGroup( GRP_A1US4_VC );
		pSlewRate->SetMouseRegion( 0.170449f, 0.618925f, 0.226497f, 0.651137f );//////
		pSlewRate->SetReference( _V( 1.0690, 2.9781, 12.2231 ), switch_rot_vert );

		pSteeringMode->SetInitialAnimState( 0.0f );
		pSteeringMode->DefineSwitchGroup( GRP_A1URS7_VC );
		pSteeringMode->SetMouseRegion( 0.45049f, 0.440562f, 0.531691f, 0.494226f );/////
		pSteeringMode->SetReference( _V( 1.1953, 2.8678, 12.2315 ), rotary_switch_rot );
		pSteeringMode->DefineRotationAngle( 90.0f );
		pSteeringMode->SetOffset( 45.0f );
		pSteeringMode->SetInitialPosition( 0 );

		pSearch->SetInitialAnimState( 0.5f );
		pSearch->DefineSwitchGroup( GRP_A1US8_VC );
		pSearch->SetMouseRegion( 0.170449f, 0.618925f, 0.226497f, 0.651137f );//////
		pSearch->SetReference( _V( 1.1282, 2.8656, 12.2231 ), switch_rot_vert );
		pSearch->SetSpringLoaded( true, 1 );

		pPower->SetInitialAnimState( 0.5f );
		pPower->DefineSwitchGroup( GRP_A1US12_VC );
		pPower->SetMouseRegion( 0.170449f, 0.618925f, 0.226497f, 0.651137f );//////
		pPower->SetReference( _V( 1.2504, 2.8016, 12.2231 ), switch_rot_vert );

		pMode->SetInitialAnimState( 0.5f );
		pMode->DefineSwitchGroup( GRP_A1US13_VC );
		pMode->SetMouseRegion( 0.170449f, 0.618925f, 0.226497f, 0.651137f );//////
		pMode->SetReference( _V( 1.2504, 2.8016, 12.2231 ), switch_rot_vert );

		pRadarOutput->SetInitialAnimState( 0.5f );
		pRadarOutput->DefineSwitchGroup( GRP_A1US14_VC );
		pRadarOutput->SetMouseRegion( 0.170449f, 0.618925f, 0.226497f, 0.651137f );//////
		pRadarOutput->SetReference( _V( 1.2504, 2.8016, 12.2231 ), switch_rot_vert );
	}

	void PanelA1U::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea( AID_A1U1, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_A1U1, 
			_V( 1.0151, 3.1734, 12.2217 ) + ofs, _V( 0.8005, 2.9113, 12.2217 ) + ofs, 
			_V( 1.4696, 2.7409, 12.2217 ) + ofs, _V( 0.8007, 2.7409, 12.2217 ) + ofs );
		/*oapiVCRegisterArea(AID_A1U1, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
		oapiVCSetAreaClickmode_Quadrilateral(AID_A1U1,
			_V(-0.264, 2.893, 12.277)+ofs, _V(0.255, 2.893, 12.277)+ofs,
			_V(-0.264, 2.507, 12.397)+ofs, _V(0.255, 2.507, 12.397)+ofs);

		oapiVCRegisterArea(AID_A1U2, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
		oapiVCSetAreaClickmode_Quadrilateral(AID_A1U2,
			_V(-0.264, 2.893, 12.277)+ofs, _V(0.255, 2.893, 12.277)+ofs,
			_V(-0.264, 2.507, 12.397)+ofs, _V(0.255, 2.507, 12.397)+ofs);*/
	}

	void PanelA1U::Realize()
	{
		AtlantisPanel::Realize();
	}
};