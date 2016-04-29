/****************************************************************************
  This file is part of Space Shuttle Ultra

  Centaur Integrated Support Structure definition



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
#ifndef __CISS_H
#define __CISS_H


#include "discsignals.h"
#include "Atlantis.h"


const static char* CISS_G_MESHNAME = "SSU\\Centaur\\CISS_CentaurG";
const static char* CISS_GPRIME_MESHNAME = "SSU\\Centaur\\CISS_CentaurG_Prime";
const static char* CISS_MISSION_KIT_MESHNAME = "SSU\\Centaur\\Centaur_Mission_Kit_plumbing";


const double CISS_G_EMPTY_MASS = 3832.856;// Kg
const double CISS_GPRIME_EMPTY_MASS = 4036.972;// Kg

const VECTOR3 CISS_G_OFFSET = _V( 0, -0.265, -4.8 );
const VECTOR3 CISS_GPRIME_OFFSET = _V( 0, -0.265, -4.8 );
const VECTOR3 CISS_MISSION_KIT_OFFSET = _V( 0, 0, 0 );

const VECTOR3 CENTAUR_G_ATTACHMENT_OFFSET = _V( 0, -0.275, -6.1 );
const VECTOR3 CENTAUR_GPRIME_ATTACHMENT_OFFSET = _V( 0, -0.275, -6.1 );

const VECTOR3 CISS_G_P1 = _V( 0, -0.0762, -2.1528 );// DA rotation axis
const VECTOR3 CISS_G_P2 = _V( 0, 1.2128, -2.2927 );// DA motor rotation axis
const VECTOR3 CISS_G_P3 = _V( 0, 1.0746, -2.7545 );// motor arm/link rotation axis
const VECTOR3 CISS_G_P4 = _V( 0, 0.7773, -3.3501 );// link rotation axis
const VECTOR3 CISS_G_P5 = _V( 0, 0.7569, -1.8076 );// LOX first bellow (from disconnect)
const VECTOR3 CISS_G_P6 = _V( 0, 0.7569, -2.2178 );// LOX second bellow (from disconnect)
const VECTOR3 CISS_G_P7 = _V( 0, -0.8704, -2.2048 );// LOX third bellow (from disconnect)
const VECTOR3 CISS_G_P8 = _V( 0, 0.5708, -1.8076 );// GOX first bellow (from disconnect)
const VECTOR3 CISS_G_P9 = _V( 0, 0.5710, -2.2993 );// GOX second bellow (from disconnect)
const VECTOR3 CISS_G_P10 = _V( 0, -0.5538, -2.1541 );// GOX third bellow (from disconnect)
const VECTOR3 CISS_G_P11 = _V( 0, 0.7569, -1.8076 );// LH2 first bellow (from disconnect)
const VECTOR3 CISS_G_P12 = _V( 0, 0.7569, -2.2178 );// LH2 second bellow (from disconnect)
const VECTOR3 CISS_G_P13 = _V( 0, -0.8704, -2.2048 );// LH2 third bellow (from disconnect)
const VECTOR3 CISS_G_P14 = _V( 0, 0.5708, -1.8076 );// GH2 first bellow (from disconnect)
const VECTOR3 CISS_G_P15 = _V( 0, 0.5710, -2.2993 );// GH2 second bellow (from disconnect)
const VECTOR3 CISS_G_P16 = _V( 0, -0.5538, -2.1541 );// GH2 third bellow (from disconnect)

const VECTOR3 CISS_GPRIME_P1 = _V( 0, -0.0762, -2.1528 );// DA rotation axis
const VECTOR3 CISS_GPRIME_P2 = _V( 0, 1.2128, -2.2927 );// motor crank rotation axis
const VECTOR3 CISS_GPRIME_P3 = _V( 0, 1.0746, -2.7545 );// motor crank/link rotation axis
const VECTOR3 CISS_GPRIME_P4 = _V( 0, 0.7773, -3.3501 );// link rotation axis
const VECTOR3 CISS_GPRIME_P5 = _V( 0, 0.7569, -1.8088 );// LOX first bellow (from disconnect)
const VECTOR3 CISS_GPRIME_P6 = _V( 0, 0.7568, -2.2178 );// LOX second bellow (from disconnect)
const VECTOR3 CISS_GPRIME_P7 = _V( 0, -0.8704, -2.2048 );// LOX third bellow (from disconnect)
const VECTOR3 CISS_GPRIME_P8 = _V( 0, 0.5720, -1.8076 );// GOX first bellow (from disconnect)
const VECTOR3 CISS_GPRIME_P9 = _V( 0, 0.5710, -2.2993 );// GOX second bellow (from disconnect)
const VECTOR3 CISS_GPRIME_P10 = _V( 0, -0.5538, -2.1541 );// GOX third bellow (from disconnect)
const VECTOR3 CISS_GPRIME_P11 = _V( 0, 0.7569, -1.8088 );// LH2 first bellow (from disconnect)
const VECTOR3 CISS_GPRIME_P12 = _V( 0, 0.7569, -2.2178 );// LH2 second bellow (from disconnect)
const VECTOR3 CISS_GPRIME_P13 = _V( 0, -0.8705, -2.2044 );// LH2 third bellow (from disconnect)
const VECTOR3 CISS_GPRIME_P14 = _V( 0, 0.5708, -1.8076 );// GH2 first bellow (from disconnect)
const VECTOR3 CISS_GPRIME_P15 = _V( 0, 0.5710, -2.2993 );// GH2 second bellow (from disconnect)
const VECTOR3 CISS_GPRIME_P16 = _V( 0, -0.5538, -2.1541 );// GH2 third bellow (from disconnect)

const double CISS_MOTOR_ROTATION_RATE = 0.00333333;// motor rotation rate
const double CISS_MOTOR_ROTATION_RANGE = 215 * RAD;// range of motor rotation


using namespace discsignals;


class CISS:public AtlantisSubsystem
{
		bool isGPrime;

		MESHHANDLE hMesh;
		UINT mesh_idx;
		MESHHANDLE hMesh_MK;
		UINT mesh_idx_MK;
		VECTOR3 CISSoffset;
		VECTOR3 Centaurattachoffset;

		UINT anim_rotate;
		UINT anim_motorcrank;
		UINT anim_link;
		UINT anim_lox_a;
		UINT anim_lox_a2;
		UINT anim_lox_b;
		UINT anim_lox_b2;
		UINT anim_gox_a;
		UINT anim_gox_a2;
		UINT anim_gox_b;
		UINT anim_gox_b2;
		UINT anim_lh2_a;
		UINT anim_lh2_a2;
		UINT anim_lh2_b;
		UINT anim_lh2_b2;
		UINT anim_gh2_a;
		UINT anim_gh2_a2;
		UINT anim_gh2_b;
		UINT anim_gh2_b2;
		AnimState rotateAnimState;
		AnimState motorcrankAnimState;
		AnimState linkAnimState;
		AnimState lox_aAnimState;
		AnimState lox_a2AnimState;
		AnimState lox_bAnimState;
		AnimState lox_b2AnimState;
		AnimState gox_aAnimState;
		AnimState gox_a2AnimState;
		AnimState gox_bAnimState;
		AnimState gox_b2AnimState;
		AnimState lh2_aAnimState;
		AnimState lh2_a2AnimState;
		AnimState lh2_bAnimState;
		AnimState lh2_b2AnimState;
		AnimState gh2_aAnimState;
		AnimState gh2_a2AnimState;
		AnimState gh2_bAnimState;
		AnimState gh2_b2AnimState;

		double x1;
		double y1;
		double r2;
		double r3;
		double r4;
		double x7;
		double y7;
		double r2_5;
		double r5;
		double r6;
		double r7;
		double x10;
		double y10;
		double r2_8;
		double r8;
		double r9;
		double r10;
		double x13;
		double y13;
		double r2_11;
		double r11;
		double r12;
		double r13;
		double x16;
		double y16;
		double r2_14;
		double r14;
		double r15;
		double r16;
		double theta2o;
		double theta3o;
		double theta4o;
		double theta2_5o;
		double theta5o;
		double theta6o;
		double theta2_8o;
		double theta8o;
		double theta9o;
		double theta2_11o;
		double theta11o;
		double theta12o;
		double theta2_14o;
		double theta14o;
		double theta15o;

		ATTACHMENTHANDLE ahToCentaur;
		VECTOR3 centaurAttachment[3];

		bool bFirstStep;

		bool bMECH_PRI_PWR;
		bool bMECH_BKUP_PWR;
		bool bDA_PRI_UP;
		bool bDA_PRI_DN;
		bool bDA_BKUP_UP;
		bool bDA_BKUP_DN;
		bool bSUPER_ZIP_FIRE;

		DiscInPort pSUPER_ZIP_PRI_FIRE;
		DiscInPort pSUPER_ZIP_PRI_ARM_ARM;
		DiscInPort pDA_PRI_ROT_DN;
		DiscInPort pDA_PRI_ROT_UP;
		DiscInPort pLOGIC_PRI_PWR_ON;
		DiscInPort pSSP_PRI_PWR_ON;
		DiscInPort pMECH_PRI_PWR_OFF;
		DiscInPort pMECH_PRI_PWR_ON;
		DiscOutPort pSUPER_ZIP_PRI_ARM_TB;
		DiscOutPort pPosition_PRI_TB;
		DiscOutPort pMECH_PRI_PWR_TB;

		DiscInPort pSUPER_ZIP_BKUP_FIRE;
		DiscInPort pSUPER_ZIP_BKUP_ARM_ARM;
		DiscInPort pDA_BKUP_ROT_DN;
		DiscInPort pDA_BKUP_ROT_UP;
		DiscInPort pLOGIC_BKUP_PWR_ON;
		DiscInPort pSSP_BKUP_PWR_ON;
		DiscInPort pMECH_BKUP_PWR_OFF;
		DiscInPort pMECH_BKUP_PWR_ON;
		DiscOutPort pSUPER_ZIP_BKUP_ARM_TB;
		DiscOutPort pPosition_BKUP_TB;
		DiscOutPort pMECH_BKUP_PWR_TB;
		
		void DefineCISSGPrimeAnimations();
		void DefineCISSGAnimations();
		void RunAnimation();
		void CalcAnimation( void );

	public:
		CISS( AtlantisSubsystemDirector* _director, bool isGPrime = true );
		~CISS( void );

		virtual void Realize();
		virtual void OnPreStep( double SimT, double DeltaT, double MJD );
		virtual void OnPostStep( double SimT, double DeltaT, double MJD );
		virtual bool OnParseLine( const char* line );
		virtual void OnSaveState( FILEHANDLE scn ) const;
		bool SingleParamParseLine() const {return true;};
		virtual double GetSubsystemEmptyMass() const;
		void CreateAttachment();
		void AddMesh();
};

#endif// __CISS_H
