#include "ETSepSequence.h"
#include "..\Atlantis.h"
#include "SSME_Operations.h"
#include "TransitionDAP.h"
#include "IO_Control.h"
#include "ATVC_SOP.h"
#include "assert.h"


namespace dps
{
	ETSepSequence::ETSepSequence( SimpleGPCSystem *_gpc ):SimpleGPCSoftware( _gpc, "ETSepSequence" )
	{
		active = false;
		done = false;
		autoETSEP = false;
		ETSEPCommand = false;
		ETSEPINH = false;
		
		t_MECO = -1;
		t_last = -1;
		timerSEP = -1;
		return;
	}

	ETSepSequence::~ETSepSequence( void )
	{
		return;
	}

	void ETSepSequence::OnPostStep( double SimT, double DeltaT, double MJD )
	{
		if (done == true) return;

		if (active == true)
		{
			if (ETSEPSW.IsSet() == true)
			{
				if (ETSEPPB.IsSet() == true)
				{
					if (ETSEPCommand == false)
					{
						char buffer[64];
						sprintf_s( buffer, 64, "MAN ET SEP" );
						oapiWriteLog( buffer );
					}
					ETSEPCommand = true;
				}
			}
			else
			{
				if (((t_MECO + ET_SEP_DELAY_MECO) <= SimT) && ((t_MECO + ET_SEP_DELAY_MECO) > t_last))
				{
					autoETSEP = true;
				}
			}

			if (((t_MECO + ET_SEP_CLOSE_PD) <= SimT) && ((t_MECO + ET_SEP_CLOSE_PD) > t_last))
			{
				// close feedline valves
				PD1_OP.ResetLine();
				PD1_CL.SetLine();
				PD2_OP.ResetLine();
				PD2_CL.SetLine();
				PD3_OP.ResetLine();
				PD3_CL.SetLine();
			}

			if (((t_MECO + ET_SEP_OPEN_SOV) <= SimT) && ((t_MECO + ET_SEP_OPEN_SOV) > t_last))
			{
				// open feedline relief SOVs
				pIO_Control->SetCommand( LO2_FEEDLINE_RLF_ISOL_CL, false );
				pIO_Control->SetCommand( LH2_FEEDLINE_RLF_ISOL_CL, false );
			}

			if (((t_MECO + ET_SEP_SSME_STOW) <= SimT) && ((t_MECO + ET_SEP_SSME_STOW) > t_last))
			{
				// SSME dump repo
				pATVC_SOP->SetSSMEActPos( 1, MPSDUMPCONFIG_1P, MPSDUMPCONFIG_1Y );
				pATVC_SOP->SetSSMEActPos( 2, MPSDUMPCONFIG_2P, MPSDUMPCONFIG_2Y );
				pATVC_SOP->SetSSMEActPos( 3, MPSDUMPCONFIG_3P, MPSDUMPCONFIG_3Y );
			}

			if (((t_MECO + ET_SEP_ARM_SEP_PIC) <= SimT) && ((t_MECO + ET_SEP_ARM_SEP_PIC) > t_last))
			{
				// TODO arm sep PICs
			}

			// TODO THC move stops auto sep

			if (autoETSEP == true)
			{
				// TODO check feedline valves retracted
				// TODO check MDMs FA2, FA3, FA4
				VECTOR3 angvel;
				STS()->GetAngularVel( angvel );
				angvel *= DEG;

				if ((fabs( angvel.z ) <= ET_SEP_ROLL_RATE_LIMIT) && // check rates
					(fabs( angvel.x ) <= ET_SEP_PITCH_RATE_LIMIT) && 
					(fabs( angvel.y ) <= ET_SEP_YAW_RATE_LIMIT) && 
					(PD1_CL_Ind_A.IsSet() == true) && // check feedline valves closed
					(PD1_CL_Ind_B.IsSet() == true) && 
					(PD2_CL_Ind_A.IsSet() == true) && 
					(PD2_CL_Ind_B.IsSet() == true) && 
					(PD3_CL_Ind.IsSet() == true))
				{
					ETSEPCommand = true;

					// remove power from LVs
					PD1_CL.ResetLine();
					PD2_CL.ResetLine();
					PD3_CL.ResetLine();
				}
				else
				{
					// TODO C&W msg
					ETSEPINH = true;
					//sprintf_s( oapiDebugString(), 255, "SEP INH" );
				}
			}

			if (ETSEPCommand == true)
			{
				if (timerSEP == -1)// first run
				{
					// -Z translation cmd
					pTransitionDAP->MinusZTranslationCommand();
					timerSEP = SimT + ET_SEP_DELAY_MINUSZ;
				}
				else if (timerSEP <= SimT)
				{
					// sep cmd
					STS()->SeparateTank();
					ETSEPINH = false;
					done = true;

					char buffer[64];
					sprintf_s( buffer, 64, "ET SEP @ MET %.2f", STS()->GetMET() );
					oapiWriteLog( buffer );
				}
			}
		}
		else
		{
			if (pSSME_Operations->GetMECOConfirmedFlag() == true)
			{
				t_MECO = SimT;
				active = true;
			}
		}
		return;
	}

	void ETSepSequence::Realize( void )
	{
		pSSME_Operations = static_cast<SSME_Operations*> (FindSoftware( "SSME_Operations" ));
		assert( (pSSME_Operations != NULL) && "MPS_Dump::Realize.pSSME_Operations" );
		pTransitionDAP = static_cast<TransitionDAP*> (FindSoftware( "TransitionDAP" ));
		assert( (pTransitionDAP != NULL) && "MPS_Dump::Realize.TransitionDAP" );
		pIO_Control = static_cast<IO_Control*> (FindSoftware( "IO_Control" ));
		assert( (pIO_Control != NULL) && "MPS_Dump::Realize.pIO_Control" );
		pATVC_SOP = static_cast<ATVC_SOP*> (FindSoftware( "ATVC_SOP" ));
		assert( (pATVC_SOP != NULL) && "MPS_Dump::Realize.ATVC_SOP" );


		DiscreteBundle* bundle = BundleManager()->CreateBundle( "C3_SEP", 4 );
		ETSEPSW.Connect( bundle, 2 );
		ETSEPPB.Connect( bundle, 3 );

		bundle = BundleManager()->CreateBundle( "MPS_LV_C", 16 );
		PD1_OP.Connect( bundle, 13 );
		PD1_CL.Connect( bundle, 14 );
		PD2_OP.Connect( bundle, 15 );

		bundle = BundleManager()->CreateBundle( "MPS_LV_D", 16 );
		PD2_CL.Connect( bundle, 0 );
		PD3_OP.Connect( bundle, 1 );
		PD3_CL.Connect( bundle, 2 );
		// TODO do unlock valves

		bundle = BundleManager()->CreateBundle( "MPS_CLInd_A", 16 );
		PD1_CL_Ind_A.Connect( bundle, 13 );
		PD1_CL_Ind_B.Connect( bundle, 14 );
		PD2_CL_Ind_A.Connect( bundle, 15 );

		bundle = BundleManager()->CreateBundle( "MPS_CLInd_B", 16 );
		PD2_CL_Ind_B.Connect( bundle, 0 );
		PD3_CL_Ind.Connect( bundle, 7 );
		return;
	}

	bool ETSepSequence::OnMajorModeChange( unsigned int newMajorMode )
	{
		switch (newMajorMode)
		{
			//case 102:
			case 103:
			case 104:
			case 601:
				return true;
			default:
				return false;
		}
	}

	bool ETSepSequence::GetETSEPINHFlag( void ) const
	{
		return ETSEPINH;
	}
}