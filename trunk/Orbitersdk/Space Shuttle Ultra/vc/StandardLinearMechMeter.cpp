#include "StandardLinearMechMeter.h"
#include "../Atlantis.h"


namespace vc
{
	StandardLinearMechMeter::StandardLinearMechMeter( Atlantis* _sts, const string& _ident ):BasicMechMeter( _sts, _ident )
	{
		needle_trans = NULL;
	}

	StandardLinearMechMeter::~StandardLinearMechMeter()
	{
		if (needle_trans) delete needle_trans;
	}

	void StandardLinearMechMeter::DefineVCAnimations( UINT vc_idx )
	{
		char pszBuffer[256];
		sprintf_s( pszBuffer, 255, "Standard Linear Mech Meter[%s]:\tDefine VC Animations()", GetQualifiedIdentifier().c_str() );
		oapiWriteLog( pszBuffer );
		
		needle_trans = new MGROUP_TRANSLATE( vc_idx, &grpIndex, 1, Normalize( GetDirection() ) * motionrange );
		anim = STS()->CreateAnimation( InitialAnimState() );
		STS()->AddAnimationComponent( anim, 0.0, 1.0, needle_trans, NULL );
		return;
	}

	void StandardLinearMechMeter::OnPostStep( double SimT, double DeltaT, double MJD )
	{
		double val = range( 0.0, input.GetVoltage() / 5.0, 1.0 );
		
		STS()->SetAnimation( anim, val );
		return;
	}
};
