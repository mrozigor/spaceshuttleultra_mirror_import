#pragma once

#ifndef _AFTMDU_H
#define _AFTMDU_H

#include "BasicPanel.h"
#include "MDU.h"

namespace vc
{
	class AftMDU : public BasicPanel
	{
	private:
		MDU* pAFD;
	public:
		AftMDU(Atlantis* _sts);
		virtual ~AftMDU();
		virtual void DefineVC();
		virtual void RegisterVC();

		//virtual void OnPostStep (double fSimT, double fDeltaT, double fMJD);
		//virtual bool OnVCRedrawEvent (int id, int _event, SURFHANDLE surf);
	};
};

#endif //_AFTMDU_H