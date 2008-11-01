#include "PanelR2.h"
#include <OrbiterSoundSDK35.h>
#include "resource.h"
#include "meshres.h"
#include "meshres_vc.h"
#include "meshres_vc_additions.h"
#include "DlgCtrl.h"
#include <stdio.h>
#include <io.h>

extern GDIParams g_Param;
extern HELPCONTEXT g_hc;
extern char *ActionString[5];

// ==============================================================

PanelR2::PanelR2(Atlantis *_sts): sts(_sts)
{
	int i;
	for(i=0;i<3;i++)
	{
		BOILER_CNTLR[i]=1;
		BOILER_N2_SUPPLY[i]=1;
		BOILER_CNTLRHTR[i]=1;
		APU_CONTROL[i]=1;
		APU_FUEL_TK_VLV[i]=1;
		APU_CNTLR_PWR[i]=1;
		APU_SPEED_SELECT[i]=1;
		HYD_MAIN_PUMP_PRESS[i]=0;
		HYD_CIRC_PUMP[i]=2;
		MPS_ENGINE_POWER[0][i]=1;
		MPS_ENGINE_POWER[1][i]=1;
		HE_ISOLATION_A[i]=1;
		HE_ISOLATION_B[i]=1;
		APU_STATE[i]=0;
		MPS_STATE[i]=0;
		APU_READY[i]=false;

		Hydraulic_Press[i]=0;
		APU_Speed[i]=0;
		Fuel_Press[i]=0;
	}
	APU_AUTO_SHUT_DOWN=0;
	CENTERLINE_LATCH=0;
	LEFT_DOOR=1;
	LEFT_LATCH=1;
	RIGHT_DOOR=1;
	RIGHT_LATCH=1;
	bHydraulicPressure=false;
	CenterlineLatches.Set(AnimState::OPEN, 1);
	LETUmbDoorStatus.Set(AnimState::CLOSED, 0);
	RETUmbDoorStatus.Set(AnimState::CLOSED, 0);
	LDoorLatches.Set(AnimState::CLOSED, 0);
	RDoorLatches.Set(AnimState::CLOSED, 0);
	
	for(i=0;i<3;i++)
	{
		tkbk_state[i]=0;
	}
	tkbk_state[3]=1;
	tkbk_state[4]=5;
	tkbk_state[5]=3;
	tkbk_state[6]=5;
	tkbk_state[7]=3;
}

void PanelR2::RegisterVC ()
{
	VECTOR3 ofs = sts->orbiter_ofs;
	SURFHANDLE panelr2_tex = oapiGetTextureHandle (sts->hOrbiterVCMesh, TEX_PANELR2_VC);
	//SURFHANDLE tkbk_tex = oapiGetTextureHandle (sts->hOrbiterVCMesh, 5);

	oapiVCRegisterArea (AID_R2, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN);
	oapiVCSetAreaClickmode_Quadrilateral (AID_R2, _V(0.9524, 1.8417, 14.6602)+ofs, _V(1.4477, 1.9868, 14.6602)+ofs,
		_V(.9524, 1.7286, 13.8048)+ofs, _V(1.4477, 1.8755, 13.8048)+ofs);

	// register the talkbacks
	oapiVCRegisterArea (AID_R2_TKBK1, _R( 451, 706, 483, 724), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelr2_tex);
	oapiVCRegisterArea (AID_R2_TKBK2, _R( 525, 706, 557, 724), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelr2_tex);
	oapiVCRegisterArea (AID_R2_TKBK3, _R( 598, 706, 630, 724), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelr2_tex);
	oapiVCRegisterArea (AID_R2_TKBK4, _R( 253, 1723, 285, 1741), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelr2_tex);
	oapiVCRegisterArea (AID_R2_TKBK5, _R( 379, 1723, 411, 1741), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelr2_tex);
	oapiVCRegisterArea (AID_R2_TKBK6, _R( 480, 1723, 512, 1741), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelr2_tex);
	oapiVCRegisterArea (AID_R2_TKBK7, _R( 574, 1723, 606, 1741), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelr2_tex);
	oapiVCRegisterArea (AID_R2_TKBK8, _R( 672, 1726, 704, 1744), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelr2_tex);
}

bool PanelR2::VCRedrawEvent(int id, int event, SURFHANDLE surf)
{
	//sprintf(oapiDebugString(), "VCRedrawEvent %f", oapiRand());
	switch (id) {
	case AID_R2_TKBK1:
	case AID_R2_TKBK2:
	case AID_R2_TKBK3:
		if(APU_READY[id-AID_R2_TKBK1] || (APU_STATE[id-AID_R2_TKBK1]==1 && APU_Speed[id-AID_R2_TKBK1]<80))
			return VCDrawTalkback (surf, id-AID_R2_TKBK1, 8);
		else return VCDrawTalkback (surf, id-AID_R2_TKBK1, 0);
		break;
	case AID_R2_TKBK4:
		if(CenterlineLatches.action==AnimState::OPEN) //OPEN(1)=stowed
			return VCDrawTalkback(surf, id-AID_R2_TKBK1, 1);
		else return VCDrawTalkback(surf, id-AID_R2_TKBK1, 0);
		break;
	case AID_R2_TKBK5:
		if(LETUmbDoorStatus.action==AnimState::CLOSED)
			return VCDrawTalkback(surf, id-AID_R2_TKBK1, 3);
		else if(LETUmbDoorStatus.action==AnimState::OPEN)
			return VCDrawTalkback(surf, id-AID_R2_TKBK1, 4);
		else return VCDrawTalkback(surf, id-AID_R2_TKBK1, 0);
		break;
	case AID_R2_TKBK6:
		if(LDoorLatches.action==AnimState::CLOSED)
			return VCDrawTalkback(surf, id-AID_R2_TKBK1, 2);
		else if(LDoorLatches.action==AnimState::OPEN)
			return VCDrawTalkback(surf, id-AID_R2_TKBK1, 10);
		else return VCDrawTalkback(surf, id-AID_R2_TKBK1, 0);
		break;
	case AID_R2_TKBK7:
		if(RETUmbDoorStatus.action==AnimState::CLOSED)
			return VCDrawTalkback(surf, id-AID_R2_TKBK1, 3);
		else if(RETUmbDoorStatus.action==AnimState::OPEN)
			return VCDrawTalkback(surf, id-AID_R2_TKBK1, 4);
		else return VCDrawTalkback(surf, id-AID_R2_TKBK1, 0);
		break;
	case AID_R2_TKBK8:
		if(RDoorLatches.action==AnimState::CLOSED)
			return VCDrawTalkback(surf, id-AID_R2_TKBK1, 2);
		else if(RDoorLatches.action==AnimState::OPEN)
			return VCDrawTalkback(surf, id-AID_R2_TKBK1, 10);
		else return VCDrawTalkback(surf, id-AID_R2_TKBK1, 0);
		break;
	}
	return false;
}

bool PanelR2::VCDrawTalkback (SURFHANDLE surf, int idx, int label)
{
	//if(idx<3) sprintf(oapiDebugString(), "1: %d %d %f", tkbk_state[idx], label, oapiRand());
	if (tkbk_state[idx] == label) return false; // nothing to do
	//sprintf(oapiDebugString(), "idx %d label %d tkbk_state %d", idx, label, tkbk_state[idx]);
	tkbk_state[idx] = label;
	if(label<8) oapiBlt (surf, g_Param.tkbk_label, 0, 0, label*32, 0, 32, 18);
	else oapiBlt (surf, g_Param.tkbk_label, 0, 0, (label-8)*32, 18, 32, 18);
	//if(idx<3) sprintf(oapiDebugString(), "2: %d %d %f", tkbk_state[idx], label, oapiRand());
	return true;
}

bool PanelR2::VCMouseEvent(int id, int event, VECTOR3 &p)
{
	if (id != AID_R2) return false;
	bool action = false;
	int eng=-1;
	int apu=-1;
	//sprintf(oapiDebugString(), "id %d event %d p %f %f %f",id,event,p.x,p.y,p.z);
	if(p.y>0.890886 && p.y<0.934839 && p.x>=0.671214 && p.x<=0.875479) {
		if(p.x<0.741418) {
			if(p.y<0.915214) {
				BOILER_N2_SUPPLY[0]=0;
				action=true;
			}
			else {
				BOILER_N2_SUPPLY[0]=1;
				action=true;
			}
		}
		else if(p.x>0.798522) {
			if(p.y<0.915214) {
				BOILER_N2_SUPPLY[2]=0;
				action=true;
			}
			else {
				BOILER_N2_SUPPLY[2]=1;
				action=true;
			}
		}
		else {
			if(p.y<0.915214) {
				BOILER_N2_SUPPLY[1]=0;
				action=true;
			}
			else {
				BOILER_N2_SUPPLY[1]=1;
				action=true;
			}
		}
	}
	else if(p.y>0.821740 && p.y<0.862008 && p.x>=0.633620 && p.x<=0.816670) {
		if(p.x<0.697079) {
			if(p.y<0.845951) {
				BOILER_CNTLR[0]=0;
				action=true;
			}
			else {
				BOILER_CNTLR[0]=1;
				action=true;
			}
		}
		else if(p.x>.755268) {
			if(p.y<0.845951) 
			{
				BOILER_CNTLR[2]=0;
				action=true;
			}
			else {
				BOILER_CNTLR[2]=1;
				action=true;
			}
		}
		else {
			if(p.y<0.845951) 
			{
				BOILER_CNTLR[1]=0;
				action=true;
			}
			else {
				BOILER_CNTLR[1]=1;
				action=true;
			}
		}
	}
	else if(p.y>0.675809 && p.y<0.714835 && p.x>=0.496828 && p.x<=0.726327) {
		if(p.x<0.575494) {
			if(p.y<0.694020) {
				APU_FUEL_TK_VLV[0]=0;
				action=true;
			}
			else {
				APU_FUEL_TK_VLV[0]=1;
				action=true;
			}
		}
		else if(p.x>0.649903) {
			if(p.y<0.694020) {
				APU_FUEL_TK_VLV[2]=0;
				action=true;
			}
			else {
				APU_FUEL_TK_VLV[2]=1;
				action=true;
			}
		}
		else {
			if(p.y<0.694020) {
				APU_FUEL_TK_VLV[1]=0;
				action=true;
			}
			else {
				APU_FUEL_TK_VLV[1]=1;
				action=true;
			}
		}
	}
	else if(p.y>0.754219 && p.y<0.788508 && p.x>=0.547575 && p.x<=0.769339) {
		if(p.x<0.625464) {
			if(p.y<0.773020) {
				if(BOILER_CNTLRHTR[0]>0) {
					BOILER_CNTLRHTR[0]--;
					action=true;
				}
			}
			else {
				if(BOILER_CNTLRHTR[0]<2) {
					BOILER_CNTLRHTR[0]++;
					action=true;
				}
			}
		}
		else if(p.x>0.693698) {
			if(p.y<0.773020) {
				if(BOILER_CNTLRHTR[2]>0) {
					BOILER_CNTLRHTR[2]--;
					action=true;
				}
			}
			else {
				if(BOILER_CNTLRHTR[2]<2) {
					BOILER_CNTLRHTR[2]++;
					action=true;
				}
			}
		}
		else {
			if(p.y<0.773020) {
				if(BOILER_CNTLRHTR[1]>0) {
					BOILER_CNTLRHTR[1]--;
					action=true;
				}
			}
			else {
				if(BOILER_CNTLRHTR[1]<2) {
					BOILER_CNTLRHTR[1]++;
					action=true;
				}
			}
		}
	}
	else if(p.y>0.606395 && p.y<0.649438 && p.x>=0.495306 && p.x<=0.682484) {
		if(p.x<0.553809) {
			if(p.y<0.624789) {
				APU_CNTLR_PWR[0]=0;
				action=true;
			}
			else {
				APU_CNTLR_PWR[0]=1;
				action=true;
			}
		}
		else if(p.x>0.617025) {
			if(p.y<0.624789) {
				APU_CNTLR_PWR[2]=0;
				action=true;
			}
			else {
				APU_CNTLR_PWR[2]=1;
				action=true;
			}
		}
		else {
			if(p.y<0.624789) {
				APU_CNTLR_PWR[1]=0;
				action=true;
			}
			else {
				APU_CNTLR_PWR[1]=1;
				action=true;
			}
		}
	}
	else if(p.y>0.601758 && p.y<0.649438 && p.x>=0.259357 && p.x<0.495306) {
		if(p.x<0.334360) {
			if(p.y<0.619906) {
				if(HYD_CIRC_PUMP[0]>0) {
					HYD_CIRC_PUMP[0]--;
					action=true;
				}
			}
			else {
				if(HYD_CIRC_PUMP[0]<2) {
					HYD_CIRC_PUMP[0]++;
					action=true;
				}
			}
		}
		else if(p.x>0.409110) {
			if(p.y<0.619906) {
				if(HYD_CIRC_PUMP[2]>0) {
					HYD_CIRC_PUMP[2]--;
					action=true;
				}
			}
			else {
				if(HYD_CIRC_PUMP[2]<2) {
					HYD_CIRC_PUMP[2]++;
					action=true;
				}
			}
		}
		else {
			if(p.y<0.619906) {
				if(HYD_CIRC_PUMP[1]>0) {
					HYD_CIRC_PUMP[1]--;
					action=true;
				}
			}
			else {
				if(HYD_CIRC_PUMP[1]<2) {
					HYD_CIRC_PUMP[1]++;
					action=true;
				}
			}
		}
	}
	else if(p.y>0.534468 && p.y<0.568684 && p.x>=0.364955 && p.x<=0.625087) {
		if(p.x<0.454298) {
			if(p.y<0.553525) {
				HYD_MAIN_PUMP_PRESS[0]=0;
				action=true;
			}
			else {
				HYD_MAIN_PUMP_PRESS[0]=1;
				action=true;
			}
		}
		else if(p.x>0.549506) {
			if(p.y<0.553525) {
				HYD_MAIN_PUMP_PRESS[2]=0;
				action=true;
			}
			else {
				HYD_MAIN_PUMP_PRESS[2]=1;
				action=true;
			}
		}
		else {
			if(p.y<0.553525) {
				HYD_MAIN_PUMP_PRESS[1]=0;
				action=true;
			}
			else {
				HYD_MAIN_PUMP_PRESS[1]=1;
				action=true;
			}
		}
	}
	else if(p.y>0.382021 && p.y<0.424506 && p.x>=.347421 && p.x<=0.535220) {
		//CheckAPUReadytoStart();
		if(p.x<0.412748) {
			if(p.y<0.404556) {
				if(APU_CONTROL[0]>0) {
					APU_CONTROL[0]--;
					action=true;
				}
			}
			else {
				if(APU_CONTROL[0]<2) {
					APU_CONTROL[0]++;
					action=true;
				}
			}
			if(APU_CONTROL[0]!=1 && APU_READY[0]) {
				APU_STATE[0]=1;
				PlayVesselWave3(sts->SoundID, APU_START, NOLOOP);
			}
		}
		else if(p.x>0.472092) {
			if(p.y<0.404556) {
				if(APU_CONTROL[2]>0) {
					APU_CONTROL[2]--;
					action=true;
				}
			}
			else {
				if(APU_CONTROL[2]<2) {
					APU_CONTROL[2]++;
					action=true;
				}
			}
			if(APU_CONTROL[2]!=1 && APU_READY[2]) {
				APU_STATE[2]=1;
				PlayVesselWave3(sts->SoundID, APU_START, NOLOOP);
			}
		}
		else {
			if(p.y<0.404556) {
				if(APU_CONTROL[1]>0) {
					APU_CONTROL[1]--;
					action=true;
				}
			}
			else {
				if(APU_CONTROL[1]<2) {
					APU_CONTROL[1]++;
					action=true;
				}
			}
			if(APU_CONTROL[1]!=1 && APU_READY[1]) {
				APU_STATE[1]=1;
				PlayVesselWave3(sts->SoundID, APU_START, NOLOOP);
			}
		}
	}
	else if(p.y>0.453919 && p.y<0.492724 && p.x>=0.281316 && p.x<=0.499178) {
		if(p.x<0.354421) {
			if(p.y<0.473246) APU_SPEED_SELECT[0]=0;
			else APU_SPEED_SELECT[0]=1;
		}
		else if(p.x>0.430424) {
			if(p.y<0.473246) APU_SPEED_SELECT[2]=0;
			else APU_SPEED_SELECT[2]=1;
		}
		else {
			if(p.y<0.473246) APU_SPEED_SELECT[1]=0;
			else APU_SPEED_SELECT[1]=1;
		}
		action=true;
	}
	else if(p.y>0.458235 && p.y<0.491960 && p.x>=0.520054 && p.x<=0.579977) {
		if(p.y<0.475573 && APU_AUTO_SHUT_DOWN!=0) {
			APU_AUTO_SHUT_DOWN=0;
			action=true;
		}
		else if(p.y>=0.475573 && APU_AUTO_SHUT_DOWN!=1) {
			APU_AUTO_SHUT_DOWN=1;
			action=true;
		}
	}
	else if(p.y>0.307365 && p.y<0.345728 && p.x>=0.081538 && p.x<=0.312041) {
		if(p.x<0.157321) {
			if(p.y<0.328413) {
				if(HE_ISOLATION_A[1]>0) {
					HE_ISOLATION_A[1]--;
					action=true;
					eng=1;
				}
			}
			else {
				if(HE_ISOLATION_A[1]<2) {
					HE_ISOLATION_A[1]++;
					action=true;
					eng=1;
				}
			}
		}
		else if(p.x>0.239897) {
			if(p.y<0.328413) {
				if(HE_ISOLATION_A[2]>0) {
					HE_ISOLATION_A[2]--;
					action=true;
					eng=2;
				}
			}
			else {
				if(HE_ISOLATION_A[2]<2) {
					HE_ISOLATION_A[2]++;
					action=true;
					eng=2;
				}
			}
		}
		else {
			if(p.y<0.328413) {
				if(HE_ISOLATION_A[0]>0) {
					HE_ISOLATION_A[0]--;
					action=true;
					eng=0;
				}
			}
			else {
				if(HE_ISOLATION_A[0]<2) {
					HE_ISOLATION_A[0]++;
					action=true;
					eng=0;
				}
			}
		}
	}
	else if(p.y>0.375239 && p.y<0.416512 && p.x>=0.085931 && p.x<=0.310743) {
		if(p.x<0.159351) {
			if(p.y<0.396946) {
				if(HE_ISOLATION_B[1]>0) {
					HE_ISOLATION_B[1]--;
					action=true;
					eng=1;
				}
			}
			else {
				if(HE_ISOLATION_B[1]<2) {
					HE_ISOLATION_B[1]++;
					action=true;
					eng=1;
				}
			}
		}
		else if(p.x>0.238773) {
			if(p.y<0.396946) {
				if(HE_ISOLATION_B[2]>0) {
					HE_ISOLATION_B[2]--;
					action=true;
					eng=2;
				}
			}
			else {
				if(HE_ISOLATION_B[2]<2) {
					HE_ISOLATION_B[2]++;
					action=true;
					eng=2;
				}
			}
		}
		else {
			if(p.y<0.396946) {
				if(HE_ISOLATION_B[0]>0) {
					HE_ISOLATION_B[0]--;
					action=true;
					eng=0;
				}
			}
			else {
				if(HE_ISOLATION_B[0]<2) {
					HE_ISOLATION_B[0]++;
					action=true;
					eng=0;
				}
			}
		}
	}
	else if(p.y>0.160622 && p.y<0.196999 && p.x>=0.091011 && p.x<=0.313464) {
		if(p.x<0.159388) {
			if(p.y<0.179752) MPS_ENGINE_POWER[0][1]=0;
			else MPS_ENGINE_POWER[0][1]=1;
			eng=1;
		}
		else if(p.x>0.239655) {
			if(p.y<0.179752) MPS_ENGINE_POWER[0][2]=0;
			else MPS_ENGINE_POWER[0][2]=1;
			eng=2;
		}
		else {
			if(p.y<0.179752) MPS_ENGINE_POWER[0][0]=0;
			else MPS_ENGINE_POWER[0][0]=1;
			eng=0;
		}
		action=true;
	}
	else if(p.y>0.230694 && p.y<0.264614 && p.x>=0.089520 && p.x<=0.316475) {
		if(p.x<0.159198) {
			if(p.y<0.248749) MPS_ENGINE_POWER[1][1]=0;
			else MPS_ENGINE_POWER[1][1]=1;
			eng=1;
		}
		else if(p.x>0.240562) {
			if(p.y<0.248749) MPS_ENGINE_POWER[1][2]=0;
			else MPS_ENGINE_POWER[1][2]=1;
			eng=2;
		}
		else {
			if(p.y<0.248749) MPS_ENGINE_POWER[1][0]=0;
			else MPS_ENGINE_POWER[1][0]=1;
			eng=0;
		}
		action=true;
	}
	else if(p.y>0.884566 && p.y<0.916514 && p.x>=0.193928 && p.x<=0.253453)
	{
		if(p.y<0.901843) {
			CENTERLINE_LATCH=0;
			if(CenterlineLatches.Moving()) CenterlineLatches.action=AnimState::STOPPED;
		}
		else {
			CENTERLINE_LATCH=1;
			CenterlineLatches.action=AnimState::OPENING;
		}
		action=true;
	}
	else if(p.y>0.885751 && p.y<0.920420 && p.x>=0.292763 && p.x<=0.351700)
	{
		if(p.y<0.902700) {
			if(LEFT_DOOR<=1) {
				LEFT_DOOR=0; //open door
				SetETUmbDoorAction(AnimState::OPENING, 0);
			}
			else {
				LEFT_DOOR=1;
				if(LETUmbDoorStatus.Moving())
					SetETUmbDoorAction(AnimState::STOPPED, 0);
			}
		}
		else {
			if(LEFT_DOOR>=1) {
				LEFT_DOOR=2;
				SetETUmbDoorAction(AnimState::CLOSING, 0);
			}
			else {
				LEFT_DOOR=1;
				if(LETUmbDoorStatus.Moving())
					SetETUmbDoorAction(AnimState::STOPPED, 0);
			}
		}
		action=true;
	}
	else if(p.y>0.887672 && p.y<0.922404 && p.x>=0.375219 && p.x<=0.433632)
	{
		if(p.y<0.904728) {
			if(LEFT_LATCH<=1) {
				LEFT_LATCH=0;
				LDoorLatches.action=AnimState::OPENING;
			}
			else {
				LEFT_LATCH=1;
				if(LDoorLatches.Moving()) LDoorLatches.action=AnimState::STOPPED;
			}
		}
		else {
			if(LEFT_LATCH>=1) {
				LEFT_LATCH=2;
				LDoorLatches.action=AnimState::CLOSING;
			}
			else {
				LEFT_LATCH=1;
				if(LDoorLatches.Moving()) LDoorLatches.action=AnimState::STOPPED;
			}
		}
		action=true;
	}
	else if(p.y>0.889641 && p.y<0.923580 && p.x>=0.455335 && p.x<=0.513106)
	{
		if(p.y<0.907640) {
			if(RIGHT_DOOR<=1) {
				RIGHT_DOOR=0; //open door
				SetETUmbDoorAction(AnimState::OPENING, 1);
			}
			else {
				RIGHT_DOOR=1;
				if(RETUmbDoorStatus.Moving())
					RETUmbDoorStatus.action=AnimState::STOPPED;
			}
		}
		else {
			if(RIGHT_DOOR>=1) {
				RIGHT_DOOR=2;
				SetETUmbDoorAction(AnimState::CLOSING, 1);
				//RETUmbDoorStatus.action=AnimState::CLOSING;
			}
			else {
				RIGHT_DOOR=1;
				if(RETUmbDoorStatus.Moving())
					SetETUmbDoorAction(AnimState::STOPPED, 1);
			}
		}
		action=true;
	}
	else if(p.y>0.890511 && p.y<0.925481 && p.x>=0.53287 && p.x<=0.590665)
	{
		if(p.y<0.908759) {
			if(RIGHT_LATCH<=1) {
				RIGHT_LATCH=0;
				RDoorLatches.action=AnimState::OPENING;
			}
			else {
				RIGHT_LATCH=1;
				if(RDoorLatches.Moving()) SetETUmbDoorAction(AnimState::STOPPED, 1);
			}
		}
		else {
			if(RIGHT_LATCH>=1) {
				RIGHT_LATCH=2;
				RDoorLatches.action=AnimState::CLOSING;
			}
			else {
				RIGHT_LATCH=1;
				if(RDoorLatches.Moving()) RDoorLatches.action=AnimState::STOPPED;
			}
		}
		action=true;
	}
	if(action)
	{
		//sprintf(oapiDebugString(), "Switch click");
		if(eng>=0) CheckMPSArmed(eng);
		else {
			CheckAPUReadytoStart();
			CheckAPUShutdown();
		}
		UpdateVC();
	}
	return false;
}


void PanelR2::UpdateVC()
{
	static int eng[3]={1,0,2};
	int i;
	for(i=0;i<3;i++)
	{
		sts->SetAnimation(anim_VC_R2[36+i],APU_FUEL_TK_VLV[i]);
		sts->SetAnimation(anim_VC_R2[41+i],double(BOILER_CNTLRHTR[i])/2.0);
		sts->SetAnimation(anim_VC_R2[44+i],BOILER_CNTLR[i]);
		sts->SetAnimation(anim_VC_R2[53+i],BOILER_N2_SUPPLY[i]);
		sts->SetAnimation(anim_VC_R2[30+i],double(HYD_CIRC_PUMP[i])/2.0);
		sts->SetAnimation(anim_VC_R2[33+i],APU_CNTLR_PWR[i]);
		sts->SetAnimation(anim_VC_R2[14+i],double(APU_CONTROL[i])/2.0);
		sts->SetAnimation(anim_VC_R2[26+i],HYD_MAIN_PUMP_PRESS[i]);
		sts->SetAnimation(anim_VC_R2[19+i],APU_SPEED_SELECT[i]);
		sts->SetAnimation(anim_VC_R2[8+eng[i]],double(HE_ISOLATION_A[i])/2.0);
		sts->SetAnimation(anim_VC_R2[11+eng[i]],double(HE_ISOLATION_B[i])/2.0);
		sts->SetAnimation(anim_VC_R2[2+eng[i]],MPS_ENGINE_POWER[0][i]);
		sts->SetAnimation(anim_VC_R2[5+eng[i]],MPS_ENGINE_POWER[1][i]);
		//oapiVCTriggerRedrawArea (-1, AID_R2_TKBK1+i);
	}
	sts->SetAnimation(anim_VC_R2[22],APU_AUTO_SHUT_DOWN);
	sts->SetAnimation(anim_VC_R2[48],CENTERLINE_LATCH);
	sts->SetAnimation(anim_VC_R2[49],double(LEFT_DOOR)/2.0);
	sts->SetAnimation(anim_VC_R2[50],double(LEFT_LATCH)/2.0);
	sts->SetAnimation(anim_VC_R2[51],double(RIGHT_DOOR)/2.0);
	sts->SetAnimation(anim_VC_R2[52],double(RIGHT_LATCH)/2.0);
	for(i=0;i<8;i++) oapiVCTriggerRedrawArea (-1, AID_R2_TKBK1+i);
	return;
}

void PanelR2::DefineVCAnimations (UINT vcidx)
{
	static VECTOR3 switch_rot = {-0.95,-0.31225,0.0};

	//APU Fuel Tank Valves
	static UINT VC_R2b37_Grp = GRP_R2b37_VC;
	static MGROUP_ROTATE VC_R2b37 (vcidx, &VC_R2b37_Grp, 1,
		_V(1.2136, 1.8363, 14.064), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[36]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[36], 0, 1, &VC_R2b37);

	static UINT VC_R2b38_Grp = GRP_R2b38_VC;
	static MGROUP_ROTATE VC_R2b38 (vcidx, &VC_R2b38_Grp, 1,
		_V(1.2536, 1.8483, 14.062), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[37]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[37], 0, 1, &VC_R2b38);

	static UINT VC_R2b39_Grp = GRP_R2b39_VC;
	static MGROUP_ROTATE VC_R2b39 (vcidx, &VC_R2b39_Grp, 1,
		_V(1.2946, 1.8603, 14.061), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[38]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[38], 0, 1, &VC_R2b39);

	//Boiler Cntlr Pwr/Htr
	static UINT VC_R2b42_Grp = GRP_R2b42_VC;
	static MGROUP_ROTATE VC_R2b42 (vcidx, &VC_R2b42_Grp, 1,
		_V(1.237, 1.84, 13.998), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[41]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[41], 0, 1, &VC_R2b42);

	static UINT VC_R2b43_Grp = GRP_R2b43_VC;
	static MGROUP_ROTATE VC_R2b43 (vcidx, &VC_R2b43_Grp, 1,
		_V(1.276, 1.851, 13.997), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[42]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[42], 0, 1, &VC_R2b43);

	static UINT VC_R2b44_Grp = GRP_R2b44_VC;
	static MGROUP_ROTATE VC_R2b44 (vcidx, &VC_R2b44_Grp, 1,
		_V(1.314, 1.862, 13.997), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[43]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[43], 0, 1, &VC_R2b44);

	//Boiler Cntlr
	static UINT VC_R2b45_Grp = GRP_R2b45_VC;
	static MGROUP_ROTATE VC_R2b45 (vcidx, &VC_R2b45_Grp, 1,
		_V(1.281, 1.845, 13.938), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[44]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[44], 0, 1, &VC_R2b45);

	static UINT VC_R2b46_Grp = GRP_R2b46_VC;
	static MGROUP_ROTATE VC_R2b46 (vcidx, &VC_R2b46_Grp, 1,
		_V(1.308, 1.853, 13.937), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[45]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[45], 0, 1, &VC_R2b46);

	static UINT VC_R2b47_Grp = GRP_R2b47_VC;
	static MGROUP_ROTATE VC_R2b47 (vcidx, &VC_R2b47_Grp, 1,
		_V(1.337, 1.861, 13.936), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[46]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[46], 0, 1, &VC_R2b47);

	//Boiler N2 Sply
	static UINT VC_R2b54_Grp = GRP_R2b54_VC;
	static MGROUP_ROTATE VC_R2b54 (vcidx, &VC_R2b54_Grp, 1,
		_V(1.303, 1.844, 13.88), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[53]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[53], 0, 1, &VC_R2b54);

	static UINT VC_R2b55_Grp = GRP_R2b55_VC;
	static MGROUP_ROTATE VC_R2b55 (vcidx, &VC_R2b55_Grp, 1,
		_V(1.33, 1.851, 13.878), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[54]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[54], 0, 1, &VC_R2b55);

	static UINT VC_R2b56_Grp = GRP_R2b56_VC;
	static MGROUP_ROTATE VC_R2b56 (vcidx, &VC_R2b56_Grp, 1,
		_V(1.358, 1.86, 13.877), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[55]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[55], 0, 1, &VC_R2b56);

	//Hyd Circ Pump
	static UINT VC_R2b31_Grp = GRP_R2b31_VC;
	static MGROUP_ROTATE VC_R2b31 (vcidx, &VC_R2b31_Grp, 1,
		_V(1.096, 1.815, 14.125), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[30]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[30], 0, 1, &VC_R2b31);

	static UINT VC_R2b32_Grp = GRP_R2b32_VC;
	static MGROUP_ROTATE VC_R2b32 (vcidx, &VC_R2b32_Grp, 1,
		_V(1.134, 1.826, 14.124), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[31]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[31], 0, 1, &VC_R2b32);

	static UINT VC_R2b33_Grp = GRP_R2b33_VC;
	static MGROUP_ROTATE VC_R2b33 (vcidx, &VC_R2b33_Grp, 1,
		_V(1.174, 1.838, 14.123), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[32]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[32], 0, 1, &VC_R2b33);

	//APU Cntlr Pwr
	static UINT VC_R2b34_Grp = GRP_R2b34_VC;
	static MGROUP_ROTATE VC_R2b34 (vcidx, &VC_R2b34_Grp, 1,
		_V(1.2106, 1.8433, 14.122), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[33]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[33], 0, 1, &VC_R2b34);

	static UINT VC_R2b35_Grp = GRP_R2b35_VC;
	static MGROUP_ROTATE VC_R2b35 (vcidx, &VC_R2b35_Grp, 1,
		_V(1.2416, 1.8523, 14.121), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[34]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[34], 0, 1, &VC_R2b35);

	static UINT VC_R2b36_Grp = GRP_R2b36_VC;
	static MGROUP_ROTATE VC_R2b36 (vcidx, &VC_R2b36_Grp, 1,
		_V(1.2726, 1.8613, 14.12), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[35]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[35], 0, 1, &VC_R2b36);

	//APU Cntrl
	static UINT VC_R2b15_Grp = GRP_R2b15_VC;
	static MGROUP_ROTATE VC_R2b15 (vcidx, &VC_R2b15_Grp, 1,
		_V(1.1416, 1.8473, 14.307), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[14]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[14], 0, 1, &VC_R2b15);

	static UINT VC_R2b16_Grp = GRP_R2b16_VC;
	static MGROUP_ROTATE VC_R2b16 (vcidx, &VC_R2b16_Grp, 1,
		_V(1.1706, 1.8563, 14.306), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[15]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[15], 0, 1, &VC_R2b16);

	static UINT VC_R2b17_Grp = GRP_R2b17_VC;
	static MGROUP_ROTATE VC_R2b17 (vcidx, &VC_R2b17_Grp, 1,
		_V(1.2006, 1.8643, 14.305), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[16]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[16], 0, 1, &VC_R2b17);

	//Hyd Main Pump Press
	static UINT VC_R2b27_Grp = GRP_R2b27_VC;
	static MGROUP_ROTATE VC_R2b27 (vcidx, &VC_R2b27_Grp, 1,
		_V(1.1496, 1.8333, 14.184), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[26]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[26], 0, 1, &VC_R2b27);

	static UINT VC_R2b28_Grp = GRP_R2b28_VC;
	static MGROUP_ROTATE VC_R2b28 (vcidx, &VC_R2b28_Grp, 1,
		_V(1.1986, 1.8473, 14.182), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[27]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[27], 0, 1, &VC_R2b28);

	static UINT VC_R2b29_Grp = GRP_R2b29_VC;
	static MGROUP_ROTATE VC_R2b29 (vcidx, &VC_R2b29_Grp, 1,
		_V(1.2446, 1.8613, 14.181), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[28]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[28], 0, 1, &VC_R2b29);

	//APU Spd Sel
	static UINT VC_R2b20_Grp = GRP_R2b20_VC;
	static MGROUP_ROTATE VC_R2b20 (vcidx, &VC_R2b20_Grp, 1,
		_V(1.1045, 1.834, 14.249), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[19]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[19], 0, 1, &VC_R2b20);

	static UINT VC_R2b21_Grp = GRP_R2b21_VC;
	static MGROUP_ROTATE VC_R2b21 (vcidx, &VC_R2b21_Grp, 1,
		_V(1.144, 1.845, 14.248), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[20]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[20], 0, 1, &VC_R2b21);

	static UINT VC_R2b22_Grp = GRP_R2b22_VC;
	static MGROUP_ROTATE VC_R2b22 (vcidx, &VC_R2b22_Grp, 1,
		_V(1.182, 1.856, 14.246), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[21]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[21], 0, 1, &VC_R2b22);

	//APU Auto Sht Dwn
	static UINT VC_R2b23_Grp = GRP_R2b23_VC;
	static MGROUP_ROTATE VC_R2b23 (vcidx, &VC_R2b23_Grp, 1,
		_V(1.221, 1.868, 14.245), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[22]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[22], 0, 1, &VC_R2b23);

	//He Isol A
	static UINT VC_R2b9_Grp = GRP_R2b9_VC;
	static MGROUP_ROTATE VC_R2b9 (vcidx, &VC_R2b9_Grp, 1,
		_V(1.008, 1.822, 14.372), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[8]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[8], 0, 1, &VC_R2b9);

	static UINT VC_R2b10_Grp = GRP_R2b10_VC;
	static MGROUP_ROTATE VC_R2b10 (vcidx, &VC_R2b10_Grp, 1,
		_V(1.049, 1.834, 14.371), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[9]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[9], 0, 1, &VC_R2b10);

	static UINT VC_R2b11_Grp = GRP_R2b11_VC;
	static MGROUP_ROTATE VC_R2b11 (vcidx, &VC_R2b11_Grp, 1,
		_V(1.089, 1.845, 14.37), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[10]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[10], 0, 1, &VC_R2b11);

	//He Isol B
	static UINT VC_R2b12_Grp = GRP_R2b12_VC;
	static MGROUP_ROTATE VC_R2b12 (vcidx, &VC_R2b12_Grp, 1,
		_V(1.008, 1.814, 14.313), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[11]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[11], 0, 1, &VC_R2b12);

	static UINT VC_R2b13_Grp = GRP_R2b13_VC;
	static MGROUP_ROTATE VC_R2b13 (vcidx, &VC_R2b13_Grp, 1,
		_V(1.048, 1.825, 14.312), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[12]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[12], 0, 1, &VC_R2b13);

	static UINT VC_R2b14_Grp = GRP_R2b14_VC;
	static MGROUP_ROTATE VC_R2b14 (vcidx, &VC_R2b14_Grp, 1,
		_V(1.088, 1.837, 14.31), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[13]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[13], 0, 1, &VC_R2b14);

	//MPS Eng Pwr
	static UINT VC_R2b3_Grp = GRP_R2b3_VC;
	static MGROUP_ROTATE VC_R2b3 (vcidx, &VC_R2b3_Grp, 1,
		_V(1.009, 1.838, 14.495), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[2]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[2], 0, 1, &VC_R2b3);

	static UINT VC_R2b4_Grp = GRP_R2b4_VC;
	static MGROUP_ROTATE VC_R2b4 (vcidx, &VC_R2b4_Grp, 1,
		_V(1.05, 1.85, 14.494), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[3]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[3], 0, 1, &VC_R2b4);

	static UINT VC_R2b5_Grp = GRP_R2b5_VC;
	static MGROUP_ROTATE VC_R2b5 (vcidx, &VC_R2b5_Grp, 1,
		_V(1.088, 1.861, 14.493), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[4]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[4], 0, 1, &VC_R2b5);

	static UINT VC_R2b6_Grp = GRP_R2b6_VC;
	static MGROUP_ROTATE VC_R2b6 (vcidx, &VC_R2b6_Grp, 1,
		_V(1.008, 1.83, 14.439), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[5]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[5], 0, 1, &VC_R2b6);

	static UINT VC_R2b7_Grp = GRP_R2b7_VC;
	static MGROUP_ROTATE VC_R2b7 (vcidx, &VC_R2b7_Grp, 1,
		_V(1.05, 1.843, 14.437), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[6]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[6], 0, 1, &VC_R2b7);

	static UINT VC_R2b8_Grp = GRP_R2b8_VC;
	static MGROUP_ROTATE VC_R2b8 (vcidx, &VC_R2b8_Grp, 1,
		_V(1.091, 1.855, 14.435), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[7]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R2[7], 0, 1, &VC_R2b8);

	//Centerline Latch
	static UINT VC_R2b49_Grp = GRP_R2b49_VC;
	static MGROUP_ROTATE VC_R2b49 (vcidx, &VC_R2b49_Grp, 1,
		_V(1.062, 1.773, 13.888), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[48]=sts->CreateAnimation(0.5);
	sts->AddAnimationComponent (anim_VC_R2[48], 0, 1, &VC_R2b49);

	//Left Door
	static UINT VC_R2b50_Grp = GRP_R2b50_VC;
	static MGROUP_ROTATE VC_R2b50 (vcidx, &VC_R2b50_Grp, 1,
		_V(1.11, 1.787, 13.885), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[49]=sts->CreateAnimation(0.5);
	sts->AddAnimationComponent (anim_VC_R2[49], 0, 1, &VC_R2b50);

	//Left Latch
	static UINT VC_R2b51_Grp = GRP_R2b51_VC;
	static MGROUP_ROTATE VC_R2b51 (vcidx, &VC_R2b51_Grp, 1,
		_V(1.151, 1.799, 13.884), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[50]=sts->CreateAnimation(0.5);
	sts->AddAnimationComponent (anim_VC_R2[50], 0, 1, &VC_R2b51);

	//Right Door
	static UINT VC_R2b52_Grp = GRP_R2b52_VC;
	static MGROUP_ROTATE VC_R2b52 (vcidx, &VC_R2b52_Grp, 1,
		_V(1.19, 1.811, 13.883), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[51]=sts->CreateAnimation(0.5);
	sts->AddAnimationComponent (anim_VC_R2[51], 0, 1, &VC_R2b52);

	//Right Latch
	static UINT VC_R2b53_Grp = GRP_R2b53_VC;
	static MGROUP_ROTATE VC_R2b53 (vcidx, &VC_R2b53_Grp, 1,
		_V(1.228, 1.822, 13.881), switch_rot, (float)(90.0*RAD));
	anim_VC_R2[52]=sts->CreateAnimation(0.5);
	sts->AddAnimationComponent (anim_VC_R2[52], 0, 1, &VC_R2b53);
	return;
}

void PanelR2::Step(double t, double dt)
{
	double dMass;
	bool bAPURunning=false;
	bHydraulicPressure=false;
	//sprintf(oapiDebugString(), "%f %f %f", CenterlineLatches.pos, LETUmbDoorStatus.pos, RETUmbDoorStatus.pos);
	//sprintf(oapiDebugString(), "%f %f", LDoorLatches.pos, RDoorLatches.pos);
	for(int i=0;i<3;i++)
	{
		if(APU_STATE[i]!=0)
		{
			bAPURunning=true;

			dMass=sts->GetPropellantMass(sts->apu_tank[i]);
			dMass-=APU_FUEL_TANK_FLOWRATE[HYD_MAIN_PUMP_PRESS[i]]*dt;
			if(dMass<=0.0) {
				APU_STATE[i]=0;
				sts->SetPropellantMass(sts->apu_tank[i], 0.0);
				UpdateVC();
				break;
			}
			//else if(sts->apu_tank[i]) sts->SetPropellantMass(sts->apu_tank[i], dMass);
			else sts->SetPropellantMass(sts->apu_tank[i], dMass);
			if(APU_STATE[i]==1) {
				if(APU_Speed[i]>=95.0) {
					APU_Speed[i]=103.0+(1.0*oapiRand())-(1.0*oapiRand());
					APU_STATE[i]=2;
				}
				else {
					APU_Speed[i]+=15.0*dt;
				}
				oapiVCTriggerRedrawArea (-1, AID_R2_TKBK1+i);
				//UpdateVC();
			}
			else {
				APU_Speed[i]+=0.75*(oapiRand()-0.5);
			}
			if(Fuel_Press[i]<1400) 
				Fuel_Press[i]+=(int)(200*dt);
			else if(Fuel_Press[i]>1500) 
				Fuel_Press[i]-=(int)(200*dt);
			else if(Fuel_Press[i]<1425 || Fuel_Press[i]>1480)
				Fuel_Press[i]=1455 - (int)(10.0 * oapiRand());
			else 
				Fuel_Press[i] += (int)(2.0 * oapiRand() - 1.0);

			if(HYD_MAIN_PUMP_PRESS[i]==0) {
				if(Hydraulic_Press[i]<2950) {
					if(Hydraulic_Press[i]>2900) 
						Hydraulic_Press[i]=3000-(int)(10*oapiRand());
					else 
						Hydraulic_Press[i]+=(int)(200*dt);
				}
				else if(Hydraulic_Press[i]>3050)
					Hydraulic_Press[i]-=(int)(200*dt);
				else
					Hydraulic_Press[i]+=(int)(2.0 * oapiRand() - 1.0);
			}
			else {
				if(Hydraulic_Press[i]<850) {
					if(Hydraulic_Press[i]>800) 
						Hydraulic_Press[i] = 900-(int)(10*oapiRand());
					else Hydraulic_Press[i] += (int)(200*dt);
				}
				else if(Hydraulic_Press[i]>950)
					Hydraulic_Press[i]-=(int)(200*dt);
				else {
					Hydraulic_Press[i]+=(int)(2.0*oapiRand() - 1.0);
				}
			}
			if(Hydraulic_Press[i]>2700) bHydraulicPressure=true;
		}
		else {
			if(APU_Speed[i]>5) {
				APU_Speed[i]=max(APU_Speed[i]-15.0*dt, 0.0);
			}
			else 
				APU_Speed[i]=0;
			if(Fuel_Press[i]>25) 
				Fuel_Press[i] -= (int)(200*dt);
			else 
				Fuel_Press[i] = 0;
			if(Hydraulic_Press[i]>50)
				Hydraulic_Press[i] -= (int)(200*dt);
			else 
				Hydraulic_Press[i] = 0;
		}
	}
	//APU sounds
	if(bAPURunning && !IsPlaying3(sts->SoundID, APU_START))
		PlayVesselWave3(sts->SoundID, APU_RUNNING, LOOP);
	else if(!bAPURunning && IsPlaying3(sts->SoundID, APU_RUNNING))
		StopVesselWave3(sts->SoundID, APU_RUNNING);

	if(LETUmbDoorStatus.Moving()) {
		double da=dt*ET_UMB_DOOR_OPERATING_SPEED;
		if(LETUmbDoorStatus.Closing()) {
			LETUmbDoorStatus.pos=max(0.0, LETUmbDoorStatus.pos-da);
			if(LETUmbDoorStatus.pos<=0.0) SetETUmbDoorAction (AnimState::CLOSED, 0);
		}
		else {
			LETUmbDoorStatus.pos=min(1.0, LETUmbDoorStatus.pos+da);
			if(LETUmbDoorStatus.pos>=1.0) SetETUmbDoorAction (AnimState::OPEN, 0);
		}
		sts->SetETUmbDoorPosition(LETUmbDoorStatus.pos, 0);
	}
	if(RETUmbDoorStatus.Moving()) {
		double da=dt*ET_UMB_DOOR_OPERATING_SPEED;
		if(RETUmbDoorStatus.Closing()) {
			RETUmbDoorStatus.pos=max(0.0, RETUmbDoorStatus.pos-da);
			if(RETUmbDoorStatus.pos<=0.0) SetETUmbDoorAction (AnimState::CLOSED, 1);
		}
		else {
			RETUmbDoorStatus.pos=min(1.0, RETUmbDoorStatus.pos+da);
			if(RETUmbDoorStatus.pos>=1.0) SetETUmbDoorAction (AnimState::OPEN, 1);
		}
		sts->SetETUmbDoorPosition(RETUmbDoorStatus.pos, 1);
	}

	//sprintf(oapiDebugString(), "ET DOORS: %f %f", LETUmbDoorStatus.pos, RETUmbDoorStatus.pos);
	if(CenterlineLatches.Moving()) {
		double da=dt*LATCH_OPERATING_SPEED;
		CenterlineLatches.pos=min(1.0, CenterlineLatches.pos+da);
		if(CenterlineLatches.pos>=1.0) {
			CenterlineLatches.action=AnimState::OPEN;
			UpdateVC();
		}
	}
	if(LDoorLatches.Moving()) {
		double da=dt*LATCH_OPERATING_SPEED;
		if(LDoorLatches.Closing()) {
			LDoorLatches.pos=max(0.0, LDoorLatches.pos-da);
			if(LDoorLatches.pos<=0.0) {
				LDoorLatches.action=AnimState::CLOSED;
				UpdateVC();
			}
		}
		else {
			LDoorLatches.pos=min(1.0, LDoorLatches.pos+da);
			if(LDoorLatches.pos>=1.0) {
				LDoorLatches.action=AnimState::OPEN;
				UpdateVC();
			}
		}
	}
	if(RDoorLatches.Moving()) {
		double da=dt*LATCH_OPERATING_SPEED;
		if(RDoorLatches.Closing()) {
			RDoorLatches.pos=max(0.0, RDoorLatches.pos-da);
			if(RDoorLatches.pos<=0.0) {
				RDoorLatches.action=AnimState::CLOSED;
				UpdateVC();
			}
		}
		else {
			RDoorLatches.pos=min(1.0, RDoorLatches.pos+da);
			if(RDoorLatches.pos>=1.0) {
				RDoorLatches.action=AnimState::OPEN;
				UpdateVC();
			}
		}
	}
	return;
}

void PanelR2::CheckMPSArmed(int eng)
{
	if(sts->status>=3) return; //th_main not defined
	if(MPS_ENGINE_POWER[0][eng]==1 && MPS_ENGINE_POWER[1][eng]==1) MPS_STATE[eng]=0;
	else if(HE_ISOLATION_A[eng]==2 && HE_ISOLATION_B[eng]==2) MPS_STATE[eng]=0;
	else if(sts->status==3) MPS_STATE[eng]=0;
	else MPS_STATE[eng]=1;
	if(MPS_STATE[eng]==1) {
		//sts->SetThrusterMax0 (sts->th_main[eng],ORBITER_OMS_THRUST);
		//sts->ex_main[eng]=sts->AddExhaust(sts->th_main[eng], 30.0, 2.0, tex_main);
		sts->SetThrusterResource(sts->th_main[eng], sts->ph_tank);
	}
	else if(MPS_STATE[eng]==0) {
		//sts->SetThrusterMax0 (sts->th_main[eng],0.0);
		//sts->DelExhaust(sts->ex_main[eng]);
		sts->SetThrusterResource(sts->th_main[eng], NULL);
	}
}

void PanelR2::CheckAPUReadytoStart()
{
	int i;
	for(i=0;i<3;i++)
	{
		APU_READY[i]=false;
		if(BOILER_N2_SUPPLY[i]==0 && BOILER_CNTLR[i]==0 && BOILER_CNTLRHTR[i]!=1)
		{
			if(APU_FUEL_TK_VLV[i]==0 && APU_AUTO_SHUT_DOWN==0)
			{
				if(HYD_MAIN_PUMP_PRESS[i]==1 && APU_CNTLR_PWR[i]==0 && APU_CONTROL[i]==1)
				{
					APU_READY[i]=true;
				}
			}
		}
	}
}

void PanelR2::CheckAPUShutdown()
{
	int i;
	bool bAPUShutdown=false;
	for(i=0;i<3;i++)
	{
		if(APU_STATE[i]!=0) {
			if(APU_CONTROL[i]==1 || APU_FUEL_TK_VLV[i]==1 || APU_CNTLR_PWR[i]==1
				|| (APU_STATE[i]==1 && HYD_MAIN_PUMP_PRESS[i]==0))
			{
				APU_STATE[i]=0;
				bAPUShutdown=true;
			}
		}
	}
	if(bAPUShutdown) PlayVesselWave3(sts->SoundID, APU_SHUTDOWN, NOLOOP);
}

void PanelR2::SetETUmbDoorAction(AnimState::Action action, int door)
{
	if(action==AnimState::CLOSING && CenterlineLatches.pos!=1.0)
		return; //aplies to both doors
	if(door==0) {
		LETUmbDoorStatus.action=action;
	}
	else {
		RETUmbDoorStatus.action=action;
	}
	UpdateVC();
	return;
}

bool PanelR2::ParseScenarioLine (char *line)
{
	int nNum, nSwitch;
	//char cbuf[255];
	if (!_strnicmp (line, "BOILERN2SPLY", 12)) 
	{
		sscanf_s (line+12, "%d", &nSwitch);
		sscanf_s (line+13, "%d", &nNum);
		if(nSwitch<3) BOILER_N2_SUPPLY[nSwitch]=nNum;
		return true;
	}
	else if (!_strnicmp (line, "BOILERCNTLR", 11)) 
	{
		sscanf_s (line+11, "%d", &nSwitch);
		sscanf_s (line+12, "%d", &nNum);
		if(nSwitch<3) BOILER_CNTLR[nSwitch]=nNum;
		return true;
	}
	else if (!_strnicmp (line, "BOILERHTR", 9)) 
	{
		sscanf_s (line+9, "%d", &nSwitch);
		sscanf_s (line+10, "%d", &nNum);
		if(nSwitch<3) BOILER_CNTLRHTR[nSwitch]=nNum;
		return true;
	}
	else if (!_strnicmp (line, "APUFUTKVLV", 10)) 
	{
		sscanf_s (line+10, "%d", &nSwitch);
		sscanf_s (line+11, "%d", &nNum);
		if(nSwitch<3) APU_FUEL_TK_VLV[nSwitch]=nNum;
		return true;
	}
	else if (!_strnicmp (line, "APUCNTLRPWR", 11)) 
	{
		sscanf_s (line+11, "%d", &nSwitch);
		sscanf_s (line+12, "%d", &nNum);
		if(nSwitch<3) APU_CNTLR_PWR[nSwitch]=nNum;
		return true;
	}
	else if (!_strnicmp (line, "HYDCIRCPUMP", 11)) 
	{
		sscanf_s (line+11, "%d", &nSwitch);
		sscanf_s (line+12, "%d", &nNum);
		if(nSwitch<3) HYD_CIRC_PUMP[nSwitch]=nNum;
		return true;
	}
	else if (!_strnicmp (line, "APUAUTOSHTDN", 12)) 
	{
		sscanf_s (line+12, "%d", &nNum);
		APU_AUTO_SHUT_DOWN=nNum;
		return true;
	}
	else if (!_strnicmp (line, "APUSPDSEL", 9)) 
	{
		sscanf_s (line+9, "%d", &nSwitch);
		sscanf_s (line+10, "%d", &nNum);
		if(nSwitch<3) APU_SPEED_SELECT[nSwitch]=nNum;
		return true;
	}
	else if (!_strnicmp (line, "APUCNTRL", 8)) 
	{
		sscanf_s (line+8, "%d", &nSwitch);
		sscanf_s (line+9, "%d", &nNum);
		if(nSwitch<3) APU_CONTROL[nSwitch]=nNum;
		/*sprintf(cbuf, "APUCNTRL %i %i", nSwitch, nNum);
		oapiWriteLog(cbuf);*/
		return true;
	}
	else if (!_strnicmp (line, "HYDMNPMPPRESS", 13)) 
	{
		sscanf_s (line+13, "%d", &nSwitch);
		sscanf_s (line+14, "%d", &nNum);
		if(nSwitch<3) HYD_MAIN_PUMP_PRESS[nSwitch]=nNum;
		return true;
	}
	else if (!_strnicmp (line, "MPSENGPWR0", 10)) 
	{
		sscanf_s (line+10, "%d", &nSwitch);
		sscanf_s (line+11, "%d", &nNum);
		if(nSwitch<3) MPS_ENGINE_POWER[0][nSwitch]=nNum;
		return true;
	}
	else if (!_strnicmp (line, "MPSENGPWR1", 10)) 
	{
		sscanf_s (line+10, "%d", &nSwitch);
		sscanf_s (line+11, "%d", &nNum);
		if(nSwitch<3) MPS_ENGINE_POWER[1][nSwitch]=nNum;
		return true;
	}
	else if (!_strnicmp (line, "HEISOLA", 7)) 
	{
		sscanf_s (line+7, "%d", &nSwitch);
		sscanf_s (line+8, "%d", &nNum);
		if(nSwitch<3) HE_ISOLATION_A[nSwitch]=nNum;
		return true;
	}
	else if (!_strnicmp (line, "HEISOLB", 7)) 
	{
		sscanf_s (line+7, "%d", &nSwitch);
		sscanf_s (line+8, "%d", &nNum);
		if(nSwitch<3) HE_ISOLATION_B[nSwitch]=nNum;
		return true;
	}
	else if (!_strnicmp (line, "APU", 3)) 
	{
		sscanf_s (line+3, "%d", &nSwitch);
		sscanf_s (line+4, "%d", &nNum);
		if(nSwitch<3) APU_STATE[nSwitch]=nNum;
		return true;
	}
	else if (!_strnicmp (line, "HYDRAULIC_PRESS", 15)) 
	{
		sscanf_s (line+15, "%d", &nSwitch);
		sscanf_s (line+16, "%d", &nNum);
		if(nSwitch<3) Hydraulic_Press[nSwitch]=nNum;
		return true;
	}
	else if (!_strnicmp (line, "FUEL_PRESS", 10)) 
	{
		sscanf_s (line+10, "%d", &nSwitch);
		sscanf_s (line+11, "%d", &nNum);
		if(nSwitch<3) Fuel_Press[nSwitch]=nNum;
		return true;
	}
	else if (!_strnicmp (line, "MPS", 3)) 
	{
		sscanf_s (line+3, "%d", &nSwitch);
		sscanf_s (line+4, "%d", &nNum);
		if(nSwitch<3) MPS_STATE[nSwitch]=nNum;
		return true;
	}
	else if(!_strnicmp(line, "L_ET_UMB_DOOR", 13))
	{
		sscanf_s (line+13, "%lf", &LETUmbDoorStatus.pos);
		if(LETUmbDoorStatus.pos==0.0) LETUmbDoorStatus.action=AnimState::CLOSED;
		else if(LETUmbDoorStatus.pos==1.0) LETUmbDoorStatus.action=AnimState::OPEN;
		else LETUmbDoorStatus.action=AnimState::STOPPED;
		sts->SetETUmbDoorPosition(LETUmbDoorStatus.pos, 0);
		return true;
	}
	else if(!_strnicmp(line, "R_ET_UMB_DOOR", 13))
	{
		sscanf_s (line+13, "%lf", &RETUmbDoorStatus.pos);
		if(RETUmbDoorStatus.pos==0.0) RETUmbDoorStatus.action=AnimState::CLOSED;
		else if(RETUmbDoorStatus.pos==1.0) RETUmbDoorStatus.action=AnimState::OPEN;
		else RETUmbDoorStatus.action=AnimState::STOPPED;
		sts->SetETUmbDoorPosition(RETUmbDoorStatus.pos, 1);
		return true;
	}
	else if(!_strnicmp(line, "ET_DOOR_LATCHES", 15))
	{
		sscanf_s(line+15, "%lf%lf%lf", &CenterlineLatches.pos, &LDoorLatches.pos, &RDoorLatches.pos);
		if(CenterlineLatches.pos==0.0) CenterlineLatches.action=AnimState::CLOSED;
		else if(CenterlineLatches.pos==1.0) CenterlineLatches.action=AnimState::OPEN;
		else CenterlineLatches.action=AnimState::STOPPED;

		if(LDoorLatches.pos==0.0) LDoorLatches.action=AnimState::CLOSED;
		else if(LDoorLatches.pos==1.0) LDoorLatches.action=AnimState::OPEN;
		else LDoorLatches.action=AnimState::STOPPED;

		if(RDoorLatches.pos==0.0) RDoorLatches.action=AnimState::CLOSED;
		else if(RDoorLatches.pos==1.0) RDoorLatches.action=AnimState::OPEN;
		else RDoorLatches.action=AnimState::STOPPED;
	}
	return false;
}

void PanelR2::SaveState (FILEHANDLE scn)
{
	char cbuf[256];
	/*for(int i=0;i<3;i++) {
		if(APU_STATE[i]!=0) {
			sprintf(cbuf, "%i %i %i", i, Hydraulic_Press[i], Fuel_Press[i]);
			oapiWriteScenario_string (scn, "APU", cbuf);
		}
	}*/
	oapiWriteScenario_int (scn, "BOILERN2SPLY0", BOILER_N2_SUPPLY[0]);
	oapiWriteScenario_int (scn, "BOILERN2SPLY1", BOILER_N2_SUPPLY[1]);
	oapiWriteScenario_int (scn, "BOILERN2SPLY2", BOILER_N2_SUPPLY[2]);
	oapiWriteScenario_int (scn, "BOILERCNTLR0", BOILER_CNTLR[0]);
	oapiWriteScenario_int (scn, "BOILERCNTLR1", BOILER_CNTLR[1]);
	oapiWriteScenario_int (scn, "BOILERCNTLR2", BOILER_CNTLR[2]);
	oapiWriteScenario_int (scn, "BOILERHTR0", BOILER_CNTLRHTR[0]);
	oapiWriteScenario_int (scn, "BOILERHTR1", BOILER_CNTLRHTR[1]);
	oapiWriteScenario_int (scn, "BOILERHTR2", BOILER_CNTLRHTR[2]);
	oapiWriteScenario_int (scn, "APUFUTKVLV0", APU_FUEL_TK_VLV[0]);
	oapiWriteScenario_int (scn, "APUFUTKVLV1", APU_FUEL_TK_VLV[1]);
	oapiWriteScenario_int (scn, "APUFUTKVLV2", APU_FUEL_TK_VLV[2]);
	oapiWriteScenario_int (scn, "APUCNTLRPWR0", APU_CNTLR_PWR[0]);
	oapiWriteScenario_int (scn, "APUCNTLRPWR1", APU_CNTLR_PWR[1]);
	oapiWriteScenario_int (scn, "APUCNTLRPWR2", APU_CNTLR_PWR[2]);
	oapiWriteScenario_int (scn, "HYDCIRCPUMP0", HYD_CIRC_PUMP[0]);
	oapiWriteScenario_int (scn, "HYDCIRCPUMP1", HYD_CIRC_PUMP[1]);
	oapiWriteScenario_int (scn, "HYDCIRCPUMP2", HYD_CIRC_PUMP[2]);
	oapiWriteScenario_int (scn, "APUAUTOSHTDN", APU_AUTO_SHUT_DOWN);
	oapiWriteScenario_int (scn, "APUSPDSEL0", APU_SPEED_SELECT[0]);
	oapiWriteScenario_int (scn, "APUSPDSEL1", APU_SPEED_SELECT[1]);
	oapiWriteScenario_int (scn, "APUSPDSEL2", APU_SPEED_SELECT[2]);
	oapiWriteScenario_int (scn, "APUCNTRL0", APU_CONTROL[0]);
	oapiWriteScenario_int (scn, "APUCNTRL1", APU_CONTROL[1]);
	oapiWriteScenario_int (scn, "APUCNTRL2", APU_CONTROL[2]);
	oapiWriteScenario_int (scn, "HYDMNPMPPRESS0", HYD_MAIN_PUMP_PRESS[0]);
	oapiWriteScenario_int (scn, "HYDMNPMPPRESS1", HYD_MAIN_PUMP_PRESS[1]);
	oapiWriteScenario_int (scn, "HYDMNPMPPRESS2", HYD_MAIN_PUMP_PRESS[2]);
	oapiWriteScenario_int (scn, "MPSENGPWR00", MPS_ENGINE_POWER[0][0]);
	oapiWriteScenario_int (scn, "MPSENGPWR01", MPS_ENGINE_POWER[0][1]);
	oapiWriteScenario_int (scn, "MPSENGPWR02", MPS_ENGINE_POWER[0][2]);
	oapiWriteScenario_int (scn, "MPSENGPWR10", MPS_ENGINE_POWER[1][0]);
	oapiWriteScenario_int (scn, "MPSENGPWR11", MPS_ENGINE_POWER[1][1]);
	oapiWriteScenario_int (scn, "MPSENGPWR12", MPS_ENGINE_POWER[1][2]);
	oapiWriteScenario_int (scn, "HEISOLA0", HE_ISOLATION_A[0]);
	oapiWriteScenario_int (scn, "HEISOLA1", HE_ISOLATION_A[1]);
	oapiWriteScenario_int (scn, "HEISOLA2", HE_ISOLATION_A[2]);
	oapiWriteScenario_int (scn, "HEISOLB0", HE_ISOLATION_B[0]);
	oapiWriteScenario_int (scn, "HEISOLB1", HE_ISOLATION_B[1]);
	oapiWriteScenario_int (scn, "HEISOLB2", HE_ISOLATION_B[2]);
	oapiWriteScenario_int (scn, "APU0", APU_STATE[0]);
	oapiWriteScenario_int (scn, "APU1", APU_STATE[1]);
	oapiWriteScenario_int (scn, "APU2", APU_STATE[2]);
	oapiWriteScenario_int (scn, "HYDRAULIC_PRESS0", Hydraulic_Press[0]);
	oapiWriteScenario_int (scn, "HYDRAULIC_PRESS1", Hydraulic_Press[1]);
	oapiWriteScenario_int (scn, "HYDRAULIC_PRESS2", Hydraulic_Press[2]);
	oapiWriteScenario_int (scn, "FUEL_PRESS0", Fuel_Press[0]);
	oapiWriteScenario_int (scn, "FUEL_PRESS1", Fuel_Press[1]);
	oapiWriteScenario_int (scn, "FUEL_PRESS2", Fuel_Press[2]);
	oapiWriteScenario_int (scn, "MPS0", MPS_STATE[0]);
	oapiWriteScenario_int (scn, "MPS1", MPS_STATE[1]);
	oapiWriteScenario_int (scn, "MPS2", MPS_STATE[2]);
	oapiWriteScenario_float (scn, "L_ET_UMB_DOOR", LETUmbDoorStatus.pos);
	oapiWriteScenario_float (scn, "R_ET_UMB_DOOR", RETUmbDoorStatus.pos);
	sprintf_s(cbuf, 255, "%f %f %f", CenterlineLatches.pos, LDoorLatches.pos, RDoorLatches.pos);
	oapiWriteScenario_string (scn, "ET_DOOR_LATCHES", cbuf);
}
