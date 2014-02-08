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
		dump_started = false;

		MM106_trans = false;

		t_last = 0;
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

				pIO_Control->SetCommand( PNEU_L_HE_XOVR_OP, true );// HACK should it be in SSME OPS?
			}
		}
		else
		{
			if (GetMajorMode() > 600)// RTLS
			{
				//
			}
			else// NOM
			{
				// LH2 B/U dump start
				if (((t_MECO + LH2_DUMP_BU_VLV_START_DELAY) <= SimT) && ((t_MECO + LH2_DUMP_BU_VLV_START_DELAY) > t_last))
				{
					pIO_Control->SetCommand( LH2_INBD_BU_DV_OP, true );
					pIO_Control->SetCommand( LH2_OTBD_BU_DV_OP, true );
					oapiWriteLog( "LH2 Backup Dump Valves open" );
				}

				// LH2 B/U dump end
				if (((t_MECO + LH2_DUMP_BU_VLV_START_DELAY + LH2_DUMP_BU_VLV_DURATION) <= SimT) && ((t_MECO + LH2_DUMP_BU_VLV_START_DELAY + LH2_DUMP_BU_VLV_DURATION) > t_last))
				{
					pIO_Control->SetCommand( LH2_INBD_BU_DV_OP, false );
					pIO_Control->SetCommand( LH2_OTBD_BU_DV_OP, false );
					oapiWriteLog( "LH2 Backup Dump Valves close" );
				}

				// He I/Cs
				if (((t_MECO + HE_IC_OP_DELAY) <= SimT) && ((t_MECO + HE_IC_OP_DELAY) > t_last))
				{
					pIO_Control->SetCommand( HE_IC_CTR_OUT_OP, true );
					pIO_Control->SetCommand( HE_IC_LEFT_IN_OP, true );
					pIO_Control->SetCommand( HE_IC_RIGHT_OUT_OP, true );
					oapiWriteLog( "Helium Interconnect valves open" );
				}

				if (dump_started == false)
				{
					if (((t_MECO + DUMP_START_DELAY) <= SimT) || (pIO_Control->GetSWPos( SW_DUMPSEQUENCE ) == 2) || ((STS()->GetLH2ManifPress() > 60) && (STS()->GetLH2ManifPress() < 90)))// time, sw in start or overpressure
					{
						// start dump
						if (pIO_Control->GetSWPos( SW_DUMPSEQUENCE ) != 0)// sw not in stop
						{
							dump_started = true;
							t_dump_start = SimT;
							oapiWriteLog( "MPS Dump Sequence start" );
						}
					}
				}

				if (dump_started == true)
				{
					// LOX dump start
					if (((t_dump_start + LOX_DUMP_START_DELAY) <= SimT) && ((t_dump_start + LOX_DUMP_START_DELAY) > t_last))
					{
						pSSME_SOP->SetOxidizerDumpStartCommandFlag( 1 );
						pSSME_SOP->SetOxidizerDumpStartCommandFlag( 2 );
						pSSME_SOP->SetOxidizerDumpStartCommandFlag( 3 );
						pIO_Control->SetCommand( LOX_REPRESS_1_OP, true );
						pIO_Control->SetCommand( LOX_REPRESS_2_OP, true );
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
						oapiWriteLog( "LOX Dump start" );
					}

					// LOX dump end
					if (((t_dump_start + LOX_DUMP_START_DELAY + LOX_DUMP_PRESS_DURATION) <= SimT) && ((t_dump_start + LOX_DUMP_START_DELAY + LOX_DUMP_PRESS_DURATION) > t_last))
					{
						pIO_Control->SetCommand( LOX_REPRESS_1_OP, false );
						pIO_Control->SetCommand( LOX_REPRESS_2_OP, false );
					}
					if (((t_dump_start + LOX_DUMP_START_DELAY + LOX_DUMP_DURATION) <= SimT) && ((t_dump_start + LOX_DUMP_START_DELAY + LOX_DUMP_DURATION) > t_last))
					{
						pSSME_SOP->SetDumpStopCommandFlag( 1 );
						pSSME_SOP->SetDumpStopCommandFlag( 2 );
						pSSME_SOP->SetDumpStopCommandFlag( 3 );
						pIO_Control->SetCommand( ME1_LOX_PVLV_OP_A, false );
						pIO_Control->SetCommand( ME2_LOX_PVLV_OP_A, false );
						pIO_Control->SetCommand( ME3_LOX_PVLV_OP_A, false );
						pIO_Control->SetCommand( ME1_LOX_PVLV_OP_B, false );
						pIO_Control->SetCommand( ME2_LOX_PVLV_OP_B, false );
						pIO_Control->SetCommand( ME3_LOX_PVLV_OP_B, false );
						oapiWriteLog( "LOX Dump complete" );
					}

					// LH2 dump start
					if (((t_dump_start + LH2_DUMP_START_DELAY) <= SimT) && ((t_dump_start + LH2_DUMP_START_DELAY) > t_last))
					{
						pIO_Control->SetCommand( LH2_OTBD_FD_VLV_CL, false );
						pIO_Control->SetCommand( LH2_OTBD_FD_VLV_OP, true );
						pIO_Control->SetCommand( LH2_INBD_FD_VLV_CL, false );
						pIO_Control->SetCommand( LH2_INBD_FD_VLV_OP, true );
						pIO_Control->SetCommand( LH2_TOPPING_VLV_OP, true );
						pIO_Control->SetCommand( ME1_LH2_PVLV_OP, true );
						pIO_Control->SetCommand( ME2_LH2_PVLV_OP, true );
						pIO_Control->SetCommand( ME3_LH2_PVLV_OP, true );
						pIO_Control->SetCommand( ME1_LH2_PVLV_CL, false );
						pIO_Control->SetCommand( ME2_LH2_PVLV_CL, false );
						pIO_Control->SetCommand( ME3_LH2_PVLV_CL, false );
						oapiWriteLog( "LH2 Dump start" );
					}

					// LH2 dump end
					if (((t_dump_start + LH2_DUMP_START_DELAY + LH2_DUMP_DURATION) <= SimT) && ((t_dump_start + LH2_DUMP_START_DELAY + LH2_DUMP_DURATION) > t_last))
					{
						pIO_Control->SetCommand( LH2_OTBD_FD_VLV_CL, true );
						pIO_Control->SetCommand( LH2_OTBD_FD_VLV_OP, false );
						pIO_Control->SetCommand( LH2_INBD_FD_VLV_OP, false );
						pIO_Control->SetCommand( LH2_TOPPING_VLV_OP, false );
						pIO_Control->SetCommand( ME1_LH2_PVLV_OP, false );
						pIO_Control->SetCommand( ME2_LH2_PVLV_OP, false );
						pIO_Control->SetCommand( ME3_LH2_PVLV_OP, false );
						oapiWriteLog( "LH2 Dump complete" );
					}

					// end of dump
					if (((t_dump_start + HE_IC_CL_DELAY) <= SimT) && ((t_dump_start + HE_IC_CL_DELAY) > t_last))
					{
						pIO_Control->SetCommand( HE_IC_CTR_OUT_OP, false );
						pIO_Control->SetCommand( HE_IC_LEFT_IN_OP, false );
						pIO_Control->SetCommand( HE_IC_RIGHT_OUT_OP, false );
						pIO_Control->SetCommand( HE_ISOL_A_CTR_OP, false );
						pIO_Control->SetCommand( HE_ISOL_A_LEFT_OP, false );
						pIO_Control->SetCommand( HE_ISOL_A_RIGHT_OP, false );
						pIO_Control->SetCommand( HE_ISOL_B_CTR_OP, false );
						pIO_Control->SetCommand( HE_ISOL_B_LEFT_OP, false );
						pIO_Control->SetCommand( HE_ISOL_B_RIGHT_OP, false );
						// TODO turn off BODY FLAP lights on panels F2 and F4
						oapiWriteLog( "MPS Dump Sequence complete" );
					}

					// contingency LH2 vac inert
					if (((t_dump_start + HE_IC_CL_DELAY) < SimT) && ((t_dump_start + FIRST_AUTOMATED_VACUUM_INERT_START_DELAY) > SimT))
					{
						if (((STS()->GetLH2ManifPress() > 60) && (STS()->GetLH2ManifPress() < 90)) && (pIO_Control->GetSWPos( SW_BACKUPLH2VLV ) != 2))// overpressure and sw not in open
						{
							pIO_Control->SetCommand( LH2_OTBD_FD_VLV_CL, false );
							pIO_Control->SetCommand( LH2_OTBD_FD_VLV_OP, true );
							pIO_Control->SetCommand( LH2_TOPPING_VLV_OP, true );
							oapiWriteLog( "Contingency LH2 Vacuum Inert start" );
						}
					}

					// vac inert 1 start
					if (((t_dump_start + FIRST_AUTOMATED_VACUUM_INERT_START_DELAY) <= SimT) && ((t_dump_start + FIRST_AUTOMATED_VACUUM_INERT_START_DELAY) > t_last))
					{
						pIO_Control->SetCommand( LOX_OTBD_FD_VLV_CL, false );
						pIO_Control->SetCommand( LOX_OTBD_FD_VLV_OP, true );
						pIO_Control->SetCommand( LOX_INBD_FD_VLV_CL, false );
						pIO_Control->SetCommand( LOX_INBD_FD_VLV_OP, true );
						pIO_Control->SetCommand( LH2_INBD_BU_DV_OP, true );
						pIO_Control->SetCommand( LH2_OTBD_BU_DV_OP, true );
						oapiWriteLog( "Automated Vacuum Inert 1 start" );
					}

					// vac inert 1 end
					if (((t_dump_start + FIRST_AUTOMATED_VACUUM_INERT_START_DELAY + FIRST_AUTOMATED_VACUUM_INERT_DURATION) <= SimT) && ((t_dump_start + FIRST_AUTOMATED_VACUUM_INERT_START_DELAY + FIRST_AUTOMATED_VACUUM_INERT_DURATION) > t_last))
					{
						pIO_Control->SetCommand( LOX_OTBD_FD_VLV_CL, true );
						pIO_Control->SetCommand( LOX_OTBD_FD_VLV_OP, false );
						pIO_Control->SetCommand( LOX_INBD_FD_VLV_OP, false );
						pIO_Control->SetCommand( LH2_INBD_BU_DV_OP, false );
						pIO_Control->SetCommand( LH2_OTBD_BU_DV_OP, false );
						// PV11 and PV13 close now if opened in the contingency LH2 vac inert
						pIO_Control->SetCommand( LH2_OTBD_FD_VLV_CL, true );
						pIO_Control->SetCommand( LH2_OTBD_FD_VLV_OP, false );
						pIO_Control->SetCommand( LH2_TOPPING_VLV_OP, false );
						oapiWriteLog( "Automated Vacuum Inert 1 complete" );
					}

					if (MM106_trans == true)
					{
						// vac inert 2 start
						if (((t_MM106_trans + SECOND_AUTOMATED_VACUUM_INERT_START_DELAY) <= SimT) && ((t_MM106_trans + SECOND_AUTOMATED_VACUUM_INERT_START_DELAY) > t_last))
						{
							pIO_Control->SetCommand( LH2_INBD_BU_DV_OP, true );
							pIO_Control->SetCommand( LH2_OTBD_BU_DV_OP, true );
							oapiWriteLog( "Automated Vacuum Inert 2 start" );
						}

						// vac inert 2 end
						if (((t_MM106_trans + SECOND_AUTOMATED_VACUUM_INERT_START_DELAY + SECOND_AUTOMATED_VACUUM_INERT_DURATION) <= SimT) && ((t_MM106_trans + SECOND_AUTOMATED_VACUUM_INERT_START_DELAY + SECOND_AUTOMATED_VACUUM_INERT_DURATION) > t_last))
						{
							pIO_Control->SetCommand( LH2_INBD_BU_DV_OP, false );
							pIO_Control->SetCommand( LH2_OTBD_BU_DV_OP, false );
							oapiWriteLog( "Automated Vacuum Inert 2 complete" );
						}
					}
				}
				t_last = SimT;
			}
		}
		return;
	}

	void MPS_Dump::Realize( void )
	{
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
			case 106:
				if (GetMajorMode() == 105)
				{
					MM106_trans = true;
					t_MM106_trans = t_last;
				}
			case 103:
			case 104:
			case 105:
			case 601:
			case 602:
			case 603:
				return true;
			default:
				return false;
		}
	}
}