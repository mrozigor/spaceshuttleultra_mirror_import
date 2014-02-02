#include "MPS_Dump.h"
#include "..\Atlantis.h"
#include "SSME_SOP.h"
#include "SSME_Operations.h"
#include "IO_Control.h"
#include "assert.h"


namespace dps
{
	MPS_Dump::MPS_Dump( SimpleGPCSystem *_gpc ):SimpleGPCSoftware( _gpc, "MPS_Dump" )
	{
		active = false;

		LH2_OTBD_FD_VLV_OP_CMD = false;
		LH2_OTBD_FD_VLV_CL_CMD = false;
		LH2_INBD_FD_VLV_OP_CMD = false;
		LH2_INBD_FD_VLV_CL_CMD = false;

		LH2_TOPPING_VLV_OP_CMD = false;
		return;
	}

	MPS_Dump::~MPS_Dump( void )
	{
		return;
	}

	void MPS_Dump::OnPreStep( double SimT, double DeltaT, double MJD )
	{
		if (active == false)
		{
			if (pSSME_Operations->GetMECOConfirmedFlag() == true)
			{
				active = true;
				t_MECO = SimT;
				t_current = SimT;
				
				pIO_Control->SetCommand( PNEU_L_HE_XOVR_OP, true );
				pIO_Control->SetCommand( HE_IC_CTR_OUT_OP, true );
				pIO_Control->SetCommand( HE_IC_LEFT_OUT_OP, true );
				pIO_Control->SetCommand( HE_IC_RIGHT_OUT_OP, true );
			}
		}
		else
		{
			// TODO get rid of t_current
			t_last = t_current;
			t_current = SimT;

			if (GetMajorMode() > 600)// RTLS
			{
				//
			}
			else// NOM
			{
				// LOX
				if (((t_current - t_MECO) >= LOX_DUMP_START_DELAY) && ((t_last - t_MECO) <= LOX_DUMP_START_DELAY))// LOX start dump
				{
					pSSME_SOP->SetOxidizerDumpStartCommandFlag( 1 );
					pSSME_SOP->SetOxidizerDumpStartCommandFlag( 2 );
					pSSME_SOP->SetOxidizerDumpStartCommandFlag( 3 );
					dspLO2_Manf_Repress_1.SetLine();
					dspLO2_Manf_Repress_2.SetLine();
				}
				if ((t_current - t_MECO) >= LOX_DUMP_START_DELAY)// LOX dump
				{
					pIO_Control->SetCommand( ME1_LOX_PVLV_OP_A, true );
					pIO_Control->SetCommand( ME2_LOX_PVLV_OP_A, true );
					pIO_Control->SetCommand( ME3_LOX_PVLV_OP_A, true );
					pIO_Control->SetCommand( ME1_LOX_PVLV_OP_B, true );
					pIO_Control->SetCommand( ME2_LOX_PVLV_OP_B, true );
					pIO_Control->SetCommand( ME3_LOX_PVLV_OP_B, true );
					pIO_Control->SetCommand( ME1_LOX_PVLV_CL_A, false );
					pIO_Control->SetCommand( ME2_LOX_PVLV_CL_A, false );
					pIO_Control->SetCommand( ME3_LOX_PVLV_CL_A, false );
					pIO_Control->SetCommand( ME1_LOX_PVLV_CL_B, false );
					pIO_Control->SetCommand( ME2_LOX_PVLV_CL_B, false );
					pIO_Control->SetCommand( ME3_LOX_PVLV_CL_B, false );
				}

				if (((t_current - t_MECO) >= (LOX_DUMP_START_DELAY + LOX_DUMP_PRESS_DURATION)) && ((t_last - t_MECO) <= (LOX_DUMP_START_DELAY + LOX_DUMP_PRESS_DURATION)))// LOX stop press dump
				{
					dspLO2_Manf_Repress_1.ResetLine();
					dspLO2_Manf_Repress_2.ResetLine();
				}
				if (((t_current - t_MECO) >= (LOX_DUMP_START_DELAY + LOX_DUMP_DURATION)) && ((t_last - t_MECO) <= (LOX_DUMP_START_DELAY + LOX_DUMP_DURATION)))// LOX stop dump
				{
					pSSME_SOP->SetDumpStopCommandFlag( 1 );
					pSSME_SOP->SetDumpStopCommandFlag( 2 );
					pSSME_SOP->SetDumpStopCommandFlag( 3 );
				}
				if ((t_current - t_MECO) >= (LOX_DUMP_START_DELAY + LOX_DUMP_DURATION))// LOX stop dump
				{
					//dspLV12.ResetLine();
					//dspLV14.ResetLine();
					//dspLV16.ResetLine();
					//dspLV13.SetLine();
					//dspLV15.SetLine();
					//dspLV17.SetLine();
				}

				// LH2
				if ((t_current - t_MECO) >= LH2_DUMP_BU_VLV_START_DELAY)// LH2 start dump B/U vlv
				{
					pIO_Control->SetCommand( LH2_INBD_BU_DV_OP, true );
					pIO_Control->SetCommand( LH2_OTBD_BU_DV_OP, true );
				}

				if ((t_current - t_MECO) >= (LH2_DUMP_BU_VLV_START_DELAY + LH2_DUMP_BU_VLV_DURATION))// LH2 stop dump B/U vlv
				{
					pIO_Control->SetCommand( LH2_INBD_BU_DV_OP, false );
					pIO_Control->SetCommand( LH2_OTBD_BU_DV_OP, false );
				}


				//if (((t_current - t_MECO) >= LH2_DUMP_START_DELAY) && ((t_last - t_MECO) <= LH2_DUMP_START_DELAY))// LH2 start dump
				//{
				//	dspLV32.SetLine();
				//	dspLV34.SetLine();
				//}
				if ((t_current - t_MECO) >= LH2_DUMP_START_DELAY)// LH2 dump
				{
					pIO_Control->SetCommand( ME1_LH2_PVLV_OP, true );
					pIO_Control->SetCommand( ME2_LH2_PVLV_OP, true );
					pIO_Control->SetCommand( ME3_LH2_PVLV_OP, true );
					pIO_Control->SetCommand( ME1_LH2_PVLV_CL, false );
					pIO_Control->SetCommand( ME2_LH2_PVLV_CL, false );
					pIO_Control->SetCommand( ME3_LH2_PVLV_CL, false );

					LH2_OTBD_FD_VLV_OP_CMD = true;
					LH2_OTBD_FD_VLV_CL_CMD = false;
					LH2_INBD_FD_VLV_OP_CMD = true;
					LH2_INBD_FD_VLV_CL_CMD = false;

					LH2_TOPPING_VLV_OP_CMD = true;
				}

				//if (((t_current - t_MECO) >= (LH2_DUMP_START_DELAY + LH2_DUMP_DURATION)) && ((t_last - t_MECO) <= (LH2_DUMP_START_DELAY + LH2_DUMP_DURATION)))// LH2 stop dump
				if ((t_current - t_MECO) >= (LH2_DUMP_START_DELAY + LH2_DUMP_DURATION))// LH2 stop dump
				{
					pIO_Control->SetCommand( ME1_LH2_PVLV_OP, false );
					pIO_Control->SetCommand( ME2_LH2_PVLV_OP, false );
					pIO_Control->SetCommand( ME3_LH2_PVLV_OP, false );
					pIO_Control->SetCommand( ME1_LH2_PVLV_CL, false );
					pIO_Control->SetCommand( ME2_LH2_PVLV_CL, false );
					pIO_Control->SetCommand( ME3_LH2_PVLV_CL, false );

					LH2_OTBD_FD_VLV_OP_CMD = false;
					LH2_OTBD_FD_VLV_CL_CMD = true;
					LH2_INBD_FD_VLV_OP_CMD = false;
					LH2_INBD_FD_VLV_CL_CMD = false;

					LH2_TOPPING_VLV_OP_CMD = false;
				}
			}

			if (LH2_OTBD_FD_VLV_OP_CMD == true) dspLH2_OTBD_FD_VLV_OP.SetLine();
			else dspLH2_OTBD_FD_VLV_OP.ResetLine();

			if (LH2_OTBD_FD_VLV_CL_CMD == true) dspLH2_OTBD_FD_VLV_CL.SetLine();
			else dspLH2_OTBD_FD_VLV_CL.ResetLine();

			if (LH2_INBD_FD_VLV_OP_CMD == true) dspLH2_INBD_FD_VLV_OP.SetLine();
			else dspLH2_INBD_FD_VLV_OP.ResetLine();

			if (LH2_INBD_FD_VLV_CL_CMD == true) dspLH2_INBD_FD_VLV_CL.SetLine();
			else dspLH2_INBD_FD_VLV_CL.ResetLine();

			if (LH2_TOPPING_VLV_OP_CMD == true) dspLH2_TOPPING_VLV_OP.SetLine();
			else dspLH2_TOPPING_VLV_OP.ResetLine();
		}
		return;
	}

	void MPS_Dump::Realize( void )
	{
		DiscreteBundle *bundle = BundleManager()->CreateBundle( "MPS_LV_B", 16 );// LV17 - LV32
		dspLH2_OTBD_FD_VLV_OP.Connect( bundle, 15 );

		bundle = BundleManager()->CreateBundle( "MPS_LV_C", 16 );// LV33 - LV48
		dspLH2_OTBD_FD_VLV_CL.Connect( bundle, 0 );
		dspLH2_INBD_FD_VLV_OP.Connect( bundle, 1 );
		dspLH2_INBD_FD_VLV_CL.Connect( bundle, 2 );
		dspLH2_TOPPING_VLV_OP.Connect( bundle, 6 );
		dspLO2_Manf_Repress_1.Connect( bundle, 7 );
		dspLO2_Manf_Repress_2.Connect( bundle, 8 );

		
		pSSME_SOP = static_cast<SSME_SOP*> (FindSoftware( "SSME_SOP" ));
		assert( (pSSME_SOP != NULL) && "MPS_Dump::Realize.pSSME_SOP" );
		pSSME_Operations = static_cast<SSME_Operations*> (FindSoftware( "SSME_Operations" ));
		assert( (pSSME_Operations != NULL) && "MPS_Dump::Realize.pSSME_Operations" );
		pIO_Control = static_cast<IO_Control*> (FindSoftware( "IO_Control" ));
		assert( (pIO_Control != NULL) && "MPS_Dump::Realize.pIO_Control" );
		return;
	}

	bool MPS_Dump::OnMajorModeChange( unsigned int newMajorMode )
	{
		switch (newMajorMode)
		{
			case 103:
			case 104:
			case 601:
			case 602:
			case 603:
				return true;
			default:
				return false;
		}
	}
}