#include "GNCSoftware.h"
#include "../Atlantis_defs.h"

namespace dps
{
	GNCSoftware::GNCSoftware(Atlantis* _sts)
		: GPCSoftware(_sts)
	{
		ScheduledFunctions.push_back(new ScheduledFunction(sts, 5.0, &UpdateElements));
	}

	void GNCSoftware::OnPostStep(double SimT, double DeltaT, double MJD)
	{
		sts->oparam.ApT-=DeltaT;
		sts->oparam.PeT-=DeltaT;

		GPCSoftware::OnPostStep(SimT, DeltaT, MJD);
		//sprintf_s(oapiDebugString(), 255, "Elements: %f %f", sts->el.a, sts->oparam.ApT);
	}

	void GNCSoftware::UpdateElements(Atlantis* sts)
	{
		if(sts->status==STATE_ORBITER)
			sts->GetElements(NULL, sts->el, &(sts->oparam), 0, FRAME_EQU);
	}
};