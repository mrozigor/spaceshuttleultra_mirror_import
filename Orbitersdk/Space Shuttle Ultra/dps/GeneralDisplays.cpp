#include "GeneralDisplays.h"
#include "IDP.h"
#include "AscentDAP.h"
#include "SSME_Operations.h"
#include "SRBSepSequence.h"
//#include "ETSepSequence.h"


namespace dps
{
	GeneralDisplays::GeneralDisplays( SimpleGPCSystem* _gpc ):SimpleGPCSoftware( _gpc, "GeneralDisplays" )
	{
		He_T = 0;

		for (int i = 0; i < 32; i++)
		{
			ITEM_STATE_SPEC112[i] = false;
		}

		for (int i = 0; i < 44; i++)
		{
			ITEM_STATE_SPEC113[i] = false;
		}
		return;
	}

	GeneralDisplays::~GeneralDisplays()
	{
	}

	void GeneralDisplays::Realize()
	{
		pAscentDAP = static_cast<AscentDAP*> (FindSoftware( "AscentDAP" ));
		pSSME_Operations = static_cast<SSME_Operations*> (FindSoftware( "SSME_Operations" ));
		pSRBSepSequence = static_cast<SRBSepSequence*> (FindSoftware( "SRBSepSequence" ));
		//pETSepSequence = static_cast<ETSepSequence*> (FindSoftware( "ETSepSequence" ));

		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "BFCCRT", 3 );
		dipBFCCRTDisplay.Connect( pBundle, 0 );// ON
		dipBFCCRTSelect[0].Connect( pBundle, 1 ); // 3+1
		dipBFCCRTSelect[1].Connect( pBundle, 2 ); // 1+2

		pBundle = BundleManager()->CreateBundle( "ET_LOX_SENSORS", 16 );
		dipLO2UllagePressureSensor[0].Connect( pBundle, 12 );
		dipLO2UllagePressureSensor[1].Connect( pBundle, 13 );
		dipLO2UllagePressureSensor[2].Connect( pBundle, 14 );

		pBundle = BundleManager()->CreateBundle( "ET_LH2_SENSORS", 16 );
		dipLH2UllagePressureSensor[0].Connect( pBundle, 12 );
		dipLH2UllagePressureSensor[1].Connect( pBundle, 13 );
		dipLH2UllagePressureSensor[2].Connect( pBundle, 14 );

		pBundle = BundleManager()->CreateBundle( "MPS_HE_SENSORS", 12 );
		dipHeSysPressureSensor[0].Connect( pBundle, 0 );
		dipHeSysPressureSensor[1].Connect( pBundle, 1 );
		dipHeSysPressureSensor[2].Connect( pBundle, 2 );
		dipHeSysPressureSensor[3].Connect( pBundle, 3 );
		dipHeSysPressureSensor[4].Connect( pBundle, 4 );
		dipHeSysPressureSensor[5].Connect( pBundle, 5 );
		dipHeSysPressureSensor[6].Connect( pBundle, 6 );
		dipHeSysPressureSensor[7].Connect( pBundle, 7 );
		dipHeSysPressureSensor[8].Connect( pBundle, 8 );
		dipHeSysPressureSensor[9].Connect( pBundle, 9 );
		dipHeSysPressureSensor[10].Connect( pBundle, 10 );
		dipHeSysPressureSensor[11].Connect( pBundle, 11 );

		pBundle = BundleManager()->CreateBundle( "MPS_SENSORS", 2 );
		dipMPSManifPressureSensor[0].Connect( pBundle, 0 );
		dipMPSManifPressureSensor[1].Connect( pBundle, 1 );

		pBundle = STS()->BundleManager()->CreateBundle( "LeftRHCTHC_A", 16 );
		for (int i = 0; i < 9; i++) LeftRHC[i].Connect( pBundle, i );
		for (int i = 9; i < 16; i++) LeftTHC[i - 9].Connect( pBundle, i );
		pBundle = STS()->BundleManager()->CreateBundle( "LeftRHCTHC_B", 16 );
		for (int i = 0; i < 11; i++) LeftTHC[i + 7].Connect( pBundle, i );
		pBundle = STS()->BundleManager()->CreateBundle( "RightRHC", 16 );
		for (int i = 0; i < 9; i++) RightRHC[i].Connect( pBundle, i );
		pBundle = STS()->BundleManager()->CreateBundle( "AftRHCTHC_A", 16 );
		for (int i = 0; i < 9; i++) AftRHC[i].Connect( pBundle, i );
		for (int i = 9; i < 16; i++) AftTHC[i - 9].Connect( pBundle, i );
		pBundle = STS()->BundleManager()->CreateBundle( "AftRHCTHC_B", 16 );
		for (int i = 0; i < 11; i++) AftTHC[i + 7].Connect( pBundle, i );

		// init He dP/dT calc
		He_P[0] = dipHeSysPressureSensor[0].GetVoltage() * 1000;
		He_P[1] = dipHeSysPressureSensor[1].GetVoltage() * 1000;
		He_P[2] = dipHeSysPressureSensor[2].GetVoltage() * 1000;
		return;
	}

	void GeneralDisplays::OnPreStep( double SimT, double DeltaT, double MJD )
	{
		// calculate He dP/dT (psia/3sec) for BFS DISP 18
		if ((SimT - He_T) >= 3)
		{
			double currentP;
			for (int i = 0; i < 3; i++)
			{
				currentP = dipHeSysPressureSensor[i * 3].GetVoltage() * 1000;
				He_dPdT[i] = (3 * (He_P[i] - currentP)) / (SimT - He_T);

				He_P[i] = currentP;// save press
			}

			He_T = SimT;// save time
		}
		return;
	}

	bool GeneralDisplays::OnMajorModeChange( unsigned int newMajorMode )
	{
		// always running
		return true;
	}

	bool GeneralDisplays::ItemInput( int spec, int item, const char* Data )
	{
		switch (spec)
		{
			/*case 51:
				return true;
			case 53:
				return true;
			case 55:
				return true;
			case 99:
				return true;*/
			case 112:
				ItemInput_SPEC112( item, Data );
				return true;
			case 113:
				ItemInput_SPEC113( item, Data );
				return true;
		}
		return false;
	}

	void GeneralDisplays::ItemInput_SPEC112( int item, const char* Data )
	{
		// TODO
		return;
	}

	void GeneralDisplays::ItemInput_SPEC113( int item, const char* Data )
	{
		// TODO
		return;
	}

	bool GeneralDisplays::OnPaint( int spec, vc::MDU* pMDU ) const
	{
		// HACK determine which display to use based on the position of the BFC CRT switches (BFS display only available in CRTx)
		bool outputBFSdisplay = false;

		if (pMDU->GetIdentifier() == "CRT1")
		{
			if (dipBFCCRTDisplay.IsSet() & dipBFCCRTSelect[1].IsSet()) outputBFSdisplay = true;
		}
		else if (pMDU->GetIdentifier() == "CRT2")
		{
			if (dipBFCCRTDisplay.IsSet() & !dipBFCCRTSelect[0].IsSet() & !dipBFCCRTSelect[1].IsSet()) outputBFSdisplay = true;
		}
		else if (pMDU->GetIdentifier() == "CRT3")
		{
			if (dipBFCCRTDisplay.IsSet() & dipBFCCRTSelect[0].IsSet()) outputBFSdisplay = true;
		}

		if (outputBFSdisplay == false)
		{
			// PASS
			switch (GetMajorMode() / 100)
			{
				case 0:
					switch (spec)
					{
						case 99:
							OnPaint_DISP99_PASS( pMDU );// FAULT
							return true;
						default:
							return false;
					}
				case 1:
					switch (spec)
					{
						case 18:
							OnPaint_DISP18_PASS( pMDU );// GNC SYS SUMM 1
							return true;
						case 51:
							OnPaint_SPEC51_PASS( pMDU );// OVERRIDE
							return true;
						case 53:
							OnPaint_SPEC53_PASS( pMDU );// CONTROLS
							return true;
						case 55:
							OnPaint_SPEC55_PASS( pMDU );// GPS STATUS
							return true;
						case 99:
							OnPaint_DISP99_PASS( pMDU );// FAULT
							return true;
						default:
							switch (GetMajorMode())// only PASS XXXXXX TRAJ displays for now
							{
								case 101:
									OnPaint_LAUNCHTRAJ1_PASS( pMDU );// OI-32 PASS LAUNCH TRAJ 1
									return true;
								case 102:
									OnPaint_ASCENTTRAJ1_PASS( pMDU );// OI-32 PASS ASCENT TRAJ 1
									return true;
								case 103:
									OnPaint_ASCENTTRAJ2_PASS( pMDU );// OI-32 PASS ASCENT TRAJ 2
									return true;
								default:
									return false;
							}
					}
				case 2:
					switch (spec)
					{
						case 18:
							OnPaint_DISP18_PASS( pMDU );// GNC SYS SUMM 1
							return true;
						case 19:
							OnPaint_DISP19_PASS( pMDU );// GNC SYS SUMM 2
							return true;
						case 25:
							OnPaint_SPEC25_PASS( pMDU );// RM ORBIT
							return true;
						case 55:
							OnPaint_SPEC55_PASS( pMDU );// GPS STATUS
							return true;
						case 99:
							OnPaint_DISP99_PASS( pMDU );// FAULT
							return true;
						default:
							return false;
					}
				case 3:
					switch (spec)
					{
						case 18:
							OnPaint_DISP18_PASS( pMDU );// GNC SYS SUMM 1
							return true;
						case 51:
							OnPaint_SPEC51_PASS( pMDU );// OVERRIDE
							return true;
						case 53:
							OnPaint_SPEC53_PASS( pMDU );// CONTROLS
							return true;
						case 55:
							OnPaint_SPEC55_PASS( pMDU );// GPS STATUS
							return true;
						case 99:
							OnPaint_DISP99_PASS( pMDU );// FAULT
							return true;
						default:
							return false;
					}
				case 6:
					switch (spec)
					{
						case 18:
							OnPaint_DISP18_PASS( pMDU );// GNC SYS SUMM 1
							return true;
						case 51:
							OnPaint_SPEC51_PASS( pMDU );// OVERRIDE
							return true;
						case 53:
							OnPaint_SPEC53_PASS( pMDU );// CONTROLS
							return true;
						case 55:
							OnPaint_SPEC55_PASS( pMDU );// GPS STATUS
							return true;
						case 99:
							OnPaint_DISP99_PASS( pMDU );// FAULT
							return true;
						default:
							return false;
					}
				case 8:
					switch (spec)
					{
						case 18:
							OnPaint_DISP18_PASS( pMDU );// GNC SYS SUMM 1
							return true;
						case 19:
							OnPaint_DISP19_PASS( pMDU );// GNC SYS SUMM 2
							return true;
						case 43:
							OnPaint_SPEC43_PASS( pMDU );// CONTROLLERS
							return true;
						case 55:
							OnPaint_SPEC55_PASS( pMDU );// GPS STATUS
							return true;
						case 99:
							OnPaint_DISP99_PASS( pMDU );// FAULT
							return true;
						default:
							return false;
					}
				case 9:
					switch (spec)
					{
						case 55:
							OnPaint_SPEC55_PASS( pMDU );// GPS STATUS
							return true;
						case 99:
							OnPaint_DISP99_PASS( pMDU );// FAULT
							return true;
						case 112:
							OnPaint_SPEC112_PASS( pMDU );// GPC/BTU I/F
							return true;
						case 113:
							OnPaint_SPEC113_PASS( pMDU );// ACTUATOR CONTROL
							return true;
						default:
							return false;
					}
				default:
					return false;
			}
		}
		else
		{
			// BFS
			switch (GetMajorMode() / 100)
			{
				case 0:
					switch (spec)
					{
						case 99:
							OnPaint_DISP99_BFS( pMDU );// FAULT
							return true;
						default:
							return false;
					}
				case 1:
					switch (spec)
					{
						case 18:
							OnPaint_DISP18_BFS( pMDU );// GNC SYS SUMM 1
							return true;
						case 19:
							OnPaint_DISP19_BFS( pMDU );// GNC SYS SUMM 2
							return true;
						case 51:
							OnPaint_SPEC51_BFS( pMDU );// OVERRIDE
							return true;
						case 55:
							OnPaint_SPEC55_BFS( pMDU );// GPS STATUS
							return true;
						case 99:
							OnPaint_DISP99_BFS( pMDU );// FAULT
							return true;
						default:
							return false;
					}
				case 3:
					switch (spec)
					{
						case 18:
							OnPaint_DISP18_BFS( pMDU );// GNC SYS SUMM 1
							return true;
						case 19:
							OnPaint_DISP19_BFS( pMDU );// GNC SYS SUMM 2
							return true;
						case 51:
							OnPaint_SPEC51_BFS( pMDU );// OVERRIDE
							return true;
						case 55:
							OnPaint_SPEC55_BFS( pMDU );// GPS STATUS
							return true;
						case 99:
							OnPaint_DISP99_BFS( pMDU );// FAULT
							return true;
						default:
							return false;
					}
				case 6:
					switch (spec)
					{
						case 18:
							OnPaint_DISP18_BFS( pMDU );// GNC SYS SUMM 1
							return true;
						case 19:
							OnPaint_DISP19_BFS( pMDU );// GNC SYS SUMM 2
							return true;
						case 51:
							OnPaint_SPEC51_BFS( pMDU );// OVERRIDE
							return true;
						case 55:
							OnPaint_SPEC55_BFS( pMDU );// GPS STATUS
							return true;
						case 99:
							OnPaint_DISP99_BFS( pMDU );// FAULT
							return true;
						default:
							return false;
					}
				default:
					return false;
			}
		}
	}

	void GeneralDisplays::OnPaint_DISP18_PASS( vc::MDU* pMDU ) const
	{
		PrintCommonHeader( " GNC SYS SUMM 1", pMDU );

		// static parts (labels)
		// RCS
		pMDU->mvprint( 0, 2, "RCS  JETISOL" );
		pMDU->mvprint( 0, 3, "MANFFAILVLV" );
		pMDU->mvprint( 2, 4, "F1" );
		pMDU->mvprint( 3, 5, "2" );
		pMDU->mvprint( 3, 6, "3" );
		pMDU->mvprint( 3, 7, "4" );
		pMDU->mvprint( 3, 8, "5" );
		pMDU->mvprint( 2, 9, "L1" );
		pMDU->mvprint( 3, 10, "2" );
		pMDU->mvprint( 3, 11, "3" );
		pMDU->mvprint( 3, 12, "4" );
		pMDU->mvprint( 3, 13, "5" );
		pMDU->mvprint( 2, 14, "R1" );
		pMDU->mvprint( 3, 15, "2" );
		pMDU->mvprint( 3, 16, "3" );
		pMDU->mvprint( 3, 17, "4" );
		pMDU->mvprint( 3, 18, "5" );

		// SURF
		pMDU->mvprint( 14, 3, "SURF     POS   MOM" );
		pMDU->mvprint( 14, 4, "L OB" );
		pMDU->mvprint( 16, 5, "IB" );
		pMDU->mvprint( 14, 6, "R IB" );
		pMDU->mvprint( 16, 7, "OB" );
		pMDU->mvprint( 14, 8, "AIL" );
		pMDU->mvprint( 14, 9, "RUD" );
		pMDU->mvprint( 14, 10, "SPD BRK" );
		pMDU->mvprint( 14, 11, "BDY FLP" );

		// DPS
		pMDU->mvprint( 34, 3, "DPS    1 2 3 4 5" );
		pMDU->mvprint( 37, 4, "GPC" );
		pMDU->mvprint( 34, 5, "MDM FF" );
		pMDU->mvprint( 38, 6, "FA" );

		// FCS
		pMDU->mvprint( 34, 9, "FCS CH 1 2 3 4" );

		// NAV
		pMDU->mvprint( 34, 13, "NAV    1 2 3 4" );
		pMDU->mvprint( 36, 14, "IMU" );
		pMDU->mvprint( 36, 15, "ACC" );
		pMDU->mvprint( 36, 16, "RGA" );
		pMDU->mvprint( 36, 17, "TAC" );
		pMDU->mvprint( 36, 18, "MLS" );
		pMDU->mvprint( 36, 19, "ADTA" );

		// CNTLR
		pMDU->mvprint( 16, 15, "CNTLR  1 2 3" );
		pMDU->mvprint( 16, 16, "RHC  L" );
		pMDU->mvprint( 21, 17, "R" );
		pMDU->mvprint( 21, 18, "A" );
		pMDU->mvprint( 16, 19, "THC  L" );
		pMDU->mvprint( 21, 20, "A" );
		pMDU->mvprint( 16, 21, "SBTC L" );
		pMDU->mvprint( 21, 22, "R" );


		// static parts (lines)
		// RCS
		pMDU->Line( 40, 14, 40, 266 );
		pMDU->Line( 80, 14, 80, 266 );
		pMDU->Line( 0, 56, 120, 56 );
		pMDU->Line( 0, 126, 120, 126 );
		pMDU->Line( 0, 196, 120, 196 );

		// SURF
		pMDU->Line( 140, 56, 320, 56 );

		// DPS
		pMDU->Line( 340, 56, 500, 56 );

		// FCS
		pMDU->Line( 340, 140, 480, 140 );

		// NAV
		pMDU->Line( 340, 196, 480, 196 );

		// CNTRL
		pMDU->Line( 160, 224, 280, 224 );


		// dynamic parts
		// TODO finish
		char cbuf[64];
		double tmp = 0;
		char pos;

		// RCS

		// SURF
		if (STS()->aerosurfaces.leftElevon > 0) pos = 'D';
		else if (STS()->aerosurfaces.leftElevon < 0) pos = 'U';
		else pos = ' ';
		sprintf_s( cbuf, 64, "%c%4.1f  %2.0f", pos, fabs( STS()->aerosurfaces.leftElevon ), tmp );
		pMDU->mvprint( 22, 4, cbuf );

		if (STS()->aerosurfaces.leftElevon > 0) pos = 'D';
		else if (STS()->aerosurfaces.leftElevon < 0) pos = 'U';
		else pos = ' ';
		sprintf_s( cbuf, 64, "%c%4.1f  %2.0f", pos, fabs( STS()->aerosurfaces.leftElevon ), tmp );
		pMDU->mvprint( 22, 5, cbuf );

		if (STS()->aerosurfaces.rightElevon > 0) pos = 'D';
		else if (STS()->aerosurfaces.rightElevon < 0) pos = 'U';
		else pos = ' ';
		sprintf_s( cbuf, 64, "%c%4.1f  %2.0f", pos, fabs( STS()->aerosurfaces.rightElevon ), tmp );
		pMDU->mvprint( 22, 6, cbuf );

		if (STS()->aerosurfaces.rightElevon > 0) pos = 'D';
		else if (STS()->aerosurfaces.rightElevon < 0) pos = 'U';
		else pos = ' ';
		sprintf_s( cbuf, 64, "%c%4.1f  %2.0f", pos, fabs( STS()->aerosurfaces.rightElevon ), tmp );
		pMDU->mvprint( 22, 7, cbuf );

		tmp = (STS()->aerosurfaces.leftElevon - STS()->aerosurfaces.rightElevon) * 0.5;
		if (tmp > 0) pos = 'R';
		else if (tmp < 0) pos = 'L';
		else pos = ' ';
		sprintf_s( cbuf, 64, "%c%4.1f", pos, fabs( tmp ) );
		pMDU->mvprint( 22, 8, cbuf );

		tmp = STS()->aerosurfaces.rudder;
		if (tmp > 0) pos = 'R';
		else if (tmp < 0) pos = 'L';
		else pos = ' ';
		sprintf_s( cbuf, 64, "%c%4.1f", pos, fabs( tmp ) );
		pMDU->mvprint( 22, 9, cbuf );

		sprintf_s( cbuf, 64, "%5.1f", fabs( STS()->aerosurfaces.speedbrake ) );
		pMDU->mvprint( 22, 10, cbuf );

		sprintf_s( cbuf, 64, "%5.1f", fabs( STS()->aerosurfaces.bodyFlap ) );
		pMDU->mvprint( 22, 11, cbuf );

		// DPS

		// FCS

		// NAV

		// CNTLR
		return;
	}

	void GeneralDisplays::OnPaint_DISP19_PASS( vc::MDU* pMDU ) const
	{
		PrintCommonHeader( " GNC SYS SUMM 2", pMDU );

		// static parts (labels)
		// OMS
		pMDU->mvprint( 0, 2, "OMS AFT QTY    L     R" );
		pMDU->mvprint( 8, 3, "OXID" );
		pMDU->mvprint( 10, 4, "FU" );
		pMDU->mvprint( 29, 2, "OMS" );
		pMDU->mvprint( 42, 2, "L" );
		pMDU->mvprint( 48, 2, "R" );
		pMDU->mvprint( 30, 3, "TK P   HE" );
		pMDU->mvprint( 35, 4, "OXID" );
		pMDU->mvprint( 37, 5, "FU" );
		pMDU->mvprint( 30, 6, "N2 TK   P" );
		pMDU->mvprint( 33, 7, "REG  P" );
		pMDU->mvprint( 33, 8, "P  VLV" );
		pMDU->mvprint( 29, 9, "ENG IN   P" );
		pMDU->mvprint( 35, 10, "OXID" );
		pMDU->mvprint( 37, 11, "FU" );
		pMDU->mvprint( 34, 12, "VLV 1" );
		pMDU->mvprint( 38, 13, "2" );

		// RCS
		pMDU->mvprint( 20, 6, "JETISOL" );
		pMDU->mvprint( 0, 7, "RCS       OXID  FU FAIL VLV" );
		pMDU->mvprint( 0, 8, "FWD  HE P" );
		pMDU->mvprint( 5, 9, "TK P" );
		pMDU->mvprint( 6, 10, "QTY" );
		pMDU->mvprint( 0, 11, "MANF  1 P" );
		pMDU->mvprint( 6, 12, "2 P" );
		pMDU->mvprint( 6, 13, "3 P" );
		pMDU->mvprint( 6, 14, "4 P" );
		pMDU->mvprint( 6, 15, "5" );
		pMDU->mvprint( 0, 16, "AFT  HE P" );
		pMDU->mvprint( 0, 17, "L    TK P" );
		pMDU->mvprint( 6, 18, "QTY" );
		pMDU->mvprint( 0, 19, "MANF  1 P" );
		pMDU->mvprint( 6, 20, "2 P" );
		pMDU->mvprint( 6, 21, "3 P" );
		pMDU->mvprint( 6, 22, "4 P" );
		pMDU->mvprint( 6, 23, "5" );
		pMDU->mvprint( 44, 14, "JETISOL" );
		pMDU->mvprint( 34, 15, "OXID  FU FAIL VLV" );
		pMDU->mvprint( 29, 16, "HE P" );
		pMDU->mvprint( 27, 17, "R TK P" );
		pMDU->mvprint( 30, 18, "QTY" );
		pMDU->mvprint( 30, 19, "1 P" );
		pMDU->mvprint( 30, 20, "2 P" );
		pMDU->mvprint( 30, 21, "3 P" );
		pMDU->mvprint( 30, 22, "4 P" );
		pMDU->mvprint( 30, 23, "5" );


		// static parts (lines)
		pMDU->Line( 90, 84, 90, 336 );
		pMDU->Line( 140, 84, 140, 336 );
		pMDU->Line( 190, 84, 190, 336 );
		pMDU->Line( 230, 84, 230, 336 );
		pMDU->Line( 270, 84, 270, 336 );
		pMDU->Line( 330, 196, 330, 336 );
		pMDU->Line( 380, 196, 380, 336 );
		pMDU->Line( 430, 196, 430, 336 );
		pMDU->Line( 470, 196, 470, 336 );
		pMDU->Line( 0, 84, 270, 84 );
		pMDU->Line( 0, 112, 270, 112 );
		pMDU->Line( 60, 154, 190, 154 );
		pMDU->Line( 270, 196, 510, 196 );
		pMDU->Line( 0, 224, 510, 224 );
		pMDU->Line( 60, 266, 190, 266 );
		pMDU->Line( 300, 266, 430, 266 );


		// TODO dynamic parts
		return;
	}

	void GeneralDisplays::OnPaint_SPEC25_PASS( vc::MDU* pMDU ) const
	{
		PrintCommonHeader( "   RM ORBIT", pMDU );

		// static parts (labels)
		// THC
		pMDU->mvprint( 3, 6, "THC TX TY TZ DES" );
		pMDU->mvprint( 3, 8, "L 1" );
		pMDU->mvprint( 17, 8, "1" );
		pMDU->mvprint( 5, 9, "2" );
		pMDU->mvprint( 17, 9, "2" );
		pMDU->mvprint( 5, 10, "3" );
		pMDU->mvprint( 17, 10, "3" );
		pMDU->mvprint( 3, 12, "A 1" );
		pMDU->mvprint( 17, 12, "4" );
		pMDU->mvprint( 5, 13, "2" );
		pMDU->mvprint( 17, 13, "5" );
		pMDU->mvprint( 5, 14, "3" );
		pMDU->mvprint( 17, 14, "6" );

		// RHC
		pMDU->mvprint( 23, 6, "RHC  R    P    Y   DES" );
		pMDU->mvprint( 23, 8, "L 1" );
		pMDU->mvprint( 43, 8, "7" );
		pMDU->mvprint( 25, 9, "2" );
		pMDU->mvprint( 43, 9, "8" );
		pMDU->mvprint( 25, 10, "3" );
		pMDU->mvprint( 43, 10, "9" );
		pMDU->mvprint( 23, 12, "R 1" );
		pMDU->mvprint( 42, 12, "10" );
		pMDU->mvprint( 25, 13, "2" );
		pMDU->mvprint( 42, 13, "11" );
		pMDU->mvprint( 25, 14, "3" );
		pMDU->mvprint( 42, 14, "12" );
		pMDU->mvprint( 23, 16, "A 1" );
		pMDU->mvprint( 42, 16, "13" );
		pMDU->mvprint( 25, 17, "2" );
		pMDU->mvprint( 42, 17, "14" );
		pMDU->mvprint( 25, 18, "3" );
		pMDU->mvprint( 42, 18, "15" );

		pMDU->mvprint( 23, 23, "SW RM INH 16" );

		// dynamic parts
		SPEC25_SPEC43_printTHC( pMDU, LeftTHC[0].GetVoltage(), LeftTHC[3].GetVoltage(), 7, 8 );
		SPEC25_SPEC43_printTHC( pMDU, LeftTHC[1].GetVoltage(), LeftTHC[4].GetVoltage(), 7, 9 );
		SPEC25_SPEC43_printTHC( pMDU, LeftTHC[2].GetVoltage(), LeftTHC[5].GetVoltage(), 7, 10 );
		SPEC25_SPEC43_printTHC( pMDU, LeftTHC[6].GetVoltage(), LeftTHC[9].GetVoltage(), 10, 8 );
		SPEC25_SPEC43_printTHC( pMDU, LeftTHC[7].GetVoltage(), LeftTHC[10].GetVoltage(), 10, 9 );
		SPEC25_SPEC43_printTHC( pMDU, LeftTHC[8].GetVoltage(), LeftTHC[11].GetVoltage(), 10, 10 );
		SPEC25_SPEC43_printTHC( pMDU, LeftTHC[12].GetVoltage(), LeftTHC[15].GetVoltage(), 13, 8 );
		SPEC25_SPEC43_printTHC( pMDU, LeftTHC[13].GetVoltage(), LeftTHC[16].GetVoltage(), 13, 9 );
		SPEC25_SPEC43_printTHC( pMDU, LeftTHC[14].GetVoltage(), LeftTHC[17].GetVoltage(), 13, 10 );

		SPEC25_SPEC43_printTHC( pMDU, AftTHC[0].GetVoltage(), AftTHC[3].GetVoltage(), 7, 12 );
		SPEC25_SPEC43_printTHC( pMDU, AftTHC[1].GetVoltage(), AftTHC[4].GetVoltage(), 7, 13 );
		SPEC25_SPEC43_printTHC( pMDU, AftTHC[2].GetVoltage(), AftTHC[5].GetVoltage(), 7, 14 );
		SPEC25_SPEC43_printTHC( pMDU, AftTHC[6].GetVoltage(), AftTHC[9].GetVoltage(), 10, 12 );
		SPEC25_SPEC43_printTHC( pMDU, AftTHC[7].GetVoltage(), AftTHC[10].GetVoltage(), 10, 13 );
		SPEC25_SPEC43_printTHC( pMDU, AftTHC[8].GetVoltage(), AftTHC[11].GetVoltage(), 10, 14 );
		SPEC25_SPEC43_printTHC( pMDU, AftTHC[12].GetVoltage(), AftTHC[15].GetVoltage(), 13, 12 );
		SPEC25_SPEC43_printTHC( pMDU, AftTHC[13].GetVoltage(), AftTHC[16].GetVoltage(), 13, 13 );
		SPEC25_SPEC43_printTHC( pMDU, AftTHC[14].GetVoltage(), AftTHC[17].GetVoltage(), 13, 14 );

		SPEC25_SPEC43_printRHC_RY( pMDU, LeftRHC[3].GetVoltage(), 27, 8 );
		SPEC25_SPEC43_printRHC_RY( pMDU, LeftRHC[4].GetVoltage(), 27, 9 );
		SPEC25_SPEC43_printRHC_RY( pMDU, LeftRHC[5].GetVoltage(), 27, 10 );
		SPEC25_SPEC43_printRHC_P( pMDU, LeftRHC[0].GetVoltage(), 32, 8 );
		SPEC25_SPEC43_printRHC_P( pMDU, LeftRHC[1].GetVoltage(), 32, 9 );
		SPEC25_SPEC43_printRHC_P( pMDU, LeftRHC[2].GetVoltage(), 32, 10 );
		SPEC25_SPEC43_printRHC_RY( pMDU, LeftRHC[6].GetVoltage(), 37, 8 );
		SPEC25_SPEC43_printRHC_RY( pMDU, LeftRHC[7].GetVoltage(), 37, 9 );
		SPEC25_SPEC43_printRHC_RY( pMDU, LeftRHC[8].GetVoltage(), 37, 10 );

		SPEC25_SPEC43_printRHC_RY( pMDU, RightRHC[3].GetVoltage(), 27, 12 );
		SPEC25_SPEC43_printRHC_RY( pMDU, RightRHC[4].GetVoltage(), 27, 13 );
		SPEC25_SPEC43_printRHC_RY( pMDU, RightRHC[5].GetVoltage(), 27, 14 );
		SPEC25_SPEC43_printRHC_P( pMDU, RightRHC[0].GetVoltage(), 32, 12 );
		SPEC25_SPEC43_printRHC_P( pMDU, RightRHC[1].GetVoltage(), 32, 13 );
		SPEC25_SPEC43_printRHC_P( pMDU, RightRHC[2].GetVoltage(), 32, 14 );
		SPEC25_SPEC43_printRHC_RY( pMDU, RightRHC[6].GetVoltage(), 37, 12 );
		SPEC25_SPEC43_printRHC_RY( pMDU, RightRHC[7].GetVoltage(), 37, 13 );
		SPEC25_SPEC43_printRHC_RY( pMDU, RightRHC[8].GetVoltage(), 37, 14 );

		SPEC25_SPEC43_printRHC_RY( pMDU, AftRHC[3].GetVoltage(), 27, 16 );
		SPEC25_SPEC43_printRHC_RY( pMDU, AftRHC[4].GetVoltage(), 27, 17 );
		SPEC25_SPEC43_printRHC_RY( pMDU, AftRHC[5].GetVoltage(), 27, 18 );
		SPEC25_SPEC43_printRHC_P( pMDU, AftRHC[0].GetVoltage(), 32, 16 );
		SPEC25_SPEC43_printRHC_P( pMDU, AftRHC[1].GetVoltage(), 32, 17 );
		SPEC25_SPEC43_printRHC_P( pMDU, AftRHC[2].GetVoltage(), 32, 18 );
		SPEC25_SPEC43_printRHC_RY( pMDU, AftRHC[6].GetVoltage(), 37, 16 );
		SPEC25_SPEC43_printRHC_RY( pMDU, AftRHC[7].GetVoltage(), 37, 17 );
		SPEC25_SPEC43_printRHC_RY( pMDU, AftRHC[8].GetVoltage(), 37, 18 );
		return;
	}

	void GeneralDisplays::SPEC25_SPEC43_printTHC( vc::MDU* pMDU, double axis_plus, double axis_minus, int x, int y ) const
	{
		char ctmp[2];
		ctmp[0] = ' ';
		ctmp[1] = 0;
		if ((axis_plus == 1.0) && (axis_minus == 0.0)) ctmp[0] = '+';
		else if ((axis_plus == 0.0) && (axis_minus == 1.0)) ctmp[0] = '-';
		pMDU->mvprint( x, y, ctmp );
		return;
	}

	void GeneralDisplays::SPEC25_SPEC43_printRHC_P( vc::MDU* pMDU, double val, int x, int y ) const
	{
		char cbuf[16];
		int itmp = 0;
		char ctmp = 0;

		itmp = min( (int)fabs( val * 100 ), 99 );
		if (val > 0) ctmp = 'U';
		else if (val < 0) ctmp = 'D';
		else ctmp = ' ';
		sprintf_s( cbuf, 16, "%c%02d", ctmp, itmp );
		pMDU->mvprint( x, y, cbuf );
		return;
	}

	void GeneralDisplays::SPEC25_SPEC43_printRHC_RY( vc::MDU* pMDU, double val, int x, int y ) const
	{
		char cbuf[16];
		int itmp = 0;
		char ctmp = 0;

		itmp = min( (int)fabs( val * 100 ), 99 );
		if (val > 0) ctmp = 'R';
		else if (val < 0) ctmp = 'L';
		else ctmp = ' ';
		sprintf_s( cbuf, 16, "%c%02d", ctmp, itmp );
		pMDU->mvprint( x, y, cbuf );
		return;
	}

	void GeneralDisplays::OnPaint_SPEC43_PASS( vc::MDU* pMDU ) const
	{
		PrintCommonHeader( "  CONTROLLERS", pMDU );

		// static parts (labels)
		pMDU->mvprint( 6, 3, "TXTYTZDES" );
		pMDU->mvprint( 5, 4, "1" );
		pMDU->mvprint( 13, 4, "1" );
		pMDU->mvprint( 4, 5, "L2" );
		pMDU->mvprint( 13, 5, "2" );
		pMDU->mvprint( 0, 6, "THC  3" );
		pMDU->mvprint( 13, 6, "3" );
		pMDU->mvprint( 5, 7, "1" );
		pMDU->mvprint( 13, 7, "4" );
		pMDU->mvprint( 4, 8, "A2" );
		pMDU->mvprint( 13, 8, "5" );
		pMDU->mvprint( 5, 9, "3" );
		pMDU->mvprint( 13, 9, "6" );
		pMDU->mvprint( 5, 10, "1" );
		pMDU->mvprint( 13, 10, "7" );
		pMDU->mvprint( 4, 11, "L2" );
		pMDU->mvprint( 13, 11, "8" );
		pMDU->mvprint( 5, 12, "3" );
		pMDU->mvprint( 13, 12, "9" );
		pMDU->mvprint( 0, 13, "SPD  1" );
		pMDU->mvprint( 12, 13, "10" );
		pMDU->mvprint( 0, 14, "BK  R2" );
		pMDU->mvprint( 12, 14, "11" );
		pMDU->mvprint( 5, 15, "3" );
		pMDU->mvprint( 12, 15, "12" );
		pMDU->mvprint( 5, 16, "1" );
		pMDU->mvprint( 12, 16, "13" );
		pMDU->mvprint( 4, 17, "L2" );
		pMDU->mvprint( 12, 17, "14" );
		pMDU->mvprint( 0, 18, "RUD  3" );
		pMDU->mvprint( 12, 18, "15" );
		pMDU->mvprint( 0, 19, "PED  1" );
		pMDU->mvprint( 12, 19, "16" );
		pMDU->mvprint( 4, 20, "R2" );
		pMDU->mvprint( 12, 20, "17" );
		pMDU->mvprint( 5, 21, "3" );
		pMDU->mvprint( 12, 21, "18" );

		pMDU->mvprint( 19, 6, "BDY FLP" );
		pMDU->mvprint( 21, 7, "UPDNDES" );
		pMDU->mvprint( 19, 8, "L1" );
		pMDU->mvprint( 25, 8, "19" );
		pMDU->mvprint( 16, 9, "SW  2" );
		pMDU->mvprint( 25, 9, "20" );
		pMDU->mvprint( 19, 10, "R1" );
		pMDU->mvprint( 25, 10, "21" );
		pMDU->mvprint( 20, 11, "2" );
		pMDU->mvprint( 25, 11, "22" );

		pMDU->mvprint( 37, 3, "R" );
		pMDU->mvprint( 41, 3, "P" );
		pMDU->mvprint( 45, 3, "Y  DES" );
		pMDU->mvprint( 35, 4, "1" );
		pMDU->mvprint( 48, 4, "23" );
		pMDU->mvprint( 34, 5, "L2" );
		pMDU->mvprint( 48, 5, "24" );
		pMDU->mvprint( 35, 6, "3" );
		pMDU->mvprint( 48, 6, "25" );
		pMDU->mvprint( 35, 7, "1" );
		pMDU->mvprint( 48, 7, "26" );
		pMDU->mvprint( 29, 8, "RHC  R2" );
		pMDU->mvprint( 48, 8, "27" );
		pMDU->mvprint( 35, 9, "3" );
		pMDU->mvprint( 48, 9, "28" );
		pMDU->mvprint( 35, 10, "1" );
		pMDU->mvprint( 48, 10, "29" );
		pMDU->mvprint( 34, 11, "A2" );
		pMDU->mvprint( 48, 11, "30" );
		pMDU->mvprint( 35, 12, "3" );
		pMDU->mvprint( 48, 12, "31" );
		pMDU->mvprint( 34, 13, "L1" );
		pMDU->mvprint( 48, 13, "32" );
		pMDU->mvprint( 29, 14, "RHC   2" );
		pMDU->mvprint( 48, 14, "33" );
		pMDU->mvprint( 29, 15, "TRIM R1" );
		pMDU->mvprint( 48, 15, "34" );
		pMDU->mvprint( 35, 16, "2" );
		pMDU->mvprint( 48, 16, "35" );
		pMDU->mvprint( 34, 17, "L1" );
		pMDU->mvprint( 48, 17, "36" );
		pMDU->mvprint( 29, 18, "PNL   2" );
		pMDU->mvprint( 48, 18, "37" );
		pMDU->mvprint( 29, 19, "TRIM R1" );
		pMDU->mvprint( 48, 19, "38" );
		pMDU->mvprint( 35, 20, "2" );
		pMDU->mvprint( 48, 20, "39" );

		// static parts (lines)
		pMDU->Line( 0, 56, 150, 56 );
		pMDU->Line( 40, 98, 150, 98 );
		pMDU->Line( 0, 140, 150, 140 );
		pMDU->Line( 40, 182, 150, 182 );
		pMDU->Line( 0, 224, 150, 224 );
		pMDU->Line( 40, 266, 150, 266 );
		pMDU->Line( 60, 42, 60, 308 );
		pMDU->Line( 80, 42, 80, 140 );
		pMDU->Line( 100, 42, 100, 140 );
		pMDU->Line( 120, 42, 120, 308 );

		pMDU->Line( 160, 112, 280, 112 );
		pMDU->Line( 190, 140, 280, 140 );
		pMDU->Line( 210, 98, 210, 168 );
		pMDU->Line( 230, 98, 230, 168 );
		pMDU->Line( 250, 98, 250, 168 );

		pMDU->Line( 290, 56, 510, 56 );
		pMDU->Line( 350, 98, 510, 98 );
		pMDU->Line( 340, 140, 510, 140 );
		pMDU->Line( 290, 182, 510, 182 );
		pMDU->Line( 340, 210, 510, 210 );
		pMDU->Line( 290, 238, 510, 238 );
		pMDU->Line( 340, 266, 510, 266 );
		pMDU->Line( 360, 42, 360, 294 );
		pMDU->Line( 400, 42, 400, 294 );
		pMDU->Line( 440, 42, 440, 294 );
		pMDU->Line( 480, 42, 480, 294 );

		// dynamic parts
		SPEC25_SPEC43_printTHC( pMDU, LeftTHC[0].GetVoltage(), LeftTHC[3].GetVoltage(), 6, 4 );
		SPEC25_SPEC43_printTHC( pMDU, LeftTHC[1].GetVoltage(), LeftTHC[4].GetVoltage(), 6, 5 );
		SPEC25_SPEC43_printTHC( pMDU, LeftTHC[2].GetVoltage(), LeftTHC[5].GetVoltage(), 6, 6 );
		SPEC25_SPEC43_printTHC( pMDU, LeftTHC[6].GetVoltage(), LeftTHC[9].GetVoltage(), 8, 4 );
		SPEC25_SPEC43_printTHC( pMDU, LeftTHC[7].GetVoltage(), LeftTHC[10].GetVoltage(), 8, 5 );
		SPEC25_SPEC43_printTHC( pMDU, LeftTHC[8].GetVoltage(), LeftTHC[11].GetVoltage(), 8, 6 );
		SPEC25_SPEC43_printTHC( pMDU, LeftTHC[12].GetVoltage(), LeftTHC[15].GetVoltage(), 10, 4 );
		SPEC25_SPEC43_printTHC( pMDU, LeftTHC[13].GetVoltage(), LeftTHC[16].GetVoltage(), 10, 5 );
		SPEC25_SPEC43_printTHC( pMDU, LeftTHC[14].GetVoltage(), LeftTHC[17].GetVoltage(), 10, 6 );

		SPEC25_SPEC43_printTHC( pMDU, AftTHC[0].GetVoltage(), AftTHC[3].GetVoltage(), 6, 7 );
		SPEC25_SPEC43_printTHC( pMDU, AftTHC[1].GetVoltage(), AftTHC[4].GetVoltage(), 6, 8 );
		SPEC25_SPEC43_printTHC( pMDU, AftTHC[2].GetVoltage(), AftTHC[5].GetVoltage(), 6, 9 );
		SPEC25_SPEC43_printTHC( pMDU, AftTHC[6].GetVoltage(), AftTHC[9].GetVoltage(), 8, 7 );
		SPEC25_SPEC43_printTHC( pMDU, AftTHC[7].GetVoltage(), AftTHC[10].GetVoltage(), 8, 8 );
		SPEC25_SPEC43_printTHC( pMDU, AftTHC[8].GetVoltage(), AftTHC[11].GetVoltage(), 8, 9 );
		SPEC25_SPEC43_printTHC( pMDU, AftTHC[12].GetVoltage(), AftTHC[15].GetVoltage(), 10, 7 );
		SPEC25_SPEC43_printTHC( pMDU, AftTHC[13].GetVoltage(), AftTHC[16].GetVoltage(), 10, 8 );
		SPEC25_SPEC43_printTHC( pMDU, AftTHC[14].GetVoltage(), AftTHC[17].GetVoltage(), 10, 9 );

		SPEC25_SPEC43_printRHC_RY( pMDU, LeftRHC[3].GetVoltage(), 36, 4 );
		SPEC25_SPEC43_printRHC_RY( pMDU, LeftRHC[4].GetVoltage(), 36, 5 );
		SPEC25_SPEC43_printRHC_RY( pMDU, LeftRHC[5].GetVoltage(), 36, 6 );
		SPEC25_SPEC43_printRHC_P( pMDU, LeftRHC[0].GetVoltage(), 40, 4 );
		SPEC25_SPEC43_printRHC_P( pMDU, LeftRHC[1].GetVoltage(), 40, 5 );
		SPEC25_SPEC43_printRHC_P( pMDU, LeftRHC[2].GetVoltage(), 40, 6 );
		SPEC25_SPEC43_printRHC_RY( pMDU, LeftRHC[6].GetVoltage(), 44, 4 );
		SPEC25_SPEC43_printRHC_RY( pMDU, LeftRHC[7].GetVoltage(), 44, 5 );
		SPEC25_SPEC43_printRHC_RY( pMDU, LeftRHC[8].GetVoltage(), 44, 6 );

		SPEC25_SPEC43_printRHC_RY( pMDU, RightRHC[3].GetVoltage(), 36, 7 );
		SPEC25_SPEC43_printRHC_RY( pMDU, RightRHC[4].GetVoltage(), 36, 8 );
		SPEC25_SPEC43_printRHC_RY( pMDU, RightRHC[5].GetVoltage(), 36, 9 );
		SPEC25_SPEC43_printRHC_P( pMDU, RightRHC[0].GetVoltage(), 40, 7 );
		SPEC25_SPEC43_printRHC_P( pMDU, RightRHC[1].GetVoltage(), 40, 8 );
		SPEC25_SPEC43_printRHC_P( pMDU, RightRHC[2].GetVoltage(), 40, 9 );
		SPEC25_SPEC43_printRHC_RY( pMDU, RightRHC[6].GetVoltage(), 44, 7 );
		SPEC25_SPEC43_printRHC_RY( pMDU, RightRHC[7].GetVoltage(), 44, 8 );
		SPEC25_SPEC43_printRHC_RY( pMDU, RightRHC[8].GetVoltage(), 44, 9 );

		SPEC25_SPEC43_printRHC_RY( pMDU, AftRHC[3].GetVoltage(), 36, 10 );
		SPEC25_SPEC43_printRHC_RY( pMDU, AftRHC[4].GetVoltage(), 36, 11 );
		SPEC25_SPEC43_printRHC_RY( pMDU, AftRHC[5].GetVoltage(), 36, 12 );
		SPEC25_SPEC43_printRHC_P( pMDU, AftRHC[0].GetVoltage(), 40, 10 );
		SPEC25_SPEC43_printRHC_P( pMDU, AftRHC[1].GetVoltage(), 40, 11 );
		SPEC25_SPEC43_printRHC_P( pMDU, AftRHC[2].GetVoltage(), 40, 12 );
		SPEC25_SPEC43_printRHC_RY( pMDU, AftRHC[6].GetVoltage(), 44, 10 );
		SPEC25_SPEC43_printRHC_RY( pMDU, AftRHC[7].GetVoltage(), 44, 11 );
		SPEC25_SPEC43_printRHC_RY( pMDU, AftRHC[8].GetVoltage(), 44, 12 );
		return;
	}

	void GeneralDisplays::OnPaint_SPEC51_PASS( vc::MDU* pMDU ) const
	{
		PrintCommonHeader( "    OVERRIDE", pMDU );

		// static parts (labels)
		// ABORT MODE
		pMDU->mvprint( 0, 2, "ABORT MODE" );
		pMDU->mvprint( 1, 3, "TAL" );
		pMDU->mvprint( 10, 3, "1" );
		pMDU->mvprint( 1, 4, "ATO" );
		pMDU->mvprint( 10, 4, "2" );
		pMDU->mvprint( 0, 5, "ABORT     3" );
		pMDU->mvprint( 0, 6, "THROT MAX 4" );
		pMDU->mvprint( 6, 7, "ABT 50" );
		pMDU->mvprint( 6, 8, "NOM 51" );

		// ENTRY FCS
		pMDU->mvprint( 29, 2, "ENTRY FCS" );
		pMDU->mvprint( 18, 3, "ELEVON    FILTER    ATMOSPHERE" );
		pMDU->mvprint( 16, 4, "AUTO  17    NOM 20    NOM    22" );
		pMDU->mvprint( 16, 5, "FIXED 18    ALT 21    N POLE 23" );
		pMDU->mvprint( 18, 6, "SSME REPOS 19       S POLE 24" );

		// IMU
		pMDU->mvprint( 18, 7, "IMU STAT ATT DES" );
		pMDU->mvprint( 19, 8, "1" );
		pMDU->mvprint( 31, 8, "25" );
		pMDU->mvprint( 19, 9, "2" );
		pMDU->mvprint( 31, 9, "26" );
		pMDU->mvprint( 19, 10, "3" );
		pMDU->mvprint( 31, 10, "27" );

		// PRL
		pMDU->mvprint( 43, 7, "PRL" );
		pMDU->mvprint( 39, 8, "SYS AUT DES" );
		pMDU->mvprint( 40, 9, "1  28  31" );
		pMDU->mvprint( 40, 10, "2  29  32" );
		pMDU->mvprint( 40, 11, "3  30  33" );

		// PRPLT DUMP
		pMDU->mvprint( 3, 9, "PRPLT DUMP" );
		pMDU->mvprint( 4, 10, "ICNCT 5" );
		pMDU->mvprint( 0, 11, "OMS DUMP" );
		pMDU->mvprint( 3, 12, "ARM    6" );
		pMDU->mvprint( 3, 13, "START  7" );
		pMDU->mvprint( 3, 14, "STOP   8" );
		pMDU->mvprint( 1, 15, "9 QUAN/SIDE" );
		pMDU->mvprint( 0, 16, "OMS DUMP TTG" );

		// ADTA
		pMDU->mvprint( 18, 12, "ADTA" );
		pMDU->mvprint( 28, 12, "H" );
		pMDU->Alpha( 36, 12 );
		pMDU->mvprint( 42, 12, "M    DES" );
		pMDU->mvprint( 20, 13, "L 1" );
		pMDU->mvprint( 47, 13, "34" );
		pMDU->mvprint( 22, 14, "3" );
		pMDU->mvprint( 47, 14, "35" );
		pMDU->mvprint( 20, 15, "R 2" );
		pMDU->mvprint( 47, 15, "36" );
		pMDU->mvprint( 22, 16, "4" );
		pMDU->mvprint( 47, 16, "37" );

		// ET SEP
		pMDU->mvprint( 18, 17, "ET SEP" );
		pMDU->mvprint( 20, 18, "AUTO    38" );
		pMDU->mvprint( 20, 19, "SEP     39" );

		// ROLL MODE
		pMDU->mvprint( 34, 17, "ROLL MODE" );
		pMDU->mvprint( 35, 18, "AUTO SEL    42" );
		pMDU->mvprint( 35, 19, "WRAP MODE 45" );

		// AFT RCS
		pMDU->mvprint( 0, 19, "AFT RCS 13" );
		pMDU->mvprint( 3, 20, "14 TIME" );

		// ET UMB DR
		pMDU->mvprint( 18, 20, "ET UMB DR" );
		pMDU->mvprint( 20, 21, "CLOSE   40" );

		// VENT DOOR CNTL
		pMDU->mvprint( 34, 20, "VENT DOOR CNTL" );
		pMDU->mvprint( 35, 21, "OPEN   43" );
		pMDU->mvprint( 35, 22, "CLOSE  44" );

		// FWD RCS
		pMDU->mvprint( 0, 22, "FWD RCS 15" );
		pMDU->mvprint( 3, 23, "16 TIME" );

		// RCS RM MANIF
		pMDU->mvprint( 18, 22, "RCS RM MANF" );
		pMDU->mvprint( 20, 23, "CL OVRD 41" );


		// static parts (lines)
		pMDU->Line( 170, 98, 170, 336 );
		pMDU->Line( 170, 98, 510, 98 );
		pMDU->Line( 170, 168, 510, 168 );
		pMDU->Line( 170, 238, 510, 238 );


		// TODO dynamic parts
		return;
	}

	void GeneralDisplays::OnPaint_SPEC53_PASS( vc::MDU* pMDU ) const
	{
		PrintCommonHeader( "       CONTROLS", pMDU );

		// static parts (labels)
		// SEC
		pMDU->mvprint( 0, 3, "SEC ACT CK" );
		pMDU->mvprint( 2, 5, "CH1 1" );
		pMDU->mvprint( 4, 6, "2 2" );
		pMDU->mvprint( 4, 7, "3 3" );
		pMDU->mvprint( 4, 8, "4 4" );
		pMDU->mvprint( 0, 9, "START 5" );
		pMDU->mvprint( 1, 10, "STOP 6" );
		pMDU->mvprint( 1, 12, "NEG STIM" );
		pMDU->mvprint( 2, 13, "ENA 7" );

		// AERO
		pMDU->mvprint( 14, 3, "AERO PORT STAT" );
		pMDU->mvprint( 21, 4, "1 2 3 4" );
		pMDU->mvprint( 14, 5, "L OB" );
		pMDU->mvprint( 16, 6, "IB" );
		pMDU->mvprint( 14, 7, "R IB" );
		pMDU->mvprint( 16, 8, "OB" );
		pMDU->mvprint( 15, 9, "RUD" );
		pMDU->mvprint( 11, 10, "SPD BRK" );

		// SSME
		pMDU->mvprint( 33, 3, "SSME PORT STAT" );
		pMDU->mvprint( 38, 4, "1 2 3 4" );
		pMDU->mvprint( 34, 5, "L P" );
		pMDU->mvprint( 36, 6, "Y" );
		pMDU->mvprint( 34, 7, "C P" );
		pMDU->mvprint( 36, 8, "Y" );
		pMDU->mvprint( 34, 9, "R P" );
		pMDU->mvprint( 36, 10, "Y" );

		// ACT
		pMDU->mvprint( 22, 12, "ACT/CH" );
		pMDU->mvprint( 13, 13, "8 BYPASS" );
		pMDU->mvprint( 13, 14, "9  RESET" );

		// LRU
		pMDU->mvprint( 1, 17, "LRU   AA      RGA   SURF FDBK" );
		pMDU->mvprint( 7, 18, "DES     DES" );
		pMDU->mvprint( 23, 18, "DES" );
		pMDU->mvprint( 2, 19, "1" );
		pMDU->mvprint( 6, 19, "10" );
		pMDU->mvprint( 14, 19, "14" );
		pMDU->mvprint( 22, 19, "18" );
		pMDU->mvprint( 2, 20, "2" );
		pMDU->mvprint( 6, 20, "11" );
		pMDU->mvprint( 14, 20, "15" );
		pMDU->mvprint( 22, 20, "19" );
		pMDU->mvprint( 2, 21, "3" );
		pMDU->mvprint( 6, 21, "12" );
		pMDU->mvprint( 14, 21, "16" );
		pMDU->mvprint( 22, 21, "20" );
		pMDU->mvprint( 2, 22, "4" );
		pMDU->mvprint( 6, 22, "13" );
		pMDU->mvprint( 14, 22, "17" );
		pMDU->mvprint( 22, 22, "21" );


		// TODO dynamic parts
		return;
	}

	void GeneralDisplays::OnPaint_SPEC55_PASS( vc::MDU* pMDU ) const
	{
		PrintCommonHeader( "    GPS STATUS", pMDU );
		
		// static parts (labels)
		pMDU->mvprint( 0, 2, "I/O 10   GPS1  GPS2  GPS3" );
		pMDU->mvprint( 0, 3, "STAT" );
		pMDU->mvprint( 0, 4, "MODE" );
		pMDU->mvprint( 0, 5, "S/TEST    11    12    13" );
		pMDU->mvprint( 0, 6, "INIT" );
		pMDU->mvprint( 10, 6, "14" );
		pMDU->mvprint( 16, 6, "15" );
		pMDU->mvprint( 22, 6, "16" );
		pMDU->mvprint( 0, 7, "NAV" );
		pMDU->mvprint( 10, 7, "17" );
		pMDU->mvprint( 16, 7, "18" );
		pMDU->mvprint( 22, 7, "19" );
		pMDU->mvprint( 0, 8, "RESTART   20    21    22" );
		pMDU->mvprint( 0, 10, "GDOP" );

		pMDU->mvprint( 33, 3, "GPS MINUS NAV" );
		pMDU->Delta( 28, 4 );
		pMDU->mvprint( 29, 4, "H" );
		pMDU->Delta( 40, 4 );
		pMDU->mvprint( 41, 4, "H" );
		pMDU->DotCharacter( 41, 4 );
		pMDU->Delta( 28, 5 );
		pMDU->mvprint( 29, 5, "DR" );
		pMDU->Delta( 40, 5 );
		pMDU->mvprint( 41, 5, "DR" );
		pMDU->DotCharacter( 41, 5 );
		pMDU->Delta( 28, 6 );
		pMDU->mvprint( 29, 6, "CR" );
		pMDU->Delta( 40, 6 );
		pMDU->mvprint( 41, 6, "CR" );
		pMDU->DotCharacter( 41, 6 );
		pMDU->mvprint( 30, 8, "LAT" );
		pMDU->mvprint( 38, 8, "LON" );
		pMDU->mvprint( 46, 8, "ALT" );

		pMDU->mvprint( 0, 11, "DG FAIL" );
		pMDU->mvprint( 0, 12, "DES RCVR  26    27    28" );
		pMDU->mvprint( 0, 13, "QA OVRD   29    30    31" );
		pMDU->mvprint( 0, 14, "SF CAND" );
		pMDU->mvprint( 0, 15, "QA1 P 1" );
		pMDU->Sigma( 7, 15 );
		pMDU->mvprint( 0, 16, "QA2 POS" );
		pMDU->mvprint( 4, 17, "VEL" );
		pMDU->mvprint( 0, 18, "QA3 POS" );
		pMDU->mvprint( 4, 19, "VEL" );

		pMDU->mvprint( 40, 11, "AUT INH FOR" );
		pMDU->mvprint( 28, 12, "GPS TO G&C  32  33  34" );
		pMDU->mvprint( 28, 13, "GPS TO NAV  35  36  37" );
		pMDU->mvprint( 28, 14, "METERING OVERRIDE   38" );
		pMDU->mvprint( 28, 15, "GPS TIME ADJUST ENA 39" );

		pMDU->mvprint( 34, 16, "SATELLITES" );
		pMDU->mvprint( 28, 17, "TRKD  C1 C2 C3 C4 C5 C6" );
		pMDU->mvprint( 29, 18, "GPS1" );
		pMDU->mvprint( 29, 19, "GPS2" );
		pMDU->mvprint( 29, 20, "GPS3" );
		pMDU->mvprint( 28, 22, "DES 43" );

		pMDU->mvprint( 10, 20, "1-2   2-3   3-1" );
		pMDU->mvprint( 0, 21, "QA4 POS" );
		pMDU->mvprint( 4, 22, "VEL" );
		pMDU->mvprint( 7, 23, "LAST SEL FIL UPDATE" );

		
		// static parts (lines)
		pMDU->Line( 140, 14, 140, 322 );
		pMDU->Line( 200, 14, 200, 322 );
		pMDU->Line( 260, 14, 260, 322 );
		pMDU->Line( 0, 154, 510, 154 );
		pMDU->Line( 260, 224, 510, 224 );
		pMDU->Line( 0, 280, 260, 280 );


		// TODO dynamic parts
		int mm = GetMajorMode();
		if (((mm / 100) != 9) && ((mm / 100) != 1) && (mm != 601))
		{
			char cbuf[16];
			double lon = 0;
			double lat = 0;
			double rad = 0;
			STS()->GetEquPos( lon, lat, rad );

			lon *= DEG;
			lat *= DEG;
			rad -= 6371010;
			if ((mm == 304) || (mm == 305) || (mm == 602) || (mm == 603)) rad *= (MPS2FPS / 1000);// kft
			else rad /= NMI2M;// nm
			
			if (lat >= 0) pMDU->mvprint( 28, 8, "N" );
			else pMDU->mvprint( 28, 8, "S" );

			sprintf_s( cbuf, 16, "%05.2f", fabs( lat ) );
			pMDU->mvprint( 28, 9, cbuf );

			if (lon >= 0) pMDU->mvprint( 36, 8, "E" );
			else pMDU->mvprint( 36, 8, "W" );

			sprintf_s( cbuf, 16, "%06.2f", fabs( lon ) );
			pMDU->mvprint( 35, 9, cbuf );

			sprintf_s( cbuf, 16, "%08.4f", rad );
			pMDU->mvprint( 43, 9, cbuf );
		}
		return;
	}

	void GeneralDisplays::OnPaint_DISP99_PASS( vc::MDU* pMDU ) const
	{
		PrintCommonHeader( "    FAULT", pMDU );

		// static parts (labels)
		pMDU->mvprint( 4, 5, "CRT" );
		pMDU->mvprint( 13, 5, "FAULT      C/W   GPC      TIME" );
		pMDU->mvprint( 4, 6, "ID" );
		return;
	}

	void GeneralDisplays::OnPaint_SPEC112_PASS( vc::MDU* pMDU ) const
	{
		PrintCommonHeader( "  GPC/BTU I/F", pMDU );

		// static parts (labels)
		// BTU SELECTION
		pMDU->mvprint( 3, 2, "BTU SELECTION" );
		pMDU->mvprint( 1, 3, "FF1 1   PCMMU 12" );
		pMDU->mvprint( 3, 4, "2 2   MMU1  13" );
		pMDU->mvprint( 3, 5, "3 3" );
		pMDU->mvprint( 12, 5, "2  14" );
		pMDU->mvprint( 3, 6, "4 4   MEC1  15" );
		pMDU->mvprint( 1, 7, "FA1 5" );
		pMDU->mvprint( 12, 7, "2  16" );
		pMDU->mvprint( 3, 8, "2 6   EIU1  17" );
		pMDU->mvprint( 3, 9, "3 7" );
		pMDU->mvprint( 12, 9, "2  18" );
		pMDU->mvprint( 3, 10, "4 8" );
		pMDU->mvprint( 12, 10, "3  19" );
		pMDU->mvprint( 1, 11, "PF1 9   DDU   20" );
		pMDU->mvprint( 3, 12, "2 10  MCIU  21" );
		pMDU->mvprint( 10, 13, "CMD  22" );
		pMDU->mvprint( 10, 14, "SRB  23" );
		pMDU->mvprint( 1, 15, "ALL" );
		pMDU->mvprint( 1, 16, "MDMS 11" );
		pMDU->mvprint( 11, 15, "DES" );
		pMDU->mvprint( 11, 16, "ALL   24" );

		// TEST STATUS
		pMDU->mvprint( 20, 2, "TEST STATUS" );
		pMDU->mvprint( 20, 4, "BTU ITEM" );
		pMDU->mvprint( 20, 5, "STEP" );
		pMDU->mvprint( 20, 6, "RDW" );
		pMDU->mvprint( 20, 7, "BCE STAT RG" );

		// MDM OUTPUT TEST
		pMDU->mvprint( 33, 3, "MDM OUTPUT TEST" );
		pMDU->mvprint( 37, 4, "MODULE" );

		pMDU->mvprint( 33, 6, "ANALOG OUTPUTS" );
		pMDU->mvprint( 34, 7, "0" );
		pMDU->mvprint( 43, 7, "1" );
		pMDU->mvprint( 34, 8, "2" );
		pMDU->mvprint( 43, 8, "3" );
		pMDU->mvprint( 34, 9, "4" );
		pMDU->mvprint( 43, 9, "5" );
		pMDU->mvprint( 34, 10, "6" );
		pMDU->mvprint( 43, 10, "7" );
		pMDU->mvprint( 34, 11, "8" );
		pMDU->mvprint( 43, 11, "9" );
		pMDU->mvprint( 33, 12, "10" );
		pMDU->mvprint( 42, 12, "11" );
		pMDU->mvprint( 33, 13, "12" );
		pMDU->mvprint( 42, 13, "13" );
		pMDU->mvprint( 33, 14, "14" );
		pMDU->mvprint( 42, 14, "15" );

		pMDU->mvprint( 33, 16, "DISCRETE OUTPUTS" );
		pMDU->mvprint( 33, 17, "CHANNEL 0" );
		pMDU->mvprint( 33, 18, "CHANNEL 1" );
		pMDU->mvprint( 33, 19, "CHANNEL 2" );

		// PORT SEL
		pMDU->mvprint( 19, 10, "PORT SEL 1 31" );
		pMDU->mvprint( 28, 11, "2 32" );

		// TEST
		pMDU->mvprint( 9, 18, "LEVEL 1    25" );
		pMDU->mvprint( 1, 19, "TEST    LEVEL 2    26" );
		pMDU->mvprint( 9, 20, "MDM OUTPUT 27" );
		pMDU->mvprint( 1, 21, "CONTROL TERMINATE  28" );
		pMDU->mvprint( 9, 22, "CONTINUE   29" );

		// BTU
		pMDU->mvprint( 31, 21, "BTU CYCLIC BITE 30" );


		// dynamic parts
		// TODO finish
		// BTU SELECTION
		if (ITEM_STATE_SPEC112[0] == true) pMDU->mvprint( 6, 3, "*" );
		if (ITEM_STATE_SPEC112[1] == true) pMDU->mvprint( 6, 4, "*" );
		if (ITEM_STATE_SPEC112[2] == true) pMDU->mvprint( 6, 5, "*" );
		if (ITEM_STATE_SPEC112[3] == true) pMDU->mvprint( 6, 6, "*" );
		if (ITEM_STATE_SPEC112[4] == true) pMDU->mvprint( 6, 7, "*" );
		if (ITEM_STATE_SPEC112[5] == true) pMDU->mvprint( 6, 8, "*" );
		if (ITEM_STATE_SPEC112[6] == true) pMDU->mvprint( 6, 9, "*" );
		if (ITEM_STATE_SPEC112[7] == true) pMDU->mvprint( 6, 10, "*" );
		if (ITEM_STATE_SPEC112[8] == true) pMDU->mvprint( 6, 11, "*" );
		if (ITEM_STATE_SPEC112[9] == true) pMDU->mvprint( 7, 12, "*" );
		if (ITEM_STATE_SPEC112[10] == true) pMDU->mvprint( 8, 16, "*" );
		if (ITEM_STATE_SPEC112[11] == true) pMDU->mvprint( 17, 3, "*" );
		if (ITEM_STATE_SPEC112[12] == true) pMDU->mvprint( 17, 4, "*" );
		if (ITEM_STATE_SPEC112[13] == true) pMDU->mvprint( 17, 5, "*" );
		if (ITEM_STATE_SPEC112[14] == true) pMDU->mvprint( 17, 6, "*" );
		if (ITEM_STATE_SPEC112[15] == true) pMDU->mvprint( 17, 7, "*" );
		if (ITEM_STATE_SPEC112[16] == true) pMDU->mvprint( 17, 8, "*" );
		if (ITEM_STATE_SPEC112[17] == true) pMDU->mvprint( 17, 9, "*" );
		if (ITEM_STATE_SPEC112[18] == true) pMDU->mvprint( 17, 10, "*" );
		if (ITEM_STATE_SPEC112[19] == true) pMDU->mvprint( 17, 11, "*" );
		if (ITEM_STATE_SPEC112[20] == true) pMDU->mvprint( 17, 12, "*" );
		if (ITEM_STATE_SPEC112[21] == true) pMDU->mvprint( 17, 13, "*" );
		if (ITEM_STATE_SPEC112[22] == true) pMDU->mvprint( 17, 14, "*" );
		if (ITEM_STATE_SPEC112[23] == true) pMDU->mvprint( 19, 16, "*" );



		// PORT SEL
		if (ITEM_STATE_SPEC112[30] == true) pMDU->mvprint( 32, 10, "*" );
		if (ITEM_STATE_SPEC112[31] == true) pMDU->mvprint( 32, 11, "*" );

		// TEST
		if (ITEM_STATE_SPEC112[24] == true) pMDU->mvprint( 22, 18, "*" );
		if (ITEM_STATE_SPEC112[25] == true) pMDU->mvprint( 22, 19, "*" );
		if (ITEM_STATE_SPEC112[26] == true) pMDU->mvprint( 22, 20, "*" );
		//if (ITEM_STATE_SPEC112[27] == true) pMDU->mvprint( 0, 0, "*" );
		//if (ITEM_STATE_SPEC112[28] == true) pMDU->mvprint( 0, 0, "*" );

		// BTU
		if (ITEM_STATE_SPEC112[29] == true) pMDU->mvprint( 49, 21, "*" );
		return;
	}

	void GeneralDisplays::OnPaint_SPEC113_PASS( vc::MDU* pMDU ) const
	{
		PrintCommonHeader( "ACTUATOR CONTROL", pMDU );

		// static parts (labels)
		// MONITOR
		pMDU->mvprint( 3, 1, "MONITOR" );
		pMDU->mvprint( 1, 2, "CMD     POS  ACT" );
		pMDU->mvprint( 15, 3, "SB  1" );
		pMDU->mvprint( 14, 4, "RUD  2" );
		pMDU->mvprint( 14, 5, "LIE  3" );
		pMDU->mvprint( 14, 6, "RIE  4" );
		pMDU->mvprint( 14, 7, "LOE  5" );
		pMDU->mvprint( 14, 8, "ROE  6" );
		pMDU->mvprint( 15, 9, "1P  7" );
		pMDU->mvprint( 15, 10, "1Y  8" );
		pMDU->mvprint( 15, 11, "2P  9" );
		pMDU->mvprint( 15, 12, "2Y 10" );
		pMDU->mvprint( 15, 13, "3P 12" );
		pMDU->mvprint( 15, 14, "3Y 12" );
		pMDU->mvprint( 14, 15, "RPA 13" );
		pMDU->mvprint( 14, 16, "RPS 14" );
		pMDU->mvprint( 14, 17, "RYA 15" );
		pMDU->mvprint( 14, 18, "RYS 16" );
		pMDU->mvprint( 14, 19, "LPA 17" );
		pMDU->mvprint( 14, 20, "LPS 18" );
		pMDU->mvprint( 14, 21, "LYA 19" );
		pMDU->mvprint( 14, 22, "LYS 20" );
		pMDU->mvprint( 15, 23, "BF 21" );

		// FAIL
		pMDU->mvprint( 23, 1, "FAIL" );
		pMDU->mvprint( 22, 2, "A B C D" );

		// ACTUATOR GIMBALING
		pMDU->mvprint( 32, 2, "ACTUATOR GIMBALING" );
		pMDU->mvprint( 30, 3, "RATE 29" );
		pMDU->mvprint( 44, 3, "DEG/SEC" );
		pMDU->mvprint( 30, 4, "FIN POS 30" );
		pMDU->mvprint( 48, 4, "DEG" );
		pMDU->mvprint( 30, 5, "START 31" );
		pMDU->mvprint( 43, 5, "STOP 32" );
		pMDU->mvprint( 40, 6, "STATUS" );
		pMDU->mvprint( 40, 7, "ECP ERR" );
		pMDU->mvprint( 41, 8, "BF MON 33" );

		// HYD PR
		pMDU->mvprint( 32, 7, "HYD PR" );
		pMDU->mvprint( 32, 8, "1" );
		pMDU->mvprint( 32, 9, "2" );
		pMDU->mvprint( 32, 10, "3" );

		// AI SRB/NW
		pMDU->mvprint( 38, 11, "AI SRB/NW 34" );

		// ME ISO V
		pMDU->mvprint( 30, 12, "ME ISO V" );
		pMDU->mvprint( 30, 13, "1  2" );
		pMDU->mvprint( 36, 13, "3" );

		// FC LIM CHNG
		pMDU->mvprint( 40, 13, "FC LIM CHNG" );
		pMDU->mvprint( 39, 14, "R/SB MAT 35" );
		pMDU->mvprint( 35, 15, "R/SB RFG/FRT 36" );
		pMDU->mvprint( 37, 16, "MAT P 40MS 37" );
		pMDU->mvprint( 37, 17, "NOP CA RFG 38" );
		pMDU->mvprint( 37, 18, "NOP CA FRT 39" );
		pMDU->mvprint( 40, 19, "NOP OMS 40" );
		pMDU->mvprint( 36, 20, "SRB S/S LIM 41" );
		pMDU->mvprint( 37, 21, "EL POS LIM 42" );
		pMDU->mvprint( 36, 22, "S/B POS LIM 43" );
		pMDU->mvprint( 34, 23, "R RATE .5 PCM 44" );

		// AI MODES
		pMDU->mvprint( 24, 16, "AI MODES" );
		pMDU->mvprint( 23, 17, "0 SAFE 22" );
		pMDU->mvprint( 23, 18, "1 MDM  23" );
		pMDU->mvprint( 23, 19, "2 FERY 24" );
		pMDU->mvprint( 23, 20, "3 RAIN 25" );
		pMDU->mvprint( 23, 21, "4 GRAV 26" );
		pMDU->mvprint( 23, 22, "5 NULL 27" );
		pMDU->mvprint( 23, 23, "6 TURN 28" );


		// static parts (lines)
		// ACTUATOR GIMBALING
		pMDU->Line( 320, 42, 500, 42 );

		pMDU->Line( 380, 56, 400, 56 );
		pMDU->Line( 410, 56, 430, 56 );
		
		// HYD PR
		pMDU->Line( 320, 112, 380, 112 );
		
		// ME ISO V
		pMDU->Line( 300, 182, 380, 182 );
		
		// FC LIM CHNG
		pMDU->Line( 400, 196, 510, 196 );
		
		// AI MODES
		pMDU->Line( 240, 238, 320, 238 );


		// dynamic parts
		// TODO finish
		char cbuf[64];

		// MONITOR
		/*sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 0, 3, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 7, 3, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 0, 4, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 7, 4, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 0, 5, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 7, 5, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 0, 6, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 7, 6, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 0, 7, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 7, 7, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 0, 8, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 7, 8, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 0, 9, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 7, 9, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 0, 10, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 7, 10, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 0, 11, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 7, 11, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 0, 12, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 7, 12, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 0, 13, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 7, 13, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 0, 14, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 7, 14, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 0, 15, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 7, 15, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 0, 16, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 7, 16, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 0, 17, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 7, 17, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 0, 18, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 7, 18, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 0, 19, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 7, 19, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 0, 20, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 7, 20, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 0, 21, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 7, 21, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 0, 22, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 7, 22, cbuf );
		sprintf_s( cbuf, 64, "%02.0f", dtmp );
		pMDU->mvprint( 1, 23, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 7, 23, cbuf );*/

		if (ITEM_STATE_SPEC113[0] == true) pMDU->mvprint( 20, 3, "*" );
		if (ITEM_STATE_SPEC113[1] == true) pMDU->mvprint( 20, 4, "*" );
		if (ITEM_STATE_SPEC113[2] == true) pMDU->mvprint( 20, 5, "*" );
		if (ITEM_STATE_SPEC113[3] == true) pMDU->mvprint( 20, 6, "*" );
		if (ITEM_STATE_SPEC113[4] == true) pMDU->mvprint( 20, 7, "*" );
		if (ITEM_STATE_SPEC113[5] == true) pMDU->mvprint( 20, 8, "*" );
		if (ITEM_STATE_SPEC113[6] == true) pMDU->mvprint( 20, 9, "*" );
		if (ITEM_STATE_SPEC113[7] == true) pMDU->mvprint( 20, 10, "*" );
		if (ITEM_STATE_SPEC113[8] == true) pMDU->mvprint( 20, 11, "*" );
		if (ITEM_STATE_SPEC113[9] == true) pMDU->mvprint( 20, 12, "*" );
		if (ITEM_STATE_SPEC113[10] == true) pMDU->mvprint( 20, 13, "*" );
		if (ITEM_STATE_SPEC113[11] == true) pMDU->mvprint( 20, 14, "*" );
		if (ITEM_STATE_SPEC113[12] == true) pMDU->mvprint( 20, 15, "*" );
		if (ITEM_STATE_SPEC113[13] == true) pMDU->mvprint( 20, 16, "*" );
		if (ITEM_STATE_SPEC113[14] == true) pMDU->mvprint( 20, 17, "*" );
		if (ITEM_STATE_SPEC113[15] == true) pMDU->mvprint( 20, 18, "*" );
		if (ITEM_STATE_SPEC113[16] == true) pMDU->mvprint( 20, 19, "*" );
		if (ITEM_STATE_SPEC113[17] == true) pMDU->mvprint( 20, 20, "*" );
		if (ITEM_STATE_SPEC113[18] == true) pMDU->mvprint( 20, 21, "*" );
		if (ITEM_STATE_SPEC113[19] == true) pMDU->mvprint( 20, 22, "*" );
		if (ITEM_STATE_SPEC113[20] == true) pMDU->mvprint( 20, 23, "*" );

		// FAIL
		/*if (abc == true) pMDU->DownArrow( 22, 3, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 22, 3, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 24, 3, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 24, 3, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 26, 3, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 26, 3, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 28, 3, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 28, 3, "M", dps::DEUATT_OVERBRIGHT );

		if (abc == true) pMDU->DownArrow( 22, 4, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 22, 4, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 24, 4, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 24, 4, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 26, 4, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 26, 4, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 28, 4, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 28, 4, "M", dps::DEUATT_OVERBRIGHT );

		if (abc == true) pMDU->DownArrow( 22, 5, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 22, 5, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 24, 5, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 24, 5, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 26, 5, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 26, 5, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 28, 5, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 28, 5, "M", dps::DEUATT_OVERBRIGHT );

		if (abc == true) pMDU->DownArrow( 22, 6, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 22, 6, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 24, 6, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 24, 6, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 26, 6, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 26, 6, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 28, 6, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 28, 6, "M", dps::DEUATT_OVERBRIGHT );

		if (abc == true) pMDU->DownArrow( 22, 7, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 22, 7, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 24, 7, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 24, 7, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 26, 7, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 26, 7, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 28, 7, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 28, 7, "M", dps::DEUATT_OVERBRIGHT );

		if (abc == true) pMDU->DownArrow( 22, 8, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 22, 8, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 24, 8, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 24, 8, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 26, 8, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 26, 8, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 28, 8, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 28, 8, "M", dps::DEUATT_OVERBRIGHT );

		if (abc == true) pMDU->DownArrow( 22, 9, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 22, 9, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 24, 9, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 24, 9, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 26, 9, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 26, 9, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 28, 9, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 28, 9, "M", dps::DEUATT_OVERBRIGHT );

		if (abc == true) pMDU->DownArrow( 22, 10, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 22, 10, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 24, 10, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 24, 10, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 26, 10, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 26, 10, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 28, 10, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 28, 10, "M", dps::DEUATT_OVERBRIGHT );

		if (abc == true) pMDU->DownArrow( 22, 11, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 22, 11, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 24, 11, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 24, 11, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 26, 11, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 26, 11, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 28, 11, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 28, 11, "M", dps::DEUATT_OVERBRIGHT );

		if (abc == true) pMDU->DownArrow( 22, 12, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 22, 12, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 24, 12, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 24, 12, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 26, 12, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 26, 12, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 28, 12, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 28, 12, "M", dps::DEUATT_OVERBRIGHT );

		if (abc == true) pMDU->DownArrow( 22, 13, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 22, 13, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 24, 13, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 24, 13, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 26, 13, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 26, 13, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 28, 13, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 28, 13, "M", dps::DEUATT_OVERBRIGHT );

		if (abc == true) pMDU->DownArrow( 22, 14, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 22, 14, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 24, 14, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 24, 14, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 26, 14, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 26, 14, "M", dps::DEUATT_OVERBRIGHT );
		if (abc == true) pMDU->DownArrow( 28, 14, dps::DEUATT_OVERBRIGHT );
		else if (abc == true) pMDU->mvprint( 28, 14, "M", dps::DEUATT_OVERBRIGHT );*/

		// ACTUATOR GIMBALING
		/*sprintf_s( cbuf, 64, "%05.2f", dtmp );
		pMDU->mvprint( 38, 3, cbuf );
		sprintf_s( cbuf, 64, "%+06.2f", dtmp );
		pMDU->mvprint( 41, 4, cbuf );*/

		//if (ITEM_STATE_SPEC113[28] == true) pMDU->mvprint( 0, 0, "*" );
		//if (ITEM_STATE_SPEC113[29] == true) pMDU->mvprint( 0, 0, "*" );
		if (ITEM_STATE_SPEC113[30] == true)
		{
			pMDU->mvprint( 38, 5, "*" );

			/*if (abc == true)
			{
				// speedbrake
				if (abc == true)
				{
					pMDU->LeftArrow( 39, 5 );
					pMDU->RightArrow( 40, 5 );
				}
				else
				{
					pMDU->RightArrow( 39, 5 );
					pMDU->LeftArrow( 40, 5 );
				}
			}
			else if (abc == true)
			{
				// other actuators (vertical)
				if (abc == true) pMDU->UpArrow( 39, 5 );
				else pMDU->DownArrow( 39, 5 );
			}
			else
			{
				// other actuators (horizontal)
				if (abc == true) pMDU->LeftArrow( 39, 5 );
				else pMDU->RightArrow( 39, 5 );
			}*/
		}
		if (ITEM_STATE_SPEC113[31] == true) pMDU->mvprint( 50, 5, "*" );
		if (ITEM_STATE_SPEC113[32] == true) pMDU->mvprint( 50, 8, "*" );

		// HYD PR
		sprintf_s( cbuf, 64, "%4.0f", STS()->pAPU[0]->GetHydraulicPressure() );
		pMDU->mvprint( 34, 8, cbuf );
		sprintf_s( cbuf, 64, "%4.0f", STS()->pAPU[1]->GetHydraulicPressure() );
		pMDU->mvprint( 34, 9, cbuf );
		sprintf_s( cbuf, 64, "%4.0f", STS()->pAPU[2]->GetHydraulicPressure() );
		pMDU->mvprint( 34, 10, cbuf );

		// AI SRB/NW
		if (ITEM_STATE_SPEC113[33] == true) pMDU->mvprint( 50, 11, "*" );

		// ME ISO V
		/*if (abc == true) pMDU->mvprint( 31, 13, "C" );
		if (abc == true) pMDU->mvprint( 34, 13, "C" );
		if (abc == true) pMDU->mvprint( 37, 13, "C" );*/

		// FC LIM CHNG
		if (ITEM_STATE_SPEC113[34] == true) pMDU->mvprint( 50, 14, "*" );
		if (ITEM_STATE_SPEC113[35] == true) pMDU->mvprint( 50, 15, "*" );
		if (ITEM_STATE_SPEC113[36] == true) pMDU->mvprint( 50, 16, "*" );
		if (ITEM_STATE_SPEC113[37] == true) pMDU->mvprint( 50, 17, "*" );
		if (ITEM_STATE_SPEC113[38] == true) pMDU->mvprint( 50, 18, "*" );
		if (ITEM_STATE_SPEC113[39] == true) pMDU->mvprint( 50, 19, "*" );
		if (ITEM_STATE_SPEC113[40] == true) pMDU->mvprint( 50, 20, "*" );
		if (ITEM_STATE_SPEC113[41] == true) pMDU->mvprint( 50, 21, "*" );
		if (ITEM_STATE_SPEC113[42] == true) pMDU->mvprint( 50, 22, "*" );
		if (ITEM_STATE_SPEC113[43] == true) pMDU->mvprint( 50, 23, "*" );

		// AI MODES
		if (ITEM_STATE_SPEC113[21] == true) pMDU->mvprint( 32, 17, "*" );
		if (ITEM_STATE_SPEC113[22] == true) pMDU->mvprint( 32, 18, "*" );
		if (ITEM_STATE_SPEC113[23] == true) pMDU->mvprint( 32, 19, "*" );
		if (ITEM_STATE_SPEC113[24] == true) pMDU->mvprint( 32, 20, "*" );
		if (ITEM_STATE_SPEC113[25] == true) pMDU->mvprint( 32, 21, "*" );
		if (ITEM_STATE_SPEC113[26] == true) pMDU->mvprint( 32, 22, "*" );
		if (ITEM_STATE_SPEC113[27] == true) pMDU->mvprint( 32, 23, "*" );
		return;
	}


	void GeneralDisplays::OnPaint_DISP18_BFS( vc::MDU* pMDU ) const
	{
		PrintCommonHeader( " GNC SYS SUMM 1", pMDU );
		pMDU->mvprint( 34, 1, "BFS", dps::DEUATT_OVERBRIGHT );

		// static parts (labels)
		// SURF
		pMDU->mvprint( 5, 3, "SURF     POS   MOM" );
		pMDU->mvprint( 5, 4, "L OB" );
		pMDU->mvprint( 7, 5, "IB" );
		pMDU->mvprint( 5, 6, "R IB" );
		pMDU->mvprint( 7, 7, "OB" );
		pMDU->mvprint( 5, 8, "AIL" );
		pMDU->mvprint( 5, 9, "RUD" );
		pMDU->mvprint( 5, 10, "SPD BRK" );
		pMDU->mvprint( 5, 11, "BDY FLP" );

		// DPS
		pMDU->mvprint( 30, 3, "DPS    1 2 3 4" );
		pMDU->mvprint( 30, 4, "MDM FF" );
		pMDU->mvprint( 30, 5, "FA" );
		pMDU->mvprint( 30, 6, "PL" );

		// FCS
		pMDU->mvprint( 30, 9, "FCS CH 1 2 3 4" );

		// NAV
		pMDU->mvprint( 30, 13, "NAV  1 2 3 4" );
		pMDU->mvprint( 30, 14, "IMU" );
		pMDU->mvprint( 30, 15, "TAC" );
		pMDU->mvprint( 30, 16, "ADTA" );

		// MPS
		pMDU->mvprint( 2, 13, "MPS" );
		pMDU->mvprint( 14, 13, "L" );
		pMDU->mvprint( 20, 13, "C" );
		pMDU->mvprint( 26, 13, "R" );
		pMDU->mvprint( 2, 14, "HE TK P" );
		pMDU->mvprint( 4, 15, "REG P A" );
		pMDU->mvprint( 10, 16, "B" );
		pMDU->mvprint( 4, 17, "dP/dT" );

		pMDU->mvprint( 30, 18, "MPS PNEU HE P" );
		pMDU->mvprint( 35, 19, "TK" );
		pMDU->mvprint( 35, 20, "REG" );
		pMDU->mvprint( 35, 21, "ACUM" );
		pMDU->mvprint( 35, 19, "TK" );

		pMDU->mvprint( 2, 19, "ULL P LH2" );
		pMDU->mvprint( 8, 20, "LO2" );

		pMDU->mvprint( 2, 22, "GH2 OUT P" );
		pMDU->mvprint( 2, 23, "GO2 OUT T" );

		pMDU->mvprint( 30, 22, "MANF P LH2" );
		pMDU->mvprint( 37, 23, "LO2" );


		// static parts (lines)
		pMDU->Line( 50, 56, 230, 56 );

		pMDU->Line( 300, 56, 440, 56 );

		pMDU->Line( 300, 140, 440, 140 );

		pMDU->Line( 300, 196, 420, 196 );

		pMDU->Line( 20, 196, 290, 196 );
		pMDU->Line( 290, 196, 290, 238 );
		pMDU->Line( 290, 238, 450, 238 );


		// dynamic parts
		// TODO finish
		char cbuf[64];
		double tmp[3] = {0};
		char pos;

		// SURF
		if (STS()->aerosurfaces.leftElevon > 0) pos = 'D';
		else if (STS()->aerosurfaces.leftElevon < 0) pos = 'U';
		else pos = ' ';
		sprintf_s( cbuf, 64, "%c%4.1f  %2.0f", pos, fabs( STS()->aerosurfaces.leftElevon ), tmp[0] );
		pMDU->mvprint( 13, 4, cbuf );

		if (STS()->aerosurfaces.leftElevon > 0) pos = 'D';
		else if (STS()->aerosurfaces.leftElevon < 0) pos = 'U';
		else pos = ' ';
		sprintf_s( cbuf, 64, "%c%4.1f  %2.0f", pos, fabs( STS()->aerosurfaces.leftElevon ), tmp[0] );
		pMDU->mvprint( 13, 5, cbuf );

		if (STS()->aerosurfaces.rightElevon > 0) pos = 'D';
		else if (STS()->aerosurfaces.rightElevon < 0) pos = 'U';
		else pos = ' ';
		sprintf_s( cbuf, 64, "%c%4.1f  %2.0f", pos, fabs( STS()->aerosurfaces.rightElevon ), tmp[0] );
		pMDU->mvprint( 13, 6, cbuf );

		if (STS()->aerosurfaces.rightElevon > 0) pos = 'D';
		else if (STS()->aerosurfaces.rightElevon < 0) pos = 'U';
		else pos = ' ';
		sprintf_s( cbuf, 64, "%c%4.1f  %2.0f", pos, fabs( STS()->aerosurfaces.rightElevon ), tmp[0] );
		pMDU->mvprint( 13, 7, cbuf );

		tmp[0] = (STS()->aerosurfaces.leftElevon - STS()->aerosurfaces.rightElevon) * 0.5;
		if (tmp[0] > 0) pos = 'R';
		else if (tmp[0] < 0) pos = 'L';
		else pos = ' ';
		sprintf_s( cbuf, 64, "%c%4.1f", pos, fabs( tmp[0] ) );
		pMDU->mvprint( 13, 8, cbuf );

		tmp[0] = STS()->aerosurfaces.rudder;
		if (tmp[0] > 0) pos = 'R';
		else if (tmp[0] < 0) pos = 'L';
		else pos = ' ';
		sprintf_s( cbuf, 64, "%c%4.1f", pos, fabs( tmp[0] ) );
		pMDU->mvprint( 13, 9, cbuf );

		sprintf_s( cbuf, 64, "%5.1f", fabs( STS()->aerosurfaces.speedbrake ) );
		pMDU->mvprint( 13, 10, cbuf );

		sprintf_s( cbuf, 64, "%5.1f", fabs( STS()->aerosurfaces.bodyFlap ) );
		pMDU->mvprint( 13, 11, cbuf );

		// DPS

		// FCS

		// NAV

		// MPS
		for (int i = 0; i < 3; i++)
		{
			tmp[i] = dipHeSysPressureSensor[i * 3].GetVoltage() * 1000;
			if (tmp[i] > 5000) tmp[i] = 5000;
			else if (tmp[i] < 0) tmp[i] = 0;
		}
		sprintf_s( cbuf, 64, "%4.0f  %4.0f  %4.0f", tmp[1], tmp[0], tmp[2] );
		pMDU->mvprint( 12, 14, cbuf );
		if (tmp[1] < 1150) pMDU->DownArrow( 16, 14, dps::DEUATT_OVERBRIGHT );
		if (tmp[0] < 1150) pMDU->DownArrow( 22, 14, dps::DEUATT_OVERBRIGHT );
		if (tmp[2] < 1150) pMDU->DownArrow( 28, 14, dps::DEUATT_OVERBRIGHT );

		for (int i = 0; i < 3; i++)
		{
			tmp[i] = dipHeSysPressureSensor[(i * 3) + 1].GetVoltage() * 200;
			if (tmp[i] > 1000) tmp[i] = 1000;
			else if (tmp[i] < 0) tmp[i] = 0;
		}
		sprintf_s( cbuf, 64, "%4.0f  %4.0f  %4.0f", tmp[1], tmp[0], tmp[2] );
		pMDU->mvprint( 12, 15, cbuf );
		if (tmp[1] < 680) pMDU->DownArrow( 16, 15, dps::DEUATT_OVERBRIGHT );
		else if (tmp[1] > 810) pMDU->UpArrow( 16, 15, dps::DEUATT_OVERBRIGHT );
		if (tmp[0] < 680) pMDU->DownArrow( 22, 15, dps::DEUATT_OVERBRIGHT );
		else if (tmp[0] > 810) pMDU->UpArrow( 22, 15, dps::DEUATT_OVERBRIGHT );
		if (tmp[2] < 680) pMDU->DownArrow( 28, 15, dps::DEUATT_OVERBRIGHT );
		else if (tmp[2] > 810) pMDU->UpArrow( 28, 15, dps::DEUATT_OVERBRIGHT );

		for (int i = 0; i < 3; i++)
		{
			tmp[i] = dipHeSysPressureSensor[(i * 3) + 2].GetVoltage() * 200;
			if (tmp[i] > 1000) tmp[i] = 1000;
			else if (tmp[i] < 0) tmp[i] = 0;
		}
		sprintf_s( cbuf, 64, "%4.0f  %4.0f  %4.0f", tmp[1], tmp[0], tmp[2] );
		pMDU->mvprint( 12, 16, cbuf );
		if (tmp[1] < 680) pMDU->DownArrow( 16, 16, dps::DEUATT_OVERBRIGHT );
		else if (tmp[1] > 810) pMDU->UpArrow( 16, 16, dps::DEUATT_OVERBRIGHT );
		if (tmp[0] < 680) pMDU->DownArrow( 22, 16, dps::DEUATT_OVERBRIGHT );
		else if (tmp[0] > 810) pMDU->UpArrow( 22, 16, dps::DEUATT_OVERBRIGHT );
		if (tmp[2] < 680) pMDU->DownArrow( 28, 16, dps::DEUATT_OVERBRIGHT );
		else if (tmp[2] > 810) pMDU->UpArrow( 28, 16, dps::DEUATT_OVERBRIGHT );

		for (int i = 0; i < 3; i++)
		{
			tmp[i] = He_dPdT[i];
			if (tmp[i] > 50) tmp[i] = 50;
			else if (tmp[i] < 0) tmp[i] = 0;
		}
		sprintf_s( cbuf, 64, "%3.0f   %3.0f   %3.0f", tmp[1], tmp[0], tmp[2] );
		pMDU->mvprint( 13, 17, cbuf );
		if (He_dPdT[1] > 20) pMDU->UpArrow( 16, 17, dps::DEUATT_OVERBRIGHT );
		if (He_dPdT[0] > 20) pMDU->UpArrow( 22, 17, dps::DEUATT_OVERBRIGHT );
		if (He_dPdT[2] > 20) pMDU->UpArrow( 28, 17, dps::DEUATT_OVERBRIGHT );

		tmp[0] = dipHeSysPressureSensor[9].GetVoltage() * 1000;
		if (tmp[0] > 5000) tmp[0] = 5000;
		else if (tmp[0] < 0) tmp[0] = 0;
		sprintf_s( cbuf, 64, "%4.0f", tmp[0] );
		pMDU->mvprint( 40, 19, cbuf );
		if (tmp[0] < 3800) pMDU->DownArrow( 44, 19, dps::DEUATT_OVERBRIGHT );

		tmp[0] = dipHeSysPressureSensor[10].GetVoltage() * 200;
		if (tmp[0] > 1000) tmp[0] = 1000;
		else if (tmp[0] < 0) tmp[0] = 0;
		sprintf_s( cbuf, 64, "%4.0f", tmp[0] );
		pMDU->mvprint( 40, 20, cbuf );
		if (tmp[0] < 700) pMDU->DownArrow( 44, 20, dps::DEUATT_OVERBRIGHT );
		else if (tmp[0] > 810) pMDU->UpArrow( 44, 20, dps::DEUATT_OVERBRIGHT );

		tmp[0] = dipHeSysPressureSensor[11].GetVoltage() * 200;
		if (tmp[0] > 1000) tmp[0] = 1000;
		else if (tmp[0] < 0) tmp[0] = 0;
		sprintf_s( cbuf, 64, "%4.0f", tmp[0] );
		pMDU->mvprint( 40, 21, cbuf );
		if (tmp[0] < 700) pMDU->DownArrow( 44, 21, dps::DEUATT_OVERBRIGHT );

		for (int i = 0; i < 3; i++)
		{
			tmp[i] = (dipLH2UllagePressureSensor[i].GetVoltage() * 8) + 12;

			if (tmp[i] > 52) tmp[i] = 52;
			else if (tmp[i] < 12) tmp[i] = 12;
		}
		sprintf_s( cbuf, 64, "%4.1f  %4.1f  %4.1f", tmp[1], tmp[0], tmp[2] );
		pMDU->mvprint( 12, 19, cbuf );
		if (tmp[1] < 28) pMDU->DownArrow( 16, 19, dps::DEUATT_OVERBRIGHT );
		else if (tmp[1] > 48.9) pMDU->UpArrow( 16, 19, dps::DEUATT_OVERBRIGHT );
		if (tmp[0] < 28) pMDU->DownArrow( 22, 19, dps::DEUATT_OVERBRIGHT );
		else if (tmp[0] > 48.9) pMDU->UpArrow( 22, 19, dps::DEUATT_OVERBRIGHT );
		if (tmp[2] < 28) pMDU->DownArrow( 28, 19, dps::DEUATT_OVERBRIGHT );
		else if (tmp[2] > 48.9) pMDU->UpArrow( 28, 19, dps::DEUATT_OVERBRIGHT );

		for (int i = 0; i < 3; i++)
		{
			tmp[i] = dipLO2UllagePressureSensor[i].GetVoltage() * 10;
			// HACK must convert because sensor is not outputting psig but psia
			tmp[i] -= 14.7;
			if (tmp[i] > 30) tmp[i] = 30;
			else if (tmp[i] < 0) tmp[i] = 0;
		}
		sprintf_s( cbuf, 64, "%4.1f  %4.1f  %4.1f", tmp[1], tmp[0], tmp[2] );
		pMDU->mvprint( 12, 20, cbuf );
		if (tmp[1] < 0) pMDU->DownArrow( 16, 20, dps::DEUATT_OVERBRIGHT );
		else if (tmp[1] > 28) pMDU->UpArrow( 16, 20, dps::DEUATT_OVERBRIGHT );
		if (tmp[0] < 0) pMDU->DownArrow( 22, 20, dps::DEUATT_OVERBRIGHT );
		else if (tmp[0] > 28) pMDU->UpArrow( 22, 20, dps::DEUATT_OVERBRIGHT );
		if (tmp[2] < 0) pMDU->DownArrow( 28, 20, dps::DEUATT_OVERBRIGHT );
		else if (tmp[2] > 28) pMDU->UpArrow( 28, 20, dps::DEUATT_OVERBRIGHT );

		/*sprintf_s( cbuf, 64, "%4.0f  %4.0f  %4.0f", abc, abc, abc );
		pMDU->mvprint( 12, 22, cbuf );
		sprintf_s( cbuf, 64, "%4.0f  %4.0f  %4.0f", abc, abc, abc );
		pMDU->mvprint( 12, 23, cbuf );*/

		tmp[0] = dipMPSManifPressureSensor[1].GetVoltage() * 20;
		if (tmp[0] > 100) tmp[0] = 100;
		else if (tmp[0] < 0) tmp[0] = 0;
		sprintf_s( cbuf, 64, "%3.0f", tmp[0] );
		pMDU->mvprint( 41, 22, cbuf );
		if (tmp[0] > 65) pMDU->UpArrow( 44, 22, dps::DEUATT_OVERBRIGHT );

		tmp[0] = dipMPSManifPressureSensor[0].GetVoltage() * 60;
		if (tmp[0] > 300) tmp[0] = 300;
		else if (tmp[0] < 0) tmp[0] = 0;
		sprintf_s( cbuf, 64, "%3.0f", tmp[0] );
		pMDU->mvprint( 41, 23, cbuf );
		if (tmp[0] > 249) pMDU->UpArrow( 44, 23, dps::DEUATT_OVERBRIGHT );
		return;
	}

	void GeneralDisplays::OnPaint_DISP19_BFS( vc::MDU* pMDU ) const
	{
		PrintCommonHeader( " GNC SYS SUMM 2", pMDU );
		pMDU->mvprint( 34, 1, "BFS", dps::DEUATT_OVERBRIGHT );

		// static parts (labels)
		// OMS
		pMDU->mvprint( 0, 2, "OMS AFT QTY    L     R" );
		pMDU->mvprint( 8, 3, "OXID" );
		pMDU->mvprint( 10, 4, "FU" );
		pMDU->mvprint( 4, 5, "FU INJ T" );
		pMDU->mvprint( 29, 2, "OMS" );
		pMDU->mvprint( 42, 2, "L" );
		pMDU->mvprint( 48, 2, "R" );
		pMDU->mvprint( 30, 3, "TK P   HE" );
		pMDU->mvprint( 35, 4, "OXID" );
		pMDU->mvprint( 37, 5, "FU" );
		pMDU->mvprint( 30, 6, "N2 TK   P" );
		pMDU->mvprint( 33, 7, "REG  P" );
		pMDU->mvprint( 33, 8, "P  VLV" );
		pMDU->mvprint( 29, 9, "ENG IN   P" );
		pMDU->mvprint( 35, 10, "OXID" );
		pMDU->mvprint( 37, 11, "FU" );
		pMDU->mvprint( 34, 12, "VLV 1" );
		pMDU->mvprint( 38, 13, "2" );

		// RCS
		pMDU->mvprint( 20, 6, "JETISOL" );
		pMDU->mvprint( 0, 7, "RCS       OXID  FU FAIL VLV" );
		pMDU->mvprint( 0, 8, "FWD  HE P" );
		pMDU->mvprint( 5, 9, "TK P" );
		pMDU->mvprint( 6, 10, "QTY" );
		pMDU->mvprint( 0, 11, "MANF  1 P" );
		pMDU->mvprint( 6, 12, "2 P" );
		pMDU->mvprint( 6, 13, "3 P" );
		pMDU->mvprint( 6, 14, "4 P" );
		pMDU->mvprint( 6, 15, "5" );
		pMDU->mvprint( 0, 16, "AFT  HE P" );
		pMDU->mvprint( 0, 17, "L    TK P" );
		pMDU->mvprint( 6, 18, "QTY" );
		pMDU->mvprint( 0, 19, "MANF  1 P" );
		pMDU->mvprint( 6, 20, "2 P" );
		pMDU->mvprint( 6, 21, "3 P" );
		pMDU->mvprint( 6, 22, "4 P" );
		pMDU->mvprint( 6, 23, "5" );
		pMDU->mvprint( 44, 14, "JETISOL" );
		pMDU->mvprint( 34, 15, "OXID  FU FAIL VLV" );
		pMDU->mvprint( 29, 16, "HE P" );
		pMDU->mvprint( 27, 17, "R TK P" );
		pMDU->mvprint( 30, 18, "QTY" );
		pMDU->mvprint( 30, 19, "1 P" );
		pMDU->mvprint( 30, 20, "2 P" );
		pMDU->mvprint( 30, 21, "3 P" );
		pMDU->mvprint( 30, 22, "4 P" );
		pMDU->mvprint( 30, 23, "5" );


		// static parts (lines)
		pMDU->Line( 90, 84, 90, 336 );
		pMDU->Line( 140, 84, 140, 336 );
		pMDU->Line( 190, 84, 190, 336 );
		pMDU->Line( 230, 84, 230, 336 );
		pMDU->Line( 270, 84, 270, 336 );
		pMDU->Line( 330, 196, 330, 336 );
		pMDU->Line( 380, 196, 380, 336 );
		pMDU->Line( 430, 196, 430, 336 );
		pMDU->Line( 470, 196, 470, 336 );
		pMDU->Line( 0, 84, 270, 84 );
		pMDU->Line( 0, 112, 270, 112 );
		pMDU->Line( 60, 154, 190, 154 );
		pMDU->Line( 270, 196, 510, 196 );
		pMDU->Line( 0, 224, 510, 224 );
		pMDU->Line( 60, 266, 190, 266 );
		pMDU->Line( 300, 266, 430, 266 );


		// TODO dynamic parts
		return;
	}

	void GeneralDisplays::OnPaint_SPEC51_BFS( vc::MDU* pMDU ) const
	{
		PrintCommonHeader( "    OVERRIDE", pMDU );
		pMDU->mvprint( 34, 1, "BFS", dps::DEUATT_OVERBRIGHT );

		// static parts (labels)
		// ABORT MODE
		pMDU->mvprint( 0, 2, "ABORT MODE" );
		pMDU->mvprint( 1, 3, "TAL" );
		pMDU->mvprint( 10, 3, "1" );
		pMDU->mvprint( 1, 4, "ATO" );
		pMDU->mvprint( 10, 4, "2" );
		pMDU->mvprint( 0, 5, "ABORT     3" );
		pMDU->mvprint( 0, 6, "THROT MAX 4" );
		pMDU->mvprint( 6, 7, "ABT 50" );
		pMDU->mvprint( 6, 8, "NOM 51" );

		// ENTRY FCS
		pMDU->mvprint( 29, 2, "ENTRY FCS" );
		pMDU->mvprint( 18, 3, "ELEVON    FILTER    ATMOSPHERE" );
		pMDU->mvprint( 16, 4, "AUTO  17    NOM 20    NOM    22" );
		pMDU->mvprint( 16, 5, "FIXED 18    ALT 21    N POLE 23" );
		pMDU->mvprint( 18, 6, "SSME REPOS 19       S POLE 24" );

		// IMU
		pMDU->mvprint( 17, 8, "IMU DES ATT" );
		pMDU->mvprint( 18, 9, "1  25" );
		pMDU->mvprint( 18, 10, "2  26" );
		pMDU->mvprint( 18, 11, "3  27" );

		// AA RGA SURF
		pMDU->mvprint( 33, 8, "AA    RGA  SURF" );
		pMDU->mvprint( 29, 9, "LRU DES   DES  DES" );
		pMDU->mvprint( 30, 10, "1  31" );
		pMDU->mvprint( 39, 10, "35   39" );
		pMDU->mvprint( 30, 11, "2  32" );
		pMDU->mvprint( 39, 11, "36   40" );
		pMDU->mvprint( 30, 12, "3  33" );
		pMDU->mvprint( 39, 12, "37   41" );
		pMDU->mvprint( 30, 13, "4  34" );
		pMDU->mvprint( 39, 13, "38   42" );

		// PRPLT DUMP
		pMDU->mvprint( 3, 9, "PRPLT DUMP" );
		pMDU->mvprint( 4, 10, "ICNCT 5" );
		pMDU->mvprint( 0, 11, "OMS DUMP" );
		pMDU->mvprint( 3, 12, "ARM    6" );
		pMDU->mvprint( 3, 13, "START  7" );
		pMDU->mvprint( 3, 14, "STOP   8" );
		pMDU->mvprint( 1, 15, "9 QUAN/SIDE" );
		pMDU->mvprint( 0, 16, "OMS DUMP TTG" );

		// ET SEP
		pMDU->mvprint( 18, 13, "ET SEP" );
		pMDU->mvprint( 20, 14, "AUTO  28" );
		pMDU->mvprint( 20, 15, "SEP   29" );

		// ROLL MODE
		pMDU->mvprint( 34, 15, "ROLL MODE" );
		pMDU->mvprint( 35, 16, "WRAP MODE 45" );

		// ET UMB DR
		pMDU->mvprint( 18, 17, "ET UMB  DR" );
		pMDU->mvprint( 20, 18, "CLOSE 30" );

		// AFT RCS
		pMDU->mvprint( 0, 18, "AFT RCS 13" );
		pMDU->mvprint( 3, 19, "14 TIME" );

		// COMM
		pMDU->mvprint( 39, 18, "COMM" );
		pMDU->mvprint( 40, 19, "TDRS    46" );
		pMDU->mvprint( 40, 20, "STDN-HI 47" );
		pMDU->mvprint( 40, 21, "STDN-LO 48" );
		pMDU->mvprint( 40, 22, "SGLS    49" );

		// VENT DOOR CNTL
		pMDU->mvprint( 18, 20, "VENT DOOR CNTL" );
		pMDU->mvprint( 20, 21, "OPEN  43" );
		pMDU->mvprint( 20, 22, "CLOSE 44" );

		// FWD RCS
		pMDU->mvprint( 0, 21, "FWD RCS 15" );
		pMDU->mvprint( 3, 22, "16 TIME" );


		// TODO dynamic parts
		return;
	}

	void GeneralDisplays::OnPaint_SPEC55_BFS( vc::MDU* pMDU ) const
	{
		PrintCommonHeader( "    GPS STATUS", pMDU );
		pMDU->mvprint( 34, 1, "BFS", dps::DEUATT_OVERBRIGHT );
		
		// static parts (labels)
		pMDU->mvprint( 9, 2, "GPS1  GPS2  GPS3" );
		pMDU->mvprint( 0, 3, "STAT" );
		pMDU->mvprint( 0, 4, "MODE" );
		pMDU->mvprint( 0, 6, "INIT" );
		pMDU->mvprint( 10, 6, "14" );
		pMDU->mvprint( 16, 6, "15" );
		pMDU->mvprint( 22, 6, "16" );
		pMDU->mvprint( 0, 8, "RESTART   20    21    22" );

		pMDU->mvprint( 33, 3, "GPS MINUS NAV" );
		pMDU->Delta( 28, 4 );
		pMDU->mvprint( 29, 4, "H" );
		pMDU->Delta( 40, 4 );
		pMDU->mvprint( 41, 4, "H" );
		pMDU->DotCharacter( 41, 4 );
		pMDU->Delta( 28, 5 );
		pMDU->mvprint( 29, 5, "DR" );
		pMDU->Delta( 40, 5 );
		pMDU->mvprint( 41, 5, "DR" );
		pMDU->DotCharacter( 41, 5 );
		pMDU->Delta( 28, 6 );
		pMDU->mvprint( 29, 6, "CR" );
		pMDU->Delta( 40, 6 );
		pMDU->mvprint( 41, 6, "CR" );
		pMDU->DotCharacter( 41, 6 );
		pMDU->mvprint( 30, 8, "LAT" );
		pMDU->mvprint( 38, 8, "LON" );
		pMDU->mvprint( 46, 8, "ALT" );

		pMDU->mvprint( 0, 11, "DG FAIL" );
		pMDU->mvprint( 0, 12, "DES RCVR  26    27    28" );
		pMDU->mvprint( 0, 13, "QA1 OVRD  29    30    31" );
		pMDU->mvprint( 0, 15, "QA1 P 1" );
		pMDU->Sigma( 7, 15 );

		pMDU->mvprint( 28, 15, "GPS TIME ADJUST ENA 39" );

		pMDU->mvprint( 34, 16, "SATELLITES" );
		pMDU->mvprint( 28, 17, "TRKD  C1 C2 C3 C4 C5 C6" );
		pMDU->mvprint( 29, 18, "GPS1" );
		pMDU->mvprint( 29, 19, "GPS2" );
		pMDU->mvprint( 29, 20, "GPS3" );
		pMDU->mvprint( 28, 22, "DES 43" );

		
		// static parts (lines)
		pMDU->Line( 140, 28, 140, 336 );
		pMDU->Line( 200, 28, 200, 336 );
		pMDU->Line( 260, 28, 260, 336 );
		pMDU->Line( 0, 154, 510, 154 );
		pMDU->Line( 260, 224, 510, 224 );
		pMDU->Line( 0, 280, 260, 280 );


		// TODO dynamic parts
		return;
	}

	void GeneralDisplays::OnPaint_DISP99_BFS( vc::MDU* pMDU ) const
	{
		PrintCommonHeader( "    FAULT", pMDU );
		pMDU->mvprint( 34, 1, "BFS", dps::DEUATT_OVERBRIGHT );

		// static parts (labels)
		pMDU->mvprint( 4, 5, "CRT" );
		pMDU->mvprint( 13, 5, "FAULT      C/W   GPC      TIME" );
		pMDU->mvprint( 4, 6, "ID" );
		return;
	}

	void GeneralDisplays::OnPaint_LAUNCHTRAJ1_PASS( vc::MDU* pMDU ) const// OI-32 PASS LAUNCH TRAJ 1
	{
		PrintCommonHeader( "  LAUNCH TRAJ 1", pMDU );
	
		// static parts (labels)
		pMDU->mvprint( 7, 5, "CONT ABORT" );
		pMDU->mvprint( 8, 6, "3EO BLUE" );
		pMDU->mvprint( 1, 7, "2  ARM 2EO BLUE" );
		pMDU->mvprint( 1, 8, "4  ABORT" );
		pMDU->mvprint( 1, 10, "5 INH YAW STEER" );
		pMDU->mvprint( 1, 12, "6  SERC" );
		pMDU->mvprint( 38, 17, "THROT" );
		pMDU->mvprint( 18, 22, "7 INH DRP 1EO" );

		pMDU->mvprint( 31, 15, "40" );
		pMDU->mvprint( 24, 18, "50" );
		pMDU->mvprint( 19, 20, "60" );
		pMDU->mvprint( 11, 23, "70" );

		// static parts (lines)
		pMDU->Line( 78, 324, 116, 317 );
		pMDU->Line( 116, 317, 159, 302 );
		pMDU->Line( 159, 302, 217, 257 );
		pMDU->Line( 217, 257, 355, 174 );
		pMDU->Line( 355, 174, 438, 119 );
		pMDU->Line( 438, 119, 455, 97 );
		pMDU->Line( 455, 97, 471, 58 );

		pMDU->Line( 322, 189, 322, 200 );
		pMDU->Line( 254, 230, 254, 241 );
		pMDU->Line( 196, 267, 196, 278 );
		pMDU->Line( 116, 313, 116, 324 );
		return;
	}
	
	void GeneralDisplays::OnPaint_ASCENTTRAJ1_PASS( vc::MDU* pMDU ) const// OI-32 PASS ASCENT TRAJ 1
	{
		PrintCommonHeader( "  ASCENT TRAJ 1", pMDU );
	
		// static parts (labels)
		pMDU->mvprint( 7, 5, "CONT ABORT" );
		pMDU->mvprint( 8, 6, "3EO BLUE" );
		pMDU->mvprint( 1, 7, "2  ARM 2EO BLUE" );
		pMDU->mvprint( 1, 8, "4  ABORT" );
		pMDU->mvprint( 1, 10, "5 INH YAW STEER" );
		pMDU->mvprint( 1, 12, "6  SERC" );
		pMDU->mvprint( 38, 17, "THROT" );
		pMDU->mvprint( 18, 22, "7 INH DRP 1EO" );

		pMDU->mvprint( 31, 15, "40" );
		pMDU->mvprint( 24, 18, "50" );
		pMDU->mvprint( 19, 20, "60" );
		pMDU->mvprint( 11, 23, "70" );

		// static parts (lines)
		pMDU->Line( 78, 324, 116, 317 );
		pMDU->Line( 116, 317, 159, 302 );
		pMDU->Line( 159, 302, 217, 257 );
		pMDU->Line( 217, 257, 355, 174 );
		pMDU->Line( 355, 174, 438, 119 );
		pMDU->Line( 438, 119, 455, 97 );
		pMDU->Line( 455, 97, 471, 58 );

		pMDU->Line( 322, 189, 322, 200 );
		pMDU->Line( 254, 230, 254, 241 );
		pMDU->Line( 196, 267, 196, 278 );
		pMDU->Line( 116, 313, 116, 324 );

		// dynamic parts
		char cbuf[64];

		sprintf_s( cbuf, 64, "%3d", (int)pAscentDAP->GetThrottleCommand() );
		pMDU->mvprint( 44, 17, cbuf );

		if (pSRBSepSequence->GetPC50Flag() == true) pMDU->mvprint( 22, 9, "PC<50", dps::DEUATT_OVERBRIGHT | dps::DEUATT_FLASHING );

		//if (pSRBSepSequence->GetSRBSEPINHFlag() == true) pMDU->mvprint( 10, 11, "SEP INH" );

		if (pAscentDAP->SERCenabled() == true) pMDU->mvprint( 9, 12, "ON", dps::DEUATT_OVERBRIGHT );

		if (pAscentDAP->GetEOVI( 1 ) != 0)
		{
			sprintf_s( cbuf, 64, "1ST EO VI %5.0f", pAscentDAP->GetEOVI( 1 ) );
			pMDU->mvprint( 34, 22, cbuf );
		}
		if (pAscentDAP->GetEOVI( 2 ) != 0)
		{
			sprintf_s( cbuf, 64, "2ND EO VI %5.0f", pAscentDAP->GetEOVI( 2 ) );
			pMDU->mvprint( 34, 23, cbuf, dps::DEUATT_OVERBRIGHT );
		}

		double VR = STS()->GetAirspeed() * MPS2FPS;
		double Altitude = STS()->GetAltitude() * MPS2FPS;

		//Draw triangle for state vector
		short stY = static_cast<short>(324 - (Altitude * 0.00164198));
		short stX = static_cast<short>(78 + (VR * 0.0930909));
		pMDU->Line( stX, stY - 6, stX - 6, stY + 6, dps::DEUATT_OVERBRIGHT );
		pMDU->Line( stX - 6, stY + 6, stX + 6, stY + 6, dps::DEUATT_OVERBRIGHT );
		pMDU->Line( stX + 6, stY + 6, stX, stY - 6, dps::DEUATT_OVERBRIGHT );

		VECTOR3 thr;
		STS()->GetThrustVector( thr );
		thr /= STS()->GetMass();
		VECTOR3 LVLH_Vel;
		STS()->GetGPCLVLHVel(0, LVLH_Vel);
		double AOA = STS()->GetAOA();
		double Pitch = STS()->GetPitch();
		// 20s predictor
		VR += (length( thr ) * cos( -AOA - atan( thr.y / thr.z ) ) - G * sin( PI - Pitch - AOA )) * 20 * MPS2FPS;
		Altitude += -LVLH_Vel.z * 20 + ((length( thr ) * sin( Pitch + atan( thr.y / thr.z ) ) - G) * 400) * MPS2FPS;
		if (VR < 0) VR = 0;
		if (Altitude < 0) Altitude = 0;
		stY = static_cast<short>(324 - (Altitude * 0.00164198));
		stX = static_cast<short>(78 + (VR * 0.0930909));
		pMDU->Ellipse( stX - 6, stY - 6, stX + 6, stY + 6, dps::DEUATT_OVERBRIGHT );
		return;
	}

	void GeneralDisplays::OnPaint_ASCENTTRAJ2_PASS( vc::MDU* pMDU ) const// OI-32 PASS ASCENT TRAJ 2
	{
		PrintCommonHeader( "  ASCENT TRAJ 2", pMDU );
	
		// static parts (labels)
		pMDU->mvprint( 9, 4, "25" );
		pMDU->mvprint( 49, 4, "26" );
		pMDU->mvprint( 38, 5, "TGO   :" );
		pMDU->mvprint( 7, 5, "CONT ABORT" );
		pMDU->mvprint( 8, 6, "3EO" );
		pMDU->mvprint( 1, 7, "2  ARM 2EO" );
		pMDU->mvprint( 1, 8, "4  ABORT" );
		pMDU->mvprint( 1, 10, "5 INH YAW STEER" );
		pMDU->mvprint( 1, 12, "6  SERC" );
		pMDU->mvprint( 38, 17, "THROT" );
		pMDU->mvprint( 38, 18, "PRPLT" );
		pMDU->mvprint( 38, 20, "TMECO   :" );
		pMDU->mvprint( 19, 19, "DROOP ALT   8" );
		pMDU->mvprint( 18, 22, "7     DRP 1EO" );


		// static parts (lines)
		//Nominal ascent line
		pMDU->Line( 116, 289, 126, 257 );
		pMDU->Line( 126, 257, 136, 236 );
		pMDU->Line( 136, 236, 146, 218 );
		pMDU->Line( 146, 218, 166, 196 );
		pMDU->Line( 166, 196, 196, 179 );
		pMDU->Line( 196, 179, 212, 173 );
		pMDU->Line( 212, 173, 242, 168 );
		pMDU->Line( 242, 168, 272, 168 );
		pMDU->Line( 272, 168, 376, 177 );
		pMDU->Line( 376, 177, 450, 180 );
		pMDU->Line( 450, 180, 496, 179 );

		pMDU->Line( 270, 151, 322, 159 );
		pMDU->Line( 322, 159, 388, 166 );
		pMDU->Line( 388, 166, 468, 166 );

		// Vr line
		pMDU->Line( 100, 48, 498, 48 );
		// 25K mark
		pMDU->Line( 100, 48, 100, 56 );
		// 26K mark
		pMDU->Line( 498, 48, 498, 56 );

		// dynamic parts
		char cbuf[64];
		int tmp = 0;
		double TgtSpd = pAscentDAP->GetTgtSpd();
		double inertialVelocity = pAscentDAP->GetInertialVelocity();

		// Vr scale
		if (((TgtSpd * MPS2FPS) > 25000) && ((TgtSpd * MPS2FPS) < 26000))// TODO update for MECO speeds > 26K???
		{
			// CO mark
			tmp = Round( ((((TgtSpd * MPS2FPS) - 25000)) * 0.398) ) + 100;
			pMDU->Line( tmp, 48, tmp, 56 );
			tmp = Round( tmp * 0.1 ) - 1;
			pMDU->mvprint( tmp, 4, "CO" );
		}
		if ((inertialVelocity * MPS2FPS) < 25000) tmp = 100;
		else if ((inertialVelocity * MPS2FPS) > 26000) tmp = 498;
		else tmp = Round( ((((inertialVelocity * MPS2FPS) - 25000)) * 0.398) ) + 100;
		pMDU->Line( tmp, 47, tmp - 6, 39, dps::DEUATT_OVERBRIGHT );
		pMDU->Line( tmp - 6, 39, tmp + 6, 39, dps::DEUATT_OVERBRIGHT );
		pMDU->Line( tmp + 6, 39, tmp, 47, dps::DEUATT_OVERBRIGHT );

		sprintf_s( cbuf, 64, "%3d", (int)pAscentDAP->GetThrottleCommand() );
		pMDU->mvprint( 44, 17, cbuf );

		tmp = STS()->GetETPropellant();
		if (tmp < 0) tmp = 0;
		sprintf_s( cbuf, 64, "%2d", tmp );
		pMDU->mvprint( 44, 18, cbuf );

		// contigency abort boundaries
		// 3EO
		if ((inertialVelocity * MPS2FPS) < 18000) pMDU->mvprint( 12, 6, "GREEN" );
		// 2EO
		if ((inertialVelocity * MPS2FPS) < 12100)
		{
			VECTOR3 vr3;
			STS()->GetHorizonAirspeedVector( vr3 );
			if ((vr3.y * MPS2FPS) < 1850) pMDU->mvprint( 12, 7, "GREEN" );
			else pMDU->mvprint( 13, 7, "BLUE" );
		}

		//if (pETSepSequence->GetETSEPINHFlag() == true) pMDU->mvprint( 10, 11, "SEP INH" );

		if (pAscentDAP->SERCenabled() == true) pMDU->mvprint( 9, 12, "ON", dps::DEUATT_OVERBRIGHT );

		if (pAscentDAP->GetEOVI( 1 ) != 0)
		{
			sprintf_s( cbuf, 64, "1ST EO VI %5.0f", pAscentDAP->GetEOVI( 1 ) );
			pMDU->mvprint( 34, 22, cbuf );
		}
		if (pAscentDAP->GetEOVI( 2 ) != 0)
		{
			sprintf_s( cbuf, 64, "2ND EO VI %5.0f", pAscentDAP->GetEOVI( 2 ) );
			pMDU->mvprint( 34, 23, cbuf, dps::DEUATT_OVERBRIGHT );
		}
		else pMDU->mvprint( 20, 22, "INH" );

		if ((pSSME_Operations->GetMECOConfirmedFlag() == false) && (pSSME_Operations->GetMECOCommandFlag() == false))
		{
			// TGO
			double timeRemaining = pAscentDAP->GetTimeRemaining();
			tmp = Round( timeRemaining );
			sprintf_s( cbuf, 64, "%2d", (tmp - (tmp % 60)) / 60 );
			pMDU->mvprint( 42, 5, cbuf );
			sprintf_s( cbuf, 64, "%02d", (tmp % 60) );
			pMDU->mvprint( 45, 5, cbuf );
			// TMECO
			tmp = Round( STS()->GetMET() + timeRemaining );
			sprintf_s( cbuf, 64, "%2d", (tmp - (tmp % 60)) / 60 );
			pMDU->mvprint( 44, 20, cbuf );
			sprintf_s( cbuf, 64, "%02d", (tmp % 60) );
			pMDU->mvprint( 47, 20, cbuf );

			double droopH = DROOP_ALT;// TODO
			char att = 0;
			if (droopH < DROOP_ALT) att = dps::DEUATT_FLASHING;
			sprintf_s( cbuf, 64, "DROOP ALT %3.0f", droopH * 0.001 );
			pMDU->mvprint( 19, 19, cbuf, att );
		}

		VECTOR3 LVLH_Vel;
		STS()->GetGPCLVLHVel(0, LVLH_Vel);

		//Current vehicle state (57NM insertion altitude):
		double VHI = LVLH_Vel.x;
		double Altitude = STS()->GetAltitude() * MPS2FPS;
		//Draw triangle for state vector
		short stY = static_cast<short>(490.558404 - (Altitude * 0.000997152));
		short stX = static_cast<short>(VHI * 0.0204);
		pMDU->Line( stX, stY - 6, stX - 6, stY + 6, dps::DEUATT_OVERBRIGHT );
		pMDU->Line( stX - 6, stY + 6, stX + 6, stY + 6, dps::DEUATT_OVERBRIGHT );
		pMDU->Line( stX + 6, stY + 6, stX, stY - 6, dps::DEUATT_OVERBRIGHT );
		
		// HACK using constant 12º for SSME offset
		// 30s predictor
		double earthR = 20902200;//6371010 * MPS2FPS;
		double thrustAcceleration = pAscentDAP->GetThrustAcceleration();
		double dv30 = thrustAcceleration * cos( STS()->GetSlipAngle() ) * 30 * MPS2FPS;
		VHI += dv30;
		Altitude += -LVLH_Vel.z * 30 + (((thrustAcceleration * sin( STS()->GetPitch() - (12 * RAD * sign( cos( STS()->GetBank() ) )) )) - G ) * 450 * MPS2FPS);
		//Draw circle for 30s predictor
		//stY = static_cast<short>(315.358974 - (Altitude * 0.000641026));
		stY = static_cast<short>(490.558404 - ((Altitude + sqrt(earthR * earthR + VHI * VHI * 900) - earthR) * 0.000997152));
		stX = static_cast<short>(VHI * 0.0204);
		pMDU->Ellipse( stX - 6, stY - 6, stX + 6, stY + 6, dps::DEUATT_OVERBRIGHT );

		// 60s predictor
		VHI += dv30;
		Altitude = (STS()->GetAltitude() * MPS2FPS) - (LVLH_Vel.z * 60) + ((thrustAcceleration * sin( STS()->GetPitch() - (12 * RAD * sign( cos( STS()->GetBank() ) )) )) - G ) * 1800 * MPS2FPS;
		//Draw circle for 60s predictor
		//stY = static_cast<short>(315.358974 - (Altitude * 0.000641026));
		stY = static_cast<short>(490.558404 - ((Altitude + sqrt(earthR * earthR + VHI * VHI * 3600) - earthR) * 0.000997152));
		stX = static_cast<short>(VHI * 0.0204);
		pMDU->Ellipse( stX - 6, stY - 6, stX + 6, stY + 6, dps::DEUATT_OVERBRIGHT );
		return;
	}

	// INFO below is the pre OI-32 PASS ASCENT TRAJ display
	//void GeneralDisplays::OnPaint_ASCENTTRAJ_PASS( vc::MDU* pMDU ) const
	//{
	//	// PASS LAUNCH TRJ/PASS ASCENT TRAJ
	//	switch(GetMajorMode())
	//	{
	//		case 101:
	//			PrintCommonHeader( "  LAUNCH TRAJ", pMDU );
	//			break;
	//		case 102:
	//		case 103:
	//			PrintCommonHeader( "  ASCENT TRAJ", pMDU );
	//			break;
	//	}
	//
	//	// static parts (labels)
	//	pMDU->mvprint( 9, 5, "CO" );
	//	pMDU->mvprint( 25, 5, "PD" );
	//	pMDU->mvprint( 32, 5, "PD3" );
	//
	//	pMDU->mvprint( 36, 6, "ABORT    ARM" );
	//	pMDU->mvprint( 33, 7, "3 E/O" );
	//	pMDU->mvprint( 33, 8, "2 E/O" );
	//	pMDU->mvprint( 46, 8, "2" );
	//	pMDU->mvprint( 31, 9, "ABORT      4" );
	//	pMDU->mvprint( 31, 10, "YAW STEER  5" );
	//
	//	pMDU->mvprint( 10, 7, "GUID" );
	//	pMDU->mvprint( 10, 8, "TMECO   :" );
	//	pMDU->mvprint( 10, 9, "PRPLT" );
	//	pMDU->mvprint( 7, 12, "6  SERC" );
	//
	//	pMDU->mvprint( 5, 13, "O" );
	//
	//	pMDU->mvprint( 46, 13, "GO" );
	//	pMDU->mvprint( 39, 15, "RTLS" );
	//
	//
	//	// static parts (lines)
	//
	//	//Nominal ascent line
	//	pMDU->Line( 181, 212, 191, 176 );
	//	pMDU->Line( 191, 176, 233, 128 );
	//	pMDU->Line( 233, 128, 255, 117 );
	//
	//	//EO at Lift-Off line
	//	pMDU->Line( 159, 212, 174, 138 );
	//	pMDU->Line( 174, 138, 78, 195 );
	//	pMDU->Line( 78, 195, 58, 201 );
	//	pMDU->Line( 58, 201, 21, 176 );
	//
	//	pMDU->Line( 183, 119, 176, 122 );
	//	pMDU->Line( 176, 122, 81, 187 );
	//
	//	//Q = 2 line
	//	pMDU->Line( 38, 187, 11, 179 );
	//
	//	//Q = 10 line
	//	pMDU->Line( 38, 171, 11, 157 );
	//
	//	//Hdot indicator
	//	pMDU->Line( 17, 30, 11, 30 );
	//	pMDU->Line( 11, 30, 11, 206 );
	//	pMDU->Line( 11, 206, 17, 206 );
	//	pMDU->Line( 11, 118, 17, 118 );
	//
	//	//DR indicator
	//	pMDU->Line( 26, 27, 247, 27 );
	//
	//	//PD3 Mark
	//	pMDU->Line( 164, 27, 164, 35 );
	//
	//	//PD Mark
	//	pMDU->Line( 128, 27, 128, 35 );
	//
	//	//CO Mark
	//	pMDU->Line( 47, 27, 47, 35 );
	//
	//
	//	// dynamic parts
	//	char cbuf[64];
	//	int tmp = 0;
	//
	//	if ((GetMajorMode() == 103) && (pSSME_Operations->GetMECOConfirmedFlag() == false))
	//	{
	//		tmp = Round( STS()->GetMET() + timeRemaining );
	//		sprintf_s( cbuf, 64, "%02d", (tmp - (tmp % 60)) / 60 );
	//		pMDU->mvprint( 16, 8, cbuf );
	//		sprintf_s( cbuf, 64, "%02d", (tmp % 60) );
	//		pMDU->mvprint( 19, 8, cbuf );
	//	}
	//
	//	tmp = STS()->GetETPropellant();
	//	if (tmp < 0) tmp = 0;
	//	sprintf_s( cbuf, 64, "%2d", tmp );
	//	pMDU->mvprint( 19, 9, cbuf );
	//
	//	if ((pSRBSepSequence->GetPC50Flag() == true) && (GetMajorMode() == 102)) pMDU->mvprint( 10, 10, "PC<50" );
	//
	//	if ((pSRBSepSequence->GetSRBSEPINHFlag() == true) || (pETSepSequence->GetETSEPINHFlag() == true)) pMDU->mvprint( 10, 11, "SEP INH" );
	//
	//	if (enaSERC == true) pMDU->mvprint( 15, 12, "ON", dps::DEUATT_OVERBRIGHT );
	//
	//	if (EOVI[0] != 0)
	//	{
	//		sprintf_s( cbuf, 64, "EO VI %5.0f", EOVI[0] );
	//		pMDU->mvprint( 7, 13, cbuf );
	//	}
	//	if (EOVI[1] != 0)
	//	{
	//		sprintf_s( cbuf, 64, "EO VI %5.0f", EOVI[1] );
	//		pMDU->mvprint( 7, 14, cbuf, dps::DEUATT_OVERBRIGHT );
	//	}
	//
	//	VECTOR3 LVLH_Vel;
	//	STS()->GetGPCLVLHVel(0, LVLH_Vel);
	//
	//	double Ref_hdot = 0.0;
	//	bool bShowHDot = (STS()->GetGPCRefHDot(0, Ref_hdot) == VARSTATE_OK);
	//
	//	//Hdot indicator
	//	if(bShowHDot)
	//	{
	//		double HDot_Error = -LVLH_Vel.z - Ref_hdot;
	//		char att = dps::DEUATT_OVERBRIGHT;
	//
	//		if(HDot_Error > 200.0)
	//		{
	//			HDot_Error = 200.0;
	//			att += dps::DEUATT_FLASHING;
	//		}
	//		else if (HDot_Error < -200.0)
	//		{
	//			HDot_Error = -200.0;
	//			att += dps::DEUATT_FLASHING;
	//		}
	//
	//		short sHDot_pry = -static_cast<short>(HDot_Error/200.0 * 88);
	//
	//		pMDU->Line( 11, 118 + sHDot_pry, 6, 113 + sHDot_pry, att );
	//		pMDU->Line( 6, 113 + sHDot_pry, 6, 123 + sHDot_pry, att );
	//		pMDU->Line( 6, 123 + sHDot_pry, 11, 118 + sHDot_pry, att );
	//	}
	//
	//	if (GetMajorMode() >= 103)// HACK because thrustAcceleration is only calculated in MM103
	//	{
	//		//Current vehicle state:
	//		double VHI = LVLH_Vel.x;
	//		double Altitude = STS()->GetAltitude() * MPS2FPS;
	//
	//		if(Altitude > 155500 && VHI < 10000)
	//		{
	//			//Draw triangle for state vector
	//			short stY = static_cast<short>(255*(1.13256 - (Altitude/513955.985)));
	//			short stX = static_cast<short>(255*(0.36194 + (VHI/15672.3964)));
	//			pMDU->Line( stX, stY - 3, stX - 3, stY + 3 );
	//			pMDU->Line( stX - 3, stY + 3, stX + 3, stY + 3 );
	//			pMDU->Line( stX + 3, stY + 3, stX, stY - 3 );
	//		}
	//		
	//		// HACK using constant 12º for SSME offset
	//		// 30s predictor
	//		double dv30 = thrustAcceleration * cos( STS()->GetSlipAngle() ) * 30 * MPS2FPS;
	//		VHI += dv30;
	//		Altitude += -LVLH_Vel.z * 30 + (((thrustAcceleration * sin( STS()->GetPitch() - (12 * RAD * sign( cos( STS()->GetBank() ) )) )) - G ) * 450 * MPS2FPS);
	//
	//		if(Altitude > 155500 && VHI < 10000)
	//		{
	//			//Draw circle for 30s predictor
	//			short stY = static_cast<short>(255*(1.13256 - (Altitude/513955.985)));
	//			short stX = static_cast<short>(255*(0.36194 + (VHI/15672.3964)));
	//			pMDU->Ellipse( stX - 3, stY - 3, stX + 3, stY + 3 );
	//		}
	//
	//		// 60s predictor
	//		VHI += dv30;
	//		Altitude = (STS()->GetAltitude() * MPS2FPS) - (LVLH_Vel.z * 60) + ((thrustAcceleration * sin( STS()->GetPitch() - (12 * RAD * sign( cos( STS()->GetBank() ) )) )) - G ) * 1800 * MPS2FPS;
	//
	//		if(Altitude > 155500 && VHI < 10000)
	//		{
	//			//Draw circle for 60s predictor
	//			short stY = static_cast<short>(255*(1.13256 - (Altitude/513955.985)));
	//			short stX = static_cast<short>(255*(0.36194 + (VHI/15672.3964)));
	//			pMDU->Ellipse( stX - 3, stY - 3, stX + 3, stY + 3 );
	//		}
	//	}
	//	return;
	//}
};
