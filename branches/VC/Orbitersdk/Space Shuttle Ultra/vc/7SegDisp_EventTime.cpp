#include "7SegDisp_EventTime.h"
#include "../Atlantis.h"


namespace vc
{
	_7SegDisp_EventTime::_7SegDisp_EventTime( Atlantis* _sts, const std::string& _ident ):_7SegDisp( _sts, _ident )
	{
		timer = dps::TIMER_FORWARD;
	}

	_7SegDisp_EventTime::~_7SegDisp_EventTime()
	{
	}

	void _7SegDisp_EventTime::SetLocation( bool fwd )
	{
		if (fwd) timer = dps::TIMER_FORWARD;
		else timer = dps::TIMER_AFT;
		return;
	}

	void _7SegDisp_EventTime::Controller( void )
	{
		short sTimerMinutes = STS()->pMTU->GetEventTimerMin( timer );
		short sTimerSeconds = STS()->pMTU->GetEventTimerSec( timer );

		next_state[3] = sTimerMinutes / 10;
		next_state[2] = sTimerMinutes % 10;
		next_state[1] = sTimerSeconds / 10;
		next_state[0] = sTimerSeconds % 10;
		return;
	}
};