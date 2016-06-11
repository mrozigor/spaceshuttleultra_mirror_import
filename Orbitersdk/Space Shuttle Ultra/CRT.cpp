#define STRICT
#define ORBITER_MODULE
#include "Atlantis.h"
#include "CRT.h"


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
	CRT::InitializeSavedParameters();
	
}

DLLCLBK void ExitModule (HINSTANCE hDLL)
{
	// Unregister the custom MFD mode when the module is unloaded
	oapiUnregisterMFDMode (g_MFDmode);
}

// ==============================================================
// MFD class implementation

/**
 * Initialize the common memory for all MFDs using the CRT mode. 
 */
void CRT::InitializeSavedParameters() 
{
	for (int i = 0; i < 11; i++) {
		CRT::saveprm.bValid[i] = false;
		CRT::saveprm.mode[i] = 3;
		CRT::saveprm.display[i] = 0;
	}
}

CRT::CRT (DWORD w, DWORD h, VESSEL *v)
: MFD (w, h, v)
{
	char cbuf[200];

	vessel=v;
	width=w;
	height=h;
	
	MDUID=-1;

	sprintf_s(cbuf, 200, "[CRT]:DIMENSIONS: %d %d\n", W, H);
	oapiWriteLog(cbuf);

	/*if(!strcmp(pV->GetClassName(), "Atlantis") || 
		!_stricmp(pV->GetClassName(), STD_CLASS_NAME)) */
	if(!_stricmp(pV->GetClassName(), STD_CLASS_NAME)) 
	{

		sts=static_cast<Atlantis*>(pV);

		sts->newmfd=this;
		mode=3;
	}
	else {
		mode=10001;
	}

	display=0;

	UpdateStatus=true;
	return;
}

CRT::~CRT ()
{
	return;
}

void CRT::Update (HDC hDC)
{
	if(mode==10001) {
			int n;
			char cbuf[255];
			sprintf_s(cbuf, 255, vessel->GetClassName());
			TextOut(hDC, 0, 5, cbuf, strlen(cbuf));
			n=strlen(cbuf);
			sprintf_s(cbuf, 255, "%i", n);
			TextOut(hDC, 0, 15, cbuf, strlen(cbuf));
			//TextOut(hDC, 0, 25, "ERROR: Vessel not from Atlantis class", 37);
			TextOut(hDC, 0, 25, "ERROR: Vessel not from", 22);
			TextOut(hDC, 120, 35, "SpaceShuttleUltra class", 23);
			return;
	}

	if(UpdateStatus) {
		RecallStatus();
		vc::MDU* mdu = sts->GetMDU( MDUID );
		if (mdu)
		{
			mdu->Set_display( display );
			mdu->Set_menu( mode );
		}
		InvalidateButtons();
		UpdateStatus=false;
	}

	switch (display)
	{
		case 0:// "DPS display"
			if (MDUID >= 0)
			{
				vc::MDU* mdu = sts->GetMDU( MDUID );
				if (mdu) mdu->Paint(hDC);
				else oapiWriteLog("CRT MFD not connected to MDU (d0)");
			}
			break;
		case 1:// A/E PFD
			{
				vc::MDU* mdu = sts->GetMDU( MDUID );
				if (mdu) mdu->AEPFD( hDC );
				else oapiWriteLog( "CRT MFD not connected to MDU (d1)" );
			}
			break;
		case 2:// ORBIT PFD
			{
				vc::MDU* mdu = sts->GetMDU( MDUID );
				if (mdu) mdu->ORBITPFD( hDC );
				else oapiWriteLog( "CRT MFD not connected to MDU (d2)" );
			}
			break;
		case 3:// OMS/MPS
			{
				vc::MDU* mdu = sts->GetMDU( MDUID );
				if (mdu) mdu->OMSMPS( hDC );
				else oapiWriteLog( "CRT MFD not connected to MDU (d3)" );
			}
			break;
		case 4:// HYD/APU
			{
				vc::MDU* mdu = sts->GetMDU( MDUID );
				if (mdu) mdu->APUHYD( hDC );
				else oapiWriteLog( "CRT MFD not connected to MDU (d4)" );
			}
			break;
		case 5:// SPI
			{
				vc::MDU* mdu = sts->GetMDU( MDUID );
				if (mdu) mdu->SPI( hDC );
				else oapiWriteLog( "CRT MFD not connected to MDU (d5)" );
			}
			break;
		case 6:// CST Menu
			{
				vc::MDU* mdu = sts->GetMDU( MDUID );
				if (mdu) mdu->SystemStatusDisplay_CSTMenu( hDC );
				else oapiWriteLog( "CRT MFD not connected to MDU (d6)" );
			}
			break;
		case 7:// IDP Interactive CST
			{
				vc::MDU* mdu = sts->GetMDU( MDUID );
				if (mdu) mdu->SystemStatusDisplay_IDPInteractiveCST( hDC );
				else oapiWriteLog( "CRT MFD not connected to MDU (d7)" );
			}
			break;
	}
}

char *CRT::ButtonLabel (int bt)
{
	static char *label[7][5] = {{"", "FLT", "SUB", "DPS", "MEDS1"},
	{"UP", "A/E", "ORBIT", "", ""},
	{"UP", "OMS", "HYD", "SPI", ""},
	{"UP", "", "", "", ""},
	{"UP", "", "", "CST", ""},
	{"UP", "", "S_IDP", "", ""},
	{"UP", "", "", "", ""}};
	return ((mode < 7 && bt < 5) ? label[mode][bt] : NULL);
}

// Return button menus
int CRT::ButtonMenu (const MFDBUTTONMENU **menu) const
{
	static const MFDBUTTONMENU mnu[7][5] = {
		{{"", 0, 'U'}, {"FLT INST", 0, '1'}, {"SUBSYS STATUS", 0, '2'}, {"DPS", 0, '3'}, {"MEDS MAINT", 0, '4'}},
		{{"Move up", 0, 'U'}, {"A/E PFD", 0, '1'}, {"ORBIT PFD", 0, '2'}, {"", 0, '3'}, {"", 0, '4'}},
		{{"Move up", 0, 'U'}, {"OMS/MPS", 0, '1'}, {"HYD/APU", 0, '2'}, {"SPI", 0, '3'}, {"", 0, '4'}},
		{{"Move up", 0, 'U'}, {"", 0, '1'}, {"", 0, '2'}, {"", 0, '3'}, {"", 0, '4'}},
		{{"Move up", 0, 'U'}, {"", 0, '1'}, {"", 0, '2'}, {"CST", 0, '3'}, {"", 0, '4'}},
		{{"Move up", 0, 'U'}, {"", 0, '1'}, {"START IDP", 0, '2'}, {"", 0, '3'}, {"", 0, '4'}},
		{{"Move up", 0, 'U'}, {"", 0, '1'}, {"", 0, '2'}, {"0", 0, '3'}, {"", 0, '4'}}
	};
	if (menu) *menu = mnu[mode];
	return 5; // return the number of buttons used
}

bool CRT::ConsumeKeyBuffered (DWORD key)
{
	vc::MDU* mdu;
	switch (mode)
	{
		case 0:// MAIN MENU
			switch (key)
			{
				case OAPI_KEY_1:
					mode = 1;
					mdu = sts->GetMDU( MDUID );
					if (mdu) mdu->Set_menu( mode );
					InvalidateButtons();
					return true;
				case OAPI_KEY_2:
					mode = 2;
					mdu = sts->GetMDU( MDUID );
					if (mdu) mdu->Set_menu( mode );
					InvalidateButtons();
					return true;
				case OAPI_KEY_3:
					mode = 3;
					display = 0;
					mdu = sts->GetMDU( MDUID );
					if (mdu)
					{
						mdu->Set_display( display );
						mdu->Set_menu( mode );
					}
					InvalidateDisplay();
					InvalidateButtons();
					return true;
				case OAPI_KEY_4:
					mode = 4;
					display = 6;
					mdu = sts->GetMDU( MDUID );
					if (mdu)
					{
						mdu->Set_display( display );
						mdu->Set_menu( mode );
					}
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
					mdu = sts->GetMDU( MDUID );
					if (mdu) mdu->Set_menu( mode );
					InvalidateButtons();
					return true;
				case OAPI_KEY_1:
					display = 1;
					mdu = sts->GetMDU( MDUID );
					if (mdu) mdu->Set_display( display );
					InvalidateDisplay();
					InvalidateButtons();
					return true;
				case OAPI_KEY_2:
					display = 2;
					mdu = sts->GetMDU( MDUID );
					if (mdu) mdu->Set_display( display );
					InvalidateDisplay();
					InvalidateButtons();
					return true;
			}
			break;
		case 2:// SUBSYS STATUS
			switch (key)
			{
				case OAPI_KEY_U:
					mode = 0;
					mdu = sts->GetMDU( MDUID );
					if (mdu) mdu->Set_menu( mode );
					InvalidateButtons();
					return true;
				case OAPI_KEY_1:
					display = 3;
					mdu = sts->GetMDU( MDUID );
					if (mdu) mdu->Set_display( display );
					InvalidateDisplay();
					InvalidateButtons();
					return true;
				case OAPI_KEY_2:
					display = 4;
					mdu = sts->GetMDU( MDUID );
					if (mdu) mdu->Set_display( display );
					InvalidateDisplay();
					InvalidateButtons();
					return true;
				case OAPI_KEY_3:
					display = 5;
					mdu = sts->GetMDU( MDUID );
					if (mdu) mdu->Set_display( display );
					InvalidateDisplay();
					InvalidateButtons();
					return true;
			}
			break;
		case 3:// DPS MENU
			switch (key)
			{
				case OAPI_KEY_U:
					mode = 0;
					mdu = sts->GetMDU( MDUID );
					if (mdu) mdu->Set_menu( mode );
					InvalidateButtons();
					return true;
			}
			break;
		case 4:// MAINTENANCE MENU
			switch (key)
			{
				case OAPI_KEY_U:
					mode = 0;
					mdu = sts->GetMDU( MDUID );
					if (mdu) mdu->Set_menu( mode );
					InvalidateButtons();
					return true;
				case OAPI_KEY_3:
					mode = 5;
					mdu = sts->GetMDU( MDUID );
					if (mdu) mdu->Set_menu( mode );
					InvalidateButtons();
					return true;
			}
			break;
		case 5:// CST MENU SELECTION
			switch (key)
			{
				case OAPI_KEY_U:
					mode = 4;
					display = 6;
					mdu = sts->GetMDU( MDUID );
					if (mdu)
					{
						mdu->Set_display( display );
						mdu->Set_menu( mode );
					}
					InvalidateDisplay();
					InvalidateButtons();
					return true;
				case OAPI_KEY_2:
					mode = 6;
					display = 7;
					mdu = sts->GetMDU( MDUID );
					if (mdu)
					{
						mdu->Set_display( display );
						mdu->Set_menu( mode );
					}
					InvalidateDisplay();
					InvalidateButtons();
					return true;
			}
			break;
		case 6:// IDPx INTERACTIVE CST
			switch (key)
			{
				case OAPI_KEY_U:
					mode = 5;
					mdu = sts->GetMDU( MDUID );
					if (mdu) mdu->Set_menu( mode );
					InvalidateButtons();
					return true;
			}
			break;
	}
	return false;
}

bool CRT::ConsumeButton (int bt, int event)
{
  if (!(event & PANEL_MOUSE_LBDOWN)) return false;
  static const DWORD btkey[7][5] = {{OAPI_KEY_U, OAPI_KEY_1, OAPI_KEY_2, OAPI_KEY_3, OAPI_KEY_4},
  {OAPI_KEY_U, OAPI_KEY_1, OAPI_KEY_2, OAPI_KEY_3, OAPI_KEY_4},
  {OAPI_KEY_U, OAPI_KEY_1, OAPI_KEY_2, OAPI_KEY_3, OAPI_KEY_4},
  {OAPI_KEY_U, OAPI_KEY_1, OAPI_KEY_2, OAPI_KEY_3, OAPI_KEY_4},
  {OAPI_KEY_U, OAPI_KEY_1, OAPI_KEY_2, OAPI_KEY_3, OAPI_KEY_4},
  {OAPI_KEY_U, OAPI_KEY_1, OAPI_KEY_2, OAPI_KEY_3, OAPI_KEY_4},
  {OAPI_KEY_U, OAPI_KEY_1, OAPI_KEY_2, OAPI_KEY_3, OAPI_KEY_4}};
  if (bt < 5) return ConsumeKeyBuffered (btkey[mode][bt]);
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
	oapiWriteScenario_int(scn, "MDUID", MDUID);
}

void CRT::ReadStatus(FILEHANDLE scn)
{
	char *line;
	while (oapiReadScenario_nextline (scn, line)) {
		if (!_strnicmp (line, "Mode2", 5)) {
			sscanf_s (line+5, "%d", &mode);
		}
		else if(!_strnicmp(line, "Display", 7)) {
			sscanf_s(line+7, "%d", &display);
		}
		else if (!_strnicmp (line, "END_MFD", 7)) break;
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
