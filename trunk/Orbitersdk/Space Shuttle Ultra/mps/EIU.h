#ifndef _mps_EIU_H_
#define _mps_EIU_H_


#include "SSME.h"
#include <orbitersdk.h>
#include "..\AtlantisSubsystem.h"


namespace mps
{
	class EIU:public AtlantisSubsystem
	{
	private:
		int ID;
		SSME* engine;
		DATA_TABLE* BUFFER_SSME_DATA_TABLE;
	public:
		EIU( SubsystemDirector*, const string&, int, SSME* );
		~EIU( void );

		// GPCs only
		int RequestDATA( DATA_TABLE* );
		int RequestCMD( int, double );

		// heart beat
		void OnPostStep( double, double, double );
		//void OnPreStep( double, double, double );
		//void OnPropagate( double, double, double );

		// TODO power swicth -> O17
	};
}


#endif// _mps_EIU_H_

/*
***EIU
stores data table in buffer
at req. send data to GPC
at req. send cmds to SSME
shut up if power lost
*/
