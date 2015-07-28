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

const unsigned int GRP_Battery_2_ASE = 6;
const unsigned int GRP_Power_Control_Unit_ASE = 7;
const unsigned int GRP_IUS_frame_tube_ASE = 8;
const unsigned int GRP_ASE_Converter_Regulator_Unit_ASE = 9;
const unsigned int GRP_Battery_1_ASE = 10;
const unsigned int GRP_Spacecraft_Converter_Regulator_Unit_ASE = 11;
const unsigned int GRP_Battery_3_ASE = 12;
const unsigned int GRP_IUS_Converter_Regulator_Unit_ASE = 13;
const unsigned int GRP_ASE_Actuator_Controller_A_ASE = 14;
const unsigned int GRP_Aft_ASE_tilt_frame_ASE = 15;
const unsigned int GRP_ASE_Actuator_Controller_B_ASE = 16;

const VECTOR3 ASE_IUS_OFFSET_AFT_LOCATION = _V( 0, -0.35, -5.85 );
const VECTOR3 ASE_IUS_OFFSET_FORWARD_LOCATION = _V( 0, -0.35, -4.4 );

const VECTOR3 IUS_ATTACHMENT_OFFSET_AFT_LOCATION = _V( 0, -0.35, -6.95 );
const VECTOR3 IUS_ATTACHMENT_OFFSET_FORWARD_LOCATION = _V( 0, -0.35, -5.5 );

const double ASE_IUS_TILT_TABLE_INTERMEDIATE_SPEED = 0.005;// s^-1
const double ASE_IUS_TILT_TABLE_MAXIMUM_SPEED = 0.01;// s^-1
const double ASE_IUS_MASS = 2531.7;// kg
const double IUS_JETTISON_VELOCITY = 0.12192;// m/s


using namespace discsignals;


class ASE_IUS:public AtlantisSubsystem
{
		bool PyroBusPri;
		bool PyroBusAlt;
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
		AnimState asTiltTable;

		ATTACHMENTHANDLE hIUSattach;
		VECTOR3 IUSattachpoints[3];

		bool firststep;

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

		DiscInPort pIUSDeploymentEnaPriOff;
		DiscInPort pIUSDeploymentEnaPriEnable;
		DiscInPort pIUSDeploymentEnaAltOff;
		DiscInPort pIUSDeploymentEnaAltEnable;

		DiscInPort pIUSDeploymentDpyPriDeploy;
		DiscInPort pIUSDeploymentDpyAltDeploy;

		DiscOutPort pPyroBusPriTB;
		DiscOutPort pPyroBusAltTB;

		DiscOutPort pIUSDeploymentEnaPriTB;
		DiscOutPort pIUSDeploymentEnaAltTB;

		DiscOutPort pTiltTableActuatorMotionPri1TB;
		DiscOutPort pTiltTableActuatorMotionAlt2TB;

		bool IsIUSAttached();

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
};

#endif //__IUS_ASE_H
