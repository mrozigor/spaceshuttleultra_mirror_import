#ifndef __DAP_H
#define __DAP_H
#pragma once

#include "../Atlantis.h"
#include "../discsignals/DiscOutPort.h"
#include "../discsignals/DiscInPort.h"

// ==============================================================
// DAPControl class
// Controls DAP PBIs
// ==============================================================

namespace vc {
	using class discsignals::DiscInPort;
	using class discsignals::DiscOutPort;

	class DAPControl {
	public:
		DAPControl(Atlantis *_sts);
		/**
		 * Handles mouse events from either set of PBIs.
		 * block==1 represents top row of PBIs; block==2 represents all other PBIs.
		 */
		bool VCMouseEvent(int block, int nEvent, const VECTOR3 &p);
		/**
		 * Returns true if PBI on
		 */
		bool GetPBIState(int id);
		/*
		 * Called after control mode is changed
		 * Adds extra initialization
		 */
		void InitializeControlMode();
		void OnPostStep(double SimT, double DeltaT, double MJD);

		void Realize();

	private:
		void ButtonPress(int id);
		//bool DrawPBILight(SURFHANDLE surf, bool &bState, bool bOn);

		Atlantis* sts;

		DiscInPort input[24];
		DiscOutPort output[24];
		bool oldValues[24];
	};
};

#endif //__DAP_H
