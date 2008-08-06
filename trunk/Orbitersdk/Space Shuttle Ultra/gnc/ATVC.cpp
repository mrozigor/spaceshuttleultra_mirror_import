#include "ATVC.h"


namespace gnc
{
	ATVC::ATVC( SubsystemDirector* _director ):AtlantisSubsystem( _director, "gncATVC" )
	{
		return;
	}

	ATVC::~ATVC( void )
	{
		// end
	}


	void ATVC::OnPostStep( double fSimT, double fDeltaT, double fMJD )
	{
		return;
	}
}