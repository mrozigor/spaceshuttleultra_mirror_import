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
		

		//Use a paint buffer for storing primitives?
	protected:
		virtual void RegisterMFDContext();
		virtual void SwitchMFDMode();
	public:
		MDU(Atlantis* _sts, const string& _ident, unsigned short usMDUID, bool _bUseCRTMFD = true);
		virtual ~MDU();
		//bool PrintChar(unsigned short x, unsigned short y, DEUCHAR c);
		//bool PrintString(unsigned short x, unsigned short y, char* pText, short sLength, char cAttr = DEUATT_NORMAL);
		//DEUCHAR GetTextBuffer(unsigned short x, unsigned short y) const;
		virtual char* GetEdgeKeyMenuLabel(int iButton);
		bool SetPrimaryIDP(dps::IDP* idp);
		bool SetSecondaryIDP(dps::IDP* idp);
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
		virtual bool RealizeMFD();

		virtual void RegisterVC();
		virtual bool DefineVCGroup(UINT mgrp);
		/**
		 * Links to label texture, when used in conjunction with CRTMFD, expects texture 
		 * for drawing full MDU when not.
		 * @return false, if failed.
		 */
		virtual bool DefineVCTexture(SURFHANDLE tex);
		virtual bool IsCRTBufferEnabled() const;

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
	};

};
