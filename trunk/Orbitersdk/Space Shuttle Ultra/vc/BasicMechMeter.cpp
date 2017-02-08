#include "BasicMechMeter.h"
#include "../Atlantis.h"


namespace vc
{
	BasicMechMeter::BasicMechMeter( Atlantis* _sts, const string& _ident ):AtlantisVCComponent( _sts, _ident )
	{
	}

	BasicMechMeter::~BasicMechMeter()
	{
	}

	void BasicMechMeter::DefineNeedleGroup( UINT _grpIndex )
	{
		grpIndex = _grpIndex;
		return;
	}

	void BasicMechMeter::SetMotionRange( double range )
	{
		motionrange = range;
		return;
	}

	void BasicMechMeter::SetInput( DiscreteBundle* pBundle, unsigned short usLine )
	{
		input.Connect( pBundle, usLine );
		return;
	}
};
