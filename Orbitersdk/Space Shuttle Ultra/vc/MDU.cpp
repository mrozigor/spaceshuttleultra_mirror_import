#include "MDU.h"

namespace vc {

	MDU::MDU(Atlantis* _sts, const string& _ident, bool _bUseCRTMFD)
		: BasicVCComponent(_sts, _ident), bUseCRTMFD(_bUseCRTMFD)
	{
		//Clear text buffer
		//Create display buffer
		//Clear display buffer
	}

	MDU::~MDU()
	{
	}

	bool MDU::OnMouseEvent(int _event, float x, float y)
	{
		sprintf_s(oapiDebugString(), 80, "MDU %s mouse event %d (%f, %f)", GetQualifiedIdentifier().c_str(), _event, x, y);
		return true;
	}

	char* MDU::GetEdgeKeyMenu(int iButton)
	{
		return NULL;
	}

	bool MDU::Paint(HDC hdc)
	{
		return false;
	}

	bool MDU::PaintEdgeMenu(HDC hdc)
	{
		return false;
	}

	MDU* MDU::CreateMDU(VESSEL2* vessel, UINT aid, const VECTOR3& top_left, const VECTOR3& top_right,
		const VECTOR3& bottom_left, const VECTOR3& bottom_right)
	{
		//Create VC definitions for creating a independent MDU
		return NULL;
	}

};