/****************************************************************************
  This file is part of Space Shuttle Ultra

  Helium System definition



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
#ifndef _mps_HESYS_H_
#define _mps_HESYS_H_


#include "PressureSource.h"
#include "..\AtlantisSubsystem.h"
#include "SolenoidValve.h"
#include "Sensor.h"


namespace mps
{
	using discsignals::DiscreteBundle;
	class HeSysPneu;

	const double R = 8.314;
	const double AR_HE = 4.00260222;
	const double TANK_VOL_ENG = 0.7560598;// m^3
	const double TANK_VOL_PNEU = 0.1330892;// m^3

	const int TANK_TEMP = 303;// K
	const int INITIAL_HE_MASS_ENG = 34000;//30500;// g
	const int INITIAL_HE_MASS_PNEU = 5897;// g
	const int MAX_REG_FLOW = 400;// g/s
	const int MAX_TANK_FLOW = 600;// g/s

	class HeSysEng:public AtlantisSubsystem, public PressureSource
	{
		// 2 4.7cf
		// 1 17.3cf
		private:
			int ID;
			HeSysEng* sys1;// not system 1, but one of the other 2 systems
			HeSysEng* sys2;// not system 2, but one of the other 2 systems
			HeSysPneu* sysP;// pneumatic system

			SolenoidValve* vlvISOL_A;// LV1 / LV3 / LV5
			SolenoidValve* vlvISOL_B;// LV2 / LV4 / LV6

			SolenoidValve* vlvInIC;// LV59 / LV61 / LV63
			SolenoidValve* vlvOutIC;// LV60 / LV62 / LV64

			double HeMass;
			double TankPress;
			double RegPress;// TODO one sensor per reg
			double TotalFlow;

			Sensor TankPressure;
			Sensor RegPressure[2];
		public:
			HeSysEng( AtlantisSubsystemDirector* _director, const string& _ident, int ID );
			~HeSysEng( void );

			void Realize( void );
			void OnPostStep( double fSimT, double fDeltaT, double fMJD );

			bool SingleParamParseLine() const {return true;};

			void OnSaveState( FILEHANDLE scn ) const;
			bool OnParseLine( const char* line );

			double Use( double flow );// flow g/s
			double UseTank( double mass );
			double FillTank( double mass );

			int GetTankPress( void ) const;
			int GetRegPress( void ) const;
			double IsOutICOpen( void ) const;
	};


	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////


	class HeSysPneu:public AtlantisSubsystem, public PressureSource
	{
		friend class HeSysEng;
		// 1 4.7cf
		private:
			HeSysEng* sys1;// system 1
			HeSysEng* sys2;// system 2
			HeSysEng* sys3;// system 3

			SolenoidValve* vlvISOL_A;// LV7
			SolenoidValve* vlvISOL_B;// LV8

			SolenoidValve* vlvLEngXOVR;// LV10

			double HeMass;
			double TankPress;
			double RegPress;
			double TotalFlow;

			Sensor TankPressure;
			Sensor RegPressure;
		public:
			HeSysPneu( AtlantisSubsystemDirector* _director, const string& _ident );
			~HeSysPneu( void );

			void Realize( void );
			void OnPostStep( double fSimT, double fDeltaT, double fMJD );

			bool SingleParamParseLine() const {return true;};

			void OnSaveState( FILEHANDLE scn ) const;
			bool OnParseLine( const char* line );

			double Use( double flow );// flow g/s
			double UseTank( double mass );
			double FillTank( double mass );

			int GetTankPress( void ) const;
			int GetRegPress( void ) const;
	};
}

#endif // _mps_HESYS_H_