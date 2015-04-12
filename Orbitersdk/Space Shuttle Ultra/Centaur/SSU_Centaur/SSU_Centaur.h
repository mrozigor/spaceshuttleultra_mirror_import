/****************************************************************************
  This file is part of Space Shuttle Ultra

  Centaur G/G-Prime definition



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
#ifndef __SSUSC_H
#define __SSUSC_H


#include "orbitersdk.h"


const static char* G_MESHNAME = "SSU\\CentaurG";
const static char* GPRIME_MESHNAME = "SSU\\CentaurG_Prime";

const VECTOR3 RL10_C1_POS_G = _V( 0, 0.67, -2.3156 );
const VECTOR3 RL10_C1_POS_GPRIME = _V( 0, 0.67, -5.9 );
const VECTOR3 RL10_C1_DIR = _V( 0, 0, 1 );
const VECTOR3 RL10_C2_POS_G = _V( 0, -0.67, -2.3156 );
const VECTOR3 RL10_C2_POS_GPRIME = _V( 0, -0.67, -5.9 );
const VECTOR3 RL10_C2_DIR = _V( 0, 0, 1 );
// quad I
const VECTOR3 ACS_P1_POS_G = _V( 0.9524, 1.1165, -0.5619 );
const VECTOR3 ACS_P1_POS_GPRIME = _V( 0.9524, 1.1165, -4.1463 );
const VECTOR3 ACS_P1_DIR = _V( 0, -1, 0 );
const VECTOR3 ACS_Y1_POS_G = _V( 1.0825, 0.9864, -0.5619 );
const VECTOR3 ACS_Y1_POS_GPRIME = _V( 1.0825, 0.9864, -4.1463 );
const VECTOR3 ACS_Y1_DIR = _V( -1, 0, 0 );
// quad II
const VECTOR3 ACS_Y2_POS_G = _V( 1.0825, -0.9864, -0.5619 );
const VECTOR3 ACS_Y2_POS_GPRIME = _V( 1.0825, -0.9864, -4.1463 );
const VECTOR3 ACS_Y2_DIR = _V( -1, 0, 0 );
const VECTOR3 ACS_S2A_POS_G = _V( 1.0038, -1.0122, -0.6378 );
const VECTOR3 ACS_S2A_POS_GPRIME = _V( 1.0038, -1.0122, -4.2222 );
const VECTOR3 ACS_S2A_DIR = _V( 0, 0, 1 );
const VECTOR3 ACS_S2B_POS_G = _V( 0.9732, -1.0422, -0.6378 );
const VECTOR3 ACS_S2B_POS_GPRIME = _V( 0.9732, -1.0422, -4.2222 );
const VECTOR3 ACS_S2B_DIR = _V( 0, 0, 1 );
const VECTOR3 ACS_P2_POS_G = _V( 0.9524, -1.1165, -0.5619 );
const VECTOR3 ACS_P2_POS_GPRIME = _V( 0.9524, -1.1165, -4.1463 );
const VECTOR3 ACS_P2_DIR = _V( 0, 1, 0 );
// quad III
const VECTOR3 ACS_P3_POS_G = _V( -0.9524, -1.1165, -0.5619 );
const VECTOR3 ACS_P3_POS_GPRIME = _V( -0.9524, -1.1165, -4.1463 );
const VECTOR3 ACS_P3_DIR = _V( 0, 1, 0 );
const VECTOR3 ACS_Y3_POS_G = _V( -1.0825, -0.9864, -0.5619 );
const VECTOR3 ACS_Y3_POS_GPRIME = _V( -1.0825, -0.9864, -4.1463 );
const VECTOR3 ACS_Y3_DIR = _V( 1, 0, 0 );
// quad IV
const VECTOR3 ACS_Y4_POS_G = _V( -1.0825, 0.9864, -0.5619 );
const VECTOR3 ACS_Y4_POS_GPRIME = _V( -1.0825, 0.9864, -4.1463 );
const VECTOR3 ACS_Y4_DIR = _V( 1, 0, 0 );
const VECTOR3 ACS_S4A_POS_G = _V( -1.0038, 1.0122, -0.6378 );
const VECTOR3 ACS_S4A_POS_GPRIME = _V( -1.0038, 1.0122, -4.2222 );
const VECTOR3 ACS_S4A_DIR = _V( 0, 0, 1 );
const VECTOR3 ACS_S4B_POS_G = _V( -0.9732, 1.0422, -0.6378 );
const VECTOR3 ACS_S4B_POS_GPRIME = _V( -0.9732, 1.0422, -4.2222 );
const VECTOR3 ACS_S4B_DIR = _V( 0, 0, 1 );
const VECTOR3 ACS_P4_POS_G = _V( -0.9524, 1.1165, -0.5619 );
const VECTOR3 ACS_P4_POS_GPRIME = _V( -0.9524, 1.1165, -4.1463 );
const VECTOR3 ACS_P4_DIR = _V( 0, -1, 0 );

const double G_EMPTY_MASS = 3243.185;// Kg
const double G_RL10_PROPELLANT_MASS = 14560.32;// Kg
const double G_ACS_PROPELLANT_MASS = 90.71847;// Kg
const double GPRIME_EMPTY_MASS = 2948.35;// Kg
const double GPRIME_RL10_PROPELLANT_MASS = 20411.66;// Kg
const double GPRIME_ACS_PROPELLANT_MASS = 113.3981;// Kg
const double RL10A33A_THRUST_VAC = 73309.5;// N
const double RL10A33A_ISP_VAC = 446.6 * G;// m/s
const double RL10A33B_THRUST_VAC = 66723.32;// N
const double RL10A33B_ISP_VAC = 436.0 * G;// m/s
const double ACS_THRUST_VAC = 26.64;// N
const double ACS_ISP_VAC = 324 * G;// m/s
const double GHe_MASS = 2;// Kg

const double ACS_ENA_DELAY = 300;// s
const double RL10_ENA_DELAY = 300;//2700;// s
// also using a delay of 5 minutes for the RL-10s as currently there's not ignition sequence, etc...


class SSU_Centaur: public VESSEL2
{
	public:
		SSU_Centaur( OBJHANDLE hVessel );
		~SSU_Centaur( void );

		void clbkSetClassCaps( FILEHANDLE cfg );
		void clbkPreStep( double simt, double simdt, double mjd );
		int clbkConsumeBufferedKey( DWORD key, bool down, char* kstate );
		void clbkSaveState( FILEHANDLE scn );
		void clbkLoadStateEx( FILEHANDLE scn, void *status );
	private:
		MESHHANDLE hMesh;
		UINT mesh_idx;

		std::string AdapterMeshName;
		double AdapterOffset;// m
		double AdapterMass;// Kg
		MESHHANDLE hAdapterMesh;
		UINT mesh_Adapter_idx;

		ATTACHMENTHANDLE ahToPayload;
		ATTACHMENTHANDLE ahToCISS;
		PROPELLANT_HANDLE phTank;
		PROPELLANT_HANDLE phACS;
		THRUSTER_HANDLE thRL10[2];
		THRUSTER_HANDLE thACS[12];

		bool separated;
		double timer_ACS;
		bool ENAtimer_ACS;
		double timer_RL10;
		bool ENAtimer_RL10;
};

#endif// __SSUSC_H
