#ifndef _mps_BLOCK_II_H_
#define _mps_BLOCK_II_H_


#include <orbitersdk.h>
#include "..\AtlantisSubsystem.h"
#include "SSME.h"


namespace mps
{
	//const double BLOCK_II_RPL_THRUST = 2090664.159; //100% thrust
	//const double BLOCK_II_NPL_THRUST = 2170732.15; //Nominal "104%" thrust
	const double BLOCK_II_FPL_THRUST = 2275094.273; //109% thrust

	const int BLOCK_II_FPL = 109;// % RPL
	const int BLOCK_II_MPL = 67;// % RPL

	const double BLOCK_II_RPL_PC_PRESS = 2746.789;// extrapolated from: 109% = 2994psia

	const double BLOCK_II_ISP0 = 453 * 9.80665;
	const double BLOCK_II_ISP1 = 363 * 9.80665;


	class BLOCK_II:public SSME
	{
	private:
		void ValveScheduleThrottle( double );

		void ValveShutdownTableUpdate( double dPC );

		void VDTUpdate( double );

		// data cookup fncts
		double dcPC_ESC( double );
		double dcPC_CO( double );
		double dcPC_MS( double );

	public:
		BLOCK_II( SubsystemDirector*, const string&, int );
		~BLOCK_II( void );

		// heart beat
		//void OnPostStep( double, double, double );
		//void OnPreStep( double, double, double );
		//void OnPropagate( double, double, double );

		void OnSaveState( FILEHANDLE ) const;
		bool OnParseLine( const char* );
		};
}


#endif// _mps_BLOCK_II_H_
