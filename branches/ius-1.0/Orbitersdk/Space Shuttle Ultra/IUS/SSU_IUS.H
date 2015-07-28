/****************************************************************************
  This file is part of Space Shuttle Ultra

  Inertial Upper Stage definition



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
#ifndef __SSUIUS_H
#define __SSUIUS_H


#include "orbitersdk.h"


const static char* STAGE_1_MESHNAME = "SSU\\IUS\\IUS_SRM1";
const static char* STAGE_2_MESHNAME = "SSU\\IUS\\IUS_SRM2";

const int STAGE2_EEC_STRUTS_GRP = 5;
const int STAGE2_EEC_STRUTS_BRACKETS_GRP = 6;
const int STAGE2_ECC_GRP = 10;

const VECTOR3 SRM1_POS = _V( 0, 0, -4.1 );
const VECTOR3 SRM1_DIR = _V( 0, 0, 1 );
const VECTOR3 SRM2_POS = _V( 0, 0, -1.7 );
const VECTOR3 SRM2_DIR = _V( 0, 0, 1 );

// REM 1
const VECTOR3 RCS1_POS = _V( -0.31, 1.53, 0.98 );
const VECTOR3 RCS1_DIR = _V( 0.0422781, -0.255162, 0.965973 );
const VECTOR3 RCS2_POS = _V( -0.19, 1.56, 0.98 );
const VECTOR3 RCS2_DIR = _V( 0.0422746, -0.255462, 0.965894 );
// REM 6
const VECTOR3 RCS3_POS = _V( 1.56, 0.14, 0.99 );
const VECTOR3 RCS3_DIR = _V( -0.258819, -0.965926, 0.0 );
const VECTOR3 RCS4_POS = _V( 1.56, 0.05, 0.98 );
const VECTOR3 RCS4_DIR = _V( -0.258819, 0.0, 0.965926 );
//REM 5
const VECTOR3 RCS5_POS = _V( 1.56, -0.06, 0.98 );
const VECTOR3 RCS5_DIR = _V( -0.258819, 0.0, 0.965926 );
const VECTOR3 RCS6_POS = _V( 1.57, -0.14, 0.99 );
const VECTOR3 RCS6_DIR = _V( -0.258819, 0.965926, 0.0 );
// REM 4
const VECTOR3 RCS7_POS = _V( 0.61, -1.44, 0.98 );
const VECTOR3 RCS7_DIR = _V( -0.09107, 0.242185, 0.965947 );
const VECTOR3 RCS8_POS = _V( 0.50, -1.48, 0.98 );
const VECTOR3 RCS8_DIR = _V( -0.0910628, 0.24249, 0.965871 );
// REM 3
const VECTOR3 RCS9_POS = _V( -1.57, -0.13, 0.99 );
const VECTOR3 RCS9_DIR = _V( 0.258819, 0.965926, 0.0 );
const VECTOR3 RCS10_POS = _V( -1.57, -0.05, 0.98 );
const VECTOR3 RCS10_DIR = _V( 0.258819, 0.0, 0.965926 );
// REM 2
const VECTOR3 RCS11_POS = _V( -1.57, 0.06, 0.98 );
const VECTOR3 RCS11_DIR = _V( 0.258819, 0.0, 0.965926 );
const VECTOR3 RCS12_POS = _V( -1.57, 0.14, 0.99 );
const VECTOR3 RCS12_DIR = _V( 0.258819, -0.965926, 0.0 );


const VECTOR3 STAGE1_OFFSET = _V( 0, 0, -1.95 );
const VECTOR3 ASE_ATTACH_OFFSET = _V( 0, 0, -2.6 );
const VECTOR3 PAYLOAD_ATTACH_OFFSET = _V( 0, 0, 0.95 );

const double STAGE1_EMPTYMASS = 1134;// kg
const double STAGE2_EMPTYMASS = 1170;// kg

const double STAGE1_PROPELLANT_MAXMASS = 9631.8;// kg
const double STAGE2_PROPELLANT_MAXMASS = 2727.45;// kg

const double STAGE1_THRUST_VAC = 184752.84;// N
const double STAGE2_THRUST_VAC = 78272;// N
const double STAGE2_EEC_THRUST_VAC = 78272;// N

const double STAGE1_ISP_VAC = 295.5 * G;// m/s
const double STAGE2_ISP_VAC = 289.1 * G;// m/s
const double STAGE2_EEC_ISP_VAC = 303.5 * G;// m/s

const double RCS_PROPELLANT_MASS = 109;// kg
const double RCS_THRUST_VAC = 157.6;// N (or 133N???)
const double RCS_ISP_VAC = 320 * G;// m/s

const double RCS_ENA_DELAY = 600;// s
const double SRM_ENA_DELAY = 600;//3600;// s

const double STAGE2_EEC_DEPLOY_RATE = 0.1;// s^-1
const double STAGE2_EEC_DEPLOY_DELAY = 7;// s


class SSU_IUS:public VESSEL2
{
	private:
		VISHANDLE MainExternalMeshVisual;
		UINT mesh_stage1;
		UINT mesh_stage2;

		ATTACHMENTHANDLE ahPayload;
		ATTACHMENTHANDLE ahASE;

		PROPELLANT_HANDLE ph_main[2];
		PROPELLANT_HANDLE ph_rcs;

		THRUSTER_HANDLE th_main[2];
		THRUSTER_HANDLE th_rcs[12];
		THGROUP_HANDLE thg_main;

		AnimState asEXTENDNOZZLE;
		UINT anim_EXTENDNOZZLE;

		bool separated;
		bool STAGE1separated;
		double timer_RCS;
		bool ENAtimer_RCS;
		double timer_SRM;
		bool ENAtimer_SRM;
		bool SRMignited;

		std::string AdapterMeshName;
		double AdapterOffset;// m
		double AdapterMass;// Kg

	public:
		SSU_IUS( OBJHANDLE hObj );

		void clbkSetClassCaps(FILEHANDLE cfg);
		void clbkPostStep(double simtt, double simdt, double mjd);
		void DefineAnimations(void);
		int clbkConsumeBufferedKey(DWORD key, bool down, char *kstate);
		void clbkAnimate( double simt );
		void clbkVisualCreated(VISHANDLE vis, int refcount);
		void clbkVisualDestroyed(VISHANDLE vis, int refcount);
		void clbkLoadStateEx(FILEHANDLE scn, void *status);
		void clbkSaveState(FILEHANDLE scn);

		void SetStage2Config( void );
};

#endif// __SSUIUS_H