#include "PanelA6U.h"
#include "..\Atlantis.h"
#include "..\meshres_vc_a6u.h"
#include "../Atlantis_defs.h"
#include "..\CommonDefs.h"


extern GDIParams g_Param;


namespace vc
{
	PanelA6U::PanelA6U(Atlantis* _sts):AtlantisPanel(_sts, "A6U")
	{
		char cbuf[255];

		hPanelMesh = oapiLoadMeshGlobal( DEFAULT_MESHNAME_PANELA6U );
		mesh_index = MESH_UNDEFINED;

		Add(pSense=new StdSwitch2(_sts, "SENSE"));
		Add(pFltCntlrPower = new LockableLever2(_sts, "Aft Flt Cntlr Pwr"));
		Add(pPayloadRetentionLogicPowerSys[0] = new StdSwitch2( _sts, "Payload Retention Logic Power Sys 1" ) );
		Add(pPayloadRetentionLogicPowerSys[1] = new StdSwitch2( _sts, "Payload Retention Logic Power Sys 2" ) );
		Add(pPayloadRetentionLatches[0] = new StdSwitch3(_sts, "Payload Ret Latch 1"));
		Add(pPayloadRetentionLatches[1] = new StdSwitch3(_sts, "Payload Ret Latch 2"));
		Add(pPayloadRetentionLatches[2] = new StdSwitch3(_sts, "Payload Ret Latch 3"));
		Add(pPayloadRetentionLatches[3] = new StdSwitch3(_sts, "Payload Ret Latch 4"));
		Add(pPayloadRetentionLatches[4] = new StdSwitch3(_sts, "Payload Ret Latch 5"));
		Add(pPayloadSelect = new RotaryDemuxSwitch(_sts, "Payload Select", 5));
		Add( pLatchState[0] = new StandardTalkback3( _sts, "Payload Ret Latch1 Tkbk" ) );
		Add( pLatchState[1] = new StandardTalkback3( _sts, "Payload Ret Latch2 Tkbk" ) );
		Add( pLatchState[2] = new StandardTalkback3( _sts, "Payload Ret Latch3 Tkbk" ) );
		Add( pLatchState[3] = new StandardTalkback3( _sts, "Payload Ret Latch4 Tkbk" ) );
		Add( pLatchState[4] = new StandardTalkback3( _sts, "Payload Ret Latch5 Tkbk" ) );
		Add( pLatchRTL[0] = new StandardTalkback2( _sts, "Payload Latch1 RTL" ) );
		Add( pLatchRTL[1] = new StandardTalkback2( _sts, "Payload Latch2 RTL" ) );
		Add( pLatchRTL[2] = new StandardTalkback2( _sts, "Payload Latch3 RTL" ) );
		Add( pLatchRTL[3] = new StandardTalkback2( _sts, "Payload Latch4 RTL" ) );
		Add( pLatchRTL[4] = new StandardTalkback2( _sts, "Payload Latch5 RTL" ) );
		Add( pADIAttitude = new StdSwitch3( _sts, "ADI Attitude" ) );
		Add( pADIError = new StdSwitch3( _sts, "ADI Error" ) );
		Add( pADIRate = new StdSwitch3( _sts, "ADI Rate" ) );

		Add( pAnnunciatorBusSelect = new StdSwitch3( _sts, "Annunciator Bus Select" ) );
		pAnnunciatorBusSelect->SetLabel( 0, "MN C" );
		pAnnunciatorBusSelect->SetLabel( 1, "OFF" );
		pAnnunciatorBusSelect->SetLabel( 2, "MN B" );

		Add( pAnnunciatorLampTest = new StdSwitch3( _sts, "Annunciator Lamp Test" ) );

		Add( pEventTimerMode = new StdSwitch3( _sts, "Event Timer Mode" ) );
		Add( pEventTimerControl = new StdSwitch3( _sts, "Event Timer Control" ) );
		Add( pEventTimerTimer = new StdSwitch3( _sts, "Event Timer Timer" ) );

		for(int i=0;i<24;i++) {
			sprintf_s(cbuf, 255, "%d", i+1);
			std::string name="A6U_PBI";
			name+=cbuf;
			Add(pPBIs[i]=new PushButtonIndicatorSingleLight(_sts, name));
		}

		pSense->SetLabel(0, "-X");
		pSense->SetLabel(1, "-Z");
		pFltCntlrPower->SetLabel(0, "OFF");
		pFltCntlrPower->SetLabel(1, "ON");

		pPayloadRetentionLogicPowerSys[0]->SetLabel( 0, "OFF" );
		pPayloadRetentionLogicPowerSys[0]->SetLabel( 1, "ON" );
		pPayloadRetentionLogicPowerSys[1]->SetLabel( 0, "OFF" );
		pPayloadRetentionLogicPowerSys[1]->SetLabel( 1, "ON" );

		for(int i=0;i<5;i++) {
			pPayloadRetentionLatches[i]->SetLabel(0, "LATCH");
			pPayloadRetentionLatches[i]->SetLabel(1, "OFF");
			pPayloadRetentionLatches[i]->SetLabel(2, "RELEASE");
		}
		pPayloadSelect->SetLabel(0, "3");
		pPayloadSelect->SetLabel(1, "MON3");
		pPayloadSelect->SetLabel(2, "2");
		pPayloadSelect->SetLabel(3, "MON1");
		pPayloadSelect->SetLabel(4, "1");

		pADIAttitude->SetLabel( 0, "REF" );
		pADIAttitude->SetLabel( 1, "LVLH" );
		pADIAttitude->SetLabel( 2, "INRTL" );
		pADIError->SetLabel( 0, "LOW" );
		pADIError->SetLabel( 1, "MED" );
		pADIError->SetLabel( 2, "HIGH" );
		pADIRate->SetLabel( 0, "LOW" );
		pADIRate->SetLabel( 1, "MED" );
		pADIRate->SetLabel( 2, "HIGH" );
		
		pEventTimerMode->SetLabel( 1, "DOWN" );
		pEventTimerMode->SetLabel( 2, "UP" );

		for (int i = 0; i < 4; i++)
		{
			tgtwheel_state[i] = 0;
			wheelState[i] = 0.0;
			wheelnumber[i] = 0;
			oldwheelnumber[i] = 9;
		}
	}

	PanelA6U::~PanelA6U()
	{
	}

	void PanelA6U::AddMeshes( const VECTOR3 &ofs )
	{
		SetHasOwnVCMesh();

		if (mesh_index == MESH_UNDEFINED)
		{
			mesh_index = STS()->AddMesh( hPanelMesh, &ofs );
			STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		}
		return;
	}

	void PanelA6U::SetMeshVisibility( bool visible )
	{
		if (visible) STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		else STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_NEVER );
		return;
	}

	UINT PanelA6U::GetVCMeshIndex( void ) const
	{
		return mesh_index;
	}

	bool PanelA6U::OnVCMouseEvent( int id, int _event, VECTOR3 &p )
	{
		if (id != AID_A6U) return false;
		
		if (_event & PANEL_MOUSE_LBDOWN)
		{
			if(p.x> 0.243644 && p.x < 0.264384)
			{
				if(p.y > 0.695075 && p.y < 0.723049)
				{
					tgtwheel_state[0] = tgtwheel_state[0] + 0.25;
					return true;
				}
				else if(p.y > 0.747519 && p.y < 0.773890)
				{
					tgtwheel_state[0] -= 0.25;
					if(tgtwheel_state[0] < 0) tgtwheel_state[0] +=EVTTMR_WHEELMAX_A6[0];
					return true;
				}
			}
			else if(p.x> 0.203010 && p.x < 0.225550)
			{
				if(p.y > 0.695075 && p.y < 0.723049)
				{
					tgtwheel_state[1] = tgtwheel_state[1] + 0.25;
					return true;
				}
				else if(p.y > 0.747519 && p.y < 0.773890)
				{
					tgtwheel_state[1] -= 0.25;
					if(tgtwheel_state[1] < 0) tgtwheel_state[1] +=EVTTMR_WHEELMAX_A6[1];
					return true;
				}
			}
			else if(p.x> 0.163198 && p.x < 0.186210)
			{
				if(p.y > 0.695075 && p.y < 0.723049)
				{
					tgtwheel_state[2] = tgtwheel_state[2] + 0.25;
					return true;
				}
				else if(p.y > 0.747519 && p.y < 0.773890)
				{
					tgtwheel_state[2] -= 0.25;
					if(tgtwheel_state[2] < 0) tgtwheel_state[2] +=EVTTMR_WHEELMAX_A6[2];
					return true;
				}
			}
			else if(p.x> 0.122897 && p.x < 0.146372)
			{
				if(p.y > 0.695075 && p.y < 0.723049)
				{
					tgtwheel_state[3] = tgtwheel_state[3] + 0.25;
					return true;
				}
				else if(p.y > 0.747519 && p.y < 0.773890)
				{
					tgtwheel_state[3] -= 0.25;
					if(tgtwheel_state[3] < 0) tgtwheel_state[3] +=EVTTMR_WHEELMAX_A6[3];
					return true;
				}
			}
		}
		return AtlantisPanel::OnVCMouseEvent( id, _event, p );
	}

	bool PanelA6U::OnVCRedrawEvent( int id, int _event, SURFHANDLE surf )
	{
		const int NUMX[10] = {0, 64, 128, 192, 0, 64, 128, 192, 0, 64};
		const int NUMY[10] = {0, 0, 0, 0, 64, 64, 64, 64, 128, 128};
	
		switch (id)
		{
			case AID_A6U_WND0:
				oapiBlt(surf, g_Param.clock_digits, 0,0, NUMX[wheelnumber[0]], NUMY[wheelnumber[0]], 63, 63);
				return true;
			case AID_A6U_WND1:
				oapiBlt(surf, g_Param.clock_digits, 0,0, NUMX[wheelnumber[1]], NUMY[wheelnumber[1]], 63, 63);
				return true;
			case AID_A6U_WND2:
				oapiBlt(surf, g_Param.clock_digits, 0,0, NUMX[wheelnumber[2]], NUMY[wheelnumber[2]], 63, 63);
				return true;
			case AID_A6U_WND3:
				oapiBlt(surf, g_Param.clock_digits, 0,0, NUMX[wheelnumber[3]], NUMY[wheelnumber[3]], 63, 63);
				return true;
		}
		return AtlantisPanel::OnVCRedrawEvent( id, _event, surf );
	}

	void PanelA6U::DefineVC()
	{
		const VECTOR3 switch_rot =  _V(-1, 0, 0);
		const VECTOR3 switch_pull = _V(0, 0.3126, 0.9499);
		const VECTOR3 push_dir = -switch_pull;
		oapiWriteLog("PanelA6U: DefineVC called");

		AddAIDToMouseEventList(AID_A6U);

		pSense->DefineSwitchGroup(GRP_S1_A6U_VC);
		pSense->SetInitialAnimState(0.5);
		pSense->SetReference(_V(0.728, 2.780, 12.313), switch_rot);
		pSense->SetMouseRegion(0.863499f, 0.262889f, 0.924675f, 0.332972f);

		pFltCntlrPower->DefineSwitchGroup(GRP_S2_A6U_VC);
		pFltCntlrPower->SetInitialAnimState(0.5);
		pFltCntlrPower->SetReference(_V(0.686, 2.781, 12.313), switch_rot);
		pFltCntlrPower->SetPullDirection(switch_pull);
		pFltCntlrPower->SetMouseRegion(0.772155f, 0.254109f, 0.836383f, 0.348049f);

		pPayloadRetentionLogicPowerSys[0]->DefineSwitchGroup( GRP_S46_A6U_VC );
		pPayloadRetentionLogicPowerSys[0]->SetInitialAnimState( 0.5f );
		pPayloadRetentionLogicPowerSys[0]->SetReference( _V( 0.7303, 2.6068, 12.3675 ), switch_rot );
		pPayloadRetentionLogicPowerSys[0]->SetMouseRegion( 0.881104f, 0.721166f, 0.917138f, 0.779604f );

		pPayloadRetentionLogicPowerSys[1]->DefineSwitchGroup( GRP_S47_A6U_VC );
		pPayloadRetentionLogicPowerSys[1]->SetInitialAnimState( 0.5f );
		pPayloadRetentionLogicPowerSys[1]->SetReference( _V( 0.6757, 2.6068, 12.3675 ), switch_rot );
		pPayloadRetentionLogicPowerSys[1]->SetMouseRegion( 0.775442f, 0.714653f, 0.813627f, 0.779674f );

		pPayloadRetentionLatches[0]->DefineSwitchGroup(GRP_S36_A6U_VC);
		pPayloadRetentionLatches[0]->SetInitialAnimState(0.5);
		pPayloadRetentionLatches[0]->SetReference(_V(0.618, 2.555, 12.383), switch_rot);
		pPayloadRetentionLatches[0]->SetMouseRegion(0.651142f, 0.844718f, 0.710472f, 0.935115f);
		
		pPayloadRetentionLatches[1]->DefineSwitchGroup(GRP_S42_A6U_VC);
		pPayloadRetentionLatches[1]->SetInitialAnimState(0.5);
		pPayloadRetentionLatches[1]->SetReference(_V(0.582, 2.555, 12.383), switch_rot);
		pPayloadRetentionLatches[1]->SetMouseRegion(0.578245f, 0.847014f, 0.640193f, 0.934493f);
		
		pPayloadRetentionLatches[2]->DefineSwitchGroup(GRP_S43_A6U_VC);
		pPayloadRetentionLatches[2]->SetInitialAnimState(0.5);
		pPayloadRetentionLatches[2]->SetReference(_V(0.543, 2.555, 12.383), switch_rot);
		pPayloadRetentionLatches[2]->SetMouseRegion(0.507261f, 0.844424f, 0.569764f, 0.932009f);
		
		pPayloadRetentionLatches[3]->DefineSwitchGroup(GRP_S44_A6U_VC);
		pPayloadRetentionLatches[3]->SetInitialAnimState(0.5);
		pPayloadRetentionLatches[3]->SetReference(_V(0.505, 2.554, 12.383), switch_rot);
		pPayloadRetentionLatches[3]->SetMouseRegion(0.432613f, 0.846585f, 0.492668f, 0.936958f);
		
		pPayloadRetentionLatches[4]->DefineSwitchGroup(GRP_S45_A6U_VC);
		pPayloadRetentionLatches[4]->SetInitialAnimState(0.5);
		pPayloadRetentionLatches[4]->SetReference(_V(0.468, 2.554, 12.383), switch_rot);
		pPayloadRetentionLatches[4]->SetMouseRegion(0.356811f, 0.845240f, 0.421112f, 0.939906f);

		pPayloadSelect->DefineSwitchGroup(GRP_S37_A6U_VC);
		pPayloadSelect->SetInitialAnimState(1.0f);
		pPayloadSelect->SetReference(_V(0.7079, 2.5535, 12.3926), -switch_pull);
		pPayloadSelect->DefineRotationAngle(120.0f);
		pPayloadSelect->SetOffset(-60.0f);
		pPayloadSelect->SetMouseRegion(0.811500f, 0.842076f, 0.881687f, 0.940961f);

		pADIAttitude->DefineSwitchGroup( GRP_S5_A6U_VC );
		pADIAttitude->SetInitialAnimState( 0.5 );
		pADIAttitude->SetReference( _V( 0.6908, 2.6717, 12.3472 ), switch_rot );
		pADIAttitude->SetMouseRegion( 0.889962f, 0.547700f, 0.925561f, 0.612007f );

		pADIError->DefineSwitchGroup( GRP_S6_A6U_VC );
		pADIError->SetInitialAnimState( 0.5 );
		pADIError->SetReference( _V( 0.6908, 2.6717, 12.3472 ), switch_rot );
		pADIError->SetMouseRegion( 0.802425f, 0.547700f, 0.841630f, 0.612007f );

		pADIRate->DefineSwitchGroup( GRP_S7_A6U_VC );
		pADIRate->SetInitialAnimState( 0.5 );
		pADIRate->SetReference( _V( 0.6908, 2.6717, 12.3472 ), switch_rot );
		pADIRate->SetMouseRegion( 0.749505f, 0.547700f, 0.783608f, 0.612007f );

		for (int i = 0; i < 24; i++)
		{
			pPBIs[i]->SetStateOffset( 1, 0.0f, 0.488281f );
			pPBIs[i]->SetDirection( push_dir );
		}

		// switch number: original name (current name)
		pPBIs[0]->SetMouseRegion( 0.656747f, 0.111248f, 0.699463f, 0.164429f );// S8: SELECT - A (SELECT - A)
		pPBIs[0]->DefineMeshGroup( mesh_index, GRP_S8_A6U_VC );

		pPBIs[1]->SetMouseRegion( 0.603330f, 0.109704f, 0.643641f, 0.164394f );// S9: SELECT - B (SELECT - B)
		pPBIs[1]->DefineMeshGroup( mesh_index, GRP_S9_A6U_VC );

		pPBIs[2]->SetMouseRegion( 0.549166f, 0.107301f, 0.590951f, 0.163038f );// S10: CONTROL - AUTO (CONTROL - AUTO)
		pPBIs[2]->DefineMeshGroup( mesh_index, GRP_S10_A6U_VC );

		pPBIs[3]->SetMouseRegion( 0.477215f, 0.107541f, 0.519102f, 0.163411f );// S11: CONTROL - MAN (CONTROL - INRTL)
		pPBIs[3]->DefineMeshGroup( mesh_index, GRP_S11_A6U_VC );

		pPBIs[4]->SetMouseRegion( 0.423618f, 0.107813f, 0.465852f, 0.163261f );// S12: RCS JETS - NORM (CONTROL - LVLH)
		pPBIs[4]->DefineMeshGroup( mesh_index, GRP_S12_A6U_VC );

		pPBIs[5]->SetMouseRegion( 0.370853f, 0.107513f, 0.412549f, 0.163068f );// S13: RCS JETS - VERN (CONTROL - FREE)
		pPBIs[5]->DefineMeshGroup( mesh_index, GRP_S13_A6U_VC );

		pPBIs[6]->SetMouseRegion( 0.655811f, 0.269728f, 0.697522f, 0.324154f );// S14: TRANSLATION X - HIGH ()
		pPBIs[6]->DefineMeshGroup( mesh_index, GRP_S14_A6U_VC );

		pPBIs[7]->SetMouseRegion( 0.603035f, 0.270625f, 0.643061f, 0.326714f );// S15: TRANSLATION Y - HIGH (TRANSLATION Y - LOW Z)
		pPBIs[7]->DefineMeshGroup( mesh_index, GRP_S15_A6U_VC );

		pPBIs[8]->SetMouseRegion( 0.549628f, 0.270656f, 0.589814f, 0.325715f );// S16: TRANSLATION Z - HIGH (TRANSLATION Z - HIGH Z)
		pPBIs[8]->DefineMeshGroup( mesh_index, GRP_S16_A6U_VC );

		pPBIs[9]->SetMouseRegion( 0.477534f, 0.268889f, 0.519005f, 0.324256f );// S17: ROTATION ROLL - DISC RATE (ROTATION ROLL - PRI)
		pPBIs[9]->DefineMeshGroup( mesh_index, GRP_S17_A6U_VC );

		pPBIs[10]->SetMouseRegion( 0.424007f, 0.269315f, 0.465630f, 0.324746f );// S18: ROTATION PITCH - DISC RATE (ROTATION PITCH - ALT)
		pPBIs[10]->DefineMeshGroup( mesh_index, GRP_S18_A6U_VC );

		pPBIs[11]->SetMouseRegion( 0.370579f, 0.268778f, 0.412773f, 0.324512f );// S19: ROTATION YAW - DISC RATE (ROTATION YAW - VERN)
		pPBIs[11]->DefineMeshGroup( mesh_index, GRP_S19_A6U_VC );

		pPBIs[12]->SetMouseRegion( 0.655404f, 0.351390f, 0.697885f, 0.406857f );// S20: TRANSLATION X - NORM (TRANSLATION X - NORM)
		pPBIs[12]->DefineMeshGroup( mesh_index, GRP_S20_A6U_VC );

		pPBIs[13]->SetMouseRegion( 0.602520f, 0.351301f, 0.644462f, 0.406483f );// S21: TRANSLATION Y - NORM (TRANSLATION Y - NORM)
		pPBIs[13]->DefineMeshGroup( mesh_index, GRP_S21_A6U_VC );

		pPBIs[14]->SetMouseRegion( 0.549848f, 0.351591f, 0.591103f, 0.407040f );// S22: TRANSLATION Z - NORM (TRANSLATION Z - NORM)
		pPBIs[14]->DefineMeshGroup( mesh_index, GRP_S22_A6U_VC );

		pPBIs[15]->SetMouseRegion( 0.477944f, 0.352003f, 0.519287f, 0.406825f );// S23: ROTATION ROLL - ACCEL (ROTATION ROLL - DISC RATE)
		pPBIs[15]->DefineMeshGroup( mesh_index, GRP_S23_A6U_VC );

		pPBIs[16]->SetMouseRegion( 0.423974f, 0.351234f, 0.465760f, 0.406969f );// S24: ROTATION PITCH - ACCEL (ROTATION PITCH - DISC RATE)
		pPBIs[16]->DefineMeshGroup( mesh_index, GRP_S24_A6U_VC );

		pPBIs[17]->SetMouseRegion( 0.371329f, 0.351780f, 0.412639f, 0.406656f );// S25: ROTATION YAW - ACCEL (ROTATION YAW - DISC RATE)
		pPBIs[17]->DefineMeshGroup( mesh_index, GRP_S25_A6U_VC );

		pPBIs[18]->SetMouseRegion( 0.656163f, 0.428680f, 0.697269f, 0.483150f );// S26: TRANSLATION X - PULSE (TRANSLATION X - PULSE)
		pPBIs[18]->DefineMeshGroup( mesh_index, GRP_S26_A6U_VC );

		pPBIs[19]->SetMouseRegion( 0.602784f, 0.428556f, 0.643567f, 0.483464f );// S27: TRANSLATION Y - PULSE (TRANSLATION Y - PULSE)
		pPBIs[19]->DefineMeshGroup( mesh_index, GRP_S27_A6U_VC );

		pPBIs[20]->SetMouseRegion( 0.549332f, 0.428658f, 0.590492f, 0.483918f );// S28: TRANSLATION Z - PULSE (TRANSLATION Z - PULSE)
		pPBIs[20]->DefineMeshGroup( mesh_index, GRP_S28_A6U_VC );

		pPBIs[21]->SetMouseRegion( 0.477266f, 0.428112f, 0.518667f, 0.483672f );// S29: ROTATION ROLL - PULSE (ROTATION ROLL - PULSE)
		pPBIs[21]->DefineMeshGroup( mesh_index, GRP_S29_A6U_VC );

		pPBIs[22]->SetMouseRegion( 0.424548f, 0.428484f, 0.465727f, 0.483587f );// S30: ROTATION PITCH - PULSE (ROTATION PITCH - PULSE)
		pPBIs[22]->DefineMeshGroup( mesh_index, GRP_S30_A6U_VC );

		pPBIs[23]->SetMouseRegion( 0.370621f, 0.428480f, 0.412666f, 0.484279f );// S31: ROTATION YAW - PULSE (ROTATION YAW - PULSE)
		pPBIs[23]->DefineMeshGroup( mesh_index, GRP_S31_A6U_VC );

		pLatchRTL[0]->DefineMeshGroups( mesh_index, GRP_DS2_U_A6U_VC, GRP_DS2_L_A6U_VC );
		pLatchRTL[1]->DefineMeshGroups( mesh_index, GRP_DS3_U_A6U_VC, GRP_DS3_L_A6U_VC );
		pLatchRTL[2]->DefineMeshGroups( mesh_index, GRP_DS4_U_A6U_VC, GRP_DS4_L_A6U_VC );
		pLatchRTL[3]->DefineMeshGroups( mesh_index, GRP_DS12_U_A6U_VC, GRP_DS12_L_A6U_VC );
		pLatchRTL[4]->DefineMeshGroups( mesh_index, GRP_DS13_U_A6U_VC, GRP_DS13_L_A6U_VC );

		pLatchState[0]->DefineMeshGroup( mesh_index, GRP_DS1_A6U_VC );
		pLatchState[1]->DefineMeshGroup( mesh_index, GRP_DS8_A6U_VC );
		pLatchState[2]->DefineMeshGroup( mesh_index, GRP_DS9_A6U_VC );
		pLatchState[3]->DefineMeshGroup( mesh_index, GRP_DS10_A6U_VC );
		pLatchState[4]->DefineMeshGroup( mesh_index, GRP_DS11_A6U_VC );

		pAnnunciatorBusSelect->SetInitialAnimState( 0.5f );
		pAnnunciatorBusSelect->DefineSwitchGroup( GRP_S33_A6U_VC );
		pAnnunciatorBusSelect->SetReference( _V( 0.4093, 2.7248, 12.3307 ), switch_rot );
		pAnnunciatorBusSelect->SetMouseRegion( 0.267251f, 0.410992f, 0.309207f, 0.479933f );
		pAnnunciatorBusSelect->SetInitialPosition( 0 );

		pAnnunciatorLampTest->SetInitialAnimState( 0.5f );
		pAnnunciatorLampTest->DefineSwitchGroup( GRP_S34_A6U_VC );
		pAnnunciatorLampTest->SetReference( _V( 0.4093, 2.7248, 12.3307 ), switch_rot );
		pAnnunciatorLampTest->SetMouseRegion( 0.209122f, 0.409257f, 0.255242f, 0.476381f );
		pAnnunciatorLampTest->SetSpringLoaded( true, 0 );
		pAnnunciatorLampTest->SetSpringLoaded( true, 2 );

		pEventTimerMode->SetInitialAnimState( 0.5f );
		pEventTimerMode->DefineSwitchGroup( GRP_S39_A6U_VC );
		pEventTimerMode->SetReference( _V( 0.3962, 2.5547, 12.3837 ), switch_rot );
		pEventTimerMode->SetMouseRegion( 0.244218f, 0.854629f, 0.282415f, 0.920201f );
		pEventTimerMode->SetSpringLoaded( true, 0 );

		pEventTimerControl->SetInitialAnimState( 0.5f );
		pEventTimerControl->DefineSwitchGroup( GRP_S40_A6U_VC );	
		pEventTimerControl->SetReference( _V( 0.3667, 2.5549, 12.3836 ), switch_rot );
		pEventTimerControl->SetMouseRegion( 0.184202f, 0.857520f, 0.230935f, 0.922421f );
		pEventTimerControl->SetSpringLoaded( true, 0 );
		pEventTimerControl->SetSpringLoaded( true, 2 );

		pEventTimerTimer->SetInitialAnimState( 0.5f );
		pEventTimerTimer->DefineSwitchGroup( GRP_S41_A6U_VC );
		pEventTimerTimer->SetReference( _V( 0.3392, 2.5549, 12.3837 ), switch_rot );
		pEventTimerTimer->SetMouseRegion( 0.134762f, 0.857368f, 0.171836f, 0.923072f );
		pEventTimerTimer->SetSpringLoaded( true, 0 );
		pEventTimerTimer->SetSpringLoaded( true, 2 );
		return;
	}

	void PanelA6U::RegisterVC()
	{
		AtlantisPanel::RegisterVC();
		oapiWriteLog("PanelA6U: RegisterVC called");

		VECTOR3 ofs=STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea(AID_A6U, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
		oapiVCSetAreaClickmode_Quadrilateral(AID_A6U,
			_V(0.262, 2.892, 12.277)+ofs, _V(0.789, 2.892, 12.277)+ofs,
			_V(0.262, 2.511, 12.397)+ofs, _V(0.789, 2.511, 12.397)+ofs);

		// event timer wheel numbers
		SURFHANDLE digittex = oapiGetTextureHandle( hPanelMesh, TEX_SSU_CLOCKNUMS_A6U_VC );
		oapiVCRegisterArea( AID_A6U_WND0, _R( 0, 64, 63, 127 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digittex );
		oapiVCRegisterArea( AID_A6U_WND1, _R( 64, 64, 127, 127 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digittex );
		oapiVCRegisterArea( AID_A6U_WND2, _R( 128, 64, 191, 127 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digittex );
		oapiVCRegisterArea( AID_A6U_WND3, _R( 192, 64, 255, 127 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digittex );
	}

	void PanelA6U::DefineVCAnimations( UINT vcidx )
	{
		static VECTOR3 wheel_rot = {-1.0, 0, 0};

		static UINT VC_A6Evt10MW_Grp = GRP_S38_THUMBWHEEL1_A6U_VC;

		static MGROUP_ROTATE VC_A6Evt10MW (mesh_index, &VC_A6Evt10MW_Grp, 1,
			_V( 0.0, 2.6080, 12.3532 ), wheel_rot, (float)(360.0*RAD));
		anim_VC_A6Wheel[0]=STS()->CreateAnimation (0.0);
		STS()->AddAnimationComponent (anim_VC_A6Wheel[0], 0, 1, &VC_A6Evt10MW);

		static UINT VC_A6Evt1MW_Grp = GRP_S38_THUMBWHEEL2_A6U_VC;

		static MGROUP_ROTATE VC_A6Evt1MW (mesh_index, &VC_A6Evt1MW_Grp, 1,
			_V( 0.0, 2.6080, 12.3532 ), wheel_rot, (float)(360.0*RAD));
		anim_VC_A6Wheel[1]=STS()->CreateAnimation (0.0);
		STS()->AddAnimationComponent (anim_VC_A6Wheel[1], 0, 1, &VC_A6Evt1MW);

		static UINT VC_A6Evt10SW_Grp = GRP_S38_THUMBWHEEL3_A6U_VC;

		static MGROUP_ROTATE VC_A6Evt10SW (mesh_index, &VC_A6Evt10SW_Grp, 1,
			_V( 0.0, 2.6080, 12.3532 ), wheel_rot, (float)(360.0*RAD));
		anim_VC_A6Wheel[2]=STS()->CreateAnimation (0.0);
		STS()->AddAnimationComponent (anim_VC_A6Wheel[2], 0, 1, &VC_A6Evt10SW);

		static UINT VC_A6Evt1SW_Grp = GRP_S38_THUMBWHEEL4_A6U_VC;

		static MGROUP_ROTATE VC_A6Evt1SW (mesh_index, &VC_A6Evt1SW_Grp, 1,
			_V( 0.0, 2.6080, 12.3532 ), wheel_rot, (float)(360.0*RAD));
		anim_VC_A6Wheel[3]=STS()->CreateAnimation (0.0);
		STS()->AddAnimationComponent (anim_VC_A6Wheel[3], 0, 1, &VC_A6Evt1SW);

		AtlantisPanel::DefineVCAnimations( mesh_index );
		return;
	}

	void PanelA6U::Realize()
	{
		DiscreteBundle* pBundle=STS()->BundleManager()->CreateBundle("Controllers", 16);
		pSense->output.Connect(pBundle, 0);
		pFltCntlrPower->ConnectPort(1, pBundle, 3);

		pBundle=STS()->BundleManager()->CreateBundle("DAP_PBIS1", 16);
		for (int i = 0; i < 16; i++) pPBIs[i]->ConnectPushButton( pBundle, i );

		pBundle=STS()->BundleManager()->CreateBundle("DAP_PBIS2", 16);
		for (int i = 16; i < 24; i++) pPBIs[i]->ConnectPushButton( pBundle, i - 16 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA4_1", 16 );
		pPBIs[0]->ConnectLight( 0, pBundle, 1 );
		pPBIs[1]->ConnectLight( 0, pBundle, 5 );
		pPBIs[2]->ConnectLight( 0, pBundle, 9 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA4_2", 16 );
		pPBIs[6]->ConnectLight( 0, pBundle, 1 );
		pPBIs[7]->ConnectLight( 0, pBundle, 5 );
		pPBIs[8]->ConnectLight( 0, pBundle, 9 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA4_3", 16 );
		pPBIs[12]->ConnectLight( 0, pBundle, 1 );
		pPBIs[13]->ConnectLight( 0, pBundle, 5 );
		pPBIs[14]->ConnectLight( 0, pBundle, 9 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA4_4", 16 );
		pPBIs[18]->ConnectLight( 0, pBundle, 1 );
		pPBIs[19]->ConnectLight( 0, pBundle, 5 );
		pPBIs[20]->ConnectLight( 0, pBundle, 9 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA5_1", 16 );
		pPBIs[3]->ConnectLight( 0, pBundle, 1 );
		pPBIs[4]->ConnectLight( 0, pBundle, 5 );
		pPBIs[5]->ConnectLight( 0, pBundle, 9 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA5_2", 16 );
		pPBIs[9]->ConnectLight( 0, pBundle, 1 );
		pPBIs[10]->ConnectLight( 0, pBundle, 5 );
		pPBIs[11]->ConnectLight( 0, pBundle, 9 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA5_3", 16 );
		pPBIs[15]->ConnectLight( 0, pBundle, 1 );
		pPBIs[16]->ConnectLight( 0, pBundle, 5 );
		pPBIs[17]->ConnectLight( 0, pBundle, 9 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA5_4", 16 );
		pPBIs[21]->ConnectLight( 0, pBundle, 1 );
		pPBIs[22]->ConnectLight( 0, pBundle, 5 );
		pPBIs[23]->ConnectLight( 0, pBundle, 9 );

		pBundle = STS()->BundleManager()->CreateBundle("A6U_LATCHSWITCHES", 16);
		for(int i=0;i<5;i++) {
			LatchSwitch_Latch[i].Connect(pBundle, i);
			LatchSwitch_Release[i].Connect(pBundle, i+5);

			pPayloadRetentionLatches[i]->outputA.Connect(pBundle, i);
			pPayloadRetentionLatches[i]->outputB.Connect(pBundle, i+5);
		}
		// connect ports for payload select rotary
		PayloadSelect[0].Connect(pBundle, 10);
		pPayloadSelect->ConnectOutputSignal(4, pBundle, 10);
		PayloadSelect[1].Connect(pBundle, 11);
		pPayloadSelect->ConnectOutputSignal(2, pBundle, 11);
		PayloadSelect[2].Connect(pBundle, 12);
		pPayloadSelect->ConnectOutputSignal(0, pBundle, 12);
		// connect 2 MON ports
		pPayloadSelect->ConnectOutputSignal(3, pBundle, 13);
		pPayloadSelect->ConnectOutputSignal(1, pBundle, 14);
		pPayloadRetentionLogicPowerSys[0]->output.Connect( pBundle, 15 );// using available line
		PLRetLogicPwrSysSwitch[0].Connect( pBundle, 15 );
		
		pBundle = STS()->BundleManager()->CreateBundle("A6U_LATCH_TKBKS", 16);
		for(int i=0;i<5;i++) {
			Latch_LatchedTkbk[i].Connect(pBundle, i);
			Latch_ReleasedTkbk[i].Connect(pBundle, i+5);
			Latch_RTLTkbk[i].Connect(pBundle, i+10);

			pLatchState[i]->SetInput( 0, pBundle, i, TB_LAT );
			pLatchState[i]->SetInput( 1, pBundle, i + 5, TB_REL );
			pLatchRTL[i]->SetInput( pBundle, i + 10, TB_GRAY );
		}
		pPayloadRetentionLogicPowerSys[1]->output.Connect( pBundle, 15 );// using available line
		PLRetLogicPwrSysSwitch[1].Connect( pBundle, 15 );

		pBundle=STS()->BundleManager()->CreateBundle("LATCH0", 12);
		DiscreteBundle* pBundle2 = STS()->BundleManager()->CreateBundle("LATCH0_STATE", 15);
		for(int i=0;i<5;i++) {
			Latch_Latch[0][i].Connect(pBundle, 2*i);
			Latch_Release[0][i].Connect(pBundle, 2*i+1);

			Latch_Latched[0][i].Connect(pBundle2, 3*i);
			Latch_Released[0][i].Connect(pBundle2, 3*i+1);
			Latch_RTL[0][i].Connect(pBundle2, 3*i+2);
		}
		LogicPowerSys[0][0].Connect( pBundle, 10 );
		LogicPowerSys[0][1].Connect( pBundle, 11 );

		pBundle=STS()->BundleManager()->CreateBundle("LATCH1", 12);
		pBundle2 = STS()->BundleManager()->CreateBundle("LATCH1_STATE", 15);
		for(int i=0;i<5;i++) {
			Latch_Latch[1][i].Connect(pBundle, 2*i);
			Latch_Release[1][i].Connect(pBundle, 2*i+1);

			Latch_Latched[1][i].Connect(pBundle2, 3*i);
			Latch_Released[1][i].Connect(pBundle2, 3*i+1);
			Latch_RTL[1][i].Connect(pBundle2, 3*i+2);
		}
		LogicPowerSys[1][0].Connect( pBundle, 10 );
		LogicPowerSys[1][1].Connect( pBundle, 11 );

		pBundle=STS()->BundleManager()->CreateBundle("LATCH2", 12);
		pBundle2 = STS()->BundleManager()->CreateBundle("LATCH2_STATE", 15);
		for(int i=0;i<5;i++) {
			Latch_Latch[2][i].Connect(pBundle, 2*i);
			Latch_Release[2][i].Connect(pBundle, 2*i+1);

			Latch_Latched[2][i].Connect(pBundle2, 3*i);
			Latch_Released[2][i].Connect(pBundle2, 3*i+1);
			Latch_RTL[2][i].Connect(pBundle2, 3*i+2);
		}
		LogicPowerSys[2][0].Connect( pBundle, 10 );
		LogicPowerSys[2][1].Connect( pBundle, 11 );

		pBundle = STS()->BundleManager()->CreateBundle( "ADI_Switches_A6U", 6 );
		pADIAttitude->outputA.Connect( pBundle, 0 );// REF
		pADIAttitude->outputB.Connect( pBundle, 1 );// INRTL
		pADIError->outputA.Connect( pBundle, 2 );// LOW
		pADIError->outputB.Connect( pBundle, 3 );// HIGH
		pADIRate->outputA.Connect( pBundle, 4 );// LOW
		pADIRate->outputB.Connect( pBundle, 5 );// HIGH

		pBundle = STS()->BundleManager()->CreateBundle( "A6U_EventTimer", 6 );
		pEventTimerMode->ConnectPort( 1, pBundle, 0 );
		pEventTimerMode->ConnectPort( 2, pBundle, 1 );
		pEventTimerControl->ConnectPort( 1, pBundle, 2 );
		pEventTimerControl->ConnectPort( 2, pBundle, 3 );
		pEventTimerTimer->ConnectPort( 1, pBundle, 4 );
		pEventTimerTimer->ConnectPort( 2, pBundle, 5 );
		pEventTimerMode_Up.Connect( pBundle, 1 );
		pEventTimerMode_Test.Connect( pBundle, 0 );
		pEventTimerControl_Start.Connect( pBundle, 3 );
		pEventTimerControl_Stop.Connect( pBundle, 2 );
		pEventTimerTimer_Set.Connect( pBundle, 5 );
		pEventTimerTimer_Reset.Connect( pBundle, 4 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA", 16 );
		pAnnunciatorBusSelect->ConnectPort( 1, pBundle, 4 );
		pAnnunciatorBusSelect->ConnectPort( 2, pBundle, 5 );
		pAnnunciatorLampTest->ConnectPort( 2, pBundle, 10 );
		pAnnunciatorLampTest->ConnectPort( 1, pBundle, 11 );
		
		AtlantisPanel::Realize();
	}

	void PanelA6U::OnPreStep(double SimT, double DeltaT, double MJD)
	{
		AtlantisPanel::OnPreStep(SimT, DeltaT, MJD);

		PanelA6U::PAYLOAD Payload = GetSelectedPayload();

		// output
		for (int pl = 0; pl < 3; pl++)
		{
			// lat/rel signals are the same for all 3 pl sys
			for (int i=0;i<5;i++)
			{
				if(LatchSwitch_Latch[i]) {
					Latch_Latch[pl][i].SetLine();
					Latch_Release[pl][i].ResetLine();
				}
				else if(LatchSwitch_Release[i]) {
					Latch_Latch[pl][i].ResetLine();
					Latch_Release[pl][i].SetLine();
				}
				else {
					Latch_Latch[pl][i].ResetLine();
					Latch_Release[pl][i].ResetLine();
				}
			}

			// logic power sys 1/2 signals selected
			if (pl == Payload)
			{
				for (int i = 0; i < 2; i++)
				{
					if (PLRetLogicPwrSysSwitch[i].IsSet())
						LogicPowerSys[pl][i].SetLine();
					else
						LogicPowerSys[pl][i].ResetLine();
				}
			}
			else
			{
				LogicPowerSys[pl][0].ResetLine();
				LogicPowerSys[pl][1].ResetLine();
			}
		}

		// input
		if (Payload != MON)
		{
			for (int i=0;i<5;i++) {
				if(Latch_Latched[Payload][i]) {
					Latch_LatchedTkbk[i].SetLine();
					Latch_ReleasedTkbk[i].ResetLine();
				}
				else if(Latch_Released[Payload][i]) {
					Latch_LatchedTkbk[i].ResetLine();
					Latch_ReleasedTkbk[i].SetLine();
				}
				else {
					Latch_LatchedTkbk[i].ResetLine();
					Latch_ReleasedTkbk[i].ResetLine();
				}

				if(Latch_RTL[Payload][i]) Latch_RTLTkbk[i].SetLine();
				else Latch_RTLTkbk[i].ResetLine();
			}
		}
		else
		{
			for (int i=0;i<5;i++)
			{
				Latch_LatchedTkbk[i].ResetLine();
				Latch_ReleasedTkbk[i].ResetLine();

				Latch_RTLTkbk[i].ResetLine();
			}
		}

		//// event timer ////
		double fCurrState = 0.0;
		double fTgtState = 0.0;

		double fDeltaWheel = 0.5 * DeltaT;

		for(int i = 0; i<4; i++)
		{
			fCurrState = wheelState[i];
			fTgtState = tgtwheel_state[i];

	
			if(0.0 == fCurrState && fTgtState >= EVTTMR_WHEELMAX_A6[i] - 0.5)
			{
				fCurrState = EVTTMR_WHEELMAX_A6[i];
			}

			if(fCurrState <= 0.5 && fTgtState == EVTTMR_WHEELMAX_A6[i])
			{
				fTgtState = 0.0;
			}

			if(fCurrState == fTgtState)
			{
				if(tgtwheel_state[i] >= EVTTMR_WHEELMAX_A6[i])
				{
					tgtwheel_state[i] -= EVTTMR_WHEELMAX_A6[i];
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

			if(wheelState[i] >= EVTTMR_WHEELMAX_A6[i])
			{
				wheelState[i] -= EVTTMR_WHEELMAX_A6[i];
				if(tgtwheel_state[i] > EVTTMR_WHEELMAX_A6[i])
					tgtwheel_state[i] -= EVTTMR_WHEELMAX_A6[i];
			}

			wheelnumber[i] = (int)(fmod(wheelState[i]/ 0.25, EVTTMR_WHEELMAX_A6[i] * 2.0) );

			STS()->SetAnimation(anim_VC_A6Wheel[i], fmod(wheelState[i], 1.0));
		}


		if(wheelnumber[0] != oldwheelnumber[0])
		{
			oapiVCTriggerRedrawArea(-1, AID_A6U_WND0);
			oldwheelnumber[0] = wheelnumber[0];
		}

		if(wheelnumber[1] != oldwheelnumber[1])
		{
			oapiVCTriggerRedrawArea(-1, AID_A6U_WND1);
			oldwheelnumber[1] = wheelnumber[1];
		}

		if(wheelnumber[2] != oldwheelnumber[2])
		{
			oapiVCTriggerRedrawArea(-1, AID_A6U_WND2);
			oldwheelnumber[2] = wheelnumber[2];
		}

		if(wheelnumber[3] != oldwheelnumber[3])
		{
			oapiVCTriggerRedrawArea(-1, AID_A6U_WND3);
			oldwheelnumber[3] = wheelnumber[3];
		}

		if (pEventTimerMode_Up.IsSet()) STS()->pMTU->SetEventTimerMode( dps::TIMER_AFT, dps::COUNT_UP );
		else if (pEventTimerMode_Test.IsSet()) STS()->pMTU->SetEventTimerMode( dps::TIMER_AFT, dps::COUNT_TEST );
		else STS()->pMTU->SetEventTimerMode( dps::TIMER_AFT, dps::COUNT_DOWN );

		if (pEventTimerControl_Start.IsSet()) STS()->pMTU->StartEventTimer( dps::TIMER_AFT );
		else if (pEventTimerControl_Stop.IsSet()) STS()->pMTU->StopEventTimer( dps::TIMER_AFT );

		if (pEventTimerTimer_Set.IsSet()) STS()->pMTU->SetEventTimer( dps::TIMER_AFT, wheelnumber[0] * 10 + wheelnumber[1], wheelnumber[2] * 10 + wheelnumber[3] );
		else if (pEventTimerTimer_Reset.IsSet()) STS()->pMTU->ResetEventTimer( dps::TIMER_AFT );
	}

	PanelA6U::PAYLOAD PanelA6U::GetSelectedPayload() const
	{
		if(PayloadSelect[0]) return PL1;
		if(PayloadSelect[1]) return PL2;
		if(PayloadSelect[2]) return PL3;
		else return MON;
	}
};
