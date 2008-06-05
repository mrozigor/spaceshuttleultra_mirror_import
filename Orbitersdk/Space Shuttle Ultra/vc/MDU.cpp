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
	}

	MDU::~MDU()
	{
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

	
	bool MDU::Paint(HDC hdc)
	{
		return false;
	}

	bool MDU::PaintEdgeMenu(HDC hdc)
	{
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
		oapiRegisterMFD (usMDUID, &mfdspec);
		sprintf_s(pszBuffer, 256, "MFD %s (%d) registered", GetQualifiedIdentifier().c_str(), usMDUID);
		oapiWriteLog(pszBuffer);
	}

	bool MDU::DefineVCGroup(UINT mgrp)
	{
		mfdspec.ngroup = mgrp;
		return true;
	}

	bool MDU::DefineVCTexture(SURFHANDLE tex)
	{
		return false;
	}
	
	bool MDU::IsCRTBufferEnabled() const
	{
		return false;
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