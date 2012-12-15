/****************************************************************************
  This file is part of Space Shuttle Ultra

  (old) Panel R2 definition



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
#ifndef __PANELR2_H
#define __PANELR2_H

#include <windows.h>
#include "Atlantis.h"
#include "discsignals/discsignals.h"

// ==============================================================
// class Panel R2
// switches on Panel R2
// ==============================================================

class PanelR2 {
	friend class Atlantis;
	friend class CRT;

public:
	PanelR2 (Atlantis *_sts);
	//int nAPUOn();
	virtual bool HydraulicPressure();
	void DefineAnimations ();
	void DefineVCAnimations (UINT vcidx);
	void RegisterVC ();
	void UpdateVC ();
	void UpdateMesh();
	bool VCMouseEvent (int id, int event, VECTOR3 &p);
	bool VCRedrawEvent (int id, int event, SURFHANDLE surf);
	void Step (double t, double dt);
	bool ParseScenarioLine (char *line);
	void SaveState (FILEHANDLE scn);
	void Realize();
private:
	Atlantis *sts; // vessel instance pointer

	bool VCDrawTalkback (SURFHANDLE surf, int idx, int label);
	void CheckMPSArmed(int eng);
	//void CheckAPUReadytoStart();
	//void CheckAPUShutdown();

	void SetETUmbDoorAction(AnimState::Action action, int door);

	// status of switches
	int BOILER_N2_SUPPLY[3]; // 0=ON, 1=OFF;
	int BOILER_CNTLR[3]; // 0=ON, 1=OFF;
	int BOILER_CNTLRHTR[3]; // 0=A, 1=OFF, 2=B;
	int APU_FUEL_TK_VLV[3]; // 0=OPEN, 1=CLOSE;
	//int APU_FUEL_PUMPVLV_COOL[2]; // 0=AUTO, 1=OFF;
	int APU_CNTLR_PWR[3]; // 0=ON, 1=OFF;
	int HYD_CIRC_PUMP[3]; // 0=ON, 1=GPC, 2=OFF;
	int APU_AUTO_SHUT_DOWN; // 0=ENABLE, 1=INHIBIT;
	int APU_SPEED_SELECT[3]; // 0=HIGH, 1=NORM;
	int APU_CONTROL[3]; // 0=START/RUN, 1=OFF, 2=START ORIDE/RUN;
	int HYD_MAIN_PUMP_PRESS[3]; // 0=NORM, 1=LOW;
	int MPS_ENGINE_POWER[2][3]; // 0=ON, 1=OFF;
	int HE_ISOLATION_A[3]; // 0=OPEN, 1=GPC, 2=CLOSE;
	int HE_ISOLATION_B[3]; // 0=OPEN, 1=GPC, 2=CLOSE;
	int CENTERLINE_LATCH; // 0=GND, 1=STOW
	int LEFT_DOOR; //0=OPEN, 1=OFF, 2=CLOSE
	int LEFT_LATCH; //0=RELEASE, 1=OFF, 2=LATCH
	int RIGHT_DOOR; //0=OPEN, 1=OFF, 2=CLOSE
	int RIGHT_LATCH; //0=RELEASE, 1=OFF, 2=LATCH
	
	//int APU_STATE[3]; // APU ON(2)/OFF(0)/START(1);
	int MPS_STATE[3]; // Engines Armed(1)/Safe(0);
	/*bool APU_READY[3];
	double APU_FUEL[3];
	int Hydraulic_Press[3];
	double APU_Speed[3]; //Speed in percent
	int Fuel_Press[3]; //psi*/

	int tkbk_state[8];

	//bool bHydraulicPressure;

	// VC switch animations
	UINT anim_VC_R2[56];

	//ET Umbilical Doors
	AnimState LETUmbDoorStatus;
	AnimState RETUmbDoorStatus;
	AnimState CenterlineLatches; //OPEN(1)=stowed, CLOSED(0)=latched
	AnimState LDoorLatches; //OPEN(1)=unlatched, CLOSED(0)=latched
	AnimState RDoorLatches; //OPEN(1)=unlatched, CLOSED(0)=latched

	//APU Disc Ports
	DiscOutPort APU_Run[3], APU_HydPumpPress[3];
	DiscOutPort APU_CntlrPwr[3], APU_FuelTankValves[3];
	DiscInPort APU_ReadyToStart[3], APU_HydraulicPressure[3];
};

#endif  // !__PANELR2_H
