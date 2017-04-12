#include "SSMEControllerSW.h"
#include "DigitalComputerUnit.h"
#include "MPSdefs.h"


namespace mps
{
	SSMEControllerSW::SSMEControllerSW( DigitalComputerUnit* DCU )
	{
#ifdef _MPSDEBUG
		char buffer[100];
		sprintf_s( buffer, 100, " SSMEControllerSW::SSMEControllerSW in" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG

		this->DCU = DCU;

#ifdef _MPSDEBUG
		sprintf_s( buffer, 100, " SSMEControllerSW::SSMEControllerSW out" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG
		return;
	}

	SSMEControllerSW::~SSMEControllerSW( void )
	{
		return;
	}
}