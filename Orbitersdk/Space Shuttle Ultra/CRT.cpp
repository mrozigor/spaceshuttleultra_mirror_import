#include "Atlantis.h"
#include "CRT.h"


// ==============================================================
// MFD class implementation


CRT::CRT (DWORD w, DWORD h, VESSEL *v, int MDUID)
: MFD2 (w, h, v)
{
	char cbuf[200];

	this->MDUID = MDUID;
	
	sprintf_s(cbuf, 200, "[CRT]:DIMENSIONS: %d %d MDUID: %d\n", W, H, MDUID);
	oapiWriteLog(cbuf);

	sts = static_cast<Atlantis*>(pV);

	mdu = sts->GetMDU( MDUID );
	return;
}

CRT::~CRT ()
{
	return;
}

bool CRT::Update( oapi::Sketchpad* skp )
{
	if (mdu) mdu->PaintDisplay( skp );
	return true;
}

char *CRT::ButtonLabel (int bt)
{
	if (mdu) return mdu->ButtonLabel( bt );
	else return NULL;
}

// Return button menus
int CRT::ButtonMenu (const MFDBUTTONMENU **menu) const
{
	if (mdu) return mdu->ButtonMenu( menu );
	else return 0;
}

bool CRT::ConsumeKeyBuffered (DWORD key)
{
	if (mdu)
	{
		switch (mdu->NavigateMenu( key ))
		{
			case 0:
				return false;
			case 1:
				InvalidateButtons();
				return true;
			/*case 2:
				InvalidateDisplay();
				return true;*/
			case 3:
				InvalidateDisplay();
				InvalidateButtons();
				return true;
		}
	}
	return false;
}

bool CRT::ConsumeButton (int bt, int event)
{
	if (!(event & PANEL_MOUSE_LBDOWN)) return false;

	static const DWORD btkey[5] = {OAPI_KEY_U, OAPI_KEY_1, OAPI_KEY_2, OAPI_KEY_3, OAPI_KEY_4};
	
	if (bt < 5) return ConsumeKeyBuffered (btkey[bt]);
	else return false;
}

// MFD message parser
int CRT::MsgProc (UINT msg, UINT mfd, WPARAM wparam, LPARAM lparam)
{
	switch (msg) {
	case OAPI_MSG_MFD_OPENED:
		// Our new MFD mode has been selected, so we create the MFD and
		// return a pointer to it.
		return (int)(new CRT (LOWORD(wparam), HIWORD(wparam), (VESSEL*)lparam, mfd));
	}
	return 0;
}
