#pragma once
#include "ExtAirlock.h"
#include "../discsignals/DiscOutPort.h"
#include "../discsignals/DiscInPort.h"

const static char* ODS_MESHNAME = "ODS";

/**
 * EVA and Docking systems
 */
namespace eva_docking {

	using class discsignals::DiscInPort;
	using class discsignals::DiscOutPort;

	class ODS: public ExtAirlock {
	protected:
		//UINT midxODS;
		double fRingState;
		double fHookState;
		double fLatchState;
		double fVestPressure[2];
		double fInterfAtmPressure[2];
		double fInterfAtmMass[2];

		bool bDSCUPower;

		AnimState RingState;

		UINT anim_ring;
		MGROUP_TRANSLATE*	pRingAnim;
		MGROUP_SCALE*		pCoilAnim;
		MGROUP_ROTATE*		pRod1LAnim[3];
		MGROUP_ROTATE*		pRod2LAnim[3];
		MGROUP_ROTATE*		pRod3LAnim[3];
		MGROUP_ROTATE*		pRod1RAnim[3];
		MGROUP_ROTATE*		pRod2RAnim[3];
		MGROUP_ROTATE*		pRod3RAnim[3];

	public:
		ODS(SubsystemDirector* pdirect, const string& _ident);
		virtual ~ODS();
		virtual void Realize();
		//virtual void AddMeshes(const VECTOR3& ofs);
		virtual double GetSubsystemEmptyMass() const;
		virtual void OnPreStep(double fSimT, double fDeltaT, double fMJD);
		virtual void OnPropagate(double fSimT, double fDeltaT, double fMJD);
		virtual void OnPostStep(double fSimT, double fDeltaT, double fMJD);
		virtual void DefineAirlockAnimations(UINT midx_extal, UINT midx_ods, const VECTOR3& ofs);

		DiscInPort dscu_PowerOn;
		DiscInPort dscu_PowerOff;
		DiscInPort dscu_RingOut;
		DiscInPort dscu_RingIn;
		DiscInPort dscu_CircProtectionOff;
		DiscInPort dscu_CloseHooks;
		DiscInPort dscu_CloseLatches;
		DiscInPort dscu_FixerOff;
		DiscInPort dscu_LampTest;

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

