/****************************************************************************
  This file is part of Space Shuttle Ultra

  Electronics Assembly 1 definition



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

#ifndef _comm_EA1_H_
#define _comm_EA1_H_


#include "..\AtlantisSubsystem.h"
#include <discsignals.h>
#include <orbitersdk.h>


namespace comm
{
	const double SLEW_RATE_SLOW = 0.4;// deg/sec
	const double SLEW_RATE_FAST = 20;// deg/sec

	const double STOW_ALPHA = 124.3;// deg
	const double STOW_BETA = -27.5;// deg

	const double COS_67 = 0.390731128489;
	const double SIN_67 = 0.920504853452;

	const double SCAN_CONE_HALF_ANGLE = 30;// deg


	class DeployedAssembly;

	class ElectronicsAssembly1:public AtlantisSubsystem
	{
			DeployedAssembly* pDeployedAssembly;
			
			DiscInPort pSlewAzimuth_R;
			DiscInPort pSlewAzimuth_L;
			DiscInPort pSlewElevation_Down;
			DiscInPort pSlewElevation_Up;
			DiscInPort pSlewRate_Fast;
			DiscInPort pSearch;
			DiscInPort pRadarOutput_Low;
			DiscInPort pRadarOutput_High;
			//DiscOutPort pScanWarnTB;
			DiscOutPort pTrackTB;
			DiscOutPort pSearchTB;

			DiscInPort pPower_Off;
			DiscInPort pPower_On;
			DiscInPort COMON;
			DiscInPort RADSTB;
			DiscInPort COMSTB;
			DiscInPort RADON;
			DiscInPort GPC;
			DiscInPort GPCDES;
			DiscInPort AUTO;
			DiscInPort MANUAL;

			DiscOutPort pRange;
			DiscOutPort pRangeRate;
			DiscOutPort pElevation;
			DiscOutPort pAzimuth;
			DiscOutPort pELrate0_20;
			DiscOutPort pELrate0_2;
			DiscOutPort pAZrate0_20;
			DiscOutPort pAZrate0_2;

			DiscInPort BOOM;
			DiscInPort XMIT;
			DiscOutPort BOOMST;
			DiscInPort AlphaMIP;
			DiscInPort BetaMIP;
			DiscOutPort GimbalLockMotors;
			DiscOutPort GimbalMotorAlpha;
			DiscOutPort GimbalMotorBeta;
			DiscInPort AlphaEncoder;
			DiscInPort BetaEncoder;

			DiscOutPort KUaz_EA2;
			DiscOutPort KUel_EA2;
			DiscOutPort RADSTB_EA2;
			DiscOutPort RADON_EA2;
			DiscOutPort RADARPOWERLOW_EA2;
			DiscOutPort RADARPOWERMEDIUM_EA2;
			DiscInPort Range_EA2;
			DiscInPort RangeRate_EA2;
			DiscInPort Detect_EA2;
			DiscInPort TGTaz_EA2;
			DiscInPort TGTel_EA2;

			bool ValidDeploy;

			double simt;
			double lastsimt;
			double dt;
			double timer;

			int deploy;

			bool AlphaMIPdone;
			bool BetaMIPdone;

			bool WiggleTestPass;
			double WiggleTestAlpha;
			double WiggleTestBeta;

			bool scan;
			bool scan_in;
			double scan_az;
			double scan_el;
			double scan_az_delta;
			double scan_el_delta;

			bool target;

			double tgt_elOLD;
			double tgt_azOLD;

			double old_alpha;
			double old_beta;

			double GimbalLockMotorsPower;
			bool GimbalLockMotorsENA;

			double omegaalpha;
			double omegabeta;

			double cur_alpha;
			double cur_beta;
			double cur_el;
			double cur_az;

			bool ZONEO;
			bool ZONEI;

			bool bBOOMST;

			void ShuttleToGimbal( double _EL, double _AZ, double &_alpha, double &_beta );
			void GimbalToShuttle( double _alpha, double _beta, double &_EL, double &_AZ );
			void AngleTransform( double &A, double &B, double C, double D, double K );
			void PositionLoop( double des_alpha, double des_beta, bool man = false );

			void Procedure_Initialize( void );
			void Procedure_Procedure_termination( void );
			//void Procedure_Recover( void );
			void Procedure_Idle( void );
			void Procedure_Deploy( void );
			void Procedure_Point( void );
			void Procedure_Slew( bool fast, bool right, bool left, bool up, bool down );
			void Procedure_Scan( void );
			void Procedure_Track( void );
			void Procedure_Self_test( void );

			bool RetractLockingPins( void );
			bool InsertLockingPins( void );
			bool InitializeEncoders( void );
			bool DriveToZenith( void );
			bool DriveToStow( void );
			bool WiggleTest( void );
			bool GimbalStopCheck( void );

			void PanelOutput( double r, double rr );

		public:
			ElectronicsAssembly1( AtlantisSubsystemDirector* _director, DeployedAssembly* pDeployedAssembly );
			virtual ~ElectronicsAssembly1();

			void Realize( void );
			void OnPostStep( double fSimT, double fDeltaT, double fMJD );

			virtual bool SingleParamParseLine() const {return true;};
			bool OnParseLine( const char* line );
			void OnSaveState( FILEHANDLE scn ) const;
	};

};

#endif// _comm_EA1_H_