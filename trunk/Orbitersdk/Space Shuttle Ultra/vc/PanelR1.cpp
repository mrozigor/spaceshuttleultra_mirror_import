#include "PanelR1.h"
#include "../Atlantis.h"
#include "../Atlantis_defs.h"


namespace vc
{
	PanelR1::PanelR1( Atlantis *_sts ):AtlantisPanel( _sts, "R1" )
	{
		Add( pControlBusPwrMNAPwr = new StdSwitch2( _sts, "Control Bus Pwr MN A Pwr" ) );
		pControlBusPwrMNAPwr->SetLabel( 1, "RESET" );
		Add( pControlBusPwrMNBPwr = new StdSwitch2( _sts, "Control Bus Pwr MN B Pwr" ) );
		pControlBusPwrMNBPwr->SetLabel( 1, "RESET" );
		Add( pControlBusPwrMNCPwr = new StdSwitch2( _sts, "Control Bus Pwr MN C Pwr" ) );
		pControlBusPwrMNCPwr->SetLabel( 1, "RESET" );

		Add( pEssBusSourceMNBC = new StdSwitch2( _sts, "Ess Bus Source MN B/C" ) );
		pEssBusSourceMNBC->SetLabel( 0, "OFF" );
		pEssBusSourceMNBC->SetLabel( 1, "ON" );
		Add( pEssBusSourceMNCA = new StdSwitch2( _sts, "Ess Bus Source MN C/A" ) );
		pEssBusSourceMNCA->SetLabel( 0, "OFF" );
		pEssBusSourceMNCA->SetLabel( 1, "ON" );
		Add( pEssBusSourceMNAB = new StdSwitch2( _sts, "Ess Bus Source MN A/B" ) );
		pEssBusSourceMNAB->SetLabel( 0, "OFF" );
		pEssBusSourceMNAB->SetLabel( 1, "ON" );

		Add( pEssBusSourceFC1 = new StdSwitch2( _sts, "Ess Bus Source FC 1" ) );
		pEssBusSourceFC1->SetLabel( 0, "OFF" );
		pEssBusSourceFC1->SetLabel( 1, "ON" );
		Add( pEssBusSourceFC2 = new StdSwitch2( _sts, "Ess Bus Source FC 2" ) );
		pEssBusSourceFC2->SetLabel( 0, "OFF" );
		pEssBusSourceFC2->SetLabel( 1, "ON" );
		Add( pEssBusSourceFC3 = new StdSwitch2( _sts, "Ess Bus Source FC 3" ) );
		pEssBusSourceFC3->SetLabel( 0, "OFF" );
		pEssBusSourceFC3->SetLabel( 1, "ON" );

		Add( pFCMainBusA = new LockableLever3( _sts, "FC/Main Bus A" ) );
		pFCMainBusA->SetLabel( 0, "OFF" );
		pFCMainBusA->SetLabel( 1, "ON" );
		Add( pFCMainBusB = new LockableLever3( _sts, "FC/Main Bus B" ) );
		pFCMainBusB->SetLabel( 0, "OFF" );
		pFCMainBusB->SetLabel( 1, "ON" );
		Add( pFCMainBusC = new LockableLever3( _sts, "FC/Main Bus C" ) );
		pFCMainBusC->SetLabel( 0, "OFF" );
		pFCMainBusC->SetLabel( 1, "ON" );

		Add( pMNBusTieA = new StdSwitch3( _sts, "MN Bus Tie A" ) );
		pMNBusTieA->SetLabel( 0, "OFF" );
		pMNBusTieA->SetLabel( 2, "ON" );
		Add( pMNBusTieB = new StdSwitch3( _sts, "MN Bus Tie B" ) );
		pMNBusTieB->SetLabel( 0, "OFF" );
		pMNBusTieB->SetLabel( 2, "ON" );
		Add( pMNBusTieC = new StdSwitch3( _sts, "MN Bus Tie C" ) );
		pMNBusTieC->SetLabel( 0, "OFF" );
		pMNBusTieC->SetLabel( 2, "ON" );

		Add( pInvPwr1 = new StdSwitch3( _sts, "Inv Pwr 1" ) );
		pInvPwr1->SetLabel( 0, "OFF" );
		pInvPwr1->SetLabel( 2, "ON" );
		Add( pInvPwr2 = new StdSwitch3( _sts, "Inv Pwr 2" ) );
		pInvPwr2->SetLabel( 0, "OFF" );
		pInvPwr2->SetLabel( 2, "ON" );
		Add( pInvPwr3 = new StdSwitch3( _sts, "Inv Pwr 3" ) );
		pInvPwr3->SetLabel( 0, "OFF" );
		pInvPwr3->SetLabel( 2, "ON" );

		Add( pInvACBus1 = new StdSwitch3( _sts, "Inv/AC Bus 1" ) );
		pInvACBus1->SetLabel( 0, "OFF" );
		pInvACBus1->SetLabel( 2, "ON" );
		Add( pInvACBus2 = new StdSwitch3( _sts, "Inv/AC Bus 2" ) );
		pInvACBus2->SetLabel( 0, "OFF" );
		pInvACBus2->SetLabel( 2, "ON" );
		Add( pInvACBus3 = new StdSwitch3( _sts, "Inv/AC Bus 3" ) );
		pInvACBus3->SetLabel( 0, "OFF" );
		pInvACBus3->SetLabel( 2, "ON" );

		Add( pACBusSnsr1 = new StdSwitch3( _sts, "AC Bus Snsr 1" ) );
		pACBusSnsr1->SetLabel( 0, "MONITOR" );
		pACBusSnsr1->SetLabel( 1, "OFF" );
		pACBusSnsr1->SetLabel( 2, "AUTO TRIP" );
		Add( pACBusSnsr2 = new StdSwitch3( _sts, "AC Bus Snsr 2" ) );
		pACBusSnsr2->SetLabel( 0, "MONITOR" );
		pACBusSnsr2->SetLabel( 1, "OFF" );
		pACBusSnsr2->SetLabel( 2, "AUTO TRIP" );
		Add( pACBusSnsr3 = new StdSwitch3( _sts, "AC Bus Snsr 3" ) );
		pACBusSnsr3->SetLabel( 0, "MONITOR" );
		pACBusSnsr3->SetLabel( 1, "OFF" );
		pACBusSnsr3->SetLabel( 2, "AUTO TRIP" );

		Add( pPayloadCabin = new StdSwitch3( _sts, "Payload Cabin" ) );
		pPayloadCabin->SetLabel( 0, "MN B" );
		pPayloadCabin->SetLabel( 1, "OFF" );
		pPayloadCabin->SetLabel( 2, "MN A" );

		Add( pPayloadPriMNB = new StdSwitch3( _sts, "Payload Pri MN B" ) );
		pPayloadPriMNB->SetLabel( 0, "OFF" );
		pPayloadPriMNB->SetLabel( 2, "ON" );

		Add( pPayloadPriFC3 = new StdSwitch3( _sts, "Payload Pri FC 3" ) );
		pPayloadPriFC3->SetLabel( 0, "OFF" );
		pPayloadPriFC3->SetLabel( 2, "ON" );

		Add( pPayloadPriMNC = new StdSwitch3( _sts, "Payload Pri MN C" ) );
		pPayloadPriMNC->SetLabel( 0, "OFF" );
		pPayloadPriMNC->SetLabel( 2, "ON" );

		Add( pPayloadAuxCover = new StandardSwitchCover( _sts, "Payload Aux Cover" ) );

		Add( pPayloadAux = new StdSwitch2( _sts, "Payload Aux" ) );
		pPayloadAux->SetLabel( 0, "OFF" );
		pPayloadAux->SetLabel( 1, "ON" );

		Add( pPayloadAftMNB = new StdSwitch2( _sts, "Payload Aft MN B" ) );
		pPayloadAftMNB->SetLabel( 0, "OFF" );
		pPayloadAftMNB->SetLabel( 1, "ON" );

		Add( pPayloadAftMNC = new StdSwitch2( _sts, "Payload Aft MN C" ) );
		pPayloadAftMNC->SetLabel( 0, "OFF" );
		pPayloadAftMNC->SetLabel( 1, "ON" );

		Add( pFuelCellReac1 = new StdSwitch3( _sts, "Fuel Cell Reac 1" ) );
		pFuelCellReac1->SetLabel( 0, "CLOSE" );
		pFuelCellReac1->SetLabel( 2, "OPEN" );

		Add( pFuelCellReac2 = new StdSwitch3( _sts, "Fuel Cell Reac 2" ) );
		pFuelCellReac2->SetLabel( 0, "CLOSE" );
		pFuelCellReac2->SetLabel( 2, "OPEN" );

		Add( pFuelCellReac3 = new StdSwitch3( _sts, "Fuel Cell Reac 3" ) );
		pFuelCellReac3->SetLabel( 0, "CLOSE" );
		pFuelCellReac3->SetLabel( 2, "OPEN" );

		Add( pO2ManifoldVlvTank1 = new StdSwitch3( _sts, "O2 Manifold Vlv Tank 1" ) );
		pO2ManifoldVlvTank1->SetLabel( 0, "CLOSE" );
		pO2ManifoldVlvTank1->SetLabel( 2, "OPEN" );

		Add( pO2ManifoldVlvTank2 = new StdSwitch3( _sts, "O2 Manifold Vlv Tank 2" ) );
		pO2ManifoldVlvTank2->SetLabel( 0, "CLOSE" );
		pO2ManifoldVlvTank2->SetLabel( 2, "OPEN" );

		Add( pH2ManifoldVlvTank1 = new StdSwitch3( _sts, "H2 Manifold Vlv Tank 1" ) );
		pH2ManifoldVlvTank1->SetLabel( 0, "CLOSE" );
		pH2ManifoldVlvTank1->SetLabel( 2, "OPEN" );

		Add( pH2ManifoldVlvTank2 = new StdSwitch3( _sts, "H2 Manifold Vlv Tank 2" ) );
		pH2ManifoldVlvTank2->SetLabel( 0, "CLOSE" );
		pH2ManifoldVlvTank2->SetLabel( 2, "OPEN" );

		Add( pO2TK1HeatersA = new StdSwitch3( _sts, "O2 TK1 Heaters A" ) );
		pO2TK1HeatersA->SetLabel( 0, "ON" );
		pO2TK1HeatersA->SetLabel( 1, "OFF" );
		pO2TK1HeatersA->SetLabel( 2, "AUTO" );

		Add( pO2TK1HeatersB = new StdSwitch3( _sts, "O2 TK1 Heaters B" ) );
		pO2TK1HeatersB->SetLabel( 0, "ON" );
		pO2TK1HeatersB->SetLabel( 1, "OFF" );
		pO2TK1HeatersB->SetLabel( 2, "AUTO" );

		Add( pO2TK1Heaters = new StdSwitch3( _sts, "O2 TK1 Heaters" ) );
		pO2TK1Heaters->SetLabel( 0, "TEST" );
		pO2TK1Heaters->SetLabel( 2, "RESET" );

		Add( pO2TK2HeatersA = new StdSwitch3( _sts, "O2 TK2 Heaters A" ) );
		pO2TK2HeatersA->SetLabel( 0, "ON" );
		pO2TK2HeatersA->SetLabel( 1, "OFF" );
		pO2TK2HeatersA->SetLabel( 2, "AUTO" );

		Add( pO2TK2HeatersB = new StdSwitch3( _sts, "O2 TK2 Heaters B" ) );
		pO2TK2HeatersB->SetLabel( 0, "ON" );
		pO2TK2HeatersB->SetLabel( 1, "OFF" );
		pO2TK2HeatersB->SetLabel( 2, "AUTO" );

		Add( pO2TK2Heaters = new StdSwitch3( _sts, "O2 TK2 Heaters" ) );
		pO2TK2Heaters->SetLabel( 0, "TEST" );
		pO2TK2Heaters->SetLabel( 2, "RESET" );

		Add( pO2TK3HeatersA = new StdSwitch3( _sts, "O2 TK3 Heaters A" ) );
		pO2TK3HeatersA->SetLabel( 0, "ON" );
		pO2TK3HeatersA->SetLabel( 1, "OFF" );
		pO2TK3HeatersA->SetLabel( 2, "AUTO" );

		Add( pO2TK3HeatersB = new StdSwitch3( _sts, "O2 TK3 Heaters B" ) );
		pO2TK3HeatersB->SetLabel( 0, "ON" );
		pO2TK3HeatersB->SetLabel( 1, "OFF" );
		pO2TK3HeatersB->SetLabel( 2, "AUTO" );

		Add( pO2TK3Heaters = new StdSwitch3( _sts, "O2 TK3 Heaters" ) );
		pO2TK3Heaters->SetLabel( 0, "TEST" );
		pO2TK3Heaters->SetLabel( 2, "RESET" );

		Add( pH2TK1HeatersA = new StdSwitch3( _sts, "H2 TK1 Heaters A" ) );
		pH2TK1HeatersA->SetLabel( 0, "ON" );
		pH2TK1HeatersA->SetLabel( 1, "OFF" );
		pH2TK1HeatersA->SetLabel( 2, "AUTO" );

		Add( pH2TK1HeatersB = new StdSwitch3( _sts, "H2 TK1 Heaters B" ) );
		pH2TK1HeatersB->SetLabel( 0, "ON" );
		pH2TK1HeatersB->SetLabel( 1, "OFF" );
		pH2TK1HeatersB->SetLabel( 2, "AUTO" );

		Add( pH2TK2HeatersA = new StdSwitch3( _sts, "H2 TK2 Heaters A" ) );
		pH2TK2HeatersA->SetLabel( 0, "ON" );
		pH2TK2HeatersA->SetLabel( 1, "OFF" );
		pH2TK2HeatersA->SetLabel( 2, "AUTO" );

		Add( pH2TK2HeatersB = new StdSwitch3( _sts, "H2 TK2 Heaters B" ) );
		pH2TK2HeatersB->SetLabel( 0, "ON" );
		pH2TK2HeatersB->SetLabel( 1, "OFF" );
		pH2TK2HeatersB->SetLabel( 2, "AUTO" );

		Add( pH2TK3HeatersA = new StdSwitch3( _sts, "H2 TK3 Heaters A" ) );
		pH2TK3HeatersA->SetLabel( 0, "ON" );
		pH2TK3HeatersA->SetLabel( 1, "OFF" );
		pH2TK3HeatersA->SetLabel( 2, "AUTO" );

		Add( pH2TK3HeatersB = new StdSwitch3( _sts, "H2 TK3 Heaters B" ) );
		pH2TK3HeatersB->SetLabel( 0, "ON" );
		pH2TK3HeatersB->SetLabel( 1, "OFF" );
		pH2TK3HeatersB->SetLabel( 2, "AUTO" );

		Add( pFuelCell1 = new LockableLever3( _sts, "Fuel Cell 1" ) );
		pFuelCell1->SetLabel( 0, "STOP" );
		pFuelCell1->SetLabel( 2, "START" );

		Add( pFuelCell2 = new LockableLever3( _sts, "Fuel Cell 2" ) );
		pFuelCell2->SetLabel( 0, "STOP" );
		pFuelCell2->SetLabel( 2, "START" );

		Add( pFuelCell3 = new LockableLever3( _sts, "Fuel Cell 3" ) );
		pFuelCell3->SetLabel( 0, "STOP" );
		pFuelCell3->SetLabel( 2, "START" );

		Add( pFCMainBusATB = new StandardTalkback2( _sts, "FC Main Bus A TB" ) );
		
		Add( pFCMainBusBTB = new StandardTalkback2( _sts, "FC Main Bus B TB" ) );
		
		Add( pFCMainBusCTB = new StandardTalkback2( _sts, "FC Main Bus C TB" ) );
		
		Add( pMNBusTieATB = new StandardTalkback2( _sts, "MN Bus Tie A TB" ) );
		
		Add( pMNBusTieBTB = new StandardTalkback2( _sts, "MN Bus Tie B TB" ) );
		
		Add( pMNBusTieCTB = new StandardTalkback2( _sts, "MN Bus Tie C TB" ) );
		
		Add( pInvPwr1TB = new StandardTalkback2( _sts, "Inv Pwr 1 TB" ) );
		
		Add( pInvPwr2TB = new StandardTalkback2( _sts, "Inv Pwr 2 TB" ) );
		
		Add( pInvPwr3TB = new StandardTalkback2( _sts, "Inv Pwr 3 TB" ) );
		
		Add( pInvACBus1TB = new StandardTalkback2( _sts, "Inv AC Bus 1 TB" ) );
		
		Add( pInvACBus2TB = new StandardTalkback2( _sts, "Inv AC Bus 2 TB" ) );

		Add( pInvACBus3TB = new StandardTalkback2( _sts, "Inv AC Bus 3 TB" ) );

		Add( pPayloadPriMNBTB = new StandardTalkback2( _sts, "Payload Pri MN B TB" ) );
		
		Add( pPayloadPriFC3TB = new StandardTalkback2( _sts, "Payload Pri FC 3 TB" ) );

		Add( pPayloadPriMNCTB = new StandardTalkback2( _sts, "Payload Pri MN C TB" ) );

		Add( pFuelCell1ReacO2 = new StandardTalkback2( _sts, "Fuel Cell 1 Reac O2" ) );
		
		Add( pFuelCell2ReacO2 = new StandardTalkback2( _sts, "Fuel Cell 2 Reac O2" ) );

		Add( pFuelCell3ReacO2 = new StandardTalkback2( _sts, "Fuel Cell 3 Reac O2" ) );

		Add( pFuelCell1ReacH2 = new StandardTalkback2( _sts, "Fuel Cell 1 Reac H2" ) );
		
		Add( pFuelCell2ReacH2 = new StandardTalkback2( _sts, "Fuel Cell 2 Reac H2" ) );

		Add( pFuelCell3ReacH2 = new StandardTalkback2( _sts, "Fuel Cell 3 Reac H2" ) );
		
		Add( pO2ManifoldVlvTank1TB = new StandardTalkback2( _sts, "O2 Manifold Vlv Tank 1 TB" ) );

		Add( pO2ManifoldVlvTank2TB = new StandardTalkback2( _sts, "O2 Manifold Vlv Tank 2 TB" ) );

		Add( pH2ManifoldVlvTank1TB = new StandardTalkback2( _sts, "H2 Manifold Vlv Tank 1 TB" ) );

		Add( pH2ManifoldVlvTank2TB = new StandardTalkback2( _sts, "H2 Manifold Vlv Tank 2 TB" ) );

		Add( pFuelCellReadyForLoad1 = new StandardTalkback2( _sts, "Fuel Cell Ready For Load 1" ) );
		
		Add( pFuelCellReadyForLoad2 = new StandardTalkback2( _sts, "Fuel Cell Ready For Load 2" ) );

		Add( pFuelCellReadyForLoad3 = new StandardTalkback2( _sts, "Fuel Cell Ready For Load 3" ) );

		Add( pFuelCellCoolantPumpdP1 = new StandardTalkback2( _sts, "Fuel Cell Coolant Pump dP 1" ) );
		
		Add( pFuelCellCoolantPumpdP2 = new StandardTalkback2( _sts, "Fuel Cell Coolant Pump dP 2" ) );

		Add( pFuelCellCoolantPumpdP3 = new StandardTalkback2( _sts, "Fuel Cell Coolant Pump dP 3" ) );
	}

	PanelR1::~PanelR1()
	{
	}

	void PanelR1::DefineVC( void )
	{
		VECTOR3 switch_rot = _V( 0.33298, -0.00978, -0.9404 );
		VECTOR3 switch_pull = _V( -0.9171, 0.2194, -0.3328 );

		AddAIDToMouseEventList( AID_R1 );

		pControlBusPwrMNAPwr->SetInitialAnimState( 0.5f );
		pControlBusPwrMNAPwr->DefineSwitchGroup( GRP_R1S1_VC );
		pControlBusPwrMNAPwr->SetReference( _V( 1.1925, 2.1526, 14.6480 ), switch_rot );
		pControlBusPwrMNAPwr->SetMouseRegion( 0.069446f, 0.166435f, 0.088014f, 0.233550f );
		pControlBusPwrMNAPwr->SetSpringLoaded( true );

		pControlBusPwrMNBPwr->SetInitialAnimState( 0.5f );
		pControlBusPwrMNBPwr->DefineSwitchGroup( GRP_R1S2_VC );
		pControlBusPwrMNBPwr->SetReference( _V( 1.1797, 2.0951, 14.6441 ), switch_rot );
		pControlBusPwrMNBPwr->SetMouseRegion( 0.057635f, 0.339876f, 0.077927f, 0.408386f );
		pControlBusPwrMNBPwr->SetSpringLoaded( true );

		pControlBusPwrMNCPwr->SetInitialAnimState( 0.5f );
		pControlBusPwrMNCPwr->DefineSwitchGroup( GRP_R1S3_VC );
		pControlBusPwrMNCPwr->SetReference( _V( 1.1664, 2.0381, 14.6399 ), switch_rot );
		pControlBusPwrMNCPwr->SetMouseRegion( 0.047596f, 0.514156f, 0.068455f, 0.585026f );
		pControlBusPwrMNCPwr->SetSpringLoaded( true );

		pEssBusSourceMNBC->SetInitialAnimState( 0.5f );
		pEssBusSourceMNBC->DefineSwitchGroup( GRP_R1S4_VC );
		pEssBusSourceMNBC->SetReference( _V( 1.1925, 2.1526, 14.6480 ), switch_rot );
		pEssBusSourceMNBC->SetMouseRegion( 0.119187f, 0.164126f, 0.137888f, 0.233977f );

		pEssBusSourceMNCA->SetInitialAnimState( 0.5f );
		pEssBusSourceMNCA->DefineSwitchGroup( GRP_R1S5_VC );
		pEssBusSourceMNCA->SetReference( _V( 1.1797, 2.0951, 14.6441 ), switch_rot );
		pEssBusSourceMNCA->SetMouseRegion( 0.107904f, 0.340237f, 0.125902f, 0.408059f );

		pEssBusSourceMNAB->SetInitialAnimState( 0.5f );
		pEssBusSourceMNAB->DefineSwitchGroup( GRP_R1S6_VC );
		pEssBusSourceMNAB->SetReference( _V( 1.1664, 2.0381, 14.6399 ), switch_rot );
		pEssBusSourceMNAB->SetMouseRegion( 0.096658f, 0.514786f, 0.115279f, 0.582150f );

		pEssBusSourceFC1->SetInitialAnimState( 0.5f );
		pEssBusSourceFC1->DefineSwitchGroup( GRP_R1S7_VC );
		pEssBusSourceFC1->SetReference( _V( 1.1925, 2.1526, 14.6480 ), switch_rot );
		pEssBusSourceFC1->SetMouseRegion( 0.169292f, 0.166575f, 0.187370f, 0.236009f );

		pEssBusSourceFC2->SetInitialAnimState( 0.5f );
		pEssBusSourceFC2->DefineSwitchGroup( GRP_R1S8_VC );
		pEssBusSourceFC2->SetReference( _V( 1.1797, 2.0951, 14.6441 ), switch_rot );
		pEssBusSourceFC2->SetMouseRegion( 0.158506f, 0.342708f, 0.177224f, 0.410471f );

		pEssBusSourceFC3->SetInitialAnimState( 0.5f );
		pEssBusSourceFC3->DefineSwitchGroup( GRP_R1S9_VC );
		pEssBusSourceFC3->SetReference( _V( 1.1664, 2.0381, 14.6399 ), switch_rot );
		pEssBusSourceFC3->SetMouseRegion( 0.147168f, 0.512602f, 0.165938f, 0.581663f );

		pFCMainBusA->SetInitialAnimState( 0.5f );
		pFCMainBusA->DefineSwitchGroup( GRP_R1S10_VC );
		pFCMainBusA->SetReference( _V( 1.1925, 2.1526, 14.6480 ), switch_rot );
		pFCMainBusA->SetPullDirection( switch_pull );
		pFCMainBusA->SetMouseRegion( 0.218102f, 0.164787f, 0.238058f, 0.234013f );
		pFCMainBusA->SetSpringLoaded( true, 0 );
		pFCMainBusA->SetSpringLoaded( true, 2 );

		pFCMainBusB->SetInitialAnimState( 0.5f );
		pFCMainBusB->DefineSwitchGroup( GRP_R1S11_VC );
		pFCMainBusB->SetReference( _V( 1.1797, 2.0951, 14.6441 ), switch_rot );
		pFCMainBusB->SetPullDirection( switch_pull );
		pFCMainBusB->SetMouseRegion( 0.207947f, 0.340869f, 0.228262f, 0.409300f );
		pFCMainBusB->SetSpringLoaded( true, 0 );
		pFCMainBusB->SetSpringLoaded( true, 2 );

		pFCMainBusC->SetInitialAnimState( 0.5f );
		pFCMainBusC->DefineSwitchGroup( GRP_R1S12_VC );
		pFCMainBusC->SetReference( _V( 1.1664, 2.0381, 14.6399 ), switch_rot );
		pFCMainBusC->SetPullDirection( switch_pull );
		pFCMainBusC->SetMouseRegion( 0.196878f, 0.513641f, 0.217177f, 0.579977f );
		pFCMainBusC->SetSpringLoaded( true, 0 );
		pFCMainBusC->SetSpringLoaded( true, 2 );

		pMNBusTieA->SetInitialAnimState( 0.5f );
		pMNBusTieA->DefineSwitchGroup( GRP_R1S13_VC );
		pMNBusTieA->SetReference( _V( 1.1925, 2.1526, 14.6480 ), switch_rot );
		pMNBusTieA->SetMouseRegion( 0.286649f, 0.163550f, 0.306875f, 0.233598f );
		pMNBusTieA->SetSpringLoaded( true, 0 );
		pMNBusTieA->SetSpringLoaded( true, 2 );

		pMNBusTieB->SetInitialAnimState( 0.5f );
		pMNBusTieB->DefineSwitchGroup( GRP_R1S14_VC );
		pMNBusTieB->SetReference( _V( 1.1797, 2.0951, 14.6441 ), switch_rot );
		pMNBusTieB->SetMouseRegion( 0.274631f, 0.339246f, 0.294351f, 0.409492f );
		pMNBusTieB->SetSpringLoaded( true, 0 );
		pMNBusTieB->SetSpringLoaded( true, 2 );

		pMNBusTieC->SetInitialAnimState( 0.5f );
		pMNBusTieC->DefineSwitchGroup( GRP_R1S15_VC );
		pMNBusTieC->SetReference( _V( 1.1664, 2.0381, 14.6399 ), switch_rot );
		pMNBusTieC->SetMouseRegion( 0.265610f, 0.515440f, 0.284506f, 0.582897f );
		pMNBusTieC->SetSpringLoaded( true, 0 );
		pMNBusTieC->SetSpringLoaded( true, 2 );

		pInvPwr1->SetInitialAnimState( 0.5f );
		pInvPwr1->DefineSwitchGroup( GRP_R1S16_VC );
		pInvPwr1->SetReference( _V( 1.2845, 2.1147, 14.3638 ), switch_rot );
		pInvPwr1->SetMouseRegion( 0.364295f, 0.267669f, 0.382682f, 0.340316f );
		pInvPwr1->SetSpringLoaded( true, 0 );
		pInvPwr1->SetSpringLoaded( true, 2 );

		pInvPwr2->SetInitialAnimState( 0.5f );
		pInvPwr2->DefineSwitchGroup( GRP_R1S17_VC );
		pInvPwr2->SetReference( _V( 1.2654, 2.0300, 14.3561 ), switch_rot );
		pInvPwr2->SetMouseRegion( 0.348628f, 0.527914f, 0.368419f, 0.600194f );
		pInvPwr2->SetSpringLoaded( true, 0 );
		pInvPwr2->SetSpringLoaded( true, 2 );

		pInvPwr3->SetInitialAnimState( 0.5f );
		pInvPwr3->DefineSwitchGroup( GRP_R1S18_VC );
		pInvPwr3->SetReference( _V( 1.2465, 1.9432, 14.3505 ), switch_rot );
		pInvPwr3->SetMouseRegion( 0.331337f, 0.788743f, 0.351055f, 0.862777f );
		pInvPwr3->SetSpringLoaded( true, 0 );
		pInvPwr3->SetSpringLoaded( true, 2 );

		pInvACBus1->SetInitialAnimState( 0.5f );
		pInvACBus1->DefineSwitchGroup( GRP_R1S19_VC );
		pInvACBus1->SetReference( _V( 1.2845, 2.1147, 14.3638 ), switch_rot );
		pInvACBus1->SetMouseRegion( 0.411120f, 0.266515f, 0.431888f, 0.342026f );
		pInvACBus1->SetSpringLoaded( true, 0 );
		pInvACBus1->SetSpringLoaded( true, 2 );

		pInvACBus2->SetInitialAnimState( 0.5f );
		pInvACBus2->DefineSwitchGroup( GRP_R1S20_VC );
		pInvACBus2->SetReference( _V( 1.2654, 2.0300, 14.3561 ), switch_rot );
		pInvACBus2->SetMouseRegion( 0.396130f, 0.527200f, 0.415532f, 0.600780f );
		pInvACBus2->SetSpringLoaded( true, 0 );
		pInvACBus2->SetSpringLoaded( true, 2 );

		pInvACBus3->SetInitialAnimState( 0.5f );
		pInvACBus3->DefineSwitchGroup( GRP_R1S21_VC );
		pInvACBus3->SetReference( _V( 1.2465, 1.9432, 14.3505 ), switch_rot );
		pInvACBus3->SetMouseRegion( 0.378402f, 0.789128f, 0.397712f, 0.863190f );
		pInvACBus3->SetSpringLoaded( true, 0 );
		pInvACBus3->SetSpringLoaded( true, 2 );

		pACBusSnsr1->SetInitialAnimState( 0.5f );
		pACBusSnsr1->DefineSwitchGroup( GRP_R1S22_VC );
		pACBusSnsr1->SetReference( _V( 1.3400, 2.1310, 14.2199 ), switch_rot );
		pACBusSnsr1->SetMouseRegion( 0.520422f, 0.216326f, 0.541343f, 0.293934f );
		
		pACBusSnsr2->SetInitialAnimState( 0.5f );
		pACBusSnsr2->DefineSwitchGroup( GRP_R1S23_VC );
		pACBusSnsr2->SetReference( _V( 1.3203, 2.0466, 14.2126 ), switch_rot );
		pACBusSnsr2->SetMouseRegion( 0.504962f, 0.477589f, 0.525558f, 0.550331f );
		
		pACBusSnsr3->SetInitialAnimState( 0.5f );
		pACBusSnsr3->DefineSwitchGroup( GRP_R1S24_VC );
		pACBusSnsr3->SetReference( _V( 1.3017, 1.9596, 14.2075 ), switch_rot );
		pACBusSnsr3->SetMouseRegion( 0.486880f, 0.734950f, 0.507597f, 0.812667f );

		pPayloadCabin->SetInitialAnimState( 0.5f );
		pPayloadCabin->DefineSwitchGroup( GRP_R1S25_VC );
		pPayloadCabin->SetReference( _V( 1.2465, 1.9432, 14.3505 ), switch_rot );
		pPayloadCabin->SetMouseRegion( 0.045919f, 0.787858f, 0.065093f, 0.860649f );

		pPayloadPriMNB->SetInitialAnimState( 0.5f );
		pPayloadPriMNB->DefineSwitchGroup( GRP_R1S26_VC );
		pPayloadPriMNB->SetReference( _V( 1.2465, 1.9432, 14.3505 ), switch_rot );
		pPayloadPriMNB->SetMouseRegion( 0.093539f, 0.784247f, 0.111473f, 0.861887f );
		pPayloadPriMNB->SetSpringLoaded( true, 0 );
		pPayloadPriMNB->SetSpringLoaded( true, 2 );

		pPayloadPriFC3->SetInitialAnimState( 0.5f );
		pPayloadPriFC3->DefineSwitchGroup( GRP_R1S27_VC );
		pPayloadPriFC3->SetReference( _V( 1.2465, 1.9432, 14.3505 ), switch_rot );
		pPayloadPriFC3->SetMouseRegion( 0.124323f, 0.784175f, 0.142225f, 0.864527f );
		pPayloadPriFC3->SetSpringLoaded( true, 0 );
		pPayloadPriFC3->SetSpringLoaded( true, 2 );

		pPayloadPriMNC->SetInitialAnimState( 0.5f );
		pPayloadPriMNC->DefineSwitchGroup( GRP_R1S28_VC );
		pPayloadPriMNC->SetReference( _V( 1.2465, 1.9432, 14.3505 ), switch_rot );
		pPayloadPriMNC->SetMouseRegion( 0.154036f, 0.786062f, 0.172829f, 0.862178f );
		pPayloadPriMNC->SetSpringLoaded( true, 0 );
		pPayloadPriMNC->SetSpringLoaded( true, 2 );

		pPayloadAuxCover->SetMouseRegion( 0, 0.200874f, 0.776744f, 0.219259f, 0.864713f );
		pPayloadAuxCover->SetMouseRegion( 1, 0.203431f, 0.732213f, 0.225585f, 0.768312f );
		pPayloadAuxCover->DefineCoverGroup( GRP_R1SCOVER1_VC );
		pPayloadAuxCover->SetReference( _V( 1.2095, 1.9622, 14.4680 ), switch_rot );

		pPayloadAux->SetInitialAnimState( 0.5f );
		pPayloadAux->DefineSwitchGroup( GRP_R1S29_VC );
		pPayloadAux->SetReference( _V( 1.2465, 1.9432, 14.3505 ), switch_rot );
		pPayloadAux->SetMouseRegion( 0.199678f, 0.790226f, 0.218994f, 0.863155f );

		pPayloadAftMNB->SetInitialAnimState( 0.5f );
		pPayloadAftMNB->DefineSwitchGroup( GRP_R1S30_VC );
		pPayloadAftMNB->SetReference( _V( 1.2465, 1.9432, 14.3505 ), switch_rot );
		pPayloadAftMNB->SetMouseRegion( 0.246471f, 0.790336f, 0.265298f, 0.863592f );

		pPayloadAftMNC->SetInitialAnimState( 0.5f );
		pPayloadAftMNC->DefineSwitchGroup( GRP_R1S31_VC );
		pPayloadAftMNC->SetReference( _V( 1.2465, 1.9432, 14.3505 ), switch_rot );
		pPayloadAftMNC->SetMouseRegion( 0.277791f, 0.791175f, 0.295811f, 0.864617f );

		pFuelCellReac1->SetInitialAnimState( 0.5f );
		pFuelCellReac1->DefineSwitchGroup( GRP_R1CS1_VC );
		pFuelCellReac1->SetReference( _V( 1.4330, 2.1530, 13.9740 ), switch_rot );
		pFuelCellReac1->SetMouseRegion( 0.789772f, 0.139809f, 0.809598f, 0.215341f );
		pFuelCellReac1->SetSpringLoaded( true, 0 );
		pFuelCellReac1->SetSpringLoaded( true, 2 );

		pFuelCellReac2->SetInitialAnimState( 0.5f );
		pFuelCellReac2->DefineSwitchGroup( GRP_R1CS7_VC );
		pFuelCellReac2->SetReference( _V( 1.4115, 2.0517, 13.9664 ), switch_rot );
		pFuelCellReac2->SetMouseRegion( 0.768949f, 0.445541f, 0.788966f, 0.522213f );
		pFuelCellReac2->SetSpringLoaded( true, 0 );
		pFuelCellReac2->SetSpringLoaded( true, 2 );

		pFuelCellReac3->SetInitialAnimState( 0.5f );
		pFuelCellReac3->DefineSwitchGroup( GRP_R1CS4_VC );
		pFuelCellReac3->SetReference( _V( 1.4225, 2.1024, 13.9698 ), switch_rot );
		pFuelCellReac3->SetMouseRegion( 0.779078f, 0.292172f, 0.799463f, 0.370995f );
		pFuelCellReac3->SetSpringLoaded( true, 0 );
		pFuelCellReac3->SetSpringLoaded( true, 2 );

		pO2ManifoldVlvTank1->SetInitialAnimState( 0.5f );
		pO2ManifoldVlvTank1->DefineSwitchGroup( GRP_R1CS2_VC );
		pO2ManifoldVlvTank1->SetReference( _V( 1.3974, 2.1473, 14.0685 ), switch_rot );
		pO2ManifoldVlvTank1->SetMouseRegion( 0.686635f, 0.159957f, 0.707208f, 0.235806f );
		pO2ManifoldVlvTank1->SetSpringLoaded( true, 0 );
		pO2ManifoldVlvTank1->SetSpringLoaded( true, 2 );

		pO2ManifoldVlvTank2->SetInitialAnimState( 0.5f );
		pO2ManifoldVlvTank2->DefineSwitchGroup( GRP_R1CS5_VC );
		pO2ManifoldVlvTank2->SetReference( _V( 1.4225, 2.1024, 13.9698 ), switch_rot );
		pO2ManifoldVlvTank2->SetMouseRegion( 0.677626f, 0.292308f, 0.698955f, 0.365463f );
		pO2ManifoldVlvTank2->SetSpringLoaded( true, 0 );
		pO2ManifoldVlvTank2->SetSpringLoaded( true, 2 );

		pH2ManifoldVlvTank1->SetInitialAnimState( 0.5f );
		pH2ManifoldVlvTank1->DefineSwitchGroup( GRP_R1CS3_VC );
		pH2ManifoldVlvTank1->SetReference( _V( 1.3974, 2.1473, 14.0685 ), switch_rot );
		pH2ManifoldVlvTank1->SetMouseRegion( 0.887703f, 0.163310f, 0.907984f, 0.242464f );
		pH2ManifoldVlvTank1->SetSpringLoaded( true, 0 );
		pH2ManifoldVlvTank1->SetSpringLoaded( true, 2 );

		pH2ManifoldVlvTank2->SetInitialAnimState( 0.5f );
		pH2ManifoldVlvTank2->DefineSwitchGroup( GRP_R1CS6_VC );
		pH2ManifoldVlvTank2->SetReference( _V( 1.4225, 2.1024, 13.9698 ), switch_rot );
		pH2ManifoldVlvTank2->SetMouseRegion( 0.879691f, 0.294468f, 0.899239f, 0.372147f );
		pH2ManifoldVlvTank2->SetSpringLoaded( true, 0 );
		pH2ManifoldVlvTank2->SetSpringLoaded( true, 2 );

		pO2TK1HeatersA->SetInitialAnimState( 0.5f );
		pO2TK1HeatersA->DefineSwitchGroup( GRP_R1CS8_VC );
		pO2TK1HeatersA->SetReference( _V( 1.3533, 2.0372, 14.1175 ), switch_rot );
		pO2TK1HeatersA->SetMouseRegion( 0.604301f, 0.494231f, 0.624059f, 0.572430f );

		pO2TK1HeatersB->SetInitialAnimState( 0.5f );
		pO2TK1HeatersB->DefineSwitchGroup( GRP_R1CS9_VC );
		pO2TK1HeatersB->SetReference( _V( 1.3533, 2.0372, 14.1175 ), switch_rot );
		pO2TK1HeatersB->SetMouseRegion( 0.643711f, 0.493677f, 0.664358f, 0.571084f );

		pO2TK1Heaters->SetInitialAnimState( 0.5f );
		pO2TK1Heaters->DefineSwitchGroup( GRP_R1CS10_VC );
		pO2TK1Heaters->SetReference( _V( 1.3533, 2.0372, 14.1175 ), switch_rot );
		pO2TK1Heaters->SetMouseRegion( 0.680614f, 0.492395f, 0.699791f, 0.568913f );
		pO2TK1Heaters->SetSpringLoaded( true, 0 );
		pO2TK1Heaters->SetSpringLoaded( true, 2 );

		pO2TK2HeatersA->SetInitialAnimState( 0.5f );
		pO2TK2HeatersA->DefineSwitchGroup( GRP_R1CS13_VC );
		pO2TK2HeatersA->SetReference( _V( 1.3428, 1.9859, 14.1137 ), switch_rot );
		pO2TK2HeatersA->SetMouseRegion( 0.594756f, 0.649479f, 0.614555f, 0.724077f );

		pO2TK2HeatersB->SetInitialAnimState( 0.5f );
		pO2TK2HeatersB->DefineSwitchGroup( GRP_R1CS14_VC );
		pO2TK2HeatersB->SetReference( _V( 1.3428, 1.9859, 14.1137 ), switch_rot );
		pO2TK2HeatersB->SetMouseRegion( 0.635746f, 0.646391f, 0.653571f, 0.725934f );

		pO2TK2Heaters->SetInitialAnimState( 0.5f );
		pO2TK2Heaters->DefineSwitchGroup( GRP_R1CS15_VC );
		pO2TK2Heaters->SetReference( _V( 1.3428, 1.9859, 14.1137 ), switch_rot );
		pO2TK2Heaters->SetMouseRegion( 0.671732f, 0.643791f, 0.689679f, 0.723404f );
		pO2TK2Heaters->SetSpringLoaded( true, 0 );
		pO2TK2Heaters->SetSpringLoaded( true, 2 );

		pO2TK3HeatersA->SetInitialAnimState( 0.5f );
		pO2TK3HeatersA->DefineSwitchGroup( GRP_R1CS21_VC );
		pO2TK3HeatersA->SetReference( _V( 1.3319, 1.9360, 14.1100 ), switch_rot );
		pO2TK3HeatersA->SetMouseRegion( 0.586690f, 0.796075f, 0.604818f, 0.876574f );

		pO2TK3HeatersB->SetInitialAnimState( 0.5f );
		pO2TK3HeatersB->DefineSwitchGroup( GRP_R1CS22_VC );
		pO2TK3HeatersB->SetReference( _V( 1.3319, 1.9360, 14.1100 ), switch_rot );
		pO2TK3HeatersB->SetMouseRegion( 0.624927f, 0.796604f, 0.643888f, 0.875995f );

		pO2TK3Heaters->SetInitialAnimState( 0.5f );
		pO2TK3Heaters->DefineSwitchGroup( GRP_R1CS23_VC );
		pO2TK3Heaters->SetReference( _V( 1.3319, 1.9360, 14.1100 ), switch_rot );
		pO2TK3Heaters->SetMouseRegion( 0.660057f, 0.794125f, 0.679893f, 0.875792f );
		pO2TK3Heaters->SetSpringLoaded( true, 0 );
		pO2TK3Heaters->SetSpringLoaded( true, 2 );

		pH2TK1HeatersA->SetInitialAnimState( 0.5f );
		pH2TK1HeatersA->DefineSwitchGroup( GRP_R1CS11_VC );
		pH2TK1HeatersA->SetReference( _V( 1.3533, 2.0372, 14.1175 ), switch_rot );
		pH2TK1HeatersA->SetMouseRegion( 0.850473f, 0.497816f, 0.871936f, 0.569279f );

		pH2TK1HeatersB->SetInitialAnimState( 0.5f );
		pH2TK1HeatersB->DefineSwitchGroup( GRP_R1CS12_VC );
		pH2TK1HeatersB->SetReference( _V( 1.3533, 2.0372, 14.1175 ), switch_rot );
		pH2TK1HeatersB->SetMouseRegion( 0.889951f, 0.496632f, 0.909783f, 0.573041f );

		pH2TK2HeatersA->SetInitialAnimState( 0.5f );
		pH2TK2HeatersA->DefineSwitchGroup( GRP_R1CS19_VC );
		pH2TK2HeatersA->SetReference( _V( 1.3428, 1.9859, 14.1137 ), switch_rot );
		pH2TK2HeatersA->SetMouseRegion( 0.840319f, 0.647562f, 0.859615f, 0.717474f );

		pH2TK2HeatersB->SetInitialAnimState( 0.5f );
		pH2TK2HeatersB->DefineSwitchGroup( GRP_R1CS20_VC );
		pH2TK2HeatersB->SetReference( _V( 1.3428, 1.9859, 14.1137 ), switch_rot );
		pH2TK2HeatersB->SetMouseRegion( 0.880004f, 0.646139f, 0.899238f, 0.719269f );

		pH2TK3HeatersA->SetInitialAnimState( 0.5f );
		pH2TK3HeatersA->DefineSwitchGroup( GRP_R1CS24_VC );
		pH2TK3HeatersA->SetReference( _V( 1.3319, 1.9360, 14.1100 ), switch_rot );
		pH2TK3HeatersA->SetMouseRegion( 0.829226f, 0.797709f, 0.849242f, 0.868222f );

		pH2TK3HeatersB->SetInitialAnimState( 0.5f );
		pH2TK3HeatersB->DefineSwitchGroup( GRP_R1CS25_VC );
		pH2TK3HeatersB->SetReference( _V( 1.3319, 1.9360, 14.1100 ), switch_rot );
		pH2TK3HeatersB->SetMouseRegion( 0.868802f, 0.799046f, 0.888961f, 0.869549f );

		pFuelCell1->SetInitialAnimState( 0.5f );
		pFuelCell1->DefineSwitchGroup( GRP_R1CS16_VC );
		pFuelCell1->SetReference( _V( 1.3718, 1.9650, 14.0063 ), switch_rot );
		pFuelCell1->SetPullDirection( switch_pull );
		pFuelCell1->SetMouseRegion( 0.711174f, 0.710608f, 0.731550f, 0.790725f );
		pFuelCell1->SetSpringLoaded( true, 0 );
		pFuelCell1->SetSpringLoaded( true, 2 );

		pFuelCell2->SetInitialAnimState( 0.5f );
		pFuelCell2->DefineSwitchGroup( GRP_R1CS17_VC );
		pFuelCell2->SetReference( _V( 1.3718, 1.9650, 14.0063 ), switch_rot );
		pFuelCell2->SetPullDirection( switch_pull );
		pFuelCell2->SetMouseRegion( 0.750424f, 0.708584f, 0.771162f, 0.788885f );
		pFuelCell2->SetSpringLoaded( true, 0 );
		pFuelCell2->SetSpringLoaded( true, 2 );

		pFuelCell3->SetInitialAnimState( 0.5f );
		pFuelCell3->DefineSwitchGroup( GRP_R1CS18_VC );
		pFuelCell3->SetReference( _V( 1.3718, 1.9650, 14.0063 ), switch_rot );
		pFuelCell3->SetPullDirection( switch_pull );
		pFuelCell3->SetMouseRegion( 0.791352f, 0.712022f, 0.811377f, 0.793408f );
		pFuelCell3->SetSpringLoaded( true, 0 );
		pFuelCell3->SetSpringLoaded( true, 2 );

		pFCMainBusATB->DefineMeshGroups( STS()->mesh_vc, GRP_R1_DS1_U_VC, GRP_R1_DS1_L_VC );
		pFCMainBusATB->SetInactiveSegment( TB_OFF );

		pFCMainBusBTB->DefineMeshGroups( STS()->mesh_vc, GRP_R1_DS3_U_VC, GRP_R1_DS3_L_VC );
		pFCMainBusBTB->SetInactiveSegment( TB_OFF );

		pFCMainBusCTB->DefineMeshGroups( STS()->mesh_vc, GRP_R1_DS5_U_VC, GRP_R1_DS5_L_VC );
		pFCMainBusCTB->SetInactiveSegment( TB_OFF );

		pMNBusTieATB->DefineMeshGroups( STS()->mesh_vc, GRP_R1_DS2_U_VC, GRP_R1_DS2_L_VC );
		pMNBusTieATB->SetInactiveSegment( TB_OFF );

		pMNBusTieBTB->DefineMeshGroups( STS()->mesh_vc, GRP_R1_DS4_U_VC, GRP_R1_DS4_L_VC );
		pMNBusTieBTB->SetInactiveSegment( TB_OFF );

		pMNBusTieCTB->DefineMeshGroups( STS()->mesh_vc, GRP_R1_DS6_U_VC, GRP_R1_DS6_L_VC );
		pMNBusTieCTB->SetInactiveSegment( TB_OFF );

		pInvPwr1TB->DefineMeshGroups( STS()->mesh_vc, GRP_R1_DS7_U_VC, GRP_R1_DS7_L_VC );
		pInvPwr1TB->SetInactiveSegment( TB_ON );

		pInvPwr2TB->DefineMeshGroups( STS()->mesh_vc, GRP_R1_DS9_U_VC, GRP_R1_DS9_L_VC );
		pInvPwr2TB->SetInactiveSegment( TB_ON );

		pInvPwr3TB->DefineMeshGroups( STS()->mesh_vc, GRP_R1_DS11_U_VC, GRP_R1_DS11_L_VC );
		pInvPwr3TB->SetInactiveSegment( TB_ON );

		pInvACBus1TB->DefineMeshGroups( STS()->mesh_vc, GRP_R1_DS8_U_VC, GRP_R1_DS8_L_VC );
		pInvACBus1TB->SetInactiveSegment( TB_OFF );

		pInvACBus2TB->DefineMeshGroups( STS()->mesh_vc, GRP_R1_DS10_U_VC, GRP_R1_DS10_L_VC );
		pInvACBus2TB->SetInactiveSegment( TB_OFF );

		pInvACBus3TB->DefineMeshGroups( STS()->mesh_vc, GRP_R1_DS12_U_VC, GRP_R1_DS12_L_VC );
		pInvACBus3TB->SetInactiveSegment( TB_OFF );

		pPayloadPriMNBTB->DefineMeshGroups( STS()->mesh_vc, GRP_R1_DS15_U_VC, GRP_R1_DS15_L_VC );
		pPayloadPriMNBTB->SetInactiveSegment( TB_OFF );

		pPayloadPriFC3TB->DefineMeshGroups( STS()->mesh_vc, GRP_R1_DS16_U_VC, GRP_R1_DS16_L_VC );
		pPayloadPriFC3TB->SetInactiveSegment( TB_OFF );

		pPayloadPriMNCTB->DefineMeshGroups( STS()->mesh_vc, GRP_R1_DS17_U_VC, GRP_R1_DS17_L_VC );
		pPayloadPriMNCTB->SetInactiveSegment( TB_OFF );

		pFuelCell1ReacO2->DefineMeshGroups( STS()->mesh_vc, GRP_R1C_DS1_U_VC, GRP_R1C_DS1_L_VC );
		pFuelCell1ReacO2->SetInactiveSegment( TB_CL );

		pFuelCell2ReacO2->DefineMeshGroups( STS()->mesh_vc, GRP_R1C_DS9_U_VC, GRP_R1C_DS9_L_VC );
		pFuelCell2ReacO2->SetInactiveSegment( TB_CL );

		pFuelCell3ReacO2->DefineMeshGroups( STS()->mesh_vc, GRP_R1C_DS5_U_VC, GRP_R1C_DS5_L_VC );
		pFuelCell3ReacO2->SetInactiveSegment( TB_CL );

		pFuelCell1ReacH2->DefineMeshGroups( STS()->mesh_vc, GRP_R1C_DS2_U_VC, GRP_R1C_DS2_L_VC );
		pFuelCell1ReacH2->SetInactiveSegment( TB_CL );

		pFuelCell2ReacH2->DefineMeshGroups( STS()->mesh_vc, GRP_R1C_DS10_U_VC, GRP_R1C_DS10_L_VC );
		pFuelCell2ReacH2->SetInactiveSegment( TB_CL );

		pFuelCell3ReacH2->DefineMeshGroups( STS()->mesh_vc, GRP_R1C_DS6_U_VC, GRP_R1C_DS6_L_VC );
		pFuelCell3ReacH2->SetInactiveSegment( TB_CL );

		pO2ManifoldVlvTank1TB->DefineMeshGroups( STS()->mesh_vc, GRP_R1C_DS3_U_VC, GRP_R1C_DS3_L_VC );
		pO2ManifoldVlvTank1TB->SetInactiveSegment( TB_CL );

		pO2ManifoldVlvTank2TB->DefineMeshGroups( STS()->mesh_vc, GRP_R1C_DS7_U_VC, GRP_R1C_DS7_L_VC );
		pO2ManifoldVlvTank2TB->SetInactiveSegment( TB_CL );

		pH2ManifoldVlvTank1TB->DefineMeshGroups( STS()->mesh_vc, GRP_R1C_DS4_U_VC, GRP_R1C_DS4_L_VC );
		pH2ManifoldVlvTank1TB->SetInactiveSegment( TB_CL );

		pH2ManifoldVlvTank2TB->DefineMeshGroups( STS()->mesh_vc, GRP_R1C_DS8_U_VC, GRP_R1C_DS8_L_VC );
		pH2ManifoldVlvTank2TB->SetInactiveSegment( TB_CL );

		pFuelCellReadyForLoad1->DefineMeshGroups( STS()->mesh_vc, GRP_R1C_DS11_U_VC, GRP_R1C_DS11_L_VC );

		pFuelCellReadyForLoad2->DefineMeshGroups( STS()->mesh_vc, GRP_R1C_DS12_U_VC, GRP_R1C_DS12_L_VC );

		pFuelCellReadyForLoad3->DefineMeshGroups( STS()->mesh_vc, GRP_R1C_DS13_U_VC, GRP_R1C_DS13_L_VC );

		pFuelCellCoolantPumpdP1->DefineMeshGroups( STS()->mesh_vc, GRP_R1C_DS14_U_VC, GRP_R1C_DS14_L_VC );

		pFuelCellCoolantPumpdP2->DefineMeshGroups( STS()->mesh_vc, GRP_R1C_DS15_U_VC, GRP_R1C_DS15_L_VC );

		pFuelCellCoolantPumpdP3->DefineMeshGroups( STS()->mesh_vc, GRP_R1C_DS16_U_VC, GRP_R1C_DS16_L_VC );
		return;
	}

	void PanelR1::RegisterVC( void )
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea( AID_R1, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_R1, 
			_V( 1.17636, 2.21861, 14.7453 ) + ofs, _V( 1.50934, 2.20883, 13.8049 ) + ofs, 
			_V( 1.12278, 1.88993, 14.66 ) + ofs, _V( 1.42939, 1.87552, 13.8055 ) + ofs );
		return;
	}

	void PanelR1::Realize( void )
	{
		// TODO
		AtlantisPanel::Realize();
		return;
	}
}
