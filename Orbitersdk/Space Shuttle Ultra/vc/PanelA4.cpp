#include "PanelA4.h"
#include "../Atlantis.h"
#include "../Atlantis_defs.h"
#include "../meshres_vc_additions.h"
#include "../dps/MasterTimingUnit.h"


extern GDIParams g_Param;

namespace vc
{
	PanelA4::PanelA4( Atlantis *_sts ):AtlantisPanel( _sts, "A4" )
	{
		Add( pMissionTimer = new StdSwitch3( _sts, "Mission Timer" ) );
		pMissionTimer->SetLabel( 0, "TEST" );
		pMissionTimer->SetLabel( 1, "MET" );
		pMissionTimer->SetLabel( 2, "GMT" );
		
		sTimerMinutes = 0;
		sTimerSeconds = 0;
		sMETSeconds = 0;
		sMETMinutes = 0;
		sMETHours = 0;
		sMETDays = 0;
	}

	PanelA4::~PanelA4()
	{
	}

	void PanelA4::DefineVC()
	{
		VECTOR3 switch_rot = _V( -0.96598, 0, 0.25860 );

		AddAIDToMouseEventList( AID_A4 );

		pMissionTimer->DefineSwitchGroup( GRP_A4MISSIONSWITCH_VC );
		pMissionTimer->SetInitialAnimState( 0.5 );
		pMissionTimer->SetReference( _V( -0.7997, 2.9475, 12.3330 ), switch_rot );
		pMissionTimer->SetMouseRegion( 0.741337f, 0.766488f, 0.860226f, 0.890987f );
		return;
	}

	void PanelA4::RegisterVC()
	{
		AtlantisPanel::RegisterVC();
		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea( AID_A4, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_A4, 
			_V( -0.6351, 3.1074, 12.2873 ) + ofs, _V( -0.8416, 3.1074, 12.3431 ) + ofs, 
			_V( -0.6351, 2.9140, 12.2873 ) + ofs, _V( -0.8416, 2.9140, 12.3431 ) + ofs );

		SURFHANDLE digit_tex = oapiGetTextureHandle( STS()->hOrbiterVCMesh, TEX_DIGITS_VC );
		
		oapiVCRegisterArea( AID_A4_EVTTMR, _R( 64, 384, 320, 448 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digit_tex );
		oapiVCRegisterArea( AID_A4_METTMR1, _R( 0, 448, 512, 512 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digit_tex );
		oapiVCRegisterArea( AID_A4_METTMR2, _R( 0, 384, 64, 448 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digit_tex );
		return;
	}

	void PanelA4::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "A4_Timer", 2 );
		pMissionTimer->ConnectPort( 1, pBundle, 0 );
		pMissionTimer->ConnectPort( 2, pBundle, 1 );
		pMissionTimerGMT.Connect( pBundle, 1 );
		pMissionTimerTEST.Connect( pBundle, 0 );

		AtlantisPanel::Realize();
		return;
	}

	bool PanelA4::OnVCRedrawEvent( int id, int _event, SURFHANDLE surf )
	{
		const int NUMX[10] = {64, 0, 64, 128, 192, 256, 320, 384, 448, 0};
		const int NUMY[10] = {384, 448, 448, 448, 448, 448, 448, 448, 448, 384};
		int i;
		int digit[8];

		if(id ==AID_A4_EVTTMR)
		{
			digit[0] = sTimerMinutes / 10;
			digit[1] = sTimerMinutes % 10;
			digit[2] = sTimerSeconds / 10;
			digit[3] = sTimerSeconds % 10;

			for(i = 0; i<4; i++)
			{
				oapiBlt(surf, g_Param.digits_7seg, i*64, 0, NUMX[digit[i]], NUMY[digit[i]], 64, 64);
			}
			return true;
		}

		else if(id == AID_A4_METTMR1)
		{
			digit[0] = sMETDays /100;
			digit[1] = (sMETDays / 10) % 10;
			digit[2] = sMETDays %10;
			digit[3] = sMETHours / 10;
			digit[4] = sMETHours % 10;
			digit[5] = sMETMinutes / 10;
			digit[6] = sMETMinutes % 10;
			digit[7] = sMETSeconds / 10;


			for(i = 0; i<8; i++)
			{
				oapiBlt(surf, g_Param.digits_7seg, i*64, 0, NUMX[digit[i]], NUMY[digit[i]], 64, 64);
			}
			return true;
		}
		else if(id == AID_A4_METTMR2)
		{
			digit[0] = sMETSeconds % 10;
		
			oapiBlt(surf, g_Param.digits_7seg, 0, 0, NUMX[digit[0]], NUMY[digit[0]], 64, 64);
			return true;
		
		}
		return AtlantisPanel::OnVCRedrawEvent(id, _event, surf);
	}

	void PanelA4::OnPreStep( double SimT, double DeltaT, double MJD )
	{
		AtlantisPanel::OnPreStep( SimT, DeltaT, MJD );
		//Check forward event timer for changes and update clock if needed

		if(STS()->pMTU->GetEventTimerMin(dps::TIMER_AFT) != sTimerMinutes || 
			STS()->pMTU->GetEventTimerSec(dps::TIMER_AFT) != sTimerSeconds) {
			sTimerMinutes = STS()->pMTU->GetEventTimerMin(dps::TIMER_AFT);
			sTimerSeconds = STS()->pMTU->GetEventTimerSec(dps::TIMER_AFT);
			oapiVCTriggerRedrawArea(-1, AID_A4_EVTTMR);
		}

		if (pMissionTimerGMT.IsSet())
		{
			if(STS()->pMTU->GetGMTDay(0) != sMETDays || 
				STS()->pMTU->GetGMTHour(0) != sMETHours ||
				STS()->pMTU->GetGMTSec(0) != sMETSeconds ||
				STS()->pMTU->GetGMTMin(0) != sMETMinutes) {
				if(STS()->pMTU->GetGMTSec(0) != sMETSeconds) {
					oapiVCTriggerRedrawArea(-1, AID_A4_METTMR2);
				}
				sMETDays = STS()->pMTU->GetGMTDay(0);
				sMETHours = STS()->pMTU->GetGMTHour(0);
				sMETMinutes = STS()->pMTU->GetGMTMin(0);
				sMETSeconds = STS()->pMTU->GetGMTSec(0);
				oapiVCTriggerRedrawArea(-1, AID_A4_METTMR1);
			}
		}
		else if (pMissionTimerTEST.IsSet())
		{
			if(sMETDays != 888)
			{
				sMETDays = 888;
				sMETHours = 88;
				sMETMinutes = 88;
				sMETSeconds = 88;
				oapiVCTriggerRedrawArea(-1, AID_A4_METTMR1);
				oapiVCTriggerRedrawArea(-1, AID_A4_METTMR2);
			}
		}
		else
		{
			if(STS()->pMTU->GetMETDay(0) != sMETDays || 
				STS()->pMTU->GetMETHour(0) != sMETHours ||
				STS()->pMTU->GetMETSec(0) != sMETSeconds ||
				STS()->pMTU->GetMETMin(0) != sMETMinutes) {
				if(STS()->pMTU->GetMETSec(0) != sMETSeconds) {
					sMETSeconds = STS()->pMTU->GetMETSec(0);
					oapiVCTriggerRedrawArea(-1, AID_A4_METTMR2);
				}
				sMETDays = STS()->pMTU->GetMETDay(0);
				sMETHours = STS()->pMTU->GetMETHour(0);
				sMETMinutes = STS()->pMTU->GetMETMin(0);
			
				oapiVCTriggerRedrawArea(-1, AID_A4_METTMR1);
			}
		}
		return;
	}
}
