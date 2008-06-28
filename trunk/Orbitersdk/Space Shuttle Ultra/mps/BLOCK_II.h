#ifndef _mps_BLOCK_II_H_
#define _mps_BLOCK_II_H_


#include <orbitersdk.h>
#include "../AtlantisSubsystem.h"
#include "SSME.h"


namespace mps
{
	const double BLOCK_II_RPL_THRUST = 2090664.159; //100% thrust
	//const double BLOCK_II_NPL_THRUST = 2170732.15; //Nominal "104%" thrust
	const double BLOCK_II_FPL_THRUST = 2275094.273; //109% thrust

	const double BLOCK_II_ISP0 = 453*9.80665;
	const double BLOCK_II_ISP1 = 363*9.80665;


	class BLOCK_II:public SSME
	{
	private:
		// data cookup fncts
		double dcPC_ESC( double );
		double dcMOV_ESC( double );
		double dcMFV_ESC( double );
		double dcCCV_ESC( double );
		double dcFPOV_ESC( double );
		double dcOPOV_ESC( double );
		
		double dcPC_CO( double );
		double dcMOV_CO( double );
		double dcMFV_CO( double );
		double dcCCV_CO( double );
		double dcFPOV_CO( double, double );
		double dcOPOV_CO( double, double );

		double dcPC_MS( double );
		double dcCCV_MS( double );
		double dcFPOV_MS( double );
		double dcOPOV_MS( double );

	public:
		// EIU only
		// command
		bool cmdThrottle( double );

		BLOCK_II( SubsystemDirector*, const string&, int, PROPELLANT_HANDLE, VECTOR3, VECTOR3 );
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
