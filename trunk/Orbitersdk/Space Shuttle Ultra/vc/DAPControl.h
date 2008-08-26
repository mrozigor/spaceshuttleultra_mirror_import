#ifndef __DAP_H
#define __DAP_H

#include "../Atlantis.h"

// ==============================================================
// DAPControl class
// Controls DAP PBIs
// ==============================================================

namespace vc {
	class DAPControl {
	public:
		DAPControl(Atlantis *_sts);
		/**
		 * Handles mouse events from either set of PBIs.
		 * block==1 represents top row of PBIs; block==2 represents all other PBIs.
		 */
		bool VCMouseEvent(int block, int nEvent, const VECTOR3 &p);
		/**
		 * Returns 1 if light is ON; 0 otherwise.
		 */
		int LightStatus(int ButtonID);

	private:
		Atlantis* sts;
	};
};

#endif //__DAP_H