#pragma once

#include "..\discsignals\DiscreteBundleManager.h"
#include "..\discsignals\DiscInPort.h"
#include "..\discsignals\DiscOutPort.h"

namespace hydraulics {

	typedef enum _HydAct
	{
		_1P,
		_1Y,
		_2P,
		_2Y,
		_3P,
		_3Y,
		_LHSRBT,
		_LHSRBR,
		_RHSRBT,
		_RHSRBR
	} HydAct;

	using namespace discsignals;

	class HydActuator {
		/**
		 * Command input port A
		 */
	public:
		HydAct ID;
		DiscreteBundleManager* bundle;

		DiscInPort in_servovalve_ATVC1;
		DiscInPort in_servovalve_ATVC2;
		DiscInPort in_servovalve_ATVC3;
		DiscInPort in_servovalve_ATVC4;

		DiscInPort in_isolationvalve_ATVC1;
		DiscInPort in_isolationvalve_ATVC2;
		DiscInPort in_isolationvalve_ATVC3;
		DiscInPort in_isolationvalve_ATVC4;

		DiscOutPort out_deltapressure_ATVC1;
		DiscOutPort out_deltapressure_ATVC2;
		DiscOutPort out_deltapressure_ATVC3;
		DiscOutPort out_deltapressure_ATVC4;

		DiscOutPort out_positiontransducer_ATVC1;
		DiscOutPort out_positiontransducer_ATVC2;
		DiscOutPort out_positiontransducer_ATVC3;
		DiscOutPort out_positiontransducer_ATVC4;

		HydActuator( HydAct nID, DiscreteBundleManager* BundleManager );
		virtual ~HydActuator();

		void Realize( void );
		void OnPostStep( double fSimT, double fDeltaT, double fMJD );
	};
};