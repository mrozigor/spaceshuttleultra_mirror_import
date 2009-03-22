#include "IMU.h"

namespace gnc {

	IMU::IMU(SubsystemDirector* _direct, const std::string& _name)
	: AtlantisSubsystem(_direct, _name),
	fc_connect(5)
	{
	
	}

	IMU::~IMU()
	{
	}

	void IMU::OnPreStep(double fSimT, double fDeltaT, double fMJD)
	{
	}

	void IMU::OnPropagate(double fSimT, double fDeltaT, double fMJD)
	{
	}

	void IMU::OnSaveState(FILEHANDLE scn) const
	{
	}

	bool IMU::OnParseLine(const char* line)
	{
		return false;
	}

};