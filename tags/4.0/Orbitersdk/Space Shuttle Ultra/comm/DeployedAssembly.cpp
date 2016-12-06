#include "DeployedAssembly.h"
#include "..\Atlantis.h"


namespace comm 
{
	DeployedAssembly::DeployedAssembly( AtlantisSubsystemDirector* _director ):AtlantisSubsystem( _director, "DeployedAssembly" )
	{
		alpha = 124.3;
		beta = -27.5;
		LockingPin1 = 1;
		LockingPin2 = 1;
		alphapos = (alpha + 154) / 360;
		betapos = (beta + 75) / 160;
	}

	DeployedAssembly::~DeployedAssembly()
	{
	}

	void DeployedAssembly::Realize( void )
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "KUAntennaControl", 16 );
		BoomStowEnableII.Connect( pBundle, 6 );
		AlphaMIP.Connect( pBundle, 7 );
		BetaMIP.Connect( pBundle, 8 );
		GimbalLockMotors.Connect( pBundle, 9 );
		GimbalMotorAlpha.Connect( pBundle, 10 );
		GimbalMotorBeta.Connect( pBundle, 11 );
		AlphaEncoder.Connect( pBundle, 12 );
		BetaEncoder.Connect( pBundle, 13 );
		return;
	}

	void DeployedAssembly::OnPostStep( double fSimT, double fDeltaT, double fMJD )
	{
		// locking pins
		if (GimbalLockMotors.GetVoltage() > 0)
		{
			// lock
			if (LockingPin1 < 1.0)
			{
				// drive 1
				LockingPin1 += LOCKINGPIN1_SPEED * fDeltaT;
				if (LockingPin1 > 1.0) LockingPin1 = 1.0;
			}
			else
			{
				// drive 2
				LockingPin2 += LOCKINGPIN2_SPEED * fDeltaT;
				if (LockingPin2 > 1.0) LockingPin2 = 1.0;
			}
		}
		else if (GimbalLockMotors.GetVoltage() < 0)
		{
			// unlock
			// drive 1
			LockingPin1 -= LOCKINGPIN1_SPEED * fDeltaT;
			if (LockingPin1 < 0.0) LockingPin1 = 0.0;
			// drive 2
			LockingPin2 -= LOCKINGPIN2_SPEED * fDeltaT;
			if (LockingPin2 < 0.0) LockingPin2 = 0.0;
		}

		// gimbal motors
		double newalpha = alpha;
		double newbeta = beta;
		if ((LockingPin1 < 0.5) || (LockingPin2 < 0.5))
		{
			newalpha += GimbalMotorAlpha.GetVoltage() * fDeltaT;
			newbeta += GimbalMotorBeta.GetVoltage() * fDeltaT;
		}

		// check for stop
		if (newalpha > 206) newalpha = 206;
		else if (newalpha < -154) newalpha = -154;
		if (newbeta > 85) newbeta = 85;
		else if (newbeta < -75) newbeta = -75;

		// MIPs (use "passed by position" logic instead of "is at position")
		if (((newalpha >= ALPHA_MASTER_PULSE_INDEX) && (ALPHA_MASTER_PULSE_INDEX >= alpha)) || ((newalpha <= ALPHA_MASTER_PULSE_INDEX) && (ALPHA_MASTER_PULSE_INDEX <= alpha))) AlphaMIP.SetLine(); 
		else AlphaMIP.ResetLine();
		if (((newbeta >= BETA_MASTER_PULSE_INDEX) && (BETA_MASTER_PULSE_INDEX >= beta)) || ((newbeta <= BETA_MASTER_PULSE_INDEX) && (BETA_MASTER_PULSE_INDEX <= beta))) BetaMIP.SetLine(); 
		else BetaMIP.ResetLine();

		// save and output positions
		alpha = newalpha;
		beta = newbeta;
		AlphaEncoder.SetLine( (float)alpha );
		BetaEncoder.SetLine( (float)beta );

		// output boom stow enable II
		BoomStowEnableII.SetLine( 5.0f * (int)(LockingPin2 == 1.0) );

		// animation
		alphapos = (alpha + 154) / 360;
		betapos = (beta + 75) / 160;
		STS()->SetKuGimbalAngles( alphapos, betapos );
		return;
	}

	bool DeployedAssembly::OnParseLine( const char* line )
	{
		if (!_strnicmp( line, "ALPHA", 5 ))
		{
			sscanf_s( line + 5, "%lf", &alpha );
			if (alpha > 206) alpha = 206;
			else if (alpha < -154) alpha = -154;
			alphapos = (alpha + 154) / 360;
			return true;
		}
		else if (!_strnicmp( line, "BETA", 4 ))
		{
			sscanf_s( line + 4, "%lf", &beta );
			if (beta > 85) beta = 85;
			else if (beta < -75) beta = -75;
			betapos = (beta + 75) / 160;
			return true;
		}
		else if (!_strnicmp( line, "LOCKINGPIN1", 11 ))
		{
			sscanf_s( line + 11, "%lf", &LockingPin1 );
			return true;
		}
		else if (!_strnicmp( line, "LOCKINGPIN2", 11 ))
		{
			sscanf_s( line + 11, "%lf", &LockingPin2 );
			return true;
		}
		return false;
	}

	void DeployedAssembly::OnSaveState( FILEHANDLE scn ) const
	{
		oapiWriteScenario_float( scn, "ALPHA", alpha );
		oapiWriteScenario_float( scn, "BETA", beta );
		if (LockingPin1 < 1) oapiWriteScenario_float( scn, "LOCKINGPIN1", LockingPin1 );
		if (LockingPin2 < 1) oapiWriteScenario_float( scn, "LOCKINGPIN2", LockingPin2 );
		return;
	}

	double DeployedAssembly::GetAlphaAnimation( void ) const
	{
		return alphapos;
	}

	double DeployedAssembly::GetBetaAnimation( void ) const
	{
		return betapos;
	}
};