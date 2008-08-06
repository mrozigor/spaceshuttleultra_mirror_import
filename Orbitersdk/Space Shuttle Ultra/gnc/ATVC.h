#ifndef _gnc_ATVC_H_
#define _gnc_ATVC_H_


#include "..\AtlantisSubsystem.h"


namespace gnc
{
	class ATVC:public AtlantisSubsystem
	{
	private:
		DiscInPort _1P;
		DiscInPort _1Y;
		DiscInPort _2P;
		DiscInPort _2Y;
		DiscInPort _3P;
		DiscInPort _3Y;
		DiscInPort LSRBT;
		DiscInPort LSRBR;
		DiscInPort RSRBT;
		DiscInPort RSRBR;

		DiscOutPort _1P;
		DiscOutPort _1Y;
		DiscOutPort _2P;
		DiscOutPort _2Y;
		DiscOutPort _3P;
		DiscOutPort _3Y;
		DiscOutPort LSRBT;
		DiscOutPort LSRBR;
		DiscOutPort RSRBT;
		DiscOutPort RSRBR;
	public:
		ATVC( SubsystemDirector* );
		~ATVC( void );

		// heart beat
		virtual void OnPostStep( double, double, double );
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