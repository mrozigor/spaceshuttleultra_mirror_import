#include "../Atlantis.h"
#include "../meshres_ods.h"
#include "ODS.h"

namespace eva_docking {

	ODS::ODS(SubsystemDirector* pdirect, const string& _ident)
		: ExtAirlock(pdirect, _ident),
		bPowerRelay(false),
		bCircuitProtectionOff(false),
		extend_goal(RETRACT_TO_FINAL),
		bLatchesOpen(false),
		bLatchesClosed(true),
		bHooks1Open(true),
		bHooks1Closed(false),
		bHooks2Open(true),
		bHooks2Closed(false),
		bFixersOn(true),
		bTargetInCone(false)
	{
		anim_ring = NULL;
		pRingAnim = NULL;
		RingState.Set(AnimState::STOPPED, 0.0);
		odsAttachVec[0] = ORBITER_DOCKPOS;
		odsAttachVec[1] = _V(0.0, 1.0, 0.0);
		odsAttachVec[2] = _V(0.0, 0.0, 1.0);
		target_pos = _V(0.0, 2000.0, 0.0);
	}

	ODS::~ODS() {
		if(pRingAnim) {
			delete pRingAnim;
			delete pRingAnimV;
			delete pCoilAnim;
			delete pRod1LAnim[0];
			delete pRod1LAnim[1];
			delete pRod1LAnim[2];
			delete pRod1RAnim[0];
			delete pRod1RAnim[1];
			delete pRod1RAnim[2];
			delete pRod2LAnim[0];
			delete pRod2LAnim[1];
			delete pRod2LAnim[2];
			delete pRod2RAnim[0];
			delete pRod2RAnim[1];
			delete pRod2RAnim[2];
			delete pRod3LAnim[0];
			delete pRod3LAnim[1];
			delete pRod3LAnim[2];
			delete pRod3RAnim[0];
			delete pRod3RAnim[1];
			delete pRod3RAnim[2];
		}
	}

	void ODS::AnalyseVessel(OBJHANDLE hVessel) {

		VESSEL* pV = oapiGetVesselInterface(hVessel);
		bool bHasOneAPASPort = false;

		for(unsigned long j = 0; j<pV->AttachmentCount(true); j++) {
			ATTACHMENTHANDLE ahX = 	pV->GetAttachmentHandle(true, j);

			if(!strnicmp(pV->GetAttachmentId(ahX), "APAS", 4)) {
				bHasOneAPASPort = true;
			}
		}

		known_objects.insert(hVessel);
		if(!bHasOneAPASPort) {
			non_apas_objects.insert(hVessel);
		}
	}

	bool ODS::FindClosestDockingRing(OBJHANDLE hVessel) {
		VECTOR3 gpos;
		VECTOR3 lpos, lpos_remote;
		VECTOR3 ring_dir, ring_up;
		VECTOR3 rel_vel;
		

		
		//calculate CG/CG distance
		oapiGetGlobalPos (hVessel, &gpos);
		STS()->Global2Local(gpos, lpos);
		//Check distance and hemisphere
		if(length(lpos) <= 609.6 && lpos.y >= 0) {
			//Gather attachment points of target
			VESSEL* pV = oapiGetVesselInterface(hVessel);

			for(unsigned long j = 0; j<pV->AttachmentCount(true); j++) {
				ATTACHMENTHANDLE ahX = 
					pV->GetAttachmentHandle(true, j);
				
				if(strnicmp(pV->GetAttachmentId(ahX), "APAS", 4))
					//Proceed with next attachment
					continue;

				
				STS()->GetRelativeVel(hVessel, rel_vel);



				pV->GetAttachmentParams(ahX, lpos_remote, 
					ring_dir, ring_up);
				pV->Local2Global(lpos_remote, gpos);
				STS()->Global2Local(gpos, lpos_remote);
				if(length(lpos_remote - odsAttachVec[0]) <= 76.2) {
					bTargetInCone = true;
					ahTarget = ahX;
					ohTarget = hVessel;
					target_pos = lpos_remote - odsAttachVec[0];
					target_vel.x = dotp(rel_vel, eX);
					target_vel.y = dotp(rel_vel, eY);
					target_vel.z = dotp(rel_vel, eZ);
					//bFoundAtLeastOne = true;
					return true;
				}
			}
		}
		return false;
	}

	bool ODS::FindClosestDockingRing() {

		/* VECTOR3 gpos;
		VECTOR3 lpos, lpos_remote;
		VECTOR3 ring_dir, ring_up; */
		//bool bFoundAtLeastOne = false;
		//Scan surrounding vessels, maximum range 2000 ft (609.6 m) CG-CG
		bTargetInCone = false;		//Cheat.

		if(bTargetInCone) {
			//Abort, this function is not for updates, but for initial search
			return false;
		}

		bool bOneValidTarget = false;

		for(DWORD i = 0; i<oapiGetVesselCount(); i++) {

			OBJHANDLE hVessel = oapiGetVesselByIndex(i);
			//If it is not our own handle and not known to be without 
			//APAS
			if(hVessel != STS()->GetHandle()) {
				if(known_objects.find(hVessel) == known_objects.end()) {
					AnalyseVessel(hVessel);
				}

				if(known_objects.find(hVessel) != known_objects.end() &&
					non_apas_objects.find(hVessel) == non_apas_objects.end()) 
				{
					bOneValidTarget |= FindClosestDockingRing(hVessel);
				}
			}
		}
		return bOneValidTarget;
	}

	double ODS::GetSubsystemEmptyMass() const {
		return 0.0;
	}

	void ODS::OnPostStep(double fSimT, double fDeltaT, double fMJD)
	{

	}

	void ODS::OnPreStep(double fSimT, double fDeltaT, double fMJD)
	{
		const double FEET = 1.0/0.3048;
		const double INCH = 1.0/0.0254;
		char pszBuffer[256];

		STS()->GlobalRot(_V(1,0,0),eX);
		STS()->GlobalRot(_V(0,1,0),eY);
		STS()->GlobalRot(_V(0,0,1),eZ);

		//If no target captured
		//Locate and update objects in 2m cylinder, closest wins
		if(FindClosestDockingRing())
		{
			VESSEL* pV;
			oapiGetObjectName(ohTarget, pszBuffer, 255);
			int iD = 0;

			pV = oapiGetVesselInterface(ohTarget);
			iD = pV->GetAttachmentIndex(ahTarget);

/*			sprintf_s(oapiDebugString(), 255, 
				"ODS: LOCK %s:%d | %5.2f\" %5.2f' %5.2f\" | %5.2f %5.2f %5.2f", 
				pszBuffer, iD, target_pos.x * INCH, 
				target_pos.y * FEET, target_pos.z * INCH,
				target_vel.x * FEET, target_vel.y * FEET, 
				target_vel.z * FEET); */
		} else {
			/*
			sprintf_s(oapiDebugString(), 255, "ODS: NUM KNOWN %d (%d W/O APAS)",
				known_objects.size(), non_apas_objects.size()); */
		}

		//If target in range:
		//Calculate contacts and trigger initial contact signal.
		//If all latches matching and overcoming resistence, capture.

		//if captured
		//simulate oscillations of structure

		if(dscu_PowerOn.IsSet()) {
			//sprintf_s(oapiDebugString(), 255, "POWER ON");
			bPowerRelay = true;
			bCircuitProtectionOff = false;
		} 

		if(dscu_PowerOff.IsSet()) {
			//sprintf_s(oapiDebugString(), 255, "POWER OFF");
			bPowerRelay = false;
		}

		

		if(HasDSCUPower()) {

			dscu_PowerOnLight.SetLine();
			dscu_RingAlignedLight.SetLine();

			if(dscu_CircProtectionOff.IsSet()) {
				bCircuitProtectionOff = true;
			}

			if(dscu_FixerOff) {
				bFixersOn = false;
			}

			if(bCircuitProtectionOff) {
				dscu_CircProtectLight.SetLine();
			} else {
				dscu_CircProtectLight.ResetLine();
			}

			if(!bFixersOn) {
				dscu_FixersOffLight.SetLine();
			} else {
				dscu_FixersOffLight.ResetLine();
			}

			if(dscu_RingOut.IsSet() && bCircuitProtectionOff) {
				RingState.action = AnimState::OPENING;
				extend_goal = EXTEND_TO_INITIAL;
				if(RingState.pos >= 0.7229167) {
					extend_goal = EXTEND_TO_FINAL;
				}
				bFixersOn = true;
			}


			if(dscu_RingIn.IsSet() && bCircuitProtectionOff) {
				RingState.action = AnimState::CLOSING;
				extend_goal = RETRACT_TO_FINAL;
				bFixersOn = true;
			}


			if(RingState.Moving() && bPowerRelay) {
				RingState.Move(0.0039815 * fDeltaT);

				if(RingState.pos >= 0.7361111 && 
					extend_goal == EXTEND_TO_INITIAL) {
					RingState.action = AnimState::STOPPED;
				}

				if(RingState.pos >= 1.0 && 
					extend_goal == EXTEND_TO_FINAL) {
					RingState.action = AnimState::STOPPED;
				}

				STS()->SetAnimation(anim_ring, RingState.pos);
				STS()->UpdateODSAttachment(odsAttachVec[0], 
					odsAttachVec[1]-odsAttachVec[0], 
					odsAttachVec[2]-odsAttachVec[0]);
			}

			if(RingState.pos < 0.0631944) {
				dscu_RingFinalLight.SetLine();
			} else {
				dscu_RingFinalLight.ResetLine();
			}

			if((RingState.pos >= 0.7229167&& RingState.pos < 0.7493056)) {
				dscu_RingInitialLight.SetLine();
			} else {
				dscu_RingInitialLight.ResetLine();
			}

			if(RingState.pos >= 0.9868056 ) {
				dscu_RingForwardLight.SetLine();
			} else {
				dscu_RingForwardLight.ResetLine();
			}

			if(bLatchesClosed) {
				dscu_LatchesClosedLight.SetLine();
			} else {
				dscu_LatchesClosedLight.ResetLine();
			}

			if(bLatchesOpen) {
				dscu_LatchesOpenLight.SetLine();
			} else {
				dscu_LatchesOpenLight.ResetLine();
			}

			if(bHooks1Open) {
				dscu_Hooks1OpenLight.SetLine();
			} else {
				dscu_Hooks1OpenLight.ResetLine();
			}

			if(bHooks2Open) {
				dscu_Hooks2OpenLight.SetLine();
			} else {
				dscu_Hooks2OpenLight.ResetLine();
			}

			if(bHooks1Closed) {
				dscu_Hooks1ClosedLight.SetLine();
			} else {
				dscu_Hooks1ClosedLight.ResetLine();
			}

			if(bHooks2Closed) {
				dscu_Hooks2ClosedLight.SetLine();
			} else {
				dscu_Hooks2ClosedLight.ResetLine();
			}

		} else {
			dscu_PowerOnLight.ResetLine();
			dscu_CircProtectLight.ResetLine();
			dscu_RingAlignedLight.ResetLine();
			dscu_FixersOffLight.ResetLine();
			dscu_RingInitialLight.ResetLine();
			dscu_Hooks1OpenLight.ResetLine();
			dscu_Hooks2OpenLight.ResetLine();
			dscu_LatchesClosedLight.ResetLine();
			dscu_UndockCompleteLight.ResetLine();

			dscu_InitialContactLight.ResetLine();
			dscu_CaptureLight.ResetLine();
			dscu_RingForwardLight.ResetLine();
			dscu_ReadyToHookLight.ResetLine();
			dscu_InterfSealedLight.ResetLine();
			dscu_Hooks1ClosedLight.ResetLine();
			dscu_Hooks2ClosedLight.ResetLine();
			dscu_LatchesOpenLight.ResetLine();
			dscu_RingFinalLight.ResetLine();
		}
	}

	void ODS::OnPropagate(double fSimT, double fDeltaT, double fMJD)
	{
	}

	void ODS::Realize() {
		oapiWriteLog("(ssu)Realize ODS...");
		STS()->SetExternalAirlockVisual(true, true);

		DiscreteBundle* pBundle = 
			STS()->BundleManager()->CreateBundle("PANELA8A3_TO_DSCU_A", 16);
		
		dscu_PowerOn.Connect(pBundle, 0);
		dscu_PowerOff.Connect(pBundle, 1);
		dscu_RingOut.Connect(pBundle, 2);
		dscu_RingIn.Connect(pBundle, 3);
		dscu_CircProtectionOff.Connect(pBundle, 4);
		dscu_CloseHooks.Connect(pBundle, 5);
		dscu_CloseLatches.Connect(pBundle, 6);
		dscu_FixerOff.Connect(pBundle, 7);

		DiscreteBundle* pBundleA = 
			STS()->BundleManager()->CreateBundle("DSCU_TO_PANELA8A3_A", 16);

		dscu_PowerOnLight.Connect(pBundleA, 0);
		dscu_CircProtectLight.Connect(pBundleA, 1);
		dscu_RingAlignedLight.Connect(pBundleA, 2);
		dscu_RingInitialLight.Connect(pBundleA, 3);
		dscu_FixersOffLight.Connect(pBundleA, 4);
		dscu_Hooks1OpenLight.Connect(pBundleA, 5);
		dscu_Hooks2OpenLight.Connect(pBundleA, 6);
		dscu_LatchesClosedLight.Connect(pBundleA, 7);

		DiscreteBundle* pBundleB = 
			STS()->BundleManager()->CreateBundle("DSCU_TO_PANELA8A3_B", 16);

		dscu_RingForwardLight.Connect(pBundleB, 2);
		dscu_Hooks1ClosedLight.Connect(pBundleB, 5);
		dscu_Hooks2ClosedLight.Connect(pBundleB, 6);
		dscu_LatchesOpenLight.Connect(pBundleB, 7);
		dscu_RingFinalLight.Connect(pBundleB, 8);
	}

	void ODS::DefineAirlockAnimations(UINT midx_extal, 
		UINT midx_ods, const VECTOR3& ofs) {

			static UINT grps_ring[1] = {GRP_docking_ring_ODS};
			static UINT grps_coil[3] = {GRP_3spiral_1_ODS, 
				GRP_3spiral_2_ODS, GRP_3spiral_3_ODS};
			static UINT grps_rod1l0[1] = {GRP_1L_DR_rod_ODS};
			static UINT grps_rod1l1[1] = {GRP_1L_D_ring_extendbase_ODS};
			static UINT grps_rod1r0[1] = {GRP_1R_DR_rod_ODS};
			static UINT grps_rod1r1[1] = {GRP_1R_D_ring_extendbase_ODS};

			static UINT grps_rod2l0[1] = {GRP_2L_DR_rod_ODS};
			static UINT grps_rod2l1[1] = {GRP_2L_D_ring_extendbase_ODS};
			static UINT grps_rod2r0[1] = {GRP_2R_DR_rod_ODS};
			static UINT grps_rod2r1[1] = {GRP_2R_D_ring_extendbase_ODS};

			static UINT grps_rod3l0[1] = {GRP_3L_DR_rod_ODS};
			static UINT grps_rod3l1[1] = {GRP_3L_D_ring_extendbase_ODS};
			static UINT grps_rod3r0[1] = {GRP_3R_DR_rod_ODS};
			static UINT grps_rod3r1[1] = {GRP_3R_D_ring_extendbase_ODS};
			

		ExtAirlock::DefineAirlockAnimations(midx_extal, 
			midx_ods, ofs);

		if(!pRingAnim) {

			odsAttachVec[0] = ORBITER_DOCKPOS + ofs;
			odsAttachVec[1] = odsAttachVec[0] + _V(0.0, 1.0, 0.0);
			odsAttachVec[2] = odsAttachVec[0] + _V(0.0, 0.0, 1.0);
			
			pRingAnim = new MGROUP_TRANSLATE(midx_ods, grps_ring, 1, 
				_V(0.0, 0.45, 0.0));

			pRingAnimV = new MGROUP_TRANSLATE(LOCALVERTEXLIST, MAKEGROUPARRAY(odsAttachVec), 3, 
				_V(0.0, 0.45, 0.0));

			pCoilAnim = new MGROUP_SCALE(midx_ods, grps_coil, 3, 
				_V(0,1.00,0) + ofs, _V(1,1.9,1));

			pRod1LAnim[0] = new MGROUP_ROTATE(midx_ods, grps_rod1l0, 1, 
				_V(-0.569,1.662,0.197) + ofs, _V(-0.866,0,0.5), (float)(22.0f * RAD));

			pRod1LAnim[1] = new MGROUP_ROTATE(midx_ods, grps_rod1l1, 1, 
				_V(-0.668,1.372,0.02) + ofs, _V(-0.866,0,0.5), (float)(13.0f * RAD));

			pRod1LAnim[2] = new MGROUP_ROTATE(midx_ods, grps_rod1l1, 1, 
				_V(-0.668,1.372,0.02) + ofs, _V(-0.866,0,0.5), (float)(3.0f * RAD));

			pRod1RAnim[0] = new MGROUP_ROTATE(midx_ods, grps_rod1r0, 1, 
				_V(-0.452,1.662,0.396) + ofs, _V(0.866,0,-0.5), (float)(22.0f * RAD));

			pRod1RAnim[1] = new MGROUP_ROTATE(midx_ods, grps_rod1r1, 1, 
				_V(-0.353,1.372,0.568) + ofs, _V(0.866,0,-0.5), (float)(13.0f * RAD));

			pRod1RAnim[2] = new MGROUP_ROTATE(midx_ods, grps_rod1r1, 1, 
				_V(-0.353,1.372,0.568) + ofs, _V(0.866,0,-0.5), (float)(3.0f * RAD));

			pRod2LAnim[0] = new MGROUP_ROTATE(midx_ods, grps_rod2l0, 1, 
				_V(0.452,1.662,0.396) + ofs, _V(0.866,0,0.5), (float)(22.0f * RAD));

			pRod2LAnim[1] = new MGROUP_ROTATE(midx_ods, grps_rod2l1, 1, 
				_V(0.353,1.372,0.568) + ofs, _V(0.866,0,0.5), (float)(13.0f * RAD));

			pRod2LAnim[2] = new MGROUP_ROTATE(midx_ods, grps_rod2l1, 1, 
				_V(0.353,1.372,0.568) + ofs, _V(0.866,0,0.5), (float)(3.0f * RAD));

			pRod2RAnim[0] = new MGROUP_ROTATE(midx_ods, grps_rod2r0, 1, 
				_V(0.569,1.662,0.197) + ofs, _V(-0.866,0,-0.5), (float)(22.0f * RAD));

			pRod2RAnim[1] = new MGROUP_ROTATE(midx_ods, grps_rod2r1, 1, 
				_V(0.668,1.372,0.02) + ofs, _V(-0.866,0,-0.5), (float)(13.0f * RAD));

			pRod2RAnim[2] = new MGROUP_ROTATE(midx_ods, grps_rod2r1, 1, 
				_V(0.668,1.372,0.02) + ofs, _V(-0.866,0,-0.5), (float)(3.0f * RAD));

			
			pRod3LAnim[0] = new MGROUP_ROTATE(midx_ods, grps_rod3l0, 1, 
				_V(0.117,1.662,0) + ofs, _V(0,0,-1), (float)(22.0f * RAD));

			pRod3LAnim[1] = new MGROUP_ROTATE(midx_ods, grps_rod3l1, 1, 
				_V(0.315,1.372,0) + ofs, _V(0,0,-1), (float)(13.0f * RAD));

			pRod3LAnim[2] = new MGROUP_ROTATE(midx_ods, grps_rod3l1, 1, 
				_V(0.315,1.372,0) + ofs, _V(0,0,-1), (float)(3.0f * RAD));

			pRod3RAnim[0] = new MGROUP_ROTATE(midx_ods, grps_rod3r0, 1, 
				_V(-0.117,1.662,0) + ofs, _V(0,0,1), (float)(22.0f * RAD));

			pRod3RAnim[1] = new MGROUP_ROTATE(midx_ods, grps_rod3r1, 1, 
				_V(-0.315,1.372,0) + ofs, _V(0,0,1), (float)(13.0f * RAD));

			pRod3RAnim[2] = new MGROUP_ROTATE(midx_ods, grps_rod3r1, 1, 
				_V(-0.315,1.372,0) + ofs, _V(0,0,1), (float)(3.0f * RAD));

			anim_ring = STS()->CreateAnimation(0.0);
			ANIMATIONCOMPONENT_HANDLE parent = 
				STS()->AddAnimationComponent(anim_ring, 
				0.05, 1.0, pRingAnim);

			STS()->AddAnimationComponent(anim_ring, 
				0.05, 1.0, pRingAnimV);

			STS()->AddAnimationComponent(anim_ring, 0.05, 1.0, 
				pCoilAnim);

			STS()->AddAnimationComponent(anim_ring, 0.00, 0.95, 
				pRod1LAnim[0], parent);
			STS()->AddAnimationComponent(anim_ring, 0.0, 0.5, 
				pRod1LAnim[1]);
			STS()->AddAnimationComponent(anim_ring, 0.5, 1.0, 
				pRod1LAnim[2]);

			STS()->AddAnimationComponent(anim_ring, 0.00, 0.95, 
				pRod1RAnim[0], parent);
			STS()->AddAnimationComponent(anim_ring, 0.0, 0.5, 
				pRod1RAnim[1]);
			STS()->AddAnimationComponent(anim_ring, 0.5, 1.0, 
				pRod1RAnim[2]);

			STS()->AddAnimationComponent(anim_ring, 0.00, 0.95, 
				pRod2LAnim[0], parent);
			STS()->AddAnimationComponent(anim_ring, 0.0, 0.5, 
				pRod2LAnim[1]);
			STS()->AddAnimationComponent(anim_ring, 0.5, 1.0, 
				pRod2LAnim[2]);

			STS()->AddAnimationComponent(anim_ring, 0.00, 0.95, 
				pRod2RAnim[0], parent);
			STS()->AddAnimationComponent(anim_ring, 0.0, 0.5, 
				pRod2RAnim[1]);
			STS()->AddAnimationComponent(anim_ring, 0.5, 1.0, 
				pRod2RAnim[2]);

			STS()->AddAnimationComponent(anim_ring, 0.00, 0.95, 
				pRod3LAnim[0], parent);
			STS()->AddAnimationComponent(anim_ring, 0.0, 0.5, 
				pRod3LAnim[1]);
			STS()->AddAnimationComponent(anim_ring, 0.5, 1.0, 
				pRod3LAnim[2]);

			STS()->AddAnimationComponent(anim_ring, 0.00, 0.95, 
				pRod3RAnim[0], parent);
			STS()->AddAnimationComponent(anim_ring, 0.0, 0.5, 
				pRod3RAnim[1]);
			STS()->AddAnimationComponent(anim_ring, 0.5, 1.0, 
				pRod3RAnim[2]);

			
			
		}


	}

	bool ODS::HasDSCUPower() const {
		return bPowerRelay;
	}

};

