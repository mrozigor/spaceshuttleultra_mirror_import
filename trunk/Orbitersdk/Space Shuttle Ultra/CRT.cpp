#define STRICT
#define ORBITER_MODULE
#include "windows.h"
#include "orbitersdk.h"
#include "CRT.h"

#define RED RGB(255, 0, 0)
#define GREEN RGB(0, 255, 0)
#define YELLOW RGB(255, 255, 0)
#define DARK_YELLOW RGB(128, 128, 0)
#define DARK_GREEN RGB(0, 128, 0)
#define WHITE RGB(255, 255, 255)
#define BLUE RGB(0, 0, 255)
#define GRAY RGB(160, 160, 160)
#define BRIGHTERGRAY RGB(200, 200, 200)
#define BLACK RGB(0, 0, 0)

// ==============================================================
// Global variables

int g_MFDmode; // identifier for new MFD mode
CRT *mfd;
CRT::SavePrm CRT::saveprm;

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
	mfd = NULL;
}

DLLCLBK void ExitModule (HINSTANCE hDLL)
{
	// Unregister the custom MFD mode when the module is unloaded
	oapiUnregisterMFDMode (g_MFDmode);
}

// ==============================================================
// MFD class implementation

CRT::CRT (DWORD w, DWORD h, VESSEL *v)
: MFD (w, h, v)
{
	int i;
	vessel=v;
	width=w;
	height=h;

	if(strcmp(pV->GetClassName(), "Atlantis")==0) {
		sts=(Atlantis *)pV;
		id=sts->last_mfd;
		mode=0;
	}
	else {
		id=10;
		mode=10001;
	}

	spec=0;
	mode=0;
	item=0;
	data=0;
	display=2;
	if(saveprm.bValid==false) {
		saveprm.mode=mode;
		saveprm.display=display;
		saveprm.spec=spec;
		saveprm.bValid=true;
	}
	bTIMER=false;
	//sprintf(oapiDebugString(), "Constructor %f", oapiRand());
	vessel->GetPMI(PMI);

	for(i=0;i<4;i++) {
		Launch_time[i]=0;
		MET_Add[i]=0;
		//START_TIME[i]=0;
	}
	mfd=this;
	return;
}

CRT::~CRT ()
{
	return;
}

void CRT::Update (HDC hDC)
{
	char cbuf[255];
	/*double dNum;
	int nPos, nLoc, EngConvert[3]={1, 0, 2};*/
	//sprintf(oapiDebugString(), "%i", id);
	if(data!=0) Data(1-data);

	Simtime=oapiGetSimTime();
	//vessel->GetElements(NULL, el, &oparam, 0, FRAME_EQU);
	//dT=Simtime-Simtime_last;
	/*vessel->GetGlobalOrientation(InertialOrientationRad);
	vessel->GetAngularVel(AngularVelocity);
	vessel->GetGlobalPos(GVesselPos);
	for(nPos=0;nPos<3;nPos++) {
		InertialOrientation.data[nPos]=DEG*InertialOrientationRad.data[nPos];
		if(InertialOrientation.data[nPos]<0.0) InertialOrientation.data[nPos]+=360.0;
	}*/
	/*if(MNVR || TRK || ROT) {
		AttControl();
		//InvalidateDisplay();
	}*/
	//sprintf(oapiDebugString(), "MODE %d OPS %d SPEC %d", mode, ops, spec);

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
		if(sts->ops==201) {
			switch(spec) {
				case 0:
					UNIVPTG(hDC);
					break;
				case 20:
					DAP_CONFIG(hDC);
					break;
			}
		}
		else if(sts->ops==104 || sts->ops==105 || sts->ops==106 || sts->ops==202 || sts->ops==301 || sts->ops==302 || sts->ops==303) {
			MNVR(hDC);
		}
		else {
			sprintf(cbuf,"OPS %d", sts->ops);
			TextOut(hDC, 0, 5, cbuf, strlen(cbuf));
		}
	}
	/*if(!sts->GroundContact()) {
		MET[0]=Simtime/86400;
		MET[1]=(Simtime-86400*MET[0])/3600;
		MET[2]=(Simtime-86400*MET[0]-3600*MET[1])/60;
		MET[3]=Simtime-86400*MET[0]-3600*MET[1]-60*MET[2];
		MET[0]+=MET_Add[0]-Launch_time[0];
		MET[1]+=MET_Add[1]-Launch_time[1];
		MET[2]+=MET_Add[2]-Launch_time[2];
		MET[3]+=MET_Add[3]-Launch_time[3];
		if(MET[3]>=60) {
			MET[3]-=60;
			MET[2]++;
		}
		if(MET[2]>=60) {
			MET[2]-=60;
			MET[1]++;
		}
		if(MET[1]>=24) {
			MET[1]-=24;
			MET[0]++;
		}
		if(landed) {
			landed=false;
			Launch_time[0]=Simtime/86400;
			Launch_time[1]=(Simtime-86400*Launch_time[0])/3600;
			Launch_time[2]=(Simtime-86400*Launch_time[0]-3600*Launch_time[1])/60;
			Launch_time[3]=(Simtime-86400*Launch_time[0]-3600*Launch_time[1]-60*Launch_time[2])/60;
		}
	}
	else {
		if(!landed) {
			landed=true;
			MET[0]=0;
			MET[1]=0;
			MET[2]=0;
			MET[3]=0;
		}
	}*/
	//sprintf(oapiDebugString(), "%f", Simtime-Simtime_last);
	Simtime_last=Simtime;
}

void CRT::OMSMPS(HDC hDC)
{
	int nPos, nLoc, EngConvert[3]={1, 0, 2};
	double dNum;
	char cbuf[255];

	HBRUSH GreenBrush=CreateSolidBrush(GREEN);
	HBRUSH WhiteBrush=CreateSolidBrush(WHITE);
	HBRUSH BlackBrush=CreateSolidBrush(BLACK);
	HPEN WhitePen=CreatePen(PS_SOLID, 0, WHITE);
	HPEN GreenPen=CreatePen(PS_SOLID, 0, GREEN);
	SetTextColor(hDC, GREEN);
	SelectObject(hDC, GreenPen);
	SelectObject(hDC, BlackBrush);
	TextOut(hDC, 10, 0, "OMS", 3);
	TextOut(hDC, 150, 0, "MPS", 3);
	MoveToEx(hDC, 80, 6, NULL);
	LineTo(hDC, 80, 250);
	MoveToEx(hDC, 1, 6, NULL);
	LineTo(hDC, 8, 6);
	MoveToEx(hDC, 32, 6, NULL);
	LineTo(hDC, 148, 6);
	MoveToEx(hDC, 172, 6, NULL);
	LineTo(hDC, 255, 6);
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
	Rectangle (hDC, 102, 24, 128, 40);
	Rectangle (hDC, 155, 24, 181, 40);
	Rectangle (hDC, 190, 19, 216, 35);
	Rectangle (hDC, 225, 24, 251, 40);
	//Reg Press
	Rectangle (hDC, 102, 92, 128, 108);
	Rectangle (hDC, 155, 92, 181, 108);
	Rectangle (hDC, 190, 87, 216, 103);
	Rectangle (hDC, 225, 92, 251, 108);
	//Engine Pc
	TextOut(hDC, 159, 152, "L/2", 3);
	TextOut(hDC, 194, 147, "C/1", 3);
	TextOut(hDC, 229, 152, "R/3", 3);
	Rectangle (hDC, 155, 167, 181, 183);
	Rectangle (hDC, 190, 162, 216, 178);
	Rectangle (hDC, 225, 167, 251, 183);
	SelectObject(hDC, WhitePen);
	//OMS
	TextOut(hDC, 0, 50, "He", 2);
	TextOut(hDC, 0, 60, "TK", 2);
	TextOut(hDC, 3, 70, "P", 1);
	Rectangle (hDC, 19, 50, 34, 90);
	Rectangle (hDC, 54, 50, 69, 90);
	TextOut(hDC, 0, 115, "N2", 2);
	TextOut(hDC, 0, 125, "TK", 2);
	TextOut(hDC, 3, 135, "P", 1);
	Rectangle (hDC, 19, 115, 34, 155);
	Rectangle (hDC, 54, 115, 69, 155);
	TextOut(hDC, 23, 155, "L", 1);
	TextOut(hDC, 58, 155, "R", 1);
	TextOut(hDC, 0, 200, "Pc", 2);
	TextOut(hDC, 3, 220, "%", 1);
	Rectangle (hDC, 19, 187, 34, 250);
	Rectangle (hDC, 54, 187, 69, 250);
	MoveToEx(hDC, 34, 212, NULL);
	LineTo(hDC, 40, 212);
	MoveToEx(hDC, 69, 212, NULL);
	LineTo(hDC, 75, 212);
	//MPS
	TextOut(hDC, 80, 42, "TANK", 4);
	Rectangle (hDC, 108, 42, 123, 82);
	TextOut(hDC, 140, 42, "He", 2);
	TextOut(hDC, 132, 52, "TANK", 4);
	TextOut(hDC, 140, 62, "P", 1);
	Rectangle (hDC, 161, 42, 176, 82);
	Rectangle (hDC, 196, 42, 211, 82);
	Rectangle (hDC, 230, 42, 245, 82);
	TextOut(hDC, 80, 110, "REG", 3);
	Rectangle (hDC, 108, 110, 123, 145);
	TextOut(hDC, 135, 110, "He", 2);
	TextOut(hDC, 125, 120, "REG A", 5);
	TextOut(hDC, 138, 130, "P", 1);
	Rectangle (hDC, 161, 110, 176, 145);
	Rectangle (hDC, 196, 110, 211, 145);
	Rectangle (hDC, 230, 110, 245, 145);
	TextOut(hDC, 180, 188, "Pc", 2);
	TextOut(hDC, 184, 198, "%", 1);
	TextOut(hDC, 215, 188, "Pc", 2);
	TextOut(hDC, 219, 198, "%", 1);
	Rectangle (hDC, 161, 187, 176, 250);
	Rectangle (hDC, 196, 187, 211, 250);
	Rectangle (hDC, 231, 187, 246, 250);
	MoveToEx(hDC, 176, 190, NULL);
	LineTo(hDC, 182, 190);
	MoveToEx(hDC, 211, 190, NULL);
	LineTo(hDC, 217, 190);
	MoveToEx(hDC, 245, 190, NULL);
	LineTo(hDC, 251, 190);
	MoveToEx(hDC, 176, 211, NULL);
	LineTo(hDC, 182, 211);
	MoveToEx(hDC, 211, 211, NULL);
	LineTo(hDC, 217, 211);
	MoveToEx(hDC, 245, 211, NULL);
	LineTo(hDC, 251, 211);
	SelectObject(hDC, GreenBrush);
	//OMS
	for(nPos=0;nPos<2;nPos++) {
		dNum=sts->GetPropellantMass(sts->oms_helium_tank[nPos])*228.6;
		sprintf(cbuf, "%.0f", dNum);
		TextOut(hDC, 14+35*nPos, 31, cbuf, strlen(cbuf));
		Rectangle (hDC, 19+35*nPos, 90-0.008*dNum, 34+35*nPos, 90);
		dNum=100.0*sts->GetThrusterLevel(sts->th_oms[nPos]);
		sprintf(cbuf, "%.0f", dNum);
		TextOut(hDC, 15+35*nPos, 169, cbuf, strlen(cbuf));
		if(dNum>1) {
			Rectangle(hDC, 19+35*nPos, 187, 34+35*nPos, 250);
		}
	}
	//MPS
	for(nPos=0;nPos<3;nPos++) {
		nLoc=35*EngConvert[nPos];
		if (sts->status<=2) dNum=(int)sts->MaxThrust*sts->GetThrusterLevel(sts->th_main[nPos]);
		else dNum=0.0;
		sprintf(cbuf, "%.0f", dNum);
		if(dNum>1) {
			Rectangle(hDC, 161+nLoc, 250-0.5769*dNum, 176+nLoc, 250);
		}
		if(EngConvert[nPos]!=1) TextOut(hDC, 157+nLoc, 169, cbuf, strlen(cbuf));
		else TextOut(hDC, 157+nLoc, 164, cbuf, strlen(cbuf));
	}
	//SelectObject(hDC, GreenPen);
	//SelectObject(hDC, BlackBrush);
	DeleteObject(GreenBrush);
	DeleteObject(WhiteBrush);
	DeleteObject(BlackBrush);
	DeleteObject(WhitePen);
	DeleteObject(GreenPen);
}
void CRT::SPI(HDC hDC)
{
	int nPos;
	double dNum;
	char cbuf[255];
	//Elevons
	SetTextColor(hDC, GREEN);
	TextOut(hDC, 0, 5, "ELEVONS", 7);
	TextOut(hDC, 12, 15, "DEG", 3);
	Rectangle (hDC, 10, 40, 21, 206);
	Rectangle (hDC, 55, 40, 66, 206);
	for(nPos=55;nPos<=205;nPos+=30) {
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
	for(nPos=40;nPos<=205;nPos+=30) {
		MoveToEx(hDC, 7, nPos, NULL);
		LineTo(hDC, 10, nPos);
		MoveToEx(hDC, 23, nPos, NULL);
		LineTo(hDC, 20, nPos);
		MoveToEx(hDC, 52, nPos, NULL);
		LineTo(hDC, 55, nPos);
		MoveToEx(hDC, 68, nPos, NULL);
		LineTo(hDC, 65, nPos);
	}
	if(sts->r2d2->bHydraulicPressure) dNum=sts->GetControlSurfaceLevel(AIRCTRL_ELEVATOR);
	else dNum=0.0;
	if(dNum>=0) dNum=dNum*-35;
	else dNum=dNum*-20;
	nPos=145+(3*dNum);
	MoveToEx(hDC, 6, nPos, NULL);
	LineTo (hDC, 14, nPos-4);
	LineTo (hDC, 14, nPos+4);
	LineTo (hDC, 6, nPos);
	MoveToEx(hDC, 24, nPos, NULL);
	LineTo (hDC, 16, nPos-4);
	LineTo (hDC, 16, nPos+4);
	LineTo (hDC, 24, nPos);
	MoveToEx(hDC, 51, nPos, NULL);
	LineTo (hDC, 59, nPos-4);
	LineTo (hDC, 59, nPos+4);
	LineTo (hDC, 51, nPos);
	MoveToEx(hDC, 69, nPos, NULL);
	LineTo (hDC, 61, nPos-4);
	LineTo (hDC, 61, nPos+4);
	LineTo (hDC, 69, nPos);
	//Body flap
	TextOut(hDC, 60, 5, "BODYFLAP", 8);
	TextOut(hDC, 80, 15, "%", 1);
	Rectangle (hDC, 85, 40, 96, 206);
	for(nPos=55;nPos<=205;nPos+=33) {
		MoveToEx(hDC, 98, nPos, NULL);
		LineTo(hDC, 95, nPos);
	}
	for(nPos=40;nPos<=205;nPos+=33) {
		MoveToEx(hDC, 101, nPos, NULL);
		LineTo(hDC, 95, nPos);
		sprintf(cbuf,"%d", 20*((nPos-40)/33));
		TextOut(hDC, 102, nPos-7, cbuf, strlen(cbuf));
	}
	MoveToEx(hDC, 95, 98, NULL);
	LineTo (hDC, 103, 94);
	LineTo (hDC, 103, 102);
	LineTo (hDC, 95, 98);
	if(sts->r2d2->bHydraulicPressure)dNum=sts->GetControlSurfaceLevel(AIRCTRL_ELEVATORTRIM);
	else dNum=0.0;
	if(dNum>=0) dNum=dNum*-35;
	else dNum=dNum*-65;
	nPos=98+(1.65*dNum);
	MoveToEx(hDC, 99, nPos, NULL);
	LineTo (hDC, 91, nPos-4);
	LineTo (hDC, 91, nPos+4);
	LineTo (hDC, 99, nPos);
	//Rudder
	TextOut(hDC, 160, 25, "RUDDER-DEG", 10);
	Rectangle (hDC, 130, 60, 251, 71);
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
	if(sts->r2d2->bHydraulicPressure) dNum=sts->GetControlSurfaceLevel(AIRCTRL_RUDDER);
	else dNum=0.0;
	nPos=190+dNum*-54.2;
	MoveToEx(hDC, nPos, 64, NULL);
	LineTo (hDC, nPos-4, 56);
	LineTo (hDC, nPos+4, 56);
	LineTo (hDC, nPos, 64);
	//Aileron
	TextOut(hDC, 160, 100, "AILERON-DEG", 11);
	Rectangle (hDC, 130, 135, 251, 146);
	TextOut(hDC, 123, 115, "5", 1);
	TextOut(hDC, 187, 115, "0", 1);
	TextOut(hDC, 245, 115, "5", 1);
	for(nPos=130;nPos<=250;nPos+=12) {
		MoveToEx(hDC, nPos, 129, NULL);
		LineTo(hDC, nPos, 135);
	}
	for(nPos=136;nPos<=250;nPos+=12) {
		MoveToEx(hDC, nPos, 132, NULL);
		LineTo(hDC, nPos, 135);
	}
	if(sts->r2d2->bHydraulicPressure) dNum=sts->GetControlSurfaceLevel(AIRCTRL_AILERON);
	else dNum=0.0;
	dNum=dNum*10.0;
	if(dNum>5.0) dNum=5.0;
	else if(dNum < -5.0) dNum= -5.0;
	nPos=190+12*dNum;
	MoveToEx(hDC, nPos, 139, NULL);
	LineTo (hDC, nPos-4, 131);
	LineTo (hDC, nPos+4, 131);
	LineTo (hDC, nPos, 139);
}

void CRT::APUHYD(HDC hDC)
{
	int nPos;
	double dNum;
	char cbuf[255];

	HBRUSH GreenBrush=CreateSolidBrush(GREEN);
	HBRUSH BlackBrush=CreateSolidBrush(BLACK);
	HPEN GreenPen=CreatePen(PS_SOLID, 0, GREEN);
	SelectObject(hDC, GreenPen);
	SelectObject(hDC, BlackBrush);
	SetTextColor(hDC, WHITE);
	TextOut(hDC, 0, 60, "FUEL", 4);
	TextOut(hDC, 0, 70, "QTY", 3);
	TextOut(hDC, 0, 80, "%", 1);
	TextOut(hDC, 37, 15, "1", 1);
	TextOut(hDC, 67, 15, "2", 1);
	TextOut(hDC, 97, 15, "3", 1);
	TextOut(hDC, 39, 165, "1", 1);
	TextOut(hDC, 74, 165, "2", 1);
	TextOut(hDC, 109, 165, "3", 1);
	TextOut(hDC, 0, 120, "H2O", 3);
	TextOut(hDC, 0, 130, "QTY", 3);
	TextOut(hDC, 0, 140, "%", 1);
	TextOut(hDC, 0, 210, "QTY", 3);
	TextOut(hDC, 0, 220, "%", 1);
	TextOut(hDC, 125, 215, "PRESS", 5);
	TextOut(hDC, 132, 120, "OIL", 3);
	TextOut(hDC, 113, 130, "IN TEMP", 7);
	TextOut(hDC, 139, 140, "F", 1);
	TextOut(hDC, 134, 60, "FUEL", 4);
	TextOut(hDC, 144, 70, "P", 1);
	SetTextColor(hDC, GREEN);
	TextOut(hDC, 127, 5, "APU", 3);
	TextOut(hDC, 115, 156, "HYDRAULIC", 9);
	//Fuel Qty
	Rectangle (hDC, 27, 30, 53, 46);
	Rectangle (hDC, 57, 30, 83, 46);
	Rectangle (hDC, 87, 30, 113, 46);
	Rectangle (hDC, 29, 50, 50, 91);
	Rectangle (hDC, 59, 50, 80, 91);
	Rectangle (hDC, 89, 50, 110, 91);
	//H2O Qty
	Rectangle (hDC, 27, 95, 53, 111);
	Rectangle (hDC, 57, 95, 83, 111);
	Rectangle (hDC, 87, 95, 113, 111);
	Rectangle (hDC, 29, 115, 50, 156);
	Rectangle (hDC, 59, 115, 80, 156);
	Rectangle (hDC, 89, 115, 110, 156);
	//Hydraulic Qty
	Rectangle (hDC, 27, 180, 53, 196);
	Rectangle (hDC, 57, 180, 83, 196);
	Rectangle (hDC, 87, 180, 113, 196);
	Rectangle (hDC, 29, 200, 50, 241);
	Rectangle (hDC, 59, 200, 80, 241);
	Rectangle (hDC, 89, 200, 110, 241);
	//Hydraulic Press
	Rectangle (hDC, 158, 180, 187, 196);
	Rectangle (hDC, 191, 180, 220, 196);
	Rectangle (hDC, 224, 180, 253, 196);
	Rectangle (hDC, 162, 200, 183, 241);
	Rectangle (hDC, 195, 200, 216, 241);
	Rectangle (hDC, 228, 200, 249, 241);
	MoveToEx(hDC, 189, 210, NULL);
	LineTo(hDC, 182, 210);
	MoveToEx(hDC, 222, 210, NULL);
	LineTo(hDC, 215, 210);
	MoveToEx(hDC, 255, 210, NULL);
	LineTo(hDC, 248, 210);
	MoveToEx(hDC, 189, 233, NULL);
	LineTo(hDC, 182, 233);
	MoveToEx(hDC, 222, 233, NULL);
	LineTo(hDC, 215, 233);
	MoveToEx(hDC, 255, 233, NULL);
	LineTo(hDC, 248, 233);
	//Oil In Temp
	Rectangle (hDC, 158, 95, 187, 111);
	Rectangle (hDC, 191, 95, 220, 111);
	Rectangle (hDC, 224, 95, 253, 111);
	Rectangle (hDC, 162, 115, 183, 156);
	Rectangle (hDC, 195, 115, 216, 156);
	Rectangle (hDC, 228, 115, 249, 156);
	//Fuel P
	Rectangle (hDC, 158, 30, 187, 46);
	Rectangle (hDC, 191, 30, 220, 46);
	Rectangle (hDC, 224, 30, 253, 46);
	Rectangle (hDC, 162, 50, 183, 91);
	Rectangle (hDC, 195, 50, 216, 91);
	Rectangle (hDC, 228, 50, 249, 91);
	SelectObject(hDC, GreenBrush);
	//Hydraulic Qty
	Rectangle (hDC, 29, 202, 50, 241);
	Rectangle (hDC, 59, 202, 80, 241);
	Rectangle (hDC, 89, 202, 110, 241);
	TextOut(hDC, 28, 182, "100", 3);
	TextOut(hDC, 58, 182, "100", 3);
	TextOut(hDC, 88, 182, "100", 3);
	for(nPos=0;nPos<3;nPos++) {
		//Fuel Qty
		dNum=100*(sts->GetPropellantMass(sts->apu_tank[nPos])/APU_FUEL_TANK_MASS);
		sprintf(cbuf, "%.0f", dNum);
		TextOut(hDC, 28+30*nPos, 32, cbuf, strlen(cbuf));
		if(dNum>=1) {
			Rectangle(hDC, 29+30*nPos, 91-0.38*dNum, 50+30*nPos, 91);
		}
		//Hydraulic Press
		sprintf(cbuf, "%d", sts->r2d2->Hydraulic_Press[nPos]);
		TextOut(hDC, 159+33*nPos, 182, cbuf, strlen(cbuf));
		if(sts->r2d2->Hydraulic_Press[nPos]>=1) {
			Rectangle(hDC, 162+33*nPos, 241-0.01*sts->r2d2->Hydraulic_Press[nPos], 183+33*nPos, 241);
		}
		//Fuel P
		sprintf(cbuf, "%d", sts->r2d2->Fuel_Press[nPos]);
		TextOut(hDC, 159+33*nPos, 32, cbuf, strlen(cbuf));
		if(sts->r2d2->Fuel_Press[nPos]>0) {
			Rectangle(hDC, 162+33*nPos, 91-0.026* sts->r2d2->Fuel_Press[nPos], 183+33*nPos, 91);
		}
	}
	DeleteObject(GreenBrush);
	DeleteObject(BlackBrush);
	DeleteObject(GreenPen);
}

void CRT::UNIVPTG(HDC hDC)
{
	char cbuf[255];

	//PitchYawRoll=ToDeg(CalcPitchYawRollAngles());
	sts->GetAngularVel(AngularVelocity);
	sts->GetGlobalOrientation(InertialOrientationRad);
	for(int nPos=0;nPos<3;nPos++) {
		InertialOrientation.data[nPos]=DEG*InertialOrientationRad.data[nPos];
		if(InertialOrientation.data[nPos]<0.0) InertialOrientation.data[nPos]+=360.0;
	}

	TextOut(hDC, 0, 0, "2011/    /", 10);
	TextOut(hDC, 100, 0, "UNIV PTG", 8);
	sprintf(cbuf, "%.3d/%.2d:%.2d:%.2d", sts->MET[0], sts->MET[1], sts->MET[2], sts->MET[3]);
	TextOut(hDC, 170, 0, cbuf, strlen(cbuf));
	
	sprintf(cbuf, "1 START TIME %.3d/%.2d:%.2d:%.2d", 
		sts->START_TIME[0], sts->START_TIME[1], sts->START_TIME[2], sts->START_TIME[3]);
	TextOut(hDC, 0, 15, cbuf, strlen(cbuf));
	
	TextOut(hDC, 0, 35, "MNVR OPTION", 11);
	sprintf(cbuf,"5 R %6.2f", sts->MNVR_OPTION.data[ROLL]);
	TextOut(hDC, 0, 45, cbuf, strlen(cbuf));
	sprintf(cbuf,"6 P %6.2f", sts->MNVR_OPTION.data[PITCH]);
	TextOut(hDC, 0, 55, cbuf, strlen(cbuf));
	sprintf(cbuf,"7 Y %6.2f", sts->MNVR_OPTION.data[YAW]);
	TextOut(hDC, 0, 65, cbuf, strlen(cbuf));

	TextOut(hDC, 0, 85, "TRK/ROT OPTIONS", 15);
	sprintf(cbuf, " 8 TGT ID %3d", sts->TGT_ID);
	TextOut(hDC, 0, 95, cbuf, strlen(cbuf));

	TextOut(hDC, 0, 115, " 9  RA", 6);
	TextOut(hDC, 0, 125, " 10 DEC", 7);
	TextOut(hDC, 0, 135, " 11 LAT", 7);
	TextOut(hDC, 0, 145, " 12 LON", 7);
	TextOut(hDC, 0, 155, " 13 ALT", 7);

	sprintf(cbuf, " 14 BODY VECT %d", sts->BODY_VECT);
	TextOut(hDC, 0, 204, cbuf, strlen(cbuf));
	sprintf(cbuf, " 15 P %6.2f", sts->P);
	TextOut(hDC, 0, 224, cbuf, strlen(cbuf));
	sprintf(cbuf, " 16 Y %6.2f", sts->Y);
	TextOut(hDC, 0, 234, cbuf, strlen(cbuf));
	if(sts->OM>=0.0) {
		sprintf(cbuf, " 17 OM %6.2f", sts->OM);
		TextOut(hDC, 0, 244, cbuf, strlen(cbuf));
	}
	else TextOut(hDC, 0, 244, " 17 OM", 6);

	TextOut(hDC, 90, 35, "START MNVR 18", 13);
	TextOut(hDC, 132, 45, "TRK  19", 7);
	TextOut(hDC, 132, 55, "ROT  20", 7);
	TextOut(hDC, 125, 65, "CNCL  21", 8);
	TextOut(hDC, 185, 25, "CUR", 3);
	TextOut(hDC, 215, 25, "FUT", 3);
	if(sts->MNVR) {
		if(sts->ManeuverinProg) TextOut(hDC, 190, 35, "X", 1);
		else TextOut(hDC, 222, 35, "X", 1);
	}
	else if(sts->TRK) {
		if(sts->ManeuverinProg) TextOut(hDC, 190, 45, "X", 1);
		else TextOut(hDC, 222, 45, "X", 1);
	}
	else if(sts->ROT) {
		if(sts->ManeuverinProg) TextOut(hDC, 190, 55, "X", 1);
		else TextOut(hDC, 222, 55, "X", 1);
	}

	TextOut(hDC, 125, 85, "ATT MON", 7);
	TextOut(hDC, 125, 95, "22 MON AXIS", 11);
	TextOut(hDC, 125, 105, "ERR TOT 23", 10);
	TextOut(hDC, 125, 115, "ERR DAP 24", 10);

	TextOut(hDC, 110, 155, "ROLL    PITCH    YAW", 20);
	sprintf(cbuf, "CUR   %6.2f  %6.2f  %6.2f", InertialOrientation.data[ROLL], InertialOrientation.data[PITCH], InertialOrientation.data[YAW]);
	TextOut(hDC, 60, 165, cbuf, strlen(cbuf));
	sprintf(cbuf, "REQD  %6.2f  %6.2f  %6.2f", sts->REQD_ATT.data[ROLL], sts->REQD_ATT.data[PITCH], sts->REQD_ATT.data[YAW]);
	TextOut(hDC, 60, 175, cbuf, strlen(cbuf));
	sprintf(cbuf, "ERR  %+7.2f %+7.2f %+7.2f", sts->PitchYawRoll.data[ROLL], sts->PitchYawRoll.data[PITCH], sts->PitchYawRoll.data[YAW]);
	TextOut(hDC, 60, 185, cbuf, strlen(cbuf));
	sprintf(cbuf, "RATE %+7.3f %+7.3f %+7.3f", DEG*AngularVelocity.data[ROLL], DEG*AngularVelocity.data[PITCH], DEG*AngularVelocity.data[YAW]);
	TextOut(hDC, 60, 195, cbuf, strlen(cbuf));
}

void CRT::DAP_CONFIG(HDC hDC)
{
	char cbuf[255];
	char *strings[3]={"ALL", "NOSE", "TAIL"};
	char *Edit[3]={"6/7 EDIT", "EDIT A", "EDIT B"};
	int lim[3]={3, 5, 5};
	int i, n;

	TextOut(hDC, 0, 0, "2011/020/", 9);
	TextOut(hDC, 80, 0, "DAP CONFIG", 10);
	sprintf(cbuf, "%.3d/%.2d:%.2d:%.2d", sts->MET[0], sts->MET[1], sts->MET[2], sts->MET[3]);
	TextOut(hDC, 170, 0, cbuf, strlen(cbuf));

	TextOut(hDC, 84, 10, "1 A", 3);
	TextOut(hDC, 149, 10, "2 B", 3);
	TextOut(hDC, 105+65*(sts->DAPMode[0]), 10, "*", 1);
	TextOut(hDC, 200, 10, Edit[edit], strlen(Edit[edit]));
	TextOut(hDC, 200, 20, "8 LOAD", 6);

	TextOut(hDC, 14, 30, "3 PRI", 5);
	TextOut(hDC, 0, 40, "ROT RATE", 8);
	TextOut(hDC, 0, 50, "ATT DB", 6);
	TextOut(hDC, 0, 60, "RATE DB", 7);
	TextOut(hDC, 0, 70, "ROT PLS", 7);
	TextOut(hDC, 0, 80, "COMP", 4);
	TextOut(hDC, 0, 90, "P OPTION", 8);
	TextOut(hDC, 0, 100, "Y OPTION", 8);
	TextOut(hDC, 0, 110, "TRAN PLS", 8);

	TextOut(hDC, 14, 120, "4 ALT", 5);
	TextOut(hDC, 0, 130, "RATE DB", 7);
	TextOut(hDC, 0, 140, "JET OPT", 7);
	TextOut(hDC, 0, 150, "# JETS", 6);
	TextOut(hDC, 0, 160, "ON TIME", 7);
	TextOut(hDC, 0, 170, "DELAY", 5);

	TextOut(hDC, 14, 180, "5 VERN", 6);
	TextOut(hDC, 0, 190, "ROT RATE", 8);
	TextOut(hDC, 0, 200, "ATT DB", 6);
	TextOut(hDC, 0, 210, "RATE DB", 7);
	TextOut(hDC, 0, 220, "ROT PLS", 7);
	TextOut(hDC, 0, 230, "COMP", 4);
	TextOut(hDC, 0, 240, "CNTL ACC", 8);
	if(sts->DAPMode[1]==0) TextOut(hDC, 20, 30, "*", 1);
	else if(sts->DAPMode[1]==1) TextOut(hDC, 20, 120, "*", 1);
	else TextOut(hDC, 20, 180, "*", 1);

	MoveToEx(hDC, 59, 42, NULL);
	LineTo (hDC, 59, 254);
	MoveToEx(hDC, 124, 42, NULL);
	LineTo (hDC, 124, 254);
	MoveToEx(hDC, 189, 42, NULL);
	LineTo (hDC, 189, 254);

	for(n=1, i=0;n<=lim[edit];n+=2, i++) {
		sprintf(cbuf, "%d %.4f", 10*n, sts->DAP[i].PRI_ROT_RATE);
		TextOut(hDC, 60+65*i, 40, cbuf, strlen(cbuf));
		sprintf(cbuf, "%d %.2f", 10*n+1, sts->DAP[i].PRI_ATT_DB);
		TextOut(hDC, 60+65*i, 50, cbuf, strlen(cbuf));
		sprintf(cbuf, "%d %.2f", 10*n+2, sts->DAP[i].PRI_RATE_DB);
		TextOut(hDC, 60+65*i, 60, cbuf, strlen(cbuf));
		sprintf(cbuf, "%d %.2f", 10*n+3, sts->DAP[i].PRI_ROT_PLS);
		TextOut(hDC, 60+65*i, 70, cbuf, strlen(cbuf));
		sprintf(cbuf, "%d %.3f", 10*n+4, sts->DAP[i].PRI_COMP);
		TextOut(hDC, 60+65*i, 80, cbuf, strlen(cbuf));
		sprintf(cbuf, "%d %s", 10*n+5, strings[sts->DAP[i].PRI_P_OPTION]);
		TextOut(hDC, 60+65*i, 90, cbuf, strlen(cbuf));
		sprintf(cbuf, "%d %s", 10*n+6, strings[sts->DAP[i].PRI_Y_OPTION]);
		TextOut(hDC, 60+65*i, 100, cbuf, strlen(cbuf));
		sprintf(cbuf, "%d %.2f", 10*n+7, sts->DAP[i].PRI_TRAN_PLS);
		TextOut(hDC, 60+65*i, 110, cbuf, strlen(cbuf));

		sprintf(cbuf, "%d %.2f", 10*n+8, sts->DAP[i].ALT_RATE_DB);
		TextOut(hDC, 60+65*i, 130, cbuf, strlen(cbuf));
		sprintf(cbuf, "%d %s", 10*n+9, strings[sts->DAP[i].ALT_JET_OPT]);
		TextOut(hDC, 60+65*i, 140, cbuf, strlen(cbuf));
		sprintf(cbuf, "%d %d", 10*n+10, sts->DAP[i].ALT_JETS);
		TextOut(hDC, 60+65*i, 150, cbuf, strlen(cbuf));
		sprintf(cbuf, "%d %.2f", 10*n+11, sts->DAP[i].ALT_ON_TIME);
		TextOut(hDC, 60+65*i, 160, cbuf, strlen(cbuf));
		sprintf(cbuf, "%d %.2f", 10*n+12, sts->DAP[i].ALT_DELAY);
		TextOut(hDC, 60+65*i, 170, cbuf, strlen(cbuf));

		sprintf(cbuf, "%d %.4f", 10*n+13, sts->DAP[i].VERN_ROT_RATE);
		TextOut(hDC, 60+65*i, 190, cbuf, strlen(cbuf));
		sprintf(cbuf, "%d %.2f", 10*n+14, sts->DAP[i].VERN_ATT_DB);
		TextOut(hDC, 60+65*i, 200, cbuf, strlen(cbuf));
		sprintf(cbuf, "%d %.3f", 10*n+15, sts->DAP[i].VERN_RATE_DB);
		TextOut(hDC, 60+65*i, 210, cbuf, strlen(cbuf));
		sprintf(cbuf, "%d %.2f", 10*n+16, sts->DAP[i].VERN_ROT_PLS);
		TextOut(hDC, 60+65*i, 220, cbuf, strlen(cbuf));
		sprintf(cbuf, "%d %.3f", 10*n+17, sts->DAP[i].VERN_COMP);
		TextOut(hDC, 60+65*i, 230, cbuf, strlen(cbuf));
		sprintf(cbuf, "%d %d", 10*n+18, sts->DAP[i].VERN_CNTL_ACC);
		TextOut(hDC, 60+65*i, 240, cbuf, strlen(cbuf));
	}
}

void CRT::MNVR(HDC hDC)
{
	char cbuf[255];
	int minutes, seconds;
	int timeDiff;
	int TIMER[4];
	switch(sts->ops) {
		case 104:
			TextOut(hDC, 0, 0, "1041/", 5);
			TextOut(hDC, 60, 0, "OMS 1 MNVR EXEC", 15);
			if((sts->oparam.PeT)<(sts->oparam.ApT)) {
				minutes=sts->oparam.PeT/60;
				seconds=sts->oparam.PeT-(60*minutes);
				sprintf(cbuf, "TTP %.2d:%.2d", minutes, seconds); 
				TextOut(hDC, 102, 130, cbuf, strlen(cbuf));
				//sprintf(oapiDebugString(), "%f %f", sts->oparam.PeT, sts->oparam.ApT);
				//sprintf(oapiDebugString(), "OPARAM %f %f", sts->oparam.PeT, sts->oparam.SMi);
			}
			else {
				minutes=sts->oparam.ApT/60;
				seconds=sts->oparam.ApT-(60*minutes);
				sprintf(cbuf, "TTA %.2d:%.2d", minutes, seconds); 
				TextOut(hDC, 102, 130, cbuf, strlen(cbuf));
			}
			break;
		case 105:
			TextOut(hDC, 0, 0, "1051/", 5);
			TextOut(hDC, 60, 0, "OMS 2 MNVR EXEC", 15);
			if((sts->oparam.PeT)<(sts->oparam.ApT)) {
				minutes=sts->oparam.PeT/60;
				seconds=sts->oparam.PeT-(60*minutes);
				sprintf(cbuf, "TTP %.2d:%.2d", minutes, seconds); 
				TextOut(hDC, 102, 130, cbuf, strlen(cbuf));
				//sprintf(oapiDebugString(), "%f %f", sts->oparam.PeT, sts->oparam.ApT);
				//sprintf(oapiDebugString(), "OPARAM %f %f", sts->oparam.PeT, sts->oparam.SMi);
			}
			else {
				minutes=sts->oparam.ApT/60;
				seconds=sts->oparam.ApT-(60*minutes);
				sprintf(cbuf, "TTA %.2d:%.2d", minutes, seconds); 
				TextOut(hDC, 102, 130, cbuf, strlen(cbuf));
			}
			break;
		case 106:
			TextOut(hDC, 0, 0, "1061/", 5);
			TextOut(hDC, 60, 0, "OMS 2 MNVR COAST", 16);
			if((sts->oparam.PeT)<(sts->oparam.ApT)) {
				minutes=sts->oparam.PeT/60;
				seconds=sts->oparam.PeT-(60*minutes);
				sprintf(cbuf, "TTP %.2d:%.2d", minutes, seconds); 
				TextOut(hDC, 102, 130, cbuf, strlen(cbuf));
				//sprintf(oapiDebugString(), "%f %f", sts->oparam.PeT, sts->oparam.ApT);
				//sprintf(oapiDebugString(), "OPARAM %f %f", sts->oparam.PeT, sts->oparam.SMi);
			}
			else {
				minutes=sts->oparam.ApT/60;
				seconds=sts->oparam.ApT-(60*minutes);
				sprintf(cbuf, "TTA %.2d:%.2d", minutes, seconds); 
				TextOut(hDC, 102, 130, cbuf, strlen(cbuf));
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
				TextOut(hDC, 102, 130, cbuf, strlen(cbuf));
				//sprintf(oapiDebugString(), "%f %f", sts->oparam.PeT, sts->oparam.ApT);
				//sprintf(oapiDebugString(), "OPARAM %f %f", sts->oparam.PeT, sts->oparam.SMi);
			}
			else {
				minutes=sts->oparam.ApT/60;
				seconds=sts->oparam.ApT-(60*minutes);
				sprintf(cbuf, "TTA %.2d:%.2d", minutes, seconds); 
				TextOut(hDC, 102, 130, cbuf, strlen(cbuf));
			}
			break;
		case 301:
			TextOut(hDC, 0, 0, "3011/", 5);
			TextOut(hDC, 60, 0, "DEORB MNVR COAST", 16);
			if((sts->oparam.PeT)<(sts->oparam.ApT)) { // should show REI
				minutes=sts->oparam.PeT/60;
				seconds=sts->oparam.PeT-(60*minutes);
				sprintf(cbuf, "TTP %.2d:%.2d", minutes, seconds); 
				TextOut(hDC, 102, 130, cbuf, strlen(cbuf));
				//sprintf(oapiDebugString(), "%f %f", sts->oparam.PeT, sts->oparam.ApT);
				//sprintf(oapiDebugString(), "OPARAM %f %f", sts->oparam.PeT, sts->oparam.SMi);
			}
			else {
				minutes=sts->oparam.ApT/60;
				seconds=sts->oparam.ApT-(60*minutes);
				sprintf(cbuf, "TTA %.2d:%.2d", minutes, seconds); 
				TextOut(hDC, 102, 130, cbuf, strlen(cbuf));
			}
			break;
		case 302:
			TextOut(hDC, 0, 0, "3021/", 5);
			TextOut(hDC, 60, 0, "DEORB MNVR EXEC", 15);
			if((sts->oparam.PeT)<(sts->oparam.ApT)) { // should show REI
				minutes=sts->oparam.PeT/60;
				seconds=sts->oparam.PeT-(60*minutes);
				sprintf(cbuf, "TTP %.2d:%.2d", minutes, seconds); 
				TextOut(hDC, 102, 130, cbuf, strlen(cbuf));
				//sprintf(oapiDebugString(), "%f %f", sts->oparam.PeT, sts->oparam.ApT);
				//sprintf(oapiDebugString(), "OPARAM %f %f", sts->oparam.PeT, sts->oparam.SMi);
			}
			else {
				minutes=sts->oparam.ApT/60;
				seconds=sts->oparam.ApT-(60*minutes);
				sprintf(cbuf, "TTA %.2d:%.2d", minutes, seconds); 
				TextOut(hDC, 102, 130, cbuf, strlen(cbuf));
			}
			break;
		case 303:
			TextOut(hDC, 0, 0, "3031/", 5);
			TextOut(hDC, 60, 0, "DEORB MNVR COAST", 16);
			if((sts->oparam.PeT)<(sts->oparam.ApT)) { // should show REI/TFF
				minutes=sts->oparam.PeT/60;
				seconds=sts->oparam.PeT-(60*minutes);
				sprintf(cbuf, "TTP %.2d:%.2d", minutes, seconds); 
				TextOut(hDC, 102, 130, cbuf, strlen(cbuf));
				//sprintf(oapiDebugString(), "%f %f", sts->oparam.PeT, sts->oparam.ApT);
				//sprintf(oapiDebugString(), "OPARAM %f %f", sts->oparam.PeT, sts->oparam.SMi);
			}
			else {
				minutes=sts->oparam.ApT/60;
				seconds=sts->oparam.ApT-(60*minutes);
				sprintf(cbuf, "TTA %.2d:%.2d", minutes, seconds); 
				TextOut(hDC, 102, 130, cbuf, strlen(cbuf));
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
		TextOut(hDC, 173, 10, cbuf, strlen(cbuf));
	}

	TextOut(hDC, 0, 10, "OMS BOTH 1", 10);
	TextOut(hDC, 49, 20, "L 2", 3);
	TextOut(hDC, 49, 30, "R 3", 3);
	TextOut(hDC, 0, 40, "RCS SEL  4", 10);
	/*if(sts->OMS==0) TextOut(hDC, 60, 10, "*");
	else if(sts->OMS==1)*/
	TextOut(hDC, 70, 10+10*sts->OMS, "*", 1);

	sprintf(cbuf, "5 TV ROLL %d", sts->TV_ROLL);
	TextOut(hDC, 0, 50, cbuf, strlen(cbuf));
	//TextOut(hDC, 0, 50, "5 TV ROLL", 9);
	TextOut(hDC, 0, 60, "TRIM LOAD", 9);
	sprintf(cbuf, " 6 P  %+2.1f", sts->Trim.data[0]);
	TextOut(hDC, 0, 70, cbuf, strlen(cbuf));
	//TextOut(hDC, 0, 70, " 6 P", 4);
	sprintf(cbuf, " 7 LY %+2.1f", sts->Trim.data[1]);
	TextOut(hDC, 0, 80, cbuf, strlen(cbuf));
	//TextOut(hDC, 0, 80, " 7 LY", 5);
	sprintf(cbuf, " 8 RY %+2.1f", sts->Trim.data[2]);
	TextOut(hDC, 0, 90, cbuf, strlen(cbuf));
	//TextOut(hDC, 0, 90, " 8 RY", 5);
	sprintf(cbuf, "9 WT %6.0f", sts->WT);
	TextOut(hDC, 0, 100, cbuf, strlen(cbuf));
	TextOut(hDC, 0, 110, "10 TIG", 6);
	sprintf(cbuf, "%03.0f/%02.0f:%02.0f:%04.1f", sts->TIG[0], sts->TIG[1], sts->TIG[2], sts->TIG[3]);
	TextOut(hDC, 0, 120, cbuf, strlen(cbuf));
	
	TextOut(hDC, 0, 130, "TGT PEG 4", 9);
	TextOut(hDC, 0, 140, " 14 C1", 6);
	TextOut(hDC, 0, 150, " 15 C2", 6);
	TextOut(hDC, 0, 160, " 16 HT", 6);
	TextOut(hDC, 0, 170, " 17  T", 6);
	Ellipse(hDC, 28, 173, 34, 182);
	MoveToEx(hDC, 28, 177, NULL);
	LineTo(hDC, 34, 177);
	TextOut(hDC, 0, 180, " 18 PRPLT", 9);
	
	TextOut(hDC, 0, 190, "TGT PEG 7", 9);
	TextOut(hDC, 0, 200, " 19  VX", 7);
	DrawDelta(hDC, 30, 204, 27, 33, 210);
	TextOut(hDC, 0, 210, " 20  VY", 7);
	DrawDelta(hDC, 30, 214, 27, 33, 220);
	TextOut(hDC, 0, 220, " 21  VZ", 7);
	DrawDelta(hDC, 30, 224, 27, 33, 230);
	if(sts->PEG7.x!=0.0 || sts->PEG7.y!=0.0 || sts->PEG7.z!=0.0) {
		sprintf(cbuf, "%+7.1f", sts->PEG7.x);
		TextOut(hDC, 49, 200, cbuf, strlen(cbuf));
		sprintf(cbuf, "%+6.1f", sts->PEG7.y);
		TextOut(hDC, 56, 210, cbuf, strlen(cbuf));
		sprintf(cbuf, "%+6.1f", sts->PEG7.z);
		TextOut(hDC, 56, 220, cbuf, strlen(cbuf));
	}
	
	if(sts->MNVRLOAD) {
		TextOut(hDC, 0, 240, "LOAD 22/TIMER 23", 16);
		sprintf(cbuf, "24 R %-3.0f", sts->BurnAtt.data[ROLL]);
		TextOut(hDC, 100, 30, cbuf, strlen(cbuf));
		sprintf(cbuf, "25 P %-3.0f", sts->BurnAtt.data[PITCH]);
		TextOut(hDC, 100, 40, cbuf, strlen(cbuf));
		sprintf(cbuf, "26 Y %-3.0f", sts->BurnAtt.data[YAW]);
		TextOut(hDC, 100, 50, cbuf, strlen(cbuf));
	}
	else {
		TextOut(hDC, 0, 240, "     22/TIMER 23", 16);
		TextOut(hDC, 100, 30, "24 R", 4);
		TextOut(hDC, 100, 40, "25 P", 4);
		TextOut(hDC, 100, 50, "26 Y", 4);
	}

	MoveToEx(hDC, 98, 15, NULL);
	LineTo(hDC, 98, 240);

	TextOut(hDC, 100, 20, "BURN ATT", 8);
	if(!sts->TRK) TextOut(hDC, 100, 60, "MNVR 27", 7);
	else TextOut(hDC, 100, 60, "MNVR 27X", 8);

	TextOut(hDC, 102, 120, "REI", 3);
	TextOut(hDC, 138, 150, "GMBL", 4);
	TextOut(hDC, 131, 160, "L    R", 6);
	TextOut(hDC, 102, 170, "P +0.0 +0.0", 11);
	TextOut(hDC, 102, 180, "Y +0.0 +0.0", 11);

	TextOut(hDC, 102, 190, "PRI 28  29", 10);
	TextOut(hDC, 102, 200, "SEC 30  31", 10);
	TextOut(hDC, 102, 210, "OFF 32  33", 10);

	TextOut(hDC, 102, 230, "GMBL CK 34", 10);

	MoveToEx(hDC, 156, 15, NULL);
	LineTo(hDC, 156, 122);
	LineTo(hDC, 250, 122);

	if(!sts->BurnInProg) {
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
	TextOut(hDC, 158, 30, cbuf, strlen(cbuf));
	DrawDelta(hDC, 161, 34, 158, 164, 40);
	sprintf(cbuf, "TGO %.2d:%.2d", TGO[0], TGO[1]);
	TextOut(hDC, 158, 40, cbuf, strlen(cbuf));
	sprintf(cbuf, "VGO X %+8.2f", sts->VGO.x);
	TextOut(hDC, 158, 50, cbuf, strlen(cbuf)); 
	sprintf(cbuf, "    Y  %+7.2f", sts->VGO.y);
	TextOut(hDC, 158, 60, cbuf, strlen(cbuf));
	sprintf(cbuf, "    Z  %+7.2f", sts->VGO.z);
	TextOut(hDC, 158, 70, cbuf, strlen(cbuf));
	TextOut(hDC, 165, 90, "    HA     HP", 13);
	sprintf(cbuf, "TGT");
	TextOut(hDC, 158, 100, cbuf, strlen(cbuf));
	sprintf(cbuf, "CUR");
	TextOut(hDC, 158, 110, cbuf, strlen(cbuf));

	sprintf(cbuf, "35 ABORT TGT");
	TextOut(hDC, 150, 140, cbuf, strlen(cbuf));
	
	TextOut(hDC, 185, 150, "FWD RCS", 7);
	TextOut(hDC, 185, 160, "  ARM  36", 9);
	TextOut(hDC, 185, 170, "  DUMP 37", 9);
	TextOut(hDC, 185, 180, "  OFF  38", 9);
	TextOut(hDC, 185, 190, "SURF DRIVE", 10);
	TextOut(hDC, 185, 200, "  ON   39", 9);
	TextOut(hDC, 185, 210, "  OFF  40", 9);
}

void CRT::DrawDelta(HDC hDC, int TopX, int TopY, int LBottomX, int RBottomX, int BottomY)
{
	MoveToEx(hDC, TopX, TopY, NULL);
	LineTo(hDC, LBottomX, BottomY);
	LineTo(hDC, RBottomX, BottomY);
	LineTo(hDC, TopX, TopY);
}

void CRT::EnableThrusters(const int Thrusters[], int nThrusters)
{
	for(int i=0;i<nThrusters;i++) {
		if(Thrusters[i]%2==1)
			sts->SetThrusterResource(sts->th_att_rcs[Thrusters[i]], sts->ph_oms);
		else
			sts->SetThrusterResource(sts->th_att_rcs[Thrusters[i]], sts->ph_frcs);
	}
}

void CRT::DisableThrusters(const int Thrusters[], int nThrusters)
{
	for(int i=0;i<nThrusters;i++) {
		//vessel->SetThrusterMax0(th_att_rcs[Thrusters[i]], 0.0);
		sts->SetThrusterResource(sts->th_att_rcs[Thrusters[i]], NULL);
	}
}

void CRT::OMSGimbal(VECTOR3 Targets)
{
	//double LAngles[2], RAngles[2];
	VECTOR3 Dir;
	//VECTOR3 Pos; //for debugging
	sts->OMSGimbal[0][0]=sts->OMSGimbal[1][0]=-(ORBITER_OMS_PITCH+Targets.data[0]);
	sts->OMSGimbal[0][1]=ORBITER_OMS_YAW+Targets.data[1];
	sts->OMSGimbal[1][1]=-ORBITER_OMS_YAW+Targets.data[2];
	// L OMS Engine
	Dir.z=sqrt(1-pow(sin(sts->OMSGimbal[0][0]*RAD), 2)-pow(sin(sts->OMSGimbal[0][1]*RAD), 2));
	Dir.y=sin(sts->OMSGimbal[0][0]*RAD);
	Dir.x=sin(sts->OMSGimbal[0][1]*RAD);
	sts->SetThrusterDir(sts->th_oms[0], Dir);
	//sprintf(oapiDebugString(), "L OMS %f %f %f", Dir.x, Dir.y, Dir.z);
	// R OMS Engine
	Dir.z=sqrt(1-pow(sin(sts->OMSGimbal[1][0]*RAD), 2)-pow(sin(sts->OMSGimbal[1][1]*RAD), 2));
	Dir.y=sin(sts->OMSGimbal[1][0]*RAD);
	Dir.x=sin(sts->OMSGimbal[1][1]*RAD);
	sts->SetThrusterDir(sts->th_oms[1], Dir);
	//sts->GetThrusterRef(sts->th_oms[0], Pos);
	//sprintf(oapiDebugString(), "%f %f %f %f", sts->OMSGimbal[0][0], sts->OMSGimbal[0][1], sts->OMSGimbal[1][0], sts->OMSGimbal[1][1]);
	//sprintf(oapiDebugString(), "R OMS %f %f %f %f %f %f", Dir.x, Dir.y, Dir.z, Pos.x, Pos.y, Pos.z);
	return;
}

void CRT::LoadManeuver()
{
	int i;
	double StartWeight, EndWeight, EndWeightLast=0.0, FuelRate;
	//VECTOR3 ThrustVector;
	bool bDone=false;
	sts->MNVRLOAD=true;
	//sts->BurnCompleted=false;
	sts->tig=sts->TIG[0]*86400+sts->TIG[1]*3600+sts->TIG[2]*60+sts->TIG[3];
	//dV
	for(i=0;i<3;i++) {
		sts->DeltaV.data[i]=sts->PEG7.data[i]*fps_to_ms;
		//sts->VGO.data[i]=sts->PEG7.data[i];
	}
	sts->DeltaVTot=length(sts->PEG7);
	sts->DeltaVTotms=length(sts->DeltaV); //make local variable?
	OMSGimbal(sts->Trim);
	//Burn Attitude
	if(sts->OMS==0) {
		sts->BurnAtt.data[PITCH]=ORBITER_OMS_PITCH+sts->Trim.data[0];
		if(sts->DeltaV.x!=0.0 || sts->DeltaV.z!=0.0) {
			if(sts->DeltaV.z<=0) sts->BurnAtt.data[PITCH]+=DEG*acos(sts->DeltaV.x/sqrt((pow(sts->DeltaV.x, 2)+pow(sts->DeltaV.z, 2))));
			else sts->BurnAtt.data[PITCH]-=DEG*acos(sts->DeltaV.x/sqrt((pow(sts->DeltaV.x, 2)+pow(sts->DeltaV.z, 2))));
		}
		if(sts->DeltaV.x!=0.0 || sts->DeltaV.y!=0.0)
			sts->BurnAtt.data[YAW]=DEG*asin(sts->DeltaV.y/sqrt((pow(sts->DeltaV.x, 2)+pow(sts->DeltaV.y, 2))))+sts->Trim.data[1]+sts->Trim.data[2];
		else sts->BurnAtt.data[YAW]=sts->Trim.data[1]+sts->Trim.data[2];
		sts->BurnAtt.data[ROLL]=sts->TV_ROLL;
	}
	else if(sts->OMS<3) {
		sts->BurnAtt.data[PITCH]=ORBITER_OMS_PITCH+sts->Trim.data[0];
		if(sts->DeltaV.x!=0.0 || sts->DeltaV.z!=0.0) {
			if(sts->DeltaV.z<=0) sts->BurnAtt.data[PITCH]+=DEG*acos(sts->DeltaV.x/sqrt((pow(sts->DeltaV.x, 2)+pow(sts->DeltaV.z, 2))));
			else sts->BurnAtt.data[PITCH]-=DEG*acos(sts->DeltaV.x/sqrt((pow(sts->DeltaV.x, 2)+pow(sts->DeltaV.z, 2))));
		}
		if(sts->DeltaV.x!=0.0 || sts->DeltaV.y!=0.0)
			sts->BurnAtt.data[YAW]=DEG*asin(sts->DeltaV.y/sqrt((pow(sts->DeltaV.x, 2)+pow(sts->DeltaV.y, 2))))+sts->Trim.data[sts->OMS-1]+ORBITER_OMS_YAW*pow(-1.0,-sts->OMS);
		else sts->BurnAtt.data[YAW]=sts->Trim.data[sts->OMS-1]+ORBITER_OMS_YAW*pow(-1.0,-sts->OMS);
		sts->BurnAtt.data[ROLL]=sts->TV_ROLL;
	}
	else {
		if(sts->DeltaV.z<=0) sts->BurnAtt.data[PITCH]=DEG*acos(sts->DeltaV.x/sqrt((pow(sts->DeltaV.x, 2)+pow(sts->DeltaV.z, 2))));
		else sts->BurnAtt.data[PITCH]=-DEG*acos(sts->DeltaV.x/sqrt((pow(sts->DeltaV.x, 2)+pow(sts->DeltaV.z, 2))));
		sts->BurnAtt.data[YAW]=DEG*asin(sts->DeltaV.y/sqrt((pow(sts->DeltaV.x, 2)+pow(sts->DeltaV.y, 2))));
		sts->BurnAtt.data[ROLL]=sts->TV_ROLL;
	}
	if(sts->TV_ROLL!=0.0) {
		double dTemp=sts->BurnAtt.data[PITCH];
		sts->BurnAtt.data[PITCH]-=sts->BurnAtt.data[PITCH]*(1.0-cos(sts->TV_ROLL*RAD));
		sts->BurnAtt.data[YAW]+=sts->BurnAtt.data[YAW]*(1.0-sin(sts->TV_ROLL*RAD));
	}
	//use rocket equation
	StartWeight=sts->WT/kg_to_pounds;
	EndWeight=StartWeight/exp(sts->DeltaVTotms/sts->GetThrusterIsp(sts->th_oms[0]));
	FuelRate=ORBITER_OMS_THRUST/(sts->GetPropellantEfficiency(sts->ph_oms)*sts->GetThrusterIsp(sts->th_oms[0]));
	if(sts->OMS==0) FuelRate=FuelRate*2.0;
	sts->BurnTime=(StartWeight-EndWeight)/FuelRate;
	TGO[0]=sts->BurnTime/60;
	TGO[1]=sts->BurnTime-(TGO[0]*60);
	sts->VGO.x=sts->DeltaVTot*cos((ORBITER_OMS_PITCH+sts->Trim.data[0])*RAD);
	sts->VGO.y=sts->DeltaVTot*sin((sts->Trim.data[1]+sts->Trim.data[2])*RAD);
	sts->VGO.z=sts->DeltaVTot*sin((ORBITER_OMS_PITCH+sts->Trim.data[0])*RAD);
}

void CRT::UpdateDAP()
{
	sprintf(oapiDebugString(), "UpdateDAP() called %f", oapiRand());
	if(sts->DAPMode[1]==0) {
		sts->RotRate=sts->DAP[sts->DAPMode[0]].PRI_ROT_RATE;
		sts->AttDeadband=sts->DAP[sts->DAPMode[0]].PRI_ATT_DB;
		sts->RateDeadband=sts->DAP[sts->DAPMode[0]].PRI_RATE_DB;
		if(sts->DAP[sts->DAPMode[0]].PRI_P_OPTION!=0)
			sts->Torque.data[PITCH]=0.5*ORBITER_PITCH_TORQUE;
		else sts->Torque.data[PITCH]=ORBITER_PITCH_TORQUE;
		if(sts->DAP[sts->DAPMode[0]].PRI_Y_OPTION!=0)
			sts->Torque.data[YAW]=0.5*ORBITER_YAW_TORQUE;
		else sts->Torque.data[YAW]=ORBITER_YAW_TORQUE;
		sts->Torque.data[ROLL]=ORBITER_ROLL_TORQUE;
	}
	else if(sts->DAPMode[1]==1) {
		sts->RotRate=sts->DAP[sts->DAPMode[0]].PRI_ROT_RATE;
		sts->AttDeadband=sts->DAP[sts->DAPMode[0]].PRI_ATT_DB;
		sts->RateDeadband=sts->DAP[sts->DAPMode[0]].ALT_RATE_DB;
		if(sts->DAP[sts->DAPMode[0]].ALT_JET_OPT==2) {
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
}

char *CRT::ButtonLabel (int bt)
{
	static char *label[2][4] = {{"UP", "OMS", "HYD", "SPI"},
	{"UP", "OPS", "SPC", "ITM"}};
	return (bt < 4 ? label[mode][bt] : 0);
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
    case OAPI_KEY_O:
	  oapiOpenInputBox("OPS", cbChangeOps, 0, 20, (void *)this);
	  InvalidateDisplay();
      return true;
    case OAPI_KEY_S:
	  oapiOpenInputBox("SPEC", cbSpec, 0, 20, (void *)this);
	  InvalidateDisplay();
      return true;
    case OAPI_KEY_I:
	  oapiOpenInputBox("ITEM", cbItem, 0, 20, (void *)this);
	  InvalidateDisplay();
      return true;
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
	oapiWriteScenario_int (scn, "Spec", spec);
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
		else if(!strnicmp(line, "Display", 7)) {
			sscanf(line+7, "%d", &display);
		}
		/*else if(!strnicmp(line, "OPS", 3)) {
			sscanf(line+3, "%d", &ops);
		}*/
		else if(!strnicmp(line, "Spec", 4)) {
			sscanf(line+4, "%d", &spec);
		}
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
	saveprm.spec=spec;
	saveprm.mode=mode;
	saveprm.display=display;
	//if(saveprm.bValid) sprintf(oapiDebugString(), "%i %i %i %i", saveprm.spec[id], saveprm.mode[id], saveprm.display[id], id);
}

void CRT::RecallStatus()
{
	spec=saveprm.spec;
	mode=saveprm.mode;
	display=saveprm.display;
	//sprintf(oapiDebugString(), "Recall %f", oapiRand());
}

void CRT::Data(int id)
{
	bool cbItemData(void *id, char *str, void *data);
	bool cbSpecData(void *id, char *str, void *data);

	data=0;
	if(id==0) oapiOpenInputBox("DATA", cbItemData, 0, 20, (void *)this);
	else if(id==1) oapiOpenInputBox("DATA", cbSpecData, 0, 20, (void *)this);
}

bool CRT::Input(int change, char *Name)
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
							}*/
							sts->REQD_ATT.x=sts->MNVR_OPTION.x;
							sts->REQD_ATT.y=sts->MNVR_OPTION.y;
							sts->REQD_ATT.z=sts->MNVR_OPTION.z;
							for(int i=0;i<3;i++) {
								if(sts->REQD_ATT.data[i]>180.0) sts->TargetAtt.data[i]=sts->REQD_ATT.data[i]-360.0;
								else sts->TargetAtt.data[i]=sts->REQD_ATT.data[i];
							}
							sts->TargetAtt=ToRad(sts->TargetAtt);
						}
						else sts->MNVR=false;
						sts->Yaw=false;
						sts->Pitch=false;
						sts->Roll=false;
						return true;
					}
					else if(nNew==19) {
						if(sts->TRK==false) {
							sts->TRK=true;
							sts->MNVR=false;
							sts->ROT=false;
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
							}*/
							if(sts->TGT_ID==2) {
								/*sts->LVLHRateVector.data[PITCH]=0.0;
								sts->LVLHRateVector.data[YAW]=0.0;
								sts->LVLHRateVector.data[ROLL]=0.0;*/
								if(sts->BODY_VECT==1) {
									sts->LVLHOrientationReqd.data[PITCH]=270.0;
									sts->LVLHOrientationReqd.data[YAW]=0.0;
									if((sts->OM)<=0.0) {
										sts->LVLHOrientationReqd.data[ROLL]=0.0;
										sts->LVLHRateVector.data[PITCH]=-1.0;
										sts->LVLHRateVector.data[YAW]=0.0;
										sts->LVLHRateVector.data[ROLL]=0.0;
									}
									else {
										sts->LVLHOrientationReqd.data[ROLL]=sts->OM;
										sts->LVLHRateVector.data[PITCH]=-1.0*cos(RAD*sts->OM);
										sts->LVLHRateVector.data[YAW]=-1.0*sin(RAD*sts->OM);
										sts->LVLHRateVector.data[ROLL]=0.0;
									}
								}
								else if(sts->BODY_VECT==2) {
									sts->LVLHOrientationReqd.data[PITCH]=90.0;
									sts->LVLHOrientationReqd.data[YAW]=0.0;
									if(sts->OM<=0.0) {
										sts->LVLHOrientationReqd.data[ROLL]=0.0;
										sts->LVLHRateVector.data[PITCH]=-1.0;
										sts->LVLHRateVector.data[YAW]=0.0;
										sts->LVLHRateVector.data[ROLL]=0.0;
									}
									else {
										sts->LVLHOrientationReqd.data[ROLL]=sts->OM;
										sts->LVLHRateVector.data[PITCH]=-1.0*cos(RAD*sts->OM);
										sts->LVLHRateVector.data[YAW]=-1.0*sin(RAD*sts->OM);
										sts->LVLHRateVector.data[ROLL]=0.0;
									}
								}
								else if(sts->BODY_VECT==3) {
									sts->LVLHOrientationReqd.data[PITCH]=180.0;
									sts->LVLHOrientationReqd.data[ROLL]=0.0;
									if((sts->OM)<=0.0) {
										sts->LVLHOrientationReqd.data[YAW]=0.0;
										sts->LVLHRateVector.data[PITCH]=-1.0;
										sts->LVLHRateVector.data[YAW]=0.0;
										sts->LVLHRateVector.data[ROLL]=0.0;
									}
									else {
										sts->LVLHOrientationReqd.data[YAW]=sts->OM;
										sts->LVLHRateVector.data[PITCH]=-1.0*cos(RAD*sts->OM);
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
									else sts->LVLHOrientationReqd.data[YAW]=sts->OM;*/
									if(sts->LVLHOrientationReqd.data[PITCH]==0.0)
									{
										sts->LVLHOrientationReqd.data[ROLL]=sts->Y;
										if(sts->OM<=0.0) {
											sts->LVLHOrientationReqd.data[YAW]=0.0;
											if(sts->LVLHOrientationReqd.data[ROLL]==0.0) {
												sts->LVLHRateVector.data[PITCH]=-1.0;
												sts->LVLHRateVector.data[YAW]=0.0;
												sts->LVLHRateVector.data[ROLL]=0.0;
											}
										}
										else sts->LVLHOrientationReqd.data[YAW]=sts->OM;
									}
									else
									{
										sts->LVLHOrientationReqd.data[YAW]=(270.0-sts->P)*sin(RAD*sts->Y);
										sts->LVLHOrientationReqd.data[ROLL]=sts->Y*cos(RAD*sts->LVLHOrientationReqd.data[PITCH]);
										if(sts->OM>0.0) sts->LVLHOrientationReqd.data[YAW]+=sts->OM;
									}
									sts->LVLHRateVector.data[PITCH]=-1.0*cos(RAD*sts->LVLHOrientationReqd.data[ROLL])*cos(RAD*sts->LVLHOrientationReqd.data[YAW]);
									sts->LVLHRateVector.data[ROLL]=1.0*sin(RAD*sts->LVLHOrientationReqd.data[YAW]);
									sts->LVLHRateVector.data[YAW]=-1.0*sin(RAD*sts->LVLHOrientationReqd.data[ROLL])*cos(RAD*sts->LVLHOrientationReqd.data[YAW]);
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
									}*/
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
							if(sts->REQD_ATT.data[i]>180.0) sts->TargetAtt.data[i]=sts->REQD_ATT.data[i]-360.0;
							else sts->TargetAtt.data[i]=sts->REQD_ATT.data[i];
						}
						sts->TargetAtt=ToRad(sts->TargetAtt);
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
					sts->LVLHRateVector.data[PITCH]=-1.0*cos(RAD*sts->LVLHOrientationReqd.data[ROLL])*cos(RAD*sts->LVLHOrientationReqd.data[YAW]);
					sts->LVLHRateVector.data[ROLL]=1.0*sin(RAD*sts->LVLHOrientationReqd.data[YAW]);
					sts->LVLHRateVector.data[YAW]=-1.0*sin(RAD*sts->LVLHOrientationReqd.data[ROLL])*cos(RAD*sts->LVLHOrientationReqd.data[YAW]);
					/*sts->LVLHRateVector.data[PITCH]=-1.0*cos(RAD*sts->BurnAtt.data[YAW]);
					sts->LVLHRateVector.data[ROLL]=1.0*sin(RAD*sts->BurnAtt.data[YAW]);
					sts->LVLHRateVector.data[YAW]=1.0*sin(RAD*sts->BurnAtt.data[PITCH]);*/
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
				if(abs(dNew)<=6.0) 
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
				if(abs(dNew)<10.0) {
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

//Math
/*VECTOR3 CRT::GetPYR(VECTOR3 Pitch, VECTOR3 YawRoll)
{	
	VECTOR3 Res = { 0, 0, 0 };

	// Normalize the vectors
	Pitch = Normalize(Pitch);
	YawRoll = Normalize(YawRoll);
	VECTOR3 H = Normalize(CrossProduct(Pitch, YawRoll));

	Res.data[YAW] = -asin(YawRoll.z);

	Res.data[ROLL] = atan2(YawRoll.y, YawRoll.x);

	Res.data[PITCH] = atan2(H.z, Pitch.z);

	return Res;

}

VECTOR3 CRT::GetPYR2(VECTOR3 Pitch, VECTOR3 YawRoll)
{	
	VECTOR3 Res = { 0, 0, 0 };
	// Normalize the vectors
	Pitch = Normalize(Pitch);
	YawRoll = Normalize(YawRoll);
	VECTOR3 H = Normalize(CrossProduct(Pitch, YawRoll));
	Res.data[YAW] = -asin(Pitch.x);
	Res.data[ROLL] = atan2(H.x, YawRoll.x);
	Res.data[PITCH] = atan2(Pitch.y, Pitch.z);
	return Res;
}

void CRT::RotateVector(const VECTOR3 &Initial, const VECTOR3 &Angles, VECTOR3 &Result)
{
	MATRIX3 RotMatrixX, RotMatrixY, RotMatrixZ;
	VECTOR3 AfterZ, AfterZY;					// Temporary variables


	GetRotMatrixX(Angles.x, RotMatrixX);
	GetRotMatrixY(Angles.y, RotMatrixY);
	GetRotMatrixZ(Angles.z, RotMatrixZ);
	
	MultiplyByMatrix(Initial, RotMatrixZ, AfterZ);
	MultiplyByMatrix(AfterZ, RotMatrixY, AfterZY);
	MultiplyByMatrix(AfterZY, RotMatrixX, Result);
}

// Returns the rotation matrix for a rotation of a given angle around the X axis (Pitch)
void CRT::GetRotMatrixX(double Angle, MATRIX3 &RotMatrixX)
{
	RotMatrixX.m11 = 1;
	RotMatrixX.m12 = 0;
	RotMatrixX.m13 = 0;
	RotMatrixX.m21 = 0;
	RotMatrixX.m22 = cos(Angle);
	RotMatrixX.m23 = sin(Angle);
	RotMatrixX.m31 = 0;
	RotMatrixX.m32 = -sin(Angle);
	RotMatrixX.m33 = cos(Angle);
}

// Returns the rotation matrix for a rotation of a given angle around the Y axis (Yaw)
void CRT::GetRotMatrixY(double Angle, MATRIX3 &RotMatrixY)
{
	RotMatrixY.m11 = cos(Angle);
	RotMatrixY.m12 = 0;
	RotMatrixY.m13 = -sin(Angle);
	RotMatrixY.m21 = 0;
	RotMatrixY.m22 = 1;
	RotMatrixY.m23 = 0;
	RotMatrixY.m31 = sin(Angle);
	RotMatrixY.m32 = 0;
	RotMatrixY.m33 = cos(Angle);
}

// Returns the rotation matrix for a rotation of a given angle around the Z axis (Roll)
void CRT::GetRotMatrixZ(double Angle, MATRIX3 &RotMatrixZ)
{
	RotMatrixZ.m11 = cos(Angle);
	RotMatrixZ.m12 = sin(Angle);
	RotMatrixZ.m13 = 0;
	RotMatrixZ.m21 = -sin(Angle);
	RotMatrixZ.m22 = cos(Angle);
	RotMatrixZ.m23 = 0;
	RotMatrixZ.m31 = 0;
	RotMatrixZ.m32 = 0;
	RotMatrixZ.m33 = 1;
}

void CRT::MultiplyByMatrix(const VECTOR3 &Initial, const MATRIX3 &RotMatrix, VECTOR3 &Result)
{

	Result.x =	(Initial.x * RotMatrix.m11) 
				+ (Initial.y * RotMatrix.m12) 
				+ (Initial.z * RotMatrix.m13);	
	Result.y =	(Initial.x * RotMatrix.m21) 
				+ (Initial.y * RotMatrix.m22) 
				+ (Initial.z * RotMatrix.m23);	
	Result.z =	(Initial.x * RotMatrix.m31) 
				+ (Initial.y * RotMatrix.m32) 
				+ (Initial.z * RotMatrix.m33);
}*/

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
}
