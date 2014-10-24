#include "HydActuator.h"


namespace hydraulics
{
	HydActuator::HydActuator( HydAct nID, DiscreteBundleManager* BundleManager )
	{
		bundle = BundleManager;
		ID = nID;
		return;
	}

	HydActuator::~HydActuator( void )
	{
		// end
		return;
	}


	void HydActuator::Realize( void )
	{
		//// connect cabling ////
		discsignals::DiscreteBundle* ATVC1_to_HydAct_srvlv;
		discsignals::DiscreteBundle* ATVC2_to_HydAct_srvlv;
		discsignals::DiscreteBundle* ATVC3_to_HydAct_srvlv;
		discsignals::DiscreteBundle* ATVC4_to_HydAct_srvlv;
		discsignals::DiscreteBundle* ATVC1_to_HydAct_isolvlv;
		discsignals::DiscreteBundle* ATVC2_to_HydAct_isolvlv;
		discsignals::DiscreteBundle* ATVC3_to_HydAct_isolvlv;
		discsignals::DiscreteBundle* ATVC4_to_HydAct_isolvlv;
		discsignals::DiscreteBundle* HydAct_to_ATVC1_dp;
		discsignals::DiscreteBundle* HydAct_to_ATVC2_dp;
		discsignals::DiscreteBundle* HydAct_to_ATVC3_dp;
		discsignals::DiscreteBundle* HydAct_to_ATVC4_dp;
		discsignals::DiscreteBundle* HydAct_to_ATVC1_pos;
		discsignals::DiscreteBundle* HydAct_to_ATVC2_pos;
		discsignals::DiscreteBundle* HydAct_to_ATVC3_pos;
		discsignals::DiscreteBundle* HydAct_to_ATVC4_pos;

		int act;

		ATVC1_to_HydAct_srvlv = bundle->CreateBundle( "ATVC1_to_HydAct_srvlv", 10 );
		ATVC1_to_HydAct_isolvlv = bundle->CreateBundle( "ATVC1_to_HydAct_isolvlv", 10 );
		HydAct_to_ATVC1_dp = bundle->CreateBundle( "HydAct_to_ATVC1_dp", 10 );
		HydAct_to_ATVC1_pos = bundle->CreateBundle( "HydAct_to_ATVC1_pos", 10 );

		ATVC2_to_HydAct_srvlv = bundle->CreateBundle( "ATVC2_to_HydAct_srvlv", 10 );
		ATVC2_to_HydAct_isolvlv = bundle->CreateBundle( "ATVC2_to_HydAct_isolvlv", 10 );
		HydAct_to_ATVC2_dp = bundle->CreateBundle( "HydAct_to_ATVC2_dp", 10 );
		HydAct_to_ATVC2_pos = bundle->CreateBundle( "HydAct_to_ATVC2_pos", 10 );

		ATVC3_to_HydAct_srvlv = bundle->CreateBundle( "ATVC3_to_HydAct_srvlv", 10 );
		ATVC3_to_HydAct_isolvlv = bundle->CreateBundle( "ATVC3_to_HydAct_isolvlv", 10 );
		HydAct_to_ATVC3_dp = bundle->CreateBundle( "HydAct_to_ATVC3_dp", 10 );
		HydAct_to_ATVC3_pos = bundle->CreateBundle( "HydAct_to_ATVC3_pos", 10 );

		ATVC4_to_HydAct_srvlv = bundle->CreateBundle( "ATVC4_to_HydAct_srvlv", 10 );
		ATVC4_to_HydAct_isolvlv = bundle->CreateBundle( "ATVC4_to_HydAct_isolvlv", 10 );
		HydAct_to_ATVC4_dp = bundle->CreateBundle( "HydAct_to_ATVC4_dp", 10 );
		HydAct_to_ATVC4_pos = bundle->CreateBundle( "HydAct_to_ATVC4_pos", 10 );

		switch (ID)
		{
			case _1P:
				act = 0;
				break;
			case _1Y:
				act = 1;
				break;
			case _2P:
				act = 2;
				break;
			case _2Y:
				act = 3;
				break;
			case _3P:
				act = 4;
				break;
			case _3Y:
				act = 5;
				break;
			case _LHSRBT:
				act = 6;
				break;
			case _LHSRBR:
				act = 7;
				break;
			case _RHSRBT:
				act = 8;
				break;
			case _RHSRBR:
				act = 9;
				break;
		}

		in_servovalve_ATVC1.Connect( ATVC1_to_HydAct_srvlv, act );
		in_servovalve_ATVC2.Connect( ATVC2_to_HydAct_srvlv, act );
		in_servovalve_ATVC3.Connect( ATVC3_to_HydAct_srvlv, act );
		in_servovalve_ATVC4.Connect( ATVC4_to_HydAct_srvlv, act );

		in_isolationvalve_ATVC1.Connect( ATVC1_to_HydAct_isolvlv, act );
		in_isolationvalve_ATVC2.Connect( ATVC2_to_HydAct_isolvlv, act );
		in_isolationvalve_ATVC3.Connect( ATVC3_to_HydAct_isolvlv, act );
		in_isolationvalve_ATVC4.Connect( ATVC4_to_HydAct_isolvlv, act );

		out_deltapressure_ATVC1.Connect( HydAct_to_ATVC1_dp, act );
		out_deltapressure_ATVC2.Connect( HydAct_to_ATVC2_dp, act );
		out_deltapressure_ATVC3.Connect( HydAct_to_ATVC3_dp, act );
		out_deltapressure_ATVC4.Connect( HydAct_to_ATVC4_dp, act );

		out_positiontransducer_ATVC1.Connect( HydAct_to_ATVC1_pos, act );
		out_positiontransducer_ATVC2.Connect( HydAct_to_ATVC2_pos, act );
		out_positiontransducer_ATVC3.Connect( HydAct_to_ATVC3_pos, act );
		out_positiontransducer_ATVC4.Connect( HydAct_to_ATVC4_pos, act );

		return;
	}

	void HydActuator::OnPostStep( double fSimT, double fDeltaT, double fMJD )
	{
		// do stuff....
		return;
	}
}