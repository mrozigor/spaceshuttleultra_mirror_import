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

		SelectObject( hDC, gdiSSUAFont_h11w9 );

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
					Rectangle( hDC, 22, 18, 132, 32 );// "hide" throttle
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
				AEPFD_Header_TransDAP( hDC, 104, adiatt );
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
				Tapes_Invalid( hDC );
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

		skp->SetFont( skpSSUAFont_h11 );
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
				Tape_Alpha( skp, 0 );
				Tape_H_Hdot( skp, Altitude_ft, vel.y );
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
					skp->Rectangle( 22, 18, 132, 32 );// "hide" throttle
				}
				Tape_MVI_KEAS( hDC, MachNumber );
				Tape_Alpha( skp, 0 );
				Tape_H_Hdot( skp, Altitude_ft, vel.y );
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
				AEPFD_Header_TransDAP( skp, 104, adiatt );
				Tape_MVI_KEAS( hDC, MachNumber );
				Tape_H_Hdot( skp, Altitude_ft, vel.y );
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
				Tape_Alpha( skp, MachNumber );
				Tape_H_Hdot( skp, Altitude_ft, vel.y );
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
				Tape_Alpha( skp, MachNumber );
				Tape_H_Hdot( skp, Altitude_ft, vel.y );
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
				Tape_Alpha( skp, 0 );
				Tape_H_Hdot( skp, Altitude_ft, vel.y );
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
				Tape_Alpha( skp, MachNumber );
				Tape_H_Hdot( skp, Altitude_ft, vel.y );
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
				Tape_Alpha( skp, MachNumber );
				Tape_H_Hdot( skp, Altitude_ft, vel.y );
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
				Tapes_Invalid( skp );
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
		// 20� in window (1� = 5.7px)
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
		hBM = CreateCompatibleBitmap( hDC_ADI, 222, 222 );
		SelectObject( hDC_ADI_ORBIT, hBM );
		SelectObject( hDC_ADI_ORBIT, gdiSSUBFont_h12w7 );

		hDC_ADIMASK_ORBIT = CreateCompatibleDC( hDC_ADI );
		hBM = CreateCompatibleBitmap( hDC_ADI, 222, 222 );
		SelectObject( hDC_ADIMASK_ORBIT, hBM );
		SelectObject( hDC_ADIMASK_ORBIT, gdiWhitePen );
		SelectObject( hDC_ADIMASK_ORBIT, gdiWhiteBrush );
		::Ellipse( hDC_ADIMASK_ORBIT, 1, 1, 221, 221 );
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
		SetTextColor( hDC, CR_LIGHT_GRAY );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%c", 254 );
		TextOut( hDC, 82, 39, cbuf, 1 );

		SelectObject( hDC, gdiWhitePen );
		SelectObject( hDC, gdiBlackBrush );
		Rectangle( hDC, 68, 54, 114, 274 );

		double alpha = STS()->GetAOA() * DEG;

		//BitBlt( hDC, 38, 40, 21, 114, hDC_Tapes, 45, 1026 - Round( alpha * 5.7 ), SRCCOPY );

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
				//Polygon( hDC, diamond, 4 );

				diamond[0].y += 3;
				diamond[1].x = 57;
				diamond[2].y -= 3;
				diamond[3].x = 55;
				SelectObject( hDC, gdiBlackBrush );
				//Polygon( hDC, diamond, 4 );
			}
		}

		SelectObject( hDC, gdiLightGrayPen );
		SelectObject( hDC, gdiBlackBrush );
		POINT poly[5] = {{111,163},{104,175},{64,175},{64,150},{104,150}};// start at tip moving cw
		Polygon( hDC, poly, 5 );
		
		SelectObject( hDC, gdiSSUBFont_h18w9 );
		SetTextColor( hDC, CR_WHITE );
		SetTextAlign( hDC, TA_RIGHT );
		sprintf_s( cbuf, 8, "%6.1f", alpha );
		TextOut( hDC, 108, 151, cbuf, strlen( cbuf ) );
		SetTextAlign( hDC, TA_LEFT );
		return;
	}
	
	void MDU::Tape_Alpha( oapi::Sketchpad2* skp, double MachNumber )
	{
		skp->SetTextColor( CR_LIGHT_GRAY );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%c", 254 );
		skp->Text( 82, 39, cbuf, 1 );

		skp->SetPen( skpWhitePen );
		skp->SetBrush( skpBlackBrush );
		skp->Rectangle( 68, 54, 114, 274 );

		double alpha = STS()->GetAOA() * DEG;

		//BitBlt( hDC, 38, 40, 21, 114, hDC_Tapes, 45, 1026 - Round( alpha * 5.7 ), SRCCOPY );

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
				oapi::IVECTOR2 diamond[4];
				diamond[0].x = 56;// start at top moving cw
				diamond[0].y = 94 - pos;
				diamond[1].x = 60;
				diamond[1].y = 98 - pos;
				diamond[2].x = 56;
				diamond[2].y = 102 - pos;
				diamond[3].x = 52;
				diamond[3].y = 98 - pos;
				skp->SetBrush( skpMagentaBrush );
				skp->SetPen( skpBlackPen );
				//skp->Polygon( diamond, 4 );

				diamond[0].y += 3;
				diamond[1].x = 57;
				diamond[2].y -= 3;
				diamond[3].x = 55;
				skp->SetBrush( skpBlackBrush );
				//skp->Polygon( diamond, 4 );
			}
		}

		skp->SetPen( skpLightGrayPen );
		skp->SetBrush( skpBlackBrush );
		oapi::IVECTOR2 poly[5] = {{111,163},{104,175},{64,175},{64,150},{104,150}};// start at tip moving cw
		skp->Polygon( poly, 5 );
		
		skp->SetFont( skpSSUBFont_h18 );
		skp->SetTextColor( CR_WHITE );
		skp->SetTextAlign( oapi::Sketchpad::RIGHT );
		sprintf_s( cbuf, 8, "%6.1f", alpha );
		skp->Text( 108, 151, cbuf, strlen( cbuf ) );
		skp->SetTextAlign( oapi::Sketchpad::LEFT );
		return;
	}

	void MDU::Tape_H_Hdot( HDC hDC, double Altitude_ft, double Hdot )
	{
		SelectObject( hDC, gdiSSUAFont_h11w9 );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 411, 39, "H", 1 );
		SelectObject( hDC, gdiWhitePen );
		Rectangle( hDC, 394, 54, 440, 274 );
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
		//BitBlt( hDC, 204, 40, 22, 114, hDC_Tapes, 68, pos, SRCCOPY );

		SelectObject( hDC, gdiLightGrayPen );
		SelectObject( hDC, gdiBlackBrush );
		Rectangle( hDC, 394, 150, 440, 175 );
		SelectObject( hDC, gdiSSUBFont_h18w9 );
		SetTextColor( hDC, CR_WHITE );
		SetTextAlign( hDC, TA_RIGHT );
		char cbuf[8];
		if (Altitude_ft < 10000)
		{
			sprintf_s( cbuf, 8, "%4d", (int)Altitude_ft );
			TextOut( hDC, 430, 151, cbuf, strlen( cbuf ) );
		}
		else if (Altitude_ft < 400000)
		{
			sprintf_s( cbuf, 8, "%3d", (int)Altitude_ft / 1000 );
			TextOut( hDC, 430, 151, cbuf, strlen( cbuf ) );
			SelectObject( hDC, gdiSSUBFont_h12w7 );
			SetTextColor( hDC, CR_LIGHT_GRAY );
			TextOut( hDC, 439, 155, "K", 1 );
		}
		else
		{
			sprintf_s( cbuf, 8, "%3.0f", Altitude_NM );
			TextOut( hDC, 430, 151, cbuf, strlen( cbuf ) );
			SelectObject( hDC, gdiSSUBFont_h12w7 );
			SetTextColor( hDC, CR_LIGHT_GRAY );
			TextOut( hDC, 439, 155, "M", 1 );
		}
		SetTextAlign( hDC, TA_LEFT );

		SelectObject( hDC, gdiSSUAFont_h11w9 );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		SetPixel( hDC, 481, 37, CR_LIGHT_GRAY );
		TextOut( hDC, 477, 39, "H", 1 );
		SelectObject( hDC, gdiWhitePen );
		Rectangle( hDC, 459, 54, 505, 274 );
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
		//BitBlt( hDC, 231, 40, 22, 114, hDC_Tapes, 91, pos, SRCCOPY );

		SelectObject( hDC, gdiLightGrayPen );
		SelectObject( hDC, gdiBlackBrush );
		Rectangle( hDC, 459, 150, 505, 175 );
		SelectObject( hDC, gdiSSUBFont_h18w9 );
		SetTextColor( hDC, CR_WHITE );
		SetTextAlign( hDC, TA_RIGHT );
		sprintf_s( cbuf, 8, "%4.0f", Hdot );
		TextOut( hDC, 503, 151, cbuf, strlen( cbuf ) );
		SetTextAlign( hDC, TA_LEFT );
		return;
	}

	void MDU::Tape_H_Hdot( oapi::Sketchpad2* skp, double Altitude_ft, double Hdot )
	{
		skp->SetFont( skpSSUAFont_h11 );
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 411, 39, "H", 1 );
		skp->SetPen( skpWhitePen );
		skp->Rectangle( 394, 54, 440, 274 );
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
		//BitBlt( hDC, 204, 40, 22, 114, hDC_Tapes, 68, pos, SRCCOPY );

		skp->SetPen( skpLightGrayPen );
		skp->SetBrush( skpBlackBrush );
		skp->Rectangle( 394, 150, 440, 175 );
		skp->SetFont( skpSSUBFont_h18 );
		skp->SetTextColor( CR_WHITE );
		skp->SetTextAlign( oapi::Sketchpad::RIGHT );
		char cbuf[8];
		if (Altitude_ft < 10000)
		{
			sprintf_s( cbuf, 8, "%4d", (int)Altitude_ft );
			skp->Text( 430, 151, cbuf, strlen( cbuf ) );
		}
		else if (Altitude_ft < 400000)
		{
			sprintf_s( cbuf, 8, "%3d", (int)Altitude_ft / 1000 );
			skp->Text( 430, 151, cbuf, strlen( cbuf ) );
			skp->SetFont( skpSSUBFont_h12 );
			skp->SetTextColor( CR_LIGHT_GRAY );
			skp->Text( 439, 155, "K", 1 );
		}
		else
		{
			sprintf_s( cbuf, 8, "%3.0f", Altitude_NM );
			skp->Text( 430, 151, cbuf, strlen( cbuf ) );
			skp->SetFont( skpSSUBFont_h12 );
			skp->SetTextColor( CR_LIGHT_GRAY );
			skp->Text( 439, 155, "M", 1 );
		}
		skp->SetTextAlign( oapi::Sketchpad::LEFT );

		skp->SetFont( skpSSUAFont_h11 );
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Pixel( 481, 37, CR_LIGHT_GRAY );
		skp->Text( 477, 39, "H", 1 );
		skp->SetPen( skpWhitePen );
		skp->Rectangle( 459, 54, 505, 274 );
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
		//BitBlt( hDC, 231, 40, 22, 114, hDC_Tapes, 91, pos, SRCCOPY );

		skp->SetPen( skpLightGrayPen );
		skp->SetBrush( skpBlackBrush );
		skp->Rectangle( 459, 150, 505, 175 );
		skp->SetFont( skpSSUBFont_h18 );
		skp->SetTextColor( CR_WHITE );
		skp->SetTextAlign( oapi::Sketchpad::RIGHT );
		sprintf_s( cbuf, 8, "%4.0f", Hdot );
		skp->Text( 503, 151, cbuf, strlen( cbuf ) );
		skp->SetTextAlign( oapi::Sketchpad::LEFT );
		return;
	}

	void MDU::Tapes_Invalid( HDC hDC )
	{
		SelectObject( hDC, gdiRedPen );
		SelectObject( hDC, gdiBlackBrush );
		Rectangle( hDC, 13, 54, 59, 274 );
		Rectangle( hDC, 13, 150, 59, 175 );
		Rectangle( hDC, 13, 285, 59, 309 );

		Rectangle( hDC, 68, 54, 114, 274 );
		POINT poly[5] = {{111,163},{104,175},{64,175},{64,150},{104,150}};// start at tip moving cw
		Polygon( hDC, poly, 5 );

		Rectangle( hDC, 394, 54, 440, 274 );
		Rectangle( hDC, 394, 150, 440, 175 );

		Rectangle( hDC, 459, 54, 505, 274 );
		Rectangle( hDC, 459, 150, 505, 175 );

		SelectObject( hDC, gdiSSUAFont_h11w9 );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 17, 39, "M/V", 3 );
		TextOut( hDC, 17, 315, "KEAS", 4 );

		char alpha[4];
		sprintf_s( alpha, 4, "%c", 254 );
		TextOut( hDC, 82, 39, alpha, 1 );

		TextOut( hDC, 411, 39, "H", 1 );
		SetPixel( hDC, 481, 37, CR_LIGHT_GRAY );
		TextOut( hDC, 477, 39, "H", 1 );
		return;
	}

	void MDU::Tapes_Invalid( oapi::Sketchpad2* skp )
	{
		skp->SetFont( skpSSUAFont_h11 );
		skp->SetPen( skpRedPen );
		skp->SetBrush( skpBlackBrush );
		skp->Rectangle( 13, 55, 59, 269 );
		skp->Rectangle( 13, 151, 59, 176 );
		skp->Rectangle( 13, 279, 59, 304 );

		skp->Rectangle( 68, 55, 114, 269 );
		oapi::IVECTOR2 poly[5] = {{111,163},{104,176},{64,176},{64,151},{104,151}};// start at tip moving cw
		skp->Polygon( poly, 5 );

		skp->Rectangle( 394, 55, 440, 269 );
		skp->Rectangle( 394, 151, 440, 176 );

		skp->Rectangle( 459, 55, 505, 269 );
		skp->Rectangle( 459, 151, 505, 176 );

		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 17, 40, "M/V", 3 );
		skp->Text( 17, 310, "KEAS", 4 );

		char alpha[4];
		sprintf_s( alpha, 4, "%c", 254 );
		skp->Text( 82, 40, alpha, 1 );

		skp->Text( 411, 40, "H", 1 );
		skp->Pixel( 481, 37, CR_LIGHT_GRAY );
		skp->Text( 477, 40, "H", 1 );
		return;
	}

	void MDU::ADI_STATIC( HDC hDC )
	{
		SelectObject( hDC, gdiSSUBFont_h12w7 );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 174, 25, "R", 1 );
		TextOut( hDC, 174, 32, "P", 1 );
		TextOut( hDC, 174, 39, "Y", 1 );

		SelectObject( hDC, gdiWhitePen );
		::Ellipse( hDC, 122, 48, 356, 282 );
		
		// rate scales
		// top scale
		SelectObject( hDC, gdiLightGrayPen );
		MoveToEx( hDC, 154, 34, NULL );
		LineTo( hDC, 324, 34 );
		MoveToEx( hDC, 154, 33, NULL );
		LineTo( hDC, 154, 44 );
		MoveToEx( hDC, 171, 33, NULL );
		LineTo( hDC, 171, 41 );
		MoveToEx( hDC, 188, 33, NULL );
		LineTo( hDC, 188, 41 );
		MoveToEx( hDC, 205, 33, NULL );
		LineTo( hDC, 205, 41 );
		MoveToEx( hDC, 222, 33, NULL );
		LineTo( hDC, 222, 41 );
		MoveToEx( hDC, 239, 33, NULL );
		LineTo( hDC, 239, 44 );
		MoveToEx( hDC, 256, 33, NULL );
		LineTo( hDC, 256, 41 );
		MoveToEx( hDC, 273, 33, NULL );
		LineTo( hDC, 273, 41 );
		MoveToEx( hDC, 290, 33, NULL );
		LineTo( hDC, 290, 41 );
		MoveToEx( hDC, 307, 33, NULL );
		LineTo( hDC, 307, 41 );
		MoveToEx( hDC, 324, 33, NULL );
		LineTo( hDC, 324, 44 );

		// side scale
		MoveToEx( hDC, 370, 80, NULL );
		LineTo( hDC, 370, 250 );
		MoveToEx( hDC, 370, 80, NULL );
		LineTo( hDC, 359, 80 );
		MoveToEx( hDC, 370, 97, NULL );
		LineTo( hDC, 362, 97 );
		MoveToEx( hDC, 370, 114, NULL );
		LineTo( hDC, 362, 114 );
		MoveToEx( hDC, 370, 131, NULL );
		LineTo( hDC, 362, 131 );
		MoveToEx( hDC, 370, 148, NULL );
		LineTo( hDC, 362, 148 );
		MoveToEx( hDC, 370, 165, NULL );
		LineTo( hDC, 359, 165 );
		MoveToEx( hDC, 370, 182, NULL );
		LineTo( hDC, 362, 182 );
		MoveToEx( hDC, 370, 199, NULL );
		LineTo( hDC, 362, 199 );
		MoveToEx( hDC, 370, 216, NULL );
		LineTo( hDC, 362, 216 );
		MoveToEx( hDC, 370, 233, NULL );
		LineTo( hDC, 362, 233 );
		MoveToEx( hDC, 370, 250, NULL );
		LineTo( hDC, 359, 250 );

		// bottom scale
		MoveToEx( hDC, 154, 296, NULL );
		LineTo( hDC, 324, 296 );
		MoveToEx( hDC, 154, 296, NULL );
		LineTo( hDC, 154, 285 );
		MoveToEx( hDC, 171, 296, NULL );
		LineTo( hDC, 171, 288 );
		MoveToEx( hDC, 188, 296, NULL );
		LineTo( hDC, 188, 288 );
		MoveToEx( hDC, 205, 296, NULL );
		LineTo( hDC, 205, 288 );
		MoveToEx( hDC, 222, 296, NULL );
		LineTo( hDC, 222, 288 );
		MoveToEx( hDC, 239, 296, NULL );
		LineTo( hDC, 239, 285 );
		MoveToEx( hDC, 256, 296, NULL );
		LineTo( hDC, 256, 288 );
		MoveToEx( hDC, 273, 296, NULL );
		LineTo( hDC, 273, 288 );
		MoveToEx( hDC, 290, 296, NULL );
		LineTo( hDC, 290, 288 );
		MoveToEx( hDC, 307, 296, NULL );
		LineTo( hDC, 307, 288 );
		MoveToEx( hDC, 324, 296, NULL );
		LineTo( hDC, 324, 285 );

		// roll lines
		// 10�/190�
		MoveToEx( hDC, 113, 43, NULL );
		LineTo( hDC, 131, 145 );
		// 20�/200�
		MoveToEx( hDC, 104, 45, NULL );
		LineTo( hDC, 140, 143 );
		// 40�/220�
		MoveToEx( hDC, 89, 54, NULL );
		LineTo( hDC, 155, 134 );
		// 50�/230�
		MoveToEx( hDC, 82, 61, NULL );
		LineTo( hDC, 162, 127 );
		// 70�/250�
		MoveToEx( hDC, 73, 76, NULL );
		LineTo( hDC, 171, 112 );
		// 80�/260�
		MoveToEx( hDC, 71, 85, NULL );
		LineTo( hDC, 173, 103 );
		// 100�/280�
		MoveToEx( hDC, 71, 103, NULL );
		LineTo( hDC, 173, 85 );
		// 110�/290�
		MoveToEx( hDC, 73, 112, NULL );
		LineTo( hDC, 171, 76 );
		// 130�/310�
		MoveToEx( hDC, 82, 127, NULL );
		LineTo( hDC, 162, 61 );
		// 140�/320�
		MoveToEx( hDC, 89, 134, NULL );
		LineTo( hDC, 155, 54 );
		// 160�/340�
		MoveToEx( hDC, 104, 143, NULL );
		LineTo( hDC, 140, 45 );
		// 170�/350�
		MoveToEx( hDC, 113, 145, NULL );
		LineTo( hDC, 131, 43 );
		SelectObject( hDC, gdiLightGrayThickPen );
		// 30�/210�
		MoveToEx( hDC, 96, 49, NULL );
		LineTo( hDC, 148, 139 );
		// 60�/240�
		MoveToEx( hDC, 77, 68, NULL );
		LineTo( hDC, 167, 120 );
		// 120�/300�
		MoveToEx( hDC, 77, 120, NULL );
		LineTo( hDC, 167, 68 );
		// 150�/330�
		MoveToEx( hDC, 96, 139, NULL );
		LineTo( hDC, 148, 49 );
		// cover center for ADI SRCPAINT
		SelectObject( hDC, gdiBlackBrush );
		SelectObject( hDC, gdiBlackPen );
		::Ellipse( hDC, 73, 45, 171, 143 );

		TextOut( hDC, 149, 48, "33", 2 );
		TextOut( hDC, 162, 61, "30", 2 );
		TextOut( hDC, 162, 120, "24", 2 );
		TextOut( hDC, 149, 134, "21", 2 );
		TextOut( hDC, 89, 134, "15", 2 );
		TextOut( hDC, 77, 121, "12", 2 );
		TextOut( hDC, 76, 61, "06", 2 );
		TextOut( hDC, 89, 47, "03", 2 );

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
		skp->SetFont( skpSSUBFont_h12 );
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 174, 25, "R", 1 );
		skp->Text( 174, 32, "P", 1 );
		skp->Text( 174, 39, "Y", 1 );

		skp->SetPen( skpWhitePen );
		skp->Ellipse( 122, 48, 356, 282 );

		// rate scales
		// top scale
		skp->SetPen( skpLightGrayPen );
		skp->Line( 154, 34, 324, 34 );
		skp->Line( 154, 33, 154, 44 );
		skp->Line( 171, 33, 171, 41 );
		skp->Line( 188, 33, 188, 41 );
		skp->Line( 205, 33, 205, 41 );
		skp->Line( 222, 33, 222, 41 );
		skp->Line( 239, 33, 239, 44 );
		skp->Line( 256, 33, 256, 41 );
		skp->Line( 273, 33, 273, 41 );
		skp->Line( 290, 33, 290, 41 );
		skp->Line( 307, 33, 307, 41 );
		skp->Line( 324, 33, 324, 44 );

		// side scale
		skp->Line( 370, 80, 370, 250 );
		skp->Line( 370, 80, 359, 80 );
		skp->Line( 370, 97, 362, 97 );
		skp->Line( 370, 114, 362, 114 );
		skp->Line( 370, 131, 362, 131 );
		skp->Line( 370, 148, 362, 148 );
		skp->Line( 370, 165, 359, 165 );
		skp->Line( 370, 182, 362, 182 );
		skp->Line( 370, 199, 362, 199 );
		skp->Line( 370, 216, 362, 216 );
		skp->Line( 370, 233, 362, 233 );
		skp->Line( 370, 250, 359, 250 );

		// bottom scale
		skp->Line( 154, 296, 324, 296 );
		skp->Line( 154, 296, 154, 285 );
		skp->Line( 171, 296, 171, 288 );
		skp->Line( 188, 296, 188, 288 );
		skp->Line( 205, 296, 205, 288 );
		skp->Line( 222, 296, 222, 288 );
		skp->Line( 239, 296, 239, 285 );
		skp->Line( 256, 296, 256, 288 );
		skp->Line( 273, 296, 273, 288 );
		skp->Line( 290, 296, 290, 288 );
		skp->Line( 307, 296, 307, 288 );
		skp->Line( 324, 296, 324, 285 );

		// roll lines
		// 10�/190�
		skp->Line( 113, 43, 131, 145 );
		// 20�/200�
		skp->Line( 104, 45, 140, 143 );
		// 40�/220�
		skp->Line( 89, 54, 155, 134 );
		// 50�/230�
		skp->Line( 82, 61, 162, 127 );
		// 70�/250�
		skp->Line( 73, 76, 171, 112 );
		// 80�/260�
		skp->Line( 71, 85, 173, 103 );
		// 100�/280�
		skp->Line( 71, 103, 173, 85 );
		// 110�/290�
		skp->Line( 73, 112, 171, 76 );
		// 130�/310�
		skp->Line( 82, 127, 162, 61 );
		// 140�/320�
		skp->Line( 89, 134, 155, 54 );
		// 160�/340�
		skp->Line( 104, 143, 140, 45 );
		// 170�/350�
		skp->Line( 113, 145, 131, 43 );
		skp->SetPen( skpLightGrayThickPen );
		// 30�/210�
		skp->Line( 96, 49, 148, 139 );
		// 60�/240�
		skp->Line( 77, 68, 167, 120 );
		// 120�/300�
		skp->Line( 77, 120, 167, 68 );
		// 150�/330�
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
		// 90� pitch/yaw "FOV"

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

		// 0� pitch line/labels
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

		// pitch lines/labels for +30�/+60�/+90�/+120�/+150�
		SelectObject( hDC_ADI, gdiDarkGrayPen );
		// +30�
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
		// +60�
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
		// +90�
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
		// +120�
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
		// +150�
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

		// pitch lines/labels for -30�/-60�/-90�/-120�/-150�
		SelectObject( hDC_ADI, gdiWhitePen );
		SetTextColor( hDC_ADI, CR_WHITE );
		SetBkColor( hDC_ADI, CR_LIGHT_GRAY );
		// -30�
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
		// -60�
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
		// -90�
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
		// -120�
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
		// -150�
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
		// yaw line 30� (above horizon)
		MoveToEx( hDC_ADI, 85, 0, NULL );
		LineTo( hDC_ADI, 85, 50 + Round( 60.012096 * sinpitch ) );
		// yaw line 330� (above horizon)
		MoveToEx( hDC_ADI, 15, 0, NULL );
		LineTo( hDC_ADI, 15, 50 + Round( 60.012096 * sinpitch ) );
		SelectObject( hDC_ADI, gdiWhitePen );
		// yaw line 30� (below horizon)
		MoveToEx( hDC_ADI, 85, 100, NULL );
		LineTo( hDC_ADI, 85, 50 + Round( 60.012096 * sinpitch ) );
		// yaw line 330� (below horizon)
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
		// 90� pitch/yaw "FOV"

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

		SelectObject( hDC, gdiSSUBFont_h12w7 );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 236, 18, "0", 1 );
		TextOut( hDC, 375, 159, "0", 1 );
		TextOut( hDC, 236, 298, "0", 1 );

		SetTextColor( hDC, CR_WHITE );
		SetTextAlign( hDC, TA_RIGHT );
		if (adirate == 1)
		{
			// ADI RATE MED
			TextOut( hDC, 151, 31, "5", 1 );
			TextOut( hDC, 151, 283, "5", 1 );

			SetTextAlign( hDC, TA_LEFT );
			//TextOut( hDC, 151, 31, "5", 1 );
			TextOut( hDC, 327, 31, "5", 1 );

			TextOut( hDC, 356, 65, "5", 1 );
			TextOut( hDC, 356, 251, "5", 1 );

			//TextOut( hDC, 151, 283, "5", 1 );
			TextOut( hDC, 327, 283, "5", 1 );

			if (roll > 5) pos_roll = 324;
			else if (roll < -5) pos_roll = 154;
			else pos_roll = 239 + Round( roll * 17 );

			if (pitch > 5) pos_pitch = 250;
			else if (pitch < -5) pos_pitch = 80;
			else pos_pitch = 165 + Round( pitch * 17 );

			if (yaw > 5) pos_yaw = 324;
			else if (yaw < -5) pos_yaw = 154;
			else pos_yaw = 239 + Round( yaw * 17 );
		}
		else if (adirate == 2)
		{
			// ADI RATE HIGH
			TextOut( hDC, 151, 31, "10", 2 );
			TextOut( hDC, 151, 283, "10", 2 );

			SetTextAlign( hDC, TA_LEFT );
			//TextOut( hDC, 151, 31, "10", 2 );
			TextOut( hDC, 327, 31, "10", 2 );

			TextOut( hDC, 356, 65, "10", 2 );
			TextOut( hDC, 356, 251, "10", 2 );

			//TextOut( hDC, 151, 283, "10", 2 );
			TextOut( hDC, 327, 283, "10", 2 );

			if (roll > 10) pos_roll = 324;
			else if (roll < -10) pos_roll = 154;
			else pos_roll = 239 + Round( roll * 8.5 );

			if (pitch > 10) pos_pitch = 250;
			else if (pitch < -10) pos_pitch = 80;
			else pos_pitch = 165 + Round( pitch * 8.5 );

			if (yaw > 10) pos_yaw = 324;
			else if (yaw < -10) pos_yaw = 154;
			else pos_yaw = 239 + Round( yaw * 8.5 );
		}
		else
		{
			// ADI RATE LOW
			TextOut( hDC, 151, 31, "1", 1 );
			TextOut( hDC, 151, 283, "1", 1 );

			SetTextAlign( hDC, TA_LEFT );
			//TextOut( hDC, 151, 31, "1", 1 );
			TextOut( hDC, 327, 31, "1", 1 );

			TextOut( hDC, 356, 65, "1", 1 );
			TextOut( hDC, 356, 251, "1", 1 );

			//TextOut( hDC, 151, 283, "1", 1 );
			TextOut( hDC, 327, 283, "1", 1 );

			if (roll > 1) pos_roll = 324;
			else if (roll < -1) pos_roll = 154;
			else pos_roll = 239 + Round( roll * 85 );

			if (pitch > 1) pos_pitch = 250;
			else if (pitch < -1) pos_pitch = 80;
			else pos_pitch = 165 + Round( pitch * 85 );

			if (yaw > 1) pos_yaw = 324;
			else if (yaw < -1) pos_yaw = 154;
			else pos_yaw = 239 + Round( yaw * 85 );
		}

		// draw triangles
		SelectObject( hDC, gdiLightGreenPen );
		SelectObject( hDC, gdiLightGreenBrush );
		POINT tri[3];
		tri[0].x = pos_roll;
		tri[0].y = 34;
		tri[1].x = tri[0].x + 10;
		tri[1].y = 18;
		tri[2].x = tri[0].x - 10;
		tri[2].y = 18;
		Polygon( hDC, tri, 3 );

		tri[0].x = 370;
		tri[0].y = pos_pitch;
		tri[1].x = 386;
		tri[1].y = tri[0].y - 10;
		tri[2].x = 386;
		tri[2].y = tri[0].y + 10;
		Polygon( hDC, tri, 3 );

		tri[0].x = pos_yaw;
		tri[0].y = 296;
		tri[1].x = tri[0].x - 10;
		tri[1].y = 312;
		tri[2].x = tri[0].x + 10;
		tri[2].y = 312;
		Polygon( hDC, tri, 3 );
		return;
	}

	void MDU::ADI_RATE_A( oapi::Sketchpad2* skp, double pitch, double roll, double yaw, int adirate )
	{
		int pos_roll;
		int pos_pitch;
		int pos_yaw;

		skp->SetFont( skpSSUBFont_h12 );
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 236, 18, "0", 1 );
		skp->Text( 375, 159, "0", 1 );
		skp->Text( 236, 298, "0", 1 );

		skp->SetTextColor( CR_WHITE );
		skp->SetTextAlign( oapi::Sketchpad::RIGHT );
		if (adirate == 1)
		{
			// ADI RATE MED
			skp->Text( 151, 31, "5", 1 );
			skp->Text( 151, 283, "5", 1 );
			
			skp->SetTextAlign( oapi::Sketchpad::LEFT );
			//skp->Text( 151, 31, "5", 1 );
			skp->Text( 327, 31, "5", 1 );

			skp->Text( 356, 65, "5", 1 );
			skp->Text( 356, 251, "5", 1 );

			//skp->Text( 151, 283, "5", 1 );
			skp->Text( 327, 283, "5", 1 );

			if (roll > 5) pos_roll = 324;
			else if (roll < -5) pos_roll = 154;
			else pos_roll = 239 + Round( roll * 17 );

			if (pitch > 5) pos_pitch = 250;
			else if (pitch < -5) pos_pitch = 80;
			else pos_pitch = 165 + Round( pitch * 17 );

			if (yaw > 5) pos_yaw = 324;
			else if (yaw < -5) pos_yaw = 154;
			else pos_yaw = 239 + Round( yaw * 17 );
		}
		else if (adirate == 2)
		{
			// ADI RATE HIGH
			skp->Text( 151, 31, "10", 2 );
			skp->Text( 151, 283, "10", 2 );

			skp->SetTextAlign( oapi::Sketchpad::LEFT );
			//skp->Text( 151, 31, "10", 2 );
			skp->Text( 327, 31, "10", 2 );

			skp->Text( 356, 65, "10", 2 );
			skp->Text( 356, 251, "10", 2 );

			//skp->Text( 151, 283, "10", 2 );
			skp->Text( 327, 283, "10", 2 );

			if (roll > 10) pos_roll = 324;
			else if (roll < -10) pos_roll = 154;
			else pos_roll = 239 + Round( roll * 8.5 );

			if (pitch > 10) pos_pitch = 250;
			else if (pitch < -10) pos_pitch = 80;
			else pos_pitch = 165 + Round( pitch * 8.5 );

			if (yaw > 10) pos_yaw = 324;
			else if (yaw < -10) pos_yaw = 154;
			else pos_yaw = 239 + Round( yaw * 8.5 );
		}
		else
		{
			// ADI RATE LOW
			skp->Text( 151, 31, "1", 1 );
			skp->Text( 151, 283, "1", 1 );

			skp->SetTextAlign( oapi::Sketchpad::LEFT );
			//skp->Text( 151, 31, "1", 1 );
			skp->Text( 327, 31, "1", 1 );

			skp->Text( 356, 65, "1", 1 );
			skp->Text( 356, 251, "1", 1 );

			//skp->Text( 151, 283, "1", 1 );
			skp->Text( 327, 283, "1", 1 );

			if (roll > 1) pos_roll = 324;
			else if (roll < -1) pos_roll = 154;
			else pos_roll = 239 + Round( roll * 85 );

			if (pitch > 1) pos_pitch = 250;
			else if (pitch < -1) pos_pitch = 80;
			else pos_pitch = 165 + Round( pitch * 85 );

			if (yaw > 1) pos_yaw = 324;
			else if (yaw < -1) pos_yaw = 154;
			else pos_yaw = 239 + Round( yaw * 85 );
		}

		// draw triangles
		skp->SetPen( skpLightGreenPen );
		skp->SetBrush( skpLightGreenBrush );
		oapi::IVECTOR2 tri[3];
		tri[0].x = pos_roll;
		tri[0].y = 34;
		tri[1].x = tri[0].x + 10;
		tri[1].y = 18;
		tri[2].x = tri[0].x - 10;
		tri[2].y = 18;
		skp->Polygon( tri, 3 );

		tri[0].x = 370;
		tri[0].y = pos_pitch;
		tri[1].x = 386;
		tri[1].y = tri[0].y - 10;
		tri[2].x = 386;
		tri[2].y = tri[0].y + 10;
		skp->Polygon( tri, 3 );

		tri[0].x = pos_yaw;
		tri[0].y = 296;
		tri[1].x = tri[0].x - 10;
		tri[1].y = 312;
		tri[2].x = tri[0].x + 10;
		tri[2].y = 312;
		skp->Polygon( tri, 3 );
		return;
	}

	void MDU::ADI_RATE_B( HDC hDC, double pitch, double roll, double yaw, int adirate, double Altitude_ft )
	{
		int pos_roll;
		int pos_pitch;
		int pos_yaw;

		SelectObject( hDC, gdiSSUBFont_h12w7 );
		SetTextColor( hDC, CR_WHITE );
		SetTextAlign( hDC, TA_RIGHT );
		if (adirate == 1)
		{
			// ADI RATE MED
			if (Altitude_ft < 6400)// HACK should use a 3sec timer starting at 7Kft, which expires about 6.4Kft 
			{
				// <7k + 3s
				TextOut( hDC, 151, 31, "5", 1 );
				TextOut( hDC, 151, 283, "5", 1 );

				SetTextAlign( hDC, TA_LEFT );
				//TextOut( hDC, 151, 31, "5", 1 );
				TextOut( hDC, 327, 31, "5", 1 );

				TextOut( hDC, 356, 65, "5", 1 );
				TextOut( hDC, 356, 251, "5", 1 );

				//TextOut( hDC, 151, 283, "5", 1 );
				TextOut( hDC, 327, 283, "5", 1 );

				SetTextColor( hDC, CR_LIGHT_GRAY );
				TextOut( hDC, 236, 18, "0", 1 );
				TextOut( hDC, 375, 159, "0", 1 );
				TextOut( hDC, 236, 298, "0", 1 );

				if (roll > 5) pos_roll = 324;
				else if (roll < -5) pos_roll = 154;
				else pos_roll = 239 + Round( roll * 17 );

				if (pitch > 5) pos_pitch = 250;
				else if (pitch < -5) pos_pitch = 80;
				else pos_pitch = 165 + Round( pitch * 17 );

				if (yaw > 5) pos_yaw = 324;
				else if (yaw < -5) pos_yaw = 154;
				else pos_yaw = 239 + Round( yaw * 17 );
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
				TextOut( hDC, 151, 283, "1K", 2 );

				SetTextAlign( hDC, TA_LEFT );
				TextOut( hDC, 356, 65, "1K", 2 );
				TextOut( hDC, 356, 251, "1K", 2 );

				//TextOut( hDC, 151, 283, "1K", 2 );
				TextOut( hDC, 327, 283, "1K", 2 );

				SetTextColor( hDC, CR_LIGHT_GRAY );
				TextOut( hDC, 375, 159, "0", 1 );
				TextOut( hDC, 236, 298, "0", 1 );

				pos_roll = 0;
				pitch = GetIDP()->GetGlideSlopeDistance();
				yaw = -GetIDP()->GetYRunwayPositionError();

				if (pitch > 1000) pos_pitch = 250;
				else if (pitch < -1000) pos_pitch = 80;
				else pos_pitch = 165 + Round( pitch * 0.085 );

				if (yaw > 1000) pos_yaw = 324;
				else if (yaw < -1000) pos_yaw = 154;
				else pos_yaw = 239 + Round( yaw * 0.085 );
			}
			else if (GetIDP()->GetOnHACState() == true)
			{
				// HAC
				// blank
				// alt err +/-5k
				// HAC x-range err +/-5k
				TextOut( hDC, 151, 283, "5K", 2 );

				SetTextAlign( hDC, TA_LEFT );
				TextOut( hDC, 356, 65, "5K", 2 );
				TextOut( hDC, 356, 251, "5K", 2 );

				//TextOut( hDC, 151, 283, "5K", 2 );
				TextOut( hDC, 327, 283, "5K", 2 );

				SetTextColor( hDC, CR_LIGHT_GRAY );
				TextOut( hDC, 375, 159, "0", 1 );
				TextOut( hDC, 236, 298, "0", 1 );

				pos_roll = 0;
				pitch = 0;// TODO get alt error (ft)
				yaw = GetIDP()->GetHACRadialError();

				if (pitch > 5000) pos_pitch = 250;
				else if (pitch < -5000) pos_pitch = 80;
				else pos_pitch = 165 + Round( pitch * 0.017 );

				if (yaw > 5000) pos_yaw = 324;
				else if (yaw < -5000) pos_yaw = 154;
				else pos_yaw = 239 + Round( yaw * 0.017 );
			}
			else
			{
				// ACQ
				// time to HAC 10s
				// alt err +/-5k
				// heading err +/-5�
				// (roll scale output is below)
				TextOut( hDC, 151, 283, "5", 1 );

				SetTextAlign( hDC, TA_LEFT );
				TextOut( hDC, 356, 65, "5K", 2 );
				TextOut( hDC, 356, 251, "5K", 2 );

				//TextOut( hDC, 151, 283, "5", 1 );
				TextOut( hDC, 327, 283, "5", 1 );

				SetTextColor( hDC, CR_LIGHT_GRAY );
				TextOut( hDC, 375, 159, "0", 1 );
				TextOut( hDC, 236, 298, "0", 1 );

				roll = ceil( GetIDP()->GetTimeToHAC() );
				pitch = 0;// TODO get alt error (ft)
				yaw = 0;// TODO get heading error (deg)

				SetTextColor( hDC, CR_WHITE );
				if (roll > 0)
				{
					SetTextAlign( hDC, TA_RIGHT );
					TextOut( hDC, 151, 31, "10s", 3 );
					SetTextAlign( hDC, TA_LEFT );
					TextOut( hDC, 327, 31, "0", 1 );
					if (roll >= 10) pos_roll = 154;
					else pos_roll = 324 - Round( roll * 17 );
				}
				else if (roll < 0)
				{
					SetTextAlign( hDC, TA_RIGHT );
					TextOut( hDC, 151, 31, "0", 1 );
					SetTextAlign( hDC, TA_LEFT );
					TextOut( hDC, 327, 31, "10s", 3 );
					if (roll <= -10) pos_roll = 324;
					else pos_roll = 154 - Round( roll * 17 );
				}
				else pos_roll = 0;

				if (pitch > 5000) pos_pitch = 250;
				else if (pitch < -5000) pos_pitch = 80;
				else pos_pitch = 165 + Round( pitch * 0.017 );

				if (yaw > 5) pos_yaw = 324;
				else if (yaw < -5) pos_yaw = 154;
				else pos_yaw = 239 + Round( yaw * 17 );
			}
		}
		else
		{
			// ADI RATE HIGH/LOW
			TextOut( hDC, 151, 31, "5", 1 );
			TextOut( hDC, 151, 283, "5", 1 );

			SetTextAlign( hDC, TA_LEFT );
			//TextOut( hDC, 151, 31, "5", 1 );
			TextOut( hDC, 327, 31, "5", 1 );

			TextOut( hDC, 356, 65, "5", 1 );
			TextOut( hDC, 356, 251, "5", 1 );

			//TextOut( hDC, 151, 283, "5", 1 );
			TextOut( hDC, 327, 283, "5", 1 );

			SetTextColor( hDC, CR_LIGHT_GRAY );
			TextOut( hDC, 236, 18, "0", 1 );
			TextOut( hDC, 375, 159, "0", 1 );
			TextOut( hDC, 236, 298, "0", 1 );

			if (roll > 5) pos_roll = 324;
			else if (roll < -5) pos_roll = 154;
			else pos_roll = 239 + Round( roll * 17 );

			if (pitch > 5) pos_pitch = 250;
			else if (pitch < -5) pos_pitch = 80;
			else pos_pitch = 165 + Round( pitch * 17 );

			if (yaw > 5) pos_yaw = 324;
			else if (yaw < -5) pos_yaw = 154;
			else pos_yaw = 239 + Round( yaw * 17 );
		}
		
		// draw triangles
		SelectObject( hDC, gdiLightGreenPen );
		SelectObject( hDC, gdiLightGreenBrush );
		POINT tri[3];
		if (pos_roll != 0)
		{
			tri[0].x = pos_roll;
			tri[0].y = 34;
			tri[1].x = tri[0].x + 10;
			tri[1].y = 18;
			tri[2].x = tri[0].x - 10;
			tri[2].y = 18;
			Polygon( hDC, tri, 3 );
		}
		if (pos_pitch != 0)
		{
			tri[0].x = 370;
			tri[0].y = pos_pitch;
			tri[1].x = 386;
			tri[1].y = tri[0].y - 10;
			tri[2].x = 386;
			tri[2].y = tri[0].y + 10;
			Polygon( hDC, tri, 3 );
		}
		if (pos_yaw != 0)
		{
			tri[0].x = pos_yaw;
			tri[0].y = 296;
			tri[1].x = tri[0].x - 10;
			tri[1].y = 312;
			tri[2].x = tri[0].x + 10;
			tri[2].y = 312;
			Polygon( hDC, tri, 3 );
		}
		return;
	}

	void MDU::ADI_RATE_B( oapi::Sketchpad2* skp, double pitch, double roll, double yaw, int adirate, double Altitude_ft )
	{
		int pos_roll;
		int pos_pitch;
		int pos_yaw;

		skp->SetFont( skpSSUBFont_h12 );
		skp->SetTextColor( CR_WHITE );
		skp->SetTextAlign( oapi::Sketchpad::RIGHT );
		if (adirate == 1)
		{
			// ADI RATE MED
			if (Altitude_ft < 6400)// HACK should use a 3sec timer starting at 7Kft, which expires about 6.4Kft 
			{
				// <7k + 3s
				skp->Text( 151, 31, "5", 1 );
				skp->Text( 151, 283, "5", 1 );

				skp->SetTextAlign( oapi::Sketchpad::LEFT );
				//skp->Text( 151, 31, "5", 1 );
				skp->Text( 327, 31, "5", 1 );

				skp->Text( 356, 65, "5", 1 );
				skp->Text( 356, 251, "5", 1 );

				//skp->Text( 151, 283, "5", 1 );
				skp->Text( 327, 283, "5", 1 );

				skp->SetTextColor( CR_LIGHT_GRAY );
				skp->Text( 236, 18, "0", 1 );
				skp->Text( 375, 159, "0", 1 );
				skp->Text( 236, 298, "0", 1 );

				if (roll > 5) pos_roll = 324;
				else if (roll < -5) pos_roll = 154;
				else pos_roll = 239 + Round( roll * 17 );

				if (pitch > 5) pos_pitch = 250;
				else if (pitch < -5) pos_pitch = 80;
				else pos_pitch = 165 + Round( pitch * 17 );

				if (yaw > 5) pos_yaw = 324;
				else if (yaw < -5) pos_yaw = 154;
				else pos_yaw = 239 + Round( yaw * 17 );
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
				skp->Text( 151, 283, "1K", 2 );

				skp->SetTextAlign( oapi::Sketchpad::LEFT );
				skp->Text( 356, 65, "1K", 2 );
				skp->Text( 356, 251, "1K", 2 );

				//skp->Text( 151, 283, "1K", 2 );
				skp->Text( 327, 283, "1K", 2 );

				skp->SetTextColor( CR_LIGHT_GRAY );
				skp->Text( 375, 159, "0", 1 );
				skp->Text( 236, 298, "0", 1 );

				pos_roll = 0;
				pitch = GetIDP()->GetGlideSlopeDistance();
				yaw = -GetIDP()->GetYRunwayPositionError();

				if (pitch > 1000) pos_pitch = 250;
				else if (pitch < -1000) pos_pitch = 80;
				else pos_pitch = 165 + Round( pitch * 0.085 );

				if (yaw > 1000) pos_yaw = 324;
				else if (yaw < -1000) pos_yaw = 154;
				else pos_yaw = 239 + Round( yaw * 0.085 );
			}
			else if (GetIDP()->GetOnHACState() == true)
			{
				// HAC
				// blank
				// alt err +/-5k
				// HAC x-range err +/-5k
				skp->Text( 151, 283, "5K", 2 );

				skp->SetTextAlign( oapi::Sketchpad::LEFT );
				skp->Text( 356, 65, "5K", 2 );
				skp->Text( 356, 251, "5K", 2 );

				//skp->Text( 151, 283, "5K", 2 );
				skp->Text( 327, 283, "5K", 2 );

				skp->SetTextColor( CR_LIGHT_GRAY );
				skp->Text( 375, 159, "0", 1 );
				skp->Text( 236, 298, "0", 1 );

				pos_roll = 0;
				pitch = 0;// TODO get alt error (ft)
				yaw = GetIDP()->GetHACRadialError();

				if (pitch > 5000) pos_pitch = 250;
				else if (pitch < -5000) pos_pitch = 80;
				else pos_pitch = 165 + Round( pitch * 0.017 );

				if (yaw > 5000) pos_yaw = 324;
				else if (yaw < -5000) pos_yaw = 154;
				else pos_yaw = 239 + Round( yaw * 0.017 );
			}
			else
			{
				// ACQ
				// time to HAC 10s
				// alt err +/-5k
				// heading err +/-5�
				// (roll scale output is below)
				skp->Text( 151, 283, "5", 1 );

				skp->SetTextAlign( oapi::Sketchpad::LEFT );
				skp->Text( 356, 65, "5K", 2 );
				skp->Text( 356, 251, "5K", 2 );

				//skp->Text( 151, 283, "5", 1 );
				skp->Text( 327, 283, "5", 1 );

				skp->SetTextColor( CR_LIGHT_GRAY );
				skp->Text( 375, 159, "0", 1 );
				skp->Text( 236, 298, "0", 1 );

				roll = ceil( GetIDP()->GetTimeToHAC() );
				pitch = 0;// TODO get alt error (ft)
				yaw = 0;// TODO get heading error (deg)

				skp->SetTextColor( CR_WHITE );
				if (roll > 0)
				{
					skp->SetTextAlign( oapi::Sketchpad::RIGHT );
					skp->Text( 151, 31, "10s", 3 );
					skp->SetTextAlign( oapi::Sketchpad::LEFT );
					skp->Text( 327, 31, "0", 1 );
					if (roll >= 10) pos_roll = 154;
					else pos_roll = 324 - Round( roll * 17 );
				}
				else if (roll < 0)
				{
					skp->SetTextAlign( oapi::Sketchpad::RIGHT );
					skp->Text( 151, 31, "0", 1 );
					skp->SetTextAlign( oapi::Sketchpad::LEFT );
					skp->Text( 327, 31, "10s", 3 );
					if (roll <= -10) pos_roll = 324;
					else pos_roll = 154 - Round( roll * 17 );
				}
				else pos_roll = 0;

				if (pitch > 5000) pos_pitch = 250;
				else if (pitch < -5000) pos_pitch = 80;
				else pos_pitch = 165 + Round( pitch * 0.017 );

				if (yaw > 5) pos_yaw = 324;
				else if (yaw < -5) pos_yaw = 154;
				else pos_yaw = 239 + Round( yaw * 17 );
			}
		}
		else
		{
			// ADI RATE HIGH/LOW
			skp->Text( 151, 31, "5", 1 );
			skp->Text( 151, 283, "5", 1 );

			skp->SetTextAlign( oapi::Sketchpad::LEFT );
			//skp->Text( 151, 31, "5", 1 );
			skp->Text( 327, 31, "5", 1 );

			skp->Text( 356, 65, "5", 1 );
			skp->Text( 356, 251, "5", 1 );

			//skp->Text( 151, 283, "5", 1 );
			skp->Text( 327, 283, "5", 1 );

			skp->SetTextColor( CR_LIGHT_GRAY );
			skp->Text( 236, 18, "0", 1 );
			skp->Text( 375, 159, "0", 1 );
			skp->Text( 236, 298, "0", 1 );

			if (roll > 5) pos_roll = 324;
			else if (roll < -5) pos_roll = 154;
			else pos_roll = 239 + Round( roll * 17 );

			if (pitch > 5) pos_pitch = 250;
			else if (pitch < -5) pos_pitch = 80;
			else pos_pitch = 165 + Round( pitch * 17 );

			if (yaw > 5) pos_yaw = 324;
			else if (yaw < -5) pos_yaw = 154;
			else pos_yaw = 239 + Round( yaw * 17 );
		}
		
		// draw triangles
		skp->SetPen( skpLightGreenPen );
		skp->SetBrush( skpLightGreenBrush );
		oapi::IVECTOR2 tri[3];
		if (pos_roll != 0)
		{
			tri[0].x = pos_roll;
			tri[0].y = 34;
			tri[1].x = tri[0].x + 10;
			tri[1].y = 18;
			tri[2].x = tri[0].x - 10;
			tri[2].y = 18;
			skp->Polygon( tri, 3 );
		}
		if (pos_pitch != 0)
		{
			tri[0].x = 370;
			tri[0].y = pos_pitch;
			tri[1].x = 386;
			tri[1].y = tri[0].y - 10;
			tri[2].x = 386;
			tri[2].y = tri[0].y + 10;
			skp->Polygon( tri, 3 );
		}
		if (pos_yaw != 0)
		{
			tri[0].x = pos_yaw;
			tri[0].y = 296;
			tri[1].x = tri[0].x - 10;
			tri[1].y = 312;
			tri[2].x = tri[0].x + 10;
			tri[2].y = 312;
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
		// center (239,436) r = 95
		SelectObject( hDC, gdiLightGrayPen );
		SelectObject( hDC, gdiBlackBrush );
		::Ellipse( hDC, 134, 331, 344, 541 );// r = 105

		MoveToEx( hDC, 134, 436, NULL );
		LineTo( hDC, 128, 436 );
		MoveToEx( hDC, 165, 362, NULL );
		LineTo( hDC, 159, 356 );
		MoveToEx( hDC, 238, 341, NULL );
		LineTo( hDC, 238, 325 );
		MoveToEx( hDC, 240, 341, NULL );
		LineTo( hDC, 240, 325 );
		MoveToEx( hDC, 313, 362, NULL );
		LineTo( hDC, 319, 356 );
		MoveToEx( hDC, 344, 436, NULL );
		LineTo( hDC, 350, 436 );

		SelectObject( hDC, gdiWhitePen );
		SelectObject( hDC, gdiDarkGrayBrush );
		::Ellipse( hDC, 144, 341, 334, 531 );// r = 95

		SelectObject( hDC, gdiBlackBrush );
		::Ellipse( hDC, 177, 374, 301, 498 );// r = 62

		int x1;
		int y1;
		int x2;
		int y2;
		double cosi;
		double sini;
		for (int i = 90; i != 0; i -= 10)// big lines (10px)
		{
			cosi = cos( (i * RAD) + (heading * sgn) );
			sini = sin( (i * RAD) + (heading * sgn) );
			x1 = Round( 95 * cosi );
			y1 = Round( 95 * sini );

			x2 = Round( 85 * cosi );
			y2 = Round( 85 * sini );

			MoveToEx( hDC, 239 + x1, 436 - y1, NULL );
			LineTo( hDC, 239 + x2, 436 - y2 );

			MoveToEx( hDC, 239 + y1, 436 + x1, NULL );
			LineTo( hDC, 239 + y2, 436 + x2 );

			MoveToEx( hDC, 239 - x1, 436 + y1, NULL );
			LineTo( hDC, 239 - x2, 436 + y2 );

			MoveToEx( hDC, 239 - y1, 436 - x1, NULL );
			LineTo( hDC, 239 - y2, 436 - x2 );
		}

		for (int i = 85; i > 0; i -= 10)// small lines (5px)
		{
			cosi = cos( (i * RAD) + (heading * sgn) );
			sini = sin( (i * RAD) + (heading * sgn) );
			x1 = Round( 95 * cosi );
			y1 = Round( 95 * sini );

			x2 = Round( 90 * cosi );
			y2 = Round( 90 * sini );

			MoveToEx( hDC, 239 + x1, 436 - y1, NULL );
			LineTo( hDC, 239 + x2, 436 - y2 );

			MoveToEx( hDC, 239 + y1, 436 + x1, NULL );
			LineTo( hDC, 239 + y2, 436 + x2 );

			MoveToEx( hDC, 239 - x1, 436 + y1, NULL );
			LineTo( hDC, 239 - x2, 436 + y2 );

			MoveToEx( hDC, 239 - y1, 436 - x1, NULL );
			LineTo( hDC, 239 - y2, 436 - x2 );
		}

		SetTextColor( hDC, CR_WHITE );
		SelectObject( hDC, gdiSSUBFont_h12w7 );
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
			WTroll.eDx = (FLOAT)(239 - (239 * WTroll.eM11) - (436 * WTroll.eM21));
			WTroll.eDy = (FLOAT)(436 - (436 * WTroll.eM11) + (239 * WTroll.eM21));
			SetWorldTransform( hDC, &WTroll );

			// paint
			sprintf_s( cbuf, 4, "%d", i );// number
			TextOut( hDC, 239 - strlen( cbuf ) * 4, 359, cbuf, strlen( cbuf ) );

			// de-rotate
			ModifyWorldTransform( hDC, &WTroll, MWT_IDENTITY );
		}
		SetGraphicsMode( hDC, GM_COMPATIBLE );

		if (arrowon) HSI_Arrow( hDC, arrowheading * sgn );

		POINT ov_symbol[6] = {{239,419},{242,437},{246,442},{239,449},{232,442},{236,437}};
		SelectObject( hDC, gdiLightGrayBrush );
		SelectObject( hDC, gdiLightGrayPen );
		Polygon( hDC, ov_symbol, 6 );

		// "delete" bottom area where the menu will be
		SelectObject( hDC, gdiBlackBrush );
		SelectObject( hDC, gdiBlackPen );
		Rectangle( hDC, 134, 456, 344, 512 );
		return;
	}

	void MDU::HSI_A( oapi::Sketchpad2* skp, double heading, double roll, bool arrowon, double arrowheading )
	{
		double sgn = sign( (90 * RAD) - fabs( roll ) );
		// center (239,436)
		skp->SetPen( skpLightGrayPen );
		skp->SetBrush( skpBlackBrush );
		skp->Ellipse( 134, 331, 344, 541 );// r = 105

		skp->Line( 134, 436, 128, 436 );
		skp->Line( 165, 362, 159, 356 );
		skp->Line( 238, 341, 238, 325 );
		skp->Line( 240, 341, 240, 325 );
		skp->Line( 313, 362, 319, 356 );
		skp->Line( 344, 436, 350, 436 );

		skp->SetPen( skpWhitePen );
		skp->SetBrush( skpDarkGrayBrush );
		skp->Ellipse( 144, 341, 334, 531 );// r = 95

		skp->SetBrush( skpBlackBrush );
		skp->Ellipse( 177, 374, 301, 498 );// r = 62

		int x1;
		int y1;
		int x2;
		int y2;
		double cosi;
		double sini;
		for (int i = 90; i != 0; i -= 10)// big lines (10px)
		{
			cosi = cos( (i * RAD) + (heading * sgn) );
			sini = sin( (i * RAD) + (heading * sgn) );
			x1 = Round( 95 * cosi );
			y1 = Round( 95 * sini );

			x2 = Round( 85 * cosi );
			y2 = Round( 85 * sini );

			skp->Line( 239 + x1, 436 - y1, 239 + x2, 436 - y2 );

			skp->Line( 239 + y1, 436 + x1, 239 + y2, 436 + x2 );

			skp->Line( 239 - x1, 436 + y1, 239 - x2, 436 + y2 );

			skp->Line( 239 - y1, 436 - x1, 239 - y2, 436 - x2 );
		}

		for (int i = 85; i > 0; i -= 10)// small lines (5px)
		{
			cosi = cos( (i * RAD) + (heading * sgn) );
			sini = sin( (i * RAD) + (heading * sgn) );
			x1 = Round( 95 * cosi );
			y1 = Round( 95 * sini );

			x2 = Round( 90 * cosi );
			y2 = Round( 90 * sini );

			skp->Line( 239 + x1, 436 - y1, 239 + x2, 436 - y2 );

			skp->Line( 239 + y1, 436 + x1, 239 + y2, 436 + x2 );

			skp->Line( 239 - x1, 436 + y1, 239 - x2, 436 + y2 );

			skp->Line( 239 - y1, 436 - x1, 239 - y2, 436 - x2 );
		}

		skp->SetTextColor( CR_WHITE );
		skp->SetFont( skpSSUBFont_h12 );
		char cbuf[4];
		oapi::IVECTOR2 cntr;
		cntr.x = 239;
		cntr.y = 436;
		for (int i = 0; i <= 33; i += 3)
		{
			// rotate
			skp->SetWorldTransform2D( 1.0f, (float)(i * 10 * RAD * sgn - (heading * sgn)), &cntr );

			// paint
			sprintf_s( cbuf, 4, "%d", i );// number
			skp->Text( 239 - strlen( cbuf ) * 4, 359, cbuf, strlen( cbuf ) );

			// de-rotate
			skp->SetWorldTransform();
		}

		if (arrowon) HSI_Arrow( skp, arrowheading * sgn );

		oapi::IVECTOR2 ov_symbol[6] = {{239,419},{242,437},{246,442},{239,449},{232,442},{236,437}};
		skp->SetBrush( skpLightGrayBrush );
		skp->SetPen( skpLightGrayPen );
		skp->Polygon( ov_symbol, 6 );

		// "delete" bottom area where the menu will be
		skp->SetBrush( skpBlackBrush );
		skp->SetPen( skpBlackPen );
		skp->Rectangle( 134, 456, 344, 512 );
		return;
	}

	void MDU::HSI_E( HDC hDC, double heading, bool arrowon, double arrowheading )
	{
		// center (239,436) r = 95
		SelectObject( hDC, gdiLightGrayPen );
		SelectObject( hDC, gdiBlackBrush );
		::Ellipse( hDC, 134, 331, 344, 541 );// r = 105

		MoveToEx( hDC, 134, 436, NULL );
		LineTo( hDC, 128, 436 );
		MoveToEx( hDC, 165, 362, NULL );
		LineTo( hDC, 159, 356 );
		MoveToEx( hDC, 238, 341, NULL );
		LineTo( hDC, 238, 325 );
		MoveToEx( hDC, 240, 341, NULL );
		LineTo( hDC, 240, 325 );
		MoveToEx( hDC, 313, 362, NULL );
		LineTo( hDC, 319, 356 );
		MoveToEx( hDC, 344, 436, NULL );
		LineTo( hDC, 350, 436 );

		SelectObject( hDC, gdiWhitePen );
		SelectObject( hDC, gdiDarkGrayBrush );
		::Ellipse( hDC, 144, 341, 334, 531 );// r = 95

		SelectObject( hDC, gdiBlackBrush );
		::Ellipse( hDC, 177, 374, 301, 498 );// r = 62

		int x1;
		int y1;
		int x2;
		int y2;
		double cosi;
		double sini;
		for (int i = 90; i != 0; i -= 10)// big lines (10px)
		{
			cosi = cos( (i * RAD) + heading );
			sini = sin( (i * RAD) + heading );
			x1 = Round( 95 * cosi );
			y1 = Round( 95 * sini );

			x2 = Round( 85 * cosi );
			y2 = Round( 85 * sini );

			MoveToEx( hDC, 239 + x1, 436 - y1, NULL );
			LineTo( hDC, 239 + x2, 436 - y2 );

			MoveToEx( hDC, 239 + y1, 436 + x1, NULL );
			LineTo( hDC, 239 + y2, 436 + x2 );

			MoveToEx( hDC, 239 - x1, 436 + y1, NULL );
			LineTo( hDC, 239 - x2, 436 + y2 );

			MoveToEx( hDC, 239 - y1, 436 - x1, NULL );
			LineTo( hDC, 239 - y2, 436 - x2 );
		}
		for (int i = 85; i > 0; i -= 10)// small lines (5px)
		{
			cosi = cos( (i * RAD) + heading );
			sini = sin( (i * RAD) + heading );
			x1 = Round( 95 * cosi );
			y1 = Round( 95 * sini );

			x2 = Round( 90 * cosi );
			y2 = Round( 90 * sini );

			MoveToEx( hDC, 239 + x1, 436 - y1, NULL );
			LineTo( hDC, 239 + x2, 436 - y2 );

			MoveToEx( hDC, 239 + y1, 436 + x1, NULL );
			LineTo( hDC, 239 + y2, 436 + x2 );

			MoveToEx( hDC, 239 - x1, 436 + y1, NULL );
			LineTo( hDC, 239 - x2, 436 + y2 );

			MoveToEx( hDC, 239 - y1, 436 - x1, NULL );
			LineTo( hDC, 239 - y2, 436 - x2 );
		}

		SetTextColor( hDC, CR_WHITE );
		SelectObject( hDC, gdiSSUBFont_h12w7 );
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
			WTroll.eDx = (FLOAT)(239 - (239 * WTroll.eM11) - (436 * WTroll.eM21));
			WTroll.eDy = (FLOAT)(436 - (436 * WTroll.eM11) + (239 * WTroll.eM21));
			SetWorldTransform( hDC, &WTroll );

			// paint
			if ((i % 9) != 0)// number
			{
				sprintf_s( cbuf, 4, "%d", i );
				TextOut( hDC, 239 - strlen( cbuf ) * 4, 359, cbuf, strlen( cbuf ) );
			}
			else// letter
			{
				SelectObject( hDC, gdiSSUBFont_h16w9 );
				if (i == 0) TextOut( hDC, 235, 354, "N", 1 );
				else if (i == 9) TextOut( hDC, 235, 354, "E", 1 );
				else if (i == 18) TextOut( hDC, 235, 354, "S", 1 );
				else TextOut( hDC, 235, 354, "W", 1 );
				SelectObject( hDC, gdiSSUBFont_h12w7 );
			}

			// de-rotate
			ModifyWorldTransform( hDC, &WTroll, MWT_IDENTITY );
		}
		SetGraphicsMode( hDC, GM_COMPATIBLE );

		if (arrowon) HSI_Arrow( hDC, (heading * DEG) - arrowheading );

		POINT ov_symbol[6] = {{239,419},{242,437},{246,442},{239,449},{232,442},{236,437}};
		SelectObject( hDC, gdiLightGrayBrush );
		SelectObject( hDC, gdiLightGrayPen );
		Polygon( hDC, ov_symbol, 6 );

		// "delete" bottom area where the menu will be
		SelectObject( hDC, gdiBlackBrush );
		SelectObject( hDC, gdiBlackPen );
		Rectangle( hDC, 134, 456, 344, 512 );
		return;
	}

	void MDU::HSI_E( oapi::Sketchpad2* skp, double heading, bool arrowon, double arrowheading )
	{
		// center (239,436)
		skp->SetPen( skpLightGrayPen );
		skp->SetBrush( skpBlackBrush );
		skp->Ellipse( 134, 331, 344, 541 );// r = 105

		skp->Line( 134, 436, 128, 436 );
		skp->Line( 165, 362, 159, 356 );
		skp->Line( 238, 341, 238, 325 );
		skp->Line( 240, 341, 240, 325 );
		skp->Line( 313, 362, 319, 356 );
		skp->Line( 344, 436, 350, 436 );

		skp->SetPen( skpWhitePen );
		skp->SetBrush( skpDarkGrayBrush );
		skp->Ellipse( 144, 341, 334, 531 );// r = 95

		skp->SetBrush( skpBlackBrush );
		skp->Ellipse( 177, 374, 301, 498 );// r = 62

		int x1;
		int y1;
		int x2;
		int y2;
		double cosi;
		double sini;
		for (int i = 90; i != 0; i -= 10)// big lines (10px)
		{
			cosi = cos( (i * RAD) + heading );
			sini = sin( (i * RAD) + heading );
			x1 = Round( 95 * cosi );
			y1 = Round( 95 * sini );

			x2 = Round( 85 * cosi );
			y2 = Round( 85 * sini );

			skp->Line( 239 + x1, 436 - y1, 239 + x2, 436 - y2 );

			skp->Line( 239 + y1, 436 + x1, 239 + y2, 436 + x2 );

			skp->Line( 239 - x1, 436 + y1, 239 - x2, 436 + y2 );

			skp->Line( 239 - y1, 436 - x1, 239 - y2, 436 - x2 );
		}

		for (int i = 85; i > 0; i -= 10)// small lines (5px)
		{
			cosi = cos( (i * RAD) + heading );
			sini = sin( (i * RAD) + heading );
			x1 = Round( 95 * cosi );
			y1 = Round( 95 * sini );

			x2 = Round( 90 * cosi );
			y2 = Round( 90 * sini );

			skp->Line( 239 + x1, 436 - y1, 239 + x2, 436 - y2 );

			skp->Line( 239 + y1, 436 + x1, 239 + y2, 436 + x2 );

			skp->Line( 239 - x1, 436 + y1, 239 - x2, 436 + y2 );

			skp->Line( 239 - y1, 436 - x1, 239 - y2, 436 - x2 );
		}

		skp->SetTextColor( CR_WHITE );
		skp->SetFont( skpSSUBFont_h12 );
		char cbuf[4];
		oapi::IVECTOR2 cntr;
		cntr.x = 239;
		cntr.y = 436;
		for (int i = 0; i <= 33; i += 3)
		{
			// rotate
			skp->SetWorldTransform2D( 1.0f, (float)(i * 10 * RAD - heading), &cntr );

			// paint
			if ((i % 9) != 0)// number
			{
				sprintf_s( cbuf, 4, "%d", i );
				skp->Text( 239 - strlen( cbuf ) * 4, 359, cbuf, strlen( cbuf ) );
			}
			else// letter
			{
				skp->SetFont( skpSSUBFont_h16 );
				if (i == 0) skp->Text( 235, 354, "N", 1 );
				else if (i == 9) skp->Text( 235, 354, "E", 1 );
				else if (i == 18) skp->Text( 235, 354, "S", 1 );
				else skp->Text( 235, 354, "W", 1 );
				skp->SetFont( skpSSUBFont_h12 );
			}

			// de-rotate
			skp->SetWorldTransform();
		}

		if (arrowon) HSI_Arrow( skp, (heading * DEG) - arrowheading );

		oapi::IVECTOR2 ov_symbol[6] = {{239,419},{242,437},{246,442},{239,449},{232,442},{236,437}};
		skp->SetBrush( skpLightGrayBrush );
		skp->SetPen( skpLightGrayPen );
		skp->Polygon( ov_symbol, 6 );

		// "delete" bottom area where the menu will be
		skp->SetBrush( skpBlackBrush );
		skp->SetPen( skpBlackPen );
		skp->Rectangle( 134, 456, 344, 512 );
		return;
	}

	void MDU::HSI_Arrow( HDC hDC, double heading )
	{
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

		SelectObject( hDC, gdiWhiteBrush );
		dotLL_X = 239 - Round( 40 * cosH );
		dotLL_Y = 436 - Round( 40 * sinH );
		dotL_X = 239 - Round( 20 * cosH );
		dotL_Y = 436 - Round( 20 * sinH );
		dotR_X = 239 + Round( 20 * cosH );
		dotR_Y = 436 + Round( 20 * sinH );
		dotRR_X = 239 + Round( 40 * cosH );
		dotRR_Y = 436 + Round( 40 * sinH );
		::Ellipse( hDC, dotLL_X - 5, dotLL_Y - 5, dotLL_X + 5, dotLL_Y + 5 );
		::Ellipse( hDC, dotL_X - 5, dotL_Y - 5, dotL_X + 5, dotL_Y + 5 );
		::Ellipse( hDC, dotR_X - 5, dotR_Y - 5, dotR_X + 5, dotR_Y + 5 );
		::Ellipse( hDC, dotRR_X - 5, dotRR_Y - 5, dotRR_X + 5, dotRR_Y + 5 );

		SelectObject( hDC, gdiMagentaBrush );
		SelectObject( hDC, gdiBlackPen );
		arrow[0].x = 239 + Round( 87 * sinH );
		arrow[0].y = 436 - Round( 87 * cosH );
		arrow[1].x = 239 - Round( 5 * cosH - 55 * sinH );
		arrow[1].y = 436 - Round( 5 * sinH + 55 * cosH );
		arrow[2].x = 239 - Round( 11 * cosH - 51 * sinH );
		arrow[2].y = 436 - Round( 11 * sinH + 51 * cosH );
		arrow[3].x = 239 - Round( 12 * cosH - 46 * sinH );
		arrow[3].y = 436 - Round( 12 * sinH + 46 * cosH );
		arrow[4].x = 239 - Round( 6 * cosH - 48 * sinH );
		arrow[4].y = 436 - Round( 6 * sinH + 48 * cosH );
		arrow[5].x = 239 + Round( 41 * sinH );
		arrow[5].y = 436 - Round( 41 * cosH );
		arrow[6].x = 239 + Round( 6 * cosH + 48 * sinH );
		arrow[6].y = 436 + Round( 6 * sinH - 48 * cosH );
		arrow[7].x = 239 + Round( 12 * cosH + 46 * sinH );
		arrow[7].y = 436 + Round( 12 * sinH - 46 * cosH );
		arrow[8].x = 239 + Round( 11 * cosH + 51 * sinH );
		arrow[8].y = 436 + Round( 11 * sinH - 51 * cosH );
		arrow[9].x = 239 + Round( 5 * cosH + 55 * sinH );
		arrow[9].y = 436 + Round( 5 * sinH - 55 * cosH );
		Polygon( hDC, arrow, 10 );
		MoveToEx( hDC, arrow[0].x, arrow[0].y, NULL );
		LineTo( hDC, arrow[5].x, arrow[5].y );

		arrow[0].x = 239 - Round( 3 * cosH + 40 * sinH );
		arrow[0].y = 436 - Round( 3 * sinH - 40 * cosH );
		arrow[1].x = 239 - Round( 3 * cosH + 77 * sinH );
		arrow[1].y = 436 - Round( 3 * sinH - 77 * cosH );
		arrow[2].x = 239 + Round( 3 * cosH - 77 * sinH );
		arrow[2].y = 436 + Round( 3 * sinH + 77 * cosH );
		arrow[3].x = 239 + Round( 3 * cosH - 40 * sinH );
		arrow[3].y = 436 + Round( 3 * sinH + 40 * cosH );
		Polygon( hDC, arrow, 4 );
		return;
	}

	void MDU::HSI_Arrow( oapi::Sketchpad2* skp, double heading )
	{
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

		skp->SetBrush( skpWhiteBrush );
		dotLL_X = 239 - Round( 40 * cosH );
		dotLL_Y = 436 - Round( 40 * sinH );
		dotL_X = 239 - Round( 20 * cosH );
		dotL_Y = 436 - Round( 20 * sinH );
		dotR_X = 239 + Round( 20 * cosH );
		dotR_Y = 436 + Round( 20 * sinH );
		dotRR_X = 239 + Round( 40 * cosH );
		dotRR_Y = 436 + Round( 40 * sinH );
		skp->Ellipse( dotLL_X - 5, dotLL_Y - 5, dotLL_X + 5, dotLL_Y + 5 );
		skp->Ellipse( dotL_X - 5, dotL_Y - 5, dotL_X + 5, dotL_Y + 5 );
		skp->Ellipse( dotR_X - 5, dotR_Y - 5, dotR_X + 5, dotR_Y + 5 );
		skp->Ellipse( dotRR_X - 5, dotRR_Y - 5, dotRR_X + 5, dotRR_Y + 5 );

		skp->SetBrush( skpMagentaBrush );
		skp->SetPen( skpBlackPen );
		arrow[0].x = 239 + Round( 87 * sinH );
		arrow[0].y = 436 - Round( 87 * cosH );
		arrow[1].x = 239 - Round( 5 * cosH - 55 * sinH );
		arrow[1].y = 436 - Round( 5 * sinH + 55 * cosH );
		arrow[2].x = 239 - Round( 11 * cosH - 51 * sinH );
		arrow[2].y = 436 - Round( 11 * sinH + 51 * cosH );
		arrow[3].x = 239 - Round( 12 * cosH - 46 * sinH );
		arrow[3].y = 436 - Round( 12 * sinH + 46 * cosH );
		arrow[4].x = 239 - Round( 6 * cosH - 48 * sinH );
		arrow[4].y = 436 - Round( 6 * sinH + 48 * cosH );
		arrow[5].x = 239 + Round( 41 * sinH );
		arrow[5].y = 436 - Round( 41 * cosH );
		arrow[6].x = 239 + Round( 6 * cosH + 48 * sinH );
		arrow[6].y = 436 + Round( 6 * sinH - 48 * cosH );
		arrow[7].x = 239 + Round( 12 * cosH + 46 * sinH );
		arrow[7].y = 436 + Round( 12 * sinH - 46 * cosH );
		arrow[8].x = 239 + Round( 11 * cosH + 51 * sinH );
		arrow[8].y = 436 + Round( 11 * sinH - 51 * cosH );
		arrow[9].x = 239 + Round( 5 * cosH + 55 * sinH );
		arrow[9].y = 436 + Round( 5 * sinH - 55 * cosH );
		skp->Polygon( arrow, 10 );
		skp->Line( arrow[0].x, arrow[0].y, arrow[5].x, arrow[5].y );

		arrow[0].x = 239 - Round( 3 * cosH + 40 * sinH );
		arrow[0].y = 436 - Round( 3 * sinH - 40 * cosH );
		arrow[1].x = 239 - Round( 3 * cosH + 77 * sinH );
		arrow[1].y = 436 - Round( 3 * sinH - 77 * cosH );
		arrow[2].x = 239 + Round( 3 * cosH - 77 * sinH );
		arrow[2].y = 436 + Round( 3 * sinH + 77 * cosH );
		arrow[3].x = 239 + Round( 3 * cosH - 40 * sinH );
		arrow[3].y = 436 + Round( 3 * sinH + 40 * cosH );
		skp->Polygon( arrow, 4 );
		return;
	}

	void MDU::AEPFD_Header_AscentDAP( HDC hDC, int MM, int adiatt )
	{
		char cbuf[8];
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 40, 3, "DAP:", 4 );

		TextOut( hDC, 413, 3, "MM:", 3 );
		
		TextOut( hDC, 22, 18, "Throt:", 6 );

		TextOut( hDC, 404, 18, "ATT:", 4 );

		SetTextColor( hDC, CR_WHITE );
		if (1) TextOut( hDC, 85, 3, "Auto", 4 );// TODO get AscentDAP state
		else
		{
			SelectObject( hDC, gdiYellowPen );
			SelectObject( hDC, GetStockObject( HOLLOW_BRUSH ) );
			Rectangle( hDC, 16, 4, 132, 20 );
			TextOut( hDC, 85, 3, "CSS", 3 );
		}

		if (0) sprintf_s( cbuf, 8, "%dR", MM );// RTLS
		else if (0) sprintf_s( cbuf, 8, "%dT", MM );// TAL
		else if (0) sprintf_s( cbuf, 8, "%dATO", MM );// ATO
		else if (0) sprintf_s( cbuf, 8, "%dAOA", MM );// AOA
		else if (0) sprintf_s( cbuf, 8, "%dCA", MM );// CA
		else sprintf_s( cbuf, 8, "%d", MM );// NOM
		TextOut( hDC, 449, 3, cbuf, strlen( cbuf ) );

		if (GetIDP()->GetAutoThrottleState() == true) TextOut( hDC, 85, 18, "Auto", 4 );
		else
		{
			SelectObject( hDC, gdiYellowPen );
			SelectObject( hDC, GetStockObject( HOLLOW_BRUSH ) );
			Rectangle( hDC, 16, 16, 132, 32 );
			TextOut( hDC, 85, 18, "MAN", 3 );
		}

		if (adiatt == 2) TextOut( hDC, 449, 18, "Inrtl", 5 );
		else if (adiatt == 1) TextOut( hDC, 449, 18, "LVLH", 4 );
		else TextOut( hDC, 449, 18, "Ref", 3 );
		return;
	}

	void MDU::AEPFD_Header_AscentDAP( oapi::Sketchpad2* skp, int MM, int adiatt )
	{
		char cbuf[8];
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 40, 3, "DAP:", 4 );

		skp->Text( 413, 3, "MM:", 3 );
		
		skp->Text( 22, 18, "Throt:", 6 );

		skp->Text( 404, 18, "ATT:", 4 );

		skp->SetTextColor( CR_WHITE );
		if (1) skp->Text( 85, 3, "Auto", 4 );// TODO get AscentDAP state
		else
		{
			skp->SetPen( skpYellowPen );
			skp->SetBrush( NULL );
			skp->Rectangle( 16, 4, 132, 20 );
			skp->Text( 85, 3, "CSS", 3 );
		}

		if (0) sprintf_s( cbuf, 8, "%dR", MM );// RTLS
		else if (0) sprintf_s( cbuf, 8, "%dT", MM );// TAL
		else if (0) sprintf_s( cbuf, 8, "%dATO", MM );// ATO
		else if (0) sprintf_s( cbuf, 8, "%dAOA", MM );// AOA
		else if (0) sprintf_s( cbuf, 8, "%dCA", MM );// CA
		else sprintf_s( cbuf, 8, "%d", MM );// NOM
		skp->Text( 449, 3, cbuf, strlen( cbuf ) );

		if (GetIDP()->GetAutoThrottleState() == true) skp->Text( 85, 18, "Auto", 4 );
		else
		{
			skp->SetPen( skpYellowPen );
			skp->SetBrush( NULL );
			skp->Rectangle( 16, 19, 132, 35 );
			skp->Text( 85, 21, "MAN", 3 );
		}

		if (adiatt == 2) skp->Text( 449, 18, "Inrtl", 5 );
		else if (adiatt == 1) skp->Text( 449, 18, "LVLH", 4 );
		else skp->Text( 449, 18, "Ref", 3 );
		return;
	}

	void MDU::AEPFD_Header_TransDAP( HDC hDC, int MM, int adiatt )
	{
		char cbuf[8];
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 40, 3, "DAP:", 4 );

		TextOut( hDC, 413, 3, "MM:", 3 );

		TextOut( hDC, 404, 18, "ATT:", 4 );

		SetTextColor( hDC, CR_WHITE );
		if (1) TextOut( hDC, 85, 3, "Auto", 4 );// TODO get TransDAP state
		else TextOut( hDC, 85, 3, "INRTL", 5 );

		if (0) sprintf_s( cbuf, 8, "%dR", MM );// RTLS
		else if (0) sprintf_s( cbuf, 8, "%dT", MM );// TAL
		else if (0) sprintf_s( cbuf, 8, "%dATO", MM );// ATO
		else if (0) sprintf_s( cbuf, 8, "%dAOA", MM );// AOA
		else if (0) sprintf_s( cbuf, 8, "%dCA", MM );// CA
		else sprintf_s( cbuf, 8, "%d", MM );// NOM
		TextOut( hDC, 449, 3, cbuf, strlen( cbuf ) );

		if (adiatt == 2) TextOut( hDC, 449, 18, "Inrtl", 5 );
		else if (adiatt == 1) TextOut( hDC, 449, 18, "LVLH", 4 );
		else TextOut( hDC, 449, 18, "Ref", 3 );
		return;
	}

	void MDU::AEPFD_Header_TransDAP( oapi::Sketchpad2* skp, int MM, int adiatt )
	{
		char cbuf[8];
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 40, 3, "DAP:", 4 );

		skp->Text( 413, 3, "MM:", 3 );

		skp->Text( 404, 18, "ATT:", 4 );

		skp->SetTextColor( CR_WHITE );
		if (1) skp->Text( 85, 3, "Auto", 4 );// TODO get TransDAP state
		else skp->Text( 85, 3, "INRTL", 5 );

		if (0) sprintf_s( cbuf, 8, "%dR", MM );// RTLS
		else if (0) sprintf_s( cbuf, 8, "%dT", MM );// TAL
		else if (0) sprintf_s( cbuf, 8, "%dATO", MM );// ATO
		else if (0) sprintf_s( cbuf, 8, "%dAOA", MM );// AOA
		else if (0) sprintf_s( cbuf, 8, "%dCA", MM );// CA
		else sprintf_s( cbuf, 8, "%d", MM );// NOM
		skp->Text( 449, 3, cbuf, strlen( cbuf ) );

		if (adiatt == 2) skp->Text( 449, 18, "Inrtl", 5 );
		else if (adiatt == 1) skp->Text( 449, 18, "LVLH", 4 );
		else skp->Text( 449, 18, "Ref", 3 );
		return;
	}

	void MDU::AEPFD_Header_AerojetDAP( HDC hDC, int MM, double MachNumber )
	{
		char cbuf[8];
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 22, 3, "Pitch:", 6 );

		TextOut( hDC, 40, 18, "R/Y:", 4 );

		TextOut( hDC, 413, 3, "MM:", 3 );

		TextOut( hDC, 413, 18, "SB:", 3 );

		SetTextColor( hDC, CR_WHITE );
		if (GetIDP()->GetAutoPitchState() == true) TextOut( hDC, 85, 3, "Auto", 4 );
		else
		{
			if (MachNumber > 1)
			{
				SelectObject( hDC, gdiYellowPen );
				SelectObject( hDC, GetStockObject( HOLLOW_BRUSH ) );
				Rectangle( hDC, 16, 1, 132, 17 );
			}
			TextOut( hDC, 85, 3, "CSS", 3 );
		}

		if (GetIDP()->GetAutoRollYawState() == true) TextOut( hDC, 85, 18, "Auto", 4 );
		else
		{
			if (MachNumber > 1)
			{
				SelectObject( hDC, gdiYellowPen );
				SelectObject( hDC, GetStockObject( HOLLOW_BRUSH ) );
				Rectangle( hDC, 16, 16, 132, 32 );
			}
			TextOut( hDC, 85, 18, "CSS", 3 );
		}

		if (0) sprintf_s( cbuf, 8, "%dR", MM );// RTLS
		else if (0) sprintf_s( cbuf, 8, "%dT", MM );// TAL
		else if (0) sprintf_s( cbuf, 8, "%dATO", MM );// ATO
		else if (0) sprintf_s( cbuf, 8, "%dAOA", MM );// AOA
		else if (0) sprintf_s( cbuf, 8, "%dCA", MM );// CA
		else sprintf_s( cbuf, 8, "%d", MM );// NOM
		TextOut( hDC, 449, 3, cbuf, strlen( cbuf ) );

		if (GetIDP()->GetAutoSpeedbrakeState() == true) TextOut( hDC, 449, 18, "Auto", 4 );
		else
		{
			SelectObject( hDC, gdiYellowPen );
			SelectObject( hDC, GetStockObject( HOLLOW_BRUSH ) );
			Rectangle( hDC, 400, 16, 500, 32 );
			TextOut( hDC, 449, 18, "MAN", 3 );
		}
		return;
	}

	void MDU::AEPFD_Header_AerojetDAP( oapi::Sketchpad2* skp, int MM, double MachNumber )
	{
		char cbuf[8];
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 22, 3, "Pitch:", 6 );

		skp->Text( 40, 18, "R/Y:", 4 );

		skp->Text( 413, 3, "MM:", 3 );

		skp->Text( 413, 18, "SB:", 3 );

		skp->SetTextColor( CR_WHITE );
		if (GetIDP()->GetAutoPitchState() == true) skp->Text( 85, 3, "Auto", 4 );
		else
		{
			if (MachNumber > 1)
			{
				skp->SetPen( skpYellowPen );
				skp->SetBrush( NULL );
				skp->Rectangle( 16, 1, 132, 17 );
			}
			skp->Text( 85, 3, "CSS", 3 );
		}

		if (GetIDP()->GetAutoRollYawState() == true) skp->Text( 85, 18, "Auto", 4 );
		else
		{
			if (MachNumber > 1)
			{
				skp->SetPen( skpYellowPen );
				skp->SetBrush( NULL );
				skp->Rectangle( 16, 16, 132, 32 );
			}
			skp->Text( 85, 18, "CSS", 3 );
		}

		if (0) sprintf_s( cbuf, 8, "%dR", MM );// RTLS
		else if (0) sprintf_s( cbuf, 8, "%dT", MM );// TAL
		else if (0) sprintf_s( cbuf, 8, "%dATO", MM );// ATO
		else if (0) sprintf_s( cbuf, 8, "%dAOA", MM );// AOA
		else if (0) sprintf_s( cbuf, 8, "%dCA", MM );// CA
		else sprintf_s( cbuf, 8, "%d", MM );// NOM
		skp->Text( 449, 3, cbuf, strlen( cbuf ) );

		if (GetIDP()->GetAutoSpeedbrakeState() == true) skp->Text( 449, 18, "Auto", 4 );
		else
		{
			skp->SetPen( skpYellowPen );
			skp->SetBrush( NULL );
			skp->Rectangle( 400, 16, 500, 32 );
			skp->Text( 449, 18, "MAN", 3 );
		}
		return;
	}

	void MDU::AEPFD_BETA( HDC hDC )
	{
		SelectObject( hDC, gdiLightGrayPen );
		SelectObject( hDC, gdiBlackBrush );
		Rectangle( hDC, 68, 285, 118, 309 );

		SelectObject( hDC, gdiSSUAFont_h11w9 );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 74, 315, "Beta", 4 );

		SelectObject( hDC, gdiSSUBFont_h12w7 );
		SetTextColor( hDC, CR_WHITE );
		double beta = STS()->GetSlipAngle() * DEG;
		if (beta < 0) TextOut( hDC, 70, 290, "L", 1 );
		else TextOut( hDC, 70, 290, "R", 1 );
		
		SelectObject( hDC, gdiSSUBFont_h18w9 );
		SetTextAlign( hDC, TA_RIGHT );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%4.1f", fabs( beta ) );
		TextOut( hDC, 115, 286, cbuf, strlen( cbuf ) );
		SetTextAlign( hDC, TA_LEFT );
		return;
	}

	void MDU::AEPFD_BETA( oapi::Sketchpad2* skp )
	{
		skp->SetPen( skpLightGrayPen );
		skp->SetBrush( skpBlackBrush );
		skp->Rectangle( 68, 285, 118, 309 );

		skp->SetFont( skpSSUAFont_h11 );
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 74, 315, "Beta", 4 );

		skp->SetFont( skpSSUBFont_h12 );
		skp->SetTextColor( CR_WHITE );
		double beta = STS()->GetSlipAngle() * DEG;
		if (beta < 0) skp->Text( 70, 290, "L", 1 );
		else skp->Text( 70, 290, "R", 1 );
		
		skp->SetFont( skpSSUBFont_h18 );
		skp->SetTextAlign( oapi::Sketchpad::RIGHT );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%4.1f", fabs( beta ) );
		skp->Text( 115, 286, cbuf, strlen( cbuf ) );
		skp->SetTextAlign( oapi::Sketchpad::LEFT );
		return;
	}

	void MDU::AEPFD_GMETER_STATIC( HDC hDC )
	{
		SelectObject( hDC, gdiLightGrayPen );
		SelectObject( hDC, gdiBlackBrush );
		Rectangle( hDC, 75, 366, 123, 390 );

		Arc( hDC, 31, 356, 107, 432, 69, 356, 96, 421 );
		
		MoveToEx( hDC, 69, 356, NULL );
		LineTo( hDC, 69, 351 );
		MoveToEx( hDC, 42, 367, NULL );
		LineTo( hDC, 37, 362 );
		MoveToEx( hDC, 31, 394, NULL );
		LineTo( hDC, 26, 394 );
		MoveToEx( hDC, 42, 420, NULL );
		LineTo( hDC, 37, 425 );
		MoveToEx( hDC, 69, 432, NULL );
		LineTo( hDC, 69, 437 );
		MoveToEx( hDC, 95, 420, NULL );
		LineTo( hDC, 100, 425 );

		SetTextColor( hDC, CR_LIGHT_GRAY );
		SelectObject( hDC, gdiSSUAFont_h11w9 );
		TextOut( hDC, 112, 373, "g", 1 );

		SelectObject( hDC, gdiSSUBFont_h12w7 );
		TextOut( hDC, 27, 346, "3", 1 );
		TextOut( hDC, 12, 387, "2", 1 );
		TextOut( hDC, 27, 424, "1", 1 );
		TextOut( hDC, 66, 439, "0", 1 );
		TextOut( hDC, 100, 427, "-1", 2 );
		return;
	}

	void MDU::AEPFD_GMETER_STATIC( oapi::Sketchpad2* skp )
	{
		skp->SetPen( skpLightGrayPen );
		skp->SetBrush( skpBlackBrush );
		skp->Ellipse( 31, 356, 107, 432 );
		skp->SetPen( skpBlackPen );
		skp->Rectangle( 69, 356, 108, 421 );// cover part of the circle to get an arc because sketchpad doesn't have Arc()

		skp->SetPen( skpLightGrayPen );
		skp->Rectangle( 75, 366, 123, 390 );
		
		skp->Line( 69, 356, 69, 351 );
		skp->Line( 42, 367, 37, 362 );
		skp->Line( 31, 394, 26, 394 );
		skp->Line( 42, 420, 37, 425 );
		skp->Line( 69, 432, 69, 437 );
		skp->Line( 95, 420, 100, 425 );

		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->SetFont( skpSSUAFont_h11 );
		skp->Text( 112, 373, "g", 1 );

		skp->SetFont( skpSSUBFont_h12 );
		skp->Text( 27, 346, "3", 1 );
		skp->Text( 12, 387, "2", 1 );
		skp->Text( 27, 424, "1", 1 );
		skp->Text( 66, 439, "0", 1 );
		skp->Text( 100, 427, "-1", 2 );
		return;
	}

	void MDU::AEPFD_GMETER_ACCEL( HDC hDC )
	{
		SelectObject( hDC, gdiSSUAFont_h11w9 );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 83, 395, "Accel", 5 );
		VECTOR3 f;
		STS()->GetForceVector( f );
		double dtmp = (f.z / (STS()->GetMass() * G)) + sin( STS()->GetPitch() );

		SelectObject( hDC, gdiSSUBFont_h18w9 );
		SetTextColor( hDC, CR_WHITE );
		char cbuf[8];
		SetTextAlign( hDC, TA_RIGHT );
		sprintf_s( cbuf, 8, "%.1f", dtmp );
		TextOut( hDC, 111, 367, cbuf, strlen( cbuf ) );
		SetTextAlign( hDC, TA_LEFT );

		if (dtmp > 4) dtmp = 4;
		else if (dtmp < -1) dtmp = -1;
		dtmp = ((dtmp * 45) - 90) * RAD;

		double cosdtmp = cos( dtmp );
		double sindtmp = sin( dtmp );
		// center (69,394)
		POINT arrow[3];
		arrow[0].x = 69 - Round( 33 * cosdtmp );
		arrow[0].y = 394 - Round( 33 * sindtmp );
		arrow[1].x = 69 - Round( (22 * cosdtmp) + (6 * sindtmp) );
		arrow[1].y = 394 - Round( (22 * sindtmp) - (6 * cosdtmp) );
		arrow[2].x = 69 - Round( (22 * cosdtmp) - (6 * sindtmp) );
		arrow[2].y = 394 - Round( (22 * sindtmp) + (6 * cosdtmp) );

		SelectObject( hDC, gdiLightGreenPen );
		SelectObject( hDC, gdiLightGreenBrush );
		Polygon( hDC, arrow, 3 );

		SelectObject( hDC, gdiLightGreenThickPen );
		MoveToEx( hDC, 69, 394, NULL );
		LineTo( hDC, 69 - Round( 31 * cosdtmp ), 394 - Round( 31 * sindtmp ) );
		return;
	}

	void MDU::AEPFD_GMETER_ACCEL( oapi::Sketchpad2* skp )
	{
		skp->SetFont( skpSSUAFont_h11 );
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 83, 395, "Accel", 5 );
		VECTOR3 f;
		STS()->GetForceVector( f );
		double dtmp = (f.z / (STS()->GetMass() * G)) + sin( STS()->GetPitch() );

		skp->SetFont( skpSSUBFont_h18 );
		skp->SetTextColor( CR_WHITE );
		char cbuf[8];
		skp->SetTextAlign( oapi::Sketchpad::RIGHT );
		sprintf_s( cbuf, 8, "%.1f", dtmp );
		skp->Text( 111, 367, cbuf, strlen( cbuf ) );
		skp->SetTextAlign( oapi::Sketchpad::LEFT );

		if (dtmp > 4) dtmp = 4;
		else if (dtmp < -1) dtmp = -1;
		dtmp = ((dtmp * 45) - 90) * RAD;

		double cosdtmp = cos( dtmp );
		double sindtmp = sin( dtmp );
		// center (69,394)
		oapi::IVECTOR2 arrow[3];
		arrow[0].x = 69 - Round( 33 * cosdtmp );
		arrow[0].y = 394 - Round( 33 * sindtmp );
		arrow[1].x = 69 - Round( (22 * cosdtmp) + (6 * sindtmp) );
		arrow[1].y = 394 - Round( (22 * sindtmp) - (6 * cosdtmp) );
		arrow[2].x = 69 - Round( (22 * cosdtmp) - (6 * sindtmp) );
		arrow[2].y = 394 - Round( (22 * sindtmp) + (6 * cosdtmp) );

		skp->SetPen( skpLightGreenPen );
		skp->SetBrush( skpLightGreenBrush );
		skp->Polygon( arrow, 3 );

		skp->SetPen( skpLightGreenThickPen );
		skp->Line( 69, 394, 69 - Round( 31 * cosdtmp ), 394 - Round( 31 * sindtmp ) );
		return;
	}

	void MDU::AEPFD_GMETER_NZ( HDC hDC )
	{
		SelectObject( hDC, gdiSSUAFont_h11w9 );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 91, 395, "Nz", 2 );
		double dtmp = GetIDP()->GetNZ();

		SelectObject( hDC, gdiSSUBFont_h18w9 );
		SetTextColor( hDC, CR_WHITE );
		char cbuf[8];
		SetTextAlign( hDC, TA_RIGHT );
		sprintf_s( cbuf, 8, "%.1f", dtmp );
		TextOut( hDC, 111, 367, cbuf, strlen( cbuf ) );
		SetTextAlign( hDC, TA_LEFT );

		if (dtmp > 4) dtmp = 4;
		else if (dtmp < -1) dtmp = -1;
		dtmp = ((dtmp * 45) - 90) * RAD;

		double cosdtmp = cos( dtmp );
		double sindtmp = sin( dtmp );
		// center (69,394)
		POINT arrow[3];
		arrow[0].x = 69 - Round( 33 * cosdtmp );
		arrow[0].y = 394 - Round( 33 * sindtmp );
		arrow[1].x = 69 - Round( (22 * cosdtmp) + (6 * sindtmp) );
		arrow[1].y = 394 - Round( (22 * sindtmp) - (6 * cosdtmp) );
		arrow[2].x = 69 - Round( (22 * cosdtmp) - (6 * sindtmp) );
		arrow[2].y = 394 - Round( (22 * sindtmp) + (6 * cosdtmp) );

		SelectObject( hDC, gdiLightGreenPen );
		SelectObject( hDC, gdiLightGreenBrush );
		Polygon( hDC, arrow, 3 );

		SelectObject( hDC, gdiLightGreenThickPen );
		MoveToEx( hDC, 69, 394, NULL );
		LineTo( hDC, 69 - Round( 31 * cosdtmp ), 394 - Round( 31 * sindtmp ) );
		return;
	}

	void MDU::AEPFD_GMETER_NZ( oapi::Sketchpad2* skp )
	{
		skp->SetFont( skpSSUAFont_h11 );
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 91, 395, "Nz", 2 );
		double dtmp = GetIDP()->GetNZ();

		skp->SetFont( skpSSUBFont_h18 );
		skp->SetTextColor( CR_WHITE );
		char cbuf[8];
		skp->SetTextAlign( oapi::Sketchpad::RIGHT );
		sprintf_s( cbuf, 8, "%.1f", dtmp );
		skp->Text( 111, 367, cbuf, strlen( cbuf ) );
		skp->SetTextAlign( oapi::Sketchpad::LEFT );

		if (dtmp > 4) dtmp = 4;
		else if (dtmp < -1) dtmp = -1;
		dtmp = ((dtmp * 45) - 90) * RAD;

		double cosdtmp = cos( dtmp );
		double sindtmp = sin( dtmp );
		// center (34.217)
		oapi::IVECTOR2 arrow[3];
		arrow[0].x = 69 - Round( 33 * cosdtmp );
		arrow[0].y = 394 - Round( 33 * sindtmp );
		arrow[1].x = 69 - Round( (22 * cosdtmp) + (6 * sindtmp) );
		arrow[1].y = 394 - Round( (22 * sindtmp) - (6 * cosdtmp) );
		arrow[2].x = 69 - Round( (22 * cosdtmp) - (6 * sindtmp) );
		arrow[2].y = 394 - Round( (22 * sindtmp) + (6 * cosdtmp) );

		skp->SetPen( skpLightGreenPen );
		skp->SetBrush( skpLightGreenBrush );
		skp->Polygon( arrow, 3 );

		skp->SetPen( skpLightGreenThickPen );
		skp->Line( 69, 394, 69 - Round( 31 * cosdtmp ), 394 - Round( 31 * sindtmp ) );
		return;
	}

	void MDU::AEPFD_HACCEL( HDC hDC )
	{
		SelectObject( hDC, gdiLightGrayPen );
		SelectObject( hDC, gdiWhiteBrush );
		Rectangle( hDC, 467, 300, 479, 452 );
		
		MoveToEx( hDC, 479, 306, NULL );
		LineTo( hDC, 485, 306 );
		MoveToEx( hDC, 479, 320, NULL );
		LineTo( hDC, 485, 320 );
		MoveToEx( hDC, 479, 334, NULL );
		LineTo( hDC, 485, 334 );
		MoveToEx( hDC, 479, 348, NULL );
		LineTo( hDC, 485, 348 );
		MoveToEx( hDC, 479, 362, NULL );
		LineTo( hDC, 485, 362 );
		MoveToEx( hDC, 479, 376, NULL );
		LineTo( hDC, 485, 376 );
		MoveToEx( hDC, 479, 390, NULL );
		LineTo( hDC, 485, 390 );
		MoveToEx( hDC, 479, 404, NULL );
		LineTo( hDC, 485, 404 );
		MoveToEx( hDC, 479, 418, NULL );
		LineTo( hDC, 485, 418 );
		MoveToEx( hDC, 479, 432, NULL );
		LineTo( hDC, 485, 432 );
		MoveToEx( hDC, 479, 446, NULL );
		LineTo( hDC, 485, 446 );
		
		SelectObject( hDC, gdiDarkGrayBrush );
		Rectangle( hDC, 467, 376, 479, 452 );

		SetPixel( hDC, 471, 283, CR_LIGHT_GRAY );
		SetPixel( hDC, 473, 283, CR_LIGHT_GRAY );
		SelectObject( hDC, gdiSSUAFont_h11w9 );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 468, 286, "H", 1 );
		SelectObject( hDC, gdiSSUBFont_h12w7 );
		SetTextColor( hDC, CR_WHITE );
		TextOut( hDC, 489, 299, "10", 2 );
		TextOut( hDC, 489, 369, "0", 1 );
		TextOut( hDC, 489, 439, "-10", 3 );

		double dtmp = GetIDP()->GetVacc();
		if (dtmp > 10) dtmp = 10;
		else if (dtmp < -10) dtmp = -10;

		POINT tri[3];// starts at tip moving cw
		tri[0].x = 475;
		tri[0].y = 376 - Round( dtmp * 7 );
		tri[1].x = 461;
		tri[1].y = tri[0].y + 8;
		tri[2].x = 461;
		tri[2].y = tri[0].y - 8;

		SelectObject( hDC, gdiLightGreenPen );
		SelectObject( hDC, gdiLightGreenBrush );
		Polygon( hDC, tri, 3 );
		return;
	}

	void MDU::AEPFD_HACCEL( oapi::Sketchpad2* skp )
	{
		skp->SetPen( skpLightGrayPen );
		skp->SetBrush( skpWhiteBrush );
		skp->Rectangle( 467, 300, 479, 452 );
		
		skp->Line( 479, 306, 485, 306 );
		skp->Line( 479, 320, 485, 320 );
		skp->Line( 479, 334, 485, 334 );
		skp->Line( 479, 348, 485, 348 );
		skp->Line( 479, 362, 485, 362 );
		skp->Line( 479, 376, 485, 376 );
		skp->Line( 479, 390, 485, 390 );
		skp->Line( 479, 404, 485, 404 );
		skp->Line( 479, 418, 485, 418 );
		skp->Line( 479, 432, 485, 432 );
		skp->Line( 479, 446, 485, 446 );

		skp->SetBrush( skpDarkGrayBrush );
		skp->Rectangle( 467, 376, 479, 452 );

		skp->Pixel( 471, 284, CR_LIGHT_GRAY );
		skp->Pixel( 473, 284, CR_LIGHT_GRAY );
		skp->SetFont( skpSSUAFont_h11 );
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 468, 286, "H", 1 );
		skp->SetFont( skpSSUBFont_h12 );
		skp->SetTextColor( CR_WHITE );
		skp->Text( 489, 299, "10", 2 );
		skp->Text( 489, 369, "0", 1 );
		skp->Text( 489, 439, "-10", 3 );

		double dtmp = GetIDP()->GetVacc();
		if (dtmp > 10) dtmp = 10;
		else if (dtmp < -10) dtmp = -10;

		oapi::IVECTOR2 tri[3];// starts at tip moving cw
		tri[0].x = 475;
		tri[0].y = 376 - Round( dtmp * 7 );
		tri[1].x = 461;
		tri[1].y = tri[0].y + 8;
		tri[2].x = 461;
		tri[2].y = tri[0].y - 8;

		skp->SetPen( skpLightGreenPen );
		skp->SetBrush( skpLightGreenBrush );
		skp->Polygon( tri, 3 );
		return;
	}

	void MDU::AEPFD_RANGERW( HDC hDC )
	{
		SelectObject( hDC, gdiLightGrayPen );
		SelectObject( hDC, gdiBlackBrush );
		Rectangle( hDC, 367, 384, 408, 401 );
		SelectObject( hDC, gdiSSUAFont_h11w9 );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 367, 371, GetIDP()->GetSelectedRunway().c_str(), 5 );
		SelectObject( hDC, gdiSSUBFont_h12w7 );
		SetTextColor( hDC, CR_WHITE );
		SetTextAlign( hDC, TA_RIGHT );
		double dtmp = GetIDP()->GetRangeToRunway();
		char cbuf[8];
		if (dtmp > 1000) sprintf_s( cbuf, 8, "%4.0f", dtmp );// HACK max should be 999.9, but this way is better for testing
		else sprintf_s( cbuf, 8, "%5.1f", dtmp );
		TextOut( hDC, 405, 385, cbuf, strlen( cbuf ) );
		SetTextAlign( hDC, TA_LEFT );
		return;
	}

	void MDU::AEPFD_RANGERW( oapi::Sketchpad2* skp )
	{
		skp->SetPen( skpLightGrayPen );
		skp->SetBrush( skpBlackBrush );
		skp->Rectangle( 367, 384, 408, 401 );
		skp->SetFont( skpSSUAFont_h11 );
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 367, 371, GetIDP()->GetSelectedRunway().c_str(), 5 );
		skp->SetFont( skpSSUBFont_h12 );
		skp->SetTextColor( CR_WHITE );
		skp->SetTextAlign( oapi::Sketchpad::RIGHT );
		double dtmp = GetIDP()->GetRangeToRunway();
		char cbuf[8];
		if (dtmp > 1000) sprintf_s( cbuf, 8, "%4.0f", dtmp );// HACK max should be 999.9, but this way is better for testing
		else sprintf_s( cbuf, 8, "%5.1f", dtmp );
		skp->Text( 405, 385, cbuf, strlen( cbuf ) );
		skp->SetTextAlign( oapi::Sketchpad::LEFT );
		return;
	}

	void MDU::AEPFD_RANGEHACC( HDC hDC )
	{
		SelectObject( hDC, gdiLightGrayPen );
		SelectObject( hDC, gdiBlackBrush );
		Rectangle( hDC, 367, 425, 408, 442 );
		SelectObject( hDC, gdiSSUAFont_h11w9 );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 367, 412, "HAC-C", 5 );
		SelectObject( hDC, gdiSSUBFont_h12w7 );
		SetTextColor( hDC, CR_WHITE );
		SetTextAlign( hDC, TA_RIGHT );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%5.1f", GetIDP()->GetDistanceToHACCenter() );
		TextOut( hDC, 405, 426, cbuf, strlen( cbuf ) );
		SetTextAlign( hDC, TA_LEFT );
		return;
	}

	void MDU::AEPFD_RANGEHACC( oapi::Sketchpad2* skp )
	{
		skp->SetPen( skpLightGrayPen );
		skp->SetBrush( skpBlackBrush );
		skp->Rectangle( 367, 425, 408, 442 );
		skp->SetFont( skpSSUAFont_h11 );
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 367, 412, "HAC-C", 5 );
		skp->SetFont( skpSSUBFont_h12 );
		skp->SetTextColor( CR_WHITE );
		skp->SetTextAlign( oapi::Sketchpad::RIGHT );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%5.1f", GetIDP()->GetDistanceToHACCenter() );
		skp->Text( 405, 426, cbuf, strlen( cbuf ) );
		skp->SetTextAlign( oapi::Sketchpad::LEFT );
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
		SelectObject( hDC, gdiLightGrayPen );
		SelectObject( hDC, gdiBlackBrush );
		Rectangle( hDC, 434, 290, 448, 442 );
		SelectObject( hDC, gdiLightGrayBrush );
		::Ellipse( hDC, 437, 297, 445, 305 );
		::Ellipse( hDC, 437, 329, 445, 337 );
		::Ellipse( hDC, 437, 395, 445, 403 );
		::Ellipse( hDC, 437, 427, 445, 435 );
		MoveToEx( hDC, 434, 366, NULL );
		LineTo( hDC, 448, 366 );

		SelectObject( hDC, gdiSSUBFont_h12w7 );

		if (Altitude_ft < 1500)
		{
			TextOut( hDC, 434, 276, "1K", 2 );
			TextOut( hDC, 434, 442, "1K", 2 );
			// no tracking -> print GS label
			SelectObject( hDC, gdiRedPen );
			SelectObject( hDC, gdiRedBrush );
			Rectangle( hDC, 427, 357, 455, 374 );
			SetTextColor( hDC, CR_BLACK );
			TextOut( hDC, 434, 359, "GS", 2 );
		}
		else
		{
			POINT tri[3];
			tri[0].x = 441;// starts at tip moving cw
			tri[1].x = 457;
			tri[2].x = 457;
			double dtmp = GetIDP()->GetGlideSlopeDistance();// get dist ABOVE glide path

			if (GetIDP()->GetApproachAndLandState() == false)
			{
				// TAEM
				TextOut( hDC, 434, 276, "5K", 2 );
				TextOut( hDC, 434, 442, "5K", 2 );

				if (dtmp > 5000)
				{
					if (GetFlash() == false) return;
					tri[0].y = 431;
				}
				else if (dtmp < -5000)
				{
					if (GetFlash() == false) return;
					tri[0].y = 301;
				}
				else tri[0].y = 366 + Round( dtmp * 0.013 );
			}
			else
			{
				// A/L
				TextOut( hDC, 434, 276, "1K", 2 );
				TextOut( hDC, 434, 442, "1K", 2 );

				if (dtmp > 1000)
				{
					if (GetFlash() == false) return;
					tri[0].y = 431;
				}
				else if (dtmp < -1000)
				{
					if (GetFlash() == false) return;
					tri[0].y = 301;
				}
				else tri[0].y = 366 + Round( dtmp * 0.065 );
			}

			tri[1].y = tri[0].y - 10;
			tri[2].y = tri[0].y + 10;
			SelectObject( hDC, gdiLightGreenPen );
			SelectObject( hDC, gdiLightGreenBrush );
			Polygon( hDC, tri, 3 );
		}
		return;
	}

	void MDU::AEPFD_GSI( oapi::Sketchpad2* skp, double Altitude_ft )
	{
		skp->SetPen( skpLightGrayPen );
		skp->SetBrush( skpBlackBrush );
		skp->Rectangle( 434, 290, 448, 442 );
		skp->SetBrush( skpLightGrayBrush );
		skp->Ellipse( 437, 297, 445, 305 );
		skp->Ellipse( 437, 329, 445, 337 );
		skp->Ellipse( 437, 395, 445, 403 );
		skp->Ellipse( 437, 427, 445, 435 );
		skp->Line( 434, 366, 448, 366 );

		skp->SetFont( skpSSUBFont_h12 );

		if (Altitude_ft < 1500)
		{
			skp->Text( 434, 276, "1K", 2 );
			skp->Text( 434, 442, "1K", 2 );
			// no tracking -> print GS label
			skp->SetPen( skpRedPen );
			skp->SetBrush( skpRedBrush );
			skp->Rectangle( 427, 357, 455, 374 );
			skp->SetTextColor( CR_BLACK );
			skp->Text( 434, 359, "GS", 2 );
		}
		else
		{
			oapi::IVECTOR2 tri[3];
			tri[0].x = 441;// starts at tip moving cw
			tri[1].x = 457;
			tri[2].x = 457;
			double dtmp = GetIDP()->GetGlideSlopeDistance();// get dist ABOVE glide path

			if (GetIDP()->GetApproachAndLandState() == false)
			{
				// TAEM
				skp->Text( 434, 276, "5K", 2 );
				skp->Text( 434, 442, "5K", 2 );

				if (dtmp > 5000)
				{
					if (GetFlash() == false) return;
					tri[0].y = 431;
				}
				else if (dtmp < -5000)
				{
					if (GetFlash() == false) return;
					tri[0].y = 301;
				}
				else tri[0].y = 366 + Round( dtmp * 0.013 );
			}
			else
			{
				// A/L
				skp->Text( 434, 276, "1K", 2 );
				skp->Text( 434, 442, "1K", 2 );

				if (dtmp > 1000)
				{
					if (GetFlash() == false) return;
					tri[0].y = 431;
				}
				else if (dtmp < -1000)
				{
					if (GetFlash() == false) return;
					tri[0].y = 301;
				}
				else tri[0].y = 366 + Round( dtmp * 0.065 );
			}

			tri[1].y = tri[0].y - 10;
			tri[2].y = tri[0].y + 10;
			skp->SetPen( skpLightGreenPen );
			skp->SetBrush( skpLightGreenBrush );
			skp->Polygon( tri, 3 );
		}
		return;
	}
};
