/****************************************************************************
  This file is part of Space Shuttle Ultra

  Multi-Function Display Unit  definition



  Space Shuttle Ultra is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Space Shuttle Ultra is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Space Shuttle Ultra; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See http://spaceshuttleultra.sourceforge.net/license/ for more details.

  **************************************************************************/
#pragma once

#include "../dps/dps_defs.h"
#include "vc_defs.h"
#include "BasicVCComponent.h"


namespace vc {


	class MDU: public BasicVCComponent
	{	
		double t0;
		bool counting;
	public:
		typedef enum {
			MDU_ADI_AVVI,
			MDU_HSI_AMI,
			MDU_COMP_ADI_HSI,
			MDU_ADI,
			MDU_OMS_MPS,
			MDU_HYD_APU,
			MDU_SPI,
			MDU_PORT_SELECT,
			MDU_DPS,
			MDU_FAULTSUMM,
			MDU_CONFIGSTATUS,
			MDU_CST,
			MDU_MEMORYMGMT
		} MDU_MODE;
	protected:
		unsigned short usMDUID;
		int MFDID;
		bool bUseCRTMFD;
		SURFHANDLE shBackGround;
		SURFHANDLE shLabelTex;
		dps::DEUCHAR textBuffer[51][26];
		dps::IDP* prim_idp;
		dps::IDP* sec_idp;
		bool bInverseX;
		bool bUseSecondaryPort;
		UINT myGroup;
		EXTMFDSPEC mfdspec;
		double fBrightness;
		bool bPower;
		bool bIsConnectedToCRTMFD;
		

		//Use a paint buffer for storing primitives?
	protected:
		virtual void RegisterMFDContext(int id);
		virtual void SwitchMFDMode();
		void DrawCommonHeader(const char* cDispTitle);
		virtual void PrintToBuffer(const char* string, int length, int col, int row, char attributes);
		
		inline void MDU::DrawDelta(HDC hDC, int TopX, int TopY, int LBottomX, int RBottomX, int BottomY)
		{
			MoveToEx(hDC, TopX, TopY, NULL);
			LineTo(hDC, LBottomX, BottomY);
			LineTo(hDC, RBottomX, BottomY);
			LineTo(hDC, TopX, TopY);
		}

	public:
		MDU(Atlantis* _sts, const string& _ident, unsigned short usMDUID, bool _bUseCRTMFD = true);
		virtual ~MDU();
		//bool PrintChar(unsigned short x, unsigned short y, DEUCHAR c);
		//bool PrintString(unsigned short x, unsigned short y, char* pText, short sLength, char cAttr = DEUATT_NORMAL);
		//DEUCHAR GetTextBuffer(unsigned short x, unsigned short y) const;
		virtual char* GetEdgeKeyMenuLabel(int iButton);
		bool SetPrimaryIDP(dps::IDP* idp);
		bool SetSecondaryIDP(dps::IDP* idp);
		inline dps::IDP* GetIDP() const {
			if(bUseSecondaryPort)
				return sec_idp;
			else
				return prim_idp;
		}
		/**
		 * Does nothing as it currently has no animations.
		 * @sa BasicVCComponent::DefineVCAnimations
		 */
		virtual void DefineVCAnimations(UINT vc_idx);
		/**
		 * handle mouse events
		 * @sa BasicVCComponent::OnMouseEvent
		 */
		virtual bool OnMouseEvent(int _event, float x, float y);

		//virtual bool OnMFDModeChange(...);
		/** 
		 * Paint the contents of the MDU on a device context.
		 */
		virtual bool Paint(HDC hdc);
		/** 
		 * Paint the edge menu area on this DC.
		 */
		virtual bool PaintEdgeMenu(HDC hdc);
		/**
		 * Register the MFD area for Orbiter. Does nothing when MFD rendering 
		 * is bypassed.
		 */
		virtual bool RealizeMFD(int id);

		virtual bool OnVCRedrawEvent(int id, int _event, SURFHANDLE surf);

		virtual void RegisterVC();
		/**
		 * define the Area ID of the MDU screen inside the VC. when in 
		 * CRTMFD mode, it paints only the edge key menu area. 
		 */
		virtual bool DefineRegionAID(UINT aid);
		virtual bool DefineVCGroup(UINT mgrp);
		/**
		 * Links to label texture, when used in conjunction with CRTMFD, expects texture 
		 * for drawing full MDU when not.
		 * @return false, if failed.
		 */
		virtual bool DefineVCTexture(SURFHANDLE tex);
		virtual bool IsCRTBufferEnabled() const;

		virtual void UpdateTextBuffer();
		

		//IDP interface functions
		/** 
		 * Act like the curses function
		 */
		inline void mvprint(int x, int y, const char* pszLine, char attributes = 0) 
		{
			//
			PrintToBuffer(pszLine, strlen(pszLine), x, y, attributes);
		}

		inline void Line(int x1, int y1, int x2, int y2, char attributes = 0) 
		{
		}

		//static MDU* CreateMDU(VESSEL2* vessel, UINT aid, const VECTOR3& top_left, const VECTOR3& top_right, const VECTOR3& bottom_left,
		//	const VECTOR3& bottom_right);

		virtual bool GetViewAngle() const;
		virtual const string& GetEdgekeyMenu() const;
		virtual short GetPortConfig() const;
		virtual bool GetSelectedPort() const;

		/**
		 * Return the identification of the IDP driving this
		 * display. 
		 * @return 0 if not connected to IDP or IDP failed
		 */
		virtual unsigned short GetDrivingIDP() const;

		/**
		 * Called by CRTMFD or compatible, when registering special functions.
		 * Disables the old MFD menu and replaces it by the MDU menus.
		 * Gets disabled when the MDU gets reset.
		 */
		virtual void ConnectToCRTMFD();

		/**
		 * Display functions
		 * Update text buffer with appropriate data for display
		 */
		void ORBIT_TGT();
		void PASSTRAJ();
		void GNCSYSSUMM1();
		void GPCMEMORY();
	};

};
