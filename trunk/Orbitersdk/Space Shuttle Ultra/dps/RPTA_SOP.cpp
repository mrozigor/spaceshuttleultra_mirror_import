#include "RPTA_SOP.h"
#include "RPTA_RM.h"
#include "assert.h"


namespace dps
{
	RPTA_SOP::RPTA_SOP( SimpleGPCSystem *_gpc ):SimpleGPCSoftware( _gpc, "RPTA_SOP" )
	{
		RPTA = 0.0;
		return;
	}

	RPTA_SOP::~RPTA_SOP( void )
	{
		return;
	}

	void RPTA_SOP::Realize( void )
	{
		pRPTA_RM = static_cast<RPTA_RM*>(FindSoftware( "RPTA_RM" ));
		assert( (pRPTA_RM != NULL) && "RPTA_SOP::Realize.pRPTA_RM" );
		return;
	}

	void RPTA_SOP::OnPostStep( double SimT, double DeltaT, double MJD )
	{
		double L = 0.0;
		bool DG_L = false;
		double R = 0.0;
		bool DG_R = false;

		// get data from RM
		pRPTA_RM->GetRPTAData_L( L, DG_L );
		pRPTA_RM->GetRPTAData_R( R, DG_R );

		// convert to degrees (TODO should be [-22.9, +22.8])
		L *= 22.8;// [-22.8, +22.8]
		R *= 22.8;// [-22.8, +22.8]

		// check data good flag
		if (DG_L == false) L = 0.0;
		if (DG_R == false) R = 0.0;
		
		// output largest magnitude cmd
		if (fabs( L ) > fabs( R )) RPTA = L;// output will be L if they are symmetrical
		else RPTA = R;
		return;
	}

	bool RPTA_SOP::OnParseLine( const char* keyword, const char* value )
	{
		return false;
	}

	void RPTA_SOP::OnSaveState( FILEHANDLE scn ) const
	{
		return;
	}

	bool RPTA_SOP::OnMajorModeChange( unsigned int newMajorMode )
	{
		switch (newMajorMode)
		{
			case 304:
			case 305:
			case 602:
			case 603:
				return true;
			default:
				return false;
		}
	}

	double RPTA_SOP::GetYawCommand( void ) const
	{
		return RPTA;
	}
}