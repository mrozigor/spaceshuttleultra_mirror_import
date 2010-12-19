/****************************************************************************
  This file is part of Space Shuttle Ultra

  Payload Bay Operation definition



  Space Shuttle Ultra is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Space Shuttle Ultra is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Space Shuttle Ultra; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See http://spaceshuttleultra.sourceforge.net/license/ for more details.

  **************************************************************************/
#ifndef __PLBAYOP_H
#define __PLBAYOP_H

#include <windows.h>
#include "Atlantis.h"

// ==============================================================
// class PayloadBayOp
// Manages payload bay operations (bay doors and radiators),
// both the physical state of the systems, and the state of
// the user interface (panel switches, etc.)
// ==============================================================

class PayloadBayOp {
	friend class Atlantis;
	friend class PanelA8;

public:
	PayloadBayOp (Atlantis *_sts);
	void DefineAnimations (UINT vcidx);
	void RegisterVC ();
	void UpdateVC ();
	bool VCMouseEvent (int id, int event, VECTOR3 &p);
	bool VCRedrawEvent (int id, int event, SURFHANDLE surf);

	void Step (double t, double dt);
	inline AnimState::Action GetDoorAction () const { return BayDoorStatus.action; }
	void SetDoorAction (AnimState::Action action);
	void AutoDoorSequenceControl(); //controls automatic door sequence; used to start next animation in sequence
	//void RevertDoorAction (); // simplified operation

	inline AnimState::Action GetRadiatorAction () const { return RadiatorStatus.action; }
	void SetRadiatorAction (AnimState::Action action);

	void SetRadLatchAction (AnimState::Action action);

	void SetKuAntennaAction (AnimState::Action action);
	void RevertKuAntennaAction (); // simplified operation

	bool ParseScenarioLine (char *line);
	void SaveState (FILEHANDLE scn);

private:
	Atlantis *sts; // vessel instance pointer
	bool VCDrawTalkback (SURFHANDLE surf, int idx, int label);

	// status of control switches
	enum {BD_ENABLE,BD_DISABLE} BayDoor[2];
	enum {BDO_OPEN,BDO_STOP,BDO_CLOSE} BayDoorOp;
	enum {MP_ON,MP_OFF} MechPwr[2];
	enum {LC_RELEASE,LC_OFF,LC_LATCH} RadLatchCtrl[2];
	enum {RC_DEPLOY,RC_OFF,RC_STOW} RadiatorCtrl[2];
	enum {KU_DEPLOY,KU_GND,KU_STOW} KuCtrl;
	enum {KU_DIRECT_ON, KU_DIRECT_OFF} KuDirectCtrl;

	// VC switch animations
	UINT anim_VC_R13L[11];
	int tkbk_state[6];

	// physical status
	AnimState BayDoorStatus;
	AnimState CLBayDoorLatch[4]; //1_4, 5_8, 9_12, 13_16
	AnimState RadiatorStatus;
	AnimState RadLatchStatus;
	AnimState KuAntennaStatus;
};

#endif // !__PLBAYOP_H
