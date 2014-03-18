// ==============================================================
//                 ORBITER MODULE: MissionControl
//
// MissionControl.cpp
//
// ==============================================================

#define _CRT_SECURE_NO_WARNINGS

#define STRICT
#define ORBITER_MODULE
#include "windows.h"
#include "orbitersdk.h"
#include "resource.h"
#include <stdio.h>

#include "mainForm.h"

#define VESSEL_NAME_LEN		248

// ==============================================================
// Controls
// ==============================================================

#define IDC_ACTIVATE_MODULE			1001
HWND button_activate_module;
void update_button_activate_module(void);

// ==============================================================
// Classes
// ==============================================================
class MyModule : oapi::Module
{
public:
	MyModule(HINSTANCE hDLL) : oapi::Module(hDLL) {}
	virtual void clbkSimulationStart(RenderMode mode);
	virtual void clbkPostStep(double simt, double simdt, double mjd);
	virtual void clbkFocusChanged(OBJHANDLE new_focus, OBJHANDLE old_focus);
};

// ==============================================================
// Global variables
// ==============================================================

HINSTANCE g_hInst;  // module instance handle
DWORD g_dwCmd;      // custom function identifier

MyModule* myModule = NULL;

HWND hMyDlg = NULL;

MyFrame *myFrame;

bool first_loading = true;	//first_loading is set to false at the end of the method MyModule::clbkSimulationStart
bool active = false;	//if the module is active
bool open_my_dialog = false;
RECT my_dialog_pos = {100,100,0,0};

int screen_width, screen_height;

NOTEHANDLE hNote = NULL;
double last_time;

OBJHANDLE hVessel;

// ==============================================================
// Local prototypes
// ==============================================================

void OpenDlgClbk (void *context);
BOOL CALLBACK MsgProc (HWND, UINT, WPARAM, LPARAM);

// ==============================================================
// API interface
// ==============================================================

// ==============================================================
// This function is called when Orbiter starts or when the module
// is activated.

DLLCLBK void InitModule (HINSTANCE hDLL)
{
	wxApp::SetInstance(new MyApp());
	int argc = 0;
	wxEntryStart(argc, (char **)NULL);
	wxTheApp->OnInit();

	myFrame = new MyFrame(NULL);
	myFrame->Show(true);

	//wxTheApp->OnRun();	//this of course hangs everything; we need to do a separate thread
	//wxTheApp->OnExit();
	//wxEntryCleanup();



	g_hInst = hDLL; // remember the instance handle

	// To allow the user to open our new dialog box, we create
	// an entry in the "Custom Functions" list which is accessed
	// in Orbiter via Ctrl-F4.
	g_dwCmd = oapiRegisterCustomCmd (MODULE_NAME_SHORT, MODULE_DESCRIPTION, OpenDlgClbk, NULL);

	myModule = new MyModule(hDLL);
	oapiRegisterModule((oapi::Module*)myModule);

	oapiWriteLog("\t"MODULE_NAME_LONG" "MODULE_VERSION";\t"MODULE_LOG_INIT_MESSAGE);
}

// ==============================================================

void MyModule::clbkSimulationStart(RenderMode mode)
{
	//	initialization

	if(active)
	{
		if(first_loading)
		{
			if(open_my_dialog)
			{	OpenDlgClbk(NULL);
				open_my_dialog = false;
			}

			//first_loading is set to false at the end of the function
		}


		//	more initialization code

		VECTOR3 noteColor = {1,1,1};
		hNote = oapiCreateAnnotation(false,1,noteColor);
		last_time = oapiGetSysTime();
		oapiAnnotationSetText(hNote,MODULE_NAME_SHORT" "MODULE_VERSION);
		oapiAnnotationSetPos(hNote,0.15,0.02,1,0.2);
		//oapiAnnotationSetSize(hNote,1);
	}

	first_loading = false;
}

void MyModule::clbkPostStep(double simt, double simdt, double mjd)
{
	if(hNote != NULL)
	{	double time = oapiGetSysTime();
		if(time - last_time >= 5)
		{	oapiDelAnnotation(hNote);
			hNote = NULL;
		}
	}
}

void MyModule::clbkFocusChanged(OBJHANDLE new_focus, OBJHANDLE old_focus)
{
	if(old_focus == hVessel && oapiIsVessel(new_focus))
		hVessel = new_focus;
}

// ==============================================================
// This function is called when Orbiter shuts down or when the
// module is deactivated

DLLCLBK void ExitModule (HINSTANCE hDLL)
{
	// Unregister the custom function in Orbiter
	oapiUnregisterCustomCmd (g_dwCmd);
}

// ==============================================================
// Write some parameters to the scenario file

DLLCLBK void opcSaveState (FILEHANDLE scn)
{
	if(active)
	{
		oapiWriteScenario_string (scn, "\tActive", "true");
		if(hMyDlg != NULL)
			oapiWriteScenario_string (scn, "\tWindow_shown", "true");
		else oapiWriteScenario_string (scn, "\tWindow_shown", "false");
		oapiWriteScenario_int (scn, "\tWindow_x", (int)my_dialog_pos.left);
		oapiWriteScenario_int (scn, "\tWindow_y", (int)my_dialog_pos.top);

		char vesselName[VESSEL_NAME_LEN] = "";
		oapiGetObjectName(hVessel,vesselName,VESSEL_NAME_LEN);
		oapiWriteScenario_string (scn, "\tShip", vesselName);
	}
}

// ==============================================================
// Read custom parameters from scenario

DLLCLBK void opcLoadState (FILEHANDLE scn)
{
	if(!first_loading)
		return;

	char *line;
	char string[100];
	while (oapiReadScenario_nextline (scn, line))
	{
		if (!_strnicmp (line, "Active", 6))
		{	sscanf (line+6, "%s", string);
			if(!_strnicmp (string, "true", 4))
				active = true;
			else return;
		}
		else if (!_strnicmp (line, "Window_shown", 12))
		{	sscanf (line+12, "%s", string);
			if(!_strnicmp (string, "true", 4))
				open_my_dialog = true;
		}
		else if (!_strnicmp (line, "Window_x", 8))
			sscanf (line+8, "%d", &my_dialog_pos.left);
		else if (!_strnicmp (line, "Window_y", 8))
			sscanf (line+8, "%d", &my_dialog_pos.top);
		else if (!_strnicmp (line, "Ship", 4))
		{	char vesselName[VESSEL_NAME_LEN] = "";
			sscanf ( line+4, "%s", vesselName );
			hVessel = oapiGetObjectByName(vesselName);
			OBJHANDLE hFocusedVessel = oapiGetFocusObject();
			if(hVessel == NULL || !oapiIsVessel(hVessel))
				hVessel = hFocusedVessel;
		}
	}
}

// ==============================================================
// Open the dialog window

void OpenDlgClbk (void *context)
{
	hMyDlg = oapiOpenDialog (g_hInst, IDD_MYDIALOG, MsgProc);

	SetParent(myFrame->GetHWND(), hMyDlg);
	myFrame->Centre(wxBOTH);

	// Don't use a standard Windows function like CreateWindow to
	// open the dialog box, because it won't work in fullscreen mode
}

// ==============================================================
// Close the dialog

void CloseDlg (HWND hDlg)
{
	oapiCloseDialog (hDlg);

	hMyDlg = NULL;
}

// ==============================================================
// Windows message handler for the dialog box

BOOL CALLBACK MsgProc (HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_INITDIALOG:
			SetWindowPos(hDlg, HWND_TOPMOST, my_dialog_pos.left, my_dialog_pos.top, 0, 0, SWP_NOSIZE);


			//	create controls

			button_activate_module = CreateWindow(L"BUTTON",L"", WS_CHILD | WS_VISIBLE, 10, 10, 100, 25, hDlg,
									(HMENU)IDC_ACTIVATE_MODULE,NULL,NULL);
			update_button_activate_module();

			return TRUE;

		case WM_COMMAND:
			switch (LOWORD (wParam))
			{
				case IDC_ACTIVATE_MODULE:
					active = !active;
					update_button_activate_module();
					return TRUE;

				case IDCANCEL: // dialog closed by user
					CloseDlg (hDlg);
					return TRUE;
			}
			break;

		case WM_MOVE:
			GetWindowRect(hMyDlg,&my_dialog_pos);
			break;
	}

	return oapiDefDialogProc (hDlg, uMsg, wParam, lParam);
}




// ==============================================================
// Controls
// ==============================================================
void update_button_activate_module(void)
{
	if(active)
		SetWindowText(button_activate_module,L"Deactivate");
	else SetWindowText(button_activate_module,L"Activate");
}
