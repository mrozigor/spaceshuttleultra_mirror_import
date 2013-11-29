#include "InputElectronics_BLOCK_II.h"
#include "SSME.h"
#include "SSMEController.h"
#include "OutputElectronics.h"
#include "MPSdefs.h"


namespace mps
{
	InputElectronics_BLOCK_II::InputElectronics_BLOCK_II( int ch, SSME* eng, SSMEController* Controller ):InputElectronics( ch, eng, Controller, S_PRESS_num, S_TEMP_num, S_SPEED_num, S_FLOW_num, FLIGHTDATASENSORS_P, FLIGHTDATASENSORS_T, FLIGHTDATASENSORS_S, FLIGHTDATASENSORS_F )
	{
#ifdef _MPSDEBUG
		char buffer[100];
		sprintf_s( buffer, 100, " InputElectronics_BLOCK_II::InputElectronics_BLOCK_II in || ch:%d", ch );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG

#ifdef _MPSDEBUG
		sprintf_s( buffer, 100, " InputElectronics_BLOCK_II::InputElectronics_BLOCK_II out" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG
		return;
	}

	InputElectronics_BLOCK_II::~InputElectronics_BLOCK_II( void )
	{
		return;
	}

	void InputElectronics_BLOCK_II::__OnSaveState( FILEHANDLE scn ) const
	{
		return;
	}

	bool InputElectronics_BLOCK_II::__OnParseLine( const char* line )
	{
		return false;
	}

	void InputElectronics_BLOCK_II::tmestp( double time )
	{
		// check power supply
		if (Controller->PSE_Power( ch ) == false) return;


		int count = 0;
		// copy flight data sensors
		while (count < FD_P)
		{
			press[count] = eng->SENSOR_P[count];
			count++;
		}
		// copy own ch sensors
		CopySensorData( eng->numP, count + ch, count, eng->SENSOR_P, press );

		count = 0;
		while (count < FD_T)
		{
			temp[count] = eng->SENSOR_T[count];
			count++;
		}
		CopySensorData( eng->numT, count + ch, count, eng->SENSOR_T, temp );

		count = 0;
		while (count < FD_S)
		{
			speed[count] = eng->SENSOR_S[count];
			count++;
		}
		CopySensorData( eng->numS, count + ch, count, eng->SENSOR_S, speed );

		count = 0;
		while (count < FD_F)
		{
			flow[count] = eng->SENSOR_F[count];
			count++;
		}
		CopySensorData( eng->numF, count + ch, count, eng->SENSOR_F, flow );


		Controller->OE_GetSH( ch, samplehold );

		Controller->OE_GetPOS( ch, actpos );
		return;
	}
}