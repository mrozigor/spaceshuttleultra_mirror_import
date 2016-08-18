#ifndef __CRT_H
#define __CRT_H

#include "orbitersdk.h"


class CRT: public MFD2 {
	friend class Atlantis;
public:
	CRT (DWORD w, DWORD h, VESSEL *vessel, int MDUID);
	~CRT ();
	char *ButtonLabel (int bt);
	int ButtonMenu (const MFDBUTTONMENU **menu) const;
	bool ConsumeKeyBuffered (DWORD key);
	bool ConsumeButton (int bt, int event);
	bool Update( oapi::Sketchpad* skp );
	static int MsgProc (UINT msg, UINT mfd, WPARAM wparam, LPARAM lparam);

	Atlantis *sts;
	vc::MDU* mdu;

private:
	int MDUID;
};

#endif // !__CRT_H