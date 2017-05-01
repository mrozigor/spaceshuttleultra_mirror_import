#include "StandardSwitchCover.h"
#include "../Atlantis.h"
#include <assert.h>

namespace vc
{
	StandardSwitchCover::StandardSwitchCover(Atlantis* _sts, const std::string& _ident)
	: BasicSwitch(_sts, 2, _ident, SWITCH_GUARD_SOUND)
	{
		anim_cover=NULL;
		pcoverrot=NULL;
		usCurrentPosition=0;
	}

	StandardSwitchCover::~StandardSwitchCover()
	{
		delete pcoverrot;
	}

	void StandardSwitchCover::DefineVCAnimations(UINT vc_idx)
	{
		if(bHasReference && bHasDirection && !bHasAnimations)
		{
			//VECTOR3 ofs = STS()->GetOrbiterCoGOffset();
			pcoverrot = new MGROUP_ROTATE(vc_idx, &grpIndex, 1, GetReference() /*+ ofs*/, GetDirection(), (float)(90 * RAD));
			anim_cover = STS()->CreateAnimation(InitialAnimState());
			STS()->AddAnimationComponent(anim_cover, 0.0, 1.0, pcoverrot, NULL);
			VerifyAnimations();
		}

		OnPositionChange(usCurrentPosition);
	}

	bool StandardSwitchCover::OnMouseEvent(int _event, float x, float y)
	{
		if(!(_event & PANEL_MOUSE_LBDOWN))
			return false;

		usCurrentPosition=1-usCurrentPosition;
		OnPositionChange(usCurrentPosition);

		return true;
	}


	void StandardSwitchCover::OnPositionChange(unsigned short usNewPosition)
	{
		BasicSwitch::OnPositionChange(usNewPosition);
		if(bHasAnimations)
		{
			SetAnimation(anim_cover, usNewPosition);
		}
		AtlantisVCComponent::SetMouseRegion(mouse_a[usNewPosition].x, mouse_a[usNewPosition].y, mouse_d[usNewPosition].x, mouse_d[usNewPosition].y);
	}

	void StandardSwitchCover::DefineCoverGroup(UINT _grpIndex)
	{
		grpIndex=_grpIndex;
	}

	void StandardSwitchCover::SetMouseRegion(unsigned short position, float xmin, float ymin, float xmax, float ymax)
	{
		assert( (position < 2) && "StandardSwitchCover::SetMouseRegion.position" );
		
		mouse_a[position].x=xmin;
		mouse_a[position].y=ymin;
		mouse_d[position].x=xmax;
		mouse_d[position].y=ymax;
	}
};
