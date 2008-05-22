#if !defined (___SPACE_SHUTTLE_ULTRA_MDU_H_INCLUDED___)
#define ___SPACE_SHUTTLE_ULTRA_MDU_H_INCLUDED___

#include "vc_defs.h"
#include "BasicVCComponent.h"
#include "../dps/IDP.h"

namespace vc {

	class MDU: public BasicVCComponent
	{	
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
		bool bUseCRTMFD;
		SURFHANDLE shBackGround;
		dps::DEUCHAR textBuffer[51][26];
		dps::IDP* prim_idp;
		dps::IDP* sec_idp;
		bool bInverseX;
		bool bUseSecondaryPort;
		

		//Use a paint buffer for storing primitives?
	public:
		MDU(Atlantis* _sts, const string& _ident, bool _bUseCRTMFD = true);
		virtual ~MDU();
		//bool PrintChar(unsigned short x, unsigned short y, DEUCHAR c);
		//bool PrintString(unsigned short x, unsigned short y, char* pText, short sLength, char cAttr = DEUATT_NORMAL);
		//DEUCHAR GetTextBuffer(unsigned short x, unsigned short y) const;
		virtual char* GetEdgeKeyMenu(int iButton);
		bool SetPrimaryIDP(dps::IDP* idp);
		bool SetSecondaryIDP(dps::IDP* idp);
		virtual bool OnMouseEvent(int _event, float x, float y);
		virtual bool Paint(HDC hdc);
		virtual bool PaintEdgeMenu(HDC hdc);

		static MDU* CreateMDU(VESSEL2* vessel, UINT aid, const VECTOR3& top_left, const VECTOR3& top_right, const VECTOR3& bottom_left,
			const VECTOR3& bottom_right);
	};

};

#endif