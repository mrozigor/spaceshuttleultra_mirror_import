#include "DAPControl.h"

// ==============================================================

namespace vc {
	DAPControl::DAPControl(Atlantis *_sts)
		: sts(_sts)
	{
	}

	bool DAPControl::VCMouseEvent(int block, int nEvent, const VECTOR3 &p)
	{
		sprintf_s(oapiDebugString(), 255, "DAPControl: %f %f %f", p.x, p.y, p.z);
		
		if(block==1) {
		}
		else if(block==2) {
		}

		return false;
	}
};