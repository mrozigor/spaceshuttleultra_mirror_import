/****************************************************************************
  This file is part of Space Shuttle Ultra

  Deployed Assembly definition



  Space Shuttle Ultra is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Space Shuttle Ultra is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Space Shuttle Ultra; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See http://spaceshuttleultra.sourceforge.net/license/ for more details.

  **************************************************************************/

#ifndef _comm_DA_H_
#define _comm_DA_H_


#include "orbitersdk.h"
#include "discsignals.h"
#include "..\AtlantisSubsystem.h"


namespace comm
{
	// no clue what the real values are for the locking pins
	const double LOCKINGPIN1_SPEED = 0.071429;// 1/sec
	const double LOCKINGPIN2_SPEED = 0.142857;// 1/sec

	const double ALPHA_MASTER_PULSE_INDEX = 116.5;// deg
	const double BETA_MASTER_PULSE_INDEX = -23.25;// deg

	/**
	 * Structural and mechanical model of the deployed assembly.
	 */
	class DeployedAssembly:public AtlantisSubsystem
	{
			double alpha;
			double beta;
			double LockingPin1;
			double LockingPin2;
			double alphapos;
			double betapos;

			DiscOutPort BoomStowEnableII;
			DiscOutPort AlphaMIP;
			DiscOutPort BetaMIP;
			DiscInPort GimbalLockMotors;
			DiscInPort GimbalMotorAlpha;
			DiscInPort GimbalMotorBeta;
			DiscOutPort AlphaEncoder;
			DiscOutPort BetaEncoder;

		public:
			DeployedAssembly( AtlantisSubsystemDirector* _director );
			virtual ~DeployedAssembly();

			void Realize( void );
			void OnPostStep( double fSimT, double fDeltaT, double fMJD );

			virtual bool SingleParamParseLine() const {return true;};
			bool OnParseLine( const char* line );
			void OnSaveState( FILEHANDLE scn ) const;

			double GetAlphaAnimation() const;
			double GetBetaAnimation() const;

			void CommandGimbalLock( bool unlock );
	};

};

#endif// _comm_DA_H_
