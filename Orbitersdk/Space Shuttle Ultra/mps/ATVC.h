#ifndef _mps_ATVC_H_
#define _mps_ATVC_H_

#include "..\AtlantisSubsystem.h"
//#include "MPS.h"

namespace mps
{
	class ATVC:public AtlantisSubsystem
	{
	private:
	public:
		ATVC( SubsystemDirector* );
		~ATVC( void );

		// heart beat
		virtual void OnPostStep( double, double, double );
	};
};

#endif// _mps_ATVC_H_
