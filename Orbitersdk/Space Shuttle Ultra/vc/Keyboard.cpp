#include "Keyboard.h"
#include "../Atlantis.h"
#include "../dps/dps_defs.h"
#include <OrbiterSoundSDK40.h>
#include <assert.h>


namespace vc
{
	Keyboard::Keyboard( Atlantis* _sts, const std::string& _ident, int ID ):AtlantisVCComponent( _sts, _ident )
	{
		this->ID = ID;
		pIDP[0] = NULL;
		pIDP[1] = NULL;
	}

	Keyboard::~Keyboard()
	{
	}

	void Keyboard::ConnectIDP( unsigned int num, dps::IDP* p_idp )
	{
		assert( (num < 2) && "Keyboard::ConnectIDP.num" );
		pIDP[num] = p_idp;
		return;
	}

	bool Keyboard::OnMouseEvent( int _event, float x, float y )
	{
		if (_event != PANEL_MOUSE_LBUP) return false;

		if (x >= 0.0 && x < 0.25)// first column of buttons
		{
			if (y >= 0 && y < 0.125)// FAULT SUMM
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_FAULTSUMM );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_FAULTSUMM );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
			else if (y >= 0.125 && y < 0.25)// GPC/CRT
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_GPCCRT );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_GPCCRT );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
			else if (y >= 0.25 && y < 0.375)// I/O RESET
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_IORESET );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_IORESET );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
			else if (y >= 0.375 && y < 0.5)// ITEM
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_ITEM );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_ITEM );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
			else if (y >= 0.5 && y < 0.625)// EXEC
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_EXEC );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_EXEC );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
			else if (y >= 0.625 && y < 0.75)// OPS
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_OPS );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_OPS );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
			else if (y >= 0.75 && y < 0.875)// SPEC
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_SPEC );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_SPEC );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
			else if (y >= 0.875 && y < 1.0)// RESUME
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_RESUME );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_RESUME );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
		}
		else if (x >= 0.25 && x < 0.5)// second column of buttons
		{
			if (y >= 0 && y < 0.125)// SYS SUMM
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_SYSSUMM );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_SYSSUMM );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
			else if (y >= 0.125 && y < 0.25)// A
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_A );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_A );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
			else if (y >= 0.25 && y < 0.375)// D
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_D );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_D );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
			else if (y >= 0.375 && y < 0.5)// 1
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_1 );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_1 );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
			else if (y >= 0.5 && y < 0.625)// 4
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_4 );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_4 );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
			else if (y >= 0.625 && y < 0.75)// 7
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_7 );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_7 );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
			else if (y >= 0.75 && y < 0.875)// -
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_MINUS );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_MINUS );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
			else if (y >= 0.875 && y < 1.0)// CLEAR
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_CLEAR );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_CLEAR );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
		}
		else if (x >= 0.5 && x < 0.75)// third column of buttons
		{
			if (y >= 0 && y < 0.125)// MSG RESET
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_MSGRESET );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_MSGRESET );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
			else if (y >= 0.125 && y < 0.25)// B
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_B );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_B );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
			else if (y >= 0.25 && y < 0.375)// E
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_E );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_E );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
			else if (y >= 0.375 && y < 0.5)// 2
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_2 );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_2 );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
			else if (y >= 0.5 && y < 0.625)// 5
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_5 );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_5 );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
			else if (y >= 0.625 && y < 0.75)// 8
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_8 );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_8 );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
			else if (y >= 0.75 && y < 0.875)// 0
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_0 );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_0 );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
			else if (y >= 0.875 && y < 1.0)// .
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_DOT );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_DOT );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
		}
		else if (x >= 0.75 && x <= 1.0)// fourth column of buttons
		{
			if (y >= 0 && y < 0.125)// ACK
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_ACK );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_ACK );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
			else if (y >= 0.125 && y < 0.25)// C
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_C );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_C );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
			else if (y >= 0.25 && y < 0.375)// F
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_F );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_F );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
			else if (y >= 0.375 && y < 0.5)// 3
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_3 );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_3 );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
			else if (y >= 0.5 && y < 0.625)// 6
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_6 );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_6 );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
			else if (y >= 0.625 && y < 0.75)// 9
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_9 );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_9 );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
			else if (y >= 0.75 && y < 0.875)// +
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_PLUS );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_PLUS );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
			else if (y >= 0.875 && y < 1.0)// PRO
			{
				if (pIDP[0] != NULL) pIDP[0]->PutKey( ID, SSU_KEY_PRO );
				if (pIDP[1] != NULL) pIDP[1]->PutKey( ID, SSU_KEY_PRO );
				PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
				return true;
			}
		}
		return false;
	}
};