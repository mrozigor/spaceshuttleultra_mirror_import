#include "../Atlantis.h"
#include "MDU.h"
#include "../dps/IDP.h"
#include "../meshres_vc_additions.h"


namespace vc
{
	MDU::MDU(Atlantis* _sts, const string& _ident, unsigned short _usMDUID, bool _bUseCRTMFD)
		: AtlantisVCComponent(_sts, _ident), usMDUID(_usMDUID),
		prim_idp(NULL), sec_idp(NULL), bUseSecondaryPort(false),
		bInverseX(false), counting(false)
	{
		_sts->RegisterMDU(_usMDUID, this);
		//Clear text buffer
		//Create display buffer
		//Clear display buffer
		shLabelTex = NULL;
		
		// set default button positions
		btnPwrXmin = 0.038f; btnPwrXmax = 0.099f;
		btnPwrYmin = 0.8350f; btnPwrYmax = 0.9144f;
		btnBrtXmin = 0.9257f; btnBrtXmax = 0.9929f;
		btnBrtYmin = 0.8350f; btnBrtYmax = 0.9144f;
		edgekeyXmin = 0.2237f; edgekeyXmax = 0.7939f;
		edgekeyYmin = 0.9185f; edgekeyYmax = 0.9601f;

		CreateGDIObjects();
		CreateSketchpadObjects();

		hADIball = gcLoadSketchMesh( "SSU\\ADI_MEDS" );

		Tape_Create();
		ADI_Create();
		//HSI_Create();

		display = 0;
		menu = 3;
	}

	MDU::~MDU()
	{
		DestroyGDIObjects();
		DestroySketchpadObjects();

		if (hADIball) gcDeleteSketchMesh( hADIball );
	}

	bool MDU::OnReadState( FILEHANDLE scn )
	{
		char* line;

		while (oapiReadScenario_nextline( scn, line ))
		{
			if (!_strnicmp( line, "@ENDOBJECT", 10 ))
			{
				return true;
			}
			else if (!_strnicmp( line, "DISPLAY", 7 ))
			{
				sscanf_s( (char*)(line + 7), "%d", &display );
			}
			else if (!_strnicmp( line, "MENU", 4 ))
			{
				sscanf_s( (char*)(line + 4), "%d", &menu );
			}
		}
		return false;
	}

	void MDU::OnSaveState( FILEHANDLE scn ) const
	{
		oapiWriteScenario_int( scn, "DISPLAY", display );
		oapiWriteScenario_int( scn, "MENU", menu );
		return;
	}

	bool MDU::DefineRegionAID(UINT aid)
	{
		AddAIDToRedrawEventList(aid);
		return true;	
	}


	void MDU::DefineVCAnimations(UINT vc_idx) 
	{
		mfdspec.nmesh = vc_idx;
		//
	}

	/*void MDU::DrawCommonHeader(const char* cDispTitle)
	{
		char cbuf[200];
		char cspecbuf[4];
		char cdispbuf[4];
		char cUplink[3];
		unsigned short usDay, usHour, usMinute, usSecond;
		strcpy_s(cUplink, "  ");
		strcpy_s(cspecbuf, "   ");
		strcpy_s(cdispbuf, "   ");

		if(GetIDP()->GetSpec() != dps::MODE_UNDEFINED)
		{
			sprintf_s(cspecbuf, 4, "%03d", GetIDP()->GetSpec());
		}
		


		if(GetIDP()->GetDisp() != dps::MODE_UNDEFINED)
		{
			sprintf_s(cdispbuf, 4, "%03d", GetIDP()->GetDisp());
		}

		STS()->GetGPCMET(1, usDay, usHour, usMinute, usSecond);
	
		//Todo: GPC count their own MET independent of the MTU
		sprintf(cbuf,"%03d1/%03s/%3s    %14s  %2s  %1d %03d/%02d:%02d:%02d", 
			GetIDP()->GetOps(), 
			cspecbuf, 
			cdispbuf, 
			cDispTitle, 
			cUplink, 
			0, 
			usDay, usHour, usMinute, usSecond);

	
		mvprint(0, 0, cbuf);
	}*/

	short MDU::GetPortConfig() const
	{
		return 0;
	}

	bool MDU::GetSelectedPort() const
	{
		return bUseSecondaryPort;
	}

	bool MDU::GetViewAngle() const
	{
		return bInverseX;
	}

	bool MDU::SetPrimaryIDP(dps::IDP *idp)
	{
		if(idp) {
			prim_idp=idp;
			return true;
		}
		else return false;
	}

	bool MDU::SetSecondaryIDP(dps::IDP *idp)
	{
		if(idp) {
			sec_idp=idp;
			return true;
		}
		else return false;
	}

	
	void MDU::SetPowerButtonArea(float xmin, float ymin, float xmax, float ymax)
	{
		btnPwrXmin = xmin;
		btnPwrXmax = xmax;
		btnPwrYmin = ymin;
		btnPwrYmax = ymax;
	}

	void MDU::SetBrightnessButtonArea(float xmin, float ymin, float xmax, float ymax)
	{
		btnBrtXmin = xmin;
		btnBrtXmax = xmax;
		btnBrtYmin = ymin;
		btnBrtYmax = ymax;
	}

	void MDU::SetEdgekeyArea(float xmin, float ymin, float xmax, float ymax)
	{
		edgekeyXmin = xmin;
		edgekeyXmax = xmax;
		edgekeyYmin = ymin;
		edgekeyYmax = ymax;
	}

	bool MDU::OnMouseEvent(int _event, float x, float y)
	{
		//sprintf_s(oapiDebugString(), 80, "MDU %s mouse event %d (%f, %f)", GetQualifiedIdentifier().c_str(), _event, x, y);
		if(y >= btnPwrYmin && y<= btnPwrYmax && x >= btnPwrXmin && x <= btnPwrXmax)
		{
			if(_event & PANEL_MOUSE_LBDOWN)
			{
				//sprintf_s(oapiDebugString(), 80, "MDU %s POWER ON/OFF", GetQualifiedIdentifier().c_str());
				oapiSendMFDKey(usMDUID, OAPI_KEY_ESCAPE);
			}
		}
		else if(y >= btnPwrYmin && y<= btnPwrYmax && x >= btnPwrXmin && x <= btnPwrXmax)
		{
			//sprintf_s(oapiDebugString(), 80, "MDU %s BRIGHTNESS", GetQualifiedIdentifier().c_str());
		}
		else if (y >= edgekeyYmin && y <= edgekeyYmax)
		{
			//const float edgekeyWidth = 0.0661;
			//const float edgekeySpace = 0.12068;
			float edgekeyClickPos = (x-edgekeyXmin)/(edgekeyXmax-edgekeyXmin); // calculate horizontal position of click relative to left edge of edgekey area (scaled between 0 and 1)
			if(edgekeyClickPos >= 0.0 && edgekeyClickPos <= 0.1)
			{
				if(_event & PANEL_MOUSE_LBDOWN)
				{
					//sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 1", GetQualifiedIdentifier().c_str());
					oapiProcessMFDButton (usMDUID, 0, _event);
				}
			}
			else if(edgekeyClickPos >= 0.18 && edgekeyClickPos <= 0.28)
			{
				if(_event & PANEL_MOUSE_LBDOWN)
				{
					//sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 2", GetQualifiedIdentifier().c_str());
					oapiProcessMFDButton (usMDUID, 1, _event);
				}
			}
			else if(edgekeyClickPos >= 0.36 && edgekeyClickPos <= 0.46)
			{
				if(_event & PANEL_MOUSE_LBDOWN)
				{
					//sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 3", GetQualifiedIdentifier().c_str());
					oapiProcessMFDButton (usMDUID, 2, _event);
				}
			} 
			else if(edgekeyClickPos >= 0.54 && edgekeyClickPos <= 0.64)
			{
				if(_event & PANEL_MOUSE_LBDOWN)
				{
					//sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 4", GetQualifiedIdentifier().c_str());
					oapiProcessMFDButton (usMDUID, 3, _event);
				}
			}
			else if(edgekeyClickPos >= 0.72 && edgekeyClickPos <= 0.82)
			{
				if(_event & PANEL_MOUSE_LBDOWN)
				{
					//sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 5", GetQualifiedIdentifier().c_str());
					oapiProcessMFDButton (usMDUID, 4, _event);
				}
			}
			else if(edgekeyClickPos >= 0.90 && edgekeyClickPos <= 1.0)
			{
				if (_event & PANEL_MOUSE_LBDOWN) {
					t0 = oapiGetSysTime();
					//sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 6 (%f)", GetQualifiedIdentifier().c_str(), t0);
					counting = true;
				} else if ((_event & PANEL_MOUSE_LBUP) && counting) {
					//sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 6: SWITCH PAGE", GetQualifiedIdentifier().c_str());
					oapiSendMFDKey (usMDUID, OAPI_KEY_F2);
					counting = false;
				} else if ((_event & PANEL_MOUSE_LBPRESSED) && counting && (oapiGetSysTime()-t0 >= 1.0)) {
					//sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 6: SWITCH MODE", GetQualifiedIdentifier().c_str());
					oapiSendMFDKey (usMDUID, OAPI_KEY_F1);
					counting = false;		
				}
			}
			//else sprintf_s(oapiDebugString(), 80, "MDU %s EDGEKEYS: %f", GetQualifiedIdentifier().c_str(), edgekeyClickPos);
		}
		return true;
	}

	void MDU::PaintDisplay( oapi::Sketchpad* skp )
	{
		switch (display)
		{
			case 0:// "DPS display"
				if (gcEnabled() && (gcSketchpadVersion( skp ) == 2)) DPS( skp );
				else
				{
					HDC hDC = skp->GetDC();
					if (hDC) DPS( hDC );
				}
				break;
			case 1:// A/E PFD
				if (gcEnabled() && (gcSketchpadVersion( skp ) == 2)) AEPFD( skp );
				else
				{
					HDC hDC = skp->GetDC();
					if (hDC) AEPFD( hDC );
				}
				break;
			case 2:// ORBIT PFD
				if (gcEnabled() && (gcSketchpadVersion( skp ) == 2)) ORBITPFD( (oapi::Sketchpad2*)skp );
				else
				{
					HDC hDC = skp->GetDC();
					if (hDC) ORBITPFD( hDC );
				}
				break;
			case 3:// OMS/MPS
				OMSMPS( skp );
				break;
			case 4:// HYD/APU
				APUHYD( skp );
				break;
			case 5:// SPI
				SPI( skp );
				break;
			case 6:// CST Menu
				SystemStatusDisplay_CSTMenu( skp );
				break;
			case 7:// IDP Interactive CST
				SystemStatusDisplay_IDPInteractiveCST( skp );
				break;
		}
		return;
	}

	int MDU::NavigateMenu( DWORD key )
	{
		switch (menu)
		{
			case 0:// MAIN MENU
				switch (key)
				{
					case OAPI_KEY_1:
						menu = 1;
						return 1;
					case OAPI_KEY_2:
						menu = 2;
						return 1;
					case OAPI_KEY_3:
						menu = 3;
						display = 0;
						return 3;
					case OAPI_KEY_4:
						menu = 4;
						display = 6;
						return 3;
				}
				break;
			case 1:// FLT INST
				switch (key)
				{
					case OAPI_KEY_U:
						menu = 0;
						return 1;
					case OAPI_KEY_1:
						display = 1;
						return 3;
					case OAPI_KEY_2:
						display = 2;
						return 3;
				}
				break;
			case 2:// SUBSYS STATUS
				switch (key)
				{
					case OAPI_KEY_U:
						menu = 0;
						return 1;
					case OAPI_KEY_1:
						display = 3;
						return 3;
					case OAPI_KEY_2:
						display = 4;
						return 3;
					case OAPI_KEY_3:
						display = 5;
						return 3;
				}
				break;
			case 3:// DPS MENU
				switch (key)
				{
					case OAPI_KEY_U:
						menu = 0;
						return 1;
				}
				break;
			case 4:// MAINTENANCE MENU
				switch (key)
				{
					case OAPI_KEY_U:
						menu = 0;
						return 1;
					case OAPI_KEY_3:
						menu = 5;
						return 1;
				}
				break;
			case 5:// CST MENU SELECTION
				switch (key)
				{
					case OAPI_KEY_U:
						menu = 4;
						display = 6;
						return 3;
					case OAPI_KEY_2:
						menu = 6;
						display = 7;
						return 3;
				}
				break;
			case 6:// IDPx INTERACTIVE CST
				switch (key)
				{
					case OAPI_KEY_U:
						menu = 5;
						return 1;
				}
				break;
		}
		return 0;
	}

	char* MDU::ButtonLabel( int bt )
	{
		static char *label[7][5] = {{"", "FLT", "SUB", "DPS", "MEDS1"},
			{"UP", "A/E", "ORBIT", "", ""},
			{"UP", "OMS", "HYD", "SPI", ""},
			{"UP", "", "", "", ""},
			{"UP", "", "", "CST", ""},
			{"UP", "", "S_IDP", "", ""},
			{"UP", "", "", "", ""}};

		return ((menu < 7 && bt < 5) ? label[menu][bt] : NULL);
	}

	int MDU::ButtonMenu( const MFDBUTTONMENU **menu ) const
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

		if (menu) *menu = mnu[this->menu];
		return 5;// return the number of buttons used
	}

	bool MDU::PaintEdgeMenu( oapi::Sketchpad* skp )
	{
		if (oapiGetMFDMode(usMDUID) != MFD_NONE)
		{
			skp->SetPen( skpCyanPen );
			skp->Line( 0, 1, 255, 1 );
			skp->SetFont( skpTahomaFont_h10w4 );
			skp->SetTextColor( CR_CYAN );
			skp->SetTextAlign( oapi::Sketchpad::CENTER );
			skp->SetBackgroundMode( oapi::Sketchpad::BK_TRANSPARENT );
			const char *label;
			int x = 28;
			
			for (int bt = 0; bt < 5; bt++) {
				if (label = oapiMFDButtonLabel (usMDUID, bt))
				{
					if (strcmp( label, "UP" ) == 0)
					{
						// draw up arrow
						skp->MoveTo( 21, 40 );
						skp->LineTo( 21, 31 );
						skp->LineTo( 13, 31 );
						skp->LineTo( 28, 22 );
						skp->LineTo( 43, 31 );
						skp->LineTo( 35, 31 );
						skp->LineTo( 35, 41 );
						skp->Text( x, 25, "UP", 2);
						DrawMenuButton( skp, x );
					}
					else if (strcmp( label, "FLT" ) == 0)
					{
						skp->Text( x, 21, "FLT ", 4 );
						skp->Text( x, 30, "INST", 4 );
						DrawMenuButton( skp, x );
					}
					else if (strcmp( label, "SUB" ) == 0)
					{
						skp->Text( x, 21, "SUBSYS", 6 );
						skp->Text( x, 30, "STATUS", 6 );
						DrawMenuButton( skp, x );
					}
					else if (strcmp( label, "A/E" ) == 0)
					{
						if (display == 1)
						{
							skp->SetTextColor( CR_WHITE );
							skp->Text( x, 21, "A/E", 3 );
							skp->Text( x, 30, "PFD", 3 );
							skp->SetTextColor( CR_CYAN );
							skp->SetPen( skpWhitePen );
							DrawMenuButton( skp, x );
							skp->SetPen( skpCyanPen );
						}
						else
						{
							skp->Text( x, 21, "A/E", 3 );
							skp->Text( x, 30, "PFD", 3 );
							DrawMenuButton( skp, x );
						}
					}
					else if (strcmp( label, "ORBIT" ) == 0)
					{
						if (display == 2)
						{
							skp->SetTextColor( CR_WHITE );
							skp->Text( x, 21, "ORBIT", 5 );
							skp->Text( x, 30, "PFD", 3 );
							skp->SetTextColor( CR_CYAN );
							skp->SetPen( skpWhitePen );
							DrawMenuButton( skp, x );
							skp->SetPen( skpCyanPen );
						}
						else
						{
							skp->Text( x, 21, "ORBIT", 5 );
							skp->Text( x, 30, "PFD", 3 );
							DrawMenuButton( skp, x );
						}
					}
					else if (strcmp( label, "OMS" ) == 0)
					{
						if (display == 3)
						{
							skp->SetTextColor( CR_WHITE );
							skp->Text( x, 21, "OMS/", 4 );
							skp->Text( x, 30, " MPS", 4 );
							skp->SetTextColor( CR_CYAN );
							skp->SetPen( skpWhitePen );
							DrawMenuButton( skp, x );
							skp->SetPen( skpCyanPen );
						}
						else
						{
							skp->Text( x, 21, "OMS/", 4 );
							skp->Text( x, 30, " MPS", 4 );
							DrawMenuButton( skp, x );
						}
					}
					else if (strcmp( label, "HYD" ) == 0)
					{
						if (display == 4)
						{
							skp->SetTextColor( CR_WHITE );
							skp->Text( x, 21, "HYD/", 4 );
							skp->Text( x, 30, " APU", 4 );
							skp->SetTextColor( CR_CYAN );
							skp->SetPen( skpWhitePen );
							DrawMenuButton( skp, x );
							skp->SetPen( skpCyanPen );
						}
						else
						{
							skp->Text( x, 21, "HYD/", 4 );
							skp->Text( x, 30, " APU", 4 );
							DrawMenuButton( skp, x );
						}
					}
					else if (strcmp( label, "SPI" ) == 0)
					{
						if (display == 5)
						{
							skp->SetTextColor( CR_WHITE );
							skp->Text( x, 21, "SPI", 3 );
							skp->SetTextColor( CR_CYAN );
							skp->SetPen( skpWhitePen );
							DrawMenuButton( skp, x );
							skp->SetPen( skpCyanPen );
						}
						else
						{
							skp->Text( x, 21, "SPI", 3 );
							DrawMenuButton( skp, x );
						}
					}
					else if (strcmp( label, "DPS" ) == 0)
					{
						skp->Text( x, 21, "DPS", 3 );
						DrawMenuButton( skp, x );
					}
					else if (strcmp( label, "MEDS1" ) == 0)
					{
						skp->Text( x + 1, 21, "MEDS", 4 );
						skp->Text( x, 30, "MAINT", 5 );
						DrawMenuButton( skp, x );
					}
					else if (strcmp( label, "CST" ) == 0)
					{
						skp->Text( x, 21, "CST", 3 );
						DrawMenuButton( skp, x );
					}
					else if (strcmp( label, "S_IDP" ) == 0)
					{
						skp->Text( x, 21, "START", 5 );
						skp->Text( x, 30, " IDP", 4 );
						DrawMenuButton( skp, x );
					}
					else
					{
						skp->Text( x, 25, label, strlen(label) );
						DrawMenuButton( skp, x );
					}
					x += 39;
				} else break;
			}

			skp->Text( 223, 25, "PG", 2 );
			DrawMenuButton( skp, 223 );

			// print title
			switch (menu)
			{
				case 0:
					skp->Text( 112, 9, "MAIN MENU", 9 );
					break;
				case 1:
					skp->Text( 112, 9, "FLIGHT INSTRUMENTATION MENU", 27 );
					break;
				case 2:
					skp->Text( 112, 9, "SUBSYSTEM MENU", 14 );
					break;
				case 3:
					skp->Text( 112, 9, "DPS MENU", 8 );
					break;
				case 4:
					skp->Text( 112, 9, "MAINTENANCE MENU", 16 );
					break;
				case 5:
					skp->Text( 112, 9, "CST MENU SELECTION", 18 );
					break;
				case 6:
					{
						char buf[32];
						sprintf_s( buf, 32, "IDP%d INTERACTIVE CST", GetIDP()->GetIDPID() );
						skp->Text( 112, 9, buf, strlen( buf ) );
					}
					break;
				default:
					// print nothing
					break;
			}
		}
		return false;
	}

	void MDU::DrawMenuButton( oapi::Sketchpad* skp, int x )
	{
		skp->MoveTo( x - 18, 40 );
		skp->LineTo( x - 18, 20 );
		skp->LineTo( x + 18, 20 );
		skp->LineTo( x + 18, 41 );
		return;
	}

	bool MDU::RealizeMFD(int id)
	{
		if(id>=0) RegisterMFDContext(id);
		return false;
	}

	void MDU::RegisterMFDContext(int id)
	{
//		char pszBuffer[256];
		mfdspec.nbt1 = 5;
		mfdspec.nbt2 = 0;
		mfdspec.flag = MFD_SHOWMODELABELS;
		mfdspec.bt_yofs  = 256/6;
		mfdspec.bt_ydist = 256/7;
		oapiRegisterMFD (id, &mfdspec);
		//sprintf_s(pszBuffer, 256, "MFD %s (%d) registered", GetQualifiedIdentifier().c_str(), usMDUID);
		//oapiWriteLog(pszBuffer);
	}

	void MDU::RegisterVC()
	{
		AddAIDToRedrawEventList(AID_CDR1_LABEL+usMDUID);
		//register lower label texture for redrawing
		SURFHANDLE label_tex;
		if(usMDUID==MDUID_PLT2 || usMDUID==MDUID_CRT4 || usMDUID==MDUID_AFD) label_tex=oapiGetTextureHandle(STS()->hOrbiterVCMesh, TEX_LABEL2_VC);
		else label_tex=oapiGetTextureHandle(STS()->hOrbiterVCMesh, TEX_LABEL_VC);

		RECT labelArea;
		switch(usMDUID) {
			case MDUID_CDR1:
			case MDUID_PLT2:
				labelArea=_R(0, 0, 256, 41);
				break;
			case MDUID_CDR2:
			case MDUID_CRT4:
				labelArea=_R(0, 61, 256, 102);
				break;
			case MDUID_CRT1:
			case MDUID_AFD:
				labelArea=_R(0, 121, 256, 162);
				break;
			case MDUID_CRT2:
				labelArea=_R(0, 180, 256, 221);
				break;
			case MDUID_CRT3:
				labelArea=_R(0, 247, 256, 288);
				break;
			case MDUID_MFD1:
				labelArea=_R(0, 308, 256, 349);
				break;
			case MDUID_MFD2:
				labelArea=_R(0, 368, 256, 409);
				break;
			case MDUID_PLT1:
				labelArea=_R(0, 427, 256, 468);
				break;
		}

		oapiVCRegisterArea(AID_CDR1_LABEL+usMDUID, labelArea, PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_BACKGROUND, label_tex);
	}

	bool MDU::OnVCRedrawEvent(int id, int _event, SURFHANDLE surf)
	{
		if(id==(AID_CDR1_LABEL+usMDUID)) {
			oapi::Sketchpad* skp = oapiGetSketchpad( surf );
			if (skp)
			{
				PaintEdgeMenu( skp );
				oapiReleaseSketchpad( skp );
				return true;
			}
		}
		return false;
	}

	bool MDU::DefineVCGroup(UINT mgrp)
	{
		mfdspec.ngroup = mgrp;
		return true;
	}

	bool MDU::DefineVCTexture(SURFHANDLE tex)
	{
		shLabelTex = tex;
		return true;
	}
	
	bool MDU::IsCRTBufferEnabled() const
	{
		return false;
	}

	void MDU::UpdateTextBuffer()
	{
		for(int i=0;i<51;i++) {
			for(int j=0;j<26;j++) {
				textBuffer[i][j].cSymbol=0;
			}
		}

		lines.clear();
		ellipses.clear();

		if(prim_idp) {
			prim_idp->OnPaint(this);
		}
		else {
			PrintToBuffer("ERROR: IDP NOT CONNECTED", 24, 0, 0, 0);
		}
		
		//print Scratch Pad line
		const char* scratch_pad=prim_idp->GetScratchPadLineString();
		PrintToBuffer(scratch_pad, strlen(scratch_pad), 0, 25, 0);
	}

	void MDU::PrintToBuffer(const char* string, int length, int col, int row, char attributes)
	{
		if((col+length) > 51) {
			char pszBuf[255];
			sprintf_s(pszBuf, 255, "ERROR: MDU %d text buffer overflow", static_cast<int>(usMDUID));
			oapiWriteLog(pszBuf);
			strcpy(oapiDebugString(), pszBuf);

			return;
		}
		for(int i=0;i<length;i++) {
			textBuffer[col+i][row].cSymbol=string[i];
			textBuffer[col+i][row].cAttr=attributes;
		}
	}

	unsigned short MDU::GetDrivingIDP() const
	{
		if(this->bUseSecondaryPort)
		{
			if(sec_idp)
			{
				return sec_idp->GetIDPID();
			} else {
				return 100;
			}
		} else {
			if(prim_idp)
			{
				return prim_idp->GetIDPID();
			} else {
				return 100;
			}
		}
		
	}

	/*void MDU::GPCMEMORY()
	{
	
		DrawCommonHeader("GPC MEMORY");

		Line(16, 3, 16, 22);
		Line(10, 7, 10, 22);
		Line(1, 22, 16, 22);
		Line(16, 14, 50, 14);
		Line(16, 20, 50, 20);
		Line(19, 14, 19, 24);

		mvprint(1, 3, "MEM/BUS CONFIG");
		mvprint(1, 4, "1 CONFIG");
		mvprint(1, 5, "2 GPC");
		mvprint(2, 7, "STRING 1");
		mvprint(9, 8, "2");
		mvprint(9, 9, "3");
		mvprint(9, 10, "4");
		mvprint(4, 11, "PL 1/2");
		mvprint(5, 13, "CRT 1");
		mvprint(9, 14, "2");
		mvprint(9, 15, "3");
		mvprint(9, 16, "4");
		mvprint(2, 18, "LAUNCH 1");
		mvprint(9, 19, "2");
		mvprint(6, 20, "MM 1");
		mvprint(9, 21, "2");
		mvprint(18, 3, "READ/WRITE");
		mvprint(19, 4, "DATA 20");
		mvprint(29, 4, "BIT SET 22");
		mvprint(42, 4, "SEQ ID 24");
		mvprint(19, 5, "CODE 21");
		mvprint(29, 5, "BIT RST 23");
		mvprint(42, 5, "WRITE 25");
		mvprint(19, 6, "26 ENG UNITS");
	}*/
	
	void MDU::CreateGDIObjects()
	{
		gdiBlackBrush = CreateSolidBrush( CR_BLACK );
		gdiDarkGrayBrush = CreateSolidBrush( CR_DARK_GRAY );
		gdiLightGrayBrush = CreateSolidBrush( CR_LIGHT_GRAY );
		gdiWhiteBrush = CreateSolidBrush( CR_WHITE );
		gdiRedBrush = CreateSolidBrush( CR_RED );
		gdiYellowBrush = CreateSolidBrush( CR_YELLOW );
		gdiMagentaBrush = CreateSolidBrush( CR_MAGENTA );
		gdiLightGreenBrush = CreateSolidBrush( CR_LIGHT_GREEN );

		gdiBlackPen = CreatePen( PS_SOLID, 0, CR_BLACK );
		gdiDarkGrayPen = CreatePen( PS_SOLID, 0, CR_DARK_GRAY );
		gdiDarkGrayThickPen = CreatePen( PS_SOLID, 3, CR_DARK_GRAY );
		gdiLightGrayPen = CreatePen( PS_SOLID, 0, CR_LIGHT_GRAY );
		gdiWhitePen = CreatePen( PS_SOLID, 0, CR_WHITE );
		gdiRedPen = CreatePen( PS_SOLID, 0, CR_RED );
		gdiYellowPen = CreatePen( PS_SOLID, 0, CR_YELLOW );
		gdiCyanPen = CreatePen( PS_SOLID, 0, CR_CYAN );
		gdiMagentaPen = CreatePen( PS_SOLID, 0, CR_MAGENTA );
		gdiLightGreenPen = CreatePen( PS_SOLID, 0, CR_LIGHT_GREEN );
		gdiLightGreenThickPen = CreatePen( PS_SOLID, 2, CR_LIGHT_GREEN );

		gdiOverbrightPen = CreatePen( PS_SOLID, 0, CR_DPS_OVERBRIGHT );
		gdiNormalPen = CreatePen( PS_SOLID, 0, CR_DPS_NORMAL );
		LOGBRUSH lb = {BS_SOLID, CR_DPS_NORMAL, 0};
		DWORD pstyle[2] = {2, 1};
		gdiDashedNormalPen = ExtCreatePen( PS_COSMETIC | PS_USERSTYLE, 1, &lb, 2, pstyle );

		gdiTahomaFont_h10w4 = CreateFont( 10, 4, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, OEM_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FIXED_PITCH, "Tahoma" );
		gdiTahomaFont_h7w3 = CreateFont( 7, 3, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, OEM_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FIXED_PITCH, "Tahoma" );
		gdiTahomaFont_h17w6 = CreateFont( 17, 6, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, OEM_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FIXED_PITCH, "Tahoma" );
		return;
	}

	void MDU::DestroyGDIObjects()
	{
		DeleteObject( gdiBlackBrush );
		DeleteObject( gdiDarkGrayBrush );
		DeleteObject( gdiLightGrayBrush );
		DeleteObject( gdiWhiteBrush );
		DeleteObject( gdiRedBrush );
		DeleteObject( gdiYellowBrush );
		DeleteObject( gdiMagentaBrush );
		DeleteObject( gdiLightGreenBrush );

		DeleteObject( gdiBlackPen );
		DeleteObject( gdiDarkGrayPen );
		DeleteObject( gdiDarkGrayThickPen );
		DeleteObject( gdiLightGrayPen );
		DeleteObject( gdiWhitePen );
		DeleteObject( gdiRedPen );
		DeleteObject( gdiYellowPen );
		DeleteObject( gdiCyanPen );
		DeleteObject( gdiMagentaPen );
		DeleteObject( gdiLightGreenPen );
		DeleteObject( gdiLightGreenThickPen );

		DeleteObject( gdiOverbrightPen );
		DeleteObject( gdiNormalPen );
		DeleteObject( gdiDashedNormalPen );

		DeleteObject( gdiTahomaFont_h10w4 );
		DeleteObject( gdiTahomaFont_h7w3 );
		DeleteObject( gdiTahomaFont_h17w6 );
		return;
	}

	void MDU::CreateSketchpadObjects()
	{
		skpBlackBrush = oapiCreateBrush( CR_BLACK );
		skpDarkGrayBrush = oapiCreateBrush( CR_DARK_GRAY );
		skpLightGrayBrush = oapiCreateBrush( CR_LIGHT_GRAY );
		skpWhiteBrush = oapiCreateBrush( CR_WHITE );
		skpRedBrush = oapiCreateBrush( CR_RED );
		skpYellowBrush = oapiCreateBrush( CR_YELLOW );
		skpCyanBrush = oapiCreateBrush( CR_CYAN );
		skpMagentaBrush = oapiCreateBrush( CR_MAGENTA );
		skpLightGreenBrush = oapiCreateBrush( CR_LIGHT_GREEN );
		skpBlueBrush = oapiCreateBrush( CR_BLUE );

		skpBlackPen = oapiCreatePen( 1, 1, CR_BLACK );
		skpDarkGrayPen = oapiCreatePen( 1, 1, CR_DARK_GRAY );
		skpDarkGrayThickPen = oapiCreatePen( 1, 2, CR_DARK_GRAY );
		skpLightGrayPen = oapiCreatePen( 1, 1, CR_LIGHT_GRAY );
		skpWhitePen = oapiCreatePen( 1, 1, CR_WHITE );
		skpRedPen = oapiCreatePen( 1, 1, CR_RED );
		skpYellowPen = oapiCreatePen( 1, 1, CR_YELLOW );
		skpCyanPen = oapiCreatePen( 1, 1, CR_CYAN );
		skpMagentaPen = oapiCreatePen( 1, 1, CR_MAGENTA );
		skpLightGreenPen = oapiCreatePen( 1, 1, CR_LIGHT_GREEN );
		skpDarkGreenPen = oapiCreatePen( 1, 1, CR_DARK_GREEN );
		skpLightGreenThickPen = oapiCreatePen( 1, 2, CR_LIGHT_GREEN );
		
		skpOverbrightPen = oapiCreatePen( 1, 1, CR_DPS_OVERBRIGHT );
		skpNormalPen = oapiCreatePen( 1, 1, CR_DPS_NORMAL );
		skpDashedNormalPen = oapiCreatePen( 2, 1, CR_DPS_NORMAL );

		skpTahomaFont_h10w4 = oapiCreateFont( 10, true, "Tahoma" );
		skpTahomaFont_h7w3 = oapiCreateFont( 7, true, "Tahoma" );
		skpTahomaFont_h17w6 = oapiCreateFont( 17, true, "Tahoma" );
		skpArialFont_h15w5 = oapiCreateFont( 15, true, "Arial" );
		skpArialFont_h13w6 = oapiCreateFont( 13, true, "Arial" );
		return;
	}

	void MDU::DestroySketchpadObjects()
	{
		oapiReleaseBrush( skpBlackBrush );
		oapiReleaseBrush( skpDarkGrayBrush );
		oapiReleaseBrush( skpLightGrayBrush );
		oapiReleaseBrush( skpWhiteBrush );
		oapiReleaseBrush( skpRedBrush );
		oapiReleaseBrush( skpYellowBrush );
		oapiReleaseBrush( skpCyanBrush );
		oapiReleaseBrush( skpMagentaBrush );
		oapiReleaseBrush( skpLightGreenBrush );
		oapiReleaseBrush( skpBlueBrush );

		oapiReleasePen( skpBlackPen );
		oapiReleasePen( skpDarkGrayPen );
		oapiReleasePen( skpDarkGrayThickPen );
		oapiReleasePen( skpLightGrayPen );
		oapiReleasePen( skpWhitePen );
		oapiReleasePen( skpRedPen );
		oapiReleasePen( skpYellowPen );
		oapiReleasePen( skpCyanPen );
		oapiReleasePen( skpMagentaPen );
		oapiReleasePen( skpLightGreenPen );
		oapiReleasePen( skpDarkGreenPen );
		oapiReleasePen( skpLightGreenThickPen );

		oapiReleasePen( skpOverbrightPen );
		oapiReleasePen( skpNormalPen );
		oapiReleasePen( skpDashedNormalPen );

		oapiReleaseFont( skpTahomaFont_h10w4 );
		oapiReleaseFont( skpTahomaFont_h7w3 );
		oapiReleaseFont( skpTahomaFont_h17w6 );
		oapiReleaseFont( skpArialFont_h15w5 );
		oapiReleaseFont( skpArialFont_h13w6 );
		return;
	}
};
