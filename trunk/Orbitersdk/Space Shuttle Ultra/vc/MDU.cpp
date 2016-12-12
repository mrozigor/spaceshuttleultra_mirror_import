#include "../Atlantis.h"
#include "MDU.h"
#include "../dps/IDP.h"


namespace vc
{
	HBRUSH MDU::gdiBlackBrush = NULL;
	HBRUSH MDU::gdiDarkGrayBrush;
	HBRUSH MDU::gdiLightGrayBrush;
	HBRUSH MDU::gdiWhiteBrush;
	HBRUSH MDU::gdiRedBrush;
	HBRUSH MDU::gdiYellowBrush;
	HBRUSH MDU::gdiCyanBrush;
	HBRUSH MDU::gdiMagentaBrush;
	HBRUSH MDU::gdiLightGreenBrush;
	HBRUSH MDU::gdiBlueBrush;

	oapi::Brush* MDU::skpBlackBrush = NULL;
	oapi::Brush* MDU::skpDarkGrayBrush;
	oapi::Brush* MDU::skpLightGrayBrush;
	oapi::Brush* MDU::skpWhiteBrush;
	oapi::Brush* MDU::skpRedBrush;
	oapi::Brush* MDU::skpYellowBrush;
	oapi::Brush* MDU::skpCyanBrush;
	oapi::Brush* MDU::skpMagentaBrush;
	oapi::Brush* MDU::skpLightGreenBrush;
	oapi::Brush* MDU::skpBlueBrush;
	
	HPEN MDU::gdiBlackPen;
	HPEN MDU::gdiDarkGrayPen;
	HPEN MDU::gdiLightGrayPen;
	HPEN MDU::gdiLightGrayThickPen;
	HPEN MDU::gdiWhitePen;
	HPEN MDU::gdiRedPen;
	HPEN MDU::gdiYellowPen;
	HPEN MDU::gdiCyanPen;
	HPEN MDU::gdiMagentaPen;
	HPEN MDU::gdiLightGreenPen;
	HPEN MDU::gdiDarkGreenPen;
	HPEN MDU::gdiLightGreenThickPen;

	oapi::Pen* MDU::skpBlackPen;
	oapi::Pen* MDU::skpDarkGrayPen;
	oapi::Pen* MDU::skpLightGrayThickPen;
	oapi::Pen* MDU::skpLightGrayPen;
	oapi::Pen* MDU::skpWhitePen;
	oapi::Pen* MDU::skpRedPen;
	oapi::Pen* MDU::skpYellowPen;
	oapi::Pen* MDU::skpCyanPen;
	oapi::Pen* MDU::skpMagentaPen;
	oapi::Pen* MDU::skpLightGreenPen;
	oapi::Pen* MDU::skpDarkGreenPen;
	oapi::Pen* MDU::skpLightGreenThickPen;

	HPEN MDU::gdiOverbrightPen;
	HPEN MDU::gdiNormalPen;
	HPEN MDU::gdiDashedNormalPen;

	oapi::Pen* MDU::skpOverbrightPen;
	oapi::Pen* MDU::skpNormalPen;
	oapi::Pen* MDU::skpDashedNormalPen;

	HFONT MDU::gdiSSUAFont_h20w17;
	HFONT MDU::gdiSSUAFont_h10w10bold;
	HFONT MDU::gdiSSUAFont_h11w9;
	HFONT MDU::gdiSSUBFont_h18w9;
	HFONT MDU::gdiSSUBFont_h12w7;
	HFONT MDU::gdiSSUBFont_h16w9;

	oapi::Font* MDU::skpSSUAFont_h20;
	oapi::Font* MDU::skpSSUAFont_h10bold;
	oapi::Font* MDU::skpSSUAFont_h11;
	oapi::Font* MDU::skpSSUBFont_h18;
	oapi::Font* MDU::skpSSUBFont_h12;
	oapi::Font* MDU::skpSSUBFont_h16;

	HDC MDU::hDC_Tape_MACHV = NULL;
	HDC MDU::hDC_Tape_KEAS;
	HDC MDU::hDC_Tape_Alpha;
	HDC MDU::hDC_Tape_H;
	HDC MDU::hDC_Tape_Hdot;
	HDC MDU::hDC_ADIMASK = NULL;
	HDC MDU::hDC_ADIMASK_ORBIT;

	HBITMAP MDU::hBM_Tape_MACHV_tmp;
	HBITMAP MDU::hBM_Tape_KEAS_tmp;
	HBITMAP MDU::hBM_Tape_Alpha_tmp;
	HBITMAP MDU::hBM_Tape_H_tmp;
	HBITMAP MDU::hBM_Tape_Hdot_tmp;
	HBITMAP MDU::hBM_ADIMASK_tmp;
	HBITMAP MDU::hBM_ADIMASK_ORBIT_tmp;
	
	SURFHANDLE MDU::sfh_Tape_MACHV;
	SURFHANDLE MDU::sfh_Tape_KEAS;
	SURFHANDLE MDU::sfh_Tape_Alpha;
	SURFHANDLE MDU::sfh_Tape_H;
	SURFHANDLE MDU::sfh_Tape_Hdot;

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

		CreateTapes();
		CreateADI();

		display = 0;
		menu = 3;
	}

	MDU::~MDU()
	{
		DestroyADI();
		DestroyTapes();

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
				if (oapiGetMFDMode( usMDUID ) != 0) oapiOpenMFD( 1000, usMDUID );
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
				if (gcEnabled() && (gcSketchpadVersion( skp ) == 2) && hADIball)
				{
					DPS( (oapi::Sketchpad2*)skp );
					PaintEdgeMenu( skp );
				}
				else
				{
					HDC hDC = skp->GetDC();
					if (hDC)
					{
						int save = SaveDC( hDC );
						DPS( hDC );
						PaintEdgeMenu( hDC );
						RestoreDC( hDC, save );
					}
				}
				break;
			case 1:// A/E PFD
				if (gcEnabled() && (gcSketchpadVersion( skp ) == 2) && hADIball)
				{
					AEPFD( (oapi::Sketchpad2*)skp );
					PaintEdgeMenu( skp );
				}
				else
				{
					HDC hDC = skp->GetDC();
					if (hDC)
					{
						int save = SaveDC( hDC );
						AEPFD( hDC );
						PaintEdgeMenu( hDC );
						RestoreDC( hDC, save );
					}
				}
				break;
			case 2:// ORBIT PFD
				if (gcEnabled() && (gcSketchpadVersion( skp ) == 2) && hADIball)
				{
					ORBITPFD( (oapi::Sketchpad2*)skp );
					PaintEdgeMenu( skp );
				}
				else
				{
					HDC hDC = skp->GetDC();
					if (hDC)
					{
						int save = SaveDC( hDC );
						ORBITPFD( hDC );
						PaintEdgeMenu( hDC );
						RestoreDC( hDC, save );
					}
				}
				break;
			case 3:// OMS/MPS
				if (gcEnabled() && (gcSketchpadVersion( skp ) == 2))
				{
					OMSMPS( (oapi::Sketchpad2*)skp );
					PaintEdgeMenu( skp );
				}
				else
				{
					HDC hDC = skp->GetDC();
					if (hDC)
					{
						int save = SaveDC( hDC );
						OMSMPS( hDC );
						PaintEdgeMenu( hDC );
						RestoreDC( hDC, save );
					}
				}
				break;
			case 4:// HYD/APU
				if (gcEnabled() && (gcSketchpadVersion( skp ) == 2))
				{
					APUHYD( (oapi::Sketchpad2*)skp );
					PaintEdgeMenu( skp );
				}
				else
				{
					HDC hDC = skp->GetDC();
					if (hDC)
					{
						int save = SaveDC( hDC );
						APUHYD( hDC );
						PaintEdgeMenu( hDC );
						RestoreDC( hDC, save );
					}
				}
				break;
			case 5:// SPI
				if (gcEnabled() && (gcSketchpadVersion( skp ) == 2))
				{
					SPI( (oapi::Sketchpad2*)skp );
					PaintEdgeMenu( skp );
				}
				else
				{
					HDC hDC = skp->GetDC();
					if (hDC)
					{
						int save = SaveDC( hDC );
						SPI( hDC );
						PaintEdgeMenu( hDC );
						RestoreDC( hDC, save );
					}
				}
				break;
			case 6:// CST Menu
				if (gcEnabled() && (gcSketchpadVersion( skp ) == 2))
				{
					SystemStatusDisplay_CSTMenu( (oapi::Sketchpad2*)skp );
					PaintEdgeMenu( skp );
				}
				else
				{
					HDC hDC = skp->GetDC();
					if (hDC)
					{
						int save = SaveDC( hDC );
						SystemStatusDisplay_CSTMenu( hDC );
						PaintEdgeMenu( hDC );
						RestoreDC( hDC, save );
					}
				}
				break;
			case 7:// IDP Interactive CST
				if (gcEnabled() && (gcSketchpadVersion( skp ) == 2))
				{
					SystemStatusDisplay_IDPInteractiveCST( (oapi::Sketchpad2*)skp );
					PaintEdgeMenu( skp );
				}
				else
				{
					HDC hDC = skp->GetDC();
					if (hDC)
					{
						int save = SaveDC( hDC );
						SystemStatusDisplay_IDPInteractiveCST( hDC );
						PaintEdgeMenu( hDC );
						RestoreDC( hDC, save );
					}
				}
				break;
		}
		return;
	}

	bool MDU::NavigateMenu( DWORD key )
	{
		switch (menu)
		{
			case 0:// MAIN MENU
				switch (key)
				{
					case OAPI_KEY_1:
						menu = 1;
						return true;
					case OAPI_KEY_2:
						menu = 2;
						return true;
					case OAPI_KEY_3:
						menu = 3;
						display = 0;
						return true;
					case OAPI_KEY_4:
						menu = 4;
						display = 6;
						return true;
				}
				break;
			case 1:// FLT INST
				switch (key)
				{
					case OAPI_KEY_U:
						menu = 0;
						return true;
					case OAPI_KEY_1:
						display = 1;
						return true;
					case OAPI_KEY_2:
						display = 2;
						return true;
				}
				break;
			case 2:// SUBSYS STATUS
				switch (key)
				{
					case OAPI_KEY_U:
						menu = 0;
						return true;
					case OAPI_KEY_1:
						display = 3;
						return true;
					case OAPI_KEY_2:
						display = 4;
						return true;
					case OAPI_KEY_3:
						display = 5;
						return true;
				}
				break;
			case 3:// DPS MENU
				switch (key)
				{
					case OAPI_KEY_U:
						menu = 0;
						return true;
				}
				break;
			case 4:// MAINTENANCE MENU
				switch (key)
				{
					case OAPI_KEY_U:
						menu = 0;
						return true;
					case OAPI_KEY_3:
						menu = 5;
						return true;
				}
				break;
			case 5:// CST MENU SELECTION
				switch (key)
				{
					case OAPI_KEY_U:
						menu = 4;
						display = 6;
						return true;
					case OAPI_KEY_2:
						menu = 6;
						display = 7;
						return true;
				}
				break;
			case 6:// IDPx INTERACTIVE CST
				switch (key)
				{
					case OAPI_KEY_U:
						menu = 5;
						return true;
				}
				break;
		}
		return false;
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

	void MDU::PaintEdgeMenu( HDC hDC )
	{
		SelectObject( hDC, gdiCyanPen );
		MoveToEx( hDC, 0, 456, NULL );
		LineTo( hDC, 511, 456 );
		SelectObject( hDC, gdiSSUAFont_h11w9 );
		SetTextColor( hDC, CR_CYAN );
		SetTextAlign( hDC, TA_CENTER );
		
		// print buttons
		int x = 66;
		// button 1
		DrawMenuButton( hDC, x );
		if (menu != 0)
		{
			// draw up arrow
			MoveToEx( hDC, 54, 510, NULL );
			LineTo( hDC, 54, 500 );
			LineTo( hDC, 40, 500 );
			LineTo( hDC, 66, 486 );
			LineTo( hDC, 92, 500 );
			LineTo( hDC, 78, 500 );
			LineTo( hDC, 78, 510 );
			TextOut( hDC, x, 493, "UP", 2);
		}

		// button 2
		x += 76;
		if (menu == 0)
		{
			TextOut( hDC, x, 486, "FLT", 3 );
			TextOut( hDC, x, 499, " INST", 5 );
			DrawMenuButton( hDC, x );
		}
		else if (menu == 1)
		{
			if (display == 1)
			{
				SetTextColor( hDC, CR_WHITE );
				TextOut( hDC, x, 486, "A/E", 3 );
				TextOut( hDC, x, 499, "PFD", 3 );
				SetTextColor( hDC, CR_CYAN );
				SelectObject( hDC, gdiWhitePen );
				DrawMenuButton( hDC, x );
				SelectObject( hDC, gdiCyanPen );
			}
			else
			{
				TextOut( hDC, x, 486, "A/E", 3 );
				TextOut( hDC, x, 499, "PFD", 3 );
				DrawMenuButton( hDC, x );
			}
		}
		else if (menu == 2)
		{
			if (display == 3)
			{
				SetTextColor( hDC, CR_WHITE );
				TextOut( hDC, x, 486, "OMS/ ", 5 );
				TextOut( hDC, x, 499, "MPS", 3 );
				SetTextColor( hDC, CR_CYAN );
				SelectObject( hDC, gdiWhitePen );
				DrawMenuButton( hDC, x );
				SelectObject( hDC, gdiCyanPen );
			}
			else
			{
				TextOut( hDC, x, 486, "OMS/ ", 5 );
				TextOut( hDC, x, 499, "MPS", 3 );
				DrawMenuButton( hDC, x );
			}
		}
		else DrawMenuButton( hDC, x );

		// button 3
		x += 76;
		if (menu == 0)
		{
			TextOut( hDC, x, 486, "SUBSYS ", 7 );
			TextOut( hDC, x, 499, "STATUS ", 7 );
			DrawMenuButton( hDC, x );
		}
		else if (menu == 1)
		{
			if (display == 2)
			{
				SetTextColor( hDC, CR_WHITE );
				TextOut( hDC, x, 486, "ORBIT", 5 );
				TextOut( hDC, x, 499, "PFD", 3 );
				SetTextColor( hDC, CR_CYAN );
				SelectObject( hDC, gdiWhitePen );
				DrawMenuButton( hDC, x );
				SelectObject( hDC, gdiCyanPen );
			}
			else
			{
				TextOut( hDC, x, 486, "ORBIT", 5 );
				TextOut( hDC, x, 499, "PFD", 3 );
				DrawMenuButton( hDC, x );
			}
		}
		else if (menu == 2)
		{
			if (display == 4)
			{
				SetTextColor( hDC, CR_WHITE );
				TextOut( hDC, x, 486, "HYD/ ", 5 );
				TextOut( hDC, x, 499, "APU", 3 );
				SetTextColor( hDC, CR_CYAN );
				SelectObject( hDC, gdiWhitePen );
				DrawMenuButton( hDC, x );
				SelectObject( hDC, gdiCyanPen );
			}
			else
			{
				TextOut( hDC, x, 486, "HYD/ ", 5 );
				TextOut( hDC, x, 499, "APU", 3 );
				DrawMenuButton( hDC, x );
			}
		}
		else if (menu == 5)
		{
			TextOut( hDC, x, 486, "START", 5 );
			TextOut( hDC, x, 499, "IDP", 3 );
			DrawMenuButton( hDC, x );
		}
		else DrawMenuButton( hDC, x );

		// button 4
		x += 76;
		if (menu == 0)
		{
			TextOut( hDC, x, 486, "DPS", 3 );
			DrawMenuButton( hDC, x );
		}
		else if (menu == 2)
		{
			if (display == 5)
			{
				SetTextColor( hDC, CR_WHITE );
				TextOut( hDC, x, 486, "SPI", 3 );
				SetTextColor( hDC, CR_CYAN );
				SelectObject( hDC, gdiWhitePen );
				DrawMenuButton( hDC, x );
				SelectObject( hDC, gdiCyanPen );
			}
			else
			{
				TextOut( hDC, x, 486, "SPI", 3 );
				DrawMenuButton( hDC, x );
			}
		}
		else if (menu == 4)
		{
			TextOut( hDC, x, 486, "CST", 3 );
			DrawMenuButton( hDC, x );
		}
		else DrawMenuButton( hDC, x );

		// button 5
		x += 76;
		DrawMenuButton( hDC, x );
		if (menu == 0)
		{
			TextOut( hDC, x, 486, "MEDS ", 5 );
			TextOut( hDC, x, 499, "MAINT", 5 );
		}

		// button 6
		x += 76;
		//TextOut( hDC, x, 50, "PG", 2 );
		DrawMenuButton( hDC, x );

		// print title
		switch (menu)
		{
			case 0:
				TextOut( hDC, 226, 471, "MAIN MENU", 9 );
				break;
			case 1:
				TextOut( hDC, 226, 471, " FLIGHT INSTRUMENT MENU", 23 );
				break;
			case 2:
				TextOut( hDC, 226, 471, "SUBSYSTEM MENU ", 15 );
				break;
			case 3:
				TextOut( hDC, 226, 471, "DPS MENU ", 9 );
				break;
			case 4:
				TextOut( hDC, 226, 471, " MAINTENANCE MENU", 17 );
				break;
			case 5:
				TextOut( hDC, 226, 471, " CST MENU SELECTION", 19 );
				break;
			case 6:
				{
					char buf[32];
					sprintf_s( buf, 32, " IDP%d INTERACTIVE CST", GetIDP()->GetIDPID() );
					TextOut( hDC, 226, 471, buf, strlen( buf ) );
				}
				break;
			default:
				// print nothing
				break;
		}

		// printf configuration info
		/*TextOut( hDC, 15, 486, "P1*", 3 );
		TextOut( hDC, 15, 499, "S3 ", 3 );
		TextOut( hDC, 497, 486, "FC2", 3 );
		TextOut( hDC, 497, 499, "AUT", 3 );

		// printf MEDS fault line
		if (!GetFlash())
		{
			SetTextColor( hDC, CR_WHITE );
			TextOut( hDC, 226, 458, "IDP 1 2 3 4 STILL WIP", 21 );
		}*/
		return;
	}

	void MDU::PaintEdgeMenu( oapi::Sketchpad* skp )
	{
		skp->SetPen( skpCyanPen );
		skp->Line( 0, 456, 511, 456 );
		skp->SetFont( skpSSUAFont_h11 );
		skp->SetTextColor( CR_CYAN );
		skp->SetTextAlign( oapi::Sketchpad::CENTER );

		// print buttons
		int x = 66;
		// button 1
		DrawMenuButton( skp, x );
		if (menu != 0)
		{
			// draw up arrow
			skp->MoveTo( 54, 510 );
			skp->LineTo( 54, 500 );
			skp->LineTo( 40, 500 );
			skp->LineTo( 66, 486 );
			skp->LineTo( 92, 500 );
			skp->LineTo( 78, 500 );
			skp->LineTo( 78, 510 );
			skp->Text( x, 493, "UP", 2);
		}

		// button 2
		x += 76;
		if (menu == 0)
		{
			skp->Text( x, 486, "FLT", 3 );
			skp->Text( x, 499, " INST", 5 );
			DrawMenuButton( skp, x );
		}
		else if (menu == 1)
		{
			if (display == 1)
			{
				skp->SetTextColor( CR_WHITE );
				skp->Text( x, 486, "A/E", 3 );
				skp->Text( x, 499, "PFD", 3 );
				skp->SetTextColor( CR_CYAN );
				skp->SetPen( skpWhitePen );
				DrawMenuButton( skp, x );
				skp->SetPen( skpCyanPen );
			}
			else
			{
				skp->Text( x, 486, "A/E", 3 );
				skp->Text( x, 499, "PFD", 3 );
				DrawMenuButton( skp, x );
			}
		}
		else if (menu == 2)
		{
			if (display == 3)
			{
				skp->SetTextColor( CR_WHITE );
				skp->Text( x, 486, "OMS/ ", 5 );
				skp->Text( x, 499, "MPS", 3 );
				skp->SetTextColor( CR_CYAN );
				skp->SetPen( skpWhitePen );
				DrawMenuButton( skp, x );
				skp->SetPen( skpCyanPen );
			}
			else
			{
				skp->Text( x, 486, "OMS/ ", 5 );
				skp->Text( x, 499, "MPS", 3 );
				DrawMenuButton( skp, x );
			}
		}
		else DrawMenuButton( skp, x );

		// button 3
		x += 76;
		if (menu == 0)
		{
			skp->Text( x, 486, "SUBSYS ", 7 );
			skp->Text( x, 499, "STATUS ", 7 );
			DrawMenuButton( skp, x );
		}
		else if (menu == 1)
		{
			if (display == 2)
			{
				skp->SetTextColor( CR_WHITE );
				skp->Text( x, 486, "ORBIT", 5 );
				skp->Text( x, 499, "PFD", 3 );
				skp->SetTextColor( CR_CYAN );
				skp->SetPen( skpWhitePen );
				DrawMenuButton( skp, x );
				skp->SetPen( skpCyanPen );
			}
			else
			{
				skp->Text( x, 486, "ORBIT", 5 );
				skp->Text( x, 499, "PFD", 3 );
				DrawMenuButton( skp, x );
			}
		}
		else if (menu == 2)
		{
			if (display == 4)
			{
				skp->SetTextColor( CR_WHITE );
				skp->Text( x, 486, "HYD/ ", 5 );
				skp->Text( x, 499, "APU", 3 );
				skp->SetTextColor( CR_CYAN );
				skp->SetPen( skpWhitePen );
				DrawMenuButton( skp, x );
				skp->SetPen( skpCyanPen );
			}
			else
			{
				skp->Text( x, 486, "HYD/ ", 5 );
				skp->Text( x, 499, "APU", 3 );
				DrawMenuButton( skp, x );
			}
		}
		else if (menu == 5)
		{
			skp->Text( x, 486, "START", 5 );
			skp->Text( x, 499, "IDP", 3 );
			DrawMenuButton( skp, x );
		}
		else DrawMenuButton( skp, x );

		// button 4
		x += 76;
		if (menu == 0)
		{
			skp->Text( x, 486, "DPS", 3 );
			DrawMenuButton( skp, x );
		}
		else if (menu == 2)
		{
			if (display == 5)
			{
				skp->SetTextColor( CR_WHITE );
				skp->Text( x, 486, "SPI", 3 );
				skp->SetTextColor( CR_CYAN );
				skp->SetPen( skpWhitePen );
				DrawMenuButton( skp, x );
				skp->SetPen( skpCyanPen );
			}
			else
			{
				skp->Text( x, 486, "SPI", 3 );
				DrawMenuButton( skp, x );
			}
		}
		else if (menu == 4)
		{
			skp->Text( x, 486, "CST", 3 );
			DrawMenuButton( skp, x );
		}
		else DrawMenuButton( skp, x );

		// button 5
		x += 76;
		DrawMenuButton( skp, x );
		if (menu == 0)
		{
			skp->Text( x, 486, "MEDS ", 5 );
			skp->Text( x, 499, "MAINT", 5 );
		}

		// button 6
		x += 76;
		//skp->Text( x, 50, "PG", 2 );
		DrawMenuButton( skp, x );

		// print title
		switch (menu)
		{
			case 0:
				skp->Text( 226, 471, "MAIN MENU", 9 );
				break;
			case 1:
				skp->Text( 226, 471, " FLIGHT INSTRUMENT MENU", 23 );
				break;
			case 2:
				skp->Text( 226, 471, "SUBSYSTEM MENU ", 15 );
				break;
			case 3:
				skp->Text( 226, 471, "DPS MENU ", 9 );
				break;
			case 4:
				skp->Text( 226, 471, " MAINTENANCE MENU", 17 );
				break;
			case 5:
				skp->Text( 226, 471, " CST MENU SELECTION", 19 );
				break;
			case 6:
				{
					char buf[32];
					sprintf_s( buf, 32, " IDP%d INTERACTIVE CST", GetIDP()->GetIDPID() );
					skp->Text( 226, 471, buf, strlen( buf ) );
				}
				break;
			default:
				// print nothing
				break;
		}

		// printf configuration info
		/*skp->Text( 15, 486, "P1*", 3 );
		skp->Text( 15, 499, "S3 ", 3 );
		skp->Text( 497, 486, "FC2", 3 );
		skp->Text( 497, 499, "AUT", 3 );

		// printf MEDS fault line
		if (!GetFlash())
		{
			skp->SetTextColor( CR_WHITE );
			skp->Text( 226, 458, "IDP 1 2 3 4 STILL WIP", 21 );
		}*/
		return;
	}

	void MDU::DrawMenuButton( HDC hDC, int x )
	{
		MoveToEx( hDC, x - 36, 510, NULL );
		LineTo( hDC, x - 36, 483 );
		MoveToEx( hDC, x - 36, 484, NULL );
		LineTo( hDC, x + 36, 484 );
		MoveToEx( hDC, x + 36, 483, NULL );
		LineTo( hDC, x + 36, 510 );
		return;
	}

	void MDU::DrawMenuButton( oapi::Sketchpad* skp, int x )
	{
		skp->Line( x - 36, 510, x - 36, 483 );
		skp->Line( x - 36, 484, x + 36, 484 );
		skp->Line( x + 36, 483, x + 36, 510 );
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
		mfdspec.bt_yofs  = 512/6;
		mfdspec.bt_ydist = 512/7;
		oapiRegisterMFD (id, &mfdspec);
		//sprintf_s(pszBuffer, 256, "MFD %s (%d) registered", GetQualifiedIdentifier().c_str(), usMDUID);
		//oapiWriteLog(pszBuffer);
	}

	bool MDU::DefineVCGroup(UINT mgrp)
	{
		mfdspec.ngroup = mgrp;
		return true;
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
		pixels.clear();

		if(prim_idp) {
			prim_idp->OnPaint(this);
		}
		else {
			PrintToBuffer("ERROR: IDP NOT CONNECTED", 24, 0, 0, 0);
		}
		
		//print Scratch Pad line
		const char* scratch_pad = prim_idp->GetScratchPadLineString_B();
		int flashingentry = prim_idp->GetFlashingEntry();
		if (flashingentry != 0)
		{
			PrintToBuffer( scratch_pad, flashingentry, 1, 25, dps::DEUATT_FLASHING );
		}
		PrintToBuffer( scratch_pad + flashingentry, strlen( scratch_pad + flashingentry ), 1 + flashingentry, 25, 0 );
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
		if (gdiBlackBrush) return;// already created

		gdiBlackBrush = CreateSolidBrush( CR_BLACK );
		gdiDarkGrayBrush = CreateSolidBrush( CR_DARK_GRAY );
		gdiLightGrayBrush = CreateSolidBrush( CR_LIGHT_GRAY );
		gdiWhiteBrush = CreateSolidBrush( CR_WHITE );
		gdiRedBrush = CreateSolidBrush( CR_RED );
		gdiYellowBrush = CreateSolidBrush( CR_YELLOW );
		gdiCyanBrush = CreateSolidBrush( CR_CYAN );
		gdiMagentaBrush = CreateSolidBrush( CR_MAGENTA );
		gdiLightGreenBrush = CreateSolidBrush( CR_LIGHT_GREEN );
		gdiBlueBrush = CreateSolidBrush( CR_BLUE );

		gdiBlackPen = CreatePen( PS_SOLID, 2, CR_BLACK );
		gdiDarkGrayPen = CreatePen( PS_SOLID, 2, CR_DARK_GRAY );
		gdiLightGrayPen = CreatePen( PS_SOLID, 2, CR_LIGHT_GRAY );
		gdiLightGrayThickPen = CreatePen( PS_SOLID, 3, CR_LIGHT_GRAY );
		gdiWhitePen = CreatePen( PS_SOLID, 2, CR_WHITE );
		gdiRedPen = CreatePen( PS_SOLID, 2, CR_RED );
		gdiYellowPen = CreatePen( PS_SOLID, 2, CR_YELLOW );
		gdiCyanPen = CreatePen( PS_SOLID, 2, CR_CYAN );
		gdiMagentaPen = CreatePen( PS_SOLID, 2, CR_MAGENTA );
		gdiLightGreenPen = CreatePen( PS_SOLID, 2, CR_LIGHT_GREEN );
		gdiDarkGreenPen = CreatePen( PS_SOLID, 2, CR_DARK_GREEN );
		gdiLightGreenThickPen = CreatePen( PS_SOLID, 4, CR_LIGHT_GREEN );

		gdiOverbrightPen = CreatePen( PS_SOLID, 2, CR_DPS_OVERBRIGHT );
		gdiNormalPen = CreatePen( PS_SOLID, 2, CR_DPS_NORMAL );
		LOGBRUSH lb = {BS_SOLID, CR_DPS_NORMAL, 0};
		DWORD pstyle[2] = {16, 8};
		gdiDashedNormalPen = ExtCreatePen( PS_GEOMETRIC | PS_USERSTYLE, 2, &lb, 2, pstyle );

		gdiSSUAFont_h20w17 = CreateFont( 20, 17, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FIXED_PITCH, "SSU_Font_A" );
		gdiSSUAFont_h10w10bold = CreateFont( 10, 10, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FIXED_PITCH, "SSU_Font_A" );
		gdiSSUAFont_h11w9 = CreateFont( 11, 9, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FIXED_PITCH, "SSU_Font_A" );
		gdiSSUBFont_h18w9 = CreateFont( 18, 9, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FIXED_PITCH, "SSU_Font_B" );
		gdiSSUBFont_h12w7 = CreateFont( 12, 7, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FIXED_PITCH, "SSU_Font_B" );
		gdiSSUBFont_h16w9 = CreateFont( 16, 9, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FIXED_PITCH, "SSU_Font_B" );
		return;
	}

	void MDU::DestroyGDIObjects()
	{
		if (!gdiBlackBrush) return;// already deleted

		DeleteObject( gdiBlackBrush );
		DeleteObject( gdiDarkGrayBrush );
		DeleteObject( gdiLightGrayBrush );
		DeleteObject( gdiWhiteBrush );
		DeleteObject( gdiRedBrush );
		DeleteObject( gdiYellowBrush );
		DeleteObject( gdiCyanBrush );
		DeleteObject( gdiMagentaBrush );
		DeleteObject( gdiLightGreenBrush );
		DeleteObject( gdiBlueBrush );

		DeleteObject( gdiBlackPen );
		DeleteObject( gdiDarkGrayPen );
		DeleteObject( gdiLightGrayPen );
		DeleteObject( gdiLightGrayThickPen );
		DeleteObject( gdiWhitePen );
		DeleteObject( gdiRedPen );
		DeleteObject( gdiYellowPen );
		DeleteObject( gdiCyanPen );
		DeleteObject( gdiMagentaPen );
		DeleteObject( gdiLightGreenPen );
		DeleteObject( gdiDarkGreenPen );
		DeleteObject( gdiLightGreenThickPen );

		DeleteObject( gdiOverbrightPen );
		DeleteObject( gdiNormalPen );
		DeleteObject( gdiDashedNormalPen );

		DeleteObject( gdiSSUAFont_h20w17 );
		DeleteObject( gdiSSUAFont_h10w10bold );
		DeleteObject( gdiSSUAFont_h11w9 );
		DeleteObject( gdiSSUBFont_h18w9 );
		DeleteObject( gdiSSUBFont_h12w7 );
		DeleteObject( gdiSSUBFont_h16w9 );

		gdiBlackBrush = NULL;
		return;
	}

	void MDU::CreateSketchpadObjects()
	{
		if (skpBlackBrush) return;// already created

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

		skpBlackPen = oapiCreatePen( 1, 2, CR_BLACK );
		skpDarkGrayPen = oapiCreatePen( 1, 2, CR_DARK_GRAY );
		skpLightGrayPen = oapiCreatePen( 1, 2, CR_LIGHT_GRAY );
		skpLightGrayThickPen = oapiCreatePen( 1, 3, CR_LIGHT_GRAY );
		skpWhitePen = oapiCreatePen( 1, 2, CR_WHITE );
		skpRedPen = oapiCreatePen( 1, 2, CR_RED );
		skpYellowPen = oapiCreatePen( 1, 2, CR_YELLOW );
		skpCyanPen = oapiCreatePen( 1, 2, CR_CYAN );
		skpMagentaPen = oapiCreatePen( 1, 2, CR_MAGENTA );
		skpLightGreenPen = oapiCreatePen( 1, 2, CR_LIGHT_GREEN );
		skpDarkGreenPen = oapiCreatePen( 1, 2, CR_DARK_GREEN );
		skpLightGreenThickPen = oapiCreatePen( 1, 4, CR_LIGHT_GREEN );
		
		skpOverbrightPen = oapiCreatePen( 1, 2, CR_DPS_OVERBRIGHT );
		skpNormalPen = oapiCreatePen( 1, 2, CR_DPS_NORMAL );
		skpDashedNormalPen = oapiCreatePen( 2, 2, CR_DPS_NORMAL );

		skpSSUAFont_h20 = oapiCreateFont( 20, true, "*SSU_Font_A" );
		skpSSUAFont_h10bold = oapiCreateFont( 10, false, "*SSU_Font_A", FONT_BOLD );
		skpSSUAFont_h11 = oapiCreateFont( 11, true, "*SSU_Font_A" );
		skpSSUBFont_h18 = oapiCreateFont( 18, true, "*SSU_Font_B" );
		skpSSUBFont_h12 = oapiCreateFont( 12, true, "*SSU_Font_B" );
		skpSSUBFont_h16 = oapiCreateFont( 16, true, "*SSU_Font_B" );
		return;
	}

	void MDU::DestroySketchpadObjects()
	{
		if (!skpBlackBrush) return;// already deleted

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
		oapiReleasePen( skpLightGrayPen );
		oapiReleasePen( skpLightGrayThickPen );
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

		oapiReleaseFont( skpSSUAFont_h20 );
		oapiReleaseFont( skpSSUAFont_h10bold );
		oapiReleaseFont( skpSSUAFont_h11 );
		oapiReleaseFont( skpSSUBFont_h18 );
		oapiReleaseFont( skpSSUBFont_h12 );
		oapiReleaseFont( skpSSUBFont_h16 );

		skpBlackBrush = NULL;
		return;
	}
};
