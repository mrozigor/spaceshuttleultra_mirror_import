#ifndef _mps_EIU_H_
#define _mps_EIU_H_

#include "SSME.h"
#include <orbitersdk.h>
#include "..\AtlantisSubsystem.h"
//#include "MPS.h"

class SSME;
namespace mps
{
	class EIU:public AtlantisSubsystem
	{
	private:
		SSME* engine;
		DATA_TABLE* BUFFER_SSME_DATA_TABLE;
	public:
		EIU( SubsystemDirector*, SSME* );
		~EIU( void );

		// GPCs only
		int RequestDATA( int, DATA_TABLE* );
		int RequestCMD( int, int, void* );

		// heart beat
		void OnPostStep( double, double, double );


	};
};

#endif// _mps_EIU_H_

/*
***EIU
stores data table in buffer
at req. send data to GPC
at req. send cmds to SSME
shut up if power lost
*/

/*
EIU:: converts ME-1 to 0, ME-2 to 1, ME-3 to 2 for use in SSME::
*/