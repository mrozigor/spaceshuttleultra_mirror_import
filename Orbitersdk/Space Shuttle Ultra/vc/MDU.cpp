#include "../Atlantis.h"
#include "MDU.h"
#include "../dps/IDP.h"
#include "../meshres_vc_additions.h"


extern GDIParams g_Param;

namespace vc {

	//find location on bitmap of letter
	void BitmapLocation(char ch, int &x, int &y)
	{
		int row=ch/16;
		int col=ch-(16*row);
		x=1+18*col;
		y=1+33*row;
	}

	MDU::MDU(Atlantis* _sts, const string& _ident, unsigned short _usMDUID, bool _bUseCRTMFD)
		: AtlantisVCComponent(_sts, _ident), usMDUID(_usMDUID), MFDID(-1),
		bUseCRTMFD(_bUseCRTMFD),
		prim_idp(NULL), sec_idp(NULL), bUseSecondaryPort(false),
		bInverseX(false), counting(false)
	{
		_sts->RegisterMDU(_usMDUID, this);
		//Clear text buffer
		//Create display buffer
		//Clear display buffer
		shLabelTex = NULL;
		bIsConnectedToCRTMFD = false;
		
		// set default button positions
		btnPwrXmin = 0.038f; btnPwrXmax = 0.099f;
		btnPwrYmin = 0.8350f; btnPwrYmax = 0.9144f;
		btnBrtXmin = 0.9257f; btnBrtXmax = 0.9929f;
		btnBrtYmin = 0.8350f; btnBrtYmax = 0.9144f;
		edgekeyXmin = 0.2237f; edgekeyXmax = 0.7939f;
		edgekeyYmin = 0.9185f; edgekeyYmax = 0.9601f;

		CreateGDIObjects();

		Tape_Create();
		ADI_Create();
		//HSI_Create();
	}

	MDU::~MDU()
	{
		DestroyGDIObjects();
	}

	void MDU::ConnectToCRTMFD()
	{
		bIsConnectedToCRTMFD = true;
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

	/*const string& MDU::GetEdgekeyMenu() const
	{
		static string r = "MAIN MENU";
		return r;
	}*/

	/*char* MDU::GetEdgeKeyMenuLabel(int iButton)
	{
		return NULL;
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

		if(MFDID!=-1) {
			if(y >= btnPwrYmin && y<= btnPwrYmax && x >= btnPwrXmin && x <= btnPwrXmax)
			{
				if(_event & PANEL_MOUSE_LBDOWN)
				{
					//sprintf_s(oapiDebugString(), 80, "MDU %s POWER ON/OFF", GetQualifiedIdentifier().c_str());
					bIsConnectedToCRTMFD = false;
					//oapiSendMFDKey(usMDUID, OAPI_KEY_ESCAPE);
					oapiSendMFDKey(MFDID, OAPI_KEY_ESCAPE);
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
						oapiProcessMFDButton (MFDID, 0, _event);
					}
				}
				else if(edgekeyClickPos >= 0.18 && edgekeyClickPos <= 0.28)
				{
					if(_event & PANEL_MOUSE_LBDOWN)
					{
						//sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 2", GetQualifiedIdentifier().c_str());
						oapiProcessMFDButton (MFDID, 1, _event);
					}
				}
				else if(edgekeyClickPos >= 0.36 && edgekeyClickPos <= 0.46)
				{
					if(_event & PANEL_MOUSE_LBDOWN)
					{
						//sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 3", GetQualifiedIdentifier().c_str());
						oapiProcessMFDButton (MFDID, 2, _event);
					}
				} 
				else if(edgekeyClickPos >= 0.54 && edgekeyClickPos <= 0.64)
				{
					if(_event & PANEL_MOUSE_LBDOWN)
					{
						//sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 4", GetQualifiedIdentifier().c_str());
						oapiProcessMFDButton (MFDID, 3, _event);
					}
				}
				else if(edgekeyClickPos >= 0.72 && edgekeyClickPos <= 0.82)
				{
					if(_event & PANEL_MOUSE_LBDOWN)
					{
						//sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 5", GetQualifiedIdentifier().c_str());
						oapiProcessMFDButton (MFDID, 4, _event);
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
						oapiSendMFDKey (MFDID, OAPI_KEY_F2);
						counting = false;
					} else if ((_event & PANEL_MOUSE_LBPRESSED) && counting && (oapiGetSysTime()-t0 >= 1.0)) {
						//sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 6: SWITCH MODE", GetQualifiedIdentifier().c_str());
						bIsConnectedToCRTMFD = false;
						oapiSendMFDKey (MFDID, OAPI_KEY_F1);
						counting = false;		
					}
				}
				//else sprintf_s(oapiDebugString(), 80, "MDU %s EDGEKEYS: %f", GetQualifiedIdentifier().c_str(), edgekeyClickPos);
			}
		}
		return true;
	}

	
	bool MDU::Paint(HDC hDC)
	{
		//oapiWriteLog("Paint called 1");
		UpdateTextBuffer();
		
		int Save=SaveDC(hDC);
		HDC CompatibleDC=CreateCompatibleDC(hDC);
		//HDC BitmapDC=CreateCompatibleDC(hDC);
		//SelectObject(BitmapDC, g_Param.deu_characters);
		HBITMAP BMP=CreateCompatibleBitmap(hDC, 816, 806);
		SelectObject(CompatibleDC, BMP);

		bool flash=GetFlash();

		//draw stuff
		for(int i=0;i<51;i++) {
			for(int j=0;j<26;j++) {
				char cbuf[2];
				if(textBuffer[i][j].cSymbol>='!') {
					if (((textBuffer[i][j].cAttr & dps::DEUATT_FLASHING) == 0) || (flash == true))
					{
						int x, y;
						sprintf_s(cbuf, 2, "%c", textBuffer[i][j].cSymbol);
						vc::BitmapLocation(textBuffer[i][j].cSymbol, x, y);

						if ((textBuffer[i][j].cAttr & dps::DEUATT_OVERBRIGHT) != 0)
						{
							// overbright intensity
							BitBlt(CompatibleDC, i*5, j*9, 5, 9, g_Param.DeuCharOvrBrgtBitmapDC, (int)(x*0.278), (int)(y*0.272), SRCCOPY);
						}
						else
						{
							// default
							// normal intensity
							BitBlt(CompatibleDC, i*5, j*9, 5, 9, g_Param.DeuCharBitmapDC, (int)(x*0.278), (int)(y*0.272), SRCCOPY);
						}
					}
				}
			}
		}
		BitBlt(hDC, 0, 0, 256, 256, CompatibleDC, 0, 0, SRCCOPY);

		for(unsigned int i=0;i<lines.size();i++) {
			if (((lines[i].cAttr & dps::DEUATT_FLASHING) == 0) || flash) {
				if ((lines[i].cAttr & dps::DEUATT_OVERBRIGHT) != 0) SelectObject(hDC, hOverbrightPen);
				else if ((lines[i].cAttr & dps::DEUATT_DASHED) != 0) SelectObject( hDC, hDashedNormalPen );
				else SelectObject(hDC, hNormalPen);
				MoveToEx(hDC, lines[i].x0, lines[i].y0, NULL);
				LineTo(hDC, lines[i].x1, lines[i].y1);
			}
		}
		for(unsigned int i=0;i<ellipses.size();i++) {
			if(ellipses[i].cAttr != dps::DEUATT_FLASHING || flash) {
				if(ellipses[i].cAttr == dps::DEUATT_OVERBRIGHT) SelectObject(hDC, hOverbrightPen);
				else SelectObject(hDC, hNormalPen);
				int startX = (ellipses[i].xLeft+ellipses[i].xRight)/2;
				int startY = ellipses[i].yTop;
				Arc(hDC, ellipses[i].xLeft, ellipses[i].yTop, ellipses[i].xRight, ellipses[i].yBottom, startX, startY, startX, startY);
			}
		}

		// driving IDP and active keyboards display
		SelectObject( hDC, hNormalPen );
		Rectangle( hDC, 118, 235, 138, 255 );
		SelectObject( hDC, TahomaFont_h17w6 );
		SetTextColor( hDC, CR_DPS_NORMAL );
		char cbuf[2];
		sprintf_s( cbuf, 2, "%d", GetDrivingIDP() );
		TextOut( hDC, 124, 236, cbuf, 1 );
		int kb = GetIDP()->GetActiveKeyboard();
		if ((kb & 1) == 1)// CDR
		{
			SelectObject( hDC, RedPen );
			Rectangle( hDC, 118, 249, 54, 251 );
		}
		if ((kb & 2) == 2)// PLT
		{
			SelectObject( hDC, YellowPen );
			Rectangle( hDC, 138, 249, 202, 251 );
		}

		RestoreDC(hDC, Save);
		DeleteDC(CompatibleDC);
		//DeleteDC(BitmapDC);
		DeleteObject(BMP);
		return false;
	}

	bool MDU::PaintEdgeMenu(HDC hDC)
	{
		
		//HDC hDC = oapiGetDC (surf);

		// D. Beachy: BUGFIX: if MFD powered off, cover separator lines and do not paint buttons
		if (MFDID==-1 || oapiGetMFDMode(MFDID) == MFD_NONE) {
			RECT r = {0, 0, 256, 41};
			FillRect(hDC, &r, (HBRUSH)GetStockObject(BLACK_BRUSH));
		} else {   // MFD powered on
			HFONT pFont = (HFONT)SelectObject (hDC, g_Param.font[0]);
			SetTextColor (hDC, CR_MENU_COLOR );
			SetTextAlign (hDC, TA_CENTER);
			SetBkMode (hDC, TRANSPARENT);
			const char *label;
			int x = 28;
			int menu = -1;
			
			for (int bt = 0; bt < 5; bt++) {
				if (label = oapiMFDButtonLabel (MFDID, bt))
				{
					if (strcmp( label, "UP" ) == 0)
					{
						// draw up arrow 
						HBRUSH tmpbrush = (HBRUSH)SelectObject( hDC, GetStockObject( NULL_BRUSH ) );
						HPEN tmppen = (HPEN)SelectObject( hDC, MenuPen );
						POINT arrow[7] = {{28, 22}, {43, 31}, {35, 31}, {35, 40}, {21, 40}, {21, 31}, {13, 31}};// CW from top
						Polygon( hDC, arrow, 7 );
						TextOut( hDC, x, 25, "UP", 2);
						menu = 3;// "inside" our menu, default to dps menu
						SelectObject( hDC, tmpbrush );
						SelectObject( hDC, tmppen );
					}
					else if (strcmp( label, "FLT" ) == 0)
					{
						TextOut( hDC, x, 21, "FLT", 3 );
						TextOut( hDC, x, 30, "INST", 4 );
						menu = 0;
					}
					else if (strcmp( label, "SUB" ) == 0)
					{
						TextOut( hDC, x, 21, "SUBSYS", 6 );
						TextOut( hDC, x, 30, "STATUS", 6 );
					}
					else if (strcmp( label, "A/E" ) == 0)
					{
						if (display == 1)
						{
							SetTextColor (hDC, CR_WHITE );
							TextOut( hDC, x, 21, "A/E", 3 );
							TextOut( hDC, x, 30, "PFD", 3 );
							SetTextColor (hDC, CR_MENU_COLOR );
						}
						else
						{
							TextOut( hDC, x, 21, "A/E", 3 );
							TextOut( hDC, x, 30, "PFD", 3 );
						}
						menu = 1;
					}
					else if (strcmp( label, "ORBIT" ) == 0)
					{
						if (display == 2)
						{
							SetTextColor (hDC, CR_WHITE );
							TextOut( hDC, x, 21, "ORBIT", 5 );
							TextOut( hDC, x, 30, "PFD", 3 );
							SetTextColor (hDC, CR_MENU_COLOR );
						}
						else
						{
							TextOut( hDC, x, 21, "ORBIT", 5 );
							TextOut( hDC, x, 30, "PFD", 3 );
						}
					}
					else if (strcmp( label, "OMS" ) == 0)
					{
						if (display == 3)
						{
							SetTextColor (hDC, CR_WHITE );
							TextOut( hDC, x, 21, "OMS/", 4 );
							TextOut( hDC, x, 30, "MPS", 3 );
							SetTextColor (hDC, CR_MENU_COLOR );
						}
						else
						{
							TextOut( hDC, x, 21, "OMS/", 4 );
							TextOut( hDC, x, 30, "MPS", 3 );
						}
						menu = 2;
					}
					else if (strcmp( label, "HYD" ) == 0)
					{
						if (display == 4)
						{
							SetTextColor (hDC, CR_WHITE );
							TextOut( hDC, x, 21, "HYD/", 4 );
							TextOut( hDC, x, 30, "APU", 3 );
							SetTextColor (hDC, CR_MENU_COLOR );
						}
						else
						{
							TextOut( hDC, x, 21, "HYD/", 4 );
							TextOut( hDC, x, 30, "APU", 3 );
						}
					}
					else if (strcmp( label, "SPI" ) == 0)
					{
						if (display == 5)
						{
							SetTextColor (hDC, CR_WHITE );
							TextOut( hDC, x, 25, "SPI", 3 );
							SetTextColor (hDC, CR_MENU_COLOR );
						}
						else TextOut( hDC, x, 25, "SPI", 3 );
					}
					else
					{
						TextOut (hDC, x, 25, label, strlen(label));
					}
					x += 39;
				} else break;
			}

			// print title
			switch (menu)
			{
				case 0:
					TextOut( hDC, 128, 8, "MAIN MENU", 9 );
					break;
				case 1:
					TextOut( hDC, 128, 8, "FLIGHT INSTRUMENTATION MENU", 27 );
					break;
				case 2:
					TextOut( hDC, 128, 8, "SUBSYSTEM MENU", 14 );
					break;
				case 3:
					TextOut( hDC, 128, 8, "DPS MENU", 8 );
					break;
				default:
					// print nothing
					break;
			}

			TextOut (hDC, 222, 25, "PG", 2);
			SelectObject (hDC, pFont);
		}

		//oapiReleaseDC (surf, hDC);

		return false;
	}

	bool MDU::RealizeMFD(int id)
	{
		MFDID=id;
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
		bIsConnectedToCRTMFD = false;
		//oapiRegisterMFD (usMDUID, &mfdspec);
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
			HDC hDC=oapiGetDC(surf);
			PaintEdgeMenu(hDC);
			oapiReleaseDC(surf, hDC);
			return true;
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

	void MDU::AEPFD( HDC hDC )
	{
		int MM = STS()->pSimpleGPC->GetMajorMode();
		double MachNumber = STS()->GetMachNumber();
		double Altitude_ft = STS()->GetAltitude() * MPS2FPS;
		int adiatt = 1;//GetIDP()->GetADIAttitude();
		int adierr = GetIDP()->GetADIError();
		int adirate = GetIDP()->GetADIRate();
		VECTOR3 vel;
		STS()->GetHorizonAirspeedVector( vel );
		vel *= MPS2FPS;
		VECTOR3 av;// x = pitch, y = yaw, z = roll
		STS()->GetAngularVel( av );
		av *= DEG;
		av.z = -av.z;
		av.x = -av.x;
		VECTOR3 atterr;
		double attRoll = -STS()->GetBank() * DEG;
		if (attRoll <= 0) attRoll += 360;
		double attPitch = STS()->GetPitch() * DEG;
		if (attPitch <= 0) attPitch += 360;
		double attYaw = 0;//STS()->GetSlipAngle() * DEG;
		//if (attYaw <= 0) attYaw += 360;

		int save = SaveDC( hDC );

		SelectObject( hDC, TahomaFont_h10w4 );

		switch (MM)
		{
			case 101:
				AEPFD_Header_AscentDAP( hDC, 101, adiatt );
				Tapes_Invalid( hDC );
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				ADI_RATE_A( hDC, av.x, av.z, av.y, adirate );
				ADI_ERROR_A( hDC, 0, 0, 0, adierr );
				HSI_E( hDC, STS()->GetYaw() );
				break;
			case 102:
				AEPFD_Header_AscentDAP( hDC, 102, adiatt );
				Tape_MVR_KEAS( hDC, MachNumber );
				Tape_Alpha( hDC, 0 );
				Tape_H_Hdot( hDC, Altitude_ft, vel.y );
				if ((Altitude_ft <= 200000) && (STS()->GetMET() <= 150)) AEPFD_BETA( hDC );
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				ADI_RATE_A( hDC, av.x, av.z, av.y, adirate );
				atterr = GetIDP()->GetAttitudeErrors_AscentGuidance();
				ADI_ERROR_A( hDC, atterr.x, atterr.z, atterr.y, adierr );
				AEPFD_GMETER_STATIC( hDC );
				AEPFD_GMETER_ACCEL( hDC );
				HSI_A( hDC, STS()->GetYaw(), STS()->GetBank() );// TODO set 0 as tgt plane
				AEPFD_XTRK( hDC );// TODO only NOM, TAL and ATO
				AEPFD_dINC( hDC );
				break;
			case 103:
				AEPFD_Header_AscentDAP( hDC, 103, adiatt );
				if (GetIDP()->GetMECOConfirmedFlag() == true)
				{
					SelectObject( hDC, BlackPen );
					SelectObject( hDC, BlackBrush );
					Rectangle( hDC, 6, 13, 60, 23 );// "hide" throttle
				}
				Tape_MVI_KEAS( hDC, MachNumber );
				Tape_Alpha( hDC, 0 );
				Tape_H_Hdot( hDC, Altitude_ft, vel.y );
				if ((Altitude_ft <= 200000) && (STS()->GetMET() <= 150)) AEPFD_BETA( hDC );
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				ADI_RATE_A( hDC, av.x, av.z, av.y, adirate );
				atterr = GetIDP()->GetAttitudeErrors_AscentGuidance();
				ADI_ERROR_A( hDC, atterr.x, atterr.z, atterr.y, adierr );
				AEPFD_GMETER_STATIC( hDC );
				AEPFD_GMETER_ACCEL( hDC );
				HSI_A( hDC, STS()->GetYaw(), STS()->GetBank() );// TODO set 0 as tgt plane
				if (0)// TODO TAL
				{
					AEPFD_dXTRK( hDC );
					AEPFD_RANGERW( hDC );// fix colision between this and dINC
				}
				AEPFD_XTRK( hDC );// TODO only NOM, TAL and ATO
				if (0) AEPFD_RANGERW( hDC );// TODO ECAL/BDA
				AEPFD_dINC( hDC );
				if (0) AEPFD_TGTINC( hDC );// TODO ATO
				break;
			case 104:
				SetTextColor( hDC, CR_GRAY_DARK );
				TextOut( hDC, 21, 4, "DAP:", 4 );
				SetTextColor( hDC, CR_WHITE );
				if (1) TextOut( hDC, 40, 4, "Auto", 4 );// TODO get TransDAP state
				else TextOut( hDC, 40, 4, "INRTL", 5 );

				SetTextColor( hDC, CR_GRAY_DARK );
				TextOut( hDC, 209, 4, "MM:", 3 );
				SetTextColor( hDC, CR_WHITE );
				if (0) TextOut( hDC, 225, 4, "104T", 4 );// TAL
				else if (0) TextOut( hDC, 225, 4, "104ATO", 6 );// ATO
				else if (0) TextOut( hDC, 225, 4, "104AOA", 6 );// AOA
				else if (0) TextOut( hDC, 225, 4, "104CA", 5 );// CA
				else TextOut( hDC, 225, 4, "104", 3 );// NOM

				SetTextColor( hDC, CR_GRAY_DARK );
				TextOut( hDC, 208, 13, "ATT:", 4 );
				SetTextColor( hDC, CR_WHITE );
				if (adiatt == 2) TextOut( hDC, 228, 13, "INRTL", 5 );
				else if (adiatt == 1) TextOut( hDC, 228, 13, "LVLH", 4 );
				else TextOut( hDC, 228, 13, "REF", 3 );

				Tape_MVI_KEAS( hDC, MachNumber );
				Tape_H_Hdot( hDC, Altitude_ft, vel.y );
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				ADI_RATE_A( hDC, av.x, av.z, av.y, adirate );
				ADI_ERROR_A( hDC, 0, 0, 0, adierr );// TODO
				HSI_E( hDC, STS()->GetYaw() );
				AEPFD_XTRK( hDC );// TODO only NOM, TAL and ATO
				AEPFD_dINC( hDC );
				break;
			case 105:
				AEPFD_Header_TransDAP( hDC, 105, adiatt );
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				ADI_RATE_A( hDC, av.x, av.z, av.y, adirate );
				ADI_ERROR_A( hDC, 0, 0, 0, adierr );// TODO
				break;
			case 106:
				AEPFD_Header_TransDAP( hDC, 106, adiatt );
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				ADI_RATE_A( hDC, av.x, av.z, av.y, adirate );
				ADI_ERROR_A( hDC, 0, 0, 0, adierr );// TODO
				break;
			case 201:
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				break;
			case 202:
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				break;
			case 301:
				AEPFD_Header_TransDAP( hDC, 301, adiatt );
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				ADI_RATE_A( hDC, av.x, av.z, av.y, adirate );
				atterr = GetIDP()->GetAttitudeErrors_AerojetDAP();
				ADI_ERROR_A( hDC, atterr.x, atterr.z, atterr.y, adierr );
				break;
			case 302:
				AEPFD_Header_TransDAP( hDC, 302, adiatt );
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				ADI_RATE_A( hDC, av.x, av.z, av.y, adirate );
				atterr = GetIDP()->GetAttitudeErrors_AerojetDAP();
				ADI_ERROR_A( hDC, atterr.x, atterr.z, atterr.y, adierr );
				break;
			case 303:
				AEPFD_Header_TransDAP( hDC, 303, adiatt );
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				ADI_RATE_A( hDC, av.x, av.z, av.y, adirate );
				atterr = GetIDP()->GetAttitudeErrors_AerojetDAP();
				ADI_ERROR_A( hDC, atterr.x, atterr.z, atterr.y, adierr );
				break;
			case 304:
				AEPFD_Header_AerojetDAP( hDC, 304, MachNumber );
				Tape_MVR_KEAS( hDC, MachNumber );
				Tape_Alpha( hDC, MachNumber );
				Tape_H_Hdot( hDC, Altitude_ft, vel.y );
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				ADI_RATE_A( hDC, av.x, av.z, av.y, adirate );
				atterr = GetIDP()->GetAttitudeErrors_AerojetDAP();
				ADI_ERROR_B( hDC, atterr.x, atterr.z, atterr.y, adierr );
				AEPFD_GMETER_STATIC( hDC );
				AEPFD_GMETER_NZ( hDC );
				HSI_E( hDC, STS()->GetYaw() );
				AEPFD_dAZ_HTA( hDC, MachNumber );
				AEPFD_RANGERW( hDC );
				AEPFD_HACCEL( hDC );
				break;
			case 305:
				AEPFD_Header_AerojetDAP( hDC, 305, MachNumber );
				if (MachNumber >= 0.9) Tape_MVR_KEAS( hDC, MachNumber );
				else Tape_KEAS_MVR( hDC, MachNumber );
				Tape_Alpha( hDC, MachNumber );
				Tape_H_Hdot( hDC, Altitude_ft, vel.y );
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				ADI_RATE_B( hDC, av.x, av.z, av.y, adirate, Altitude_ft );
				atterr = GetIDP()->GetAttitudeErrors_AerojetDAP();
				if (GetIDP()->GetWOW() == false) ADI_ERROR_C( hDC, GetIDP()->GetNZError(), atterr.z, atterr.y, adierr );
				else ADI_ERROR_D( hDC, atterr.x, atterr.z, atterr.y, adierr );
				AEPFD_GMETER_STATIC( hDC );
				AEPFD_GMETER_NZ( hDC );
				HSI_E( hDC, STS()->GetYaw() );
				if (GetIDP()->GetPrefinalState() == false)
				{
					AEPFD_dAZ_HTA( hDC, MachNumber );
					AEPFD_RANGEHACC( hDC );
				}
				AEPFD_RANGERW( hDC );
				AEPFD_GSI( hDC, Altitude_ft );
				break;
			case 601:
				AEPFD_Header_AscentDAP( hDC, 601, adiatt );
				if (0) Tape_MVR_KEAS( hDC, MachNumber );// TODO PPA
				Tape_MVI_KEAS( hDC, MachNumber );
				Tape_Alpha( hDC, 0 );
				Tape_H_Hdot( hDC, Altitude_ft, vel.y );
				AEPFD_BETA( hDC );
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				ADI_RATE_A( hDC, av.x, av.z, av.y, adirate );
				atterr = GetIDP()->GetAttitudeErrors_AscentGuidance();
				ADI_ERROR_A( hDC, atterr.x, atterr.z, atterr.y, adierr );
				AEPFD_GMETER_STATIC( hDC );
				AEPFD_GMETER_ACCEL( hDC );
				HSI_E( hDC, STS()->GetYaw() );
				AEPFD_dAZ_HTA( hDC, MachNumber );
				AEPFD_RANGERW( hDC );
				break;
			case 602:
				AEPFD_Header_AerojetDAP( hDC, 602, MachNumber );
				Tape_MVR_KEAS( hDC, MachNumber );
				Tape_Alpha( hDC, MachNumber );
				Tape_H_Hdot( hDC, Altitude_ft, vel.y );
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				ADI_RATE_A( hDC, av.x, av.z, av.y, adirate );
				atterr = GetIDP()->GetAttitudeErrors_AerojetDAP();
				if (0) ADI_ERROR_C( hDC, GetIDP()->GetNZError(), atterr.z, atterr.y, adierr );// TODO Nz hold
				else ADI_ERROR_B( hDC, atterr.x, atterr.z, atterr.y, adierr );
				AEPFD_GMETER_STATIC( hDC );
				AEPFD_GMETER_NZ( hDC );
				HSI_E( hDC, STS()->GetYaw() );
				AEPFD_dAZ_HTA( hDC, MachNumber );
				AEPFD_RANGERW( hDC );
				AEPFD_HACCEL( hDC );
				break;
			case 603:
				AEPFD_Header_AerojetDAP( hDC, 603, MachNumber );
				if (MachNumber >= 0.9) Tape_MVR_KEAS( hDC, MachNumber );
				else Tape_KEAS_MVR( hDC, MachNumber );
				Tape_Alpha( hDC, MachNumber );
				Tape_H_Hdot( hDC, Altitude_ft, vel.y );
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				ADI_RATE_B( hDC, av.x, av.z, av.y, adirate, Altitude_ft );
				atterr = GetIDP()->GetAttitudeErrors_AerojetDAP();
				if (GetIDP()->GetWOW() == false) ADI_ERROR_C( hDC, GetIDP()->GetNZError(), atterr.z, atterr.y, adierr );
				else ADI_ERROR_D( hDC, atterr.x, atterr.z, atterr.y, adierr );
				AEPFD_GMETER_STATIC( hDC );
				AEPFD_GMETER_NZ( hDC );
				HSI_E( hDC, STS()->GetYaw() );
				if (GetIDP()->GetPrefinalState() == false)
				{
					AEPFD_dAZ_HTA( hDC, MachNumber );
					AEPFD_RANGEHACC( hDC );
				}
				AEPFD_RANGERW( hDC );
				AEPFD_GSI( hDC, Altitude_ft );
				break;
			case 801:
				ADI_STATIC( hDC );
				ADI( hDC, attPitch, attRoll, attYaw );
				break;
			case 901:
				ADI_STATIC( hDC );// TODO no rate and error scales
				ADI( hDC, attPitch, attRoll, attYaw );
				AEPFD_GMETER_STATIC( hDC );
				HSI_E( hDC, STS()->GetYaw() );
				break;
			default:
				break;
		}
		RestoreDC( hDC, save );
		return;
	}

	void MDU::ORBITPFD( HDC hDC )
	{
		int MM = STS()->pSimpleGPC->GetMajorMode();
		//int adiatt = GetIDP()->GetADIAttitude();
		int adierr = GetIDP()->GetADIError();
		int adirate = GetIDP()->GetADIRate();
		VECTOR3 av;// x = pitch, y = yaw, z = roll
		STS()->GetAngularVel( av );
		av *= DEG;
		av.z = -av.z;
		av.x = -av.x;
		VECTOR3 atterr = GetIDP()->GetAttitudeCommandErrors();
		double attRoll = -STS()->GetBank() * DEG;
		if (attRoll <= 0) attRoll += 360;
		double attPitch = STS()->GetPitch() * DEG;
		if (attPitch <= 0) attPitch += 360;
		double attYaw = 0;//STS()->GetSlipAngle() * DEG;
		//if (attYaw <= 0) attYaw += 360;

		int save = SaveDC( hDC );

		SelectObject( hDC, TahomaFont_h10w4 );

		ADI_STATIC_ORBIT( hDC );
		if ((MM == 201) || (MM == 202) || (MM == 801))
		{
			ADI_ORBIT( hDC, attPitch, attRoll, attYaw );
			ADI_RATE_ORBIT( hDC, av.x, av.z, av.y, adirate );
			ADI_ERROR_ORBIT( hDC, atterr.x, atterr.z, atterr.y, adierr );
		}
		else ADI_ORBIT( hDC, 129, 77, 14 );


		RestoreDC( hDC, save );
		return;
	}

	void MDU::CreateGDIObjects()
	{
		BlackBrush = CreateSolidBrush( CR_BLACK );
		WhiteBrush = CreateSolidBrush( CR_WHITE );
		GrayLightBrush = CreateSolidBrush( CR_GRAY_LIGHT );
		GrayDarkBrush = CreateSolidBrush( CR_GRAY_DARK );
		GreenBrush = CreateSolidBrush( CR_GREEN );
		MagentaBrush = CreateSolidBrush( CR_MAGENTA );
		YellowBrush = CreateSolidBrush( CR_YELLOW );
		RedBrush = CreateSolidBrush( CR_RED );

		BlackPen = CreatePen( PS_SOLID, 0, CR_BLACK );
		WhitePen = CreatePen( PS_SOLID, 0, CR_WHITE );
		GrayLightPen = CreatePen( PS_SOLID, 0, CR_GRAY_LIGHT );
		GrayDarkPen = CreatePen( PS_SOLID, 0, CR_GRAY_DARK );
		RedPen = CreatePen( PS_SOLID, 0, CR_RED );
		GreenPen = CreatePen( PS_SOLID, 0, CR_GREEN );
		GreenThickPen = CreatePen( PS_SOLID, 2, CR_GREEN );
		YellowPen = CreatePen( PS_SOLID, 0, CR_YELLOW );
		MagentaPen = CreatePen( PS_SOLID, 0, CR_MAGENTA );
		TurquoisePen = CreatePen( PS_SOLID, 0, CR_TURQUOISE );
		hOverbrightPen = CreatePen( PS_SOLID, 0, CR_DPS_OVERBRIGHT );
		hNormalPen = CreatePen( PS_SOLID, 0, CR_DPS_NORMAL );
		LOGBRUSH lb = {BS_SOLID, CR_DPS_NORMAL, 0};
		DWORD pstyle[2] = {2, 1};
		hDashedNormalPen = ExtCreatePen( PS_COSMETIC | PS_USERSTYLE, 1, &lb, 2, pstyle );
		MenuPen = CreatePen( PS_SOLID, 0, CR_MENU_COLOR );

		TahomaFont_h10w4 = CreateFont( 10, 4, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, OEM_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FIXED_PITCH, "Tahoma" );
		TahomaFont_h7w3 = CreateFont( 7, 3, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, OEM_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FIXED_PITCH, "Tahoma" );
		TahomaFont_h17w6 = CreateFont( 17, 6, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, OEM_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FIXED_PITCH, "Tahoma" );
		return;
	}

	void MDU::DestroyGDIObjects()
	{
		DeleteObject( BlackBrush );
		DeleteObject( WhiteBrush );
		DeleteObject( GrayLightBrush );
		DeleteObject( GrayDarkBrush );
		DeleteObject( GreenBrush );
		DeleteObject( MagentaBrush );
		DeleteObject( YellowBrush );
		DeleteObject( RedBrush );

		DeleteObject( BlackPen );
		DeleteObject( WhitePen );
		DeleteObject( GrayLightPen );
		DeleteObject( GrayDarkPen );
		DeleteObject( RedPen );
		DeleteObject( GreenPen );
		DeleteObject( GreenThickPen );
		DeleteObject( YellowPen );
		DeleteObject( MagentaPen );
		DeleteObject( TurquoisePen );
		DeleteObject( hOverbrightPen );
		DeleteObject( hNormalPen );
		DeleteObject( hDashedNormalPen );
		DeleteObject( MenuPen );

		DeleteObject( TahomaFont_h10w4 );
		DeleteObject( TahomaFont_h7w3 );
		DeleteObject( TahomaFont_h17w6 );
		return;
	}

	void MDU::Tape_Create( void )
	{
		hDC_Tapes = CreateCompatibleDC( GetDC( NULL ) );
		HBITMAP hBM = CreateCompatibleBitmap( GetDC( NULL ), 113, 3700 );
		SelectObject( hDC_Tapes, hBM );
		SelectObject( hDC_Tapes, WhitePen );
		SelectObject( hDC_Tapes, WhiteBrush );
		Rectangle( hDC_Tapes, 0, 0, 113, 3700 );
		SelectObject( hDC_Tapes, TahomaFont_h10w4 );
		// TODO cleanup at end?

		// draw tapes
		int offset = 57;
		char cbuf[8];
		int y = 0;

		// Mach/V
		// 1K in window (1ft = 0.114px) (4-27K)
		// M0.5 in window (M1 = 228px) (0-4)
		// 2622 + 912 = 3534px + offsets
		SelectObject( hDC_Tapes, BlackPen );
		SelectObject( hDC_Tapes, BlackBrush );
		Rectangle( hDC_Tapes, 0, 3534 + offset, 22, 3534 + offset + offset );

		SetTextColor( hDC_Tapes, CR_BLACK );
		SetBkMode( hDC_Tapes, TRANSPARENT );
		SelectObject( hDC_Tapes, BlackPen );
		for (int i = 270; i >= 42; i -= 1)
		{
			y = Round( ((270 - i) * 11.4) + offset );

			if ((i % 10) == 0)
			{
				sprintf_s( cbuf, 8, "%4.1fK", (double)i / 10 );
				TextOut( hDC_Tapes, 2, y - 5, cbuf, strlen( cbuf ) );
			}
			else if ((i % 2) == 0)
			{
				sprintf_s( cbuf, 8, "%4.1f", (double)i / 10 );
				TextOut( hDC_Tapes, 4, y - 5, cbuf, strlen( cbuf ) );
			}
			else
			{
				MoveToEx( hDC_Tapes, 4, y, NULL );
				LineTo( hDC_Tapes, 18, y );
			}
		}
		for (int i = 40; i >= 0; i--)
		{
			y = Round( ((40 - i) * 22.8) + offset ) + 2622;

			if ((i % 2) == 0)
			{
				if ((i % 10) == 0)
				{
					sprintf_s( cbuf, 8, "%2.0f", (double)i / 10 );
					TextOut( hDC_Tapes, 6, y - 5, cbuf, strlen( cbuf ) );
				}
				else
				{
					sprintf_s( cbuf, 8, "%3.1f", (double)i / 10 );
					TextOut( hDC_Tapes, 6, y - 5, cbuf, strlen( cbuf ) );
				}
			}
			else
			{
				MoveToEx( hDC_Tapes, 4, y, NULL );
				LineTo( hDC_Tapes, 18, y );
			}
		}
			
		// KEAS
		// 30KEAS in window (1KEAS = 3.8px) (500-0)
		// 1900px + offsets
		SelectObject( hDC_Tapes, BlackPen );
		SelectObject( hDC_Tapes, BlackBrush );
		Rectangle( hDC_Tapes, 23, 1900 + offset, 45, 1900 + offset + offset );

		SetTextColor( hDC_Tapes, CR_BLACK );
		SetBkMode( hDC_Tapes, TRANSPARENT );
		SelectObject( hDC_Tapes, BlackPen );
		for (int i = 500; i >= 0; i -= 5)
		{
			y = Round( ((500 - i) * 3.8) + offset );

			if ((i % 10) == 0)
			{
				sprintf_s( cbuf, 8, "%3d", i );
				TextOut( hDC_Tapes, 28, y - 5, cbuf, strlen( cbuf ) );
			}
			else
			{
				MoveToEx( hDC_Tapes, 27, y, NULL );
				LineTo( hDC_Tapes, 41, y );
			}
		}

		// alpha
		// 20º in window (1º = 5.7px)
		// 2052px long + offsets
		SelectObject( hDC_Tapes, GrayLightPen );
		SelectObject( hDC_Tapes, GrayLightBrush );
		Rectangle( hDC_Tapes, 45, 1026 + offset, 67, 2052 + offset + offset );

		SetTextColor( hDC_Tapes, CR_BLACK );
		SetBkMode( hDC_Tapes, TRANSPARENT );
		SelectObject( hDC_Tapes, BlackPen );
		for (int i = 180; i >= -180; i--)
		{
			if (i < 0)// yes, not very efficient but the loop only runs once
			{
				SetTextColor( hDC_Tapes, CR_WHITE );
				SelectObject( hDC_Tapes, WhitePen );
			}

			y = Round( ((180 - i) * 5.7) + offset );

			MoveToEx( hDC_Tapes, 62, y, NULL );
			LineTo( hDC_Tapes, 67, y );

			if ((i % 5) == 0)
			{
				sprintf_s( cbuf, 8, "%4d", i );
				TextOut( hDC_Tapes, 45, y - 5, cbuf, strlen( cbuf ) );
			}
		}

		// H
		// NM scale 10NM in window (1NM = 11.4px) (165-65.83137NM)
		// FT scale 100Kft in window (1ft = 0.00114px) (400-100Kft)
		// FT scale 11.5Kft in window (1ft = 0.00991px) (100-30Kft)
		// FT scale 6Kft in window (1ft = 0.019px) n1(n5K)/n1000 m0.5 (30-2Kft)
		//0 FT scale 1000ft in window (1ft = 0.114px) (2000-200ft)
		//0 FT scale 150ft in window (1ft = 0.76px) n50m10 (200-0ft)
		//? FT scale 500ft in window (1ft = 0.228px) (0-(-100)ft)
		//? FT scale 40ft in window (1ft = 0.057px) ((-100)-(-1100)ft)
		// 1130.522382 + 342 + 693.7 + 532 + 205.2 + 152 + 0 + 0 = 0px long + offsets
		SelectObject( hDC_Tapes, YellowPen );
		SelectObject( hDC_Tapes, YellowBrush );
		Rectangle( hDC_Tapes, 68, 2698 + offset, 90, 3055 + offset );

		SelectObject( hDC_Tapes, GrayLightPen );
		SelectObject( hDC_Tapes, GrayLightBrush );
		Rectangle( hDC_Tapes, 68, 3055 + offset, 90, 3380 + offset + offset );

		SetTextColor( hDC_Tapes, CR_BLACK );
		SetBkMode( hDC_Tapes, TRANSPARENT );
		SelectObject( hDC_Tapes, BlackPen );
		for (int i = 165; i >= 67; i--)
		{
			y = Round( ((165 - i) * 11.4) + offset );

			if ((i % 5) == 0)
			{
				sprintf_s( cbuf, 8, "%3dM", i );
				TextOut( hDC_Tapes, 70, y - 5, cbuf, strlen( cbuf ) );
			}
			else
			{
				MoveToEx( hDC_Tapes, 83, y, NULL );
				LineTo( hDC_Tapes, 90, y );
			}
		}
		for (int i = 400; i >= 100; i -= 5)
		{
			y = Round( ((400 - i) * 1.14) + offset ) + 1131;

			if ((i % 50) == 0)
			{
				sprintf_s( cbuf, 8, "%3dK", i );
				TextOut( hDC_Tapes, 71, y - 5, cbuf, strlen( cbuf ) );
			}
			else if ((i % 10) == 0)
			{
				MoveToEx( hDC_Tapes, 68, y, NULL );
				LineTo( hDC_Tapes, 79, y );
			}
		}
		for (int i = 99; i >= 30; i--)
		{
			y = Round( ((100 - i) * 9.91) + offset ) + 1473;

			if ((i % 5) == 0)
			{
				sprintf_s( cbuf, 8, "%2dK", i );
				TextOut( hDC_Tapes, 73, y - 5, cbuf, strlen( cbuf ) );
			}
			else
			{
				MoveToEx( hDC_Tapes, 72, y, NULL );
				LineTo( hDC_Tapes, 86, y );
			}
		}
		for (int i = 295; i > 20; i -= 5)
		{
			y = Round( ((300 - i) * 1.9) + offset ) + 2167;

			if ((i % 50) == 0)
			{
				sprintf_s( cbuf, 8, "%2.0fK", (double)i / 10 );
				TextOut( hDC_Tapes, 73, y - 5, cbuf, strlen( cbuf ) );
			}
			else if ((i % 10) == 0)
			{
				sprintf_s( cbuf, 8, "%2.0f", (double)i / 10 );
				TextOut( hDC_Tapes, 75, y - 5, cbuf, strlen( cbuf ) );
			}
			else
			{
				MoveToEx( hDC_Tapes, 72, y, NULL );
				LineTo( hDC_Tapes, 86, y );
			}
		}

		for (int i = 20; i > 2; i -= 1)
		{
			y = Round( ((20 - i) * 11.4) + offset ) + 2699;

			if ((i % 2) == 0)
			{
				sprintf_s( cbuf, 8, "%4d", i * 100 );
				TextOut( hDC_Tapes, 71, y - 5, cbuf, strlen( cbuf ) );
			}
			else
			{
				MoveToEx( hDC_Tapes, 72, y, NULL );
				LineTo( hDC_Tapes, 86, y );
			}
		}
		for (int i = 20; i >= 0; i -= 1)
		{
			y = Round( ((20 - i) * 7.6) + offset ) + 2904;

			if ((i % 5) == 0)
			{
				sprintf_s( cbuf, 8, "%3d", i * 10 );
				TextOut( hDC_Tapes, 73, y - 5, cbuf, strlen( cbuf ) );
			}
			else
			{
				MoveToEx( hDC_Tapes, 72, y, NULL );
				LineTo( hDC_Tapes, 86, y );
			}
		}
		SetTextColor( hDC_Tapes, CR_WHITE );
		for (int i = 0; i >= -10; i -= 1)
		{
			y = Round( ((20 - i) * 7.6) + offset ) + 2904;

			if ((i % 5) == 0)
			{
				sprintf_s( cbuf, 8, "%3d", i * 10 );
				TextOut( hDC_Tapes, 73, y - 5, cbuf, strlen( cbuf ) );
			}
			else
			{
				MoveToEx( hDC_Tapes, 72, y, NULL );
				LineTo( hDC_Tapes, 86, y );
			}
		}

		// Hdot
		// small scale 160ft in window (1ft = 0.7125px) (800-(-800))
		// large scale 800ft in window (1ft = 0.1425px) (3000-800/(-800)-(-3000))
		// 313.5 + 1140 + 313.5 = 1767px long + offsets
		SelectObject( hDC_Tapes, GrayLightPen );
		SelectObject( hDC_Tapes, GrayLightBrush );
		Rectangle( hDC_Tapes, 91, 884 + offset, 113, 1767 + offset + offset );

		SetTextColor( hDC_Tapes, CR_BLACK );
		SetBkMode( hDC_Tapes, TRANSPARENT );
		SelectObject( hDC_Tapes, BlackPen );
		for (int i = 30; i >= 8; i--)
		{
			y = Round( ((30 - i) * 14.25) + offset );

			if (i < 10)
			{
				sprintf_s( cbuf, 8, "%4d", i * 100 );
				TextOut( hDC_Tapes, 94, y - 5, cbuf, strlen( cbuf ) );
			}
			else if ((i % 5) == 0)
			{
				sprintf_s( cbuf, 8, "%3.1fK", (double)i / 10 );
				TextOut( hDC_Tapes, 94, y - 5, cbuf, strlen( cbuf ) );
			}
			else
			{
				MoveToEx( hDC_Tapes, 95, y, NULL );
				LineTo( hDC_Tapes, 109, y );
			}
		}
		for (int i = 79; i >= 0; i--)
		{
			y = Round( ((80 - i) * 7.125) + 313.5 + offset );

			if ((i % 2) == 0)
			{
				sprintf_s( cbuf, 8, "%3d", i * 10 );
				TextOut( hDC_Tapes, 96, y - 5, cbuf, strlen( cbuf ) );
			}
			else
			{
				MoveToEx( hDC_Tapes, 96, y, NULL );
				LineTo( hDC_Tapes, 108, y );
			}
		}
		SetTextColor( hDC_Tapes, CR_WHITE );
		SelectObject( hDC_Tapes, WhitePen );
		for (int i = -1; i >= -79; i--)
		{
			y = Round( ((80 - i) * 7.125) + 313.5 + offset );

			if ((i % 2) == 0)
			{
				sprintf_s( cbuf, 8, "%4d", i * 10 );
				TextOut( hDC_Tapes, 94, y - 5, cbuf, strlen( cbuf ) );
			}
			else
			{
				MoveToEx( hDC_Tapes, 96, y, NULL );
				LineTo( hDC_Tapes, 108, y );
			}
		}
		for (int i = -8; i >= -30; i--)
		{
			y = Round( ((-8 - i) * 14.25) + 313.5 + 1140 + offset );

			if (i > -10)
			{
				sprintf_s( cbuf, 8, "%4d", i * 100 );
				TextOut( hDC_Tapes, 94, y - 5, cbuf, strlen( cbuf ) );
			}
			else if ((i % 5) == 0)
			{
				sprintf_s( cbuf, 8, "%4.1fK", (double)i / 10 );
				TextOut( hDC_Tapes, 93, y - 5, cbuf, strlen( cbuf ) );
			}
			else
			{
				MoveToEx( hDC_Tapes, 95, y, NULL );
				LineTo( hDC_Tapes, 109, y );
			}
		}
		return;
	}

	void MDU::ADI_Create( void )
	{
		hDC_ADI = CreateCompatibleDC( GetDC( NULL ) );
		HBITMAP hBM = CreateCompatibleBitmap( GetDC( NULL ), 100, 100 );
		SelectObject( hDC_ADI, hBM );
		SelectObject( hDC_ADI, TahomaFont_h7w3 );

		hDC_ADIMASK = CreateCompatibleDC( hDC_ADI );
		hBM = CreateCompatibleBitmap( hDC_ADI, 100, 100 );
		SelectObject( hDC_ADIMASK, hBM );
		SelectObject( hDC_ADIMASK, WhitePen );
		SelectObject( hDC_ADIMASK, WhiteBrush );
		::Ellipse( hDC_ADIMASK, 1, 1, 99, 99 );

		hDC_ADI_ORBIT = CreateCompatibleDC( hDC_ADI );
		hBM = CreateCompatibleBitmap( hDC_ADI, 112, 112 );
		SelectObject( hDC_ADI_ORBIT, hBM );
		SelectObject( hDC_ADI_ORBIT, TahomaFont_h10w4 );

		hDC_ADIMASK_ORBIT = CreateCompatibleDC( hDC_ADI );
		hBM = CreateCompatibleBitmap( hDC_ADI, 112, 112 );
		SelectObject( hDC_ADIMASK_ORBIT, hBM );
		SelectObject( hDC_ADIMASK_ORBIT, WhitePen );
		SelectObject( hDC_ADIMASK_ORBIT, WhiteBrush );
		::Ellipse( hDC_ADIMASK_ORBIT, 1, 1, 111, 111 );
		return;
	}

	void MDU::Tape_KEAS_MVR( HDC hDC, double MachNumber )
	{
		char cbuf[8];
		double keas = sqrt( STS()->GetDynPressure() * PA2PSF ) * 17.18;
		SelectObject( hDC, WhitePen );
		SelectObject( hDC, BlackBrush );
		Rectangle( hDC, 9, 39, 33, 155 );
		SelectObject( hDC, GrayDarkPen );
		Rectangle( hDC, 9, 159, 33, 171 );
		SetTextColor( hDC, CR_GRAY_DARK );
		TextOut( hDC, 12, 30, "KEAS", 4 );
		TextOut( hDC, 12, 172, "M/VR", 4 );

		double tapekeas = keas;
		if (tapekeas > 500) tapekeas = 500;
		BitBlt( hDC, 10, 40, 22, 114, hDC_Tapes, 23, 1900 - Round( tapekeas * 3.8 ), SRCCOPY );

		Rectangle( hDC, 9, 91, 33, 105 );
		SetTextColor( hDC, CR_WHITE );
		sprintf_s( cbuf, 8, "%3.0f", keas );
		TextOut( hDC, 15, 93, cbuf, strlen( cbuf ) );
		sprintf_s( cbuf, 8, "%5.2f", MachNumber );
		TextOut( hDC, 13, 160, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::Tape_MVR_KEAS( HDC hDC, double MachNumber )
	{
		char cbuf[8];
		int pos;
		double keas = sqrt( STS()->GetDynPressure() * PA2PSF ) * 17.18;
		SelectObject( hDC, WhitePen );
		SelectObject( hDC, BlackBrush );
		Rectangle( hDC, 9, 39, 33, 155 );
		SelectObject( hDC, GrayDarkPen );
		Rectangle( hDC, 9, 159, 33, 171 );
		SetTextColor( hDC, CR_GRAY_DARK );
		TextOut( hDC, 12, 30, "M/VR", 4 );
		TextOut( hDC, 12, 172, "KEAS", 4 );
		SetTextColor( hDC, CR_WHITE );
		if (MachNumber > 4)
		{
			double vr = STS()->GetAirspeed() * MPS2FPS;

			pos = 3078 - Round( vr * 0.114 );
			BitBlt( hDC, 10, 40, 22, 114, hDC_Tapes, 0, pos, SRCCOPY );

			Rectangle( hDC, 9, 91, 33, 105 );
			sprintf_s( cbuf, 8, "%5.0f", vr );// ft
			TextOut( hDC, 11, 93, cbuf, strlen( cbuf ) );
		}
		else
		{
			pos = 3534 - Round( MachNumber * 228 );
			BitBlt( hDC, 10, 40, 22, 114, hDC_Tapes, 0, pos, SRCCOPY );

			Rectangle( hDC, 9, 91, 33, 105 );
			sprintf_s( cbuf, 8, "%5.2f", MachNumber );// Mach
			TextOut( hDC, 12, 93, cbuf, strlen( cbuf ) );
		}
		sprintf_s( cbuf, 8, "%3.0f", keas );
		TextOut( hDC, 14, 160, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::Tape_MVI_KEAS( HDC hDC, double MachNumber )
	{
		char cbuf[8];
		int pos;
		double keas = sqrt( STS()->GetDynPressure() * PA2PSF ) * 17.18;
		SelectObject( hDC, WhitePen );
		SelectObject( hDC, BlackBrush );
		Rectangle( hDC, 9, 39, 33, 155 );
		SelectObject( hDC, GrayDarkPen );
		Rectangle( hDC, 9, 159, 33, 171 );
		SetTextColor( hDC, CR_GRAY_DARK );
		TextOut( hDC, 12, 30, "M/VI", 4 );
		TextOut( hDC, 12, 172, "KEAS", 4 );
		SetTextColor( hDC, CR_WHITE );
		if (MachNumber > 4)
		{
			VECTOR3 v3vi;
			STS()->GetRelativeVel( STS()->GetSurfaceRef(), v3vi );
			double vi = length( v3vi ) * MPS2FPS;
			
			pos = 3078 - Round( vi * 0.114 );
			BitBlt( hDC, 10, 40, 22, 114, hDC_Tapes, 0, pos, SRCCOPY );

			Rectangle( hDC, 9, 91, 33, 105 );
			sprintf_s( cbuf, 8, "%5.0f", vi );// ft
			TextOut( hDC, 11, 93, cbuf, strlen( cbuf ) );
		}
		else
		{
			pos = 3534 - Round( MachNumber * 228 );
			BitBlt( hDC, 10, 40, 22, 114, hDC_Tapes, 0, pos, SRCCOPY );

			Rectangle( hDC, 9, 91, 33, 105 );
			sprintf_s( cbuf, 8, "%5.2f", MachNumber );// Mach
			TextOut( hDC, 12, 93, cbuf, strlen( cbuf ) );
		}
		sprintf_s( cbuf, 8, "%3.0f", keas );
		TextOut( hDC, 14, 160, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::Tape_Alpha( HDC hDC, double MachNumber )
	{
		SelectObject( hDC, GrayDarkPen );
		SelectObject( hDC, BlackBrush );
		::Ellipse( hDC, 46, 34, 50, 38 );
		SetPixel( hDC, 50, 34, CR_GRAY_DARK );
		SetPixel( hDC, 50, 37, CR_GRAY_DARK );
		SelectObject( hDC, WhitePen );
		Rectangle( hDC, 37, 39, 60, 155 );

		double alpha = STS()->GetAOA() * DEG;

		BitBlt( hDC, 38, 40, 21, 114, hDC_Tapes, 45, 1026 - Round( alpha * 5.7 ), SRCCOPY );

		if ((MachNumber > 0) && (MachNumber < 3))
		{
			// max L/D (linear aprox)
			double maxLD;
			if (MachNumber < 0.9) maxLD = 10.5;
			else if (MachNumber < 1.1) maxLD = (MachNumber * 15) - 3;
			else if (MachNumber < 2) maxLD = (MachNumber * 1.6667) + 11.6667;
			else maxLD = (MachNumber * 2) + 11;

			maxLD -= alpha;
			if (fabs( maxLD ) < 10)
			{
				int pos = Round( maxLD * 5.7 );
				POINT diamond[4];
				diamond[0].x = 56;// start at top moving cw
				diamond[0].y = 94 - pos;
				diamond[1].x = 60;
				diamond[1].y = 98 - pos;
				diamond[2].x = 56;
				diamond[2].y = 102 - pos;
				diamond[3].x = 52;
				diamond[3].y = 98 - pos;
				SelectObject( hDC, MagentaBrush );
				SelectObject( hDC, BlackPen );
				Polygon( hDC, diamond, 4 );

				diamond[0].y += 3;
				diamond[1].x = 57;
				diamond[2].y -= 3;
				diamond[3].x = 55;
				SelectObject( hDC, BlackBrush );
				Polygon( hDC, diamond, 4 );
			}
		}

		SelectObject( hDC, GrayDarkPen );
		SelectObject( hDC, BlackBrush );
		POINT poly[5] = {{59,98},{53,105},{35,105},{35,91},{53,91}};// start at tip moving cw
		Polygon( hDC, poly, 5 );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%6.1f", alpha );
		TextOut( hDC, 36, 93, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::Tape_H_Hdot( HDC hDC, double Altitude_ft, double Hdot )
	{
		SetTextColor( hDC, CR_GRAY_DARK );
		TextOut( hDC, 212, 30, "H", 1 );
		SelectObject( hDC, WhitePen );
		Rectangle( hDC, 203, 39, 227, 155 );
		int pos;
		double Altitude_NM = 0;
		if (Altitude_ft > 400000)
		{
			Altitude_NM = Altitude_ft * F2NM;
			if (Altitude_NM > 165) pos = 1881;
			else pos = 1881 - Round( Altitude_NM * 11.4 );
		}
		else if (Altitude_ft > 100000)
		{
			pos = 1587 - Round( Altitude_ft * 0.00114 );
		}
		else if (Altitude_ft > 30000)
		{
			pos = 2464 - Round( Altitude_ft * 0.00991 );
		}
		else if (Altitude_ft > 2000)
		{
			pos = 2737 - Round( Altitude_ft * 0.019 );
		}
		else if (Altitude_ft > 200)
		{
			pos = 2927 - Round( Altitude_ft * 0.114 );
		}// HACK below 0
		else// if (Altitude_ft > 0)
		{
			pos = 3056 - Round( Altitude_ft * 0.76 );
		}
		/*else if (Altitude_ft > -100)
		{
			pos = 0;// TODO
		}
		else if (Altitude_ft > -1100)
		{
			pos = 0;// TODO
		}
		else
		{
			if (Altitude_ft < -1100) Altitude_ft = -1100;
			pos = 0;// TODO
		}*/
		BitBlt( hDC, 204, 40, 22, 114, hDC_Tapes, 68, pos, SRCCOPY );

		SelectObject( hDC, GrayDarkPen );
		SelectObject( hDC, BlackBrush );
		Rectangle( hDC, 203, 91, 227, 105 );
		SetTextColor( hDC, CR_WHITE );
		char cbuf[8];
		if (Altitude_ft < 10000)
		{
			sprintf_s( cbuf, 8, "%4d", (int)Altitude_ft );
			TextOut( hDC, 207, 93, cbuf, strlen( cbuf ) );
		}
		else if (Altitude_ft < 400000)
		{
			sprintf_s( cbuf, 8, "%3d", (int)Altitude_ft / 1000 );
			TextOut( hDC, 206, 93, cbuf, strlen( cbuf ) );
			SetTextColor( hDC, CR_GRAY_DARK );
			TextOut( hDC, 220, 93, "K", 1 );
		}
		else
		{
			sprintf_s( cbuf, 8, "%3.0f", Altitude_NM );
			TextOut( hDC, 206, 93, cbuf, strlen( cbuf ) );
			SetTextColor( hDC, CR_GRAY_DARK );
			TextOut( hDC, 220, 93, "M", 1 );
		}

		SetTextColor( hDC, CR_GRAY_DARK );
		SetPixel( hDC, 241, 29, CR_GRAY_DARK );
		TextOut( hDC, 239, 30, "H", 1 );
		SelectObject( hDC, WhitePen );
		Rectangle( hDC, 230, 39, 254, 155 );
		if (Hdot > 800)
		{
			if (Hdot > 3000) Hdot = 3000;
			pos = Round( (3000 - Hdot) * 0.1425 );
		}
		else if (Hdot > -800)
		{
			pos = Round( 883.5 - (Hdot * 0.7125) );
		}
		else
		{
			if (Hdot < -3000) Hdot = -3000;
			pos = Round( 1339.5 - (Hdot * 0.1425) );
		}
		BitBlt( hDC, 231, 40, 22, 114, hDC_Tapes, 91, pos, SRCCOPY );

		SelectObject( hDC, GrayDarkPen );
		SelectObject( hDC, BlackBrush );
		Rectangle( hDC, 230, 91, 254, 105 );

		SetTextColor( hDC, CR_WHITE );
		if ((Hdot < 1000) && (Hdot > -1000))
		{
			sprintf_s( cbuf, 8, "%4.0f", Hdot );
			TextOut( hDC, 233, 93, cbuf, strlen( cbuf ) );
		}
		else
		{
			int itmp = (int)Hdot;
			Hdot = itmp % 100;
			if (Hdot == 0) Hdot = (double)itmp / 1000;
			else Hdot = ((itmp - Hdot) / 1000);
			sprintf_s( cbuf, 8, "%4.1f", Hdot );
			TextOut( hDC, 231, 93, cbuf, strlen( cbuf ) );

			SetTextColor( hDC, CR_GRAY_DARK );
			TextOut( hDC, 245, 93, "K", 1 );
		}
		return;
	}

	void MDU::Tapes_Invalid( HDC hDC )
	{
		SelectObject( hDC, RedPen );
		SelectObject( hDC, BlackBrush );
		Rectangle( hDC, 9, 39, 33, 155 );
		Rectangle( hDC, 9, 91, 33, 105 );
		Rectangle( hDC, 9, 159, 33, 171 );
		Rectangle( hDC, 37, 39, 60, 155 );
		POINT poly[5] = {{59,98},{53,105},{35,105},{35,91},{53,91}};// start at tip moving cw
		Polygon( hDC, poly, 5 );

		Rectangle( hDC, 203, 39, 227, 155 );
		Rectangle( hDC, 203, 91, 227, 105 );
		Rectangle( hDC, 230, 39, 254, 155 );
		Rectangle( hDC, 230, 91, 254, 105 );

		SetTextColor( hDC, CR_GRAY_DARK );
		TextOut( hDC, 12, 30, "M/V", 3 );
		TextOut( hDC, 12, 172, "KEAS", 4 );

		SelectObject( hDC, GrayDarkPen );
		::Ellipse( hDC, 46, 34, 50, 38 );
		SetPixel( hDC, 50, 34, CR_GRAY_DARK );
		SetPixel( hDC, 50, 37, CR_GRAY_DARK );

		TextOut( hDC, 212, 30, "H", 1 );
		SetPixel( hDC, 241, 29, CR_GRAY_DARK );
		TextOut( hDC, 239, 30, "H", 1 );
		return;
	}

	void MDU::ADI_STATIC( HDC hDC )
	{
		SetTextColor( hDC, CR_GRAY_DARK );
		TextOut( hDC, 174, 25, "R", 1 );
		TextOut( hDC, 174, 32, "P", 1 );
		TextOut( hDC, 174, 39, "Y", 1 );

		SelectObject( hDC, WhitePen );
		::Ellipse( hDC, 65, 37, 179, 151 );

		// rate scales
		// top scale
		SelectObject( hDC, GrayDarkPen );
		MoveToEx( hDC, 87, 27, NULL );
		LineTo( hDC, 157, 27 );
		MoveToEx( hDC, 87, 27, NULL );
		LineTo( hDC, 87, 34 );
		MoveToEx( hDC, 94, 27, NULL );
		LineTo( hDC, 94, 32 );
		MoveToEx( hDC, 101, 27, NULL );
		LineTo( hDC, 101, 32 );
		MoveToEx( hDC, 108, 27, NULL );
		LineTo( hDC, 108, 32 );
		MoveToEx( hDC, 115, 27, NULL );
		LineTo( hDC, 115, 32 );
		MoveToEx( hDC, 122, 27, NULL );
		LineTo( hDC, 122, 34 );
		MoveToEx( hDC, 129, 27, NULL );
		LineTo( hDC, 129, 32 );
		MoveToEx( hDC, 136, 27, NULL );
		LineTo( hDC, 136, 32 );
		MoveToEx( hDC, 143, 27, NULL );
		LineTo( hDC, 143, 32 );
		MoveToEx( hDC, 150, 27, NULL );
		LineTo( hDC, 150, 32 );
		MoveToEx( hDC, 157, 27, NULL );
		LineTo( hDC, 157, 34 );

		// side scale
		MoveToEx( hDC, 190, 59, NULL );
		LineTo( hDC, 190, 129 );
		MoveToEx( hDC, 190, 59, NULL );
		LineTo( hDC, 183, 59 );
		MoveToEx( hDC, 190, 66, NULL );
		LineTo( hDC, 185, 66 );
		MoveToEx( hDC, 190, 73, NULL );
		LineTo( hDC, 185, 73 );
		MoveToEx( hDC, 190, 80, NULL );
		LineTo( hDC, 185, 80 );
		MoveToEx( hDC, 190, 87, NULL );
		LineTo( hDC, 185, 87 );
		MoveToEx( hDC, 190, 94, NULL );
		LineTo( hDC, 183, 94 );
		MoveToEx( hDC, 190, 101, NULL );
		LineTo( hDC, 185, 101 );
		MoveToEx( hDC, 190, 108, NULL );
		LineTo( hDC, 185, 108 );
		MoveToEx( hDC, 190, 115, NULL );
		LineTo( hDC, 185, 115 );
		MoveToEx( hDC, 190, 122, NULL );
		LineTo( hDC, 185, 122 );
		MoveToEx( hDC, 190, 129, NULL );
		LineTo( hDC, 183, 129 );

		// bottom scale
		MoveToEx( hDC, 87, 163, NULL );
		LineTo( hDC, 157, 163 );
		MoveToEx( hDC, 87, 163, NULL );
		LineTo( hDC, 87, 156 );
		MoveToEx( hDC, 94, 163, NULL );
		LineTo( hDC, 94, 158 );
		MoveToEx( hDC, 101, 163, NULL );
		LineTo( hDC, 101, 158 );
		MoveToEx( hDC, 108, 163, NULL );
		LineTo( hDC, 108, 158 );
		MoveToEx( hDC, 115, 163, NULL );
		LineTo( hDC, 115, 158 );
		MoveToEx( hDC, 122, 163, NULL );
		LineTo( hDC, 122, 156 );
		MoveToEx( hDC, 129, 163, NULL );
		LineTo( hDC, 129, 158 );
		MoveToEx( hDC, 136, 163, NULL );
		LineTo( hDC, 136, 158 );
		MoveToEx( hDC, 143, 163, NULL );
		LineTo( hDC, 143, 158 );
		MoveToEx( hDC, 150, 163, NULL );
		LineTo( hDC, 150, 158 );
		MoveToEx( hDC, 157, 163, NULL );
		LineTo( hDC, 157, 156 );

		// roll lines
		// 10º/190º
		MoveToEx( hDC, 113, 43, NULL );
		LineTo( hDC, 131, 145 );
		// 20º/200º
		MoveToEx( hDC, 104, 45, NULL );
		LineTo( hDC, 140, 143 );
		// 40º/220º
		MoveToEx( hDC, 89, 54, NULL );
		LineTo( hDC, 155, 134 );
		// 50º/230º
		MoveToEx( hDC, 82, 61, NULL );
		LineTo( hDC, 162, 127 );
		// 70º/250º
		MoveToEx( hDC, 73, 76, NULL );
		LineTo( hDC, 171, 112 );
		// 80º/260º
		MoveToEx( hDC, 71, 85, NULL );
		LineTo( hDC, 173, 103 );
		// 100º/280º
		MoveToEx( hDC, 71, 103, NULL );
		LineTo( hDC, 173, 85 );
		// 110º/290º
		MoveToEx( hDC, 73, 112, NULL );
		LineTo( hDC, 171, 76 );
		// 130º/310º
		MoveToEx( hDC, 82, 127, NULL );
		LineTo( hDC, 162, 61 );
		// 140º/320º
		MoveToEx( hDC, 89, 134, NULL );
		LineTo( hDC, 155, 54 );
		// 160º/340º
		MoveToEx( hDC, 104, 143, NULL );
		LineTo( hDC, 140, 45 );
		// 170º/350º
		MoveToEx( hDC, 113, 145, NULL );
		LineTo( hDC, 131, 43 );
		//SelectObject( hDC, GrayDarkPen );// TODO thick pen
		// 30º/210º
		MoveToEx( hDC, 96, 49, NULL );
		LineTo( hDC, 148, 139 );
		// 60º/240º
		MoveToEx( hDC, 77, 68, NULL );
		LineTo( hDC, 167, 120 );
		// 120º/300º
		MoveToEx( hDC, 77, 120, NULL );
		LineTo( hDC, 167, 68 );
		// 150º/330º
		MoveToEx( hDC, 96, 139, NULL );
		LineTo( hDC, 148, 49 );
		// cover center for ADI SRCPAINT
		SelectObject( hDC, BlackBrush );
		SelectObject( hDC, BlackPen );
		::Ellipse( hDC, 73, 45, 171, 143 );

		SelectObject( hDC, TahomaFont_h7w3 );
		TextOut( hDC, 148, 47, "33", 2 );
		TextOut( hDC, 163, 63, "30", 2 );
		TextOut( hDC, 163, 118, "24", 2 );
		TextOut( hDC, 148, 134, "21", 2 );
		TextOut( hDC, 89, 134, "15", 2 );
		TextOut( hDC, 74, 118, "12", 2 );
		TextOut( hDC, 74, 63, "06", 2 );
		TextOut( hDC, 89, 47, "03", 2 );
		SelectObject( hDC, TahomaFont_h10w4 );
		SelectObject( hDC, GrayLightPen );
		SelectObject( hDC, GrayLightBrush );
		POINT diamond[4];
		diamond[0].x = 122;
		diamond[0].y = 40;
		diamond[1].x = 124;
		diamond[1].y = 42;
		diamond[2].x = 122;
		diamond[2].y = 44;
		diamond[3].x = 120;
		diamond[3].y = 42;
		Polygon( hDC, diamond, 4 );// top
		//diamond[0].x = 122;
		diamond[0].y = 143;
		//diamond[1].x = 124;
		diamond[1].y = 145;
		//diamond[2].x = 122;
		diamond[2].y = 147;
		//diamond[3].x = 120;
		diamond[3].y = 145;
		Polygon( hDC, diamond, 4 );// bottom
		diamond[0].x = 173;
		diamond[0].y = 92;
		diamond[1].x = 175;
		diamond[1].y = 94;
		diamond[2].x = 173;
		diamond[2].y = 96;
		diamond[3].x = 171;
		diamond[3].y = 94;
		Polygon( hDC, diamond, 4 );// right
		diamond[0].x = 70;
		//diamond[0].y = 92;
		diamond[1].x = 72;
		//diamond[1].y = 94;
		diamond[2].x = 70;
		//diamond[2].y = 96;
		diamond[3].x = 68;
		//diamond[3].y = 94;
		Polygon( hDC, diamond, 4 );// left

		// att needles
		// top scale
		SelectObject( hDC, MagentaPen );
		SelectObject( hDC, MagentaBrush );
		Arc( hDC, 67, 39, 177, 149, 147, 45, 97, 45 );
		MoveToEx( hDC, 97, 45, NULL );
		LineTo( hDC, 97, 48 );
		MoveToEx( hDC, 102, 43, NULL );
		LineTo( hDC, 102, 46 );
		MoveToEx( hDC, 107, 41, NULL );
		LineTo( hDC, 107, 44 );
		MoveToEx( hDC, 112, 40, NULL );
		LineTo( hDC, 112, 43 );
		MoveToEx( hDC, 117, 39, NULL );
		LineTo( hDC, 117, 42 );
		MoveToEx( hDC, 127, 39, NULL );
		LineTo( hDC, 127, 42 );
		MoveToEx( hDC, 132, 40, NULL );
		LineTo( hDC, 132, 43 );
		MoveToEx( hDC, 137, 41, NULL );
		LineTo( hDC, 137, 44 );
		MoveToEx( hDC, 142, 43, NULL );
		LineTo( hDC, 142, 46 );
		MoveToEx( hDC, 147, 45, NULL );
		LineTo( hDC, 147, 48 );

		// side scale
		Arc( hDC, 67, 39, 177, 149, 171, 119, 171, 69 );
		MoveToEx( hDC, 171, 69, NULL );
		LineTo( hDC, 168, 69 );
		MoveToEx( hDC, 172, 74, NULL );
		LineTo( hDC, 169, 74 );
		MoveToEx( hDC, 174, 79, NULL );
		LineTo( hDC, 171, 79 );
		MoveToEx( hDC, 175, 84, NULL );
		LineTo( hDC, 172, 84 );
		MoveToEx( hDC, 176, 89, NULL );
		LineTo( hDC, 173, 89 );
		MoveToEx( hDC, 176, 99, NULL );
		LineTo( hDC, 173, 99 );
		MoveToEx( hDC, 175, 104, NULL );
		LineTo( hDC, 172, 104 );
		MoveToEx( hDC, 174, 109, NULL );
		LineTo( hDC, 171, 109 );
		MoveToEx( hDC, 172, 114, NULL );
		LineTo( hDC, 169, 114 );
		MoveToEx( hDC, 171, 119, NULL );
		LineTo( hDC, 168, 119 );

		// bottom scale
		Arc( hDC, 67, 39, 177, 149, 97, 143, 147, 143 );
		MoveToEx( hDC, 97, 143, NULL );
		LineTo( hDC, 97, 140 );
		MoveToEx( hDC, 102, 144, NULL );
		LineTo( hDC, 102, 141 );
		MoveToEx( hDC, 107, 146, NULL );
		LineTo( hDC, 107, 143 );
		MoveToEx( hDC, 112, 147, NULL );
		LineTo( hDC, 112, 144 );
		MoveToEx( hDC, 117, 148, NULL );
		LineTo( hDC, 117, 145 );
		MoveToEx( hDC, 127, 148, NULL );
		LineTo( hDC, 127, 145 );
		MoveToEx( hDC, 132, 147, NULL );
		LineTo( hDC, 132, 144 );
		MoveToEx( hDC, 137, 146, NULL );
		LineTo( hDC, 137, 143 );
		MoveToEx( hDC, 142, 144, NULL );
		LineTo( hDC, 142, 141 );
		MoveToEx( hDC, 147, 143, NULL );
		LineTo( hDC, 147, 140 );
		return;
	}

	void MDU::ADI_STATIC_ORBIT( HDC hDC )
	{
		SetTextColor( hDC, CR_GRAY_DARK );
		TextOut( hDC, 186, 42, "R", 1 );
		TextOut( hDC, 186, 50, "P", 1 );
		TextOut( hDC, 186, 58, "Y", 1 );

		SelectObject( hDC, WhitePen );
		::Ellipse( hDC, 64, 64, 192, 192 );

		// rate scales
		// top scale
		SelectObject( hDC, GrayDarkPen );
		MoveToEx( hDC, 89, 53, NULL );
		LineTo( hDC, 167, 53 );
		MoveToEx( hDC, 89, 53, NULL );
		LineTo( hDC, 89, 61 );
		MoveToEx( hDC, 97, 53, NULL );
		LineTo( hDC, 97, 58 );
		MoveToEx( hDC, 104, 53, NULL );
		LineTo( hDC, 104, 58 );
		MoveToEx( hDC, 112, 53, NULL );
		LineTo( hDC, 112, 58 );
		MoveToEx( hDC, 120, 53, NULL );
		LineTo( hDC, 120, 58 );
		MoveToEx( hDC, 128, 53, NULL );
		LineTo( hDC, 128, 61 );
		MoveToEx( hDC, 136, 53, NULL );
		LineTo( hDC, 136, 58 );
		MoveToEx( hDC, 144, 53, NULL );
		LineTo( hDC, 144, 58 );
		MoveToEx( hDC, 152, 53, NULL );
		LineTo( hDC, 152, 58 );
		MoveToEx( hDC, 159, 53, NULL );
		LineTo( hDC, 159, 58 );
		MoveToEx( hDC, 167, 53, NULL );
		LineTo( hDC, 167, 61 );

		// side scale
		MoveToEx( hDC, 204, 89, NULL );
		LineTo( hDC, 204, 167 );
		MoveToEx( hDC, 204, 89, NULL );
		LineTo( hDC, 196, 89 );
		MoveToEx( hDC, 204, 97, NULL );
		LineTo( hDC, 199, 97 );
		MoveToEx( hDC, 204, 104, NULL );
		LineTo( hDC, 199, 104 );
		MoveToEx( hDC, 204, 112, NULL );
		LineTo( hDC, 199, 112 );
		MoveToEx( hDC, 204, 120, NULL );
		LineTo( hDC, 199, 120 );
		MoveToEx( hDC, 204, 128, NULL );
		LineTo( hDC, 196, 128 );
		MoveToEx( hDC, 204, 136, NULL );
		LineTo( hDC, 199, 136 );
		MoveToEx( hDC, 204, 144, NULL );
		LineTo( hDC, 199, 144 );
		MoveToEx( hDC, 204, 152, NULL );
		LineTo( hDC, 199, 152 );
		MoveToEx( hDC, 204, 159, NULL );
		LineTo( hDC, 199, 159 );
		MoveToEx( hDC, 204, 167, NULL );
		LineTo( hDC, 196, 167 );////////

		// bottom scale
		MoveToEx( hDC, 89, 205, NULL );
		LineTo( hDC, 167, 205 );
		MoveToEx( hDC, 89, 205, NULL );
		LineTo( hDC, 89, 198 );
		MoveToEx( hDC, 97, 205, NULL );
		LineTo( hDC, 97, 200 );
		MoveToEx( hDC, 104, 205, NULL );
		LineTo( hDC, 104, 200 );
		MoveToEx( hDC, 112, 205, NULL );
		LineTo( hDC, 112, 200 );
		MoveToEx( hDC, 120, 205, NULL );
		LineTo( hDC, 120, 200 );
		MoveToEx( hDC, 128, 205, NULL );
		LineTo( hDC, 128, 198 );
		MoveToEx( hDC, 136, 205, NULL );
		LineTo( hDC, 136, 200 );
		MoveToEx( hDC, 144, 205, NULL );
		LineTo( hDC, 144, 200 );
		MoveToEx( hDC, 152, 205, NULL );
		LineTo( hDC, 152, 200 );
		MoveToEx( hDC, 159, 205, NULL );
		LineTo( hDC, 159, 200 );
		MoveToEx( hDC, 167, 205, NULL );
		LineTo( hDC, 167, 198 );

		// roll lines
		// 10º/190º
		MoveToEx( hDC, 118, 71, NULL );
		LineTo( hDC, 138, 185 );
		// 20º/200º
		MoveToEx( hDC, 108, 73, NULL );
		LineTo( hDC, 148, 183 );
		// 40º/220º
		MoveToEx( hDC, 91, 83, NULL );
		LineTo( hDC, 165, 173 );
		// 50º/230º
		MoveToEx( hDC, 83, 91, NULL );
		LineTo( hDC, 173, 165 );
		// 70º/250º
		MoveToEx( hDC, 73, 108, NULL );
		LineTo( hDC, 183, 148 );
		// 80º/260º
		MoveToEx( hDC, 71, 118, NULL );
		LineTo( hDC, 185, 138 );
		// 100º/280º
		MoveToEx( hDC, 71, 138, NULL );
		LineTo( hDC, 185, 118 );
		// 110º/290º
		MoveToEx( hDC, 73, 148, NULL );
		LineTo( hDC, 183, 108 );
		// 130º/310º
		MoveToEx( hDC, 83, 165, NULL );
		LineTo( hDC, 173, 91 );
		// 140º/320º
		MoveToEx( hDC, 91, 173, NULL );
		LineTo( hDC, 165, 83 );
		// 160º/340º
		MoveToEx( hDC, 108, 183, NULL );
		LineTo( hDC, 148, 73 );
		// 170º/350º
		MoveToEx( hDC, 118, 185, NULL );
		LineTo( hDC, 138, 71 );
		//SelectObject( hDC, GrayDarkPen );// TODO thick pen
		// 30º/210º
		MoveToEx( hDC, 99, 77, NULL );
		LineTo( hDC, 157, 179 );
		// 60º/240º
		MoveToEx( hDC, 77, 99, NULL );
		LineTo( hDC, 179, 157 );
		// 120º/300º
		MoveToEx( hDC, 77, 157, NULL );
		LineTo( hDC, 179, 99 );
		// 150º/330º
		MoveToEx( hDC, 99, 179, NULL );
		LineTo( hDC, 157, 77 );
		// cover center for ADI SRCPAINT
		SelectObject( hDC, BlackBrush );
		SelectObject( hDC, BlackPen );
		::Ellipse( hDC, 73, 73, 183, 183 );

		TextOut( hDC, 157, 75, "33", 2 );
		TextOut( hDC, 174, 93, "30", 2 );
		TextOut( hDC, 174, 155, "24", 2 );
		TextOut( hDC, 157, 173, "21", 2 );
		TextOut( hDC, 91, 173, "15", 2 );
		TextOut( hDC, 74, 155, "12", 2 );
		TextOut( hDC, 74, 93, "06", 2 );
		TextOut( hDC, 91, 75, "03", 2 );
		SelectObject( hDC, GrayLightPen );
		SelectObject( hDC, GrayLightBrush );
		POINT diamond[4];
		diamond[0].x = 128;
		diamond[0].y = 66;
		diamond[1].x = 131;
		diamond[1].y = 69;
		diamond[2].x = 128;
		diamond[2].y = 72;
		diamond[3].x = 125;
		diamond[3].y = 69;
		Polygon( hDC, diamond, 4 );// top
		//diamond[0].x = 128;
		diamond[0].y = 183;
		//diamond[1].x = 131;
		diamond[1].y = 186;
		//diamond[2].x = 128;
		diamond[2].y = 189;
		//diamond[3].x = 125;
		diamond[3].y = 186;
		Polygon( hDC, diamond, 4 );// bottom
		diamond[0].x = 186;
		diamond[0].y = 125;
		diamond[1].x = 189;
		diamond[1].y = 128;
		diamond[2].x = 186;
		diamond[2].y = 131;
		diamond[3].x = 183;
		diamond[3].y = 128;
		Polygon( hDC, diamond, 4 );// right
		diamond[0].x = 69;
		//diamond[0].y = 125;
		diamond[1].x = 72;
		//diamond[1].y = 128;
		diamond[2].x = 69;
		//diamond[2].y = 131;
		diamond[3].x = 66;
		//diamond[3].y = 128;
		Polygon( hDC, diamond, 4 );// left

		// att needles
		// top scale
		SelectObject( hDC, MagentaPen );
		SelectObject( hDC, MagentaBrush );
		Arc( hDC, 66, 66, 190, 190, 156, 73, 100, 73 );
		MoveToEx( hDC, 100, 73, NULL );
		LineTo( hDC, 100, 76 );
		MoveToEx( hDC, 106, 71, NULL );
		LineTo( hDC, 106, 74 );
		MoveToEx( hDC, 111, 68, NULL );
		LineTo( hDC, 111, 72 );
		MoveToEx( hDC, 117, 67, NULL );
		LineTo( hDC, 117, 71 );
		MoveToEx( hDC, 122, 66, NULL );
		LineTo( hDC, 122, 70 );
		MoveToEx( hDC, 134, 66, NULL );
		LineTo( hDC, 134, 70 );
		MoveToEx( hDC, 139, 67, NULL );
		LineTo( hDC, 139, 71 );
		MoveToEx( hDC, 145, 68, NULL );
		LineTo( hDC, 145, 72 );
		MoveToEx( hDC, 150, 71, NULL );
		LineTo( hDC, 150, 74 );
		MoveToEx( hDC, 156, 73, NULL );
		LineTo( hDC, 156, 76 );

		// side scale
		Arc( hDC, 66, 66, 190, 190, 183, 156, 183, 100 );
		MoveToEx( hDC, 183, 100, NULL );
		LineTo( hDC, 180, 100 );
		MoveToEx( hDC, 184, 106, NULL );
		LineTo( hDC, 181, 106 );
		MoveToEx( hDC, 186, 111, NULL );
		LineTo( hDC, 183, 111 );
		MoveToEx( hDC, 188, 117, NULL );
		LineTo( hDC, 184, 117 );
		MoveToEx( hDC, 189, 122, NULL );
		LineTo( hDC, 185, 122 );
		MoveToEx( hDC, 189, 134, NULL );
		LineTo( hDC, 185, 134 );
		MoveToEx( hDC, 188, 139, NULL );
		LineTo( hDC, 184, 139 );
		MoveToEx( hDC, 186, 145, NULL );
		LineTo( hDC, 183, 145 );
		MoveToEx( hDC, 184, 150, NULL );
		LineTo( hDC, 181, 150 );
		MoveToEx( hDC, 183, 156, NULL );
		LineTo( hDC, 180, 156 );

		// bottom scale
		Arc( hDC, 66, 66, 190, 190, 100, 183, 156, 183 );
		MoveToEx( hDC, 100, 183, NULL );
		LineTo( hDC, 100, 180 );
		MoveToEx( hDC, 106, 184, NULL );
		LineTo( hDC, 106, 181 );
		MoveToEx( hDC, 111, 186, NULL );
		LineTo( hDC, 111, 183 );
		MoveToEx( hDC, 117, 188, NULL );
		LineTo( hDC, 117, 184 );
		MoveToEx( hDC, 122, 189, NULL );
		LineTo( hDC, 122, 185 );
		MoveToEx( hDC, 134, 189, NULL );
		LineTo( hDC, 134, 185 );
		MoveToEx( hDC, 139, 188, NULL );
		LineTo( hDC, 139, 184 );
		MoveToEx( hDC, 145, 186, NULL );
		LineTo( hDC, 145, 183 );
		MoveToEx( hDC, 151, 184, NULL );
		LineTo( hDC, 151, 181 );
		MoveToEx( hDC, 156, 183, NULL );
		LineTo( hDC, 156, 180 );
		return;
	}

	void MDU::ADI( HDC hDC, double pitch, double roll, double yaw )
	{
		// center (122,94) r = 57
		// view r = 49, ball r = 49 * sqrt( 2 )
		// 90º pitch/yaw "FOV"

		// apply roll
		XFORM WTroll;
		WTroll.eM11 = (FLOAT)cos( roll * RAD );
		WTroll.eM12 = (FLOAT)(-sin( roll * RAD ));
		WTroll.eM21 = -WTroll.eM12;
		WTroll.eM22 = WTroll.eM11;
		WTroll.eDx = (FLOAT)(50 * (1 - WTroll.eM11 - WTroll.eM21));
		WTroll.eDy = (FLOAT)(50 * (1 - WTroll.eM11 + WTroll.eM21));
		SetGraphicsMode( hDC_ADI, GM_ADVANCED );
		SetWorldTransform( hDC_ADI, &WTroll );

		if (pitch > 180) pitch -= 360;// TODO get rid of this

		// 0º pitch line/labels
		double sinpitch = sin( pitch * RAD );
		double cospitch = cos( pitch * RAD );
		int hP;
		double hP2;
		SetTextColor( hDC_ADI, CR_BLACK );
		SetBkColor( hDC_ADI, CR_WHITE );
		if (fabs( pitch ) <= 45)
		{
			hP = Round( 69.296 * sinpitch );
			if (pitch < 0)
			{
				SelectObject( hDC_ADI, WhiteBrush );
				SelectObject( hDC_ADI, WhitePen );
				Rectangle( hDC_ADI, 0, 0, 100, 50 );
				SelectObject( hDC_ADI, GrayLightBrush );
				SelectObject( hDC_ADI, GrayLightPen );
				Chord( hDC_ADI, -19, 50 - hP, 119, 50 + hP, 119, 50, -19, 50 );
				Rectangle( hDC_ADI, 0, 50, 100, 100 );
				SelectObject( hDC_ADI, BlackPen );
				Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, 119, 50, -19, 50 );
			}
			else
			{
				SelectObject( hDC_ADI, GrayLightBrush );
				SelectObject( hDC_ADI, GrayLightPen );
				Rectangle( hDC_ADI, 0, 50, 100, 100 );
				SelectObject( hDC_ADI, WhiteBrush );
				SelectObject( hDC_ADI, WhitePen );
				Chord( hDC_ADI, -19, 50 - hP, 119, 50 + hP, -19, 50, 119, 50 );
				Rectangle( hDC_ADI, 0, 0, 100, 50 );
				SelectObject( hDC_ADI, BlackPen );
				Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, -19, 50, 119, 50 );
			}
			hP = 46 + Round( 66.935245 * sinpitch );
			TextOut( hDC_ADI, 31, hP, "0", 1 );
			TextOut( hDC_ADI, 67, hP, "0", 1 );
		}
		else if (pitch > 0)
		{
			SelectObject( hDC_ADI, WhiteBrush );
			Rectangle( hDC_ADI, 0, 0, 100, 100 );
		}
		else
		{
			SelectObject( hDC_ADI, GrayLightBrush );
			Rectangle( hDC_ADI, 0, 0, 100, 100 );
		}

		// pitch lines/labels for +30º/+60º/+90º/+120º/+150º
		SelectObject( hDC_ADI, GrayDarkPen );
		// +30º
		if (fabs( pitch - 30 ) <= 45)
		{
			hP2 = sinpitch * 60.012499 - cospitch * 34.648232;//hP = 69.296 * sin( (pitch - 30) * RAD );
			hP = Round( hP2 );
			if (pitch < 30) Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, 119, 50, -19, 50 );
			else Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, -19, 50, 119, 50 );
			hP = 46 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 31, hP, "3", 1 );
			TextOut( hDC_ADI, 67, hP, "3", 1 );
		}
		// +60º
		if (fabs( pitch - 60 ) <= 45)
		{
			hP2 = sinpitch * 34.648232 - cospitch * 60.012499;
			hP = Round( hP2 );
			if (pitch < 60) Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, 119, 50, -19, 50 );
			else Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, -19, 50, 119, 50 );
			hP = 46 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 31, hP, "6", 1 );
			TextOut( hDC_ADI, 67, hP, "6", 1 );
		}
		// +90º
		if (fabs( pitch - 90 ) <= 45)
		{
			hP2 = 69.296465 * (-cospitch);
			hP = Round( hP2 );
			if (pitch < 90) Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, 119, 50, -19, 50 );
			else Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, -19, 50, 119, 50 );
			hP = 46 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 31, hP, "9", 1 );
			TextOut( hDC_ADI, 67, hP, "9", 1 );
		}
		// +120º
		if (fabs( pitch - 120 ) <= 45)
		{
			hP2 = (-sinpitch) * 34.648232 - cospitch * 60.012499;
			hP = Round( hP2 );
			if (pitch < 120) Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, 119, 50, -19, 50 );
			else Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, -19, 50, 119, 50 );
			hP = 46 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 30, hP, "12", 2 );
			TextOut( hDC_ADI, 66, hP, "12", 2 );
		}
		// +150º
		if (fabs( pitch - 150 ) <= 45)
		{
			hP2 = (-sinpitch) * 60.012499 - cospitch * 34.648232;
			hP = Round( hP2 );
			if (pitch < 150) Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, 119, 50, -19, 50 );
			else Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, -19, 50, 119, 50 );
			hP = 46 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 30, hP, "15", 2 );
			TextOut( hDC_ADI, 66, hP, "15", 2 );
		}

		// pitch lines/labels for -30º/-60º/-90º/-120º/-150º
		SelectObject( hDC_ADI, WhitePen );
		SetTextColor( hDC_ADI, CR_WHITE );
		SetBkColor( hDC_ADI, CR_GRAY_LIGHT );
		// -30º
		if (fabs( pitch + 30 ) <= 45)
		{
			hP2 = sinpitch * 60.012499 + cospitch * 34.648232;
			hP = Round( hP2 );
			if (pitch < -30) Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, 119, 50, -19, 50 );
			else Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, -19, 50, 119, 50 );
			hP = 46 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 30, hP, "33", 2 );
			TextOut( hDC_ADI, 66, hP, "33", 2 );
		}
		// -60º
		if (fabs( pitch + 60 ) <= 45)
		{
			hP2 = sinpitch * 34.648232 + cospitch * 60.012499;
			hP = Round( hP2 );
			if (pitch < -60) Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, 119, 50, -19, 50 );
			else Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, -19, 50, 119, 50 );
			hP = 46 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 30, hP, "30", 2 );
			TextOut( hDC_ADI, 66, hP, "30", 2 );
		}
		// -90º
		if (fabs( pitch + 90 ) <= 45)
		{
			hP2 = 69.296 * cospitch;
			hP = Round( hP2 );
			if (pitch < -90) Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, 119, 50, -19, 50 );
			else Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, -19, 50, 119, 50 );
			hP = 46 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 30, hP, "27", 2 );
			TextOut( hDC_ADI, 66, hP, "27", 2 );
		}
		// -120º
		if (fabs( pitch + 120 ) <= 45)
		{
			hP2 = (-sinpitch) * 34.648232 + cospitch * 60.012499;
			hP = Round( hP2 );
			if (pitch < -120) Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, 119, 50, -19, 50 );
			else Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, -19, 50, 119, 50 );
			hP = 46 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 30, hP, "24", 2 );
			TextOut( hDC_ADI, 66, hP, "24", 2 );
		}
		// -150º
		if (fabs( pitch + 150 ) <= 45)
		{
			hP2 = (-sinpitch) * 60.012499 + cospitch * 34.648232;
			hP = Round( hP2 );
			if (pitch < -150) Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, 119, 50, -19, 50 );
			else Arc( hDC_ADI, -19, 50 - hP, 119, 50 + hP, -19, 50, 119, 50 );
			hP = 46 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI, 30, hP, "21", 2 );
			TextOut( hDC_ADI, 66, hP, "21", 2 );
		}

		// TODO yaw
		// HACK
		// "central plane"
		SelectObject( hDC_ADI, WhiteBrush );
		SelectObject( hDC_ADI, BlackPen );
		Rectangle( hDC_ADI, 48, 0, 52, 100 );
		SelectObject( hDC_ADI, GrayDarkPen );
		MoveToEx( hDC_ADI, 50, 0, NULL );
		LineTo( hDC_ADI, 50, 100 );

		SelectObject( hDC_ADI, GrayDarkPen );
		// yaw line 30º (above horizon)
		MoveToEx( hDC_ADI, 85, 0, NULL );
		LineTo( hDC_ADI, 85, 50 + Round( 60.012096 * sinpitch ) );
		// yaw line 330º (above horizon)
		MoveToEx( hDC_ADI, 15, 0, NULL );
		LineTo( hDC_ADI, 15, 50 + Round( 60.012096 * sinpitch ) );
		SelectObject( hDC_ADI, WhitePen );
		// yaw line 30º (below horizon)
		MoveToEx( hDC_ADI, 85, 100, NULL );
		LineTo( hDC_ADI, 85, 50 + Round( 60.012096 * sinpitch ) );
		// yaw line 330º (below horizon)
		MoveToEx( hDC_ADI, 15, 100, NULL );
		LineTo( hDC_ADI, 15, 50 + Round( 60.012096 * sinpitch ) );

		// TODO yaw labels

		// roll triangle
		SelectObject( hDC_ADI, GreenPen );
		SelectObject( hDC_ADI, GreenBrush );
		POINT tri[3];
		tri[0].x = 50;
		tri[0].y = 1;
		tri[1].x = 46;
		tri[1].y = 9;
		tri[2].x = 54;
		tri[2].y = 9;
		Polygon( hDC_ADI, tri, 3 );
		
		// clean up
		ModifyWorldTransform( hDC_ADI, &WTroll, MWT_IDENTITY );
		SetGraphicsMode( hDC_ADI, GM_COMPATIBLE );

		// flight director
		SelectObject( hDC_ADI, GreenThickPen );
		Arc( hDC_ADI, 44, 44, 56, 57, 44, 50, 56, 50 );
		SelectObject( hDC_ADI, GreenPen );
		SelectObject( hDC_ADI, GreenBrush );
		Rectangle( hDC_ADI, 49, 33, 51, 67 );
		Rectangle( hDC_ADI, 33, 49, 67, 51 );

		// digital RPY
		if (pitch < 0) pitch += 360;// TODO get rid of this
		SetTextColor( hDC, CR_WHITE );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%03.0f", roll );
		TextOut( hDC, 180, 25, cbuf, strlen( cbuf ) );
		sprintf_s( cbuf, 8, "%03.0f", pitch );
		TextOut( hDC, 180, 32, cbuf, strlen( cbuf ) );
		sprintf_s( cbuf, 8, "%03.0f", yaw );
		TextOut( hDC, 180, 39, cbuf, strlen( cbuf ) );

		BitBlt( hDC_ADI, 0, 0, 100, 100, hDC_ADIMASK, 0, 0, SRCAND );
		BitBlt( hDC, 72, 44, 100, 100, hDC_ADI, 0, 0, SRCPAINT );
		return;
	}

	void MDU::ADI_ORBIT( HDC hDC, double pitch, double roll, double yaw )
	{
		// center (128,128) r = 64
		// view r = 55, ball r = 55 * sqrt( 2 )
		// 90º pitch/yaw "FOV"

		// apply roll
		XFORM WTroll;
		WTroll.eM11 = (FLOAT)cos( roll * RAD );
		WTroll.eM12 = (FLOAT)(-sin( roll * RAD ));
		WTroll.eM21 = -WTroll.eM12;
		WTroll.eM22 = WTroll.eM11;
		WTroll.eDx = (FLOAT)(56 * (1 - WTroll.eM11 - WTroll.eM21));
		WTroll.eDy = (FLOAT)(56 * (1 - WTroll.eM11 + WTroll.eM21));
		SetGraphicsMode( hDC_ADI_ORBIT, GM_ADVANCED );
		SetWorldTransform( hDC_ADI_ORBIT, &WTroll );

		if (pitch > 180) pitch -= 360;// TODO get rid of this

		// 0º pitch line/labels
		double sinpitch = sin( pitch * RAD );
		double cospitch = cos( pitch * RAD );
		int hP;
		double hP2;
		SetTextColor( hDC_ADI_ORBIT, CR_BLACK );
		SetBkColor( hDC_ADI_ORBIT, CR_WHITE );
		if (fabs( pitch ) <= 45)
		{
			hP = Round( 77.7817 * sinpitch );
			if (pitch < 0)
			{
				SelectObject( hDC_ADI_ORBIT, WhiteBrush );
				SelectObject( hDC_ADI_ORBIT, WhitePen );
				Rectangle( hDC_ADI_ORBIT, 0, 0, 112, 56 );
				SelectObject( hDC_ADI_ORBIT, GrayLightBrush );
				SelectObject( hDC_ADI_ORBIT, GrayLightPen );
				Chord( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, 134, 56, -22, 56 );
				Rectangle( hDC_ADI_ORBIT, 0, 56, 112, 112 );
				SelectObject( hDC_ADI_ORBIT, BlackPen );
				Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, 134, 56, -22, 56 );
			}
			else
			{
				SelectObject( hDC_ADI_ORBIT, GrayLightBrush );
				SelectObject( hDC_ADI_ORBIT, GrayLightPen );
				Rectangle( hDC_ADI_ORBIT, 0, 56, 112, 112 );
				SelectObject( hDC_ADI_ORBIT, WhiteBrush );
				SelectObject( hDC_ADI_ORBIT, WhitePen );
				Chord( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, -22, 56, 134, 56 );
				Rectangle( hDC_ADI_ORBIT, 0, 0, 112, 56 );
				SelectObject( hDC_ADI_ORBIT, BlackPen );
				Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, -22, 56, 134, 56 );
			}
			hP = 51 + Round( 75.131397 * sinpitch );
			TextOut( hDC_ADI_ORBIT, 35, hP, "0", 1 );
			TextOut( hDC_ADI_ORBIT, 75, hP, "0", 1 );
		}
		else if (pitch > 0)
		{
			SelectObject( hDC_ADI_ORBIT, WhiteBrush );
			Rectangle( hDC_ADI_ORBIT, 0, 0, 112, 112 );
		}
		else
		{
			SelectObject( hDC_ADI_ORBIT, GrayLightBrush );
			Rectangle( hDC_ADI_ORBIT, 0, 0, 112, 112 );
		}

		// pitch lines/labels for +30º/+60º/+90º/+120º/+150º
		SelectObject( hDC_ADI_ORBIT, GrayDarkPen );
		// +30º
		if (fabs( pitch - 30 ) <= 45)
		{
			hP2 = sinpitch * 67.360968 - cospitch * 38.890873;//hP = 69.296 * sin( (pitch - 30) * RAD );
			hP = Round( hP2 );
			if (pitch < 30) Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, 134, 56, -22, 56 );
			else Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, -22, 56, 134, 56 );
			hP = 51 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI_ORBIT, 35, hP, "3", 1 );
			TextOut( hDC_ADI_ORBIT, 75, hP, "3", 1 );
		}
		// +60º
		if (fabs( pitch - 60 ) <= 45)
		{
			hP2 = sinpitch * 38.890873 - cospitch * 67.360968;
			hP = Round( hP2 );
			if (pitch < 60) Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, 134, 56, -22, 56 );
			else Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, -22, 56, 134, 56 );
			hP = 51 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI_ORBIT, 35, hP, "6", 1 );
			TextOut( hDC_ADI_ORBIT, 75, hP, "6", 1 );
		}
		// +90º
		if (fabs( pitch - 90 ) <= 45)
		{
			hP2 = 77.7817 * (-cospitch);
			hP = Round( hP2 );
			if (pitch < 90) Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, 134, 56, -22, 56 );
			else Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, -22, 56, 134, 56 );
			hP = 51 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI_ORBIT, 35, hP, "9", 1 );
			TextOut( hDC_ADI_ORBIT, 75, hP, "9", 1 );
		}
		// +120º
		if (fabs( pitch - 120 ) <= 45)
		{
			hP2 = (-sinpitch) * 38.890873 - cospitch * 67.360968;
			hP = Round( hP2 );
			if (pitch < 120) Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, 134, 56, -22, 56 );
			else Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, -22, 56, 134, 56 );
			hP = 51 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI_ORBIT, 33, hP, "12", 2 );
			TextOut( hDC_ADI_ORBIT, 73, hP, "12", 2 );
		}
		// +150º
		if (fabs( pitch - 150 ) <= 45)
		{
			hP2 = (-sinpitch) * 67.360968 - cospitch * 38.890873;
			hP = Round( hP2 );
			if (pitch < 150) Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, 134, 56, -22, 56 );
			else Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, -22, 56, 134, 56 );
			hP = 51 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI_ORBIT, 33, hP, "15", 2 );
			TextOut( hDC_ADI_ORBIT, 73, hP, "15", 2 );
		}

		// pitch lines/labels for -30º/-60º/-90º/-120º/-150º
		SelectObject( hDC_ADI_ORBIT, WhitePen );
		SetTextColor( hDC_ADI_ORBIT, CR_WHITE );
		SetBkColor( hDC_ADI_ORBIT, CR_GRAY_LIGHT );
		// -30º
		if (fabs( pitch + 30 ) <= 45)
		{
			hP2 = sinpitch * 67.360968 + cospitch * 38.890873;
			hP = Round( hP2 );
			if (pitch < -30) Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, 134, 56, -22, 56 );
			else Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, -22, 56, 134, 56 );
			hP = 51 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI_ORBIT, 33, hP, "33", 2 );
			TextOut( hDC_ADI_ORBIT, 73, hP, "33", 2 );
		}
		// -60º
		if (fabs( pitch + 60 ) <= 45)
		{
			hP2 = sinpitch * 38.890873 + cospitch * 67.360968;
			hP = Round( hP2 );
			if (pitch < -60) Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, 134, 56, -22, 56 );
			else Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, -22, 56, 134, 56 );
			hP = 51 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI_ORBIT, 33, hP, "30", 2 );
			TextOut( hDC_ADI_ORBIT, 73, hP, "30", 2 );
		}
		// -90º
		if (fabs( pitch + 90 ) <= 45)
		{
			hP2 = 77.7817 * cospitch;
			hP = Round( hP2 );
			if (pitch < -90) Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, 134, 56, -22, 56 );
			else Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, -22, 56, 134, 56 );
			hP = 51 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI_ORBIT, 33, hP, "27", 2 );
			TextOut( hDC_ADI_ORBIT, 73, hP, "27", 2 );
		}
		// -120º
		if (fabs( pitch + 120 ) <= 45)
		{
			hP2 = (-sinpitch) * 38.890873 + cospitch * 67.360968;
			hP = Round( hP2 );
			if (pitch < -120) Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, 134, 56, -22, 56 );
			else Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, -22, 56, 134, 56 );
			hP = 51 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI_ORBIT, 33, hP, "24", 2 );
			TextOut( hDC_ADI_ORBIT, 73, hP, "24", 2 );
		}
		// -150º
		if (fabs( pitch + 150 ) <= 45)
		{
			hP2 = (-sinpitch) * 67.360968 + cospitch * 38.890873;
			hP = Round( hP2 );
			if (pitch < -150) Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, 134, 56, -22, 56 );
			else Arc( hDC_ADI_ORBIT, -22, 56 - hP, 134, 56 + hP, -22, 56, 134, 56 );
			hP = 51 + Round( hP2 * 0.965926 );
			TextOut( hDC_ADI_ORBIT, 33, hP, "21", 2 );
			TextOut( hDC_ADI_ORBIT, 73, hP, "21", 2 );
		}

		// TODO yaw
		// HACK
		// "central plane"
		SelectObject( hDC_ADI_ORBIT, WhiteBrush );
		SelectObject( hDC_ADI_ORBIT, BlackPen );
		Rectangle( hDC_ADI_ORBIT, 54, 0, 58, 112 );
		SelectObject( hDC_ADI_ORBIT, GrayDarkPen );
		MoveToEx( hDC_ADI_ORBIT, 56, 0, NULL );
		LineTo( hDC_ADI_ORBIT, 56, 112 );

		SelectObject( hDC_ADI_ORBIT, GrayDarkPen );
		// yaw line 30º (above horizon)
		MoveToEx( hDC_ADI_ORBIT, 95, 0, NULL );
		LineTo( hDC_ADI_ORBIT, 95, 56 + Round( 67.360968 * sinpitch ) );
		// yaw line 330º (above horizon)
		MoveToEx( hDC_ADI_ORBIT, 17, 0, NULL );
		LineTo( hDC_ADI_ORBIT, 17, 56 + Round( 67.360968 * sinpitch ) );
		SelectObject( hDC_ADI_ORBIT, WhitePen );
		// yaw line 30º (below horizon)
		MoveToEx( hDC_ADI_ORBIT, 95, 112, NULL );
		LineTo( hDC_ADI_ORBIT, 95, 56 + Round( 67.360968 * sinpitch ) );
		// yaw line 330º (below horizon)
		MoveToEx( hDC_ADI_ORBIT, 17, 112, NULL );
		LineTo( hDC_ADI_ORBIT, 17, 56 + Round( 67.360968 * sinpitch ) );

		// TODO yaw labels

		// roll triangle
		SelectObject( hDC_ADI_ORBIT, GreenPen );
		SelectObject( hDC_ADI_ORBIT, GreenBrush );
		POINT tri[3];
		tri[0].x = 56;
		tri[0].y = 1;
		tri[1].x = 52;
		tri[1].y = 9;
		tri[2].x = 60;
		tri[2].y = 9;
		Polygon( hDC_ADI_ORBIT, tri, 3 );
		
		// clean up
		ModifyWorldTransform( hDC_ADI_ORBIT, &WTroll, MWT_IDENTITY );
		SetGraphicsMode( hDC_ADI_ORBIT, GM_COMPATIBLE );

		// flight director
		SelectObject( hDC_ADI_ORBIT, GreenThickPen );
		Arc( hDC_ADI_ORBIT, 49, 49, 63, 64, 49, 56, 63, 56 );
		SelectObject( hDC_ADI_ORBIT, GreenPen );
		SelectObject( hDC_ADI_ORBIT, GreenBrush );
		Rectangle( hDC_ADI_ORBIT, 55, 37, 57, 75 );
		Rectangle( hDC_ADI_ORBIT, 37, 55, 75, 57 );

		// digital RPY
		if (pitch < 0) pitch += 360;// TODO get rid of this
		SetTextColor( hDC, CR_WHITE );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%03.0f", roll );
		TextOut( hDC, 193, 42, cbuf, strlen( cbuf ) );
		sprintf_s( cbuf, 8, "%03.0f", pitch );
		TextOut( hDC, 193, 50, cbuf, strlen( cbuf ) );
		sprintf_s( cbuf, 8, "%03.0f", yaw );
		TextOut( hDC, 193, 58, cbuf, strlen( cbuf ) );

		BitBlt( hDC_ADI_ORBIT, 0, 0, 112, 112, hDC_ADIMASK_ORBIT, 0, 0, SRCAND );
		BitBlt( hDC, 72, 72, 112, 112, hDC_ADI_ORBIT, 0, 0, SRCPAINT );
		return;
	}

	void MDU::ADI_RATE_A( HDC hDC, double pitch, double roll, double yaw, int adirate )
	{
		int pos_roll;
		int pos_pitch;
		int pos_yaw;

		SelectObject( hDC, TahomaFont_h7w3 );
		SetTextColor( hDC, CR_GRAY_DARK );
		TextOut( hDC, 121, 19, "0", 1 );
		TextOut( hDC, 192, 91, "0", 1 );
		TextOut( hDC, 121, 164, "0", 1 );
		SelectObject( hDC, TahomaFont_h10w4 );
		SetTextColor( hDC, CR_WHITE );
		if (adirate == 1)
		{
			// ADI RATE MED
			TextOut( hDC, 81, 23, "5", 1 );
			TextOut( hDC, 159, 23, "5", 1 );

			TextOut( hDC, 187, 50, "5", 1 );
			TextOut( hDC, 187, 129, "5", 1 );

			TextOut( hDC, 81, 159, "5", 1 );
			TextOut( hDC, 159, 159, "5", 1 );

			if (roll > 5) pos_roll = 157;
			else if (roll < -5) pos_roll = 87;
			else pos_roll = 122 + Round( roll * 7 );

			if (pitch > 5) pos_pitch = 129;
			else if (pitch < -5) pos_pitch = 59;
			else pos_pitch = 94 + Round( pitch * 7 );

			if (yaw > 5) pos_yaw = 157;
			else if (yaw < -5) pos_yaw = 87;
			else pos_yaw = 122 + Round( yaw * 7 );
		}
		else if (adirate == 2)
		{
			// ADI RATE HIGH
			TextOut( hDC, 78, 23, "10", 2 );
			TextOut( hDC, 158, 23, "10", 2 );

			TextOut( hDC, 187, 50, "10", 2 );
			TextOut( hDC, 187, 129, "10", 2 );

			TextOut( hDC, 78, 159, "10", 2 );
			TextOut( hDC, 158, 159, "10", 2 );

			if (roll > 10) pos_roll = 157;
			else if (roll < -10) pos_roll = 87;
			else pos_roll = 122 + Round( roll * 3.5 );

			if (pitch > 10) pos_pitch = 129;
			else if (pitch < -10) pos_pitch = 59;
			else pos_pitch = 94 + Round( pitch * 3.5 );

			if (yaw > 10) pos_yaw = 157;
			else if (yaw < -10) pos_yaw = 87;
			else pos_yaw = 122 + Round( yaw * 3.5 );
		}
		else
		{
			// ADI RATE LOW
			TextOut( hDC, 81, 23, "1", 1 );
			TextOut( hDC, 159, 23, "1", 1 );

			TextOut( hDC, 187, 50, "1", 1 );
			TextOut( hDC, 187, 129, "1", 1 );

			TextOut( hDC, 81, 159, "1", 1 );
			TextOut( hDC, 159, 159, "1", 1 );

			if (roll > 1) pos_roll = 157;
			else if (roll < -1) pos_roll = 87;
			else pos_roll = 122 + Round( roll * 35 );

			if (pitch > 1) pos_pitch = 129;
			else if (pitch < -1) pos_pitch = 59;
			else pos_pitch = 94 + Round( pitch * 35 );

			if (yaw > 1) pos_yaw = 157;
			else if (yaw < -1) pos_yaw = 87;
			else pos_yaw = 122 + Round( yaw * 35 );
		}

		// draw triangles
		SelectObject( hDC, GreenPen );
		SelectObject( hDC, GreenBrush );
		POINT tri[3];
		tri[0].x = pos_roll;
		tri[0].y = 27;
		tri[1].x = tri[0].x + 5;
		tri[1].y = 19;
		tri[2].x = tri[0].x - 5;
		tri[2].y = 19;
		Polygon( hDC, tri, 3 );

		tri[0].x = 190;
		tri[0].y = pos_pitch;
		tri[1].x = 198;
		tri[1].y = tri[0].y - 5;
		tri[2].x = 198;
		tri[2].y = tri[0].y + 5;
		Polygon( hDC, tri, 3 );

		tri[0].x = pos_yaw;
		tri[0].y = 163;
		tri[1].x = tri[0].x - 5;
		tri[1].y = 171;
		tri[2].x = tri[0].x + 5;
		tri[2].y = 171;
		Polygon( hDC, tri, 3 );
		return;
	}

	void MDU::ADI_RATE_B( HDC hDC, double pitch, double roll, double yaw, int adirate, double Altitude_ft )
	{
		int pos_roll;
		int pos_pitch;
		int pos_yaw;

		SetTextColor( hDC, CR_WHITE );
		if (adirate == 1)
		{
			// ADI RATE MED
			if (Altitude_ft < 6400)// HACK should use a 3sec timer starting at 7Kft, which expires about 6.4Kft 
			{
				// <7k + 3s
				TextOut( hDC, 81, 23, "5", 1 );
				TextOut( hDC, 159, 23, "5", 1 );

				TextOut( hDC, 187, 50, "5", 1 );
				TextOut( hDC, 187, 129, "5", 1 );

				TextOut( hDC, 81, 159, "5", 1 );
				TextOut( hDC, 159, 159, "5", 1 );

				SelectObject( hDC, TahomaFont_h7w3 );
				SetTextColor( hDC, CR_GRAY_DARK );
				TextOut( hDC, 121, 19, "0", 1 );
				TextOut( hDC, 192, 91, "0", 1 );
				TextOut( hDC, 121, 164, "0", 1 );
				SelectObject( hDC, TahomaFont_h10w4 );

				if (roll > 5) pos_roll = 157;
				else if (roll < -5) pos_roll = 87;
				else pos_roll = 122 + Round( roll * 7 );

				if (pitch > 5) pos_pitch = 129;
				else if (pitch < -5) pos_pitch = 59;
				else pos_pitch = 94 + Round( pitch * 7 );

				if (yaw > 5) pos_yaw = 157;
				else if (yaw < -5) pos_yaw = 87;
				else pos_yaw = 122 + Round( yaw * 7 );
			}
			else if (Altitude_ft < 7000)
			{
				// <7k
				// blank
				// blank
				// blank
				pos_roll = 0;
				pos_pitch = 0;
				pos_yaw = 0;
			}
			else if (GetIDP()->GetPrefinalState() == true)
			{
				// PRFNL - 7k
				// blank
				// alt err +/-1k
				// y-runway position err +/-1k
				TextOut( hDC, 187, 50, "1K", 2 );
				TextOut( hDC, 187, 129, "1K", 2 );

				TextOut( hDC, 78, 159, "1K", 2 );
				TextOut( hDC, 158, 159, "1K", 2 );

				SelectObject( hDC, TahomaFont_h7w3 );
				SetTextColor( hDC, CR_GRAY_DARK );
				TextOut( hDC, 192, 91, "0", 1 );
				TextOut( hDC, 121, 164, "0", 1 );
				SelectObject( hDC, TahomaFont_h10w4 );

				pos_roll = 0;
				pitch = GetIDP()->GetGlideSlopeDistance();
				yaw = -GetIDP()->GetYRunwayPositionError();

				if (pitch > 1000) pos_pitch = 129;
				else if (pitch < -1000) pos_pitch = 59;
				else pos_pitch = 94 + Round( pitch * 0.035 );

				if (yaw > 1000) pos_yaw = 157;
				else if (yaw < -1000) pos_yaw = 87;
				else pos_yaw = 122 + Round( yaw * 0.035 );
			}
			else if (GetIDP()->GetOnHACState() == true)
			{
				// HAC
				// blank
				// alt err +/-5k
				// HAC x-range err +/-5k
				TextOut( hDC, 187, 50, "5K", 2 );
				TextOut( hDC, 187, 129, "5K", 2 );

				TextOut( hDC, 78, 159, "5K", 2 );
				TextOut( hDC, 158, 159, "5K", 2 );

				SelectObject( hDC, TahomaFont_h7w3 );
				SetTextColor( hDC, CR_GRAY_DARK );
				TextOut( hDC, 192, 91, "0", 1 );
				TextOut( hDC, 121, 164, "0", 1 );
				SelectObject( hDC, TahomaFont_h10w4 );

				pos_roll = 0;
				pitch = 0;// TODO get alt error (ft)
				yaw = GetIDP()->GetHACRadialError();

				if (pitch > 5000) pos_pitch = 129;
				else if (pitch < -5000) pos_pitch = 59;
				else pos_pitch = 94 + Round( pitch * 0.007 );

				if (yaw > 5000) pos_yaw = 157;
				else if (yaw < -5000) pos_yaw = 87;
				else pos_yaw = 122 + Round( yaw * 0.007 );
			}
			else
			{
				// ACQ
				// time to HAC 10s
				// alt err +/-5k
				// heading err +/-5º
				TextOut( hDC, 81, 23, "0", 1 );// TODO what to show here?
				TextOut( hDC, 159, 23, "0", 1 );

				TextOut( hDC, 187, 50, "5K", 2 );
				TextOut( hDC, 187, 129, "5K", 2 );

				TextOut( hDC, 81, 159, "5", 1 );
				TextOut( hDC, 159, 159, "5", 1 );

				SelectObject( hDC, TahomaFont_h7w3 );
				SetTextColor( hDC, CR_GRAY_DARK );
				TextOut( hDC, 192, 91, "0", 1 );
				TextOut( hDC, 121, 164, "0", 1 );
				SelectObject( hDC, TahomaFont_h10w4 );

				roll = ceil( GetIDP()->GetTimeToHAC() );
				pitch = 0;// TODO get alt error (ft)
				yaw = 0;// TODO get heading error (deg)

				if (roll > 0)
				{
					if (roll >= 10) pos_roll = 87;
					else pos_roll = 157 - Round( roll * 7 );
				}
				else if (roll < 0)
				{
					if (roll <= -10) pos_roll = 157;
					else pos_roll = 87 - Round( roll * 7 );
				}
				else pos_roll = 0;

				if (pitch > 5000) pos_pitch = 129;
				else if (pitch < -5000) pos_pitch = 59;
				else pos_pitch = 94 + Round( pitch * 0.007 );

				if (yaw > 5) pos_yaw = 157;
				else if (yaw < -5) pos_yaw = 87;
				else pos_yaw = 122 + Round( yaw * 7 );
			}
		}
		else
		{
			// ADI RATE HIGH/LOW
			TextOut( hDC, 81, 23, "5", 1 );
			TextOut( hDC, 159, 23, "5", 1 );

			TextOut( hDC, 187, 50, "5", 1 );
			TextOut( hDC, 187, 129, "5", 1 );

			TextOut( hDC, 81, 159, "5", 1 );
			TextOut( hDC, 159, 159, "5", 1 );

			SelectObject( hDC, TahomaFont_h7w3 );
			SetTextColor( hDC, CR_GRAY_DARK );
			TextOut( hDC, 121, 19, "0", 1 );
			TextOut( hDC, 192, 91, "0", 1 );
			TextOut( hDC, 121, 164, "0", 1 );
			SelectObject( hDC, TahomaFont_h10w4 );

			if (roll > 5) pos_roll = 157;
			else if (roll < -5) pos_roll = 87;
			else pos_roll = 122 + Round( roll * 7 );

			if (pitch > 5) pos_pitch = 129;
			else if (pitch < -5) pos_pitch = 59;
			else pos_pitch = 94 + Round( pitch * 7 );

			if (yaw > 5) pos_yaw = 157;
			else if (yaw < -5) pos_yaw = 87;
			else pos_yaw = 122 + Round( yaw * 7 );
		}
		
		// draw triangles
		SelectObject( hDC, GreenPen );
		SelectObject( hDC, GreenBrush );
		POINT tri[3];
		if (pos_roll != 0)
		{
			tri[0].x = pos_roll;
			tri[0].y = 27;
			tri[1].x = tri[0].x + 5;
			tri[1].y = 19;
			tri[2].x = tri[0].x - 5;
			tri[2].y = 19;
			Polygon( hDC, tri, 3 );
		}
		if (pos_pitch != 0)
		{
			tri[0].x = 190;
			tri[0].y = pos_pitch;
			tri[1].x = 198;
			tri[1].y = tri[0].y - 5;
			tri[2].x = 198;
			tri[2].y = tri[0].y + 5;
			Polygon( hDC, tri, 3 );
		}
		if (pos_yaw != 0)
		{
			tri[0].x = pos_yaw;
			tri[0].y = 163;
			tri[1].x = tri[0].x - 5;
			tri[1].y = 171;
			tri[2].x = tri[0].x + 5;
			tri[2].y = 171;
			Polygon( hDC, tri, 3 );
		}
		return;
	}

	void MDU::ADI_RATE_ORBIT( HDC hDC, double pitch, double roll, double yaw, int adirate )
	{
		int pos_roll;
		int pos_pitch;
		int pos_yaw;

		SetTextColor( hDC, CR_GRAY_DARK );
		TextOut( hDC, 126, 43, "0", 1 );
		TextOut( hDC, 207, 124, "0", 1 );
		TextOut( hDC, 126, 206, "0", 1 );
		SetTextColor( hDC, CR_WHITE );
		if (adirate == 1)
		{
			// ADI RATE MED
			TextOut( hDC, 82, 48, "5", 1 );
			TextOut( hDC, 170, 48, "5", 1 );

			TextOut( hDC, 201, 79, "5", 1 );
			TextOut( hDC, 201, 167, "5", 1 );

			TextOut( hDC, 82, 201, "5", 1 );
			TextOut( hDC, 170, 201, "5", 1 );

			if (roll > 5) pos_roll = 167;
			else if (roll < -5) pos_roll = 89;
			else pos_roll = 128 + Round( roll * 7.8 );

			if (pitch > 5) pos_pitch = 167;
			else if (pitch < -5) pos_pitch = 89;
			else pos_pitch = 128 + Round( pitch * 7.8 );

			if (yaw > 5) pos_yaw = 167;
			else if (yaw < -5) pos_yaw = 89;
			else pos_yaw = 128 + Round( yaw * 7.8 );
		}
		else if (adirate == 2)
		{
			// ADI RATE HIGH
			TextOut( hDC, 79, 48, "10", 2 );
			TextOut( hDC, 168, 48, "10", 2 );

			TextOut( hDC, 201, 79, "10", 2 );
			TextOut( hDC, 201, 167, "10", 2 );

			TextOut( hDC, 79, 201, "10", 2 );
			TextOut( hDC, 168, 201, "10", 2 );

			if (roll > 10) pos_roll = 167;
			else if (roll < -10) pos_roll = 89;
			else pos_roll = 128 + Round( roll * 3.9 );

			if (pitch > 10) pos_pitch = 167;
			else if (pitch < -10) pos_pitch = 89;
			else pos_pitch = 128 + Round( pitch * 3.9 );

			if (yaw > 10) pos_yaw = 167;
			else if (yaw < -10) pos_yaw = 89;
			else pos_yaw = 128 + Round( yaw * 3.9 );
		}
		else
		{
			// ADI RATE LOW
			TextOut( hDC, 82, 48, "1", 1 );
			TextOut( hDC, 170, 48, "1", 1 );

			TextOut( hDC, 201, 79, "1", 1 );
			TextOut( hDC, 201, 167, "1", 1 );

			TextOut( hDC, 82, 201, "1", 1 );
			TextOut( hDC, 170, 201, "1", 1 );

			if (roll > 1) pos_roll = 167;
			else if (roll < -1) pos_roll = 89;
			else pos_roll = 128 + Round( roll * 39 );

			if (pitch > 1) pos_pitch = 167;
			else if (pitch < -1) pos_pitch = 89;
			else pos_pitch = 128 + Round( pitch * 39 );

			if (yaw > 1) pos_yaw = 167;
			else if (yaw < -1) pos_yaw = 89;
			else pos_yaw = 128 + Round( yaw * 39 );
		}

		// draw triangles
		SelectObject( hDC, GreenPen );
		SelectObject( hDC, GreenBrush );
		POINT tri[3];
		tri[0].x = pos_roll;
		tri[0].y = 53;
		tri[1].x = tri[0].x + 5;
		tri[1].y = 44;
		tri[2].x = tri[0].x - 5;
		tri[2].y = 44;
		Polygon( hDC, tri, 3 );

		tri[0].x = 204;
		tri[0].y = pos_pitch;
		tri[1].x = 213;
		tri[1].y = tri[0].y - 5;
		tri[2].x = 213;
		tri[2].y = tri[0].y + 5;
		Polygon( hDC, tri, 3 );

		tri[0].x = pos_yaw;
		tri[0].y = 205;
		tri[1].x = tri[0].x - 5;
		tri[1].y = 214;
		tri[2].x = tri[0].x + 5;
		tri[2].y = 214;
		Polygon( hDC, tri, 3 );
		return;
	}

	void MDU::ADI_ERROR_A( HDC hDC, double pitch, double roll, double yaw, int adierr )
	{
		double pos_pitch;
		double pos_roll;
		double pos_yaw;

		SetTextColor( hDC, CR_MAGENTA );
		SelectObject( hDC, TahomaFont_h7w3 );
		if (adierr == 1)
		{
			// ADI ERROR MED
			// 5/5/5
			TextOut( hDC, 173, 64, "5", 1 );
			TextOut( hDC, 173, 118, "5", 1 );

			if (roll > 5) pos_roll = 25;
			else if (roll < -5) pos_roll = -25;
			else pos_roll = roll * 5;

			if (pitch > 5) pos_pitch = 25;
			else if (pitch < -5) pos_pitch = -25;
			else pos_pitch = pitch * 5;

			if (yaw > 5) pos_yaw = 25;
			else if (yaw < -5) pos_yaw = -25;
			else pos_yaw = yaw * 5;
		}
		else if (adierr == 2)
		{
			// ADI ERROR HIGH
			// 10/10/10
			TextOut( hDC, 173, 64, "10", 2 );
			TextOut( hDC, 173, 118, "10", 2 );

			if (roll > 10) pos_roll = 25;
			else if (roll < -10) pos_roll = -25;
			else pos_roll = roll * 2.5;

			if (pitch > 10) pos_pitch = 25;
			else if (pitch < -10) pos_pitch = -25;
			else pos_pitch = pitch * 2.5;

			if (yaw > 10) pos_yaw = 25;
			else if (yaw < -10) pos_yaw = -25;
			else pos_yaw = yaw * 2.5;
		}
		else
		{
			// ADI ERROR LOW
			// 1/1/1
			TextOut( hDC, 173, 64, "1", 1 );
			TextOut( hDC, 173, 118, "1", 1 );

			if (roll > 1) pos_roll = 25;
			else if (roll < -1) pos_roll = -25;
			else pos_roll = roll * 25;

			if (pitch > 1) pos_pitch = 25;
			else if (pitch < -1) pos_pitch = -25;
			else pos_pitch = pitch * 25;

			if (yaw > 1) pos_yaw = 25;
			else if (yaw < -1) pos_yaw = -25;
			else pos_yaw = yaw * 25;
		}
		SelectObject( hDC, TahomaFont_h10w4 );

		// draw needles
		SelectObject( hDC, MagentaPen );
		SelectObject( hDC, MagentaBrush );
		Rectangle( hDC, 121 + Round( pos_roll ), 94 - Round( sqrt( 3156 - (pos_roll * pos_roll) ) ), 123 + Round( pos_roll ), 77 );// roll
		Rectangle( hDC, 139, 93 + Round( pos_pitch ), 122 + Round( sqrt( 3156 - (pos_pitch * pos_pitch) ) ), 95 + Round( pos_pitch ) );// pitch
		Rectangle( hDC, 121 + Round( pos_yaw ), 111, 123 + Round( pos_yaw ), 94 + Round( sqrt( 3156 - (pos_yaw * pos_yaw) ) ) );// yaw
		return;
	}

	void MDU::ADI_ERROR_B( HDC hDC, double pitch, double roll, double yaw, int adierr )
	{
		double pos_pitch;
		double pos_roll;
		double pos_yaw;

		SetTextColor( hDC, CR_MAGENTA );
		SelectObject( hDC, TahomaFont_h7w3 );
		if (adierr == 1)
		{
			// ADI ERROR MED
			// 25/2/2.5
			TextOut( hDC, 173, 64, "2", 1 );
			TextOut( hDC, 173, 118, "2", 1 );

			if (roll > 25) pos_roll = 25;
			else if (roll < -25) pos_roll = -25;
			else pos_roll = roll;

			if (pitch > 2) pos_pitch = 25;
			else if (pitch < -2) pos_pitch = -25;
			else pos_pitch = pitch * 12.5;
		}
		else if (adierr == 2)
		{
			// ADI ERROR HIGH
			// 25/5/2.5
			TextOut( hDC, 173, 64, "5", 1 );
			TextOut( hDC, 173, 118, "5", 1 );

			if (roll > 25) pos_roll = 25;
			else if (roll < -25) pos_roll = -25;
			else pos_roll = roll;

			if (pitch > 5) pos_pitch = 25;
			else if (pitch < -5) pos_pitch = -25;
			else pos_pitch = pitch * 5;
		}
		else
		{
			// ADI ERROR LOW
			// 10/1/2.5
			TextOut( hDC, 173, 64, "1", 1 );
			TextOut( hDC, 173, 118, "1", 1 );

			if (roll > 10) pos_roll = 25;
			else if (roll < -10) pos_roll = -25;
			else pos_roll = roll * 2.5;

			if (pitch > 1) pos_pitch = 25;
			else if (pitch < -1) pos_pitch = -25;
			else pos_pitch = pitch * 25;
		}
		if (yaw > 2.5) pos_yaw = 25;
		else if (yaw < -2.5) pos_yaw = -25;
		else pos_yaw = yaw * 10;
		SelectObject( hDC, TahomaFont_h10w4 );

		// draw needles
		SelectObject( hDC, MagentaPen );
		SelectObject( hDC, MagentaBrush );
		Rectangle( hDC, 121 + Round( pos_roll ), 94 - Round( sqrt( 3156 - (pos_roll * pos_roll) ) ), 123 + Round( pos_roll ), 77 );// roll
		Rectangle( hDC, 139, 93 + Round( pos_pitch ), 122 + Round( sqrt( 3156 - (pos_pitch * pos_pitch) ) ), 95 + Round( pos_pitch ) );// pitch
		Rectangle( hDC, 121 + Round( pos_yaw ), 111, 123 + Round( pos_yaw ), 94 + Round( sqrt( 3156 - (pos_yaw * pos_yaw) ) ) );// yaw
		return;// 25/25/10 5/2/1 2.5/2.5/2.5
	}
	
	void MDU::ADI_ERROR_C( HDC hDC, double pitch, double roll, double yaw, int adierr )
	{
		double pos_pitch;
		double pos_roll;
		double pos_yaw;

		SetTextColor( hDC, CR_MAGENTA );
		SelectObject( hDC, TahomaFont_h7w3 );
		if (adierr == 0)
		{
			// ADI ERROR LOW
			// 10/0.5g/2.5
			TextOut( hDC, 173, 64, "0.5g", 4 );
			TextOut( hDC, 173, 118, "0.5g", 4 );

			if (roll > 10) pos_roll = 25;
			else if (roll < -10) pos_roll = -25;
			else pos_roll = roll * 2.5;

			if (pitch > 0.5) pos_pitch = 25;
			else if (pitch < -0.5) pos_pitch = -25;
			else pos_pitch = pitch * 50;
		}
		else
		{
			// ADI ERROR MED/HIGH
			// 25/1.25g/2.5
			TextOut( hDC, 173, 64, "1.2g", 4 );
			TextOut( hDC, 173, 118, "1.2g", 4 );

			if (roll > 25) pos_roll = 25;
			else if (roll < -25) pos_roll = -25;
			else pos_roll = roll;

			if (pitch > 1.25) pos_pitch = 25;
			else if (pitch < -1.25) pos_pitch = -25;
			else pos_pitch = pitch * 20;
		}
		if (yaw > 2.5) pos_yaw = 25;
		else if (yaw < -2.5) pos_yaw = -25;
		else pos_yaw = yaw * 10;
		SelectObject( hDC, TahomaFont_h10w4 );

		// draw needles
		SelectObject( hDC, MagentaPen );
		SelectObject( hDC, MagentaBrush );
		Rectangle( hDC, 121 + Round( pos_roll ), 94 - Round( sqrt( 3156 - (pos_roll * pos_roll) ) ), 123 + Round( pos_roll ), 77 );// roll
		Rectangle( hDC, 139, 93 + Round( pos_pitch ), 122 + Round( sqrt( 3156 - (pos_pitch * pos_pitch) ) ), 95 + Round( pos_pitch ) );// pitch
		Rectangle( hDC, 121 + Round( pos_yaw ), 111, 123 + Round( pos_yaw ), 94 + Round( sqrt( 3156 - (pos_yaw * pos_yaw) ) ) );// yaw
		return;// 25/25/10 1.25/1.25/0.5 2.5/2.5/2.5
	}

	void MDU::ADI_ERROR_D( HDC hDC, double pitch, double roll, double yaw, int adierr )
	{
		double pos_pitch;
		double pos_roll;
		double pos_yaw;

		SetTextColor( hDC, CR_MAGENTA );
		SelectObject( hDC, TahomaFont_h7w3 );
		if (adierr == 1)
		{
			// ADI ERROR MED
			// 5/5/2.5
			TextOut( hDC, 173, 64, "5", 1 );
			TextOut( hDC, 173, 118, "5", 1 );

			if (roll > 5) pos_roll = 25;
			else if (roll < -5) pos_roll = -25;
			else pos_roll = roll * 5;

			if (pitch > 5) pos_pitch = 25;
			else if (pitch < -5) pos_pitch = -25;
			else pos_pitch = pitch * 5;
		}
		else if (adierr == 2)
		{
			// ADI ERROR HIGH
			// 20/10/2.5
			TextOut( hDC, 173, 64, "10", 2 );
			TextOut( hDC, 173, 118, "10", 2 );

			if (roll > 20) pos_roll = 25;
			else if (roll < -20) pos_roll = -25;
			else pos_roll = roll * 1.25;

			if (pitch > 10) pos_pitch = 25;
			else if (pitch < -10) pos_pitch = -25;
			else pos_pitch = pitch * 2.5;
		}
		else
		{
			// ADI ERROR LOW
			// 1/1/2.5
			TextOut( hDC, 173, 64, "1", 1 );
			TextOut( hDC, 173, 118, "1", 1 );

			if (roll > 1) pos_roll = 25;
			else if (roll < -1) pos_roll = -25;
			else pos_roll = roll * 25;

			if (pitch > 1) pos_pitch = 25;
			else if (pitch < -1) pos_pitch = -25;
			else pos_pitch = pitch * 25;
		}
		if (yaw > 2.5) pos_yaw = 25;
		else if (yaw < -2.5) pos_yaw = -25;
		else pos_yaw = yaw * 10;
		SelectObject( hDC, TahomaFont_h10w4 );

		// draw needles
		SelectObject( hDC, MagentaPen );
		SelectObject( hDC, MagentaBrush );
		Rectangle( hDC, 121 + Round( pos_roll ), 94 - Round( sqrt( 3156 - (pos_roll * pos_roll) ) ), 123 + Round( pos_roll ), 77 );// roll
		Rectangle( hDC, 139, 93 + Round( pos_pitch ), 122 + Round( sqrt( 3156 - (pos_pitch * pos_pitch) ) ), 95 + Round( pos_pitch ) );// pitch
		Rectangle( hDC, 121 + Round( pos_yaw ), 111, 123 + Round( pos_yaw ), 94 + Round( sqrt( 3156 - (pos_yaw * pos_yaw) ) ) );// yaw
		return;// 20/5/1 10/5/1 2.5/2.5/2.5
	}

	void MDU::ADI_ERROR_ORBIT( HDC hDC, double pitch, double roll, double yaw, int adierr )
	{
		double pos_pitch;
		double pos_roll;
		double pos_yaw;

		SetTextColor( hDC, CR_MAGENTA );
		if (adierr == 1)
		{
			// ADI ERROR MED
			// 5/5/5
			TextOut( hDC, 185, 94, "5", 1 );
			TextOut( hDC, 185, 155, "5", 1 );

			if (roll > 5) pos_roll = 28;
			else if (roll < -5) pos_roll = -28;
			else pos_roll = roll * 5.6;

			if (pitch > 5) pos_pitch = 28;
			else if (pitch < -5) pos_pitch = -28;
			else pos_pitch = pitch * 5.6;

			if (yaw > 5) pos_yaw = 28;
			else if (yaw < -5) pos_yaw = -28;
			else pos_yaw = yaw * 5.6;
		}
		else if (adierr == 2)
		{
			// ADI ERROR HIGH
			// 10/10/10
			TextOut( hDC, 185, 94, "10", 2 );
			TextOut( hDC, 185, 155, "10", 2 );

			if (roll > 10) pos_roll = 28;
			else if (roll < -10) pos_roll = -28;
			else pos_roll = roll * 2.8;

			if (pitch > 10) pos_pitch = 28;
			else if (pitch < -10) pos_pitch = -28;
			else pos_pitch = pitch * 2.8;

			if (yaw > 10) pos_yaw = 28;
			else if (yaw < -10) pos_yaw = -28;
			else pos_yaw = yaw * 2.8;
		}
		else
		{
			// ADI ERROR LOW
			// 1/1/1
			TextOut( hDC, 185, 94, "1", 1 );
			TextOut( hDC, 185, 155, "1", 1 );

			if (roll > 1) pos_roll = 28;
			else if (roll < -1) pos_roll = -28;
			else pos_roll = roll * 28;

			if (pitch > 1) pos_pitch = 28;
			else if (pitch < -1) pos_pitch = -28;
			else pos_pitch = pitch * 28;

			if (yaw > 1) pos_yaw = 28;
			else if (yaw < -1) pos_yaw = -28;
			else pos_yaw = yaw * 28;
		}

		// draw needles
		SelectObject( hDC, MagentaPen );
		SelectObject( hDC, MagentaBrush );
		Rectangle( hDC, 127 + Round( pos_roll ), 128 - Round( sqrt( 3969 - (pos_roll * pos_roll) ) ), 129 + Round( pos_roll ), 109 );// roll
		Rectangle( hDC, 147, 127 + Round( pos_pitch ), 128 + Round( sqrt( 3969 - (pos_pitch * pos_pitch) ) ), 129 + Round( pos_pitch ) );// pitch
		Rectangle( hDC, 127 + Round( pos_yaw ), 147, 129 + Round( pos_yaw ), 128 + Round( sqrt( 3969 - (pos_yaw * pos_yaw) ) ) );// yaw
		return;
	}

	void MDU::HSI_A( HDC hDC, double heading, double roll )
	{
		double sgn = sign( (90 * RAD) - fabs( roll ) );
		// center (122,238) r = 57
		SelectObject( hDC, TurquoisePen );
		MoveToEx( hDC, 65, 238, NULL );
		LineTo( hDC, 61, 238 );
		MoveToEx( hDC, 82, 198, NULL );
		LineTo( hDC, 79, 195 );
		MoveToEx( hDC, 122, 186, NULL );
		LineTo( hDC, 122, 177 );
		MoveToEx( hDC, 162, 198, NULL );
		LineTo( hDC, 165, 195 );
		MoveToEx( hDC, 179, 238, NULL );
		LineTo( hDC, 183, 238 );

		SelectObject( hDC, WhitePen );
		SelectObject( hDC, GrayLightBrush );
		::Ellipse( hDC, 70, 186, 175, 291 );

		SelectObject( hDC, BlackBrush );
		::Ellipse( hDC, 89, 205, 156, 272 );

		Arc( hDC, 65, 181, 180, 296, 176, 256, 69, 257 );

		int x1;
		int y1;
		int x2;
		int y2;
		double cosi;
		double sini;
		SetTextColor( hDC, CR_WHITE );
		for (int i = 90; i != 0; i -= 10)// big lines
		{
			cosi = cos( (i * RAD) + (heading * sgn) );
			sini = sin( (i * RAD) + (heading * sgn) );
			x1 = Round( 51 * cosi );
			y1 = Round( 51 * sini );

			x2 = Round( 46 * cosi );
			y2 = Round( 46 * sini );

			MoveToEx( hDC, 122 + x1, 238 - y1, NULL );
			LineTo( hDC, 122 + x2, 238 - y2 );

			MoveToEx( hDC, 122 + y1, 238 + x1, NULL );
			LineTo( hDC, 122 + y2, 238 + x2 );

			MoveToEx( hDC, 122 - x1, 238 + y1, NULL );
			LineTo( hDC, 122 - x2, 238 + y2 );

			MoveToEx( hDC, 122 - y1, 238 - x1, NULL );
			LineTo( hDC, 122 - y2, 238 - x2 );
		}

		for (int i = 85; i > 0; i -= 10)// small lines
		{
			cosi = cos( (i * RAD) + (heading * sgn) );
			sini = sin( (i * RAD) + (heading * sgn) );
			x1 = Round( 51 * cosi );
			y1 = Round( 51 * sini );

			x2 = Round( 48 * cosi );
			y2 = Round( 48 * sini );

			MoveToEx( hDC, 122 + x1, 238 - y1, NULL );
			LineTo( hDC, 122 + x2, 238 - y2 );

			MoveToEx( hDC, 122 + y1, 238 + x1, NULL );
			LineTo( hDC, 122 + y2, 238 + x2 );

			MoveToEx( hDC, 122 - x1, 238 + y1, NULL );
			LineTo( hDC, 122 - x2, 238 + y2 );

			MoveToEx( hDC, 122 - y1, 238 - x1, NULL );
			LineTo( hDC, 122 - y2, 238 - x2 );
		}

		XFORM WTroll;
		char cbuf[4];
		SetGraphicsMode( hDC, GM_ADVANCED );
		for (int i = 0; i <= 33; i += 3)
		{
			// rotate
			WTroll.eM11 = (FLOAT)cos( (heading * sgn) - i * 10 * RAD * sgn );
			WTroll.eM12 = (FLOAT)(-sin( (heading * sgn) - i * 10 * RAD * sgn ));
			WTroll.eM21 = -WTroll.eM12;
			WTroll.eM22 = WTroll.eM11;
			WTroll.eDx = (FLOAT)(122 - (122 * WTroll.eM11) - (238 * WTroll.eM21));
			WTroll.eDy = (FLOAT)(238 - (238 * WTroll.eM11) + (122 * WTroll.eM21));
			SetWorldTransform( hDC, &WTroll );

			// paint
			sprintf_s( cbuf, 4, "%d", i );// number
			TextOut( hDC, 122 - strlen( cbuf ) * 2, 193, cbuf, strlen( cbuf ) );

			// de-rotate
			ModifyWorldTransform( hDC, &WTroll, MWT_IDENTITY );
		}
		SetGraphicsMode( hDC, GM_COMPATIBLE );
		return;
	}

	void MDU::HSI_E( HDC hDC, double heading )
	{
		// center (122,238) r = 57
		SelectObject( hDC, TurquoisePen );
		MoveToEx( hDC, 65, 238, NULL );
		LineTo( hDC, 61, 238 );
		MoveToEx( hDC, 82, 198, NULL );
		LineTo( hDC, 79, 195 );
		MoveToEx( hDC, 122, 186, NULL );
		LineTo( hDC, 122, 177 );
		MoveToEx( hDC, 162, 198, NULL );
		LineTo( hDC, 165, 195 );
		MoveToEx( hDC, 179, 238, NULL );
		LineTo( hDC, 183, 238 );

		SelectObject( hDC, WhitePen );
		SelectObject( hDC, GrayLightBrush );
		::Ellipse( hDC, 70, 186, 175, 291 );

		SelectObject( hDC, BlackBrush );
		::Ellipse( hDC, 89, 205, 156, 272 );

		Arc( hDC, 65, 181, 180, 296, 176, 256, 69, 257 );

		int x1;
		int y1;
		int x2;
		int y2;
		double cosi;
		double sini;
		SetTextColor( hDC, CR_WHITE );
		for (int i = 90; i != 0; i -= 10)// big lines
		{
			cosi = cos( (i * RAD) + heading );
			sini = sin( (i * RAD) + heading );
			x1 = Round( 51 * cosi );
			y1 = Round( 51 * sini );

			x2 = Round( 46 * cosi );
			y2 = Round( 46 * sini );

			MoveToEx( hDC, 122 + x1, 238 - y1, NULL );
			LineTo( hDC, 122 + x2, 238 - y2 );

			MoveToEx( hDC, 122 + y1, 238 + x1, NULL );
			LineTo( hDC, 122 + y2, 238 + x2 );

			MoveToEx( hDC, 122 - x1, 238 + y1, NULL );
			LineTo( hDC, 122 - x2, 238 + y2 );

			MoveToEx( hDC, 122 - y1, 238 - x1, NULL );
			LineTo( hDC, 122 - y2, 238 - x2 );
		}

		for (int i = 85; i > 0; i -= 10)// small lines
		{
			cosi = cos( (i * RAD) + heading );
			sini = sin( (i * RAD) + heading );
			x1 = Round( 51 * cosi );
			y1 = Round( 51 * sini );

			x2 = Round( 48 * cosi );
			y2 = Round( 48 * sini );

			MoveToEx( hDC, 122 + x1, 238 - y1, NULL );
			LineTo( hDC, 122 + x2, 238 - y2 );

			MoveToEx( hDC, 122 + y1, 238 + x1, NULL );
			LineTo( hDC, 122 + y2, 238 + x2 );

			MoveToEx( hDC, 122 - x1, 238 + y1, NULL );
			LineTo( hDC, 122 - x2, 238 + y2 );

			MoveToEx( hDC, 122 - y1, 238 - x1, NULL );
			LineTo( hDC, 122 - y2, 238 - x2 );
		}

		XFORM WTroll;
		char cbuf[4];
		SetGraphicsMode( hDC, GM_ADVANCED );
		for (int i = 0; i <= 33; i += 3)
		{
			// rotate
			WTroll.eM11 = (FLOAT)cos( heading - i * 10 * RAD );
			WTroll.eM12 = (FLOAT)(-sin( heading - i * 10 * RAD ));
			WTroll.eM21 = -WTroll.eM12;
			WTroll.eM22 = WTroll.eM11;
			WTroll.eDx = (FLOAT)(122 - (122 * WTroll.eM11) - (238 * WTroll.eM21));
			WTroll.eDy = (FLOAT)(238 - (238 * WTroll.eM11) + (122 * WTroll.eM21));
			SetWorldTransform( hDC, &WTroll );

			// paint
			if ((i % 9) != 0) sprintf_s( cbuf, 4, "%d", i );// number
			else if (i == 0) sprintf_s( cbuf, 4, "N" );// letter
			else if (i == 9) sprintf_s( cbuf, 4, "E" );// letter
			else if (i == 18) sprintf_s( cbuf, 4, "S" );// letter
			else sprintf_s( cbuf, 4, "W" );// letter
			TextOut( hDC, 122 - strlen( cbuf ) * 2, 193, cbuf, strlen( cbuf ) );

			// de-rotate
			ModifyWorldTransform( hDC, &WTroll, MWT_IDENTITY );
		}
		SetGraphicsMode( hDC, GM_COMPATIBLE );
		return;
	}

	void MDU::AEPFD_Header_AscentDAP( HDC hDC, int MM, int adiatt )
	{
		char cbuf[8];
		SetTextColor( hDC, CR_GRAY_DARK );
		TextOut( hDC, 21, 4, "DAP:", 4 );

		TextOut( hDC, 209, 4, "MM:", 3 );
		
		TextOut( hDC, 9, 13, "THROT:", 6 );

		TextOut( hDC, 208, 13, "ATT:", 4 );

		SetTextColor( hDC, CR_WHITE );
		if (1) TextOut( hDC, 40, 4, "Auto", 4 );// TODO get AscentDAP state
		else
		{
			SelectObject( hDC, YellowPen );
			SelectObject( hDC, GetStockObject( HOLLOW_BRUSH ) );
			Rectangle( hDC, 6, 4, 60, 14 );
			TextOut( hDC, 40, 4, "CSS", 3 );
		}

		if (0) sprintf_s( cbuf, 8, "%dR", MM );// RTLS
		else if (0) sprintf_s( cbuf, 8, "%dT", MM );// TAL
		else if (0) sprintf_s( cbuf, 8, "%dATO", MM );// ATO
		else if (0) sprintf_s( cbuf, 8, "%dAOA", MM );// AOA
		else if (0) sprintf_s( cbuf, 8, "%dCA", MM );// CA
		else sprintf_s( cbuf, 8, "%d", MM );// NOM
		TextOut( hDC, 225, 4, cbuf, strlen( cbuf ) );

		if (GetIDP()->GetAutoThrottleState() == true) TextOut( hDC, 40, 13, "Auto", 4 );
		else
		{
			SelectObject( hDC, YellowPen );
			SelectObject( hDC, GetStockObject( HOLLOW_BRUSH ) );
			Rectangle( hDC, 6, 13, 60, 23 );
			TextOut( hDC, 40, 13, "MAN", 3 );
		}

		if (adiatt == 2) TextOut( hDC, 228, 13, "INRTL", 5 );
		else if (adiatt == 1) TextOut( hDC, 228, 13, "LVLH", 4 );
		else TextOut( hDC, 228, 13, "REF", 3 );
		return;
	}

	void MDU::AEPFD_Header_TransDAP( HDC hDC, int MM, int adiatt )
	{
		char cbuf[4];
		SetTextColor( hDC, CR_GRAY_DARK );
		TextOut( hDC, 21, 4, "DAP:", 4 );

		TextOut( hDC, 209, 4, "MM:", 3 );

		TextOut( hDC, 208, 13, "ATT:", 4 );

		SetTextColor( hDC, CR_WHITE );
		if (1) TextOut( hDC, 40, 4, "Auto", 4 );// TODO get TransDAP state
		else TextOut( hDC, 40, 4, "INRTL", 5 );

		sprintf_s( cbuf, 4, "%d", MM );
		TextOut( hDC, 225, 4, cbuf, strlen( cbuf ) );

		if (adiatt == 2) TextOut( hDC, 228, 13, "INRTL", 5 );
		else if (adiatt == 1) TextOut( hDC, 228, 13, "LVLH", 4 );
		else TextOut( hDC, 228, 13, "REF", 3 );
		return;
	}

	void MDU::AEPFD_Header_AerojetDAP( HDC hDC, int MM, double MachNumber )
	{
		char cbuf[8];
		SetTextColor( hDC, CR_GRAY_DARK );
		TextOut( hDC, 16, 4, "Pitch:", 6 );

		TextOut( hDC, 21, 13, "R/Y:", 4 );

		TextOut( hDC, 209, 4, "MM:", 3 );

		TextOut( hDC, 210, 13, "SB:", 3 );

		SetTextColor( hDC, CR_WHITE );
		if (GetIDP()->GetAutoPitchState() == true) TextOut( hDC, 40, 4, "Auto", 4 );
		else
		{
			if (MachNumber > 1)
			{
				SelectObject( hDC, YellowPen );
				SelectObject( hDC, GetStockObject( HOLLOW_BRUSH ) );
				Rectangle( hDC, 6, 4, 60, 14 );
			}
			TextOut( hDC, 40, 4, "CSS", 3 );
		}

		if (GetIDP()->GetAutoRollYawState() == true) TextOut( hDC, 40, 13, "Auto", 4 );
		else
		{
			if (MachNumber > 1)
			{
				SelectObject( hDC, YellowPen );
				SelectObject( hDC, GetStockObject( HOLLOW_BRUSH ) );
				Rectangle( hDC, 6, 13, 60, 23 );
			}
			TextOut( hDC, 40, 13, "CSS", 3 );
		}

		if (0) sprintf_s( cbuf, 8, "%dR", MM );// RTLS
		else if (0) sprintf_s( cbuf, 8, "%dT", MM );// TAL
		else if (0) sprintf_s( cbuf, 8, "%dATO", MM );// ATO
		else if (0) sprintf_s( cbuf, 8, "%dAOA", MM );// AOA
		else if (0) sprintf_s( cbuf, 8, "%dCA", MM );// CA
		else sprintf_s( cbuf, 8, "%d", MM );// NOM
		TextOut( hDC, 225, 4, cbuf, strlen( cbuf ) );

		if (GetIDP()->GetAutoSpeedbrakeState() == true) TextOut( hDC, 225, 13, "Auto", 4 );
		else
		{
			SelectObject( hDC, YellowPen );
			SelectObject( hDC, GetStockObject( HOLLOW_BRUSH ) );
			Rectangle( hDC, 207, 13, 245, 23 );
			TextOut( hDC, 225, 13, "MAN", 3 );
		}
		return;
	}

	void MDU::AEPFD_BETA( HDC hDC )
	{
		SelectObject( hDC, GrayDarkPen );
		SelectObject( hDC, BlackBrush );
		Rectangle( hDC, 37, 159, 60, 171 );
		SetTextColor( hDC, CR_GRAY_DARK );
		TextOut( hDC, 41, 172, "Beta", 4 );

		double beta = STS()->GetSlipAngle() * DEG;
		char side = 'R';
		if (beta < 0) side = 'L';
		SetTextColor( hDC, CR_WHITE );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%c%04.1f", side, fabs( beta ) );
		TextOut( hDC, 39, 160, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::AEPFD_GMETER_STATIC( HDC hDC )
	{
		SelectObject( hDC, GrayDarkPen );
		SelectObject( hDC, BlackBrush );
		Rectangle( hDC, 40, 198, 66, 211 );

		SelectObject( hDC, GrayLightPen );
		Arc( hDC, 10, 193, 58, 241, 34, 193, 41, 224 );
		
		MoveToEx( hDC, 34, 193, NULL );
		LineTo( hDC, 34, 189 );
		MoveToEx( hDC, 17, 200, NULL );
		LineTo( hDC, 14, 197 );
		MoveToEx( hDC, 10, 217, NULL );
		LineTo( hDC, 6, 217 );
		MoveToEx( hDC, 17, 234, NULL );
		LineTo( hDC, 14, 237 );
		MoveToEx( hDC, 34, 241, NULL );
		LineTo( hDC, 34, 245 );
		MoveToEx( hDC, 50, 234, NULL );
		LineTo( hDC, 54, 237 );

		SetTextColor( hDC, CR_GRAY_LIGHT );
		TextOut( hDC, 10, 193, "3", 1 );
		TextOut( hDC, 2, 213, "2", 1 );
		TextOut( hDC, 10, 231, "1", 1 );
		TextOut( hDC, 32, 244, "0", 1 );
		TextOut( hDC, 52, 234, "-1", 2 );
		return;
	}

	void MDU::AEPFD_GMETER_ACCEL( HDC hDC )
	{
		SetTextColor( hDC, CR_GRAY_DARK );
		TextOut( hDC, 43, 212, "Accel", 5 );
		VECTOR3 f;
		STS()->GetForceVector( f );
		double dtmp = (f.z / (STS()->GetMass() * G)) + sin( STS()->GetPitch() );

		SetTextColor( hDC, CR_WHITE );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%4.1f g", dtmp );
		TextOut( hDC, 43, 200, cbuf, strlen( cbuf ) );

		if (dtmp > 4) dtmp = 4;
		else if (dtmp < -1) dtmp = -1;
		dtmp = ((dtmp * 45) - 90) * RAD;

		double cosdtmp = cos( dtmp );
		double sindtmp = sin( dtmp );
		// center (34.217)
		POINT arrow[3];
		arrow[0].x = 34 - Round( 23 * cosdtmp );
		arrow[0].y = 217 - Round( 23 * sindtmp );
		arrow[1].x = 34 - Round( (17 * cosdtmp) + (4 * sindtmp) );
		arrow[1].y = 217 - Round( (17 * sindtmp) - (4 * cosdtmp) );
		arrow[2].x = 34 - Round( (17 * cosdtmp) - (4 * sindtmp) );
		arrow[2].y = 217 - Round( (17 * sindtmp) + (4 * cosdtmp) );

		SelectObject( hDC, GreenPen );
		SelectObject( hDC, GreenBrush );
		Polygon( hDC, arrow, 3 );

		SelectObject( hDC, GreenThickPen );
		MoveToEx( hDC, 34, 217, NULL );
		LineTo( hDC, arrow[0].x, arrow[0].y );
		return;
	}

	void MDU::AEPFD_GMETER_NZ( HDC hDC )
	{
		SetTextColor( hDC, CR_GRAY_DARK );
		TextOut( hDC, 43, 212, "Nz", 2 );
		double dtmp = GetIDP()->GetNZ();

		SetTextColor( hDC, CR_WHITE );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%4.1f g", dtmp );
		TextOut( hDC, 43, 200, cbuf, strlen( cbuf ) );

		if (dtmp > 4) dtmp = 4;
		else if (dtmp < -1) dtmp = -1;
		dtmp = ((dtmp * 45) - 90) * RAD;

		double cosdtmp = cos( dtmp );
		double sindtmp = sin( dtmp );
		// center (34.217)
		POINT arrow[3];
		arrow[0].x = 34 - Round( 23 * cosdtmp );
		arrow[0].y = 217 - Round( 23 * sindtmp );
		arrow[1].x = 34 - Round( (17 * cosdtmp) + (4 * sindtmp) );
		arrow[1].y = 217 - Round( (17 * sindtmp) - (4 * cosdtmp) );
		arrow[2].x = 34 - Round( (17 * cosdtmp) - (4 * sindtmp) );
		arrow[2].y = 217 - Round( (17 * sindtmp) + (4 * cosdtmp) );

		SelectObject( hDC, GreenPen );
		SelectObject( hDC, GreenBrush );
		Polygon( hDC, arrow, 3 );

		SelectObject( hDC, GreenThickPen );
		MoveToEx( hDC, 34, 217, NULL );
		LineTo( hDC, arrow[0].x, arrow[0].y );
		return;
	}

	void MDU::AEPFD_HACCEL( HDC hDC )
	{
		SelectObject( hDC, GrayDarkPen );
		Rectangle( hDC, 234, 168, 241, 240 );
		MoveToEx( hDC, 241, 169, NULL );
		LineTo( hDC, 244, 169 );
		MoveToEx( hDC, 241, 176, NULL );
		LineTo( hDC, 244, 176 );
		MoveToEx( hDC, 241, 183, NULL );
		LineTo( hDC, 244, 183 );
		MoveToEx( hDC, 241, 190, NULL );
		LineTo( hDC, 244, 190 );
		MoveToEx( hDC, 241, 197, NULL );
		LineTo( hDC, 244, 197 );
		MoveToEx( hDC, 241, 204, NULL );
		LineTo( hDC, 244, 204 );
		MoveToEx( hDC, 241, 211, NULL );
		LineTo( hDC, 244, 211 );
		MoveToEx( hDC, 241, 218, NULL );
		LineTo( hDC, 244, 218 );
		MoveToEx( hDC, 241, 225, NULL );
		LineTo( hDC, 244, 225 );
		MoveToEx( hDC, 241, 232, NULL );
		LineTo( hDC, 244, 232 );
		MoveToEx( hDC, 241, 239, NULL );
		LineTo( hDC, 244, 239 );
		SetPixel( hDC, 236, 158, CR_GRAY_DARK );
		SetPixel( hDC, 238, 158, CR_GRAY_DARK );
		SetTextColor( hDC, CR_GRAY_DARK );
		TextOut( hDC, 235, 159, "H", 1 );
		SetTextColor( hDC, CR_WHITE );
		TextOut( hDC, 244, 165, "10", 2 );
		TextOut( hDC, 245, 200, "0", 1 );
		TextOut( hDC, 244, 235, "-10", 3 );

		double dtmp = GetIDP()->GetVacc();
		if (dtmp > 10) dtmp = 10;
		else if (dtmp < -10) dtmp = -10;

		POINT tri[3];// starts at tip moving cw
		tri[0].x = 237;
		tri[0].y = 204 - Round( dtmp * 3.5 );
		tri[1].x = 229;
		tri[1].y = tri[0].y + 5;
		tri[2].x = 229;
		tri[2].y = tri[0].y - 5;

		SelectObject( hDC, GreenPen );
		SelectObject( hDC, GreenBrush );
		Polygon( hDC, tri, 3 );
		return;
	}

	void MDU::AEPFD_RANGERW( HDC hDC )
	{
		SelectObject( hDC, GrayDarkPen );
		SelectObject( hDC, BlackBrush );
		Rectangle( hDC, 190, 206, 213, 219 );
		SetTextColor( hDC, CR_GRAY_DARK );
		TextOut( hDC, 189, 196, GetIDP()->GetSelectedRunway().c_str(), 5 );
		SetTextColor( hDC, CR_WHITE );
		double dtmp = GetIDP()->GetRangeToRunway();
		char cbuf[8];
		if (dtmp > 1000) sprintf_s( cbuf, 8, "%4.0f", dtmp );// HACK max should be 999.9, but this way is better for testing
		else sprintf_s( cbuf, 8, "%5.1f", dtmp );
		TextOut( hDC, 193, 207, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::AEPFD_RANGEHACC( HDC hDC )
	{
		SelectObject( hDC, GrayDarkPen );
		SelectObject( hDC, BlackBrush );
		Rectangle( hDC, 190, 231, 213, 244 );
		SetTextColor( hDC, CR_GRAY_DARK );
		TextOut( hDC, 190, 221, "HAC-C", 5 );
		SetTextColor( hDC, CR_WHITE );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%5.1f", GetIDP()->GetDistanceToHACCenter() );
		TextOut( hDC, 193, 233, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::AEPFD_dAZ_HTA( HDC hDC, double MachNumber )
	{
		double dtmp;
		char cbuf[8];
		SelectObject( hDC, GrayDarkPen );
		SelectObject( hDC, BlackBrush );
		SetTextColor( hDC, CR_GRAY_DARK );
		if (GetIDP()->GetOnHACState() == false)
		{
			MoveToEx( hDC, 149, 179, NULL );
			LineTo( hDC, 151, 174 );
			LineTo( hDC, 153, 179 );
			LineTo( hDC, 149, 179 );
			TextOut( hDC, 154, 172, "AZ", 2 );
			dtmp = fabs( GetIDP()->GetdeltaAZ() );
			if (dtmp >= GetIDP()->GetdeltaAZLimit( MachNumber ))
			{
				// rectangle flashes red for limits
				if (GetFlash() == false) SelectObject( hDC, BlackPen );
				else SelectObject( hDC, RedPen );
			}
		}
		else
		{
			TextOut( hDC, 149, 172, "HTA", 3 );
			dtmp = GetIDP()->GetHTA();
		}

		Rectangle( hDC, 167, 171, 188, 183 );

		SetTextColor( hDC, CR_WHITE );
		sprintf_s( cbuf, 8, "%3.0f%c", dtmp, 248 );
		TextOut( hDC, 170, 172, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::AEPFD_dXTRK( HDC hDC )
	{
		SelectObject( hDC, GrayDarkPen );
		SelectObject( hDC, BlackBrush );
		SetTextColor( hDC, CR_GRAY_DARK );
		POINT tri[3] = {{203, 168}, {205, 173}, {201, 173}};
		Polygon( hDC, tri, 3 );
		TextOut( hDC, 208, 166, "X-Trk", 5 );
		Rectangle( hDC, 229, 164, 252, 176 );
		SetTextColor( hDC, CR_WHITE );
		double dtmp = 0;// TODO
		char cbuf[8];
		sprintf_s( cbuf, 8, "%5.1f", dtmp );
		TextOut( hDC, 232, 165, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::AEPFD_XTRK( HDC hDC )
	{
		SelectObject( hDC, GrayDarkPen );
		SelectObject( hDC, BlackBrush );
		SetTextColor( hDC, CR_GRAY_DARK );
		TextOut( hDC, 207, 184, "X-Trk", 5 );
		Rectangle( hDC, 229, 182, 252, 194 );
		SetTextColor( hDC, CR_WHITE );
		double dtmp = 0;// TODO
		char cbuf[8];
		sprintf_s( cbuf, 8, "%5.1f", dtmp );
		TextOut( hDC, 232, 183, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::AEPFD_dINC( HDC hDC )
	{
		SelectObject( hDC, GrayDarkPen );
		SelectObject( hDC, BlackBrush );
		SetTextColor( hDC, CR_GRAY_DARK );
		POINT tri[3] = {{205, 205}, {207, 210}, {203, 210}};
		Polygon( hDC, tri, 3 );
		TextOut( hDC, 210, 203, "Inc", 3 );
		Rectangle( hDC, 223, 201, 252, 213 );
		SetTextColor( hDC, CR_WHITE );
		ELEMENTS el;
		STS()->GetElements( STS()->GetGravityRef(), el, NULL, 0, FRAME_EQU );
		char cbuf[8];
		sprintf_s( cbuf, 8, "%6.2f", (STS()->pMission->GetMECOInc() - el.i) * DEG );
		TextOut( hDC, 228, 202, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::AEPFD_TGTINC( HDC hDC )
	{
		SelectObject( hDC, GrayDarkPen );
		SelectObject( hDC, BlackBrush );
		SetTextColor( hDC, CR_GRAY_DARK );
		TextOut( hDC, 196, 222, "Tgt Inc", 7 );
		Rectangle( hDC, 223, 220, 252, 232 );
		SetTextColor( hDC, CR_WHITE );
		double dtmp = 0;// TODO
		char cbuf[8];
		sprintf_s( cbuf, 8, "%5.2f", dtmp );
		TextOut( hDC, 226, 221, cbuf, strlen( cbuf ) );
		return;
	}

	void MDU::AEPFD_GSI( HDC hDC, double Altitude_ft )
	{
		SelectObject( hDC, GrayDarkPen );
		SelectObject( hDC, BlackBrush );
		Rectangle( hDC, 217, 165, 224, 235 );
		SelectObject( hDC, GrayDarkBrush );
		::Ellipse( hDC, 219, 166, 222, 169 );
		::Ellipse( hDC, 219, 183, 222, 186 );
		::Ellipse( hDC, 219, 215, 222, 218 );
		::Ellipse( hDC, 219, 231, 222, 234 );
		MoveToEx( hDC, 217, 200, NULL );
		LineTo( hDC, 224, 200 );

		if (Altitude_ft < 1500)
		{
			TextOut( hDC, 216, 156, "1K", 2 );
			TextOut( hDC, 216, 234, "1K", 2 );
			// no tracking -> print GS label
			SelectObject( hDC, RedPen );
			SelectObject( hDC, RedBrush );
			Rectangle( hDC, 215, 196, 226, 204 );
			SetTextColor( hDC, CR_BLACK );
			TextOut( hDC, 216, 195, "GS", 2 );
		}
		else
		{
			POINT tri[3];
			tri[0].x = 220;// starts at tip moving cw
			tri[1].x = 228;
			tri[2].x = 228;
			double dtmp = GetIDP()->GetGlideSlopeDistance();// get dist ABOVE glide path

			if (GetIDP()->GetApproachAndLandState() == false)
			{
				// TAEM
				TextOut( hDC, 216, 156, "5K", 2 );
				TextOut( hDC, 216, 234, "5K", 2 );

				if (dtmp > 5000)
				{
					if (GetFlash() == false) return;
					tri[0].y = 235;
				}
				else if (dtmp < -5000)
				{
					if (GetFlash() == false) return;
					tri[0].y = 165;
				}
				else tri[0].y = 200 + Round( dtmp * 0.007 );
			}
			else
			{
				// A/L
				TextOut( hDC, 216, 156, "1K", 2 );
				TextOut( hDC, 216, 234, "1K", 2 );

				if (dtmp > 1000)
				{
					if (GetFlash() == false) return;
					tri[0].y = 235;
				}
				else if (dtmp < -1000)
				{
					if (GetFlash() == false) return;
					tri[0].y = 165;
				}
				else tri[0].y = 200 + Round( dtmp * 0.035 );
			}

			tri[1].y = tri[0].y - 5;
			tri[2].y = tri[0].y + 5;
			SelectObject( hDC, GreenPen );
			SelectObject( hDC, GreenBrush );
			Polygon( hDC, tri, 3 );
		}
		return;
	}
};
