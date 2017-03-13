#include "SBTC_SOP.h"
#include "SBTC_RM.h"
#include "SSME_SOP.h"
#include "AscentDAP.h"
#include "SSME_Operations.h"
#include "assert.h"


namespace dps
{
	SBTC_SOP::SBTC_SOP( SimpleGPCSystem *_gpc ):SimpleGPCSoftware( _gpc, "SBTC_SOP" )
	{
		MAN_THROTTLE = false;
		MAN_THROTTLE_ACTIVE = false;
		MAN_THROTTLE_COMMAND = 0.0;

		CDR_TAKEOVER = false;
		PLT_TAKEOVER = false;
		MAN_SPEEDBRAKE_COMMAND = 0.0;
		CDR_DATA = true;
		return;
	}

	SBTC_SOP::~SBTC_SOP( void )
	{
		return;
	}

	void SBTC_SOP::Realize( void )
	{
		DiscreteBundle* pBundle = BundleManager()->CreateBundle( "DAP_CH_CONTROLS", 16 );
		CDR_SPDBK_THROT.Connect( pBundle, 8 );// HACK should first go to switch RM
		PLT_SPDBK_THROT.Connect( pBundle, 9 );// HACK should first go to switch RM

		pBundle = BundleManager()->CreateBundle( "ACA1_4", 16 );
		CDR_SPDBK_THROT_AUTO_LT.Connect( pBundle, 0 );
		CDR_SPDBK_THROT_MAN_LT.Connect( pBundle, 4 );

		pBundle = BundleManager()->CreateBundle( "ACA2_4", 16 );
		PLT_SPDBK_THROT_AUTO_LT.Connect( pBundle, 2 );
		PLT_SPDBK_THROT_MAN_LT.Connect( pBundle, 6 );

		pSBTC_RM = static_cast<SBTC_RM*>(FindSoftware( "SBTC_RM" ));
		assert( (pSBTC_RM != NULL) && "SBTC_SOP::Realize.pSBTC_RM" );

		pSSME_SOP = static_cast<SSME_SOP*>(FindSoftware( "SSME_SOP" ));
		assert( (pSSME_SOP != NULL) && "SBTC_SOP::Realize.pSSME_SOP" );

		pAscentDAP = static_cast<AscentDAP*>(FindSoftware( "AscentDAP" ));
		assert( (pAscentDAP != NULL) && "SBTC_SOP::Realize.pAscentDAP" );

		pSSME_Operations = static_cast<SSME_Operations*> (FindSoftware( "SSME_Operations" ));
		assert( (pSSME_Operations != NULL) && "SBTC_SOP::Realize.pSSME_Operations" );
		return;
	}

	void SBTC_SOP::OnPostStep( double SimT, double DeltaT, double MJD )
	{
		switch (GetMajorMode())
		{
			case 304:
			case 305:
			case 602:
			case 603:
			case 801:
				ManSpeedbrake();// man speedbrake
				break;
			default:
				if (pSSME_Operations->GetMECOConfirmedFlag() == false) ManThrottle();// man throttle
				else
				{
					// lights off
					CDR_SPDBK_THROT_AUTO_LT.ResetLine();
					CDR_SPDBK_THROT_MAN_LT.ResetLine();
					PLT_SPDBK_THROT_AUTO_LT.ResetLine();
					PLT_SPDBK_THROT_MAN_LT.ResetLine();
				}
				break;
		}
		return;
	}

	bool SBTC_SOP::OnParseLine( const char* keyword, const char* value )
	{
		if (!_strnicmp( keyword, "MAN_THROTTLE", 12 ))
		{
			if (!_strnicmp( value, "TRUE", 4 ))
			{
				MAN_THROTTLE = true;
				MAN_THROTTLE_ACTIVE = true;
			}
			else
			{
				MAN_THROTTLE = false;
				MAN_THROTTLE_ACTIVE = false;
			}
			return true;
		}
		else if (!_strnicmp( keyword, "DATA", 4 ))
		{
			if (!_strnicmp( value, "PLT", 3 )) CDR_DATA = false;
			else CDR_DATA = true;
			return true;
		}
		return false;
	}

	void SBTC_SOP::OnSaveState( FILEHANDLE scn ) const
	{
		if (MAN_THROTTLE_ACTIVE == true) oapiWriteScenario_string( scn, "MAN_THROTTLE", "TRUE" );
		if (CDR_DATA == false) oapiWriteScenario_string( scn, "DATA", "PLT" );
		return;
	}

	bool SBTC_SOP::OnMajorModeChange( unsigned int newMajorMode )
	{
		if (newMajorMode == 101)
		{
			// AUTO lights on prelaunch
			CDR_SPDBK_THROT_AUTO_LT.SetLine();
			CDR_SPDBK_THROT_MAN_LT.ResetLine();
			PLT_SPDBK_THROT_AUTO_LT.SetLine();
			PLT_SPDBK_THROT_MAN_LT.ResetLine();
		}

		switch (newMajorMode)
		{
			case 102:
			case 103:
			case 304:
			case 305:
			case 601:
			case 602:
			case 603:
			case 801:
				return true;
			default:
				return false;
		}
	}

	void SBTC_SOP::ManSpeedbrake( void )
	{
		double L = 0.0;
		bool DG_L = false;
		bool TO_L = false;
		double R = 0.0;
		bool DG_R = false;
		bool TO_R = false;

		// get data from RM
		pSBTC_RM->GetSBTCData_L( L, DG_L, TO_L );
		pSBTC_RM->GetSBTCData_R( R, DG_R, TO_R );

		// TODO what to do when DG flag is false?

		// convert to degrees
		L = (1.0 - L) * 98.6;
		R = (1.0 - R) * 98.6;

		// AUTO/MAN speedbrake logic
		if (TO_L == true)
		{
			// CDR
			CDR_TAKEOVER = true;
			PLT_TAKEOVER = false;
			CDR_DATA = true;
		}
		else if (TO_R == true)
		{
			// PLT
			CDR_TAKEOVER = false;
			PLT_TAKEOVER = true;
			CDR_DATA = false;
		}
		else
		{
			CDR_TAKEOVER = false;
			PLT_TAKEOVER = false;
		}

		// data switch
		if (CDR_DATA == true) MAN_SPEEDBRAKE_COMMAND = L;
		else MAN_SPEEDBRAKE_COMMAND = R;
		return;
	}

	void SBTC_SOP::ManThrottle( void )
	{
		double R = 0.0;
		bool DG_R = false;
		bool TO_R = false;

		// get data from RM
		pSBTC_RM->GetSBTCData_R( R, DG_R, TO_R );

		// TODO what to do when DG flag is false?

		// convert to throttle percentage
		// TODO get the correct numbers
		double KMIN = 67.0;
		double KMAX = 104.5;
		R = 67.0 + (R * (KMAX - KMIN));
		
		// AUTO/MAN throttle logic
		if (MAN_THROTTLE == true)
		{
			if (MAN_THROTTLE_ACTIVE == true)
			{
				// full MAN control
				pSSME_SOP->SetThrottlePercent( R );

				// check for AUTO
				if (CDR_SPDBK_THROT.IsSet() || PLT_SPDBK_THROT.IsSet())
				{
					MAN_THROTTLE = false;
					MAN_THROTTLE_ACTIVE = false;
					CDR_SPDBK_THROT_AUTO_LT.SetLine();
					CDR_SPDBK_THROT_MAN_LT.ResetLine();
					PLT_SPDBK_THROT_AUTO_LT.SetLine();
					PLT_SPDBK_THROT_MAN_LT.ResetLine();
				}
			}
			else
			{
				if (TO_R == true)
				{
					// check for 4% tolerance
					if (fabs( R - pAscentDAP->GetAutoThrottleCommand() ) < 4.0)
					{
						MAN_THROTTLE_ACTIVE = true;
						CDR_SPDBK_THROT_AUTO_LT.ResetLine();
						CDR_SPDBK_THROT_MAN_LT.ResetLine();
						PLT_SPDBK_THROT_AUTO_LT.ResetLine();
						PLT_SPDBK_THROT_MAN_LT.SetLine();
						pSSME_SOP->SetThrottlePercent( R );
					}
				}
				else
				{
					// back to AUTO
					MAN_THROTTLE = false;
					CDR_SPDBK_THROT_AUTO_LT.SetLine();
					CDR_SPDBK_THROT_MAN_LT.ResetLine();
					PLT_SPDBK_THROT_AUTO_LT.SetLine();
					PLT_SPDBK_THROT_MAN_LT.ResetLine();
				}
			}
		}
		else
		{
			// check for MAN
			if (TO_R == true)
			{
				MAN_THROTTLE = true;
				CDR_SPDBK_THROT_AUTO_LT.ResetLine();
				CDR_SPDBK_THROT_MAN_LT.ResetLine();
				PLT_SPDBK_THROT_AUTO_LT.ResetLine();
				PLT_SPDBK_THROT_MAN_LT.ResetLine();
			}
		}
		return;
	}

	bool SBTC_SOP::GetManThrottle( void ) const
	{
		return MAN_THROTTLE;
	}

	double SBTC_SOP::GetManThrottleCommand( void ) const
	{
		return MAN_THROTTLE_COMMAND;
	}

	bool SBTC_SOP::GetCDRTakeover( void ) const
	{
		return CDR_TAKEOVER;
	}

	bool SBTC_SOP::GetPLTTakeover( void ) const
	{
		return PLT_TAKEOVER;
	}

	double SBTC_SOP::GetManSpeedbrakeCommand( void ) const
	{
		return MAN_SPEEDBRAKE_COMMAND;
	}
}