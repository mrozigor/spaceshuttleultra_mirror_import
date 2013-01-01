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
		: BasicVCComponent(_sts, _ident), usMDUID(_usMDUID), MFDID(-1),
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

	

	bool MDU::OnMouseEvent(int _event, float x, float y)
	{
		//sprintf_s(oapiDebugString(), 80, "MDU %s mouse event %d (%f, %f)", GetQualifiedIdentifier().c_str(), _event, x, y);

		if(MFDID!=-1) {
			if(y >= 0.8350 && y<= 0.9144)
			{
				if( x >= 0.038 && x <= 0.099)
				{
					if(_event & PANEL_MOUSE_LBDOWN)
					{
						sprintf_s(oapiDebugString(), 80, "MDU %s POWER ON/OFF", GetQualifiedIdentifier().c_str());
						STS()->SetLastCreatedMFD(usMDUID);
						bIsConnectedToCRTMFD = false;
						//oapiSendMFDKey(usMDUID, OAPI_KEY_ESCAPE);
						oapiSendMFDKey(MFDID, OAPI_KEY_ESCAPE);
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
						oapiProcessMFDButton (MFDID, 0, _event);
					}
				}
				else if( x >= 0.3285 && x <= 0.3687)
				{
					if(_event & PANEL_MOUSE_LBDOWN)
					{
						sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 2", GetQualifiedIdentifier().c_str());
						oapiProcessMFDButton (MFDID, 1, _event);
					}
				}
				else if( x >= 0.4356 && x <= 0.4724)
				{
					if(_event & PANEL_MOUSE_LBDOWN)
					{
						sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 3", GetQualifiedIdentifier().c_str());
						oapiProcessMFDButton (MFDID, 2, _event);
					}
				} 
				else if( x >= 0.5477 && x <= 0.5821)
				{
					if(_event & PANEL_MOUSE_LBDOWN)
					{
						sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 4", GetQualifiedIdentifier().c_str());
						oapiProcessMFDButton (MFDID, 3, _event);
					}
				}
				else if( x >= 0.6487 && x <= 0.6865)
				{
					if(_event & PANEL_MOUSE_LBDOWN)
					{
						sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 5", GetQualifiedIdentifier().c_str());
						oapiProcessMFDButton (MFDID, 4, _event);
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
						oapiSendMFDKey (MFDID, OAPI_KEY_F2);
						counting = false;
					} else if ((_event & PANEL_MOUSE_LBPRESSED) && counting && (oapiGetSysTime()-t0 >= 1.0)) {
						sprintf_s(oapiDebugString(), 80, "MDU %s BUTTON 6: SWITCH MODE", GetQualifiedIdentifier().c_str());
						bIsConnectedToCRTMFD = false;
						STS()->SetLastCreatedMFD(usMDUID);
						oapiSendMFDKey (MFDID, OAPI_KEY_F1);
						counting = false;		
					}
				}
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
		HDC BitmapDC=CreateCompatibleDC(hDC);
		SelectObject(BitmapDC, g_Param.deu_characters);
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
					switch(textBuffer[i][j].cAttr) {
						case dps::DEUATT_FLASHING:
							if(!flash) break;
						default:
							sprintf_s(cbuf, 2, "%c", textBuffer[i][j].cSymbol);
							int x, y;
							vc::BitmapLocation(textBuffer[i][j].cSymbol, x, y);
							BitBlt(CompatibleDC, i*5, j*9, 5, 9, g_Param.DeuCharBitmapDC, (int)(x*0.278), (int)(y*0.272), SRCCOPY);
					}
				}
			}
		}
		BitBlt(hDC, 0, 0, 256, 256, CompatibleDC, 0, 0, SRCCOPY);

		RestoreDC(hDC, Save);
		DeleteDC(CompatibleDC);
		DeleteDC(BitmapDC);
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
		/*
		mfds[i].ngroup   = mfdgrp[i];
		mfds[i].flag     = MFD_SHOWMODELABELS;
		mfds[i].nbt1     = 5;
		mfds[i].nbt2     = 0;
		mfds[i].bt_yofs  = 256/6;
		mfds[i].bt_ydist = 256/7;
		*/
//		char pszBuffer[256];
		mfdspec.nbt1 = 5;
		mfdspec.nbt2 = 0;
		mfdspec.flag = MFD_SHOWMODELABELS;
		mfdspec.bt_yofs  = 256/6;
		mfdspec.bt_ydist = 256/7;
		STS()->SetLastCreatedMFD(usMDUID);
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

	/*void MDU::UNIVPTG()
	{
		char cbuf[255];
		//PrintToBuffer("TEST - MM 201", 13, 0, 0, 0);
		PrintToBuffer("2011/   /", 9, 1, 0, 0);
		PrintToBuffer("UNIV PTG", 8, 19, 0, 0);
		sprintf_s(cbuf, 255, "%.3d/%.2d:%.2d:%.2d", STS()->MET[0], STS()->MET[1], STS()->MET[2], STS()->MET[3]);
		PrintToBuffer(cbuf, strlen(cbuf), 38, 0, 0);

		PrintToBuffer("CUR MNVR COMPL", 14, 3, 2, 0);
		sprintf_s(cbuf, 255, "1 START TIME %.3d/%.2d:%.2d:%.2d", 
			STS()->START_TIME[0], STS()->START_TIME[1], STS()->START_TIME[2], STS()->START_TIME[3]);
		PrintToBuffer(cbuf, strlen(cbuf), 1, 2, 0);
		
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
		sprintf_s(cbuf, 255, "15 P  %6.2f", STS()->P);
		PrintToBuffer(cbuf, strlen(cbuf), 1, 20, 0);
		sprintf_s(cbuf, 255, "16 Y  %6.2f", STS()->Y);
		PrintToBuffer(cbuf, strlen(cbuf), 1, 21, 0);
		if(STS()->OM>=0.0) {
			sprintf_s(cbuf, 255, "17 OM %6.2f", STS()->OM);
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
			if(STS()->ManeuverinProg) PrintToBuffer("X", 1, 29, 5, 0);
			else PrintToBuffer("X", 1, 33, 5, 0);
		}
		else if(STS()->ROT) {
			if(STS()->ManeuverinProg) PrintToBuffer("X", 1, 29, 6, 0);
			else PrintToBuffer("X", 1, 33, 6, 0);
		}

		PrintToBuffer("ATT MON", 7, 19, 9, 0);
		PrintToBuffer("22 MON AXIS", 11, 20, 10, 0);
		PrintToBuffer("ERR TOT 23", 10, 20, 11, 0);
		PrintToBuffer("ERR DAP 24", 10, 20, 11, 0);

		PrintToBuffer("ROLL    PITCH    YAW", 20, 26, 14, 0);
		sprintf_s(cbuf, 255, "CUR   %6.2f  %6.2f  %6.2f", DEG*STS()->CurrentAttitude.data[ROLL], DEG*STS()->CurrentAttitude.data[PITCH], DEG*STS()->CurrentAttitude.data[YAW]);
		PrintToBuffer(cbuf, strlen(cbuf), 19, 15, 0);
		sprintf_s(cbuf, 255, "REQD  %6.2f  %6.2f  %6.2f", STS()->REQD_ATT.data[ROLL], STS()->REQD_ATT.data[PITCH], STS()->REQD_ATT.data[YAW]);
		PrintToBuffer(cbuf, strlen(cbuf), 19, 16, 0);
		sprintf_s(cbuf, 255, "ERR  %+7.2f %+7.2f %+7.2f", STS()->PitchYawRoll.data[ROLL], STS()->PitchYawRoll.data[PITCH], STS()->PitchYawRoll.data[YAW]);
		PrintToBuffer(cbuf, strlen(cbuf), 19, 17, 0);
		sprintf_s(cbuf, 255, "RATE %+7.3f %+7.3f %+7.3f", DEG*STS()->AngularVelocity.data[ROLL], DEG*STS()->AngularVelocity.data[PITCH], DEG*STS()->AngularVelocity.data[YAW]);
		PrintToBuffer(cbuf, strlen(cbuf), 19, 18, 0);
	}*/

	/*void MDU::MNVR()
	{
		int minutes, seconds;
		int timeDiff;
		int TIMER[4];
		int TGO[2];
		char cbuf[255];

		switch(STS()->ops) {
		case 104:
			DrawCommonHeader("OMS 1 MNVR EXEC");
			if((STS()->oparam.PeT)<(STS()->oparam.ApT)) {
				minutes=(int)(STS()->oparam.PeT/60);
				seconds=(int)(STS()->oparam.PeT-(60*minutes));
				sprintf_s(cbuf, 255, "TTP %.2d:%.2d", minutes, seconds); 
				PrintToBuffer(cbuf, strlen(cbuf), 20, 9, 0);
				//TextOut(hDC, 102, 117, cbuf, strlen(cbuf));
				//sprintf(oapiDebugString(), "%f %f", sts->oparam.PeT, sts->oparam.ApT);
				//sprintf(oapiDebugString(), "OPARAM %f %f", sts->oparam.PeT, sts->oparam.SMi);
			}
			else {
				minutes=(int)(STS()->oparam.ApT/60);
				seconds=(int)(STS()->oparam.ApT-(60*minutes));
				sprintf(cbuf, "TTA %.2d:%.2d", minutes, seconds);
				PrintToBuffer(cbuf, strlen(cbuf), 20, 9, 0);
				//TextOut(hDC, 102, 117, cbuf, strlen(cbuf));
			}
			break;
		case 105:
			DrawCommonHeader("OMS 2 MNVR EXEC");
			if((STS()->oparam.PeT)<(STS()->oparam.ApT)) {
				minutes=(int)(STS()->oparam.PeT/60);
				seconds=(int)(STS()->oparam.PeT-(60*minutes));
				sprintf(cbuf, "TTP %.2d:%.2d", minutes, seconds); 
				PrintToBuffer(cbuf, strlen(cbuf), 20, 9, 0);
				//sprintf(oapiDebugString(), "%f %f", sts->oparam.PeT, sts->oparam.ApT);
				//sprintf(oapiDebugString(), "OPARAM %f %f", sts->oparam.PeT, sts->oparam.SMi);
			}
			else {
				minutes=(int)(STS()->oparam.ApT/60);
				seconds=(int)(STS()->oparam.ApT-(60*minutes));
				sprintf(cbuf, "TTA %.2d:%.2d", minutes, seconds);
				PrintToBuffer(cbuf, strlen(cbuf), 20, 9, 0);
			}
			break;
		case 106:
			DrawCommonHeader("OMS 2 MNVR COAST");
			if((STS()->oparam.PeT)<(STS()->oparam.ApT)) {
				minutes=(int)(STS()->oparam.PeT/60);
				seconds=(int)(STS()->oparam.PeT-(60*minutes));
				sprintf(cbuf, "TTP %.2d:%.2d", minutes, seconds);
				PrintToBuffer(cbuf, strlen(cbuf), 20, 9, 0);
				//sprintf(oapiDebugString(), "%f %f", sts->oparam.PeT, sts->oparam.ApT);
				//sprintf(oapiDebugString(), "OPARAM %f %f", sts->oparam.PeT, sts->oparam.SMi);
			}
			else {
				minutes=(int)(STS()->oparam.ApT/60);
				seconds=(int)(STS()->oparam.ApT-(60*minutes));
				sprintf(cbuf, "TTA %.2d:%.2d", minutes, seconds);
				PrintToBuffer(cbuf, strlen(cbuf), 20, 9, 0);
			}
			break;
		case 202:
			DrawCommonHeader("ORBIT MNVR EXEC");
			if((STS()->oparam.PeT)<(STS()->oparam.ApT)) {
				minutes=(int)(STS()->oparam.PeT/60);
				seconds=(int)(STS()->oparam.PeT-(60*minutes));
				sprintf(cbuf, "TTP %.2d:%.2d", minutes, seconds); 
				PrintToBuffer(cbuf, strlen(cbuf), 20, 9, 0);
				//sprintf(oapiDebugString(), "%f %f", sts->oparam.PeT, sts->oparam.ApT);
				//sprintf(oapiDebugString(), "OPARAM %f %f", sts->oparam.PeT, sts->oparam.SMi);
			}
			else {
				minutes=(int)(STS()->oparam.ApT/60);
				seconds=(int)(STS()->oparam.ApT-(60*minutes));
				sprintf(cbuf, "TTA %.2d:%.2d", minutes, seconds); 
				PrintToBuffer(cbuf, strlen(cbuf), 20, 9, 0);
			}
			break;
		case 301:
			DrawCommonHeader("DEORB MNVR COAST");
			if((STS()->oparam.PeT)<(STS()->oparam.ApT)) { // should show REI
				minutes=(int)(STS()->oparam.PeT/60);
				seconds=(int)(STS()->oparam.PeT-(60*minutes));
				sprintf(cbuf, "TTP %.2d:%.2d", minutes, seconds);
				PrintToBuffer(cbuf, strlen(cbuf), 20, 9, 0);
				//sprintf(oapiDebugString(), "%f %f", sts->oparam.PeT, sts->oparam.ApT);
				//sprintf(oapiDebugString(), "OPARAM %f %f", sts->oparam.PeT, sts->oparam.SMi);
			}
			else {
				minutes=(int)(STS()->oparam.ApT/60);
				seconds=(int)(STS()->oparam.ApT-(60*minutes));
				sprintf(cbuf, "TTA %.2d:%.2d", minutes, seconds);
				PrintToBuffer(cbuf, strlen(cbuf), 20, 9, 0);
			}
			break;
		case 302:
			DrawCommonHeader("DEORB MNVR EXEC");
			if((STS()->oparam.PeT)<(STS()->oparam.ApT)) { // should show REI
				minutes=(int)(STS()->oparam.PeT/60);
				seconds=(int)(STS()->oparam.PeT-(60*minutes));
				sprintf(cbuf, "TTP %.2d:%.2d", minutes, seconds);
				PrintToBuffer(cbuf, strlen(cbuf), 20, 9, 0);
				//sprintf(oapiDebugString(), "%f %f", sts->oparam.PeT, sts->oparam.ApT);
				//sprintf(oapiDebugString(), "OPARAM %f %f", sts->oparam.PeT, sts->oparam.SMi);
			}
			else {
				minutes=(int)(STS()->oparam.ApT/60);
				seconds=(int)(STS()->oparam.ApT-(60*minutes));
				sprintf(cbuf, "TTA %.2d:%.2d", minutes, seconds);
				PrintToBuffer(cbuf, strlen(cbuf), 20, 9, 0);
			}
			break;
		case 303:
			DrawCommonHeader("DEORB MNVR COAST");
			if((STS()->oparam.PeT)<(STS()->oparam.ApT)) { // should show REI/TFF
				minutes=(int)(STS()->oparam.PeT/60);
				seconds=(int)(STS()->oparam.PeT-(60*minutes));
				sprintf(cbuf, "TTP %.2d:%.2d", minutes, seconds);
				PrintToBuffer(cbuf, strlen(cbuf), 20, 9, 0);
				//sprintf(oapiDebugString(), "%f %f", sts->oparam.PeT, sts->oparam.ApT);
				//sprintf(oapiDebugString(), "OPARAM %f %f", sts->oparam.PeT, sts->oparam.SMi);
			}
			else {
				minutes=(int)(STS()->oparam.ApT/60);
				seconds=(int)(STS()->oparam.ApT-(60*minutes));
				sprintf(cbuf, "TTA %.2d:%.2d", minutes, seconds);
				PrintToBuffer(cbuf, strlen(cbuf), 20, 9, 0);
			}
			break;
		}

		timeDiff=(int)(STS()->tig-STS()->met+1);
		if(true) { //for the moment, timer will always be drawn; this will change next version
			TIMER[0]=timeDiff/86400;
			TIMER[1]=(timeDiff-TIMER[0]*86400)/3600;
			TIMER[2]=(timeDiff-TIMER[0]*86400-TIMER[1]*3600)/60;
			TIMER[3]=timeDiff-TIMER[0]*86400-TIMER[1]*3600-TIMER[2]*60;
			sprintf(cbuf, "%.3d/%.2d:%.2d:%.2d", abs(TIMER[0]), abs(TIMER[1]), abs(TIMER[2]), abs(TIMER[3]));
			PrintToBuffer(cbuf, strlen(cbuf), 38, 1, 0);
		}

		PrintToBuffer("OMS BOTH 1", 10, 1, 1, 0);
		PrintToBuffer("L 2", 3, 8, 2, 0);
		PrintToBuffer("R 3", 3, 8, 3, 0);
		PrintToBuffer("RCS SEL  4", 10, 1, 4, 0);
		PrintToBuffer("*", 1, 11, STS()->OMS+1, 0);

		sprintf(cbuf, "5 TV ROLL %d", (int)(STS()->TV_ROLL));
		PrintToBuffer(cbuf, strlen(cbuf), 1, 5, 0);
		PrintToBuffer("TRIM LOAD", 9, 1, 6, 0);
		sprintf(cbuf, "6 P  %+2.1f", STS()->Trim.data[0]);
		PrintToBuffer(cbuf, strlen(cbuf), 2, 7, 0);
		sprintf(cbuf, "7 LY %+2.1f", STS()->Trim.data[1]);
		PrintToBuffer(cbuf, strlen(cbuf), 2, 8, 0);
		sprintf(cbuf, "8 RY %+2.1f", STS()->Trim.data[2]);
		PrintToBuffer(cbuf, strlen(cbuf), 2, 9, 0);
		sprintf(cbuf, "9 WT %6.0f", STS()->WT);
		PrintToBuffer(cbuf, strlen(cbuf), 1, 10, 0);
		PrintToBuffer("10 TIG", 6, 0, 11, 0);
		sprintf(cbuf, "%03.0f/%02.0f:%02.0f:%04.1f", STS()->TIG[0], STS()->TIG[1], STS()->TIG[2], STS()->TIG[3]);
		PrintToBuffer(cbuf, strlen(cbuf), 3, 12, 0);

		PrintToBuffer("TGT PEG 4", 9, 0, 13, 0);
		PrintToBuffer("14 C1", 5, 1, 14, 0);
		PrintToBuffer("15 C2", 5, 1, 15, 0);
		PrintToBuffer("16 HT", 5, 1, 16, 0);
		PrintToBuffer("17  T", 5, 1, 17, 0); //theta symbol should be before T
		//TextOut(hDC, 0, 153, " 17  T", 6);
		//Ellipse(hDC, 28, 156, 34, 165);
		//MoveToEx(hDC, 28, 160, NULL);
		//LineTo(hDC, 34, 160);
		PrintToBuffer("18 PRPLT", 5, 1, 18, 0);

		PrintToBuffer("TGT PEG 7", 9, 0, 19, 0);
		PrintToBuffer("19  VX", 6, 1, 20, 0);
		PrintToBuffer("20  VY", 6, 1, 21, 0);
		PrintToBuffer("21  VZ", 6, 1, 22, 0);
		//TextOut(hDC, 0, 171, "TGT PEG 7", 9);
		//TextOut(hDC, 0, 180, " 19  VX", 7);
		//DrawDelta(hDC, 30, 184, 27, 33, 190);
		//TextOut(hDC, 0, 189, " 20  VY", 7);
		//DrawDelta(hDC, 30, 193, 27, 33, 199);
		//TextOut(hDC, 0, 198, " 21  VZ", 7);
		//DrawDelta(hDC, 30, 202, 27, 33, 208);
		if(STS()->PEG7.x!=0.0 || STS()->PEG7.y!=0.0 || STS()->PEG7.z!=0.0) {
			sprintf(cbuf, "%+7.1f", STS()->PEG7.x);
			PrintToBuffer(cbuf, strlen(cbuf), 8, 20, 0);
			sprintf(cbuf, "%+6.1f", STS()->PEG7.y);
			PrintToBuffer(cbuf, strlen(cbuf), 8, 21, 0);
			sprintf(cbuf, "%+6.1f", STS()->PEG7.z);
			PrintToBuffer(cbuf, strlen(cbuf), 8, 22, 0);
		}

		if(STS()->MNVRLOAD) {
			PrintToBuffer("LOAD 22/TIMER 23", 16, 0, 23, 0);
			sprintf(cbuf, "24 R %-3.0f", STS()->BurnAtt.data[ROLL]);
			PrintToBuffer(cbuf, strlen(cbuf), 21, 3, 0);
			sprintf(cbuf, "25 P %-3.0f", STS()->BurnAtt.data[PITCH]);
			PrintToBuffer(cbuf, strlen(cbuf), 21, 4, 0);
			sprintf(cbuf, "26 Y %-3.0f", STS()->BurnAtt.data[YAW]);
			PrintToBuffer(cbuf, strlen(cbuf), 21, 5, 0);
			if(!STS()->MnvrExecute && timeDiff<=15.0) PrintToBuffer("EXEC", 4, 46, 2, dps::DEUATT_FLASHING);
		}
		else {
			PrintToBuffer("     22/TIMER 23", 16, 0, 23, 0);
			PrintToBuffer("24 R", 4, 21, 3, 0);
			PrintToBuffer("25 P", 4, 21, 4, 0);
			PrintToBuffer("26 Y", 4, 21, 5, 0);
		}

		//MoveToEx(hDC, 98, 15, NULL);
		//LineTo(hDC, 98, 218);

		PrintToBuffer("BURN ATT", 8, 20, 2, 0);
		if(!STS()->MnvrToBurnAtt) PrintToBuffer("MNVR 27", 7, 20, 6, 0);
		else PrintToBuffer("MNVR 27*", 8, 20, 6, 0);

		PrintToBuffer("REI", 3, 20, 8, 0);
		PrintToBuffer("GMBL", 4, 25, 10, 0);
		PrintToBuffer("L", 1, 24, 11, 0);
		PrintToBuffer("R", 1, 30, 11, 0);
		sprintf(cbuf, "P %+02.1f %+02.1f", STS()->OMSGimbal[0][0], STS()->OMSGimbal[1][0]);
		PrintToBuffer(cbuf, strlen(cbuf), 20, 12, 0);
		sprintf(cbuf, "Y %+02.1f %+02.1f", STS()->OMSGimbal[0][1], STS()->OMSGimbal[1][1]);
		PrintToBuffer(cbuf, strlen(cbuf), 20, 13, 0);

		PrintToBuffer("PRI 28   29", 11, 20, 15, 0);
		PrintToBuffer("SEC 30   31", 11, 20, 16, 0);
		PrintToBuffer("OFF 32   33", 11, 20, 17, 0);
		PrintToBuffer("GMBL CK  34", 11, 20, 18, 0);

		//MoveToEx(hDC, 156, 15, NULL);
		//LineTo(hDC, 156, 111);
		//LineTo(hDC, 250, 111);

		if(!STS()->BurnInProg && !STS()->BurnCompleted) {
			TGO[0]=(int)(STS()->BurnTime/60);
			TGO[1]=(int)(STS()->BurnTime-(TGO[0]*60));
		}
		else if(!STS()->BurnCompleted) {
			double btRemaining=STS()->IgnitionTime+STS()->BurnTime-STS()->met;
			TGO[0]=(int)btRemaining/60;
			TGO[1]=(int)btRemaining%60;
		}
		else TGO[0]=TGO[1]=0;
		sprintf(cbuf, "VTOT   %6.2f", STS()->DeltaVTot);
		PrintToBuffer(cbuf, strlen(cbuf), 37, 3, 0);
		//DrawDelta(hDC, 161, 31, 158, 164, 37);
		sprintf(cbuf, "TGO %.2d:%.2d", TGO[0], TGO[1]);
		PrintToBuffer(cbuf, strlen(cbuf), 36, 4, 0);
		sprintf(cbuf, "VGO X %+8.2f", STS()->VGO.x);
		PrintToBuffer(cbuf, strlen(cbuf), 36, 6, 0);
		sprintf(cbuf, "Y  %+7.2f", STS()->VGO.y);
		PrintToBuffer(cbuf, strlen(cbuf), 40, 7, 0);
		sprintf(cbuf, "Z  %+7.2f", STS()->VGO.z);
		PrintToBuffer(cbuf, strlen(cbuf), 40, 8, 0);
		PrintToBuffer("HA     HP", 9, 40, 10, 0);
		sprintf(cbuf, "TGT");
		PrintToBuffer(cbuf, strlen(cbuf), 36, 11, 0);
		//TextOut(hDC, 158, 90, cbuf, strlen(cbuf));
		sprintf(cbuf, "CUR");
		PrintToBuffer(cbuf, strlen(cbuf), 36, 12, 0);
		//TextOut(hDC, 158, 99, cbuf, strlen(cbuf));

		sprintf(cbuf, "35 ABORT TGT");
		PrintToBuffer(cbuf, strlen(cbuf), 35, 15, 0);
		//TextOut(hDC, 150, 126, cbuf, strlen(cbuf));

		//TextOut(hDC, 185, 135, "FWD RCS", 7);
		//TextOut(hDC, 185, 144, "  ARM  36", 9);
		//TextOut(hDC, 185, 153, "  DUMP 37", 9);
		//TextOut(hDC, 185, 162, "  OFF  38", 9);
		//TextOut(hDC, 185, 171, "SURF DRIVE", 10);
		//TextOut(hDC, 185, 180, "  ON   39", 9);
		//TextOut(hDC, 185, 189, "  OFF  40", 9);
	}*/


	/*void MDU::DAP_CONFIG()
	{
		char *strings[3]={" ALL", "NOSE", "TAIL"};
		char cbuf[255];
		int lim[3]={3, 5, 5};
		int i, n;

		DrawCommonHeader("DAP CONFIG");
		
		//TextOut(hDC, 84, 9, "1 A", 3);
		//TextOut(hDC, 149, 9, "2 B", 3);
		//TextOut(hDC, 105+65*(sts->DAPMode[0]), 9, "*", 1);
		//PrintToBuffer("*", 1, 7, 2+9*STS()->DAPMode[1], 0);
		//TextOut(hDC, 200, 9, Edit[edit], strlen(Edit[edit]));
		//TextOut(hDC, 200, 18, "8 LOAD", 6);

		PrintToBuffer("PRI", 3, 4, 2, 0);
		PrintToBuffer("1 DAP A", 7, 9, 2, 0);
		PrintToBuffer("2 DAP B", 7, 20, 2, 0);
		PrintToBuffer("PRI", 3, 33, 2, 0);
		//TextOut(hDC, 14, 27, "3 PRI", 5);
		PrintToBuffer("ROT RATE", 8, 0, 3, 0);
		//TextOut(hDC, 0, 36, "ROT RATE", 8);
		PrintToBuffer("ATT DB", 6, 0, 4, 0);
		//TextOut(hDC, 0, 45, "ATT DB", 6);
		PrintToBuffer("RATE DB", 7, 0, 5, 0);
		//TextOut(hDC, 0, 54, "RATE DB", 7);
		PrintToBuffer("ROT PLS", 7, 0, 6, 0);
		//TextOut(hDC, 0, 63, "ROT PLS", 7);
		PrintToBuffer("COMP", 4, 0, 7, 0);
		//TextOut(hDC, 0, 72, "COMP", 4);
		PrintToBuffer("P OPTION", 8, 0, 8, 0);
		//TextOut(hDC, 0, 81, "P OPTION", 8);
		PrintToBuffer("Y OPTION", 8, 0, 9, 0);
		//TextOut(hDC, 0, 90, "Y OPTION", 8);
		PrintToBuffer("TRAN PLS", 8, 0, 10, 0);
		//TextOut(hDC, 0, 99, "TRAN PLS", 8);

		PrintToBuffer("ALT", 3, 4, 11, 0);
		PrintToBuffer("ALT", 3, 33, 11, 0);
		//TextOut(hDC, 14, 108, "4 ALT", 5);
		PrintToBuffer("RATE DB", 7, 0, 12, 0);
		//TextOut(hDC, 0, 117, "RATE DB", 7);
		PrintToBuffer("JET OPT", 7, 0, 13, 0);
		//TextOut(hDC, 0, 126, "JET OPT", 7);
		PrintToBuffer("# JETS", 6, 0, 14, 0);
		//TextOut(hDC, 0, 135, "# JETS", 6);
		PrintToBuffer("ON TIME", 7, 0, 15, 0);
		//TextOut(hDC, 0, 144, "ON TIME", 7);
		PrintToBuffer("DELAY", 5, 0, 16, 0);
		//TextOut(hDC, 0, 153, "DELAY", 5);

		PrintToBuffer("VERN", 4, 4, 17, 0);
		PrintToBuffer("VERN", 4, 33, 17, 0);
		//TextOut(hDC, 14, 162, "5 VERN", 6);
		PrintToBuffer("ROT RATE", 8, 0, 18, 0);
		//TextOut(hDC, 0, 171, "ROT RATE", 8);
		PrintToBuffer("ATT DB", 6, 0, 19, 0);
		//TextOut(hDC, 0, 180, "ATT DB", 6);
		PrintToBuffer("RATE DB", 7, 0, 20, 0);
		//TextOut(hDC, 0, 189, "RATE DB", 7);
		PrintToBuffer("ROT PLS", 7, 0, 21, 0);
		//TextOut(hDC, 0, 198, "ROT PLS", 7);
		PrintToBuffer("COMP", 4, 0, 22, 0);
		//TextOut(hDC, 0, 207, "COMP", 4);
		PrintToBuffer("CNTL ACC", 8, 0, 23, 0);
		//TextOut(hDC, 0, 216, "CNTL ACC", 8);
		//if(sts->DAPMode[1]==0) TextOut(hDC, 20, 27, "*", 1);
		//else if(sts->DAPMode[1]==1) TextOut(hDC, 20, 108, "*", 1);
		//else TextOut(hDC, 20, 162, "*", 1);

		//MoveToEx(hDC, 59, 38, NULL);
		//LineTo (hDC, 59, 229);
		//MoveToEx(hDC, 124, 38, NULL);
		//LineTo (hDC, 124, 229);
		//MoveToEx(hDC, 189, 38, NULL);
		//LineTo (hDC, 189, 229);

		int edit=2; //temporary
		for(n=1, i=0;n<=lim[edit];n+=2, i++) {
			sprintf_s(cbuf, 255, "%d %.4f", 10*n, STS()->DAP[i].PRI_ROT_RATE);
			PrintToBuffer(cbuf, strlen(cbuf), 9+11*i, 3, 0);
			//TextOut(hDC, 60+65*i, 36, cbuf, strlen(cbuf));
			sprintf_s(cbuf, 255, "%d  %05.2f", 10*n+1, STS()->DAP[i].PRI_ATT_DB);
			PrintToBuffer(cbuf, strlen(cbuf), 9+11*i, 4, 0);
			//TextOut(hDC, 60+65*i, 45, cbuf, strlen(cbuf));
			sprintf_s(cbuf, 255, "%d   %.2f", 10*n+2, STS()->DAP[i].PRI_RATE_DB);
			PrintToBuffer(cbuf, strlen(cbuf), 9+11*i, 5, 0);
			//TextOut(hDC, 60+65*i, 54, cbuf, strlen(cbuf));
			sprintf_s(cbuf, 255, "%d   %.2f", 10*n+3, STS()->DAP[i].PRI_ROT_PLS);
			PrintToBuffer(cbuf, strlen(cbuf), 9+11*i, 6, 0);
			//TextOut(hDC, 60+65*i, 63, cbuf, strlen(cbuf));
			sprintf_s(cbuf, 255, "%d  %.3f", 10*n+4, STS()->DAP[i].PRI_COMP);
			PrintToBuffer(cbuf, strlen(cbuf), 9+11*i, 7, 0);
			//TextOut(hDC, 60+65*i, 72, cbuf, strlen(cbuf));
			sprintf_s(cbuf, 255, "%d   %s", 10*n+5, strings[STS()->DAP[i].PRI_P_OPTION]);
			PrintToBuffer(cbuf, strlen(cbuf), 9+11*i, 8, 0);
			//TextOut(hDC, 60+65*i, 81, cbuf, strlen(cbuf));
			sprintf_s(cbuf, 255, "%d   %s", 10*n+6, strings[STS()->DAP[i].PRI_Y_OPTION]);
			PrintToBuffer(cbuf, strlen(cbuf), 9+11*i, 9, 0);
			//TextOut(hDC, 60+65*i, 90, cbuf, strlen(cbuf));
			sprintf_s(cbuf, 255, "%d   %.2f", 10*n+7, STS()->DAP[i].PRI_TRAN_PLS);
			PrintToBuffer(cbuf, strlen(cbuf), 9+11*i, 10, 0);
			//TextOut(hDC, 60+65*i, 99, cbuf, strlen(cbuf));

			sprintf(cbuf, "%d  %.3f", 10*n+8, STS()->DAP[i].ALT_RATE_DB);
			PrintToBuffer(cbuf, strlen(cbuf), 9+11*i, 12, 0);
			//TextOut(hDC, 60+65*i, 117, cbuf, strlen(cbuf));
			sprintf(cbuf, "%d   %s", 10*n+9, strings[STS()->DAP[i].ALT_JET_OPT]);
			PrintToBuffer(cbuf, strlen(cbuf), 9+11*i, 13, 0);
			//TextOut(hDC, 60+65*i, 126, cbuf, strlen(cbuf));
			sprintf(cbuf, "%d      %d", 10*n+10, STS()->DAP[i].ALT_JETS);
			PrintToBuffer(cbuf, strlen(cbuf), 9+11*i, 14, 0);
			//TextOut(hDC, 60+65*i, 135, cbuf, strlen(cbuf));
			sprintf(cbuf, "%d   %.2f", 10*n+11, STS()->DAP[i].ALT_ON_TIME);
			PrintToBuffer(cbuf, strlen(cbuf), 9+11*i, 15, 0);
			//TextOut(hDC, 60+65*i, 144, cbuf, strlen(cbuf));
			sprintf(cbuf, "%d   %.2f", 10*n+12, STS()->DAP[i].ALT_DELAY);
			PrintToBuffer(cbuf, strlen(cbuf), 9+11*i, 16, 0);
			//TextOut(hDC, 60+65*i, 153, cbuf, strlen(cbuf));

			sprintf(cbuf, "%d %.4f", 10*n+13, STS()->DAP[i].VERN_ROT_RATE);
			PrintToBuffer(cbuf, strlen(cbuf), 9+11*i, 18, 0);
			//TextOut(hDC, 60+65*i, 171, cbuf, strlen(cbuf));
			sprintf(cbuf, "%d   %.2f", 10*n+14, STS()->DAP[i].VERN_ATT_DB);
			PrintToBuffer(cbuf, strlen(cbuf), 9+11*i, 19, 0);
			//TextOut(hDC, 60+65*i, 180, cbuf, strlen(cbuf));
			sprintf(cbuf, "%d  %.3f", 10*n+15, STS()->DAP[i].VERN_RATE_DB);
			PrintToBuffer(cbuf, strlen(cbuf), 9+11*i, 20, 0);
			//TextOut(hDC, 60+65*i, 189, cbuf, strlen(cbuf));
			sprintf(cbuf, "%d   %.2f", 10*n+16, STS()->DAP[i].VERN_ROT_PLS);
			PrintToBuffer(cbuf, strlen(cbuf), 9+11*i, 21, 0);
			//TextOut(hDC, 60+65*i, 198, cbuf, strlen(cbuf));
			sprintf(cbuf, "%d  %.3f", 10*n+17, STS()->DAP[i].VERN_COMP);
			PrintToBuffer(cbuf, strlen(cbuf), 9+11*i, 22, 0);
			//TextOut(hDC, 60+65*i, 207, cbuf, strlen(cbuf));
			sprintf(cbuf, "%d      %d", 10*n+18, STS()->DAP[i].VERN_CNTL_ACC);
			PrintToBuffer(cbuf, strlen(cbuf), 9+11*i, 23, 0);
			//TextOut(hDC, 60+65*i, 216, cbuf, strlen(cbuf));
		}
	}


	void MDU::ORBIT_TGT()
	{
		DrawCommonHeader("ORBIT TGT");
	}*/

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
};
