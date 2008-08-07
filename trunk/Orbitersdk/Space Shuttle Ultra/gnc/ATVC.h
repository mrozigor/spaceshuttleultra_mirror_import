#ifndef _gnc_ATVC_H_
#define _gnc_ATVC_H_


#include "..\AtlantisSubsystem.h"
#include "..\discsignals\DiscInPort.h"
#include "..\discsignals\DiscOutPort.h"


namespace gnc
{
	using class discsignals::DiscInPort;
	using class discsignals::DiscOutPort;

	class ATVC:public AtlantisSubsystem
	{
	public:
		//Ports are public
		DiscInPort in_1P;
		DiscInPort in_1Y;
		DiscInPort in_2P;
		DiscInPort in_2Y;
		DiscInPort in_3P;
		DiscInPort in_3Y;
		DiscInPort in_LSRBT;
		DiscInPort in_LSRBR;
		DiscInPort in_RSRBT;
		DiscInPort in_RSRBR;

		DiscOutPort out_1P;
		DiscOutPort out_1Y;
		DiscOutPort out_2P;
		DiscOutPort out_2Y;
		DiscOutPort out_3P;
		DiscOutPort out_3Y;
		DiscOutPort out_LSRBT;
		DiscOutPort out_LSRBR;
		DiscOutPort out_RSRBT;
		DiscOutPort out_RSRBR;
	public:
		ATVC( SubsystemDirector* director, const string& ident);
		~ATVC( void );

		// heart beat
		virtual void OnPostStep( double fSimT, double fDeltaT, double fMJD);
	};
}


#endif// _gnc_ATVC_H_

/*
EA P/S
------
1P 1/3
1Y 3/1

2P 2/1
2Y 1/2

3P 3/2
3Y 2/3


P +/- 10.5deg
Y +/- 8.5deg

10 <= RATE(deg/sec) <= 20

1P 16
1Y 0

2P 10
2Y 3.5

3P 10
3Y 3.5
*/