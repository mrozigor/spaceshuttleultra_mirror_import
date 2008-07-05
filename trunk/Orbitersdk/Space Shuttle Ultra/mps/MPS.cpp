#include "MPS.h"


namespace mps
{
	MPS::MPS( SubsystemDirector* _director ):AtlantisSubsystem( _director, "mpsMPS" )
	{
		// mostly speculation...
		ptrPV1 = new ValveTypeBool( true, 50 );
		ptrPV2 = new ValveTypeBool( true, 50 );
		ptrPV3 = new ValveTypeBool( true, 50 );
		ptrPV4 = new ValveTypeBool( true, 50 );
		ptrPV5 = new ValveTypeBool( true, 50 );
		ptrPV6 = new ValveTypeBool( true, 50 );
		ptrPV7 = new ValveTypeBool( true, 50 );
		ptrPV8 = new ValveTypeBool( true, 50 );
		ptrPV9 = new ValveTypeBool( true, 50 );
		ptrPV10 = new ValveTypeBool( true, 50 );
		ptrPV11 = new ValveTypeBool( true, 50 );
		ptrPV12 = new ValveTypeBool( true, 50 );
		ptrPV13 = new ValveTypeBool( true, 50 );
		ptrPV17 = new ValveTypeBool( false, 50 );
		ptrPV18 = new ValveTypeBool( false, 50 );
		ptrPV19 = new ValveTypeBool( false, 50 );
		ptrPV20 = new ValveTypeBool( true, 50 );
		ptrPV21 = new ValveTypeBool( true, 50 );
		ptrPV22 = new ValveTypeBool( false, 50 );

		posPD1 = true;
		posPD2 = true;
		posPD3 = true;
		return;
	}

	MPS::~MPS( void )
	{
		delete ptrPV1;
		delete ptrPV2;
		delete ptrPV3;
		delete ptrPV4;
		delete ptrPV5;
		delete ptrPV6;
		delete ptrPV7;
		delete ptrPV8;
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
		delete ptrPV22;
		// the end
	}

	bool MPS::OnParseLine( const char* line )// TODO val checks
	{
		double fdata1 = 0;
		double fdata2 = 0;
		double fdata3 = 0;
		double fdata4 = 0;
		double fdata5 = 0;
		double fdata6 = 0;
		double fdata7 = 0;
		double fdata8 = 0;
		double fdata9 = 0;
		double fdata10 = 0;
		double fdata11 = 0;
		double fdata12 = 0;
		double fdata13 = 0;
		double fdata14 = 0;
		double fdata15 = 0;
		double fdata16 = 0;
		double fdata17 = 0;
		double fdata18 = 0;
		double fdata19 = 0;

		if (!_strnicmp( line, "MPS", 3 ))
		{
			sscanf( line + 3, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
				&fdata1,
				&fdata2,
				&fdata3,
				&fdata4,
				&fdata5,
				&fdata6,
				&fdata7,
				&fdata8,
				&fdata9,
				&fdata10,
				&fdata11,
				&fdata12,
				&fdata13,
				&fdata14,
				&fdata15,
				&fdata16,
				&fdata17,
				&fdata18,
				&fdata19 );

			// TODO set vlv pos using back door

			return true;
		}
		return false;
	}

	void MPS::OnSaveState( FILEHANDLE scn ) const
	{
		char sdata[256];

		sprintf( sdata, "  MPS %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
			(double)ptrPV1->GetPos(),
			(double)ptrPV2->GetPos(), 
			(double)ptrPV3->GetPos(),
			(double)ptrPV4->GetPos(),
			(double)ptrPV5->GetPos(), 
			(double)ptrPV6->GetPos(),
			(double)ptrPV7->GetPos(),
			(double)ptrPV8->GetPos(), 
			(double)ptrPV9->GetPos(),
			(double)ptrPV10->GetPos(),
			(double)ptrPV11->GetPos(), 
			(double)ptrPV12->GetPos(),
			(double)ptrPV13->GetPos(),
			(double)ptrPV17->GetPos(), 
			(double)ptrPV18->GetPos(),
			(double)ptrPV19->GetPos(),
			(double)ptrPV20->GetPos(),
			(double)ptrPV21->GetPos(),
			(double)ptrPV22->GetPos() );
		oapiWriteLine( scn, sdata );
		return;
	}

	bool MPS::MPSValveOpen( MPS_VALVE vlv )
	{
		switch (vlv)
		{
			case PV1:
				return ptrPV1->Open();
			case PV2:
				return ptrPV2->Open();
			case PV3:
				return ptrPV3->Open();
			case PV4:
				return ptrPV4->Open();
			case PV5:
				return ptrPV5->Open();
			case PV6:
				return ptrPV6->Open();
			case PV7:
				return ptrPV7->Open();
			case PV8:
				return ptrPV8->Open();
			case PV9:
				return ptrPV9->Open();
			case PV10:
				return ptrPV10->Open();
			case PV11:
				return ptrPV11->Open();
			case PV12:
				return ptrPV12->Open();
			case PV13:
				return ptrPV13->Open();
			case PV17:
				return ptrPV17->Open();
			case PV18:
				return ptrPV18->Open();
			case PV19:
				return ptrPV19->Open();
			case PV20:
				return ptrPV20->Open();
			case PV21:
				return ptrPV21->Open();
			case PV22:
				return ptrPV22->Open();
			case PD1:
				posPD1 = true;
				return true;
			case PD2:
				posPD2 = true;
				return true;
			case PD3:
				posPD3 = true;
				return true;
		}
		return false;
	}

	bool MPS::MPSValveClose( MPS_VALVE vlv )
	{
		switch (vlv)
		{
			case PV1:
				return ptrPV1->Close();
			case PV2:
				return ptrPV2->Close();
			case PV3:
				return ptrPV3->Close();
			case PV4:
				return ptrPV4->Close();
			case PV5:
				return ptrPV5->Close();
			case PV6:
				return ptrPV6->Close();
			case PV7:
				return ptrPV7->Close();
			case PV8:
				return ptrPV8->Close();
			case PV9:
				return ptrPV9->Close();
			case PV10:
				return ptrPV10->Close();
			case PV11:
				return ptrPV11->Close();
			case PV12:
				return ptrPV12->Close();
			case PV13:
				return ptrPV13->Close();
			case PV17:
				return ptrPV17->Close();
			case PV18:
				return ptrPV18->Close();
			case PV19:
				return ptrPV19->Close();
			case PV20:
				return ptrPV20->Close();
			case PV21:
				return ptrPV21->Close();
			case PV22:
				return ptrPV22->Close();
			case PD1:
				posPD1 = false;
				return true;
			case PD2:
				posPD2 = false;
				return true;
			case PD3:
				posPD3 = false;
				return true;
		}
		return false;
	}

	double MPS::MPSValveStatus( MPS_VALVE vlv )
	{
		switch (vlv)
		{
			case PV1:
				return (double)ptrPV1->GetPos();
			case PV2:
				return (double)ptrPV2->GetPos();
			case PV3:
				return (double)ptrPV3->GetPos();
			case PV4:
				return (double)ptrPV4->GetPos();
			case PV5:
				return (double)ptrPV5->GetPos();
			case PV6:
				return (double)ptrPV6->GetPos();
			case PV7:
				return (double)ptrPV7->GetPos();
			case PV8:
				return (double)ptrPV8->GetPos();
			case PV9:
				return (double)ptrPV9->GetPos();
			case PV10:
				return (double)ptrPV10->GetPos();
			case PV11:
				return (double)ptrPV11->GetPos();
			case PV12:
				return (double)ptrPV12->GetPos();
			case PV13:
				return (double)ptrPV13->GetPos();
			case PV17:
				return (double)ptrPV17->GetPos();
			case PV18:
				return (double)ptrPV18->GetPos();
			case PV19:
				return (double)ptrPV19->GetPos();
			case PV20:
				return (double)ptrPV20->GetPos();
			case PV21:
				return (double)ptrPV21->GetPos();
			case PV22:
				return (double)ptrPV22->GetPos();
			case PD1:
				return posPD1;
			case PD2:
				return posPD2;
			case PD3:
				return posPD3;
		}
		return false;// ?????
	}

	void MPS::OnPostStep( double fSimT, double fDeltaT, double fMJD )
	{
		ptrPV1->tmestp( fSimT );
		ptrPV2->tmestp( fSimT );
		ptrPV3->tmestp( fSimT );
		ptrPV4->tmestp( fSimT );
		ptrPV5->tmestp( fSimT );
		ptrPV6->tmestp( fSimT );
		ptrPV7->tmestp( fSimT );
		ptrPV8->tmestp( fSimT );
		ptrPV9->tmestp( fSimT );
		ptrPV10->tmestp( fSimT );
		ptrPV11->tmestp( fSimT );
		ptrPV12->tmestp( fSimT );
		ptrPV13->tmestp( fSimT );
		ptrPV17->tmestp( fSimT );
		ptrPV18->tmestp( fSimT );
		ptrPV19->tmestp( fSimT );
		ptrPV20->tmestp( fSimT );
		ptrPV21->tmestp( fSimT );
		ptrPV22->tmestp( fSimT );
		return;
	}
}