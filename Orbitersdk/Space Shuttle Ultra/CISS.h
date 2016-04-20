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

const VECTOR3 CENTAUR_G_ATTACHMENT_OFFSET = _V( 0, -0.25, -6.1 );
const VECTOR3 CENTAUR_GPRIME_ATTACHMENT_OFFSET = _V( 0, -0.29, -6.1 );


const VECTOR3 CISS_G_DA_ROTATION_AXIS = _V( 0, -0.0776, -2.1529 );
const VECTOR3 CISS_GPRIME_DA_ROTATION_AXIS = _V( 0, -0.0763, -2.1527 );


const double CISS_ROTATION_RATE = 0.00333333; // rate at which table rotates to deploy position


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
		AnimState rotateAnimState;

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
		
		void AddMesh();
		void DefineCISSGPrimeAnimations();
		void DefineCISSGAnimations();
		void RunAnimation();

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
};

#endif// __CISS_H
