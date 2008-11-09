#pragma once
#include "BasicVCComponent.h"
#include "../discsignals/DiscreteBundle.h"

namespace vc {

	using class discsignals::DiscreteBundle;


	class BasicLight:public BasicVCComponent {
	protected:

		POINT redrawBase;
	public:
		BasicLight(Atlantis* _sts, const string& _ident);
		virtual ~BasicLight();
		void SetBase(unsigned short usX, unsigned short usY);
		virtual bool Connect(unsigned short usPort, discsignals::DiscreteBundle* pBundle, unsigned short usLine);
		virtual void OnPostStep(double fSimT, double fDeltaT, double fMJD);
		virtual bool OnVCRedrawEvent(int id, int _event, SURFHANDLE surf);
	};


};