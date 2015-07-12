#include "PanelC2.h"
#include "../Atlantis.h"
#include "../Atlantis_defs.h"
#include "../meshres_vc_additions.h"


extern GDIParams g_Param;


namespace vc
{
	PanelC2::PanelC2( Atlantis* psts ):AtlantisPanel( psts, "C2" )
	{
		Add( pKeyboardCDR = new Keyboard( psts, "LH Keyboard", 1 ) );
		Add( pKeyboardPLT = new Keyboard( psts, "RH Keyboard", 2 ) );

		Add( pIDPCRTPower[0] = new StdSwitch2( psts, "IDP/CRT 1 Power" ) );
		pIDPCRTPower[0]->SetLabel( 0, "OFF" );
		pIDPCRTPower[0]->SetLabel( 1, "ON" );
		Add( pIDPCRTPower[1] = new StdSwitch2( psts, "IDP/CRT 2 Power" ) );
		pIDPCRTPower[1]->SetLabel( 0, "OFF" );
		pIDPCRTPower[1]->SetLabel( 1, "ON" );
		Add( pIDPCRTPower[2] = new StdSwitch2( psts, "IDP/CRT 3 Power" ) );
		pIDPCRTPower[2]->SetLabel( 0, "OFF" );
		pIDPCRTPower[2]->SetLabel( 1, "ON" );

		Add( pIDPCRTMajFunc[0] = new StdSwitch3( psts, "IDP/CRT 1 Maj Func" ) );
		pIDPCRTMajFunc[0]->SetLabel( 0, "PL" );
		pIDPCRTMajFunc[0]->SetLabel( 1, "SM" );
		pIDPCRTMajFunc[0]->SetLabel( 2, "GNC" );
		Add( pIDPCRTMajFunc[1] = new StdSwitch3( psts, "IDP/CRT 2 Maj Func" ) );
		pIDPCRTMajFunc[1]->SetLabel( 0, "PL" );
		pIDPCRTMajFunc[1]->SetLabel( 1, "SM" );
		pIDPCRTMajFunc[1]->SetLabel( 2, "GNC" );
		Add( pIDPCRTMajFunc[2] = new StdSwitch3( psts, "IDP/CRT 3 Maj Func" ) );
		pIDPCRTMajFunc[2]->SetLabel( 0, "PL" );
		pIDPCRTMajFunc[2]->SetLabel( 1, "SM" );
		pIDPCRTMajFunc[2]->SetLabel( 2, "GNC" );

		Add( pCRTSEL[0] = new StdSwitch2( psts, "Left CRT Sel" ) );
		pCRTSEL[0]->SetLabel( 0, "3" );
		pCRTSEL[0]->SetLabel( 1, "1" );
		Add( pCRTSEL[1] = new StdSwitch2( psts, "Right CRT Sel" ) );
		pCRTSEL[1]->SetLabel( 0, "2" );
		pCRTSEL[1]->SetLabel( 1, "3" );

		Add( pEventTimerMode = new StdSwitch3( psts, "Event Timer Mode" ) );
		pEventTimerMode->SetLabel( 0, "Test" );
		pEventTimerMode->SetLabel( 1, "Down" );
		pEventTimerMode->SetLabel( 2, "Up" );

		Add( pEventTimerControl = new StdSwitch3( psts, "Event Timer Mode" ) );
		pEventTimerControl->SetLabel( 0, "Stop" );
		pEventTimerControl->SetLabel( 1, "-" );
		pEventTimerControl->SetLabel( 2, "Start" );

		Add( pTimer = new StdSwitch3( psts, "Event Timer Mode" ) );
		pTimer->SetLabel( 0, "Reset" );
		pTimer->SetLabel( 1, "-" );
		pTimer->SetLabel( 2, "Set" );

		for (int i = 0; i < 4; i++)
		{
			tgtwheel_state[i] = 0;
			wheelState[i] = 0.0;
			wheelnumber[i] = 0;
			oldwheelnumber[i] = 9;
		}
	}

	PanelC2::~PanelC2()
	{
	}

	void PanelC2::OnPreStep( double SimT, double DeltaT, double MJD )
	{
		AtlantisPanel::OnPreStep( SimT, DeltaT, MJD );

		double fCurrState = 0.0;
		double fTgtState = 0.0;

		double fDeltaWheel = 0.5 * DeltaT;

		for(int i = 0; i<4; i++)
		{
			fCurrState = wheelState[i];
			fTgtState = tgtwheel_state[i];

	
			if(0.0 == fCurrState && fTgtState >= EVTTMR_WHEELMAX[i] - 0.5)
			{
				fCurrState = EVTTMR_WHEELMAX[i];
			}

			if(fCurrState <= 0.5 && fTgtState == EVTTMR_WHEELMAX[i])
			{
				fTgtState = 0.0;
			}

			if(fCurrState == fTgtState)
			{
				if(tgtwheel_state[i] >= EVTTMR_WHEELMAX[i])
				{
					tgtwheel_state[i] -= EVTTMR_WHEELMAX[i];
				}
				continue;
			}
			else if(fCurrState > fTgtState)
			{
				if(fCurrState - fDeltaWheel < fTgtState)
					wheelState[i] = fTgtState;
				else
					wheelState[i] = fCurrState - fDeltaWheel;
			}
			else
			{
				if(fCurrState + fDeltaWheel > fTgtState)
					wheelState[i] = fTgtState;
				else
					wheelState[i] = fCurrState + fDeltaWheel;
			}

			if(wheelState[i] >= EVTTMR_WHEELMAX[i])
			{
				wheelState[i] -= EVTTMR_WHEELMAX[i];
				if(tgtwheel_state[i] > EVTTMR_WHEELMAX[i])
					tgtwheel_state[i] -= EVTTMR_WHEELMAX[i];
			}

			wheelnumber[i] = (int)(fmod(wheelState[i]/ 0.25, EVTTMR_WHEELMAX[i] * 2.0) );

			STS()->SetAnimation(anim_VC_C2Wheel[i], fmod(wheelState[i], 1.0));
		}


		if(wheelnumber[0] != oldwheelnumber[0])
		{
			oapiVCTriggerRedrawArea(-1, AID_C2_WND0);
			oldwheelnumber[0] = wheelnumber[0];
		}

		if(wheelnumber[1] != oldwheelnumber[1])
		{
			oapiVCTriggerRedrawArea(-1, AID_C2_WND1);
			oldwheelnumber[1] = wheelnumber[1];
		}

		if(wheelnumber[2] != oldwheelnumber[2])
		{
			oapiVCTriggerRedrawArea(-1, AID_C2_WND2);
			oldwheelnumber[2] = wheelnumber[2];
		}

		if(wheelnumber[3] != oldwheelnumber[3])
		{
			oapiVCTriggerRedrawArea(-1, AID_C2_WND3);
			oldwheelnumber[3] = wheelnumber[3];
		}

		if (pEventTimerMode_Up.IsSet()) STS()->pMTU->SetEventTimerMode( dps::TIMER_FORWARD, dps::COUNT_UP );
		else if (pEventTimerMode_Test.IsSet()) STS()->pMTU->SetEventTimerMode( dps::TIMER_FORWARD, dps::COUNT_TEST );
		else STS()->pMTU->SetEventTimerMode( dps::TIMER_FORWARD, dps::COUNT_DOWN );

		if (pEventTimerControl_Start.IsSet()) STS()->pMTU->StartEventTimer( dps::TIMER_FORWARD );
		else if (pEventTimerControl_Stop.IsSet()) STS()->pMTU->StopEventTimer( dps::TIMER_FORWARD );

		if (pTimer_Set.IsSet()) STS()->pMTU->SetEventTimer( dps::TIMER_FORWARD, wheelnumber[0] * 10 + wheelnumber[1], wheelnumber[2] * 10 + wheelnumber[3] );
		else if (pTimer_Reset.IsSet()) STS()->pMTU->ResetEventTimer( dps::TIMER_FORWARD );
		return;
	}

	bool PanelC2::OnVCMouseEvent( int id, int _event, VECTOR3 &p )
	{
		if (id != AID_C2) return false;
		
		if (_event == PANEL_MOUSE_LBDOWN)
		{
			if(p.x> 0.478585 && p.x < 0.505111)
			{
				if(p.y > 0.631991 && p.y < 0.693657)
				{
					tgtwheel_state[0] = tgtwheel_state[0] + 0.25;
					return true;
				}
				else if(p.y > 0.738849 && p.y < 0.790335)
				{
					tgtwheel_state[0] -= 0.25;
					if(tgtwheel_state[0] < 0) tgtwheel_state[0] +=EVTTMR_WHEELMAX[0];
					return true;
				}
			}
			else if(p.x> 0.518210 && p.x < 0.545747)
			{
				if(p.y > 0.631991 && p.y < 0.693657)
				{
					tgtwheel_state[1] = tgtwheel_state[1] + 0.25;
					return true;
				}
				else if(p.y > 0.738849 && p.y < 0.790335)
				{
					tgtwheel_state[1] -= 0.25;
					if(tgtwheel_state[1] < 0) tgtwheel_state[1] +=EVTTMR_WHEELMAX[1];
					return true;
				}
			}
			else if(p.x> 0.558321 && p.x < 0.585850)
			{
				if(p.y > 0.631991 && p.y < 0.693657)
				{
					tgtwheel_state[2] = tgtwheel_state[2] + 0.25;
					return true;
				}
				else if(p.y > 0.738849 && p.y < 0.790335)
				{
					tgtwheel_state[2] -= 0.25;
					if(tgtwheel_state[2] < 0) tgtwheel_state[2] +=EVTTMR_WHEELMAX[2];
					return true;
				}
			}
			else if(p.x> 0.598419 && p.x < 0.625950)
			{
				if(p.y > 0.631991 && p.y < 0.693657)
				{
					tgtwheel_state[3] = tgtwheel_state[3] + 0.25;
					return true;
				}
				else if(p.y > 0.738849 && p.y < 0.790335)
				{
					tgtwheel_state[3] -= 0.25;
					if(tgtwheel_state[3] < 0) tgtwheel_state[3] +=EVTTMR_WHEELMAX[3];
					return true;
				}
			}
		}
		return AtlantisPanel::OnVCMouseEvent( id, _event, p );
	}

	bool PanelC2::OnVCRedrawEvent( int id, int _event, SURFHANDLE surf )
	{
		const int NUMX[10] = {0, 64, 128, 192, 0, 64, 128, 192, 0, 64};
		const int NUMY[10] = {0, 0, 0, 0, 64, 64, 64, 64, 128, 128};
	
		switch (id)
		{
			case AID_C2_WND0:
				oapiBlt(surf, g_Param.clock_digits, 0,0, NUMX[wheelnumber[0]], NUMY[wheelnumber[0]], 63, 63);
				return true;
			case AID_C2_WND1:
				oapiBlt(surf, g_Param.clock_digits, 0,0, NUMX[wheelnumber[1]], NUMY[wheelnumber[1]], 63, 63);
				return true;
			case AID_C2_WND2:
				oapiBlt(surf, g_Param.clock_digits, 0,0, NUMX[wheelnumber[2]], NUMY[wheelnumber[2]], 63, 63);
				return true;
			case AID_C2_WND3:
				oapiBlt(surf, g_Param.clock_digits, 0,0, NUMX[wheelnumber[3]], NUMY[wheelnumber[3]], 63, 63);
				return true;
		}
		return AtlantisPanel::OnVCRedrawEvent( id, _event, surf );
	}

	void PanelC2::DefineVC()
	{
		VECTOR3 switch_rot = _V( 1, 0, 0 );
		VECTOR3 switch_rotH = _V( 0, 0.707107, 0.707107 );
		VECTOR3 wheel_rot = _V( 1, 0, 0 );

		AddAIDToMouseEventList( AID_C2 );

		pKeyboardCDR->SetMouseRegion( 0.040320f, 0.094652f, 0.220764f, 0.953784f );
		
		pKeyboardPLT->SetMouseRegion( 0.776391f, 0.096152f, 0.956499f, 0.953310f );


		pIDPCRTPower[0]->SetInitialAnimState( 0.5f );
		pIDPCRTPower[0]->DefineSwitchGroup( GRP_C2B1_VC );
		pIDPCRTPower[0]->SetReference( _V( -0.10, 1.911417, 14.50198 ), switch_rot );
		pIDPCRTPower[0]->SetMouseRegion( 0.289743f, 0.163456f, 0.324921f, 0.276103f );
		pIDPCRTPower[0]->SetInitialPosition( 1 );

		pIDPCRTPower[1]->SetInitialAnimState( 0.5f );
		pIDPCRTPower[1]->DefineSwitchGroup( GRP_C2B5_VC );
		pIDPCRTPower[1]->SetReference( _V( 0.07, 1.911417, 14.50198 ), switch_rot );
		pIDPCRTPower[1]->SetMouseRegion( 0.613179f, 0.160406f, 0.650175f, 0.284562f );
		pIDPCRTPower[1]->SetInitialPosition( 1 );

		pIDPCRTPower[2]->SetInitialAnimState( 0.5f );
		pIDPCRTPower[2]->DefineSwitchGroup( GRP_C2B3_VC );
		pIDPCRTPower[2]->SetReference( _V( -0.02, 1.911417, 14.50198 ), switch_rot );
		pIDPCRTPower[2]->SetMouseRegion( 0.449651f, 0.157388f, 0.487955f, 0.281491f );
		pIDPCRTPower[2]->SetInitialPosition( 1 );


		pIDPCRTMajFunc[0]->SetInitialAnimState( 0.5f );
		pIDPCRTMajFunc[0]->DefineSwitchGroup( GRP_C2B2_VC );
		pIDPCRTMajFunc[0]->SetReference( _V( -0.07, 1.911417, 14.50198 ), switch_rot );
		pIDPCRTMajFunc[0]->SetMouseRegion( 0.344109f, 0.162173f, 0.382536f, 0.283917f );
		pIDPCRTMajFunc[0]->SetInitialPosition( 2 );

		pIDPCRTMajFunc[1]->SetInitialAnimState( 0.5f );
		pIDPCRTMajFunc[1]->DefineSwitchGroup( GRP_C2B6_VC );
		pIDPCRTMajFunc[1]->SetReference( _V( -0.10, 1.911417, 14.50198 ), switch_rot );
		pIDPCRTMajFunc[1]->SetMouseRegion( 0.669981f, 0.161427f, 0.709157f, 0.283448f );
		pIDPCRTMajFunc[1]->SetInitialPosition( 2 );

		pIDPCRTMajFunc[2]->SetInitialAnimState( 0.5f );
		pIDPCRTMajFunc[2]->DefineSwitchGroup( GRP_C2B4_VC );
		pIDPCRTMajFunc[2]->SetReference( _V( 0.01, 1.911417, 14.50198 ), switch_rot );
		pIDPCRTMajFunc[2]->SetMouseRegion( 0.507477f, 0.156221f, 0.546810f, 0.282617f );
		pIDPCRTMajFunc[2]->SetInitialPosition( 2 );


		pCRTSEL[0]->SetInitialAnimState( 0.5f );
		pCRTSEL[0]->DefineSwitchGroup( GRP_C2B7V_VC );
		pCRTSEL[0]->SetReference( _V( -0.100152, 1.859752, 14.45152 ), switch_rotH );
		pCRTSEL[0]->SetMouseRegion( 0.295351f, 0.438155f, 0.343899f, 0.542759f );
		pCRTSEL[0]->SetInitialPosition( 1 );
		pCRTSEL[0]->SetOrientation( true );


		pCRTSEL[1]->SetInitialAnimState( 0.5f );
		pCRTSEL[1]->DefineSwitchGroup( GRP_C2B8V_VC );
		pCRTSEL[1]->SetReference( _V( 0.100152, 1.859752, 14.45152 ), switch_rotH );
		pCRTSEL[1]->SetMouseRegion( 0.665661f, 0.437980f, 0.712859f, 0.538354f );
		pCRTSEL[1]->SetOrientation( true );


		pEventTimerMode->SetInitialAnimState( 0.5f );
		pEventTimerMode->DefineSwitchGroup( GRP_C2B9_VC );
		pEventTimerMode->SetReference( _V( 0.0, 1.801141, 14.3961 ), switch_rot );
		pEventTimerMode->SetMouseRegion( 0.299141f, 0.727767f, 0.335464f, 0.850107f );
		pEventTimerMode->SetSpringLoaded( true, 0 );

		pEventTimerControl->SetInitialAnimState( 0.5f );
		pEventTimerControl->DefineSwitchGroup( GRP_C2B10_VC );
		pEventTimerControl->SetReference( _V( -0.07, 1.801141, 14.3961 ), switch_rot );
		pEventTimerControl->SetMouseRegion( 0.353268f, 0.726625f, 0.390057f, 0.850045f );
		pEventTimerControl->SetSpringLoaded( true, 0 );
		pEventTimerControl->SetSpringLoaded( true, 2 );

		pTimer->SetInitialAnimState( 0.5f );
		pTimer->DefineSwitchGroup( GRP_C2B11_VC );
		pTimer->SetReference( _V( 0.0994235, 1.801141, 14.3961 ), switch_rot );
		pTimer->SetMouseRegion( 0.672017f, 0.727122f, 0.709002f, 0.851895f );
		pTimer->SetSpringLoaded( true, 0 );
		pTimer->SetSpringLoaded( true, 2 );
		return;
	}

	void PanelC2::RegisterVC()
	{
		AtlantisPanel::RegisterVC();
		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea( AID_C2, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP );
		oapiVCSetAreaClickmode_Quadrilateral( AID_C2, 
			_V( -0.2725, 1.95676, 14.5416 ) + ofs, _V( 0.2683, 1.95676, 14.5416 ) + ofs, 
			_V( -0.2725, 1.7619, 14.3552 ) + ofs, _V( 0.2683, 1.7619, 14.3552 ) + ofs );

		SURFHANDLE digittex = oapiGetTextureHandle( STS()->hOrbiterVCMesh, TEX_CLOCKNUMS_VC );
		oapiVCRegisterArea( AID_C2_WND0, _R( 0, 0, 63, 63 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digittex );
		oapiVCRegisterArea( AID_C2_WND1, _R( 64, 0, 127, 63 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digittex );
		oapiVCRegisterArea( AID_C2_WND2, _R( 128, 0, 191, 63 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digittex );
		oapiVCRegisterArea( AID_C2_WND3, _R( 192, 0, 255, 63 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digittex );
		return;
	}

	void PanelC2::DefineVCAnimations( UINT vcidx )
	{
		static VECTOR3 switch_rot = {-1,0,0};
		static VECTOR3 wheel_rot = {1.0, 0, 0};

		static UINT VC_C2Evt10MW_Grp = GRP_C2TOG1_VC;

		static MGROUP_ROTATE VC_C2Evt10MW (vcidx, &VC_C2Evt10MW_Grp, 1,
			_V( 0.131577, 1.80661, 14.42035 ), wheel_rot, (float)(360.0*RAD));
		anim_VC_C2Wheel[0]=STS()->CreateAnimation (0.0);
		STS()->AddAnimationComponent (anim_VC_C2Wheel[0], 0, 1, &VC_C2Evt10MW);

		static UINT VC_C2Evt1MW_Grp = GRP_C2TOG2_VC;

		static MGROUP_ROTATE VC_C2Evt1MW (vcidx, &VC_C2Evt1MW_Grp, 1,
			_V( 0.131577, 1.80661, 14.42035 ), wheel_rot, (float)(360.0*RAD));
		anim_VC_C2Wheel[1]=STS()->CreateAnimation (0.0);
		STS()->AddAnimationComponent (anim_VC_C2Wheel[1], 0, 1, &VC_C2Evt1MW);

		static UINT VC_C2Evt10SW_Grp = GRP_C2TOG3_VC;

		static MGROUP_ROTATE VC_C2Evt10SW (vcidx, &VC_C2Evt10SW_Grp, 1,
			_V( 0.131577, 1.80661, 14.42035 ), wheel_rot, (float)(360.0*RAD));
		anim_VC_C2Wheel[2]=STS()->CreateAnimation (0.0);
		STS()->AddAnimationComponent (anim_VC_C2Wheel[2], 0, 1, &VC_C2Evt10SW);

		static UINT VC_C2Evt1SW_Grp = GRP_C2TOG4_VC;

		static MGROUP_ROTATE VC_C2Evt1SW (vcidx, &VC_C2Evt1SW_Grp, 1,
			_V( 0.131577, 1.80661, 14.42035 ), wheel_rot, (float)(360.0*RAD));
		anim_VC_C2Wheel[3]=STS()->CreateAnimation (0.0);
		STS()->AddAnimationComponent (anim_VC_C2Wheel[3], 0, 1, &VC_C2Evt1SW);

		AtlantisPanel::DefineVCAnimations( vcidx );
		return;
	}

	void PanelC2::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "C2_R11_IDP", 14 );
		pIDPCRTPower[0]->output.Connect( pBundle, 0 );
		pIDPCRTPower[1]->output.Connect( pBundle, 1 );
		pIDPCRTPower[2]->output.Connect( pBundle, 2 );

		pIDPCRTMajFunc[0]->ConnectPort( 1, pBundle, 4 );
		pIDPCRTMajFunc[1]->ConnectPort( 1, pBundle, 5 );
		pIDPCRTMajFunc[2]->ConnectPort( 1, pBundle, 6 );

		pIDPCRTMajFunc[0]->ConnectPort( 2, pBundle, 8 );
		pIDPCRTMajFunc[1]->ConnectPort( 2, pBundle, 9 );
		pIDPCRTMajFunc[2]->ConnectPort( 2, pBundle, 10 );

		pCRTSEL[0]->output.Connect( pBundle, 12 );
		pCRTSEL[1]->output.Connect( pBundle, 13 );


		pBundle = STS()->BundleManager()->CreateBundle( "C2_EventTimer", 6 );
		pEventTimerMode->ConnectPort( 1, pBundle, 0 );
		pEventTimerMode->ConnectPort( 2, pBundle, 1 );
		pEventTimerControl->ConnectPort( 1, pBundle, 2 );
		pEventTimerControl->ConnectPort( 2, pBundle, 3 );
		pTimer->ConnectPort( 1, pBundle, 4 );
		pTimer->ConnectPort( 2, pBundle, 5 );
		pEventTimerMode_Up.Connect( pBundle, 1 );
		pEventTimerMode_Test.Connect( pBundle, 0 );
		pEventTimerControl_Start.Connect( pBundle, 3 );
		pEventTimerControl_Stop.Connect( pBundle, 2 );
		pTimer_Set.Connect( pBundle, 5 );
		pTimer_Reset.Connect( pBundle, 4 );


		pKeyboardCDR->ConnectIDP( 0, STS()->GetIDP( 1 ) );
		pKeyboardCDR->ConnectIDP( 1, STS()->GetIDP( 3 ) );

		pKeyboardPLT->ConnectIDP( 0, STS()->GetIDP( 3 ) );
		pKeyboardPLT->ConnectIDP( 1, STS()->GetIDP( 2 ) );

		AtlantisPanel::Realize();
		return;
	}
};
