#include "../Atlantis.h"
#include "MDU.h"
#include "../dps/IDP.h"


namespace vc
{
	void MDU::ORBITPFD( HDC hDC )
	{
		int MM = STS()->pSimpleGPC->GetMajorMode();
		//int adiatt = GetIDP()->GetADIAttitude();
		int adierr = GetIDP()->GetADIError();
		int adirate = GetIDP()->GetADIRate();
		VECTOR3 av;// x = pitch, y = yaw, z = roll
		STS()->GetAngularVel( av );
		av *= DEG;
		av.z = -av.z;
		av.x = -av.x;
		VECTOR3 atterr = GetIDP()->GetAttitudeCommandErrors();
		double attRoll = -STS()->GetBank() * DEG;
		if (attRoll <= 0) attRoll += 360;
		double attPitch = STS()->GetPitch() * DEG;
		if (attPitch <= 0) attPitch += 360;
		double attYaw = 0;//STS()->GetSlipAngle() * DEG;
		//if (attYaw <= 0) attYaw += 360;

		int save = SaveDC( hDC );

		SelectObject( hDC, gdiTahomaFont_h10w4 );

		ADI_STATIC_ORBIT( hDC );
		if ((MM == 201) || (MM == 202) || (MM == 801))
		{
			ADI_ORBIT( hDC, attPitch, attRoll, attYaw );
			ADI_RATE_ORBIT( hDC, av.x, av.z, av.y, adirate );
			ADI_ERROR_ORBIT( hDC, atterr.x, atterr.z, atterr.y, adierr );
		}
		else ADI_ORBIT( hDC, 129, 77, 14 );


		RestoreDC( hDC, save );
		return;
	}

	void MDU::ORBITPFD( oapi::Sketchpad2* skp )
	{
		int MM = STS()->pSimpleGPC->GetMajorMode();
		//int adiatt = GetIDP()->GetADIAttitude();
		int adierr = GetIDP()->GetADIError();
		int adirate = GetIDP()->GetADIRate();
		VECTOR3 av;// x = pitch, y = yaw, z = roll
		STS()->GetAngularVel( av );
		av *= DEG;
		av.z = -av.z;
		av.x = -av.x;
		VECTOR3 atterr = GetIDP()->GetAttitudeCommandErrors();
		double attRoll = -STS()->GetBank() * DEG;
		if (attRoll <= 0) attRoll += 360;
		double attPitch = STS()->GetPitch() * DEG;
		if (attPitch <= 0) attPitch += 360;
		double attYaw = STS()->GetSlipAngle() * DEG;
		if (attYaw <= 0) attYaw += 360;

		skp->SetFont( skpTahomaFont_h10w4 );

		ADI_STATIC_ORBIT( skp );
		if ((MM == 201) || (MM == 202) || (MM == 801))
		{
			ADI_ORBIT( skp, attPitch, attRoll, attYaw );
			ADI_RATE_ORBIT( skp, av.x, av.z, av.y, adirate );
			ADI_ERROR_ORBIT( skp, atterr.x, atterr.z, atterr.y, adierr );
		}
		else ADI_ORBIT( skp, 129, 77, 14 );
		return;
	}
	
	void MDU::ADI_STATIC_ORBIT( HDC hDC )
	{
		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 186, 42, "R", 1 );
		TextOut( hDC, 186, 50, "P", 1 );
		TextOut( hDC, 186, 58, "Y", 1 );

		SelectObject( hDC, gdiWhitePen );
		::Ellipse( hDC, 64, 64, 192, 192 );

		// rate scales
		// top scale
		SelectObject( hDC, gdiDarkGrayPen );
		MoveToEx( hDC, 89, 53, NULL );
		LineTo( hDC, 167, 53 );
		MoveToEx( hDC, 89, 53, NULL );
		LineTo( hDC, 89, 61 );
		MoveToEx( hDC, 97, 53, NULL );
		LineTo( hDC, 97, 58 );
		MoveToEx( hDC, 104, 53, NULL );
		LineTo( hDC, 104, 58 );
		MoveToEx( hDC, 112, 53, NULL );
		LineTo( hDC, 112, 58 );
		MoveToEx( hDC, 120, 53, NULL );
		LineTo( hDC, 120, 58 );
		MoveToEx( hDC, 128, 53, NULL );
		LineTo( hDC, 128, 61 );
		MoveToEx( hDC, 136, 53, NULL );
		LineTo( hDC, 136, 58 );
		MoveToEx( hDC, 144, 53, NULL );
		LineTo( hDC, 144, 58 );
		MoveToEx( hDC, 152, 53, NULL );
		LineTo( hDC, 152, 58 );
		MoveToEx( hDC, 159, 53, NULL );
		LineTo( hDC, 159, 58 );
		MoveToEx( hDC, 167, 53, NULL );
		LineTo( hDC, 167, 61 );

		// side scale
		MoveToEx( hDC, 204, 89, NULL );
		LineTo( hDC, 204, 167 );
		MoveToEx( hDC, 204, 89, NULL );
		LineTo( hDC, 196, 89 );
		MoveToEx( hDC, 204, 97, NULL );
		LineTo( hDC, 199, 97 );
		MoveToEx( hDC, 204, 104, NULL );
		LineTo( hDC, 199, 104 );
		MoveToEx( hDC, 204, 112, NULL );
		LineTo( hDC, 199, 112 );
		MoveToEx( hDC, 204, 120, NULL );
		LineTo( hDC, 199, 120 );
		MoveToEx( hDC, 204, 128, NULL );
		LineTo( hDC, 196, 128 );
		MoveToEx( hDC, 204, 136, NULL );
		LineTo( hDC, 199, 136 );
		MoveToEx( hDC, 204, 144, NULL );
		LineTo( hDC, 199, 144 );
		MoveToEx( hDC, 204, 152, NULL );
		LineTo( hDC, 199, 152 );
		MoveToEx( hDC, 204, 159, NULL );
		LineTo( hDC, 199, 159 );
		MoveToEx( hDC, 204, 167, NULL );
		LineTo( hDC, 196, 167 );////////

		// bottom scale
		MoveToEx( hDC, 89, 205, NULL );
		LineTo( hDC, 167, 205 );
		MoveToEx( hDC, 89, 205, NULL );
		LineTo( hDC, 89, 198 );
		MoveToEx( hDC, 97, 205, NULL );
		LineTo( hDC, 97, 200 );
		MoveToEx( hDC, 104, 205, NULL );
		LineTo( hDC, 104, 200 );
		MoveToEx( hDC, 112, 205, NULL );
		LineTo( hDC, 112, 200 );
		MoveToEx( hDC, 120, 205, NULL );
		LineTo( hDC, 120, 200 );
		MoveToEx( hDC, 128, 205, NULL );
		LineTo( hDC, 128, 198 );
		MoveToEx( hDC, 136, 205, NULL );
		LineTo( hDC, 136, 200 );
		MoveToEx( hDC, 144, 205, NULL );
		LineTo( hDC, 144, 200 );
		MoveToEx( hDC, 152, 205, NULL );
		LineTo( hDC, 152, 200 );
		MoveToEx( hDC, 159, 205, NULL );
		LineTo( hDC, 159, 200 );
		MoveToEx( hDC, 167, 205, NULL );
		LineTo( hDC, 167, 198 );

		// roll lines
		// 10º/190º
		MoveToEx( hDC, 118, 71, NULL );
		LineTo( hDC, 138, 185 );
		// 20º/200º
		MoveToEx( hDC, 108, 73, NULL );
		LineTo( hDC, 148, 183 );
		// 40º/220º
		MoveToEx( hDC, 91, 83, NULL );
		LineTo( hDC, 165, 173 );
		// 50º/230º
		MoveToEx( hDC, 83, 91, NULL );
		LineTo( hDC, 173, 165 );
		// 70º/250º
		MoveToEx( hDC, 73, 108, NULL );
		LineTo( hDC, 183, 148 );
		// 80º/260º
		MoveToEx( hDC, 71, 118, NULL );
		LineTo( hDC, 185, 138 );
		// 100º/280º
		MoveToEx( hDC, 71, 138, NULL );
		LineTo( hDC, 185, 118 );
		// 110º/290º
		MoveToEx( hDC, 73, 148, NULL );
		LineTo( hDC, 183, 108 );
		// 130º/310º
		MoveToEx( hDC, 83, 165, NULL );
		LineTo( hDC, 173, 91 );
		// 140º/320º
		MoveToEx( hDC, 91, 173, NULL );
		LineTo( hDC, 165, 83 );
		// 160º/340º
		MoveToEx( hDC, 108, 183, NULL );
		LineTo( hDC, 148, 73 );
		// 170º/350º
		MoveToEx( hDC, 118, 185, NULL );
		LineTo( hDC, 138, 71 );
		SelectObject( hDC, gdiDarkGrayThickPen );
		// 30º/210º
		MoveToEx( hDC, 99, 77, NULL );
		LineTo( hDC, 157, 179 );
		// 60º/240º
		MoveToEx( hDC, 77, 99, NULL );
		LineTo( hDC, 179, 157 );
		// 120º/300º
		MoveToEx( hDC, 77, 157, NULL );
		LineTo( hDC, 179, 99 );
		// 150º/330º
		MoveToEx( hDC, 99, 179, NULL );
		LineTo( hDC, 157, 77 );
		// cover center for ADI SRCPAINT
		SelectObject( hDC, gdiBlackBrush );
		SelectObject( hDC, gdiBlackPen );
		::Ellipse( hDC, 73, 73, 183, 183 );

		TextOut( hDC, 158, 75, "33", 2 );
		TextOut( hDC, 173, 91, "30", 2 );
		TextOut( hDC, 173, 156, "24", 2 );
		TextOut( hDC, 157, 173, "21", 2 );
		TextOut( hDC, 91, 173, "15", 2 );
		TextOut( hDC, 75, 156, "12", 2 );
		TextOut( hDC, 75, 90, "06", 2 );
		TextOut( hDC, 91, 75, "03", 2 );
		SelectObject( hDC, gdiLightGrayPen );
		SelectObject( hDC, gdiLightGrayBrush );
		POINT diamond[4];
		diamond[0].x = 128;
		diamond[0].y = 66;
		diamond[1].x = 131;
		diamond[1].y = 69;
		diamond[2].x = 128;
		diamond[2].y = 72;
		diamond[3].x = 125;
		diamond[3].y = 69;
		Polygon( hDC, diamond, 4 );// top
		//diamond[0].x = 128;
		diamond[0].y = 183;
		//diamond[1].x = 131;
		diamond[1].y = 186;
		//diamond[2].x = 128;
		diamond[2].y = 189;
		//diamond[3].x = 125;
		diamond[3].y = 186;
		Polygon( hDC, diamond, 4 );// bottom
		diamond[0].x = 186;
		diamond[0].y = 125;
		diamond[1].x = 189;
		diamond[1].y = 128;
		diamond[2].x = 186;
		diamond[2].y = 131;
		diamond[3].x = 183;
		diamond[3].y = 128;
		Polygon( hDC, diamond, 4 );// right
		diamond[0].x = 69;
		//diamond[0].y = 125;
		diamond[1].x = 72;
		//diamond[1].y = 128;
		diamond[2].x = 69;
		//diamond[2].y = 131;
		diamond[3].x = 66;
		//diamond[3].y = 128;
		Polygon( hDC, diamond, 4 );// left

		// att needles
		// top scale
		SelectObject( hDC, gdiMagentaPen );
		SelectObject( hDC, gdiMagentaBrush );
		Arc( hDC, 66, 66, 190, 190, 156, 73, 100, 73 );
		MoveToEx( hDC, 100, 73, NULL );
		LineTo( hDC, 100, 76 );
		MoveToEx( hDC, 106, 71, NULL );
		LineTo( hDC, 106, 74 );
		MoveToEx( hDC, 111, 68, NULL );
		LineTo( hDC, 111, 72 );
		MoveToEx( hDC, 117, 67, NULL );
		LineTo( hDC, 117, 71 );
		MoveToEx( hDC, 122, 66, NULL );
		LineTo( hDC, 122, 70 );
		MoveToEx( hDC, 134, 66, NULL );
		LineTo( hDC, 134, 70 );
		MoveToEx( hDC, 139, 67, NULL );
		LineTo( hDC, 139, 71 );
		MoveToEx( hDC, 145, 68, NULL );
		LineTo( hDC, 145, 72 );
		MoveToEx( hDC, 150, 71, NULL );
		LineTo( hDC, 150, 74 );
		MoveToEx( hDC, 156, 73, NULL );
		LineTo( hDC, 156, 76 );

		// side scale
		Arc( hDC, 66, 66, 190, 190, 183, 156, 183, 100 );
		MoveToEx( hDC, 183, 100, NULL );
		LineTo( hDC, 180, 100 );
		MoveToEx( hDC, 184, 106, NULL );
		LineTo( hDC, 181, 106 );
		MoveToEx( hDC, 186, 111, NULL );
		LineTo( hDC, 183, 111 );
		MoveToEx( hDC, 188, 117, NULL );
		LineTo( hDC, 184, 117 );
		MoveToEx( hDC, 189, 122, NULL );
		LineTo( hDC, 185, 122 );
		MoveToEx( hDC, 189, 134, NULL );
		LineTo( hDC, 185, 134 );
		MoveToEx( hDC, 188, 139, NULL );
		LineTo( hDC, 184, 139 );
		MoveToEx( hDC, 186, 145, NULL );
		LineTo( hDC, 183, 145 );
		MoveToEx( hDC, 184, 150, NULL );
		LineTo( hDC, 181, 150 );
		MoveToEx( hDC, 183, 156, NULL );
		LineTo( hDC, 180, 156 );

		// bottom scale
		Arc( hDC, 66, 66, 190, 190, 100, 183, 156, 183 );
		MoveToEx( hDC, 100, 183, NULL );
		LineTo( hDC, 100, 180 );
		MoveToEx( hDC, 106, 184, NULL );
		LineTo( hDC, 106, 181 );
		MoveToEx( hDC, 111, 186, NULL );
		LineTo( hDC, 111, 183 );
		MoveToEx( hDC, 117, 188, NULL );
		LineTo( hDC, 117, 184 );
		MoveToEx( hDC, 122, 189, NULL );
		LineTo( hDC, 122, 185 );
		MoveToEx( hDC, 134, 189, NULL );
		LineTo( hDC, 134, 185 );
		MoveToEx( hDC, 139, 188, NULL );
		LineTo( hDC, 139, 184 );
		MoveToEx( hDC, 145, 186, NULL );
		LineTo( hDC, 145, 183 );
		MoveToEx( hDC, 151, 184, NULL );
		LineTo( hDC, 151, 181 );
		MoveToEx( hDC, 156, 183, NULL );
		LineTo( hDC, 156, 180 );
		return;
	}

	void MDU::ADI_STATIC_ORBIT( oapi::Sketchpad2* skp )
	{
		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 186, 42, "R", 1 );
		skp->Text( 186, 50, "P", 1 );
		skp->Text( 186, 58, "Y", 1 );

		skp->SetPen( skpWhitePen );
		skp->Ellipse( 64, 64, 192, 192 );

		// rate scales
		// top scale
		skp->SetPen( skpDarkGrayPen );
		skp->Line( 89, 53, 167, 53 );
		skp->Line( 89, 53, 89, 61 );
		skp->Line( 97, 53, 97, 58 );
		skp->Line( 104, 53, 104, 58 );
		skp->Line( 112, 53, 112, 58 );
		skp->Line( 120, 53, 120, 58 );
		skp->Line( 128, 53, 128, 61 );
		skp->Line( 136, 53, 136, 58 );
		skp->Line( 144, 53, 144, 58 );
		skp->Line( 152, 53, 152, 58 );
		skp->Line( 159, 53, 159, 58 );
		skp->Line( 167, 53, 167, 61 );

		// side scale
		skp->Line( 204, 89, 204, 167 );
		skp->Line( 204, 89, 196, 89 );
		skp->Line( 204, 97, 199, 97 );
		skp->Line( 204, 104, 199, 104 );
		skp->Line( 204, 112, 199, 112 );
		skp->Line( 204, 120, 199, 120 );
		skp->Line( 204, 128, 196, 128 );
		skp->Line( 204, 136, 199, 136 );
		skp->Line( 204, 144, 199, 144 );
		skp->Line( 204, 152, 199, 152 );
		skp->Line( 204, 159, 199, 159 );
		skp->Line( 204, 167, 196, 167 );

		// bottom scale
		skp->Line( 89, 205, 167, 205 );
		skp->Line( 89, 205, 89, 198 );
		skp->Line( 97, 205, 97, 200 );
		skp->Line( 104, 205, 104, 200 );
		skp->Line( 112, 205, 112, 200 );
		skp->Line( 120, 205, 120, 200 );
		skp->Line( 128, 205, 128, 198 );
		skp->Line( 136, 205, 136, 200 );
		skp->Line( 144, 205, 144, 200 );
		skp->Line( 152, 205, 152, 200 );
		skp->Line( 159, 205, 159, 200 );
		skp->Line( 167, 205, 167, 198 );

		// roll lines
		// 10º/190º
		skp->Line( 118, 71, 138, 185 );
		// 20º/200º
		skp->Line( 108, 73, 148, 183 );
		// 40º/220º
		skp->Line( 91, 83, 165, 173 );
		// 50º/230º
		skp->Line( 83, 91, 173, 165 );
		// 70º/250º
		skp->Line( 73, 108, 183, 148 );
		// 80º/260º
		skp->Line( 71, 118, 185, 138 );
		// 100º/280º
		skp->Line( 71, 138, 185, 118 );
		// 110º/290º
		skp->Line( 73, 148, 183, 108 );
		// 130º/310º
		skp->Line( 83, 165, 173, 91 );
		// 140º/320º
		skp->Line( 91, 173, 165, 83 );
		// 160º/340º
		skp->Line( 108, 183, 148, 73 );
		// 170º/350º
		skp->Line( 118, 185, 138, 71 );
		skp->SetPen( skpDarkGrayThickPen );
		// 30º/210º
		skp->Line( 99, 77, 157, 179 );
		// 60º/240º
		skp->Line( 77, 99, 179, 157 );
		// 120º/300º
		skp->Line( 77, 157, 179, 99 );
		// 150º/330º
		skp->Line( 99, 179, 157, 77 );
		// cover center for ADI SRCPAINT
		skp->SetBrush( skpBlackBrush );
		skp->SetPen( skpBlackPen );
		skp->Ellipse( 73, 73, 183, 183 );

		skp->Text( 158, 75, "33", 2 );
		skp->Text( 173, 91, "30", 2 );
		skp->Text( 173, 156, "24", 2 );
		skp->Text( 157, 173, "21", 2 );
		skp->Text( 91, 173, "15", 2 );
		skp->Text( 75, 156, "12", 2 );
		skp->Text( 75, 90, "06", 2 );
		skp->Text( 91, 75, "03", 2 );
		skp->SetPen( skpLightGrayPen );
		skp->SetBrush( skpLightGrayBrush );
		oapi::IVECTOR2 diamond[4];
		diamond[0].x = 128;
		diamond[0].y = 66;
		diamond[1].x = 131;
		diamond[1].y = 69;
		diamond[2].x = 128;
		diamond[2].y = 72;
		diamond[3].x = 125;
		diamond[3].y = 69;
		skp->Polygon( diamond, 4 );// top
		//diamond[0].x = 128;
		diamond[0].y = 183;
		//diamond[1].x = 131;
		diamond[1].y = 186;
		//diamond[2].x = 128;
		diamond[2].y = 189;
		//diamond[3].x = 125;
		diamond[3].y = 186;
		skp->Polygon( diamond, 4 );// bottom
		diamond[0].x = 186;
		diamond[0].y = 125;
		diamond[1].x = 189;
		diamond[1].y = 128;
		diamond[2].x = 186;
		diamond[2].y = 131;
		diamond[3].x = 183;
		diamond[3].y = 128;
		skp->Polygon( diamond, 4 );// right
		diamond[0].x = 69;
		//diamond[0].y = 125;
		diamond[1].x = 72;
		//diamond[1].y = 128;
		diamond[2].x = 69;
		//diamond[2].y = 131;
		diamond[3].x = 66;
		//diamond[3].y = 128;
		skp->Polygon( diamond, 4 );// left

		// att needles
		// top scale
		skp->SetPen( skpMagentaPen );
		skp->SetBrush( skpMagentaBrush );
		//Arc( hDC, 66, 66, 190, 190, 156, 73, 100, 73 );
		skp->Line( 100, 73, 100, 76 );
		skp->Line( 106, 71, 106, 74 );
		skp->Line( 111, 68, 111, 72 );
		skp->Line( 117, 67, 117, 71 );
		skp->Line( 122, 66, 122, 70 );
		skp->Line( 134, 66, 134, 70 );
		skp->Line( 139, 67, 139, 71 );
		skp->Line( 145, 68, 145, 72 );
		skp->Line( 150, 71, 150, 74 );
		skp->Line( 156, 73, 156, 76 );

		// side scale
		//Arc( hDC, 66, 66, 190, 190, 183, 156, 183, 100 );
		skp->Line( 183, 100, 180, 100 );
		skp->Line( 184, 106, 181, 106 );
		skp->Line( 186, 111, 183, 111 );
		skp->Line( 188, 117, 184, 117 );
		skp->Line( 189, 122, 185, 122 );
		skp->Line( 189, 134, 185, 134 );
		skp->Line( 188, 139, 184, 139 );
		skp->Line( 186, 145, 183, 145 );
		skp->Line( 184, 150, 181, 150 );
		skp->Line( 183, 156, 180, 156 );

		// bottom scale
		//Arc( hDC, 66, 66, 190, 190, 100, 183, 156, 183 );
		skp->Line( 100, 183, 100, 180 );
		skp->Line( 106, 184, 106, 181 );
		skp->Line( 111, 186, 111, 183 );
		skp->Line( 117, 188, 117, 184 );
		skp->Line( 122, 189, 122, 185 );
		skp->Line( 134, 189, 134, 185 );
		skp->Line( 139, 188, 139, 184 );
		skp->Line( 145, 186, 145, 183 );
		skp->Line( 151, 184, 151, 181 );
		skp->Line( 156, 183, 156, 180 );
		return;
	}

	void MDU::ADI_ORBIT( HDC hDC, double pitch, double roll, double yaw )
	{
		// center (128,128) r = 64
		// view r = 55, ball r = 55 * sqrt( 2 )
		// 90º pitch/yaw "FOV"

		// apply roll
		XFORM WTroll;
		WTroll.eM11 = (FLOAT)cos( roll * RAD );
		WTroll.eM12 = (FLOAT)(-sin( roll * RAD ));
		WTroll.eM21 = -WTroll.eM12;
		WTroll.eM22 = WTroll.eM11;
		WTroll.eDx = (FLOAT)(56 * (1 - WTroll.eM11 - WTroll.eM21));
		WTroll.eDy = (FLOAT)(56 * (1 - WTroll.eM11 + WTroll.eM21));
		SetGraphicsMode( hDC_ADI_ORBIT, GM_ADVANCED );
		SetWorldTransform( hDC_ADI_ORBIT, &WTroll );

		if (pitch > 180) pitch -= 360;// TODO get rid of this

		// 0º pitch line/labels
		double sinpitch = sin( pitch * RAD );
		double cospitch = cos( pitch * RAD );
		int hP;
		double hP2;
		SetTextColor( hDC_ADI_ORBIT, CR_BLACK );
		SetBkColor( hDC_ADI_ORBIT, CR_WHITE );
		if (fabs( pitch ) <= 45)
		{
			hP = Round( 77.7817 * sinpitch );
			if (pitch < 0)
			{
				SelectObject( hDC_ADI_ORBIT, gdiWhiteBrush );
				SelectObject( hDC_ADI_ORBIT, gdiWhitePen );
				Rectangle( hDC_ADI_ORBIT, 0, 0, 112, 56 );
				SelectObject( hDC_ADI_ORBIT, gdiLightGrayBrush );
				SelectObject( hDC_ADI_ORBIT, gdiLightGrayPen );
				Chord( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, 134, 56, -22, 56 );
				Rectangle( hDC_ADI_ORBIT, 0, 56, 112, 112 );
				SelectObject( hDC_ADI_ORBIT, gdiBlackPen );
				Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, 134, 56, -22, 56 );
			}
			else
			{
				SelectObject( hDC_ADI_ORBIT, gdiLightGrayBrush );
				SelectObject( hDC_ADI_ORBIT, gdiLightGrayPen );
				Rectangle( hDC_ADI_ORBIT, 0, 56, 112, 112 );
				SelectObject( hDC_ADI_ORBIT, gdiWhiteBrush );
				SelectObject( hDC_ADI_ORBIT, gdiWhitePen );
				Chord( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, -22, 56, 134, 56 );
				Rectangle( hDC_ADI_ORBIT, 0, 0, 112, 56 );
				SelectObject( hDC_ADI_ORBIT, gdiBlackPen );
				Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, -22, 56, 134, 56 );
			}
			hP = 51 + Round( 75.131397 * sinpitch );
			TextOut( hDC_ADI_ORBIT, 35, hP, "0", 1 );
			TextOut( hDC_ADI_ORBIT, 75, hP, "0", 1 );
		}
		else if (pitch > 0)
		{
			SelectObject( hDC_ADI_ORBIT, gdiWhiteBrush );
			Rectangle( hDC_ADI_ORBIT, 0, 0, 112, 112 );
		}
		else
		{
			SelectObject( hDC_ADI_ORBIT, gdiLightGrayBrush );
			Rectangle( hDC_ADI_ORBIT, 0, 0, 112, 112 );
		}

		// pitch lines/labels for +30º/+60º/+90º/+120º/+150º
		SelectObject( hDC_ADI_ORBIT, gdiDarkGrayPen );
		// +30º
		if (fabs( pitch - 30 ) <= 45)
		{
			hP2 = sinpitch * 67.360968 - cospitch * 38.890873;//hP = 69.296 * sin( (pitch - 30) * RAD );
			hP = Round( hP2 );
			if (pitch < 30) Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, 134, 56, -22, 56 );
			else Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, -22, 56, 134, 56 );
			hP = 51 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI_ORBIT, 35, hP, "3", 1 );
			TextOut( hDC_ADI_ORBIT, 75, hP, "3", 1 );
		}
		// +60º
		if (fabs( pitch - 60 ) <= 45)
		{
			hP2 = sinpitch * 38.890873 - cospitch * 67.360968;
			hP = Round( hP2 );
			if (pitch < 60) Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, 134, 56, -22, 56 );
			else Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, -22, 56, 134, 56 );
			hP = 51 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI_ORBIT, 35, hP, "6", 1 );
			TextOut( hDC_ADI_ORBIT, 75, hP, "6", 1 );
		}
		// +90º
		if (fabs( pitch - 90 ) <= 45)
		{
			hP2 = 77.7817 * (-cospitch);
			hP = Round( hP2 );
			if (pitch < 90) Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, 134, 56, -22, 56 );
			else Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, -22, 56, 134, 56 );
			hP = 51 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI_ORBIT, 35, hP, "9", 1 );
			TextOut( hDC_ADI_ORBIT, 75, hP, "9", 1 );
		}
		// +120º
		if (fabs( pitch - 120 ) <= 45)
		{
			hP2 = (-sinpitch) * 38.890873 - cospitch * 67.360968;
			hP = Round( hP2 );
			if (pitch < 120) Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, 134, 56, -22, 56 );
			else Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, -22, 56, 134, 56 );
			hP = 51 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI_ORBIT, 33, hP, "12", 2 );
			TextOut( hDC_ADI_ORBIT, 73, hP, "12", 2 );
		}
		// +150º
		if (fabs( pitch - 150 ) <= 45)
		{
			hP2 = (-sinpitch) * 67.360968 - cospitch * 38.890873;
			hP = Round( hP2 );
			if (pitch < 150) Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, 134, 56, -22, 56 );
			else Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, -22, 56, 134, 56 );
			hP = 51 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI_ORBIT, 33, hP, "15", 2 );
			TextOut( hDC_ADI_ORBIT, 73, hP, "15", 2 );
		}

		// pitch lines/labels for -30º/-60º/-90º/-120º/-150º
		SelectObject( hDC_ADI_ORBIT, gdiWhitePen );
		SetTextColor( hDC_ADI_ORBIT, CR_WHITE );
		SetBkColor( hDC_ADI_ORBIT, CR_LIGHT_GRAY );
		// -30º
		if (fabs( pitch + 30 ) <= 45)
		{
			hP2 = sinpitch * 67.360968 + cospitch * 38.890873;
			hP = Round( hP2 );
			if (pitch < -30) Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, 134, 56, -22, 56 );
			else Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, -22, 56, 134, 56 );
			hP = 51 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI_ORBIT, 33, hP, "33", 2 );
			TextOut( hDC_ADI_ORBIT, 73, hP, "33", 2 );
		}
		// -60º
		if (fabs( pitch + 60 ) <= 45)
		{
			hP2 = sinpitch * 38.890873 + cospitch * 67.360968;
			hP = Round( hP2 );
			if (pitch < -60) Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, 134, 56, -22, 56 );
			else Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, -22, 56, 134, 56 );
			hP = 51 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI_ORBIT, 33, hP, "30", 2 );
			TextOut( hDC_ADI_ORBIT, 73, hP, "30", 2 );
		}
		// -90º
		if (fabs( pitch + 90 ) <= 45)
		{
			hP2 = 77.7817 * cospitch;
			hP = Round( hP2 );
			if (pitch < -90) Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, 134, 56, -22, 56 );
			else Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, -22, 56, 134, 56 );
			hP = 51 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI_ORBIT, 33, hP, "27", 2 );
			TextOut( hDC_ADI_ORBIT, 73, hP, "27", 2 );
		}
		// -120º
		if (fabs( pitch + 120 ) <= 45)
		{
			hP2 = (-sinpitch) * 38.890873 + cospitch * 67.360968;
			hP = Round( hP2 );
			if (pitch < -120) Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, 134, 56, -22, 56 );
			else Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, -22, 56, 134, 56 );
			hP = 51 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI_ORBIT, 33, hP, "24", 2 );
			TextOut( hDC_ADI_ORBIT, 73, hP, "24", 2 );
		}
		// -150º
		if (fabs( pitch + 150 ) <= 45)
		{
			hP2 = (-sinpitch) * 67.360968 + cospitch * 38.890873;
			hP = Round( hP2 );
			if (pitch < -150) Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, 134, 56, -22, 56 );
			else Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, -22, 56, 134, 56 );
			hP = 51 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI_ORBIT, 33, hP, "21", 2 );
			TextOut( hDC_ADI_ORBIT, 73, hP, "21", 2 );
		}

		// TODO yaw
		// HACK
		// "central plane"
		SelectObject( hDC_ADI_ORBIT, gdiWhiteBrush );
		SelectObject( hDC_ADI_ORBIT, gdiBlackPen );
		Rectangle( hDC_ADI_ORBIT, 54, 0, 58, 112 );
		SelectObject( hDC_ADI_ORBIT, gdiDarkGrayPen );
		MoveToEx( hDC_ADI_ORBIT, 56, 0, NULL );
		LineTo( hDC_ADI_ORBIT, 56, 112 );

		SelectObject( hDC_ADI_ORBIT, gdiDarkGrayPen );
		// yaw line 30º (above horizon)
		MoveToEx( hDC_ADI_ORBIT, 95, 0, NULL );
		LineTo( hDC_ADI_ORBIT, 95, 56 + Round( 67.360968 * sinpitch ) );
		// yaw line 330º (above horizon)
		MoveToEx( hDC_ADI_ORBIT, 17, 0, NULL );
		LineTo( hDC_ADI_ORBIT, 17, 56 + Round( 67.360968 * sinpitch ) );
		SelectObject( hDC_ADI_ORBIT, gdiWhitePen );
		// yaw line 30º (below horizon)
		MoveToEx( hDC_ADI_ORBIT, 95, 112, NULL );
		LineTo( hDC_ADI_ORBIT, 95, 56 + Round( 67.360968 * sinpitch ) );
		// yaw line 330º (below horizon)
		MoveToEx( hDC_ADI_ORBIT, 17, 112, NULL );
		LineTo( hDC_ADI_ORBIT, 17, 56 + Round( 67.360968 * sinpitch ) );

		// TODO yaw labels

		// roll triangle
		SelectObject( hDC_ADI_ORBIT, gdiLightGreenPen );
		SelectObject( hDC_ADI_ORBIT, gdiLightGreenBrush );
		POINT tri[3];
		tri[0].x = 56;
		tri[0].y = 1;
		tri[1].x = 52;
		tri[1].y = 9;
		tri[2].x = 60;
		tri[2].y = 9;
		Polygon( hDC_ADI_ORBIT, tri, 3 );
		
		// clean up
		ModifyWorldTransform( hDC_ADI_ORBIT, &WTroll, MWT_IDENTITY );
		SetGraphicsMode( hDC_ADI_ORBIT, GM_COMPATIBLE );

		// flight director
		SelectObject( hDC_ADI_ORBIT, gdiLightGreenThickPen );
		Arc( hDC_ADI_ORBIT, 49, 49, 63, 64, 49, 56, 63, 56 );
		SelectObject( hDC_ADI_ORBIT, gdiLightGreenPen );
		SelectObject( hDC_ADI_ORBIT, gdiLightGreenBrush );
		Rectangle( hDC_ADI_ORBIT, 55, 37, 57, 75 );
		Rectangle( hDC_ADI_ORBIT, 37, 55, 75, 57 );

		// digital RPY
		if (pitch < 0) pitch += 360;// TODO get rid of this
		SetTextColor( hDC, CR_WHITE );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%03.0f", roll );
		TextOut( hDC, 193, 42, cbuf, strlen( cbuf ) );
		sprintf_s( cbuf, 8, "%03.0f", pitch );
		TextOut( hDC, 193, 50, cbuf, strlen( cbuf ) );
		sprintf_s( cbuf, 8, "%03.0f", yaw );
		TextOut( hDC, 193, 58, cbuf, strlen( cbuf ) );

		BitBlt( hDC_ADI_ORBIT, 0, 0, 112, 112, hDC_ADIMASK_ORBIT, 0, 0, SRCAND );
		BitBlt( hDC, 72, 72, 112, 112, hDC_ADI_ORBIT, 0, 0, SRCPAINT );
		return;
	}

	void MDU::ADI_ORBIT( oapi::Sketchpad2* skp, double pitch, double roll, double yaw )
	{
		// center (128,128) r = 64
		// view r = 55, ball r = 55 * sqrt( 2 )
		// 90º pitch/yaw "FOV"

		// draw ball mesh
		FMATRIX4 mat;
		int H = 256;
		int W = 256;
	
		double croll = cos( roll * RAD );
		double sroll = sin( -roll * RAD );
		double cyaw = cos( yaw * RAD );
		double syaw = sin( yaw * RAD );
		double cpitch = cos( pitch * RAD );
		double spitch = sin( pitch * RAD );
		
		VECTOR3 xdir = _V( cyaw * croll, sroll, -croll * syaw );
		VECTOR3 zdir = _V( syaw * cpitch + sroll * cyaw * spitch, -croll * spitch, cpitch * cyaw - syaw * sroll * spitch );
		gcWorldMatrix( &mat, _V(float(W/2), float(H/2), float(W + 55)), xdir, zdir, 0.7778f );

		skp->SetWorldTransform( &mat );
		skp->SetPen( skpWhitePen );
		skp->DrawSketchMesh( hADIball, 0, 0 );
		skp->SetWorldTransform();

		// roll triangle
		skp->SetPen( skpLightGreenPen );
		skp->SetBrush( skpLightGreenBrush );
		oapi::IVECTOR2 tri[3];
		tri[0].x = (long)(128 + 55 * sroll);
		tri[0].y = (long)(128 - 55 * croll);
		tri[1].x = (long)(128 - 4 * croll + 47 * sroll);
		tri[1].y = (long)(128 - 4 * sroll - 47 * croll);
		tri[2].x = (long)(128 + 4 * croll + 47 * sroll);
		tri[2].y = (long)(128 + 4 * sroll - 47 * croll);
		skp->Polygon( tri, 3 );

		// flight director
		skp->SetPen( skpLightGreenThickPen );
		//Arc( hDC_ADI_ORBIT, 49, 49, 63, 64, 49, 56, 63, 56 );
		skp->SetPen( skpLightGreenPen );
		skp->SetBrush( skpLightGreenBrush );
		skp->Rectangle( 127, 109, 129, 147 );
		skp->Rectangle( 109, 127, 147, 129 );

		// digital RPY
		if (pitch < 0) pitch += 360;// TODO get rid of this
		skp->SetTextColor( CR_WHITE );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%03.0f", roll );
		skp->Text( 193, 42, cbuf, strlen( cbuf ) );
		sprintf_s( cbuf, 8, "%03.0f", pitch );
		skp->Text( 193, 50, cbuf, strlen( cbuf ) );
		sprintf_s( cbuf, 8, "%03.0f", yaw );
		skp->Text( 193, 58, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::ADI_RATE_ORBIT( HDC hDC, double pitch, double roll, double yaw, int adirate )
	{
		int pos_roll;
		int pos_pitch;
		int pos_yaw;

		SetTextColor( hDC, CR_LIGHT_GRAY );
		TextOut( hDC, 126, 43, "0", 1 );
		TextOut( hDC, 207, 124, "0", 1 );
		TextOut( hDC, 126, 206, "0", 1 );
		SetTextColor( hDC, CR_WHITE );
		if (adirate == 1)
		{
			// ADI RATE MED
			TextOut( hDC, 82, 48, "5", 1 );
			TextOut( hDC, 170, 48, "5", 1 );

			TextOut( hDC, 201, 79, "5", 1 );
			TextOut( hDC, 201, 167, "5", 1 );

			TextOut( hDC, 82, 201, "5", 1 );
			TextOut( hDC, 170, 201, "5", 1 );

			if (roll > 5) pos_roll = 167;
			else if (roll < -5) pos_roll = 89;
			else pos_roll = 128 + Round( roll * 7.8 );

			if (pitch > 5) pos_pitch = 167;
			else if (pitch < -5) pos_pitch = 89;
			else pos_pitch = 128 + Round( pitch * 7.8 );

			if (yaw > 5) pos_yaw = 167;
			else if (yaw < -5) pos_yaw = 89;
			else pos_yaw = 128 + Round( yaw * 7.8 );
		}
		else if (adirate == 2)
		{
			// ADI RATE HIGH
			TextOut( hDC, 79, 48, "10", 2 );
			TextOut( hDC, 168, 48, "10", 2 );

			TextOut( hDC, 201, 79, "10", 2 );
			TextOut( hDC, 201, 167, "10", 2 );

			TextOut( hDC, 79, 201, "10", 2 );
			TextOut( hDC, 168, 201, "10", 2 );

			if (roll > 10) pos_roll = 167;
			else if (roll < -10) pos_roll = 89;
			else pos_roll = 128 + Round( roll * 3.9 );

			if (pitch > 10) pos_pitch = 167;
			else if (pitch < -10) pos_pitch = 89;
			else pos_pitch = 128 + Round( pitch * 3.9 );

			if (yaw > 10) pos_yaw = 167;
			else if (yaw < -10) pos_yaw = 89;
			else pos_yaw = 128 + Round( yaw * 3.9 );
		}
		else
		{
			// ADI RATE LOW
			TextOut( hDC, 82, 48, "1", 1 );
			TextOut( hDC, 170, 48, "1", 1 );

			TextOut( hDC, 201, 79, "1", 1 );
			TextOut( hDC, 201, 167, "1", 1 );

			TextOut( hDC, 82, 201, "1", 1 );
			TextOut( hDC, 170, 201, "1", 1 );

			if (roll > 1) pos_roll = 167;
			else if (roll < -1) pos_roll = 89;
			else pos_roll = 128 + Round( roll * 39 );

			if (pitch > 1) pos_pitch = 167;
			else if (pitch < -1) pos_pitch = 89;
			else pos_pitch = 128 + Round( pitch * 39 );

			if (yaw > 1) pos_yaw = 167;
			else if (yaw < -1) pos_yaw = 89;
			else pos_yaw = 128 + Round( yaw * 39 );
		}

		// draw triangles
		SelectObject( hDC, gdiLightGreenPen );
		SelectObject( hDC, gdiLightGreenBrush );
		POINT tri[3];
		tri[0].x = pos_roll;
		tri[0].y = 53;
		tri[1].x = tri[0].x + 5;
		tri[1].y = 44;
		tri[2].x = tri[0].x - 5;
		tri[2].y = 44;
		Polygon( hDC, tri, 3 );

		tri[0].x = 204;
		tri[0].y = pos_pitch;
		tri[1].x = 213;
		tri[1].y = tri[0].y - 5;
		tri[2].x = 213;
		tri[2].y = tri[0].y + 5;
		Polygon( hDC, tri, 3 );

		tri[0].x = pos_yaw;
		tri[0].y = 205;
		tri[1].x = tri[0].x - 5;
		tri[1].y = 214;
		tri[2].x = tri[0].x + 5;
		tri[2].y = 214;
		Polygon( hDC, tri, 3 );
		return;
	}

	void MDU::ADI_RATE_ORBIT( oapi::Sketchpad2* skp, double pitch, double roll, double yaw, int adirate )
	{
		int pos_roll;
		int pos_pitch;
		int pos_yaw;

		skp->SetTextColor( CR_LIGHT_GRAY );
		skp->Text( 126, 43, "0", 1 );
		skp->Text( 207, 124, "0", 1 );
		skp->Text( 126, 206, "0", 1 );
		skp->SetTextColor( CR_WHITE );
		if (adirate == 1)
		{
			// ADI RATE MED
			skp->Text( 82, 48, "5", 1 );
			skp->Text( 170, 48, "5", 1 );

			skp->Text( 201, 79, "5", 1 );
			skp->Text( 201, 167, "5", 1 );

			skp->Text( 82, 201, "5", 1 );
			skp->Text( 170, 201, "5", 1 );

			if (roll > 5) pos_roll = 167;
			else if (roll < -5) pos_roll = 89;
			else pos_roll = 128 + Round( roll * 7.8 );

			if (pitch > 5) pos_pitch = 167;
			else if (pitch < -5) pos_pitch = 89;
			else pos_pitch = 128 + Round( pitch * 7.8 );

			if (yaw > 5) pos_yaw = 167;
			else if (yaw < -5) pos_yaw = 89;
			else pos_yaw = 128 + Round( yaw * 7.8 );
		}
		else if (adirate == 2)
		{
			// ADI RATE HIGH
			skp->Text( 79, 48, "10", 2 );
			skp->Text( 168, 48, "10", 2 );

			skp->Text( 201, 79, "10", 2 );
			skp->Text( 201, 167, "10", 2 );

			skp->Text( 79, 201, "10", 2 );
			skp->Text( 168, 201, "10", 2 );

			if (roll > 10) pos_roll = 167;
			else if (roll < -10) pos_roll = 89;
			else pos_roll = 128 + Round( roll * 3.9 );

			if (pitch > 10) pos_pitch = 167;
			else if (pitch < -10) pos_pitch = 89;
			else pos_pitch = 128 + Round( pitch * 3.9 );

			if (yaw > 10) pos_yaw = 167;
			else if (yaw < -10) pos_yaw = 89;
			else pos_yaw = 128 + Round( yaw * 3.9 );
		}
		else
		{
			// ADI RATE LOW
			skp->Text( 82, 48, "1", 1 );
			skp->Text( 170, 48, "1", 1 );

			skp->Text( 201, 79, "1", 1 );
			skp->Text( 201, 167, "1", 1 );

			skp->Text( 82, 201, "1", 1 );
			skp->Text( 170, 201, "1", 1 );

			if (roll > 1) pos_roll = 167;
			else if (roll < -1) pos_roll = 89;
			else pos_roll = 128 + Round( roll * 39 );

			if (pitch > 1) pos_pitch = 167;
			else if (pitch < -1) pos_pitch = 89;
			else pos_pitch = 128 + Round( pitch * 39 );

			if (yaw > 1) pos_yaw = 167;
			else if (yaw < -1) pos_yaw = 89;
			else pos_yaw = 128 + Round( yaw * 39 );
		}

		// draw triangles
		skp->SetPen( skpLightGreenPen );
		skp->SetBrush( skpLightGreenBrush );
		oapi::IVECTOR2 tri[3];
		tri[0].x = pos_roll;
		tri[0].y = 53;
		tri[1].x = tri[0].x + 5;
		tri[1].y = 44;
		tri[2].x = tri[0].x - 5;
		tri[2].y = 44;
		skp->Polygon( tri, 3 );

		tri[0].x = 204;
		tri[0].y = pos_pitch;
		tri[1].x = 213;
		tri[1].y = tri[0].y - 5;
		tri[2].x = 213;
		tri[2].y = tri[0].y + 5;
		skp->Polygon( tri, 3 );

		tri[0].x = pos_yaw;
		tri[0].y = 205;
		tri[1].x = tri[0].x - 5;
		tri[1].y = 214;
		tri[2].x = tri[0].x + 5;
		tri[2].y = 214;
		skp->Polygon( tri, 3 );
		return;
	}

	void MDU::ADI_ERROR_ORBIT( HDC hDC, double pitch, double roll, double yaw, int adierr )
	{
		double pos_pitch;
		double pos_roll;
		double pos_yaw;

		SetTextColor( hDC, CR_MAGENTA );
		if (adierr == 1)
		{
			// ADI ERROR MED
			// 5/5/5
			TextOut( hDC, 185, 94, "5", 1 );
			TextOut( hDC, 185, 155, "5", 1 );

			if (roll > 5) pos_roll = 28;
			else if (roll < -5) pos_roll = -28;
			else pos_roll = roll * 5.6;

			if (pitch > 5) pos_pitch = 28;
			else if (pitch < -5) pos_pitch = -28;
			else pos_pitch = pitch * 5.6;

			if (yaw > 5) pos_yaw = 28;
			else if (yaw < -5) pos_yaw = -28;
			else pos_yaw = yaw * 5.6;
		}
		else if (adierr == 2)
		{
			// ADI ERROR HIGH
			// 10/10/10
			TextOut( hDC, 185, 94, "10", 2 );
			TextOut( hDC, 185, 155, "10", 2 );

			if (roll > 10) pos_roll = 28;
			else if (roll < -10) pos_roll = -28;
			else pos_roll = roll * 2.8;

			if (pitch > 10) pos_pitch = 28;
			else if (pitch < -10) pos_pitch = -28;
			else pos_pitch = pitch * 2.8;

			if (yaw > 10) pos_yaw = 28;
			else if (yaw < -10) pos_yaw = -28;
			else pos_yaw = yaw * 2.8;
		}
		else
		{
			// ADI ERROR LOW
			// 1/1/1
			TextOut( hDC, 185, 94, "1", 1 );
			TextOut( hDC, 185, 155, "1", 1 );

			if (roll > 1) pos_roll = 28;
			else if (roll < -1) pos_roll = -28;
			else pos_roll = roll * 28;

			if (pitch > 1) pos_pitch = 28;
			else if (pitch < -1) pos_pitch = -28;
			else pos_pitch = pitch * 28;

			if (yaw > 1) pos_yaw = 28;
			else if (yaw < -1) pos_yaw = -28;
			else pos_yaw = yaw * 28;
		}

		// draw needles
		SelectObject( hDC, gdiMagentaPen );
		SelectObject( hDC, gdiMagentaBrush );
		Rectangle( hDC, 127 + Round( pos_roll ), 128 - Round( sqrt( 3969 - (pos_roll * pos_roll) ) ), 129 + Round( pos_roll ), 109 );// roll
		Rectangle( hDC, 147, 127 + Round( pos_pitch ), 128 + Round( sqrt( 3969 - (pos_pitch * pos_pitch) ) ), 129 + Round( pos_pitch ) );// pitch
		Rectangle( hDC, 127 + Round( pos_yaw ), 147, 129 + Round( pos_yaw ), 128 + Round( sqrt( 3969 - (pos_yaw * pos_yaw) ) ) );// yaw
		return;
	}

	void MDU::ADI_ERROR_ORBIT( oapi::Sketchpad2* skp, double pitch, double roll, double yaw, int adierr )
	{
		double pos_pitch;
		double pos_roll;
		double pos_yaw;

		skp->SetTextColor( CR_MAGENTA );
		if (adierr == 1)
		{
			// ADI ERROR MED
			// 5/5/5
			skp->Text( 185, 94, "5", 1 );
			skp->Text( 185, 155, "5", 1 );

			if (roll > 5) pos_roll = 28;
			else if (roll < -5) pos_roll = -28;
			else pos_roll = roll * 5.6;

			if (pitch > 5) pos_pitch = 28;
			else if (pitch < -5) pos_pitch = -28;
			else pos_pitch = pitch * 5.6;

			if (yaw > 5) pos_yaw = 28;
			else if (yaw < -5) pos_yaw = -28;
			else pos_yaw = yaw * 5.6;
		}
		else if (adierr == 2)
		{
			// ADI ERROR HIGH
			// 10/10/10
			skp->Text( 185, 94, "10", 2 );
			skp->Text( 185, 155, "10", 2 );

			if (roll > 10) pos_roll = 28;
			else if (roll < -10) pos_roll = -28;
			else pos_roll = roll * 2.8;

			if (pitch > 10) pos_pitch = 28;
			else if (pitch < -10) pos_pitch = -28;
			else pos_pitch = pitch * 2.8;

			if (yaw > 10) pos_yaw = 28;
			else if (yaw < -10) pos_yaw = -28;
			else pos_yaw = yaw * 2.8;
		}
		else
		{
			// ADI ERROR LOW
			// 1/1/1
			skp->Text( 185, 94, "1", 1 );
			skp->Text( 185, 155, "1", 1 );

			if (roll > 1) pos_roll = 28;
			else if (roll < -1) pos_roll = -28;
			else pos_roll = roll * 28;

			if (pitch > 1) pos_pitch = 28;
			else if (pitch < -1) pos_pitch = -28;
			else pos_pitch = pitch * 28;

			if (yaw > 1) pos_yaw = 28;
			else if (yaw < -1) pos_yaw = -28;
			else pos_yaw = yaw * 28;
		}

		// draw needles
		skp->SetPen( skpMagentaPen );
		skp->SetBrush( skpMagentaBrush );
		skp->Rectangle( 127 + Round( pos_roll ), 128 - Round( sqrt( 3969 - (pos_roll * pos_roll) ) ), 129 + Round( pos_roll ), 109 );// roll
		skp->Rectangle( 147, 127 + Round( pos_pitch ), 128 + Round( sqrt( 3969 - (pos_pitch * pos_pitch) ) ), 129 + Round( pos_pitch ) );// pitch
		skp->Rectangle( 127 + Round( pos_yaw ), 147, 129 + Round( pos_yaw ), 128 + Round( sqrt( 3969 - (pos_yaw * pos_yaw) ) ) );// yaw
		return;
	}
};
