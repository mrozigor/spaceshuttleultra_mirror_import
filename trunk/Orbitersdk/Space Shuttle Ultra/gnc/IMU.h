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
	public:
		IMU(SubsystemDirector* _director, const std::string& name);
		virtual ~IMU();
		virtual void OnPreStep(double fSimT, double fDeltaT, double fMJD);
		virtual void OnPropagate(double fSimT, double fDeltaT, double fMJD);
		virtual void OnSaveState(FILEHANDLE scn) const;
		virtual bool OnParseLine(const char* line);

		dps::BIU fc_connect;

	};
};