#include "../Atlantis.h"
#include "MDU.h"
#include "../dps/IDP.h"


namespace vc
{
	void MDU::AEPFD( HDC hDC )
	{
		int MM = STS()->pSimpleGPC->GetMajorMode();
		double MachNumber = STS()->GetMachNumber();
		double Altitude_ft = STS()->GetAltitude() * MPS2FPS;
		int adiatt = 1;//GetIDP()->GetADIAttitude();
		int adierr = GetIDP()->GetADIError();
		int adirate = GetIDP()->GetADIRate();
		VECTOR3 vel;
		STS()->GetHorizonAirspeedVector( vel );
		vel *= MPS2FPS;
		VECTOR3 av;// x = pitch, y = yaw, z = roll
		STS()->GetAngularVel( av );
		av *= DEG;
		av.z = -av.z;
		av.x = -av.x;
		VECTOR3 atterr;
		double attRoll = -STS()->GetBank() * DEG;
		if (attRoll <= 0) attRoll += 360;
		double attPitch = STS()->GetPitch() * DEG;
		if (attPitch <= 0) attPitch += 360;
		double attYaw = 0;//STS()->GetSlipAngle() * DEG;
		//if (attYaw <= 0) attYaw += 360;
		double hsiheading;

		int save = SaveDC( hDC );

		SelectObject( hDC, gdiTahomaFont_h10w4 );

		switch (MM)
		{
			case 101:
				AEPFD_Header_AscentDAP( hDC, 101, adiatt );
				Tapes_Invalid( hDC );
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				ADI_RATE_A( hDC, av.x, av.z, av.y, adirate );
				ADI_ERROR_A( hDC, 0, 0, 0, adierr );
				HSI_E( hDC, 0, true, 0 );
				break;
			case 102:
				AEPFD_Header_AscentDAP( hDC, 102, adiatt );
				Tape_MVR_KEAS( hDC, MachNumber );
				Tape_Alpha( hDC, 0 );
				Tape_H_Hdot( hDC, Altitude_ft, vel.y );
				if ((Altitude_ft <= 200000) && (STS()->GetMET() <= 150)) AEPFD_BETA( hDC );
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				ADI_RATE_A( hDC, av.x, av.z, av.y, adirate );
				atterr = GetIDP()->GetAttitudeErrors_AscentGuidance();
				ADI_ERROR_A( hDC, atterr.x, atterr.z, atterr.y, adierr );
				AEPFD_GMETER_STATIC( hDC );
				AEPFD_GMETER_ACCEL( hDC );
				hsiheading = STS()->GetYaw() - GetIDP()->GetTargetHeading();
				HSI_A( hDC, hsiheading, STS()->GetBank(), true, hsiheading * DEG );
				AEPFD_XTRK( hDC );// TODO only NOM, TAL and ATO
				AEPFD_dINC( hDC );
				break;
			case 103:
				AEPFD_Header_AscentDAP( hDC, 103, adiatt );
				if (GetIDP()->GetMECOConfirmedFlag() == true)
				{
					SelectObject( hDC, gdiBlackPen );
					SelectObject( hDC, gdiBlackBrush );
					Rectangle( hDC, 6, 13, 60, 23 );// "hide" throttle
				}
				Tape_MVI_KEAS( hDC, MachNumber );
				Tape_Alpha( hDC, 0 );
				Tape_H_Hdot( hDC, Altitude_ft, vel.y );
				if ((Altitude_ft <= 200000) && (STS()->GetMET() <= 150)) AEPFD_BETA( hDC );
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				ADI_RATE_A( hDC, av.x, av.z, av.y, adirate );
				atterr = GetIDP()->GetAttitudeErrors_AscentGuidance();
				ADI_ERROR_A( hDC, atterr.x, atterr.z, atterr.y, adierr );
				AEPFD_GMETER_STATIC( hDC );
				AEPFD_GMETER_ACCEL( hDC );
				hsiheading = STS()->GetYaw() - GetIDP()->GetTargetHeading();
				HSI_A( hDC, hsiheading, STS()->GetBank(), true, hsiheading * DEG );
				if (0)// TODO TAL
				{
					AEPFD_dXTRK( hDC );
					AEPFD_RANGERW( hDC );// fix colision between this and dINC
				}
				AEPFD_XTRK( hDC );// TODO only NOM, TAL and ATO
				if (0) AEPFD_RANGERW( hDC );// TODO ECAL/BDA
				AEPFD_dINC( hDC );
				if (0) AEPFD_TGTINC( hDC );// TODO ATO
				break;
			case 104:
				SetTextColor( hDC, CR_LIGHT_GRAY );
				TextOut( hDC, 21, 4, "DAP:", 4 );
				SetTextColor( hDC, CR_WHITE );
				if (1) TextOut( hDC, 40, 4, "Auto", 4 );// TODO get TransDAP state
				else TextOut( hDC, 40, 4, "INRTL", 5 );

				SetTextColor( hDC, CR_LIGHT_GRAY );
				TextOut( hDC, 209, 4, "MM:", 3 );
				SetTextColor( hDC, CR_WHITE );
				if (0) TextOut( hDC, 225, 4, "104T", 4 );// TAL
				else if (0) TextOut( hDC, 225, 4, "104ATO", 6 );// ATO
				else if (0) TextOut( hDC, 225, 4, "104AOA", 6 );// AOA
				else if (0) TextOut( hDC, 225, 4, "104CA", 5 );// CA
				else TextOut( hDC, 225, 4, "104", 3 );// NOM

				SetTextColor( hDC, CR_LIGHT_GRAY );
				TextOut( hDC, 208, 13, "ATT:", 4 );
				SetTextColor( hDC, CR_WHITE );
				if (adiatt == 2) TextOut( hDC, 228, 13, "INRTL", 5 );
				else if (adiatt == 1) TextOut( hDC, 228, 13, "LVLH", 4 );
				else TextOut( hDC, 228, 13, "REF", 3 );

				Tape_MVI_KEAS( hDC, MachNumber );
				Tape_H_Hdot( hDC, Altitude_ft, vel.y );
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				ADI_RATE_A( hDC, av.x, av.z, av.y, adirate );
				ADI_ERROR_A( hDC, 0, 0, 0, adierr );// TODO
				HSI_E( hDC, STS()->GetYaw(), true, 0 );
				AEPFD_XTRK( hDC );// TODO only NOM, TAL and ATO
				AEPFD_dINC( hDC );
				break;
			case 105:
				AEPFD_Header_TransDAP( hDC, 105, adiatt );
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				ADI_RATE_A( hDC, av.x, av.z, av.y, adirate );
				ADI_ERROR_A( hDC, 0, 0, 0, adierr );// TODO
				break;
			case 106:
				AEPFD_Header_TransDAP( hDC, 106, adiatt );
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				ADI_RATE_A( hDC, av.x, av.z, av.y, adirate );
				ADI_ERROR_A( hDC, 0, 0, 0, adierr );// TODO
				break;
			case 201:
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				break;
			case 202:
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				break;
			case 301:
				AEPFD_Header_TransDAP( hDC, 301, adiatt );
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				ADI_RATE_A( hDC, av.x, av.z, av.y, adirate );
				atterr = GetIDP()->GetAttitudeErrors_AerojetDAP();
				ADI_ERROR_A( hDC, atterr.x, atterr.z, atterr.y, adierr );
				break;
			case 302:
				AEPFD_Header_TransDAP( hDC, 302, adiatt );
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				ADI_RATE_A( hDC, av.x, av.z, av.y, adirate );
				atterr = GetIDP()->GetAttitudeErrors_AerojetDAP();
				ADI_ERROR_A( hDC, atterr.x, atterr.z, atterr.y, adierr );
				break;
			case 303:
				AEPFD_Header_TransDAP( hDC, 303, adiatt );
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				ADI_RATE_A( hDC, av.x, av.z, av.y, adirate );
				atterr = GetIDP()->GetAttitudeErrors_AerojetDAP();
				ADI_ERROR_A( hDC, atterr.x, atterr.z, atterr.y, adierr );
				break;
			case 304:
				AEPFD_Header_AerojetDAP( hDC, 304, MachNumber );
				Tape_MVR_KEAS( hDC, MachNumber );
				Tape_Alpha( hDC, MachNumber );
				Tape_H_Hdot( hDC, Altitude_ft, vel.y );
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				ADI_RATE_A( hDC, av.x, av.z, av.y, adirate );
				atterr = GetIDP()->GetAttitudeErrors_AerojetDAP();
				ADI_ERROR_B( hDC, atterr.x, atterr.z, atterr.y, adierr );
				AEPFD_GMETER_STATIC( hDC );
				AEPFD_GMETER_NZ( hDC );
				HSI_E( hDC, atan2( vel.x, vel.z ), true, GetIDP()->GetSelectedRunwayHeading() );
				AEPFD_dAZ_HTA( hDC, MachNumber );
				AEPFD_RANGERW( hDC );
				AEPFD_HACCEL( hDC );
				break;
			case 305:
				AEPFD_Header_AerojetDAP( hDC, 305, MachNumber );
				if (MachNumber >= 0.9) Tape_MVR_KEAS( hDC, MachNumber );
				else Tape_KEAS_MVR( hDC, MachNumber );
				Tape_Alpha( hDC, MachNumber );
				Tape_H_Hdot( hDC, Altitude_ft, vel.y );
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				ADI_RATE_B( hDC, av.x, av.z, av.y, adirate, Altitude_ft );
				atterr = GetIDP()->GetAttitudeErrors_AerojetDAP();
				if (GetIDP()->GetWOW() == false) ADI_ERROR_C( hDC, GetIDP()->GetNZError(), atterr.z, atterr.y, adierr );
				else ADI_ERROR_D( hDC, atterr.x, atterr.z, atterr.y, adierr );
				AEPFD_GMETER_STATIC( hDC );
				AEPFD_GMETER_NZ( hDC );
				HSI_E( hDC, STS()->GetYaw(), true, GetIDP()->GetSelectedRunwayHeading() );
				if (GetIDP()->GetPrefinalState() == false)
				{
					AEPFD_dAZ_HTA( hDC, MachNumber );
					AEPFD_RANGEHACC( hDC );
				}
				AEPFD_RANGERW( hDC );
				AEPFD_GSI( hDC, Altitude_ft );
				break;
			case 601:
				AEPFD_Header_AscentDAP( hDC, 601, adiatt );
				if (0) Tape_MVR_KEAS( hDC, MachNumber );// TODO PPA
				Tape_MVI_KEAS( hDC, MachNumber );
				Tape_Alpha( hDC, 0 );
				Tape_H_Hdot( hDC, Altitude_ft, vel.y );
				AEPFD_BETA( hDC );
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				ADI_RATE_A( hDC, av.x, av.z, av.y, adirate );
				atterr = GetIDP()->GetAttitudeErrors_AscentGuidance();
				ADI_ERROR_A( hDC, atterr.x, atterr.z, atterr.y, adierr );
				AEPFD_GMETER_STATIC( hDC );
				AEPFD_GMETER_ACCEL( hDC );
				HSI_E( hDC, STS()->GetYaw(), true, 0 );
				AEPFD_dAZ_HTA( hDC, MachNumber );
				AEPFD_RANGERW( hDC );
				break;
			case 602:
				AEPFD_Header_AerojetDAP( hDC, 602, MachNumber );
				Tape_MVR_KEAS( hDC, MachNumber );
				Tape_Alpha( hDC, MachNumber );
				Tape_H_Hdot( hDC, Altitude_ft, vel.y );
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				ADI_RATE_A( hDC, av.x, av.z, av.y, adirate );
				atterr = GetIDP()->GetAttitudeErrors_AerojetDAP();
				if (0) ADI_ERROR_C( hDC, GetIDP()->GetNZError(), atterr.z, atterr.y, adierr );// TODO Nz hold
				else ADI_ERROR_B( hDC, atterr.x, atterr.z, atterr.y, adierr );
				AEPFD_GMETER_STATIC( hDC );
				AEPFD_GMETER_NZ( hDC );
				HSI_E( hDC, atan2( vel.x, vel.z ), true, GetIDP()->GetSelectedRunwayHeading() );
				AEPFD_dAZ_HTA( hDC, MachNumber );
				AEPFD_RANGERW( hDC );
				AEPFD_HACCEL( hDC );
				break;
			case 603:
				AEPFD_Header_AerojetDAP( hDC, 603, MachNumber );
				if (MachNumber >= 0.9) Tape_MVR_KEAS( hDC, MachNumber );
				else Tape_KEAS_MVR( hDC, MachNumber );
				Tape_Alpha( hDC, MachNumber );
				Tape_H_Hdot( hDC, Altitude_ft, vel.y );
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				ADI_RATE_B( hDC, av.x, av.z, av.y, adirate, Altitude_ft );
				atterr = GetIDP()->GetAttitudeErrors_AerojetDAP();
				if (GetIDP()->GetWOW() == false) ADI_ERROR_C( hDC, GetIDP()->GetNZError(), atterr.z, atterr.y, adierr );
				else ADI_ERROR_D( hDC, atterr.x, atterr.z, atterr.y, adierr );
				AEPFD_GMETER_STATIC( hDC );
				AEPFD_GMETER_NZ( hDC );
				HSI_E( hDC, STS()->GetYaw(), true, GetIDP()->GetSelectedRunwayHeading() );
				if (GetIDP()->GetPrefinalState() == false)
				{
					AEPFD_dAZ_HTA( hDC, MachNumber );
					AEPFD_RANGEHACC( hDC );
				}
				AEPFD_RANGERW( hDC );
				AEPFD_GSI( hDC, Altitude_ft );
				break;
			case 801:
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				break;
			case 901:
				ADI_STATIC( hDC );// TODO no rate and error scales
				ADI( hDC, attPitch, attRoll, attYaw );
				AEPFD_GMETER_STATIC( hDC );
				HSI_E( hDC, 0, true, 0 );
				break;
			default:
				break;
		}
		RestoreDC( hDC, save );
		return;
	}

	void MDU::AEPFD( oapi::Sketchpad2* skp )
	{
		int MM = STS()->pSimpleGPC->GetMajorMode();
		double MachNumber = STS()->GetMachNumber();
		double Altitude_ft = STS()->GetAltitude() * MPS2FPS;
		int adiatt = 1;//GetIDP()->GetADIAttitude();
		int adierr = GetIDP()->GetADIError();
		int adirate = GetIDP()->GetADIRate();
		VECTOR3 vel;
		STS()->GetHorizonAirspeedVector( vel );
		vel *= MPS2FPS;
		VECTOR3 av;// x = pitch, y = yaw, z = roll
		STS()->GetAngularVel( av );
		av *= DEG;
		av.z = -av.z;
		av.x = -av.x;
		VECTOR3 atterr;
		double attRoll = -STS()->GetBank() * DEG;
		if (attRoll <= 0) attRoll += 360;
		double attPitch = STS()->GetPitch() * DEG;
		if (attPitch <= 0) attPitch += 360;
		double attYaw = 0;//STS()->GetSlipAngle() * DEG;
		//if (attYaw <= 0) attYaw += 360;
		double hsiheading;

		skp->SetFont( skpTahomaFont_h10w4 );
		HDC hDC = NULL;////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		switch (MM)
		{
			case 101:
				AEPFD_Header_AscentDAP( skp, 101, adiatt );
				Tapes_Invalid( skp );
				ADI_STATIC( skp );
				ADI( skp, attPitch, attRoll, attYaw );
				ADI_RATE_A( skp, av.x, av.z, av.y, adirate );
				ADI_ERROR_A( skp, 0, 0, 0, adierr );
				HSI_E( skp, 0, true, 0 );
				break;
			case 102:
				AEPFD_Header_AscentDAP( skp, 102, adiatt );
				Tape_MVR_KEAS( hDC, MachNumber );
				Tape_Alpha( hDC, 0 );
				Tape_H_Hdot( hDC, Altitude_ft, vel.y );
				if ((Altitude_ft <= 200000) && (STS()->GetMET() <= 150)) AEPFD_BETA( skp );
				ADI_STATIC( skp );
				ADI( skp, attPitch, attRoll, attYaw );
				ADI_RATE_A( skp, av.x, av.z, av.y, adirate );
				atterr = GetIDP()->GetAttitudeErrors_AscentGuidance();
				ADI_ERROR_A( skp, atterr.x, atterr.z, atterr.y, adierr );
				AEPFD_GMETER_STATIC( skp );
				AEPFD_GMETER_ACCEL( skp );
				hsiheading = STS()->GetYaw() - GetIDP()->GetTargetHeading();
				HSI_A( skp, hsiheading, STS()->GetBank(), true, hsiheading * DEG );
				AEPFD_XTRK( skp );// TODO only NOM, TAL and ATO
				AEPFD_dINC( skp );
				break;
			case 103:
				AEPFD_Header_AscentDAP( skp, 103, adiatt );
				if (GetIDP()->GetMECOConfirmedFlag() == true)
				{
					skp->SetPen( skpBlackPen );
					skp->SetBrush( skpBlackBrush );
					Rectangle( hDC, 6, 13, 60, 23 );// "hide" throttle
				}
				Tape_MVI_KEAS( hDC, MachNumber );
				Tape_Alpha( hDC, 0 );
				Tape_H_Hdot( hDC, Altitude_ft, vel.y );
				if ((Altitude_ft <= 200000) && (STS()->GetMET() <= 150)) AEPFD_BETA( skp );
				ADI_STATIC( skp );
				ADI( skp, attPitch, attRoll, attYaw );
				ADI_RATE_A( skp, av.x, av.z, av.y, adirate );
				atterr = GetIDP()->GetAttitudeErrors_AscentGuidance();
				ADI_ERROR_A( skp, atterr.x, atterr.z, atterr.y, adierr );
				AEPFD_GMETER_STATIC( skp );
				AEPFD_GMETER_ACCEL( skp );
				hsiheading = STS()->GetYaw() - GetIDP()->GetTargetHeading();
				HSI_A( skp, hsiheading, STS()->GetBank(), true, hsiheading * DEG );
				if (0)// TODO TAL
				{
					AEPFD_dXTRK( skp );
					AEPFD_RANGERW( skp );// fix colision between this and dINC
				}
				AEPFD_XTRK( skp );// TODO only NOM, TAL and ATO
				if (0) AEPFD_RANGERW( skp );// TODO ECAL/BDA
				AEPFD_dINC( skp );
				if (0) AEPFD_TGTINC( skp );// TODO ATO
				break;
			case 104:
				skp->SetTextColor( CR_LIGHT_GRAY );
				skp->Text( 21, 4, "DAP:", 4 );
				skp->SetTextColor( CR_WHITE );
				if (1) skp->Text( 40, 4, "Auto", 4 );// TODO get TransDAP state
				else skp->Text( 40, 4, "INRTL", 5 );

				skp->SetTextColor( CR_LIGHT_GRAY );
				skp->Text( 209, 4, "MM:", 3 );
				skp->SetTextColor( CR_WHITE );
				if (0) skp->Text( 225, 4, "104T", 4 );// TAL
				else if (0) skp->Text( 225, 4, "104ATO", 6 );// ATO
				else if (0) skp->Text( 225, 4, "104AOA", 6 );// AOA
				else if (0) skp->Text( 225, 4, "104CA", 5 );// CA
				else skp->Text( 225, 4, "104", 3 );// NOM

				skp->SetTextColor( CR_LIGHT_GRAY );
				skp->Text( 208, 13, "ATT:", 4 );
				skp->SetTextColor( CR_WHITE );
				if (adiatt == 2) skp->Text( 228, 13, "INRTL", 5 );
				else if (adiatt == 1) skp->Text( 228, 13, "LVLH", 4 );
				else skp->Text( 228, 13, "REF", 3 );

				Tape_MVI_KEAS( hDC, MachNumber );
				Tape_H_Hdot( hDC, Altitude_ft, vel.y );
				ADI_STATIC( skp );
				ADI( skp, attPitch, attRoll, attYaw );
				ADI_RATE_A( skp, av.x, av.z, av.y, adirate );
				ADI_ERROR_A( skp, 0, 0, 0, adierr );// TODO
				HSI_E( skp, STS()->GetYaw(), true, 0 );
				AEPFD_XTRK( skp );// TODO only NOM, TAL and ATO
				AEPFD_dINC( skp );
				break;
			case 105:
				AEPFD_Header_TransDAP( skp, 105, adiatt );
				ADI_STATIC( skp );
				ADI( skp, attPitch, attRoll, attYaw );
				ADI_RATE_A( skp, av.x, av.z, av.y, adirate );
				ADI_ERROR_A( skp, 0, 0, 0, adierr );// TODO
				break;
			case 106:
				AEPFD_Header_TransDAP( skp, 106, adiatt );
				ADI_STATIC( skp );
				ADI( skp, attPitch, attRoll, attYaw );
				ADI_RATE_A( skp, av.x, av.z, av.y, adirate );
				ADI_ERROR_A( skp, 0, 0, 0, adierr );// TODO
				break;
			case 201:
				ADI_STATIC( skp );
				ADI( skp, attPitch, attRoll, attYaw );
				break;
			case 202:
				ADI_STATIC( skp );
				ADI( skp, attPitch, attRoll, attYaw );
				break;
			case 301:
				AEPFD_Header_TransDAP( skp, 301, adiatt );
				ADI_STATIC( skp );
				ADI( skp, attPitch, attRoll, attYaw );
				ADI_RATE_A( skp, av.x, av.z, av.y, adirate );
				atterr = GetIDP()->GetAttitudeErrors_AerojetDAP();
				ADI_ERROR_A( skp, atterr.x, atterr.z, atterr.y, adierr );
				break;
			case 302:
				AEPFD_Header_TransDAP( skp, 302, adiatt );
				ADI_STATIC( skp );
				ADI( skp, attPitch, attRoll, attYaw );
				ADI_RATE_A( skp, av.x, av.z, av.y, adirate );
				atterr = GetIDP()->GetAttitudeErrors_AerojetDAP();
				ADI_ERROR_A( skp, atterr.x, atterr.z, atterr.y, adierr );
				break;
			case 303:
				AEPFD_Header_TransDAP( skp, 303, adiatt );
				ADI_STATIC( skp );
				ADI( skp, attPitch, attRoll, attYaw );
				ADI_RATE_A( skp, av.x, av.z, av.y, adirate );
				atterr = GetIDP()->GetAttitudeErrors_AerojetDAP();
				ADI_ERROR_A( skp, atterr.x, atterr.z, atterr.y, adierr );
				break;
			case 304:
				AEPFD_Header_AerojetDAP( skp, 304, MachNumber );
				Tape_MVR_KEAS( hDC, MachNumber );
				Tape_Alpha( hDC, MachNumber );
				Tape_H_Hdot( hDC, Altitude_ft, vel.y );
				ADI_STATIC( skp );
				ADI( skp, attPitch, attRoll, attYaw );
				ADI_RATE_A( skp, av.x, av.z, av.y, adirate );
				atterr = GetIDP()->GetAttitudeErrors_AerojetDAP();
				ADI_ERROR_B( skp, atterr.x, atterr.z, atterr.y, adierr );
				AEPFD_GMETER_STATIC( skp );
				AEPFD_GMETER_NZ( skp );
				HSI_E( skp, atan2( vel.x, vel.z ), true, GetIDP()->GetSelectedRunwayHeading() );
				AEPFD_dAZ_HTA( skp, MachNumber );
				AEPFD_RANGERW( skp );
				AEPFD_HACCEL( skp );
				break;
			case 305:
				AEPFD_Header_AerojetDAP( skp, 305, MachNumber );
				if (MachNumber >= 0.9) Tape_MVR_KEAS( hDC, MachNumber );
				else Tape_KEAS_MVR( hDC, MachNumber );
				Tape_Alpha( hDC, MachNumber );
				Tape_H_Hdot( hDC, Altitude_ft, vel.y );
				ADI_STATIC( skp );
				ADI( skp, attPitch, attRoll, attYaw );
				ADI_RATE_B( skp, av.x, av.z, av.y, adirate, Altitude_ft );
				atterr = GetIDP()->GetAttitudeErrors_AerojetDAP();
				if (GetIDP()->GetWOW() == false) ADI_ERROR_C( skp, GetIDP()->GetNZError(), atterr.z, atterr.y, adierr );
				else ADI_ERROR_D( skp, atterr.x, atterr.z, atterr.y, adierr );
				AEPFD_GMETER_STATIC( skp );
				AEPFD_GMETER_NZ( skp );
				HSI_E( skp, STS()->GetYaw(), true, GetIDP()->GetSelectedRunwayHeading() );
				if (GetIDP()->GetPrefinalState() == false)
				{
					AEPFD_dAZ_HTA( skp, MachNumber );
					AEPFD_RANGEHACC( skp );
				}
				AEPFD_RANGERW( skp );
				AEPFD_GSI( skp, Altitude_ft );
				break;
			case 601:
				AEPFD_Header_AscentDAP( skp, 601, adiatt );
				if (0) Tape_MVR_KEAS( hDC, MachNumber );// TODO PPA
				Tape_MVI_KEAS( hDC, MachNumber );
				Tape_Alpha( hDC, 0 );
				Tape_H_Hdot( hDC, Altitude_ft, vel.y );
				AEPFD_BETA( skp );
				ADI_STATIC( skp );
				ADI( skp, attPitch, attRoll, attYaw );
				ADI_RATE_A( skp, av.x, av.z, av.y, adirate );
				atterr = GetIDP()->GetAttitudeErrors_AscentGuidance();
				ADI_ERROR_A( skp, atterr.x, atterr.z, atterr.y, adierr );
				AEPFD_GMETER_STATIC( skp );
				AEPFD_GMETER_ACCEL( skp );
				HSI_E( skp, STS()->GetYaw(), true, 0 );
				AEPFD_dAZ_HTA( skp, MachNumber );
				AEPFD_RANGERW( skp );
				break;
			case 602:
				AEPFD_Header_AerojetDAP( skp, 602, MachNumber );
				Tape_MVR_KEAS( hDC, MachNumber );
				Tape_Alpha( hDC, MachNumber );
				Tape_H_Hdot( hDC, Altitude_ft, vel.y );
				ADI_STATIC( skp );
				ADI( skp, attPitch, attRoll, attYaw );
				ADI_RATE_A( skp, av.x, av.z, av.y, adirate );
				atterr = GetIDP()->GetAttitudeErrors_AerojetDAP();
				if (0) ADI_ERROR_C( skp, GetIDP()->GetNZError(), atterr.z, atterr.y, adierr );// TODO Nz hold
				else ADI_ERROR_B( skp, atterr.x, atterr.z, atterr.y, adierr );
				AEPFD_GMETER_STATIC( skp );
				AEPFD_GMETER_NZ( skp );
				HSI_E( skp, atan2( vel.x, vel.z ), true, GetIDP()->GetSelectedRunwayHeading() );
				AEPFD_dAZ_HTA( skp, MachNumber );
				AEPFD_RANGERW( skp );
				AEPFD_HACCEL( skp );
				break;
			case 603:
				AEPFD_Header_AerojetDAP( skp, 603, MachNumber );
				if (MachNumber >= 0.9) Tape_MVR_KEAS( hDC, MachNumber );
				else Tape_KEAS_MVR( hDC, MachNumber );
				Tape_Alpha( hDC, MachNumber );
				Tape_H_Hdot( hDC, Altitude_ft, vel.y );
				ADI_STATIC( skp );
				ADI( skp, attPitch, attRoll, attYaw );
				ADI_RATE_B( skp, av.x, av.z, av.y, adirate, Altitude_ft );
				atterr = GetIDP()->GetAttitudeErrors_AerojetDAP();
				if (GetIDP()->GetWOW() == false) ADI_ERROR_C( skp, GetIDP()->GetNZError(), atterr.z, atterr.y, adierr );
				else ADI_ERROR_D( skp, atterr.x, atterr.z, atterr.y, adierr );
				AEPFD_GMETER_STATIC( skp );
				AEPFD_GMETER_NZ( skp );
				HSI_E( skp, STS()->GetYaw(), true, GetIDP()->GetSelectedRunwayHeading() );
				if (GetIDP()->GetPrefinalState() == false)
				{
					AEPFD_dAZ_HTA( skp, MachNumber );
					AEPFD_RANGEHACC( skp );
				}
				AEPFD_RANGERW( skp );
				AEPFD_GSI( skp, Altitude_ft );
				break;
			case 801:
				ADI_STATIC( skp );
				ADI( skp, attPitch, attRoll, attYaw );
				break;
			case 901:
				ADI_STATIC( skp );// TODO no rate and error scales
				ADI( skp, attPitch, attRoll, attYaw );
				AEPFD_GMETER_STATIC( skp );
				HSI_E( skp, 0, true, 0 );
				break;
			default:
				break;
		}
		return;
	}

	void MDU::Tape_Create( void )
	{
		hDC_Tapes = CreateCompatibleDC( GetDC( NULL ) );
		HBITMAP hBM = CreateCompatibleBitmap( GetDC( NULL ), 113, 3700 );
		SelectObject( hDC_Tapes, hBM );
		SelectObject( hDC_Tapes, gdiWhitePen );
		SelectObject( hDC_Tapes, gdiWhiteBrush );
		Rectangle( hDC_Tapes, 0, 0, 113, 3700 );
		SelectObject( hDC_Tapes, gdiTahomaFont_h10w4 );
		// TODO cleanup at end?

		// draw tapes
		int offset = 57;
		char cbuf[8];
		int y = 0;

		// Mach/V
		// 1K in window (1ft = 0.114px) (4-27K)
		// M0.5 in window (M1 = 228px) (0-4)
		// 2622 + 912 = 3534px + offsets
		SelectObject( hDC_Tapes, gdiBlackPen );
		SelectObject( hDC_Tapes, gdiBlackBrush );
		Rectangle( hDC_Tapes, 0, 3534 + offset, 22, 3534 + offset + offset );

		SetTextColor( hDC_Tapes, CR_BLACK );
		SetBkMode( hDC_Tapes, TRANSPARENT );
		SelectObject( hDC_Tapes, gdiBlackPen );
		for (int i = 270; i >= 42; i -= 1)
		{
			y = Round( ((270 - i) * 11.4) + offset );

			if ((i % 10) == 0)
			{
				sprintf_s( cbuf, 8, "%4.1fK", (double)i / 10 );
				TextOut( hDC_Tapes, 2, y - 5, cbuf, strlen( cbuf ) );
			}
			else if ((i % 2) == 0)
			{
				sprintf_s( cbuf, 8, "%4.1f", (double)i / 10 );
				TextOut( hDC_Tapes, 4, y - 5, cbuf, strlen( cbuf ) );
			}
			else
			{
				MoveToEx( hDC_Tapes, 4, y, NULL );
				LineTo( hDC_Tapes, 18, y );
			}
		}
		for (int i = 40; i >= 0; i--)
		{
			y = Round( ((40 - i) * 22.8) + offset ) + 2622;

			if ((i % 2) == 0)
			{
				if ((i % 10) == 0)
				{
					sprintf_s( cbuf, 8, "%2.0f", (double)i / 10 );
					TextOut( hDC_Tapes, 6, y - 5, cbuf, strlen( cbuf ) );
				}
				else
				{
					sprintf_s( cbuf, 8, "%3.1f", (double)i / 10 );
					TextOut( hDC_Tapes, 6, y - 5, cbuf, strlen( cbuf ) );
				}
			}
			else
			{
				MoveToEx( hDC_Tapes, 4, y, NULL );
				LineTo( hDC_Tapes, 18, y );
			}
		}
			
		// KEAS
		// 30KEAS in window (1KEAS = 3.8px) (500-0)
		// 1900px + offsets
		SelectObject( hDC_Tapes, gdiBlackPen );
		SelectObject( hDC_Tapes, gdiBlackBrush );
		Rectangle( hDC_Tapes, 23, 1900 + offset, 45, 1900 + offset + offset );

		SetTextColor( hDC_Tapes, CR_BLACK );
		SetBkMode( hDC_Tapes, TRANSPARENT );
		SelectObject( hDC_Tapes, gdiBlackPen );
		for (int i = 500; i >= 0; i -= 5)
		{
			y = Round( ((500 - i) * 3.8) + offset );

			if ((i % 10) == 0)
			{
				sprintf_s( cbuf, 8, "%3d", i );
				TextOut( hDC_Tapes, 28, y - 5, cbuf, strlen( cbuf ) );
			}
			else
			{
				MoveToEx( hDC_Tapes, 27, y, NULL );
				LineTo( hDC_Tapes, 41, y );
			}
		}

		// alpha
		// 20º in window (1º = 5.7px)
		// 2052px long + offsets
		SelectObject( hDC_Tapes, gdiLightGrayPen );
		SelectObject( hDC_Tapes, gdiLightGrayBrush );
		Rectangle( hDC_Tapes, 45, 1026 + offset, 67, 2052 + offset + offset );

		SetTextColor( hDC_Tapes, CR_BLACK );
		SetBkMode( hDC_Tapes, TRANSPARENT );
		SelectObject( hDC_Tapes, gdiBlackPen );
		for (int i = 180; i >= -180; i--)
		{
			if (i < 0)// yes, not very efficient but the loop only runs once
			{
				SetTextColor( hDC_Tapes, CR_WHITE );
				SelectObject( hDC_Tapes, gdiWhitePen );
			}

			y = Round( ((180 - i) * 5.7) + offset );

			MoveToEx( hDC_Tapes, 62, y, NULL );
			LineTo( hDC_Tapes, 67, y );

			if ((i % 5) == 0)
			{
				sprintf_s( cbuf, 8, "%4d", i );
				TextOut( hDC_Tapes, 45, y - 5, cbuf, strlen( cbuf ) );
			}
		}

		// H
		// NM scale 10NM in window (1NM = 11.4px) (165-65.83137NM)
		// FT scale 100Kft in window (1ft = 0.00114px) (400-100Kft)
		// FT scale 11.5Kft in window (1ft = 0.00991px) (100-30Kft)
		// FT scale 6Kft in window (1ft = 0.019px) n1(n5K)/n1000 m0.5 (30-2Kft)
		//0 FT scale 1000ft in window (1ft = 0.114px) (2000-200ft)
		//0 FT scale 150ft in window (1ft = 0.76px) n50m10 (200-0ft)
		//? FT scale 500ft in window (1ft = 0.228px) (0-(-100)ft)
		//? FT scale 40ft in window (1ft = 0.057px) ((-100)-(-1100)ft)
		// 1130.522382 + 342 + 693.7 + 532 + 205.2 + 152 + 0 + 0 = 0px long + offsets
		SelectObject( hDC_Tapes, gdiYellowPen );
		SelectObject( hDC_Tapes, gdiYellowBrush );
		Rectangle( hDC_Tapes, 68, 2698 + offset, 90, 3055 + offset );

		SelectObject( hDC_Tapes, gdiLightGrayPen );
		SelectObject( hDC_Tapes, gdiLightGrayBrush );
		Rectangle( hDC_Tapes, 68, 3055 + offset, 90, 3380 + offset + offset );

		SetTextColor( hDC_Tapes, CR_BLACK );
		SetBkMode( hDC_Tapes, TRANSPARENT );
		SelectObject( hDC_Tapes, gdiBlackPen );
		for (int i = 165; i >= 67; i--)
		{
			y = Round( ((165 - i) * 11.4) + offset );

			if ((i % 5) == 0)
			{
				sprintf_s( cbuf, 8, "%3dM", i );
				TextOut( hDC_Tapes, 70, y - 5, cbuf, strlen( cbuf ) );
			}
			else
			{
				MoveToEx( hDC_Tapes, 83, y, NULL );
				LineTo( hDC_Tapes, 90, y );
			}
		}
		for (int i = 400; i >= 100; i -= 5)
		{
			y = Round( ((400 - i) * 1.14) + offset ) + 1131;

			if ((i % 50) == 0)
			{
				sprintf_s( cbuf, 8, "%3dK", i );
				TextOut( hDC_Tapes, 71, y - 5, cbuf, strlen( cbuf ) );
			}
			else if ((i % 10) == 0)
			{
				MoveToEx( hDC_Tapes, 68, y, NULL );
				LineTo( hDC_Tapes, 79, y );
			}
		}
		for (int i = 99; i >= 30; i--)
		{
			y = Round( ((100 - i) * 9.91) + offset ) + 1473;

			if ((i % 5) == 0)
			{
				sprintf_s( cbuf, 8, "%2dK", i );
				TextOut( hDC_Tapes, 73, y - 5, cbuf, strlen( cbuf ) );
			}
			else
			{
				MoveToEx( hDC_Tapes, 72, y, NULL );
				LineTo( hDC_Tapes, 86, y );
			}
		}
		for (int i = 295; i > 20; i -= 5)
		{
			y = Round( ((300 - i) * 1.9) + offset ) + 2167;

			if ((i % 50) == 0)
			{
				sprintf_s( cbuf, 8, "%2.0fK", (double)i / 10 );
				TextOut( hDC_Tapes, 73, y - 5, cbuf, strlen( cbuf ) );
			}
			else if ((i % 10) == 0)
			{
				sprintf_s( cbuf, 8, "%2.0f", (double)i / 10 );
				TextOut( hDC_Tapes, 75, y - 5, cbuf, strlen( cbuf ) );
			}
			else
			{
				MoveToEx( hDC_Tapes, 72, y, NULL );
				LineTo( hDC_Tapes, 86, y );
			}
		}

		for (int i = 20; i > 2; i -= 1)
		{
			y = Round( ((20 - i) * 11.4) + offset ) + 2699;

			if ((i % 2) == 0)
			{
				sprintf_s( cbuf, 8, "%4d", i * 100 );
				TextOut( hDC_Tapes, 71, y - 5, cbuf, strlen( cbuf ) );
			}
			else
			{
				MoveToEx( hDC_Tapes, 72, y, NULL );
				LineTo( hDC_Tapes, 86, y );
			}
		}
		for (int i = 20; i >= 0; i -= 1)
		{
			y = Round( ((20 - i) * 7.6) + offset ) + 2904;

			if ((i % 5) == 0)
			{
				sprintf_s( cbuf, 8, "%3d", i * 10 );
				TextOut( hDC_Tapes, 73, y - 5, cbuf, strlen( cbuf ) );
			}
			else
			{
				MoveToEx( hDC_Tapes, 72, y, NULL );
				LineTo( hDC_Tapes, 86, y );
			}
		}
		SetTextColor( hDC_Tapes, CR_WHITE );
		for (int i = 0; i >= -10; i -= 1)
		{
			y = Round( ((20 - i) * 7.6) + offset ) + 2904;

			if ((i % 5) == 0)
			{
				sprintf_s( cbuf, 8, "%3d", i * 10 );
				TextOut( hDC_Tapes, 73, y - 5, cbuf, strlen( cbuf ) );
			}
			else
			{
				MoveToEx( hDC_Tapes, 72, y, NULL );
				LineTo( hDC_Tapes, 86, y );
			}
		}

		// Hdot
		// small scale 160ft in window (1ft = 0.7125px) (800-(-800))
		// large scale 800ft in window (1ft = 0.1425px) (3000-800/(-800)-(-3000))
		// 313.5 + 1140 + 313.5 = 1767px long + offsets
		SelectObject( hDC_Tapes, gdiLightGrayPen );
		SelectObject( hDC_Tapes, gdiLightGrayBrush );
		Rectangle( hDC_Tapes, 91, 884 + offset, 113, 1767 + offset + offset );

		SetTextColor( hDC_Tapes, CR_BLACK );
		SetBkMode( hDC_Tapes, TRANSPARENT );
		SelectObject( hDC_Tapes, gdiBlackPen );
		for (int i = 30; i >= 8; i--)
		{
			y = Round( ((30 - i) * 14.25) + offset );

			if (i < 10)
			{
				sprintf_s( cbuf, 8, "%4d", i * 100 );
				TextOut( hDC_Tapes, 94, y - 5, cbuf, strlen( cbuf ) );
			}
			else if ((i % 5) == 0)
			{
				sprintf_s( cbuf, 8, "%3.1fK", (double)i / 10 );
				TextOut( hDC_Tapes, 94, y - 5, cbuf, strlen( cbuf ) );
			}
			else
			{
				MoveToEx( hDC_Tapes, 95, y, NULL );
				LineTo( hDC_Tapes, 109, y );
			}
		}
		for (int i = 79; i >= 0; i--)
		{
			y = Round( ((80 - i) * 7.125) + 313.5 + offset );

			if ((i % 2) == 0)
			{
				sprintf_s( cbuf, 8, "%3d", i * 10 );
				TextOut( hDC_Tapes, 96, y - 5, cbuf, strlen( cbuf ) );
			}
			else
			{
				MoveToEx( hDC_Tapes, 96, y, NULL );
				LineTo( hDC_Tapes, 108, y );
			}
		}
		SetTextColor( hDC_Tapes, CR_WHITE );
		SelectObject( hDC_Tapes, gdiWhitePen );
		for (int i = -1; i >= -79; i--)
		{
			y = Round( ((80 - i) * 7.125) + 313.5 + offset );

			if ((i % 2) == 0)
			{
				sprintf_s( cbuf, 8, "%4d", i * 10 );
				TextOut( hDC_Tapes, 94, y - 5, cbuf, strlen( cbuf ) );
			}
			else
			{
				MoveToEx( hDC_Tapes, 96, y, NULL );
				LineTo( hDC_Tapes, 108, y );
			}
		}
		for (int i = -8; i >= -30; i--)
		{
			y = Round( ((-8 - i) * 14.25) + 313.5 + 1140 + offset );

			if (i > -10)
			{
				sprintf_s( cbuf, 8, "%4d", i * 100 );
				TextOut( hDC_Tapes, 94, y - 5, cbuf, strlen( cbuf ) );
			}
			else if ((i % 5) == 0)
			{
				sprintf_s( cbuf, 8, "%4.1fK", (double)i / 10 );
				TextOut( hDC_Tapes, 93, y - 5, cbuf, strlen( cbuf ) );
			}
			else
			{
				MoveToEx( hDC_Tapes, 95, y, NULL );
				LineTo( hDC_Tapes, 109, y );
			}
		}
		return;
	}

	void MDU::ADI_Create( void )
	{
		hDC_ADI = CreateCompatibleDC( GetDC( NULL ) );
		HBITMAP hBM = CreateCompatibleBitmap( GetDC( NULL ), 100, 100 );
		SelectObject( hDC_ADI, hBM );
		SelectObject( hDC_ADI, gdiTahomaFont_h7w3 );

		hDC_ADIMASK = CreateCompatibleDC( hDC_ADI );
		hBM = CreateCompatibleBitmap( hDC_ADI, 100, 100 );
		SelectObject( hDC_ADIMASK, hBM );
		SelectObject( hDC_ADIMASK, gdiWhitePen );
		SelectObject( hDC_ADIMASK, gdiWhiteBrush );
		::Ellipse( hDC_ADIMASK, 1, 1, 99, 99 );

		hDC_ADI_ORBIT = CreateCompatibleDC( hDC_ADI );
		hBM = CreateCompatibleBitmap( hDC_ADI, 112, 112 );
		SelectObject( hDC_ADI_ORBIT, hBM );
		SelectObject( hDC_ADI_ORBIT, gdiTahomaFont_h10w4 );

		hDC_ADIMASK_ORBIT = CreateCompatibleDC( hDC_ADI );
		hBM = CreateCompatibleBitmap( hDC_ADI, 112, 112 );
		SelectObject( hDC_ADIMASK_ORBIT, hBM );
		SelectObject( hDC_ADIMASK_ORBIT, gdiWhitePen );
		SelectObject( hDC_ADIMASK_ORBIT, gdiWhiteBrush );
		::Ellipse( hDC_ADIMASK_ORBIT, 1, 1, 111, 111 );
		return;
	}

	void MDU::Tape_KEAS_MVR( HDC hDC, double MachNumber )
	{
		char cbuf[8];
		double keas = sqrt( STS()->GetDynPressure() * PA2PSF ) * 17.18;
		SelectObject( hDC, gdiWhitePen );
		SelectObject( hDC, gdiBlackBrush );
		Rectangle( hDC, 9, 39, 33, 155 );
		SelectObject( hDC, gdiDarkGrayPen );
		Rectangle( hDC, 9, 159, 33, 171 );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 12, 30, "KEAS", 4 );
		TextOut( hDC, 12, 172, "M/VR", 4 );

		double tapekeas = keas;
		if (tapekeas > 500) tapekeas = 500;
		BitBlt( hDC, 10, 40, 22, 114, hDC_Tapes, 23, 1900 - Round( tapekeas * 3.8 ), SRCCOPY );

		Rectangle( hDC, 9, 91, 33, 105 );
		SetTextColor( hDC, CR_WHITE );
		sprintf_s( cbuf, 8, "%3.0f", keas );
		TextOut( hDC, 15, 93, cbuf, strlen( cbuf ) );
		sprintf_s( cbuf, 8, "%5.2f", MachNumber );
		TextOut( hDC, 13, 160, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::Tape_MVR_KEAS( HDC hDC, double MachNumber )
	{
		char cbuf[8];
		int pos;
		double keas = sqrt( STS()->GetDynPressure() * PA2PSF ) * 17.18;
		SelectObject( hDC, gdiWhitePen );
		SelectObject( hDC, gdiBlackBrush );
		Rectangle( hDC, 9, 39, 33, 155 );
		SelectObject( hDC, gdiDarkGrayPen );
		Rectangle( hDC, 9, 159, 33, 171 );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 12, 30, "M/VR", 4 );
		TextOut( hDC, 12, 172, "KEAS", 4 );
		SetTextColor( hDC, CR_WHITE );
		if (MachNumber > 4)
		{
			double vr = STS()->GetAirspeed() * MPS2FPS;

			pos = 3078 - Round( vr * 0.114 );
			BitBlt( hDC, 10, 40, 22, 114, hDC_Tapes, 0, pos, SRCCOPY );

			Rectangle( hDC, 9, 91, 33, 105 );
			sprintf_s( cbuf, 8, "%5.0f", vr );// ft
			TextOut( hDC, 11, 93, cbuf, strlen( cbuf ) );
		}
		else
		{
			pos = 3534 - Round( MachNumber * 228 );
			BitBlt( hDC, 10, 40, 22, 114, hDC_Tapes, 0, pos, SRCCOPY );

			Rectangle( hDC, 9, 91, 33, 105 );
			sprintf_s( cbuf, 8, "%5.2f", MachNumber );// Mach
			TextOut( hDC, 12, 93, cbuf, strlen( cbuf ) );
		}
		sprintf_s( cbuf, 8, "%3.0f", keas );
		TextOut( hDC, 14, 160, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::Tape_MVI_KEAS( HDC hDC, double MachNumber )
	{
		char cbuf[8];
		int pos;
		double keas = sqrt( STS()->GetDynPressure() * PA2PSF ) * 17.18;
		SelectObject( hDC, gdiWhitePen );
		SelectObject( hDC, gdiBlackBrush );
		Rectangle( hDC, 9, 39, 33, 155 );
		SelectObject( hDC, gdiDarkGrayPen );
		Rectangle( hDC, 9, 159, 33, 171 );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 12, 30, "M/VI", 4 );
		TextOut( hDC, 12, 172, "KEAS", 4 );
		SetTextColor( hDC, CR_WHITE );
		if (MachNumber > 4)
		{
			VECTOR3 v3vi;
			STS()->GetRelativeVel( STS()->GetSurfaceRef(), v3vi );
			double vi = length( v3vi ) * MPS2FPS;
			
			pos = 3078 - Round( vi * 0.114 );
			BitBlt( hDC, 10, 40, 22, 114, hDC_Tapes, 0, pos, SRCCOPY );

			Rectangle( hDC, 9, 91, 33, 105 );
			sprintf_s( cbuf, 8, "%5.0f", vi );// ft
			TextOut( hDC, 11, 93, cbuf, strlen( cbuf ) );
		}
		else
		{
			pos = 3534 - Round( MachNumber * 228 );
			BitBlt( hDC, 10, 40, 22, 114, hDC_Tapes, 0, pos, SRCCOPY );

			Rectangle( hDC, 9, 91, 33, 105 );
			sprintf_s( cbuf, 8, "%5.2f", MachNumber );// Mach
			TextOut( hDC, 12, 93, cbuf, strlen( cbuf ) );
		}
		sprintf_s( cbuf, 8, "%3.0f", keas );
		TextOut( hDC, 14, 160, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::Tape_Alpha( HDC hDC, double MachNumber )
	{
		SelectObject( hDC, gdiLightGrayPen );
		SelectObject( hDC, gdiBlackBrush );
		::Ellipse( hDC, 46, 34, 50, 38 );
		SetPixel( hDC, 50, 34, CR_LIGHT_GRAY );
		SetPixel( hDC, 50, 37, CR_LIGHT_GRAY );
		SelectObject( hDC, gdiWhitePen );
		Rectangle( hDC, 37, 39, 60, 155 );

		double alpha = STS()->GetAOA() * DEG;

		BitBlt( hDC, 38, 40, 21, 114, hDC_Tapes, 45, 1026 - Round( alpha * 5.7 ), SRCCOPY );

		if ((MachNumber > 0) && (MachNumber < 3))
		{
			// max L/D (linear aprox)
			double maxLD;
			if (MachNumber < 0.9) maxLD = 10.5;
			else if (MachNumber < 1.1) maxLD = (MachNumber * 15) - 3;
			else if (MachNumber < 2) maxLD = (MachNumber * 1.6667) + 11.6667;
			else maxLD = (MachNumber * 2) + 11;

			maxLD -= alpha;
			if (fabs( maxLD ) < 10)
			{
				int pos = Round( maxLD * 5.7 );
				POINT diamond[4];
				diamond[0].x = 56;// start at top moving cw
				diamond[0].y = 94 - pos;
				diamond[1].x = 60;
				diamond[1].y = 98 - pos;
				diamond[2].x = 56;
				diamond[2].y = 102 - pos;
				diamond[3].x = 52;
				diamond[3].y = 98 - pos;
				SelectObject( hDC, gdiMagentaBrush );
				SelectObject( hDC, gdiBlackPen );
				Polygon( hDC, diamond, 4 );

				diamond[0].y += 3;
				diamond[1].x = 57;
				diamond[2].y -= 3;
				diamond[3].x = 55;
				SelectObject( hDC, gdiBlackBrush );
				Polygon( hDC, diamond, 4 );
			}
		}

		SelectObject( hDC, gdiDarkGrayPen );
		SelectObject( hDC, gdiBlackBrush );
		POINT poly[5] = {{59,98},{53,105},{35,105},{35,91},{53,91}};// start at tip moving cw
		Polygon( hDC, poly, 5 );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%6.1f", alpha );
		TextOut( hDC, 36, 93, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::Tape_H_Hdot( HDC hDC, double Altitude_ft, double Hdot )
	{
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 212, 30, "H", 1 );
		SelectObject( hDC, gdiWhitePen );
		Rectangle( hDC, 203, 39, 227, 155 );
		int pos;
		double Altitude_NM = 0;
		if (Altitude_ft > 400000)
		{
			Altitude_NM = Altitude_ft * F2NM;
			if (Altitude_NM > 165) pos = 1881;
			else pos = 1881 - Round( Altitude_NM * 11.4 );
		}
		else if (Altitude_ft > 100000)
		{
			pos = 1587 - Round( Altitude_ft * 0.00114 );
		}
		else if (Altitude_ft > 30000)
		{
			pos = 2464 - Round( Altitude_ft * 0.00991 );
		}
		else if (Altitude_ft > 2000)
		{
			pos = 2737 - Round( Altitude_ft * 0.019 );
		}
		else if (Altitude_ft > 200)
		{
			pos = 2927 - Round( Altitude_ft * 0.114 );
		}// HACK below 0
		else// if (Altitude_ft > 0)
		{
			pos = 3056 - Round( Altitude_ft * 0.76 );
		}
		/*else if (Altitude_ft > -100)
		{
			pos = 0;// TODO
		}
		else if (Altitude_ft > -1100)
		{
			pos = 0;// TODO
		}
		else
		{
			if (Altitude_ft < -1100) Altitude_ft = -1100;
			pos = 0;// TODO
		}*/
		BitBlt( hDC, 204, 40, 22, 114, hDC_Tapes, 68, pos, SRCCOPY );

		SelectObject( hDC, gdiDarkGrayPen );
		SelectObject( hDC, gdiBlackBrush );
		Rectangle( hDC, 203, 91, 227, 105 );
		SetTextColor( hDC, CR_WHITE );
		char cbuf[8];
		if (Altitude_ft < 10000)
		{
			sprintf_s( cbuf, 8, "%4d", (int)Altitude_ft );
			TextOut( hDC, 207, 93, cbuf, strlen( cbuf ) );
		}
		else if (Altitude_ft < 400000)
		{
			sprintf_s( cbuf, 8, "%3d", (int)Altitude_ft / 1000 );
			TextOut( hDC, 206, 93, cbuf, strlen( cbuf ) );
			SetTextColor( hDC, CR_LIGHT_GRAY );
			TextOut( hDC, 220, 93, "K", 1 );
		}
		else
		{
			sprintf_s( cbuf, 8, "%3.0f", Altitude_NM );
			TextOut( hDC, 206, 93, cbuf, strlen( cbuf ) );
			SetTextColor( hDC, CR_LIGHT_GRAY );
			TextOut( hDC, 220, 93, "M", 1 );
		}

		SetTextColor( hDC, CR_LIGHT_GRAY );
		SetPixel( hDC, 241, 29, CR_LIGHT_GRAY );
		TextOut( hDC, 239, 30, "H", 1 );
		SelectObject( hDC, gdiWhitePen );
		Rectangle( hDC, 230, 39, 254, 155 );
		if (Hdot > 800)
		{
			if (Hdot > 3000) Hdot = 3000;
			pos = Round( (3000 - Hdot) * 0.1425 );
		}
		else if (Hdot > -800)
		{
			pos = Round( 883.5 - (Hdot * 0.7125) );
		}
		else
		{
			if (Hdot < -3000) Hdot = -3000;
			pos = Round( 1339.5 - (Hdot * 0.1425) );
		}
		BitBlt( hDC, 231, 40, 22, 114, hDC_Tapes, 91, pos, SRCCOPY );

		SelectObject( hDC, gdiDarkGrayPen );
		SelectObject( hDC, gdiBlackBrush );
		Rectangle( hDC, 230, 91, 254, 105 );

		SetTextColor( hDC, CR_WHITE );
		if ((Hdot < 1000) && (Hdot > -1000))
		{
			sprintf_s( cbuf, 8, "%4.0f", Hdot );
			TextOut( hDC, 233, 93, cbuf, strlen( cbuf ) );
		}
		else
		{
			int itmp = (int)Hdot;
			Hdot = itmp % 100;
			if (Hdot == 0) Hdot = (double)itmp / 1000;
			else Hdot = ((itmp - Hdot) / 1000);
			sprintf_s( cbuf, 8, "%4.1f", Hdot );
			TextOut( hDC, 231, 93, cbuf, strlen( cbuf ) );

			SetTextColor( hDC, CR_LIGHT_GRAY );
			TextOut( hDC, 245, 93, "K", 1 );
		}
		return;
	}

	void MDU::Tapes_Invalid( HDC hDC )
	{
		SelectObject( hDC, gdiRedPen );
		SelectObject( hDC, gdiBlackBrush );
		Rectangle( hDC, 9, 39, 33, 155 );
		Rectangle( hDC, 9, 91, 33, 105 );
		Rectangle( hDC, 9, 159, 33, 171 );
		Rectangle( hDC, 37, 39, 60, 155 );
		POINT poly[5] = {{59,98},{53,105},{35,105},{35,91},{53,91}};// start at tip moving cw
		Polygon( hDC, poly, 5 );

		Rectangle( hDC, 203, 39, 227, 155 );
		Rectangle( hDC, 203, 91, 227, 105 );
		Rectangle( hDC, 230, 39, 254, 155 );
		Rectangle( hDC, 230, 91, 254, 105 );

		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 12, 30, "M/V", 3 );
		TextOut( hDC, 12, 172, "KEAS", 4 );

		SelectObject( hDC, gdiLightGrayPen );
		::Ellipse( hDC, 46, 34, 50, 38 );
		SetPixel( hDC, 50, 34, CR_LIGHT_GRAY );
		SetPixel( hDC, 50, 37, CR_LIGHT_GRAY );

		TextOut( hDC, 212, 30, "H", 1 );
		SetPixel( hDC, 241, 29, CR_LIGHT_GRAY );
		TextOut( hDC, 239, 30, "H", 1 );
		return;
	}

	void MDU::Tapes_Invalid( oapi::Sketchpad2* skp )
	{
		skp->SetPen( skpRedPen );
		skp->SetBrush( skpBlackBrush );
		skp->Rectangle( 9, 39, 33, 155 );
		skp->Rectangle( 9, 91, 33, 105 );
		skp->Rectangle( 9, 159, 33, 171 );
		skp->Rectangle( 37, 39, 60, 155 );
		oapi::IVECTOR2 poly[5] = {{59,98},{53,105},{35,105},{35,91},{53,91}};// start at tip moving cw
		skp->Polygon( poly, 5 );

		skp->Rectangle( 203, 39, 227, 155 );
		skp->Rectangle( 203, 91, 227, 105 );
		skp->Rectangle( 230, 39, 254, 155 );
		skp->Rectangle( 230, 91, 254, 105 );

		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 12, 30, "M/V", 3 );
		skp->Text( 12, 172, "KEAS", 4 );

		skp->SetPen( skpLightGrayPen );
		skp->Ellipse( 46, 34, 50, 38 );
		skp->Pixel( 50, 34, CR_LIGHT_GRAY );
		skp->Pixel( 50, 37, CR_LIGHT_GRAY );

		skp->Text( 212, 30, "H", 1 );
		skp->Pixel( 241, 29, CR_LIGHT_GRAY );
		skp->Text( 239, 30, "H", 1 );
		return;
	}

	void MDU::ADI_STATIC( HDC hDC )
	{
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 174, 25, "R", 1 );
		TextOut( hDC, 174, 32, "P", 1 );
		TextOut( hDC, 174, 39, "Y", 1 );

		SelectObject( hDC, gdiWhitePen );
		::Ellipse( hDC, 65, 37, 179, 151 );

		// rate scales
		// top scale
		SelectObject( hDC, gdiDarkGrayPen );
		MoveToEx( hDC, 87, 27, NULL );
		LineTo( hDC, 157, 27 );
		MoveToEx( hDC, 87, 27, NULL );
		LineTo( hDC, 87, 34 );
		MoveToEx( hDC, 94, 27, NULL );
		LineTo( hDC, 94, 32 );
		MoveToEx( hDC, 101, 27, NULL );
		LineTo( hDC, 101, 32 );
		MoveToEx( hDC, 108, 27, NULL );
		LineTo( hDC, 108, 32 );
		MoveToEx( hDC, 115, 27, NULL );
		LineTo( hDC, 115, 32 );
		MoveToEx( hDC, 122, 27, NULL );
		LineTo( hDC, 122, 34 );
		MoveToEx( hDC, 129, 27, NULL );
		LineTo( hDC, 129, 32 );
		MoveToEx( hDC, 136, 27, NULL );
		LineTo( hDC, 136, 32 );
		MoveToEx( hDC, 143, 27, NULL );
		LineTo( hDC, 143, 32 );
		MoveToEx( hDC, 150, 27, NULL );
		LineTo( hDC, 150, 32 );
		MoveToEx( hDC, 157, 27, NULL );
		LineTo( hDC, 157, 34 );

		// side scale
		MoveToEx( hDC, 190, 59, NULL );
		LineTo( hDC, 190, 129 );
		MoveToEx( hDC, 190, 59, NULL );
		LineTo( hDC, 183, 59 );
		MoveToEx( hDC, 190, 66, NULL );
		LineTo( hDC, 185, 66 );
		MoveToEx( hDC, 190, 73, NULL );
		LineTo( hDC, 185, 73 );
		MoveToEx( hDC, 190, 80, NULL );
		LineTo( hDC, 185, 80 );
		MoveToEx( hDC, 190, 87, NULL );
		LineTo( hDC, 185, 87 );
		MoveToEx( hDC, 190, 94, NULL );
		LineTo( hDC, 183, 94 );
		MoveToEx( hDC, 190, 101, NULL );
		LineTo( hDC, 185, 101 );
		MoveToEx( hDC, 190, 108, NULL );
		LineTo( hDC, 185, 108 );
		MoveToEx( hDC, 190, 115, NULL );
		LineTo( hDC, 185, 115 );
		MoveToEx( hDC, 190, 122, NULL );
		LineTo( hDC, 185, 122 );
		MoveToEx( hDC, 190, 129, NULL );
		LineTo( hDC, 183, 129 );

		// bottom scale
		MoveToEx( hDC, 87, 163, NULL );
		LineTo( hDC, 157, 163 );
		MoveToEx( hDC, 87, 163, NULL );
		LineTo( hDC, 87, 156 );
		MoveToEx( hDC, 94, 163, NULL );
		LineTo( hDC, 94, 158 );
		MoveToEx( hDC, 101, 163, NULL );
		LineTo( hDC, 101, 158 );
		MoveToEx( hDC, 108, 163, NULL );
		LineTo( hDC, 108, 158 );
		MoveToEx( hDC, 115, 163, NULL );
		LineTo( hDC, 115, 158 );
		MoveToEx( hDC, 122, 163, NULL );
		LineTo( hDC, 122, 156 );
		MoveToEx( hDC, 129, 163, NULL );
		LineTo( hDC, 129, 158 );
		MoveToEx( hDC, 136, 163, NULL );
		LineTo( hDC, 136, 158 );
		MoveToEx( hDC, 143, 163, NULL );
		LineTo( hDC, 143, 158 );
		MoveToEx( hDC, 150, 163, NULL );
		LineTo( hDC, 150, 158 );
		MoveToEx( hDC, 157, 163, NULL );
		LineTo( hDC, 157, 156 );

		// roll lines
		// 10º/190º
		MoveToEx( hDC, 113, 43, NULL );
		LineTo( hDC, 131, 145 );
		// 20º/200º
		MoveToEx( hDC, 104, 45, NULL );
		LineTo( hDC, 140, 143 );
		// 40º/220º
		MoveToEx( hDC, 89, 54, NULL );
		LineTo( hDC, 155, 134 );
		// 50º/230º
		MoveToEx( hDC, 82, 61, NULL );
		LineTo( hDC, 162, 127 );
		// 70º/250º
		MoveToEx( hDC, 73, 76, NULL );
		LineTo( hDC, 171, 112 );
		// 80º/260º
		MoveToEx( hDC, 71, 85, NULL );
		LineTo( hDC, 173, 103 );
		// 100º/280º
		MoveToEx( hDC, 71, 103, NULL );
		LineTo( hDC, 173, 85 );
		// 110º/290º
		MoveToEx( hDC, 73, 112, NULL );
		LineTo( hDC, 171, 76 );
		// 130º/310º
		MoveToEx( hDC, 82, 127, NULL );
		LineTo( hDC, 162, 61 );
		// 140º/320º
		MoveToEx( hDC, 89, 134, NULL );
		LineTo( hDC, 155, 54 );
		// 160º/340º
		MoveToEx( hDC, 104, 143, NULL );
		LineTo( hDC, 140, 45 );
		// 170º/350º
		MoveToEx( hDC, 113, 145, NULL );
		LineTo( hDC, 131, 43 );
		SelectObject( hDC, gdiDarkGrayThickPen );
		// 30º/210º
		MoveToEx( hDC, 96, 49, NULL );
		LineTo( hDC, 148, 139 );
		// 60º/240º
		MoveToEx( hDC, 77, 68, NULL );
		LineTo( hDC, 167, 120 );
		// 120º/300º
		MoveToEx( hDC, 77, 120, NULL );
		LineTo( hDC, 167, 68 );
		// 150º/330º
		MoveToEx( hDC, 96, 139, NULL );
		LineTo( hDC, 148, 49 );
		// cover center for ADI SRCPAINT
		SelectObject( hDC, gdiBlackBrush );
		SelectObject( hDC, gdiBlackPen );
		::Ellipse( hDC, 73, 45, 171, 143 );

		SelectObject( hDC, gdiTahomaFont_h7w3 );
		TextOut( hDC, 149, 48, "33", 2 );
		TextOut( hDC, 162, 61, "30", 2 );
		TextOut( hDC, 162, 120, "24", 2 );
		TextOut( hDC, 149, 134, "21", 2 );
		TextOut( hDC, 89, 134, "15", 2 );
		TextOut( hDC, 77, 121, "12", 2 );
		TextOut( hDC, 76, 61, "06", 2 );
		TextOut( hDC, 89, 47, "03", 2 );
		SelectObject( hDC, gdiTahomaFont_h10w4 );
		SelectObject( hDC, gdiLightGrayPen );
		SelectObject( hDC, gdiLightGrayBrush );
		POINT diamond[4];
		diamond[0].x = 122;
		diamond[0].y = 40;
		diamond[1].x = 124;
		diamond[1].y = 42;
		diamond[2].x = 122;
		diamond[2].y = 44;
		diamond[3].x = 120;
		diamond[3].y = 42;
		Polygon( hDC, diamond, 4 );// top
		//diamond[0].x = 122;
		diamond[0].y = 143;
		//diamond[1].x = 124;
		diamond[1].y = 145;
		//diamond[2].x = 122;
		diamond[2].y = 147;
		//diamond[3].x = 120;
		diamond[3].y = 145;
		Polygon( hDC, diamond, 4 );// bottom
		diamond[0].x = 173;
		diamond[0].y = 92;
		diamond[1].x = 175;
		diamond[1].y = 94;
		diamond[2].x = 173;
		diamond[2].y = 96;
		diamond[3].x = 171;
		diamond[3].y = 94;
		Polygon( hDC, diamond, 4 );// right
		diamond[0].x = 70;
		//diamond[0].y = 92;
		diamond[1].x = 72;
		//diamond[1].y = 94;
		diamond[2].x = 70;
		//diamond[2].y = 96;
		diamond[3].x = 68;
		//diamond[3].y = 94;
		Polygon( hDC, diamond, 4 );// left

		// att needles
		// top scale
		SelectObject( hDC, gdiMagentaPen );
		SelectObject( hDC, gdiMagentaBrush );
		Arc( hDC, 67, 39, 177, 149, 147, 45, 97, 45 );
		MoveToEx( hDC, 97, 45, NULL );
		LineTo( hDC, 97, 48 );
		MoveToEx( hDC, 102, 43, NULL );
		LineTo( hDC, 102, 46 );
		MoveToEx( hDC, 107, 41, NULL );
		LineTo( hDC, 107, 44 );
		MoveToEx( hDC, 112, 40, NULL );
		LineTo( hDC, 112, 43 );
		MoveToEx( hDC, 117, 39, NULL );
		LineTo( hDC, 117, 42 );
		MoveToEx( hDC, 127, 39, NULL );
		LineTo( hDC, 127, 42 );
		MoveToEx( hDC, 132, 40, NULL );
		LineTo( hDC, 132, 43 );
		MoveToEx( hDC, 137, 41, NULL );
		LineTo( hDC, 137, 44 );
		MoveToEx( hDC, 142, 43, NULL );
		LineTo( hDC, 142, 46 );
		MoveToEx( hDC, 147, 45, NULL );
		LineTo( hDC, 147, 48 );

		// side scale
		Arc( hDC, 67, 39, 177, 149, 171, 119, 171, 69 );
		MoveToEx( hDC, 171, 69, NULL );
		LineTo( hDC, 168, 69 );
		MoveToEx( hDC, 172, 74, NULL );
		LineTo( hDC, 169, 74 );
		MoveToEx( hDC, 174, 79, NULL );
		LineTo( hDC, 171, 79 );
		MoveToEx( hDC, 175, 84, NULL );
		LineTo( hDC, 172, 84 );
		MoveToEx( hDC, 176, 89, NULL );
		LineTo( hDC, 173, 89 );
		MoveToEx( hDC, 176, 99, NULL );
		LineTo( hDC, 173, 99 );
		MoveToEx( hDC, 175, 104, NULL );
		LineTo( hDC, 172, 104 );
		MoveToEx( hDC, 174, 109, NULL );
		LineTo( hDC, 171, 109 );
		MoveToEx( hDC, 172, 114, NULL );
		LineTo( hDC, 169, 114 );
		MoveToEx( hDC, 171, 119, NULL );
		LineTo( hDC, 168, 119 );

		// bottom scale
		Arc( hDC, 67, 39, 177, 149, 97, 143, 147, 143 );
		MoveToEx( hDC, 97, 143, NULL );
		LineTo( hDC, 97, 140 );
		MoveToEx( hDC, 102, 144, NULL );
		LineTo( hDC, 102, 141 );
		MoveToEx( hDC, 107, 146, NULL );
		LineTo( hDC, 107, 143 );
		MoveToEx( hDC, 112, 147, NULL );
		LineTo( hDC, 112, 144 );
		MoveToEx( hDC, 117, 148, NULL );
		LineTo( hDC, 117, 145 );
		MoveToEx( hDC, 127, 148, NULL );
		LineTo( hDC, 127, 145 );
		MoveToEx( hDC, 132, 147, NULL );
		LineTo( hDC, 132, 144 );
		MoveToEx( hDC, 137, 146, NULL );
		LineTo( hDC, 137, 143 );
		MoveToEx( hDC, 142, 144, NULL );
		LineTo( hDC, 142, 141 );
		MoveToEx( hDC, 147, 143, NULL );
		LineTo( hDC, 147, 140 );
		return;
	}

	void MDU::ADI_STATIC( oapi::Sketchpad2* skp )
	{
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 174, 25, "R", 1 );
		skp->Text( 174, 32, "P", 1 );
		skp->Text( 174, 39, "Y", 1 );

		skp->SetPen( skpWhitePen );
		skp->Ellipse( 65, 37, 179, 151 );

		// rate scales
		// top scale
		skp->SetPen( skpDarkGrayPen );
		skp->Line( 87, 27, 157, 27 );
		skp->Line( 87, 27, 87, 34 );
		skp->Line( 94, 27, 94, 32 );
		skp->Line( 101, 27, 101, 32 );
		skp->Line( 108, 27, 108, 32 );
		skp->Line( 115, 27, 115, 32 );
		skp->Line( 122, 27, 122, 34 );
		skp->Line( 129, 27, 129, 32 );
		skp->Line( 136, 27, 136, 32 );
		skp->Line( 143, 27, 143, 32 );
		skp->Line( 150, 27, 150, 32 );
		skp->Line( 157, 27, 157, 34 );

		// side scale
		skp->Line( 190, 59, 190, 129 );
		skp->Line( 190, 59, 183, 59 );
		skp->Line( 190, 66, 185, 66 );
		skp->Line( 190, 73, 185, 73 );
		skp->Line( 190, 80, 185, 80 );
		skp->Line( 190, 87, 185, 87 );
		skp->Line( 190, 94, 183, 94 );
		skp->Line( 190, 101, 185, 101 );
		skp->Line( 190, 108, 185, 108 );
		skp->Line( 190, 115, 185, 115 );
		skp->Line( 190, 122, 185, 122 );
		skp->Line( 190, 129, 183, 129 );

		// bottom scale
		skp->Line( 87, 163, 157, 163 );
		skp->Line( 87, 163, 87, 156 );
		skp->Line( 94, 163, 94, 158 );
		skp->Line( 101, 163, 101, 158 );
		skp->Line( 108, 163, 108, 158 );
		skp->Line( 115, 163, 115, 158 );
		skp->Line( 122, 163, 122, 156 );
		skp->Line( 129, 163, 129, 158 );
		skp->Line( 136, 163, 136, 158 );
		skp->Line( 143, 163, 143, 158 );
		skp->Line( 150, 163, 150, 158 );
		skp->Line( 157, 163, 157, 156 );

		// roll lines
		// 10º/190º
		skp->Line( 113, 43, 131, 145 );
		// 20º/200º
		skp->Line( 104, 45, 140, 143 );
		// 40º/220º
		skp->Line( 89, 54, 155, 134 );
		// 50º/230º
		skp->Line( 82, 61, 162, 127 );
		// 70º/250º
		skp->Line( 73, 76, 171, 112 );
		// 80º/260º
		skp->Line( 71, 85, 173, 103 );
		// 100º/280º
		skp->Line( 71, 103, 173, 85 );
		// 110º/290º
		skp->Line( 73, 112, 171, 76 );
		// 130º/310º
		skp->Line( 82, 127, 162, 61 );
		// 140º/320º
		skp->Line( 89, 134, 155, 54 );
		// 160º/340º
		skp->Line( 104, 143, 140, 45 );
		// 170º/350º
		skp->Line( 113, 145, 131, 43 );
		skp->SetPen( skpDarkGrayThickPen );
		// 30º/210º
		skp->Line( 96, 49, 148, 139 );
		// 60º/240º
		skp->Line( 77, 68, 167, 120 );
		// 120º/300º
		skp->Line( 77, 120, 167, 68 );
		// 150º/330º
		skp->Line( 96, 139, 148, 49 );
		// cover center for ADI SRCPAINT
		skp->SetBrush( skpBlackBrush );
		skp->SetPen( skpBlackPen );
		skp->Ellipse( 73, 45, 171, 143 );

		skp->SetFont( skpTahomaFont_h7w3 );
		skp->Text( 149, 48, "33", 2 );
		skp->Text( 162, 61, "30", 2 );
		skp->Text( 162, 120, "24", 2 );
		skp->Text( 149, 134, "21", 2 );
		skp->Text( 89, 134, "15", 2 );
		skp->Text( 77, 121, "12", 2 );
		skp->Text( 76, 61, "06", 2 );
		skp->Text( 89, 47, "03", 2 );
		skp->SetFont( skpTahomaFont_h10w4 );
		skp->SetPen( skpLightGrayPen );
		skp->SetBrush( skpLightGrayBrush );
		oapi::IVECTOR2 diamond[4];
		diamond[0].x = 122;
		diamond[0].y = 40;
		diamond[1].x = 124;
		diamond[1].y = 42;
		diamond[2].x = 122;
		diamond[2].y = 44;
		diamond[3].x = 120;
		diamond[3].y = 42;
		skp->Polygon( diamond, 4 );// top
		//diamond[0].x = 122;
		diamond[0].y = 143;
		//diamond[1].x = 124;
		diamond[1].y = 145;
		//diamond[2].x = 122;
		diamond[2].y = 147;
		//diamond[3].x = 120;
		diamond[3].y = 145;
		skp->Polygon( diamond, 4 );// bottom
		diamond[0].x = 173;
		diamond[0].y = 92;
		diamond[1].x = 175;
		diamond[1].y = 94;
		diamond[2].x = 173;
		diamond[2].y = 96;
		diamond[3].x = 171;
		diamond[3].y = 94;
		skp->Polygon( diamond, 4 );// right
		diamond[0].x = 70;
		//diamond[0].y = 92;
		diamond[1].x = 72;
		//diamond[1].y = 94;
		diamond[2].x = 70;
		//diamond[2].y = 96;
		diamond[3].x = 68;
		//diamond[3].y = 94;
		skp->Polygon( diamond, 4 );// left

		// att needles
		// top scale
		skp->SetPen( skpMagentaPen );
		skp->SetBrush( skpMagentaBrush );
		//Arc( hDC, 67, 39, 177, 149, 147, 45, 97, 45 );
		skp->Line( 97, 45, 97, 48 );
		skp->Line( 102, 43, 102, 46 );
		skp->Line( 107, 41, 107, 44 );
		skp->Line( 112, 40, 112, 43 );
		skp->Line( 117, 39, 117, 42 );
		skp->Line( 127, 39, 127, 42 );
		skp->Line( 132, 40, 132, 43 );
		skp->Line( 137, 41, 137, 44 );
		skp->Line( 142, 43, 142, 46 );
		skp->Line( 147, 45, 147, 48 );

		// side scale
		//Arc( hDC, 67, 39, 177, 149, 171, 119, 171, 69 );
		skp->Line( 171, 69, 168, 69 );
		skp->Line( 172, 74, 169, 74 );
		skp->Line( 174, 79, 171, 79 );
		skp->Line( 175, 84, 172, 84 );
		skp->Line( 176, 89, 173, 89 );
		skp->Line( 176, 99, 173, 99 );
		skp->Line( 175, 104, 172, 104 );
		skp->Line( 174, 109, 171, 109 );
		skp->Line( 172, 114, 169, 114 );
		skp->Line( 171, 119, 168, 119 );

		// bottom scale
		//Arc( hDC, 67, 39, 177, 149, 97, 143, 147, 143 );
		skp->Line( 97, 143, 97, 140 );
		skp->Line( 102, 144, 102, 141 );
		skp->Line( 107, 146, 107, 143 );
		skp->Line( 112, 147, 112, 144 );
		skp->Line( 117, 148, 117, 145 );
		skp->Line( 127, 148, 127, 145 );
		skp->Line( 132, 147, 132, 144 );
		skp->Line( 137, 146, 137, 143 );
		skp->Line( 142, 144, 142, 141 );
		skp->Line( 147, 143, 147, 140 );
		return;
	}

	void MDU::ADI( HDC hDC, double pitch, double roll, double yaw )
	{
		// center (122,94) r = 57
		// view r = 49, ball r = 49 * sqrt( 2 )
		// 90º pitch/yaw "FOV"

		// apply roll
		XFORM WTroll;
		WTroll.eM11 = (FLOAT)cos( roll * RAD );
		WTroll.eM12 = (FLOAT)(-sin( roll * RAD ));
		WTroll.eM21 = -WTroll.eM12;
		WTroll.eM22 = WTroll.eM11;
		WTroll.eDx = (FLOAT)(50 * (1 - WTroll.eM11 - WTroll.eM21));
		WTroll.eDy = (FLOAT)(50 * (1 - WTroll.eM11 + WTroll.eM21));
		SetGraphicsMode( hDC_ADI, GM_ADVANCED );
		SetWorldTransform( hDC_ADI, &WTroll );

		if (pitch > 180) pitch -= 360;// TODO get rid of this

		// 0º pitch line/labels
		double sinpitch = sin( pitch * RAD );
		double cospitch = cos( pitch * RAD );
		int hP;
		double hP2;
		SetTextColor( hDC_ADI, CR_BLACK );
		SetBkColor( hDC_ADI, CR_WHITE );
		if (fabs( pitch ) <= 45)
		{
			hP = Round( 69.296 * sinpitch );
			if (pitch < 0)
			{
				SelectObject( hDC_ADI, gdiWhiteBrush );
				SelectObject( hDC_ADI, gdiWhitePen );
				Rectangle( hDC_ADI, 0, 0, 100, 50 );
				SelectObject( hDC_ADI, gdiLightGrayBrush );
				SelectObject( hDC_ADI, gdiLightGrayPen );
				Chord( hDC_ADI, -19, 50 - hP, 119, 50 + hP, 119, 50, -19, 50 );
				Rectangle( hDC_ADI, 0, 50, 100, 100 );
				SelectObject( hDC_ADI, gdiBlackPen );
				Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, 119, 50, -19, 50 );
			}
			else
			{
				SelectObject( hDC_ADI, gdiLightGrayBrush );
				SelectObject( hDC_ADI, gdiLightGrayPen );
				Rectangle( hDC_ADI, 0, 50, 100, 100 );
				SelectObject( hDC_ADI, gdiWhiteBrush );
				SelectObject( hDC_ADI, gdiWhitePen );
				Chord( hDC_ADI, -19, 50 - hP, 119, 50 + hP, -19, 50, 119, 50 );
				Rectangle( hDC_ADI, 0, 0, 100, 50 );
				SelectObject( hDC_ADI, gdiBlackPen );
				Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, -19, 50, 119, 50 );
			}
			hP = 46 + Round( 66.935245 * sinpitch );
			TextOut( hDC_ADI, 31, hP, "0", 1 );
			TextOut( hDC_ADI, 67, hP, "0", 1 );
		}
		else if (pitch > 0)
		{
			SelectObject( hDC_ADI, gdiWhiteBrush );
			Rectangle( hDC_ADI, 0, 0, 100, 100 );
		}
		else
		{
			SelectObject( hDC_ADI, gdiLightGrayBrush );
			Rectangle( hDC_ADI, 0, 0, 100, 100 );
		}

		// pitch lines/labels for +30º/+60º/+90º/+120º/+150º
		SelectObject( hDC_ADI, gdiDarkGrayPen );
		// +30º
		if (fabs( pitch - 30 ) <= 45)
		{
			hP2 = sinpitch * 60.012499 - cospitch * 34.648232;//hP = 69.296 * sin( (pitch - 30) * RAD );
			hP = Round( hP2 );
			if (pitch < 30) Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, 119, 50, -19, 50 );
			else Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, -19, 50, 119, 50 );
			hP = 46 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 31, hP, "3", 1 );
			TextOut( hDC_ADI, 67, hP, "3", 1 );
		}
		// +60º
		if (fabs( pitch - 60 ) <= 45)
		{
			hP2 = sinpitch * 34.648232 - cospitch * 60.012499;
			hP = Round( hP2 );
			if (pitch < 60) Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, 119, 50, -19, 50 );
			else Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, -19, 50, 119, 50 );
			hP = 46 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 31, hP, "6", 1 );
			TextOut( hDC_ADI, 67, hP, "6", 1 );
		}
		// +90º
		if (fabs( pitch - 90 ) <= 45)
		{
			hP2 = 69.296465 * (-cospitch);
			hP = Round( hP2 );
			if (pitch < 90) Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, 119, 50, -19, 50 );
			else Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, -19, 50, 119, 50 );
			hP = 46 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 31, hP, "9", 1 );
			TextOut( hDC_ADI, 67, hP, "9", 1 );
		}
		// +120º
		if (fabs( pitch - 120 ) <= 45)
		{
			hP2 = (-sinpitch) * 34.648232 - cospitch * 60.012499;
			hP = Round( hP2 );
			if (pitch < 120) Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, 119, 50, -19, 50 );
			else Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, -19, 50, 119, 50 );
			hP = 46 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 30, hP, "12", 2 );
			TextOut( hDC_ADI, 66, hP, "12", 2 );
		}
		// +150º
		if (fabs( pitch - 150 ) <= 45)
		{
			hP2 = (-sinpitch) * 60.012499 - cospitch * 34.648232;
			hP = Round( hP2 );
			if (pitch < 150) Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, 119, 50, -19, 50 );
			else Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, -19, 50, 119, 50 );
			hP = 46 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 30, hP, "15", 2 );
			TextOut( hDC_ADI, 66, hP, "15", 2 );
		}

		// pitch lines/labels for -30º/-60º/-90º/-120º/-150º
		SelectObject( hDC_ADI, gdiWhitePen );
		SetTextColor( hDC_ADI, CR_WHITE );
		SetBkColor( hDC_ADI, CR_LIGHT_GRAY );
		// -30º
		if (fabs( pitch + 30 ) <= 45)
		{
			hP2 = sinpitch * 60.012499 + cospitch * 34.648232;
			hP = Round( hP2 );
			if (pitch < -30) Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, 119, 50, -19, 50 );
			else Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, -19, 50, 119, 50 );
			hP = 46 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 30, hP, "33", 2 );
			TextOut( hDC_ADI, 66, hP, "33", 2 );
		}
		// -60º
		if (fabs( pitch + 60 ) <= 45)
		{
			hP2 = sinpitch * 34.648232 + cospitch * 60.012499;
			hP = Round( hP2 );
			if (pitch < -60) Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, 119, 50, -19, 50 );
			else Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, -19, 50, 119, 50 );
			hP = 46 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 30, hP, "30", 2 );
			TextOut( hDC_ADI, 66, hP, "30", 2 );
		}
		// -90º
		if (fabs( pitch + 90 ) <= 45)
		{
			hP2 = 69.296 * cospitch;
			hP = Round( hP2 );
			if (pitch < -90) Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, 119, 50, -19, 50 );
			else Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, -19, 50, 119, 50 );
			hP = 46 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 30, hP, "27", 2 );
			TextOut( hDC_ADI, 66, hP, "27", 2 );
		}
		// -120º
		if (fabs( pitch + 120 ) <= 45)
		{
			hP2 = (-sinpitch) * 34.648232 + cospitch * 60.012499;
			hP = Round( hP2 );
			if (pitch < -120) Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, 119, 50, -19, 50 );
			else Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, -19, 50, 119, 50 );
			hP = 46 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 30, hP, "24", 2 );
			TextOut( hDC_ADI, 66, hP, "24", 2 );
		}
		// -150º
		if (fabs( pitch + 150 ) <= 45)
		{
			hP2 = (-sinpitch) * 60.012499 + cospitch * 34.648232;
			hP = Round( hP2 );
			if (pitch < -150) Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, 119, 50, -19, 50 );
			else Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, -19, 50, 119, 50 );
			hP = 46 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 30, hP, "21", 2 );
			TextOut( hDC_ADI, 66, hP, "21", 2 );
		}

		// TODO yaw
		// HACK
		// "central plane"
		SelectObject( hDC_ADI, gdiWhiteBrush );
		SelectObject( hDC_ADI, gdiBlackPen );
		Rectangle( hDC_ADI, 48, 0, 52, 100 );
		SelectObject( hDC_ADI, gdiDarkGrayPen );
		MoveToEx( hDC_ADI, 50, 0, NULL );
		LineTo( hDC_ADI, 50, 100 );

		SelectObject( hDC_ADI, gdiDarkGrayPen );
		// yaw line 30º (above horizon)
		MoveToEx( hDC_ADI, 85, 0, NULL );
		LineTo( hDC_ADI, 85, 50 + Round( 60.012096 * sinpitch ) );
		// yaw line 330º (above horizon)
		MoveToEx( hDC_ADI, 15, 0, NULL );
		LineTo( hDC_ADI, 15, 50 + Round( 60.012096 * sinpitch ) );
		SelectObject( hDC_ADI, gdiWhitePen );
		// yaw line 30º (below horizon)
		MoveToEx( hDC_ADI, 85, 100, NULL );
		LineTo( hDC_ADI, 85, 50 + Round( 60.012096 * sinpitch ) );
		// yaw line 330º (below horizon)
		MoveToEx( hDC_ADI, 15, 100, NULL );
		LineTo( hDC_ADI, 15, 50 + Round( 60.012096 * sinpitch ) );

		// TODO yaw labels

		// roll triangle
		SelectObject( hDC_ADI, gdiLightGreenPen );
		SelectObject( hDC_ADI, gdiLightGreenBrush );
		POINT tri[3];
		tri[0].x = 50;
		tri[0].y = 1;
		tri[1].x = 46;
		tri[1].y = 9;
		tri[2].x = 54;
		tri[2].y = 9;
		Polygon( hDC_ADI, tri, 3 );
		
		// clean up
		ModifyWorldTransform( hDC_ADI, &WTroll, MWT_IDENTITY );
		SetGraphicsMode( hDC_ADI, GM_COMPATIBLE );

		// flight director
		SelectObject( hDC_ADI, gdiLightGreenThickPen );
		Arc( hDC_ADI, 44, 44, 56, 57, 44, 50, 56, 50 );
		SelectObject( hDC_ADI, gdiLightGreenPen );
		SelectObject( hDC_ADI, gdiLightGreenBrush );
		Rectangle( hDC_ADI, 49, 33, 51, 67 );
		Rectangle( hDC_ADI, 33, 49, 67, 51 );

		// digital RPY
		if (pitch < 0) pitch += 360;// TODO get rid of this
		SetTextColor( hDC, CR_WHITE );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%03.0f", roll );
		TextOut( hDC, 180, 25, cbuf, strlen( cbuf ) );
		sprintf_s( cbuf, 8, "%03.0f", pitch );
		TextOut( hDC, 180, 32, cbuf, strlen( cbuf ) );
		sprintf_s( cbuf, 8, "%03.0f", yaw );
		TextOut( hDC, 180, 39, cbuf, strlen( cbuf ) );

		BitBlt( hDC_ADI, 0, 0, 100, 100, hDC_ADIMASK, 0, 0, SRCAND );
		BitBlt( hDC, 72, 44, 100, 100, hDC_ADI, 0, 0, SRCPAINT );
		return;
	}

	void MDU::ADI( oapi::Sketchpad2* skp, double pitch, double roll, double yaw )
	{
		// center (122,94) r = 57
		// view r = 49, ball r = 49 * sqrt( 2 )
		// 90º pitch/yaw "FOV"

		if (pitch > 180) pitch -= 360;// TODO get rid of this

		// draw ball mesh
		FMATRIX4 mat;
		int H = 188;
		int W = 244;
	
		double croll = cos( roll * RAD );
		double sroll = sin( -roll * RAD );
		double cyaw = cos( yaw * RAD );
		double syaw = sin( yaw * RAD );
		double cpitch = cos( pitch * RAD );
		double spitch = sin( pitch * RAD );
		
		VECTOR3 xdir = _V( cyaw * croll, sroll, -croll * syaw );
		VECTOR3 zdir = _V( syaw * cpitch + sroll * cyaw * spitch, -croll * spitch, cpitch * cyaw - syaw * sroll * spitch );
		gcWorldMatrix( &mat, _V(float(W/2 - 1), float(H/2 - 1), float(W + 60)), xdir, zdir, 0.69296f );

		skp->SetWorldTransform( &mat );
		skp->SetPen( skpWhitePen );
		skp->DrawSketchMesh( hADIball, 0, 0 );
		skp->SetWorldTransform();
		
		// roll triangle
		skp->SetPen( skpLightGreenPen );
		skp->SetBrush( skpLightGreenBrush );
		oapi::IVECTOR2 tri[3];
		tri[0].x = (long)(122 + 49 * sroll);
		tri[0].y = (long)(94 - 49 * croll);
		tri[1].x = (long)(122 - 4 * croll + 41 * sroll);
		tri[1].y = (long)(94 - 4 * sroll - 41 * croll);
		tri[2].x = (long)(122 + 4 * croll + 41 * sroll);
		tri[2].y = (long)(94 + 4 * sroll - 41 * croll);
		skp->Polygon( tri, 3 );
	

		// flight director
		skp->SetPen( skpLightGreenThickPen );
		//Arc( hDC_ADI, 44, 44, 56, 57, 44, 50, 56, 50 );
		skp->SetPen( skpLightGreenPen );
		skp->SetBrush( skpLightGreenBrush );
		skp->Rectangle( 121, 77, 123, 111 );
		skp->Rectangle( 105, 93, 139, 95 );

		// digital RPY
		if (pitch < 0) pitch += 360;// TODO get rid of this
		skp->SetTextColor( CR_WHITE );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%03.0f", roll );
		skp->Text( 180, 25, cbuf, strlen( cbuf ) );
		sprintf_s( cbuf, 8, "%03.0f", pitch );
		skp->Text( 180, 32, cbuf, strlen( cbuf ) );
		sprintf_s( cbuf, 8, "%03.0f", yaw );
		skp->Text( 180, 39, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::ADI_RATE_A( HDC hDC, double pitch, double roll, double yaw, int adirate )
	{
		int pos_roll;
		int pos_pitch;
		int pos_yaw;

		SelectObject( hDC, gdiTahomaFont_h7w3 );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 121, 19, "0", 1 );
		TextOut( hDC, 192, 91, "0", 1 );
		TextOut( hDC, 121, 164, "0", 1 );
		SelectObject( hDC, gdiTahomaFont_h10w4 );
		SetTextColor( hDC, CR_WHITE );
		if (adirate == 1)
		{
			// ADI RATE MED
			TextOut( hDC, 81, 23, "5", 1 );
			TextOut( hDC, 159, 23, "5", 1 );

			TextOut( hDC, 187, 50, "5", 1 );
			TextOut( hDC, 187, 129, "5", 1 );

			TextOut( hDC, 81, 159, "5", 1 );
			TextOut( hDC, 159, 159, "5", 1 );

			if (roll > 5) pos_roll = 157;
			else if (roll < -5) pos_roll = 87;
			else pos_roll = 122 + Round( roll * 7 );

			if (pitch > 5) pos_pitch = 129;
			else if (pitch < -5) pos_pitch = 59;
			else pos_pitch = 94 + Round( pitch * 7 );

			if (yaw > 5) pos_yaw = 157;
			else if (yaw < -5) pos_yaw = 87;
			else pos_yaw = 122 + Round( yaw * 7 );
		}
		else if (adirate == 2)
		{
			// ADI RATE HIGH
			TextOut( hDC, 78, 23, "10", 2 );
			TextOut( hDC, 158, 23, "10", 2 );

			TextOut( hDC, 187, 50, "10", 2 );
			TextOut( hDC, 187, 129, "10", 2 );

			TextOut( hDC, 78, 159, "10", 2 );
			TextOut( hDC, 158, 159, "10", 2 );

			if (roll > 10) pos_roll = 157;
			else if (roll < -10) pos_roll = 87;
			else pos_roll = 122 + Round( roll * 3.5 );

			if (pitch > 10) pos_pitch = 129;
			else if (pitch < -10) pos_pitch = 59;
			else pos_pitch = 94 + Round( pitch * 3.5 );

			if (yaw > 10) pos_yaw = 157;
			else if (yaw < -10) pos_yaw = 87;
			else pos_yaw = 122 + Round( yaw * 3.5 );
		}
		else
		{
			// ADI RATE LOW
			TextOut( hDC, 81, 23, "1", 1 );
			TextOut( hDC, 159, 23, "1", 1 );

			TextOut( hDC, 187, 50, "1", 1 );
			TextOut( hDC, 187, 129, "1", 1 );

			TextOut( hDC, 81, 159, "1", 1 );
			TextOut( hDC, 159, 159, "1", 1 );

			if (roll > 1) pos_roll = 157;
			else if (roll < -1) pos_roll = 87;
			else pos_roll = 122 + Round( roll * 35 );

			if (pitch > 1) pos_pitch = 129;
			else if (pitch < -1) pos_pitch = 59;
			else pos_pitch = 94 + Round( pitch * 35 );

			if (yaw > 1) pos_yaw = 157;
			else if (yaw < -1) pos_yaw = 87;
			else pos_yaw = 122 + Round( yaw * 35 );
		}

		// draw triangles
		SelectObject( hDC, gdiLightGreenPen );
		SelectObject( hDC, gdiLightGreenBrush );
		POINT tri[3];
		tri[0].x = pos_roll;
		tri[0].y = 27;
		tri[1].x = tri[0].x + 5;
		tri[1].y = 19;
		tri[2].x = tri[0].x - 5;
		tri[2].y = 19;
		Polygon( hDC, tri, 3 );

		tri[0].x = 190;
		tri[0].y = pos_pitch;
		tri[1].x = 198;
		tri[1].y = tri[0].y - 5;
		tri[2].x = 198;
		tri[2].y = tri[0].y + 5;
		Polygon( hDC, tri, 3 );

		tri[0].x = pos_yaw;
		tri[0].y = 163;
		tri[1].x = tri[0].x - 5;
		tri[1].y = 171;
		tri[2].x = tri[0].x + 5;
		tri[2].y = 171;
		Polygon( hDC, tri, 3 );
		return;
	}

	void MDU::ADI_RATE_A( oapi::Sketchpad2* skp, double pitch, double roll, double yaw, int adirate )
	{
		int pos_roll;
		int pos_pitch;
		int pos_yaw;

		skp->SetFont( skpTahomaFont_h7w3 );
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 121, 19, "0", 1 );
		skp->Text( 192, 91, "0", 1 );
		skp->Text( 121, 164, "0", 1 );
		skp->SetFont( skpTahomaFont_h10w4 );
		skp->SetTextColor( CR_WHITE );
		if (adirate == 1)
		{
			// ADI RATE MED
			skp->Text( 81, 23, "5", 1 );
			skp->Text( 159, 23, "5", 1 );

			skp->Text( 187, 50, "5", 1 );
			skp->Text( 187, 129, "5", 1 );

			skp->Text( 81, 159, "5", 1 );
			skp->Text( 159, 159, "5", 1 );

			if (roll > 5) pos_roll = 157;
			else if (roll < -5) pos_roll = 87;
			else pos_roll = 122 + Round( roll * 7 );

			if (pitch > 5) pos_pitch = 129;
			else if (pitch < -5) pos_pitch = 59;
			else pos_pitch = 94 + Round( pitch * 7 );

			if (yaw > 5) pos_yaw = 157;
			else if (yaw < -5) pos_yaw = 87;
			else pos_yaw = 122 + Round( yaw * 7 );
		}
		else if (adirate == 2)
		{
			// ADI RATE HIGH
			skp->Text( 78, 23, "10", 2 );
			skp->Text( 158, 23, "10", 2 );

			skp->Text( 187, 50, "10", 2 );
			skp->Text( 187, 129, "10", 2 );

			skp->Text( 78, 159, "10", 2 );
			skp->Text( 158, 159, "10", 2 );

			if (roll > 10) pos_roll = 157;
			else if (roll < -10) pos_roll = 87;
			else pos_roll = 122 + Round( roll * 3.5 );

			if (pitch > 10) pos_pitch = 129;
			else if (pitch < -10) pos_pitch = 59;
			else pos_pitch = 94 + Round( pitch * 3.5 );

			if (yaw > 10) pos_yaw = 157;
			else if (yaw < -10) pos_yaw = 87;
			else pos_yaw = 122 + Round( yaw * 3.5 );
		}
		else
		{
			// ADI RATE LOW
			skp->Text( 81, 23, "1", 1 );
			skp->Text( 159, 23, "1", 1 );

			skp->Text( 187, 50, "1", 1 );
			skp->Text( 187, 129, "1", 1 );

			skp->Text( 81, 159, "1", 1 );
			skp->Text( 159, 159, "1", 1 );

			if (roll > 1) pos_roll = 157;
			else if (roll < -1) pos_roll = 87;
			else pos_roll = 122 + Round( roll * 35 );

			if (pitch > 1) pos_pitch = 129;
			else if (pitch < -1) pos_pitch = 59;
			else pos_pitch = 94 + Round( pitch * 35 );

			if (yaw > 1) pos_yaw = 157;
			else if (yaw < -1) pos_yaw = 87;
			else pos_yaw = 122 + Round( yaw * 35 );
		}

		// draw triangles
		skp->SetPen( skpLightGreenPen );
		skp->SetBrush( skpLightGreenBrush );
		oapi::IVECTOR2 tri[3];
		tri[0].x = pos_roll;
		tri[0].y = 27;
		tri[1].x = tri[0].x + 5;
		tri[1].y = 19;
		tri[2].x = tri[0].x - 5;
		tri[2].y = 19;
		skp->Polygon( tri, 3 );

		tri[0].x = 190;
		tri[0].y = pos_pitch;
		tri[1].x = 198;
		tri[1].y = tri[0].y - 5;
		tri[2].x = 198;
		tri[2].y = tri[0].y + 5;
		skp->Polygon( tri, 3 );

		tri[0].x = pos_yaw;
		tri[0].y = 163;
		tri[1].x = tri[0].x - 5;
		tri[1].y = 171;
		tri[2].x = tri[0].x + 5;
		tri[2].y = 171;
		skp->Polygon( tri, 3 );
		return;
	}

	void MDU::ADI_RATE_B( HDC hDC, double pitch, double roll, double yaw, int adirate, double Altitude_ft )
	{
		int pos_roll;
		int pos_pitch;
		int pos_yaw;

		SetTextColor( hDC, CR_WHITE );
		if (adirate == 1)
		{
			// ADI RATE MED
			if (Altitude_ft < 6400)// HACK should use a 3sec timer starting at 7Kft, which expires about 6.4Kft 
			{
				// <7k + 3s
				TextOut( hDC, 81, 23, "5", 1 );
				TextOut( hDC, 159, 23, "5", 1 );

				TextOut( hDC, 187, 50, "5", 1 );
				TextOut( hDC, 187, 129, "5", 1 );

				TextOut( hDC, 81, 159, "5", 1 );
				TextOut( hDC, 159, 159, "5", 1 );

				SelectObject( hDC, gdiTahomaFont_h7w3 );
				SetTextColor( hDC, CR_LIGHT_GRAY );
				TextOut( hDC, 121, 19, "0", 1 );
				TextOut( hDC, 192, 91, "0", 1 );
				TextOut( hDC, 121, 164, "0", 1 );
				SelectObject( hDC, gdiTahomaFont_h10w4 );

				if (roll > 5) pos_roll = 157;
				else if (roll < -5) pos_roll = 87;
				else pos_roll = 122 + Round( roll * 7 );

				if (pitch > 5) pos_pitch = 129;
				else if (pitch < -5) pos_pitch = 59;
				else pos_pitch = 94 + Round( pitch * 7 );

				if (yaw > 5) pos_yaw = 157;
				else if (yaw < -5) pos_yaw = 87;
				else pos_yaw = 122 + Round( yaw * 7 );
			}
			else if (Altitude_ft < 7000)
			{
				// <7k
				// blank
				// blank
				// blank
				pos_roll = 0;
				pos_pitch = 0;
				pos_yaw = 0;
			}
			else if (GetIDP()->GetPrefinalState() == true)
			{
				// PRFNL - 7k
				// blank
				// alt err +/-1k
				// y-runway position err +/-1k
				TextOut( hDC, 187, 50, "1K", 2 );
				TextOut( hDC, 187, 129, "1K", 2 );

				TextOut( hDC, 78, 159, "1K", 2 );
				TextOut( hDC, 158, 159, "1K", 2 );

				SelectObject( hDC, gdiTahomaFont_h7w3 );
				SetTextColor( hDC, CR_LIGHT_GRAY );
				TextOut( hDC, 192, 91, "0", 1 );
				TextOut( hDC, 121, 164, "0", 1 );
				SelectObject( hDC, gdiTahomaFont_h10w4 );

				pos_roll = 0;
				pitch = GetIDP()->GetGlideSlopeDistance();
				yaw = -GetIDP()->GetYRunwayPositionError();

				if (pitch > 1000) pos_pitch = 129;
				else if (pitch < -1000) pos_pitch = 59;
				else pos_pitch = 94 + Round( pitch * 0.035 );

				if (yaw > 1000) pos_yaw = 157;
				else if (yaw < -1000) pos_yaw = 87;
				else pos_yaw = 122 + Round( yaw * 0.035 );
			}
			else if (GetIDP()->GetOnHACState() == true)
			{
				// HAC
				// blank
				// alt err +/-5k
				// HAC x-range err +/-5k
				TextOut( hDC, 187, 50, "5K", 2 );
				TextOut( hDC, 187, 129, "5K", 2 );

				TextOut( hDC, 78, 159, "5K", 2 );
				TextOut( hDC, 158, 159, "5K", 2 );

				SelectObject( hDC, gdiTahomaFont_h7w3 );
				SetTextColor( hDC, CR_LIGHT_GRAY );
				TextOut( hDC, 192, 91, "0", 1 );
				TextOut( hDC, 121, 164, "0", 1 );
				SelectObject( hDC, gdiTahomaFont_h10w4 );

				pos_roll = 0;
				pitch = 0;// TODO get alt error (ft)
				yaw = GetIDP()->GetHACRadialError();

				if (pitch > 5000) pos_pitch = 129;
				else if (pitch < -5000) pos_pitch = 59;
				else pos_pitch = 94 + Round( pitch * 0.007 );

				if (yaw > 5000) pos_yaw = 157;
				else if (yaw < -5000) pos_yaw = 87;
				else pos_yaw = 122 + Round( yaw * 0.007 );
			}
			else
			{
				// ACQ
				// time to HAC 10s
				// alt err +/-5k
				// heading err +/-5º
				// (roll scale output is below)

				TextOut( hDC, 187, 50, "5K", 2 );
				TextOut( hDC, 187, 129, "5K", 2 );

				TextOut( hDC, 81, 159, "5", 1 );
				TextOut( hDC, 159, 159, "5", 1 );

				SelectObject( hDC, gdiTahomaFont_h7w3 );
				SetTextColor( hDC, CR_LIGHT_GRAY );
				TextOut( hDC, 192, 91, "0", 1 );
				TextOut( hDC, 121, 164, "0", 1 );
				SelectObject( hDC, gdiTahomaFont_h10w4 );

				roll = ceil( GetIDP()->GetTimeToHAC() );
				pitch = 0;// TODO get alt error (ft)
				yaw = 0;// TODO get heading error (deg)

				if (roll > 0)
				{
					TextOut( hDC, 74, 23, "10s", 3 );
					TextOut( hDC, 159, 23, "0", 1 );
					if (roll >= 10) pos_roll = 87;
					else pos_roll = 157 - Round( roll * 7 );
				}
				else if (roll < 0)
				{
					TextOut( hDC, 81, 23, "0", 1 );
					TextOut( hDC, 159, 23, "10s", 3 );
					if (roll <= -10) pos_roll = 157;
					else pos_roll = 87 - Round( roll * 7 );
				}
				else pos_roll = 0;

				if (pitch > 5000) pos_pitch = 129;
				else if (pitch < -5000) pos_pitch = 59;
				else pos_pitch = 94 + Round( pitch * 0.007 );

				if (yaw > 5) pos_yaw = 157;
				else if (yaw < -5) pos_yaw = 87;
				else pos_yaw = 122 + Round( yaw * 7 );
			}
		}
		else
		{
			// ADI RATE HIGH/LOW
			TextOut( hDC, 81, 23, "5", 1 );
			TextOut( hDC, 159, 23, "5", 1 );

			TextOut( hDC, 187, 50, "5", 1 );
			TextOut( hDC, 187, 129, "5", 1 );

			TextOut( hDC, 81, 159, "5", 1 );
			TextOut( hDC, 159, 159, "5", 1 );

			SelectObject( hDC, gdiTahomaFont_h7w3 );
			SetTextColor( hDC, CR_LIGHT_GRAY );
			TextOut( hDC, 121, 19, "0", 1 );
			TextOut( hDC, 192, 91, "0", 1 );
			TextOut( hDC, 121, 164, "0", 1 );
			SelectObject( hDC, gdiTahomaFont_h10w4 );

			if (roll > 5) pos_roll = 157;
			else if (roll < -5) pos_roll = 87;
			else pos_roll = 122 + Round( roll * 7 );

			if (pitch > 5) pos_pitch = 129;
			else if (pitch < -5) pos_pitch = 59;
			else pos_pitch = 94 + Round( pitch * 7 );

			if (yaw > 5) pos_yaw = 157;
			else if (yaw < -5) pos_yaw = 87;
			else pos_yaw = 122 + Round( yaw * 7 );
		}
		
		// draw triangles
		SelectObject( hDC, gdiLightGreenPen );
		SelectObject( hDC, gdiLightGreenBrush );
		POINT tri[3];
		if (pos_roll != 0)
		{
			tri[0].x = pos_roll;
			tri[0].y = 27;
			tri[1].x = tri[0].x + 5;
			tri[1].y = 19;
			tri[2].x = tri[0].x - 5;
			tri[2].y = 19;
			Polygon( hDC, tri, 3 );
		}
		if (pos_pitch != 0)
		{
			tri[0].x = 190;
			tri[0].y = pos_pitch;
			tri[1].x = 198;
			tri[1].y = tri[0].y - 5;
			tri[2].x = 198;
			tri[2].y = tri[0].y + 5;
			Polygon( hDC, tri, 3 );
		}
		if (pos_yaw != 0)
		{
			tri[0].x = pos_yaw;
			tri[0].y = 163;
			tri[1].x = tri[0].x - 5;
			tri[1].y = 171;
			tri[2].x = tri[0].x + 5;
			tri[2].y = 171;
			Polygon( hDC, tri, 3 );
		}
		return;
	}

	void MDU::ADI_RATE_B( oapi::Sketchpad2* skp, double pitch, double roll, double yaw, int adirate, double Altitude_ft )
	{
		int pos_roll;
		int pos_pitch;
		int pos_yaw;

		skp->SetTextColor( CR_WHITE );
		if (adirate == 1)
		{
			// ADI RATE MED
			if (Altitude_ft < 6400)// HACK should use a 3sec timer starting at 7Kft, which expires about 6.4Kft 
			{
				// <7k + 3s
				skp->Text( 81, 23, "5", 1 );
				skp->Text( 159, 23, "5", 1 );

				skp->Text( 187, 50, "5", 1 );
				skp->Text( 187, 129, "5", 1 );

				skp->Text( 81, 159, "5", 1 );
				skp->Text( 159, 159, "5", 1 );

				skp->SetFont( skpTahomaFont_h7w3 );
				skp->SetTextColor( CR_LIGHT_GRAY );
				skp->Text( 121, 19, "0", 1 );
				skp->Text( 192, 91, "0", 1 );
				skp->Text( 121, 164, "0", 1 );
				skp->SetFont( skpTahomaFont_h10w4 );

				if (roll > 5) pos_roll = 157;
				else if (roll < -5) pos_roll = 87;
				else pos_roll = 122 + Round( roll * 7 );

				if (pitch > 5) pos_pitch = 129;
				else if (pitch < -5) pos_pitch = 59;
				else pos_pitch = 94 + Round( pitch * 7 );

				if (yaw > 5) pos_yaw = 157;
				else if (yaw < -5) pos_yaw = 87;
				else pos_yaw = 122 + Round( yaw * 7 );
			}
			else if (Altitude_ft < 7000)
			{
				// <7k
				// blank
				// blank
				// blank
				pos_roll = 0;
				pos_pitch = 0;
				pos_yaw = 0;
			}
			else if (GetIDP()->GetPrefinalState() == true)
			{
				// PRFNL - 7k
				// blank
				// alt err +/-1k
				// y-runway position err +/-1k
				skp->Text( 187, 50, "1K", 2 );
				skp->Text( 187, 129, "1K", 2 );

				skp->Text( 78, 159, "1K", 2 );
				skp->Text( 158, 159, "1K", 2 );

				skp->SetFont( skpTahomaFont_h7w3 );
				skp->SetTextColor( CR_LIGHT_GRAY );
				skp->Text( 192, 91, "0", 1 );
				skp->Text( 121, 164, "0", 1 );
				skp->SetFont( skpTahomaFont_h10w4 );

				pos_roll = 0;
				pitch = GetIDP()->GetGlideSlopeDistance();
				yaw = -GetIDP()->GetYRunwayPositionError();

				if (pitch > 1000) pos_pitch = 129;
				else if (pitch < -1000) pos_pitch = 59;
				else pos_pitch = 94 + Round( pitch * 0.035 );

				if (yaw > 1000) pos_yaw = 157;
				else if (yaw < -1000) pos_yaw = 87;
				else pos_yaw = 122 + Round( yaw * 0.035 );
			}
			else if (GetIDP()->GetOnHACState() == true)
			{
				// HAC
				// blank
				// alt err +/-5k
				// HAC x-range err +/-5k
				skp->Text( 187, 50, "5K", 2 );
				skp->Text( 187, 129, "5K", 2 );

				skp->Text( 78, 159, "5K", 2 );
				skp->Text( 158, 159, "5K", 2 );

				skp->SetFont( skpTahomaFont_h7w3 );
				skp->SetTextColor( CR_LIGHT_GRAY );
				skp->Text( 192, 91, "0", 1 );
				skp->Text( 121, 164, "0", 1 );
				skp->SetFont( skpTahomaFont_h10w4 );

				pos_roll = 0;
				pitch = 0;// TODO get alt error (ft)
				yaw = GetIDP()->GetHACRadialError();

				if (pitch > 5000) pos_pitch = 129;
				else if (pitch < -5000) pos_pitch = 59;
				else pos_pitch = 94 + Round( pitch * 0.007 );

				if (yaw > 5000) pos_yaw = 157;
				else if (yaw < -5000) pos_yaw = 87;
				else pos_yaw = 122 + Round( yaw * 0.007 );
			}
			else
			{
				// ACQ
				// time to HAC 10s
				// alt err +/-5k
				// heading err +/-5º
				// (roll scale output is below)

				skp->Text( 187, 50, "5K", 2 );
				skp->Text( 187, 129, "5K", 2 );

				skp->Text( 81, 159, "5", 1 );
				skp->Text( 159, 159, "5", 1 );

				skp->SetFont( skpTahomaFont_h7w3 );
				skp->SetTextColor( CR_LIGHT_GRAY );
				skp->Text( 192, 91, "0", 1 );
				skp->Text( 121, 164, "0", 1 );
				skp->SetFont( skpTahomaFont_h10w4 );

				roll = ceil( GetIDP()->GetTimeToHAC() );
				pitch = 0;// TODO get alt error (ft)
				yaw = 0;// TODO get heading error (deg)

				if (roll > 0)
				{
					skp->Text( 74, 23, "10s", 3 );
					skp->Text( 159, 23, "0", 1 );
					if (roll >= 10) pos_roll = 87;
					else pos_roll = 157 - Round( roll * 7 );
				}
				else if (roll < 0)
				{
					skp->Text( 81, 23, "0", 1 );
					skp->Text( 159, 23, "10s", 3 );
					if (roll <= -10) pos_roll = 157;
					else pos_roll = 87 - Round( roll * 7 );
				}
				else pos_roll = 0;

				if (pitch > 5000) pos_pitch = 129;
				else if (pitch < -5000) pos_pitch = 59;
				else pos_pitch = 94 + Round( pitch * 0.007 );

				if (yaw > 5) pos_yaw = 157;
				else if (yaw < -5) pos_yaw = 87;
				else pos_yaw = 122 + Round( yaw * 7 );
			}
		}
		else
		{
			// ADI RATE HIGH/LOW
			skp->Text( 81, 23, "5", 1 );
			skp->Text( 159, 23, "5", 1 );

			skp->Text( 187, 50, "5", 1 );
			skp->Text( 187, 129, "5", 1 );

			skp->Text( 81, 159, "5", 1 );
			skp->Text( 159, 159, "5", 1 );

			skp->SetFont( skpTahomaFont_h7w3 );
			skp->SetTextColor( CR_LIGHT_GRAY );
			skp->Text( 121, 19, "0", 1 );
			skp->Text( 192, 91, "0", 1 );
			skp->Text( 121, 164, "0", 1 );
			skp->SetFont( skpTahomaFont_h10w4 );

			if (roll > 5) pos_roll = 157;
			else if (roll < -5) pos_roll = 87;
			else pos_roll = 122 + Round( roll * 7 );

			if (pitch > 5) pos_pitch = 129;
			else if (pitch < -5) pos_pitch = 59;
			else pos_pitch = 94 + Round( pitch * 7 );

			if (yaw > 5) pos_yaw = 157;
			else if (yaw < -5) pos_yaw = 87;
			else pos_yaw = 122 + Round( yaw * 7 );
		}
		
		// draw triangles
		skp->SetPen( skpLightGreenPen );
		skp->SetBrush( skpLightGreenBrush );
		oapi::IVECTOR2 tri[3];
		if (pos_roll != 0)
		{
			tri[0].x = pos_roll;
			tri[0].y = 27;
			tri[1].x = tri[0].x + 5;
			tri[1].y = 19;
			tri[2].x = tri[0].x - 5;
			tri[2].y = 19;
			skp->Polygon( tri, 3 );
		}
		if (pos_pitch != 0)
		{
			tri[0].x = 190;
			tri[0].y = pos_pitch;
			tri[1].x = 198;
			tri[1].y = tri[0].y - 5;
			tri[2].x = 198;
			tri[2].y = tri[0].y + 5;
			skp->Polygon( tri, 3 );
		}
		if (pos_yaw != 0)
		{
			tri[0].x = pos_yaw;
			tri[0].y = 163;
			tri[1].x = tri[0].x - 5;
			tri[1].y = 171;
			tri[2].x = tri[0].x + 5;
			tri[2].y = 171;
			skp->Polygon( tri, 3 );
		}
		return;
	}

	void MDU::ADI_ERROR_A( HDC hDC, double pitch, double roll, double yaw, int adierr )
	{
		double pos_pitch;
		double pos_roll;
		double pos_yaw;

		SetTextColor( hDC, CR_MAGENTA );
		SelectObject( hDC, gdiTahomaFont_h7w3 );
		if (adierr == 1)
		{
			// ADI ERROR MED
			// 5/5/5
			TextOut( hDC, 173, 64, "5", 1 );
			TextOut( hDC, 173, 118, "5", 1 );

			if (roll > 5) pos_roll = 25;
			else if (roll < -5) pos_roll = -25;
			else pos_roll = roll * 5;

			if (pitch > 5) pos_pitch = 25;
			else if (pitch < -5) pos_pitch = -25;
			else pos_pitch = pitch * 5;

			if (yaw > 5) pos_yaw = 25;
			else if (yaw < -5) pos_yaw = -25;
			else pos_yaw = yaw * 5;
		}
		else if (adierr == 2)
		{
			// ADI ERROR HIGH
			// 10/10/10
			TextOut( hDC, 173, 64, "10", 2 );
			TextOut( hDC, 173, 118, "10", 2 );

			if (roll > 10) pos_roll = 25;
			else if (roll < -10) pos_roll = -25;
			else pos_roll = roll * 2.5;

			if (pitch > 10) pos_pitch = 25;
			else if (pitch < -10) pos_pitch = -25;
			else pos_pitch = pitch * 2.5;

			if (yaw > 10) pos_yaw = 25;
			else if (yaw < -10) pos_yaw = -25;
			else pos_yaw = yaw * 2.5;
		}
		else
		{
			// ADI ERROR LOW
			// 1/1/1
			TextOut( hDC, 173, 64, "1", 1 );
			TextOut( hDC, 173, 118, "1", 1 );

			if (roll > 1) pos_roll = 25;
			else if (roll < -1) pos_roll = -25;
			else pos_roll = roll * 25;

			if (pitch > 1) pos_pitch = 25;
			else if (pitch < -1) pos_pitch = -25;
			else pos_pitch = pitch * 25;

			if (yaw > 1) pos_yaw = 25;
			else if (yaw < -1) pos_yaw = -25;
			else pos_yaw = yaw * 25;
		}
		SelectObject( hDC, gdiTahomaFont_h10w4 );

		// draw needles
		SelectObject( hDC, gdiMagentaPen );
		SelectObject( hDC, gdiMagentaBrush );
		Rectangle( hDC, 121 + Round( pos_roll ), 94 - Round( sqrt( 3156 - (pos_roll * pos_roll) ) ), 123 + Round( pos_roll ), 77 );// roll
		Rectangle( hDC, 139, 93 + Round( pos_pitch ), 122 + Round( sqrt( 3156 - (pos_pitch * pos_pitch) ) ), 95 + Round( pos_pitch ) );// pitch
		Rectangle( hDC, 121 + Round( pos_yaw ), 111, 123 + Round( pos_yaw ), 94 + Round( sqrt( 3156 - (pos_yaw * pos_yaw) ) ) );// yaw
		return;
	}

	void MDU::ADI_ERROR_A( oapi::Sketchpad2* skp, double pitch, double roll, double yaw, int adierr )
	{
		double pos_pitch;
		double pos_roll;
		double pos_yaw;

		skp->SetTextColor( CR_MAGENTA );
		skp->SetFont( skpTahomaFont_h7w3 );
		if (adierr == 1)
		{
			// ADI ERROR MED
			// 5/5/5
			skp->Text( 173, 64, "5", 1 );
			skp->Text( 173, 118, "5", 1 );

			if (roll > 5) pos_roll = 25;
			else if (roll < -5) pos_roll = -25;
			else pos_roll = roll * 5;

			if (pitch > 5) pos_pitch = 25;
			else if (pitch < -5) pos_pitch = -25;
			else pos_pitch = pitch * 5;

			if (yaw > 5) pos_yaw = 25;
			else if (yaw < -5) pos_yaw = -25;
			else pos_yaw = yaw * 5;
		}
		else if (adierr == 2)
		{
			// ADI ERROR HIGH
			// 10/10/10
			skp->Text( 173, 64, "10", 2 );
			skp->Text( 173, 118, "10", 2 );

			if (roll > 10) pos_roll = 25;
			else if (roll < -10) pos_roll = -25;
			else pos_roll = roll * 2.5;

			if (pitch > 10) pos_pitch = 25;
			else if (pitch < -10) pos_pitch = -25;
			else pos_pitch = pitch * 2.5;

			if (yaw > 10) pos_yaw = 25;
			else if (yaw < -10) pos_yaw = -25;
			else pos_yaw = yaw * 2.5;
		}
		else
		{
			// ADI ERROR LOW
			// 1/1/1
			skp->Text( 173, 64, "1", 1 );
			skp->Text( 173, 118, "1", 1 );

			if (roll > 1) pos_roll = 25;
			else if (roll < -1) pos_roll = -25;
			else pos_roll = roll * 25;

			if (pitch > 1) pos_pitch = 25;
			else if (pitch < -1) pos_pitch = -25;
			else pos_pitch = pitch * 25;

			if (yaw > 1) pos_yaw = 25;
			else if (yaw < -1) pos_yaw = -25;
			else pos_yaw = yaw * 25;
		}
		skp->SetFont( skpTahomaFont_h10w4 );

		// draw needles
		skp->SetPen( skpMagentaPen );
		skp->SetBrush( skpMagentaBrush );
		skp->Rectangle( 121 + Round( pos_roll ), 94 - Round( sqrt( 3156 - (pos_roll * pos_roll) ) ), 123 + Round( pos_roll ), 77 );// roll
		skp->Rectangle( 139, 93 + Round( pos_pitch ), 122 + Round( sqrt( 3156 - (pos_pitch * pos_pitch) ) ), 95 + Round( pos_pitch ) );// pitch
		skp->Rectangle( 121 + Round( pos_yaw ), 111, 123 + Round( pos_yaw ), 94 + Round( sqrt( 3156 - (pos_yaw * pos_yaw) ) ) );// yaw
		return;
	}

	void MDU::ADI_ERROR_B( HDC hDC, double pitch, double roll, double yaw, int adierr )
	{
		double pos_pitch;
		double pos_roll;
		double pos_yaw;

		SetTextColor( hDC, CR_MAGENTA );
		SelectObject( hDC, gdiTahomaFont_h7w3 );
		if (adierr == 1)
		{
			// ADI ERROR MED
			// 25/2/2.5
			TextOut( hDC, 173, 64, "2", 1 );
			TextOut( hDC, 173, 118, "2", 1 );

			if (roll > 25) pos_roll = 25;
			else if (roll < -25) pos_roll = -25;
			else pos_roll = roll;

			if (pitch > 2) pos_pitch = 25;
			else if (pitch < -2) pos_pitch = -25;
			else pos_pitch = pitch * 12.5;
		}
		else if (adierr == 2)
		{
			// ADI ERROR HIGH
			// 25/5/2.5
			TextOut( hDC, 173, 64, "5", 1 );
			TextOut( hDC, 173, 118, "5", 1 );

			if (roll > 25) pos_roll = 25;
			else if (roll < -25) pos_roll = -25;
			else pos_roll = roll;

			if (pitch > 5) pos_pitch = 25;
			else if (pitch < -5) pos_pitch = -25;
			else pos_pitch = pitch * 5;
		}
		else
		{
			// ADI ERROR LOW
			// 10/1/2.5
			TextOut( hDC, 173, 64, "1", 1 );
			TextOut( hDC, 173, 118, "1", 1 );

			if (roll > 10) pos_roll = 25;
			else if (roll < -10) pos_roll = -25;
			else pos_roll = roll * 2.5;

			if (pitch > 1) pos_pitch = 25;
			else if (pitch < -1) pos_pitch = -25;
			else pos_pitch = pitch * 25;
		}
		if (yaw > 2.5) pos_yaw = 25;
		else if (yaw < -2.5) pos_yaw = -25;
		else pos_yaw = yaw * 10;
		SelectObject( hDC, gdiTahomaFont_h10w4 );

		// draw needles
		SelectObject( hDC, gdiMagentaPen );
		SelectObject( hDC, gdiMagentaBrush );
		Rectangle( hDC, 121 + Round( pos_roll ), 94 - Round( sqrt( 3156 - (pos_roll * pos_roll) ) ), 123 + Round( pos_roll ), 77 );// roll
		Rectangle( hDC, 139, 93 + Round( pos_pitch ), 122 + Round( sqrt( 3156 - (pos_pitch * pos_pitch) ) ), 95 + Round( pos_pitch ) );// pitch
		Rectangle( hDC, 121 + Round( pos_yaw ), 111, 123 + Round( pos_yaw ), 94 + Round( sqrt( 3156 - (pos_yaw * pos_yaw) ) ) );// yaw
		return;// 25/25/10 5/2/1 2.5/2.5/2.5
	}

	void MDU::ADI_ERROR_B( oapi::Sketchpad2* skp, double pitch, double roll, double yaw, int adierr )
	{
		double pos_pitch;
		double pos_roll;
		double pos_yaw;

		skp->SetTextColor( CR_MAGENTA );
		skp->SetFont( skpTahomaFont_h7w3 );
		if (adierr == 1)
		{
			// ADI ERROR MED
			// 25/2/2.5
			skp->Text( 173, 64, "2", 1 );
			skp->Text( 173, 118, "2", 1 );

			if (roll > 25) pos_roll = 25;
			else if (roll < -25) pos_roll = -25;
			else pos_roll = roll;

			if (pitch > 2) pos_pitch = 25;
			else if (pitch < -2) pos_pitch = -25;
			else pos_pitch = pitch * 12.5;
		}
		else if (adierr == 2)
		{
			// ADI ERROR HIGH
			// 25/5/2.5
			skp->Text( 173, 64, "5", 1 );
			skp->Text( 173, 118, "5", 1 );

			if (roll > 25) pos_roll = 25;
			else if (roll < -25) pos_roll = -25;
			else pos_roll = roll;

			if (pitch > 5) pos_pitch = 25;
			else if (pitch < -5) pos_pitch = -25;
			else pos_pitch = pitch * 5;
		}
		else
		{
			// ADI ERROR LOW
			// 10/1/2.5
			skp->Text( 173, 64, "1", 1 );
			skp->Text( 173, 118, "1", 1 );

			if (roll > 10) pos_roll = 25;
			else if (roll < -10) pos_roll = -25;
			else pos_roll = roll * 2.5;

			if (pitch > 1) pos_pitch = 25;
			else if (pitch < -1) pos_pitch = -25;
			else pos_pitch = pitch * 25;
		}
		if (yaw > 2.5) pos_yaw = 25;
		else if (yaw < -2.5) pos_yaw = -25;
		else pos_yaw = yaw * 10;
		skp->SetFont( skpTahomaFont_h10w4 );

		// draw needles
		skp->SetPen( skpMagentaPen );
		skp->SetBrush( skpMagentaBrush );
		skp->Rectangle( 121 + Round( pos_roll ), 94 - Round( sqrt( 3156 - (pos_roll * pos_roll) ) ), 123 + Round( pos_roll ), 77 );// roll
		skp->Rectangle( 139, 93 + Round( pos_pitch ), 122 + Round( sqrt( 3156 - (pos_pitch * pos_pitch) ) ), 95 + Round( pos_pitch ) );// pitch
		skp->Rectangle( 121 + Round( pos_yaw ), 111, 123 + Round( pos_yaw ), 94 + Round( sqrt( 3156 - (pos_yaw * pos_yaw) ) ) );// yaw
		return;// 25/25/10 5/2/1 2.5/2.5/2.5
	}

	void MDU::ADI_ERROR_C( HDC hDC, double pitch, double roll, double yaw, int adierr )
	{
		double pos_pitch;
		double pos_roll;
		double pos_yaw;

		SetTextColor( hDC, CR_MAGENTA );
		SelectObject( hDC, gdiTahomaFont_h7w3 );
		if (adierr == 0)
		{
			// ADI ERROR LOW
			// 10/0.5g/2.5
			TextOut( hDC, 173, 64, "0.5g", 4 );
			TextOut( hDC, 173, 118, "0.5g", 4 );

			if (roll > 10) pos_roll = 25;
			else if (roll < -10) pos_roll = -25;
			else pos_roll = roll * 2.5;

			if (pitch > 0.5) pos_pitch = 25;
			else if (pitch < -0.5) pos_pitch = -25;
			else pos_pitch = pitch * 50;
		}
		else
		{
			// ADI ERROR MED/HIGH
			// 25/1.25g/2.5
			TextOut( hDC, 173, 64, "1.2g", 4 );
			TextOut( hDC, 173, 118, "1.2g", 4 );

			if (roll > 25) pos_roll = 25;
			else if (roll < -25) pos_roll = -25;
			else pos_roll = roll;

			if (pitch > 1.25) pos_pitch = 25;
			else if (pitch < -1.25) pos_pitch = -25;
			else pos_pitch = pitch * 20;
		}
		if (yaw > 2.5) pos_yaw = 25;
		else if (yaw < -2.5) pos_yaw = -25;
		else pos_yaw = yaw * 10;
		SelectObject( hDC, gdiTahomaFont_h10w4 );

		// draw needles
		SelectObject( hDC, gdiMagentaPen );
		SelectObject( hDC, gdiMagentaBrush );
		Rectangle( hDC, 121 + Round( pos_roll ), 94 - Round( sqrt( 3156 - (pos_roll * pos_roll) ) ), 123 + Round( pos_roll ), 77 );// roll
		Rectangle( hDC, 139, 93 + Round( pos_pitch ), 122 + Round( sqrt( 3156 - (pos_pitch * pos_pitch) ) ), 95 + Round( pos_pitch ) );// pitch
		Rectangle( hDC, 121 + Round( pos_yaw ), 111, 123 + Round( pos_yaw ), 94 + Round( sqrt( 3156 - (pos_yaw * pos_yaw) ) ) );// yaw
		return;// 25/25/10 1.25/1.25/0.5 2.5/2.5/2.5
	}

	void MDU::ADI_ERROR_C( oapi::Sketchpad2* skp, double pitch, double roll, double yaw, int adierr )
	{
		double pos_pitch;
		double pos_roll;
		double pos_yaw;

		skp->SetTextColor( CR_MAGENTA );
		skp->SetFont( skpTahomaFont_h7w3 );
		if (adierr == 0)
		{
			// ADI ERROR LOW
			// 10/0.5g/2.5
			skp->Text( 173, 64, "0.5g", 4 );
			skp->Text( 173, 118, "0.5g", 4 );

			if (roll > 10) pos_roll = 25;
			else if (roll < -10) pos_roll = -25;
			else pos_roll = roll * 2.5;

			if (pitch > 0.5) pos_pitch = 25;
			else if (pitch < -0.5) pos_pitch = -25;
			else pos_pitch = pitch * 50;
		}
		else
		{
			// ADI ERROR MED/HIGH
			// 25/1.25g/2.5
			skp->Text( 173, 64, "1.2g", 4 );
			skp->Text( 173, 118, "1.2g", 4 );

			if (roll > 25) pos_roll = 25;
			else if (roll < -25) pos_roll = -25;
			else pos_roll = roll;

			if (pitch > 1.25) pos_pitch = 25;
			else if (pitch < -1.25) pos_pitch = -25;
			else pos_pitch = pitch * 20;
		}
		if (yaw > 2.5) pos_yaw = 25;
		else if (yaw < -2.5) pos_yaw = -25;
		else pos_yaw = yaw * 10;
		skp->SetFont( skpTahomaFont_h10w4 );

		// draw needles
		skp->SetPen( skpMagentaPen );
		skp->SetBrush( skpMagentaBrush );
		skp->Rectangle( 121 + Round( pos_roll ), 94 - Round( sqrt( 3156 - (pos_roll * pos_roll) ) ), 123 + Round( pos_roll ), 77 );// roll
		skp->Rectangle( 139, 93 + Round( pos_pitch ), 122 + Round( sqrt( 3156 - (pos_pitch * pos_pitch) ) ), 95 + Round( pos_pitch ) );// pitch
		skp->Rectangle( 121 + Round( pos_yaw ), 111, 123 + Round( pos_yaw ), 94 + Round( sqrt( 3156 - (pos_yaw * pos_yaw) ) ) );// yaw
		return;// 25/25/10 1.25/1.25/0.5 2.5/2.5/2.5
	}

	void MDU::ADI_ERROR_D( HDC hDC, double pitch, double roll, double yaw, int adierr )
	{
		double pos_pitch;
		double pos_roll;
		double pos_yaw;

		SetTextColor( hDC, CR_MAGENTA );
		SelectObject( hDC, gdiTahomaFont_h7w3 );
		if (adierr == 1)
		{
			// ADI ERROR MED
			// 5/5/2.5
			TextOut( hDC, 173, 64, "5", 1 );
			TextOut( hDC, 173, 118, "5", 1 );

			if (roll > 5) pos_roll = 25;
			else if (roll < -5) pos_roll = -25;
			else pos_roll = roll * 5;

			if (pitch > 5) pos_pitch = 25;
			else if (pitch < -5) pos_pitch = -25;
			else pos_pitch = pitch * 5;
		}
		else if (adierr == 2)
		{
			// ADI ERROR HIGH
			// 20/10/2.5
			TextOut( hDC, 173, 64, "10", 2 );
			TextOut( hDC, 173, 118, "10", 2 );

			if (roll > 20) pos_roll = 25;
			else if (roll < -20) pos_roll = -25;
			else pos_roll = roll * 1.25;

			if (pitch > 10) pos_pitch = 25;
			else if (pitch < -10) pos_pitch = -25;
			else pos_pitch = pitch * 2.5;
		}
		else
		{
			// ADI ERROR LOW
			// 1/1/2.5
			TextOut( hDC, 173, 64, "1", 1 );
			TextOut( hDC, 173, 118, "1", 1 );

			if (roll > 1) pos_roll = 25;
			else if (roll < -1) pos_roll = -25;
			else pos_roll = roll * 25;

			if (pitch > 1) pos_pitch = 25;
			else if (pitch < -1) pos_pitch = -25;
			else pos_pitch = pitch * 25;
		}
		if (yaw > 2.5) pos_yaw = 25;
		else if (yaw < -2.5) pos_yaw = -25;
		else pos_yaw = yaw * 10;
		SelectObject( hDC, gdiTahomaFont_h10w4 );

		// draw needles
		SelectObject( hDC, gdiMagentaPen );
		SelectObject( hDC, gdiMagentaBrush );
		Rectangle( hDC, 121 + Round( pos_roll ), 94 - Round( sqrt( 3156 - (pos_roll * pos_roll) ) ), 123 + Round( pos_roll ), 77 );// roll
		Rectangle( hDC, 139, 93 + Round( pos_pitch ), 122 + Round( sqrt( 3156 - (pos_pitch * pos_pitch) ) ), 95 + Round( pos_pitch ) );// pitch
		Rectangle( hDC, 121 + Round( pos_yaw ), 111, 123 + Round( pos_yaw ), 94 + Round( sqrt( 3156 - (pos_yaw * pos_yaw) ) ) );// yaw
		return;// 20/5/1 10/5/1 2.5/2.5/2.5
	}

	void MDU::ADI_ERROR_D( oapi::Sketchpad2* skp, double pitch, double roll, double yaw, int adierr )
	{
		double pos_pitch;
		double pos_roll;
		double pos_yaw;

		skp->SetTextColor( CR_MAGENTA );
		skp->SetFont( skpTahomaFont_h7w3 );
		if (adierr == 1)
		{
			// ADI ERROR MED
			// 5/5/2.5
			skp->Text( 173, 64, "5", 1 );
			skp->Text( 173, 118, "5", 1 );

			if (roll > 5) pos_roll = 25;
			else if (roll < -5) pos_roll = -25;
			else pos_roll = roll * 5;

			if (pitch > 5) pos_pitch = 25;
			else if (pitch < -5) pos_pitch = -25;
			else pos_pitch = pitch * 5;
		}
		else if (adierr == 2)
		{
			// ADI ERROR HIGH
			// 20/10/2.5
			skp->Text( 173, 64, "10", 2 );
			skp->Text( 173, 118, "10", 2 );

			if (roll > 20) pos_roll = 25;
			else if (roll < -20) pos_roll = -25;
			else pos_roll = roll * 1.25;

			if (pitch > 10) pos_pitch = 25;
			else if (pitch < -10) pos_pitch = -25;
			else pos_pitch = pitch * 2.5;
		}
		else
		{
			// ADI ERROR LOW
			// 1/1/2.5
			skp->Text( 173, 64, "1", 1 );
			skp->Text( 173, 118, "1", 1 );

			if (roll > 1) pos_roll = 25;
			else if (roll < -1) pos_roll = -25;
			else pos_roll = roll * 25;

			if (pitch > 1) pos_pitch = 25;
			else if (pitch < -1) pos_pitch = -25;
			else pos_pitch = pitch * 25;
		}
		if (yaw > 2.5) pos_yaw = 25;
		else if (yaw < -2.5) pos_yaw = -25;
		else pos_yaw = yaw * 10;
		skp->SetFont( skpTahomaFont_h10w4 );

		// draw needles
		skp->SetPen( skpMagentaPen );
		skp->SetBrush( skpMagentaBrush );
		skp->Rectangle( 121 + Round( pos_roll ), 94 - Round( sqrt( 3156 - (pos_roll * pos_roll) ) ), 123 + Round( pos_roll ), 77 );// roll
		skp->Rectangle( 139, 93 + Round( pos_pitch ), 122 + Round( sqrt( 3156 - (pos_pitch * pos_pitch) ) ), 95 + Round( pos_pitch ) );// pitch
		skp->Rectangle( 121 + Round( pos_yaw ), 111, 123 + Round( pos_yaw ), 94 + Round( sqrt( 3156 - (pos_yaw * pos_yaw) ) ) );// yaw
		return;// 20/5/1 10/5/1 2.5/2.5/2.5
	}

	void MDU::HSI_A( HDC hDC, double heading, double roll, bool arrowon, double arrowheading )
	{
		double sgn = sign( (90 * RAD) - fabs( roll ) );
		// center (122,238) r = 57
		SelectObject( hDC, gdiCyanPen );
		MoveToEx( hDC, 65, 238, NULL );
		LineTo( hDC, 61, 238 );
		MoveToEx( hDC, 82, 198, NULL );
		LineTo( hDC, 79, 195 );
		MoveToEx( hDC, 122, 186, NULL );
		LineTo( hDC, 122, 177 );
		MoveToEx( hDC, 162, 198, NULL );
		LineTo( hDC, 165, 195 );
		MoveToEx( hDC, 179, 238, NULL );
		LineTo( hDC, 183, 238 );

		SelectObject( hDC, gdiWhitePen );
		SelectObject( hDC, gdiLightGrayBrush );
		::Ellipse( hDC, 70, 186, 175, 291 );

		SelectObject( hDC, gdiBlackBrush );
		::Ellipse( hDC, 89, 205, 156, 272 );

		Arc( hDC, 65, 181, 180, 296, 176, 256, 69, 257 );

		int x1;
		int y1;
		int x2;
		int y2;
		double cosi;
		double sini;
		SetTextColor( hDC, CR_WHITE );
		for (int i = 90; i != 0; i -= 10)// big lines
		{
			cosi = cos( (i * RAD) + (heading * sgn) );
			sini = sin( (i * RAD) + (heading * sgn) );
			x1 = Round( 51 * cosi );
			y1 = Round( 51 * sini );

			x2 = Round( 46 * cosi );
			y2 = Round( 46 * sini );

			MoveToEx( hDC, 122 + x1, 238 - y1, NULL );
			LineTo( hDC, 122 + x2, 238 - y2 );

			MoveToEx( hDC, 122 + y1, 238 + x1, NULL );
			LineTo( hDC, 122 + y2, 238 + x2 );

			MoveToEx( hDC, 122 - x1, 238 + y1, NULL );
			LineTo( hDC, 122 - x2, 238 + y2 );

			MoveToEx( hDC, 122 - y1, 238 - x1, NULL );
			LineTo( hDC, 122 - y2, 238 - x2 );
		}

		for (int i = 85; i > 0; i -= 10)// small lines
		{
			cosi = cos( (i * RAD) + (heading * sgn) );
			sini = sin( (i * RAD) + (heading * sgn) );
			x1 = Round( 51 * cosi );
			y1 = Round( 51 * sini );

			x2 = Round( 48 * cosi );
			y2 = Round( 48 * sini );

			MoveToEx( hDC, 122 + x1, 238 - y1, NULL );
			LineTo( hDC, 122 + x2, 238 - y2 );

			MoveToEx( hDC, 122 + y1, 238 + x1, NULL );
			LineTo( hDC, 122 + y2, 238 + x2 );

			MoveToEx( hDC, 122 - x1, 238 + y1, NULL );
			LineTo( hDC, 122 - x2, 238 + y2 );

			MoveToEx( hDC, 122 - y1, 238 - x1, NULL );
			LineTo( hDC, 122 - y2, 238 - x2 );
		}

		XFORM WTroll;
		char cbuf[4];
		SetGraphicsMode( hDC, GM_ADVANCED );
		for (int i = 0; i <= 33; i += 3)
		{
			// rotate
			WTroll.eM11 = (FLOAT)cos( (heading * sgn) - i * 10 * RAD * sgn );
			WTroll.eM12 = (FLOAT)(-sin( (heading * sgn) - i * 10 * RAD * sgn ));
			WTroll.eM21 = -WTroll.eM12;
			WTroll.eM22 = WTroll.eM11;
			WTroll.eDx = (FLOAT)(122 - (122 * WTroll.eM11) - (238 * WTroll.eM21));
			WTroll.eDy = (FLOAT)(238 - (238 * WTroll.eM11) + (122 * WTroll.eM21));
			SetWorldTransform( hDC, &WTroll );

			// paint
			sprintf_s( cbuf, 4, "%d", i );// number
			TextOut( hDC, 122 - strlen( cbuf ) * 2, 193, cbuf, strlen( cbuf ) );

			// de-rotate
			ModifyWorldTransform( hDC, &WTroll, MWT_IDENTITY );
		}
		SetGraphicsMode( hDC, GM_COMPATIBLE );

		if (arrowon) HSI_Arrow( hDC, arrowheading * sgn );

		POINT ov_symbol[6];
		ov_symbol[0].x = 122;
		ov_symbol[0].y = 229;
		ov_symbol[1].x = 120;
		ov_symbol[1].y = 244;
		ov_symbol[2].x = 117;
		ov_symbol[2].y = 247;
		ov_symbol[3].x = 122;
		ov_symbol[3].y = 252;
		ov_symbol[4].x = 127;
		ov_symbol[4].y = 247;
		ov_symbol[5].x = 124;
		ov_symbol[5].y = 244;
		SelectObject( hDC, gdiLightGrayBrush );
		SelectObject( hDC, gdiLightGrayPen );
		Polygon( hDC, ov_symbol, 6 );
		return;
	}

	void MDU::HSI_A( oapi::Sketchpad2* skp, double heading, double roll, bool arrowon, double arrowheading )
	{
		double sgn = sign( (90 * RAD) - fabs( roll ) );
		// center (122,238) r = 57
		skp->SetPen( skpCyanPen );
		skp->Line( 65, 238, 61, 238 );
		skp->Line( 82, 198, 79, 195 );
		skp->Line( 122, 186, 122, 177 );
		skp->Line( 162, 198, 165, 195 );
		skp->Line( 179, 238, 183, 238 );

		skp->SetPen( skpWhitePen );
		skp->SetBrush( skpLightGrayBrush );
		skp->Ellipse( 70, 186, 175, 291 );

		skp->SetBrush( skpBlackBrush );
		skp->Ellipse( 89, 205, 156, 272 );

		//Arc( hDC, 65, 181, 180, 296, 176, 256, 69, 257 );

		int x1;
		int y1;
		int x2;
		int y2;
		double cosi;
		double sini;
		skp->SetTextColor( CR_WHITE );
		for (int i = 90; i != 0; i -= 10)// big lines
		{
			cosi = cos( (i * RAD) + (heading * sgn) );
			sini = sin( (i * RAD) + (heading * sgn) );
			x1 = Round( 51 * cosi );
			y1 = Round( 51 * sini );

			x2 = Round( 46 * cosi );
			y2 = Round( 46 * sini );

			skp->Line( 122 + x1, 238 - y1, 122 + x2, 238 - y2 );

			skp->Line( 122 + y1, 238 + x1, 122 + y2, 238 + x2 );

			skp->Line( 122 - x1, 238 + y1, 122 - x2, 238 + y2 );

			skp->Line( 122 - y1, 238 - x1, 122 - y2, 238 - x2 );
		}

		for (int i = 85; i > 0; i -= 10)// small lines
		{
			cosi = cos( (i * RAD) + (heading * sgn) );
			sini = sin( (i * RAD) + (heading * sgn) );
			x1 = Round( 51 * cosi );
			y1 = Round( 51 * sini );

			x2 = Round( 48 * cosi );
			y2 = Round( 48 * sini );

			skp->Line( 122 + x1, 238 - y1, 122 + x2, 238 - y2 );

			skp->Line( 122 + y1, 238 + x1, 122 + y2, 238 + x2 );

			skp->Line( 122 - x1, 238 + y1, 122 - x2, 238 + y2 );

			skp->Line( 122 - y1, 238 - x1, 122 - y2, 238 - x2 );
		}

		char cbuf[4];
		oapi::IVECTOR2 cntr;
		cntr.x = 122;
		cntr.y = 238;
		for (int i = 0; i <= 33; i += 3)
		{
			// rotate
			skp->SetWorldTransform2D( 1.0f, (float)(i * 10 * RAD * sgn - (heading * sgn)), &cntr );

			// paint
			sprintf_s( cbuf, 4, "%d", i );// number
			skp->Text( 122 - strlen( cbuf ) * 2, 193, cbuf, strlen( cbuf ) );

			// de-rotate
			skp->SetWorldTransform();
		}

		if (arrowon) HSI_Arrow( skp, arrowheading * sgn );

		oapi::IVECTOR2 ov_symbol[6];
		ov_symbol[0].x = 122;
		ov_symbol[0].y = 229;
		ov_symbol[1].x = 120;
		ov_symbol[1].y = 244;
		ov_symbol[2].x = 117;
		ov_symbol[2].y = 247;
		ov_symbol[3].x = 122;
		ov_symbol[3].y = 252;
		ov_symbol[4].x = 127;
		ov_symbol[4].y = 247;
		ov_symbol[5].x = 124;
		ov_symbol[5].y = 244;
		skp->SetBrush( skpLightGrayBrush );
		skp->SetPen( skpLightGrayPen );
		skp->Polygon( ov_symbol, 6 );
		return;
	}

	void MDU::HSI_E( HDC hDC, double heading, bool arrowon, double arrowheading )
	{
		// center (122,238) r = 57
		SelectObject( hDC, gdiCyanPen );
		MoveToEx( hDC, 65, 238, NULL );
		LineTo( hDC, 61, 238 );
		MoveToEx( hDC, 82, 198, NULL );
		LineTo( hDC, 79, 195 );
		MoveToEx( hDC, 122, 186, NULL );
		LineTo( hDC, 122, 177 );
		MoveToEx( hDC, 162, 198, NULL );
		LineTo( hDC, 165, 195 );
		MoveToEx( hDC, 179, 238, NULL );
		LineTo( hDC, 183, 238 );

		SelectObject( hDC, gdiWhitePen );
		SelectObject( hDC, gdiLightGrayBrush );
		::Ellipse( hDC, 70, 186, 175, 291 );

		SelectObject( hDC, gdiBlackBrush );
		::Ellipse( hDC, 89, 205, 156, 272 );

		Arc( hDC, 65, 181, 180, 296, 176, 256, 69, 257 );

		int x1;
		int y1;
		int x2;
		int y2;
		double cosi;
		double sini;
		SetTextColor( hDC, CR_WHITE );
		for (int i = 90; i != 0; i -= 10)// big lines
		{
			cosi = cos( (i * RAD) + heading );
			sini = sin( (i * RAD) + heading );
			x1 = Round( 51 * cosi );
			y1 = Round( 51 * sini );

			x2 = Round( 46 * cosi );
			y2 = Round( 46 * sini );

			MoveToEx( hDC, 122 + x1, 238 - y1, NULL );
			LineTo( hDC, 122 + x2, 238 - y2 );

			MoveToEx( hDC, 122 + y1, 238 + x1, NULL );
			LineTo( hDC, 122 + y2, 238 + x2 );

			MoveToEx( hDC, 122 - x1, 238 + y1, NULL );
			LineTo( hDC, 122 - x2, 238 + y2 );

			MoveToEx( hDC, 122 - y1, 238 - x1, NULL );
			LineTo( hDC, 122 - y2, 238 - x2 );
		}

		for (int i = 85; i > 0; i -= 10)// small lines
		{
			cosi = cos( (i * RAD) + heading );
			sini = sin( (i * RAD) + heading );
			x1 = Round( 51 * cosi );
			y1 = Round( 51 * sini );

			x2 = Round( 48 * cosi );
			y2 = Round( 48 * sini );

			MoveToEx( hDC, 122 + x1, 238 - y1, NULL );
			LineTo( hDC, 122 + x2, 238 - y2 );

			MoveToEx( hDC, 122 + y1, 238 + x1, NULL );
			LineTo( hDC, 122 + y2, 238 + x2 );

			MoveToEx( hDC, 122 - x1, 238 + y1, NULL );
			LineTo( hDC, 122 - x2, 238 + y2 );

			MoveToEx( hDC, 122 - y1, 238 - x1, NULL );
			LineTo( hDC, 122 - y2, 238 - x2 );
		}

		XFORM WTroll;
		char cbuf[4];
		SetGraphicsMode( hDC, GM_ADVANCED );
		for (int i = 0; i <= 33; i += 3)
		{
			// rotate
			WTroll.eM11 = (FLOAT)cos( heading - i * 10 * RAD );
			WTroll.eM12 = (FLOAT)(-sin( heading - i * 10 * RAD ));
			WTroll.eM21 = -WTroll.eM12;
			WTroll.eM22 = WTroll.eM11;
			WTroll.eDx = (FLOAT)(122 - (122 * WTroll.eM11) - (238 * WTroll.eM21));
			WTroll.eDy = (FLOAT)(238 - (238 * WTroll.eM11) + (122 * WTroll.eM21));
			SetWorldTransform( hDC, &WTroll );

			// paint
			if ((i % 9) != 0) sprintf_s( cbuf, 4, "%d", i );// number
			else if (i == 0) sprintf_s( cbuf, 4, "N" );// letter
			else if (i == 9) sprintf_s( cbuf, 4, "E" );// letter
			else if (i == 18) sprintf_s( cbuf, 4, "S" );// letter
			else sprintf_s( cbuf, 4, "W" );// letter
			TextOut( hDC, 122 - strlen( cbuf ) * 2, 193, cbuf, strlen( cbuf ) );

			// de-rotate
			ModifyWorldTransform( hDC, &WTroll, MWT_IDENTITY );
		}
		SetGraphicsMode( hDC, GM_COMPATIBLE );

		if (arrowon) HSI_Arrow( hDC, (heading * DEG) - arrowheading );

		POINT ov_symbol[6];
		ov_symbol[0].x = 122;
		ov_symbol[0].y = 229;
		ov_symbol[1].x = 120;
		ov_symbol[1].y = 244;
		ov_symbol[2].x = 117;
		ov_symbol[2].y = 247;
		ov_symbol[3].x = 122;
		ov_symbol[3].y = 252;
		ov_symbol[4].x = 127;
		ov_symbol[4].y = 247;
		ov_symbol[5].x = 124;
		ov_symbol[5].y = 244;
		SelectObject( hDC, gdiLightGrayBrush );
		SelectObject( hDC, gdiLightGrayPen );
		Polygon( hDC, ov_symbol, 6 );
		return;
	}

	void MDU::HSI_E( oapi::Sketchpad2* skp, double heading, bool arrowon, double arrowheading )
	{
		// center (122,238) r = 57
		skp->SetPen( skpCyanPen );
		skp->Line( 65, 238, 61, 238 );
		skp->Line( 82, 198, 79, 195 );
		skp->Line( 122, 186, 122, 177 );
		skp->Line( 162, 198, 165, 195 );
		skp->Line( 179, 238, 183, 238 );

		skp->SetPen( skpWhitePen );
		skp->SetBrush( skpLightGrayBrush );
		skp->Ellipse( 70, 186, 175, 291 );

		skp->SetBrush( skpBlackBrush );
		skp->Ellipse( 89, 205, 156, 272 );

		//Arc( hDC, 65, 181, 180, 296, 176, 256, 69, 257 );

		int x1;
		int y1;
		int x2;
		int y2;
		double cosi;
		double sini;
		skp->SetTextColor( CR_WHITE );
		for (int i = 90; i != 0; i -= 10)// big lines
		{
			cosi = cos( (i * RAD) + heading );
			sini = sin( (i * RAD) + heading );
			x1 = Round( 51 * cosi );
			y1 = Round( 51 * sini );

			x2 = Round( 46 * cosi );
			y2 = Round( 46 * sini );

			skp->Line( 122 + x1, 238 - y1, 122 + x2, 238 - y2 );

			skp->Line( 122 + y1, 238 + x1, 122 + y2, 238 + x2 );

			skp->Line( 122 - x1, 238 + y1, 122 - x2, 238 + y2 );

			skp->Line( 122 - y1, 238 - x1, 122 - y2, 238 - x2 );
		}

		for (int i = 85; i > 0; i -= 10)// small lines
		{
			cosi = cos( (i * RAD) + heading );
			sini = sin( (i * RAD) + heading );
			x1 = Round( 51 * cosi );
			y1 = Round( 51 * sini );

			x2 = Round( 48 * cosi );
			y2 = Round( 48 * sini );

			skp->Line( 122 + x1, 238 - y1, 122 + x2, 238 - y2 );

			skp->Line( 122 + y1, 238 + x1, 122 + y2, 238 + x2 );

			skp->Line( 122 - x1, 238 + y1, 122 - x2, 238 + y2 );

			skp->Line( 122 - y1, 238 - x1, 122 - y2, 238 - x2 );
		}

		char cbuf[4];
		oapi::IVECTOR2 cntr;
		cntr.x = 122;
		cntr.y = 238;
		for (int i = 0; i <= 33; i += 3)
		{
			// rotate
			skp->SetWorldTransform2D( 1.0f, (float)(i * 10 * RAD - heading), &cntr );

			// paint
			if ((i % 9) != 0) sprintf_s( cbuf, 4, "%d", i );// number
			else if (i == 0) sprintf_s( cbuf, 4, "N" );// letter
			else if (i == 9) sprintf_s( cbuf, 4, "E" );// letter
			else if (i == 18) sprintf_s( cbuf, 4, "S" );// letter
			else sprintf_s( cbuf, 4, "W" );// letter
			skp->Text( 122 - strlen( cbuf ) * 2, 193, cbuf, strlen( cbuf ) );

			// de-rotate
			skp->SetWorldTransform();
		}

		if (arrowon) HSI_Arrow( skp, (heading * DEG) - arrowheading );

		oapi::IVECTOR2 ov_symbol[6];
		ov_symbol[0].x = 122;
		ov_symbol[0].y = 229;
		ov_symbol[1].x = 120;
		ov_symbol[1].y = 244;
		ov_symbol[2].x = 117;
		ov_symbol[2].y = 247;
		ov_symbol[3].x = 122;
		ov_symbol[3].y = 252;
		ov_symbol[4].x = 127;
		ov_symbol[4].y = 247;
		ov_symbol[5].x = 124;
		ov_symbol[5].y = 244;
		skp->SetBrush( skpLightGrayBrush );
		skp->SetPen( skpLightGrayPen );
		skp->Polygon( ov_symbol, 6 );
		return;
	}

	void MDU::HSI_Arrow( HDC hDC, double heading )
	{
		SelectObject( hDC, gdiWhiteBrush );
		double cosH = cos( -heading * RAD );
		double sinH = sin( -heading * RAD );
		int dotLL_X;
		int dotLL_Y;
		int dotL_X;
		int dotL_Y;
		int dotR_X;
		int dotR_Y;
		int dotRR_X;
		int dotRR_Y;
		POINT arrow[10];

		dotLL_X = 122 - Round( 24 * cosH );
		dotLL_Y = 238 - Round( 24 * sinH );
		dotL_X = 122 - Round( 12 * cosH );
		dotL_Y = 238 - Round( 12 * sinH );
		dotR_X = 122 + Round( 12 * cosH );
		dotR_Y = 238 + Round( 12 * sinH );
		dotRR_X = 122 + Round( 24 * cosH );
		dotRR_Y = 238 + Round( 24 * sinH );

		arrow[0].x = 122 + Round( 49 * sinH );
		arrow[0].y = 238 - Round( 49 * cosH );
		arrow[1].x = 122 - Round( 3 * cosH - 30 * sinH );
		arrow[1].y = 238 - Round( 3 * sinH + 30 * cosH );
		arrow[2].x = 122 - Round( 8 * cosH - 28 * sinH );
		arrow[2].y = 238 - Round( 8 * sinH + 28 * cosH );
		arrow[3].x = 122 - Round( 8 * cosH - 23 * sinH );
		arrow[3].y = 238 - Round( 8 * sinH + 23 * cosH );
		arrow[4].x = 122 - Round( 4 * cosH - 25 * sinH );
		arrow[4].y = 238 - Round( 4 * sinH + 25 * cosH );
		arrow[5].x = 122 + Round( 20 * sinH );
		arrow[5].y = 238 - Round( 20 * cosH );
		arrow[6].x = 122 + Round( 4 * cosH + 25 * sinH );
		arrow[6].y = 238 + Round( 4 * sinH - 25 * cosH );
		arrow[7].x = 122 + Round( 8 * cosH + 23 * sinH );
		arrow[7].y = 238 + Round( 8 * sinH - 23 * cosH );
		arrow[8].x = 122 + Round( 8 * cosH + 28 * sinH );
		arrow[8].y = 238 + Round( 8 * sinH - 28 * cosH );
		arrow[9].x = 122 + Round( 3 * cosH + 30 * sinH );
		arrow[9].y = 238 + Round( 3 * sinH - 30 * cosH );

		::Ellipse( hDC, dotLL_X - 2, dotLL_Y - 2, dotLL_X + 2, dotLL_Y + 2 );
		::Ellipse( hDC, dotL_X - 2, dotL_Y - 2, dotL_X + 2, dotL_Y + 2 );
		::Ellipse( hDC, dotR_X - 2, dotR_Y - 2, dotR_X + 2, dotR_Y + 2 );
		::Ellipse( hDC, dotRR_X - 2, dotRR_Y - 2, dotRR_X + 2, dotRR_Y + 2 );

		SelectObject( hDC, gdiMagentaBrush );
		SelectObject( hDC, gdiBlackPen );
		Polygon( hDC, arrow, 10 );

		arrow[0].x = 122 - Round( 2 * cosH + 20 * sinH );
		arrow[0].y = 238 - Round( 2 * sinH - 20 * cosH );
		arrow[1].x = 122 - Round( 2 * cosH + 40 * sinH );
		arrow[1].y = 238 - Round( 2 * sinH - 40 * cosH );
		arrow[2].x = 122 + Round( 2 * cosH - 40 * sinH );
		arrow[2].y = 238 + Round( 2 * sinH + 40 * cosH );
		arrow[3].x = 122 + Round( 2 * cosH - 20 * sinH );
		arrow[3].y = 238 + Round( 2 * sinH + 20 * cosH );
		Polygon( hDC, arrow, 4 );
		return;
	}

	void MDU::HSI_Arrow( oapi::Sketchpad2* skp, double heading )
	{
		skp->SetBrush( skpWhiteBrush );
		double cosH = cos( -heading * RAD );
		double sinH = sin( -heading * RAD );
		int dotLL_X;
		int dotLL_Y;
		int dotL_X;
		int dotL_Y;
		int dotR_X;
		int dotR_Y;
		int dotRR_X;
		int dotRR_Y;
		oapi::IVECTOR2 arrow[10];

		dotLL_X = 122 - Round( 24 * cosH );
		dotLL_Y = 238 - Round( 24 * sinH );
		dotL_X = 122 - Round( 12 * cosH );
		dotL_Y = 238 - Round( 12 * sinH );
		dotR_X = 122 + Round( 12 * cosH );
		dotR_Y = 238 + Round( 12 * sinH );
		dotRR_X = 122 + Round( 24 * cosH );
		dotRR_Y = 238 + Round( 24 * sinH );

		arrow[0].x = 122 + Round( 49 * sinH );
		arrow[0].y = 238 - Round( 49 * cosH );
		arrow[1].x = 122 - Round( 3 * cosH - 30 * sinH );
		arrow[1].y = 238 - Round( 3 * sinH + 30 * cosH );
		arrow[2].x = 122 - Round( 8 * cosH - 28 * sinH );
		arrow[2].y = 238 - Round( 8 * sinH + 28 * cosH );
		arrow[3].x = 122 - Round( 8 * cosH - 23 * sinH );
		arrow[3].y = 238 - Round( 8 * sinH + 23 * cosH );
		arrow[4].x = 122 - Round( 4 * cosH - 25 * sinH );
		arrow[4].y = 238 - Round( 4 * sinH + 25 * cosH );
		arrow[5].x = 122 + Round( 20 * sinH );
		arrow[5].y = 238 - Round( 20 * cosH );
		arrow[6].x = 122 + Round( 4 * cosH + 25 * sinH );
		arrow[6].y = 238 + Round( 4 * sinH - 25 * cosH );
		arrow[7].x = 122 + Round( 8 * cosH + 23 * sinH );
		arrow[7].y = 238 + Round( 8 * sinH - 23 * cosH );
		arrow[8].x = 122 + Round( 8 * cosH + 28 * sinH );
		arrow[8].y = 238 + Round( 8 * sinH - 28 * cosH );
		arrow[9].x = 122 + Round( 3 * cosH + 30 * sinH );
		arrow[9].y = 238 + Round( 3 * sinH - 30 * cosH );

		skp->Ellipse( dotLL_X - 2, dotLL_Y - 2, dotLL_X + 2, dotLL_Y + 2 );
		skp->Ellipse( dotL_X - 2, dotL_Y - 2, dotL_X + 2, dotL_Y + 2 );
		skp->Ellipse( dotR_X - 2, dotR_Y - 2, dotR_X + 2, dotR_Y + 2 );
		skp->Ellipse( dotRR_X - 2, dotRR_Y - 2, dotRR_X + 2, dotRR_Y + 2 );

		skp->SetBrush( skpMagentaBrush );
		skp->SetPen( skpBlackPen );
		skp->Polygon( arrow, 10 );

		arrow[0].x = 122 - Round( 2 * cosH + 20 * sinH );
		arrow[0].y = 238 - Round( 2 * sinH - 20 * cosH );
		arrow[1].x = 122 - Round( 2 * cosH + 40 * sinH );
		arrow[1].y = 238 - Round( 2 * sinH - 40 * cosH );
		arrow[2].x = 122 + Round( 2 * cosH - 40 * sinH );
		arrow[2].y = 238 + Round( 2 * sinH + 40 * cosH );
		arrow[3].x = 122 + Round( 2 * cosH - 20 * sinH );
		arrow[3].y = 238 + Round( 2 * sinH + 20 * cosH );
		skp->Polygon( arrow, 4 );
		return;
	}

	void MDU::AEPFD_Header_AscentDAP( HDC hDC, int MM, int adiatt )
	{
		char cbuf[8];
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 21, 4, "DAP:", 4 );

		TextOut( hDC, 209, 4, "MM:", 3 );
		
		TextOut( hDC, 9, 13, "THROT:", 6 );

		TextOut( hDC, 208, 13, "ATT:", 4 );

		SetTextColor( hDC, CR_WHITE );
		if (1) TextOut( hDC, 40, 4, "Auto", 4 );// TODO get AscentDAP state
		else
		{
			SelectObject( hDC, gdiYellowPen );
			SelectObject( hDC, GetStockObject( HOLLOW_BRUSH ) );
			Rectangle( hDC, 6, 4, 60, 14 );
			TextOut( hDC, 40, 4, "CSS", 3 );
		}

		if (0) sprintf_s( cbuf, 8, "%dR", MM );// RTLS
		else if (0) sprintf_s( cbuf, 8, "%dT", MM );// TAL
		else if (0) sprintf_s( cbuf, 8, "%dATO", MM );// ATO
		else if (0) sprintf_s( cbuf, 8, "%dAOA", MM );// AOA
		else if (0) sprintf_s( cbuf, 8, "%dCA", MM );// CA
		else sprintf_s( cbuf, 8, "%d", MM );// NOM
		TextOut( hDC, 225, 4, cbuf, strlen( cbuf ) );

		if (GetIDP()->GetAutoThrottleState() == true) TextOut( hDC, 40, 13, "Auto", 4 );
		else
		{
			SelectObject( hDC, gdiYellowPen );
			SelectObject( hDC, GetStockObject( HOLLOW_BRUSH ) );
			Rectangle( hDC, 6, 13, 60, 23 );
			TextOut( hDC, 40, 13, "MAN", 3 );
		}

		if (adiatt == 2) TextOut( hDC, 228, 13, "INRTL", 5 );
		else if (adiatt == 1) TextOut( hDC, 228, 13, "LVLH", 4 );
		else TextOut( hDC, 228, 13, "REF", 3 );
		return;
	}

	void MDU::AEPFD_Header_AscentDAP( oapi::Sketchpad2* skp, int MM, int adiatt )
	{
		char cbuf[8];
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 21, 4, "DAP:", 4 );

		skp->Text( 209, 4, "MM:", 3 );
		
		skp->Text( 9, 13, "THROT:", 6 );

		skp->Text( 208, 13, "ATT:", 4 );

		skp->SetTextColor( CR_WHITE );
		if (1) skp->Text( 40, 4, "Auto", 4 );// TODO get AscentDAP state
		else
		{
			skp->SetPen( skpYellowPen );
			skp->SetBrush( NULL );
			skp->Rectangle( 6, 4, 60, 14 );
			skp->Text( 40, 4, "CSS", 3 );
		}

		if (0) sprintf_s( cbuf, 8, "%dR", MM );// RTLS
		else if (0) sprintf_s( cbuf, 8, "%dT", MM );// TAL
		else if (0) sprintf_s( cbuf, 8, "%dATO", MM );// ATO
		else if (0) sprintf_s( cbuf, 8, "%dAOA", MM );// AOA
		else if (0) sprintf_s( cbuf, 8, "%dCA", MM );// CA
		else sprintf_s( cbuf, 8, "%d", MM );// NOM
		skp->Text( 225, 4, cbuf, strlen( cbuf ) );

		if (GetIDP()->GetAutoThrottleState() == true) skp->Text( 40, 13, "Auto", 4 );
		else
		{
			skp->SetPen( skpYellowPen );
			skp->SetBrush( NULL );
			skp->Rectangle( 6, 13, 60, 23 );
			skp->Text( 40, 13, "MAN", 3 );
		}

		if (adiatt == 2) skp->Text( 228, 13, "INRTL", 5 );
		else if (adiatt == 1) skp->Text( 228, 13, "LVLH", 4 );
		else skp->Text( 228, 13, "REF", 3 );
		return;
	}

	void MDU::AEPFD_Header_TransDAP( HDC hDC, int MM, int adiatt )
	{
		char cbuf[4];
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 21, 4, "DAP:", 4 );

		TextOut( hDC, 209, 4, "MM:", 3 );

		TextOut( hDC, 208, 13, "ATT:", 4 );

		SetTextColor( hDC, CR_WHITE );
		if (1) TextOut( hDC, 40, 4, "Auto", 4 );// TODO get TransDAP state
		else TextOut( hDC, 40, 4, "INRTL", 5 );

		sprintf_s( cbuf, 4, "%d", MM );
		TextOut( hDC, 225, 4, cbuf, strlen( cbuf ) );

		if (adiatt == 2) TextOut( hDC, 228, 13, "INRTL", 5 );
		else if (adiatt == 1) TextOut( hDC, 228, 13, "LVLH", 4 );
		else TextOut( hDC, 228, 13, "REF", 3 );
		return;
	}

	void MDU::AEPFD_Header_TransDAP( oapi::Sketchpad2* skp, int MM, int adiatt )
	{
		char cbuf[4];
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 21, 4, "DAP:", 4 );

		skp->Text( 209, 4, "MM:", 3 );

		skp->Text( 208, 13, "ATT:", 4 );

		skp->SetTextColor( CR_WHITE );
		if (1) skp->Text( 40, 4, "Auto", 4 );// TODO get TransDAP state
		else skp->Text( 40, 4, "INRTL", 5 );

		sprintf_s( cbuf, 4, "%d", MM );
		skp->Text( 225, 4, cbuf, strlen( cbuf ) );

		if (adiatt == 2) skp->Text( 228, 13, "INRTL", 5 );
		else if (adiatt == 1) skp->Text( 228, 13, "LVLH", 4 );
		else skp->Text( 228, 13, "REF", 3 );
		return;
	}

	void MDU::AEPFD_Header_AerojetDAP( HDC hDC, int MM, double MachNumber )
	{
		char cbuf[8];
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 16, 4, "Pitch:", 6 );

		TextOut( hDC, 21, 13, "R/Y:", 4 );

		TextOut( hDC, 209, 4, "MM:", 3 );

		TextOut( hDC, 210, 13, "SB:", 3 );

		SetTextColor( hDC, CR_WHITE );
		if (GetIDP()->GetAutoPitchState() == true) TextOut( hDC, 40, 4, "Auto", 4 );
		else
		{
			if (MachNumber > 1)
			{
				SelectObject( hDC, gdiYellowPen );
				SelectObject( hDC, GetStockObject( HOLLOW_BRUSH ) );
				Rectangle( hDC, 6, 4, 60, 14 );
			}
			TextOut( hDC, 40, 4, "CSS", 3 );
		}

		if (GetIDP()->GetAutoRollYawState() == true) TextOut( hDC, 40, 13, "Auto", 4 );
		else
		{
			if (MachNumber > 1)
			{
				SelectObject( hDC, gdiYellowPen );
				SelectObject( hDC, GetStockObject( HOLLOW_BRUSH ) );
				Rectangle( hDC, 6, 13, 60, 23 );
			}
			TextOut( hDC, 40, 13, "CSS", 3 );
		}

		if (0) sprintf_s( cbuf, 8, "%dR", MM );// RTLS
		else if (0) sprintf_s( cbuf, 8, "%dT", MM );// TAL
		else if (0) sprintf_s( cbuf, 8, "%dATO", MM );// ATO
		else if (0) sprintf_s( cbuf, 8, "%dAOA", MM );// AOA
		else if (0) sprintf_s( cbuf, 8, "%dCA", MM );// CA
		else sprintf_s( cbuf, 8, "%d", MM );// NOM
		TextOut( hDC, 225, 4, cbuf, strlen( cbuf ) );

		if (GetIDP()->GetAutoSpeedbrakeState() == true) TextOut( hDC, 225, 13, "Auto", 4 );
		else
		{
			SelectObject( hDC, gdiYellowPen );
			SelectObject( hDC, GetStockObject( HOLLOW_BRUSH ) );
			Rectangle( hDC, 207, 13, 245, 23 );
			TextOut( hDC, 225, 13, "MAN", 3 );
		}
		return;
	}

	void MDU::AEPFD_Header_AerojetDAP( oapi::Sketchpad2* skp, int MM, double MachNumber )
	{
		char cbuf[8];
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 16, 4, "Pitch:", 6 );

		skp->Text( 21, 13, "R/Y:", 4 );

		skp->Text( 209, 4, "MM:", 3 );

		skp->Text( 210, 13, "SB:", 3 );

		skp->SetTextColor( CR_WHITE );
		if (GetIDP()->GetAutoPitchState() == true) skp->Text( 40, 4, "Auto", 4 );
		else
		{
			if (MachNumber > 1)
			{
				skp->SetPen( skpYellowPen );
				skp->SetBrush( NULL );
				skp->Rectangle( 6, 4, 60, 14 );
			}
			skp->Text( 40, 4, "CSS", 3 );
		}

		if (GetIDP()->GetAutoRollYawState() == true) skp->Text( 40, 13, "Auto", 4 );
		else
		{
			if (MachNumber > 1)
			{
				skp->SetPen( skpYellowPen );
				skp->SetBrush( NULL );
				skp->Rectangle( 6, 13, 60, 23 );
			}
			skp->Text( 40, 13, "CSS", 3 );
		}

		if (0) sprintf_s( cbuf, 8, "%dR", MM );// RTLS
		else if (0) sprintf_s( cbuf, 8, "%dT", MM );// TAL
		else if (0) sprintf_s( cbuf, 8, "%dATO", MM );// ATO
		else if (0) sprintf_s( cbuf, 8, "%dAOA", MM );// AOA
		else if (0) sprintf_s( cbuf, 8, "%dCA", MM );// CA
		else sprintf_s( cbuf, 8, "%d", MM );// NOM
		skp->Text( 225, 4, cbuf, strlen( cbuf ) );

		if (GetIDP()->GetAutoSpeedbrakeState() == true) skp->Text( 225, 13, "Auto", 4 );
		else
		{
			skp->SetPen( skpYellowPen );
			skp->SetBrush( NULL );
			skp->Rectangle( 207, 13, 245, 23 );
			skp->Text( 225, 13, "MAN", 3 );
		}
		return;
	}

	void MDU::AEPFD_BETA( HDC hDC )
	{
		SelectObject( hDC, gdiDarkGrayPen );
		SelectObject( hDC, gdiBlackBrush );
		Rectangle( hDC, 37, 159, 60, 171 );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 41, 172, "Beta", 4 );

		double beta = STS()->GetSlipAngle() * DEG;
		char side = 'R';
		if (beta < 0) side = 'L';
		SetTextColor( hDC, CR_WHITE );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%c%04.1f", side, fabs( beta ) );
		TextOut( hDC, 39, 160, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::AEPFD_BETA( oapi::Sketchpad2* skp )
	{
		skp->SetPen( skpDarkGrayPen );
		skp->SetBrush( skpBlackBrush );
		skp->Rectangle( 37, 159, 60, 171 );
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 41, 172, "Beta", 4 );

		double beta = STS()->GetSlipAngle() * DEG;
		char side = 'R';
		if (beta < 0) side = 'L';
		skp->SetTextColor( CR_WHITE );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%c%04.1f", side, fabs( beta ) );
		skp->Text( 39, 160, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::AEPFD_GMETER_STATIC( HDC hDC )
	{
		SelectObject( hDC, gdiDarkGrayPen );
		SelectObject( hDC, gdiBlackBrush );
		Rectangle( hDC, 40, 198, 66, 211 );

		SelectObject( hDC, gdiLightGrayPen );
		Arc( hDC, 10, 193, 58, 241, 34, 193, 41, 224 );
		
		MoveToEx( hDC, 34, 193, NULL );
		LineTo( hDC, 34, 189 );
		MoveToEx( hDC, 17, 200, NULL );
		LineTo( hDC, 14, 197 );
		MoveToEx( hDC, 10, 217, NULL );
		LineTo( hDC, 6, 217 );
		MoveToEx( hDC, 17, 234, NULL );
		LineTo( hDC, 14, 237 );
		MoveToEx( hDC, 34, 241, NULL );
		LineTo( hDC, 34, 245 );
		MoveToEx( hDC, 50, 234, NULL );
		LineTo( hDC, 54, 237 );

		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 10, 193, "3", 1 );
		TextOut( hDC, 2, 213, "2", 1 );
		TextOut( hDC, 10, 231, "1", 1 );
		TextOut( hDC, 32, 244, "0", 1 );
		TextOut( hDC, 52, 234, "-1", 2 );
		return;
	}

	void MDU::AEPFD_GMETER_STATIC( oapi::Sketchpad2* skp )
	{
		skp->SetPen( skpLightGrayPen );
		skp->SetBrush( skpBlackBrush );
		skp->Ellipse( 10, 193, 58, 241 );
		skp->SetPen( skpBlackPen );
		skp->Rectangle( 34, 193, 58, 234 );// cover part of the circle to get an arc because sketchpad doesn't have Arc()

		skp->SetPen( skpDarkGrayPen );
		skp->Rectangle( 40, 198, 66, 211 );
		
		skp->SetPen( skpLightGrayPen );
		skp->Line( 34, 193, 34, 189 );
		skp->Line( 17, 200, 14, 197 );
		skp->Line( 10, 217, 6, 217 );
		skp->Line( 17, 234, 14, 237 );
		skp->Line( 34, 241, 34, 245 );
		skp->Line( 50, 234, 54, 237 );

		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 10, 193, "3", 1 );
		skp->Text( 2, 213, "2", 1 );
		skp->Text( 10, 231, "1", 1 );
		skp->Text( 32, 244, "0", 1 );
		skp->Text( 52, 234, "-1", 2 );
		return;
	}

	void MDU::AEPFD_GMETER_ACCEL( HDC hDC )
	{
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 43, 212, "Accel", 5 );
		VECTOR3 f;
		STS()->GetForceVector( f );
		double dtmp = (f.z / (STS()->GetMass() * G)) + sin( STS()->GetPitch() );

		SetTextColor( hDC, CR_WHITE );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%4.1f g", dtmp );
		TextOut( hDC, 43, 200, cbuf, strlen( cbuf ) );

		if (dtmp > 4) dtmp = 4;
		else if (dtmp < -1) dtmp = -1;
		dtmp = ((dtmp * 45) - 90) * RAD;

		double cosdtmp = cos( dtmp );
		double sindtmp = sin( dtmp );
		// center (34.217)
		POINT arrow[3];
		arrow[0].x = 34 - Round( 23 * cosdtmp );
		arrow[0].y = 217 - Round( 23 * sindtmp );
		arrow[1].x = 34 - Round( (17 * cosdtmp) + (4 * sindtmp) );
		arrow[1].y = 217 - Round( (17 * sindtmp) - (4 * cosdtmp) );
		arrow[2].x = 34 - Round( (17 * cosdtmp) - (4 * sindtmp) );
		arrow[2].y = 217 - Round( (17 * sindtmp) + (4 * cosdtmp) );

		SelectObject( hDC, gdiLightGreenPen );
		SelectObject( hDC, gdiLightGreenBrush );
		Polygon( hDC, arrow, 3 );

		SelectObject( hDC, gdiLightGreenThickPen );
		MoveToEx( hDC, 34, 217, NULL );
		LineTo( hDC, arrow[0].x, arrow[0].y );
		return;
	}

	void MDU::AEPFD_GMETER_ACCEL( oapi::Sketchpad2* skp )
	{
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 43, 212, "Accel", 5 );
		VECTOR3 f;
		STS()->GetForceVector( f );
		double dtmp = (f.z / (STS()->GetMass() * G)) + sin( STS()->GetPitch() );

		skp->SetTextColor( CR_WHITE );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%4.1f g", dtmp );
		skp->Text( 43, 200, cbuf, strlen( cbuf ) );

		if (dtmp > 4) dtmp = 4;
		else if (dtmp < -1) dtmp = -1;
		dtmp = ((dtmp * 45) - 90) * RAD;

		double cosdtmp = cos( dtmp );
		double sindtmp = sin( dtmp );
		// center (34.217)
		oapi::IVECTOR2 arrow[3];
		arrow[0].x = 34 - Round( 23 * cosdtmp );
		arrow[0].y = 217 - Round( 23 * sindtmp );
		arrow[1].x = 34 - Round( (17 * cosdtmp) + (4 * sindtmp) );
		arrow[1].y = 217 - Round( (17 * sindtmp) - (4 * cosdtmp) );
		arrow[2].x = 34 - Round( (17 * cosdtmp) - (4 * sindtmp) );
		arrow[2].y = 217 - Round( (17 * sindtmp) + (4 * cosdtmp) );

		skp->SetPen( skpLightGreenPen );
		skp->SetBrush( skpLightGreenBrush );
		skp->Polygon( arrow, 3 );

		skp->SetPen( skpLightGreenThickPen );
		skp->Line( 34, 217, arrow[0].x, arrow[0].y );
		return;
	}

	void MDU::AEPFD_GMETER_NZ( HDC hDC )
	{
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 43, 212, "Nz", 2 );
		double dtmp = GetIDP()->GetNZ();

		SetTextColor( hDC, CR_WHITE );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%4.1f g", dtmp );
		TextOut( hDC, 43, 200, cbuf, strlen( cbuf ) );

		if (dtmp > 4) dtmp = 4;
		else if (dtmp < -1) dtmp = -1;
		dtmp = ((dtmp * 45) - 90) * RAD;

		double cosdtmp = cos( dtmp );
		double sindtmp = sin( dtmp );
		// center (34.217)
		POINT arrow[3];
		arrow[0].x = 34 - Round( 23 * cosdtmp );
		arrow[0].y = 217 - Round( 23 * sindtmp );
		arrow[1].x = 34 - Round( (17 * cosdtmp) + (4 * sindtmp) );
		arrow[1].y = 217 - Round( (17 * sindtmp) - (4 * cosdtmp) );
		arrow[2].x = 34 - Round( (17 * cosdtmp) - (4 * sindtmp) );
		arrow[2].y = 217 - Round( (17 * sindtmp) + (4 * cosdtmp) );

		SelectObject( hDC, gdiLightGreenPen );
		SelectObject( hDC, gdiLightGreenBrush );
		Polygon( hDC, arrow, 3 );

		SelectObject( hDC, gdiLightGreenThickPen );
		MoveToEx( hDC, 34, 217, NULL );
		LineTo( hDC, arrow[0].x, arrow[0].y );
		return;
	}

	void MDU::AEPFD_GMETER_NZ( oapi::Sketchpad2* skp )
	{
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 43, 212, "Nz", 2 );
		double dtmp = GetIDP()->GetNZ();

		skp->SetTextColor( CR_WHITE );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%4.1f g", dtmp );
		skp->Text( 43, 200, cbuf, strlen( cbuf ) );

		if (dtmp > 4) dtmp = 4;
		else if (dtmp < -1) dtmp = -1;
		dtmp = ((dtmp * 45) - 90) * RAD;

		double cosdtmp = cos( dtmp );
		double sindtmp = sin( dtmp );
		// center (34.217)
		oapi::IVECTOR2 arrow[3];
		arrow[0].x = 34 - Round( 23 * cosdtmp );
		arrow[0].y = 217 - Round( 23 * sindtmp );
		arrow[1].x = 34 - Round( (17 * cosdtmp) + (4 * sindtmp) );
		arrow[1].y = 217 - Round( (17 * sindtmp) - (4 * cosdtmp) );
		arrow[2].x = 34 - Round( (17 * cosdtmp) - (4 * sindtmp) );
		arrow[2].y = 217 - Round( (17 * sindtmp) + (4 * cosdtmp) );

		skp->SetPen( skpLightGreenPen );
		skp->SetBrush( skpLightGreenBrush );
		skp->Polygon( arrow, 3 );

		skp->SetPen( skpLightGreenThickPen );
		skp->Line( 34, 217, arrow[0].x, arrow[0].y );
		return;
	}

	void MDU::AEPFD_HACCEL( HDC hDC )
	{
		SelectObject( hDC, gdiDarkGrayPen );
		Rectangle( hDC, 234, 168, 241, 240 );
		SelectObject( hDC, gdiWhiteBrush );
		Rectangle( hDC, 234, 168, 241, 205 );
		SelectObject( hDC, gdiLightGrayBrush );
		SelectObject( hDC, gdiLightGrayPen );
		Rectangle( hDC, 235, 204, 240, 239 );
		SelectObject( hDC, gdiDarkGrayPen );
		SelectObject( hDC, gdiBlackBrush );
		MoveToEx( hDC, 241, 169, NULL );
		LineTo( hDC, 244, 169 );
		MoveToEx( hDC, 241, 176, NULL );
		LineTo( hDC, 244, 176 );
		MoveToEx( hDC, 241, 183, NULL );
		LineTo( hDC, 244, 183 );
		MoveToEx( hDC, 241, 190, NULL );
		LineTo( hDC, 244, 190 );
		MoveToEx( hDC, 241, 197, NULL );
		LineTo( hDC, 244, 197 );
		MoveToEx( hDC, 241, 204, NULL );
		LineTo( hDC, 244, 204 );
		MoveToEx( hDC, 241, 211, NULL );
		LineTo( hDC, 244, 211 );
		MoveToEx( hDC, 241, 218, NULL );
		LineTo( hDC, 244, 218 );
		MoveToEx( hDC, 241, 225, NULL );
		LineTo( hDC, 244, 225 );
		MoveToEx( hDC, 241, 232, NULL );
		LineTo( hDC, 244, 232 );
		MoveToEx( hDC, 241, 239, NULL );
		LineTo( hDC, 244, 239 );
		SetPixel( hDC, 236, 158, CR_LIGHT_GRAY );
		SetPixel( hDC, 238, 158, CR_LIGHT_GRAY );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 235, 159, "H", 1 );
		SetTextColor( hDC, CR_WHITE );
		TextOut( hDC, 244, 165, "10", 2 );
		TextOut( hDC, 245, 200, "0", 1 );
		TextOut( hDC, 244, 235, "-10", 3 );

		double dtmp = GetIDP()->GetVacc();
		if (dtmp > 10) dtmp = 10;
		else if (dtmp < -10) dtmp = -10;

		POINT tri[3];// starts at tip moving cw
		tri[0].x = 237;
		tri[0].y = 204 - Round( dtmp * 3.5 );
		tri[1].x = 229;
		tri[1].y = tri[0].y + 5;
		tri[2].x = 229;
		tri[2].y = tri[0].y - 5;

		SelectObject( hDC, gdiLightGreenPen );
		SelectObject( hDC, gdiLightGreenBrush );
		Polygon( hDC, tri, 3 );
		return;
	}

	void MDU::AEPFD_HACCEL( oapi::Sketchpad2* skp )
	{
		skp->SetPen( skpDarkGrayPen );
		skp->Rectangle( 234, 168, 241, 240 );
		skp->SetBrush( skpWhiteBrush );
		skp->Rectangle( 234, 168, 241, 205 );
		skp->SetBrush( skpLightGrayBrush );
		skp->SetPen( skpLightGrayPen );
		skp->Rectangle( 235, 204, 240, 239 );
		skp->SetPen( skpDarkGrayPen );
		skp->SetBrush( skpBlackBrush );
		skp->Line( 241, 169, 244, 169 );
		skp->Line( 241, 176, 244, 176 );
		skp->Line( 241, 183, 244, 183 );
		skp->Line( 241, 190, 244, 190 );
		skp->Line( 241, 197, 244, 197 );
		skp->Line( 241, 204, 244, 204 );
		skp->Line( 241, 211, 244, 211 );
		skp->Line( 241, 218, 244, 218 );
		skp->Line( 241, 225, 244, 225 );
		skp->Line( 241, 232, 244, 232 );
		skp->Line( 241, 239, 244, 239 );
		skp->Pixel( 236, 158, CR_LIGHT_GRAY );
		skp->Pixel( 238, 158, CR_LIGHT_GRAY );
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 235, 159, "H", 1 );
		skp->SetTextColor( CR_WHITE );
		skp->Text( 244, 165, "10", 2 );
		skp->Text( 245, 200, "0", 1 );
		skp->Text( 244, 235, "-10", 3 );

		double dtmp = GetIDP()->GetVacc();
		if (dtmp > 10) dtmp = 10;
		else if (dtmp < -10) dtmp = -10;

		oapi::IVECTOR2 tri[3];// starts at tip moving cw
		tri[0].x = 237;
		tri[0].y = 204 - Round( dtmp * 3.5 );
		tri[1].x = 229;
		tri[1].y = tri[0].y + 5;
		tri[2].x = 229;
		tri[2].y = tri[0].y - 5;

		skp->SetPen( skpLightGreenPen );
		skp->SetBrush( skpLightGreenBrush );
		skp->Polygon( tri, 3 );
		return;
	}

	void MDU::AEPFD_RANGERW( HDC hDC )
	{
		SelectObject( hDC, gdiDarkGrayPen );
		SelectObject( hDC, gdiBlackBrush );
		Rectangle( hDC, 190, 206, 213, 219 );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 189, 196, GetIDP()->GetSelectedRunway().c_str(), 5 );
		SetTextColor( hDC, CR_WHITE );
		double dtmp = GetIDP()->GetRangeToRunway();
		char cbuf[8];
		if (dtmp > 1000) sprintf_s( cbuf, 8, "%4.0f", dtmp );// HACK max should be 999.9, but this way is better for testing
		else sprintf_s( cbuf, 8, "%5.1f", dtmp );
		TextOut( hDC, 193, 207, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::AEPFD_RANGERW( oapi::Sketchpad2* skp )
	{
		skp->SetPen( skpDarkGrayPen );
		skp->SetBrush( skpBlackBrush );
		skp->Rectangle( 190, 206, 213, 219 );
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 189, 196, GetIDP()->GetSelectedRunway().c_str(), 5 );
		skp->SetTextColor( CR_WHITE );
		double dtmp = GetIDP()->GetRangeToRunway();
		char cbuf[8];
		if (dtmp > 1000) sprintf_s( cbuf, 8, "%4.0f", dtmp );// HACK max should be 999.9, but this way is better for testing
		else sprintf_s( cbuf, 8, "%5.1f", dtmp );
		skp->Text( 193, 207, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::AEPFD_RANGEHACC( HDC hDC )
	{
		SelectObject( hDC, gdiDarkGrayPen );
		SelectObject( hDC, gdiBlackBrush );
		Rectangle( hDC, 190, 231, 213, 244 );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 190, 221, "HAC-C", 5 );
		SetTextColor( hDC, CR_WHITE );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%5.1f", GetIDP()->GetDistanceToHACCenter() );
		TextOut( hDC, 193, 233, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::AEPFD_RANGEHACC( oapi::Sketchpad2* skp )
	{
		skp->SetPen( skpDarkGrayPen );
		skp->SetBrush( skpBlackBrush );
		skp->Rectangle( 190, 231, 213, 244 );
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 190, 221, "HAC-C", 5 );
		skp->SetTextColor( CR_WHITE );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%5.1f", GetIDP()->GetDistanceToHACCenter() );
		skp->Text( 193, 233, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::AEPFD_dAZ_HTA( HDC hDC, double MachNumber )
	{
		double dtmp;
		char cbuf[8];
		SelectObject( hDC, gdiBlackBrush );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		if (GetIDP()->GetOnHACState() == false)
		{
			MoveToEx( hDC, 149, 179, NULL );
			SelectObject( hDC, gdiLightGrayPen );
			LineTo( hDC, 151, 174 );
			LineTo( hDC, 153, 179 );
			LineTo( hDC, 149, 179 );
			TextOut( hDC, 154, 172, "AZ", 2 );
			dtmp = fabs( GetIDP()->GetdeltaAZ() );
			if (dtmp >= GetIDP()->GetdeltaAZLimit( MachNumber ))
			{
				// rectangle flashes red for limits
				if (GetFlash() == false) SelectObject( hDC, gdiBlackPen );
				else SelectObject( hDC, gdiRedPen );
			}
			else SelectObject( hDC, gdiDarkGrayPen );
		}
		else
		{
			TextOut( hDC, 149, 172, "HTA", 3 );
			dtmp = GetIDP()->GetHTA();
			SelectObject( hDC, gdiDarkGrayPen );
		}

		Rectangle( hDC, 167, 171, 188, 183 );

		SetTextColor( hDC, CR_WHITE );
		sprintf_s( cbuf, 8, "%3.0f%c", dtmp, 248 );
		TextOut( hDC, 170, 172, cbuf, strlen( cbuf ) );
		return;
	}
	
	void MDU::AEPFD_dAZ_HTA( oapi::Sketchpad2* skp, double MachNumber )
	{
		double dtmp;
		char cbuf[8];
		skp->SetBrush( skpBlackBrush );
		skp->SetTextColor( CR_LIGHT_GRAY );
		if (GetIDP()->GetOnHACState() == false)
		{
			skp->MoveTo( 149, 179 );
			skp->SetPen( skpLightGrayPen );
			skp->LineTo( 151, 174 );
			skp->LineTo( 153, 179 );
			skp->LineTo( 149, 179 );
			skp->Text( 154, 172, "AZ", 2 );
			dtmp = fabs( GetIDP()->GetdeltaAZ() );
			if (dtmp >= GetIDP()->GetdeltaAZLimit( MachNumber ))
			{
				// rectangle flashes red for limits
				if (GetFlash() == false) skp->SetPen( skpBlackPen );
				else skp->SetPen( skpRedPen );
			}
			else skp->SetPen( skpDarkGrayPen );
		}
		else
		{
			skp->Text( 149, 172, "HTA", 3 );
			dtmp = GetIDP()->GetHTA();
			skp->SetPen( skpDarkGrayPen );
		}

		skp->Rectangle( 167, 171, 188, 183 );

		skp->SetTextColor( CR_WHITE );
		sprintf_s( cbuf, 8, "%3.0f%c", dtmp, 176 );
		skp->Text( 170, 172, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::AEPFD_dXTRK( HDC hDC )
	{
		SelectObject( hDC, gdiDarkGrayPen );
		SelectObject( hDC, gdiBlackBrush );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		POINT tri[3] = {{203, 168}, {205, 173}, {201, 173}};
		Polygon( hDC, tri, 3 );
		TextOut( hDC, 208, 166, "X-Trk", 5 );
		Rectangle( hDC, 229, 164, 252, 176 );
		SetTextColor( hDC, CR_WHITE );
		double dtmp = 0;// TODO
		char cbuf[8];
		sprintf_s( cbuf, 8, "%5.1f", dtmp );
		TextOut( hDC, 232, 165, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::AEPFD_dXTRK( oapi::Sketchpad2* skp )
	{
		skp->SetPen( skpDarkGrayPen );
		skp->SetBrush( skpBlackBrush );
		skp->SetTextColor( CR_LIGHT_GRAY );
		oapi::IVECTOR2 tri[3] = {{203, 168}, {205, 173}, {201, 173}};
		skp->Polygon( tri, 3 );
		skp->Text( 208, 166, "X-Trk", 5 );
		skp->Rectangle( 229, 164, 252, 176 );
		skp->SetTextColor( CR_WHITE );
		double dtmp = 0;// TODO
		char cbuf[8];
		sprintf_s( cbuf, 8, "%5.1f", dtmp );
		skp->Text( 232, 165, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::AEPFD_XTRK( HDC hDC )
	{
		SelectObject( hDC, gdiDarkGrayPen );
		SelectObject( hDC, gdiBlackBrush );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 207, 184, "X-Trk", 5 );
		Rectangle( hDC, 229, 182, 252, 194 );
		SetTextColor( hDC, CR_WHITE );
		double dtmp = 0;// TODO
		char cbuf[8];
		sprintf_s( cbuf, 8, "%5.1f", dtmp );
		TextOut( hDC, 232, 183, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::AEPFD_XTRK( oapi::Sketchpad2* skp )
	{
		skp->SetPen( skpDarkGrayPen );
		skp->SetBrush( skpBlackBrush );
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 207, 184, "X-Trk", 5 );
		skp->Rectangle( 229, 182, 252, 194 );
		skp->SetTextColor( CR_WHITE );
		double dtmp = 0;// TODO
		char cbuf[8];
		sprintf_s( cbuf, 8, "%5.1f", dtmp );
		skp->Text( 232, 183, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::AEPFD_dINC( HDC hDC )
	{
		SelectObject( hDC, gdiLightGrayPen );
		SelectObject( hDC, gdiBlackBrush );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		POINT tri[3] = {{205, 205}, {207, 210}, {203, 210}};
		Polygon( hDC, tri, 3 );
		SelectObject( hDC, gdiDarkGrayPen );
		TextOut( hDC, 210, 203, "Inc", 3 );
		Rectangle( hDC, 223, 201, 252, 213 );
		SetTextColor( hDC, CR_WHITE );
		ELEMENTS el;
		STS()->GetElements( STS()->GetGravityRef(), el, NULL, 0, FRAME_EQU );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%6.2f", (STS()->pMission->GetMECOInc() - el.i) * DEG );
		TextOut( hDC, 228, 202, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::AEPFD_dINC( oapi::Sketchpad2* skp )
	{
		skp->SetPen( skpLightGrayPen );
		skp->SetBrush( skpBlackBrush );
		skp->SetTextColor( CR_LIGHT_GRAY );
		oapi::IVECTOR2 tri[3] = {{205, 205}, {207, 210}, {203, 210}};
		skp->Polygon( tri, 3 );
		skp->SetPen( skpDarkGrayPen );
		skp->Text( 210, 203, "Inc", 3 );
		skp->Rectangle( 223, 201, 252, 213 );
		skp->SetTextColor( CR_WHITE );
		ELEMENTS el;
		STS()->GetElements( STS()->GetGravityRef(), el, NULL, 0, FRAME_EQU );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%6.2f", (STS()->pMission->GetMECOInc() - el.i) * DEG );
		skp->Text( 228, 202, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::AEPFD_TGTINC( HDC hDC )
	{
		SelectObject( hDC, gdiDarkGrayPen );
		SelectObject( hDC, gdiBlackBrush );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 196, 222, "Tgt Inc", 7 );
		Rectangle( hDC, 223, 220, 252, 232 );
		SetTextColor( hDC, CR_WHITE );
		double dtmp = 0;// TODO
		char cbuf[8];
		sprintf_s( cbuf, 8, "%5.2f", dtmp );
		TextOut( hDC, 226, 221, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::AEPFD_TGTINC( oapi::Sketchpad2* skp )
	{
		skp->SetPen( skpDarkGrayPen );
		skp->SetBrush( skpBlackBrush );
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 196, 222, "Tgt Inc", 7 );
		skp->Rectangle( 223, 220, 252, 232 );
		skp->SetTextColor( CR_WHITE );
		double dtmp = 0;// TODO
		char cbuf[8];
		sprintf_s( cbuf, 8, "%5.2f", dtmp );
		skp->Text( 226, 221, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::AEPFD_GSI( HDC hDC, double Altitude_ft )
	{
		SelectObject( hDC, gdiDarkGrayPen );
		SelectObject( hDC, gdiBlackBrush );
		Rectangle( hDC, 217, 165, 224, 235 );
		SelectObject( hDC, gdiDarkGrayBrush );
		::Ellipse( hDC, 219, 166, 222, 169 );
		::Ellipse( hDC, 219, 183, 222, 186 );
		::Ellipse( hDC, 219, 215, 222, 218 );
		::Ellipse( hDC, 219, 231, 222, 234 );
		MoveToEx( hDC, 217, 200, NULL );
		LineTo( hDC, 224, 200 );

		if (Altitude_ft < 1500)
		{
			TextOut( hDC, 216, 156, "1K", 2 );
			TextOut( hDC, 216, 234, "1K", 2 );
			// no tracking -> print GS label
			SelectObject( hDC, gdiRedPen );
			SelectObject( hDC, gdiRedBrush );
			Rectangle( hDC, 215, 196, 226, 204 );
			SetTextColor( hDC, CR_BLACK );
			TextOut( hDC, 216, 195, "GS", 2 );
		}
		else
		{
			POINT tri[3];
			tri[0].x = 220;// starts at tip moving cw
			tri[1].x = 228;
			tri[2].x = 228;
			double dtmp = GetIDP()->GetGlideSlopeDistance();// get dist ABOVE glide path

			if (GetIDP()->GetApproachAndLandState() == false)
			{
				// TAEM
				TextOut( hDC, 216, 156, "5K", 2 );
				TextOut( hDC, 216, 234, "5K", 2 );

				if (dtmp > 5000)
				{
					if (GetFlash() == false) return;
					tri[0].y = 235;
				}
				else if (dtmp < -5000)
				{
					if (GetFlash() == false) return;
					tri[0].y = 165;
				}
				else tri[0].y = 200 + Round( dtmp * 0.007 );
			}
			else
			{
				// A/L
				TextOut( hDC, 216, 156, "1K", 2 );
				TextOut( hDC, 216, 234, "1K", 2 );

				if (dtmp > 1000)
				{
					if (GetFlash() == false) return;
					tri[0].y = 235;
				}
				else if (dtmp < -1000)
				{
					if (GetFlash() == false) return;
					tri[0].y = 165;
				}
				else tri[0].y = 200 + Round( dtmp * 0.035 );
			}

			tri[1].y = tri[0].y - 5;
			tri[2].y = tri[0].y + 5;
			SelectObject( hDC, gdiLightGreenPen );
			SelectObject( hDC, gdiLightGreenBrush );
			Polygon( hDC, tri, 3 );
		}
		return;
	}

	void MDU::AEPFD_GSI( oapi::Sketchpad2* skp, double Altitude_ft )
	{
		skp->SetPen( skpDarkGrayPen );
		skp->SetBrush( skpBlackBrush );
		skp->Rectangle( 217, 165, 224, 235 );
		skp->SetBrush( skpDarkGrayBrush );
		skp->Ellipse(  219, 166, 222, 169 );
		skp->Ellipse( 219, 183, 222, 186 );
		skp->Ellipse( 219, 215, 222, 218 );
		skp->Ellipse( 219, 231, 222, 234 );
		skp->Line( 217, 200, 224, 200 );

		if (Altitude_ft < 1500)
		{
			skp->Text( 216, 156, "1K", 2 );
			skp->Text( 216, 234, "1K", 2 );
			// no tracking -> print GS label
			skp->SetPen( skpRedPen );
			skp->SetBrush( skpRedBrush );
			skp->Rectangle( 215, 196, 226, 204 );
			skp->SetTextColor( CR_BLACK );
			skp->Text( 216, 195, "GS", 2 );
		}
		else
		{
			oapi::IVECTOR2 tri[3];
			tri[0].x = 220;// starts at tip moving cw
			tri[1].x = 228;
			tri[2].x = 228;
			double dtmp = GetIDP()->GetGlideSlopeDistance();// get dist ABOVE glide path

			if (GetIDP()->GetApproachAndLandState() == false)
			{
				// TAEM
				skp->Text( 216, 156, "5K", 2 );
				skp->Text( 216, 234, "5K", 2 );

				if (dtmp > 5000)
				{
					if (GetFlash() == false) return;
					tri[0].y = 235;
				}
				else if (dtmp < -5000)
				{
					if (GetFlash() == false) return;
					tri[0].y = 165;
				}
				else tri[0].y = 200 + Round( dtmp * 0.007 );
			}
			else
			{
				// A/L
				skp->Text( 216, 156, "1K", 2 );
				skp->Text( 216, 234, "1K", 2 );

				if (dtmp > 1000)
				{
					if (GetFlash() == false) return;
					tri[0].y = 235;
				}
				else if (dtmp < -1000)
				{
					if (GetFlash() == false) return;
					tri[0].y = 165;
				}
				else tri[0].y = 200 + Round( dtmp * 0.035 );
			}

			tri[1].y = tri[0].y - 5;
			tri[2].y = tri[0].y + 5;
			skp->SetPen( skpLightGreenPen );
			skp->SetBrush( skpLightGreenBrush );
			skp->Polygon( tri, 3 );
		}
		return;
	}
};
