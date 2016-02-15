#ifndef __CRT_H
#define __CRT_H

#include "orbitersdk.h"


typedef struct {
	HINSTANCE hDLL;
	HFONT hCRTFont;
} MFD_GDIPARAM;

class CRT: public MFD {
	friend class Atlantis;
	friend class Keyboard;
public:
	CRT (DWORD w, DWORD h, VESSEL *vessel);
	~CRT ();
	char *ButtonLabel (int bt);
	int ButtonMenu (const MFDBUTTONMENU **menu) const;
	bool ConsumeKeyBuffered (DWORD key);
	bool ConsumeButton (int bt, int event);
	void Update (HDC hDC);
	static int MsgProc (UINT msg, UINT mfd, WPARAM wparam, LPARAM lparam);
	void WriteStatus (FILEHANDLE scn) const;
	void ReadStatus (FILEHANDLE scn);
	void StoreStatus() const;
	void RecallStatus();

	Atlantis *sts;

	static void InitializeSavedParameters();

private:
	// GDI stuff
	HBRUSH GreenBrush;
	HBRUSH TurquoiseBrush; 
	HBRUSH WhiteBrush;
	HBRUSH BlackBrush;
	HBRUSH RedBrush;
	HBRUSH PurpleBrush;
	HBRUSH YellowBrush;
	HBRUSH LightBlueBrush;
	HPEN WhitePen;
	HPEN GreenPen;
	HPEN TurquoisePen;
	HPEN RedPen;
	HPEN BlackPen;
	HPEN PurplePen;
	HPEN PurpleThinPen;
	HPEN DarkPurplePen;
	HPEN YellowPen;
	HPEN YellowThickPen;
	HPEN BoldWhitePen;
	HPEN LightBluePen;
	HFONT ArialFont_h15w5;
	HFONT ArialFont_h13w6;

	//Display Functions
	void OMSMPS(HDC hDC);
	void SPI(HDC hDC);
	void APUHYD(HDC hDC);


	VESSEL *vessel;
	
	int width, height;
	int mode, display;
	int MDUID;

	bool UpdateStatus;
	static struct SavePrm{
		int mode[11], display[11];
		bool bValid[11];
	} saveprm;

	
protected:
	HFONT hCRTFont;
};

#endif // !__CRT_H
