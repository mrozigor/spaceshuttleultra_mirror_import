#pragma once

#include "..\AtlantisSubsystem.h"
#include "..\dps\BIU.h"

namespace gnc {

	class IMU: public AtlantisSubsystem {
		double fOuterGimbalAngle;
		double fMiddleGimbalAngle;
		double fInnerGimbalAngle;
		double fInnerRollGimbalAngle;

		VECTOR3 navbase;
		VECTOR3 old_vel;
		VECTOR3 stable_member_gpos_old;
		VECTOR3 sm_xref_gpos_old;
		VECTOR3 sm_yref_gpos_old;
		VECTOR3 sm_zref_gpos_old;
	public:
		IMU(SubsystemDirector* _director, const std::string& name, unsigned short usTerminalID=5);
		virtual ~IMU();
		virtual void OnPreStep(double fSimT, double fDeltaT, double fMJD);
		virtual void OnPropagate(double fSimT, double fDeltaT, double fMJD);
		virtual void OnSaveState(FILEHANDLE scn) const;
		virtual bool OnParseLine(const char* line);

		virtual bool Cage();
		virtual bool Uncage();


		virtual const VECTOR3& GetM50Position() const;
		virtual const VECTOR3& GetM50Velocity() const;
		virtual const VECTOR3& GetM50Acceleration() const;
		virtual const VECTOR3& GetM50Attitude() const;

		dps::BIU fc_connect;

	};
};