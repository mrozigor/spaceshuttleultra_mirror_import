#include "GeneralDisplays.h"
#include "IDP.h"


namespace dps
{
	GeneralDisplays::GeneralDisplays( SimpleGPCSystem* _gpc ):SimpleGPCSoftware( _gpc, "GeneralDisplays" )
	{
		He_T = 0;
		return;
	}

	GeneralDisplays::~GeneralDisplays()
	{
	}

	void GeneralDisplays::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "BFCCRT", 3 );
		dipBFCCRTDisplay.Connect( pBundle, 0 );// ON
		dipBFCCRTSelect[0].Connect( pBundle, 1 ); // 3+1
		dipBFCCRTSelect[1].Connect( pBundle, 2 ); // 1+2

		pBundle = BundleManager()->CreateBundle( "ET_LOX_SENSORS", 16 );
		dipLO2UllagePressureSensor[0].Connect( pBundle, 12 );
		dipLO2UllagePressureSensor[1].Connect( pBundle, 13 );
		dipLO2UllagePressureSensor[2].Connect( pBundle, 14 );
		dipLO2UllagePressureSensor[3].Connect( pBundle, 15 );

		pBundle = BundleManager()->CreateBundle( "ET_LH2_SENSORS", 16 );
		dipLH2UllagePressureSensor[0].Connect( pBundle, 12 );
		dipLH2UllagePressureSensor[1].Connect( pBundle, 13 );
		dipLH2UllagePressureSensor[2].Connect( pBundle, 14 );
		dipLH2UllagePressureSensor[3].Connect( pBundle, 15 );

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
		// TODO SPEC item input
		return false;
	}

	bool GeneralDisplays::OnPaint( int spec, vc::MDU* pMDU ) const
	{
		if (spec == dps::MODE_UNDEFINED) return false;

		// HACK determine which display to use based on the position of the BFC CRT switches (BFS display only available in CRTx)
		bool outputBFSdisplay = false;

		if (pMDU->GetIdentifier() == "CRT1")
		{
			if ((dipBFCCRTDisplay.IsSet() == true) && (dipBFCCRTSelect[1].IsSet() == true)) outputBFSdisplay = true;
		}
		else if (pMDU->GetIdentifier() == "CRT2")
		{
			if ((dipBFCCRTDisplay.IsSet() == true) && (dipBFCCRTSelect[0].IsSet() == false) && (dipBFCCRTSelect[1].IsSet() == false)) outputBFSdisplay = true;
		}
		else if (pMDU->GetIdentifier() == "CRT3")
		{
			if ((dipBFCCRTDisplay.IsSet() == true) && (dipBFCCRTSelect[0].IsSet() == true)) outputBFSdisplay = true;
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
							DISP99_PASS( pMDU );// FAULT
							return true;
						default:
							return false;
					}
				case 1:
					switch (spec)
					{
						case 18:
							DISP18_PASS( pMDU );// GNC SYS SUMM 1
							return true;
						case 51:
							SPEC51_PASS( pMDU );// OVERRIDE
							return true;
						case 53:
							SPEC53_PASS( pMDU );// CONTROLS
							return true;
						case 55:
							SPEC55_PASS( pMDU );// GPS STATUS
							return true;
						case 99:
							DISP99_PASS( pMDU );// FAULT
							return true;
						default:
							return false;
					}
				case 2:
					switch (spec)
					{
						case 18:
							DISP18_PASS( pMDU );// GNC SYS SUMM 1
							return true;
						case 19:
							DISP19_PASS( pMDU );// GNC SYS SUMM 2
							return true;
						case 55:
							SPEC55_PASS( pMDU );// GPS STATUS
							return true;
						case 99:
							DISP99_PASS( pMDU );// FAULT
							return true;
						default:
							return false;
					}
				case 3:
					switch (spec)
					{
						case 18:
							DISP18_PASS( pMDU );// GNC SYS SUMM 1
							return true;
						case 51:
							SPEC51_PASS( pMDU );// OVERRIDE
							return true;
						case 53:
							SPEC53_PASS( pMDU );// CONTROLS
							return true;
						case 55:
							SPEC55_PASS( pMDU );// GPS STATUS
							return true;
						case 99:
							DISP99_PASS( pMDU );// FAULT
							return true;
						default:
							return false;
					}
				case 6:
					switch (spec)
					{
						case 18:
							DISP18_PASS( pMDU );// GNC SYS SUMM 1
							return true;
						case 51:
							SPEC51_PASS( pMDU );// OVERRIDE
							return true;
						case 53:
							SPEC53_PASS( pMDU );// CONTROLS
							return true;
						case 55:
							SPEC55_PASS( pMDU );// GPS STATUS
							return true;
						case 99:
							DISP99_PASS( pMDU );// FAULT
							return true;
						default:
							return false;
					}
				case 8:
					switch (spec)
					{
						case 18:
							DISP18_PASS( pMDU );// GNC SYS SUMM 1
							return true;
						case 19:
							DISP19_PASS( pMDU );// GNC SYS SUMM 2
							return true;
						case 55:
							SPEC55_PASS( pMDU );// GPS STATUS
							return true;
						case 99:
							DISP99_PASS( pMDU );// FAULT
							return true;
						default:
							return false;
					}
				case 9:
					switch (spec)
					{
						case 55:
							SPEC55_PASS( pMDU );// GPS STATUS
							return true;
						case 99:
							DISP99_PASS( pMDU );// FAULT
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
							DISP99_BFS( pMDU );// FAULT
							return true;
						default:
							return false;
					}
				case 1:
					switch (spec)
					{
						case 18:
							DISP18_BFS( pMDU );// GNC SYS SUMM 1
							return true;
						case 19:
							DISP19_BFS( pMDU );// GNC SYS SUMM 2
							return true;
						case 51:
							SPEC51_BFS( pMDU );// OVERRIDE
							return true;
						case 55:
							SPEC55_BFS( pMDU );// GPS STATUS
							return true;
						case 99:
							DISP99_BFS( pMDU );// FAULT
							return true;
						default:
							return false;
					}
				case 3:
					switch (spec)
					{
						case 18:
							DISP18_BFS( pMDU );// GNC SYS SUMM 1
							return true;
						case 19:
							DISP19_BFS( pMDU );// GNC SYS SUMM 2
							return true;
						case 51:
							SPEC51_BFS( pMDU );// OVERRIDE
							return true;
						case 55:
							SPEC55_BFS( pMDU );// GPS STATUS
							return true;
						case 99:
							DISP99_BFS( pMDU );// FAULT
							return true;
						default:
							return false;
					}
				case 6:
					switch (spec)
					{
						case 18:
							DISP18_BFS( pMDU );// GNC SYS SUMM 1
							return true;
						case 19:
							DISP19_BFS( pMDU );// GNC SYS SUMM 2
							return true;
						case 51:
							SPEC51_BFS( pMDU );// OVERRIDE
							return true;
						case 55:
							SPEC55_BFS( pMDU );// GPS STATUS
							return true;
						case 99:
							DISP99_BFS( pMDU );// FAULT
							return true;
						default:
							return false;
					}
				default:
					return false;
			}
		}
	}

	void GeneralDisplays::DISP18_PASS( vc::MDU* pMDU ) const
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
		pMDU->Line( 20, 18, 20, 171 );
		pMDU->Line( 40, 18, 40, 171 );
		pMDU->Line( 0, 36, 60, 36 );
		pMDU->Line( 0, 81, 60, 81 );
		pMDU->Line( 0, 126, 60, 126 );

		// SURF
		pMDU->Line( 70, 36, 160, 36 );

		// DPS
		pMDU->Line( 170, 36, 250, 36 );

		// FCS
		pMDU->Line( 170, 90, 240, 90 );

		// NAV
		pMDU->Line( 170, 126, 240, 126 );

		// CNTRL
		pMDU->Line( 80, 144, 140, 144 );


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

		tmp = STS()->GetControlSurfaceLevel( AIRCTRL_RUDDER ) * 27.1 ;
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

	void GeneralDisplays::DISP19_PASS( vc::MDU* pMDU ) const
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
		pMDU->Line( 45, 54, 45, 216 );
		pMDU->Line( 70, 54, 70, 216 );
		pMDU->Line( 95, 54, 95, 216 );
		pMDU->Line( 115, 54, 115, 216 );
		pMDU->Line( 135, 54, 135, 216 );
		pMDU->Line( 165, 126, 165, 216 );
		pMDU->Line( 190, 126, 190, 216 );
		pMDU->Line( 215, 126, 215, 216 );
		pMDU->Line( 235, 126, 235, 216 );
		pMDU->Line( 0, 54, 135, 54 );
		pMDU->Line( 0, 72, 135, 72 );
		pMDU->Line( 30, 99, 95, 99 );
		pMDU->Line( 135, 126, 255, 126 );
		pMDU->Line( 0, 144, 255, 144 );
		pMDU->Line( 30, 171, 95, 171 );
		pMDU->Line( 150, 171, 215, 171 );


		// TODO dynamic parts
		return;
	}

	void GeneralDisplays::SPEC51_PASS( vc::MDU* pMDU ) const
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
		pMDU->Line( 85, 63, 85, 216 );
		pMDU->Line( 85, 63, 255, 63 );
		pMDU->Line( 85, 108, 255, 108 );
		pMDU->Line( 85, 153, 255, 153 );


		// TODO dynamic parts
		return;
	}

	void GeneralDisplays::SPEC53_PASS( vc::MDU* pMDU ) const
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

	void GeneralDisplays::SPEC55_PASS( vc::MDU* pMDU ) const
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
		pMDU->Line( 70, 9, 70, 207 );
		pMDU->Line( 100, 9, 100, 207 );
		pMDU->Line( 130, 9, 130, 207 );
		pMDU->Line( 0, 99, 255, 99 );
		pMDU->Line( 130, 144, 255, 144 );
		pMDU->Line( 0, 180, 130, 180 );


		// TODO dynamic parts
		return;
	}

	void GeneralDisplays::DISP99_PASS( vc::MDU* pMDU ) const
	{
		PrintCommonHeader( "    FAULT", pMDU );

		// static parts (labels)
		pMDU->mvprint( 4, 5, "CRT" );
		pMDU->mvprint( 13, 5, "FAULT      C/W   GPC      TIME" );
		pMDU->mvprint( 4, 6, "ID" );
		return;
	}

	void GeneralDisplays::DISP18_BFS( vc::MDU* pMDU ) const
	{
		PrintCommonHeader( " GNC SYS SUMM 1", pMDU );
		pMDU->mvprint( 34, 1, "BFS" );

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
		pMDU->Line( 25, 36, 115, 36 );

		pMDU->Line( 150, 36, 220, 36 );

		pMDU->Line( 150, 90, 220, 90 );

		pMDU->Line( 150, 126, 210, 126 );

		pMDU->Line( 10, 126, 145, 126 );
		pMDU->Line( 145, 126, 145, 153 );
		pMDU->Line( 145, 153, 225, 153 );


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

		tmp[0] = STS()->GetControlSurfaceLevel( AIRCTRL_RUDDER ) * 27.1 ;
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

		for (int i = 0; i < 3; i++)
		{
			tmp[i] = dipHeSysPressureSensor[(i * 3) + 1].GetVoltage() * 200;
			if (tmp[i] > 1000) tmp[i] = 1000;
			else if (tmp[i] < 0) tmp[i] = 0;
		}
		sprintf_s( cbuf, 64, "%4.0f  %4.0f  %4.0f", tmp[1], tmp[0], tmp[2] );
		pMDU->mvprint( 12, 15, cbuf );

		for (int i = 0; i < 3; i++)
		{
			tmp[i] = dipHeSysPressureSensor[(i * 3) + 2].GetVoltage() * 200;
			if (tmp[i] > 1000) tmp[i] = 1000;
			else if (tmp[i] < 0) tmp[i] = 0;
		}
		sprintf_s( cbuf, 64, "%4.0f  %4.0f  %4.0f", tmp[1], tmp[0], tmp[2] );
		pMDU->mvprint( 12, 16, cbuf );

		for (int i = 0; i < 3; i++)
		{
			tmp[i] = He_dPdT[i];
			if (tmp[i] > 50) tmp[i] = 50;
			else if (tmp[i] < 0) tmp[i] = 0;
		}
		sprintf_s( cbuf, 64, "%3.0f   %3.0f   %3.0f", tmp[1], tmp[0], tmp[2] );
		pMDU->mvprint( 13, 17, cbuf );
		if (He_dPdT[1] == 50) pMDU->UpArrow( 16, 17 );
		if (He_dPdT[0] == 50) pMDU->UpArrow( 22, 17 );
		if (He_dPdT[2] == 50) pMDU->UpArrow( 28, 17 );

		tmp[0] = dipHeSysPressureSensor[9].GetVoltage() * 1000;
		if (tmp[0] > 5000) tmp[0] = 5000;
		else if (tmp[0] < 0) tmp[0] = 0;
		sprintf_s( cbuf, 64, "%4.0f", tmp[0] );
		pMDU->mvprint( 40, 19, cbuf );

		tmp[0] = dipHeSysPressureSensor[10].GetVoltage() * 200;
		if (tmp[0] > 1000) tmp[0] = 1000;
		else if (tmp[0] < 0) tmp[0] = 0;
		sprintf_s( cbuf, 64, "%4.0f", tmp[0] );
		pMDU->mvprint( 40, 20, cbuf );

		tmp[0] = dipHeSysPressureSensor[11].GetVoltage() * 200;
		if (tmp[0] > 1000) tmp[0] = 1000;
		else if (tmp[0] < 0) tmp[0] = 0;
		sprintf_s( cbuf, 64, "%4.0f", tmp[0] );
		pMDU->mvprint( 40, 21, cbuf );

		for (int i = 0; i < 3; i++)
		{
			tmp[i] = (dipLH2UllagePressureSensor[i].GetVoltage() * 8) + 12;

			if (tmp[i] > 52) tmp[i] = 52;
			else if (tmp[i] < 12) tmp[i] = 12;
		}
		sprintf_s( cbuf, 64, "%4.1f  %4.1f  %4.1f", tmp[1], tmp[0], tmp[2] );
		pMDU->mvprint( 12, 19, cbuf );
		if (tmp[1] == 52) pMDU->UpArrow( 16, 19 );
		else if (tmp[1] == 12) pMDU->DownArrow( 16, 19 );
		if (tmp[0] == 52) pMDU->UpArrow( 22, 19 );
		else if (tmp[0] == 12) pMDU->DownArrow( 22, 19 );
		if (tmp[2] == 52) pMDU->UpArrow( 28, 19 );
		else if (tmp[2] == 12) pMDU->DownArrow( 28, 19 );

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
		if (tmp[1] == 30) pMDU->UpArrow( 16, 20 );
		else if (tmp[1] == 0) pMDU->DownArrow( 16, 20 );
		if (tmp[0] == 30) pMDU->UpArrow( 22, 20 );
		else if (tmp[0] == 0) pMDU->DownArrow( 22, 20 );
		if (tmp[2] == 30) pMDU->UpArrow( 28, 20 );
		else if (tmp[2] == 0) pMDU->DownArrow( 28, 20 );

		/*sprintf_s( cbuf, 64, "%4.0f  %4.0f  %4.0f", abc, abc, abc );
		pMDU->mvprint( 12, 22, cbuf );
		sprintf_s( cbuf, 64, "%4.0f  %4.0f  %4.0f", abc, abc, abc );
		pMDU->mvprint( 12, 23, cbuf );*/

		tmp[0] = dipMPSManifPressureSensor[1].GetVoltage() * 20;
		if (tmp[0] > 100) tmp[0] = 100;
		else if (tmp[0] < 0) tmp[0] = 0;
		sprintf_s( cbuf, 64, "%3.0f", tmp[0] );
		pMDU->mvprint( 41, 22, cbuf );

		tmp[0] = dipMPSManifPressureSensor[0].GetVoltage() * 60;
		if (tmp[0] > 300) tmp[0] = 300;
		else if (tmp[0] < 0) tmp[0] = 0;
		sprintf_s( cbuf, 64, "%3.0f", tmp[0] );
		pMDU->mvprint( 41, 23, cbuf );
		return;
	}

	void GeneralDisplays::DISP19_BFS( vc::MDU* pMDU ) const
	{
		PrintCommonHeader( " GNC SYS SUMM 2", pMDU );
		pMDU->mvprint( 34, 1, "BFS" );

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
		pMDU->Line( 45, 54, 45, 216 );
		pMDU->Line( 70, 54, 70, 216 );
		pMDU->Line( 95, 54, 95, 216 );
		pMDU->Line( 115, 54, 115, 216 );
		pMDU->Line( 135, 54, 135, 216 );
		pMDU->Line( 165, 126, 165, 216 );
		pMDU->Line( 190, 126, 190, 216 );
		pMDU->Line( 215, 126, 215, 216 );
		pMDU->Line( 235, 126, 235, 216 );
		pMDU->Line( 0, 54, 135, 54 );
		pMDU->Line( 0, 72, 135, 72 );
		pMDU->Line( 30, 99, 95, 99 );
		pMDU->Line( 135, 126, 255, 126 );
		pMDU->Line( 0, 144, 255, 144 );
		pMDU->Line( 30, 171, 95, 171 );
		pMDU->Line( 150, 171, 215, 171 );


		// TODO dynamic parts
		return;
	}

	void GeneralDisplays::SPEC51_BFS( vc::MDU* pMDU ) const
	{
		PrintCommonHeader( "    OVERRIDE", pMDU );
		pMDU->mvprint( 34, 1, "BFS" );

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

	void GeneralDisplays::SPEC55_BFS( vc::MDU* pMDU ) const
	{
		PrintCommonHeader( "    GPS STATUS", pMDU );
		pMDU->mvprint( 34, 1, "BFS" );
		
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
		pMDU->Line( 70, 9, 70, 207 );
		pMDU->Line( 100, 9, 100, 207 );
		pMDU->Line( 130, 9, 130, 207 );
		pMDU->Line( 0, 99, 255, 99 );
		pMDU->Line( 130, 144, 255, 144 );
		pMDU->Line( 0, 180, 130, 180 );


		// TODO dynamic parts
		return;
	}

	void GeneralDisplays::DISP99_BFS( vc::MDU* pMDU ) const
	{
		PrintCommonHeader( "    FAULT", pMDU );
		pMDU->mvprint( 34, 1, "BFS" );

		// static parts (labels)
		pMDU->mvprint( 4, 5, "CRT" );
		pMDU->mvprint( 13, 5, "FAULT      C/W   GPC      TIME" );
		pMDU->mvprint( 4, 6, "ID" );
		return;
	}
};
