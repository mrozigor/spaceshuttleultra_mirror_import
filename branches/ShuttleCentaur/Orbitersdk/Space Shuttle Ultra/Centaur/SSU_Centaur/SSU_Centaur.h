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

const VECTOR3 RL10_L_POS = _V( -0.785, 0, -4.5 );
const VECTOR3 RL10_L_DIR = _V( 0, 0, 1 );
const VECTOR3 RL10_R_POS = _V( 0.785, 0, -4.5 );
const VECTOR3 RL10_R_DIR = _V( 0, 0, 1 );

const VECTOR3 ACS_1_POS = _V( -1.06112, 0.751872, -2.92064 );
const VECTOR3 ACS_1_DIR = _V( -0.840362, -0.0981861, -0.533059 );
const VECTOR3 ACS_2_POS = _V( -0.920389, 0.894691, -2.91137 );
const VECTOR3 ACS_2_DIR = _V( 0.144188, 0.882767, -0.447137 );

const VECTOR3 ACS_3_POS = _V( 0.993467, 0.931987, -2.9481 );
const VECTOR3 ACS_3_DIR = _V(-0.00410334, 0.840309, -0.542092);
const VECTOR3 ACS_4_POS = _V( 1.01708, 0.784585, -2.9354 );
const VECTOR3 ACS_4_DIR = _V(0.280308, 0.485194, 0.828259);
const VECTOR3 ACS_5_POS = _V(0.280308, 0.485194, 0.828259);
const VECTOR3 ACS_5_DIR = _V( 0.280308, 0.485194, 0.828259 );
const VECTOR3 ACS_6_POS = _V( 1.17345, 0.707999, -2.89724 );
const VECTOR3 ACS_6_DIR = _V(0.947194, -0.144266, -0.286375);

const VECTOR3 ACS_7_POS = _V( 1.05051, 0.921445, -2.9037 );
const VECTOR3 ACS_7_DIR = _V(0.946922, 0.144193, -0.287311);
const VECTOR3 ACS_8_POS = _V( 0.950936, -1.03279, -2.933 );
const VECTOR3 ACS_8_DIR = _V(-0.00410506, -0.840504, -0.54179);

const VECTOR3 ACS_9_POS = _V( -0.780553, -1.08102, -2.92859 );
const VECTOR3 ACS_9_DIR = _V(0.098237, -0.840524, -0.532794);
const VECTOR3 ACS_10_POS = _V( -0.808875, -0.955035, -2.92294 );
const VECTOR3 ACS_10_DIR = _V(-0.41986, -0.485129, 0.767051);
const VECTOR3 ACS_11_POS = _V(0.857318, -0.906415, -2.9187 );
const VECTOR3 ACS_11_DIR = _V(-0.41986, -0.485129, 0.767051);
const VECTOR3 ACS_12_POS = _V(-1.00633, -0.88639, -2.91207);
const VECTOR3 ACS_12_DIR = _V(-0.882732, 0.144208, -0.447201);

const double G_EMPTY_MASS = 2559.15;// Kg
const double G_RL10_PROPELLANT_MASS = 13178.25;// Kg
const double GPRIME_EMPTY_MASS = 2605;// Kg
const double GPRIME_RL10_PROPELLANT_MASS = 20833;// Kg
const double RL10_THRUST_VAC = 73309.5;// N
const double RL10_ISP_VAC = 446.6 * G;// m/s
const double ACS_PROPELLANT_MASS = 76.5;// Kg
const double ACS_THRUST_VAC = 26.64;// N
const double ACS_ISP_VAC = 324 * G;// m/s
const double GHe_MASS = 2;// Kg



class SSU_Centaur: public VESSEL2
{
	public:
		SSU_Centaur( OBJHANDLE hVessel );
		~SSU_Centaur( void );

		void clbkSetClassCaps( FILEHANDLE cfg );
		void clbkPreStep( double simt, double simdt, double mjd );
		int clbkConsumeBufferedKey( DWORD key, bool down, char* keystate );
	private:
		MESHHANDLE hMesh;
		UINT mesh_idx;
		ATTACHMENTHANDLE ahToPayload;
		ATTACHMENTHANDLE ahToCISS;
		PROPELLANT_HANDLE phTank;
		PROPELLANT_HANDLE phACS;
		THRUSTER_HANDLE thRL10[2];
		THRUSTER_HANDLE thACS[12];

		bool enginesENA;
};

#endif// __SSUSC_H