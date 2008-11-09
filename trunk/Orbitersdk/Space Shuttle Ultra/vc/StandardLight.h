#pragma once 

#include "BasicLight.h"
#include "../discsignals/DiscInPort.h"



namespace vc {

	using class discsignals::DiscInPort;

	class StandardLight: public BasicLight {
		SURFHANDLE shSource;
		POINT sourceOn;
		POINT sourceOff;
		unsigned short usWidth;
		unsigned short usHeight;

		bool bOldState;
		bool bCurrentState;
		bool bHasSourceImage;
	public:
		StandardLight(Atlantis* _sts, const string& _ident);
		~StandardLight();
		void SetSourceImage(SURFHANDLE _shSource);
		void SetDimensions(unsigned short usWidth, unsigned short usHeight);
		void SetSourceCoords(bool fState, unsigned short usX, unsigned short usY);
//		virtual bool Connect(unsigned short usPort, ::discsignals::DiscreteBundle* pBundle, unsigned short usLine);
		virtual void OnPostStep(double fSimT, double fDeltaT, double fMJD);
		virtual bool OnVCRedrawEvent(int id, int _event, SURFHANDLE surf);

		DiscInPort input;
	};
}