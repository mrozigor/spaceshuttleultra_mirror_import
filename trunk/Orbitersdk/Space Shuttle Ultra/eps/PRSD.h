/****************************************************************************
  This file is part of Space Shuttle Ultra

  PRSD definition



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

#ifndef _comm_PRSD_H_
#define _comm_PRSD_H_


//#include "orbitersdk.h"
#include "discsignals.h"
#include "..\AtlantisSubsystem.h"
#include "PressureSource.h"
#include "SolenoidValve.h"
#include "ValveManager.h"
#include "Sensor.h"


namespace eps
{
	const double R = 8.314;
	const double AR_O2 = 31.99880;
	const double AR_H2 = 2.01588;

	const double O2_TANK_DAY_MASS = 97.52236;// Kg
	const double O2_TANK_MASS_CAP = 354255.6;// g
	const double O2_TANK_VOL = 40.5058;// m^3
	const double O2_TANK_INITIAL_TEMP = 97.0;// K

	const double H2_TANK_DAY_MASS = 102.9655;// Kg
	const double H2_TANK_MASS_CAP = 41730.5;// g
	const double H2_TANK_VOL = 40.5058;// m^3
	const double H2_TANK_INITIAL_TEMP = 22.0;// K

	const double EDO_KIT_MASS = 123;// Kg (TODO)
	const double EDO_PALLET_DRY_MASS = 789.25;// Kg

	class PRSD:public AtlantisSubsystem, public PressureSource
	{
		private:
			int sets;
			vector<double> O2mass;// g
			vector<double> O2press;// Pa
			vector<double> O2temp;// K
			vector<double> H2mass;// g
			vector<double> H2press;// Pa
			vector<double> H2temp;// K

			int pallets;
			bool EDOhw;

			ValveManager vman;

			SolenoidLatchingValve* O2Manif[2];
			SolenoidLatchingValve* H2Manif[2];

			SolenoidLatchingValve* FCO2Reac[3];
			SolenoidLatchingValve* FCH2Reac[3];

			SolenoidLatchingValve* O2ECLSSSupply[2];

			vector<Sensor> O2TKqty;
			vector<Sensor> O2TKpress;
			vector<Sensor> O2TKtemp;
			vector<Sensor> O2TKhtrcntrpress;
			vector<Sensor> O2TKhtrassytemp1;
			vector<Sensor> O2TKhtrassytemp2;
			vector<Sensor> H2TKqty;
			vector<Sensor> H2TKpress;
			vector<Sensor> H2TKtemp;
			vector<Sensor> H2TKhtrcntrpress;
			vector<Sensor> H2TKhtrassytemp;

			Sensor* O2ManifPress[2];
			Sensor* H2ManifPress[2];

			vector<DiscInPort> O2HeatersA_On;
			vector<DiscInPort> O2HeatersA_Auto;
			vector<DiscInPort> O2HeatersB_On;
			vector<DiscInPort> O2HeatersB_Auto;
			vector<DiscInPort> O2Heaters_Test;
			vector<DiscInPort> O2Heaters_Reset;
			vector<DiscInPort> H2HeatersA_On;
			vector<DiscInPort> H2HeatersA_Auto;
			vector<DiscInPort> H2HeatersB_On;
			vector<DiscInPort> H2HeatersB_Auto;

		public:
			PRSD( int internalsets, bool EDOhw, int pallets, AtlantisSubsystemDirector* _director );
			virtual ~PRSD();

			void Realize( void );
			void OnPreStep( double fSimT, double fDeltaT, double fMJD );

			virtual bool SingleParamParseLine() const {return true;};
			bool OnParseLine( const char* line );
			void OnSaveState( FILEHANDLE scn ) const;

			double GetSubsystemEmptyMass() const;

			double Use( double flow, int source );
	};

};

#endif// _comm_PRSD_H_
