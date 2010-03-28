#include "IMU.h"

namespace gnc {

	IMU::IMU(SubsystemDirector* _direct, const std::string& _name, unsigned short usTerminalID)
	: AtlantisSubsystem(_direct, _name),
	fc_connect()
	{
		fc_connect.Init(this, NULL, _name + "-FC", usTerminalID);
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

	bool IMU::Cage()
	{
		return false;
	}


	bool IMU::Uncage()
	{
		return false;
	}

	const VECTOR3& IMU::GetM50Position() const
	{
		static VECTOR3 x;
		x = _V(0.0, 0.0, 0.0);
		return x;
	}
	
	const VECTOR3& IMU::GetM50Velocity() const
	{
		static VECTOR3 x;
		x = _V(0.0, 0.0, 0.0);
		return x;
	}
		
	const VECTOR3& IMU::GetM50Acceleration() const
	{
		static VECTOR3 x;
		x = _V(0.0, 0.0, 0.0);
		return x;
	}
	
	const VECTOR3& IMU::GetM50Attitude() const
	{
		static VECTOR3 x;
		x = _V(0.0, 0.0, 0.0);
		return x;
	}

	void IMU::TestInput(unsigned short word1, unsigned short word2)
	{
		static char buffer[400];
		//verify behavior of IMU interface
		sprintf_s(buffer, "(Space Shuttle Ultra) IMU data received: %04X %04X",
			word1, word2);
		oapiWriteLog(buffer);
	}

	void IMU::busCommandPhase(dps::BusController* biu)
	{
	}

	void IMU::busReadPhase(dps::BusController* biu)
	{
	}

	dps::BUS_COMMAND_WORD IMU::busCommand(dps::BusTerminal* biu, dps::BUS_COMMAND_WORD cw, 
		unsigned long num_data, dps::word16 *cdw)
	{
		if(num_data>1)
		{
			TestInput(cdw[0], cdw[1]);
		}
		return cw;
	}

};