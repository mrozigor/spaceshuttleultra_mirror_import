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
				Tape_MVR_KEAS( skp, MachNumber );
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
				Tape_MVI_KEAS( skp, MachNumber );
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
				Tape_MVI_KEAS( skp, MachNumber );
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
				Tape_MVR_KEAS( skp, MachNumber );
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
				if (MachNumber >= 0.9) Tape_MVR_KEAS( skp, MachNumber );
				else Tape_KEAS_MVR( skp, MachNumber );
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
				if (0) Tape_MVR_KEAS( skp, MachNumber );// TODO PPA
				Tape_MVI_KEAS( skp, MachNumber );
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
				Tape_MVR_KEAS( skp, MachNumber );
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
				if (MachNumber >= 0.9) Tape_MVR_KEAS( skp, MachNumber );
				else Tape_KEAS_MVR( skp, MachNumber );
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
		//SelectObject( hDC_Tapes, gdiTahomaFont_h10w4 );

		// draw tapes
		int offset = 110;
		int offset_top = 109;
		int offset_bottom = 111;
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
		// 23º in window (1º = 9.435px)
		// 3396px long + offsets
		hDC_Tape_Alpha = CreateCompatibleDC( GetDC( NULL ) );
		/*HBITMAP*/ hBM = CreateCompatibleBitmap( GetDC( NULL ), 43, 3396 + offset_top + offset_bottom );
		SelectObject( hDC_Tape_Alpha, hBM );

		SelectObject( hDC_Tape_Alpha, gdiWhitePen );
		SelectObject( hDC_Tape_Alpha, gdiWhiteBrush );
		Rectangle( hDC_Tape_Alpha, 0, 0, 43, 1698 + offset_top );

		SelectObject( hDC_Tape_Alpha, gdiDarkGrayPen );
		SelectObject( hDC_Tape_Alpha, gdiDarkGrayBrush );
		Rectangle( hDC_Tape_Alpha, 0, 1698 + 1 + offset_top, 43, 3396 + offset_top + offset_bottom );

		SetTextColor( hDC_Tape_Alpha, CR_BLACK );
		SetTextAlign( hDC_Tape_Alpha, TA_CENTER );
		SelectObject( hDC_Tape_Alpha, gdiSSUBFont_h18w9 );
		SetBkMode( hDC_Tape_Alpha, TRANSPARENT );
		SelectObject( hDC_Tape_Alpha, gdiBlackPen );

		sfh_Tape_Alpha = oapiCreateSurface( 43, 3396 + offset_top + offset_bottom );
		oapi::Sketchpad* skp_Tape_Alpha = oapiGetSketchpad( sfh_Tape_Alpha );

		skp_Tape_Alpha->SetPen( skpWhitePen );
		skp_Tape_Alpha->SetBrush( skpWhiteBrush );
		skp_Tape_Alpha->Rectangle( 0, 0, 43, 1698 + offset_top );

		skp_Tape_Alpha->SetPen( skpDarkGrayPen );
		skp_Tape_Alpha->SetBrush( skpDarkGrayBrush );
		skp_Tape_Alpha->Rectangle( 0, 1698 + 1 + offset_top, 43, 3396 + offset_top + offset_bottom );

		skp_Tape_Alpha->SetTextColor( CR_BLACK );
		skp_Tape_Alpha->SetTextAlign( oapi::Sketchpad::CENTER );
		skp_Tape_Alpha->SetFont( skpSSUBFont_h18 );
		skp_Tape_Alpha->SetBackgroundColor( oapi::Sketchpad::BK_TRANSPARENT );
		skp_Tape_Alpha->SetPen( skpBlackPen );

		for (int i = 180; i >= -180; i--)
		{
			if (i == 0)
			{
				SetTextColor( hDC_Tape_Alpha, CR_WHITE );
				SelectObject( hDC_Tape_Alpha, gdiWhitePen );

				skp_Tape_Alpha->SetTextColor( CR_WHITE );
				skp_Tape_Alpha->SetPen( skpWhitePen );
			}

			y = Round( ((180 - i) * 9.435) + offset );

			MoveToEx( hDC_Tape_Alpha, 35, y, NULL );
			LineTo( hDC_Tape_Alpha, 43, y );

			skp_Tape_Alpha->Line( 35, y, 43, y );

			if ((i % 5) == 0)
			{
				sprintf_s( cbuf, 8, "%4d", i );
				if (i > -100)
				{
					TextOut( hDC_Tape_Alpha, 19, y - 11, cbuf, strlen( cbuf ) );

					skp_Tape_Alpha->Text( 19, y - 11, cbuf, strlen( cbuf ) );
				}
				else
				{
					TextOut( hDC_Tape_Alpha, 16, y - 11, cbuf, strlen( cbuf ) );

					skp_Tape_Alpha->Text( 16, y - 11, cbuf, strlen( cbuf ) );
				}
			}
		}
		oapiReleaseSketchpad( skp_Tape_Alpha );

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
		HBITMAP hBM = CreateCompatibleBitmap( GetDC( NULL ), 192, 192 );
		SelectObject( hDC_ADI, hBM );
		SelectObject( hDC_ADI, gdiSSUBFont_h12w7 );

		hDC_ADIMASK = CreateCompatibleDC( hDC_ADI );
		hBM = CreateCompatibleBitmap( hDC_ADI, 192, 192 );
		SelectObject( hDC_ADIMASK, hBM );
		SelectObject( hDC_ADIMASK, gdiWhitePen );
		SelectObject( hDC_ADIMASK, gdiWhiteBrush );
		::Ellipse( hDC_ADIMASK, 1, 1, 191, 191 );

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
		Rectangle( hDC, 13, 54, 59, 274 );
		SelectObject( hDC, gdiLightGrayPen );
		Rectangle( hDC, 13, 285, 59, 309 );

		SelectObject( hDC, gdiSSUAFont_h11w9 );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 17, 39, "KEAS", 4 );
		TextOut( hDC, 17, 315, "M/VR", 4 );

		double tapekeas = keas;
		if (tapekeas > 500) tapekeas = 500;
		//BitBlt( hDC, 10, 40, 22, 114, hDC_Tapes, 23, 1900 - Round( tapekeas * 3.8 ), SRCCOPY );

		Rectangle( hDC, 13, 150, 59, 175 );
		
		SelectObject( hDC, gdiSSUBFont_h18w9 );
		SetTextColor( hDC, CR_WHITE );
		SetTextAlign( hDC, TA_RIGHT );
		sprintf_s( cbuf, 8, "%3.0f", keas );
		TextOut( hDC, 49, 151, cbuf, strlen( cbuf ) );
		sprintf_s( cbuf, 8, "%5.2f", MachNumber );
		TextOut( hDC, 53, 285, cbuf, strlen( cbuf ) );
		SetTextAlign( hDC, TA_LEFT );
		return;
	}
	
	void MDU::Tape_KEAS_MVR( oapi::Sketchpad2* skp, double MachNumber )
	{
		char cbuf[8];
		double keas = sqrt( STS()->GetDynPressure() * PA2PSF ) * 17.18;
		skp->SetPen( skpWhitePen );
		skp->SetBrush( skpBlackBrush );
		skp->Rectangle( 13, 54, 59, 274 );
		skp->SetPen( skpLightGrayPen );
		skp->Rectangle( 13, 285, 59, 309 );

		skp->SetFont( skpSSUAFont_h11 );
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 17, 39, "KEAS", 4 );
		skp->Text( 17, 315, "M/VR", 4 );

		double tapekeas = keas;
		if (tapekeas > 500) tapekeas = 500;
		//BitBlt( hDC, 10, 40, 22, 114, hDC_Tapes, 23, 1900 - Round( tapekeas * 3.8 ), SRCCOPY );

		skp->Rectangle( 13, 150, 59, 175 );
		
		skp->SetFont( skpSSUBFont_h18 );
		skp->SetTextColor( CR_WHITE );
		skp->SetTextAlign( oapi::Sketchpad::RIGHT );
		sprintf_s( cbuf, 8, "%3.0f", keas );
		skp->Text( 49, 151, cbuf, strlen( cbuf ) );
		sprintf_s( cbuf, 8, "%5.2f", MachNumber );
		skp->Text( 53, 285, cbuf, strlen( cbuf ) );
		skp->SetTextAlign( oapi::Sketchpad::LEFT );
		return;
	}

	void MDU::Tape_MVR_KEAS( HDC hDC, double MachNumber )
	{
		char cbuf[8];
		int pos;
		double keas = sqrt( STS()->GetDynPressure() * PA2PSF ) * 17.18;
		SelectObject( hDC, gdiWhitePen );
		SelectObject( hDC, gdiBlackBrush );
		Rectangle( hDC, 13, 54, 59, 274 );
		SelectObject( hDC, gdiLightGrayPen );
		Rectangle( hDC, 13, 285, 59, 309 );

		SelectObject( hDC, gdiSSUAFont_h11w9 );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 17, 39, "M/VR", 4 );
		TextOut( hDC, 17, 315, "KEAS", 4 );

		SelectObject( hDC, gdiSSUBFont_h18w9 );
		SetTextColor( hDC, CR_WHITE );
		SetTextAlign( hDC, TA_RIGHT );
		if (MachNumber > 4)
		{
			double vr = STS()->GetAirspeed() * MPS2FPS;

			pos = 3078 - Round( vr * 0.114 );
			//BitBlt( hDC, 10, 40, 22, 114, hDC_Tapes, 0, pos, SRCCOPY );

			Rectangle( hDC, 13, 150, 59, 175 );
			sprintf_s( cbuf, 8, "%5.0f", vr );// ft
			TextOut( hDC, 57, 151, cbuf, strlen( cbuf ) );
		}
		else
		{
			pos = 3534 - Round( MachNumber * 228 );
			//BitBlt( hDC, 10, 40, 22, 114, hDC_Tapes, 0, pos, SRCCOPY );

			Rectangle( hDC, 13, 150, 59, 175 );
			sprintf_s( cbuf, 8, "%5.2f", MachNumber );// Mach
			TextOut( hDC, 57, 151, cbuf, strlen( cbuf ) );
		}
		sprintf_s( cbuf, 8, "%3.0f", keas );
		TextOut( hDC, 49, 285, cbuf, strlen( cbuf ) );
		SetTextAlign( hDC, TA_LEFT );
		return;
	}
	
	void MDU::Tape_MVR_KEAS( oapi::Sketchpad2* skp, double MachNumber )
	{
		char cbuf[8];
		int pos;
		double keas = sqrt( STS()->GetDynPressure() * PA2PSF ) * 17.18;
		skp->SetPen( skpWhitePen );
		skp->SetBrush( skpBlackBrush );
		skp->Rectangle( 13, 54, 59, 274 );
		skp->SetPen( skpLightGrayPen );
		skp->Rectangle( 13, 285, 59, 309 );

		skp->SetFont( skpSSUAFont_h11 );
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 17, 39, "M/VR", 4 );
		skp->Text( 17, 315, "KEAS", 4 );

		skp->SetFont( skpSSUBFont_h18 );
		skp->SetTextColor( CR_WHITE );
		skp->SetTextAlign( oapi::Sketchpad::RIGHT );
		if (MachNumber > 4)
		{
			double vr = STS()->GetAirspeed() * MPS2FPS;

			pos = 3078 - Round( vr * 0.114 );
			//BitBlt( hDC, 10, 40, 22, 114, hDC_Tapes, 0, pos, SRCCOPY );

			skp->Rectangle( 13, 150, 59, 175 );
			sprintf_s( cbuf, 8, "%5.0f", vr );// ft
			skp->Text( 57, 151, cbuf, strlen( cbuf ) );
		}
		else
		{
			pos = 3534 - Round( MachNumber * 228 );
			//BitBlt( hDC, 10, 40, 22, 114, hDC_Tapes, 0, pos, SRCCOPY );

			skp->Rectangle( 13, 150, 59, 175 );
			sprintf_s( cbuf, 8, "%5.2f", MachNumber );// Mach
			skp->Text( 57, 151, cbuf, strlen( cbuf ) );
		}
		sprintf_s( cbuf, 8, "%3.0f", keas );
		skp->Text( 49, 285, cbuf, strlen( cbuf ) );
		skp->SetTextAlign( oapi::Sketchpad::LEFT );
		return;
	}

	void MDU::Tape_MVI_KEAS( HDC hDC, double MachNumber )
	{
		char cbuf[8];
		int pos;
		double keas = sqrt( STS()->GetDynPressure() * PA2PSF ) * 17.18;
		SelectObject( hDC, gdiWhitePen );
		SelectObject( hDC, gdiBlackBrush );
		Rectangle( hDC, 13, 54, 59, 274 );
		SelectObject( hDC, gdiLightGrayPen );
		Rectangle( hDC, 13, 285, 59, 309 );

		SelectObject( hDC, gdiSSUAFont_h11w9 );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 17, 39, "M/VI", 4 );
		TextOut( hDC, 17, 315, "KEAS", 4 );

		SelectObject( hDC, gdiSSUBFont_h18w9 );
		SetTextColor( hDC, CR_WHITE );
		SetTextAlign( hDC, TA_RIGHT );
		if (MachNumber > 4)
		{
			VECTOR3 v3vi;
			STS()->GetRelativeVel( STS()->GetSurfaceRef(), v3vi );
			double vi = length( v3vi ) * MPS2FPS;
			
			pos = 3078 - Round( vi * 0.114 );
			//BitBlt( hDC, 10, 40, 22, 114, hDC_Tapes, 0, pos, SRCCOPY );

			Rectangle( hDC, 13, 150, 59, 175 );
			sprintf_s( cbuf, 8, "%5.0f", vi );// ft
			TextOut( hDC, 57, 151, cbuf, strlen( cbuf ) );
		}
		else
		{
			pos = 3534 - Round( MachNumber * 228 );
			//BitBlt( hDC, 10, 40, 22, 114, hDC_Tapes, 0, pos, SRCCOPY );

			Rectangle( hDC, 13, 150, 59, 175 );
			sprintf_s( cbuf, 8, "%5.2f", MachNumber );// Mach
			TextOut( hDC, 57, 151, cbuf, strlen( cbuf ) );
		}
		sprintf_s( cbuf, 8, "%3.0f", keas );
		TextOut( hDC, 49, 285, cbuf, strlen( cbuf ) );
		SetTextAlign( hDC, TA_LEFT );
		return;
	}
	
	void MDU::Tape_MVI_KEAS( oapi::Sketchpad2* skp, double MachNumber )
	{
		char cbuf[8];
		int pos;
		double keas = sqrt( STS()->GetDynPressure() * PA2PSF ) * 17.18;
		skp->SetPen( skpWhitePen );
		skp->SetBrush( skpBlackBrush );
		skp->Rectangle( 13, 54, 59, 274 );
		skp->SetPen( skpLightGrayPen );
		skp->Rectangle( 13, 285, 59, 309 );

		skp->SetFont( skpSSUAFont_h11 );
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 17, 39, "M/VI", 4 );
		skp->Text( 17, 315, "KEAS", 4 );

		skp->SetFont( skpSSUBFont_h18 );
		skp->SetTextColor( CR_WHITE );
		skp->SetTextAlign( oapi::Sketchpad::RIGHT );
		if (MachNumber > 4)
		{
			VECTOR3 v3vi;
			STS()->GetRelativeVel( STS()->GetSurfaceRef(), v3vi );
			double vi = length( v3vi ) * MPS2FPS;
			
			pos = 3078 - Round( vi * 0.114 );
			//BitBlt( hDC, 10, 40, 22, 114, hDC_Tapes, 0, pos, SRCCOPY );

			skp->Rectangle( 13, 150, 59, 175 );
			sprintf_s( cbuf, 8, "%5.0f", vi );// ft
			skp->Text( 57, 151, cbuf, strlen( cbuf ) );
		}
		else
		{
			pos = 3534 - Round( MachNumber * 228 );
			//BitBlt( hDC, 10, 40, 22, 114, hDC_Tapes, 0, pos, SRCCOPY );

			skp->Rectangle( 13, 150, 59, 175 );
			sprintf_s( cbuf, 8, "%5.2f", MachNumber );// Mach
			skp->Text( 57, 151, cbuf, strlen( cbuf ) );
		}
		sprintf_s( cbuf, 8, "%3.0f", keas );
		skp->Text( 49, 285, cbuf, strlen( cbuf ) );
		skp->SetTextAlign( oapi::Sketchpad::LEFT );
		return;
	}

	void MDU::Tape_Alpha( HDC hDC, double MachNumber )
	{
		SelectObject( hDC, gdiSSUAFont_h11w9 );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%c", 254 );
		TextOut( hDC, 82, 39, cbuf, 1 );

		SelectObject( hDC, gdiWhitePen );
		SelectObject( hDC, gdiBlackBrush );
		Rectangle( hDC, 68, 54, 114, 274 );

		double alpha = STS()->GetAOA() * DEG;

		BitBlt( hDC, 69, 55, 43, 217, hDC_Tape_Alpha, 0, 1698 - Round( alpha * 9.435 ), SRCCOPY );

		if ((MachNumber > 0) && (MachNumber < 3))
		{
			// max L/D (linear aprox)
			double maxLD;
			if (MachNumber < 0.9) maxLD = 10.5;
			else if (MachNumber < 1.1) maxLD = (MachNumber * 15) - 3;
			else if (MachNumber < 2) maxLD = (MachNumber * 1.6667) + 11.6667;
			else maxLD = (MachNumber * 2) + 11;

			maxLD -= alpha;
			if (fabs( maxLD ) < 11)
			{
				int pos = Round( maxLD * 9.435 );
				POINT diamond[4];
				diamond[0].x = 103;// start at top moving cw
				diamond[0].y = 155 - pos;
				diamond[1].x = 111;
				diamond[1].y = 163 - pos;
				diamond[2].x = 103;
				diamond[2].y = 171 - pos;
				diamond[3].x = 95;
				diamond[3].y = 163 - pos;
				SelectObject( hDC, gdiMagentaBrush );
				SelectObject( hDC, gdiBlackPen );
				Polygon( hDC, diamond, 4 );

				diamond[0].y += 6;
				diamond[1].x = 105;
				diamond[2].y -= 6;
				diamond[3].x = 101;
				SelectObject( hDC, gdiBlackBrush );
				Polygon( hDC, diamond, 4 );
			}
		}

		SelectObject( hDC, gdiLightGrayPen );
		SelectObject( hDC, gdiBlackBrush );
		POINT poly[5] = {{111,163},{104,175},{64,175},{64,150},{104,150}};// start at tip moving cw
		Polygon( hDC, poly, 5 );
		
		SelectObject( hDC, gdiSSUBFont_h18w9 );
		SetTextColor( hDC, CR_WHITE );
		SetTextAlign( hDC, TA_RIGHT );
		if (alpha >= -100) sprintf_s( cbuf, 8, "%4.1f", alpha );
		else sprintf_s( cbuf, 8, "%4.0f", alpha );
		TextOut( hDC, 108, 151, cbuf, strlen( cbuf ) );
		SetTextAlign( hDC, TA_LEFT );
		return;
	}
	
	void MDU::Tape_Alpha( oapi::Sketchpad2* skp, double MachNumber )
	{
		skp->SetFont( skpSSUAFont_h11 );
		skp->SetTextColor( CR_LIGHT_GRAY );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%c", 254 );
		skp->Text( 82, 39, cbuf, 1 );

		skp->SetPen( skpWhitePen );
		skp->SetBrush( skpBlackBrush );
		skp->Rectangle( 68, 54, 114, 274 );

		double alpha = STS()->GetAOA() * DEG;

		RECT src;
		src.left = 0;
		src.top = 1698 - Round( alpha * 9.435 );
		src.right = 43;
		src.bottom = src.top + 217;
		skp->CopyRect( sfh_Tape_Alpha, &src, 69, 56 );

		if ((MachNumber > 0) && (MachNumber < 3))
		{
			// max L/D (linear aprox)
			double maxLD;
			if (MachNumber < 0.9) maxLD = 10.5;
			else if (MachNumber < 1.1) maxLD = (MachNumber * 15) - 3;
			else if (MachNumber < 2) maxLD = (MachNumber * 1.6667) + 11.6667;
			else maxLD = (MachNumber * 2) + 11;

			maxLD -= alpha;
			if (fabs( maxLD ) < 11)
			{
				int pos = Round( maxLD * 9.435 );
				oapi::IVECTOR2 diamond[4];
				diamond[0].x = 103;// start at top moving cw
				diamond[0].y = 155 - pos;
				diamond[1].x = 111;
				diamond[1].y = 163 - pos;
				diamond[2].x = 103;
				diamond[2].y = 171 - pos;
				diamond[3].x = 95;
				diamond[3].y = 163 - pos;
				skp->SetBrush( skpMagentaBrush );
				skp->SetPen( skpBlackPen );
				//skp->Polygon( diamond, 4 );

				diamond[0].y += 6;
				diamond[1].x = 105;
				diamond[2].y -= 6;
				diamond[3].x = 101;
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
		if (alpha >= -100) sprintf_s( cbuf, 8, "%4.1f", alpha );
		else sprintf_s( cbuf, 8, "%4.0f", alpha );
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
			TextOut( hDC, 438, 155, "K", 1 );
		}
		else
		{
			sprintf_s( cbuf, 8, "%3.0f", Altitude_NM );
			TextOut( hDC, 430, 151, cbuf, strlen( cbuf ) );
			SelectObject( hDC, gdiSSUBFont_h12w7 );
			SetTextColor( hDC, CR_LIGHT_GRAY );
			TextOut( hDC, 438, 155, "M", 1 );
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
			skp->Text( 438, 155, "K", 1 );
		}
		else
		{
			sprintf_s( cbuf, 8, "%3.0f", Altitude_NM );
			skp->Text( 430, 151, cbuf, strlen( cbuf ) );
			skp->SetFont( skpSSUBFont_h12 );
			skp->SetTextColor( CR_LIGHT_GRAY );
			skp->Text( 438, 155, "M", 1 );
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
		TextOut( hDC, 349, 14, "R", 1 );
		TextOut( hDC, 349, 29, "P", 1 );
		TextOut( hDC, 349, 44, "Y", 1 );

		SelectObject( hDC, gdiWhitePen );
		::Ellipse( hDC, 122, 48, 356, 282 );

		SelectObject( hDC, gdiLightGrayPen );
		SelectObject( hDC, gdiLightGrayBrush );
		POINT diamond[4];
		diamond[0].x = 239;
		diamond[0].y = 53;
		diamond[1].x = 244;
		diamond[1].y = 62;
		diamond[2].x = 239;
		diamond[2].y = 71;
		diamond[3].x = 234;
		diamond[3].y = 62;
		Polygon( hDC, diamond, 4 );// top
		//diamond[0].x = 239;
		diamond[0].y = 259;
		//diamond[1].x = 244;
		diamond[1].y = 268;
		//diamond[2].x = 239;
		diamond[2].y = 277;
		//diamond[3].x = 234;
		diamond[3].y = 268;
		Polygon( hDC, diamond, 4 );// bottom
		diamond[0].x = 342;
		diamond[0].y = 160;
		diamond[1].x = 351;
		diamond[1].y = 165;
		diamond[2].x = 342;
		diamond[2].y = 170;
		diamond[3].x = 333;
		diamond[3].y = 165;
		Polygon( hDC, diamond, 4 );// right
		diamond[0].x = 136;
		//diamond[0].y = 160;
		diamond[1].x = 145;
		//diamond[1].y = 165;
		diamond[2].x = 136;
		//diamond[2].y = 170;
		diamond[3].x = 127;
		//diamond[3].y = 165;
		Polygon( hDC, diamond, 4 );// left
		
		// rate scales
		// top scale
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
		MoveToEx( hDC, 230, 65, NULL );// 5º/185º
		LineTo( hDC, 248, 265 );
		MoveToEx( hDC, 222, 67, NULL );// 10º/190º
		LineTo( hDC, 256, 263 );
		MoveToEx( hDC, 213, 68, NULL );// 15º/195º
		LineTo( hDC, 265, 262 );
		MoveToEx( hDC, 205, 71, NULL );// 20º/200º
		LineTo( hDC, 273, 259 );
		MoveToEx( hDC, 197, 74, NULL );// 25º/205º
		LineTo( hDC, 281, 256 );

		MoveToEx( hDC, 182, 83, NULL );// 35º/215º
		LineTo( hDC, 296, 247 );
		MoveToEx( hDC, 175, 88, NULL );// 40º/220º
		LineTo( hDC, 303, 242 );
		MoveToEx( hDC, 168, 94, NULL );// 45º/225º
		LineTo( hDC, 310, 236 );
		MoveToEx( hDC, 162, 101, NULL );// 50º/230º
		LineTo( hDC, 316, 229 );
		MoveToEx( hDC, 157, 108, NULL );// 55º/235º
		LineTo( hDC, 321, 222 );

		MoveToEx( hDC, 148, 123, NULL );// 65º/245º
		LineTo( hDC, 330, 207 );
		MoveToEx( hDC, 145, 131, NULL );// 70º/250º
		LineTo( hDC, 333, 199 );
		MoveToEx( hDC, 142, 139, NULL );// 75º/255º
		LineTo( hDC, 336, 191 );
		MoveToEx( hDC, 141, 148, NULL );// 80º/260º
		LineTo( hDC, 337, 182 );
		MoveToEx( hDC, 139, 156, NULL );// 85º/265º
		LineTo( hDC, 339, 174 );

		MoveToEx( hDC, 139, 174, NULL );// 95º/275º
		LineTo( hDC, 339, 156 );
		MoveToEx( hDC, 141, 182, NULL );// 100º/280º
		LineTo( hDC, 337, 148 );
		MoveToEx( hDC, 142, 191, NULL );// 105º/285º
		LineTo( hDC, 336, 139 );
		MoveToEx( hDC, 145, 199, NULL );// 110º/290º
		LineTo( hDC, 333, 131 );
		MoveToEx( hDC, 148, 207, NULL );// 115º/295º
		LineTo( hDC, 330, 123 );

		MoveToEx( hDC, 157, 222, NULL );// 125º/305º
		LineTo( hDC, 321, 108 );
		MoveToEx( hDC, 162, 229, NULL );// 130º/310º
		LineTo( hDC, 316, 101 );
		MoveToEx( hDC, 168, 236, NULL );// 135º/315º
		LineTo( hDC, 310, 94 );
		MoveToEx( hDC, 175, 242, NULL );// 140º/320º
		LineTo( hDC, 303, 88 );
		MoveToEx( hDC, 182, 247, NULL );// 145º/325º
		LineTo( hDC, 296, 83 );
		
		MoveToEx( hDC, 197, 256, NULL );// 155º/335º
		LineTo( hDC, 281, 74 );
		MoveToEx( hDC, 205, 259, NULL );// 160º/340º
		LineTo( hDC, 273, 71 );
		MoveToEx( hDC, 213, 262, NULL );// 165º/345º
		LineTo( hDC, 265, 68 );
		MoveToEx( hDC, 222, 263, NULL );// 170º/350º
		LineTo( hDC, 256, 67 );
		MoveToEx( hDC, 230, 265, NULL );// 175º/355º
		LineTo( hDC, 248, 65 );
		SelectObject( hDC, gdiLightGrayThickPen );
		MoveToEx( hDC, 187, 75, NULL );// 30º/210º
		LineTo( hDC, 291, 255 );
		MoveToEx( hDC, 149, 113, NULL );// 60º/240º
		LineTo( hDC, 329, 217 );
		MoveToEx( hDC, 149, 217, NULL );// 120º/300º
		LineTo( hDC, 329, 113 );
		MoveToEx( hDC, 187, 255, NULL );// 150º/330º
		LineTo( hDC, 291, 75 );

		// cover center for ADI SRCPAINT
		SelectObject( hDC, gdiBlackBrush );
		SelectObject( hDC, gdiBlackPen );
		::Ellipse( hDC, 144, 70, 334, 260 );

		TextOut( hDC, 294, 70, "33", 2 );
		TextOut( hDC, 323, 99, "30", 2 );
		TextOut( hDC, 323, 217, "24", 2 );
		TextOut( hDC, 294, 247, "21", 2 );
		TextOut( hDC, 171, 247, "15", 2 );
		TextOut( hDC, 142, 217, "12", 2 );
		TextOut( hDC, 142, 99, "06", 2 );
		TextOut( hDC, 171, 70, "03", 2 );

		// att needles
		// top scale
		SelectObject( hDC, gdiMagentaPen );
		SelectObject( hDC, gdiMagentaBrush );
		Arc( hDC, 129, 55, 349, 275, 286, 66, 192, 66 );
		MoveToEx( hDC, 192, 66, NULL );
		LineTo( hDC, 192, 72 );
		MoveToEx( hDC, 201, 62, NULL );
		LineTo( hDC, 201, 68 );
		MoveToEx( hDC, 211, 59, NULL );
		LineTo( hDC, 211, 65 );
		MoveToEx( hDC, 220, 57, NULL );
		LineTo( hDC, 220, 63 );
		MoveToEx( hDC, 230, 55, NULL );
		LineTo( hDC, 230, 61 );
		MoveToEx( hDC, 239, 55, NULL );
		LineTo( hDC, 239, 61 );
		MoveToEx( hDC, 248, 55, NULL );
		LineTo( hDC, 248, 61 );
		MoveToEx( hDC, 258, 57, NULL );
		LineTo( hDC, 258, 63 );
		MoveToEx( hDC, 267, 59, NULL );
		LineTo( hDC, 267, 65 );
		MoveToEx( hDC, 277, 62, NULL );
		LineTo( hDC, 277, 68 );
		MoveToEx( hDC, 286, 66, NULL );
		LineTo( hDC, 286, 72 );

		// side scale
		Arc( hDC, 129, 55, 349, 275, 338, 212, 338, 118 );
		MoveToEx( hDC, 338, 118, NULL );
		LineTo( hDC, 332, 118 );
		MoveToEx( hDC, 342, 127, NULL );
		LineTo( hDC, 336, 127 );
		MoveToEx( hDC, 345, 137, NULL );
		LineTo( hDC, 339, 137 );
		MoveToEx( hDC, 347, 146, NULL );
		LineTo( hDC, 341, 146 );
		MoveToEx( hDC, 349, 156, NULL );
		LineTo( hDC, 343, 156 );
		MoveToEx( hDC, 349, 165, NULL );
		LineTo( hDC, 343, 165 );
		MoveToEx( hDC, 349, 174, NULL );
		LineTo( hDC, 343, 174 );
		MoveToEx( hDC, 347, 184, NULL );
		LineTo( hDC, 341, 184 );
		MoveToEx( hDC, 345, 193, NULL );
		LineTo( hDC, 339, 193 );
		MoveToEx( hDC, 342, 203, NULL );
		LineTo( hDC, 336, 203 );
		MoveToEx( hDC, 338, 212, NULL );
		LineTo( hDC, 332, 212 );

		// bottom scale
		Arc( hDC, 129, 55, 349, 275, 192, 264, 286, 264 );
		MoveToEx( hDC, 192, 264, NULL );
		LineTo( hDC, 192, 258 );
		MoveToEx( hDC, 201, 268, NULL );
		LineTo( hDC, 201, 262 );
		MoveToEx( hDC, 211, 271, NULL );
		LineTo( hDC, 211, 265 );
		MoveToEx( hDC, 220, 273, NULL );
		LineTo( hDC, 220, 267 );
		MoveToEx( hDC, 230, 275, NULL );
		LineTo( hDC, 230, 269 );
		MoveToEx( hDC, 239, 275, NULL );
		LineTo( hDC, 239, 269 );
		MoveToEx( hDC, 248, 275, NULL );
		LineTo( hDC, 249, 269 );
		MoveToEx( hDC, 258, 273, NULL );
		LineTo( hDC, 258, 267 );
		MoveToEx( hDC, 267, 271, NULL );
		LineTo( hDC, 267, 265 );
		MoveToEx( hDC, 277, 268, NULL );
		LineTo( hDC, 277, 262 );
		MoveToEx( hDC, 286, 264, NULL );
		LineTo( hDC, 286, 258 );
		return;
	}

	void MDU::ADI_STATIC( oapi::Sketchpad2* skp )
	{
		skp->SetFont( skpSSUBFont_h12 );
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 349, 14, "R", 1 );
		skp->Text( 349, 29, "P", 1 );
		skp->Text( 349, 44, "Y", 1 );

		skp->SetPen( skpWhitePen );
		skp->Ellipse( 122, 48, 356, 282 );

		skp->SetPen( skpLightGrayPen );
		skp->SetBrush( skpLightGrayBrush );
		oapi::IVECTOR2 diamond[4];
		diamond[0].x = 239;
		diamond[0].y = 53;
		diamond[1].x = 244;
		diamond[1].y = 62;
		diamond[2].x = 239;
		diamond[2].y = 71;
		diamond[3].x = 234;
		diamond[3].y = 62;
		skp->Polygon( diamond, 4 );// top
		//diamond[0].x = 239;
		diamond[0].y = 259;
		//diamond[1].x = 244;
		diamond[1].y = 268;
		//diamond[2].x = 239;
		diamond[2].y = 277;
		//diamond[3].x = 234;
		diamond[3].y = 268;
		skp->Polygon( diamond, 4 );// bottom
		diamond[0].x = 342;
		diamond[0].y = 160;
		diamond[1].x = 351;
		diamond[1].y = 165;
		diamond[2].x = 342;
		diamond[2].y = 170;
		diamond[3].x = 333;
		diamond[3].y = 165;
		skp->Polygon( diamond, 4 );// right
		diamond[0].x = 136;
		//diamond[0].y = 160;
		diamond[1].x = 145;
		//diamond[1].y = 165;
		diamond[2].x = 136;
		//diamond[2].y = 170;
		diamond[3].x = 127;
		//diamond[3].y = 165;
		skp->Polygon( diamond, 4 );// left

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
		skp->Line( 230, 65, 248, 265 );// 5º/185º
		skp->Line( 222, 67, 256, 263 );// 10º/190º
		skp->Line( 213, 68, 265, 262 );// 15º/195º
		skp->Line( 205, 71, 273, 259 );// 20º/200º
		skp->Line( 197, 74, 281, 256 );// 25º/205º

		skp->Line( 182, 83, 296, 247 );// 35º/215º
		skp->Line( 175, 88, 303, 242 );// 40º/220º
		skp->Line( 168, 94, 310, 236 );// 45º/225º
		skp->Line( 162, 101, 316, 229 );// 50º/230º
		skp->Line( 157, 108, 321, 222 );// 55º/235º
		
		skp->Line( 148, 123, 330, 207 );// 65º/245º
		skp->Line( 145, 131, 333, 199 );// 70º/250º
		skp->Line( 142, 139, 336, 191 );// 75º/255º
		skp->Line( 141, 148, 337, 182 );// 80º/260º
		skp->Line( 139, 156, 339, 174 );// 85º/265º
		
		skp->Line( 139, 174, 339, 156 );// 95º/275º
		skp->Line( 141, 182, 337, 148 );// 100º/280º
		skp->Line( 142, 191, 336, 139 );// 105º/285º
		skp->Line( 145, 199, 333, 131 );// 110º/290º
		skp->Line( 148, 207, 330, 123 );// 115º/295º
		
		skp->Line( 157, 222, 321, 108 );// 125º/305º
		skp->Line( 162, 229, 316, 101 );// 130º/310º
		skp->Line( 168, 236, 310, 94 );// 135º/315º
		skp->Line( 175, 242, 303, 88 );// 140º/320º
		skp->Line( 182, 247, 296, 83 );// 145º/325º
		
		skp->Line( 197, 256, 281, 74 );// 155º/335º
		skp->Line( 205, 259, 273, 71 );// 160º/340º
		skp->Line( 213, 262, 265, 68 );// 165º/345º
		skp->Line( 222, 263, 256, 67 );// 170º/350º
		skp->Line( 230, 265, 248, 65 );// 175º/355º
		skp->SetPen( skpLightGrayThickPen );
		skp->Line( 187, 75, 291, 255 );// 30º/210º
		skp->Line( 149, 113, 329, 217 );// 60º/240º
		skp->Line( 149, 217, 329, 113 );// 120º/300º
		skp->Line( 187, 255, 291, 75 );// 150º/330º

		// cover center for ADI SRCPAINT
		skp->SetBrush( skpBlackBrush );
		skp->SetPen( skpBlackPen );
		skp->Ellipse( 144, 70, 334, 260 );

		skp->Text( 294, 70, "33", 2 );
		skp->Text( 323, 99, "30", 2 );
		skp->Text( 323, 217, "24", 2 );
		skp->Text( 294, 247, "21", 2 );
		skp->Text( 171, 247, "15", 2 );
		skp->Text( 142, 217, "12", 2 );
		skp->Text( 142, 99, "06", 2 );
		skp->Text( 171, 70, "03", 2 );

		// att needles
		// top scale
		skp->SetPen( skpMagentaPen );
		skp->SetBrush( skpMagentaBrush );
		//Arc( 129, 55, 349, 275, 286, 66, 192, 66 );
		skp->Line( 192, 66, 192, 72 );
		skp->Line( 201, 62, 201, 68 );
		skp->Line( 211, 59, 211, 65 );
		skp->Line( 220, 57, 220, 63 );
		skp->Line( 230, 55, 230, 61 );
		skp->Line( 239, 55, 239, 61 );
		skp->Line( 248, 55, 248, 61 );
		skp->Line( 258, 57, 258, 63 );
		skp->Line( 267, 59, 267, 65 );
		skp->Line( 277, 62, 277, 68 );
		skp->Line( 286, 66, 286, 72 );

		// side scale
		//Arc( 129, 55, 349, 275, 338, 212, 338, 118 );
		skp->Line( 338, 118, 332, 118 );
		skp->Line( 342, 127, 336, 127 );
		skp->Line( 345, 137, 339, 137 );
		skp->Line( 347, 146, 341, 146 );
		skp->Line( 349, 156, 343, 156 );
		skp->Line( 349, 165, 343, 165 );
		skp->Line( 349, 174, 343, 174 );
		skp->Line( 347, 184, 341, 184 );
		skp->Line( 345, 193, 339, 193 );
		skp->Line( 342, 203, 336, 203 );
		skp->Line( 338, 212, 332, 212 );

		// bottom scale
		//Arc( 129, 55, 349, 275, 192, 264, 286, 264 );
		skp->Line( 192, 264, 192, 258 );
		skp->Line( 201, 268, 201, 262 );
		skp->Line( 211, 271, 211, 265 );
		skp->Line( 220, 273, 220, 267 );
		skp->Line( 230, 275, 230, 269 );
		skp->Line( 239, 275, 239, 269 );
		skp->Line( 248, 275, 249, 269 );
		skp->Line( 258, 273, 258, 267 );
		skp->Line( 267, 271, 267, 265 );
		skp->Line( 277, 268, 277, 262 );
		skp->Line( 286, 264, 286, 258 );
		return;
	}

	void MDU::ADI( HDC hDC, double pitch, double roll, double yaw )
	{
		// center (239,165) r = 117
		// view r = 95, ball r = 95 * sqrt( 2 )
		// 90º pitch/yaw "FOV"

		// apply roll
		XFORM WTroll;
		WTroll.eM11 = (FLOAT)cos( roll * RAD );
		WTroll.eM12 = (FLOAT)(-sin( roll * RAD ));
		WTroll.eM21 = -WTroll.eM12;
		WTroll.eM22 = WTroll.eM11;
		WTroll.eDx = (FLOAT)(96 * (1 - WTroll.eM11 - WTroll.eM21));
		WTroll.eDy = (FLOAT)(96 * (1 - WTroll.eM11 + WTroll.eM21));
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
			hP = Round( 134.35 * sinpitch );
			if (pitch < 0)
			{
				SelectObject( hDC_ADI, gdiWhiteBrush );
				SelectObject( hDC_ADI, gdiWhitePen );
				Rectangle( hDC_ADI, 0, 0, 190, 95 );
				SelectObject( hDC_ADI, gdiDarkGrayBrush );
				SelectObject( hDC_ADI, gdiDarkGrayPen );
				Chord( hDC_ADI, -37, 95 - hP, 231, 95 + hP, 231, 95, -37, 95 );
				Rectangle( hDC_ADI, 0, 95, 190, 190 );
				SelectObject( hDC_ADI, gdiBlackPen );
				Arc( hDC_ADI, -37, 95 - hP, 231, 95 + hP, 231, 95, -37, 95 );
			}
			else
			{
				SelectObject( hDC_ADI, gdiDarkGrayBrush );
				SelectObject( hDC_ADI, gdiDarkGrayPen );
				Rectangle( hDC_ADI, 0, 95, 190, 190 );
				SelectObject( hDC_ADI, gdiWhiteBrush );
				SelectObject( hDC_ADI, gdiWhitePen );
				Chord( hDC_ADI, -37, 95 - hP, 231, 95 + hP, -37, 95, 231, 95 );
				Rectangle( hDC_ADI, 0, 0, 190, 95 );
				SelectObject( hDC_ADI, gdiBlackPen );
				Arc( hDC_ADI, -37, 95 - hP, 231, 95 + hP, -37, 95, 231, 95 );
			}
			hP = 89 + Round( 129.772414 * sinpitch );
			TextOut( hDC_ADI, 60, hP, "0", 1 );
			TextOut( hDC_ADI, 130, hP, "0", 1 );
		}
		else if (pitch > 0)
		{
			SelectObject( hDC_ADI, gdiWhiteBrush );
			Rectangle( hDC_ADI, -2, -2, 194, 194 );
		}
		else
		{
			SelectObject( hDC_ADI, gdiDarkGrayBrush );
			Rectangle( hDC_ADI, -2, -2, 194, 194 );
		}

		// pitch lines/labels for +30º/+60º/+90º/+120º/+150º
		SelectObject( hDC_ADI, gdiDarkGrayPen );
		// +30º
		if (fabs( pitch - 30 ) <= 45)
		{
			hP2 = sinpitch * 116.350763 - cospitch * 67.175144;//hP = 69.296 * sin( (pitch - 30) * RAD );
			hP = Round( hP2 );
			if (pitch < 30) Arc( hDC_ADI, -37, 95 - hP, 231, 95 + hP, 231, 95, -37, 95 );
			else Arc( hDC_ADI, -37, 95 - hP, 231, 95 + hP, -37, 95, 231, 95 );
			hP = 89 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 60, hP, "3", 1 );
			TextOut( hDC_ADI, 130, hP, "3", 1 );
		}
		// +60º
		if (fabs( pitch - 60 ) <= 45)
		{
			hP2 = sinpitch * 67.175144 - cospitch * 116.350763;
			hP = Round( hP2 );
			if (pitch < 60) Arc( hDC_ADI, -37, 95 - hP, 231, 95 + hP, 231, 95, -37, 95 );
			else Arc( hDC_ADI, -37, 95 - hP, 231, 95 + hP, -37, 95, 231, 95 );
			hP = 89 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 60, hP, "6", 1 );
			TextOut( hDC_ADI, 130, hP, "6", 1 );
		}
		// +90º
		if (fabs( pitch - 90 ) <= 45)
		{
			hP2 = 134.350289 * (-cospitch);
			hP = Round( hP2 );
			if (pitch < 90) Arc( hDC_ADI, -37, 95 - hP, 231, 95 + hP, 231, 95, -37, 95 );
			else Arc( hDC_ADI, -37, 95 - hP, 231, 95 + hP, -37, 95, 231, 95 );
			hP = 89 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 60, hP, "9", 1 );
			TextOut( hDC_ADI, 130, hP, "9", 1 );
		}
		// +120º
		if (fabs( pitch - 120 ) <= 45)
		{
			hP2 = (-sinpitch) * 67.175144 - cospitch * 116.350763;
			hP = Round( hP2 );
			if (pitch < 120) Arc( hDC_ADI, -37, 95 - hP, 231, 95 + hP, 231, 95, -37, 95 );
			else Arc( hDC_ADI, -37, 95 - hP, 231, 95 + hP, -37, 95, 231, 95 );
			hP = 89 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 58, hP, "12", 2 );
			TextOut( hDC_ADI, 128, hP, "12", 2 );
		}
		// +150º
		if (fabs( pitch - 150 ) <= 45)
		{
			hP2 = (-sinpitch) * 116.350763 - cospitch * 67.175144;
			hP = Round( hP2 );
			if (pitch < 150) Arc( hDC_ADI, -37, 95 - hP, 231, 95 + hP, 231, 95, -37, 95 );
			else Arc( hDC_ADI, -37, 95 - hP, 231, 95 + hP, -37, 95, 231, 95 );
			hP = 89 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 58, hP, "15", 2 );
			TextOut( hDC_ADI, 128, hP, "15", 2 );
		}

		// pitch lines/labels for -30º/-60º/-90º/-120º/-150º
		SelectObject( hDC_ADI, gdiWhitePen );
		SetTextColor( hDC_ADI, CR_WHITE );
		SetBkColor( hDC_ADI, CR_DARK_GRAY );
		// -30º
		if (fabs( pitch + 30 ) <= 45)
		{
			hP2 = sinpitch * 116.350763 + cospitch * 67.175144;
			hP = Round( hP2 );
			if (pitch < -30) Arc( hDC_ADI, -37, 95 - hP, 231, 95 + hP, 231, 95, -37, 95 );
			else Arc( hDC_ADI, -37, 95 - hP, 231, 95 + hP, -37, 95, 231, 95 );
			hP = 89 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 58, hP, "33", 2 );
			TextOut( hDC_ADI, 128, hP, "33", 2 );
		}
		// -60º
		if (fabs( pitch + 60 ) <= 45)
		{
			hP2 = sinpitch * 67.175144 + cospitch * 116.350763;
			hP = Round( hP2 );
			if (pitch < -60) Arc( hDC_ADI, -37, 95 - hP, 231, 95 + hP, 231, 95, -37, 95 );
			else Arc( hDC_ADI, -37, 95 - hP, 231, 95 + hP, -37, 95, 231, 95 );
			hP = 89 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 58, hP, "30", 2 );
			TextOut( hDC_ADI, 128, hP, "30", 2 );
		}
		// -90º
		if (fabs( pitch + 90 ) <= 45)
		{
			hP2 = 134.350289 * cospitch;
			hP = Round( hP2 );
			if (pitch < -90) Arc( hDC_ADI, -37, 95 - hP, 231, 95 + hP, 231, 95, -37, 95 );
			else Arc( hDC_ADI, -37, 95 - hP, 231, 95 + hP, -37, 95, 231, 95 );
			hP = 89 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 58, hP, "27", 2 );
			TextOut( hDC_ADI, 128, hP, "27", 2 );
		}
		// -120º
		if (fabs( pitch + 120 ) <= 45)
		{
			hP2 = (-sinpitch) * 67.175144 + cospitch * 116.350763;
			hP = Round( hP2 );
			if (pitch < -120) Arc( hDC_ADI, -37, 95 - hP, 231, 95 + hP, 231, 95, -37, 95 );
			else Arc( hDC_ADI, -37, 95 - hP, 231, 95 + hP, -37, 95, 231, 95 );
			hP = 89 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 58, hP, "24", 2 );
			TextOut( hDC_ADI, 128, hP, "24", 2 );
		}
		// -150º
		if (fabs( pitch + 150 ) <= 45)
		{
			hP2 = (-sinpitch) * 116.350763 + cospitch * 67.175144;
			hP = Round( hP2 );
			if (pitch < -150) Arc( hDC_ADI, -37, 95 - hP, 231, 95 + hP, 231, 95, -37, 95 );
			else Arc( hDC_ADI, -37, 95 - hP, 231, 95 + hP, -37, 95, 231, 95 );
			hP = 89 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 58, hP, "21", 2 );
			TextOut( hDC_ADI, 128, hP, "21", 2 );
		}

		// TODO yaw
		// HACK
		// "central plane"
		SelectObject( hDC_ADI, gdiWhiteBrush );
		SelectObject( hDC_ADI, gdiBlackPen );
		Rectangle( hDC_ADI, 92, 0, 98, 192 );
		MoveToEx( hDC_ADI, 95, 0, NULL );
		LineTo( hDC_ADI, 95, 190 );

		SelectObject( hDC_ADI, gdiDarkGrayPen );
		// yaw line 30º (above horizon)
		MoveToEx( hDC_ADI, 163, 0, NULL );
		LineTo( hDC_ADI, 163, 95 + Round( 116.349982 * sinpitch ) );
		// yaw line 330º (above horizon)
		MoveToEx( hDC_ADI, 27, 0, NULL );
		LineTo( hDC_ADI, 27, 95 + Round( 116.349982 * sinpitch ) );
		SelectObject( hDC_ADI, gdiWhitePen );
		// yaw line 30º (below horizon)
		MoveToEx( hDC_ADI, 163, 190, NULL );
		LineTo( hDC_ADI, 163, 95 + Round( 116.349982 * sinpitch ) );
		// yaw line 330º (below horizon)
		MoveToEx( hDC_ADI, 27, 190, NULL );
		LineTo( hDC_ADI, 27, 95 + Round( 116.349982 * sinpitch ) );

		// TODO yaw labels

		// roll triangle
		SelectObject( hDC_ADI, gdiLightGreenPen );
		SelectObject( hDC_ADI, gdiLightGreenBrush );
		POINT tri[3];
		tri[0].x = 95;
		tri[0].y = 1;
		tri[1].x = 90;
		tri[1].y = 11;
		tri[2].x = 100;
		tri[2].y = 11;
		Polygon( hDC_ADI, tri, 3 );
		
		// clean up
		ModifyWorldTransform( hDC_ADI, &WTroll, MWT_IDENTITY );
		SetGraphicsMode( hDC_ADI, GM_COMPATIBLE );

		// digital RPY
		if (pitch < 0) pitch += 360;// TODO get rid of this
		SetTextColor( hDC, CR_WHITE );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%03.0f", roll );
		TextOut( hDC, 362, 14, cbuf, strlen( cbuf ) );
		sprintf_s( cbuf, 8, "%03.0f", pitch );
		TextOut( hDC, 362, 29, cbuf, strlen( cbuf ) );
		sprintf_s( cbuf, 8, "%03.0f", yaw );
		TextOut( hDC, 362, 44, cbuf, strlen( cbuf ) );

		// copy ball
		BitBlt( hDC_ADI, 0, 0, 192, 192, hDC_ADIMASK, 0, 0, SRCAND );
		BitBlt( hDC, 144, 70, 190, 190, hDC_ADI, 1, 1, SRCPAINT );

		// flight director
		SelectObject( hDC, gdiBlackPen );
		SelectObject( hDC, gdiLightGreenBrush );
		POINT fd[18] = {{203,163},{203,167},{223,167},{228,175},{236,181},{242,181},{250,175},{255,167},{275,167},{275,163},{251,163},{251,167},{248,173},{242,177},{236,177},{230,173},{227,167},{227,163}};
		Polygon( hDC, fd, 18 );
		// center marker
		SelectObject( hDC, gdiLightGreenPen );
		Rectangle( hDC, 238, 137, 240, 193 );
		Rectangle( hDC, 211, 164, 267, 166 );
		return;
	}

	void MDU::ADI( oapi::Sketchpad2* skp, double pitch, double roll, double yaw )
	{
		// center (239,165) r = 117
		// view r = 95, ball r = 95 * sqrt( 2 )
		// 90º pitch/yaw "FOV"

		// draw ball mesh
		FMATRIX4 mat;
		int H = 330;
		int W = 478;
	
		double croll = cos( roll * RAD );
		double sroll = sin( -roll * RAD );
		double cyaw = cos( yaw * RAD );
		double syaw = sin( yaw * RAD );
		double cpitch = cos( pitch * RAD );
		double spitch = sin( pitch * RAD );
		
		VECTOR3 xdir = _V( cyaw * croll, sroll, -croll * syaw );
		VECTOR3 zdir = _V( syaw * cpitch + sroll * cyaw * spitch, -croll * spitch, cpitch * cyaw - syaw * sroll * spitch );
		gcWorldMatrix( &mat, _V(float(W/2), float(H/2), float(W + 126)), xdir, zdir, 1.3435f );

		skp->SetWorldTransform( &mat );
		skp->SetPen( skpWhitePen );
		skp->DrawSketchMesh( hADIball, 0, 0 );
		skp->SetWorldTransform();
		
		// roll triangle
		skp->SetPen( skpLightGreenPen );
		skp->SetBrush( skpLightGreenBrush );
		oapi::IVECTOR2 tri[3];
		tri[0].x = (long)(239 + 95 * sroll);
		tri[0].y = (long)(165 - 95 * croll);
		tri[1].x = (long)(239 - 5 * croll + 84 * sroll);
		tri[1].y = (long)(165 - 5 * sroll - 84 * croll);
		tri[2].x = (long)(239 + 5 * croll + 84 * sroll);
		tri[2].y = (long)(165 + 5 * sroll - 84 * croll);
		skp->Polygon( tri, 3 );

		// digital RPY
		if (pitch < 0) pitch += 360;// TODO get rid of this
		skp->SetTextColor( CR_WHITE );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%03.0f", roll );
		skp->Text( 362, 14, cbuf, strlen( cbuf ) );
		sprintf_s( cbuf, 8, "%03.0f", pitch );
		skp->Text( 362, 29, cbuf, strlen( cbuf ) );
		sprintf_s( cbuf, 8, "%03.0f", yaw );
		skp->Text( 362, 44, cbuf, strlen( cbuf ) );

		// flight director
		skp->SetPen( skpBlackPen );
		oapi::IVECTOR2 fd[18] = {{203,163},{203,167},{223,167},{228,175},{236,181},{242,181},{250,175},{255,167},{275,167},{275,163},{251,163},{251,167},{248,173},{242,177},{236,177},{230,173},{227,167},{227,163}};
		skp->Polygon( fd, 18 );
		// center marker
		skp->SetPen( skpLightGreenPen );
		skp->Rectangle( 238, 137, 240, 193 );
		skp->Rectangle( 211, 164, 267, 166 );
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
				// heading err +/-5º
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
				// heading err +/-5º
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
		SelectObject( hDC, gdiSSUBFont_h12w7 );
		if (adierr == 1)
		{
			// ADI ERROR MED
			// 5/5/5
			TextOut( hDC, 337, 105, "5", 1 );
			TextOut( hDC, 337, 208, "5", 1 );

			if (roll > 5) pos_roll = 47;
			else if (roll < -5) pos_roll = -47;
			else pos_roll = roll * 9.4;

			if (pitch > 5) pos_pitch = 47;
			else if (pitch < -5) pos_pitch = -47;
			else pos_pitch = pitch * 9.4;

			if (yaw > 5) pos_yaw = 47;
			else if (yaw < -5) pos_yaw = -47;
			else pos_yaw = yaw * 9.4;
		}
		else if (adierr == 2)
		{
			// ADI ERROR HIGH
			// 10/10/10
			TextOut( hDC, 337, 105, "10", 2 );
			TextOut( hDC, 337, 208, "10", 2 );

			if (roll > 10) pos_roll = 47;
			else if (roll < -10) pos_roll = -47;
			else pos_roll = roll * 4.7;

			if (pitch > 10) pos_pitch = 47;
			else if (pitch < -10) pos_pitch = -47;
			else pos_pitch = pitch * 4.7;

			if (yaw > 10) pos_yaw = 47;
			else if (yaw < -10) pos_yaw = -47;
			else pos_yaw = yaw * 4.7;
		}
		else
		{
			// ADI ERROR LOW
			// 1/1/1
			TextOut( hDC, 337, 105, "1", 1 );
			TextOut( hDC, 337, 208, "1", 1 );

			if (roll > 1) pos_roll = 47;
			else if (roll < -1) pos_roll = -47;
			else pos_roll = roll * 47;

			if (pitch > 1) pos_pitch = 47;
			else if (pitch < -1) pos_pitch = -47;
			else pos_pitch = pitch * 47;

			if (yaw > 1) pos_yaw = 47;
			else if (yaw < -1) pos_yaw = -47;
			else pos_yaw = yaw * 47;
		}

		// draw needles
		SelectObject( hDC, gdiMagentaPen );
		SelectObject( hDC, gdiMagentaBrush );
		Rectangle( hDC, 238 + Round( pos_roll ), 165 - Round( sqrt( 12100 - (pos_roll * pos_roll) ) ), 240 + Round( pos_roll ), 134 );// roll
		Rectangle( hDC, 270, 164 + Round( pos_pitch ), 239 + Round( sqrt( 12100 - (pos_pitch * pos_pitch) ) ), 166 + Round( pos_pitch ) );// pitch
		Rectangle( hDC, 238 + Round( pos_yaw ), 196, 240 + Round( pos_yaw ), 165 + Round( sqrt( 12100 - (pos_yaw * pos_yaw) ) ) );// yaw
		return;
	}

	void MDU::ADI_ERROR_A( oapi::Sketchpad2* skp, double pitch, double roll, double yaw, int adierr )
	{
		double pos_pitch;
		double pos_roll;
		double pos_yaw;

		skp->SetTextColor( CR_MAGENTA );
		skp->SetFont( skpSSUBFont_h12 );
		if (adierr == 1)
		{
			// ADI ERROR MED
			// 5/5/5
			skp->Text( 337, 105, "5", 1 );
			skp->Text( 337, 208, "5", 1 );

			if (roll > 5) pos_roll = 47;
			else if (roll < -5) pos_roll = -47;
			else pos_roll = roll * 9.4;

			if (pitch > 5) pos_pitch = 47;
			else if (pitch < -5) pos_pitch = -47;
			else pos_pitch = pitch * 9.4;

			if (yaw > 5) pos_yaw = 47;
			else if (yaw < -5) pos_yaw = -47;
			else pos_yaw = yaw * 9.4;
		}
		else if (adierr == 2)
		{
			// ADI ERROR HIGH
			// 10/10/10
			skp->Text( 337, 105, "10", 2 );
			skp->Text( 337, 208, "10", 2 );

			if (roll > 10) pos_roll = 47;
			else if (roll < -10) pos_roll = -47;
			else pos_roll = roll * 4.7;

			if (pitch > 10) pos_pitch = 47;
			else if (pitch < -10) pos_pitch = -47;
			else pos_pitch = pitch * 4.7;

			if (yaw > 10) pos_yaw = 47;
			else if (yaw < -10) pos_yaw = -47;
			else pos_yaw = yaw * 4.7;
		}
		else
		{
			// ADI ERROR LOW
			// 1/1/1
			skp->Text( 337, 105, "1", 1 );
			skp->Text( 337, 208, "1", 1 );

			if (roll > 1) pos_roll = 47;
			else if (roll < -1) pos_roll = -47;
			else pos_roll = roll * 47;

			if (pitch > 1) pos_pitch = 47;
			else if (pitch < -1) pos_pitch = -47;
			else pos_pitch = pitch * 47;

			if (yaw > 1) pos_yaw = 47;
			else if (yaw < -1) pos_yaw = -47;
			else pos_yaw = yaw * 47;
		}

		// draw needles
		skp->SetPen( skpMagentaPen );
		skp->SetBrush( skpMagentaBrush );
		skp->Rectangle( 238 + Round( pos_roll ), 165 - Round( sqrt( 12100 - (pos_roll * pos_roll) ) ), 240 + Round( pos_roll ), 134 );// roll
		skp->Rectangle( 270, 164 + Round( pos_pitch ), 239 + Round( sqrt( 12100 - (pos_pitch * pos_pitch) ) ), 166 + Round( pos_pitch ) );// pitch
		skp->Rectangle( 238 + Round( pos_yaw ), 196, 240 + Round( pos_yaw ), 165 + Round( sqrt( 12100 - (pos_yaw * pos_yaw) ) ) );// yaw
		return;
	}

	void MDU::ADI_ERROR_B( HDC hDC, double pitch, double roll, double yaw, int adierr )
	{
		double pos_pitch;
		double pos_roll;
		double pos_yaw;

		SetTextColor( hDC, CR_MAGENTA );
		SelectObject( hDC, gdiSSUBFont_h12w7 );
		if (adierr == 1)
		{
			// ADI ERROR MED
			// 25/2/2.5
			TextOut( hDC, 337, 105, "2", 1 );
			TextOut( hDC, 337, 208, "2", 1 );

			if (roll > 25) pos_roll = 47;
			else if (roll < -25) pos_roll = -47;
			else pos_roll = roll * 1.88;

			if (pitch > 2) pos_pitch = 47;
			else if (pitch < -2) pos_pitch = -47;
			else pos_pitch = pitch * 23.5;
		}
		else if (adierr == 2)
		{
			// ADI ERROR HIGH
			// 25/5/2.5
			TextOut( hDC, 337, 105, "5", 1 );
			TextOut( hDC, 337, 208, "5", 1 );

			if (roll > 25) pos_roll = 47;
			else if (roll < -25) pos_roll = -47;
			else pos_roll = roll * 1.88;

			if (pitch > 5) pos_pitch = 47;
			else if (pitch < -5) pos_pitch = -47;
			else pos_pitch = pitch * 9.4;
		}
		else
		{
			// ADI ERROR LOW
			// 10/1/2.5
			TextOut( hDC, 337, 105, "1", 1 );
			TextOut( hDC, 337, 208, "1", 1 );

			if (roll > 10) pos_roll = 47;
			else if (roll < -10) pos_roll = -47;
			else pos_roll = roll * 4.7;

			if (pitch > 1) pos_pitch = 47;
			else if (pitch < -1) pos_pitch = -47;
			else pos_pitch = pitch * 47;
		}
		if (yaw > 2.5) pos_yaw = 47;
		else if (yaw < -2.5) pos_yaw = -47;
		else pos_yaw = yaw * 18.8;

		// draw needles
		SelectObject( hDC, gdiMagentaPen );
		SelectObject( hDC, gdiMagentaBrush );
		Rectangle( hDC, 238 + Round( pos_roll ), 165 - Round( sqrt( 12100 - (pos_roll * pos_roll) ) ), 240 + Round( pos_roll ), 134 );// roll
		Rectangle( hDC, 270, 164 + Round( pos_pitch ), 239 + Round( sqrt( 12100 - (pos_pitch * pos_pitch) ) ), 166 + Round( pos_pitch ) );// pitch
		Rectangle( hDC, 238 + Round( pos_yaw ), 196, 240 + Round( pos_yaw ), 165 + Round( sqrt( 12100 - (pos_yaw * pos_yaw) ) ) );// yaw
		return;// 25/25/10 5/2/1 2.5/2.5/2.5
	}

	void MDU::ADI_ERROR_B( oapi::Sketchpad2* skp, double pitch, double roll, double yaw, int adierr )
	{
		double pos_pitch;
		double pos_roll;
		double pos_yaw;

		skp->SetTextColor( CR_MAGENTA );
		skp->SetFont( skpSSUBFont_h12 );
		if (adierr == 1)
		{
			// ADI ERROR MED
			// 25/2/2.5
			skp->Text( 337, 105, "2", 1 );
			skp->Text( 337, 208, "2", 1 );

			if (roll > 25) pos_roll = 47;
			else if (roll < -25) pos_roll = -47;
			else pos_roll = roll * 1.88;

			if (pitch > 2) pos_pitch = 47;
			else if (pitch < -2) pos_pitch = -47;
			else pos_pitch = pitch * 23.5;
		}
		else if (adierr == 2)
		{
			// ADI ERROR HIGH
			// 25/5/2.5
			skp->Text( 337, 105, "5", 1 );
			skp->Text( 337, 208, "5", 1 );

			if (roll > 25) pos_roll = 47;
			else if (roll < -25) pos_roll = -47;
			else pos_roll = roll * 1.88;

			if (pitch > 5) pos_pitch = 47;
			else if (pitch < -5) pos_pitch = -47;
			else pos_pitch = pitch * 9.4;
		}
		else
		{
			// ADI ERROR LOW
			// 10/1/2.5
			skp->Text( 337, 105, "1", 1 );
			skp->Text( 337, 208, "1", 1 );

			if (roll > 10) pos_roll = 47;
			else if (roll < -10) pos_roll = -47;
			else pos_roll = roll * 4.7;

			if (pitch > 1) pos_pitch = 47;
			else if (pitch < -1) pos_pitch = -47;
			else pos_pitch = pitch * 47;
		}
		if (yaw > 2.5) pos_yaw = 47;
		else if (yaw < -2.5) pos_yaw = -47;
		else pos_yaw = yaw * 18.8;

		// draw needles
		skp->SetPen( skpMagentaPen );
		skp->SetBrush( skpMagentaBrush );
		skp->Rectangle( 238 + Round( pos_roll ), 165 - Round( sqrt( 12100 - (pos_roll * pos_roll) ) ), 240 + Round( pos_roll ), 134 );// roll
		skp->Rectangle( 270, 164 + Round( pos_pitch ), 239 + Round( sqrt( 12100 - (pos_pitch * pos_pitch) ) ), 166 + Round( pos_pitch ) );// pitch
		skp->Rectangle( 238 + Round( pos_yaw ), 196, 240 + Round( pos_yaw ), 165 + Round( sqrt( 12100 - (pos_yaw * pos_yaw) ) ) );// yaw
		return;// 25/25/10 5/2/1 2.5/2.5/2.5
	}

	void MDU::ADI_ERROR_C( HDC hDC, double pitch, double roll, double yaw, int adierr )
	{
		double pos_pitch;
		double pos_roll;
		double pos_yaw;

		SetTextColor( hDC, CR_MAGENTA );
		SelectObject( hDC, gdiSSUBFont_h12w7 );
		if (adierr == 0)
		{
			// ADI ERROR LOW
			// 10/0.5g/2.5
			TextOut( hDC, 337, 105, "0.5g", 4 );
			TextOut( hDC, 337, 208, "0.5g", 4 );

			if (roll > 10) pos_roll = 47;
			else if (roll < -10) pos_roll = -47;
			else pos_roll = roll * 4.7;

			if (pitch > 0.5) pos_pitch = 47;
			else if (pitch < -0.5) pos_pitch = -47;
			else pos_pitch = pitch * 94;
		}
		else
		{
			// ADI ERROR MED/HIGH
			// 25/1.25g/2.5
			TextOut( hDC, 337, 105, "1.2g", 4 );
			TextOut( hDC, 337, 208, "1.2g", 4 );

			if (roll > 25) pos_roll = 47;
			else if (roll < -25) pos_roll = -47;
			else pos_roll = roll * 1.88;

			if (pitch > 1.25) pos_pitch = 47;
			else if (pitch < -1.25) pos_pitch = -47;
			else pos_pitch = pitch * 37.6;
		}
		if (yaw > 2.5) pos_yaw = 47;
		else if (yaw < -2.5) pos_yaw = -47;
		else pos_yaw = yaw * 18.8;

		// draw needles
		SelectObject( hDC, gdiMagentaPen );
		SelectObject( hDC, gdiMagentaBrush );
		Rectangle( hDC, 238 + Round( pos_roll ), 165 - Round( sqrt( 12100 - (pos_roll * pos_roll) ) ), 240 + Round( pos_roll ), 134 );// roll
		Rectangle( hDC, 270, 164 + Round( pos_pitch ), 239 + Round( sqrt( 12100 - (pos_pitch * pos_pitch) ) ), 166 + Round( pos_pitch ) );// pitch
		Rectangle( hDC, 238 + Round( pos_yaw ), 196, 240 + Round( pos_yaw ), 165 + Round( sqrt( 12100 - (pos_yaw * pos_yaw) ) ) );// yaw
		return;// 25/25/10 1.25/1.25/0.5 2.5/2.5/2.5
	}

	void MDU::ADI_ERROR_C( oapi::Sketchpad2* skp, double pitch, double roll, double yaw, int adierr )
	{
		double pos_pitch;
		double pos_roll;
		double pos_yaw;

		skp->SetTextColor( CR_MAGENTA );
		skp->SetFont( skpSSUBFont_h12 );
		if (adierr == 0)
		{
			// ADI ERROR LOW
			// 10/0.5g/2.5
			skp->Text( 337, 105, "0.5g", 4 );
			skp->Text( 337, 208, "0.5g", 4 );

			if (roll > 10) pos_roll = 47;
			else if (roll < -10) pos_roll = -47;
			else pos_roll = roll * 4.7;

			if (pitch > 0.5) pos_pitch = 47;
			else if (pitch < -0.5) pos_pitch = -47;
			else pos_pitch = pitch * 94;
		}
		else
		{
			// ADI ERROR MED/HIGH
			// 25/1.25g/2.5
			skp->Text( 337, 105, "1.2g", 4 );
			skp->Text( 337, 208, "1.2g", 4 );

			if (roll > 25) pos_roll = 47;
			else if (roll < -25) pos_roll = -47;
			else pos_roll = roll * 1.88;

			if (pitch > 1.25) pos_pitch = 47;
			else if (pitch < -1.25) pos_pitch = -47;
			else pos_pitch = pitch * 37.6;
		}
		if (yaw > 2.5) pos_yaw = 47;
		else if (yaw < -2.5) pos_yaw = -47;
		else pos_yaw = yaw * 18.8;

		// draw needles
		skp->SetPen( skpMagentaPen );
		skp->SetBrush( skpMagentaBrush );
		skp->Rectangle( 238 + Round( pos_roll ), 165 - Round( sqrt( 12100 - (pos_roll * pos_roll) ) ), 240 + Round( pos_roll ), 134 );// roll
		skp->Rectangle( 270, 164 + Round( pos_pitch ), 239 + Round( sqrt( 12100 - (pos_pitch * pos_pitch) ) ), 166 + Round( pos_pitch ) );// pitch
		skp->Rectangle( 238 + Round( pos_yaw ), 196, 240 + Round( pos_yaw ), 165 + Round( sqrt( 12100 - (pos_yaw * pos_yaw) ) ) );// yaw
		return;// 25/25/10 1.25/1.25/0.5 2.5/2.5/2.5
	}

	void MDU::ADI_ERROR_D( HDC hDC, double pitch, double roll, double yaw, int adierr )
	{
		double pos_pitch;
		double pos_roll;
		double pos_yaw;

		SetTextColor( hDC, CR_MAGENTA );
		SelectObject( hDC, gdiSSUBFont_h12w7 );
		if (adierr == 1)
		{
			// ADI ERROR MED
			// 5/5/2.5
			TextOut( hDC, 337, 105, "5", 1 );
			TextOut( hDC, 337, 208, "5", 1 );

			if (roll > 5) pos_roll = 47;
			else if (roll < -5) pos_roll = -47;
			else pos_roll = roll * 9.4;

			if (pitch > 5) pos_pitch = 47;
			else if (pitch < -5) pos_pitch = -47;
			else pos_pitch = pitch * 9.4;
		}
		else if (adierr == 2)
		{
			// ADI ERROR HIGH
			// 20/10/2.5
			TextOut( hDC, 337, 105, "10", 2 );
			TextOut( hDC, 337, 208, "10", 2 );

			if (roll > 10) pos_roll = 47;
			else if (roll < -10) pos_roll = -47;
			else pos_roll = roll * 4.7;

			if (pitch > 10) pos_pitch = 47;
			else if (pitch < -10) pos_pitch = -47;
			else pos_pitch = pitch * 4.7;
		}
		else
		{
			// ADI ERROR LOW
			// 1/1/2.5
			TextOut( hDC, 337, 105, "1", 1 );
			TextOut( hDC, 337, 208, "1", 1 );

			if (roll > 1) pos_roll = 47;
			else if (roll < -1) pos_roll = -47;
			else pos_roll = roll * 47;

			if (pitch > 1) pos_pitch = 47;
			else if (pitch < -1) pos_pitch = -47;
			else pos_pitch = pitch * 47;
		}
		if (yaw > 2.5) pos_yaw = 47;
		else if (yaw < -2.5) pos_yaw = -47;
		else pos_yaw = yaw * 18.8;

		// draw needles
		SelectObject( hDC, gdiMagentaPen );
		SelectObject( hDC, gdiMagentaBrush );
		Rectangle( hDC, 238 + Round( pos_roll ), 165 - Round( sqrt( 12100 - (pos_roll * pos_roll) ) ), 240 + Round( pos_roll ), 134 );// roll
		Rectangle( hDC, 270, 164 + Round( pos_pitch ), 239 + Round( sqrt( 12100 - (pos_pitch * pos_pitch) ) ), 166 + Round( pos_pitch ) );// pitch
		Rectangle( hDC, 238 + Round( pos_yaw ), 196, 240 + Round( pos_yaw ), 165 + Round( sqrt( 12100 - (pos_yaw * pos_yaw) ) ) );// yaw
		return;// 20/5/1 10/5/1 2.5/2.5/2.5
	}

	void MDU::ADI_ERROR_D( oapi::Sketchpad2* skp, double pitch, double roll, double yaw, int adierr )
	{
		double pos_pitch;
		double pos_roll;
		double pos_yaw;

		skp->SetTextColor( CR_MAGENTA );
		skp->SetFont( skpSSUBFont_h12 );
		if (adierr == 1)
		{
			// ADI ERROR MED
			// 5/5/2.5
			skp->Text( 337, 105, "5", 1 );
			skp->Text( 337, 208, "5", 1 );

			if (roll > 5) pos_roll = 47;
			else if (roll < -5) pos_roll = -47;
			else pos_roll = roll * 9.4;

			if (pitch > 5) pos_pitch = 47;
			else if (pitch < -5) pos_pitch = -47;
			else pos_pitch = pitch * 9.4;
		}
		else if (adierr == 2)
		{
			// ADI ERROR HIGH
			// 20/10/2.5
			skp->Text( 337, 105, "10", 2 );
			skp->Text( 337, 208, "10", 2 );

			if (roll > 10) pos_roll = 47;
			else if (roll < -10) pos_roll = -47;
			else pos_roll = roll * 4.7;

			if (pitch > 10) pos_pitch = 47;
			else if (pitch < -10) pos_pitch = -47;
			else pos_pitch = pitch * 4.7;
		}
		else
		{
			// ADI ERROR LOW
			// 1/1/2.5
			skp->Text( 337, 105, "1", 1 );
			skp->Text( 337, 208, "1", 1 );

			if (roll > 1) pos_roll = 47;
			else if (roll < -1) pos_roll = -47;
			else pos_roll = roll * 47;

			if (pitch > 1) pos_pitch = 47;
			else if (pitch < -1) pos_pitch = -47;
			else pos_pitch = pitch * 47;
		}
		if (yaw > 2.5) pos_yaw = 47;
		else if (yaw < -2.5) pos_yaw = -47;
		else pos_yaw = yaw * 18.8;

		// draw needles
		skp->SetPen( skpMagentaPen );
		skp->SetBrush( skpMagentaBrush );
		skp->Rectangle( 238 + Round( pos_roll ), 165 - Round( sqrt( 12100 - (pos_roll * pos_roll) ) ), 240 + Round( pos_roll ), 134 );// roll
		skp->Rectangle( 270, 164 + Round( pos_pitch ), 239 + Round( sqrt( 12100 - (pos_pitch * pos_pitch) ) ), 166 + Round( pos_pitch ) );// pitch
		skp->Rectangle( 238 + Round( pos_yaw ), 196, 240 + Round( pos_yaw ), 165 + Round( sqrt( 12100 - (pos_yaw * pos_yaw) ) ) );// yaw
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
		SelectObject( hDC, gdiSSUAFont_h11w9 );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		if (GetIDP()->GetOnHACState() == false)
		{
			sprintf_s( cbuf, 8, "%cAZ", 255 );
			TextOut( hDC, 315, 327, cbuf, 3 );
			dtmp = fabs( GetIDP()->GetdeltaAZ() );
			if (dtmp >= GetIDP()->GetdeltaAZLimit( MachNumber ))
			{
				// rectangle flashes red for limits
				if (GetFlash() == false) SelectObject( hDC, gdiBlackPen );
				else SelectObject( hDC, gdiRedPen );
			}
			else SelectObject( hDC, gdiLightGrayPen );
		}
		else
		{
			TextOut( hDC, 315, 327, "HTA", 3 );
			dtmp = GetIDP()->GetHTA();
			SelectObject( hDC, gdiLightGrayPen );
		}

		Rectangle( hDC, 346, 322, 378, 343 );

		SelectObject( hDC, gdiSSUBFont_h12w7 );
		SetTextColor( hDC, CR_WHITE );
		SetTextAlign( hDC, TA_RIGHT );
		sprintf_s( cbuf, 8, "%3.0f%c", dtmp, 176 );
		TextOut( hDC, 377, 325, cbuf, strlen( cbuf ) );
		SetTextAlign( hDC, TA_LEFT );
		return;
	}
	
	void MDU::AEPFD_dAZ_HTA( oapi::Sketchpad2* skp, double MachNumber )
	{
		double dtmp;
		char cbuf[8];
		skp->SetBrush( skpBlackBrush );
		skp->SetFont( skpSSUAFont_h11 );
		skp->SetTextColor( CR_LIGHT_GRAY );
		if (GetIDP()->GetOnHACState() == false)
		{
			sprintf_s( cbuf, 8, "%cAZ", 255 );
			skp->Text( 315, 327, cbuf, 3 );
			dtmp = fabs( GetIDP()->GetdeltaAZ() );
			if (dtmp >= GetIDP()->GetdeltaAZLimit( MachNumber ))
			{
				// rectangle flashes red for limits
				if (GetFlash() == false) skp->SetPen( skpBlackPen );
				else skp->SetPen( skpRedPen );
			}
			else skp->SetPen( skpLightGrayPen );
		}
		else
		{
			skp->Text( 315, 327, "HTA", 3 );
			dtmp = GetIDP()->GetHTA();
			skp->SetPen( skpLightGrayPen );
		}

		skp->Rectangle( 346, 322, 378, 343 );

		skp->SetFont( skpSSUBFont_h12 );
		skp->SetTextColor( CR_WHITE );
		skp->SetTextAlign( oapi::Sketchpad::RIGHT );
		sprintf_s( cbuf, 8, "%3.0f%c", dtmp, 176 );
		skp->Text( 377, 325, cbuf, strlen( cbuf ) );
		skp->SetTextAlign( oapi::Sketchpad::LEFT );
		return;
	}

	void MDU::AEPFD_dXTRK( HDC hDC )
	{
		SelectObject( hDC, gdiSSUAFont_h11w9 );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%c X-Trk", 255 );
		TextOut( hDC, 379, 294, cbuf, 7 );

		SelectObject( hDC, gdiLightGrayPen );
		SelectObject( hDC, gdiBlackBrush );
		Rectangle( hDC, 446, 289, 496, 313 );

		SelectObject( hDC, gdiSSUBFont_h18w9 );
		SetTextColor( hDC, CR_WHITE );
		SetTextAlign( hDC, TA_RIGHT );
		double dtmp = 0;// TODO
		sprintf_s( cbuf, 8, "%4.1f", dtmp );
		TextOut( hDC, 493, 290, cbuf, strlen( cbuf ) );
		SetTextAlign( hDC, TA_LEFT );
		return;
	}

	void MDU::AEPFD_dXTRK( oapi::Sketchpad2* skp )
	{
		skp->SetFont( skpSSUAFont_h11 );
		skp->SetTextColor( CR_LIGHT_GRAY );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%c X-Trk", 255 );
		skp->Text( 379, 294, cbuf, 7 );

		skp->SetPen( skpLightGrayPen );
		skp->SetBrush( skpBlackBrush );
		skp->Rectangle( 446, 289, 496, 313 );

		skp->SetFont( skpSSUBFont_h18 );
		skp->SetTextColor( CR_WHITE );
		skp->SetTextAlign( oapi::Sketchpad::RIGHT );
		double dtmp = 0;// TODO
		sprintf_s( cbuf, 8, "%4.1f", dtmp );
		skp->Text( 493, 290, cbuf, strlen( cbuf ) );
		skp->SetTextAlign( oapi::Sketchpad::LEFT );
		return;
	}

	void MDU::AEPFD_XTRK( HDC hDC )
	{
		SelectObject( hDC, gdiSSUAFont_h11w9 );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 397, 337, "X-Trk", 5 );

		SelectObject( hDC, gdiLightGrayPen );
		SelectObject( hDC, gdiBlackBrush );
		Rectangle( hDC, 446, 332, 496, 356 );

		SelectObject( hDC, gdiSSUBFont_h18w9 );
		SetTextColor( hDC, CR_WHITE );
		SetTextAlign( hDC, TA_RIGHT );
		double dtmp = 0;// TODO
		char cbuf[8];
		sprintf_s( cbuf, 8, "%4.1f", dtmp );
		TextOut( hDC, 493, 333, cbuf, strlen( cbuf ) );
		SetTextAlign( hDC, TA_LEFT );
		return;
	}

	void MDU::AEPFD_XTRK( oapi::Sketchpad2* skp )
	{
		skp->SetFont( skpSSUAFont_h11 );
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 397, 337, "X-Trk", 5 );

		skp->SetPen( skpLightGrayPen );
		skp->SetBrush( skpBlackBrush );
		skp->Rectangle( 446, 332, 496, 356 );

		skp->SetFont( skpSSUBFont_h18 );
		skp->SetTextColor( CR_WHITE );
		skp->SetTextAlign( oapi::Sketchpad::RIGHT );
		double dtmp = 0;// TODO
		char cbuf[8];
		sprintf_s( cbuf, 8, "%4.1f", dtmp );
		skp->Text( 493, 333, cbuf, strlen( cbuf ) );
		skp->SetTextAlign( oapi::Sketchpad::LEFT );
		return;
	}

	void MDU::AEPFD_dINC( HDC hDC )
	{
		SelectObject( hDC, gdiSSUAFont_h11w9 );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%c Inc", 255 );
		TextOut( hDC, 388, 380, cbuf, 5 );

		SelectObject( hDC, gdiLightGrayPen );
		SelectObject( hDC, gdiBlackBrush );
		Rectangle( hDC, 437, 375, 496, 399 );

		SelectObject( hDC, gdiSSUBFont_h18w9 );
		SetTextColor( hDC, CR_WHITE );
		SetTextAlign( hDC, TA_RIGHT );
		ELEMENTS el;
		STS()->GetElements( STS()->GetGravityRef(), el, NULL, 0, FRAME_EQU );
		sprintf_s( cbuf, 8, "%6.2f", (STS()->pMission->GetMECOInc() - el.i) * DEG );
		TextOut( hDC, 493, 376, cbuf, strlen( cbuf ) );
		SetTextAlign( hDC, TA_LEFT );
		return;
	}

	void MDU::AEPFD_dINC( oapi::Sketchpad2* skp )
	{
		skp->SetFont( skpSSUAFont_h11 );
		skp->SetTextColor( CR_LIGHT_GRAY );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%c Inc", 255 );
		skp->Text( 388, 380, cbuf, 5 );

		skp->SetPen( skpLightGrayPen );
		skp->SetBrush( skpBlackBrush );
		skp->Rectangle( 437, 375, 496, 399 );

		skp->SetFont( skpSSUBFont_h18 );
		skp->SetTextColor( CR_WHITE );
		skp->SetTextAlign( oapi::Sketchpad::RIGHT );
		ELEMENTS el;
		STS()->GetElements( STS()->GetGravityRef(), el, NULL, 0, FRAME_EQU );
		sprintf_s( cbuf, 8, "%6.2f", (STS()->pMission->GetMECOInc() - el.i) * DEG );
		skp->Text( 493, 376, cbuf, strlen( cbuf ) );
		skp->SetTextAlign( oapi::Sketchpad::LEFT );
		return;
	}

	void MDU::AEPFD_TGTINC( HDC hDC )
	{
		SelectObject( hDC, gdiSSUAFont_h11w9 );
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 370, 423, "Tgt Inc", 7 );
		
		SelectObject( hDC, gdiLightGrayPen );
		SelectObject( hDC, gdiBlackBrush );
		Rectangle( hDC, 437, 418, 496, 442 );
		
		SelectObject( hDC, gdiSSUBFont_h18w9 );
		SetTextColor( hDC, CR_WHITE );
		SetTextAlign( hDC, TA_RIGHT );
		double dtmp = 0;// TODO
		char cbuf[8];
		sprintf_s( cbuf, 8, "%5.2f", dtmp );
		TextOut( hDC, 489, 419, cbuf, strlen( cbuf ) );
		SetTextAlign( hDC, TA_LEFT );
		return;
	}

	void MDU::AEPFD_TGTINC( oapi::Sketchpad2* skp )
	{
		skp->SetFont( skpSSUAFont_h11 );
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 370, 423, "Tgt Inc", 7 );
		
		skp->SetPen( skpLightGrayPen );
		skp->SetBrush( skpBlackBrush );
		skp->Rectangle( 437, 418, 496, 442 );
		
		skp->SetFont( skpSSUBFont_h18 );
		skp->SetTextColor( CR_WHITE );
		skp->SetTextAlign( oapi::Sketchpad::RIGHT );
		double dtmp = 0;// TODO
		char cbuf[8];
		sprintf_s( cbuf, 8, "%5.2f", dtmp );
		skp->Text( 489, 419, cbuf, strlen( cbuf ) );
		skp->SetTextAlign( oapi::Sketchpad::LEFT );
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
