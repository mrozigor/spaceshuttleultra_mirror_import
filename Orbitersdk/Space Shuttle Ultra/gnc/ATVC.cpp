#include "ATVC.h"


namespace gnc
{
	ATVC::ATVC( AtlantisSubsystemDirector* _director, const string& _ident, short nID ):AtlantisSubsystem( _director, _ident )
	{
		ID = nID;
		return;
	}

	ATVC::~ATVC( void )
	{
		// end
	}


	void ATVC::Realize( void )
	{
		//// connect cabling ////
		discsignals::DiscreteBundle* ATVC_to_HydAct_srvlv;
		discsignals::DiscreteBundle* ATVC_to_HydAct_isolvlv;
		discsignals::DiscreteBundle* HydAct_to_ATVC_dp;
		discsignals::DiscreteBundle* HydAct_to_ATVC_pos;
		discsignals::DiscreteBundle* ATVC_to_MDMFA;
		discsignals::DiscreteBundle* MDMFA_to_ATVC;

		switch (ID)// use your ID to connect to the right stuff
		{
			case 1:
				ATVC_to_HydAct_srvlv = BundleManager()->CreateBundle( "ATVC1_to_HydAct_srvlv", 10 );
				ATVC_to_HydAct_isolvlv = BundleManager()->CreateBundle( "ATVC1_to_HydAct_isolvlv", 10 );
				HydAct_to_ATVC_dp = BundleManager()->CreateBundle( "HydAct_to_ATVC1_dp", 10 );
				HydAct_to_ATVC_pos = BundleManager()->CreateBundle( "HydAct_to_ATVC1_pos", 10 );
				ATVC_to_MDMFA = BundleManager()->CreateBundle( "ATVC1_to_MDMFA1", 10 );
				MDMFA_to_ATVC = BundleManager()->CreateBundle( "MDMFA1_to_ATVC1", 11 );
				break;
			case 2:
				ATVC_to_HydAct_srvlv = BundleManager()->CreateBundle( "ATVC2_to_HydAct_srvlv", 10 );
				ATVC_to_HydAct_isolvlv = BundleManager()->CreateBundle( "ATVC2_to_HydAct_isolvlv", 10 );
				HydAct_to_ATVC_dp = BundleManager()->CreateBundle( "HydAct_to_ATVC2_dp", 10 );
				HydAct_to_ATVC_pos = BundleManager()->CreateBundle( "HydAct_to_ATVC2_pos", 10 );
				ATVC_to_MDMFA = BundleManager()->CreateBundle( "ATVC2_to_MDMFA2", 10 );
				MDMFA_to_ATVC = BundleManager()->CreateBundle( "MDMFA2_to_ATVC2", 11 );
				break;
			case 3:
				ATVC_to_HydAct_srvlv = BundleManager()->CreateBundle( "ATVC3_to_HydAct_srvlv", 10 );
				ATVC_to_HydAct_isolvlv = BundleManager()->CreateBundle( "ATVC3_to_HydAct_isolvlv", 10 );
				HydAct_to_ATVC_dp = BundleManager()->CreateBundle( "HydAct_to_ATVC3_dp", 10 );
				HydAct_to_ATVC_pos = BundleManager()->CreateBundle( "HydAct_to_ATVC3_pos", 10 );
				ATVC_to_MDMFA = BundleManager()->CreateBundle( "ATVC3_to_MDMFA3", 10 );
				MDMFA_to_ATVC = BundleManager()->CreateBundle( "MDMFA3_to_ATVC3", 11 );
				break;
			case 4:
				ATVC_to_HydAct_srvlv = BundleManager()->CreateBundle( "ATVC4_to_HydAct_srvlv", 10 );
				ATVC_to_HydAct_isolvlv = BundleManager()->CreateBundle( "ATVC4_to_HydAct_isolvlv", 10 );
				HydAct_to_ATVC_dp = BundleManager()->CreateBundle( "HydAct_to_ATVC4_dp", 10 );
				HydAct_to_ATVC_pos = BundleManager()->CreateBundle( "HydAct_to_ATVC4_pos", 10 );
				ATVC_to_MDMFA = BundleManager()->CreateBundle( "ATVC4_to_MDMFA4", 10 );
				MDMFA_to_ATVC = BundleManager()->CreateBundle( "MDMFA4_to_ATVC4", 11 );
				break;
			default:
				return;
		}

		in_MDMFA_pos_1P.Connect( MDMFA_to_ATVC, 0 );
		in_MDMFA_pos_1Y.Connect( MDMFA_to_ATVC, 1 );
		in_MDMFA_pos_2P.Connect( MDMFA_to_ATVC, 2 );
		in_MDMFA_pos_2Y.Connect( MDMFA_to_ATVC, 3 );
		in_MDMFA_pos_3P.Connect( MDMFA_to_ATVC, 4 );
		in_MDMFA_pos_3Y.Connect( MDMFA_to_ATVC, 5 );
		in_MDMFA_pos_LSRBT.Connect( MDMFA_to_ATVC, 6 );
		in_MDMFA_pos_LSRBR.Connect( MDMFA_to_ATVC, 7 );
		in_MDMFA_pos_RSRBT.Connect( MDMFA_to_ATVC, 8 );
		in_MDMFA_pos_RSRBR.Connect( MDMFA_to_ATVC, 9 );
		in_MDMFA_FCSCHSW.Connect( MDMFA_to_ATVC, 10 );// FCS CH sw (C3) ??comes thru MDMFA??

		in_1P_dp.Connect( HydAct_to_ATVC_dp, 0 );
		in_1Y_dp.Connect( HydAct_to_ATVC_dp, 1 );
		in_2P_dp.Connect( HydAct_to_ATVC_dp, 2 );
		in_2Y_dp.Connect( HydAct_to_ATVC_dp, 3 );
		in_3P_dp.Connect( HydAct_to_ATVC_dp, 4 );
		in_3Y_dp.Connect( HydAct_to_ATVC_dp, 5 );
		in_LSRBT_dp.Connect( HydAct_to_ATVC_dp, 6 );
		in_LSRBR_dp.Connect( HydAct_to_ATVC_dp, 7 );
		in_RSRBT_dp.Connect( HydAct_to_ATVC_dp, 8 );
		in_RSRBR_dp.Connect( HydAct_to_ATVC_dp, 9 );

		in_1P_pos.Connect( HydAct_to_ATVC_pos, 0 );
		in_1Y_pos.Connect( HydAct_to_ATVC_pos, 1 );
		in_2P_pos.Connect( HydAct_to_ATVC_pos, 2 );
		in_2Y_pos.Connect( HydAct_to_ATVC_pos, 3 );
		in_3P_pos.Connect( HydAct_to_ATVC_pos, 4 );
		in_3Y_pos.Connect( HydAct_to_ATVC_pos, 5 );
		in_LSRBT_pos.Connect( HydAct_to_ATVC_pos, 6);
		in_LSRBR_pos.Connect( HydAct_to_ATVC_pos, 7 );
		in_RSRBT_pos.Connect( HydAct_to_ATVC_pos, 8 );
		in_RSRBR_pos.Connect( HydAct_to_ATVC_pos, 9 );

		out_MDMFA_pos_1P.Connect( ATVC_to_MDMFA, 0 );
		out_MDMFA_pos_1Y.Connect( ATVC_to_MDMFA, 1 );
		out_MDMFA_pos_2P.Connect( ATVC_to_MDMFA, 2 );
		out_MDMFA_pos_2Y.Connect( ATVC_to_MDMFA, 3 );
		out_MDMFA_pos_3P.Connect( ATVC_to_MDMFA, 4 );
		out_MDMFA_pos_3Y.Connect( ATVC_to_MDMFA, 5 );
		out_MDMFA_pos_LSRBT.Connect( ATVC_to_MDMFA, 6 );
		out_MDMFA_pos_LSRBR.Connect( ATVC_to_MDMFA, 7 );
		out_MDMFA_pos_RSRBT.Connect( ATVC_to_MDMFA, 8 );
		out_MDMFA_pos_RSRBR.Connect( ATVC_to_MDMFA, 9 );

		out_1P_srvlv.Connect( ATVC_to_HydAct_srvlv, 0 );
		out_1Y_srvlv.Connect( ATVC_to_HydAct_srvlv, 1 );
		out_2P_srvlv.Connect( ATVC_to_HydAct_srvlv, 2 );
		out_2Y_srvlv.Connect( ATVC_to_HydAct_srvlv, 3 );
		out_3P_srvlv.Connect( ATVC_to_HydAct_srvlv, 4 );
		out_3Y_srvlv.Connect( ATVC_to_HydAct_srvlv, 5 );
		out_LSRBT_srvlv.Connect( ATVC_to_HydAct_srvlv, 6 );
		out_LSRBR_srvlv.Connect( ATVC_to_HydAct_srvlv, 7 );
		out_RSRBT_srvlv.Connect( ATVC_to_HydAct_srvlv, 8 );
		out_RSRBR_srvlv.Connect( ATVC_to_HydAct_srvlv, 9 );

		out_1P_isolvlv.Connect( ATVC_to_HydAct_isolvlv, 0 );
		out_1Y_isolvlv.Connect( ATVC_to_HydAct_isolvlv, 1 );
		out_2P_isolvlv.Connect( ATVC_to_HydAct_isolvlv, 2 );
		out_2Y_isolvlv.Connect( ATVC_to_HydAct_isolvlv, 3 );
		out_3P_isolvlv.Connect( ATVC_to_HydAct_isolvlv, 4 );
		out_3Y_isolvlv.Connect( ATVC_to_HydAct_isolvlv, 5 );
		out_LSRBT_isolvlv.Connect( ATVC_to_HydAct_isolvlv, 6 );
		out_LSRBR_isolvlv.Connect( ATVC_to_HydAct_isolvlv, 7 );
		out_RSRBT_isolvlv.Connect( ATVC_to_HydAct_isolvlv, 8 );
		out_RSRBR_isolvlv.Connect( ATVC_to_HydAct_isolvlv, 9 );
		return;
	}

	void ATVC::OnPostStep( double fSimT, double fDeltaT, double fMJD )
	{
		// get current act pos
		float act_pos[10];
		act_pos[0] = (in_1P_pos.GetVoltage() - 5) * 10;// TODO validate line
		act_pos[1] = (in_1Y_pos.GetVoltage() - 5) * 10;
		act_pos[2] = (in_2P_pos.GetVoltage() - 5) * 10;
		act_pos[3] = (in_2Y_pos.GetVoltage() - 5) * 10;
		act_pos[4] = (in_3P_pos.GetVoltage() - 5) * 10;
		act_pos[5] = (in_3Y_pos.GetVoltage() - 5) * 10;
		act_pos[6] = (in_LSRBT_pos.GetVoltage() - 5) * 10;
		act_pos[7] = (in_LSRBR_pos.GetVoltage() - 5) * 10;
		act_pos[8] = (in_RSRBT_pos.GetVoltage() - 5) * 10;
		act_pos[9] = (in_RSRBR_pos.GetVoltage() - 5) * 10;


		// send position to MDM FA
		out_MDMFA_pos_1P.SetLine( (act_pos[0] / 10) + 5 );
		out_MDMFA_pos_1Y.SetLine( (act_pos[1] / 10) + 5 );
		out_MDMFA_pos_2P.SetLine( (act_pos[2] / 10) + 5 );
		out_MDMFA_pos_2Y.SetLine( (act_pos[3] / 10) + 5 );
		out_MDMFA_pos_3P.SetLine( (act_pos[4] / 10) + 5 );
		out_MDMFA_pos_3Y.SetLine( (act_pos[5] / 10) + 5 );
		out_MDMFA_pos_LSRBT.SetLine( (act_pos[6] / 10) + 5 );
		out_MDMFA_pos_LSRBR.SetLine( (act_pos[7] / 10) + 5 );
		out_MDMFA_pos_RSRBT.SetLine( (act_pos[8] / 10) + 5 );
		out_MDMFA_pos_RSRBR.SetLine( (act_pos[9] / 10) + 5 );


		float FCSCHSW = in_MDMFA_FCSCHSW.GetVoltage();// 5v < OVERRIDE < 10v, 10v < AUTO < 15v, 15v < OFF < 20v
		if ((FCSCHSW > 5) && (FCSCHSW < 10))
		{
			// OVERRIDE
			out_1P_isolvlv.ResetLine();
			out_1Y_isolvlv.ResetLine();
			out_2P_isolvlv.ResetLine();
			out_2Y_isolvlv.ResetLine();
			out_3P_isolvlv.ResetLine();
			out_3Y_isolvlv.ResetLine();
			out_LSRBT_isolvlv.ResetLine();
			out_LSRBR_isolvlv.ResetLine();
			out_RSRBT_isolvlv.ResetLine();
			out_RSRBR_isolvlv.ResetLine();
		}
		else
		{
			if ((FCSCHSW > 10) && (FCSCHSW < 15))
			{
				// AUTO
				// TODO improve...
				if (in_1P_dp.GetVoltage() > 10)
				{
					out_1P_isolvlv.SetLine();
				}
				else
				{
					out_1P_isolvlv.ResetLine();
				}

				if (in_1Y_dp.GetVoltage() > 10)
				{
					out_1Y_isolvlv.SetLine();
				}
				else
				{
					out_1Y_isolvlv.ResetLine();
				}

				if (in_2P_dp.GetVoltage() > 10)
				{
					out_2P_isolvlv.SetLine();
				}
				else
				{
					out_2P_isolvlv.ResetLine();
				}

				if (in_2Y_dp.GetVoltage() > 10)
				{
					out_2Y_isolvlv.SetLine();
				}
				else
				{
					out_2Y_isolvlv.ResetLine();
				}

				if (in_3P_dp.GetVoltage() > 10)
				{
					out_3P_isolvlv.SetLine();
				}
				else
				{
					out_3P_isolvlv.ResetLine();
				}

				if (in_3Y_dp.GetVoltage() > 10)
				{
					out_3Y_isolvlv.SetLine();
				}
				else
				{
					out_3Y_isolvlv.ResetLine();
				}

				if (in_LSRBT_dp.GetVoltage() > 10)
				{
					out_LSRBT_isolvlv.SetLine();
				}
				else
				{
					out_LSRBT_isolvlv.ResetLine();
				}

				if (in_LSRBR_dp.GetVoltage() > 10)
				{
					out_LSRBR_isolvlv.SetLine();
				}
				else
				{
					out_LSRBR_isolvlv.ResetLine();
				}

				if (in_RSRBT_dp.GetVoltage() > 10)
				{
					out_RSRBT_isolvlv.SetLine();
				}
				else
				{
					out_RSRBT_isolvlv.ResetLine();
				}

				if (in_RSRBR_dp.GetVoltage() > 10)
				{
					out_RSRBR_isolvlv.SetLine();
				}
				else
				{
					out_RSRBR_isolvlv.ResetLine();
				}
			}
			else
			{
				if ((FCSCHSW > 15) && (FCSCHSW < 20))
				{
					// OFF
					out_1P_isolvlv.SetLine();
					out_1Y_isolvlv.SetLine();
					out_2P_isolvlv.SetLine();
					out_2Y_isolvlv.SetLine();
					out_3P_isolvlv.SetLine();
					out_3Y_isolvlv.SetLine();
					out_LSRBT_isolvlv.SetLine();
					out_LSRBR_isolvlv.SetLine();
					out_RSRBT_isolvlv.SetLine();
					out_RSRBR_isolvlv.SetLine();

					out_1P_srvlv.SetLine( 0 );
					out_1Y_srvlv.SetLine( 0 );
					out_2P_srvlv.SetLine( 0 );
					out_2Y_srvlv.SetLine( 0 );
					out_3P_srvlv.SetLine( 0 );
					out_3Y_srvlv.SetLine( 0 );
					out_LSRBT_srvlv.SetLine( 0 );
					out_LSRBR_srvlv.SetLine( 0 );
					out_RSRBT_srvlv.SetLine( 0 );
					out_RSRBR_srvlv.SetLine( 0 );
					return;
				}
			}
		}

		float cmd_pos[10];
		float srvlv_volt[10];
		short count = 0;

		cmd_pos[0] = (in_MDMFA_pos_1P.GetVoltage() - 5) * 10;// TODO validate line
		cmd_pos[1] = (in_MDMFA_pos_1Y.GetVoltage() - 5) * 10;
		cmd_pos[2] = (in_MDMFA_pos_2P.GetVoltage() - 5) * 10;
		cmd_pos[3] = (in_MDMFA_pos_2Y.GetVoltage() - 5) * 10;
		cmd_pos[4] = (in_MDMFA_pos_3P.GetVoltage() - 5) * 10;
		cmd_pos[5] = (in_MDMFA_pos_3Y.GetVoltage() - 5) * 10;
		cmd_pos[6] = (in_MDMFA_pos_LSRBT.GetVoltage() - 5) * 10;
		cmd_pos[7] = (in_MDMFA_pos_LSRBR.GetVoltage() - 5) * 10;
		cmd_pos[8] = (in_MDMFA_pos_RSRBT.GetVoltage() - 5) * 10;
		cmd_pos[9] = (in_MDMFA_pos_RSRBR.GetVoltage() - 5) * 10;

		while (count < 10)
		{
			if (cmd_pos[count] != act_pos[count])
			{
				srvlv_volt[count] = (cmd_pos[count] - act_pos[count]) / 10;// move servovalve
			}
			else
			{
				srvlv_volt[count] = 0;// close servovalve
			}
			count++;
		}

		out_1P_srvlv.SetLine( srvlv_volt[0] );
		out_1Y_srvlv.SetLine( srvlv_volt[1] );
		out_2P_srvlv.SetLine( srvlv_volt[2] );
		out_2Y_srvlv.SetLine( srvlv_volt[3] );
		out_3P_srvlv.SetLine( srvlv_volt[4] );
		out_3Y_srvlv.SetLine( srvlv_volt[5] );
		out_LSRBT_srvlv.SetLine( srvlv_volt[6] );
		out_LSRBR_srvlv.SetLine( srvlv_volt[7] );
		out_RSRBT_srvlv.SetLine( srvlv_volt[8] );
		out_RSRBR_srvlv.SetLine( srvlv_volt[9] );
		return;
	}
}