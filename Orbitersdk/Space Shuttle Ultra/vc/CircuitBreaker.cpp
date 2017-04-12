#include "CircuitBreaker.h"
#include "../Atlantis.h"


namespace vc
{
	CircuitBreaker::CircuitBreaker( Atlantis* _sts, const std::string& _ident ):AtlantisVCComponent( _sts, _ident )
	{
		CBin = true;
		move = NULL;
		counting = false;
	}

	CircuitBreaker::~CircuitBreaker()
	{
		if (move) delete move;
	}

	bool CircuitBreaker::OnParseLine( const char* line )
	{
		char pszBuffer[256];
		sprintf_s( pszBuffer, 255, "\t\tSetting circuit breaker \"%s\" to state \"%s\".", GetQualifiedIdentifier().c_str(), line );
		oapiWriteLog( pszBuffer );

		if (!_strnicmp( line, "OUT", 3 ))
		{
			CBin = false;
			return true;
		}
		else if (!_strnicmp( line, "IN", 2 )) return true;
		else
		{
			sprintf_s( pszBuffer, 255, "\t\tError setting circuit breaker \"%s\" state, default used.", GetQualifiedIdentifier().c_str() );
			oapiWriteLog( pszBuffer );
			return true;
		}
	}

	bool CircuitBreaker::GetStateString( unsigned long ulBufferSize, char* pszBuffer )
	{
		try
		{
			if (CBin) sprintf_s( pszBuffer, ulBufferSize, "IN" );
			else sprintf_s( pszBuffer, ulBufferSize, "OUT" );
		}
		catch (...)
		{
			sprintf_s( pszBuffer, ulBufferSize, "Circuit breaked %s: exception thrown", GetIdentifier().c_str() );
		}
		return true;
	}

	void CircuitBreaker::DefineGroup( UINT _grpIndex )
	{
		grpIndex = _grpIndex;
		return;
	}

	void CircuitBreaker::DefineDirection( VECTOR3 _dir )
	{
		dir = _dir;
		return;
	}

	void CircuitBreaker::Connect( DiscreteBundle* pBundle, unsigned short usLine )
	{
		output.Connect( pBundle, usLine );
		return;
	}

	void CircuitBreaker::DefineVCAnimations( UINT vc_idx )
	{
		char pszBuffer[256];
		sprintf_s( pszBuffer, 255, "Circuit Breaker[%s]:\tDefine VC Animations()", GetQualifiedIdentifier().c_str() );
		oapiWriteLog( pszBuffer );
			
		move = new MGROUP_TRANSLATE( vc_idx, &grpIndex, 1, dir * CB_MOVEMENT_RANGE );
		anim_move = STS()->CreateAnimation( 0.0 );// default in CB in
		STS()->AddAnimationComponent( anim_move, 0.0, 1.0, move );

		VerifyAnimations();
		OnChange( CBin );
		return;
	}

	void CircuitBreaker::OnChange( bool _CBin )
	{
		CBin = _CBin;

		if (CBin)
		{
			output.SetLine();
			SetAnimation( anim_move, 0.0 );
		}
		else
		{
			output.ResetLine();
			SetAnimation( anim_move, 1.0 );
		}
		// TODO add sound
		return;
	}

	bool CircuitBreaker::OnMouseEvent( int _event, float x, float y )
	{
		if (_event & PANEL_MOUSE_LBDOWN)
		{
			t0 = oapiGetSysTime();
			counting = true;
			return true;
		}
		else if ((_event & PANEL_MOUSE_LBPRESSED) && counting && ((oapiGetSysTime() - t0) >= CB_MOVEMENT_TIME))
		{
			OnChange( !CBin );
			counting = false;
			return true;
		}
		return false;
	}
};