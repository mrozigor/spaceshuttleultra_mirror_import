#include "../Atlantis.h"
#include "../Atlantis_defs.h"
#include "../meshres_ods.h"
#include "ODS.h"
#include "VesselAPI.h"
#include "..\CommonDefs.h"


namespace eva_docking {

	const VECTOR3 ODS_RING_TRANSLATION = _V(0.0, 0.45, 0.0);

	//const double ODS_ROD_REF_Y = 1.662;
	const double ODS_ROD_REF_Y = 2.20551;
	//const double ODS_ROD_ACT_REF_Y = 1.372;
	const double ODS_ROD_ACT_REF_Y = 1.94684;
	

	const VECTOR3 ODS_ROD1L_REF = _V(-0.440812, ODS_ROD_REF_Y, -0.15653);
	const VECTOR3 ODS_ROD1L_DIR = _V(-0.866, 0, 0.5);
	const VECTOR3 ODS_ROD1L_ROTAXIS = _V(-0.287224, -0.824477, -0.487587);
	const VECTOR3 ODS_ROD1L_ACT_REF = _V(-0.518709, 1.94684, -0.313008);
	const VECTOR3 ODS_ROD1L_ACT_DIR = _V(-0.866, 0, 0.5);

	const VECTOR3 ODS_ROD1R_REF = _V(-0.357432, ODS_ROD_REF_Y, -0.0121291);
	const VECTOR3 ODS_ROD1R_DIR = _V(0.866, 0, -0.5);
	const VECTOR3 ODS_ROD1R_ROTAXIS = _V(0.278716, -0.824529, 0.492411);
	const VECTOR3 ODS_ROD1R_ACT_REF = _V(-0.260867, 1.94684, 0.133575);
	const VECTOR3 ODS_ROD1R_ACT_DIR = _V(0.866, 0, -0.5);

	const VECTOR3 ODS_ROD2L_REF = _V(0.379246, ODS_ROD_REF_Y, -0.0112028);
	const VECTOR3 ODS_ROD2L_DIR = _V(-0.866, 0, -0.5);
	const VECTOR3 ODS_ROD2L_ROTAXIS = _V(-0.278716, -0.824529, 0.492411);
	const VECTOR3 ODS_ROD2L_ACT_REF = _V(0.282677, 1.94923, 0.134417);
	const VECTOR3 ODS_ROD2L_ACT_DIR = _V(0.866, 0, 0.5);

	const VECTOR3 ODS_ROD2R_REF = _V(0.462613, ODS_ROD_REF_Y, -0.155687);
	const VECTOR3 ODS_ROD2R_DIR = _V(0.866, 0, 0.5);
	const VECTOR3 ODS_ROD2R_ROTAXIS = _V(0.287224, -0.824477, -0.487587);
	const VECTOR3 ODS_ROD2R_ACT_REF = _V(0.540518, 1.94923, -0.312122);
	const VECTOR3 ODS_ROD2R_ACT_DIR = _V(-0.866, 0, -0.5);

	const VECTOR3 ODS_ROD3L_REF = _V(0.0868684, ODS_ROD_REF_Y, -0.806937);
	const VECTOR3 ODS_ROD3L_DIR = _V(0, 0, -1);
	const VECTOR3 ODS_ROD3L_ROTAXIS = _V(0.565876, -0.824436, 0.00943811);
	const VECTOR3 ODS_ROD3L_ACT_REF = _V(0.261339, 1.94801, -0.791595);
	const VECTOR3 ODS_ROD3L_ACT_DIR = _V(0, 0, 1);

	const VECTOR3 ODS_ROD3R_REF = _V(-0.079876, ODS_ROD_REF_Y, -0.806962);
	const VECTOR3 ODS_ROD3R_DIR = _V(0, 0, 1);
	const VECTOR3 ODS_ROD3R_ROTAXIS = _V(-0.565876, -0.824436, 0.00943811);
	const VECTOR3 ODS_ROD3R_ACT_REF = _V(-0.254345, 1.94801, -0.791629);
	const VECTOR3 ODS_ROD3R_ACT_DIR = _V(0, 0, -1);

	const VECTOR3 ODS_ROD_DISPLACEMENT = ODS_ROD3L_REF - ODS_ROD3L_ACT_REF;
	const double ODS_ROD_DISPLACEMENT_LEN = length(ODS_ROD_DISPLACEMENT);

	const double ODS_ROD_NULLANGLE = atan2(ODS_ROD_DISPLACEMENT.y, fabs(ODS_ROD_DISPLACEMENT.x));
	const float ODS_ROD_ROTATION = static_cast<float>(30.0f * RAD);
	

	//20 rotations per meter
	const float ODS_RODDRIVE_ROTATION = static_cast<float>(400.0 * PI);

	ODS::ODS(AtlantisSubsystemDirector* pdirect, const string& _ident)
		: ExtAirlock(pdirect, _ident, true),
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
		odsAttachVec[0] = ODS_DOCKPOS_OFFSET;
		odsAttachVec[1] = odsAttachVec[0] + _V(0.0, 1.0, 0.0);
		odsAttachVec[2] = odsAttachVec[0] + _V(0.0, 0.0, 1.0);
		target_pos = _V(0.0, 2000.0, 0.0);

		mesh_ods = MESH_UNDEFINED;
		hODSMesh = oapiLoadMeshGlobal( DEFAULT_MESHNAME_ODS );
		oapiWriteLog( "ODS mesh loaded" );

		ahDockAux = NULL;
	}

	ODS::~ODS() {
		if(pRingAnim) {
			delete pRingAnim;
			delete pRingAnimV;
			delete pCoilAnim;

			for (int i = 0; i < 2; ++i) 
			{
				delete pRod1LAnim[i];
				delete pRod1RAnim[i];

				delete pRod2LAnim[i];
				delete pRod2RAnim[i];

				delete pRod3LAnim[i];
				delete pRod3RAnim[i];
			}
			
		}
	}

	void ODS::AnalyseVessel(OBJHANDLE hVessel) {

		VESSEL* pV = oapiGetVesselInterface(hVessel);
		bool bHasOneAPASPort = false;

		//for(unsigned long j = 0; j<pV->AttachmentCount(true); j++) {
		for(unsigned long j = pV->AttachmentCount(true) - 1; j>0; j--) {
			ATTACHMENTHANDLE ahX = 	pV->GetAttachmentHandle(true, j);

			if(!_strnicmp(pV->GetAttachmentId(ahX), "APAS", 4)) {
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
				
				if(_strnicmp(pV->GetAttachmentId(ahX), "APAS", 4))
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
		return 419.025; // According to a SSP PRCB presentation on the STS-125 pros/cons of Single Pad vs Dual Pad ops for LON, ODS mass is 925 lbs, this is for the ODS alone, not including the airlock hardware
	}

	void ODS::CalculateRodAnimation() 
	{
		double ringPos = RingState.pos * ODS_RING_TRANSLATION.y;
		
		double angle = atan2(ringPos + ODS_ROD_DISPLACEMENT.y, fabs(ODS_ROD_DISPLACEMENT.x));

		double pos = min(max((angle - ODS_ROD_NULLANGLE) / ODS_ROD_ROTATION, 0.0), 1.0);
		
		/*sprintf_s(oapiDebugString(), 255, "ODS ROD ANIMATION: %.1f cm ==> %5.2f°/%5.2f° ==> %5.3f pos",
			ringPos * 100.0, angle * DEG, ODS_ROD_NULLANGLE * DEG, pos);*/

		STS()->SetAnimation(anim_rods, pos);
		
	}

	void ODS::OnPostStep(double fSimT, double fDeltaT, double fMJD)
	{

	}

	void ODS::OnPreStep(double fSimT, double fDeltaT, double fMJD)
	{
		//char pszBuffer[256];

		STS()->GlobalRot(_V(1,0,0),eX);
		STS()->GlobalRot(_V(0,1,0),eY);
		STS()->GlobalRot(_V(0,0,1),eZ);

		//If no target captured
		//Locate and update objects in 2m cylinder, closest wins
		/*if(FindClosestDockingRing())
		{
			VESSEL* pV;
			oapiGetObjectName(ohTarget, pszBuffer, 255);
			int iD = 0;

			pV = oapiGetVesselInterface(ohTarget);
			iD = pV->GetAttachmentIndex(ahTarget);

			sprintf_s(oapiDebugString(), 255, 
				"ODS: LOCK %s:%d | %5.4f\" %5.4f' %5.4f\" | %5.4f %5.4f %5.4f", 
				pszBuffer, iD, target_pos.x / INCH, target_pos.y / MPS2FPS, target_pos.z / INCH,
				target_vel.x / MPS2FPS, target_vel.y / MPS2FPS, target_vel.z / MPS2FPS);
		} else {
			sprintf_s(oapiDebugString(), 255, "ODS: NUM KNOWN %d (%d W/O APAS)",
				known_objects.size(), non_apas_objects.size());
		}*/

		//If target in range:
		//Calculate contacts and trigger initial contact signal.
		//If all latches matching and overcoming resistence, capture.

		//if captured
		//simulate oscillations of structure

		if(dscu_PowerOn.IsSet()) {
			bPowerRelay = true;
			bCircuitProtectionOff = false;
		} 

		if(dscu_PowerOff.IsSet()) {
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
				//UpdateODSAttachment(STS()->GetOrbiterCoGOffset());

				CalculateRodAnimation();
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

		STS()->SetAnimation(anim_ring, RingState.pos);

		CalculateRodAnimation();
		
	}

	void ODS::OnSaveState(FILEHANDLE scn) const
	{
		WriteScenario_state(scn, "RING_STATE", RingState);
		return ExtAirlock::OnSaveState(scn);
	}

	void ODS::DefineAnimations(const VECTOR3& ofs) {

			static UINT grps_ring[2] = {GRP_DOCKING_RING_ODS, GRP_DOCKING_SIGHT_ODS};
			static UINT grps_coil[3] = {GRP_3SPIRAL_1_ODS, 
				GRP_3SPIRAL_2_ODS, GRP_3SPIRAL_3_ODS};
			static UINT grps_rod1l0[1] = {GRP_1L_DR_ROD_ODS};
			static UINT grps_rod1l1[1] = {GRP_1L_D_RING_EXTENDBASE_ODS};
			static UINT grps_rod1r0[1] = {GRP_1R_DR_ROD_ODS};
			static UINT grps_rod1r1[1] = {GRP_1R_D_RING_EXTENDBASE_ODS};

			static UINT grps_rod2l0[1] = {GRP_2L_DR_ROD_ODS};
			static UINT grps_rod2l1[1] = {GRP_2L_D_RING_EXTENDBASE_ODS};
			static UINT grps_rod2r0[1] = {GRP_2R_DR_ROD_ODS};
			static UINT grps_rod2r1[1] = {GRP_2R_D_RING_EXTENDBASE_ODS};

			static UINT grps_rod3l0[1] = {GRP_3L_DR_ROD_ODS};
			static UINT grps_rod3l1[1] = {GRP_3L_D_RING_EXTENDBASE_ODS};
			static UINT grps_rod3r0[1] = {GRP_3R_DR_ROD_ODS};
			static UINT grps_rod3r1[1] = {GRP_3R_D_RING_EXTENDBASE_ODS};
			

		ExtAirlock::DefineAnimations( ofs );

		if(!pRingAnim) {

			odsAttachVec[0] = ODS_DOCKPOS_OFFSET + ofs;
			odsAttachVec[1] = odsAttachVec[0] + _V(0.0, 1.0, 0.0);
			odsAttachVec[2] = odsAttachVec[0] + _V(0.0, 0.0, 1.0);
			
			pRingAnim = new MGROUP_TRANSLATE(mesh_ods, grps_ring, 2, 
				ODS_RING_TRANSLATION);

			pRingAnimV = new MGROUP_TRANSLATE(LOCALVERTEXLIST, MAKEGROUPARRAY(odsAttachVec), 3, 
				_V(0.0, 0.45, 0.0));

			pCoilAnim = new MGROUP_SCALE(mesh_ods, grps_coil, 3, 
				_V(0,1.00,0), _V(1,1.4,1));

			pRod1LAnim[0] = new MGROUP_ROTATE(mesh_ods, grps_rod1l0, 1, 
				ODS_ROD1L_REF, ODS_ROD1L_DIR, ODS_ROD_ROTATION);

			pRod1LAnim[1] = new MGROUP_ROTATE(mesh_ods, grps_rod1l1, 1, 
				ODS_ROD1L_ACT_REF, ODS_ROD1L_ACT_DIR, ODS_ROD_ROTATION);

					
			pRod1RAnim[0] = new MGROUP_ROTATE(mesh_ods, grps_rod1r0, 1, 
				ODS_ROD1R_REF, ODS_ROD1R_DIR, ODS_ROD_ROTATION);

			pRod1RAnim[1] = new MGROUP_ROTATE(mesh_ods, grps_rod1r1, 1, 
				ODS_ROD1R_ACT_REF, ODS_ROD1R_ACT_DIR, ODS_ROD_ROTATION);

			

			
			pRod2LAnim[0] = new MGROUP_ROTATE(mesh_ods, grps_rod2l0, 1, 
				ODS_ROD2L_REF, ODS_ROD2L_DIR, -ODS_ROD_ROTATION);

			pRod2LAnim[1] = new MGROUP_ROTATE(mesh_ods, grps_rod2l1, 1, 
				ODS_ROD2L_ACT_REF, ODS_ROD2L_ACT_DIR, ODS_ROD_ROTATION);

			

			pRod2RAnim[0] = new MGROUP_ROTATE(mesh_ods, grps_rod2r0, 1, 
				ODS_ROD2R_REF, ODS_ROD2R_DIR, -ODS_ROD_ROTATION);

			pRod2RAnim[1] = new MGROUP_ROTATE(mesh_ods, grps_rod2r1, 1, 
				ODS_ROD2R_ACT_REF, ODS_ROD2R_ACT_DIR, ODS_ROD_ROTATION);

					
			
			pRod3LAnim[0] = new MGROUP_ROTATE(mesh_ods, grps_rod3l0, 1, 
				ODS_ROD3L_REF, ODS_ROD3L_DIR, ODS_ROD_ROTATION);

			pRod3LAnim[1] = new MGROUP_ROTATE(mesh_ods, grps_rod3l1, 1, 
				ODS_ROD3L_ACT_REF, ODS_ROD3L_ACT_DIR, -ODS_ROD_ROTATION);

			
			
			pRod3RAnim[0] = new MGROUP_ROTATE(mesh_ods, grps_rod3r0, 1, 
				ODS_ROD3R_REF, ODS_ROD3R_DIR, ODS_ROD_ROTATION);

			pRod3RAnim[1] = new MGROUP_ROTATE(mesh_ods, grps_rod3r1, 1, 
				ODS_ROD3R_ACT_REF, ODS_ROD3R_ACT_DIR, -ODS_ROD_ROTATION);

			
		
			anim_ring = STS()->CreateAnimation(0.0);
			anim_rods = STS()->CreateAnimation(0.0);
		
			ANIMATIONCOMPONENT_HANDLE parent = 
				STS()->AddAnimationComponent(anim_ring, 
				0.00, 1.0, pRingAnim);

			STS()->AddAnimationComponent(anim_ring, 
				0.00, 1.0, pRingAnimV);

			STS()->AddAnimationComponent(anim_ring, 0.00, 1.0, 
				pCoilAnim);


			//Counterclockwise actuator of pair 1
			STS()->AddAnimationComponent(anim_rods, 0.0, 1.0, 
				pRod1LAnim[0], parent);
			STS()->AddAnimationComponent(anim_rods, 0.0, 1.0, 
				pRod1LAnim[1]);
			
		
			//Clockwise actuator of pair 1
			STS()->AddAnimationComponent(anim_rods, 0.0, 1.0,
				pRod1RAnim[0], parent);
			STS()->AddAnimationComponent(anim_rods, 0.0, 1.0, 
				pRod1RAnim[1]);
		
		
			//Counterclockwise actuator of pair 2
			STS()->AddAnimationComponent(anim_rods, 0.0, 1.0, 
				pRod2LAnim[0], parent);
			STS()->AddAnimationComponent(anim_rods, 0.0, 1.0, 
				pRod2LAnim[1]);
		
		
			//Clockwise actuator of pair 2
			STS()->AddAnimationComponent(anim_rods, 0.0, 1.0, 
				pRod2RAnim[0], parent);
			STS()->AddAnimationComponent(anim_rods, 0.0, 1.0, 
				pRod2RAnim[1]);
		
		
			//Counterclockwise actuator of pair 3
			STS()->AddAnimationComponent(anim_rods, 0.0, 1.0, 
				pRod3LAnim[0], parent);
			STS()->AddAnimationComponent(anim_rods, 0.0, 1.0, 
				pRod3LAnim[1]);
		
		
			//Clockwise actuator of pair 3
			STS()->AddAnimationComponent(anim_rods, 0.0, 1.0, 
				pRod3RAnim[0], parent);
			STS()->AddAnimationComponent(anim_rods, 0.0, 1.0, 
				pRod3RAnim[1]);
		
		

			
			
		}

		
	}

	bool ODS::HasDSCUPower() const {
		return bPowerRelay;
	}

	bool ODS::OnParseLine(const char* keyword, const char* line)
	{
		if(!_strnicmp(keyword, "RING_STATE", 10))
		{
			sscan_state((char*)line, RingState);
			return true;
		}
		else {
			return false;
		}
	}

	void ODS::AddMeshes( const VECTOR3 &ofs )
	{
		if (mesh_ods == MESH_UNDEFINED)
		{
			VECTOR3 pos = _V( EXTERNAL_AIRLOCK_POS.x, EXTERNAL_AIRLOCK_POS.y, ofs.z );
			mesh_ods = STS()->AddMesh( hODSMesh, &pos );
			oapiWriteLog( "ODS mesh added" );
		}
		STS()->SetMeshVisibilityMode( mesh_ods, MESHVIS_EXTERNAL | MESHVIS_VC | MESHVIS_EXTPASS );

		ExtAirlock::AddMeshes( ofs );
		return;
	}

	void ODS::SetDockParams( double EALzpos )
	{
		VECTOR3 DockPos = _V( EXTERNAL_AIRLOCK_POS.x + ODS_DOCKPOS_OFFSET.x, EXTERNAL_AIRLOCK_POS.y + ODS_DOCKPOS_OFFSET.y, EALzpos + ODS_DOCKPOS_OFFSET.z );
		STS()->SetDockParams( DockPos, _V( 0, 1, 0 ), _V( 0, 0, -1 ) );
		return;
	}

	void ODS::UpdateODSAttachment( const VECTOR3 &ofs )
	{
		VECTOR3 pos = ofs + ODS_DOCKPOS_OFFSET + EXTERNAL_AIRLOCK_POS;
		if (ahDockAux) STS()->SetAttachmentParams( ahDockAux, pos, odsAttachVec[1] - odsAttachVec[0], odsAttachVec[2] - odsAttachVec[0] );
		else ahDockAux = STS()->CreateAttachment( false, pos, odsAttachVec[1] - odsAttachVec[0], odsAttachVec[2] - odsAttachVec[0], "APAS" );
		return;
	}
};

