#ifndef __CRT_H
#define __CRT_H

#include "orbitersdk.h"


typedef struct {
	HINSTANCE hDLL;
} MFD_GDIPARAM;

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
	void WriteStatus (FILEHANDLE scn) const;
	void ReadStatus (FILEHANDLE scn);
	void StoreStatus() const;
	void RecallStatus();

	Atlantis *sts;

	static void InitializeSavedParameters();

private:
	VESSEL *vessel;
	
	int width, height;
	int mode, display;
	int MDUID;

	bool UpdateStatus;
	static struct SavePrm{
		int mode[11], display[11];
		bool bValid[11];
	} saveprm;
};

#endif // !__CRT_H
