#include "MPS.h"
#include <orbitersdk.h>
#include "..\AtlantisSubsystem.h"
#include "HeliumSystem.h"
#include <assert.h>


namespace mps
{
	MPS::MPS( AtlantisSubsystemDirector* _director, HeSysPneu* HeSys ):AtlantisSubsystem( _director, "MPS" )
	{
		this->HeSys = HeSys;

		ptrLV12 = new SolenoidValve( 0, 1000, true, HeSys, nullptr );
		ptrLV13 = new SolenoidValve( 0, 1000, true, HeSys, nullptr );
		ptrLV14 = new SolenoidValve( 0, 1000, true, HeSys, nullptr );
		ptrLV15 = new SolenoidValve( 0, 1000, true, HeSys, nullptr );
		ptrLV16 = new SolenoidValve( 0, 1000, true, HeSys, nullptr );
		ptrLV17 = new SolenoidValve( 0, 1000, true, HeSys, nullptr );
		ptrLV18 = new SolenoidValve( 0, 1000, true, HeSys, nullptr );
		ptrLV19 = new SolenoidValve( 0, 1000, true, HeSys, nullptr );
		ptrLV20 = new SolenoidValve( 0, 1000, true, HeSys, nullptr );
		ptrLV21 = new SolenoidValve( 0, 1000, true, HeSys, nullptr );
		ptrLV22 = new SolenoidValve( 0, 1000, true, HeSys, nullptr );
		ptrLV23 = new SolenoidValve( 0, 1000, true, HeSys, nullptr );

		ptrLV28 = new SolenoidValve( 0, 1000, true, HeSys, nullptr );
		ptrLV29 = new SolenoidValve( 0, 1000, true, HeSys, nullptr );
		ptrLV30 = new SolenoidValve( 0, 1000, true, HeSys, nullptr );
		ptrLV31 = new SolenoidValve( 0, 1000, true, HeSys, nullptr );
		ptrLV32 = new SolenoidValve( 0, 1000, true, HeSys, nullptr );
		ptrLV33 = new SolenoidValve( 0, 1000, true, HeSys, nullptr );
		ptrLV34 = new SolenoidValve( 0, 1000, true, HeSys, nullptr );
		ptrLV35 = new SolenoidValve( 0, 1000, true, HeSys, nullptr );

		ptrLV39 = new SolenoidValve( 0, 1000, true, HeSys, nullptr );
		ptrLV40 = new SolenoidValve( 0, 1000, true, HeSys, nullptr );
		ptrLV41 = new SolenoidValve( 0, 1000, true, ptrLV40, nullptr );

		ptrLV72 = new SolenoidValve( 0, 1000, true, HeSys, nullptr );
		ptrLV73 = new SolenoidValve( 0, 1000, true, HeSys, nullptr );

		ptrLV76 = new SolenoidValve( 0, 1000, true, HeSys, nullptr );
		ptrLV77 = new SolenoidValve( 1, 1000, true, HeSys, nullptr );
		ptrLV78 = new SolenoidValve( 1, 1000, true, HeSys, nullptr );

		// TODO find a way to make LV80, 81 and 82 connect to PV1, 2 and 3
		ptrLV80 = new SolenoidValve( 0, 1000, true, HeSys, nullptr );
		ptrLV81 = new SolenoidValve( 0, 1000, true, HeSys, nullptr );
		ptrLV82 = new SolenoidValve( 0, 1000, true, HeSys, nullptr );
		ptrLV83 = new SolenoidValve( 0, 1000, true, ptrLV12, nullptr );
		ptrLV84 = new SolenoidValve( 0, 1000, true, ptrLV14, nullptr );
		ptrLV85 = new SolenoidValve( 0, 1000, true, ptrLV16, nullptr );
		
		ptrPV1 = new PressureActuatedValve( 1, 100, ptrLV83, ptrLV13, nullptr, nullptr );
		ptrPV2 = new PressureActuatedValve( 1, 100, ptrLV84, ptrLV15, nullptr, nullptr );
		ptrPV3 = new PressureActuatedValve( 1, 100, ptrLV85, ptrLV17, nullptr, nullptr );
		ptrPV4 = new PressureActuatedValve( 0, 100, ptrLV18, ptrLV19, nullptr, nullptr );
		ptrPV5 = new PressureActuatedValve( 0, 100, ptrLV20, ptrLV21, nullptr, nullptr );
		ptrPV6 = new PressureActuatedValve( 0, 100, ptrLV22, ptrLV23, nullptr, nullptr );
/*		ptrPV7 = new ValveTypeBool( true, 100 );
		ptrPV8 = new ValveTypeBool( true, 100 );*/
		ptrPV9 = new PressureActuatedValve( 0, 100, ptrLV28, ptrLV29, nullptr, nullptr );
		ptrPV10 = new PressureActuatedValve( 0, 100, ptrLV30, ptrLV31, nullptr, nullptr );
		ptrPV11 = new PressureActuatedValve( 0, 100, ptrLV32, ptrLV33, nullptr, nullptr );
		ptrPV12 = new PressureActuatedValve( 0, 100, ptrLV34, ptrLV35, nullptr, nullptr );
		ptrPV13 = new PressureActuatedValve( 0, 100, ptrLV39, nullptr, nullptr, nullptr );
		ptrPV17 = new PressureActuatedValve( 0, 100, ptrLV72, nullptr, nullptr, nullptr );
		ptrPV18 = new PressureActuatedValve( 0, 100, ptrLV73, nullptr, nullptr, nullptr );
		ptrPV19 = new PressureActuatedValve( 1, 100, nullptr, ptrLV76, nullptr, nullptr );
		ptrPV20 = new PressureActuatedValve( 0, 100, nullptr, ptrLV77, nullptr, nullptr );
		ptrPV21 = new PressureActuatedValve( 0, 100, nullptr, ptrLV78, nullptr, nullptr );
		//ptrPV22 = new ValveTypeBool( false, 50 );// Normal closing response of the valve is less than 2 seconds (signal to switch).

		//posPD1 = true;
		//posPD2 = true;
		//posPD3 = true;
		
		LOXManifPress = 105;
		LH2ManifPress = 45;

		LOXinitpress = 23;
		LH2initpress = 33;
		return;
	}

	MPS::~MPS( void )
	{
		delete ptrLV12;
		delete ptrLV13;
		delete ptrLV14;
		delete ptrLV15;
		delete ptrLV16;
		delete ptrLV17;
		delete ptrLV18;
		delete ptrLV19;
		delete ptrLV20;
		delete ptrLV21;
		delete ptrLV22;
		delete ptrLV23;

		delete ptrLV28;
		delete ptrLV29;
		delete ptrLV30;
		delete ptrLV31;
		delete ptrLV32;
		delete ptrLV33;
		delete ptrLV34;
		delete ptrLV35;

		delete ptrLV39;
		delete ptrLV40;
		delete ptrLV41;

		delete ptrLV72;
		delete ptrLV73;

		delete ptrLV76;
		delete ptrLV77;
		delete ptrLV78;

		delete ptrLV80;
		delete ptrLV81;
		delete ptrLV82;
		delete ptrLV83;
		delete ptrLV84;
		delete ptrLV85;

		delete ptrPV1;
		delete ptrPV2;
		delete ptrPV3;
		delete ptrPV4;
		delete ptrPV5;
		delete ptrPV6;
		//delete ptrPV7;
		//delete ptrPV8;
		delete ptrPV9;
		delete ptrPV10;
		delete ptrPV11;
		delete ptrPV12;
		delete ptrPV13;
		delete ptrPV17;
		delete ptrPV18;
		delete ptrPV19;
		delete ptrPV20;
		delete ptrPV21;
		//delete ptrPV22;
	}

	bool MPS::OnParseLine( const char* line )
	{
		return false;
	}

	void MPS::OnSaveState( FILEHANDLE scn ) const
	{
		return;
	}

	void MPS::Realize( void )
	{
		discsignals::DiscreteBundle* bundle = BundleManager()->CreateBundle( "MPS_LV_A", 16 );// LV1 - LV16
		//ptrLV1->Connect( 0, bundle, 0 );
		//ptrLV2->Connect( 0, bundle, 1 );
		//ptrLV3->Connect( 0, bundle, 2 );
		//ptrLV4->Connect( 0, bundle, 3 );
		//ptrLV5->Connect( 0, bundle, 4 );
		//ptrLV6->Connect( 0, bundle, 5 );
		//ptrLV7->Connect( 0, bundle, 6 );
		//ptrLV8->Connect( 0, bundle, 7 );
		//ptrLV9->Connect( 0, bundle, 8 );
		//ptrLV10->Connect( 0, bundle, 9 );
		//ptrLV11->Connect( 0, bundle, 10 );
		ptrLV12->Connect( 0, bundle, 11 );
		ptrLV13->Connect( 0, bundle, 12 );
		ptrLV14->Connect( 0, bundle, 13 );
		ptrLV15->Connect( 0, bundle, 14 );
		ptrLV16->Connect( 0, bundle, 15 );

		bundle = BundleManager()->CreateBundle( "MPS_LV_B", 16 );// LV17 - LV32
		ptrLV17->Connect( 0, bundle, 0 );
		ptrLV18->Connect( 0, bundle, 1 );
		ptrLV19->Connect( 0, bundle, 2 );
		ptrLV20->Connect( 0, bundle, 3 );
		ptrLV21->Connect( 0, bundle, 4 );
		ptrLV22->Connect( 0, bundle, 5 );
		ptrLV23->Connect( 0, bundle, 6 );
		//ptrLV24->Connect( 0, bundle, 7 );
		//ptrLV25->Connect( 0, bundle, 8 );
		//ptrLV26->Connect( 0, bundle, 9 );
		//ptrLV27->Connect( 0, bundle, 10 );
		ptrLV28->Connect( 0, bundle, 11 );
		ptrLV29->Connect( 0, bundle, 12 );
		ptrLV30->Connect( 0, bundle, 13 );
		ptrLV31->Connect( 0, bundle, 14 );
		ptrLV32->Connect( 0, bundle, 15 );

		bundle = BundleManager()->CreateBundle( "MPS_LV_C", 16 );// LV33 - LV48
		ptrLV33->Connect( 0, bundle, 0 );
		ptrLV34->Connect( 0, bundle, 1 );
		ptrLV35->Connect( 0, bundle, 2 );
		//ptrLV36->Connect( 0, bundle, 3 );
		//ptrLV37->Connect( 0, bundle, 4 );
		//ptrLV38->Connect( 0, bundle, 5 );
		ptrLV39->Connect( 0, bundle, 6 );
		ptrLV40->Connect( 0, bundle, 7 );
		ptrLV41->Connect( 0, bundle, 8 );
		//ptrLV42->Connect( 0, bundle, 9 );
		//ptrLV43->Connect( 0, bundle, 10 );
		//ptrLV44->Connect( 0, bundle, 11 );
		//ptrLV45->Connect( 0, bundle, 12 );
		//ptrLV46->Connect( 0, bundle, 13 );
		//ptrLV47->Connect( 0, bundle, 14 );
		//ptrLV48->Connect( 0, bundle, 15 );

		//bundle = BundleManager()->CreateBundle( "MPS_LV_D", 16 );// LV49 - LV64
		//ptrLV49->Connect( 0, bundle, 0 );
		//ptrLV50->Connect( 0, bundle, 1 );
		//ptrLV51->Connect( 0, bundle, 2 );
		//ptrLV52->Connect( 0, bundle, 3 );
		//ptrLV53->Connect( 0, bundle, 4 );
		//ptrLV54->Connect( 0, bundle, 5 );
		//ptrLV55->Connect( 0, bundle, 6 );
		//ptrLV56->Connect( 0, bundle, 7 );
		//ptrLV57->Connect( 0, bundle, 8 );
		//ptrLV58->Connect( 0, bundle, 9 );
		//ptrLV59->Connect( 0, bundle, 10 );
		//ptrLV60->Connect( 0, bundle, 11 );
		//ptrLV61->Connect( 0, bundle, 12 );
		//ptrLV62->Connect( 0, bundle, 13 );
		//ptrLV63->Connect( 0, bundle, 14 );
		//ptrLV64->Connect( 0, bundle, 15 );

		bundle = BundleManager()->CreateBundle( "MPS_LV_E", 16 );// LV65 - LV80
		//ptrLV65->Connect( 0, bundle, 0 );
		//ptrLV66->Connect( 0, bundle, 1 );
		//ptrLV67->Connect( 0, bundle, 2 );
		//ptrLV68->Connect( 0, bundle, 3 );
		//ptrLV69->Connect( 0, bundle, 4 );
		//ptrLV70->Connect( 0, bundle, 5 );
		//ptrLV71->Connect( 0, bundle, 6 );
		ptrLV72->Connect( 0, bundle, 7 );
		ptrLV73->Connect( 0, bundle, 8 );
		//ptrLV74->Connect( 0, bundle, 9 );
		//ptrLV75->Connect( 0, bundle, 10 );
		ptrLV76->Connect( 0, bundle, 11 );
		ptrLV77->Connect( 0, bundle, 12 );
		ptrLV78->Connect( 0, bundle, 13 );
		//ptrLV79->Connect( 0, bundle, 14 );
		ptrLV80->Connect( 0, bundle, 15 );

		bundle = BundleManager()->CreateBundle( "MPS_LV_F", 16 );// LV81 - LV85
		ptrLV81->Connect( 0, bundle, 0 );
		ptrLV82->Connect( 0, bundle, 1 );
		ptrLV83->Connect( 0, bundle, 2 );
		ptrLV84->Connect( 0, bundle, 3 );
		ptrLV85->Connect( 0, bundle, 4 );


		// connect indications
		bundle = BundleManager()->CreateBundle( "MPS_CLInd_A", 16 );
		ptrPV19->ConnectIndication( false, 0, bundle, 8 );
		ptrPV19->ConnectIndication( false, 1, bundle, 9 );
		ptrPV20->ConnectIndication( false, 0, bundle, 10 );
		ptrPV21->ConnectIndication( false, 0, bundle, 11 );

		//bundle = BundleManager()->CreateBundle( "MPS_CLInd_B", 16 );

		bundle = BundleManager()->CreateBundle( "MPS_OPInd_A", 16 );
		ptrPV4->ConnectIndication( true, 0, bundle, 3 );
		ptrPV4->ConnectIndication( true, 1, bundle, 4 );
		ptrPV5->ConnectIndication( true, 0, bundle, 5 );
		ptrPV5->ConnectIndication( true, 1, bundle, 6 );
		ptrPV6->ConnectIndication( true, 0, bundle, 7 );
		ptrPV6->ConnectIndication( true, 1, bundle, 8 );

		ptrPV20->ConnectIndication( true, 0, bundle, 15 );

		bundle = BundleManager()->CreateBundle( "MPS_OPInd_B", 16 );
		ptrPV21->ConnectIndication( true, 0, bundle, 0 );
		return;
	}

	void MPS::OnPostStep( double fSimT, double fDeltaT, double fMJD )
	{
		ptrLV12->tmestp( fDeltaT );
		ptrLV13->tmestp( fDeltaT );
		ptrLV14->tmestp( fDeltaT );
		ptrLV15->tmestp( fDeltaT );
		ptrLV16->tmestp( fDeltaT );
		ptrLV17->tmestp( fDeltaT );
		ptrLV18->tmestp( fDeltaT );
		ptrLV19->tmestp( fDeltaT );
		ptrLV20->tmestp( fDeltaT );
		ptrLV21->tmestp( fDeltaT );
		ptrLV22->tmestp( fDeltaT );
		ptrLV23->tmestp( fDeltaT );

		ptrLV28->tmestp( fDeltaT );
		ptrLV29->tmestp( fDeltaT );
		ptrLV30->tmestp( fDeltaT );
		ptrLV31->tmestp( fDeltaT );
		ptrLV32->tmestp( fDeltaT );
		ptrLV33->tmestp( fDeltaT );
		ptrLV34->tmestp( fDeltaT );
		ptrLV35->tmestp( fDeltaT );

		ptrLV39->tmestp( fDeltaT );
		ptrLV40->tmestp( fDeltaT );
		ptrLV41->tmestp( fDeltaT );

		ptrLV72->tmestp( fDeltaT );
		ptrLV73->tmestp( fDeltaT );

		ptrLV76->tmestp( fDeltaT );
		ptrLV77->tmestp( fDeltaT );
		ptrLV78->tmestp( fDeltaT );

		ptrLV80->tmestp( fDeltaT );
		ptrLV81->tmestp( fDeltaT );
		ptrLV82->tmestp( fDeltaT );
		ptrLV83->tmestp( fDeltaT );
		ptrLV84->tmestp( fDeltaT );
		ptrLV85->tmestp( fDeltaT );

		ptrPV1->tmestp( fDeltaT );
		ptrPV2->tmestp( fDeltaT );
		ptrPV3->tmestp( fDeltaT );
		ptrPV4->tmestp( fDeltaT );
		ptrPV5->tmestp( fDeltaT );
		ptrPV6->tmestp( fDeltaT );
		//ptrPV7->tmestp( fDeltaT );
		//ptrPV8->tmestp( fDeltaT );
		ptrPV9->tmestp( fDeltaT );
		ptrPV10->tmestp( fDeltaT );
		ptrPV11->tmestp( fDeltaT );
		ptrPV12->tmestp( fDeltaT );
		ptrPV13->tmestp( fDeltaT );
		ptrPV17->tmestp( fDeltaT );
		ptrPV18->tmestp( fDeltaT );
		ptrPV19->tmestp( fDeltaT );
		ptrPV20->tmestp( fDeltaT );
		ptrPV21->tmestp( fDeltaT );
		//ptrPV22->tmestp( fDeltaT );

		ptrLV41->Use( 800 * fDeltaT );

		PROPELLANT_HANDLE LOXTank = STS()->GetLOXTank();

		/*double LOXTankLevel = STS()->GetPropellantLevel( LOXTank ) / 100;
		double LH2TankLevel = STS()->GetPropellantLevel( STS()->GetLH2Tank() ) / 100;*/

		double LOXTankLevel = 0;
		double LH2TankLevel = 0;
		double LOXTankMass = 0;

		if (LOXTank != NULL)
		{
			LOXTankLevel = STS()->GetPropellantLevel( LOXTank ) / 100;
			LH2TankLevel = STS()->GetPropellantLevel( STS()->GetLH2Tank() ) / 100;
			LOXTankMass = STS()->GetPropellantMaxMass( LOXTank );
		}

		if (LOXTankMass > 5000)// simple ET vs manifold check
		{
			// ET
			// TODO fix vent decreasing during ascent
			LOXManifPress = 23 + (82 * LOXTankLevel); // TODO >>100 -> SRB sep -> ~50
			LH2ManifPress = 33 + LH2TankLevel;
		}
		else
		{
			// manifold
			// TODO use ideal gas here for temp increase?
			LOXinitpress += 0.4 * fDeltaT * LOXTankLevel;// pressure rises due to heat soak back
			LH2initpress += 1 * fDeltaT * LH2TankLevel;// pressure rises due to heat soak back (STS-1 data: ~1 psi/sec)
			LOXManifPress = LOXinitpress * LOXTankLevel;
			LH2ManifPress = LH2initpress * LH2TankLevel;
		}

		STS()->SetMPSDumpLevel( 3, ptrPV17->GetPos() * ptrPV18->GetPos() * LH2TankLevel );
		STS()->SetMPSDumpLevel( 4, ptrPV11->GetPos() * (ptrPV12->GetPos() * 0.7 + ptrPV13->GetPos() * 0.3) * LH2TankLevel );

		/*char buffer[100];
		sprintf_s( buffer, 100, "PV1 %f LV12 %f LV83 %f LV13 %f LV80 %f", ptrPV1->GetPos(), ptrLV12->GetPos(), ptrLV83->GetPos(), ptrLV13->GetPos(), ptrLV80->GetPos() );
		oapiWriteLog( buffer );
		sprintf_s( buffer, 100, "PV4 %f LV18 %f LV19 %f", ptrPV4->GetPos(), ptrLV18->GetPos(), ptrLV19->GetPos() );
		oapiWriteLog( buffer );*/
		return;
	}

	double MPS::GetLOXManifPress( void ) const
	{
		return LOXManifPress;
	}
	
	double MPS::GetLH2ManifPress( void ) const
	{
		return LH2ManifPress;
	}

	double MPS::GetLOXPVPos( int eng ) const
	{
		assert( (eng >= 1) && (eng <= 3) && "MPS::GetLOXPVPos.eng" );
		if (eng == 1)
		{
			return ptrPV1->GetPos();
		}
		else if (eng == 2)
		{
			return ptrPV2->GetPos();
		}
		return ptrPV3->GetPos();
	}

	double MPS::GetLH2PVPos( int eng ) const
	{
		assert( (eng >= 1) && (eng <= 3) && "MPS::GetLH2PVPos.eng" );
		if (eng == 1)
		{
			return ptrPV4->GetPos();
		}
		else if (eng == 2)
		{
			return ptrPV5->GetPos();
		}
		return ptrPV6->GetPos();
	}
}