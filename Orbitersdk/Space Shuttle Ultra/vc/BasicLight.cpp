#include "BasicLight.h"

namespace vc
{
	BasicLight::BasicLight( Atlantis* _sts, const string& _ident ):AtlantisVCComponent( _sts, _ident )
	{
		state = 0;
		next_state = 0;
		default_state = 0;
	}

	BasicLight::~BasicLight()
	{
	}

	void BasicLight::SetDefaultState( int _state )
	{
		return;
	}

	void BasicLight::SetStateOffset( int _state, float _U, float _V )
	{
		return;
	}

	bool BasicLight::ConnectLight( unsigned short usPort, DiscreteBundle* pBundle, unsigned short usLine )
	{
		return false;
	}
}