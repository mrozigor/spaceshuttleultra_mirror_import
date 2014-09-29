#include "../Atlantis.h"
#include "MDU.h"
#include "../dps/IDP.h"
#include "../meshres_vc_additions.h"
#include "..\dps\AscentGuidance.h"
#include "..\dps\AerojetDAP.h"
#include "..\dps\SSME_Operations.h"

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

		hDCTapes = NULL;
		RangeAlpha[0] = 1;
		RangeAlpha[1] = 0;
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

	void MDU::DrawCommonHeader(const char* cDispTitle)
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
	}

	const string& MDU::GetEdgekeyMenu() const
	{
		static string r = "MAIN MENU";
		return r;
	}

	char* MDU::GetEdgeKeyMenuLabel(int iButton)
	{
		return NULL;
	}

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
		sprintf_s(oapiDebugString(), 80, "MDU %s mouse event %d (%f, %f)", GetQualifiedIdentifier().c_str(), _event, x, y);

		if(MFDID!=-1) {
			if(y >= btnPwrYmin && y<= btnPwrYmax && x >= btnPwrXmin && x <= btnPwrXmax)
			{
				if(_event & PANEL_MOUSE_LBDOWN)
				{
					sprintf_s(oapiDebugString(), 80, "MDU %s POWER ON/OFF", GetQualifiedIdentifier().c_str());
					bIsConnectedToCRTMFD = false;
					//oapiSendMFDKey(usMDUID, OAPI_KEY_ESCAPE);
					oapiSendMFDKey(MFDID, OAPI_KEY_ESCAPE);
				}
			}
			else if(y >= btnPwrYmin && y<= btnPwrYmax && x >= btnPwrXmin && x <= btnPwrXmax)
			{
				sprintf_s(oapiDebugString(), 80, "MDU %s BRIGHTNESS", GetQualifiedIdentifier().c_str());
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
						sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 1", GetQualifiedIdentifier().c_str());
						oapiProcessMFDButton (MFDID, 0, _event);
					}
				}
				else if(edgekeyClickPos >= 0.18 && edgekeyClickPos <= 0.28)
				{
					if(_event & PANEL_MOUSE_LBDOWN)
					{
						sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 2", GetQualifiedIdentifier().c_str());
						oapiProcessMFDButton (MFDID, 1, _event);
					}
				}
				else if(edgekeyClickPos >= 0.36 && edgekeyClickPos <= 0.46)
				{
					if(_event & PANEL_MOUSE_LBDOWN)
					{
						sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 3", GetQualifiedIdentifier().c_str());
						oapiProcessMFDButton (MFDID, 2, _event);
					}
				} 
				else if(edgekeyClickPos >= 0.54 && edgekeyClickPos <= 0.64)
				{
					if(_event & PANEL_MOUSE_LBDOWN)
					{
						sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 4", GetQualifiedIdentifier().c_str());
						oapiProcessMFDButton (MFDID, 3, _event);
					}
				}
				else if(edgekeyClickPos >= 0.72 && edgekeyClickPos <= 0.82)
				{
					if(_event & PANEL_MOUSE_LBDOWN)
					{
						sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 5", GetQualifiedIdentifier().c_str());
						oapiProcessMFDButton (MFDID, 4, _event);
					}
				}
				else if(edgekeyClickPos >= 0.90 && edgekeyClickPos <= 1.0)
				{
					if (_event & PANEL_MOUSE_LBDOWN) {
						t0 = oapiGetSysTime();
						sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 6 (%f)", GetQualifiedIdentifier().c_str(), t0);
						counting = true;
					} else if ((_event & PANEL_MOUSE_LBUP) && counting) {
						sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 6: SWITCH PAGE", GetQualifiedIdentifier().c_str());
						oapiSendMFDKey (MFDID, OAPI_KEY_F2);
						counting = false;
					} else if ((_event & PANEL_MOUSE_LBPRESSED) && counting && (oapiGetSysTime()-t0 >= 1.0)) {
						sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 6: SWITCH MODE", GetQualifiedIdentifier().c_str());
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

		//sprintf_s(oapiDebugString(), 255, "IDP: %d", GetDrivingIDP());
		int SimT=(int)oapiGetSimTime();
		bool flash=(SimT%2)==1;

		//draw stuff
		for(int i=0;i<51;i++) {
			for(int j=0;j<26;j++) {
				char cbuf[2];
				if(textBuffer[i][j].cSymbol>='!') {
					int x, y;
					switch(textBuffer[i][j].cAttr) {
						case dps::DEUATT_FLASHING:
							if(!flash) break;
						case dps::DEUATT_OVERBRIGHT:
							sprintf_s(cbuf, 2, "%c", textBuffer[i][j].cSymbol);
							vc::BitmapLocation(textBuffer[i][j].cSymbol, x, y);
							BitBlt(CompatibleDC, i*5, j*9, 5, 9, g_Param.DeuCharOvrBrgtBitmapDC, (int)(x*0.278), (int)(y*0.272), SRCCOPY);
							break;
						default:
							sprintf_s(cbuf, 2, "%c", textBuffer[i][j].cSymbol);
							vc::BitmapLocation(textBuffer[i][j].cSymbol, x, y);
							BitBlt(CompatibleDC, i*5, j*9, 5, 9, g_Param.DeuCharBitmapDC, (int)(x*0.278), (int)(y*0.272), SRCCOPY);
					}
				}
			}
		}
		BitBlt(hDC, 0, 0, 256, 256, CompatibleDC, 0, 0, SRCCOPY);

		// draw lines/circles
		for(unsigned int i=0;i<lines.size();i++) {
			if(lines[i].cAttr != dps::DEUATT_FLASHING || flash) {
				if(lines[i].cAttr == dps::DEUATT_OVERBRIGHT) SelectObject(hDC, hOverbrightPen);
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
			SetTextColor (hDC, RGB(0,255,216));
			SetTextAlign (hDC, TA_CENTER);
			SetBkMode (hDC, TRANSPARENT);
			const char *label;
			int x = 25;
			
			for (int bt = 0; bt < 5; bt++) {
				if (label = oapiMFDButtonLabel (MFDID, bt)) {
					TextOut (hDC, x, 23, label, strlen(label));
					x += 40;
				} else break;
			}
			TextOut (hDC, 224, 23, "PG", 2);
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
		//char cbuf[255];

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
		/*if(!prim_idp->OnPaint(this)) {
			if(STS()->ops==201) {
				if(prim_idp) {
					//if DISP is not set, show appropriate SPEC/default display
					if(prim_idp->GetDisp()==dps::MODE_UNDEFINED) {
						if(prim_idp->GetSpec()==dps::MODE_UNDEFINED) UNIVPTG();
						else if(prim_idp->GetSpec()==20) DAP_CONFIG();
						else {
							char cbuf[55];
							sprintf_s(cbuf, 55, "Spec: %d", (float)prim_idp->GetSpec());
						}
					}
					else {
						char cbuf[55];
						sprintf_s(cbuf, 55, "Disp: %d", (float)prim_idp->GetDisp());
					}
					//else UNIVPTG();
				}
				else {
					PrintToBuffer("ERROR: IDP NOT CONNECTED", 24, 0, 0, 0);
				}
			}
		}*/
		
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


	

	/*
	MDU* MDU::CreateMDU(VESSEL2* vessel, UINT aid, const VECTOR3& top_left, const VECTOR3& top_right,
		const VECTOR3& bottom_left, const VECTOR3& bottom_right)
	{
		//Create VC definitions for creating a independent MDU
		return NULL;
	}
	*/

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

	void MDU::SwitchMFDMode()
	{
	}

	void MDU::GPCMEMORY()
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
	}

	void MDU::AEPFD( HDC hDC )
	{
		char cbuf[64];
		int MM = STS()->pSimpleGPC->GetMajorMode();
		double dtmp = 0;
		double dtmp2 = 0;
		double dtmp3 = 0;
		bool btmp = false;
		POINT tri[3];
		POINT diamond[4];
		VECTOR3 vel;
		STS()->GetHorizonAirspeedVector( vel );
		vel *= MPS2FPS;
		dps::AscentGuidance *adap = static_cast<dps::AscentGuidance*> (STS()->pSimpleGPC->FindSoftware( "AscentGuidance" ));
		dps::AerojetDAP *ajdap = static_cast<dps::AerojetDAP*> (STS()->pSimpleGPC->FindSoftware( "AerojetDAP" ));

		int save = SaveDC( hDC );

		// TODO remove when finished
		TextOut( hDC, 100, 0, "A/E PFD", 7 );
		TextOut( hDC, 100, 8, "WIP", 3 );

		SelectObject( hDC, TahomaFont_h10w4 );

		////<<<< header >>>>////
		SetTextColor( hDC, CR_GRAY_DARK );
		TextOut( hDC, 209, 4, "MM:", 3 );

		char abortstr[4];
		if (0) sprintf_s( abortstr, 4, "R  " );// RTLS
		else if (0) sprintf_s( abortstr, 4, "T  " );// TAL
		else if (0) sprintf_s( abortstr, 4, "ATO" );// ATO
		else if (0) sprintf_s( abortstr, 4, "AOA" );// AOA
		else if (0) sprintf_s( abortstr, 4, "CA " );// CA
		else sprintf_s( abortstr, 4, "   " );// NOM
		SetTextColor( hDC, CR_WHITE );
		sprintf_s( cbuf, 64, "%d%s", MM, abortstr );
		TextOut( hDC, 225, 4, cbuf, strlen( cbuf ) );

		switch (MM)
		{
			case 101:
			case 102:
			case 103:
			case 601:
				{
					dps::SSME_Operations *ssmeops = static_cast<dps::SSME_Operations*> (STS()->pSimpleGPC->FindSoftware( "SSME_Operations" ));
					if (ssmeops->GetMECOConfirmedFlag() == false)
					{
						SetTextColor( hDC, CR_GRAY_DARK );
						TextOut( hDC, 9, 13, "THROT:", 6 );
					
						SetTextColor( hDC, CR_WHITE );
						if (adap->GetAutoThrottleState() == true) TextOut( hDC, 40, 13, "Auto", 4 );
						else
						{
							SelectObject( hDC, YellowPen );
							SelectObject( hDC, GetStockObject( HOLLOW_BRUSH ) );
							Rectangle( hDC, 6, 13, 60, 23 );
							TextOut( hDC, 40, 13, "MAN", 3 );
						}
					}
				}
			case 104:
			case 105:
			case 106:
			case 301:
			case 302:
			case 303:
				SetTextColor( hDC, CR_GRAY_DARK );
				TextOut( hDC, 21, 4, "DAP:", 4 );
				TextOut( hDC, 208, 13, "ATT:", 4 );

				SetTextColor( hDC, CR_WHITE );
				if ((MM == 101) || (MM == 102) || (MM == 103) || (MM == 601))
				{
					if (1) TextOut( hDC, 40, 4, "Auto", 4 );// TODO get AscentDAP state
					else
					{
						SelectObject( hDC, YellowPen );
						SelectObject( hDC, GetStockObject( HOLLOW_BRUSH ) );
						Rectangle( hDC, 6, 4, 60, 14 );
						TextOut( hDC, 40, 4, "CSS", 3 );
					}
				}
				else
				{
					if (1) TextOut( hDC, 40, 4, "Auto", 4 );// TODO get TransDAP state
					else TextOut( hDC, 40, 4, "INRTL", 5 );
				}

				if (0) TextOut( hDC, 228, 13, "INRTL", 5 );// TODO
				else if (1) TextOut( hDC, 228, 13, "LVLH", 4 );
				else TextOut( hDC, 208, 228, "REF", 3 );
				break;
			case 304:
			case 305:
			case 602:
			case 603:
				SetTextColor( hDC, CR_GRAY_DARK );
				TextOut( hDC, 16, 4, "Pitch:", 6 );
				TextOut( hDC, 21, 13, "R/Y:", 4 );
				TextOut( hDC, 210, 13, "SB:", 3 );

				SetTextColor( hDC, CR_WHITE );
				if (ajdap->GetAutoPitchState() == true) TextOut( hDC, 40, 4, "Auto", 4 );
				else
				{
					if (STS()->GetMachNumber() > 1)
					{
						SelectObject( hDC, YellowPen );
						SelectObject( hDC, GetStockObject( HOLLOW_BRUSH ) );
						Rectangle( hDC, 6, 4, 60, 14 );
					}
					TextOut( hDC, 40, 4, "CSS", 3 );
				}

				if (ajdap->GetAutoRollYawState() == true) TextOut( hDC, 40, 13, "Auto", 4 );
				else
				{
					if (STS()->GetMachNumber() > 1)
					{
						SelectObject( hDC, YellowPen );
						SelectObject( hDC, GetStockObject( HOLLOW_BRUSH ) );
						Rectangle( hDC, 6, 13, 60, 23 );
					}
					TextOut( hDC, 40, 13, "CSS", 3 );
				}

				if (ajdap->GetAutoSpeedbrakeState() == true) TextOut( hDC, 225, 13, "Auto", 4 );
				else
				{
					SelectObject( hDC, YellowPen );
					SelectObject( hDC, GetStockObject( HOLLOW_BRUSH ) );
					Rectangle( hDC, 207, 13, 245, 23 );
					TextOut( hDC, 225, 13, "MAN", 3 );
				}
				break;
		}

		SelectObject( hDC, BlackBrush );
		if (MM == 101)
		{
			////<<<< left tapes >>>>////
			SelectObject( hDC, RedPen );
			Rectangle( hDC, 9, 39, 33, 155 );
			Rectangle( hDC, 9, 91, 33, 105 );
			Rectangle( hDC, 9, 159, 33, 171 );
			Rectangle( hDC, 37, 39, 60, 155 );
			POINT poly[5] = {{59,98},{53,105},{35,105},{35,91},{53,91}};// start at tip moving cw
			Polygon( hDC, poly, 5 );
			SetTextColor( hDC, CR_GRAY_DARK );
			TextOut( hDC, 12, 30, "M/V", 3 );
			TextOut( hDC, 12, 172, "KEAS", 4 );
			SelectObject( hDC, GrayDarkPen );
			::Ellipse( hDC, 46, 34, 50, 38 );
			SetPixel( hDC, 50, 34, CR_GRAY_DARK );
			SetPixel( hDC, 50, 37, CR_GRAY_DARK );

			////<<<< right tapes >>>>////
			SelectObject( hDC, RedPen );
			Rectangle( hDC, 203, 39, 227, 155 );
			Rectangle( hDC, 203, 91, 227, 105 );
			Rectangle( hDC, 230, 39, 254, 155 );
			Rectangle( hDC, 230, 91, 254, 105 );
			TextOut( hDC, 212, 30, "H", 1 );
			SetPixel( hDC, 241, 29, CR_GRAY_DARK );
			TextOut( hDC, 239, 30, "H", 1 );
		}
		else
		{
			////<<<< left tapes >>>>////
			SelectObject( hDC, WhitePen );
			Rectangle( hDC, 9, 39, 33, 155 );
			SelectObject( hDC, GrayDarkPen );
			SelectObject( hDC, BlackBrush );
			Rectangle( hDC, 9, 159, 33, 171 );

			dtmp = STS()->GetMachNumber();
			dtmp2 = sqrt( STS()->GetDynPressure() * PA2PSF ) * 17.18;
			SetTextColor( hDC, CR_GRAY_DARK );
			if (((MM == 305) || (MM == 603)) && (dtmp < 0.9))
			{
				TextOut( hDC, 12, 30, "KEAS", 4 );
				TextOut( hDC, 12, 172, "M/VR", 4 );
				Tape( hDC, 2, dtmp2 );
				Rectangle( hDC, 9, 91, 33, 105 );
				SetTextColor( hDC, CR_WHITE );
				sprintf_s( cbuf, 64, "%3.0f", dtmp2 );
				TextOut( hDC, 15, 93, cbuf, strlen( cbuf ) );
				sprintf_s( cbuf, 64, "%5.2f", dtmp );
				TextOut( hDC, 13, 160, cbuf, strlen( cbuf ) );
			}
			else
			{
				btmp = false;
				if ((MM == 103) || (MM == 104) || ((MM == 601) && (0)))// TODO PPA status
				{
					TextOut( hDC, 12, 30, "M/VI", 4 );
					VECTOR3 vi;
					STS()->GetRelativeVel( STS()->GetSurfaceRef(), vi );
					if (dtmp > 4)
					{
						dtmp = length( vi ) * MPS2FPS;
						btmp = true;
					}
				}
				else
				{
					TextOut( hDC, 12, 30, "M/VR", 4 );
					if (dtmp > 4)
					{
						dtmp = STS()->GetAirspeed() * MPS2FPS;
						btmp = true;
					}
				}
				TextOut( hDC, 12, 172, "KEAS", 4 );
				Tape( hDC, 1, dtmp );
				Rectangle( hDC, 9, 91, 33, 105 );
				SetTextColor( hDC, CR_WHITE );
				if (btmp == true)
				{
					sprintf_s( cbuf, 64, "%5.0f", dtmp );// ft
					TextOut( hDC, 11, 93, cbuf, strlen( cbuf ) );
				}
				else
				{
					sprintf_s( cbuf, 64, "%5.2f", dtmp );// Mach
					TextOut( hDC, 12, 93, cbuf, strlen( cbuf ) );
				}
				sprintf_s( cbuf, 64, "%3.0f", dtmp2 );
				TextOut( hDC, 14, 160, cbuf, strlen( cbuf ) );
			}

			::Ellipse( hDC, 46, 34, 50, 38 );
			SetPixel( hDC, 50, 34, CR_GRAY_DARK );
			SetPixel( hDC, 50, 37, CR_GRAY_DARK );
			SelectObject( hDC, WhitePen );
			Rectangle( hDC, 37, 39, 60, 155 );
			dtmp = STS()->GetAOA() * DEG;
			Tape( hDC, 3, dtmp );

			dtmp2 = STS()->GetMachNumber();
			if (dtmp2 < 3)
			{
				// max L/D (linear aprox)
				if (dtmp2 < 0.9) dtmp2 = 10.5;
				else if (dtmp2 < 1.1) dtmp2 = (dtmp2 * 15) - 3;
				else if (dtmp2 < 2) dtmp2 = (dtmp2 * 1.6667) + 11.6667;
				else dtmp2 = (dtmp2 * 2) + 11;

				dtmp2 -= dtmp;
				if (fabs( dtmp2 ) < 10)
				{
					dtmp2 *= 5.7;
					diamond[0].x = 56;// start at top moving cw
					diamond[0].y = (int)(94 - dtmp2 + 0.5);
					diamond[1].x = 60;
					diamond[1].y = (int)(98 - dtmp2 + 0.5);
					diamond[2].x = 56;
					diamond[2].y = (int)(102 - dtmp2 + 0.5);
					diamond[3].x = 52;
					diamond[3].y = (int)(98 - dtmp2 + 0.5);
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
			sprintf_s( cbuf, 64, "%6.1f", dtmp );
			TextOut( hDC, 36, 93, cbuf, strlen( cbuf ) );

			if ((((MM == 102) || (MM == 103)) && ((STS()->GetMET() < 150) && (STS()->GetAltitude() < 60960))) || (MM == 601))
			{
				Rectangle( hDC, 37, 159, 60, 171 );
				SetTextColor( hDC, CR_GRAY_DARK );
				TextOut( hDC, 41, 172, "Beta", 4 );

				dtmp = STS()->GetSlipAngle() * DEG;
				char side = 'R';
				if (dtmp < 0) side = 'L';
				SetTextColor( hDC, CR_WHITE );
				sprintf_s( cbuf, 64, "%c%04.1f", side, fabs( dtmp ) );
				TextOut( hDC, 39, 160, cbuf, strlen( cbuf ) );
			}

			////<<<< right tapes >>>>////
			SetTextColor( hDC, CR_GRAY_DARK );
			TextOut( hDC, 212, 30, "H", 1 );
			SelectObject( hDC, WhitePen );
			Rectangle( hDC, 203, 39, 227, 155 );
			dtmp = STS()->GetAltitude() * MPS2FPS;
			Tape( hDC, 4, dtmp );
			SelectObject( hDC, GrayDarkPen );
			SelectObject( hDC, BlackBrush );
			Rectangle( hDC, 203, 91, 227, 105 );

			SetTextColor( hDC, CR_WHITE );
			if (dtmp < 10000)
			{
				sprintf_s( cbuf, 64, "%4d", (int)dtmp );
				TextOut( hDC, 207, 93, cbuf, strlen( cbuf ) );
			}
			else if (dtmp < 400000)
			{
				sprintf_s( cbuf, 64, "%3d", (int)dtmp / 1000 );
				TextOut( hDC, 206, 93, cbuf, strlen( cbuf ) );
				SetTextColor( hDC, CR_GRAY_DARK );
				TextOut( hDC, 220, 93, "K", 1 );
			}
			else
			{
				dtmp = STS()->GetAltitude() / NMI2M;
				sprintf_s( cbuf, 64, "%3.0f", dtmp );
				TextOut( hDC, 206, 93, cbuf, strlen( cbuf ) );
				SetTextColor( hDC, CR_GRAY_DARK );
				TextOut( hDC, 220, 93, "M", 1 );
			}

			SetTextColor( hDC, CR_GRAY_DARK );
			SetPixel( hDC, 241, 29, CR_GRAY_DARK );
			TextOut( hDC, 239, 30, "H", 1 );
			SelectObject( hDC, WhitePen );
			Rectangle( hDC, 230, 39, 254, 155 );
			dtmp = vel.y;
			Tape( hDC, 5, dtmp );
			SelectObject( hDC, GrayDarkPen );
			SelectObject( hDC, BlackBrush );
			Rectangle( hDC, 230, 91, 254, 105 );

			SetTextColor( hDC, CR_WHITE );
			if ((dtmp < 1000) && (dtmp > -1000))
			{
				sprintf_s( cbuf, 64, "%4.0f", dtmp );
				TextOut( hDC, 233, 93, cbuf, strlen( cbuf ) );
			}
			else
			{
				int itmp = (int)dtmp;
				dtmp = itmp % 100;
				if (dtmp == 0) dtmp = (double)itmp / 1000;
				else dtmp = ((itmp - dtmp) / 1000);
				sprintf_s( cbuf, 64, "%4.1f", dtmp );
				TextOut( hDC, 231, 93, cbuf, strlen( cbuf ) );

				SetTextColor( hDC, CR_GRAY_DARK );
				TextOut( hDC, 245, 93, "K", 1 );
			}

			////<<<< bottom left (static part) >>>>////
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
		}

		////<<<< center (RPY) >>>>////
		SetTextColor( hDC, CR_GRAY_DARK );
		TextOut( hDC, 174, 25, "R", 1 );
		TextOut( hDC, 174, 32, "P", 1 );
		TextOut( hDC, 174, 39, "Y", 1 );

		SetTextColor( hDC, CR_WHITE );
		dtmp = -STS()->GetBank() * DEG;
		if (dtmp <= 0) dtmp += 360;
		sprintf_s( cbuf, 64, "%03.0f", dtmp );
		TextOut( hDC, 180, 25, cbuf, strlen( cbuf ) );

		dtmp = STS()->GetPitch() * DEG;
		if (dtmp <= 0) dtmp += 360;
		sprintf_s( cbuf, 64, "%03.0f", dtmp );
		TextOut( hDC, 180, 32, cbuf, strlen( cbuf ) );

		dtmp = STS()->GetSlipAngle() * DEG;
		if (dtmp <= 0) dtmp += 360;
		sprintf_s( cbuf, 64, "%03.0f", dtmp );
		TextOut( hDC, 180, 39, cbuf, strlen( cbuf ) );

		////<<<< center (ADI) >>>>////
		// center (122,94) r = 57
		SelectObject( hDC, WhitePen );
		::Ellipse( hDC, 65, 37, 179, 151 );
		SelectObject( hDC, GrayLightPen );
		::Ellipse( hDC, 73, 45, 171, 143 );

		// TODO draw lines
		SelectObject( hDC, TahomaFont_h7w3 );
		SetTextColor( hDC, CR_GRAY_DARK );
		TextOut( hDC, 148, 47, "33", 2 );
		TextOut( hDC, 163, 63, "30", 2 );
		TextOut( hDC, 163, 118, "24", 2 );
		TextOut( hDC, 148, 134, "21", 2 );
		TextOut( hDC, 89, 134, "15", 2 );
		TextOut( hDC, 74, 118, "12", 2 );
		TextOut( hDC, 74, 63, "06", 2 );
		TextOut( hDC, 89, 47, "03", 2 );
		SelectObject( hDC, GrayLightBrush );
		diamond[0].x = 122;
		diamond[0].y = 40;
		diamond[1].x = 124;
		diamond[1].y = 42;
		diamond[2].x = 122;
		diamond[2].y = 44;
		diamond[3].x = 120;
		diamond[3].y = 42;
		Polygon( hDC, diamond, 4 );
		diamond[0].x = 173;
		diamond[0].y = 92;
		diamond[1].x = 175;
		diamond[1].y = 94;
		diamond[2].x = 173;
		diamond[2].y = 96;
		diamond[3].x = 171;
		diamond[3].y = 94;
		Polygon( hDC, diamond, 4 );
		diamond[0].x = 122;
		diamond[0].y = 143;
		diamond[1].x = 124;
		diamond[1].y = 145;
		diamond[2].x = 122;
		diamond[2].y = 147;
		diamond[3].x = 120;
		diamond[3].y = 145;
		Polygon( hDC, diamond, 4 );
		diamond[0].x = 70;
		diamond[0].y = 92;
		diamond[1].x = 72;
		diamond[1].y = 94;
		diamond[2].x = 70;
		diamond[2].y = 96;
		diamond[3].x = 68;
		diamond[3].y = 94;
		Polygon( hDC, diamond, 4 );

		// TODO ON/OFF flag
		// TODO copy (or draw here?) "ball"

		// flight director
		SelectObject( hDC, GreenThickPen );
		Arc( hDC, 116, 88, 128, 101, 116, 94, 128, 94 );
		SelectObject( hDC, GreenPen );
		SelectObject( hDC, GreenBrush );
		Rectangle( hDC, 121, 77, 123, 111 );
		Rectangle( hDC, 105, 93, 139, 95 );

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

		SetTextColor( hDC, CR_MAGENTA );
		VECTOR3 atterr;
		dtmp = 999;// "stowed" values
		dtmp2 = 999;
		dtmp3 = 999;
		// TODO check the sign of the errors so the needles move in the right direction (they're fly-to indications)
		switch (MM)
		{
			//case 101:
			case 102:
			case 103:
			case 104:
			case 105:
			case 106:
			case 601:
			//case 201:
			//case 202:
			//case 801:
			case 301:
			case 302:
			case 303:
				if ((MM == 301) || (MM == 302) || (MM == 303)) atterr = ajdap->GetAttitudeErrors();
				else atterr = adap->GetAttitudeErrors();

				if (1)// TODO ADI ERROR sw High pos
				{
					// High
					// 10/10/10
					TextOut( hDC, 173, 64, "10", 2 );
					TextOut( hDC, 173, 118, "10", 2 );

					if (atterr.z > 10) dtmp = 10;
					else if (atterr.z < -10) dtmp = -10;
					else dtmp = atterr.z;

					if (atterr.x > 10) dtmp2 = 10;
					else if (atterr.x < -10) dtmp2 = -10;
					else dtmp2 = atterr.x;

					if (atterr.y > 10) dtmp3 = 10;
					else if (atterr.y < -10) dtmp3 = -10;
					else dtmp3 = atterr.y;

					dtmp *= 2.5;
					dtmp2 *= 2.5;
					dtmp3 *= 2.5;
				}
				else if (0)// TODO ADI ERROR sw Med pos
				{
					// Med
					// 5/5/5
					TextOut( hDC, 173, 64, "5", 1 );
					TextOut( hDC, 173, 118, "5", 1 );

					if (atterr.z > 5) dtmp = 5;
					else if (atterr.z < -5) dtmp = -5;
					else dtmp = atterr.z;

					if (atterr.x > 5) dtmp2 = 5;
					else if (atterr.x < -5) dtmp2 = -5;
					else dtmp2 = atterr.x;

					if (atterr.y > 5) dtmp3 = 5;
					else if (atterr.y < -5) dtmp3 = -5;
					else dtmp3 = atterr.y;

					dtmp *= 5;
					dtmp2 *= 5;
					dtmp3 *= 5;
				}
				else
				{
					// Low
					// 1/1/1
					TextOut( hDC, 173, 64, "1", 1 );
					TextOut( hDC, 173, 118, "1", 1 );

					if (atterr.z > 1) dtmp = 1;
					else if (atterr.z < -1) dtmp = -1;
					else dtmp = atterr.z;

					if (atterr.x > 1) dtmp2 = 1;
					else if (atterr.x < -1) dtmp2 = -1;
					else dtmp2 = atterr.x;

					if (atterr.y > 1) dtmp3 = 1;
					else if (atterr.y < -1) dtmp3 = -1;
					else dtmp3 = atterr.y;

					dtmp *= 25;
					dtmp2 *= 25;
					dtmp3 *= 25;
				}
				break;
			default:// 304, 305, 602, 603
				if ((MM == 304) || ((MM == 602) && (0)))// TODO alpha recovery & alpha transition
				{
					atterr = ajdap->GetAttitudeErrors();// TODO correct source for 602
					if (1)// TODO ADI ERROR sw High pos
					{
						// High
						// 25/5/2.5
						TextOut( hDC, 173, 64, "5", 1 );
						TextOut( hDC, 173, 118, "5", 1 );

						if (atterr.z > 25) dtmp = 25;
						else if (atterr.z < -25) dtmp = -25;
						else dtmp = atterr.z;

						if (atterr.x > 5) dtmp2 = 5;
						else if (atterr.x < -5) dtmp2 = -5;
						else dtmp2 = atterr.x;

						if (atterr.y > 2.5) dtmp3 = 2.5;
						else if (atterr.y < -2.5) dtmp3 = -2.5;
						else dtmp3 = atterr.y;

						//dtmp *= 1;
						dtmp2 *= 5;
						dtmp3 *= 10;
					}
					else if (0)// TODO ADI ERROR sw Med pos
					{
						// Med
						// 25/2/2.5
						TextOut( hDC, 173, 64, "2", 1 );
						TextOut( hDC, 173, 118, "2", 1 );

						if (atterr.z > 25) dtmp = 25;
						else if (atterr.z < -25) dtmp = -25;
						else dtmp = atterr.z;

						if (atterr.x > 2) dtmp2 = 2;
						else if (atterr.x < -2) dtmp2 = -2;
						else dtmp2 = atterr.x;

						if (atterr.y > 2.5) dtmp3 = 2.5;
						else if (atterr.y < -2.5) dtmp3 = -2.5;
						else dtmp3 = atterr.y;

						//dtmp *= 1;
						dtmp2 *= 12.5;
						dtmp3 *= 10;
					}
					else
					{
						// Low
						// 10/1/2.5
						TextOut( hDC, 173, 64, "1", 1 );
						TextOut( hDC, 173, 118, "1", 1 );

						if (atterr.z > 10) dtmp = 10;
						else if (atterr.z < -10) dtmp = -10;
						else dtmp = atterr.z;

						if (atterr.x > 1) dtmp2 = 1;
						else if (atterr.x < -1) dtmp2 = -1;
						else dtmp2 = atterr.x;

						if (atterr.y > 2.5) dtmp3 = 2.5;
						else if (atterr.y < -2.5) dtmp3 = -2.5;
						else dtmp3 = atterr.y;

						dtmp *= 2.5;
						dtmp2 *= 25;
						dtmp3 *= 10;
					}
				}
				else if ((((MM == 305) || (MM == 603)) && (ajdap->GetWOW() == false)) || ((MM == 602) && (0)))// TODO NZ hold
				{
					atterr = ajdap->GetAttitudeErrors();// TODO correct source for 603
					dtmp2 = ajdap->GetNZError();
					if (0)// TODO ADI ERROR sw High pos
					{
						// High
						// 25/1.25g/2.5
						TextOut( hDC, 173, 64, "1.25g", 5 );
						TextOut( hDC, 173, 118, "1.25g", 5 );

						if (atterr.z > 25) dtmp = 25;
						else if (atterr.z < -25) dtmp = -25;
						else dtmp = atterr.z;

						if (dtmp2 > 1.25) dtmp2 = 1.25;
						else if (dtmp2 < -1.25) dtmp2 = -1.25;

						if (atterr.y > 2.5) dtmp3 = 2.5;
						else if (atterr.y < -2.5) dtmp3 = -2.5;
						else dtmp3 = atterr.y;

						//dtmp *= 1;
						dtmp2 *= 20;
						dtmp3 *= 10;
					}
					else if (0)// TODO ADI ERROR sw Med pos
					{
						// Med
						// 25/1.25g/2.5
						TextOut( hDC, 173, 64, "1.25g", 5 );
						TextOut( hDC, 173, 118, "1.25g", 5 );

						if (atterr.z > 25) dtmp = 25;
						else if (atterr.z < -25) dtmp = -25;
						else dtmp = atterr.z;

						if (dtmp2 > 1.25) dtmp2 = 1.25;
						else if (dtmp2 < -1.25) dtmp2 = -1.25;

						if (atterr.y > 2.5) dtmp3 = 2.5;
						else if (atterr.y < -2.5) dtmp3 = -2.5;
						else dtmp3 = atterr.y;

						//dtmp *= 1;
						dtmp2 *= 20;
						dtmp3 *= 10;
					}
					else
					{
						// Low
						// 10/0.5g/2.5
						TextOut( hDC, 173, 64, "0.5g", 4 );
						TextOut( hDC, 173, 118, "0.5g", 4 );

						if (atterr.z > 10) dtmp = 10;
						else if (atterr.z < -10) dtmp = -10;
						else dtmp = atterr.z;

						if (dtmp2 > 0.5) dtmp2 = 0.5;
						else if (dtmp2 < -0.5) dtmp2 = -0.5;

						if (atterr.y > 2.5) dtmp3 = 2.5;
						else if (atterr.y < -2.5) dtmp3 = -2.5;
						else dtmp3 = atterr.y;

						dtmp *= 2.5;
						dtmp2 *= 50;
						dtmp3 *= 10;
					}
				}
				else if (((MM == 305) || (MM == 603)) && (ajdap->GetWOW() == true))
				{
					atterr = ajdap->GetAttitudeErrors();// TODO correct source for 603
					if (1)// TODO ADI ERROR sw High pos
					{
						// High
						// 20/10/2.5
						TextOut( hDC, 173, 64, "10", 2 );
						TextOut( hDC, 173, 118, "10", 2 );

						if (atterr.z > 20) dtmp = 20;
						else if (atterr.z < -20) dtmp = -20;
						else dtmp = atterr.z;

						if (atterr.x > 10) dtmp2 = 10;
						else if (atterr.x < -10) dtmp2 = -10;
						else dtmp2 = atterr.x;

						if (atterr.y > 2.5) dtmp3 = 2.5;
						else if (atterr.y < -2.5) dtmp3 = -2.5;
						else dtmp3 = atterr.y;

						dtmp *= 1.25;
						dtmp2 *= 2.5;
						dtmp3 *= 10;
					}
					else if (0)// TODO ADI ERROR sw Med pos
					{
						// Med
						// 5/5/2.5
						TextOut( hDC, 173, 64, "5", 1 );
						TextOut( hDC, 173, 118, "5", 1 );

						if (atterr.z > 5) dtmp = 5;
						else if (atterr.z < -5) dtmp = -5;
						else dtmp = atterr.z;

						if (atterr.x > 5) dtmp2 = 5;
						else if (atterr.x < -5) dtmp2 = -5;
						else dtmp2 = atterr.x;

						if (atterr.y > 2.5) dtmp3 = 2.5;
						else if (atterr.y < -2.5) dtmp3 = -2.5;
						else dtmp3 = atterr.y;

						dtmp *= 5;
						dtmp2 *= 5;
						dtmp3 *= 10;
					}
					else
					{
						// Low
						// 1/1/2.5
						TextOut( hDC, 173, 64, "1", 1 );
						TextOut( hDC, 173, 118, "1", 1 );

						if (atterr.z > 1) dtmp = 1;
						else if (atterr.z < -1) dtmp = -1;
						else dtmp = atterr.z;

						if (atterr.x > 1) dtmp2 = 1;
						else if (atterr.x < -1) dtmp2 = -1;
						else dtmp2 = atterr.x;

						if (atterr.y > 2.5) dtmp3 = 2.5;
						else if (atterr.y < -2.5) dtmp3 = -2.5;
						else dtmp3 = atterr.y;

						dtmp *= 25;
						dtmp2 *= 25;
						dtmp3 *= 10;
					}
				}
				break;
		}

		if (dtmp != 999) Rectangle( hDC, (int)(121 + dtmp), (int)(94 - sqrt( 3156 - (dtmp * dtmp) )), (int)(123 + dtmp), 77 );
		if (dtmp2 != 999) Rectangle( hDC, 139, (int)(93 + dtmp2), (int)(122 + sqrt( 3156 - (dtmp2 * dtmp2) )), (int)(95 + dtmp2) );
		if (dtmp3 != 999) Rectangle( hDC, (int)(121 + dtmp3), 111, (int)(123 + dtmp3), (int)(94 + sqrt( 3156 - (dtmp3 * dtmp3) )) );
		
		// top scale
		SetTextColor( hDC, CR_GRAY_DARK );
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
		TextOut( hDC, 120, 18, "0", 1 );

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
		TextOut( hDC, 192, 90, "0", 1 );

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
		TextOut( hDC, 120, 163, "0", 1 );

		VECTOR3 av;
		STS()->GetAngularVel( av );
		av *= DEG;
		dtmp = -av.z;// roll
		dtmp2 = -av.x;// pitch
		dtmp3 = av.y;// yaw
		SelectObject( hDC, TahomaFont_h10w4 );
		SetTextColor( hDC, CR_WHITE );
		if (0)// TODO ADI RATE sw High pos
		{
			// High
			if ((MM == 305) || (MM == 603))
			{
				TextOut( hDC, 81, 23, "5", 1 );
				TextOut( hDC, 159, 23, "5", 1 );

				TextOut( hDC, 187, 50, "5", 1 );
				TextOut( hDC, 187, 129, "5", 1 );

				TextOut( hDC, 81, 159, "5", 1 );
				TextOut( hDC, 159, 159, "5", 1 );

				if (dtmp > 5) dtmp = 5;
				else if (dtmp < -5) dtmp = -5;
				dtmp = (int)((dtmp * 7) + 122);

				if (dtmp2 > 5) dtmp2 = 5;
				else if (dtmp2 < -5) dtmp2 = -5;
				dtmp2 = (int)((dtmp2 * 7) + 94);

				if (dtmp3 > 5) dtmp3 = 5;
				else if (dtmp3 < -5) dtmp3 = -5;
				dtmp3 = (int)((dtmp3 * 7) + 122);
			}
			else
			{
				TextOut( hDC, 78, 23, "10", 2 );
				TextOut( hDC, 158, 23, "10", 2 );

				TextOut( hDC, 187, 50, "10", 2 );
				TextOut( hDC, 187, 129, "10", 2 );

				TextOut( hDC, 78, 159, "10", 2 );
				TextOut( hDC, 158, 159, "10", 2 );

				if (dtmp > 10) dtmp = 10;
				else if (dtmp < -10) dtmp = -10;
				dtmp = (int)((dtmp * 3.5) + 122);

				if (dtmp2 > 10) dtmp2 = 10;
				else if (dtmp2 < -10) dtmp2 = -10;
				dtmp2 = (int)((dtmp2 * 3.5) + 94);

				if (dtmp3 > 10) dtmp3 = 10;
				else if (dtmp3 < -10) dtmp3 = -10;
				dtmp3 = (int)((dtmp3 * 3.5) + 122);
			}
		}
		else if (1)// TODO ADI RATE sw Med pos
		{
			// Med
			if ((MM == 305) || (MM == 603))
			{
				if ((STS()->GetAltitude() * MPS2FPS) < 6400)// HACK should use a 3sec timer starting at 7Kft, which expires about 6.4Kft 
				{
					// <7k + 3s
					TextOut( hDC, 81, 23, "5", 1 );
					TextOut( hDC, 159, 23, "5", 1 );

					TextOut( hDC, 187, 50, "5", 1 );
					TextOut( hDC, 187, 129, "5", 1 );

					TextOut( hDC, 81, 159, "5", 1 );
					TextOut( hDC, 159, 159, "5", 1 );

					if (dtmp > 5) dtmp = 5;
					else if (dtmp < -5) dtmp = -5;
					dtmp = (int)((dtmp * 7) + 122);

					if (dtmp2 > 5) dtmp2 = 5;
					else if (dtmp2 < -5) dtmp2 = -5;
					dtmp2 = (int)((dtmp2 * 7) + 94);

					if (dtmp3 > 5) dtmp3 = 5;
					else if (dtmp3 < -5) dtmp3 = -5;
					dtmp3 = (int)((dtmp3 * 7) + 122);
				}
				else if ((STS()->GetAltitude() * MPS2FPS) < 7000)
				{
					// <7k
					// blank
					// blank
					// blank
					dtmp = 0;
					dtmp2 = 0;
					dtmp3 = 0;
				}
				else if (ajdap->GetPrefinalState() == true)
				{
					// PRFNL - 7k
					// blank
					// alt err +/-1k
					// y-runway position err +/-1k
					TextOut( hDC, 187, 50, "1K", 2 );
					TextOut( hDC, 187, 129, "1K", 2 );

					TextOut( hDC, 78, 159, "1K", 2 );
					TextOut( hDC, 158, 159, "1K", 2 );

					dtmp = 0;
					dtmp2 = 0;// TODO get alt error (ft)
					dtmp3 = -ajdap->GetYRunwayPositionError();

					if (dtmp2 > 1000) dtmp2 = 1000;
					else if (dtmp2 < -1000) dtmp2 = -1000;
					dtmp2 = (int)((dtmp2 * 0.035) + 94);

					if (dtmp3 > 1000) dtmp3 = 1000;
					else if (dtmp3 < -1000) dtmp3 = -1000;
					dtmp3 = (int)((dtmp3 * 0.035) + 122);
				}
				else if (ajdap->GetOnHACState() == true)
				{
					// HAC
					// blank
					// alt err +/-5k
					// HAC x-range err +/-5k
					TextOut( hDC, 187, 50, "5K", 2 );
					TextOut( hDC, 187, 129, "5K", 2 );

					TextOut( hDC, 78, 159, "5K", 2 );
					TextOut( hDC, 158, 159, "5K", 2 );

					dtmp = 0;
					dtmp2 = 0;// TODO get alt error (ft)
					dtmp3 = ajdap->GetHACRadialError();

					if (dtmp2 > 5000) dtmp2 = 5000;
					else if (dtmp2 < -5000) dtmp2 = -5000;
					dtmp2 = (int)((dtmp2 * 0.007) + 94);

					if (dtmp3 > 5000) dtmp3 = 5000;
					else if (dtmp3 < -5000) dtmp3 = -5000;
					dtmp3 = (int)((dtmp3 * 0.007) + 122);
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

					dtmp = ajdap->GetTimeToHAC();
					dtmp2 = 0;// TODO get alt error (ft)
					dtmp3 = 0;// TODO get heading error (deg)

					if (dtmp > 0)
					{
						if (dtmp > 10) dtmp = 10;
						dtmp = (int)((-dtmp * 7) + 157);
					}
					else if (dtmp < 0)
					{
						if (dtmp < -10) dtmp = -10;
						dtmp = (int)((-dtmp * 7) + 87);
					}

					if (dtmp2 > 5000) dtmp2 = 5000;
					else if (dtmp2 < -5000) dtmp2 = -5000;
					dtmp2 = (int)((dtmp2 * 0.007) + 94);

					if (dtmp3 > 5) dtmp3 = 5;
					else if (dtmp3 < -5) dtmp3 = -5;
					dtmp3 = (int)((dtmp3 * 7) + 122);
				}
			}
			else
			{
				TextOut( hDC, 81, 23, "5", 1 );
				TextOut( hDC, 159, 23, "5", 1 );

				TextOut( hDC, 187, 50, "5", 1 );
				TextOut( hDC, 187, 129, "5", 1 );

				TextOut( hDC, 81, 159, "5", 1 );
				TextOut( hDC, 159, 159, "5", 1 );

				if (dtmp > 5) dtmp = 5;
				else if (dtmp < -5) dtmp = -5;
				dtmp = (int)((dtmp * 7) + 122);

				if (dtmp2 > 5) dtmp2 = 5;
				else if (dtmp2 < -5) dtmp2 = -5;
				dtmp2 = (int)((dtmp2 * 7) + 94);

				if (dtmp3 > 5) dtmp3 = 5;
				else if (dtmp3 < -5) dtmp3 = -5;
				dtmp3 = (int)((dtmp3 * 7) + 122);
			}
		}
		else
		{
			// Low
			if ((MM == 305) || (MM == 603))
			{
				TextOut( hDC, 81, 23, "5", 1 );
				TextOut( hDC, 159, 23, "5", 1 );

				TextOut( hDC, 187, 50, "5", 1 );
				TextOut( hDC, 187, 129, "5", 1 );

				TextOut( hDC, 81, 159, "5", 1 );
				TextOut( hDC, 159, 159, "5", 1 );

				if (dtmp > 5) dtmp = 5;
				else if (dtmp < -5) dtmp = -5;
				dtmp = (int)((dtmp * 7) + 122);

				if (dtmp2 > 5) dtmp2 = 5;
				else if (dtmp2 < -5) dtmp2 = -5;
				dtmp2 = (int)((dtmp2 * 7) + 94);

				if (dtmp3 > 5) dtmp3 = 5;
				else if (dtmp3 < -5) dtmp3 = -5;
				dtmp3 = (int)((dtmp3 * 7) + 122);
			}
			else
			{
				TextOut( hDC, 81, 23, "1", 1 );
				TextOut( hDC, 159, 23, "1", 1 );

				TextOut( hDC, 187, 50, "1", 1 );
				TextOut( hDC, 187, 129, "1", 1 );

				TextOut( hDC, 81, 159, "1", 1 );
				TextOut( hDC, 159, 159, "1", 1 );

				if (dtmp > 1) dtmp = 1;
				else if (dtmp < -1) dtmp = -1;
				dtmp = (int)((dtmp * 35) + 122);

				if (dtmp2 > 1) dtmp2 = 1;
				else if (dtmp2 < -1) dtmp2 = -1;
				dtmp2 = (int)((dtmp2 * 35) + 94);

				if (dtmp3 > 1) dtmp3 = 1;
				else if (dtmp3 < -1) dtmp3 = -1;
				dtmp3 = (int)((dtmp3 * 35) + 122);
			}
		}

		// draw triangles
		SelectObject( hDC, GreenPen );
		SelectObject( hDC, GreenBrush );
		if (dtmp != 0)
		{
			tri[0].x = (int)dtmp;
			tri[0].y = 27;
			tri[1].x = tri[0].x + 5;
			tri[1].y = 19;
			tri[2].x = tri[0].x - 5;
			tri[2].y = 19;
			Polygon( hDC, tri, 3 );
		}

		if (dtmp2 != 0)
		{
			tri[0].x = 190;
			tri[0].y = (int)dtmp2;
			tri[1].x = 198;
			tri[1].y = tri[0].y - 5;
			tri[2].x = 198;
			tri[2].y = tri[0].y + 5;
			Polygon( hDC, tri, 3 );
		}

		if (dtmp3 != 0)
		{
			tri[0].x = (int)dtmp3;
			tri[0].y = 163;
			tri[1].x = tri[0].x - 5;
			tri[1].y = 171;
			tri[2].x = tri[0].x + 5;
			tri[2].y = 171;
			Polygon( hDC, tri, 3 );
		}

		////<<<< bottom left (dynamic part) >>>>////
		btmp = false;
		SetTextColor( hDC, CR_GRAY_DARK );
		switch (MM)
		{
			case 102:
			case 103:
			case 601:
				{
					VECTOR3 f;
					TextOut( hDC, 43, 212, "Accel", 5 );
					STS()->GetForceVector( f );
					dtmp = (f.z / (STS()->GetMass() * G)) + sin( STS()->GetPitch() );
					btmp = true;
				}
				break;
			case 304:
			case 305:
			case 602:
			case 603:
				{
					TextOut( hDC, 43, 212, "Nz", 2 );
					VECTOR3 lift, drag, gravity;// code strait out of AerojetDAP
					STS()->GetLiftVector(lift);
					STS()->GetDragVector(drag);
					STS()->GetWeightVector(gravity);
					dtmp = (lift.y+drag.y)/length(gravity);
					btmp = true;
				}
				break;
		}
		if (btmp == true)
		{
			SetTextColor( hDC, CR_WHITE );
			sprintf_s( cbuf, 64, "%4.1f g", dtmp );
			TextOut( hDC, 43, 200, cbuf, strlen( cbuf ) );

			if (dtmp > 4) dtmp = 4;
			else if (dtmp < -1) dtmp = -1;
			dtmp = ((dtmp * 45) - 90) * RAD;
			SelectObject( hDC, GreenThickPen );
			MoveToEx( hDC, 34, 217, NULL );// center
			double xt = 34 - (21 * cos( dtmp ));
			double yt = 217 - (21 * sin( dtmp ));
			LineTo( hDC, (int)xt, (int)yt );

			SelectObject( hDC, GreenPen );
			SelectObject( hDC, GreenBrush );
			dtmp = dtmp + (90 * RAD);
			tri[0].x = (int)xt;
			tri[0].y = (int)yt;
			tri[1].x = (int)(xt - 3 * cos( dtmp ) + 6 * sin( dtmp ));
			tri[1].y = (int)(yt - 3 * sin( dtmp ) - 6 * cos( dtmp ));
			tri[2].x = (int)(xt + 3 * cos( dtmp ) + 6 * sin( dtmp ));
			tri[2].y = (int)(yt + 3 * sin( dtmp ) - 6 * cos( dtmp ));
			Polygon( hDC, tri, 3 );
		}

		////<<<< bottom right >>>>////
		if ((MM == 304) || (MM == 305))
		{
			SelectObject( hDC, GrayDarkPen );
			SelectObject( hDC, BlackBrush );
			SetTextColor( hDC, CR_GRAY_DARK );
			if (ajdap->GetOnHACState() == false)
			{
				MoveToEx( hDC, 149, 179, NULL );
				LineTo( hDC, 151, 174 );
				LineTo( hDC, 153, 179 );
				LineTo( hDC, 149, 179 );
				TextOut( hDC, 154, 172, "AZ", 2 );
				dtmp = ajdap->GetdeltaAZ();
			}
			else
			{
				// HTA ?
				TextOut( hDC, 149, 172, "HTA", 3 );
				dtmp = 0;// TODO
			}

			// TODO flashing red for limits
			Rectangle( hDC, 167, 171, 188, 183 );

			SetTextColor( hDC, CR_WHITE );
			sprintf_s( cbuf, 64, "%.0f%c", dtmp, 248 );
			TextOut( hDC, 170, 172, cbuf, strlen( cbuf ) );
		}


		if ((MM == 103) && (0))// TODO TAL
		{
			SelectObject( hDC, GrayDarkPen );
			SelectObject( hDC, BlackBrush );
			SetTextColor( hDC, CR_GRAY_DARK );
			tri[0].x = 203;
			tri[0].y = 168;
			tri[1].x = 205;
			tri[1].y = 173;
			tri[2].x = 201;
			tri[2].y = 173;
			Polygon( hDC, tri, 3 );
			TextOut( hDC, 208, 166, "X-Trk", 5 );
			Rectangle( hDC, 229, 164, 252, 176 );
			SetTextColor( hDC, CR_WHITE );
			dtmp = 0;// TODO
			sprintf_s( cbuf, 64, "%5.1f", dtmp );
			TextOut( hDC, 232, 165, cbuf, strlen( cbuf ) );
		}
		if ((MM == 102) || ((MM == 103) && ((1) || (0) || (0))))// TODO TAL and ATO
		{
			SelectObject( hDC, GrayDarkPen );
			SelectObject( hDC, BlackBrush );
			SetTextColor( hDC, CR_GRAY_DARK );
			TextOut( hDC, 207, 184, "X-Trk", 5 );
			Rectangle( hDC, 229, 182, 252, 194 );
			SetTextColor( hDC, CR_WHITE );
			dtmp = 0;// TODO
			sprintf_s( cbuf, 64, "%5.1f", dtmp );
			TextOut( hDC, 232, 183, cbuf, strlen( cbuf ) );
		}
		if ((MM == 102) || ((MM == 103) && ((1) || (0) || (0) || (0))))// TODO TAL, ATO and AOA
		{
			SelectObject( hDC, GrayDarkPen );
			SelectObject( hDC, BlackBrush );
			SetTextColor( hDC, CR_GRAY_DARK );
			tri[0].x = 205;
			tri[0].y = 205;
			tri[1].x = 207;
			tri[1].y = 210;
			tri[2].x = 203;
			tri[2].y = 210;
			Polygon( hDC, tri, 3 );
			TextOut( hDC, 210, 203, "Inc", 3 );
			Rectangle( hDC, 223, 201, 252, 213 );
			SetTextColor( hDC, CR_WHITE );
			ELEMENTS el;
			STS()->GetElements( STS()->GetGravityRef(), el, NULL, 0, FRAME_EQU );
			dtmp = fabs( STS()->pMission->GetMECOInc() - el.i ) * DEG;
			sprintf_s( cbuf, 64, "%5.2f", dtmp );
			TextOut( hDC, 226, 202, cbuf, strlen( cbuf ) );
		}
		if ((MM == 103) && (0))// TODO ATO
		{
			SelectObject( hDC, GrayDarkPen );
			SelectObject( hDC, BlackBrush );
			SetTextColor( hDC, CR_GRAY_DARK );
			TextOut( hDC, 196, 222, "Tgt Inc", 7 );
			Rectangle( hDC, 223, 220, 252, 232 );
			SetTextColor( hDC, CR_WHITE );
			dtmp = 0;// TODO
			sprintf_s( cbuf, 64, "%5.2f", dtmp );
			TextOut( hDC, 226, 221, cbuf, strlen( cbuf ) );
		}

		SelectObject( hDC, BlackBrush );
		switch (MM)
		{
			case 305:
			case 603:
				SelectObject( hDC, GrayDarkPen );
				Rectangle( hDC, 190, 231, 213, 244 );

				SetTextColor( hDC, CR_GRAY_DARK );
				TextOut( hDC, 190, 221, "HAC-C", 5 );

				SetTextColor( hDC, CR_WHITE );
				dtmp = ajdap->GetDistanceToHACCenter();
				sprintf_s( cbuf, 64, "%4.0f", dtmp );
				TextOut( hDC, 193, 233, cbuf, strlen( cbuf ) );
			case 103:
				if ((1) && (MM == 103)) break;// TODO if not TAL/ECAL/BDA break
			case 304:
			case 601:
			case 602:
				// TODO fix "colision" between this and "deltaInc" in MM103 TAL
				SelectObject( hDC, GrayDarkPen );
				Rectangle( hDC, 190, 206, 213, 219 );

				SetTextColor( hDC, CR_GRAY_DARK );
				TextOut( hDC, 189, 196, ajdap->GetSelectedRunway().c_str(), 6 );

				SetTextColor( hDC, CR_WHITE );
				dtmp = ajdap->GetRangeToRunway();
				if (dtmp > 100) sprintf_s( cbuf, 64, "%4.0f", dtmp );
				else sprintf_s( cbuf, 64, "%4.1f", dtmp );
				TextOut( hDC, 193, 207, cbuf, strlen( cbuf ) );
				break;
		}

		if ((MM == 305) || (MM == 603))
		{
			SelectObject( hDC, GrayDarkPen );
			Rectangle( hDC, 217, 165, 224, 235 );
			SelectObject( hDC, GrayDarkBrush );
			::Ellipse( hDC, 219, 166, 222, 169 );
			::Ellipse( hDC, 219, 183, 222, 186 );
			::Ellipse( hDC, 219, 215, 222, 218 );
			::Ellipse( hDC, 219, 231, 222, 234 );
			MoveToEx( hDC, 217, 200, NULL );
			LineTo( hDC, 221, 200 );

			tri[0].x = 220;// starts at tip moving cw
			tri[1].x = 228;
			tri[2].x = 228;
			dtmp = 0;// TODO get dist ABOVE glide path (ft)
			if (ajdap->GetApproachAndLandState() == false)
			{
				// TAEM
				TextOut( hDC, 216, 156, "5K", 2 );
				TextOut( hDC, 216, 234, "5K", 2 );
				if (dtmp > 5000) dtmp = 5000;
				else if (dtmp < -5000) dtmp = -5000;
				tri[0].y = (int)((dtmp * 0.0066) + 200);
				tri[1].y = tri[0].y - 5;
				tri[2].y = tri[0].y + 5;
				SelectObject( hDC, GreenPen );
				SelectObject( hDC, GreenBrush );
				Polygon( hDC, tri, 3 );
			}
			else
			{
				// A/L
				TextOut( hDC, 216, 156, "1K", 2 );
				TextOut( hDC, 216, 234, "1K", 2 );

				if ((STS()->GetAltitude() * MPS2FPS) < 1500)
				{
					// no tracking -> print GS label
					SelectObject( hDC, RedPen );
					SelectObject( hDC, RedBrush );
					Rectangle( hDC, 215, 196, 226, 204 );
					SetTextColor( hDC, CR_BLACK );
					TextOut( hDC, 216, 195, "GS", 2 );
				}
				else
				{
					if (dtmp > 1000) dtmp = 1000;
					else if (dtmp < -1000) dtmp = -1000;
					tri[0].y = (int)((dtmp * 0.0033) + 200);
					tri[1].y = tri[0].y - 5;
					tri[2].y = tri[0].y + 5;
					SelectObject( hDC, GreenPen );
					SelectObject( hDC, GreenBrush );
					Polygon( hDC, tri, 3 );
				}
			}
		}

		if ((MM == 304) || (MM == 602))
		{
			SelectObject( hDC, GrayDarkPen );
			Rectangle( hDC, 234, 168, 241, 240 );
			SelectObject( hDC, GrayDarkBrush );
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
			TextOut( hDC, 235, 159, "H", 1 );
			TextOut( hDC, 244, 165, "10", 2 );
			TextOut( hDC, 245, 200, "0", 1 );
			TextOut( hDC, 244, 235, "-10", 3 );

			tri[0].x = 237;// starts at tip moving cw
			tri[1].x = 229;
			tri[2].x = 229;
			dtmp = ajdap->GetVacc();
			if (dtmp > 10) dtmp = 10;
			else if (dtmp < -10) dtmp = -10;
			tri[0].y = (int)((-dtmp * 3.5) + 204);
				
			tri[1].y = tri[0].y + 5;
			tri[2].y = tri[0].y - 5;
			SelectObject( hDC, GreenPen );
			SelectObject( hDC, GreenBrush );
			Polygon( hDC, tri, 3 );
		}

		////<<<< bottom (HSI) >>>>////
		// center (122,238) r = 57
		SelectObject( hDC, WhitePen );
		Arc( hDC, 65, 181, 179, 295, 176, 256, 68, 256 );
		// TODO

		RestoreDC( hDC, save );
		return;
	}

	void MDU::ORBITPFD( HDC hDC )
	{
		TextOut( hDC, 100, 100, "ORBIT PFD", 9 );
		TextOut( hDC, 100, 110, "WIP", 3 );
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
		hOverbrightPen = CreatePen( PS_SOLID, 0, RGB( 255, 255, 0 ) );
		hNormalPen = CreatePen( PS_SOLID, 0, RGB( 128, 255, 0 ) );

		TahomaFont_h10w4 = CreateFont( 10, 4, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, OEM_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FIXED_PITCH, "Tahoma" );
		TahomaFont_h7w3 = CreateFont( 7, 3, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, OEM_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FIXED_PITCH, "Tahoma" );
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
		DeleteObject( hOverbrightPen );
		DeleteObject( hNormalPen );

		DeleteObject( TahomaFont_h10w4 );
		DeleteObject( TahomaFont_h7w3 );
		return;
	}

	void MDU::Tape( HDC hDC, int tapeID, double tapeVAL )
	{
		// setup
		if (hDCTapes == NULL)
		{
			hDCTapes = CreateCompatibleDC( hDC );
			HBITMAP hBM = CreateCompatibleBitmap( hDC, 113, 3700 );
			SelectObject( hDCTapes, hBM );
			SelectObject( hDCTapes, WhitePen );
			SelectObject( hDCTapes, WhiteBrush );
			Rectangle( hDCTapes, 0, 0, 113, 3700 );
			SelectObject( hDCTapes, TahomaFont_h10w4 );
			// TODO cleanup at end?

			// draw tapes
			int offset = 57;
			char cbuf[8];
			int y = 0;

			// Mach/V
			// 1K in window (1ft = 0.114px) (4-27K)
			// M0.5 in window (M1 = 228px) (0-4)
			// 2622 + 912 = 3534px + offsets
			SelectObject( hDCTapes, BlackPen );
			SelectObject( hDCTapes, BlackBrush );
			Rectangle( hDCTapes, 0, 3534 + offset, 22, 3534 + offset + offset );

			SetTextColor( hDCTapes, CR_BLACK );
			SetBkMode( hDCTapes, TRANSPARENT );
			SelectObject( hDCTapes, BlackPen );
			for (int i = 270; i >= 42; i -= 1)
			{
				y = (int)(((270 - i) * 11.4) + offset + 0.5);

				if ((i % 10) == 0)
				{
					sprintf_s( cbuf, 8, "%4.1fK", (double)i / 10 );
					TextOut( hDCTapes, 2, y - 5, cbuf, strlen( cbuf ) );
				}
				else if ((i % 2) == 0)
				{
					sprintf_s( cbuf, 8, "%4.1f", (double)i / 10 );
					TextOut( hDCTapes, 4, y - 5, cbuf, strlen( cbuf ) );
				}
				else
				{
					MoveToEx( hDCTapes, 4, y, NULL );
					LineTo( hDCTapes, 18, y );
				}
			}
			for (int i = 40; i >= 0; i--)
			{
				y = (int)(((40 - i) * 22.8) + offset + 0.5) + 2622;

				if ((i % 2) == 0)
				{
					if ((i % 10) == 0)
					{
						sprintf_s( cbuf, 8, "%2.0f", (double)i / 10 );
						TextOut( hDCTapes, 6, y - 5, cbuf, strlen( cbuf ) );
					}
					else
					{
						sprintf_s( cbuf, 8, "%3.1f", (double)i / 10 );
						TextOut( hDCTapes, 6, y - 5, cbuf, strlen( cbuf ) );
					}
				}
				else
				{
					MoveToEx( hDCTapes, 4, y, NULL );
					LineTo( hDCTapes, 18, y );
				}
			}
			
			// KEAS
			// 30KEAS in window (1KEAS = 3.8px) (500-0)
			// 1900px + offsets
			SelectObject( hDCTapes, BlackPen );
			SelectObject( hDCTapes, BlackBrush );
			Rectangle( hDCTapes, 23, 1900 + offset, 44, 1900 + offset + offset );

			SetTextColor( hDCTapes, CR_BLACK );
			SetBkMode( hDCTapes, TRANSPARENT );
			SelectObject( hDCTapes, BlackPen );
			for (int i = 500; i >= 0; i -= 5)
			{
				y = (int)(((500 - i) * 3.8) + offset + 0.5);

				if ((i % 10) == 0)
				{
					sprintf_s( cbuf, 8, "%3d", i );
					TextOut( hDCTapes, 28, y - 5, cbuf, strlen( cbuf ) );
				}
				else
				{
					MoveToEx( hDCTapes, 27, y, NULL );
					LineTo( hDCTapes, 41, y );
				}
			}

			// alpha
			// 20º in window (1º = 5.7px)
			// 2052px long + offsets
			SelectObject( hDCTapes, GrayLightPen );
			SelectObject( hDCTapes, GrayLightBrush );
			Rectangle( hDCTapes, 45, 1026 + offset, 67, 2052 + offset + offset );

			SetTextColor( hDCTapes, CR_BLACK );
			SetBkMode( hDCTapes, TRANSPARENT );
			SelectObject( hDCTapes, BlackPen );
			for (int i = 180; i >= -180; i--)
			{
				if (i < 0)// yes, not very efficient but the loop only runs once
				{
					SetTextColor( hDCTapes, CR_WHITE );
					SelectObject( hDCTapes, WhitePen );
				}

				y = (int)(((180 - i) * 5.7) + offset + 0.5);

				MoveToEx( hDCTapes, 62, y, NULL );
				LineTo( hDCTapes, 67, y );

				if ((i % 5) == 0)
				{
					sprintf_s( cbuf, 8, "%4d", i );
					TextOut( hDCTapes, 45, y - 5, cbuf, strlen( cbuf ) );
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
			SelectObject( hDCTapes, YellowPen );
			SelectObject( hDCTapes, YellowBrush );
			Rectangle( hDCTapes, 68, 2698 + offset, 90, 3055 + offset );

			SelectObject( hDCTapes, GrayLightPen );
			SelectObject( hDCTapes, GrayLightBrush );
			Rectangle( hDCTapes, 68, 3055 + offset, 90, 3380 + offset + offset );

			SetTextColor( hDCTapes, CR_BLACK );
			SetBkMode( hDCTapes, TRANSPARENT );
			SelectObject( hDCTapes, BlackPen );
			for (int i = 165; i >= 67; i--)
			{
				y = (int)(((165 - i) * 11.4) + offset + 0.5);

				if ((i % 5) == 0)
				{
					sprintf_s( cbuf, 8, "%3dM", i );
					TextOut( hDCTapes, 70, y - 5, cbuf, strlen( cbuf ) );
				}
				else
				{
					MoveToEx( hDCTapes, 83, y, NULL );
					LineTo( hDCTapes, 90, y );
				}
			}
			for (int i = 400; i >= 100; i -= 5)
			{
				y = (int)(((400 - i) * 1.14) + offset + 0.5) + 1131;

				if ((i % 50) == 0)
				{
					sprintf_s( cbuf, 8, "%3dK", i );
					TextOut( hDCTapes, 71, y - 5, cbuf, strlen( cbuf ) );
				}
				else if ((i % 10) == 0)
				{
					MoveToEx( hDCTapes, 68, y, NULL );
					LineTo( hDCTapes, 79, y );
				}
			}
			for (int i = 99; i >= 30; i--)
			{
				y = (int)(((100 - i) * 9.91) + offset + 0.5) + 1131 + 342;

				if ((i % 5) == 0)
				{
					sprintf_s( cbuf, 8, "%2dK", i );
					TextOut( hDCTapes, 73, y - 5, cbuf, strlen( cbuf ) );
				}
				else
				{
					MoveToEx( hDCTapes, 72, y, NULL );
					LineTo( hDCTapes, 86, y );
				}
			}
			for (int i = 295; i > 20; i -= 5)
			{
				y = (int)(((300 - i) * 1.9) + offset + 0.5) + 1131 + 342 + 694;

				if ((i % 50) == 0)
				{
					sprintf_s( cbuf, 8, "%2.0fK", (double)i / 10 );
					TextOut( hDCTapes, 73, y - 5, cbuf, strlen( cbuf ) );
				}
				else if ((i % 10) == 0)
				{
					sprintf_s( cbuf, 8, "%2.0f", (double)i / 10 );
					TextOut( hDCTapes, 75, y - 5, cbuf, strlen( cbuf ) );
				}
				else
				{
					MoveToEx( hDCTapes, 72, y, NULL );
					LineTo( hDCTapes, 86, y );
				}
			}

			for (int i = 20; i > 2; i -= 1)
			{
				y = (int)(((20 - i) * 11.4) + offset + 0.5) + 1131 + 342 + 694 + 532;

				if ((i % 2) == 0)
				{
					sprintf_s( cbuf, 8, "%4d", i * 100 );
					TextOut( hDCTapes, 71, y - 5, cbuf, strlen( cbuf ) );
				}
				else
				{
					MoveToEx( hDCTapes, 72, y, NULL );
					LineTo( hDCTapes, 86, y );
				}
			}
			for (int i = 20; i >= 0; i -= 1)
			{
				y = (int)(((20 - i) * 7.6) + offset + 0.5) + 1131 + 342 + 694 + 532 + 205;

				if ((i % 5) == 0)
				{
					sprintf_s( cbuf, 8, "%3d", i * 10 );
					TextOut( hDCTapes, 73, y - 5, cbuf, strlen( cbuf ) );
				}
				else
				{
					MoveToEx( hDCTapes, 72, y, NULL );
					LineTo( hDCTapes, 86, y );
				}
			}

			// Hdot
			// small scale 160ft in window (1ft = 0.7125px) (800-(-800))
			// large scale 800ft in window (1ft = 0.1425px) (3000-800/(-800)-(-3000))
			// 313.5 + 1140 + 313.5 = 1767px long + offsets
			SelectObject( hDCTapes, GrayLightPen );
			SelectObject( hDCTapes, GrayLightBrush );
			Rectangle( hDCTapes, 91, 884 + offset, 113, 1767 + offset + offset );

			SetTextColor( hDCTapes, CR_BLACK );
			SetBkMode( hDCTapes, TRANSPARENT );
			SelectObject( hDCTapes, BlackPen );
			for (int i = 30; i >= 8; i--)
			{
				y = (int)(((30 - i) * 14.25) + offset + 0.5);

				if (i < 10)
				{
					sprintf_s( cbuf, 8, "%4d", i * 100 );
					TextOut( hDCTapes, 94, y - 5, cbuf, strlen( cbuf ) );
				}
				else if ((i % 5) == 0)
				{
					sprintf_s( cbuf, 8, "%3.1fK", (double)i / 10 );
					TextOut( hDCTapes, 93, y - 5, cbuf, strlen( cbuf ) );
				}
				else
				{
					MoveToEx( hDCTapes, 95, y, NULL );
					LineTo( hDCTapes, 109, y );
				}
			}
			for (int i = 79; i >= 0; i--)
			{
				y = (int)(((80 - i) * 7.125) + 313.5 + offset + 0.5);

				if ((i % 2) == 0)
				{
					sprintf_s( cbuf, 8, "%3d", i * 10 );
					TextOut( hDCTapes, 96, y - 5, cbuf, strlen( cbuf ) );
				}
				else
				{
					MoveToEx( hDCTapes, 96, y, NULL );
					LineTo( hDCTapes, 108, y );
				}
			}
			SetTextColor( hDCTapes, CR_WHITE );
			SelectObject( hDCTapes, WhitePen );
			for (int i = -1; i >= -79; i--)
			{
				y = (int)(((80 - i) * 7.125) + 313.5 + offset + 0.5);

				if ((i % 2) == 0)
				{
					sprintf_s( cbuf, 8, "%4d", i * 10 );
					TextOut( hDCTapes, 94, y - 5, cbuf, strlen( cbuf ) );
				}
				else
				{
					MoveToEx( hDCTapes, 96, y, NULL );
					LineTo( hDCTapes, 108, y );
				}
			}
			for (int i = -8; i >= -30; i--)
			{
				y = (int)(((-8 - i) * 14.25) + 313.5 + 1140 + offset + 0.5);

				if (i > -10)
				{
					sprintf_s( cbuf, 8, "%4d", i * 100 );
					TextOut( hDCTapes, 94, y - 5, cbuf, strlen( cbuf ) );
				}
				else if ((i % 5) == 0)
				{
					sprintf_s( cbuf, 8, "%4.1fK", (double)i / 10 );
					TextOut( hDCTapes, 93, y - 5, cbuf, strlen( cbuf ) );
				}
				else
				{
					MoveToEx( hDCTapes, 95, y, NULL );
					LineTo( hDCTapes, 109, y );
				}
			}
		}

		// copy to display
		switch (tapeID)
		{
			case 1:// Mach/V
				{
					int pos;
					if (tapeVAL <= 4)
					{
						pos = (int)(((4 - tapeVAL) * 228) + 0.5) + 2622;
					}
					else
					{
						pos = (int)(((27000 - tapeVAL) * 0.114) + 0.5);
					}

					BitBlt( hDC, 10, 40, 22, 114, hDCTapes, 0, pos, SRCCOPY );
				}
				break;
			case 2:// KEAS
				if (tapeVAL > 500) tapeVAL = 500;
				BitBlt( hDC, 10, 40, 22, 114, hDCTapes, 23, (int)(((500 - tapeVAL) * 3.8) + 0.5), SRCCOPY );
				break;
			case 3:// alpha
				BitBlt( hDC, 38, 40, 21, 114, hDCTapes, 45, (int)(((180 - tapeVAL) * 5.7) + 0.5), SRCCOPY );
				break;
			case 4:// H
				{
					int pos;
					if (tapeVAL > 400000)
					{
						tapeVAL *= F2NM;
						if (tapeVAL > 165) tapeVAL = 165;
						pos = (int)(((165 - tapeVAL) * 11.4) + 0.5);
					}
					else if (tapeVAL > 100000)
					{
						pos = (int)(((400000 - tapeVAL) * 0.00114) + 0.5) + 1131;
					}
					else if (tapeVAL > 30000)
					{
						pos = (int)(((100000 - tapeVAL) * 0.00991) + 0.5) + 1131 + 342;
					}
					else if (tapeVAL > 2000)
					{
						pos = (int)(((30000 - tapeVAL) * 0.019) + 0.5) + 1131 + 342 + 694;
					}
					else if (tapeVAL > 200)
					{
						pos = (int)(((2000 - tapeVAL) * 0.114) + 0.5) + 1131 + 342 + 694 + 532;
					}
					else if (tapeVAL > 0)
					{
						pos = (int)(((200 - tapeVAL) * 0.76) + 0.5) + 1131 + 342 + 694 + 532 + 205;
					}
					else if (tapeVAL > -100)
					{
						// TODO
					}
					else if (tapeVAL > -1100)
					{
						// TODO
					}
					else
					{
						if (tapeVAL < -1100) tapeVAL = -1100;
					}

					BitBlt( hDC, 204, 40, 22, 114, hDCTapes, 68, pos, SRCCOPY );
				}
				break;
			case 5:// Hdot
				{
					int pos;
					if (tapeVAL > 800)
					{
						if (tapeVAL > 3000) tapeVAL = 3000;
						pos = (int)(((3000 - tapeVAL) * 0.1425) + 0.5);
					}
					else if (tapeVAL > -800)
					{
						pos = (int)(((800 - tapeVAL) * 0.7125) + 0.5 + 313.5);
					}
					else
					{
						if (tapeVAL < -3000) tapeVAL = -3000;
						pos = (int)(((-800 - tapeVAL) * 0.1425) + 0.5 + 313.5 + 1140);
					}
					BitBlt( hDC, 231, 40, 22, 114, hDCTapes, 91, pos, SRCCOPY );
				}
				break;
		}
		return;
	}
};
