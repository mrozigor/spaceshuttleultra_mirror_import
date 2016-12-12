/****************************************************************************
  This file is part of Space Shuttle Ultra

  Airborne Support Structure for the Inertial Upper Stage definition



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
#ifndef __ASE_IUS_H
#define __ASE_IUS_H


#include "discsignals.h"
#include "Atlantis.h"


const static char* ASE_IUS_MESHNAME = "SSU\\IUS\\IUS_ASE";

const VECTOR3 ASE_IUS_OFFSET_AFT_LOCATION = _V( 0, -0.35, -5.85 );
const VECTOR3 ASE_IUS_OFFSET_FORWARD_LOCATION = _V( 0, -0.35, -4.4 );

const VECTOR3 IUS_ATTACHMENT_OFFSET_AFT_LOCATION = _V( 0, -0.35, -6.95 );
const VECTOR3 IUS_ATTACHMENT_OFFSET_FORWARD_LOCATION = _V( 0, -0.35, -5.5 );

const VECTOR3 ASE_IUS_TILT_TABLE_ROTATION_AXIS_POS = _V( 0, 0.3985, -1.3304 );
const VECTOR3 ASE_IUS_UMBILICAL_ROTATION_AXIS_POS = _V( 0.5169, -1.3438, -0.6282 );

const double ASE_IUS_TILT_TABLE_SPEED = 0.001493;// s^-1 (= 0.1º/s)
const double ASE_IUS_MASS = 3684.213;// kg
const double IUS_JETTISON_VELOCITY = 0.12192;// m/s

// animation position for ASE angle
const double ASE_IUS_TILT_TABLE_POS_61 = 1.0;
const double ASE_IUS_TILT_TABLE_POS_595 = 0.977612;
const double ASE_IUS_TILT_TABLE_POS_58 = 0.955224;
const double ASE_IUS_TILT_TABLE_POS_57 = 0.940299;
const double ASE_IUS_TILT_TABLE_POS_30 = 0.537313;
const double ASE_IUS_TILT_TABLE_POS_295 = 0.529851;
const double ASE_IUS_TILT_TABLE_POS_28 = 0.507463;
const double ASE_IUS_TILT_TABLE_POS_265 = 0.485075;
const double ASE_IUS_TILT_TABLE_POS_0 = 0.089552;
const double ASE_IUS_TILT_TABLE_POS_06 = 0.0;
// 0.5º delta position window
const double ASE_IUS_TILT_TABLE_DP = 0.007463;


using namespace discsignals;


class ASE_IUS:public AtlantisSubsystem
{
		bool PyroBusPri;
		bool PyroBusAlt;
		bool UmbilicalsEnaPri;
		bool UmbilicalsEnaAlt;
		bool IUSDeploymentEnaPri;
		bool IUSDeploymentEnaAlt;
		bool TiltTableActuatorMotionPri1Lower;
		bool TiltTableActuatorMotionPri1Raise;
		bool TiltTableActuatorMotionAlt2Lower;
		bool TiltTableActuatorMotionAlt2Raise;

		UINT mesh_index;
		MESHHANDLE hMesh;
		VECTOR3 ASEoffset;
		VECTOR3 IUSattachoffset;

		UINT animTiltTable;
		UINT animUmbilical;
		AnimState asTiltTable;
		AnimState asUmbilical;

		double oldposition;

		ATTACHMENTHANDLE hIUSattach;
		VECTOR3 IUSattachpoints[3];

		bool firststep;
		bool umbilicalreleased;

		DiscInPort pTiltTableActuatorDriveEnablePri1Intermediate;
		DiscInPort pTiltTableActuatorDriveEnablePri1Maximum;
		DiscInPort pTiltTableActuatorDriveEnableAlt2Intermediate;
		DiscInPort pTiltTableActuatorDriveEnableAlt2Maximum;

		DiscInPort pTiltTableActuatorMotionPri1Lower;
		DiscInPort pTiltTableActuatorMotionPri1Raise;
		DiscInPort pTiltTableActuatorMotionAlt2Lower;
		DiscInPort pTiltTableActuatorMotionAlt2Raise;

		DiscInPort pPyroBusPriOff;
		DiscInPort pPyroBusPriOn;
		DiscInPort pPyroBusAltOff;
		DiscInPort pPyroBusAltOn;

		DiscInPort pUmbilicalsEnaPriOff;
		DiscInPort pUmbilicalsEnaPriEnable;
		DiscInPort pUmbilicalsEnaAltOff;
		DiscInPort pUmbilicalsEnaAltEnable;
		DiscInPort pUmbilicalsRelPriRelease;
		DiscInPort pUmbilicalsRelAltRelease;

		DiscInPort pIUSDeploymentEnaPriOff;
		DiscInPort pIUSDeploymentEnaPriEnable;
		DiscInPort pIUSDeploymentEnaAltOff;
		DiscInPort pIUSDeploymentEnaAltEnable;

		DiscInPort pIUSDeploymentDpyPriDeploy;
		DiscInPort pIUSDeploymentDpyAltDeploy;

		DiscOutPort pPyroBusPriTB;
		DiscOutPort pPyroBusAltTB;

		DiscOutPort pUmbilicalsEnaPriTB;
		DiscOutPort pUmbilicalsEnaAltTB;
		DiscOutPort pUmbilicalsRelPriTB;
		DiscOutPort pUmbilicalsRelAltTB;

		DiscOutPort pIUSDeploymentEnaPriTB;
		DiscOutPort pIUSDeploymentEnaAltTB;

		DiscOutPort pTiltTableActuatorMotionPri1TB;
		DiscOutPort pTiltTableActuatorMotionAlt2TB;

		DiscOutPort pTiltTableActuatorPositionPri1TB;
		DiscOutPort pTiltTableActuatorPositionAlt2TB;

		bool IsIUSAttached();
		void CalcUmbilicalAnimation( void );
		void RunAnimation();

	public:
		ASE_IUS( AtlantisSubsystemDirector* _director, bool AftLocation = false );
		virtual ~ASE_IUS();

		virtual void Realize();
		virtual void OnPreStep( double SimT, double DeltaT, double MJD );
		virtual void OnPostStep( double SimT, double DeltaT, double MJD );
		virtual bool OnParseLine( const char* line );
		virtual void OnSaveState( FILEHANDLE scn ) const;
		bool SingleParamParseLine() const {return true;};
		virtual double GetSubsystemEmptyMass() const;
		void AddMesh();
		void DefineAnimations();
		void CreateAttachment();

		void UpdateAttachment( void );
};

#endif //__IUS_ASE_H
