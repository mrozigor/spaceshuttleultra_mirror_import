#define STRICT
#define ORBITER_MODULE
#include <math.h>
#include <UltraMath.h>
#include "Atlantis.h"
#include "orbitersdk.h"
#include "CRT.h"

#define RED RGB(255, 0, 0)
#define GREEN RGB(0, 255, 0)
#define YELLOW RGB(255, 255, 0)
#define DARK_YELLOW RGB(128, 128, 0)
#define TURQUOISE RGB(0, 183, 146)
#define DARK_GREEN RGB(0, 128, 0)
#define WHITE RGB(255, 255, 255)
#define BLUE RGB(0, 0, 255)
#define GRAY RGB(160, 160, 160)
#define BRIGHTERGRAY RGB(200, 200, 200)
#define BLACK RGB(0, 0, 0)

// ==============================================================
// Global variables

int g_MFDmode; // identifier for new MFD mode

CRT::SavePrm CRT::saveprm;

MFD_GDIPARAM mfd_gparam;

// ==============================================================
// API interface

DLLCLBK void InitModule (HINSTANCE hDLL)
{
	static char *name = "CRT";   // MFD mode name
	MFDMODESPEC spec;
	spec.name = name;
	spec.key = OAPI_KEY_T;                // MFD mode selection key
	spec.msgproc = CRT::MsgProc;  // MFD mode callback function

	// Register the new MFD mode with Orbiter
	g_MFDmode = oapiRegisterMFDMode (spec);

	mfd_gparam.hDLL = hDLL;
	mfd_gparam.hCRTFont = CreateFont(10,10, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, OEM_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, FIXED_PITCH, "Arial");
	
}

DLLCLBK void ExitModule (HINSTANCE hDLL)
{
	// Unregister the custom MFD mode when the module is unloaded
	oapiUnregisterMFDMode (g_MFDmode);
	DeleteObject(mfd_gparam.hCRTFont);	
}

// ==============================================================
// MFD class implementation

CRT::CRT (DWORD w, DWORD h, VESSEL *v)
: MFD (w, h, v)
{
	char cbuf[200];

	GreenBrush = CreateSolidBrush( GREEN );
	TurquoiseBrush = CreateSolidBrush( TURQUOISE );
	WhiteBrush = CreateSolidBrush( WHITE );
	BlackBrush = CreateSolidBrush( BLACK );
	RedBrush = CreateSolidBrush( RED );
	PurpleBrush = CreateSolidBrush( RGB( 100, 100, 100 ) );
	YellowBrush = CreateSolidBrush( RGB( 255, 255, 0 ) );
	LightBlueBrush = CreateSolidBrush( RGB( 0, 255, 255 ) );
	WhitePen = CreatePen( PS_SOLID, 0, WHITE );
	GreenPen = CreatePen( PS_SOLID, 0, GREEN );
	TurquoisePen = CreatePen( PS_SOLID, 0, TURQUOISE );
	RedPen = CreatePen( PS_SOLID, 0, RED );
	BlackPen = CreatePen( PS_SOLID, 0, BLACK );
	PurplePen = CreatePen( PS_SOLID, 2, RGB( 150, 50, 150 ) );
	PurpleThinPen = CreatePen( PS_SOLID, 0, RGB(150, 50, 150 ) );
	DarkPurplePen = CreatePen( PS_SOLID, 0, RGB( 100, 100, 100 ) );
	YellowPen = CreatePen( PS_SOLID, 1, RGB( 255, 255, 0 ) );
	YellowThickPen = CreatePen( PS_SOLID, 2, RGB( 255, 255, 0 ) );
	BoldWhitePen = CreatePen( PS_SOLID, 2, WHITE );
	LightBluePen = CreatePen( PS_SOLID, 0, RGB( 0, 255, 255 ) );
	ArialFont_h15w5 = CreateFont( 15, 5, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, OEM_CHARSET, OUT_DEFAULT_PRECIS, 
			CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, FIXED_PITCH, "Arial" );
	ArialFont_h13w6 = CreateFont( 13, 6, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, OEM_CHARSET, OUT_DEFAULT_PRECIS, 
			CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FIXED_PITCH, "Arial" );

	vessel=v;
	width=w;
	height=h;
	
	MDUID=-1;

	sprintf_s(cbuf, 200, "[CRT]:DIMENSIONS: %d %d\n", W, H);
	oapiWriteLog(cbuf);

	hCRTFont = CreateFont(8,4, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, OEM_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, FIXED_PITCH, "Arial");


	if(!strcmp(pV->GetClassName(), "Atlantis") || 
		!stricmp(pV->GetClassName(), STD_CLASS_NAME)) {

		sts=static_cast<Atlantis*>(pV);

		sts->newmfd=this;
		mode=0;
	}
	else {
		mode=10001;
	}

	display=1;

	UpdateStatus=true;
	return;
}

CRT::~CRT ()
{
	DeleteObject(hCRTFont);

	DeleteObject( GreenBrush );
	DeleteObject( TurquoiseBrush );
	DeleteObject( WhiteBrush );
	DeleteObject( BlackBrush );
	DeleteObject( RedBrush );
	DeleteObject( PurpleBrush );
	DeleteObject( YellowBrush );
	DeleteObject( LightBlueBrush );
	DeleteObject( WhitePen );
	DeleteObject( GreenPen );
	DeleteObject( TurquoisePen );
	DeleteObject( RedPen );
	DeleteObject( BlackPen );
	DeleteObject( PurplePen );
	DeleteObject( PurpleThinPen );
	DeleteObject( DarkPurplePen );
	DeleteObject( YellowPen );
	DeleteObject( YellowThickPen );
	DeleteObject( BoldWhitePen );
	DeleteObject( LightBluePen );
	DeleteObject( ArialFont_h15w5 );
	DeleteObject( ArialFont_h13w6 );
	return;
}

void CRT::Update (HDC hDC)
{
	if(UpdateStatus) {
		RecallStatus();
		InvalidateButtons();
		UpdateStatus=false;
	}

	if(mode==10001) {
			int n;
			char cbuf[255];
			sprintf(cbuf, vessel->GetClassName());
			TextOut(hDC, 0, 5, cbuf, strlen(cbuf));
			n=strlen(cbuf);
			sprintf(cbuf, "%i", n);
			TextOut(hDC, 0, 15, cbuf, strlen(cbuf));
			TextOut(hDC, 0, 25, "ERROR: Vessel not from Atlantis class", 37);
			return;
	}

	switch (mode)
	{
		case 0:// "CRT display"
			if (MDUID >= 0)
			{
				vc::MDU* pMDU=sts->GetMDU(MDUID);
				pMDU->Paint(hDC);
			}
			else
			{
				oapiWriteLog("CRT MFD not connected to MDU (m0)");
			}
			break;
		case 1:// FLT INST
			if (display == 1)
			{
				vc::MDU* mdu = sts->GetMDU( MDUID );
				if (mdu) mdu->AEPFD( hDC );// A/E PFD
				else oapiWriteLog( "CRT MFD not connected to MDU (m1d1)" );
			}
			else if (display == 2)
			{
				vc::MDU* mdu = sts->GetMDU( MDUID );
				if (mdu) mdu->ORBITPFD( hDC );// ORBIT PFD
				else oapiWriteLog( "CRT MFD not connected to MDU (m1d2)" );
			}
			break;
		case 2:// SUBSYS STATUS
			if (display == 1) OMSMPS( hDC );
			else if (display == 2) APUHYD( hDC );
			else if (display == 3) SPI( hDC );
			break;
	}
}

void CRT::OMSMPS(HDC hDC)
{
	int nPos, nLoc, EngConvert[3]={1, 0, 2};
	double dNum;
	char cbuf[255];

	int save = SaveDC(hDC);
	SetTextColor(hDC, TURQUOISE);
	SelectObject(hDC, ArialFont_h13w6);
	SelectObject(hDC, TurquoisePen);
	SelectObject(hDC, BlackBrush);

	TextOut(hDC, 33.5, 0, "OMS", 3);
	TextOut(hDC, 150, 0, "MPS", 3);
	MoveToEx(hDC, 80, 6, NULL);
	LineTo(hDC, 80, 250);
	MoveToEx(hDC, 1, 6, NULL);
	LineTo(hDC, 20, 6); 
	MoveToEx(hDC, 67, 6, NULL);
	LineTo(hDC, 145, 6);
	MoveToEx(hDC, 180, 6, NULL);
	LineTo(hDC, 255, 6);
	MoveToEx(hDC, 0, 6, NULL);
	LineTo(hDC, 0, 30);
	MoveToEx(hDC, 255, 6, NULL);
	LineTo(hDC, 255, 30);

	SetTextColor(hDC, WHITE);

	//OMS
	TextOut(hDC, 23, 15, "L", 1);
	TextOut(hDC, 58, 15, "R", 1);
	Rectangle (hDC, 12, 30, 42, 46);
	Rectangle (hDC, 47, 30, 77, 46);
	Rectangle (hDC, 12, 95, 42, 111);
	Rectangle (hDC, 47, 95, 77, 111);
	Rectangle (hDC, 14, 167, 40, 183);
	Rectangle (hDC, 49, 167, 75, 183);
	//MPS
	TextOut(hDC, 159, 10, "L/2", 3);
	TextOut(hDC, 194, 6, "C/1", 3);
	TextOut(hDC, 229, 10, "R/3", 3);
	//Tank P
	Rectangle (hDC, 100, 24, 129, 40);
	Rectangle (hDC, 153, 24, 182, 40);
	Rectangle (hDC, 188, 19, 217, 35);
	Rectangle (hDC, 223, 24, 252, 40);
	//Reg Press
	Rectangle (hDC, 100, 92, 129, 108);
	Rectangle (hDC, 153, 92, 182, 108);
	Rectangle (hDC, 188, 87, 217, 103);
	Rectangle (hDC, 223, 92, 252, 108);
	//Engine Pc
	TextOut(hDC, 159, 152, "L/2", 3);
	TextOut(hDC, 194, 147, "C/1", 3);
	TextOut(hDC, 229, 152, "R/3", 3);
	Rectangle (hDC, 155, 167, 181, 183);
	Rectangle (hDC, 190, 162, 216, 178);
	Rectangle (hDC, 225, 167, 251, 183);
	// ENG MANF
	Rectangle( hDC, 83, 179, 109, 194 );
	Rectangle( hDC, 116, 179, 142, 194 );

	SelectObject(hDC, WhitePen);

	//OMS
	TextOut(hDC, 0, 50, "He", 2);
	TextOut(hDC, 0, 60, "TK", 2);
	TextOut(hDC, 3, 70, "P", 1);
	Rectangle (hDC, 19, 50, 34, 90);
	Rectangle (hDC, 54, 50, 69, 90);
	MoveToEx(hDC, 34, 62, NULL);
	LineTo(hDC, 40, 62);
	MoveToEx(hDC, 69, 62, NULL);
	LineTo(hDC, 75, 62);

	TextOut(hDC, 0, 115, "N2", 2);
	TextOut(hDC, 0, 125, "TK", 2);
	TextOut(hDC, 3, 135, "P", 1);
	Rectangle (hDC, 19, 115, 34, 155);
	Rectangle (hDC, 54, 115, 69, 155);
	MoveToEx(hDC, 34, 139, NULL);
	LineTo(hDC, 40, 139);
	MoveToEx(hDC, 69, 139, NULL);
	LineTo(hDC, 75, 139);

	TextOut(hDC, 23, 155, "L", 1);
	TextOut(hDC, 58, 155, "R", 1);
	TextOut(hDC, 0, 202, "Pc", 2);
	TextOut(hDC, 3, 217, "%", 1);
	Rectangle (hDC, 19, 187, 34, 250);
	Rectangle (hDC, 54, 187, 69, 250);
	MoveToEx(hDC, 34, 208, NULL);
	LineTo(hDC, 40, 208);
	MoveToEx(hDC, 69, 208, NULL);
	LineTo(hDC, 75, 208);

	//MPS
	TextOut(hDC, 100, 10, "PNEU", 4);
	TextOut(hDC, 80, 42, "TANK", 4);
	TextOut(hDC, 88, 52, "P", 1);
	Rectangle (hDC, 108, 42, 123, 82);
	TextOut(hDC, 140, 42, "He", 2);
	TextOut(hDC, 132, 52, "TANK", 4);
	TextOut(hDC, 139, 62, "P", 1);
	Rectangle (hDC, 161, 42, 176, 82);
	Rectangle (hDC, 196, 42, 211, 82);
	Rectangle (hDC, 230, 42, 245, 82);
	MoveToEx( hDC, 123, 66, NULL );
	LineTo( hDC, 129, 66 );
	MoveToEx( hDC, 176, 80, NULL );
	LineTo( hDC, 182, 80 );
	MoveToEx( hDC, 211, 80, NULL );
	LineTo( hDC, 217, 80 );
	MoveToEx( hDC, 245, 80, NULL );
	LineTo( hDC, 251, 80 );

	TextOut(hDC, 83, 110, "REG", 3);
	TextOut(hDC, 90, 120, "P", 1);
	Rectangle (hDC, 108, 110, 123, 145);
	TextOut(hDC, 135, 110, "He", 2);
	TextOut(hDC, 125, 120, "REG A", 5);
	TextOut(hDC, 138, 130, "P", 1);
	Rectangle (hDC, 161, 110, 176, 145);
	Rectangle (hDC, 196, 110, 211, 145);
	Rectangle (hDC, 230, 110, 245, 145);
	MoveToEx( hDC, 123, 120, NULL );
	LineTo( hDC, 129, 120 );
	MoveToEx( hDC, 123, 136, NULL );
	LineTo( hDC, 129, 136 );
	MoveToEx( hDC, 176, 120, NULL );
	LineTo( hDC, 182, 120 );
	MoveToEx( hDC, 176, 136, NULL );
	LineTo( hDC, 182, 136 );
	MoveToEx( hDC, 211, 120, NULL );
	LineTo( hDC, 217, 120 );
	MoveToEx( hDC, 211, 136, NULL );
	LineTo( hDC, 217, 136 );
	MoveToEx( hDC, 245, 120, NULL );
	LineTo( hDC, 251, 120 );
	MoveToEx( hDC, 245, 136, NULL );
	LineTo( hDC, 251, 136 );

	TextOut(hDC, 178, 203, "Pc", 2);
	TextOut(hDC, 182, 213, "%", 1);
	TextOut(hDC, 212, 203, "Pc", 2);
	TextOut(hDC, 217, 213, "%", 1);
	Rectangle (hDC, 161, 187, 176, 250);
	Rectangle (hDC, 196, 187, 211, 250);
	Rectangle (hDC, 231, 187, 246, 250);
	MoveToEx(hDC, 176, 193, NULL);
	LineTo(hDC, 182, 193);
	MoveToEx(hDC, 211, 193, NULL);
	LineTo(hDC, 217, 193);
	MoveToEx(hDC, 245, 193, NULL);
	LineTo(hDC, 251, 193);
	MoveToEx(hDC, 176, 228, NULL);
	LineTo(hDC, 182, 228);
	MoveToEx(hDC, 211, 228, NULL);
	LineTo(hDC, 217, 228);
	MoveToEx(hDC, 245, 228, NULL);
	LineTo(hDC, 251, 228);

	TextOut( hDC, 85, 152, "ENG MANF", 8 );
	TextOut( hDC, 85, 164, "LO2", 3 );
	TextOut( hDC, 118, 164, "LH2", 3 );
	TextOut( hDC, 109, 191, "P", 1 );
	TextOut( hDC, 109, 201, "S", 1 );
	TextOut( hDC, 109, 211, "I", 1 );
	TextOut( hDC, 109, 221, "A", 1 );
	Rectangle( hDC, 88, 200, 103, 239 );
	Rectangle( hDC, 121, 200, 136, 239 );
	MoveToEx( hDC, 102, 206, NULL );
	LineTo( hDC, 108, 206 );
	MoveToEx( hDC, 136, 213, NULL );
	LineTo( hDC, 142, 213 );


	//SelectObject(hDC, GreenBrush);
	
	//OMS
	for(nPos=0;nPos<2;nPos++) {
		dNum=sts->GetPropellantMass(sts->oms_helium_tank[nPos])*228.6;
		sprintf(cbuf, "%04.0f", dNum);
		TextOut(hDC, 13+35*nPos, 31, cbuf, strlen(cbuf));
		if (dNum >= 1500)
		{
			SelectObject(hDC, GreenBrush);
			SelectObject(hDC, GreenPen);
			if (dNum > 5000) dNum = 5000;
		}
		else
		{
			SelectObject(hDC, RedBrush);
			SelectObject(hDC, RedPen);
			if (dNum < 0) dNum = 0;
		}
		Rectangle (hDC, 20+35*nPos, Round( (89-0.0076*dNum) ), 33+35*nPos, 89);

		dNum = 0;// TODO get val
		sprintf(cbuf, "%04.0f", dNum);
		TextOut(hDC, 13+35*nPos, 96, cbuf, strlen(cbuf));
		if (dNum >= 1200)
		{
			SelectObject(hDC, GreenBrush);
			SelectObject(hDC, GreenPen);
			if (dNum > 3000) dNum = 3000;
		}
		else
		{
			SelectObject(hDC, RedBrush);
			SelectObject(hDC, RedPen);
			if (dNum < 0) dNum = 0;
		}
		Rectangle (hDC, 20+35*nPos, Round( (154-0.012667*dNum) ), 33+35*nPos, 154);
		
		dNum=100.0*sts->GetThrusterLevel(sts->th_oms[nPos]) + (sts->GetAtmPressure() / 8618.44625);// HACK should have this in the sensor
		sprintf(cbuf, "%03.0f", dNum);
		TextOut(hDC, 16+35*nPos, 168, cbuf, strlen(cbuf));
		if (dNum >= 80)
		{
			SelectObject(hDC, WhiteBrush);
			SelectObject(hDC, WhitePen);
			if (dNum > 120) dNum = 120;
		}
		else if (dNum >= 4)
		{
			SelectObject(hDC, RedBrush);
			SelectObject(hDC, RedPen);
		}
		else
		{
			SelectObject(hDC, BlackBrush);
			SelectObject(hDC, BlackPen);
			if (dNum < 0) dNum = 0;
		}
		Rectangle(hDC, 20+35*nPos, Round( 249 - (0.508333 * dNum) ), 33+35*nPos, 249);
	}

	//MPS
	for(nPos=0;nPos<3;nPos++) {
		nLoc=35*EngConvert[nPos];
		if (sts->status <= 2) dNum = sts->GetSSMEPress( nPos + 1 );
		else dNum = 0.0;
		sprintf(cbuf, "%03.0f", dNum);
		if(EngConvert[nPos]!=1) TextOut(hDC, 157+nLoc, 168, cbuf, strlen(cbuf));
		else TextOut(hDC, 157+nLoc, 163, cbuf, strlen(cbuf));
		if (dNum >= 65)
		{
			SelectObject(hDC, WhiteBrush);
			SelectObject(hDC, WhitePen);
			if (dNum > 109) dNum = 109;
		}
		else
		{
			SelectObject(hDC, RedBrush);
			SelectObject(hDC, RedPen);
			if (dNum < 45) dNum = 45;
		}
		Rectangle( hDC, 162 + nLoc, Round( 249 - 0.953125 * (dNum - 45) ), 175 + nLoc, 249 );
	}
	
	// He Tank Press Pneu
	dNum = sts->GetHeTankPress( 0 );
	sprintf( cbuf, "%04.0f", dNum );
	TextOut( hDC, 100, 25, cbuf, strlen( cbuf ) );
	if (dNum >= 3800)
	{
		SelectObject(hDC, GreenBrush);
		SelectObject(hDC, GreenPen);
		if (dNum > 5000) dNum = 5000;
	}
	else
	{
		SelectObject(hDC, RedBrush);
		SelectObject(hDC, RedPen);
		if (dNum < 3000) dNum = 3000;
	}
	Rectangle( hDC, 109, Round( 81 - 0.019 * (dNum - 3000) ), 122, 81 );

	// He Tank Press Eng 2
	dNum = sts->GetHeTankPress( 2 );
	sprintf( cbuf, "%04.0f", dNum );
	TextOut( hDC, 153, 25, cbuf, strlen( cbuf ) );
	if (dNum >= 1150)
	{
		SelectObject(hDC, GreenBrush);
		SelectObject(hDC, GreenPen);
		if (dNum > 5000) dNum = 5000;
	}
	else
	{
		SelectObject(hDC, RedBrush);
		SelectObject(hDC, RedPen);
		if (dNum < 1000) dNum = 1000;
	}
	Rectangle( hDC, 162, Round( 81 - 0.0095 * (dNum - 1000) ), 175, 81 );

	// He Tank Press Eng 1
	dNum = sts->GetHeTankPress( 1 );
	sprintf( cbuf, "%04.0f", dNum );
	TextOut( hDC, 188, 20, cbuf, strlen( cbuf ) );
	if (dNum >= 1150)
	{
		SelectObject(hDC, GreenBrush);
		SelectObject(hDC, GreenPen);
		if (dNum > 5000) dNum = 5000;
	}
	else
	{
		SelectObject(hDC, RedBrush);
		SelectObject(hDC, RedPen);
		if (dNum < 1000) dNum = 1000;
	}
	Rectangle( hDC, 197, Round( 81 - 0.0095 * (dNum - 1000) ), 210, 81 );

	// He Tank Press Eng 3
	dNum = sts->GetHeTankPress( 3 );
	sprintf( cbuf, "%04.0f", dNum );
	TextOut( hDC, 223, 25, cbuf, strlen( cbuf ) );
	if (dNum >= 1150)
	{
		SelectObject(hDC, GreenBrush);
		SelectObject(hDC, GreenPen);
		if (dNum > 5000) dNum = 5000;
	}
	else
	{
		SelectObject(hDC, RedBrush);
		SelectObject(hDC, RedPen);
		if (dNum < 1000) dNum = 1000;
	}
	Rectangle( hDC, 231, Round( 81 - 0.0095 * (dNum - 1000) ), 244, 81 );

	// He Reg Press Pneu
	dNum = sts->GetHeRegPress( 0 );
	sprintf( cbuf, "%04.0f", dNum );
	TextOut( hDC, 100, 93, cbuf, strlen( cbuf ) );
	if ((dNum >= 680) && (dNum <= 810))
	{
		SelectObject(hDC, GreenBrush);
		SelectObject(hDC, GreenPen);
	}
	else
	{
		SelectObject(hDC, RedBrush);
		SelectObject(hDC, RedPen);
		if (dNum < 600) dNum = 600;
		if (dNum > 900) dNum = 900;
	}
	Rectangle( hDC, 109, Round( 144 - 0.11 * (dNum - 600) ), 122, 144 );

	// He Reg Press Eng 2
	dNum = sts->GetHeRegPress( 2 );
	sprintf( cbuf, "%04.0f", dNum );
	TextOut( hDC, 153, 93, cbuf, strlen( cbuf ) );
	if ((dNum >= 680) && (dNum <= 810))
	{
		SelectObject(hDC, GreenBrush);
		SelectObject(hDC, GreenPen);
	}
	else
	{
		SelectObject(hDC, RedBrush);
		SelectObject(hDC, RedPen);
		if (dNum < 600) dNum = 600;
		if (dNum > 900) dNum = 900;
	}
	Rectangle( hDC, 162, Round( 144 - 0.11 * (dNum - 600) ), 175, 144 );

	// He Reg Press Eng 1
	dNum = sts->GetHeRegPress( 1 );
	sprintf( cbuf, "%04.0f", dNum );
	TextOut( hDC, 188, 88, cbuf, strlen( cbuf ) );
	if ((dNum >= 680) && (dNum <= 810))
	{
		SelectObject(hDC, GreenBrush);
		SelectObject(hDC, GreenPen);
	}
	else
	{
		SelectObject(hDC, RedBrush);
		SelectObject(hDC, RedPen);
		if (dNum < 600) dNum = 600;
		if (dNum > 900) dNum = 900;
	}
	Rectangle( hDC, 197, Round( 144 - 0.11 * (dNum - 600) ), 210, 144 );

	// He Reg Press Eng 3
	dNum = sts->GetHeRegPress( 3 );
	sprintf( cbuf, "%04.0f", dNum );
	TextOut( hDC, 223, 93, cbuf, strlen( cbuf ) );
	if ((dNum >= 680) && (dNum <= 810))
	{
		SelectObject(hDC, GreenBrush);
		SelectObject(hDC, GreenPen);
	}
	else
	{
		SelectObject(hDC, RedBrush);
		SelectObject(hDC, RedPen);
		if (dNum < 600) dNum = 600;
		if (dNum > 900) dNum = 900;
	}
	Rectangle( hDC, 231, Round( 144 - 0.11 * (dNum - 600) ), 244, 144 );

	// ENG MANF LO2
	dNum = sts->GetLOXManifPress();
	sprintf( cbuf, "%03.0f", dNum );
	TextOut( hDC, 85, 180, cbuf, strlen( cbuf ) );
	if (dNum >= 250)
	{
		SelectObject(hDC, RedBrush);
		SelectObject(hDC, RedPen);
		if (dNum > 300) dNum = 300;
	}
	else
	{
		SelectObject(hDC, GreenBrush);
		SelectObject(hDC, GreenPen);
		if (dNum < 0) dNum = 0;
	}
	Rectangle( hDC, 89, Round( 238 - (0.123333 * dNum) ), 102, 238 );

	// ENG MANF LH2
	dNum = sts->GetLH2ManifPress();
	sprintf( cbuf, "%03.0f", dNum );
	TextOut( hDC, 118, 180, cbuf, strlen( cbuf ) );
	if (dNum >= 66)
	{
		SelectObject(hDC, RedBrush);
		SelectObject(hDC, RedPen);
		if (dNum > 100) dNum = 100;
	}
	else
	{
		SelectObject(hDC, GreenBrush);
		SelectObject(hDC, GreenPen);
		if (dNum < 0) dNum = 0;
	}
	Rectangle( hDC, 122, Round( 238 - (0.37 * dNum) ), 135, 238 );

	RestoreDC(hDC, save);
}

void CRT::SPI(HDC hDC)
{
	HPEN GreenPen=CreatePen(PS_SOLID, 0, RGB(50,255,50));

	POINT tri[3];

	int savedDC = SaveDC(hDC);
	
	int nPos;
	double dNum;
	char cbuf[255];
	//Elevons
	SelectObject(hDC,ArialFont_h15w5);
	SelectObject(hDC, PurplePen);
	SetTextColor(hDC, WHITE);
	TextOut(hDC, 0, 5, "ELEVONS", 7);
	TextOut(hDC, 12, 15, "DEG", 3);
	MoveToEx(hDC,10,55,NULL);
	LineTo(hDC,10,221);
	MoveToEx(hDC,21,55,NULL);
	LineTo(hDC,21,221);
	MoveToEx(hDC,55,55,NULL);
	LineTo(hDC,55,221);
	MoveToEx(hDC,66,55,NULL);
	LineTo(hDC,66,221);
	for(nPos=70;nPos<=130;nPos+=30) {
		MoveToEx(hDC, 4, nPos, NULL);
		LineTo(hDC, 10, nPos);
		MoveToEx(hDC, 26, nPos, NULL);
		LineTo(hDC, 20, nPos);
		MoveToEx(hDC, 49, nPos, NULL);
		LineTo(hDC, 55, nPos);
		MoveToEx(hDC, 71, nPos, NULL);
		LineTo(hDC, 65, nPos);
		sprintf(cbuf,"%+d", 10*((nPos-55)/30)-30);
		TextOut(hDC, 27, nPos-7, cbuf, strlen(cbuf));
	}
	for(nPos=190;nPos<=220;nPos+=30) {
		MoveToEx(hDC, 4, nPos, NULL);
		LineTo(hDC, 10, nPos);
		MoveToEx(hDC, 26, nPos, NULL);
		LineTo(hDC, 20, nPos);
		MoveToEx(hDC, 49, nPos, NULL);
		LineTo(hDC, 55, nPos);
		MoveToEx(hDC, 71, nPos, NULL);
		LineTo(hDC, 65, nPos);
		sprintf(cbuf,"%+d", 10*((nPos-55)/30)-30);
		TextOut(hDC, 27, nPos-7, cbuf, strlen(cbuf));
	}
	for(nPos=55;nPos<=220;nPos+=30) {
		MoveToEx(hDC, 7, nPos, NULL);
		LineTo(hDC, 10, nPos);
		MoveToEx(hDC, 23, nPos, NULL);
		LineTo(hDC, 20, nPos);
		MoveToEx(hDC, 52, nPos, NULL);
		LineTo(hDC, 55, nPos);
		MoveToEx(hDC, 68, nPos, NULL);
		LineTo(hDC, 65, nPos);
	}
	// yellow 0 and line
	SetTextColor(hDC,RGB(255,255,0));
	SelectObject(hDC,YellowThickPen);
	TextOut(hDC, 34, 153, "0", 1);
	MoveToEx(hDC, 4, 160, NULL);
	LineTo(hDC, 10, 160);
	MoveToEx(hDC, 26, 160, NULL);
	LineTo(hDC, 20, 160);
	MoveToEx(hDC, 49, 160, NULL);
	LineTo(hDC, 55, 160);
	MoveToEx(hDC, 71, 160, NULL);
	LineTo(hDC, 65, 160);

	//DRAW INSIDE OF BOXES WITH SLIGHTLY DARKER PURPLE
	SelectObject(hDC, PurpleBrush);
	SelectObject(hDC, DarkPurplePen);
	Rectangle (hDC, 12, 50, 19, 226);
	Rectangle (hDC, 57, 50, 64, 226);
	SelectObject(hDC,GreenPen);
	SelectObject(hDC,BlackBrush);

	SelectObject(hDC,YellowPen);
	SelectObject(hDC,YellowBrush);

	if(sts->HydraulicsOK()) dNum=sts->aerosurfaces.leftElevon;
	else dNum=0.0;
	nPos=static_cast<int>(145.0+(3.0*dNum))+15;
	tri[0].x = 6;
	tri[0].y = nPos;
	tri[1].x = 14;
	tri[1].y = nPos - 4;
	tri[2].x = 14;
	tri[2].y = nPos + 4;
	Polygon( hDC, tri, 3 );
	tri[0].x = 24;
	tri[0].y = nPos;
	tri[1].x = 16;
	tri[1].y = nPos - 4;
	tri[2].x = 16;
	tri[2].y = nPos + 4;
	Polygon( hDC, tri, 3 );
	if(sts->HydraulicsOK()) dNum=sts->aerosurfaces.rightElevon;
	else dNum=0.0;
	nPos=static_cast<int>(145.0+(3.0*dNum))+15;
	tri[0].x = 51;
	tri[0].y = nPos;
	tri[1].x = 59;
	tri[1].y = nPos - 4;
	tri[2].x = 59;
	tri[2].y = nPos + 4;
	Polygon( hDC, tri, 3 );
	tri[0].x = 69;
	tri[0].y = nPos;
	tri[1].x = 61;
	tri[1].y = nPos - 4;
	tri[2].x = 61;
	tri[2].y = nPos + 4;
	Polygon( hDC, tri, 3 );

	SelectObject(hDC,GreenPen);

	//DRAW TE UP TE DW
	SelectObject(hDC,GreenPen);
	MoveToEx(hDC,5,45,NULL);
	LineTo(hDC,5,35);
	MoveToEx(hDC,5,35,NULL);
	LineTo(hDC,35,35);
	SetTextColor(hDC,GREEN);
	TextOut(hDC,40,28,"TE UP",5);
	MoveToEx(hDC,80,35,NULL);
	LineTo(hDC,115,35);
	MoveToEx(hDC,115,35,NULL);
	LineTo(hDC,115,45);

	//TE DW
	MoveToEx(hDC,5,230,NULL);
	LineTo(hDC,5,240);
	MoveToEx(hDC,5,240,NULL);
	LineTo(hDC,35,240);
	SetTextColor(hDC,GREEN);
	TextOut(hDC,40,233,"TE DW",5);
	MoveToEx(hDC,80,240,NULL);
	LineTo(hDC,115,240);
	MoveToEx(hDC,115,240,NULL);
	LineTo(hDC,115,230);
	
	
	//Body flap
	SetTextColor(hDC,WHITE);
	TextOut(hDC, 60, 5, "BODY FLAP", 9);
	TextOut(hDC, 87, 15, "%", 1);
	SelectObject(hDC,PurplePen);
	MoveToEx(hDC,96,55,NULL);
	LineTo(hDC,96,221);
	for(nPos=70;nPos<=220;nPos+=33) {
		MoveToEx(hDC, 98, nPos, NULL);
		LineTo(hDC, 95, nPos);
	}
	for(nPos=55;nPos<=220;nPos+=33) {
		MoveToEx(hDC, 101, nPos, NULL);
		LineTo(hDC, 95, nPos);
		sprintf(cbuf,"%d", 20*((nPos-40)/33));
		TextOut(hDC, 102, nPos-7, cbuf, strlen(cbuf));
	}
	SelectObject(hDC,PurpleBrush);
	SelectObject(hDC,DarkPurplePen);
	Rectangle (hDC, 87, 50, 94, 226);

	SelectObject(hDC,YellowPen);
	SelectObject(hDC,YellowBrush);
	tri[0].x = 99;
	tri[0].y = 111;
	tri[1].x = 107;
	tri[1].y = 107;
	tri[2].x = 107;
	tri[2].y = 115;
	Polygon( hDC, tri, 3 );
	Rectangle( hDC, 107, 107, 111, 115 );
	
	if(sts->HydraulicsOK()) dNum=sts->aerosurfaces.bodyFlap;
	else dNum=0.0;
	nPos=static_cast<int>(55 + (4.825 * (dNum + 11.7)));// assuming dNum ranges from -11.7º to +22.5º
	tri[0].x = 99;
	tri[0].y = nPos;
	tri[1].x = 91;
	tri[1].y = nPos - 4;
	tri[2].x = 91;
	tri[2].y = nPos + 4;
	Polygon( hDC, tri, 3 );

	//Rudder
	SetTextColor(hDC,WHITE);
	TextOut(hDC, 160, 25, "RUDDER-DEG", 10);
	SelectObject(hDC,DarkPurplePen);
	SelectObject(hDC,PurpleBrush);
	Rectangle (hDC, 125, 62, 256, 71);
	SelectObject(hDC,PurpleThinPen);
	MoveToEx(hDC,130,60,NULL);
	LineTo(hDC,251,60);
	for(nPos=130;nPos<=170;nPos+=20) {
		MoveToEx(hDC, nPos, 53, NULL);
		LineTo(hDC, nPos, 60);
		sprintf(cbuf,"%d", abs(30-10*((nPos-130)/20)));
		TextOut(hDC, nPos-7, 40, cbuf, strlen(cbuf));
	}
	for(nPos=210;nPos<=250;nPos+=20) {
		MoveToEx(hDC, nPos, 53, NULL);
		LineTo(hDC, nPos, 60);
		sprintf(cbuf,"%d", abs(30-10*((nPos-130)/20)));
		TextOut(hDC, nPos-7, 40, cbuf, strlen(cbuf));
	}
	for(nPos=140;nPos<=250;nPos+=20) {
		MoveToEx(hDC, nPos, 57, NULL);
		LineTo(hDC, nPos, 60);
	}
	// yellow 0 and line
	SetTextColor(hDC,RGB(255,255,0));
	SelectObject(hDC,YellowPen);
	SelectObject(hDC,YellowBrush);
	TextOut(hDC, 187, 40, "0", 1);
	MoveToEx(hDC, 190, 53, NULL);
	LineTo(hDC, 190, 60);

	if(sts->HydraulicsOK()) dNum=-sts->GetControlSurfaceLevel(AIRCTRL_RUDDER);
	else dNum=0.0;
	nPos=static_cast<int>(190.0+dNum*-54.2);
	tri[0].x = nPos;
	tri[0].y = 56;
	tri[1].x = nPos - 4;
	tri[1].y = 64;
	tri[2].x = nPos + 4;
	tri[2].y = 64;
	Polygon( hDC, tri, 3 );

	SetTextColor(hDC,GREEN);
	TextOut(hDC,125,73,"L RUD",5);
	TextOut(hDC,210,73,"R RUD",5);
	//Aileron
	SelectObject(hDC,PurpleThinPen);
	MoveToEx(hDC,130,125,NULL);
	LineTo(hDC,251,125);
	SelectObject(hDC,DarkPurplePen);
	SelectObject(hDC,PurpleBrush);
	SetTextColor(hDC,WHITE);
	TextOut(hDC, 160, 90, "AILERON-DEG", 11);
	Rectangle (hDC, 125, 127, 256, 135);
	TextOut(hDC, 123, 105, "5", 1);
	TextOut(hDC, 245, 105, "5", 1);
	SelectObject(hDC,PurpleThinPen);
	for(nPos=130;nPos<=178;nPos+=12) {
		MoveToEx(hDC, nPos, 119, NULL);
		LineTo(hDC, nPos, 125);
	}
	for(nPos=202;nPos<=250;nPos+=12) {
		MoveToEx(hDC, nPos, 119, NULL);
		LineTo(hDC, nPos, 125);
	}
	for(nPos=136;nPos<=250;nPos+=12) {
		MoveToEx(hDC, nPos, 122, NULL);
		LineTo(hDC, nPos, 125);
	}

	// yellow 0 and line
	SetTextColor(hDC,RGB(255,255,0));
	SelectObject(hDC,YellowPen);
	SelectObject(hDC,YellowBrush);
	TextOut(hDC, 187, 105, "0", 1);
	MoveToEx(hDC, 190, 119, NULL);
	LineTo(hDC, 190, 125);

	if(sts->HydraulicsOK()) dNum=(sts->aerosurfaces.leftElevon-sts->aerosurfaces.rightElevon)/2.0;
	else dNum=0.0;

	nPos=static_cast<int>(190.0+12.0*dNum);
	tri[0].x = nPos;
	tri[0].y = 121;
	tri[1].x = nPos - 4;
	tri[1].y = 129;
	tri[2].x = nPos + 4;
	tri[2].y = 129;
	Polygon( hDC, tri, 3 );
	
	SetTextColor(hDC,GREEN);
	TextOut(hDC,125,137,"L AIL",5);
	TextOut(hDC,220,137,"R AIL",5);


	//SPEEDBRAKE
	SetTextColor(hDC,WHITE);
	TextOut(hDC, 140, 160, "SPEEDBRAKE %", 12);
	SetTextColor(hDC,RGB(255,255,0));
	TextOut(hDC, 160, 175,"ACTUAL",6);
	SelectObject(hDC,BlackBrush);
	SelectObject(hDC,BoldWhitePen);
	Rectangle(hDC,220,175,250,190);
	double Actual = sts->aerosurfaces.speedbrake;
	char ActualBuf[3];
	sprintf(ActualBuf,"%.0lf",Actual);
	TextOut(hDC,223,175,ActualBuf,strlen(ActualBuf));
	SelectObject(hDC,PurpleThinPen);
	SetTextColor(hDC,WHITE);
	double capt = 0;
	for(int i=140; i<=240; i+=20)
	{
		char captbuf[3];
		sprintf(captbuf,"%.0lf",capt);
		TextOut(hDC,i-7,190,captbuf, strlen( captbuf ) );
		capt+=20;
	}

	for(int i=140; i<240; i+=20)
	{
		MoveToEx(hDC,i,203,NULL);
		LineTo(hDC,i,208);
		LineTo(hDC,i+10,208);
		MoveToEx(hDC,i+10,205,NULL);
		LineTo(hDC,i+10,208);
		LineTo(hDC,i+20,208);
		LineTo(hDC,i+20,203);
	}

	SelectObject(hDC,DarkPurplePen);
	SelectObject(hDC,PurpleBrush);
	Rectangle(hDC,135,210,245,218);

	SelectObject(hDC,PurpleThinPen);
	for(int i=140; i<240; i+=20)
	{
		MoveToEx(hDC,i,224,NULL);
		LineTo(hDC,i,219);
		LineTo(hDC,i+10,219);
		MoveToEx(hDC,i+10,221,NULL);
		LineTo(hDC,i+10,219);
		LineTo(hDC,i+20,219);
		LineTo(hDC,i+20,224);
	}

	SetTextColor(hDC,RGB(0,255,255));
	TextOut(hDC, 155, 230,"COMMAND",7);
	SelectObject(hDC,BlackBrush);
	SelectObject(hDC,BoldWhitePen);
	Rectangle(hDC,220,230,250,245);
	double Command = sts->spdb_tgt*100;
	char CommandBuf[3];
	sprintf(CommandBuf,"%.0lf",Command);
	TextOut(hDC,223,230,CommandBuf,strlen(CommandBuf));

	SelectObject(hDC,YellowPen);
	SelectObject(hDC,YellowBrush);
	tri[0].x = static_cast<int>(140+((Actual/100)*100));
	tri[0].y = 206;
	tri[1].x = static_cast<int>(135+((Actual/100)*100));
	tri[1].y = 212;
	tri[2].x = static_cast<int>(145+((Actual/100)*100));
	tri[2].y = 212;
	Polygon( hDC, tri, 3 );
	
	SelectObject(hDC,LightBluePen);
	SelectObject(hDC,LightBlueBrush);
	tri[0].x = static_cast<int>(140+((Command/100)*100));
	tri[0].y = 221;
	tri[1].x = static_cast<int>(135+((Command/100)*100));
	tri[1].y = 214;
	tri[2].x = static_cast<int>(145+((Command/100)*100));
	tri[2].y = 214;
	Polygon( hDC, tri, 3 );

	RestoreDC(hDC, savedDC); // deselect pens, brushes, etc. so we can delete them
	DeleteObject(GreenPen);
}

void CRT::APUHYD(HDC hDC)
{
	int nPos;
	double dNum;
	char cbuf[255];

	int save = SaveDC(hDC);
	SelectObject(hDC, ArialFont_h13w6);
	SelectObject(hDC, GreenPen);
	SelectObject(hDC, BlackBrush);
	SelectDefaultFont(hDC, 0);

	SetTextColor(hDC, GREEN);
	TextOut(hDC, 127, 0, "APU", 3);
	MoveToEx(hDC, 1, 6, NULL);
	LineTo(hDC, 123, 6);
	MoveToEx(hDC, 156, 6, NULL);
	LineTo(hDC, 255, 6);
	MoveToEx(hDC, 0, 6, NULL);
	LineTo(hDC, 0, 16);
	MoveToEx(hDC, 255, 6, NULL);
	LineTo(hDC, 255, 16);

	TextOut(hDC, 115, 165, "HYDRAULIC", 9);
	MoveToEx(hDC, 1, 171, NULL);
	LineTo(hDC, 110, 171);
	MoveToEx(hDC, 185, 171, NULL);
	LineTo(hDC, 255, 171);
	MoveToEx(hDC, 0, 171, NULL);
	LineTo(hDC, 0, 181);
	MoveToEx(hDC, 255, 171, NULL);
	LineTo(hDC, 255, 181);

	SetTextColor(hDC, WHITE);

	// APU
	TextOut(hDC, 37, 7, "1", 1);
	TextOut(hDC, 67, 7, "2", 1);
	TextOut(hDC, 97, 7, "3", 1);
	TextOut(hDC, 0, 45, "FUEL", 4);
	TextOut(hDC, 2, 55, "QTY", 3);
	TextOut(hDC, 10, 65, "%", 1);
	Rectangle (hDC, 27, 20, 53, 36);
	Rectangle (hDC, 57, 20, 83, 36);
	Rectangle (hDC, 87, 20, 113, 36);

	TextOut(hDC, 0, 120, "H2O", 3);
	TextOut(hDC, 0, 130, "QTY", 3);
	TextOut(hDC, 8, 140, "%", 1);
	Rectangle (hDC, 27, 95, 53, 111);
	Rectangle (hDC, 57, 95, 83, 111);
	Rectangle (hDC, 87, 95, 113, 111);

	TextOut(hDC, 168, 7, "1", 1);
	TextOut(hDC, 201, 7, "2", 1);
	TextOut(hDC, 234, 7, "3", 1);
	TextOut(hDC, 134, 50, "FUEL", 4);
	TextOut(hDC, 144, 60, "P", 1);
	Rectangle (hDC, 158, 20, 187, 36);
	Rectangle (hDC, 191, 20, 220, 36);
	Rectangle (hDC, 224, 20, 253, 36);

	TextOut(hDC, 132, 120, "OIL", 3);
	TextOut(hDC, 113, 130, "IN TEMP", 7);
	SelectObject(hDC, WhitePen);
	Ellipse( hDC, 134, 144, 137, 147 );// draw circle instead of "º" as it causes problems in current font in D3D9
	SelectObject(hDC, GreenPen);
	TextOut(hDC, 137, 140, "F", 1 );
	Rectangle (hDC, 158, 95, 187, 111);
	Rectangle (hDC, 191, 95, 220, 111);
	Rectangle (hDC, 224, 95, 253, 111);

	// HYD
	TextOut(hDC, 37, 174, "1", 1);
	TextOut(hDC, 67, 174, "2", 1);
	TextOut(hDC, 97, 174, "3", 1);
	TextOut(hDC, 0, 210, "QTY", 3);
	TextOut(hDC, 8, 220, "%", 1);
	Rectangle (hDC, 27, 187, 53, 203);
	Rectangle (hDC, 57, 187, 83, 203);
	Rectangle (hDC, 87, 187, 113, 203);

	TextOut(hDC, 168, 174, "1", 1);
	TextOut(hDC, 201, 174, "2", 1);
	TextOut(hDC, 234, 174, "3", 1);
	TextOut(hDC, 125, 215, "PRESS", 5);
	Rectangle (hDC, 158, 187, 187, 203);
	Rectangle (hDC, 191, 187, 220, 203);
	Rectangle (hDC, 224, 187, 253, 203);
	TextOut(hDC, 185, 239, "L", 1);
	TextOut(hDC, 218, 239, "L", 1);
	TextOut(hDC, 250, 239, "L", 1);

	SelectObject(hDC, WhitePen);
	
	//Fuel Qty
	Rectangle (hDC, 32, 40, 47, 83);
	Rectangle (hDC, 62, 40, 77, 83);
	Rectangle (hDC, 92, 40, 107, 83);
	MoveToEx(hDC, 47, 74, NULL);
	LineTo(hDC, 53, 74);
	MoveToEx(hDC, 77, 74, NULL);
	LineTo(hDC, 83, 74);
	MoveToEx(hDC, 107, 74, NULL);
	LineTo(hDC, 113, 74);

	//H2O Qty
	Rectangle (hDC, 32, 115, 47, 158);
	Rectangle (hDC, 62, 115, 77, 158);
	Rectangle (hDC, 92, 115, 107, 158);
	MoveToEx(hDC, 47, 141, NULL);
	LineTo(hDC, 53, 141);
	MoveToEx(hDC, 77, 141, NULL);
	LineTo(hDC, 83, 141);
	MoveToEx(hDC, 107, 141, NULL);
	LineTo(hDC, 113, 141);

	//Fuel P
	Rectangle (hDC, 165, 40, 180, 83);
	Rectangle (hDC, 198, 40, 213, 83);
	Rectangle (hDC, 231, 40, 246, 83);

	//Oil In Temp
	Rectangle (hDC, 165, 115, 180, 158);
	Rectangle (hDC, 198, 115, 213, 158);
	Rectangle (hDC, 231, 115, 246, 158);
	MoveToEx(hDC, 180, 153, NULL);
	LineTo(hDC, 186, 153);
	MoveToEx(hDC, 213, 153, NULL);
	LineTo(hDC, 219, 153);
	MoveToEx(hDC, 246, 153, NULL);
	LineTo(hDC, 252, 153);
	MoveToEx(hDC, 180, 133, NULL);
	LineTo(hDC, 186, 133);
	MoveToEx(hDC, 213, 133, NULL);
	LineTo(hDC, 219, 133);
	MoveToEx(hDC, 246, 133, NULL);
	LineTo(hDC, 252, 133);

	//Hydraulic Qty
	Rectangle (hDC, 32, 207, 47, 250);
	Rectangle (hDC, 62, 207, 77, 250);
	Rectangle (hDC, 92, 207, 107, 250);
	MoveToEx(hDC, 47, 233, NULL);
	LineTo(hDC, 53, 233);
	MoveToEx(hDC, 77, 233, NULL);
	LineTo(hDC, 83, 233);
	MoveToEx(hDC, 107, 233, NULL);
	LineTo(hDC, 113, 233);
	MoveToEx(hDC, 47, 210, NULL);
	LineTo(hDC, 53, 210);
	MoveToEx(hDC, 77, 210, NULL);
	LineTo(hDC, 83, 210);
	MoveToEx(hDC, 107, 210, NULL);
	LineTo(hDC, 113, 210);

	//Hydraulic Press
	Rectangle (hDC, 165, 207, 180, 250);
	Rectangle (hDC, 198, 207, 213, 250);
	Rectangle (hDC, 231, 207, 246, 250);
	MoveToEx(hDC, 180, 244, NULL);
	LineTo(hDC, 186, 244);
	MoveToEx(hDC, 213, 244, NULL);
	LineTo(hDC, 219, 244);
	MoveToEx(hDC, 246, 244, NULL);
	LineTo(hDC, 252, 244);
	MoveToEx(hDC, 180, 239, NULL);
	LineTo(hDC, 186, 239);
	MoveToEx(hDC, 213, 239, NULL);
	LineTo(hDC, 219, 239);
	MoveToEx(hDC, 246, 239, NULL);
	LineTo(hDC, 252, 239);
	MoveToEx(hDC, 180, 224, NULL);
	LineTo(hDC, 186, 224);
	MoveToEx(hDC, 213, 224, NULL);
	LineTo(hDC, 219, 224);
	MoveToEx(hDC, 246, 224, NULL);
	LineTo(hDC, 252, 224);


	for(nPos=0;nPos<3;nPos++) {
		//Fuel Qty
		dNum=(sts->pAPU[nPos]->GetFuelLevel()/APU_FUEL_TANK_MASS)*100.0;
		sprintf_s(cbuf, 10, "%03.0f", dNum);
		TextOut(hDC, 29+30*nPos, 22, cbuf, strlen(cbuf));
		if (dNum >= 20)
		{
			SelectObject(hDC, GreenBrush);
			SelectObject(hDC, GreenPen);
			if (dNum > 100) dNum = 100;
		}
		else
		{
			SelectObject(hDC, RedBrush);
			SelectObject(hDC, RedPen);
			if (dNum < 0) dNum = 0;
		}
		Rectangle(hDC, 33+30*nPos, static_cast<int>(82-0.41*dNum), 46+30*nPos, 82);

		//H2O Qty
		dNum = 0;// TODO get real value
		sprintf_s(cbuf, 10, "%03.0f", dNum);
		TextOut(hDC, 29+30*nPos, 97, cbuf, strlen(cbuf));
		if (dNum >= 40)
		{
			SelectObject(hDC, GreenBrush);
			SelectObject(hDC, GreenPen);
			if (dNum > 100) dNum = 100;
		}
		else
		{
			SelectObject(hDC, RedBrush);
			SelectObject(hDC, RedPen);
			if (dNum < 0) dNum = 0;
		}
		Rectangle(hDC, 33+30*nPos, static_cast<int>(157-0.41*dNum), 46+30*nPos, 157);

		//Fuel P
		dNum = sts->pAPU[nPos]->GetFuelPressure();
		sprintf_s(cbuf, 10, "%04.0f", dNum);
		TextOut(hDC, 159+33*nPos, 22, cbuf, strlen(cbuf));
		SelectObject(hDC, GreenBrush);
		SelectObject(hDC, GreenPen);
		if (dNum > 500) dNum = 500;
		if (dNum < 0) dNum = 0;
		Rectangle(hDC, 166+33*nPos, static_cast<int>(82-0.082*dNum), 179+33*nPos, 82);

		//Oil In Temp
		dNum = 0;// TODO get real value
		sprintf_s(cbuf, 10, "%04.0f", dNum);
		TextOut(hDC, 159+33*nPos, 97, cbuf, strlen(cbuf));
		if (dNum >= 291)
		{
			SelectObject(hDC, RedBrush);
			SelectObject(hDC, RedPen);
			if (dNum > 500) dNum = 500;
		}
		else if (dNum >= 45)
		{
			SelectObject(hDC, GreenBrush);
			SelectObject(hDC, GreenPen);
		}
		else
		{
			SelectObject(hDC, RedBrush);
			SelectObject(hDC, RedPen);
			if (dNum < 0) dNum = 0;
		}
		Rectangle(hDC, 166+33*nPos, static_cast<int>(157-0.082*dNum), 179+33*nPos, 157);

		//Hydraulic Qty
		dNum = 0;// TODO get real value
		sprintf_s(cbuf, 10, "%03.0f", dNum);
		TextOut(hDC, 29+30*nPos, 189, cbuf, strlen(cbuf));
		if (dNum >= 96)
		{
			SelectObject(hDC, RedBrush);
			SelectObject(hDC, RedPen);
			if (dNum > 100) dNum = 100;
		}
		else if (dNum >= 40)
		{
			SelectObject(hDC, GreenBrush);
			SelectObject(hDC, GreenPen);
		}
		else
		{
			SelectObject(hDC, RedBrush);
			SelectObject(hDC, RedPen);
			if (dNum < 0) dNum = 0;
		}
		Rectangle(hDC, 33+30*nPos, static_cast<int>(249-0.41*dNum), 46+30*nPos, 249);

		//Hydraulic Press
		dNum = sts->pAPU[nPos]->GetHydraulicPressure();
		sprintf_s(cbuf, 10, "%04.0f", dNum);
		TextOut(hDC, 159+33*nPos, 187, cbuf, strlen(cbuf));
		if (dNum >= 2400)
		{
			SelectObject(hDC, GreenBrush);
			SelectObject(hDC, GreenPen);
			if (dNum > 4000) dNum = 4000;
		}
		else if (dNum >= 1001)
		{
			SelectObject(hDC, RedBrush);
			SelectObject(hDC, RedPen);
		}
		else if (dNum >= 501)
		{
			SelectObject(hDC, GreenBrush);
			SelectObject(hDC, GreenPen);
		}
		else
		{
			SelectObject(hDC, RedBrush);
			SelectObject(hDC, RedPen);
			if (dNum < 0) dNum = 0;
		}
		Rectangle(hDC, 166+33*nPos, static_cast<int>(249-0.01025*dNum), 179+33*nPos, 249);
	}

	RestoreDC(hDC, save);
}

char *CRT::ButtonLabel (int bt)
{
	static char *label[3][4] = {{"", "FLT", "SUB", ""},
	{"UP", "A/E", "ORBIT", ""},
	{"UP", "OMS", "HYD", "SPI"}};
	return ((mode < 3 && bt < 4) ? label[mode][bt] : NULL);
}

// Return button menus
int CRT::ButtonMenu (const MFDBUTTONMENU **menu) const
{
	static const MFDBUTTONMENU mnu[3][4] = {
		{{"", 0, 'U'}, {"FLT INST", 0, '1'}, {"SUBSYS STATUS", 0, '2'}, {"", 0, '3'}},
		{{"Move up", 0, 'U'}, {"A/E PFD", 0, '1'}, {"ORBIT PFD", 0, '2'}, {"", 0, '3'}},
		{{"Move up", 0, 'U'}, {"OMS/MPS", 0, '1'}, {"HYD/APU", 0, '2'}, {"SPI", 0, '3'}}
	};
	if (menu) *menu = mnu[mode];
	return 4; // return the number of buttons used
}

bool CRT::ConsumeKeyBuffered (DWORD key)
{
	switch (mode)
	{
		case 0:// "CRT display"
			switch (key)
			{
				case OAPI_KEY_1:
					mode = 1;
					display = 1;
					InvalidateDisplay();
					InvalidateButtons();
					return true;
				case OAPI_KEY_2:
					mode = 2;
					display = 1;
					InvalidateDisplay();
					InvalidateButtons();
					return true;
			}
			break;
		case 1:// FLT INST
			switch (key)
			{
				case OAPI_KEY_U:
					mode = 0;
					InvalidateDisplay();
					InvalidateButtons();
					return true;
				case OAPI_KEY_1:
					display = 1;
					InvalidateDisplay();
					return true;
				case OAPI_KEY_2:
					display = 2;
					InvalidateDisplay();
					return true;
			}
			break;
		case 2:// SUBSYS STATUS
			switch (key)
			{
				case OAPI_KEY_U:
					mode = 0;
					InvalidateDisplay();
					InvalidateButtons();
					return true;
				case OAPI_KEY_1:
					display = 1;
					InvalidateDisplay();
					return true;
				case OAPI_KEY_2:
					display = 2;
					InvalidateDisplay();
					return true;
				case OAPI_KEY_3:
					display = 3;
					InvalidateDisplay();
					return true;
			}
			break;
	}
	return false;
}

bool CRT::ConsumeButton (int bt, int event)
{
  if (!(event & PANEL_MOUSE_LBDOWN)) return false;
  static const DWORD btkey[3][4] = {{OAPI_KEY_U, OAPI_KEY_1, OAPI_KEY_2, OAPI_KEY_3},
  {OAPI_KEY_U, OAPI_KEY_1, OAPI_KEY_2, OAPI_KEY_3},
  {OAPI_KEY_U, OAPI_KEY_1, OAPI_KEY_2, OAPI_KEY_3}};
  if (bt < 4) return ConsumeKeyBuffered (btkey[mode][bt]);
  else return false;
}

// MFD message parser
int CRT::MsgProc (UINT msg, UINT mfd, WPARAM wparam, LPARAM lparam)
{
	switch (msg) {
	case OAPI_MSG_MFD_OPENED:
		// Our new MFD mode has been selected, so we create the MFD and
		// return a pointer to it.
		return (int)(new CRT (LOWORD(wparam), HIWORD(wparam), (VESSEL*)lparam));
	}
	return 0;
}

void CRT::WriteStatus(FILEHANDLE scn) const
{
	oapiWriteScenario_int (scn, "Mode2", mode);
	oapiWriteScenario_int (scn, "Display", display);
}

void CRT::ReadStatus(FILEHANDLE scn)
{
	char *line;
	while (oapiReadScenario_nextline (scn, line)) {
		if (!strnicmp (line, "Mode2", 5)) {
			sscanf (line+5, "%d", &mode);
		}
		else if(!strnicmp(line, "Display", 7)) {
			sscanf(line+7, "%d", &display);
		}
		else if (!strnicmp (line, "END_MFD", 7)) break;
	}
}

void CRT::StoreStatus() const
{
	if(MDUID==-1) return;
	saveprm.mode[MDUID]=mode;
	saveprm.display[MDUID]=display;
	saveprm.bValid[MDUID]=true;
}

void CRT::RecallStatus()
{
	if(MDUID==-1) return;
	if(!saveprm.bValid[MDUID]) return;
	mode=saveprm.mode[MDUID];
	display=saveprm.display[MDUID];
}
