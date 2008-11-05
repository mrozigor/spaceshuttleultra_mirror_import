#ifndef _mps_EIU_H_
#define _mps_EIU_H_


#include "SSME.h"
#include <orbitersdk.h>
#include "..\AtlantisSubsystem.h"
#include "..\dps\BIU.h"


namespace mps
{
	class EIU:public AtlantisSubsystem
	{
	private:
		int ID;
		SSME* engine;
		VDT_32* PrimaryData;
		VDT_6* SecondaryData;
	public:
		EIU( SubsystemDirector*, const string&, int, SSME* );
		~EIU( void );

		// GPCs only

		/**
		 * Return primary data
		 * @param ptrPrimaryData pointer to primary data structure VDT_32
		 */
		int RequestPrimaryData( VDT_32* ptrPrimaryData );

		/**
		 * Return secondary data
		 * @param ptrSecondaryData pointer to secondary data structure VDT_6
		 */
		int RequestSecondaryData( VDT_6* ptrSecondaryData );
		int RequestCMD( int, double );

		// heart beat
		void OnPostStep( double, double, double );
		//void OnPreStep( double, double, double );
		//void OnPropagate( double, double, double );

		// TODO power swicth -> O17

		dps::BIU mia[4];
	};
}


#endif// _mps_EIU_H_
