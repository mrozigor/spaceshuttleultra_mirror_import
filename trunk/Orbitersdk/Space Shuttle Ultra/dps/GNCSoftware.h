#ifndef _GNCSOFTWARE_H
#define _GNCSOFTWARE_H

#include "../Atlantis.h"
#include "GPCSoftware.h"

namespace dps
{
	class GNCSoftware : public GPCSoftware
	{
	public:
		GNCSoftware(Atlantis* _sts);

		//virtual void OnPreStep(double SimT, double DeltaT, double MJD);
		virtual void OnPostStep(double SimT, double DeltaT, double MJD);
	private:
		void GPC(double dt);
		void Maneuver(double dt);
		void SetILoads();
		//scheduled functions
		static void UpdateElements(Atlantis* sts);
	};
};

#endif //!_GNCSOFTWARE_H