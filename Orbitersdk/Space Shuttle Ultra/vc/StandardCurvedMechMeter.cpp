#include "StandardCurvedMechMeter.h"
#include "../Atlantis.h"
#include <UltraMath.h>


namespace vc
{
	StandardCurvedMechMeter::StandardCurvedMechMeter( Atlantis* _sts, const string& _ident ):BasicMechMeter( _sts, _ident )
	{
		needle_rot = NULL;
	}

	StandardCurvedMechMeter::~StandardCurvedMechMeter()
	{
		if (needle_rot) delete needle_rot;
	}

	void StandardCurvedMechMeter::DefineVCAnimations( UINT vc_idx )
	{
		char pszBuffer[256];
		sprintf_s( pszBuffer, 255, "Standard Curved Mech Meter[%s]:\tDefine VC Animations()", GetQualifiedIdentifier().c_str() );
		oapiWriteLog( pszBuffer );
		
		needle_rot = new MGROUP_ROTATE( vc_idx, &grpIndex, 1, GetReference(), GetDirection(), (float)(motionrange * RAD) );
		anim = STS()->CreateAnimation( InitialAnimState() );
		STS()->AddAnimationComponent( anim, 0.0, 1.0, needle_rot, NULL );
		return;
	}

	void StandardCurvedMechMeter::OnPostStep( double SimT, double DeltaT, double MJD )
	{
		double val = range( 0.0, input.GetVoltage() / 5.0, 1.0 );
		
		STS()->SetAnimation( anim, val );
		return;
	}
};
