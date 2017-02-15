#include "PanelO2.h"
#include "../Atlantis.h"
#include "../Atlantis_defs.h"


namespace vc
{
	PanelO2::PanelO2( Atlantis *_sts ):AtlantisPanel( _sts, "O2" )
	{
		Add( pCryoO2HtrAssyTempMeter = new StandardCurvedMechMeter( _sts, "Cryo O2 Htr Assy Temp Meter" ) );
		Add( pCryoO2PressMeter = new StandardCurvedMechMeter( _sts, "Cryo O2 Press Meter" ) );
		Add( pCryoO2QtyMeter = new StandardCurvedMechMeter( _sts, "Cryo O2 Qty Meter" ) );
		Add( pCryoH2PressMeter = new StandardCurvedMechMeter( _sts, "Cryo H2 Press Meter" ) );
		Add( pCryoH2QtyMeter = new StandardCurvedMechMeter( _sts, "Cryo H2 Qty Meter" ) );
		Add( pFuelCellStackTempMeter = new StandardCurvedMechMeter( _sts, "Fuel Cell Stack Temp Meter" ) );

		Add( pCryoO2HtrAssyTemp = new StandardRotarySwitch( _sts, "Cryo O2 Htr Assy Temp", 10, 1 ) );
		pCryoO2HtrAssyTemp->SetLabel( 0, "TK 5 2" );
		pCryoO2HtrAssyTemp->SetLabel( 1, "TK 5 1" );
		pCryoO2HtrAssyTemp->SetLabel( 2, "TK 4 2" );
		pCryoO2HtrAssyTemp->SetLabel( 3, "TK 4 1" );
		pCryoO2HtrAssyTemp->SetLabel( 4, "TK 3 2" );
		pCryoO2HtrAssyTemp->SetLabel( 5, "TK 3 1" );
		pCryoO2HtrAssyTemp->SetLabel( 6, "TK 2 2" );
		pCryoO2HtrAssyTemp->SetLabel( 7, "TK 2 1" );
		pCryoO2HtrAssyTemp->SetLabel( 8, "TK 1 2" );
		pCryoO2HtrAssyTemp->SetLabel( 9, "TK 1 1" );

		Add( pCryoPressQty = new StandardRotarySwitch( _sts, "Cryo Press Qty", 5, 4 ) );
		pCryoPressQty->SetLabel( 0, "TK5" );
		pCryoPressQty->SetLabel( 1, "TK4" );
		pCryoPressQty->SetLabel( 2, "TK3" );
		pCryoPressQty->SetLabel( 3, "TK2" );
		pCryoPressQty->SetLabel( 4, "TK1" );

		Add( pFuelCellStackTemp = new StdSwitch3( _sts, "Fuel Cell Stack Temp" ) );
		pFuelCellStackTemp->SetLabel( 0, "3" );
		pFuelCellStackTemp->SetLabel( 1, "2" );
		pFuelCellStackTemp->SetLabel( 2, "1" );
	}

	PanelO2::~PanelO2()
	{
	}

	void PanelO2::DefineVC( void )
	{
		VECTOR3 switch_rot = _V( 1, 0, 0 );
		VECTOR3 rotary_switch_rot = _V( 0, -0.1984, -0.9801 );
		VECTOR3 needle_ref = _V( 0, 2.952158, 14.59572 );// 0.27m radius
		VECTOR3 needle_rot = _V( 1, 0, 0 );

		AddAIDToMouseEventList( AID_O2 );

		pCryoO2HtrAssyTempMeter->DefineNeedleGroup( GRP_O2M5T_VC );
		pCryoO2HtrAssyTempMeter->SetInitialAnimState( 0.55 );
		pCryoO2HtrAssyTempMeter->SetMotionRange( 12.5 );
		pCryoO2HtrAssyTempMeter->SetReference( needle_ref, needle_rot );

		pCryoO2PressMeter->DefineNeedleGroup( GRP_O2M6P_VC );
		pCryoO2PressMeter->SetInitialAnimState( 0.55 );
		pCryoO2PressMeter->SetMotionRange( 15.0 );
		pCryoO2PressMeter->SetReference( needle_ref, needle_rot );

		pCryoO2QtyMeter->DefineNeedleGroup( GRP_O2M6Q_VC );
		pCryoO2QtyMeter->SetInitialAnimState( 0.55 );
		pCryoO2QtyMeter->SetMotionRange( 15.0 );
		pCryoO2QtyMeter->SetReference( needle_ref, needle_rot );

		pCryoH2PressMeter->DefineNeedleGroup( GRP_O2M7P_VC );
		pCryoH2PressMeter->SetInitialAnimState( 0.55 );
		pCryoH2PressMeter->SetMotionRange( 15.0 );
		pCryoH2PressMeter->SetReference( needle_ref, needle_rot );

		pCryoH2QtyMeter->DefineNeedleGroup( GRP_O2M7Q_VC );
		pCryoH2QtyMeter->SetInitialAnimState( 0.55 );
		pCryoH2QtyMeter->SetMotionRange( 15.0 );
		pCryoH2QtyMeter->SetReference( needle_ref, needle_rot );

		pFuelCellStackTempMeter->DefineNeedleGroup( GRP_O2M8T_VC );
		pFuelCellStackTempMeter->SetInitialAnimState( 0.5 );
		pFuelCellStackTempMeter->SetMotionRange( 15.0 );
		pFuelCellStackTempMeter->SetReference( needle_ref, needle_rot );

		pCryoO2HtrAssyTemp->DefineSwitchGroup( GRP_O2S7_VC );
		pCryoO2HtrAssyTemp->DefineRotationAngle( 270.0f );
		pCryoO2HtrAssyTemp->SetOffset( 45.0f );
		pCryoO2HtrAssyTemp->SetInitialAnimState( 1.0 );
		pCryoO2HtrAssyTemp->SetReference( _V( -0.0746, 2.7949, 14.3503 ), rotary_switch_rot );
		pCryoO2HtrAssyTemp->SetMouseRegion( 0.098072f, 0.698591f, 0.253288f, 0.816324f );

		pCryoPressQty->DefineSwitchGroup( GRP_O2S8_VC );
		pCryoPressQty->DefineRotationAngle( 120.0f );
		pCryoPressQty->SetOffset( 120.0f );
		pCryoPressQty->SetInitialAnimState( 1.0 );
		pCryoPressQty->SetReference( _V( -0.0031, 2.7910, 14.3511 ), rotary_switch_rot );
		pCryoPressQty->SetMouseRegion( 0.397906f, 0.715321f, 0.576245f, 0.827557f );

		pFuelCellStackTemp->DefineSwitchGroup( GRP_O2S9_VC );
		pFuelCellStackTemp->SetInitialAnimState( 0.5 );
		pFuelCellStackTemp->SetReference( _V( 0.0523, 2.7866, 14.3599 ), switch_rot );
		pFuelCellStackTemp->SetMouseRegion( 0.728346f, 0.749182f, 0.823905f, 0.834874f );
		return;
	}

	void PanelO2::RegisterVC( void )
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea( AID_O2, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_O2, 
			_V( -0.1139, 3.0096, 14.3150 ) + ofs, _V( 0.1082, 3.0096, 14.3150 ) + ofs, 
			_V( -0.1139, 2.7283, 14.3728 ) + ofs, _V( 0.1082, 2.7283, 14.3728 ) + ofs );
		return;
	}

	void PanelO2::Realize( void )
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "PRSD_O2HtrAssyTemp2", 16 );
		pCryoO2HtrAssyTemp->ConnectInputSignal( 0, 9, pBundle, 0 );
		pCryoO2HtrAssyTemp->ConnectInputSignal( 0, 7, pBundle, 2 );
		pCryoO2HtrAssyTemp->ConnectInputSignal( 0, 5, pBundle, 4 );
		pCryoO2HtrAssyTemp->ConnectInputSignal( 0, 3, pBundle, 6 );
		pCryoO2HtrAssyTemp->ConnectInputSignal( 0, 1, pBundle, 8 );

		pBundle = STS()->BundleManager()->CreateBundle( "PRSD_O2HtrAssyTemp1", 16 );
		pCryoO2HtrAssyTemp->ConnectInputSignal( 0, 8, pBundle, 0 );
		pCryoO2HtrAssyTemp->ConnectInputSignal( 0, 6, pBundle, 3 );
		pCryoO2HtrAssyTemp->ConnectInputSignal( 0, 4, pBundle, 5 );
		pCryoO2HtrAssyTemp->ConnectInputSignal( 0, 2, pBundle, 7 );
		pCryoO2HtrAssyTemp->ConnectInputSignal( 0, 0, pBundle, 9 );
		
		pBundle = STS()->BundleManager()->CreateBundle( "PRSD_H2Qty", 16 );
		pCryoPressQty->ConnectInputSignal( 0, 4, pBundle, 0 );
		pCryoPressQty->ConnectInputSignal( 0, 3, pBundle, 1 );
		pCryoPressQty->ConnectInputSignal( 0, 2, pBundle, 2 );
		pCryoPressQty->ConnectInputSignal( 0, 1, pBundle, 3 );
		pCryoPressQty->ConnectInputSignal( 0, 0, pBundle, 4 );
		
		pBundle = STS()->BundleManager()->CreateBundle( "PRSD_H2Press", 16 );
		pCryoPressQty->ConnectInputSignal( 1, 4, pBundle, 0 );
		pCryoPressQty->ConnectInputSignal( 1, 3, pBundle, 1 );
		pCryoPressQty->ConnectInputSignal( 1, 2, pBundle, 2 );
		pCryoPressQty->ConnectInputSignal( 1, 1, pBundle, 3 );
		pCryoPressQty->ConnectInputSignal( 1, 0, pBundle, 4 );
		
		pBundle = STS()->BundleManager()->CreateBundle( "PRSD_O2Qty", 16 );
		pCryoPressQty->ConnectInputSignal( 2, 4, pBundle, 0 );
		pCryoPressQty->ConnectInputSignal( 2, 3, pBundle, 1 );
		pCryoPressQty->ConnectInputSignal( 2, 2, pBundle, 2 );
		pCryoPressQty->ConnectInputSignal( 2, 1, pBundle, 3 );
		pCryoPressQty->ConnectInputSignal( 2, 0, pBundle, 4 );
		
		pBundle = STS()->BundleManager()->CreateBundle( "PRSD_O2Press", 16 );
		pCryoPressQty->ConnectInputSignal( 3, 4, pBundle, 0 );
		pCryoPressQty->ConnectInputSignal( 3, 3, pBundle, 1 );
		pCryoPressQty->ConnectInputSignal( 3, 2, pBundle, 2 );
		pCryoPressQty->ConnectInputSignal( 3, 1, pBundle, 3 );
		pCryoPressQty->ConnectInputSignal( 3, 0, pBundle, 4 );

		pBundle = STS()->BundleManager()->CreateBundle( "PanelO2", 16 );
		pFuelCellStackTemp->ConnectPort( 1, pBundle, 0 );
		pFuelCellStackTemp->ConnectPort( 2, pBundle, 1 );
		pFuelCellStackTemp3.Connect( pBundle, 0 );
		pFuelCellStackTemp1.Connect( pBundle, 1 );

		pCryoO2HtrAssyTemp->ConnectOutputSignal( 0, pBundle, 2 );
		pCryoO2HtrAssyTempMeter->SetInput( pBundle, 2 );

		pCryoPressQty->ConnectOutputSignal( 3, pBundle, 3 );
		pCryoO2PressMeter->SetInput( pBundle, 3 );

		pCryoPressQty->ConnectOutputSignal( 2, pBundle, 4 );
		pCryoO2QtyMeter->SetInput( pBundle, 4 );

		pCryoPressQty->ConnectOutputSignal( 1, pBundle, 5 );
		pCryoH2PressMeter->SetInput( pBundle, 5 );

		pCryoPressQty->ConnectOutputSignal( 0, pBundle, 6 );
		pCryoH2QtyMeter->SetInput( pBundle, 6 );

		pFuelCellStackTempMeter->SetInput( pBundle, 7 );
		pFuelCellStackTempMeterInput.Connect( pBundle, 7 );

		AtlantisPanel::Realize();
		return;
	}

	void PanelO2::OnPreStep( double SimT, double DeltaT, double MJD )
	{
		AtlantisPanel::OnPreStep( SimT, DeltaT, MJD );

		// TODO sensor value conversion to meter range

		if (pFuelCellStackTemp1.IsSet())
		{
			// FC 1
			pFuelCellStackTempMeterInput.SetLine( 1.25f );
		}
		else if (pFuelCellStackTemp3.IsSet())
		{
			// FC 3
			pFuelCellStackTempMeterInput.SetLine( 3.75f );
		}
		else
		{
			// FC 2
			pFuelCellStackTempMeterInput.SetLine( 2.5f );
		}
		return;
	}
}
