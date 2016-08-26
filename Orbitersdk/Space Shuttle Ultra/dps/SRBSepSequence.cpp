#include "SRBSepSequence.h"
#include "..\Atlantis.h"
#include "SSME_Operations.h"
#include "AscentGuidance.h"
#include "MEC_SOP.h"


namespace dps
{
	SRBSepSequence::SRBSepSequence( SimpleGPCSystem *_gpc ):SimpleGPCSoftware( _gpc, "SRBSepSequence" )
	{
		PC50 = false;
		SRBSEPinit = false;
		SRBSEPCommand = false;
		SRBSEPINH = false;
		
		timePC50L = -1;
		timePC50R = -1;
		timeSRBSEPinit = -1;
		return;
	}

	SRBSepSequence::~SRBSepSequence( void )
	{
		return;
	}

	void SRBSepSequence::OnPostStep( double SimT, double DeltaT, double MJD )
	{
		if (STS()->GetMET() >= SRB_SEP_SEQUENCE_START_TIME)
		{
			if (SRBSEPCommand)
			{
				// TODO wait 4s
				pMEC_SOP->SetMECMasterResetFlag();
				SetMajorMode( 103 );
			}
			else if (SRBSEPinit == false)
			{
				if ((STS()->GetSRBChamberPressure( 0 ) < 50) && (timePC50L == -1)) timePC50L = SimT;
				if ((STS()->GetSRBChamberPressure( 1 ) < 50) && (timePC50R == -1)) timePC50R = SimT;
				
				if ((timePC50L != -1) && (timePC50R != -1) && (fabs( timePC50L - timePC50R ) <= SRB_SEP_PC50_DT))
				{
					PC50 = true;
					SRBSEPinit = true;
					timeSRBSEPinit = SimT;

					char buffer[64];
					sprintf_s( buffer, 64, "PC < 50 @ MET %.2f", STS()->GetMET() );
					oapiWriteLog( buffer );

					//sprintf_s( oapiDebugString(), 255, "PC < 50" );
				}
				else if (STS()->GetMET() >= SRB_SEP_BACKUP_CUE_T)
				{
					SRBSEPinit = true;
					timeSRBSEPinit = SimT;

					char buffer[64];
					sprintf_s( buffer, 64, "SRB timer expired" );
					oapiWriteLog( buffer );
				}
			}
			else
			{
				if ((timeSRBSEPinit + SRB_SEP_MOD_DELAY) <= SimT)
				{
					pMEC_SOP->SetSRBSepSequencerFlag( MECSOP_SRBSEP_SRB_SEP_PICS_ARM );

					// SRB nozzles to null
					pAscentGuidance->NullSRBNozzles();

					if ((timeSRBSEPinit + SRB_SEP_CMD_DELAY) <= SimT)
					{
						// TODO SRB TVC -> off
						VECTOR3 angvel;
						STS()->GetAngularVel( angvel );
						angvel *= DEG;

						if ((SRBSEPSW.IsSet() == true) && (SRBSEPPB.IsSet() == true))
						{
							SRBSEPCommand = true;

							char buffer[64];
							sprintf_s( buffer, 64, "MAN SRB SEP" );
							oapiWriteLog( buffer );
						}
						else if ((fabs( angvel.z ) < SRB_SEP_ROLL_RATE_LIMIT) && // check rates
							(fabs( angvel.x ) < SRB_SEP_PITCH_RATE_LIMIT) && 
							(fabs( angvel.y ) < SRB_SEP_YAW_RATE_LIMIT) && 
							((STS()->GetDynPressure() * 0.020885434272991) < SRB_SEP_QBAR_LIMIT))// check qbar
						{
							SRBSEPCommand = true;
						}
						else
						{
							// TODO C&W SEP INH
							SRBSEPINH = true;
							//sprintf_s( oapiDebugString(), 255, "SEP INH" );
							return;
						}

					}
				}

				if ((SRBSEPCommand == true) && (pSSME_Operations->GetMECOConfirmedFlag() == false))// no SRB SEP for 3 EO, SRBs go together with ET
				{
					// sep cmd
					pMEC_SOP->SetSRBSepSequencerFlag( MECSOP_SRBSEP_SRB_SEP_FIRE_1 );
					pMEC_SOP->SetSRBSepSequencerFlag( MECSOP_SRBSEP_SRB_SEP_FIRE_2 );
					SRBSEPINH = false;

					char buffer[64];
					sprintf_s( buffer, 64, "SRB SEP @ MET %.2f", STS()->GetMET() );
					oapiWriteLog( buffer );
				}
			}
		}
		return;
	}

	void SRBSepSequence::Realize( void )
	{
		pSSME_Operations = static_cast<SSME_Operations*> (FindSoftware( "SSME_Operations" ));
		pAscentGuidance = static_cast<AscentGuidance*> (FindSoftware( "AscentGuidance" ));
		pMEC_SOP = static_cast<MEC_SOP*> (FindSoftware( "MEC_SOP" ));

		DiscreteBundle* bundle = BundleManager()->CreateBundle( "C3_SEP", 4 );
		SRBSEPSW.Connect( bundle, 0 );
		SRBSEPPB.Connect( bundle, 1 );
		return;
	}

	bool SRBSepSequence::OnMajorModeChange( unsigned int newMajorMode )
	{
		if (newMajorMode == 102) return true;
		else return false;
	}

	bool SRBSepSequence::GetPC50Flag( void ) const
	{
		return PC50;
	}

	bool SRBSepSequence::GetSRBSEPINHFlag( void ) const
	{
		return SRBSEPINH;
	}
}