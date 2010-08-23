#include "MPS.h"


namespace mps
{
	MPS::MPS( AtlantisSubsystemDirector* _director ):AtlantisSubsystem( _director, "mpsMPS" )
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

	bool MPS::OnParseLine( const char* line )// waiting for .scn format standard
	{
		/*bool pos = true;// open

		if (strncmp( line, "mpsPV1 ", 7 ) == 0)
		{
			if (strcmp( line, "mpsPV1 CL" ) == 0) pos = false;
			ptrPV1->_backdoor( pos );
			return true;
		}
		else if (strncmp( line, "mpsPV2 ", 7 ) == 0)
		{
			if (strcmp( line, "mpsPV2 CL" ) == 0) pos = false;
			ptrPV2->_backdoor( pos );
			return true;
		}
		else if (strncmp( line, "mpsPV3 ", 7 ) == 0)
		{
			if (strcmp( line, "mpsPV3 CL" ) == 0) pos = false;
			ptrPV3->_backdoor( pos );
			return true;
		}
		else if (strncmp( line, "mpsPV4 ", 7 ) == 0)
		{
			if (strcmp( line, "mpsPV4 CL" ) == 0) pos = false;
			ptrPV4->_backdoor( pos );
			return true;
		}
		else if (strncmp( line, "mpsPV5 ", 7 ) == 0)
		{
			if (strcmp( line, "mpsPV5 CL" ) == 0) pos = false;
			ptrPV5->_backdoor( pos );
			return true;
		}
		else if (strncmp( line, "mpsPV6 ", 7 ) == 0)
		{
			if (strcmp( line, "mpsPV6 CL" ) == 0) pos = false;
			ptrPV6->_backdoor( pos );
			return true;
		}
		else if (strncmp( line, "mpsPV7 ", 7 ) == 0)
		{
			if (strcmp( line, "mpsPV7 CL" ) == 0) pos = false;
			ptrPV7->_backdoor( pos );
			return true;
		}
		else if (strncmp( line, "mpsPV8 ", 7 ) == 0)
		{
			if (strcmp( line, "mpsPV8 CL" ) == 0) pos = false;
			ptrPV8->_backdoor( pos );
			return true;
		}
		else if (strncmp( line, "mpsPV9 ", 7 ) == 0)
		{
			if (strcmp( line, "mpsPV9 CL" ) == 0) pos = false;
			ptrPV9->_backdoor( pos );
			return true;
		}
		else if (strncmp( line, "mpsPV10", 7 ) == 0)
		{
			if (strcmp( line, "mpsPV10 CL" ) == 0) pos = false;
			ptrPV10->_backdoor( pos );
			return true;
		}
		else if (strncmp( line, "mpsPV11", 7 ) == 0)
		{
			if (strcmp( line, "mpsPV11 CL" ) == 0) pos = false;
			ptrPV11->_backdoor( pos );
			return true;
		}
		else if (strncmp( line, "mpsPV12", 7 ) == 0)
		{
			if (strcmp( line, "mpsPV12 CL" ) == 0) pos = false;
			ptrPV12->_backdoor( pos );
			return true;
		}
		else if (strncmp( line, "mpsPV13", 7 ) == 0)
		{
			if (strcmp( line, "mpsPV13 CL" ) == 0) pos = false;
			ptrPV13->_backdoor( pos );
			return true;
		}
		else if (strncmp( line, "mpsPV17", 7 ) == 0)
		{
			if (strcmp( line, "mpsPV17 CL" ) == 0) pos = false;
			ptrPV17->_backdoor( pos );
			return true;
		}
		else if (strncmp( line, "mpsPV18", 7 ) == 0)
		{
			if (strcmp( line, "mpsPV18 CL" ) == 0) pos = false;
			ptrPV18->_backdoor( pos );
			return true;
		}
		else if (strncmp( line, "mpsPV19", 7 ) == 0)
		{
			if (strcmp( line, "mpsPV19 CL" ) == 0) pos = false;
			ptrPV19->_backdoor( pos );
			return true;
		}
		else if (strncmp( line, "mpsPV20", 7 ) == 0)
		{
			if (strcmp( line, "mpsPV20 CL" ) == 0) pos = false;
			ptrPV20->_backdoor( pos );
			return true;
		}
		else if (strncmp( line, "mpsPV21", 7 ) == 0)
		{
			if (strcmp( line, "mpsPV21 CL" ) == 0) pos = false;
			ptrPV21->_backdoor( pos );
			return true;
		}
		else if (strncmp( line, "mpsPV22", 7 ) == 0)
		{
			if (strcmp( line, "mpsPV22 CL" ) == 0) pos = false;
			ptrPV22->_backdoor( pos );
			return true;
		}*/

		return false;
	}

	void MPS::OnSaveState( FILEHANDLE scn ) const// waiting for .scn format standard
	{
		/*char line[16];

		strcpy( line, "  mpsPV1 " );
		if (ptrPV1->GetPos() == true)
		{
			strcat( line, "OP" );
		}
		else
		{
			strcat( line, "CL" );
		}
		oapiWriteLine( scn, line );

		strcpy( line, "  mpsPV2 " );
		if (ptrPV2->GetPos() == true)
		{
			strcat( line, "OP" );
		}
		else
		{
			strcat( line, "CL" );
		}
		oapiWriteLine( scn, line );

		strcpy( line, "  mpsPV3 " );
		if (ptrPV3->GetPos() == true)
		{
			strcat( line, "OP" );
		}
		else
		{
			strcat( line, "CL" );
		}
		oapiWriteLine( scn, line );

		strcpy( line, "  mpsPV4 " );
		if (ptrPV4->GetPos() == true)
		{
			strcat( line, "OP" );
		}
		else
		{
			strcat( line, "CL" );
		}
		oapiWriteLine( scn, line );

		strcpy( line, "  mpsPV5 " );
		if (ptrPV5->GetPos() == true)
		{
			strcat( line, "OP" );
		}
		else
		{
			strcat( line, "CL" );
		}
		oapiWriteLine( scn, line );

		strcpy( line, "  mpsPV6 " );
		if (ptrPV6->GetPos() == true)
		{
			strcat( line, "OP" );
		}
		else
		{
			strcat( line, "CL" );
		}
		oapiWriteLine( scn, line );

		strcpy( line, "  mpsPV7 " );
		if (ptrPV7->GetPos() == true)
		{
			strcat( line, "OP" );
		}
		else
		{
			strcat( line, "CL" );
		}
		oapiWriteLine( scn, line );

		strcpy( line, "  mpsPV8 " );
		if (ptrPV8->GetPos() == true)
		{
			strcat( line, "OP" );
		}
		else
		{
			strcat( line, "CL" );
		}
		oapiWriteLine( scn, line );

		strcpy( line, "  mpsPV9 " );
		if (ptrPV9->GetPos() == true)
		{
			strcat( line, "OP" );
		}
		else
		{
			strcat( line, "CL" );
		}
		oapiWriteLine( scn, line );

		strcpy( line, "  mpsPV10 " );
		if (ptrPV10->GetPos() == true)
		{
			strcat( line, "OP" );
		}
		else
		{
			strcat( line, "CL" );
		}
		oapiWriteLine( scn, line );

		strcpy( line, "  mpsPV11 " );
		if (ptrPV11->GetPos() == true)
		{
			strcat( line, "OP" );
		}
		else
		{
			strcat( line, "CL" );
		}
		oapiWriteLine( scn, line );

		strcpy( line, "  mpsPV12 " );
		if (ptrPV12->GetPos() == true)
		{
			strcat( line, "OP" );
		}
		else
		{
			strcat( line, "CL" );
		}
		oapiWriteLine( scn, line );

		strcpy( line, "  mpsPV13 " );
		if (ptrPV13->GetPos() == true)
		{
			strcat( line, "OP" );
		}
		else
		{
			strcat( line, "CL" );
		}
		oapiWriteLine( scn, line );

		strcpy( line, "  mpsPV17 " );
		if (ptrPV17->GetPos() == true)
		{
			strcat( line, "OP" );
		}
		else
		{
			strcat( line, "CL" );
		}
		oapiWriteLine( scn, line );

		strcpy( line, "  mpsPV18 " );
		if (ptrPV18->GetPos() == true)
		{
			strcat( line, "OP" );
		}
		else
		{
			strcat( line, "CL" );
		}
		oapiWriteLine( scn, line );

		strcpy( line, "  mpsPV19 " );
		if (ptrPV19->GetPos() == true)
		{
			strcat( line, "OP" );
		}
		else
		{
			strcat( line, "CL" );
		}
		oapiWriteLine( scn, line );

		strcpy( line, "  mpsPV20 " );
		if (ptrPV20->GetPos() == true)
		{
			strcat( line, "OP" );
		}
		else
		{
			strcat( line, "CL" );
		}
		oapiWriteLine( scn, line );

		strcpy( line, "  mpsPV21 " );
		if (ptrPV21->GetPos() == true)
		{
			strcat( line, "OP" );
		}
		else
		{
			strcat( line, "CL" );
		}
		oapiWriteLine( scn, line );

		strcpy( line, "  mpsPV22 " );
		if (ptrPV22->GetPos() == true)
		{
			strcat( line, "OP" );
		}
		else
		{
			strcat( line, "CL" );
		}
		oapiWriteLine( scn, line );*/

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
		ptrPV1->tmestp( fDeltaT );
		ptrPV2->tmestp( fDeltaT );
		ptrPV3->tmestp( fDeltaT );
		ptrPV4->tmestp( fDeltaT );
		ptrPV5->tmestp( fDeltaT );
		ptrPV6->tmestp( fDeltaT );
		ptrPV7->tmestp( fDeltaT );
		ptrPV8->tmestp( fDeltaT );
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
		ptrPV22->tmestp( fDeltaT );
		return;
	}
}