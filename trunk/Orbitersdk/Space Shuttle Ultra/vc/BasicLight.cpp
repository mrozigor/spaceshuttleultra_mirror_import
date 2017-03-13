#include "BasicLight.h"

namespace vc {

	BasicLight::BasicLight(Atlantis* _sts, const string& _ident)
		:AtlantisVCComponent(_sts, _ident)
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

	/////////////////////////////////////////////////////////

	BasicLight_2::BasicLight_2( Atlantis* _sts, const string& _ident ):AtlantisVCComponent( _sts, _ident )
	{
		state = 0;
		next_state = 0;
		default_state = 0;
	}

	BasicLight_2::~BasicLight_2()
	{
	}

	void BasicLight_2::SetDefaultState( int _state )
	{
		return;
	}

	void BasicLight_2::SetStateOffset( int _state, float _U, float _V )
	{
		return;
	}

	bool BasicLight_2::ConnectLight( unsigned short usPort, DiscreteBundle* pBundle, unsigned short usLine )
	{
		return false;
	}
}