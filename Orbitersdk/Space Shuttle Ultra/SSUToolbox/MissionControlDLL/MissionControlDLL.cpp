//code by Face

// ==============================================================
//                 ORBITER MODULE: DialogTemplate
//                  Part of the ORBITER SDK
//            Copyright (C) 2003 Martin Schweiger
//                   All rights reserved
//
// DialogTemplate.cpp
//
// This module demonstrates how to build an Orbiter plugin which
// opens a Windows dialog box. This is a good starting point for
// your own dialog-based addons.
// ==============================================================
#define STRICT
#define ORBITER_MODULE

#include <vcclr.h>
#include "Launcher.h"

#pragma unmanaged

#include "orbitersdk.h"

#include "windows.h"
#include "resource.h"

// ==============================================================
// Global variables
// ==============================================================

HINSTANCE g_hInst;  // module instance handle
DWORD g_dwCmd;      // custom function identifier

// ==============================================================
// Local prototypes
// ==============================================================

void OpenDlgClbk (void *context);
BOOL CALLBACK MsgProc (HWND, UINT, WPARAM, LPARAM);

#pragma managed

using namespace SSU::Toolbox::Client;
using namespace System;
using namespace System::IO;
using namespace System::Text;
using namespace System::Reflection;
using namespace System::Runtime::InteropServices;

Wrapper wrapper(gcnew Launcher());

namespace SSU
{
	namespace Toolbox
	{
		namespace Client
		{
			void __stdcall CreateUnmanagedDialog(Wrapper *wrapper, HWND managed){ wrapper->CreateUnmanagedDialog(managed); }
			void __stdcall CloseUnmanagedDialog(Wrapper *wrapper, HWND managed){ wrapper->CloseUnmanagedDialog(managed); }

			Launcher::Launcher()
			{
				String^ pathSSUTB =
					Path::GetDirectoryName(Assembly::GetExecutingAssembly()->Location);
				wchar_t backslash = Path::DirectorySeparatorChar;
				pathSSUTB = pathSSUTB->Remove(pathSSUTB->LastIndexOf(backslash, pathSSUTB->LastIndexOf(backslash)-1));

				pathSSUTB = Path::Combine(pathSSUTB, "SSUToolbox\\SSUToolbox.exe");


				Assembly^ assm=Assembly::LoadFrom(pathSSUTB);
				if (assm!=nullptr)
				{
					for each (Type^ t in assm->GetTypes())
					{
						if (t->Name->EndsWith("Program"))
						{
							t->GetProperty("CreateDialogFunction")->SetValue(nullptr, (IntPtr)CreateUnmanagedDialog, nullptr);
							t->GetProperty("CloseDialogFunction")->SetValue(nullptr, (IntPtr)CloseUnmanagedDialog, nullptr);
							wrapper=t->GetProperty("Wrapper");
							open=t->GetMethod("Open");
							init=t->GetMethod("Init");
							break;
						}
					}
				}				
			}
			void Launcher::SetWrapper(Wrapper *wrapper)
			{
				this->wrapper->SetValue(nullptr, (System::IntPtr)wrapper, nullptr);
			}
			void Launcher::Open(){open->Invoke(nullptr, nullptr);}
			void Launcher::Init(){init->Invoke(nullptr, nullptr);}

			Wrapper::Wrapper(Launcher ^launcher)
			{
				current=NULL;
				this->launcher=launcher;
				launcher->SetWrapper(this);
			}
			HWND Wrapper::GetManaged(HWND unmanaged){return current==NULL?unmanaged2managed[unmanaged]:current;}
			void Wrapper::CreateUnmanagedDialog(HWND managed)
			{
				current=managed;
				HWND unmanaged=oapiOpenDialogEx(g_hInst, IDD_EMPTY, MsgProc, DLG_ALLOWMULTI);
				unmanaged2managed[unmanaged]=managed;
				managed2unmanaged[managed]=unmanaged;
				current=NULL;
			}
			void Wrapper::CloseUnmanagedDialog(HWND managed)
			{
				oapiCloseDialog(managed2unmanaged[managed]);
				unmanaged2managed.erase(managed2unmanaged[managed]);
				managed2unmanaged.erase(managed);
			}
			void Wrapper::Open(){launcher->Open();}
			void Wrapper::Init(){launcher->Init();}
		}
	}
}

#pragma unmanaged

// ==============================================================
// API interface
// ==============================================================

// ==============================================================
// This function is called when Orbiter starts or when the module
// is activated.

DLLCLBK void InitModule (HINSTANCE hDLL)
{
	g_hInst = hDLL; // remember the instance handle
	g_dwCmd = oapiRegisterCustomCmd ("SSU Mission Control",
		"SSU Mission Control.\r\nMission Control for Space Shuttle Ultra",
		OpenDlgClbk, NULL);
}

// ==============================================================
// This function is called when Orbiter shuts down or when the
// module is deactivated

DLLCLBK void ExitModule (HINSTANCE hDLL)
{
	//oapiUnregisterCustomCmd (g_dwCmd);	
}

void OpenDlgClbk (void *context)
{	
	wrapper.Open();
}

DLLCLBK void opcOpenRenderViewport (HWND hRenderWnd, DWORD width, DWORD height, BOOL fullscreen)
{
	wrapper.Init();
}

// ==============================================================
// Windows message handler for the dialog box
BOOL CALLBACK MsgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{		
	LONG style;
	HWND hManaged;
	RECT rManaged;
	static char title[1025];
	
	switch (uMsg)
	{
	case WM_SHOWWINDOW:		
		if (wParam==TRUE)
		{
			hManaged=wrapper.GetManaged(hDlg);
			style=GetWindowLong(hManaged, GWL_STYLE);
			SetWindowLong(hManaged, GWL_STYLE, style | WS_CHILD);
			GetWindowRect(hManaged, &rManaged);
			AdjustWindowRect(&rManaged, WS_POPUPWINDOW | WS_CAPTION, false);
			SetParent(hManaged, hDlg);
			SetWindowPos(hDlg, NULL, rManaged.left, rManaged.top, rManaged.right-rManaged.left, rManaged.bottom-rManaged.top, NULL);
			GetWindowText(hManaged, (LPWSTR)title, 1024);
			SetWindowText(hDlg, (LPWSTR)title);
			ShowWindow(hManaged, SW_SHOW);
			GetClientRect(hDlg, &rManaged);
			SendMessage(hManaged, WM_SIZING, WMSZ_BOTTOMRIGHT, (LPARAM)&rManaged);
		}		
		break;		
	case WM_SIZING:
		hManaged=wrapper.GetManaged(hDlg);
		GetClientRect(hDlg, &rManaged);
		SendMessage(hManaged, uMsg, wParam, (LPARAM)&rManaged);
		break;
	case WM_CLOSE:
	case WM_ACTIVATE:
		SendMessage(wrapper.GetManaged(hDlg), uMsg, wParam, lParam);
	}
	return oapiDefDialogProc (hDlg, uMsg, wParam, lParam);
}