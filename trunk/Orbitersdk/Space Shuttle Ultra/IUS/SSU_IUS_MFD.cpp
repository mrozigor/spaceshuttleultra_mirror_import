#include "SSU_IUS_MFD.h"


SSU_IUS_MFD::SSU_IUS_MFD( DWORD w, DWORD h, VESSEL *v ):MFD2( w, h, v )
{
	iusvessel = (SSU_IUS*)v;
	return;
}

SSU_IUS_MFD::~SSU_IUS_MFD( void )
{
	return;
}

bool SSU_IUS_MFD::Update( oapi::Sketchpad *skp )
{
	Title( skp, "Space Shuttle Ultra IUS MFD" );

	char cbuf[64];
	double Ypos = 0.3;

	skp->SetTextColor( 0x00FF00 );

	if (!iusvessel->separated)
	{
		sprintf_s( cbuf, 64, "Not Active (still attached to ASE)" );
		skp->Text( (int)(W * 0.01), (int)(H * Ypos), cbuf, strlen( cbuf ) );
		return true;
	}
	
	sprintf_s( cbuf, 64, "RCS available: %.2f Kg", iusvessel->GetPropellantMass( iusvessel->ph_rcs ) );
	skp->Text( (int)(W * 0.01), (int)(H * Ypos), cbuf, strlen( cbuf ) );
	Ypos += 0.05;

	if (!iusvessel->SRM_ENA)
	{
		sprintf_s( cbuf, 64, "SRM inhibits removed in: %.0f s", SRM_ENA_DELAY - iusvessel->timer_SRM_ENA );
		skp->Text( (int)(W * 0.01), (int)(H * Ypos), cbuf, strlen( cbuf ) );
		Ypos += 0.05;
	}

	if (!iusvessel->RCS_ENA)
	{
		sprintf_s( cbuf, 64, "RCS inhibits removed in: %.0f s", RCS_ENA_DELAY - iusvessel->timer_RCS_ENA );
		skp->Text( (int)(W * 0.01), (int)(H * Ypos), cbuf, strlen( cbuf ) );
		Ypos += 0.05;
	}

	OBJHANDLE earth = iusvessel->GetGravityRef();
	char str[8];
	oapiGetObjectName( earth, str, 8 ); 
	if (!_strnicmp( str, "Earth", 5 ))
	{
		VECTOR3 vel;
		VECTOR3 pos;
		iusvessel->GetRelativeVel( earth, vel );
		iusvessel->GetRelativePos( earth, pos );
		double v = length( vel );
		double r = length( pos );
		double mu = 398600439968871.19;
		double C3 = ((0.5 * v * v) - (mu / r)) * 0.000002;
		sprintf_s( cbuf, 64, "C3 energy: %.2f km^2/sec^2", C3 );
		skp->Text( (int)(W * 0.01), (int)(H * Ypos), cbuf, strlen( cbuf ) );
	}
	return true;
}

int SSU_IUS_MFD::MsgProc( UINT msg, UINT mfd, WPARAM wparam, LPARAM lparam )
{
	switch (msg)
	{
		case OAPI_MSG_MFD_OPENED:
			return (int)(new SSU_IUS_MFD( LOWORD(wparam), HIWORD(wparam), (VESSEL*)lparam ));
	}
	return 0;
}