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
#include "PIDControl.h"


const static char* STAGE_1_MESHNAME = "SSU\\IUS\\IUS_SRM1";
const static char* STAGE_2_MESHNAME = "SSU\\IUS\\IUS_SRM2";

const VECTOR3 SRM1_POS = _V( 0, 0, -4.1 );
const VECTOR3 SRM1_DIR = _V( 0, 0, 1 );
const VECTOR3 SRM2_POS = _V( 0, 0, -1.7 );
const VECTOR3 SRM2_DIR = _V( 0, 0, 1 );

// REM 1
const VECTOR3 RCS1_POS = _V( -0.2355, 1.3854, 0.8408 );
const VECTOR3 RCS1_DIR = _V( 0.0, 0.0, 1.0 );
const VECTOR3 RCS2_POS = _V( -0.1563, 1.3966, 0.8408 );
const VECTOR3 RCS2_DIR = _V( 0.0, 0.0, 1.0 );
// REM 6
const VECTOR3 RCS3_POS = _V( 1.4055, 0.2083, 0.8598 );
const VECTOR3 RCS3_DIR = _V( 0.0, -1.0, 0.0 );
const VECTOR3 RCS4_POS = _V( 1.4051, 0.0609, 0.8408 );
const VECTOR3 RCS4_DIR = _V( 0.0, 0.0, 1.0 );
//REM 5
const VECTOR3 RCS5_POS = _V( 1.4051, -0.0609, 0.8408 );
const VECTOR3 RCS5_DIR = _V( 0.0, 0.0, 1.0 );
const VECTOR3 RCS6_POS = _V( 1.4055, -0.2083, 0.8598 );
const VECTOR3 RCS6_DIR = _V( 0.0, 1.0, 0.0 );
// REM 4
const VECTOR3 RCS7_POS = _V( 0.2355, -1.3854, 0.8408 );
const VECTOR3 RCS7_DIR = _V( 0.0, 0.0, 1.0 );
const VECTOR3 RCS8_POS = _V( 0.1563, -1.3966, 0.8408 );
const VECTOR3 RCS8_DIR = _V( 0.0, 0.0, 1.0 );
// REM 3
const VECTOR3 RCS9_POS = _V( -1.4055, -0.2083, 0.8598 );
const VECTOR3 RCS9_DIR = _V( 0.0, 1.0, 0.0 );
const VECTOR3 RCS10_POS = _V( -1.4051, -0.0609, 0.8408 );
const VECTOR3 RCS10_DIR = _V( 0.0, 0.0, 1.0 );
// REM 2
const VECTOR3 RCS11_POS = _V( -1.4051, 0.0609, 0.8408 );
const VECTOR3 RCS11_DIR = _V( 0.0, 0.0, 1.0 );
const VECTOR3 RCS12_POS = _V( -1.4055, 0.2083, 0.8598 );
const VECTOR3 RCS12_DIR = _V( 0.0, -1.0, 0.0 );


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

const double RCS_TANK_PROPELLANT_MASS = 55.56507;// kg
const double RCS_THRUST_VAC = 157.6;// N (or 133N???)
const double RCS_ISP_VAC = 320 * G;// m/s

const double RCS_ENA_DELAY = 600;// s
const double SRM_ENA_DELAY = 3600;// s

const double STAGE2_EEC_DEPLOY_RATE = 0.1;// s^-1
const double STAGE2_EEC_DEPLOY_DELAY = 7;// s

const double SRM_MAN_GIMBAL_RATE = 1;// deg/s
const double SRM_AUTO_GIMBAL_RATE = 1;// deg/s
const double SRM1_GIMBAL_RANGE = 4;// deg
const double SRM2_GIMBAL_RANGE = 7;// deg


class SSU_IUS:public VESSEL3
{
	private:
		VISHANDLE MainExternalMeshVisual;
		UINT mesh_stage1;
		UINT mesh_stage2;

		ATTACHMENTHANDLE ahPayload;
		ATTACHMENTHANDLE ahASE;

		PROPELLANT_HANDLE ph_main[2];
		PROPELLANT_HANDLE ph_rcs;

		int RCStanks;
		bool FourAntennas;
		double propload[2];

		THRUSTER_HANDLE th_main[2];
		THRUSTER_HANDLE th_rcs[12];
		
		PIDControl ctrlSRM_P;
		PIDControl ctrlSRM_Y;
		double SRM_P;
		double SRM_Y;
		double manP;
		double manY;

		AnimState asEXTENDNOZZLE;
		UINT anim_EXTENDNOZZLE;

		bool separated;
		bool STAGE1separated;
		double timer_RCS_ENA;
		bool RCS_ENA;
		double timer_SRM_ENA;
		bool SRM_ENA;
		bool rollRCSonly;

		std::string AdapterMeshName;
		double AdapterOffset;// m
		double AdapterMass;// Kg

	public:
		SSU_IUS( OBJHANDLE hObj );

		void clbkSetClassCaps(FILEHANDLE cfg);
		void clbkPostStep(double simtt, double simdt, double mjd);
		void DefineAnimations(void);
		bool clbkDrawHUD( int mode, const HUDPAINTSPEC *hps, oapi::Sketchpad *skp );
		int clbkConsumeBufferedKey(DWORD key, bool down, char *kstate);
		void clbkAnimate( double simt );
		void clbkVisualCreated(VISHANDLE vis, int refcount);
		void clbkVisualDestroyed(VISHANDLE vis, int refcount);
		void clbkLoadStateEx(FILEHANDLE scn, void *status);
		void clbkSaveState(FILEHANDLE scn);

		void SetStage2Config( void );

		void EnableRCS( void );
		void EnablePitchYawRCS( void );
		void InhibitPitchYawRCS( void );
};

#endif// __SSUIUS_H