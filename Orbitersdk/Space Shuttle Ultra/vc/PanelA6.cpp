#include "PanelA6.h"
#include "../meshres_vc.h"
#include "../meshres_vc_additions.h"
#include "../Atlantis_defs.h"

extern GDIParams g_Param;

//bounding box for DAP Block1
const VECTOR3 DAP1ULCorner=_V(0.715293, 0.10022, 0); // Upper Left
const VECTOR3 DAP1LRCorner=_V(0.370117, 0.190631, 0); // Lower Right
//bounding box for DAP Block2
const VECTOR3 DAP2ULCorner=_V(0.708954, 0.263163, 0); // Upper Left
const VECTOR3 DAP2LRCorner=_V(0.364316, 0.494076, 0); // Lower Right

namespace vc
{
	PanelA6::PanelA6(Atlantis* _sts)
		:AtlantisPanel(_sts, "A6")
	{
		char cbuf[255];

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
		Add(pLatchState[0] = new StandardTalkback(_sts, "Payload Ret Latch1 Tkbk", 2));
		Add(pLatchState[1] = new StandardTalkback(_sts, "Payload Ret Latch2 Tkbk", 2));
		Add(pLatchState[2] = new StandardTalkback(_sts, "Payload Ret Latch3 Tkbk", 2));
		Add(pLatchState[3] = new StandardTalkback(_sts, "Payload Ret Latch4 Tkbk", 2));
		Add(pLatchState[4] = new StandardTalkback(_sts, "Payload Ret Latch5 Tkbk", 2));
		Add(pLatchRTL[0] = new StandardTalkback(_sts, "Payload Latch1 RTL", 1));
		Add(pLatchRTL[1] = new StandardTalkback(_sts, "Payload Latch2 RTL", 1));
		Add(pLatchRTL[2] = new StandardTalkback(_sts, "Payload Latch3 RTL", 1));
		Add(pLatchRTL[3] = new StandardTalkback(_sts, "Payload Latch4 RTL", 1));
		Add(pLatchRTL[4] = new StandardTalkback(_sts, "Payload Latch5 RTL", 1));
		Add( pADIAttitude = new StdSwitch3( _sts, "ADI Attitude" ) );
		Add( pADIError = new StdSwitch3( _sts, "ADI Error" ) );
		Add( pADIRate = new StdSwitch3( _sts, "ADI Rate" ) );
		Add( pEventTimerMode = new StdSwitch3( _sts, "Event Timer Mode" ) );
		Add( pEventTimerControl = new StdSwitch3( _sts, "Event Timer Control" ) );
		Add( pEventTimerTimer = new StdSwitch3( _sts, "Event Timer Timer" ) );

		for(int i=0;i<24;i++) {
			//PBI_Lights[i]=false;

			sprintf_s(cbuf, 255, "%d", i+1);
			std::string name="A6_PBI";
			name+=cbuf;
			Add(pPBIs[i]=new PushButtonIndicator(_sts, name));
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

	PanelA6::~PanelA6()
	{
	}

	bool PanelA6::OnVCMouseEvent( int id, int _event, VECTOR3 &p )
	{
		if (id != AID_A6) return false;
		
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

	bool PanelA6::OnVCRedrawEvent( int id, int _event, SURFHANDLE surf )
	{
		const int NUMX[10] = {0, 64, 128, 192, 0, 64, 128, 192, 0, 64};
		const int NUMY[10] = {0, 0, 0, 0, 64, 64, 64, 64, 128, 128};
	
		switch (id)
		{
			case AID_A6_WND0:
				oapiBlt(surf, g_Param.clock_digits, 0,0, NUMX[wheelnumber[0]], NUMY[wheelnumber[0]], 63, 63);
				return true;
			case AID_A6_WND1:
				oapiBlt(surf, g_Param.clock_digits, 0,0, NUMX[wheelnumber[1]], NUMY[wheelnumber[1]], 63, 63);
				return true;
			case AID_A6_WND2:
				oapiBlt(surf, g_Param.clock_digits, 0,0, NUMX[wheelnumber[2]], NUMY[wheelnumber[2]], 63, 63);
				return true;
			case AID_A6_WND3:
				oapiBlt(surf, g_Param.clock_digits, 0,0, NUMX[wheelnumber[3]], NUMY[wheelnumber[3]], 63, 63);
				return true;
		}
		return AtlantisPanel::OnVCRedrawEvent( id, _event, surf );
	}

	void PanelA6::DefineVC()
	{
		const VECTOR3 switch_rot =  _V(-1, 0, 0);
		const VECTOR3 switch_pull = _V(0, 0.3126, 0.9499);
		oapiWriteLog("PanelA6: DefineVC called");

		AddAIDToMouseEventList(AID_A6);

		pSense->DefineSwitchGroup(GRP_A6U1_VC);
		pSense->SetInitialAnimState(0.5);
		pSense->SetReference(_V(0.728, 2.780, 12.313), switch_rot);
		pSense->SetMouseRegion(0.863499f, 0.262889f, 0.924675f, 0.332972f);

		pFltCntlrPower->DefineSwitchGroup(GRP_A6U2_VC);
		pFltCntlrPower->SetInitialAnimState(0.5);
		pFltCntlrPower->SetReference(_V(0.686, 2.781, 12.313), switch_rot);
		pFltCntlrPower->SetPullDirection(switch_pull);
		pFltCntlrPower->SetMouseRegion(0.772155f, 0.254109f, 0.836383f, 0.348049f);

		pPayloadRetentionLogicPowerSys[0]->DefineSwitchGroup( GRP_A6U46_VC );
		pPayloadRetentionLogicPowerSys[0]->SetInitialAnimState( 0.5f );
		pPayloadRetentionLogicPowerSys[0]->SetReference( _V( 0.7303, 2.6068, 12.3675 ), switch_rot );
		pPayloadRetentionLogicPowerSys[0]->SetMouseRegion( 0.881104f, 0.721166f, 0.917138f, 0.779604f );

		pPayloadRetentionLogicPowerSys[1]->DefineSwitchGroup( GRP_A6U47_VC );
		pPayloadRetentionLogicPowerSys[1]->SetInitialAnimState( 0.5f );
		pPayloadRetentionLogicPowerSys[1]->SetReference( _V( 0.6757, 2.6068, 12.3675 ), switch_rot );
		pPayloadRetentionLogicPowerSys[1]->SetMouseRegion( 0.775442f, 0.714653f, 0.813627f, 0.779674f );

		pPayloadRetentionLatches[0]->DefineSwitchGroup(GRP_A6U36_VC);
		pPayloadRetentionLatches[0]->SetInitialAnimState(0.5);
		pPayloadRetentionLatches[0]->SetReference(_V(0.618, 2.555, 12.383), switch_rot);
		pPayloadRetentionLatches[0]->SetMouseRegion(0.651142f, 0.844718f, 0.710472f, 0.935115f);
		
		pPayloadRetentionLatches[1]->DefineSwitchGroup(GRP_A6U42_VC);
		pPayloadRetentionLatches[1]->SetInitialAnimState(0.5);
		pPayloadRetentionLatches[1]->SetReference(_V(0.582, 2.555, 12.383), switch_rot);
		pPayloadRetentionLatches[1]->SetMouseRegion(0.578245f, 0.847014f, 0.640193f, 0.934493f);
		
		pPayloadRetentionLatches[2]->DefineSwitchGroup(GRP_A6U43_VC);
		pPayloadRetentionLatches[2]->SetInitialAnimState(0.5);
		pPayloadRetentionLatches[2]->SetReference(_V(0.543, 2.555, 12.383), switch_rot);
		pPayloadRetentionLatches[2]->SetMouseRegion(0.507261f, 0.844424f, 0.569764f, 0.932009f);
		
		pPayloadRetentionLatches[3]->DefineSwitchGroup(GRP_A6U44_VC);
		pPayloadRetentionLatches[3]->SetInitialAnimState(0.5);
		pPayloadRetentionLatches[3]->SetReference(_V(0.505, 2.554, 12.383), switch_rot);
		pPayloadRetentionLatches[3]->SetMouseRegion(0.432613f, 0.846585f, 0.492668f, 0.936958f);
		
		pPayloadRetentionLatches[4]->DefineSwitchGroup(GRP_A6U45_VC);
		pPayloadRetentionLatches[4]->SetInitialAnimState(0.5);
		pPayloadRetentionLatches[4]->SetReference(_V(0.468, 2.554, 12.383), switch_rot);
		pPayloadRetentionLatches[4]->SetMouseRegion(0.356811f, 0.845240f, 0.421112f, 0.939906f);

		pPayloadSelect->DefineSwitchGroup(GRP_A6US37_VC);
		pPayloadSelect->SetInitialAnimState(1.0f);
		pPayloadSelect->SetReference(_V(0.7079, 2.5535, 12.3926), -switch_pull);
		pPayloadSelect->DefineRotationAngle(120.0f);
		pPayloadSelect->SetOffset(-60.0f);
		pPayloadSelect->SetMouseRegion(0.811500f, 0.842076f, 0.881687f, 0.940961f);

		pADIAttitude->DefineSwitchGroup( GRP_A6U5_VC );
		pADIAttitude->SetInitialAnimState( 0.5 );
		pADIAttitude->SetReference( _V( 0.6908, 2.6717, 12.3472 ), switch_rot );
		pADIAttitude->SetMouseRegion( 0.889962f, 0.547700f, 0.925561f, 0.612007f );

		pADIError->DefineSwitchGroup( GRP_A6U6_VC );
		pADIError->SetInitialAnimState( 0.5 );
		pADIError->SetReference( _V( 0.6908, 2.6717, 12.3472 ), switch_rot );
		pADIError->SetMouseRegion( 0.802425f, 0.547700f, 0.841630f, 0.612007f );

		pADIRate->DefineSwitchGroup( GRP_A6U7_VC );
		pADIRate->SetInitialAnimState( 0.5 );
		pADIRate->SetReference( _V( 0.6908, 2.6717, 12.3472 ), switch_rot );
		pADIRate->SetMouseRegion( 0.749505f, 0.547700f, 0.783608f, 0.612007f );

		for(int i=0;i<24;i++) {
			pPBIs[i]->AddAIDToRedrawEventList(AID_A6_PBI1+i);
			pPBIs[i]->SetSourceImage(g_Param.pbi_lights);
			pPBIs[i]->SetBase(0, 0);
			pPBIs[i]->SetSourceCoords(true, 0, 0);
			pPBIs[i]->SetSourceCoords(false, 0, 14);
			pPBIs[i]->SetDimensions(37, 11);
		}

		for(int i=0;i<5;i++) {
			pLatchRTL[i]->AddAIDToRedrawEventList(AID_A6_TKBK1+i);
			pLatchRTL[i]->SetTalkbackLocation(0, 0);
			pLatchRTL[i]->SetDimensions(40, 22);

			pLatchState[i]->AddAIDToRedrawEventList(AID_A6_TKBK6+i);
			pLatchState[i]->SetTalkbackLocation(0, 0);
			pLatchState[i]->SetDimensions(40, 22);
		}

		pEventTimerMode->SetInitialAnimState( 0.5f );
		pEventTimerMode->DefineSwitchGroup( GRP_A6U39_VC );
		pEventTimerMode->SetReference( _V( 0.3962, 2.5547, 12.3837 ), switch_rot );
		pEventTimerMode->SetMouseRegion( 0.244218f, 0.854629f, 0.282415f, 0.920201f );
		pEventTimerMode->SetSpringLoaded( true, 0 );

		pEventTimerControl->SetInitialAnimState( 0.5f );
		pEventTimerControl->DefineSwitchGroup( GRP_A6U40_VC );	
		pEventTimerControl->SetReference( _V( 0.3667, 2.5549, 12.3836 ), switch_rot );
		pEventTimerControl->SetMouseRegion( 0.184202f, 0.857520f, 0.230935f, 0.922421f );
		pEventTimerControl->SetSpringLoaded( true, 0 );
		pEventTimerControl->SetSpringLoaded( true, 2 );

		pEventTimerTimer->SetInitialAnimState( 0.5f );
		pEventTimerTimer->DefineSwitchGroup( GRP_A6U41_VC );
		pEventTimerTimer->SetReference( _V( 0.3392, 2.5549, 12.3837 ), switch_rot );
		pEventTimerTimer->SetMouseRegion( 0.134762f, 0.857368f, 0.171836f, 0.923072f );
		pEventTimerTimer->SetSpringLoaded( true, 0 );
		pEventTimerTimer->SetSpringLoaded( true, 2 );

		//mouse regions
		pPBIs[0]->SetMouseRegion(0.659691f, 0.112755f, 0.705132f, 0.174825f); //A
		pPBIs[1]->SetMouseRegion(0.60511f, 0.114699f, 0.649507f, 0.172828f); //B
		pPBIs[2]->SetMouseRegion(0.550367f, 0.115484f, 0.595548f, 0.176009f); //AUTO
		pPBIs[3]->SetMouseRegion(0.493128f, 0.114615f, 0.540278f, 0.176737f); //INRTL
		pPBIs[4]->SetMouseRegion(0.437027f, 0.114202f, 0.484483f, 0.175445f); //LVLH
		pPBIs[5]->SetMouseRegion(0.379574f, 0.116267f, 0.42722f, 0.175610f); //FREE
		pPBIs[6]->SetMouseRegion(0.653568f, 0.271157f, 0.699381f, 0.331649f); //PCT
		pPBIs[9]->SetMouseRegion(0.486053f, 0.272087f, 0.532732f, 0.331215f); //PRI
		pPBIs[10]->SetMouseRegion(0.431093f, 0.27408f, 0.476635f, 0.331248f); //ALT
		pPBIs[11]->SetMouseRegion(0.37393f, 0.270786f, 0.421151f, 0.333474f); //VERN
		pPBIs[12]->SetMouseRegion(0.651264f, 0.3451f, 0.698885f, 0.407937f); //X NORM
		pPBIs[13]->SetMouseRegion(0.596782f, 0.347135f, 0.643202f, 0.407219f); //Y NORM
		pPBIs[14]->SetMouseRegion(0.543387f, 0.348282f, 0.588769f, 0.408374f); //Z NORM
		pPBIs[15]->SetMouseRegion(0.486485f, 0.347593f, 0.533187f, 0.408542f); //ROLL DISC RATE
		pPBIs[16]->SetMouseRegion(0.428891f, 0.347792f, 0.476103f, 0.408729f); //PITCH DISC RATE
		pPBIs[17]->SetMouseRegion(0.37329f, 0.346197f, 0.421186f, 0.40707f); //YAW DISC RATE
		pPBIs[18]->SetMouseRegion(0.651442f, 0.423913f, 0.697746f, 0.48492f); //X PULSE
		pPBIs[19]->SetMouseRegion(0.595887f, 0.424997f, 0.644165f, 0.485769f); //Y PULSE
		pPBIs[20]->SetMouseRegion(0.54198f, 0.423774f, 0.588723f, 0.485417f); //Z PULSE
		pPBIs[21]->SetMouseRegion(0.486516f, 0.4234f, 0.532259f, 0.486258f); //ROLL PULSE
		pPBIs[22]->SetMouseRegion(0.429934f, 0.424362f, 0.475646f, 0.485086f); //PITCH PULSE
		pPBIs[23]->SetMouseRegion(0.374104f, 0.424197f, 0.420746f, 0.486088f); //YAW PULSE
	}

	void PanelA6::RegisterVC()
	{
		AtlantisPanel::RegisterVC();
		oapiWriteLog("PanelA6: RegisterVC called");

		VECTOR3 ofs=STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea(AID_A6, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
		oapiVCSetAreaClickmode_Quadrilateral(AID_A6,
			_V(0.262, 2.892, 12.277)+ofs, _V(0.789, 2.892, 12.277)+ofs,
			_V(0.262, 2.511, 12.397)+ofs, _V(0.789, 2.511, 12.397)+ofs);

		//register DAP PBIs
		SURFHANDLE panela6_tex = oapiGetTextureHandle (STS()->hOrbiterVCMesh, TEX_PANELA6A7_VC);
		//A
		oapiVCRegisterArea(AID_A6_PBI1, _R(703, 1178, 740, 1189), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//B
		oapiVCRegisterArea(AID_A6_PBI2, _R(779, 1178, 816, 1189), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//AUTO
		oapiVCRegisterArea(AID_A6_PBI3, _R(855, 1178, 892, 1189), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//INRTL
		oapiVCRegisterArea(AID_A6_PBI4, _R(934, 1180, 971, 1191), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//LVLH
		oapiVCRegisterArea(AID_A6_PBI5, _R(1012, 1180, 1049, 1191), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//FREE
		oapiVCRegisterArea(AID_A6_PBI6, _R(1092, 1181, 1129, 1192), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//PCT
		oapiVCRegisterArea(AID_A6_PBI7, _R(713, 1341, 750, 1352), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//LOW Z
		//oapiVCRegisterArea(AID_A6_PBI8, _R(792, 1341, 829, 1352), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//HIGH Z
		//oapiVCRegisterArea(AID_A6_PBI9, _R(868, 1341, 905, 1352), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//PRI
		oapiVCRegisterArea(AID_A6_PBI10, _R(943, 1340, 980, 1351), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//ALT
		oapiVCRegisterArea(AID_A6_PBI11, _R(1023, 1340, 1060, 1351), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//VERN
		oapiVCRegisterArea(AID_A6_PBI12, _R(1100, 1340, 1137, 1351), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//TRANS X NORM
		oapiVCRegisterArea(AID_A6_PBI13, _R(712, 1416, 749, 1427), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//TRANS Y NORM
		oapiVCRegisterArea(AID_A6_PBI14, _R(790, 1416, 827, 1427), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//TRANS Z NORM
		oapiVCRegisterArea(AID_A6_PBI15, _R(866, 1418, 903, 1429), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//ROT ROLL DISC RATE
		oapiVCRegisterArea(AID_A6_PBI16, _R(944, 1416, 981, 1427), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//ROT PITCH DISC RATE
		oapiVCRegisterArea(AID_A6_PBI17, _R(1023, 1416, 1060, 1427), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//ROT YAW DISC RATE
		oapiVCRegisterArea(AID_A6_PBI18, _R(1100, 1415, 1137, 1426), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//TRANS X PULSE
		oapiVCRegisterArea(AID_A6_PBI19, _R(711, 1496, 748, 1507), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//TRANS Y PULSE
		oapiVCRegisterArea(AID_A6_PBI20, _R(790, 1496, 827, 1507), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//TRANS Z PULSE
		oapiVCRegisterArea(AID_A6_PBI21, _R(867, 1496, 904, 1507), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//ROT ROLL PULSE
		oapiVCRegisterArea(AID_A6_PBI22, _R(945, 1496, 982, 1507), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//ROT PITCH PULSE
		oapiVCRegisterArea(AID_A6_PBI23, _R(1023, 1496, 1060, 1507), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//ROT YAW PULSE
		oapiVCRegisterArea(AID_A6_PBI24, _R(1102, 1496, 1139, 1507), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		// RTL Talkbacks
		oapiVCRegisterArea(AID_A6_TKBK1, _R(708, 1721, 748, 1743), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		oapiVCRegisterArea(AID_A6_TKBK2, _R(806, 1721, 846, 1743), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		oapiVCRegisterArea(AID_A6_TKBK3, _R(908, 1721, 948, 1743), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		oapiVCRegisterArea(AID_A6_TKBK4, _R(1008, 1721, 1048, 1743), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		oapiVCRegisterArea(AID_A6_TKBK5, _R(1108, 1721, 1148, 1743), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		// Latch Talkbacks
		oapiVCRegisterArea(AID_A6_TKBK6, _R(710, 1809, 750, 1831), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		oapiVCRegisterArea(AID_A6_TKBK7, _R(807, 1809, 847, 1831), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		oapiVCRegisterArea(AID_A6_TKBK8, _R(910, 1809, 950, 1831), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		oapiVCRegisterArea(AID_A6_TKBK9, _R(1010, 1809, 1050, 1831), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		oapiVCRegisterArea(AID_A6_TKBK10, _R(1111, 1809, 1151, 1831), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		// event timer wheel numbers
		SURFHANDLE digittex = oapiGetTextureHandle( STS()->hOrbiterVCMesh, TEX_CLOCKNUMS_VC );
		oapiVCRegisterArea( AID_A6_WND0, _R( 0, 64, 63, 127 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digittex );
		oapiVCRegisterArea( AID_A6_WND1, _R( 64, 64, 127, 127 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digittex );
		oapiVCRegisterArea( AID_A6_WND2, _R( 128, 64, 191, 127 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digittex );
		oapiVCRegisterArea( AID_A6_WND3, _R( 192, 64, 255, 127 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digittex );
	}

	void PanelA6::DefineVCAnimations( UINT vcidx )
	{
		static VECTOR3 wheel_rot = {-1.0, 0, 0};

		static UINT VC_A6Evt10MW_Grp = GRP_A6UTOG1_VC;

		static MGROUP_ROTATE VC_A6Evt10MW (vcidx, &VC_A6Evt10MW_Grp, 1,
			_V( 0.0, 2.6080, 12.3532 ), wheel_rot, (float)(360.0*RAD));
		anim_VC_A6Wheel[0]=STS()->CreateAnimation (0.0);
		STS()->AddAnimationComponent (anim_VC_A6Wheel[0], 0, 1, &VC_A6Evt10MW);

		static UINT VC_A6Evt1MW_Grp = GRP_A6UTOG2_VC;

		static MGROUP_ROTATE VC_A6Evt1MW (vcidx, &VC_A6Evt1MW_Grp, 1,
			_V( 0.0, 2.6080, 12.3532 ), wheel_rot, (float)(360.0*RAD));
		anim_VC_A6Wheel[1]=STS()->CreateAnimation (0.0);
		STS()->AddAnimationComponent (anim_VC_A6Wheel[1], 0, 1, &VC_A6Evt1MW);

		static UINT VC_A6Evt10SW_Grp = GRP_A6UTOG3_VC;

		static MGROUP_ROTATE VC_A6Evt10SW (vcidx, &VC_A6Evt10SW_Grp, 1,
			_V( 0.0, 2.6080, 12.3532 ), wheel_rot, (float)(360.0*RAD));
		anim_VC_A6Wheel[2]=STS()->CreateAnimation (0.0);
		STS()->AddAnimationComponent (anim_VC_A6Wheel[2], 0, 1, &VC_A6Evt10SW);

		static UINT VC_A6Evt1SW_Grp = GRP_A6UTOG4_VC;

		static MGROUP_ROTATE VC_A6Evt1SW (vcidx, &VC_A6Evt1SW_Grp, 1,
			_V( 0.0, 2.6080, 12.3532 ), wheel_rot, (float)(360.0*RAD));
		anim_VC_A6Wheel[3]=STS()->CreateAnimation (0.0);
		STS()->AddAnimationComponent (anim_VC_A6Wheel[3], 0, 1, &VC_A6Evt1SW);

		AtlantisPanel::DefineVCAnimations( vcidx );
		return;
	}

	void PanelA6::Realize()
	{
		DiscreteBundle* pBundle=STS()->BundleManager()->CreateBundle("Controllers", 16);
		pSense->output.Connect(pBundle, 0);
		//pFltCntlrPower->output.Connect(pBundle, 3);
		pFltCntlrPower->ConnectPort(1, pBundle, 3);

		pBundle=STS()->BundleManager()->CreateBundle("DAP_PBIS1", 16);
		for(int i=0;i<16;i++) {
			pPBIs[i]->input.Connect(pBundle, i);
			pPBIs[i]->output.Connect(pBundle, i);
			pPBIs[i]->test.Connect(pBundle, i);
		}
		pBundle=STS()->BundleManager()->CreateBundle("DAP_PBIS2", 16);
		for(int i=16;i<24;i++) {
			pPBIs[i]->input.Connect(pBundle, i-16);
			pPBIs[i]->output.Connect(pBundle, i-16);
			pPBIs[i]->test.Connect(pBundle, i-16);
		}

		pBundle = STS()->BundleManager()->CreateBundle("A6_LATCHSWITCHES", 16);
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
		
		pBundle = STS()->BundleManager()->CreateBundle("A6_LATCH_TKBKS", 16);
		for(int i=0;i<5;i++) {
			Latch_LatchedTkbk[i].Connect(pBundle, i);
			Latch_ReleasedTkbk[i].Connect(pBundle, i+5);
			Latch_RTLTkbk[i].Connect(pBundle, i+10);

			pLatchState[i]->SetInput(0, pBundle, i, TB_LAT);
			pLatchState[i]->SetInput(1, pBundle, i+5, TB_REL);
			pLatchRTL[i]->SetInput(0, pBundle, i+10, TB_GRAY);
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

		pBundle = STS()->BundleManager()->CreateBundle( "A6_EventTimer", 6 );
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
		
		AtlantisPanel::Realize();
	}

	void PanelA6::OnPreStep(double SimT, double DeltaT, double MJD)
	{
		AtlantisPanel::OnPreStep(SimT, DeltaT, MJD);

		PanelA6::PAYLOAD Payload = GetSelectedPayload();

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
			oapiVCTriggerRedrawArea(-1, AID_A6_WND0);
			oldwheelnumber[0] = wheelnumber[0];
		}

		if(wheelnumber[1] != oldwheelnumber[1])
		{
			oapiVCTriggerRedrawArea(-1, AID_A6_WND1);
			oldwheelnumber[1] = wheelnumber[1];
		}

		if(wheelnumber[2] != oldwheelnumber[2])
		{
			oapiVCTriggerRedrawArea(-1, AID_A6_WND2);
			oldwheelnumber[2] = wheelnumber[2];
		}

		if(wheelnumber[3] != oldwheelnumber[3])
		{
			oapiVCTriggerRedrawArea(-1, AID_A6_WND3);
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

	PanelA6::PAYLOAD PanelA6::GetSelectedPayload() const
	{
		if(PayloadSelect[0]) return PL1;
		if(PayloadSelect[1]) return PL2;
		if(PayloadSelect[2]) return PL3;
		else return MON;
	}
};
