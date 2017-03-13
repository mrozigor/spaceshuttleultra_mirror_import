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
		bAPDSCircuitProtectionOff(false),
		extend_goal(RETRACT_TO_FINAL),
		bLatchesOpen(false),
		bLatchesClosed(true),
		bHooks1Open(true),
		bHooks1Closed(false),
		bHooks2Open(true),
		bHooks2Closed(false),
		bFixersOn(true),
		bTargetInCone(false),
		bTargetCaptured(false),
		APASdevices_populated(false),
		bFirstStep(true)
	{
		anim_ring = NULL;
		pRingAnim = NULL;
		RingState.Set(AnimState::STOPPED, 0.0);
		odsAttachVec[0] = ODS_DOCKPOS_OFFSET + EXTERNAL_AIRLOCK_POS;
		odsAttachVec[1] = odsAttachVec[0] + _V(0.0, 1.0, 0.0);
		odsAttachVec[2] = odsAttachVec[0] + _V(0.0, 0.0, -1.0);
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

	void ODS::PopulateAPASdevices( void )
	{
		APASdevices.clear();

		OBJHANDLE pOH;
		VESSEL* pV;
		ATTACHMENTHANDLE pAH;
		for (DWORD i = oapiGetVesselCount(); i > 0; i--)
		{
			pOH = oapiGetVesselByIndex( i - 1 );
			if (pOH != STS()->GetHandle())
			{
				pV = oapiGetVesselInterface( pOH );
				for (DWORD j = pV->AttachmentCount( true ); j > 0; j--)
				{
					pAH = pV->GetAttachmentHandle( true, j - 1 );

					if (!_strnicmp( pV->GetAttachmentId( pAH ), "APAS", 4 ))
					{
						APASdevices.push_back( make_pair( pOH, pAH ) );

						// log output
						char cbuf[64];
						sprintf_s( cbuf, 64, "ODS APAS search TGT:%s ID:%d", pV->GetName(), j );
						oapiWriteLog( cbuf );
					}
				}
			}
		}

		APASdevices_populated = true;
		return;
	}

	bool ODS::FindClosestAPAS( void )
	{
		bool ret = false;
		OBJHANDLE pOH;
		ATTACHMENTHANDLE pAH;
		VECTOR3 gpos;
		VECTOR3 tgt_pos;
		VECTOR3 tgtAPAS_pos;
		VECTOR3 tgtAPAS_dir;
		VECTOR3 tgtAPAS_rot;
		VECTOR3 tgt_pos_APAS;
		VECTOR3 rel_vel;

		target_pos = _V( 999, 999, 999 );// "reset" tgt vessel

		for (unsigned int i = APASdevices.size(); i > 0; i--)
		{
			pOH = APASdevices[i - 1].first;

			// calculate CG/CG distance
			oapiGetGlobalPos( pOH, &gpos );
			STS()->Global2Local( gpos, tgt_pos );

			// check if cg/cg distance is under 200m (protect for large vessels)
			if (length( tgt_pos ) < 200)
			{
				// calculate APAS/APAS distance
				pAH = APASdevices[i - 1].second;

				VESSEL* pV = oapiGetVesselInterface( pOH );

				pV->GetAttachmentParams( pAH, tgtAPAS_pos, tgtAPAS_dir, tgtAPAS_rot );
				pV->Local2Global( tgtAPAS_pos, gpos );
				STS()->Global2Local( gpos, tgt_pos_APAS );

				// correct for our APAS position and c.g.
				tgt_pos_APAS -= odsAttachVec[0] + STS()->GetOrbiterCoGOffset();

				// check if APAS/APAS distance under 5m
				if (length( tgt_pos_APAS ) < 5)
				{
					if (length( tgt_pos_APAS ) < length( target_pos ))
					{
						ahTarget = pAH;
						ohTarget = pOH;
						target_pos = tgt_pos_APAS;

						pV->Local2Global( tgtAPAS_pos + tgtAPAS_dir, gpos );
						STS()->Global2Local( gpos, target_dir );
						target_dir -= tgt_pos_APAS + odsAttachVec[0] + STS()->GetOrbiterCoGOffset();
						target_dir = -target_dir;// correct direction so when lined up the angle is 0º instead of 180º

						pV->Local2Global( tgtAPAS_pos + tgtAPAS_rot, gpos );
						STS()->Global2Local( gpos, target_rot );
						target_rot -= tgt_pos_APAS + odsAttachVec[0] + STS()->GetOrbiterCoGOffset();
						
						STS()->GetRelativeVel( pOH, rel_vel );
						target_vel.x = dotp( rel_vel, eX );
						target_vel.y = dotp( rel_vel, eY );
						target_vel.z = dotp( rel_vel, eZ );

						ret = true;
					}
				}
			}
		}
		return ret;
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
		/*if (bFirstStep)
		{
			UpdateODSAttachment();
			bFirstStep = false;
		}*/
	}

	void ODS::OnPreStep(double fSimT, double fDeltaT, double fMJD)
	{
		//if (!APASdevices_populated) PopulateAPASdevices();

		//STS()->GlobalRot(_V(1,0,0),eX);
		//STS()->GlobalRot(_V(0,1,0),eY);
		//STS()->GlobalRot(_V(0,0,1),eZ);
		//
		//if (bLatchesClosed && !bTargetCaptured)// search target to attach
		//{
		//	if (FindClosestAPAS())
		//	{
		//		VESSEL* pV = oapiGetVesselInterface( ohTarget );
		//		int iD = pV->GetAttachmentIndex( ahTarget );

		//		if ((target_pos.x < 0.1) &&(target_pos.y < 0.1) && (target_pos.z < 0.1) &&
		//			(acos( dotp( target_dir, odsAttachVec[1] - odsAttachVec[0] ) ) < (5 * RAD)) && (acos( dotp( target_rot, odsAttachVec[2] - odsAttachVec[0] ) ) < (5 * RAD)))
		//		{
		//			if (STS()->AttachChild( ohTarget, ahDockAux, ahTarget )) bTargetCaptured = true;
		//		}

		//		sprintf_s( oapiDebugString(), 255, "ODS: LOCK %s:%d | %5.4f %5.4f %5.4f | %5.4f %5.4f %5.4f | %.3f %.3f", 
		//			pV->GetName(), iD, target_pos.x, target_pos.y, target_pos.z,
		//			target_vel.x, target_vel.y, target_vel.z, 
		//			acos( dotp( target_dir, odsAttachVec[1] - odsAttachVec[0] ) ) * DEG, acos( dotp( target_rot, odsAttachVec[2] - odsAttachVec[0] ) ) * DEG );
		//		/*sprintf_s( oapiDebugString(), 255, "ODS: LOCK %s:%d | %5.4f\" %5.4f' %5.4f\" | %5.4f %5.4f %5.4f", 
		//			pV->GetName(), iD, target_pos.x / INCH, target_pos.y / MPS2FPS, target_pos.z / INCH,
		//			target_vel.x / MPS2FPS, target_vel.y / MPS2FPS, target_vel.z / MPS2FPS);*/
		//	}
		//	else
		//	{
		//		sprintf_s( oapiDebugString(), 255, "ODS: APAS PORTS:%d", APASdevices.size() );
		//	}

		//	//If target in range:
		//	//Calculate contacts and trigger initial contact signal.
		//	//If all latches matching and overcoming resistence, capture.

		//	//if captured
		//	//simulate oscillations of structure
		//}

		bool CNTL_PNL = (dscu_ControlPanelPowerA.IsSet() && dscu_ControlPanelPowerB.IsSet()) || (dscu_ControlPanelPowerB.IsSet() && dscu_ControlPanelPowerC.IsSet()) || (dscu_ControlPanelPowerC.IsSet() && dscu_ControlPanelPowerA.IsSet());

		if ((dscu_APDSPowerA.IsSet() && dscu_APDSPowerB.IsSet()) || (dscu_APDSPowerB.IsSet() && dscu_APDSPowerC.IsSet()) || (dscu_APDSPowerC.IsSet() && dscu_APDSPowerA.IsSet()))
		{
			if (dscu_PowerOn.IsSet() && CNTL_PNL)
			{
				bPowerRelay = true;
				bAPDSCircuitProtectionOff = false;
			} 

			if (dscu_PowerOff.IsSet() && CNTL_PNL) bPowerRelay = false;
		}
		else
		{
			bPowerRelay = false;
		}
		

		if (HasDSCUPower())
		{
			dscu_PowerOnLight.SetLine( (int)CNTL_PNL * 5.0f );
			dscu_RingAlignedLight.SetLine( (int)CNTL_PNL * 5.0f );

			if(dscu_APDSCircProtectionOff.IsSet() && CNTL_PNL) {
				bAPDSCircuitProtectionOff = true;
			}

			if(dscu_FixerOff && CNTL_PNL) {
				bFixersOn = false;
			}

			if(bAPDSCircuitProtectionOff) {
				dscu_APDSCircProtectLight.SetLine( (int)CNTL_PNL * 5.0f );
			} else {
				dscu_APDSCircProtectLight.ResetLine();
			}

			if(!bFixersOn) {
				dscu_FixersOffLight.SetLine( (int)CNTL_PNL * 5.0f );
			} else {
				dscu_FixersOffLight.ResetLine();
			}

			if(dscu_RingOut.IsSet() && CNTL_PNL && bAPDSCircuitProtectionOff) {
				RingState.action = AnimState::OPENING;
				extend_goal = EXTEND_TO_INITIAL;
				if(RingState.pos >= 0.7229167) {
					extend_goal = EXTEND_TO_FINAL;
				}
				bFixersOn = true;
			}


			if(dscu_RingIn.IsSet() && CNTL_PNL && bAPDSCircuitProtectionOff) {
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
				UpdateODSAttachment();

				CalculateRodAnimation();
			}

			if(RingState.pos < 0.0631944) {
				dscu_RingFinalLight.SetLine( (int)CNTL_PNL * 5.0f );
			} else {
				dscu_RingFinalLight.ResetLine();
			}

			if((RingState.pos >= 0.7229167&& RingState.pos < 0.7493056)) {
				dscu_RingInitialLight.SetLine( (int)CNTL_PNL * 5.0f );
			} else {
				dscu_RingInitialLight.ResetLine();
			}

			if(RingState.pos >= 0.9868056 ) {
				dscu_RingForwardLight.SetLine( (int)CNTL_PNL * 5.0f );
			} else {
				dscu_RingForwardLight.ResetLine();
			}

			if(bLatchesClosed) {
				dscu_LatchesClosedLight.SetLine( (int)CNTL_PNL * 5.0f );
			} else {
				dscu_LatchesClosedLight.ResetLine();
			}

			if(bLatchesOpen) {
				dscu_LatchesOpenLight.SetLine( (int)CNTL_PNL * 5.0f );
			} else {
				dscu_LatchesOpenLight.ResetLine();
			}

			if(bHooks1Open) {
				dscu_Hooks1OpenLight.SetLine( (int)CNTL_PNL * 5.0f );
			} else {
				dscu_Hooks1OpenLight.ResetLine();
			}

			if(bHooks2Open) {
				dscu_Hooks2OpenLight.SetLine( (int)CNTL_PNL * 5.0f );
			} else {
				dscu_Hooks2OpenLight.ResetLine();
			}

			if(bHooks1Closed) {
				dscu_Hooks1ClosedLight.SetLine( (int)CNTL_PNL * 5.0f );
			} else {
				dscu_Hooks1ClosedLight.ResetLine();
			}

			if(bHooks2Closed) {
				dscu_Hooks2ClosedLight.SetLine( (int)CNTL_PNL * 5.0f );
			} else {
				dscu_Hooks2ClosedLight.ResetLine();
			}

			if(bTargetCaptured) dscu_CaptureLight.SetLine( (int)CNTL_PNL * 5.0f );
			else dscu_CaptureLight.ResetLine();

		} else {
			dscu_PowerOnLight.ResetLine();
			dscu_APDSCircProtectLight.ResetLine();
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

		dscu_ADSLight.SetLine( (int)dscu_APDSPowerA.IsSet() * 5.0f );
		dscu_BDSLight.SetLine( (int)dscu_APDSPowerB.IsSet() * 5.0f );
		dscu_CDSLight.SetLine( (int)dscu_APDSPowerC.IsSet() * 5.0f );
		dscu_APLight.SetLine( (int)dscu_PyrosAp.IsSet() * 5.0f );
		dscu_BPLight.SetLine( (int)dscu_PyrosBp.IsSet() * 5.0f );
		dscu_CPLight.SetLine( (int)dscu_PyrosCp.IsSet() * 5.0f );
		return;
	}

	void ODS::OnPropagate(double fSimT, double fDeltaT, double fMJD)
	{
	}

	void ODS::Realize() {
		oapiWriteLog("(ssu)Realize ODS...");

		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "PANELA8A3_TO_DSCU_A", 16 );
		dscu_PowerOn.Connect( pBundle, 0 );
		dscu_PowerOff.Connect( pBundle, 1 );
		dscu_RingOut.Connect( pBundle, 2 );
		dscu_RingIn.Connect( pBundle, 3 );
		dscu_APDSCircProtectionOff.Connect( pBundle, 4 );
		dscu_CloseHooks.Connect( pBundle, 5 );
		dscu_CloseLatches.Connect( pBundle, 6 );
		dscu_FixerOff.Connect( pBundle, 7 );
		dscu_PyroCircProtOff.Connect( pBundle, 8 );
		dscu_PyroCircProtOn.Connect( pBundle, 9 );
		dscu_ActHooksFiring.Connect( pBundle, 10 );
		dscu_PasHooksFiring.Connect( pBundle, 11 );
		dscu_OpenHooks.Connect( pBundle, 12 );
		dscu_OpenLatches.Connect( pBundle, 13 );
		dscu_Undocking.Connect( pBundle, 14 );

		pBundle = STS()->BundleManager()->CreateBundle( "PANELA8A3_TO_DSCU_B", 16 );
		dscu_ControlPanelPowerA.Connect( pBundle, 0 );
		dscu_ControlPanelPowerB.Connect( pBundle, 1 );
		dscu_ControlPanelPowerC.Connect( pBundle, 2 );
		dscu_HeatersDCUPowerH1.Connect( pBundle, 3 );
		dscu_HeatersDCUPowerH2DCU.Connect( pBundle, 4 );
		dscu_HeatersDCUPowerH3DCU.Connect( pBundle, 5 );
		dscu_APDSPowerA.Connect( pBundle, 6 );
		dscu_APDSPowerB.Connect( pBundle, 7 );
		dscu_APDSPowerC.Connect( pBundle, 8 );
		dscu_PyrosAp.Connect( pBundle, 9 );
		dscu_PyrosBp.Connect( pBundle, 10 );
		dscu_PyrosCp.Connect( pBundle, 11 );

		DiscreteBundle* pBundleA = 
			STS()->BundleManager()->CreateBundle("DSCU_TO_PANELA8A3_A", 16);
		dscu_PowerOnLight.Connect( pBundleA, 0 );
		dscu_APDSCircProtectLight.Connect( pBundleA, 1 );
		dscu_RingAlignedLight.Connect( pBundleA, 2 );
		dscu_RingInitialLight.Connect( pBundleA, 3 );
		dscu_FixersOffLight.Connect( pBundleA, 4 );
		dscu_Hooks1OpenLight.Connect( pBundleA, 5 );
		dscu_Hooks2OpenLight.Connect( pBundleA, 6 );
		dscu_LatchesClosedLight.Connect( pBundleA, 7 );
		dscu_UndockCompleteLight.Connect( pBundleA, 8 );
		dscu_InitialContactLight.Connect( pBundleA, 9 );
		dscu_CaptureLight.Connect( pBundleA, 10 );
		dscu_RingForwardLight.Connect( pBundleA, 11 );
		dscu_ReadyToHookLight.Connect( pBundleA, 12 );
		dscu_InterfSealedLight.Connect( pBundleA, 13 );
		dscu_Hooks1ClosedLight.Connect( pBundleA, 14 );
		dscu_Hooks2ClosedLight.Connect( pBundleA, 15 );

		DiscreteBundle* pBundleB = 
			STS()->BundleManager()->CreateBundle("DSCU_TO_PANELA8A3_B", 16);
		dscu_LatchesOpenLight.Connect( pBundleB, 0 );
		dscu_RingFinalLight.Connect( pBundleB, 1 );
		dscu_PyroProtectCircuitOff.Connect( pBundleB, 2 );
		dscu_ADSLight.Connect( pBundleB, 3 );
		dscu_BDSLight.Connect( pBundleB, 4 );
		dscu_CDSLight.Connect( pBundleB, 5 );
		dscu_APLight.Connect( pBundleB, 6 );
		dscu_BPLight.Connect( pBundleB, 7 );
		dscu_CPLight.Connect( pBundleB, 8 );

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

		Zpos = EALzpos;
		odsAttachVec[0] = ODS_DOCKPOS_OFFSET + EXTERNAL_AIRLOCK_POS + _V( 0, 0, Zpos );
		odsAttachVec[1] = odsAttachVec[0] + _V(0.0, 1.0, 0.0);
		odsAttachVec[2] = odsAttachVec[0] + _V(0.0, 0.0, -1.0);
		return;
	}

	void ODS::UpdateODSAttachment( void )
	{
		/*if (ahDockAux) STS()->SetAttachmentParams( ahDockAux, odsAttachVec[0] + STS()->GetOrbiterCoGOffset(), odsAttachVec[1] - odsAttachVec[0], odsAttachVec[2] - odsAttachVec[0] );
		else ahDockAux = STS()->CreateAttachment( false, odsAttachVec[0] + STS()->GetOrbiterCoGOffset(), odsAttachVec[1] - odsAttachVec[0], odsAttachVec[2] - odsAttachVec[0], "APAS" );*/
		return;
	}
};

