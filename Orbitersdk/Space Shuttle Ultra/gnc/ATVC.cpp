#include "ATVC.h"


namespace gnc
{
	ATVC::ATVC( SubsystemDirector* _director, const string& _ident ):AtlantisSubsystem( _director, _ident )
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