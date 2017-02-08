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

		Add( pCryoO2HtrAssyTemp = new RotaryDemuxSwitch( _sts, "Cryo O2 Htr Assy Temp", 10 ) );
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

		Add( pCryoPressQty = new RotaryDemuxSwitch( _sts, "Cryo Press Qty", 5 ) );
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
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "PanelO2_A", 16 );
		pCryoO2HtrAssyTemp->ConnectOutputSignal( 9, pBundle, 0 );
		pCryoO2HtrAssyTemp->ConnectOutputSignal( 8, pBundle, 1 );
		pCryoO2HtrAssyTemp->ConnectOutputSignal( 7, pBundle, 2 );
		pCryoO2HtrAssyTemp->ConnectOutputSignal( 6, pBundle, 3 );
		pCryoO2HtrAssyTemp->ConnectOutputSignal( 5, pBundle, 4 );
		pCryoO2HtrAssyTemp->ConnectOutputSignal( 4, pBundle, 5 );
		pCryoO2HtrAssyTemp->ConnectOutputSignal( 3, pBundle, 6 );
		pCryoO2HtrAssyTemp->ConnectOutputSignal( 2, pBundle, 7 );
		pCryoO2HtrAssyTemp->ConnectOutputSignal( 1, pBundle, 8 );
		pCryoO2HtrAssyTemp->ConnectOutputSignal( 0, pBundle, 9 );
		pCryoO2HtrAssyTempTK1[0].Connect( pBundle, 0 );
		pCryoO2HtrAssyTempTK1[1].Connect( pBundle, 1 );
		pCryoO2HtrAssyTempTK2[0].Connect( pBundle, 2 );
		pCryoO2HtrAssyTempTK2[1].Connect( pBundle, 3 );
		pCryoO2HtrAssyTempTK3[0].Connect( pBundle, 4 );
		pCryoO2HtrAssyTempTK3[1].Connect( pBundle, 5 );
		pCryoO2HtrAssyTempTK4[0].Connect( pBundle, 6 );
		pCryoO2HtrAssyTempTK4[1].Connect( pBundle, 7 );
		pCryoO2HtrAssyTempTK5[0].Connect( pBundle, 8 );
		pCryoO2HtrAssyTempTK5[1].Connect( pBundle, 9 );

		pCryoPressQty->ConnectOutputSignal( 4, pBundle, 10 );
		pCryoPressQty->ConnectOutputSignal( 3, pBundle, 11 );
		pCryoPressQty->ConnectOutputSignal( 2, pBundle, 12 );
		pCryoPressQty->ConnectOutputSignal( 1, pBundle, 13 );
		pCryoPressQty->ConnectOutputSignal( 0, pBundle, 14 );
		pCryoPressQtyTK[0].Connect( pBundle, 10 );
		pCryoPressQtyTK[1].Connect( pBundle, 11 );
		pCryoPressQtyTK[2].Connect( pBundle, 12 );
		pCryoPressQtyTK[3].Connect( pBundle, 13 );
		pCryoPressQtyTK[4].Connect( pBundle, 14 );

		pBundle = STS()->BundleManager()->CreateBundle( "PanelO2_B", 16 );
		pFuelCellStackTemp->ConnectPort( 1, pBundle, 0 );
		pFuelCellStackTemp->ConnectPort( 2, pBundle, 1 );
		pFuelCellStackTemp3.Connect( pBundle, 0 );
		pFuelCellStackTemp1.Connect( pBundle, 1 );

		pCryoO2HtrAssyTempMeter->SetInput( pBundle, 2 );
		pCryoO2PressMeter->SetInput( pBundle, 3 );
		pCryoO2QtyMeter->SetInput( pBundle, 4 );
		pCryoH2PressMeter->SetInput( pBundle, 5 );
		pCryoH2QtyMeter->SetInput( pBundle, 6 );
		pFuelCellStackTempMeter->SetInput( pBundle, 7 );
		pCryoO2HtrAssyTempMeterInput.Connect( pBundle, 2 );
		pCryoO2PressMeterInput.Connect( pBundle, 3 );
		pCryoO2QtyMeterInput.Connect( pBundle, 4 );
		pCryoH2PressMeterInput.Connect( pBundle, 5 );
		pCryoH2QtyMeterInput.Connect( pBundle, 6 );
		pFuelCellStackTempMeterInput.Connect( pBundle, 7 );

		AtlantisPanel::Realize();
		return;
	}

	void PanelO2::OnPreStep( double SimT, double DeltaT, double MJD )
	{
		AtlantisPanel::OnPreStep( SimT, DeltaT, MJD );

		// TODO sensor value conversion to meter range

		if (pCryoO2HtrAssyTempTK1[0].IsSet())
		{
			// TK 1 1
			pCryoO2HtrAssyTempMeterInput.SetLine( 0.45f );
		}
		else if (pCryoO2HtrAssyTempTK1[1].IsSet())
		{
			// TK 1 2
			pCryoO2HtrAssyTempMeterInput.SetLine( 0.91f );
		}
		else if (pCryoO2HtrAssyTempTK2[0].IsSet())
		{
			// TK 2 1
			pCryoO2HtrAssyTempMeterInput.SetLine( 1.36f );
		}
		else if (pCryoO2HtrAssyTempTK2[1].IsSet())
		{
			// TK 2 2
			pCryoO2HtrAssyTempMeterInput.SetLine( 1.82f );
		}
		else if (pCryoO2HtrAssyTempTK3[0].IsSet())
		{
			// TK 3 1
			pCryoO2HtrAssyTempMeterInput.SetLine( 2.27f );
		}
		else if (pCryoO2HtrAssyTempTK3[1].IsSet())
		{
			// TK 3 2
			pCryoO2HtrAssyTempMeterInput.SetLine( 2.73f );
		}
		else if (pCryoO2HtrAssyTempTK4[0].IsSet())
		{
			// TK 4 1
			pCryoO2HtrAssyTempMeterInput.SetLine( 3.18f );
		}
		else if (pCryoO2HtrAssyTempTK4[1].IsSet())
		{
			// TK 4 2
			pCryoO2HtrAssyTempMeterInput.SetLine( 3.64f );
		}
		else if (pCryoO2HtrAssyTempTK5[0].IsSet())
		{
			// TK 5 1
			pCryoO2HtrAssyTempMeterInput.SetLine( 4.09f );
		}
		else //if (pCryoO2HtrAssyTempTK5[1].IsSet())
		{
			// TK 5 2
			pCryoO2HtrAssyTempMeterInput.SetLine( 4.55f );
		}

		if (pCryoPressQtyTK[0].IsSet())
		{
			// TK1
			pCryoO2PressMeterInput.SetLine( 1.0f );
			pCryoO2QtyMeterInput.SetLine( 2.0f );
			pCryoH2PressMeterInput.SetLine( 3.0f );
			pCryoH2QtyMeterInput.SetLine( 4.0f );
		}
		else if (pCryoPressQtyTK[1].IsSet())
		{
			// TK2
			pCryoO2PressMeterInput.SetLine( 2.0f );
			pCryoO2QtyMeterInput.SetLine( 3.0f );
			pCryoH2PressMeterInput.SetLine( 4.0f );
			pCryoH2QtyMeterInput.SetLine( 1.0f );
		}
		else if (pCryoPressQtyTK[2].IsSet())
		{
			// TK3
			pCryoO2PressMeterInput.SetLine( 3.0f );
			pCryoO2QtyMeterInput.SetLine( 4.0f );
			pCryoH2PressMeterInput.SetLine( 1.0f );
			pCryoH2QtyMeterInput.SetLine( 2.0f );
		}
		else if (pCryoPressQtyTK[3].IsSet())
		{
			// TK4
			pCryoO2PressMeterInput.SetLine( 4.0f );
			pCryoO2QtyMeterInput.SetLine( 1.0f );
			pCryoH2PressMeterInput.SetLine( 2.0f );
			pCryoH2QtyMeterInput.SetLine( 3.0f );
		}
		else //if (pCryoPressQtyTK[4].IsSet())
		{
			// TK5
			pCryoO2PressMeterInput.SetLine( 1.0f );
			pCryoO2QtyMeterInput.SetLine( 4.0f );
			pCryoH2PressMeterInput.SetLine( 3.0f );
			pCryoH2QtyMeterInput.SetLine( 2.0f );
		}

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
