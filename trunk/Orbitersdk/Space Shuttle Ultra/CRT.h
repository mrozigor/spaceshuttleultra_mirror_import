#ifndef __CRT_H
#define __CRT_H

#include "Atlantis.h"
#include "PanelR2.h"
#include "orbitersdk.h"
#include <math.h>


typedef struct {
	HINSTANCE hDLL;
	HFONT hCRTFont;
} MFD_GDIPARAM;

const int convert[69] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};

class CRT: public MFD {
	friend class Atlantis;
public:
	CRT (DWORD w, DWORD h, VESSEL *vessel);
	~CRT ();
	char *ButtonLabel (int bt);
	int ButtonMenu (const MFDBUTTONMENU **menu) const;
	bool ConsumeKeyBuffered (DWORD key);
	bool ConsumeButton (int bt, int event);
	void Update (HDC hDC);
	static int MsgProc (UINT msg, UINT mfd, WPARAM wparam, LPARAM lparam);
	bool Input(int change, char *Name);
	void Data(int id);
	void WriteStatus (FILEHANDLE scn) const;
	void ReadStatus (FILEHANDLE scn);
	void StoreStatus() const;
	void RecallStatus();

	Atlantis *sts;

private:
	void SetDisplayTitle(const char* pszTitle);
	unsigned short usPageNumber;
	unsigned short usDisp;
	unsigned short usGPCDriver;
	//Math
	//VECTOR3 CalcPitchYawRollAngles(VECTOR3 &RelAttitude);
	/*VECTOR3 GetPYR(VECTOR3 Pitch, VECTOR3 YawRoll);
	VECTOR3 GetPYR2(VECTOR3 Pitch, VECTOR3 YawRoll);
	void RotateVector(const VECTOR3 &Initial, const VECTOR3 &Angles, VECTOR3 &Result);
	void GetRotMatrixX(double Angle, MATRIX3 &RotMatrixX);
	void GetRotMatrixY(double Angle, MATRIX3 &RotMatrixY);
	void GetRotMatrixZ(double Angle, MATRIX3 &RotMatrixZ);
	void MultiplyByMatrix(const VECTOR3 &Initial, const MATRIX3 &RotMatrix, VECTOR3 &Result);
	inline double Mag(const VECTOR3 &a)
	{
		return (sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2)));
	}
	inline VECTOR3 Normalize(const VECTOR3 &a)
	{
		return (a/Mag(a));
	}
	inline VECTOR3 CrossProduct(const VECTOR3 &a, const VECTOR3 &b)
	{
		VECTOR3 c;
		c.x = (a.y * b.z - a.z * b.y);
		c.y = (a.z * b.x - a.x * b.z);
		c.z = (a.x * b.y - a.y * b.x);
		return c;
	}*/

	inline VECTOR3 ToRad(const VECTOR3 &Input)
	{
		VECTOR3 Output;
		for(int i=0;i<3;i++) {
			Output.data[i]=Input.data[i]*RAD;
		}
		return Output;
	}
	inline VECTOR3 ToDeg(const VECTOR3 &Input)
	{
		VECTOR3 Output;
		for(int i=0;i<3;i++) {
			Output.data[i]=Input.data[i]*DEG;
		}
		return Output;
	}
	inline bool Eq(const double d1, const double d2)
	{
		if(fabs(d1-d2)>0.000001) return false;
		return true;
	}

	//Display Functions
	void OMSMPS(HDC hDC);
	void SPI(HDC hDC);
	void APUHYD(HDC hDC);
	void UNIVPTG(HDC hDC);
	void DAP_CONFIG(HDC hDC);
	void MNVR(HDC hDC);
	void DrawDelta(HDC hDC, int TopX, int TopY, int LBottomX, int RBottomX, int BottomY);

	//Thruster Control
	//void EnableAllThrusters();
	void EnableThrusters(const int Thrusters[], int nThrusters);
	void DisableThrusters(const int Thrusters[], int nThrusters);
	
	void OMSGimbal(VECTOR3 Targets);
	void LoadManeuver();
	void UpdateDAP(); //updates rot rates, torques

	VESSEL *vessel;
	VECTOR3 GVesselPos, GVesselVel;
	VESSELSTATUS Status;

	int width, height;
	int ops, item, spec, mode, display;
	int data;
	int id;

	int MET[4], Launch_time[4], MET_Add[4]; // day,hour,min,sec
	double dT;
	bool landed;

	int MPS_Manifold_Press[2]; //0=LO2, 1=LH2
	int MPS_He_Reg_Press[4], MPS_He_Tank_Press[4];
	int Hydraulic_Press[3];
	double APU_Fuel_Last[3], APU_Fuel_Rate, Simtime_last, Simtime;

	VECTOR3 InertialOrientation, InertialOrientationRad, AngularVelocity;
	int edit; //0=Blank, 1=A, 2=B
	VECTOR3 PMI;

	//MNVR
	bool bTIMER;
	int TGO[2]; // min, sec
	//int TV_ROLL;
	//double C1, C2, HT, ThetaT; // PEG4 Targets
	//VECTOR3 Trim; // 0=P, 1=LY, 2=RY

	static struct SavePrm{
		int spec, mode, display;
		bool bValid;
	} saveprm;

	
protected:
	char cDispTitle[20];
	void DrawCommonHeader(HDC hdc);
	void PASSTRAJ(HDC hdc);
	HFONT hCRTFont;
};

#endif // !__CRT_H