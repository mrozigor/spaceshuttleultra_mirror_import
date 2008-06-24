#include "ATVC.h"


namespace mps
{
	ATVC::ATVC( SubsystemDirector* _director ):AtlantisSubsystem( _director, "mpsATVC" )
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