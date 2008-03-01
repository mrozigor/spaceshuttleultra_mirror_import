#include "GearOp.h"
#include "resource.h"
#include "meshres.h"
#include "meshres_vc.h"
#include "DlgCtrl.h"
#include <stdio.h>

extern GDIParams g_Param;
extern HELPCONTEXT g_hc;
extern char *ActionString[5];

// ==============================================================

GearOp::GearOp (Atlantis *_sts): sts(_sts) {
	
	int i;

	// Button Status
	for (i = 0; i < 2; i++) {
		SwitchCover[i] = SWC_CLOSE;
		SwitchPush[i]=SW_NOT_PUSH;
	}
	GearDamaged=false;
/*
	BayDoorOp = BDO_STOP;
	GearStatus.Set(AnimState::CLOSED, 0);
*/
}

// ==============================================================

void GearOp::Step (double t, double dt) {
  // ***** Animate landing gear *****

  if (gear_status >= AnimState::CLOSING) {
    double da = dt * GEAR_OPERATING_SPEED;
    if (gear_status == AnimState::CLOSING) { // retract gear
      if (gear_proc > 0.0) gear_proc = max (0.0, gear_proc-da);
	  else {
		  gear_status = AnimState::CLOSED;
		  UpdateVC();
	  }
    } else {                           // deploy gear
      if (gear_proc < 1.0) gear_proc = min (1.0, gear_proc+da);
	  else {
		  gear_status = AnimState::OPEN;
		  UpdateVC();
	  }
    }
    sts->SetAnimation (anim_gear, gear_proc);
    SetGearParameters (gear_proc);
  }

}

void GearOp::SetGearParameters (double state) {
  if (state == 1.0) { // gear fully deployed
    sts->SetTouchdownPoints (_V(0,-4,17), _V(-3.96,-5.6,-4.3), _V(3.96,-5.6,-4.3)); // gear wheel tips
    sts->SetSurfaceFrictionCoeff (0.05, 0.4);
  } else {
    sts->SetTouchdownPoints (_V(0,-2.5,14), _V(-8,-2.8,-9), _V(8,-2.8,-9)); // belly landing
    sts->SetSurfaceFrictionCoeff (0.4, 0.4);
  }
}


// ==============================================================

bool GearOp::ParseScenarioLine (char *line) {
  int action, damage;
  if (!strnicmp (line, "GEAR", 4)) {
    sscanf (line+4, "%d%lf", &action, &gear_proc);
    gear_status = (AnimState::Action)(action+1);
	return true;
  }	
  else if (!strnicmp (line, "GDAMAGE", 7)) {
	  sscanf (line+7, "%d", &damage);
	  if(damage==1) {
		  GearDamaged=true;
		  DamageGear();
	  }
	  return true;
  }
  return false;
}

// ==============================================================

void GearOp::SaveState (FILEHANDLE scn) {
  char cbuf[256];
  sprintf (cbuf, "%d %0.4f", max(gear_status-1, 0), gear_proc); //if gear_status==0, set gear to closed state
  oapiWriteScenario_string (scn, "GEAR", cbuf);
  if(GearDamaged) oapiWriteScenario_int (scn, "GDAMAGE", 1);
}


// ==============================================================

void GearOp::DefineVCAnimations (UINT vcidx) {
	// Animations for switches on panel F6 in the VC
	static UINT VC_F6_Cover1_Grp = GRP_F6cover1_VC;
	static MGROUP_ROTATE VC_F6cover1 (vcidx, &VC_F6_Cover1_Grp, 1,
		_V(-0.945,1.93,15.043), _V(0.0,0.9,0.3), (float)(90.0*RAD));
	anim_VC_F6[0] = sts->CreateAnimation(0.0);
	sts->AddAnimationComponent(anim_VC_F6[0], 0, 1, &VC_F6cover1);

	static UINT VC_F6_Cover2_Grp = GRP_F6cover2_VC;
	static MGROUP_ROTATE VC_F6cover2 (vcidx, &VC_F6_Cover2_Grp, 1,
		_V(-0.897,1.93,15.043), _V(0.0,0.9,0.3), (float)(90.0*RAD));
	anim_VC_F6[1] = sts->CreateAnimation(0.0);
	sts->AddAnimationComponent(anim_VC_F6[1], 0, 1, &VC_F6cover2);

}
 
// ==============================================================

void GearOp::RegisterVC ()
{
	VECTOR3 ofs = sts->orbiter_ofs;
	SURFHANDLE tkbk_tex = oapiGetTextureHandle (sts->hOrbiterVCMesh, 5);

	// register the complete panel for mouse events
	oapiVCRegisterArea (AID_F6, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN);
	oapiVCSetAreaClickmode_Quadrilateral (AID_F6, _V(-0.97, 2.06, 15.0)+ofs, _V(-0.43, 2.06, 15.0)+ofs, _V(-0.97, 1.88, 15.0)+ofs, _V(-0.43, 1.88, 15.0)+ofs);
	// register the talkbacks
	/*oapiVCRegisterArea (AID_F6_TKBK1, _R(  0,0, 32,18), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tkbk_tex);
	oapiVCRegisterArea (AID_F6_TKBK2, _R( 32,0, 64,18), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tkbk_tex);
	oapiVCRegisterArea (AID_F6_TKBK3, _R( 64,0, 96,18), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tkbk_tex);*/
	//oapiVCRegisterArea (AID_F6_TKBK1, _R(154, 33.453824, 197.299712, 59.281664), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tkbk_tex);
	oapiVCRegisterArea (AID_F6_TKBK1, _R(157, 36, 189, 54), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tkbk_tex);
	//oapiVCRegisterArea (AID_F6_TKBK2, _R(224,0, 256,18), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tkbk_tex);
	//oapiVCRegisterArea (AID_F6_TKBK2, _R(153.869568, 32.94336, 197.169152, 58.7712), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tkbk_tex);
	oapiVCRegisterArea (AID_F6_TKBK2, _R(157, 36, 189, 54), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tkbk_tex);
	//oapiVCRegisterArea (AID_F6_TKBK3, _R(0, 18, 32, 32), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tkbk_tex);
	//oapiVCRegisterArea (AID_F6_TKBK3, _R(153.700096, 33.655552, 196.999936, 59.483392), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tkbk_tex);
	oapiVCRegisterArea (AID_F6_TKBK3, _R(157, 36, 189, 54), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tkbk_tex);

	for (int i = 0; i < 3; i++) tkbk_state[i] = 0;
}

// ==============================================================

void GearOp::UpdateVC () {
	int i;
	for (i = 0; i < 2; i++) {
		sts->SetAnimation (anim_VC_F6[i], (SwitchCover[i] == SWC_OPEN) ? 1:0);
	}
	for (i = 0; i < 3; i++)
		oapiVCTriggerRedrawArea (-1, AID_F6_TKBK1+i);
}

// ==============================================================

bool GearOp::VCDrawTalkback (SURFHANDLE surf, int idx, int label) {
	//sprintf(oapiDebugString(), "VCRedrawEvent");
	if (tkbk_state[idx] == label) return false; // nothing to do
	//sprintf(oapiDebugString(), "idx %d label %d tkbk_state %d", idx, label, tkbk_state[idx]);
	tkbk_state[idx] = label;
	oapiBlt (surf, g_Param.tkbk_label, 0, 0, label*32, 0, 32, 18);
	return true;
}

// ==============================================================

bool GearOp::VCMouseEvent (int id, int event, VECTOR3 &p)
{
	if (id != AID_F6) return false;
	bool action = false;

	//sprintf(oapiDebugString(), "id %d event %d p %f %f %f",id,event,p.x,p.y,p.z);
	if (p.y >= 0.382 && p.y <= 0.575) {
		if (p.x >= 0.084 && p.x <= 0.164) {
    		if(SwitchCover[0]==SWC_CLOSE) {
				SwitchCover[0] = SWC_OPEN; 
			} else if(p.x<0.106) {
				SwitchCover[0] = SWC_CLOSE; 
			} else if(SwitchPush[0]==SW_NOT_PUSH) {
				SwitchPush[0]=SW_PUSH;
				//sprintf(oapiDebugString(), "Gear Armed");
			} else {
				SwitchPush[0]=SW_NOT_PUSH;
				//sprintf(oapiDebugString(), "Gear Locked");
			}
			action = true;
		} 
		if (p.x >= 0.164 && p.x <= 0.237) {
    		if(SwitchCover[1]==SWC_CLOSE) {
				SwitchCover[1] = SWC_OPEN; 
			} else if(p.x<0.186) {
				SwitchCover[1] = SWC_CLOSE; 
			} else if(SwitchPush[0]==SW_PUSH) {
				SwitchPush[1]=SW_PUSH;
				RevertLandingGear();
				//sprintf(oapiDebugString(), "Gear");
			}
			action = true;
		} 
	}
	if (action) {
		UpdateVC ();
	}
	return false;
}

void GearOp::OperateLandingGear (AnimState::Action action) {
  if (sts->status < 3) return;
  // operate landing gear only once the orbiter is free from the tank
  //sprintf(oapiDebugString(), "Gear"); //display gear change
  gear_status = action;
  sts->RecordEvent ("GEAR", action == AnimState::CLOSING ? "UP" : "DOWN");
}

void GearOp::RevertLandingGear () {
  if(SwitchPush[0]==SW_PUSH && !GearDamaged) OperateLandingGear (gear_status == AnimState::CLOSED || gear_status == AnimState::CLOSING ?
    AnimState::OPENING : AnimState::CLOSING);
}

void GearOp::ArmGear()
{
	SwitchPush[0]=SW_PUSH;
}

bool GearOp::GearArmed()
{
	if(SwitchPush[0]==SW_PUSH) return true;
	else return false;
}

void GearOp::DamageGear()
{
	if(GearDamaged) return;
	GearDamaged=true;
	gear_status=AnimState::CLOSED;
	gear_proc=0.0;
	sts->SetAnimation (anim_gear, gear_proc);
    SetGearParameters (gear_proc);
}

// ==============================================================

bool GearOp::VCRedrawEvent (int id, int event, SURFHANDLE surf) {
    //sprintf(oapiDebugString(), "VCRedrawEvent");
	switch (id) {
	  case AID_F6_TKBK1: 
	  case AID_F6_TKBK2:
	  case AID_F6_TKBK3: {
		static int label[5] = {0,2,4,0,0};
		if(gear_status==AnimState::CLOSING || gear_status==AnimState::OPENING)
			return VCDrawTalkback (surf, id-AID_F6_TKBK1, label[0]);
		else if(gear_status==AnimState::CLOSED)
			return VCDrawTalkback (surf, id-AID_F6_TKBK1, label[1]);
		else return VCDrawTalkback (surf, id-AID_F6_TKBK1, label[2]);
		}
	}
	return false;
}

void GearOp::DefineAnimations () {
  UINT midx = 1; // mesh index for all external animations
  //DaveS edit: Fixed animations so they align correctly with the scaled down orbiter mesh

  static UINT LNosewheelDoorGrp[1] = {GRP_NOSEDOORL};
  static MGROUP_ROTATE LNosewheelDoor (midx, LNosewheelDoorGrp, 1,
    _V(-0.651575,-2.090875,16.5325), _V(0, 0.1896375, 0.9540225), (float)(-60.0*RAD));
  static UINT RNosewheelDoorGrp[1] = {GRP_NOSEDOORR};
  static MGROUP_ROTATE RNosewheelDoor (midx, RNosewheelDoorGrp, 1,
    _V(0.651575,-2.090875,16.5325), _V(0, 0.1896375, 0.9540225), (float)(60.0*RAD));
  static UINT NosewheelGrp[2] = {GRP_NOSEWHEEL,GRP_NOSEGEAR};
  static MGROUP_ROTATE Nosewheel (midx, NosewheelGrp, 2,
    _V(0.5835,-1.945,17.6995), _V(1, 0, 0), (float)(94.5*RAD));
  static UINT RGearDoorGrp[1] = {GRP_GEARDOORR};
  static MGROUP_ROTATE RGearDoor (midx, RGearDoorGrp, 1,
    _V(4.25, -2.39, -1.69), _V(0, 0.02, 0.9), (float)(96.2*RAD));
  static UINT LGearDoorGrp[1] = {GRP_GEARDOORL};
  static MGROUP_ROTATE LGearDoor (midx, LGearDoorGrp, 1,
    _V(-4.25, -2.39, -1.69), _V(0, 0.02, 0.9), (float)(-96.2*RAD));
  static UINT MainGearGrp[4] = {GRP_WHEELR,GRP_GEARR,GRP_WHEELL,GRP_GEARL};
  static MGROUP_ROTATE MainGear (midx, MainGearGrp, 4,
    _V(0, -2.26, -3.68), _V(1, 0, 0), (float)(94.5*RAD));

  anim_gear = sts->CreateAnimation (0);
  sts->AddAnimationComponent (anim_gear, 0,   0.5, &LNosewheelDoor);
  sts->AddAnimationComponent (anim_gear, 0,   0.5, &RNosewheelDoor);
  sts->AddAnimationComponent (anim_gear, 0.4, 1.0, &Nosewheel);
  sts->AddAnimationComponent (anim_gear, 0,   0.5, &RGearDoor);
  sts->AddAnimationComponent (anim_gear, 0,   0.5, &LGearDoor);
  sts->AddAnimationComponent (anim_gear, 0.4, 1.0, &MainGear);
}

void GearOp::UpdateMesh() {
  sts->SetAnimation (anim_gear, gear_proc);
}