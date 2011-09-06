#include "BasicLight.h"

namespace vc {

	BasicLight::BasicLight(Atlantis* _sts, const string& _ident)
		:BasicVCComponent(_sts, _ident)
	{
		redrawBase.x = 0;
		redrawBase.y = 0;
	}

	BasicLight::~BasicLight() {
	}

	void BasicLight::OnPostStep(double fSimT, double fDeltaT, double fMJD)
	{

	}

	bool BasicLight::OnVCRedrawEvent(int id, int _event, SURFHANDLE surf)
	{
		return false;
	}

	bool BasicLight::Connect(unsigned short usPort, discsignals::DiscreteBundle* pBundle, unsigned short usLine) {
		return false;
	}

	void BasicLight::SetBase(unsigned short usX, unsigned short usY) {
		redrawBase.x = usX;
		redrawBase.y = usY;
	}


};