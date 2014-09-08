#include "ATVC_SOP.h"
#include "..\Atlantis.h"
#include "assert.h"


namespace dps
{
	ATVC_SOP::ATVC_SOP( SimpleGPCSystem *_gpc ):SimpleGPCSoftware( _gpc, "ATVC_SOP" )
	{
		Ppos[0] = 0;
		Ppos[1] = 0;
		Ppos[2] = 0;
		Ypos[0] = 0;
		Ypos[1] = 0;
		Ypos[2] = 0;

		Rpos[0] = 0;
		Rpos[1] = 0;
		Tpos[0] = 0;
		Tpos[1] = 0;
		return;
	}

	ATVC_SOP::~ATVC_SOP( void )
	{
		return;
	}

	void ATVC_SOP::OnPostStep( double SimT, double DeltaT, double MJD )
	{
		if (GetMajorMode() == 304)
		{
			if (STS()->GetMachNumber() < 8)
			{
				Ppos[0] = ENTRYSTOWCHUTECONFIG_1P;
				Ppos[1] = ENTRYSTOWCHUTECONFIG_2P;
				Ppos[2] = ENTRYSTOWCHUTECONFIG_3P;
				Ypos[0] = ENTRYSTOWCHUTECONFIG_1Y;
				Ypos[1] = ENTRYSTOWCHUTECONFIG_2Y;
				Ypos[2] = ENTRYSTOWCHUTECONFIG_3Y;
			}
		}

		// TODO check for nozzle colision
		// TODO position limit
		for (int i = 0; i < 3; i++)
		{
			dopPpos[i].SetLine( (float)Ppos[i] );
			dopYpos[i].SetLine( (float)Ypos[i] );
		}

		for (int i = 0; i < 2; i++)
		{
			dopRpos[i].SetLine( (float)Rpos[i] );
			dopTpos[i].SetLine( (float)Tpos[i] );
		}
		//sprintf_s( oapiDebugString(), 255, "%.2f %.2f %.2f %.2f %.2f %.2f | %.2f %.2f %.2f %.2f", Ppos[0], Ppos[1], Ppos[2], Ypos[0], Ypos[1], Ypos[2], Rpos[0], Rpos[1], Tpos[0], Tpos[1] );
		return;
	}

	void ATVC_SOP::Realize( void )
	{
		DiscreteBundle* bundle = BundleManager()->CreateBundle( "ATVC", 10 );
		dopPpos[0].Connect( bundle, 0 );
		dopPpos[1].Connect( bundle, 1 );
		dopPpos[2].Connect( bundle, 2 );
		dopYpos[0].Connect( bundle, 3 );
		dopYpos[1].Connect( bundle, 4 );
		dopYpos[2].Connect( bundle, 5 );
		dopRpos[0].Connect( bundle, 6 );
		dopRpos[1].Connect( bundle, 7 );
		dopTpos[0].Connect( bundle, 8 );
		dopTpos[1].Connect( bundle, 9 );

		for (int i = 0; i < 3; i++)
		{
			dopPpos[i].SetLine( (float)Ppos[i] );
			dopYpos[i].SetLine( (float)Ypos[i] );
		}

		for (int i = 0; i < 2; i++)
		{
			dopRpos[i].SetLine( (float)Rpos[i] );
			dopTpos[i].SetLine( (float)Tpos[i] );
		}
		return;
	}

	bool ATVC_SOP::OnParseLine( const char* keyword, const char* value )
	{
		int config = 0;

		if (!_stricmp( keyword, "CONFIG" ))
		{
			sscanf_s( value, "%d", &config );
			switch (config)
			{
				/*case 0:// null
					Ppos[0] = 0;
					Ppos[1] = 0;
					Ppos[2] = 0;
					Ypos[0] = 0;
					Ypos[1] = 0;
					Ypos[2] = 0;
					break;*/
				case 1:// start config
					Ppos[0] = STARTCONFIG_1P;
					Ppos[1] = STARTCONFIG_2P;
					Ppos[2] = STARTCONFIG_3P;
					Ypos[0] = STARTCONFIG_1Y;
					Ypos[1] = STARTCONFIG_2Y;
					Ypos[2] = STARTCONFIG_3Y;
					break;
				case 2:// entry stow
					Ppos[0] = ENTRYSTOWCONFIG_1P;
					Ppos[1] = ENTRYSTOWCONFIG_2P;
					Ppos[2] = ENTRYSTOWCONFIG_3P;
					Ypos[0] = ENTRYSTOWCONFIG_1Y;
					Ypos[1] = ENTRYSTOWCONFIG_2Y;
					Ypos[2] = ENTRYSTOWCONFIG_3Y;
					break;
				case 3:// entry stow (drag chute)
					Ppos[0] = ENTRYSTOWCHUTECONFIG_1P;
					Ppos[1] = ENTRYSTOWCHUTECONFIG_2P;
					Ppos[2] = ENTRYSTOWCHUTECONFIG_3P;
					Ypos[0] = ENTRYSTOWCHUTECONFIG_1Y;
					Ypos[1] = ENTRYSTOWCHUTECONFIG_2Y;
					Ypos[2] = ENTRYSTOWCHUTECONFIG_3Y;
					break;
			}
			return true;
		}
		return false;
	}

	void ATVC_SOP::OnSaveState( FILEHANDLE scn ) const
	{
		int config = 0;

		if (Ypos[1] == STARTCONFIG_2Y)
		{
			config = 1;
		}
		else if (Ypos[1] == ENTRYSTOWCONFIG_2Y)
		{
			if (Ppos[1] == ENTRYSTOWCONFIG_2P)
			{
				config = 2;
			}
			else
			{
				config = 3;
			}
		}
		
		oapiWriteScenario_int( scn, "CONFIG", 1 );
		return;
	}

	bool ATVC_SOP::OnMajorModeChange( unsigned int newMajorMode )
	{
		switch (newMajorMode)
		{
			case 101:
			case 102:
			case 103:
			case 104:
			case 304:
			case 305:
			case 601:
			case 602:
			case 603:
				return true;
			default:
				return false;
		}
	}

	void ATVC_SOP::SetSSMEActPos( int num, double Ppos, double Ypos )
	{
		assert( (num >= 1) && (num <= 3) && "ATVC_SOP::SetSSMEActPos.num" );

		this->Ppos[num - 1] = Ppos;
		this->Ypos[num - 1] = Ypos;
		return;
	}

	void ATVC_SOP::SetSRBActPos( int num, double Rpos, double Tpos )
	{
		assert( (num >= 1) && (num <= 2) && "ATVC_SOP::SetSRBActPos.num" );

		this->Rpos[num - 1] = Rpos;
		this->Tpos[num - 1] = Tpos;
		return;
	}
}