#ifndef ___SPACESHUTTLEULTRA_COMM_MODE_HANDLER_H_INCLUDED___
#define ___SPACESHUTTLEULTRA_COMM_MODE_HANDLER_H_INCLUDED___

#include <string>

using namespace std;

class Atlantis;

typedef enum
{
	COMMROLE_CDR_PRELAUNCH,
	COMMROLE_CDR_ASCENT,
	COMMROLE_CDR_ORBIT,
} COMM_ROLE;

class CommModeHandler
{
private:
	Atlantis* pSTS;
	string strDecompG2A;
	string strDecompA2G;

	string strCompA2G;

	string strNegAnswer_Comp;
	string strNegAnswer_Decomp;

	string strPosAnswer_Comp;
	string strPosAnswer_Decomp;

	double fRecDisplayT0;
	double fSendDisplayT0;

	int iMenu;
private:
	void ClearRecAnnotations();
	void ClearSendAnnotations();
public:
	CommModeHandler(Atlantis* _pSTS);
	void DefineAnnotations();
	bool ConsumeBufferedKey(DWORD key, bool pressed, char* keymap);
	void PostStep(double fSimT, double fDeltaT);
	void OnReceiveFocus();
	void OnLooseFocus();
	bool IsInCommMode() const;
	void EnterCommMode();
protected:
	NOTEHANDLE nhLastSend;
	NOTEHANDLE nhOptionPos;
	void BlankReceiveAnnotation();
	void BlankSendAnnotations();
	void LeaveCommMode();
	NOTEHANDLE nhLastReceived;
	bool bCommMode;
};

#endif
