#include "../Atlantis.h"
#include "MDU.h"

namespace vc {

	MDU::MDU(Atlantis* _sts, const string& _ident, unsigned short _usMDUID, bool _bUseCRTMFD)
		: BasicVCComponent(_sts, _ident), usMDUID(_usMDUID),
		bUseCRTMFD(_bUseCRTMFD),
		prim_idp(NULL), sec_idp(NULL), bUseSecondaryPort(false),
		bInverseX(false)
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
		mfdspec.ngroup = myGroup;
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
		sprintf_s(oapiDebugString(), 80, "MDU %s mouse event %d (%f, %f)", GetQualifiedIdentifier().c_str(), _event, x, y);
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
		mfdspec.nbt1 = 6;
		mfdspec.nbt2 = 6;
		oapiRegisterMFD (usMDUID, &mfdspec);
	}

	bool MDU::DefineVCGroup(UINT mgrp)
	{
		myGroup = mgrp;
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

};