#include "../Atlantis.h"
#include "MDU.h"

namespace vc {

	MDU::MDU(Atlantis* _sts, const string& _ident, unsigned short _usMDUID, bool _bUseCRTMFD)
		: BasicVCComponent(_sts, _ident), usMDUID(_usMDUID),
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
	}

	MDU::~MDU()
	{
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

	const string& MDU::GetEdgekeyMenu() const
	{
		return "MAIN MENU";
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

	

	bool MDU::OnMouseEvent(int _event, float x, float y)
	{
		//sprintf_s(oapiDebugString(), 80, "MDU %s mouse event %d (%f, %f)", GetQualifiedIdentifier().c_str(), _event, x, y);

		if(y >= 0.8350 && y<= 0.9144)
		{
			if( x >= 0.038 && x <= 0.099)
			{
				if(_event & PANEL_MOUSE_LBDOWN)
				{
					sprintf_s(oapiDebugString(), 80, "MDU %s POWER ON/OFF", GetQualifiedIdentifier().c_str());
					STS()->SetLastCreatedMFD(usMDUID);
					bIsConnectedToCRTMFD = false;
					oapiSendMFDKey(usMDUID, OAPI_KEY_ESCAPE);
				}
			}
			else if( x >= 0.9257 && x <= 0.9929)
			{
				sprintf_s(oapiDebugString(), 80, "MDU %s BRIGHTNESS", GetQualifiedIdentifier().c_str());
			}
		}
		else if (y >= 0.9185 && y <= 0.9601)
		{
			if( x >= 0.2237 && x <= 0.2624)
			{
				if(_event & PANEL_MOUSE_LBDOWN)
				{
					sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 1", GetQualifiedIdentifier().c_str());
					oapiProcessMFDButton (usMDUID, 0, _event);
				}
			}
			else if( x >= 0.3285 && x <= 0.3687)
			{
				if(_event & PANEL_MOUSE_LBDOWN)
				{
					sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 2", GetQualifiedIdentifier().c_str());
					oapiProcessMFDButton (usMDUID, 1, _event);
				}
			}
			else if( x >= 0.4356 && x <= 0.4724)
			{
				if(_event & PANEL_MOUSE_LBDOWN)
				{
					sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 3", GetQualifiedIdentifier().c_str());
					oapiProcessMFDButton (usMDUID, 2, _event);
				}
			} 
			else if( x >= 0.5477 && x <= 0.5821)
			{
				if(_event & PANEL_MOUSE_LBDOWN)
				{
					sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 4", GetQualifiedIdentifier().c_str());
					oapiProcessMFDButton (usMDUID, 3, _event);
				}
			}
			else if( x >= 0.6487 && x <= 0.6865)
			{
				if(_event & PANEL_MOUSE_LBDOWN)
				{
					sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 5", GetQualifiedIdentifier().c_str());
					oapiProcessMFDButton (usMDUID, 4, _event);
				}
			}
			else if( x >= 0.7558 && x <= 0.7939)
			{
				if (_event & PANEL_MOUSE_LBDOWN) {
					t0 = oapiGetSysTime();
					sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 6 (%f)", GetQualifiedIdentifier().c_str(), t0);
					counting = true;
				} else if ((_event & PANEL_MOUSE_LBUP) && counting) {
					sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 6: SWITCH PAGE", GetQualifiedIdentifier().c_str());
					oapiSendMFDKey (usMDUID, OAPI_KEY_F2);
					counting = false;
				} else if ((_event & PANEL_MOUSE_LBPRESSED) && counting && (oapiGetSysTime()-t0 >= 1.0)) {
					sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 6: SWITCH MODE", GetQualifiedIdentifier().c_str());
					bIsConnectedToCRTMFD = false;
					STS()->SetLastCreatedMFD(usMDUID);
					oapiSendMFDKey (usMDUID, OAPI_KEY_F1);
					counting = false;		
				}
			}
		}
		/*
		int bt = (int)(p.x*5.99);
		if (bt < 5) 
		else {
		  
		}
		}
		sprintf(oapiDebugString(), "MDU %d EDGE KEY %d", mfd, bt);
		*/
		return true;
	}

	
	bool MDU::Paint(HDC hDC)
	{
		//oapiWriteLog("Paint called 1");
		UpdateTextBuffer();
		//HGDIOBJ DefaultFont;
		HFONT DefaultFont;
		DefaultFont=CreateFont(13, 7, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, 
								ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
								DEFAULT_QUALITY, FIXED_PITCH|FF_MODERN, "Courier New");
		//DefaultFont=GetStockObject(ANSI_FIXED_FONT);
		if(DefaultFont==NULL) return false;

		int Save=SaveDC(hDC);
		SelectObject(hDC, DefaultFont);
		/*char cbuf[255];
		GetTextFace(hDC, 255, cbuf);
		oapiWriteLog(cbuf);*/

		//draw stuff
		for(int i=0;i<51;i++) {
			for(int j=0;j<26;j++) {
				char cbuf[2];
				if(textBuffer[i][j].cSymbol>='!') {
					sprintf_s(cbuf, 2, "%c", textBuffer[i][j].cSymbol);
					//sprintf_s(oapiDebugString(), 2, "%c", textBuffer[i][j].cSymbol);
					//oapiWriteLog(cbuf);
					TextOut(hDC, i*5, j*9, cbuf, 1);
				}
			}
		}

		//TextOut(hDC, 5, 5, "TEST", 4);
		//for(int i=0;i<26;i++) TextOut(hDC, 0, i*9, "TEST", 4);
		//TextOut(hDC, 1, 1, "ABCDEFGHIJKLMNOPQRSTUWXYZ", 26);
		/*char cbuf[255];
		for(int i=0;i<51;i++) {
			sprintf_s(cbuf, 255, "%d", i+1);
			TextOut(hDC, i*5, 1, cbuf, 1);
		}*/

		RestoreDC(hDC, Save);
		DeleteObject(DefaultFont);
		//oapiWriteLog("Paint called 2");
		return false;
	}

	bool MDU::PaintEdgeMenu(HDC hdc)
	{
		/*
		HDC hDC = oapiGetDC (surf);

  // D. Beachy: BUGFIX: if MFD powered off, cover separator lines and do not paint buttons
    if (oapiGetMFDMode(mfd) == MFD_NONE) {
        RECT r = { 0,0,255,13 };
        FillRect(hDC, &r, (HBRUSH)GetStockObject(BLACK_BRUSH));
    } else {   // MFD powered on
    HFONT pFont = (HFONT)SelectObject (hDC, g_Param.font[0]);
    SetTextColor (hDC, RGB(0,255,216));
    SetTextAlign (hDC, TA_CENTER);
    SetBkMode (hDC, TRANSPARENT);
    const char *label;
    int x = 24;

    for (int bt = 0; bt < 5; bt++) {
      if (label = oapiMFDButtonLabel (mfd, bt)) {
        TextOut (hDC, x, 1, label, strlen(label));
        x += 42;
      } else break;
    }
    TextOut (hDC, 234, 1, "PG", 2);
    SelectObject (hDC, pFont);
  }

  oapiReleaseDC (surf, hDC);
  */
		return false;
	}

	bool MDU::RealizeMFD()
	{
		RegisterMFDContext();
		return false;
	}

	void MDU::RegisterMFDContext()
	{
		/*
		mfds[i].ngroup   = mfdgrp[i];
		mfds[i].flag     = MFD_SHOWMODELABELS;
		mfds[i].nbt1     = 5;
		mfds[i].nbt2     = 0;
		mfds[i].bt_yofs  = 256/6;
		mfds[i].bt_ydist = 256/7;
		*/
		char pszBuffer[256];
		mfdspec.nbt1 = 5;
		mfdspec.nbt2 = 0;
		mfdspec.bt_yofs  = 256/6;
		mfdspec.bt_ydist = 256/7;
		STS()->SetLastCreatedMFD(usMDUID);
		bIsConnectedToCRTMFD = false;
		oapiRegisterMFD (usMDUID, &mfdspec);
		sprintf_s(pszBuffer, 256, "MFD %s (%d) registered", GetQualifiedIdentifier().c_str(), usMDUID);
		oapiWriteLog(pszBuffer);
	}

	void MDU::RegisterVC()
	{

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
		char cbuf[255];

		for(int i=0;i<51;i++) {
			for(int j=0;j<26;j++) {
				textBuffer[i][j].cSymbol=0;
			}
		}

		if(STS()->ops==201) {
			PrintToBuffer("TEST - MM 201", 13, 0, 0, 0);
			sprintf_s(cbuf, 255, "1 START TIME %.3d/%.2d:%.2d:%.2d", 
				STS()->START_TIME[0], STS()->START_TIME[1], STS()->START_TIME[2], STS()->START_TIME[3]);
			PrintToBuffer(cbuf, strlen(cbuf), 0, 2, 0);
			
			PrintToBuffer("MNVR OPTION", 11, 0, 4, 0);
			sprintf_s(cbuf, 255, "5 R %6.2f", STS()->MNVR_OPTION.data[ROLL]);
			PrintToBuffer(cbuf, strlen(cbuf), 1, 5, 0);
			sprintf_s(cbuf, 255, "6 P %6.2f", STS()->MNVR_OPTION.data[PITCH]);
			PrintToBuffer(cbuf, strlen(cbuf), 1, 6, 0);
			sprintf_s(cbuf, 255, "7 Y %6.2f", STS()->MNVR_OPTION.data[YAW]);
			PrintToBuffer(cbuf, strlen(cbuf), 1, 7, 0);

			PrintToBuffer("TRK/ROT OPTIONS", 15, 0, 9, 0);
			sprintf_s(cbuf, 255, "8 TGT ID %03d", STS()->TGT_ID);
			PrintToBuffer(cbuf, strlen(cbuf), 1, 10, 0);

			PrintToBuffer("9  RA", 5, 1, 12, 0);
			PrintToBuffer("10 DEC", 6, 1, 13, 0);
			PrintToBuffer("11 LAT", 6, 1, 14, 0);
			PrintToBuffer("12 LON", 6, 1, 15, 0);
			PrintToBuffer("13 ALT", 6, 1, 16, 0);

			sprintf_s(cbuf, 255, "14 BODY VECT %d", STS()->BODY_VECT);
			PrintToBuffer(cbuf, strlen(cbuf), 1, 18, 0);
			sprintf_s(cbuf, 255, "15 P %6.2f", STS()->P);
			PrintToBuffer(cbuf, strlen(cbuf), 1, 20, 0);
			sprintf_s(cbuf, 255, "16 Y %6.2f", STS()->Y);
			PrintToBuffer(cbuf, strlen(cbuf), 1, 21, 0);
			if(STS()->OM>=0.0) {
				sprintf_s(cbuf, 255, " 17 OM %6.2f", STS()->OM);
				PrintToBuffer(cbuf, strlen(cbuf), 1, 22, 0);
			}
			else PrintToBuffer("17 OM", 6, 1, 22, 0);

			PrintToBuffer("START MNVR 18", 13, 14, 4, 0);
			PrintToBuffer("TRK  19", 7, 20, 5, 0);
			PrintToBuffer("ROT  20", 7, 20, 6, 0);
			PrintToBuffer("CNCL  21", 8, 19, 7, 0);
			PrintToBuffer("CUR", 3, 28, 3, 0);
			PrintToBuffer("FUT", 3, 32, 3, 0);
			if(STS()->MNVR) {
				if(STS()->ManeuverinProg) PrintToBuffer("X", 1, 29, 4, 0);
				else PrintToBuffer("X", 1, 33, 4, 0);
			}
			else if(STS()->TRK) {
				if(STS()->ManeuverinProg) PrintToBuffer("X", 1, 5, 29, 0);
				else PrintToBuffer("X", 1, 5, 33, 0);
			}
			else if(STS()->ROT) {
				if(STS()->ManeuverinProg) PrintToBuffer("X", 1, 6, 29, 0);
				else PrintToBuffer("X", 1, 6, 33, 0);
			}

			PrintToBuffer("ATT MON", 7, 19, 9, 0);
			PrintToBuffer("22 MON AXIS", 11, 20, 10, 0);
			PrintToBuffer("ERR TOT 23", 10, 20, 11, 0);
			PrintToBuffer("ERR DAP 24", 10, 20, 11, 0);

			/*TextOut(hDC, 110, 144, "ROLL    PITCH    YAW", 20);
			sprintf_s(cbuf, 255, "CUR   %6.2f  %6.2f  %6.2f", DEG*STS()->CurrentAttitude.data[ROLL], DEG*STS()->CurrentAttitude.data[PITCH], DEG*STS()->CurrentAttitude.data[YAW]);
			TextOut(hDC, 60, 153, cbuf, strlen(cbuf));
			sprintf_s(cbuf, 255, "REQD  %6.2f  %6.2f  %6.2f", STS()->REQD_ATT.data[ROLL], STS()->REQD_ATT.data[PITCH], STS()->REQD_ATT.data[YAW]);
			TextOut(hDC, 60, 162, cbuf, strlen(cbuf));
			sprintf_s(cbuf, 255, "ERR  %+7.2f %+7.2f %+7.2f", STS()->PitchYawRoll.data[ROLL], STS()->PitchYawRoll.data[PITCH], STS()->PitchYawRoll.data[YAW]);
			TextOut(hDC, 60, 171, cbuf, strlen(cbuf));
			sprintf_s(cbuf, 255, "RATE %+7.3f %+7.3f %+7.3f", DEG*AngularVelocity.data[ROLL], DEG*AngularVelocity.data[PITCH], DEG*AngularVelocity.data[YAW]);
			TextOut(hDC, 60, 180, cbuf, strlen(cbuf));*/
		}
		return;
	}

	void MDU::PrintToBuffer(char* string, int length, int col, int row, char attributes)
	{
		for(int i=0;i<length;i++) {
			textBuffer[col+i][row].cSymbol=string[i];
		}
		//oapiWriteLog(string);
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
		return 0;
	}

	void MDU::SwitchMFDMode()
	{
	}

};
