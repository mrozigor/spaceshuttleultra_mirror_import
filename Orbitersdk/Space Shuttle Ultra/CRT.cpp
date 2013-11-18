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
//CRT *mfd;
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
	//mfd = NULL;
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

	/*char cbuf[255];
	sprintf(cbuf, "CRT Constructor");
	oapiWriteLog(cbuf);*/

	GreenBrush = CreateSolidBrush( GREEN );
	TurquoiseBrush = CreateSolidBrush( TURQUOISE );
	WhiteBrush = CreateSolidBrush( WHITE );
	BlackBrush = CreateSolidBrush( BLACK );
	RedBrush = CreateSolidBrush( RED );
	WhitePen = CreatePen( PS_SOLID, 0, WHITE );
	GreenPen = CreatePen( PS_SOLID, 0, GREEN );
	TurquoisePen = CreatePen( PS_SOLID, 0, TURQUOISE );
	RedPen = CreatePen( PS_SOLID, 0, RED );
	BlackPen = CreatePen( PS_SOLID, 0, BLACK );

	vessel=v;
	width=w;
	height=h;
	usMDU = 11;
	
	id=-1;

	sprintf_s(cbuf, 200, "[CRT]:DIMENSIONS: %d %d\n", W, H);
	oapiWriteLog(cbuf);

	hCRTFont = CreateFont(8,4, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, OEM_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, FIXED_PITCH, "Arial");


	if(!strcmp(pV->GetClassName(), "Atlantis") || 
		!stricmp(pV->GetClassName(), STD_CLASS_NAME)) {
		//sts=(Atlantis *)pV;
		sts=static_cast<Atlantis*>(pV);
		//id=sts->last_mfd;
		/*sprintf(oapiDebugString(), "%d %f", sts->last_mfd, oapiRand());
		if(sts->last_mfd==4) id=0;
		else if(sts->last_mfd==7) id=1;
		else if(sts->last_mfd==6) id=2;
		else id=-1;
		if(id!=-1) {
			sts->Display[id]=this;
		}*/
		sts->newmfd=this;
		usMDU = sts->GetLastCreatedMFD();
		if(sts->GetMDU(usMDU))
		{
			vc::MDU* pMDU = sts->GetMDU(usMDU); 
			pMDU->ConnectToCRTMFD();
		}
		mode=0;
	}
	else {
		mode=10001;
	}
	strcpy(cDispTitle, "GPC MEMORY");
	usPageNumber = 1;
	usGPCDriver = 1;

	spec=0;
	//mode=0;
	item=0;
	//data=0;
	display=2;

	/*if(saveprm.bValid==false) {
		saveprm.mode=mode;
		saveprm.display=display;
		saveprm.spec=spec;
		saveprm.bValid=true;
	}*/
	//UpdateStatus=true;

	//bTIMER=false;
	//sprintf(oapiDebugString(), "Constructor %f", oapiRand());
	//vessel->GetPMI(PMI);

	//RMS_SEL=0;

	/*for(i=0;i<4;i++) {
		Launch_time[i]=0;
		MET_Add[i]=0;
		//START_TIME[i]=0;
	}*/
	//mfd=this;
	return;
}

CRT::~CRT ()
{
	//if(id>=0 && id<3) sts->Display[id]=NULL;
	DeleteObject(hCRTFont);

	DeleteObject( GreenBrush );
	DeleteObject( TurquoiseBrush );
	DeleteObject( WhiteBrush );
	DeleteObject( BlackBrush );
	DeleteObject( RedBrush );
	DeleteObject( WhitePen );
	DeleteObject( GreenPen );
	DeleteObject( TurquoisePen );
	DeleteObject( RedPen );
	DeleteObject( BlackPen );
	return;
}

void CRT::Update (HDC hDC)
{
	char cbuf[255];
	/*double dNum;
	int nPos, nLoc, EngConvert[3]={1, 0, 2};*/
	//sprintf(oapiDebugString(), "%i", id);
	if(UpdateStatus) {
		RecallStatus();
		UpdateStatus=false;
	}
	//sprintf(oapiDebugString(), "%d %d %d", mode, display, id);

	//if(data!=0) Data(1-data);

	//sprintf(oapiDebugString(), "%d", id);

	//Simtime=oapiGetSimTime();

	if(mode==10001) {
			int n;
			sprintf(cbuf, vessel->GetClassName());
			TextOut(hDC, 0, 5, cbuf, strlen(cbuf));
			n=strlen(cbuf);
			sprintf(cbuf, "%i", n);
			TextOut(hDC, 0, 15, cbuf, strlen(cbuf));
			TextOut(hDC, 0, 25, "ERROR: Vessel not from Atlantis class", 37);
			return;
	}
	if(mode==0) {
		if(display==1) {
			OMSMPS(hDC);
		}
		else if(display==2) {
			APUHYD(hDC);
		}
		else if(display==3) {
			SPI(hDC);
		}
	}
	else if(mode==1) {
		//DrawCommonHeader(hDC);
		vc::MDU* pMDU=sts->GetMDU(usMDU);
		pMDU->Paint(hDC);
		//dps::IDP* pIDP=sts->GetIDP(pMDU->GetDrivingIDP());
		/*if( (id>=vc::MDUID_CRT1 && id<=vc::MDUID_CRT4) || 
			sts->panelc2->switch_state[SWITCH2+2*(id - vc::MDUID_CRT1)]==0) //GNC*/
		/*if(pIDP->GetMajfunc()==dps::GNC)
		{
			if(sts->ops==201) {
				switch(spec) {
					case 0:
						UNIVPTG(hDC);
						break;
					case 18: 
						GNCSYSSUMM1(hDC);
						break;
					case 20:
						DAP_CONFIG(hDC);
						break;
				}
			}
			else if(sts->ops == 0) {
				GPCMEMORY(hDC);
			}
			else if(sts->ops==101 || sts->ops==102 || sts->ops==103) {
				PASSTRAJ(hDC);
			}
			else if(sts->ops==104 || sts->ops==105 || sts->ops==106 || sts->ops==202 || sts->ops==301 || sts->ops==302 || sts->ops==303) {
				MNVR(hDC);
			}
			else {
				DrawCommonHeader(hDC);
			}
		}
		//else if(sts->panelc2->switch_state[SWITCH2+2*(id - vc::MDUID_CRT1)]==1) //SM
		else if(pIDP->GetMajfunc()==dps::SM)
		{
			if(sts->ops == 0) {
				GPCMEMORY(hDC);
			}
			else
			{
				PDRS(hDC);
			}
		}
		else //PL
		{
				GPCMEMORY(hDC);	
		}*/
		/*if(sts->ops==201) {
			switch(spec) {
				case 0:
					UNIVPTG(hDC);
					break;
				case 20:
					DAP_CONFIG(hDC);
					break;
			}
		}
		else if(sts->ops==101 || sts->ops==102 || sts->ops==103) {
			PASSTRAJ(hDC);
		}
		else if(sts->ops==104 || sts->ops==105 || sts->ops==106 || sts->ops==202 || sts->ops==301 || sts->ops==302 || sts->ops==303) {
			MNVR(hDC);
		}
		else {
			DrawCommonHeader(hDC);
		}*/
	}
	//sprintf(oapiDebugString(), "%f", Simtime-Simtime_last);
	//Simtime_last=Simtime;
}

void CRT::OMSMPS(HDC hDC)
{
	int nPos, nLoc, EngConvert[3]={1, 0, 2};
	double dNum;
	char cbuf[255];

	SelectDefaultFont(hDC, 0);

	int save = SaveDC(hDC);
		HFONT ArialFont = CreateFont(13,6, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, OEM_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FIXED_PITCH, "Arial");
	SetTextColor(hDC, TURQUOISE);
	SelectObject(hDC,ArialFont);
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
		Rectangle (hDC, 20+35*nPos, round( (89-0.0076*dNum) ), 33+35*nPos, 89);

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
		Rectangle (hDC, 20+35*nPos, round( (154-0.012667*dNum) ), 33+35*nPos, 154);
		
		dNum=100.0*sts->GetThrusterLevel(sts->th_oms[nPos]);
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
		Rectangle(hDC, 20+35*nPos, round( 249 - (0.508333 * dNum) ), 33+35*nPos, 249);
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
		Rectangle( hDC, 162 + nLoc, round( 249 - 0.953125 * (dNum - 45) ), 175 + nLoc, 249 );
	}
	
	// He Tank Press Pneu
	dNum = 0;//sts->GetHeTankPress( 0 );
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
	Rectangle( hDC, 109, round( 81 - 0.019 * (dNum - 3000) ), 122, 81 );

	// He Tank Press Eng 2
	dNum = 0;//sts->GetHeTankPress( 2 );
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
	Rectangle( hDC, 162, round( 81 - 0.0095 * (dNum - 1000) ), 175, 81 );

	// He Tank Press Eng 1
	dNum = 0;//sts->GetHeTankPress( 1 );
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
	Rectangle( hDC, 197, round( 81 - 0.0095 * (dNum - 1000) ), 210, 81 );

	// He Tank Press Eng 3
	dNum = 0;//sts->GetHeTankPress( 3 );
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
	Rectangle( hDC, 231, round( 81 - 0.0095 * (dNum - 1000) ), 244, 81 );

	// He Reg Press Pneu
	dNum = 0;//sts->GetHeRegPress( 0 );
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
	Rectangle( hDC, 109, round( 144 - 0.11 * (dNum - 600) ), 122, 144 );

	// He Reg Press Eng 2
	dNum = 0;//sts->GetHeRegPress( 2 );
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
	Rectangle( hDC, 162, round( 144 - 0.11 * (dNum - 600) ), 175, 144 );

	// He Reg Press Eng 1
	dNum = 0;//sts->GetHeRegPress( 1 );
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
	Rectangle( hDC, 197, round( 144 - 0.11 * (dNum - 600) ), 210, 144 );

	// He Reg Press Eng 3
	dNum = 0;//sts->GetHeRegPress( 3 );
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
	Rectangle( hDC, 231, round( 144 - 0.11 * (dNum - 600) ), 244, 144 );

	// ENG MANF LO2
	dNum = 0;// TODO get val
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
	Rectangle( hDC, 89, round( 238 - (0.123333 * dNum) ), 102, 238 );

	// ENG MANF LH2
	dNum = 0;// TODO get val
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
	Rectangle( hDC, 122, round( 238 - (0.37 * dNum) ), 135, 238 );

	RestoreDC(hDC, save);
	DeleteObject(ArialFont);
}

void CRT::SPI(HDC hDC)
{
	HFONT ArialFont = CreateFont(15,5, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, OEM_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, FIXED_PITCH, "Arial");
	//HBRUSH GreenBrush=CreateSolidBrush(RGB(50,255,50));
	//HBRUSH WhiteBrush=CreateSolidBrush(WHITE);
	//HBRUSH BlackBrush=CreateSolidBrush(BLACK);
	HBRUSH PurpleBrush=CreateSolidBrush(RGB(100,100,100));
	//HPEN WhitePen=CreatePen(PS_SOLID, 0, WHITE);
	HPEN GreenPen=CreatePen(PS_SOLID, 0, RGB(50,255,50));
	HPEN PurplePen=CreatePen(PS_SOLID,2, RGB(150,50,150));
	HPEN PurpleThinPen=CreatePen(PS_SOLID,0, RGB(150,50,150));
	HPEN DarkPurplePen=CreatePen(PS_SOLID,0,RGB(100,100,100));
	HPEN YellowPen=CreatePen(PS_SOLID,1,RGB(255,255,0));
	HPEN BoldWhitePen = CreatePen(PS_SOLID,2,WHITE);
	HPEN LightBluePen = CreatePen(PS_SOLID,0,RGB(0,255,255));

	int savedDC = SaveDC(hDC);
	
	int nPos;
	double dNum;
	char cbuf[255];
	//Elevons
	//SelectDefaultFont(hDC, 0);
	SelectObject(hDC,ArialFont);
	SelectObject(hDC, PurplePen);
	SetTextColor(hDC, WHITE);
	TextOut(hDC, 0, 5, "ELEVONS", 7);
	TextOut(hDC, 12, 15, "DEG", 3);
	//Rectangle (hDC, 10, 40, 21, 206);
	MoveToEx(hDC,10,55,NULL);
	LineTo(hDC,10,221);
	MoveToEx(hDC,21,55,NULL);
	LineTo(hDC,21,221);
	//Rectangle (hDC, 55, 40, 66, 206);
	MoveToEx(hDC,55,55,NULL);
	LineTo(hDC,55,221);
	MoveToEx(hDC,66,55,NULL);
	LineTo(hDC,66,221);
	for(nPos=70;nPos<=220;nPos+=30) {
		MoveToEx(hDC, 4, nPos, NULL);
		LineTo(hDC, 10, nPos);
		MoveToEx(hDC, 26, nPos, NULL);
		LineTo(hDC, 20, nPos);
		MoveToEx(hDC, 49, nPos, NULL);
		LineTo(hDC, 55, nPos);
		MoveToEx(hDC, 71, nPos, NULL);
		LineTo(hDC, 65, nPos);
		sprintf(cbuf,"%d", 10*((nPos-55)/30)-30);
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
	//DRAW INSIDE OF BOXES WITH SLIGHTLY DARKER PURPLE
	SelectObject(hDC, PurpleBrush);
	SelectObject(hDC, DarkPurplePen);
	Rectangle (hDC, 12, 50, 19, 226);
	Rectangle (hDC, 57, 50, 64, 226);
	SelectObject(hDC,GreenPen);
	SelectObject(hDC,BlackBrush);
	/*if(sts->panelr2->HydraulicPressure()) dNum=sts->GetControlSurfaceLevel(AIRCTRL_ELEVATOR);
	else dNum=0.0;
	if(dNum>=0) dNum=dNum*-35;
	else dNum=dNum*-20;*/
	SelectObject(hDC,YellowPen);
	//if(sts->HydraulicsOK()) dNum=(sts->aerosurfaces.leftElevon+sts->aerosurfaces.rightElevon)/2.0;
	if(sts->HydraulicsOK()) dNum=sts->aerosurfaces.leftElevon;
	else dNum=0.0;
	//sprintf(oapiDebugString(),"%lf",dNum);
	nPos=static_cast<int>(145.0+(3.0*dNum))+15;
	MoveToEx(hDC, 6, nPos, NULL);
	LineTo (hDC, 14, nPos-4);
	LineTo (hDC, 14, nPos+4);
	LineTo (hDC, 6, nPos);
	MoveToEx(hDC, 24, nPos, NULL);
	LineTo (hDC, 16, nPos-4);
	LineTo (hDC, 16, nPos+4);
	LineTo (hDC, 24, nPos);
	if(sts->HydraulicsOK()) dNum=sts->aerosurfaces.rightElevon;
	else dNum=0.0;
	nPos=static_cast<int>(145.0+(3.0*dNum))+15;
	MoveToEx(hDC, 51, nPos, NULL);
	LineTo (hDC, 59, nPos-4);
	LineTo (hDC, 59, nPos+4);
	LineTo (hDC, 51, nPos);
	MoveToEx(hDC, 69, nPos, NULL);
	LineTo (hDC, 61, nPos-4);
	LineTo (hDC, 61, nPos+4);
	LineTo (hDC, 69, nPos);
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
	//Rectangle (hDC, 85, 40, 96, 206);
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
	MoveToEx(hDC, 95, 98, NULL);
	LineTo (hDC, 103, 94);
	LineTo (hDC, 103, 102);
	LineTo (hDC, 95, 98);
	/*if(sts->panelr2->HydraulicPressure())dNum=sts->GetControlSurfaceLevel(AIRCTRL_ELEVATORTRIM);
	else dNum=0.0;
	if(dNum>=0) dNum=dNum*-35;
	else dNum=dNum*-65;*/
	
	if(sts->HydraulicsOK()) dNum=sts->aerosurfaces.bodyFlap;
	else dNum=0.0;
	//sprintf(oapiDebugString(),"%lf",dNum+5);
	nPos=static_cast<int>(98.0+(1.65*dNum))+15;
	MoveToEx(hDC, 99, nPos, NULL);
	LineTo (hDC, 91, nPos-4);
	LineTo (hDC, 91, nPos+4);
	LineTo (hDC, 99, nPos);
	//Rudder
	SetTextColor(hDC,WHITE);
	TextOut(hDC, 160, 25, "RUDDER-DEG", 10);
	SelectObject(hDC,DarkPurplePen);
	SelectObject(hDC,PurpleBrush);
	Rectangle (hDC, 125, 62, 256, 71);
	SelectObject(hDC,PurpleThinPen);
	MoveToEx(hDC,130,60,NULL);
	LineTo(hDC,251,60);
	for(nPos=130;nPos<=250;nPos+=20) {
		MoveToEx(hDC, nPos, 53, NULL);
		LineTo(hDC, nPos, 60);
		sprintf(cbuf,"%d", abs(30-10*((nPos-130)/20)));
		TextOut(hDC, nPos-7, 40, cbuf, strlen(cbuf));
	}
	for(nPos=140;nPos<=250;nPos+=20) {
		MoveToEx(hDC, nPos, 57, NULL);
		LineTo(hDC, nPos, 60);
	}
	SelectObject(hDC,YellowPen);
	if(sts->HydraulicsOK()) dNum=-sts->GetControlSurfaceLevel(AIRCTRL_RUDDER);
	else dNum=0.0;
	nPos=static_cast<int>(190.0+dNum*-54.2);
	MoveToEx(hDC, nPos, 56, NULL);
	LineTo (hDC, nPos-4, 64);
	LineTo (hDC, nPos+4, 64);
	LineTo (hDC, nPos, 56);

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
	TextOut(hDC, 187, 105, "0", 1);
	TextOut(hDC, 245, 105, "5", 1);
	SelectObject(hDC,PurpleThinPen);
	for(nPos=130;nPos<=250;nPos+=12) {
		MoveToEx(hDC, nPos, 119, NULL);
		LineTo(hDC, nPos, 125);
	}
	for(nPos=136;nPos<=250;nPos+=12) {
		MoveToEx(hDC, nPos, 122, NULL);
		LineTo(hDC, nPos, 125);
	}
	SelectObject(hDC,YellowPen);
	if(sts->HydraulicsOK()) dNum=(sts->aerosurfaces.leftElevon-sts->aerosurfaces.rightElevon)/2.0;
	else dNum=0.0;
	//dNum=dNum*10.0;
	//if(dNum>5.0) dNum=5.0;
	//else if(dNum < -5.0) dNum= -5.0;
	nPos=static_cast<int>(190.0+12.0*dNum);
	MoveToEx(hDC, nPos, 121, NULL);
	LineTo (hDC, nPos-4, 129);
	LineTo (hDC, nPos+4, 129);
	LineTo (hDC, nPos, 121);
	
	SetTextColor(hDC,GREEN);
	TextOut(hDC,125,137,"L AIL",5);
	TextOut(hDC,220,137,"R AIL",5);


	//SPEEDBRAKE
	SetTextColor(hDC,WHITE);
	TextOut(hDC, 140, 160, "SPEEDBRAKE %", 12);
	SetTextColor(hDC,RGB(255,200,0));
	TextOut(hDC, 160, 175,"ACTUAL",6);
	SelectObject(hDC,BlackBrush);
	SelectObject(hDC,BoldWhitePen);
	Rectangle(hDC,220,175,250,190);
	double Actual = sts->aerosurfaces.speedbrake;
	char ActualBuf[3];
	//sprintf(oapiDebugString(),"%.0lf",Actual);
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
	//sprintf(oapiDebugString(),"%lf",Command);
	char CommandBuf[3];
	sprintf(CommandBuf,"%.0lf",Command);
	TextOut(hDC,223,230,CommandBuf,strlen(CommandBuf));

	SelectObject(hDC,YellowPen);
	MoveToEx(hDC,static_cast<int>(140+((Actual/100)*100)),206,NULL);
	LineTo(hDC,static_cast<int>(135+((Actual/100)*100)),212);
	LineTo(hDC,static_cast<int>(145+((Actual/100)*100)),212);
	LineTo(hDC,static_cast<int>(140+((Actual/100)*100)),206);
	

	SelectObject(hDC,LightBluePen);
	MoveToEx(hDC,static_cast<int>(140+((Command/100)*100)),221,NULL);
	LineTo(hDC,static_cast<int>(135+((Command/100)*100)),214);
	LineTo(hDC,static_cast<int>(145+((Command/100)*100)),214);
	LineTo(hDC,static_cast<int>(140+((Command/100)*100)),221);

	RestoreDC(hDC, savedDC); // deselect pens, brushes, etc. so we can delete them
	DeleteObject(ArialFont);
	//DeleteObject(GreenBrush);
	//DeleteObject(WhiteBrush);
	//DeleteObject(BlackBrush);
	DeleteObject(PurpleBrush);
	//DeleteObject(WhitePen);
	DeleteObject(GreenPen);
	DeleteObject(PurplePen);
	DeleteObject(PurpleThinPen);
	DeleteObject(DarkPurplePen);
	DeleteObject(YellowPen);
	DeleteObject(BoldWhitePen);
	DeleteObject(LightBluePen);


}

void CRT::APUHYD(HDC hDC)
{
	int nPos;
	double dNum;
	char cbuf[255];
		HFONT ArialFont = CreateFont(13,6, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, OEM_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FIXED_PITCH, "Arial");


	//HBRUSH GreenBrush=CreateSolidBrush(GREEN);
	//HBRUSH BlackBrush=CreateSolidBrush(BLACK);
	//HPEN GreenPen=CreatePen(PS_SOLID, 0, GREEN);

	int save = SaveDC(hDC);
	SelectObject(hDC,ArialFont);
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
	TextOut(hDC, 137, 140, "ºF", 2);
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
		//dNum=100*(sts->GetPropellantMass(sts->apu_tank[nPos])/APU_FUEL_TANK_MASS);
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
		dNum = sts->pAPU[nPos]->GetFuelPressure();// TODO check possible bug/misslabeling in APU, as this value should be around 365 psi
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
		//sprintf(cbuf, "%d", sts->panelr2->Hydraulic_Press[nPos]);
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
	DeleteObject(ArialFont);
	//DeleteObject(GreenBrush);
	//DeleteObject(BlackBrush);
	//DeleteObject(GreenPen);
}

void CRT::UNIVPTG(HDC hDC)
{
	vc::MDU* mdu=sts->GetMDU(id);
	if(mdu) mdu->Paint(hDC);
	else sprintf_s(oapiDebugString(), 255, "MDU not initialized");
	return;
}

void CRT::DAP_CONFIG(HDC hDC)
{
	vc::MDU* mdu=sts->GetMDU(id);
	if(mdu) mdu->Paint(hDC);
	else sprintf_s(oapiDebugString(), 255, "MDU not initialized");
	return;
}

void CRT::PASSTRAJ(HDC hdc)
{
	float charW = static_cast<float>(W)/50.0f;
	float charH = static_cast<float>(H)/25.0f;
	char cbuf[255];
	double Ref_hdot = 0.0;

	bool bShowHDot = (sts->GetGPCRefHDot(usGPCDriver, Ref_hdot) == VARSTATE_OK);

	VECTOR3 LVLH_Vel;
	sts->GetGPCLVLHVel(usGPCDriver, LVLH_Vel);
	switch(sts->GetGPCMajorMode())
	{
	case 101:
		SetDisplayTitle("LAUNCH TRAJ");
		break;
	case 102:
	case 103:
		switch(usPageNumber)
		{
		case 1:
			SetDisplayTitle("ASCENT TRAJ");
			break;
		case 2:
			SetDisplayTitle("   TAL TRAJ");
			break;
		case 3:
			SetDisplayTitle("   ATO TRAJ");
			break;
		case 4:
			SetDisplayTitle("  RTLS TRAJ");
			break;
		}
		break;
	case 601:
		SetDisplayTitle("  RTLS TRAJ");
		break;
	}
	DrawCommonHeader(hdc);

	
	//Nominal ascent line
	MoveToEx(hdc, static_cast<int>(0.7083*W), static_cast<int>(0.830*H), NULL);
	LineTo(hdc, static_cast<int>(0.7500*W), static_cast<int>(0.6915*H));
	LineTo(hdc, static_cast<int>(0.9125*W), static_cast<int>(0.5000*H));
	LineTo(hdc, W, static_cast<int>(0.4574*H));

	//EO at Lift-Off line
	MoveToEx(hdc, static_cast<int>(0.625*W), static_cast<int>(0.830*H), NULL);
	LineTo(hdc, static_cast<int>(0.6833*W), static_cast<int>(0.5426*H));
	LineTo(hdc, static_cast<int>(0.3042*W), static_cast<int>(0.7660*H));
	LineTo(hdc, static_cast<int>(0.2292*W), static_cast<int>(0.7872*H));
	LineTo(hdc, static_cast<int>(0.0833*W), static_cast<int>(0.6915*H));

	MoveToEx(hdc, static_cast<int>(0.7167*W), static_cast<int>(0.4681*H), NULL);
	LineTo(hdc, static_cast<int>(0.6917*W), static_cast<int>(0.4787*H));
	LineTo(hdc, static_cast<int>(0.3167*W), static_cast<int>(0.734*H));

	//Q = 2 line
	MoveToEx(hdc, static_cast<int>(0.150*W), static_cast<int>(0.734*H), NULL);
	LineTo(hdc, static_cast<int>(0.0417*W), static_cast<int>(0.7021*H));

	//Q = 10 line
	MoveToEx(hdc, static_cast<int>(0.150*W), static_cast<int>(0.6702*H), NULL);
	LineTo(hdc, static_cast<int>(0.0417*W), static_cast<int>(0.617*H));

	//Hdot indicator
	MoveToEx(hdc, static_cast<int>(0.0667*W), static_cast<int>(0.1170*H), NULL);
	LineTo(hdc, static_cast<int>(0.0416*W), static_cast<int>(0.1170*H));
	LineTo(hdc, static_cast<int>(0.0416*W), static_cast<int>(0.8085*H));
	LineTo(hdc, static_cast<int>(0.0667*W), static_cast<int>(0.8085*H));
	short sHDot_cy = static_cast<short>(0.4737*H);
	short sHDot_cx = static_cast<short>(0.0416*W);
	float HDot_Scale = (0.4737f - 0.1170f) * W;
	MoveToEx(hdc, sHDot_cx, sHDot_cy, NULL);
	LineTo(hdc, static_cast<int>(0.0667*W), sHDot_cy);

	if(bShowHDot)
	{
		double HDot_Error = -LVLH_Vel.z - Ref_hdot;

		if(HDot_Error > 200.0)
		{
			HDot_Error = 200.0;
		}
		else if (HDot_Error < -200.0)
		{
			HDot_Error = -200.0;
		}

		short sHDot_pry = sHDot_cy - static_cast<short>(HDot_Error/200.0 * HDot_Scale);

		MoveToEx(hdc, sHDot_cx, sHDot_cy + sHDot_pry, NULL);
		LineTo(hdc, sHDot_cx-W/50, sHDot_cy + sHDot_pry - W/50);
		LineTo(hdc, sHDot_cx-W/50, sHDot_cy + sHDot_pry + W/50);
		LineTo(hdc, sHDot_cx, sHDot_cy + sHDot_pry);
	}

	//DR indicator
	short sDR_cy = static_cast<short>(0.1064*H);
	short sDR_by = static_cast<short>(0.1383*H);

	MoveToEx(hdc, static_cast<int>(0.1*W), sDR_cy, NULL);
	LineTo(hdc, static_cast<int>(0.9667*W), sDR_cy);

	//PD3 Mark
	MoveToEx(hdc, static_cast<int>(0.6417*W), sDR_cy, NULL);
	LineTo(hdc, static_cast<int>(0.6417*W), sDR_by);

	//PD Mark
	MoveToEx(hdc, static_cast<int>(0.5*W), sDR_cy, NULL);
	LineTo(hdc, static_cast<int>(0.5*W), sDR_by);

	//CO Mark
	MoveToEx(hdc, static_cast<int>(0.1833*W), sDR_cy, NULL);
	LineTo(hdc, static_cast<int>(0.1833*W), sDR_by);

	//Current vehicle state:
	double VHI = LVLH_Vel.x;
	double Altitude = sts->GetAltitude() * MPS2FPS;
	//sprintf(oapiDebugString(), "%f %f", VHI, Altitude);

	/*if(Altitude > 200.0E3 && VHI < 13.9E3)
	{
		//Draw triangle for state vector.
		short stX = width*(VHI/13.9E3 - 1.0);
		short stY = height*(Altitude/200.0E3 - 1.0);
		DrawDelta(hdc, stX, stY, stX-3, stX+3, stY+3);
		sprintf(oapiDebugString(), "%f %f %d %d", VHI, Altitude, stX, stY);
	}*/
	if(Altitude > 155500 && VHI < 10000)
	{
		//Draw triangle for state vector
		short stY = static_cast<short>(H*(1.13256 - (Altitude/513955.985)));
		short stX = static_cast<short>(W*(0.36194 + (VHI/15672.3964)));
		DrawDelta(hdc, stX, stY, stX-6, stX+6, stY+9);
		//sprintf(oapiDebugString(), "%f %f %d %d", VHI, Altitude, stX, stY);
	}
	/*if(Altitude < 350.0E3 && VHI < 13.9E3)
	{
		//Draw triangle for state vector.
		short stX = width*(1.0 - VHI/13.9E3);
		short stY = height*(1.0 - Altitude/350.0E3);
		DrawDelta(hdc, stX, stY, stX-3, stX+3, stY+3);
		sprintf(oapiDebugString(), "%f %f %d %d", VHI, Altitude, stX, stY);
	}*/


	if(sts->GetGPCMajorMode() && (sts->GetSRBChamberPressure(0) < 50 || sts->GetSRBChamberPressure(1) < 50))
	{
		strcpy(cbuf, "PC < 50");
		TextOut(hdc, (short)(charW * 9), 5 + (short)(charH * 9), cbuf, strlen(cbuf));
	}

	int iETLevel = sts->GetETPropellant();

	if(iETLevel >= 0)
	{
		sprintf(cbuf, "PRPLT     %02d", iETLevel);
	}
	else {
		sprintf(cbuf, "PRPLT     00M");
	}
	TextOut(hdc, (short)(charW * 9), 5 + (short)(charH * 8), cbuf, strlen(cbuf));
}


void CRT::MNVR(HDC hDC)
{
	//oapiWriteLog("Calling Paint 1");
	vc::MDU* mdu=sts->GetMDU(id);
	//oapiWriteLog("Calling Paint 2");
	if(mdu) mdu->Paint(hDC);
	else sprintf_s(oapiDebugString(), 255, "MDU not initialized");
	return;

	/*SelectDefaultFont(hDC, 0);

	switch(sts->ops) {
		case 104:
			TextOut(hDC, 0, 0, "1041/", 5);
			TextOut(hDC, 60, 0, "OMS 1 MNVR EXEC", 15);
			if((sts->oparam.PeT)<(sts->oparam.ApT)) {
				minutes=sts->oparam.PeT/60;
				seconds=sts->oparam.PeT-(60*minutes);
				sprintf(cbuf, "TTP %.2d:%.2d", minutes, seconds); 
				TextOut(hDC, 102, 117, cbuf, strlen(cbuf));
				//sprintf(oapiDebugString(), "%f %f", sts->oparam.PeT, sts->oparam.ApT);
				//sprintf(oapiDebugString(), "OPARAM %f %f", sts->oparam.PeT, sts->oparam.SMi);
			}
			else {
				minutes=sts->oparam.ApT/60;
				seconds=sts->oparam.ApT-(60*minutes);
				sprintf(cbuf, "TTA %.2d:%.2d", minutes, seconds); 
				TextOut(hDC, 102, 117, cbuf, strlen(cbuf));
			}
			break;
		case 105:
			TextOut(hDC, 0, 0, "1051/", 5);
			TextOut(hDC, 60, 0, "OMS 2 MNVR EXEC", 15);
			if((sts->oparam.PeT)<(sts->oparam.ApT)) {
				minutes=sts->oparam.PeT/60;
				seconds=sts->oparam.PeT-(60*minutes);
				sprintf(cbuf, "TTP %.2d:%.2d", minutes, seconds); 
				TextOut(hDC, 102, 117, cbuf, strlen(cbuf));
				//sprintf(oapiDebugString(), "%f %f", sts->oparam.PeT, sts->oparam.ApT);
				//sprintf(oapiDebugString(), "OPARAM %f %f", sts->oparam.PeT, sts->oparam.SMi);
			}
			else {
				minutes=sts->oparam.ApT/60;
				seconds=sts->oparam.ApT-(60*minutes);
				sprintf(cbuf, "TTA %.2d:%.2d", minutes, seconds); 
				TextOut(hDC, 102, 117, cbuf, strlen(cbuf));
			}
			break;
		case 106:
			TextOut(hDC, 0, 0, "1061/", 5);
			TextOut(hDC, 60, 0, "OMS 2 MNVR COAST", 16);
			if((sts->oparam.PeT)<(sts->oparam.ApT)) {
				minutes=sts->oparam.PeT/60;
				seconds=sts->oparam.PeT-(60*minutes);
				sprintf(cbuf, "TTP %.2d:%.2d", minutes, seconds); 
				TextOut(hDC, 102, 117, cbuf, strlen(cbuf));
				//sprintf(oapiDebugString(), "%f %f", sts->oparam.PeT, sts->oparam.ApT);
				//sprintf(oapiDebugString(), "OPARAM %f %f", sts->oparam.PeT, sts->oparam.SMi);
			}
			else {
				minutes=sts->oparam.ApT/60;
				seconds=sts->oparam.ApT-(60*minutes);
				sprintf(cbuf, "TTA %.2d:%.2d", minutes, seconds); 
				TextOut(hDC, 102, 117, cbuf, strlen(cbuf));
			}
			break;
		case 202:
			TextOut(hDC, 0, 0, "2021/", 5);
			TextOut(hDC, 60, 0, "ORBIT MNVR EXEC", 15);
			//sts->GetElements(NULL, sts->el, &sts->oparam, 0, FRAME_EQU);
			if((sts->oparam.PeT)<(sts->oparam.ApT)) {
				minutes=sts->oparam.PeT/60;
				seconds=sts->oparam.PeT-(60*minutes);
				sprintf(cbuf, "TTP %.2d:%.2d", minutes, seconds); 
				TextOut(hDC, 102, 117, cbuf, strlen(cbuf));
				//sprintf(oapiDebugString(), "%f %f", sts->oparam.PeT, sts->oparam.ApT);
				//sprintf(oapiDebugString(), "OPARAM %f %f", sts->oparam.PeT, sts->oparam.SMi);
			}
			else {
				minutes=sts->oparam.ApT/60;
				seconds=sts->oparam.ApT-(60*minutes);
				sprintf(cbuf, "TTA %.2d:%.2d", minutes, seconds); 
				TextOut(hDC, 102, 117, cbuf, strlen(cbuf));
			}
			break;
		case 301:
			TextOut(hDC, 0, 0, "3011/", 5);
			TextOut(hDC, 60, 0, "DEORB MNVR COAST", 16);
			if((sts->oparam.PeT)<(sts->oparam.ApT)) { // should show REI
				minutes=sts->oparam.PeT/60;
				seconds=sts->oparam.PeT-(60*minutes);
				sprintf(cbuf, "TTP %.2d:%.2d", minutes, seconds); 
				TextOut(hDC, 102, 117, cbuf, strlen(cbuf));
				//sprintf(oapiDebugString(), "%f %f", sts->oparam.PeT, sts->oparam.ApT);
				//sprintf(oapiDebugString(), "OPARAM %f %f", sts->oparam.PeT, sts->oparam.SMi);
			}
			else {
				minutes=sts->oparam.ApT/60;
				seconds=sts->oparam.ApT-(60*minutes);
				sprintf(cbuf, "TTA %.2d:%.2d", minutes, seconds); 
				TextOut(hDC, 102, 117, cbuf, strlen(cbuf));
			}
			break;
		case 302:
			TextOut(hDC, 0, 0, "3021/", 5);
			TextOut(hDC, 60, 0, "DEORB MNVR EXEC", 15);
			if((sts->oparam.PeT)<(sts->oparam.ApT)) { // should show REI
				minutes=sts->oparam.PeT/60;
				seconds=sts->oparam.PeT-(60*minutes);
				sprintf(cbuf, "TTP %.2d:%.2d", minutes, seconds); 
				TextOut(hDC, 102, 117, cbuf, strlen(cbuf));
				//sprintf(oapiDebugString(), "%f %f", sts->oparam.PeT, sts->oparam.ApT);
				//sprintf(oapiDebugString(), "OPARAM %f %f", sts->oparam.PeT, sts->oparam.SMi);
			}
			else {
				minutes=sts->oparam.ApT/60;
				seconds=sts->oparam.ApT-(60*minutes);
				sprintf(cbuf, "TTA %.2d:%.2d", minutes, seconds); 
				TextOut(hDC, 102, 117, cbuf, strlen(cbuf));
			}
			break;
		case 303:
			TextOut(hDC, 0, 0, "3031/", 5);
			TextOut(hDC, 60, 0, "DEORB MNVR COAST", 16);
			if((sts->oparam.PeT)<(sts->oparam.ApT)) { // should show REI/TFF
				minutes=sts->oparam.PeT/60;
				seconds=sts->oparam.PeT-(60*minutes);
				sprintf(cbuf, "TTP %.2d:%.2d", minutes, seconds); 
				TextOut(hDC, 102, 117, cbuf, strlen(cbuf));
				//sprintf(oapiDebugString(), "%f %f", sts->oparam.PeT, sts->oparam.ApT);
				//sprintf(oapiDebugString(), "OPARAM %f %f", sts->oparam.PeT, sts->oparam.SMi);
			}
			else {
				minutes=sts->oparam.ApT/60;
				seconds=sts->oparam.ApT-(60*minutes);
				sprintf(cbuf, "TTA %.2d:%.2d", minutes, seconds); 
				TextOut(hDC, 102, 117, cbuf, strlen(cbuf));
			}
			break;
	}
	sprintf(cbuf, "%.3d/%.2d:%.2d:%.2d", sts->MET[0], sts->MET[1], sts->MET[2], sts->MET[3]);
	TextOut(hDC, 173, 0, cbuf, strlen(cbuf));
	if(bTIMER) {
		timeDiff=sts->tig-sts->met+1;
		TIMER[0]=timeDiff/86400;
		TIMER[1]=(timeDiff-TIMER[0]*86400)/3600;
		TIMER[2]=(timeDiff-TIMER[0]*86400-TIMER[1]*3600)/60;
		TIMER[3]=timeDiff-TIMER[0]*86400-TIMER[1]*3600-TIMER[2]*60;
		sprintf(cbuf, "%.3d/%.2d:%.2d:%.2d", abs(TIMER[0]), abs(TIMER[1]), abs(TIMER[2]), abs(TIMER[3]));
		TextOut(hDC, 173, 9, cbuf, strlen(cbuf));
	}

	TextOut(hDC, 0, 9, "OMS BOTH 1", 10);
	TextOut(hDC, 49, 18, "L 2", 3);
	TextOut(hDC, 49, 27, "R 3", 3);
	TextOut(hDC, 0, 36, "RCS SEL  4", 10);
	TextOut(hDC, 70, 9+9*sts->OMS, "*", 1);

	sprintf(cbuf, "5 TV ROLL %d", sts->TV_ROLL);
	TextOut(hDC, 0, 45, cbuf, strlen(cbuf));
	TextOut(hDC, 0, 54, "TRIM LOAD", 9);
	sprintf(cbuf, " 6 P  %+2.1f", sts->Trim.data[0]);
	TextOut(hDC, 0, 63, cbuf, strlen(cbuf));
	sprintf(cbuf, " 7 LY %+2.1f", sts->Trim.data[1]);
	TextOut(hDC, 0, 72, cbuf, strlen(cbuf));
	sprintf(cbuf, " 8 RY %+2.1f", sts->Trim.data[2]);
	TextOut(hDC, 0, 81, cbuf, strlen(cbuf));
	sprintf(cbuf, "9 WT %6.0f", sts->WT);
	TextOut(hDC, 0, 90, cbuf, strlen(cbuf));
	TextOut(hDC, 0, 99, "10 TIG", 6);
	sprintf(cbuf, "%03.0f/%02.0f:%02.0f:%04.1f", sts->TIG[0], sts->TIG[1], sts->TIG[2], sts->TIG[3]);
	TextOut(hDC, 0, 108, cbuf, strlen(cbuf));
	
	TextOut(hDC, 0, 117, "TGT PEG 4", 9);
	TextOut(hDC, 0, 126, " 14 C1", 6);
	TextOut(hDC, 0, 135, " 15 C2", 6);
	TextOut(hDC, 0, 144, " 16 HT", 6);
	TextOut(hDC, 0, 153, " 17  T", 6);
	Ellipse(hDC, 28, 156, 34, 165);
	MoveToEx(hDC, 28, 160, NULL);
	LineTo(hDC, 34, 160);
	TextOut(hDC, 0, 162, " 18 PRPLT", 9);
	
	TextOut(hDC, 0, 171, "TGT PEG 7", 9);
	TextOut(hDC, 0, 180, " 19  VX", 7);
	DrawDelta(hDC, 30, 184, 27, 33, 190);
	TextOut(hDC, 0, 189, " 20  VY", 7);
	DrawDelta(hDC, 30, 193, 27, 33, 199);
	TextOut(hDC, 0, 198, " 21  VZ", 7);
	DrawDelta(hDC, 30, 202, 27, 33, 208);
	if(sts->PEG7.x!=0.0 || sts->PEG7.y!=0.0 || sts->PEG7.z!=0.0) {
		sprintf(cbuf, "%+7.1f", sts->PEG7.x);
		TextOut(hDC, 49, 180, cbuf, strlen(cbuf));
		sprintf(cbuf, "%+6.1f", sts->PEG7.y);
		TextOut(hDC, 56, 189, cbuf, strlen(cbuf));
		sprintf(cbuf, "%+6.1f", sts->PEG7.z);
		TextOut(hDC, 56, 198, cbuf, strlen(cbuf));
	}
	
	if(sts->MNVRLOAD) {
		TextOut(hDC, 0, 216, "LOAD 22/TIMER 23", 16);
		sprintf(cbuf, "24 R %-3.0f", sts->BurnAtt.data[ROLL]);
		TextOut(hDC, 100, 27, cbuf, strlen(cbuf));
		sprintf(cbuf, "25 P %-3.0f", sts->BurnAtt.data[PITCH]);
		TextOut(hDC, 100, 36, cbuf, strlen(cbuf));
		sprintf(cbuf, "26 Y %-3.0f", sts->BurnAtt.data[YAW]);
		TextOut(hDC, 100, 45, cbuf, strlen(cbuf));
	}
	else {
		TextOut(hDC, 0, 216, "     22/TIMER 23", 16);
		TextOut(hDC, 100, 27, "24 R", 4);
		TextOut(hDC, 100, 36, "25 P", 4);
		TextOut(hDC, 100, 45, "26 Y", 4);
	}

	MoveToEx(hDC, 98, 15, NULL);
	LineTo(hDC, 98, 218);

	TextOut(hDC, 100, 18, "BURN ATT", 8);
	if(!sts->TRK) TextOut(hDC, 100, 54, "MNVR 27", 7);
	else TextOut(hDC, 100, 54, "MNVR 27X", 8);

	TextOut(hDC, 102, 108, "REI", 3);
	TextOut(hDC, 138, 135, "GMBL", 4);
	TextOut(hDC, 131, 144, "L    R", 6);
	sprintf(cbuf, "P %+02.1f %+02.1f", sts->OMSGimbal[0][0], sts->OMSGimbal[1][0]);
	TextOut(hDC, 102, 153, cbuf, strlen(cbuf));
	sprintf(cbuf, "Y %+02.1f %+02.1f", sts->OMSGimbal[0][1], sts->OMSGimbal[1][1]);
	TextOut(hDC, 102, 162, cbuf, strlen(cbuf));

	TextOut(hDC, 102, 171, "PRI 28  29", 10);
	TextOut(hDC, 102, 180, "SEC 30  31", 10);
	TextOut(hDC, 102, 189, "OFF 32  33", 10);

	TextOut(hDC, 102, 207, "GMBL CK 34", 10);

	MoveToEx(hDC, 156, 15, NULL);
	LineTo(hDC, 156, 111);
	LineTo(hDC, 250, 111);

	if(!sts->BurnInProg && !sts->BurnCompleted) {
		TGO[0]=sts->BurnTime/60;
		TGO[1]=sts->BurnTime-(TGO[0]*60);
	}
	else if(!sts->BurnCompleted) {
		double btRemaining=sts->IgnitionTime+sts->BurnTime-sts->met;
		TGO[0]=(int)btRemaining/60;
		TGO[1]=(int)btRemaining%60;
	}
	else TGO[0]=TGO[1]=0;
	sprintf(cbuf, " VTOT   %6.2f", sts->DeltaVTot);
	TextOut(hDC, 158, 27, cbuf, strlen(cbuf));
	DrawDelta(hDC, 161, 31, 158, 164, 37);
	sprintf(cbuf, "TGO %.2d:%.2d", TGO[0], TGO[1]);
	TextOut(hDC, 158, 36, cbuf, strlen(cbuf));
	sprintf(cbuf, "VGO X %+8.2f", sts->VGO.x);
	TextOut(hDC, 158, 45, cbuf, strlen(cbuf)); 
	sprintf(cbuf, "    Y  %+7.2f", sts->VGO.y);
	TextOut(hDC, 158, 54, cbuf, strlen(cbuf));
	sprintf(cbuf, "    Z  %+7.2f", sts->VGO.z);
	TextOut(hDC, 158, 63, cbuf, strlen(cbuf));
	TextOut(hDC, 165, 81, "    HA     HP", 13);
	sprintf(cbuf, "TGT");
	TextOut(hDC, 158, 90, cbuf, strlen(cbuf));
	sprintf(cbuf, "CUR");
	TextOut(hDC, 158, 99, cbuf, strlen(cbuf));

	sprintf(cbuf, "35 ABORT TGT");
	TextOut(hDC, 150, 126, cbuf, strlen(cbuf));
	
	TextOut(hDC, 185, 135, "FWD RCS", 7);
	TextOut(hDC, 185, 144, "  ARM  36", 9);
	TextOut(hDC, 185, 153, "  DUMP 37", 9);
	TextOut(hDC, 185, 162, "  OFF  38", 9);
	TextOut(hDC, 185, 171, "SURF DRIVE", 10);
	TextOut(hDC, 185, 180, "  ON   39", 9);
	TextOut(hDC, 185, 189, "  OFF  40", 9);*/
}

/*void CRT::PDRS(HDC hDC)
{
	char cbuf[255];

	TextOut(hDC, 0, 0, "2011/094", 8);
	TextOut(hDC, 80, 0, "PDRS CONTROL", 12);
	sprintf(cbuf, "%.3d/%.2d:%.2d:%.2d", sts->MET[0], sts->MET[1], sts->MET[2], sts->MET[3]);
	TextOut(hDC, 173, 0, cbuf, strlen(cbuf));

	//RMS_SEL=0; //should not have to do this!!
	if(RMS_SEL==0) TextOut(hDC, 0, 18, "RMS SEL: PORT", 13);
	else TextOut(hDC, 0, 18, "RMS SEL: STBD", 13);
	TextOut(hDC, 35, 27, "PORT  1", 7);
	TextOut(hDC, 35, 36, "STBD  2", 7);
	TextOut(hDC, 83, 27+9*RMS_SEL, "*", 1);
	//sprintf(oapiDebugString(), "%d", RMS_SEL);

	TextOut(hDC, 0, 108, "  END POS", 9);
	sprintf(cbuf, "18 X");
	TextOut(hDC, 0, 117, cbuf, strlen(cbuf));
	sprintf(cbuf, "19 Y");
	TextOut(hDC, 0, 126, cbuf, strlen(cbuf));
	sprintf(cbuf, "20 Z");
	TextOut(hDC, 0, 135, cbuf, strlen(cbuf));

	TextOut(hDC, 0, 153, "RMS   STO/DPLY", 14);
	sprintf(cbuf, " SHLD %d %d %d %d", sts->MPM_Microswitches[0][0], sts->MPM_Microswitches[0][0],
		sts->MPM_Microswitches[0][1], sts->MPM_Microswitches[0][1]);
	TextOut(hDC, 0, 162, cbuf, strlen(cbuf));
	/*if(sts->RMSRollout.pos==0.0) TextOut(hDC, 0, 162, " SHLD 1 1 0 0", 13);
	else if(sts->RMSRollout.pos==1.0) TextOut(hDC, 0, 162, " SHLD 0 0 1 1", 13);
	else TextOut(hDC, 0, 162, " SHLD 0 0 0 0", 13);*

	TextOut(hDC, 0, 180, "RMS  LAT/REL/RDY", 16);
	sprintf(cbuf, " AFT %d %d %d %d %d %d", sts->MRL_AftMicroswitches[0][0], sts->MRL_AftMicroswitches[0][0],
		sts->MRL_AftMicroswitches[0][1], sts->MRL_AftMicroswitches[0][1], sts->MRL_AftMicroswitches[0][2], sts->MRL_AftMicroswitches[0][2]);
	sprintf(oapiDebugString(), "MRL: %d", sts->MRL_FwdMicroswitches[0][0]);
	TextOut(hDC, 0, 189, cbuf, strlen(cbuf));
	sprintf(cbuf, " MID %d %d %d %d %d %d", sts->MRL_MidMicroswitches[0][0], sts->MRL_MidMicroswitches[0][0],
		sts->MRL_MidMicroswitches[0][1], sts->MRL_MidMicroswitches[0][1], sts->MRL_AftMicroswitches[0][2], sts->MRL_MidMicroswitches[0][2]);
	TextOut(hDC, 0, 198, cbuf, strlen(cbuf));
	sprintf(cbuf, " FWD %d %d %d %d %d %d", sts->MRL_FwdMicroswitches[0][0], sts->MRL_FwdMicroswitches[0][0],
		sts->MRL_FwdMicroswitches[0][1], sts->MRL_FwdMicroswitches[0][1], sts->MRL_FwdMicroswitches[0][2], sts->MRL_FwdMicroswitches[0][2]);
	TextOut(hDC, 0, 207, cbuf, strlen(cbuf));
	//TextOut(hDC, 0, 198, " MID", 4);
	//TextOut(hDC, 0, 207, " FWD", 4);
}*/

void CRT::DrawDelta(HDC hDC, int TopX, int TopY, int LBottomX, int RBottomX, int BottomY)
{
	MoveToEx(hDC, TopX, TopY, NULL);
	LineTo(hDC, LBottomX, BottomY);
	LineTo(hDC, RBottomX, BottomY);
	LineTo(hDC, TopX, TopY);
}

void CRT::EnableThrusters(const int Thrusters[], int nThrusters)
{
/*
	for(int i=0;i<nThrusters;i++) {
		if(Thrusters[i]%2==1)
			sts->SetThrusterResource(sts->th_att_rcs[Thrusters[i]], sts->ph_oms);
		else
			sts->SetThrusterResource(sts->th_att_rcs[Thrusters[i]], sts->ph_frcs);
	}
*/
}

void CRT::DisableThrusters(const int Thrusters[], int nThrusters)
{
/*
	for(int i=0;i<nThrusters;i++) {
		//vessel->SetThrusterMax0(th_att_rcs[Thrusters[i]], 0.0);
		sts->SetThrusterResource(sts->th_att_rcs[Thrusters[i]], NULL);
	}
*/
}

char *CRT::ButtonLabel (int bt)
{
	static char *label[2][4] = {{"UP", "OMS", "HYD", "SPI"},
	{"UP", "", "", ""}};
	return ((mode < 2 && bt < 4) ? label[mode][bt] : NULL);
}

// Return button menus
int CRT::ButtonMenu (const MFDBUTTONMENU **menu) const
{
	static const MFDBUTTONMENU mnu[2][4] = {
		{{"Move up", 0, 'M'},
		{"OMS/MPS", 0, '1'},
		{"APU/HYD", 0, '2'},
		{"SPI", 0, '3'}},
		{{"Move up", 0, 'M'},
		{"OPS", 0, 'O'},
		{"SPEC", 0, 'S'},
		{"ITEM", 0, 'I'}}
	};
	if (menu) *menu = mnu[mode];
	return 4; // return the number of buttons used
}

bool CRT::ConsumeKeyBuffered (DWORD key) {
  bool cbChangeOps(void *id, char *str, void *data);
  bool cbItem(void *id, char *str, void *data);
  bool cbSpec(void *id, char *str, void *data);

  switch (key) {
	case OAPI_KEY_M:
	  if(mode==1) mode=0;
	  else if(mode==0) mode=1;
	  InvalidateDisplay();
	  InvalidateButtons();
	  return true;
	case OAPI_KEY_1:
	  display=1;
	  InvalidateDisplay();
	  return true;
	case OAPI_KEY_2:
	  display=2;
	  InvalidateDisplay();
	  return true;
	case OAPI_KEY_3:
	  display=3;
	  InvalidateDisplay();
	  return true;
	/*case OAPI_KEY_O:
	  oapiOpenInputBox("OPS", cbChangeOps, 0, 20, (void *)this);
	  InvalidateDisplay();
	  return true;
	case OAPI_KEY_S:
	  oapiOpenInputBox("SPEC", cbSpec, 0, 20, (void *)this);
	  InvalidateDisplay();
	  return true;
	case OAPI_KEY_I:
	  return false; //disable item input
	  oapiOpenInputBox("ITEM", cbItem, 0, 20, (void *)this);
	  InvalidateDisplay();
	  return true;*/
  }
  return false;
}

bool CRT::ConsumeButton (int bt, int event)
{
  if (!(event & PANEL_MOUSE_LBDOWN)) return false;
  static const DWORD btkey[2][4] = {{OAPI_KEY_M, OAPI_KEY_1, OAPI_KEY_2, OAPI_KEY_3},
  {OAPI_KEY_M, OAPI_KEY_O, OAPI_KEY_S, OAPI_KEY_I}};
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
	//char cbuf[255];
	oapiWriteScenario_int (scn, "Mode2", mode);
	oapiWriteScenario_int (scn, "Display", display);
	//oapiWriteScenario_int (scn, "OPS", ops);
	//oapiWriteScenario_int (scn, "Spec", spec);
	//oapiWriteScenario_int (scn, "MDU", usMDU);
	/*oapiWriteScenario_int (scn, "MET0", MET[0]);
	oapiWriteScenario_int (scn, "MET1", MET[1]);
	oapiWriteScenario_int (scn, "MET2", MET[2]);
	oapiWriteScenario_int (scn, "MET3", MET[3]);
	oapiWriteScenario_int (scn, "TGT_ID", TGT_ID);
	oapiWriteScenario_int (scn, "BODY_VECT", BODY_VECT);
	oapiWriteScenario_float (scn, "ROLL", MNVR_OPTION.data[ROLL]);
	oapiWriteScenario_float (scn, "PITCH", MNVR_OPTION.data[PITCH]);
	oapiWriteScenario_float (scn, "YAW", MNVR_OPTION.data[YAW]);*/
}

void CRT::ReadStatus(FILEHANDLE scn)
{
	char *line;
	while (oapiReadScenario_nextline (scn, line)) {
		if (!strnicmp (line, "Mode2", 5)) {
			sscanf (line+5, "%d", &mode);
		}
		/*else if (!strnicmp (line, "MDU", 3)) {
			sscanf (line+3, "%u", &usMDU);
			if(sts && sts->GetMDU(usMDU))
			{
				vc::MDU* pMDU = sts->GetMDU(usMDU); 
				pMDU->ConnectToCRTMFD();
			}
		}*/
		else if(!strnicmp(line, "Display", 7)) {
			sscanf(line+7, "%d", &display);
		}
		else if (!strnicmp (line, "END_MFD", 7)) break;
		/*else if(!strnicmp(line, "OPS", 3)) {
			sscanf(line+3, "%d", &ops);
		}
		else if(!strnicmp(line, "Spec", 4)) {
			sscanf(line+4, "%d", &spec);
		}*/
		/*else if(!strnicmp(line, "TIG", 3)) {
			sscanf(line+3, "%lf%lf%lf%lf", &TIG[0], &TIG[1], &TIG[2], &TIG[3]);
		}*/
		/*else if(!strnicmp(line, "MET", 3)) {
			sscanf(line+3, "%d", &nNum);
			sscanf(line+4, "%d", &MET_Add[nNum]);
		}
		else if(!strnicmp(line, "TGT_ID", 6)) {
			sscanf(line+6, "%d", &TGT_ID);
		}
		else if(!strnicmp(line, "BODY_VECT", 9)) {
			sscanf(line+9, "%d", &BODY_VECT);
		}
		else if(!strnicmp(line, "ROLL", 4)) {
			sscanf(line+4, "%lf", &MNVR_OPTION.data[ROLL]);
		}
		else if(!strnicmp(line, "PITCH", 5)) {
			sscanf(line+5, "%lf", &MNVR_OPTION.data[PITCH]);
		}
		else if(!strnicmp(line, "YAW", 3)) {
			sscanf(line+3, "%lf", &MNVR_OPTION.data[YAW]);
		}*/
	}
}

void CRT::StoreStatus() const
{
	if(id==-1) {
		return;
	}
	saveprm.spec[id]=spec;
	saveprm.mode[id]=mode;
	saveprm.display[id]=display;
	saveprm.bValid[id]=true;
	//sprintf(oapiDebugString(), "%i %i %i %i", saveprm.spec[id], saveprm.mode[id], saveprm.display[id], id);
}

void CRT::RecallStatus()
{
	if(id==-1) {
		/*spec=0;
		mode=0;
		display=2;*/
		return;
	}
	if(!saveprm.bValid[id]) return;
	spec=saveprm.spec[id];
	mode=saveprm.mode[id];
	display=saveprm.display[id];
	//sprintf(oapiDebugString(), "Recall %f %d %d", oapiRand(), mode, display);
}

/*void CRT::Data(int id)
{
	bool cbItemData(void *id, char *str, void *data);
	bool cbSpecData(void *id, char *str, void *data);

	data=0;
	if(id==0) oapiOpenInputBox("DATA", cbItemData, 0, 20, (void *)this);
	else if(id==1) oapiOpenInputBox("DATA", cbSpecData, 0, 20, (void *)this);
}*/

/*bool CRT::Input(int change, char *Name)
{
	int nNew;
	double dNew;
	if(change==0) {
		nNew=atoi(Name);
		if(nNew==104 && sts->ops==103) {
			sts->ops=104;
			sts->WT=sts->GetMass()*kg_to_pounds;
			sts->BurnInProg=false;
			sts->BurnCompleted=false;
			sts->MNVRLOAD=false;
			bTIMER=false;
		}
		else if(nNew==105 && sts->ops==104) {
			sts->ops=105;
			sts->WT=sts->GetMass()*kg_to_pounds;
			sts->BurnInProg=false;
			sts->BurnCompleted=false;
			sts->MNVRLOAD=false;
			bTIMER=false;
		}
		else if(nNew==106 && sts->ops==105) {
			sts->ops=106;
		}
		else if(nNew==201 && (sts->ops==202 || sts->ops==106))
		{
			sts->ops=201;
			sts->MNVR=false;
			sts->TRK=false;
			sts->ROT=false;
		}
		else if(nNew==202 && sts->ops==201)
		{
			sts->ops=202;
			sts->WT=sts->GetMass()*kg_to_pounds;
			sts->BurnInProg=false;
			sts->BurnCompleted=false;
			sts->MNVRLOAD=false;
			bTIMER=false;
		}
		else if(nNew==301 && sts->ops==201)
		{
			sts->ops=301;
			sts->WT=sts->GetMass()*kg_to_pounds;
			sts->BurnInProg=false;
			sts->BurnCompleted=false;
			sts->MNVRLOAD=false;
			bTIMER=false;
		}
		else if(nNew==302 && sts->ops==301)
		{
			sts->ops=302;
		}
		else if(nNew==303 && sts->ops==302)
		{
			sts->ops=303;
		}
		return true;
	}
	else if(change==1) {
		nNew=atoi(Name);
		if(sts->ops==201) {
			switch(spec) {
				case 0:
					if(nNew<=17) {
						item=nNew;
						data=1;
						return true;
					}
					else if(nNew==18) {
						if(sts->MNVR==false) 
						{
							sts->MNVR=true;
							sts->ROT=false;
							sts->TRK=false;
							/*if(sts->DAPMode[1]==0) {
								sts->RotRate=sts->DAP[sts->DAPMode[0]].PRI_ROT_RATE;
								sts->AttDeadband=sts->DAP[sts->DAPMode[0]].PRI_ATT_DB;
								sts->RateDeadband=sts->DAP[sts->DAPMode[0]].PRI_RATE_DB;
								if(sts->DAPConfig[sts->DAPMode[0]].PRI_P_OPTION!=0)
									sts->Torque.data[PITCH]=0.5*ORBITER_PITCH_TORQUE;
								else sts->Torque.data[PITCH]=ORBITER_PITCH_TORQUE;
								if(sts->DAPConfig[sts->DAPMode[0]].PRI_Y_OPTION!=0)
									sts->Torque.data[YAW]=0.5*ORBITER_YAW_TORQUE;
								else sts->Torque.data[YAW]=ORBITER_YAW_TORQUE;
								sts->Torque.data[ROLL]=ORBITER_ROLL_TORQUE;
							}
							else if(sts->DAPMode[1]==1) {
								sts->RotRate=sts->DAP[sts->DAPMode[0]].PRI_ROT_RATE;
								sts->AttDeadband=sts->DAP[sts->DAPMode[0]].PRI_ATT_DB;
								sts->RateDeadband=sts->DAP[sts->DAPMode[0]].ALT_RATE_DB;
								if(sts->DAPConfig[sts->DAPMode[0]].ALT_JET_OPT==2) {
									sts->Torque.data[PITCH]=0.5*ORBITER_PITCH_TORQUE;
									sts->Torque.data[YAW]=0.5*ORBITER_YAW_TORQUE;
									sts->Torque.data[ROLL]=0.5*ORBITER_ROLL_TORQUE;
								}
								else {
									sts->Torque.data[PITCH]=ORBITER_PITCH_TORQUE;
									sts->Torque.data[YAW]=ORBITER_YAW_TORQUE;
									sts->Torque.data[ROLL]=ORBITER_ROLL_TORQUE;
								}
							}
							else if(sts->DAPMode[1]==2) {
								sts->RotRate=sts->DAP[sts->DAPMode[0]].VERN_ROT_RATE;
								sts->AttDeadband=sts->DAP[sts->DAPMode[0]].VERN_ATT_DB;
								sts->RateDeadband=sts->DAP[sts->DAPMode[0]].VERN_RATE_DB;
								sts->Torque.data[PITCH]=0.1*ORBITER_PITCH_TORQUE;
								sts->Torque.data[YAW]=0.1*ORBITER_YAW_TORQUE;
								sts->Torque.data[ROLL]=0.1*ORBITER_ROLL_TORQUE;
							}
							sts->REQD_ATT.x=sts->MNVR_OPTION.x;
							sts->REQD_ATT.y=sts->MNVR_OPTION.y;
							sts->REQD_ATT.z=sts->MNVR_OPTION.z;
							for(int i=0;i<3;i++) {
								if(sts->REQD_ATT.data[i]>180.0) sts->TargetAttM50.data[i]=sts->REQD_ATT.data[i]-360.0;
								else sts->TargetAttM50.data[i]=sts->REQD_ATT.data[i];
							}
							sts->TargetAttM50=sts->TargetAttM50*RAD;
							//sts->TargetAttOrbiter=ConvertAnglesBetweenM50AndOrbiter(sts->TargetAttM50, true);
						}
						else sts->MNVR=false;
						sts->Yaw=false;
						sts->Pitch=false;
						sts->Roll=false;
						return true;
					}
					else if(nNew==19) {
						if(sts->TRK==false) {
							sts->MNVR_TIME=0.0; //guess
							sts->TRK=true;
							sts->MNVR=false;
							sts->ROT=false;
							sts->ManeuverinProg=false;
							/*if(sts->DAPMode[1]==0) {
								sts->RotRate=sts->DAP[sts->DAPMode[0]].PRI_ROT_RATE;
								sts->AttDeadband=sts->DAP[sts->DAPMode[0]].PRI_ATT_DB;
								sts->RateDeadband=sts->DAP[sts->DAPMode[0]].PRI_RATE_DB;
							}
							else if(sts->DAPMode[1]==1) {
								sts->RotRate=sts->DAP[sts->DAPMode[0]].PRI_ROT_RATE;
								sts->AttDeadband=sts->DAP[sts->DAPMode[0]].PRI_ATT_DB;
								sts->RateDeadband=sts->DAP[sts->DAPMode[0]].ALT_RATE_DB;
							}
							else if(sts->DAPMode[1]==2) {
								sts->RotRate=sts->DAP[sts->DAPMode[0]].VERN_ROT_RATE;
								sts->AttDeadband=sts->DAP[sts->DAPMode[0]].VERN_ATT_DB;
								sts->RateDeadband=sts->DAP[sts->DAPMode[0]].VERN_RATE_DB;
							}
							if(sts->TGT_ID==2) {
								/*sts->LVLHRateVector.data[PITCH]=0.0;
								sts->LVLHRateVector.data[YAW]=0.0;
								sts->LVLHRateVector.data[ROLL]=0.0;*
								if(sts->BODY_VECT==1) {
									sts->LVLHOrientationReqd.data[PITCH]=270.0;
									sts->LVLHOrientationReqd.data[YAW]=0.0;
									if((sts->OM)<=0.0) {
										sts->LVLHOrientationReqd.data[ROLL]=0.0;
										/*sts->LVLHRateVector.data[PITCH]=-1.0;
										sts->LVLHRateVector.data[YAW]=0.0;
										sts->LVLHRateVector.data[ROLL]=0.0;*
									}
									else {
										sts->LVLHOrientationReqd.data[ROLL]=sts->OM;
										/*sts->LVLHRateVector.data[PITCH]=-1.0*cos(RAD*sts->OM);
										sts->LVLHRateVector.data[YAW]=-1.0*sin(RAD*sts->OM);
										sts->LVLHRateVector.data[ROLL]=0.0;
									}
								}
								else if(sts->BODY_VECT==2) {
									sts->LVLHOrientationReqd.data[PITCH]=90.0;
									sts->LVLHOrientationReqd.data[YAW]=0.0;
									if(sts->OM<=0.0) {
										sts->LVLHOrientationReqd.data[ROLL]=0.0;
										/*sts->LVLHRateVector.data[PITCH]=-1.0;
										sts->LVLHRateVector.data[YAW]=0.0;
										sts->LVLHRateVector.data[ROLL]=0.0;
									}
									else {
										sts->LVLHOrientationReqd.data[ROLL]=sts->OM;
										/*sts->LVLHRateVector.data[PITCH]=-1.0*cos(RAD*sts->OM);
										sts->LVLHRateVector.data[YAW]=-1.0*sin(RAD*sts->OM);
										sts->LVLHRateVector.data[ROLL]=0.0;
									}
								}
								else if(sts->BODY_VECT==3) {
									sts->LVLHOrientationReqd.data[PITCH]=180.0;
									sts->LVLHOrientationReqd.data[ROLL]=0.0;
									if((sts->OM)<=0.0) {
										sts->LVLHOrientationReqd.data[YAW]=0.0;
										/*sts->LVLHRateVector.data[PITCH]=-1.0;
										sts->LVLHRateVector.data[YAW]=0.0;
										sts->LVLHRateVector.data[ROLL]=0.0;
									}
									else {
										sts->LVLHOrientationReqd.data[YAW]=sts->OM;
										/*sts->LVLHRateVector.data[PITCH]=-1.0*cos(RAD*sts->OM);
										sts->LVLHRateVector.data[ROLL]=1.0*sin(RAD*sts->OM);
										sts->LVLHRateVector.data[YAW]=0.0;
									}
								}
								else if(sts->BODY_VECT==5) {
									sts->LVLHOrientationReqd.data[PITCH]=270-sts->P;
									//sts->LVLHOrientationReqd.data[ROLL]=sts->Y;
									/*if((sts->OM)<=0.0) {
										sts->LVLHOrientationReqd.data[YAW]=0.0;
										if(sts->Y==0) {
											sts->LVLHRateVector.data[PITCH]=-1.0;
											sts->LVLHRateVector.data[ROLL]=0.0;
										}
										else {
											sts->LVLHRateVector.data[PITCH]=-1.0*cos(RAD*sts->Y);
											sts->LVLHRateVector.data[ROLL]=-1.0*sin(RAD*sts->Y);
											sts->LVLHRateVector.data[YAW]=0.0;
										}
									}
									else sts->LVLHOrientationReqd.data[YAW]=sts->OM;*
									if(sts->LVLHOrientationReqd.data[PITCH]==0.0)
									{
										sts->LVLHOrientationReqd.data[ROLL]=sts->Y;
										if(sts->OM<=0.0) {
											sts->LVLHOrientationReqd.data[YAW]=0.0;
											/*if(sts->LVLHOrientationReqd.data[ROLL]==0.0) {
												sts->LVLHRateVector.data[PITCH]=-1.0;
												sts->LVLHRateVector.data[YAW]=0.0;
												sts->LVLHRateVector.data[ROLL]=0.0;
											}*
										}
										else sts->LVLHOrientationReqd.data[YAW]=sts->OM;
									}
									else
									{
										sts->LVLHOrientationReqd.data[YAW]=(270.0-sts->P)*sin(RAD*sts->Y);
										sts->LVLHOrientationReqd.data[ROLL]=sts->Y*cos(RAD*sts->LVLHOrientationReqd.data[PITCH]);
										if(sts->OM>0.0) sts->LVLHOrientationReqd.data[YAW]+=sts->OM;
									}
									/*sts->LVLHRateVector.data[PITCH]=-1.0*cos(RAD*sts->LVLHOrientationReqd.data[ROLL])*cos(RAD*sts->LVLHOrientationReqd.data[YAW]);
									sts->LVLHRateVector.data[ROLL]=1.0*sin(RAD*sts->LVLHOrientationReqd.data[YAW]);
									sts->LVLHRateVector.data[YAW]=-1.0*sin(RAD*sts->LVLHOrientationReqd.data[ROLL])*cos(RAD*sts->LVLHOrientationReqd.data[YAW]);*/
									/*else if(sts->OM>=0.0)
									{
										sts->LVLHOrientationReqd.data[ROLL]=sts->Y*cos(RAD*sts->LVLHOrientationReqd.data[PITCH])
											+sts->OM*sin(RAD*sts->LVLHOrientationReqd.data[PITCH]);
										sts->LVLHOrientationReqd.data[YAW]=sts->OM*cos(RAD*sts->LVLHOrientationReqd.data[PITCH])
											+sts->Y*sin(RAD*sts->LVLHOrientationReqd.data[PITCH]);
										if(sts->LVLHOrientationReqd.data[YAW]<0.0) sts->LVLHOrientationReqd.data[YAW]+=360.0;
									}
									else
									{
										sts->LVLHOrientationReqd.data[ROLL]=sts->Y*cos(RAD*sts->LVLHOrientationReqd.data[PITCH]);
										sts->LVLHOrientationReqd.data[YAW]=sts->Y*sin(RAD*sts->LVLHOrientationReqd.data[PITCH]);
										if(sts->LVLHOrientationReqd.data[YAW]<0.0) sts->LVLHOrientationReqd.data[YAW]+=360.0;
									}
									if(sts->LVLHOrientationReqd.data[YAW]>=90.0)
									{
										sts->LVLHOrientationReqd.data[PITCH]-=180.0;
										sts->LVLHOrientationReqd.data[ROLL]+=180.0;
										sts->LVLHOrientationReqd.data[YAW]-=180.0;
									}*
								}
							}
						}
						else sts->TRK=false;
						sts->Yaw=false;
						sts->Pitch=false;
						sts->Roll=false;
						return true;
					}
					else if(nNew==20) {
						if(sts->ROT==false) {
							sts->ROT=true;
							sts->MNVR=false;
							sts->TRK=false;
						}
						else sts->ROT=false;
						return true;
					}
					else if(nNew==21) {
						sts->MNVR=false;
						sts->ROT=false;
						sts->TRK=false;
						sts->SetThrusterGroupLevel(THGROUP_ATT_PITCHUP, 0.0);
						sts->SetThrusterGroupLevel(THGROUP_ATT_PITCHDOWN, 0.0);
						sts->SetThrusterGroupLevel(THGROUP_ATT_YAWLEFT, 0.0);
						sts->SetThrusterGroupLevel(THGROUP_ATT_YAWRIGHT, 0.0);
						sts->SetThrusterGroupLevel(THGROUP_ATT_BANKLEFT, 0.0);
						sts->SetThrusterGroupLevel(THGROUP_ATT_BANKRIGHT, 0.0);
						sts->REQD_ATT.x=sts->MNVR_OPTION.x;
						sts->REQD_ATT.y=sts->MNVR_OPTION.y;
						sts->REQD_ATT.z=sts->MNVR_OPTION.z;
						for(int i=0;i<3;i++) {
							if(sts->REQD_ATT.data[i]>180.0) sts->TargetAttM50.data[i]=sts->REQD_ATT.data[i]-360.0;
							else sts->TargetAttM50.data[i]=sts->REQD_ATT.data[i];
						}
						sts->TargetAttM50=sts->TargetAttM50*RAD;
						//sts->TargetAttOrbiter=ConvertAnglesBetweenM50AndOrbiter(sts->TargetAttM50, true);
						return true;
					}
					break;
				case 20:
					if(nNew==1 || nNew==2) {
						sts->DAPMode[0]=nNew-1;
						UpdateDAP();
						return true;
					}
					else if(nNew>=3 && nNew<=5) {
						sts->DAPMode[1]=nNew-3;
						UpdateDAP();
						return true;
					}
					else if(nNew==6 || nNew==7) {
						edit=nNew-5;
						int i=edit-1;
						sts->DAP[2].PRI_ROT_RATE=sts->DAP[i].PRI_ROT_RATE;
						sts->DAP[2].PRI_ATT_DB=sts->DAP[i].PRI_ATT_DB;
						sts->DAP[2].PRI_RATE_DB=sts->DAP[i].PRI_RATE_DB;
						sts->DAP[2].PRI_ROT_PLS=sts->DAP[i].PRI_ROT_PLS;
						sts->DAP[2].PRI_COMP=sts->DAP[i].PRI_COMP;
						sts->DAP[2].PRI_TRAN_PLS=sts->DAP[i].PRI_TRAN_PLS;
						sts->DAP[2].PRI_P_OPTION=sts->DAP[i].PRI_P_OPTION;
						sts->DAP[2].PRI_Y_OPTION=sts->DAP[i].PRI_Y_OPTION;
						sts->DAP[2].ALT_RATE_DB=sts->DAP[i].ALT_RATE_DB;
						sts->DAP[2].ALT_ON_TIME=sts->DAP[i].ALT_ON_TIME;
						sts->DAP[2].ALT_DELAY=sts->DAP[i].ALT_DELAY;
						sts->DAP[2].ALT_JET_OPT=sts->DAP[i].ALT_JET_OPT;
						sts->DAP[2].ALT_JETS=sts->DAP[i].ALT_JETS;
						sts->DAP[2].VERN_ROT_RATE=sts->DAP[i].VERN_ROT_RATE;
						sts->DAP[2].VERN_ATT_DB=sts->DAP[i].VERN_ATT_DB;
						sts->DAP[2].VERN_RATE_DB=sts->DAP[i].VERN_RATE_DB;
						sts->DAP[2].VERN_ROT_PLS=sts->DAP[i].VERN_ROT_PLS;
						sts->DAP[2].VERN_COMP=sts->DAP[i].VERN_COMP;
						sts->DAP[2].VERN_CNTL_ACC=sts->DAP[i].VERN_CNTL_ACC;
						return true;
					}
					else if(nNew==8) {
						int i=edit-1;
						sts->DAP[i].PRI_ROT_RATE=sts->DAP[2].PRI_ROT_RATE;
						sts->DAP[i].PRI_ATT_DB=sts->DAP[2].PRI_ATT_DB;
						sts->DAP[i].PRI_RATE_DB=sts->DAP[2].PRI_RATE_DB;
						sts->DAP[i].PRI_ROT_PLS=sts->DAP[2].PRI_ROT_PLS;
						sts->DAP[i].PRI_COMP=sts->DAP[2].PRI_COMP;
						sts->DAP[i].PRI_TRAN_PLS=sts->DAP[2].PRI_TRAN_PLS;
						sts->DAP[i].PRI_P_OPTION=sts->DAP[2].PRI_P_OPTION;
						sts->DAP[i].PRI_Y_OPTION=sts->DAP[2].PRI_Y_OPTION;
						sts->DAP[i].ALT_RATE_DB=sts->DAP[2].ALT_RATE_DB;
						sts->DAP[i].ALT_ON_TIME=sts->DAP[2].ALT_ON_TIME;
						sts->DAP[i].ALT_DELAY=sts->DAP[2].ALT_DELAY;
						sts->DAP[i].ALT_JET_OPT=sts->DAP[2].ALT_JET_OPT;
						sts->DAP[i].ALT_JETS=sts->DAP[2].ALT_JETS;
						sts->DAP[i].VERN_ROT_RATE=sts->DAP[2].VERN_ROT_RATE;
						sts->DAP[i].VERN_ATT_DB=sts->DAP[2].VERN_ATT_DB;
						sts->DAP[i].VERN_RATE_DB=sts->DAP[2].VERN_RATE_DB;
						sts->DAP[i].VERN_ROT_PLS=sts->DAP[2].VERN_ROT_PLS;
						sts->DAP[i].VERN_COMP=sts->DAP[2].VERN_COMP;
						sts->DAP[i].VERN_CNTL_ACC=sts->DAP[2].VERN_CNTL_ACC;
						edit=0;
						return true;
					}
					else if(nNew==15 || nNew==35 || nNew==55) {
						if(sts->DAP[convert[nNew]].PRI_P_OPTION<2) {
							sts->DAP[convert[nNew]].PRI_P_OPTION++;
							if(sts->DAPMode[1]==0) {
								if(sts->DAP[sts->DAPMode[0]].PRI_P_OPTION==1) {
									DisableThrusters(AftPitchThrusters, 2);
									UpdateDAP();
								}
								else if(sts->DAP[sts->DAPMode[0]].PRI_P_OPTION==2) {
									EnableThrusters(AftPitchThrusters, 2);
									DisableThrusters(NosePitchThrusters, 2);
									UpdateDAP();
								}
							}
						}
						else {
							sts->DAP[convert[nNew]].PRI_P_OPTION=0;
							if(sts->DAP[sts->DAPMode[0]].PRI_P_OPTION==0) {
								EnableThrusters(NosePitchThrusters, 2);
								UpdateDAP();
							}
						}
						return true;
					}
					else if(nNew==16 || nNew==36 || nNew==56) {
						if(sts->DAP[convert[nNew]].PRI_Y_OPTION<2) {
							sts->DAP[convert[nNew]].PRI_Y_OPTION++;
							if(sts->DAPMode[1]==0) {
								if(sts->DAP[sts->DAPMode[0]].PRI_Y_OPTION==1) {
									DisableThrusters(AftYawThrusters, 2);
									UpdateDAP();
								}
								else if(sts->DAP[sts->DAPMode[0]].PRI_Y_OPTION==2) {
									EnableThrusters(AftYawThrusters, 2);
									DisableThrusters(NoseYawThrusters, 2);
									UpdateDAP();
								}
							}
						}
						else {
							sts->DAP[convert[nNew]].PRI_Y_OPTION=0;
							if(sts->DAP[sts->DAPMode[0]].PRI_Y_OPTION==0) {
								EnableThrusters(NoseYawThrusters, 2);
								UpdateDAP();
							}
						}
						return true;
					}
					else if(nNew==19 || nNew==39 || nNew==59) {
						if(sts->DAP[convert[nNew]].ALT_JET_OPT==0) {
							sts->DAP[convert[nNew]].ALT_JET_OPT=2;
							if(sts->DAPMode[1]==1) {
								if(sts->DAP[sts->DAPMode[0]].ALT_JET_OPT==2) {
									DisableThrusters(NoseRotThrusters, 6);
									EnableThrusters(AftRotThrusters, 6);
									UpdateDAP();
								}
							}
						}
						else {
							sts->DAP[convert[nNew]].ALT_JET_OPT=0;
							if(sts->DAPMode[1]==1) {
								if(sts->DAP[sts->DAPMode[0]].ALT_JET_OPT==0) {
									EnableThrusters(NoseRotThrusters, 6);
									UpdateDAP();
								}
							}
						}
						return true;
					}
					else if(nNew>=10 && nNew<=28 || nNew>=30 && nNew<=48 || nNew>=50 && nNew<=68) {
						item=nNew;
						data=1;
						return true;
					}
					break;
			}
		}
		else if(sts->ops==104 || sts->ops==105 || sts->ops==202 || sts->ops==301 || sts->ops==302) {
			if(nNew>=1 && nNew<=4) {
				sts->OMS=nNew-1;
				return true;
			}
			else if(nNew>=5 && nNew<=21) {
				item=nNew;
				data=1;
				return true;
			}
			else if(nNew==22) {
				LoadManeuver();
				return true;
			}
			else if(nNew==23) {
				bTIMER=true;
				return true;
			}
			else if(nNew==27) {
				if(!sts->TRK) {
					sts->TRK=true;
					sts->MNVR=false;
					sts->ROT=false;
					sts->Yaw=false;
					sts->Pitch=false;
					sts->Roll=false;
					sts->TGT_ID=2;
					if((Eq(sts->BurnAtt.data[YAW], 90.00) || Eq(sts->BurnAtt.data[YAW], -90.00)) && !Eq(sts->BurnAtt.data[PITCH], 0.0)) {
						sts->LVLHOrientationReqd.data[PITCH]=90.0;
						sts->LVLHOrientationReqd.data[YAW]=sts->BurnAtt.data[YAW]-sts->BurnAtt.data[PITCH];
						sts->LVLHOrientationReqd.data[ROLL]=sts->BurnAtt.data[YAW]+sts->BurnAtt.data[ROLL];
					}
					else {
						sts->LVLHOrientationReqd.data[PITCH]=sts->BurnAtt.data[PITCH];
						sts->LVLHOrientationReqd.data[YAW]=sts->BurnAtt.data[YAW];
						sts->LVLHOrientationReqd.data[ROLL]=sts->BurnAtt.data[ROLL];
					}
					/*sts->LVLHRateVector.data[PITCH]=-1.0*cos(RAD*sts->LVLHOrientationReqd.data[ROLL])*cos(RAD*sts->LVLHOrientationReqd.data[YAW]);
					sts->LVLHRateVector.data[ROLL]=1.0*sin(RAD*sts->LVLHOrientationReqd.data[YAW]);
					sts->LVLHRateVector.data[YAW]=-1.0*sin(RAD*sts->LVLHOrientationReqd.data[ROLL])*cos(RAD*sts->LVLHOrientationReqd.data[YAW]);*/
					/*sts->LVLHRateVector.data[PITCH]=-1.0*cos(RAD*sts->BurnAtt.data[YAW]);
					sts->LVLHRateVector.data[ROLL]=1.0*sin(RAD*sts->BurnAtt.data[YAW]);
					sts->LVLHRateVector.data[YAW]=1.0*sin(RAD*sts->BurnAtt.data[PITCH]);*
					sts->AttDeadband=0.05;
					for(int i=0;i<4;i++) sts->START_TIME[i]=MET[i];
				}
				else {
					sts->MNVR=false;
					sts->ROT=false;
					sts->TRK=false;
					sts->SetThrusterGroupLevel(THGROUP_ATT_PITCHUP, 0.0);
					sts->SetThrusterGroupLevel(THGROUP_ATT_PITCHDOWN, 0.0);
					sts->SetThrusterGroupLevel(THGROUP_ATT_YAWLEFT, 0.0);
					sts->SetThrusterGroupLevel(THGROUP_ATT_YAWRIGHT, 0.0);
					sts->SetThrusterGroupLevel(THGROUP_ATT_BANKLEFT, 0.0);
					sts->SetThrusterGroupLevel(THGROUP_ATT_BANKRIGHT, 0.0);
				}
				return true;
			}
			return false;
		}
	}
	else if(change==2) {
		nNew=atoi(Name);
		if(sts->ops==201) {
			if(nNew==20 || nNew==0) {
				spec=nNew;
				InvalidateDisplay();
				return true;
			}
			else return false;
		}
	}
	else if(change==3) {
		if(sts->ops==201) {
			switch(spec) {
				case 0:
					if(item>=1 && item<=4) {
						nNew=atoi(Name);
						if((item==1 && nNew<365) || (item==2 && nNew<24) || (item>2 && nNew<60)) {
							sts->START_TIME[item-1]=nNew;
							return true;
						}
						return false;
					}
					else if(item==5 || item==6) {
						dNew=atof(Name);
						if(dNew>=0.0 && dNew<360.0) {
							if(item==5) sts->MNVR_OPTION.data[ROLL]=dNew;
							else sts->MNVR_OPTION.data[PITCH]=dNew;
							return true;
						}
						return false;
					}
					else if(item==7) {
						dNew=atof(Name);
						if((dNew>=0.0 && dNew<=90.0) || (dNew>=270.0 && dNew<360.0)) {
							sts->MNVR_OPTION.data[YAW]=dNew;
							return true;
						}
						return false;
					}
					if(item==8) {
						nNew=atoi(Name);
						if(nNew==2 || nNew==5) {
							sts->TGT_ID=nNew;
							return true;
						}
						return false;
					}
					if(item==14) {
						nNew=atoi(Name);
						if(nNew>=1 && nNew<=5) {
							sts->BODY_VECT=nNew;
							if(sts->BODY_VECT==1) {
								sts->P=0.0;
								sts->Y=0.0;
							}
							else if(sts->BODY_VECT==2) {
								sts->P=180.0;
								sts->Y=0.0;
							}
							else if(sts->BODY_VECT==3) {
								sts->P=90.0;
								sts->Y=0.0;
							}
							return true;
						}
						return false;
					}
					if(item==15 && sts->BODY_VECT==5) {
						dNew=atof(Name);
						if(dNew>=0.0 && dNew<360.0) {
							sts->P=dNew;
							return true;
						}
						return false;
					}
					if(item==16 && sts->BODY_VECT==5) {
						dNew=atof(Name);
						if((dNew>=0.0 && dNew<=90.0) || (dNew>=270.0 && dNew<360.0)) {
							sts->Y=dNew;
							return true;
						}
						return false;
					}
					if(item==17) {
						dNew=atof(Name);
						if(dNew>=0.0 && dNew<360.0) {
							sts->OM=dNew;
							return true;
						}
						return false;
					}
					return false;
					break;
				case 20:
					if(item==10 || item==30 || item==50) {
						dNew=atof(Name);
						if(dNew>0.0 && dNew<10.0) {
							sts->DAP[convert[item]].PRI_ROT_RATE=dNew;
							if(convert[item]==sts->DAPMode[0] && sts->DAPMode[1]==0) UpdateDAP();
							return true;
						}
						return false;
					}
					else if(item==11 || item==31 || item==51) {
						dNew=atof(Name);
						if(dNew>0.0 && dNew<100.0) {
							sts->DAP[convert[item]].PRI_ATT_DB=dNew;
							if(convert[item]==sts->DAPMode[0] && sts->DAPMode[1]==0) UpdateDAP();
							return true;
						}
						return false;
					}
					else if(item==12 || item==32 || item==52) {
						dNew=atof(Name);
						if(dNew>0.0 && dNew<10.0) {
							sts->DAP[convert[item]].PRI_RATE_DB=dNew;
							if(convert[item]==sts->DAPMode[0] && sts->DAPMode[1]==0) UpdateDAP();
							return true;
						}
						return false;
					}
					else if(item==13 || item==33 || item==53) {
						dNew=atof(Name);
						if(dNew>0.0 && dNew<10.0) {
							sts->DAP[convert[item]].PRI_ROT_PLS=dNew;
							if(convert[item]==sts->DAPMode[0] && sts->DAPMode[1]==0) UpdateDAP();
							return true;
						}
						return false;
					}
					else if(item==14 || item==34 || item==54) {
						dNew=atof(Name);
						if(dNew>0.0 && dNew<1.0) {
							sts->DAP[convert[item]].PRI_COMP=dNew;
							if(convert[item]==sts->DAPMode[0] && sts->DAPMode[1]==0) UpdateDAP();
							return true;
						}
						return false;
					}
					else if(item==17 || item==37 || item==57) {
						dNew=atof(Name);
						if(dNew>0.0 && dNew<10.0) {
							sts->DAP[convert[item]].PRI_ROT_PLS=dNew;
							if(convert[item]==sts->DAPMode[0] && sts->DAPMode[1]==0) UpdateDAP();
							return true;
						}
						return false;
					}
					else if(item==18 || item==38 || item==58) {
						dNew=atof(Name);
						if(dNew>0.0 && dNew<10.0) {
							sts->DAP[convert[item]].ALT_RATE_DB=dNew;
							if(convert[item]==sts->DAPMode[0] && sts->DAPMode[1]==1) UpdateDAP();
							return true;
						}
						return false;
					}
					else if(item==20 || item==40 || item==60) {
						nNew=atoi(Name);
						if(nNew>=1 && nNew<=3) {
							sts->DAP[convert[item]].ALT_JETS=nNew;
							if(convert[item]==sts->DAPMode[0] && sts->DAPMode[1]==1) UpdateDAP();
							return true;
						}
						return false;
					}
					else if(item==21 || item==41 || item==61) {
						dNew=atof(Name);
						if(dNew>0.0 && dNew<10.0) {
							sts->DAP[convert[item]].ALT_ON_TIME=dNew;
							if(convert[item]==sts->DAPMode[0] && sts->DAPMode[1]==1) UpdateDAP();
							return true;
						}
						return false;
					}
					else if(item==22 || item==42 || item==62) {
						dNew=atof(Name);
						if(dNew>0.0 && dNew<100.0) {
							sts->DAP[convert[item]].ALT_DELAY=dNew;
							if(convert[item]==sts->DAPMode[0] && sts->DAPMode[1]==1) UpdateDAP();
							return true;
						}
						return false;
					}
					else if(item==23 || item==43 || item==63) {
						dNew=atof(Name);
						if(dNew>0.0 && dNew<10.0) {
							sts->DAP[convert[item]].VERN_ROT_RATE=dNew;
							if(convert[item]==sts->DAPMode[0] && sts->DAPMode[1]==2) UpdateDAP();
							return true;
						}
						return false;
					}
					else if(item==24 || item==44 || item==64) {
						dNew=atof(Name);
						if(dNew>0.0 && dNew<100.0) {
							sts->DAP[convert[item]].VERN_ATT_DB=dNew;
							if(convert[item]==sts->DAPMode[0] && sts->DAPMode[1]==2) UpdateDAP();
							return true;
						}
						return false;
					}
					else if(item==25 || item==45 || item==65) {
						dNew=atof(Name);
						if(dNew>0.0 && dNew<1.0) {
							sts->DAP[convert[item]].VERN_RATE_DB=dNew;
							if(convert[item]==sts->DAPMode[0] && sts->DAPMode[1]==2) UpdateDAP();
							return true;
						}
						return false;
					}
					else if(item==26 || item==46 || item==66) {
						dNew=atof(Name);
						if(dNew>0.0 && dNew<10.0) {
							sts->DAP[convert[item]].VERN_ROT_PLS=dNew;
							if(convert[item]==sts->DAPMode[0] && sts->DAPMode[1]==2) UpdateDAP();
							return true;
						}
						return false;
					}
					else if(item==27 || item==47 || item==67) {
						dNew=atof(Name);
						if(dNew>0.0 && dNew<10.0) {
							sts->DAP[convert[item]].VERN_COMP=dNew;
							if(convert[item]==sts->DAPMode[0] && sts->DAPMode[1]==2) UpdateDAP();
							return true;
						}
						return false;
					}
					else if(item==28 || item==48 || item==68) {
						nNew=atoi(Name);
						if(nNew>=0 && nNew<=9) {
							sts->DAP[convert[item]].VERN_CNTL_ACC=nNew;
							if(convert[item]==sts->DAPMode[0] && sts->DAPMode[1]==2)
								UpdateDAP();
							return true;
						}
						return false;
					}
					return false;
					break;
			}
		}
		else if(sts->ops==104 || sts->ops==105 || sts->ops==202 || sts->ops==301 || sts->ops==302) {
			if(item==5) {
				sts->TV_ROLL=atoi(Name);
				return true;
			}
			else if(item>=6 && item<=8) {
				dNew=atof(Name);
				if(fabs(dNew)<=6.0) 
				{
					sts->Trim.data[item-6]=dNew;
					return true;
				}
			}
			else if(item==9) {
				//dNew=atof(Name);
				sts->WT=atof(Name);
				return true;
			}
			else if(item>=10 && item<=13) {
				if(item==13) dNew=atof(Name);
				else dNew=atoi(Name);
				//sprintf(oapiDebugString(), "%f", dNew);
				if((item==10 && dNew<365.0) || (item==11 && dNew<24.0) || (item>11 && dNew<60.0)) {
					sts->TIG[item-10]=dNew;
					return true;
				}
				return false;
			}
			else if(item==14 && sts->ops!=202) {
				sts->C1=atof(Name);
				return true;
			}
			else if(item==15 && sts->ops!=202) {
				dNew=atof(Name);
				if(fabs(dNew)<10.0) {
					sts->C2=dNew;
					return true;
				}
			}
			else if(item==16 && sts->ops!=202) {
				sts->HT=atof(Name);
				return true;
			}
			else if(item==17 && sts->ops!=202) {
				sts->ThetaT=atof(Name);
				return true;
			}
			else if(item>=19 && item<=21) {
				dNew=atof(Name);
				sts->PEG7.data[item-19]=dNew;
				return true;
			}
			return false;
		}
		item=0;
		return true;
	}
	return false;
}

bool cbChangeOps(void *id, char *str, void *data)
{
	return (((CRT *)data)->Input(0, str));
}
bool cbItem(void *id, char *str, void *data)
{
	return (((CRT *)data)->Input(1, str));
}
bool cbSpec(void *id, char *str, void *data)
{
	return (((CRT *)data)->Input(2, str));
}
bool cbItemData(void *id, char *str, void *data)
{
	//sprintf(oapiDebugString(), "DATA INPUT");
	return (((CRT *)data)->Input(3, str));
}
bool cbSpecData(void *id, char *str, void *data)
{
	//sprintf(oapiDebugString(), "DATA INPUT");
	return (((CRT *)data)->Input(4, str));
}*/


void CRT::DrawCommonHeader(HDC hdc)
{
	char cbuf[200];
	char cspecbuf[4];
	char cdispbuf[4];
	char cUplink[3];
	unsigned short usDay, usHour, usMinute, usSecond;
	strcpy_s(cUplink, "  ");
	strcpy_s(cspecbuf, "   ");
	strcpy_s(cdispbuf, "   ");

	if(spec >= 0)
	{
		sprintf_s(cspecbuf, 4, "%03d", spec);
	}


	if(display >= 0)
	{
		sprintf_s(cdispbuf, 4, "%03d", display);
	}

	//this->SelectDefaultFont(hdc, 1);
	HGDIOBJ hOldFont = SelectObject(hdc, hCRTFont);

	sts->GetGPCMET(usGPCDriver, usDay, usHour, usMinute, usSecond);
	
	//Todo: GPC count their own MET independent of the MTU
	sprintf(cbuf,"%03d1/%03s/%3s    %14s  %2s  %1d %03d/%02d:%02d:%02d", 
		sts->GetGPCMajorMode(),
		cspecbuf, 
		cdispbuf, 
		cDispTitle, 
		cUplink, 
		usGPCDriver, 
		usDay, usHour, usMinute, usSecond);

	
	CRTTextOut(hdc, 1, 1, cbuf);

	SelectObject(hdc, hOldFont);
}

void CRT::SetDisplayTitle(const char *pszTitle)
{
	strncpy(cDispTitle, pszTitle, 14);
}

void CRT::GNCSYSSUMM1(HDC hdc)
{
	SetDisplayTitle("GNC SYS SUMM 1");
	DrawCommonHeader(hdc);

	CRTLine(hdc, 0, 4, 12, 4);
	CRTLine(hdc, 0, 9, 12, 9);
	CRTLine(hdc, 0, 14, 12, 14);

	CRTLine(hdc, 14, 4, 32,4);

	CRTLine(hdc, 16, 16, 28, 16);

	CRTLine(hdc, 34, 4, 50, 4);
	CRTLine(hdc, 34, 10, 48, 10);
	CRTLine(hdc, 34, 14, 48, 14);

	CRTLine(hdc, 4, 2, 4, 19);
	CRTLine(hdc, 8, 2, 8, 19);

	CRTTextOut(hdc, 1, 3, "RCS");
	CRTTextOut(hdc, 6, 3, "JET");
	CRTTextOut(hdc, 9, 3, "ISOL");
	CRTTextOut(hdc, 1, 4, "MANF");
	CRTTextOut(hdc, 5, 4, "FAIL");
	CRTTextOut(hdc, 9, 4, "VLV");

	CRTTextOut(hdc, 3, 5, "F1");
	CRTTextOut(hdc, 4, 6, "2");
	CRTTextOut(hdc, 4, 7, "3");
	CRTTextOut(hdc, 4, 8, "4");
	CRTTextOut(hdc, 4, 9, "5");

	CRTTextOut(hdc, 3, 10, "L1");
	CRTTextOut(hdc, 4, 11, "2");
	CRTTextOut(hdc, 4, 12, "3");
	CRTTextOut(hdc, 4, 13, "4");
	CRTTextOut(hdc, 4, 14, "5");

	CRTTextOut(hdc, 3, 15, "R1");
	CRTTextOut(hdc, 4, 16, "2");
	CRTTextOut(hdc, 4, 17, "3");
	CRTTextOut(hdc, 4, 18, "4");
	CRTTextOut(hdc, 4, 19, "5");

	CRTTextOut(hdc, 15, 4, "SURF");
	CRTTextOut(hdc, 24, 4, "POS");
	CRTTextOut(hdc, 30, 4, "MOM");

	CRTTextOut(hdc, 15, 5, "L OB");
	CRTTextOut(hdc, 17, 6, "IB");
	CRTTextOut(hdc, 15, 7, "R IB");
	CRTTextOut(hdc, 17, 8, "OB");

	CRTTextOut(hdc, 15, 9, "AIL");
	CRTTextOut(hdc, 15, 10, "RUD");
	CRTTextOut(hdc, 15, 11, "SPD BRK");
	CRTTextOut(hdc, 15, 12, "BDY FLP");
	
	CRTTextOut(hdc, 17, 16, "CNTLR  1 2 3");
	CRTTextOut(hdc, 17, 17, "RHC  L");
	CRTTextOut(hdc, 22, 18, "R");
	CRTTextOut(hdc, 22, 19, "A");
	CRTTextOut(hdc, 17, 20, "THC  L");
	CRTTextOut(hdc, 22, 21, "A");
	CRTTextOut(hdc, 22, 22, "SBTC L");
	CRTTextOut(hdc, 22, 23, "R");

	CRTTextOut(hdc, 35, 4, "DPS");
	CRTTextOut(hdc, 42, 4, "1 2 3 4 5");

	CRTTextOut(hdc, 38, 5, "GPC");
	CRTTextOut(hdc, 35, 6, "MDM FF");
	CRTTextOut(hdc, 39, 7, "FA");

	CRTTextOut(hdc, 35, 10, "FCS CH 1 2 3 4");
	CRTTextOut(hdc, 35, 14, "NAV    1 2 3 4");
	CRTTextOut(hdc, 37, 15, "IMU");
	CRTTextOut(hdc, 37, 16, "ACC");
	CRTTextOut(hdc, 37, 17, "RGA");
	CRTTextOut(hdc, 37, 18, "TAC");
	CRTTextOut(hdc, 37, 19, "MLS");
	CRTTextOut(hdc, 37, 20, "ADTA");
	
}

bool CRT::CRTTextOut(HDC hdc, short sCol, short sLine, char *text)
{
	float charW = static_cast<float>(W)/50.0f;
	float charH = static_cast<float>(H)/25.0f;

	if(sCol < 1 || sCol > 50 || sLine < 1 || sLine > 25)
	{
		return false;
	}

	TextOut(hdc, static_cast<int>(charW * (sCol - 1)), static_cast<int>(charH * (sLine - 1)), text, strlen(text));

	return true;
}

bool CRT::CRTLine(HDC hdc, short sX1, short sY1, short sX2, short sY2)
{
	float charW = static_cast<float>(W)/50.0f;
	float charH = static_cast<float>(H)/25.0f;

	int iX1 = (int)(charW * sX1);
	int iY1 = (int)(charH * sY1);
	int iX2 = (int)(charW * sX2);
	int iY2 = (int)(charH * sY2);

	MoveToEx(hdc, iX1, iY2, NULL);
	LineTo(hdc, iX2, iY2);
	return true;
}

void CRT::RecallCriticalFormat(int iFormatCode)
{
	switch (iFormatCode)
	{
	case 0:
	return;
	}
}

void CRT::GPCMEMORY(HDC hdc)
{
	SetDisplayTitle("GPC MEMORY");
	DrawCommonHeader(hdc);

	CRTLine(hdc, 16, 3, 16, 22);
	CRTLine(hdc, 10, 7, 10, 22);
	CRTLine(hdc, 1, 22, 16, 22);
	CRTLine(hdc, 16, 14, 50, 14);
	CRTLine(hdc, 16, 20, 50, 20);
	CRTLine(hdc, 19, 14, 19, 24);

	CRTTextOut(hdc, 1, 3, "MEM/BUS CONFIG");
	CRTTextOut(hdc, 1, 4, "1 CONFIG");
	CRTTextOut(hdc, 1, 5, "2 GPC");
	CRTTextOut(hdc, 2, 7, "STRING 1");
	CRTTextOut(hdc, 9, 8, "2");
	CRTTextOut(hdc, 9, 9, "3");
	CRTTextOut(hdc, 9, 10, "4");
	CRTTextOut(hdc, 4, 11, "PL 1/2");
	CRTTextOut(hdc, 5, 13, "CRT 1");
	CRTTextOut(hdc, 9, 14, "2");
	CRTTextOut(hdc, 9, 15, "3");
	CRTTextOut(hdc, 9, 16, "4");
	CRTTextOut(hdc, 2, 18, "LAUNCH 1");
	CRTTextOut(hdc, 9, 19, "2");
	CRTTextOut(hdc, 6, 20, "MM 1");
	CRTTextOut(hdc, 9, 21, "2");

	

	CRTTextOut(hdc, 18, 3, "READ/WRITE");
	CRTTextOut(hdc, 19, 4, "DATA 20");
	CRTTextOut(hdc, 29, 4, "BIT SET 22");
	CRTTextOut(hdc, 42, 4, "SEQ ID 24");
	CRTTextOut(hdc, 19, 5, "CODE 21");
	CRTTextOut(hdc, 29, 5, "BIT RST 23");
	CRTTextOut(hdc, 42, 5, "WRITE 25");
	CRTTextOut(hdc, 19, 6, "26 ENG UNITS");


}
