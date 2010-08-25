/****************************************************************************
  This file is part of Space Shuttle Ultra

  Orbiter Docking System definition



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
#pragma once
#include "ExtAirlock.h"
#include "DiscOutPort.h"
#include "DiscInPort.h"
#include <set>

const static char* ODS_MESHNAME = "ODS";

/**
 * EVA and Docking systems
 */
namespace eva_docking {

	using class discsignals::DiscInPort;
	using class discsignals::DiscOutPort;

	using namespace std;

	class ODS: public ExtAirlock {
	protected:
		//UINT midxODS;
		double fRingState;
		double fHookState;
		double fLatchState;
		//double fVestPressure[2];
		double fInterfAtmP[2];
		double fInterfAtmT[2];
		double fInterfAtmMass[2];

		bool bDSCUPower;

		AnimState RingState;

		//Target data
		bool bTargetInCone;
		bool bTargetCaptured;
		OBJHANDLE ohTarget;
		ATTACHMENTHANDLE ahTarget;
		VECTOR3 target_pos;
		VECTOR3 target_vel;	//Use also for relative speed in captured case.
		VECTOR3 target_dir;
		VECTOR3 target_up;
		VECTOR3 target_avel;
		VECTOR3 eX, eY, eZ;

		set<OBJHANDLE> known_objects;			//list of known vessels
		set<OBJHANDLE> non_apas_objects;		//Black list

		typedef enum ___extend_goal {
			EXTEND_TO_INITIAL,
			EXTEND_TO_FINAL,
			RETRACT_TO_FINAL
		} EXTEND_GOAL;

		EXTEND_GOAL extend_goal;

		UINT anim_ring;
		MGROUP_TRANSLATE*	pRingAnim;
		MGROUP_TRANSLATE*	pRingAnimV;
		MGROUP_SCALE*		pCoilAnim;
		MGROUP_ROTATE*		pRod1LAnim[3];
		MGROUP_ROTATE*		pRod2LAnim[3];
		MGROUP_ROTATE*		pRod3LAnim[3];
		MGROUP_ROTATE*		pRod1RAnim[3];
		MGROUP_ROTATE*		pRod2RAnim[3];
		MGROUP_ROTATE*		pRod3RAnim[3];

		bool bPowerRelay;
		bool bCircuitProtectionOff;
		bool bFixersOn;
		bool bLatchesOpen;
		bool bLatchesClosed;
		bool bHooks1Open;
		bool bHooks1Closed;
		bool bHooks2Open;
		bool bHooks2Closed;

		VECTOR3 odsAttachVec[3];

		bool HasDSCUPower() const;
		bool FindClosestDockingRing();
		bool FindClosestDockingRing(OBJHANDLE hVessel);
		void AnalyseVessel(OBJHANDLE hVessel);
		


	public:
		ODS(AtlantisSubsystemDirector* pdirect, const string& _ident);
		virtual ~ODS();
		virtual void Realize();
		//virtual void AddMeshes(const VECTOR3& ofs);
		virtual double GetSubsystemEmptyMass() const;
		virtual void OnSaveState(FILEHANDLE scn) const;
		virtual void OnPreStep(double fSimT, double fDeltaT, double fMJD);
		virtual void OnPropagate(double fSimT, double fDeltaT, double fMJD);
		virtual void OnPostStep(double fSimT, double fDeltaT, double fMJD);
		virtual void DefineAirlockAnimations(UINT midx_extal, UINT midx_ods, const VECTOR3& ofs);
		virtual bool OnParseLine(const char* keyword, const char* line);

		DiscInPort dscu_PowerOn;
		DiscInPort dscu_PowerOff;
		DiscInPort dscu_RingOut;
		DiscInPort dscu_RingIn;
		DiscInPort dscu_CircProtectionOff;
		DiscInPort dscu_CloseHooks;
		DiscInPort dscu_CloseLatches;
		DiscInPort dscu_FixerOff;

		DiscOutPort dscu_PowerA;
		DiscOutPort dscu_PowerB;
		DiscOutPort dscu_PowerC;
		
		DiscOutPort dscu_PowerOnLight;
		DiscOutPort dscu_CircProtectLight;
		DiscOutPort dscu_RingAlignedLight;
		DiscOutPort dscu_RingInitialLight;
		DiscOutPort dscu_FixersOffLight;
		DiscOutPort dscu_Hooks1OpenLight;
		DiscOutPort dscu_Hooks2OpenLight;
		DiscOutPort dscu_LatchesClosedLight;
		DiscOutPort dscu_UndockCompleteLight;
		DiscOutPort dscu_InitialContactLight;
		DiscOutPort dscu_CaptureLight;
		DiscOutPort dscu_RingForwardLight;
		DiscOutPort dscu_ReadyToHookLight;
		DiscOutPort dscu_InterfSealedLight;
		DiscOutPort dscu_Hooks1ClosedLight;
		DiscOutPort dscu_Hooks2ClosedLight;
		DiscOutPort dscu_LatchesOpenLight;
		DiscOutPort dscu_RingFinalLight;
	};

};

