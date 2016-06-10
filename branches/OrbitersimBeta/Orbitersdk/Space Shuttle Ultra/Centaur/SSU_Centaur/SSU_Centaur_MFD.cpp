#include "SSU_Centaur_MFD.h"


SSU_Centaur_MFD::SSU_Centaur_MFD( DWORD w, DWORD h, VESSEL *v ):MFD2( w, h, v )
{
	centaurvessel = (SSU_Centaur*)v;
	return;
}

SSU_Centaur_MFD::~SSU_Centaur_MFD( void )
{
	return;
}

bool SSU_Centaur_MFD::Update( oapi::Sketchpad *skp )
{
	Title( skp, "Space Shuttle Ultra Centaur G MFD" );

	char cbuf[64];
	double Ypos = 0.3;

	skp->SetTextColor( 0x00FF00 );

	if (!centaurvessel->separated)
	{
		sprintf_s( cbuf, 64, "Not Active (still attached to CISS)" );
		skp->Text( (int)(W * 0.01), (int)(H * Ypos), cbuf, strlen( cbuf ) );
		return true;
	}
	
	sprintf_s( cbuf, 64, "ACS available: %.2f Kg", centaurvessel->GetPropellantMass( centaurvessel->phACS ) );
	skp->Text( (int)(W * 0.01), (int)(H * Ypos), cbuf, strlen( cbuf ) );
	Ypos += 0.05;

	if (!centaurvessel->RL10_ENA)
	{
		sprintf_s( cbuf, 64, "RL-10 inhibits removed in: %.0f s", RL10_ENA_DELAY - centaurvessel->timer_RL10_ENA );
		skp->Text( (int)(W * 0.01), (int)(H * Ypos), cbuf, strlen( cbuf ) );
		Ypos += 0.05;
	}
	else if ((centaurvessel->RL10_startseq) && (centaurvessel->RL10_chill))
	{
		sprintf_s( cbuf, 64, "RL-10 ignition in: %.0f s", RL10_START_SEQ - centaurvessel->timer_RL10_startseq );
		skp->Text( (int)(W * 0.01), (int)(H * Ypos), cbuf, strlen( cbuf ) );
		Ypos += 0.05;
	}

	if (!centaurvessel->ACS_ENA)
	{
		sprintf_s( cbuf, 64, "ACS inhibits removed in: %.0f s", ACS_ENA_DELAY - centaurvessel->timer_ACS_ENA );
		skp->Text( (int)(W * 0.01), (int)(H * Ypos), cbuf, strlen( cbuf ) );
		Ypos += 0.05;
	}

	OBJHANDLE earth = centaurvessel->GetGravityRef();
	char str[8];
	oapiGetObjectName( earth, str, 8 ); 
	if (!_strnicmp( str, "Earth", 5 ))
	{
		VECTOR3 vel;
		VECTOR3 pos;
		centaurvessel->GetRelativeVel( earth, vel );
		centaurvessel->GetRelativePos( earth, pos );
		double v = length( vel );
		double r = length( pos );
		double mu = 398600439968871.19;
		double C3 = ((0.5 * v * v) - (mu / r)) * 0.000002;
		sprintf_s( cbuf, 64, "C3 energy: %.2f km^2/sec^2", C3 );
		skp->Text( (int)(W * 0.01), (int)(H * Ypos), cbuf, strlen( cbuf ) );
	}
	return true;
}

int SSU_Centaur_MFD::MsgProc( UINT msg, UINT mfd, WPARAM wparam, LPARAM lparam )
{
	switch (msg)
	{
		case OAPI_MSG_MFD_OPENED:
			return (int)(new SSU_Centaur_MFD( LOWORD(wparam), HIWORD(wparam), (VESSEL*)lparam ));
	}
	return 0;
}