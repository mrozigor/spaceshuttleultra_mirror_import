// ==============================================================
//                 ORBITER MODULE: Atlantis
//                  Part of the ORBITER SDK
//          Copyright (C) 2001-2003 Martin Schweiger
//                   All rights reserved
//
// Atlantis.cpp
// Reference implementation of Atlantis (Space Shuttle) vessel
// class module
//
// RMS, grappling and MMU capabilities by Robert Conley
// ==============================================================

#define STRICT 1
#define ORBITER_MODULE
#include "Atlantis.h"
#include "Atlantis_defs.h"
#include "Atlantis_SRB.h"
#include "Atlantis_Tank.h"
#include "CRT.h"
#include "ParameterValues.h"
#include "AerodynamicLookupTables.h"
#include "CommonDefs.h"
#include "SSUOptions.h"
#include "Atlantis_vc_defs.h"
#include <OrbiterSoundSDK40.h>
#include "DlgCtrl.h"
#include "meshres.h"
#include "meshres_vc.h"
#include "meshres_RMS.h"
#include "meshres_KU.h"
#include "meshres_vc_additions.h"
#include "resource.h"
#include "AtlantisSubsystemDirector.h"
#include "dps/AerojetDAP.h"
#include "dps/AP101S.h"
#include "dps/IDP.h"
#include "dps/MasterTimingUnit.h"
#include "dps/MDM.h"
#include "dps/RSLS.h"
#include "dps/ShuttleBus.h"
#include "dps/SimpleGPCSystem.h"
#include "eva_docking/ODS.h"
#include "AirDataProbeSystem.h"
#include "ETUmbDoorSystem.h"
#include "WSB.h"
#include "MCA.h"
#include "Latch.h"
#include "RMSSystem.h"
#include "StbdMPMSystem.h"
#include "ASE_IUS.h"
#include "CISS.h"
#include "MechActuator.h"
#include "PayloadBay.h"
#include "mps/SSME_BLOCK_II.h"
#include "vc/PanelA7A8ODS.h"
#include "vc/PanelF2.h"
#include "vc/PanelF3.h"
#include "vc/PanelF4.h"
#include "vc/PanelF6.h"
#include "vc/PanelF7.h"
#include "vc/PanelF8.h"
#include "vc/PanelO6.h"
#include "vc/PanelO17.h"
#include "vc/PanelA1U.h"
#include "vc/PanelA2.h"
#include "vc/PanelA4.h"
#include "vc/PanelA6.h"
#include "vc/PanelA8.h"
#include "vc/PanelA7U.h"
#include "vc/PanelR2.h"
#include "vc/PanelR11.h"
#include "vc/PanelR13L.h"
#include "vc/AftMDU.h"
#include "vc/PanelC2.h"
#include "vc/PanelC3.h"
#include "vc/PanelL10.h"
#include "vc/PanelL12U.h"
#include "comm\GCIL.h"
#include "comm\DeployedAssembly.h"
#include "comm\ElectronicsAssembly1.h"
#include "comm\ElectronicsAssembly2.h"
#include <UltraMath.h>
#include <cassert>



#ifdef INCLUDE_OMS_CODE
#include "OMSSubsystem.h"
#endif
#include "CommModeHandler.h"


#include "MLP/MLP.h"
#include "SLC6/SLC6.h"

#include <stdio.h>
#include <fstream>

#include "util/StopWatch.h"

#ifdef _DEBUG
// D. Beachy: for BoundsChecker debugging
extern int GrowStack();
#endif

#define LOADBMP(id) (LoadBitmap (g_Param.hDLL, MAKEINTRESOURCE (id)))

// ==============================================================
// Global (class-wide) parameters

GDIParams g_Param;

HELPCONTEXT g_hc = {
	"html/vessels/Atlantis.chm",
	0,
	"html/vessels/Atlantis.chm::/Atlantis.hhc",
	"html/vessels/Atlantis.chm::/Atlantis.hhk"
};

std::ofstream animlog;

inline void BeginLoggingAnims(void)
{
	animlog.open("SSUAnimation.log");
	animlog << "ANIMATION LOG" << std::endl;
}

VECTOR3 CalcOMSThrustDir(unsigned int side, double pitch, double yaw)
{
	VECTOR3 dir;
	if (side == 0) dir = L_OMS_DIR;
	else dir = R_OMS_DIR;

	dir = RotateVectorX(dir, pitch); //positive OMS gimbal directs thrust upwards
	dir = RotateVectorY(dir, -yaw); //positive yaw gimbal directs thrust to right
	return dir;
}

inline void LogAnim(const std::string& name, UINT anim)
{

	animlog << name << "\t" << std::dec << anim << " ( 0x" << std::hex << anim << ")" << std::endl;
}

inline void EndLoggingAnims(void)
{

	animlog.close();
}


inline void ssuAssert(bool expression, const char* context)
{
	static char buf[200];
	if (!expression)
	{
		sprintf_s(buf, 200, "[SpaceShuttleUltra] ERROR assertion failed : %s", context);
		oapiWriteLog(buf);
	}

}

/**
 * Computes mass and CoG of vessel, including attached payload
 * Return mass and CoG in local reference frame of vessel
 */
double GetMassAndCoG(VESSEL* v, VECTOR3& CoG)
{
	double mass = v->GetMass();
	CoG = _V(0, 0, 0);
	// iterate over attached children
	DWORD ahCount = v->AttachmentCount(false);
	for (DWORD i = 0; i < ahCount; i++) {
		ATTACHMENTHANDLE ah = v->GetAttachmentHandle(false, i);
		OBJHANDLE hV = v->GetAttachmentStatus(ah);
		if (hV) {
			VESSEL* pV = oapiGetVesselInterface(hV);
			VECTOR3 childCoG;
			double childMass = GetMassAndCoG(pV, childCoG);
			// convert child CoG to CoG in frame of this vessel
			VECTOR3 rpos;
			pV->GetRelativePos(v->GetHandle(), rpos);
			MATRIX3 LocalToGlob, ChildToGlob;
			v->GetRotationMatrix(LocalToGlob);
			pV->GetRotationMatrix(ChildToGlob);
			childCoG = tmul(LocalToGlob, mul(ChildToGlob, childCoG) + rpos);
			// calculate CoG of payload relative to center of Orbiter mesh
			CoG += childCoG*childMass;
			mass += childMass;

			char cbuf[255];
			sprintf_s(cbuf, 255, "Parent: %s Child: %s dist: %f", v->GetName(), pV->GetName(), length(childCoG));
			oapiWriteLog(cbuf);
		}
	}
	CoG = CoG / mass;
	return mass;
}

// ==============================================================
// Local prototypes

extern void GetSRB_State(double met, double &thrust_level, double &prop_level);


// initialized in InitModule
Aerodynamics::ThreeDLookup elevonVerticalLookup;
Aerodynamics::ThreeDLookup verticalLookup;
Aerodynamics::ThreeDLookup bodyFlapVerticalLookup;
Aerodynamics::ThreeDLookup groundEffectLookup;
Aerodynamics::ThreeDLookup groundEffectBodyFlapLookup;
Aerodynamics::FourDLookup horizontalLookup;
Aerodynamics::ThreeDLookup aileronHorizontalLookup;
//const Aerodynamics::SpeedbrakeVerticalLookup speedbrakeVerticalLookup;

void AscentLiftCoeff(double aoa, double M, double Re, double *cl, double *cm, double *cd)
{
	// we don't have data for first stage aerodynamics
	// use entry model and modify coefficients to get realistic first stage performance
	AerosurfacePositions aero;
	aero.leftElevon = aero.rightElevon = aero.bodyFlap = aero.speedbrake = aero.rudder = 0.0;
	GetShuttleVerticalAeroCoefficients(M, aoa*DEG, 1e3, &aero, cl, cm, cd); // hardcode altitude to value large enough that shuttle is out of ground effect
	// scale coefficients to get accurate first stage performance
	*cl = 0.8*(*cl);
	*cm = 0.5*(*cm);
	*cd = 2.5*(*cd);
}

void VLiftCoeff(VESSEL *v, double aoa, double M, double Re, void* lv, double *cl, double *cm, double *cd)
{
	/*double basicLift, basicDrag, basicMoment;
	double elevonLift, elevonDrag, elevonMoment;
	double bodyFlapLift, bodyFlapDrag, bodyFlapMoment;
	//double speedbrakeLift, speedbrakeDrag, speedbrakeMoment;

	AerosurfacePositions* aerosurfaces = static_cast<AerosurfacePositions*>(lv);
	double elevonPos = (aerosurfaces->leftElevon+aerosurfaces->rightElevon)/2.0;
	//elevonPos = range(-15.0, elevonPos, 15.0);
	verticalLookup.GetValues(M, aoa*DEG, aerosurfaces->speedbrake, basicLift, basicDrag, basicMoment);
	elevonVerticalLookup.GetValues(M, aoa*DEG, elevonPos, elevonLift, elevonDrag, elevonMoment);
	bodyFlapVerticalLookup.GetValues(M, aoa*DEG, aerosurfaces->bodyFlap, bodyFlapLift, bodyFlapDrag, bodyFlapMoment);

	//speedbrakeVerticalLookup.GetValues(M, aoa*DEG, aerosurfaces->speedbrake, speedbrakeLift, speedbrakeDrag, speedbrakeMoment);

	*cl = basicLift+elevonLift;
	*cd = basicDrag+elevonDrag;
	*cm = basicMoment+elevonMoment;*/

	if (v->GetAltitude() < 150e3) { // if we are above 150km, ignore aerodynamic forces (lookup tables don't give good model)
		if (abs(aoa) > 90.0*RAD) aoa = 0.0; // handle Orbitersim bug which results in very large AOA at first timestep

		AerosurfacePositions* aerosurfaces = static_cast<AerosurfacePositions*>(lv);
		GetShuttleVerticalAeroCoefficients(M, aoa*DEG, v->GetAltitude(), aerosurfaces, cl, cm, cd);
	}
	else {
		*cl = 0.0;
		*cm = 0.0;
		*cd = 0.0;
	}
}

void HLiftCoeff(VESSEL *v, double beta, double M, double Re, void* lv, double *cl, double *cm, double *cd)
{
	//static const double step = RAD*22.5;
	//static const double istep = 1.0/step;
	//static const int nabsc = 17;
	//static const double CL[nabsc] = {0, 0.2, 0.3, 0.2, 0, -0.2, -0.3, -0.2, 0, 0.2, 0.3, 0.2, 0, -0.2, -0.3, -0.2, 0};

	if (v->GetAltitude() < 150e3) { // if we are above 150km, ignore aerodynamic forces (lookup tables don't give good model)
		if (abs(beta) > 90.0*RAD) beta = 0.0; // handle Orbitersim bug which results in very large beta at first timestep

		double aoa = v->GetAOA()*DEG;
		AerosurfacePositions* aerosurfaces = static_cast<AerosurfacePositions*>(lv);
		double elevonPos = (aerosurfaces->leftElevon + aerosurfaces->rightElevon) / 2.0;
		//double aileronPos = (aerosurfaces->leftElevon-aerosurfaces->rightElevon)/2.0;
		double aileronPos = aerosurfaces->rightElevon - aerosurfaces->leftElevon;

		double sideForce, yawMoment, rollMoment;
		horizontalLookup.GetValues(M, aoa, abs(beta)*DEG, aerosurfaces->speedbrake, sideForce, yawMoment, rollMoment);
		sideForce *= sign(beta);
		yawMoment *= sign(beta);
		rollMoment *= sign(beta);
		//sideForce = yawMoment = rollMoment = 0.0;

		double ailSideForce, ailYawMoment, ailRollMoment;
		// get linearized derivatives and multiply results by aileron deflection
		//aileronHorizontalLookup.GetValues(M, aoa*DEG, elevonPos, ailSideForce, ailYawMoment, ailRollMoment);
		aileronHorizontalLookup.GetValues(M, aoa, elevonPos, ailSideForce, ailYawMoment, ailRollMoment);
		sideForce += ailSideForce*aileronPos;
		yawMoment += ailYawMoment*aileronPos;
		rollMoment += ailRollMoment*aileronPos;

		// split side force into 'lift' and 'drag' components
		VECTOR3 sideForceVec = _V(sideForce, 0.0, 0.0);
		VECTOR3 ld = RotateVectorZ(sideForceVec, beta*DEG);

		double qbar = v->GetDynPressure();
		double rollForce = rollMoment*qbar*ORBITER_WING_AREA*ORBITER_SPAN;
		// add force caused by aileron deflection
		v->AddForce(_V(0.0, -0.5*rollForce, 0.0), _V(-1.0, 0.0, 0.0));
		v->AddForce(_V(0.0, 0.5*rollForce, 0.0), _V(1.0, 0.0, 0.0));

		*cl = ld.x;
		*cm = yawMoment;
		*cd = ld.y;
	}
	else {
		*cl = 0.0;
		*cm = 0.0;
		*cd = 0.0;
	}
}
void GetShuttleVerticalAeroCoefficients(double mach, double degAOA, double altitude, const AerosurfacePositions* aerosurfaces, double * cl, double * cm, double * cd)
{
	double basicLift, basicDrag, basicMoment;
	double elevonLift, elevonDrag, elevonMoment;
	double bodyFlapLift, bodyFlapDrag, bodyFlapMoment;
	double groundEffectLift, groundEffectDrag, groundEffectMoment;
	double groundEffectLift_BF, groundEffectDrag_BF, groundEffectMoment_BF;

	double elevonPos = (aerosurfaces->leftElevon + aerosurfaces->rightElevon) / 2.0;
	verticalLookup.GetValues(mach, degAOA, aerosurfaces->speedbrake, basicLift, basicDrag, basicMoment);
	elevonVerticalLookup.GetValues(mach, degAOA, elevonPos, elevonLift, elevonDrag, elevonMoment);
	bodyFlapVerticalLookup.GetValues(mach, degAOA, aerosurfaces->bodyFlap, bodyFlapLift, bodyFlapDrag, bodyFlapMoment);

	const VECTOR3 GROUND_EFFECT_REF = _V(0.0, -3.926, -14.043336); // location of reference point for computing altitude (for ground effect calculations)
	VECTOR3 groundEffectOffset = RotateVectorX(GROUND_EFFECT_REF, degAOA);
	double groundEffectHeight = altitude + groundEffectOffset.y;
	double heightOverSpan = groundEffectHeight / ORBITER_SPAN;
	//groundEffectLookup.GetValues(heightOverSpan, degAOA, elevonPos, groundEffectLift, groundEffectDrag, groundEffectMoment);
	if (heightOverSpan < 1.5) {
		// because of how ground effect tables are set up in NASA data, ground effect lookup tables are slightly different from other lookup tables
		// h/b (height over span) replaces aerosurface deflection, and aerosurface deflection replaces mach (ground effect is not mach-dependent)
		heightOverSpan = max(0.0, heightOverSpan);
		groundEffectLookup.GetValues(elevonPos, degAOA, heightOverSpan, groundEffectLift, groundEffectDrag, groundEffectMoment);
		groundEffectBodyFlapLookup.GetValues(aerosurfaces->bodyFlap, degAOA, heightOverSpan, groundEffectLift_BF, groundEffectDrag_BF, groundEffectMoment_BF);
	}
	else {
		groundEffectLift = 0.0;
		groundEffectDrag = 0.0;
		groundEffectMoment = 0.0;
		groundEffectLift_BF = 0.0;
		groundEffectDrag_BF = 0.0;
		groundEffectMoment_BF = 0.0;
	}

	*cl = basicLift + elevonLift + groundEffectLift + groundEffectLift_BF;
	*cd = basicDrag + elevonDrag + groundEffectDrag + groundEffectDrag_BF;
	*cm = basicMoment + elevonMoment + groundEffectMoment + groundEffectMoment_BF;
}

// ==============================================================
// Specialised vessel class Atlantis
// ==============================================================

// --------------------------------------------------------------
// Constructor
// --------------------------------------------------------------
Atlantis::Atlantis(OBJHANDLE hObj, int fmodel)
	: VESSEL3(hObj, fmodel),
	BodyFlap(0.5, 0.25, 0.1, -1.0, 1.0, -1.0, 1.0),
	ElevonPitch(0.25, 0.10, 0.01, -1.0, 1.0, -50.0, 50.0), //NOTE: may be better to reduce integral limits and increase i gain
	PitchControl(0.25, 0.001, 0.10, -1.0, 1.0, -5.0, 5.0),
	pActiveLatches(3, NULL)
{
#ifdef _DEBUG
	// D. Beachy: for BoundsChecker debugging
	GrowStack();
#endif
	int i;

	//----------------------------------------------------
	// Make these first, for avoiding CTDs
	//----------------------------------------------------
	pMission = ssuGetDefaultMission();			//No mission selected
	options = new SSUOptions();
	bundleManager = new DiscreteBundleManager();
	busManager = new dps::ShuttleBusManager();
	pCommModeHandler = new CommModeHandler(this);

	// initialize mdu pointers to NULL, otherwise we may get CTDs
	for (i = 0; i < 11; i++) mdus[i] = NULL;

	pgForward.AddPanel(new vc::PanelF2(this));
	pgForward.AddPanel(new vc::PanelF3(this));
	pgForward.AddPanel(new vc::PanelF4(this));
	pgForward.AddPanel(new vc::PanelF6(this));
	pgForward.AddPanel(new vc::PanelF7(this));
	pgForward.AddPanel(new vc::PanelF8(this));

	pgCenter.AddPanel(new vc::PanelC2(this));
	pgCenter.AddPanel(new vc::PanelC3(this));

	pgRight.AddPanel(panelr2 = new vc::PanelR2(this));

	pgOverhead.AddPanel(new vc::PanelO3(this));
	pgOverhead.AddPanel(new vc::PanelO6(this));

	pgOverheadAft.AddPanel(new vc::PanelO17(this));

	pgAft.AddPanel(new vc::PanelA6(this));
	pgAft.AddPanel(new vc::AftMDU(this));
	pgAft.AddPanel(new vc::PanelA7U(this));
	pgAft.AddPanel(new vc::PanelA4(this));
	pgAft.AddPanel( new vc::PanelA1U( this ) );
	pgAft.AddPanel( new vc::PanelA2( this ) );
	
	pgAftStbd.AddPanel(new vc::PanelR11(this));
	pgAftStbd.AddPanel(new vc::PanelR13L(this));
	
	pPanelA8 = NULL;
	pA7A8Panel = NULL;
	pPanelL10 = NULL;
	pPanelL12U = NULL;
	pExtAirlock = NULL;

	psubsystems = new AtlantisSubsystemDirector(this);

	psubsystems->AddSubsystem(pHeEng[0] = new mps::HeSysEng(psubsystems, "HeEng_C", 1));
	psubsystems->AddSubsystem(pHeEng[1] = new mps::HeSysEng(psubsystems, "HeEng_L", 2));
	psubsystems->AddSubsystem(pHeEng[2] = new mps::HeSysEng(psubsystems, "HeEng_R", 3));
	psubsystems->AddSubsystem(pHePneu = new mps::HeSysPneu(psubsystems, "HePneu"));

	psubsystems->AddSubsystem(pMPS = new mps::MPS(psubsystems, pHePneu));

	psubsystems->AddSubsystem(pSSME[0] = new mps::SSME_BLOCK_II(psubsystems, "MPS_C", 1, 2, "AD08", pHeEng[0]));
	psubsystems->AddSubsystem(pSSME[1] = new mps::SSME_BLOCK_II(psubsystems, "MPS_L", 2, 2, "AD08", pHeEng[1]));
	psubsystems->AddSubsystem(pSSME[2] = new mps::SSME_BLOCK_II(psubsystems, "MPS_R", 3, 2, "AD08", pHeEng[2]));

	psubsystems->AddSubsystem(pFMC1 = new MCA(psubsystems, "FMC1"));
	psubsystems->AddSubsystem(pFMC2 = new MCA(psubsystems, "FMC2"));
	psubsystems->AddSubsystem(pFMC3 = new MCA(psubsystems, "FMC3"));
	psubsystems->AddSubsystem(pMMC1 = new MCA(psubsystems, "MMC1"));
	psubsystems->AddSubsystem(pMMC2 = new MCA(psubsystems, "MMC2"));
	psubsystems->AddSubsystem(pMMC3 = new MCA(psubsystems, "MMC3"));
	psubsystems->AddSubsystem(pMMC4 = new MCA(psubsystems, "MMC4"));
	psubsystems->AddSubsystem(pAMC1 = new MCA(psubsystems, "AMC1"));
	psubsystems->AddSubsystem(pAMC2 = new MCA(psubsystems, "AMC2"));
	psubsystems->AddSubsystem(pAMC3 = new MCA(psubsystems, "AMC3"));

	psubsystems->AddSubsystem(pFF[0] = new dps::MDM(psubsystems, "FF1", 10, 10));
	psubsystems->AddSubsystem(pFF[1] = new dps::MDM(psubsystems, "FF2", 11, 11));
	psubsystems->AddSubsystem(pFF[2] = new dps::MDM(psubsystems, "FF3", 12, 12));
	psubsystems->AddSubsystem(pFF[3] = new dps::MDM(psubsystems, "FF4", 13, 13));

	psubsystems->AddSubsystem(pFA[0] = new dps::MDM(psubsystems, "FA1", 14, 14));
	psubsystems->AddSubsystem(pFA[1] = new dps::MDM(psubsystems, "FA2", 15, 15));
	psubsystems->AddSubsystem(pFA[2] = new dps::MDM(psubsystems, "FA3", 16, 16));
	psubsystems->AddSubsystem(pFA[3] = new dps::MDM(psubsystems, "FA4", 17, 17));

	psubsystems->AddSubsystem(pPL[0] = new dps::MDM(psubsystems, "PL1", 20, 20));
	psubsystems->AddSubsystem(pPL[1] = new dps::MDM(psubsystems, "PL2", 21, 21));

	psubsystems->AddSubsystem(pLF1 = new dps::MDM(psubsystems, "LF1", 22, 22));
	psubsystems->AddSubsystem(pLM1 = new dps::MDM(psubsystems, "LM1", 24, 24));
	psubsystems->AddSubsystem(pLA1 = new dps::MDM(psubsystems, "LA1", 23, 23));

	psubsystems->AddSubsystem(pOF[0] = new dps::MDM(psubsystems, "OF1", 1, 1));
	psubsystems->AddSubsystem(pOF[1] = new dps::MDM(psubsystems, "OF2", 2, 2));
	psubsystems->AddSubsystem(pOF[2] = new dps::MDM(psubsystems, "OF3", 3, 3));
	psubsystems->AddSubsystem(pOF[3] = new dps::MDM(psubsystems, "OF4", 4, 4));

	psubsystems->AddSubsystem(pOA[0] = new dps::MDM(psubsystems, "OA1", 5, 5));
	psubsystems->AddSubsystem(pOA[1] = new dps::MDM(psubsystems, "OA2", 6, 6));
	psubsystems->AddSubsystem(pOA[2] = new dps::MDM(psubsystems, "OA3", 7, 7));

	psubsystems->AddSubsystem(pLL[0] = new dps::MDM(psubsystems, "LL1", 25, 25));
	psubsystems->AddSubsystem(pLL[1] = new dps::MDM(psubsystems, "LL2", 26, 26));

	psubsystems->AddSubsystem(pLR[0] = new dps::MDM(psubsystems, "LR1", 27, 27));
	psubsystems->AddSubsystem(pLR[1] = new dps::MDM(psubsystems, "LR2", 28, 28));

	//Flexible MDMs
	//There are no flexible MDMs supported yet
	pFMDM[0] = pFMDM[1] = NULL;


	psubsystems->AddSubsystem(pEIU[0] = new mps::EIU(psubsystems, "EIU1", 1, pSSME[0]));
	psubsystems->AddSubsystem(pEIU[1] = new mps::EIU(psubsystems, "EIU2", 2, pSSME[1]));
	psubsystems->AddSubsystem(pEIU[2] = new mps::EIU(psubsystems, "EIU3", 3, pSSME[2]));

	psubsystems->AddSubsystem(pMTU = new dps::MasterTimingUnit(psubsystems));

	psubsystems->AddSubsystem(pIDP[0] = new dps::IDP(psubsystems, "IDP1", 1));
	psubsystems->AddSubsystem(pIDP[1] = new dps::IDP(psubsystems, "IDP2", 2));
	psubsystems->AddSubsystem(pIDP[2] = new dps::IDP(psubsystems, "IDP3", 3));
	psubsystems->AddSubsystem(pIDP[3] = new dps::IDP(psubsystems, "IDP4", 4));

	psubsystems->AddSubsystem(pIMU[0] = new gnc::IMU(psubsystems, "IMU1", 5));
	psubsystems->AddSubsystem(pIMU[1] = new gnc::IMU(psubsystems, "IMU2", 5));
	psubsystems->AddSubsystem(pIMU[2] = new gnc::IMU(psubsystems, "IMU3", 5));

	psubsystems->AddSubsystem(pGPC[0] = new dps::AP101S(psubsystems, "GPC1", 1));
	psubsystems->AddSubsystem(pGPC[1] = new dps::AP101S(psubsystems, "GPC2", 2));
	psubsystems->AddSubsystem(pGPC[2] = new dps::AP101S(psubsystems, "GPC3", 3));
	psubsystems->AddSubsystem(pGPC[3] = new dps::AP101S(psubsystems, "GPC4", 4));
	psubsystems->AddSubsystem(pGPC[4] = new dps::AP101S(psubsystems, "GPC5", 5));
	//Create Mass Memory Units
	psubsystems->AddSubsystem(pMMU[0] = new dps::MMU(psubsystems, "MMU1"));
	psubsystems->AddSubsystem(pMMU[1] = new dps::MMU(psubsystems, "MMU2"));

	pMMU[0]->SetTapeImage("Data/SSU/MMU1_TEST.dat");
	pMMU[1]->SetTapeImage("Data/SSU/MMU2_TEST.dat");

	psubsystems->AddSubsystem(pSimpleGPC = new dps::SimpleGPCSystem(psubsystems));
	pRSLS = static_cast<dps::RSLS_old*>(pSimpleGPC->FindSoftware("RSLS_old"));
	pATVC_SOP = static_cast<dps::ATVC_SOP*>(pSimpleGPC->FindSoftware("ATVC_SOP"));
	pSSME_SOP = static_cast<dps::SSME_SOP*>(pSimpleGPC->FindSoftware("SSME_SOP"));
	pASE_IUS = NULL;
	pCISS = NULL;

	psubsystems->AddSubsystem(pADPS = new AirDataProbeSystem(psubsystems));

	psubsystems->AddSubsystem(new ETUmbDoorSystem(psubsystems));

	psubsystems->AddSubsystem(pSTYDoorMotor = new MechActuator(psubsystems, "-YStarTrackerDoorMotor", 8.0));
	psubsystems->AddSubsystem(pSTZDoorMotor = new MechActuator(psubsystems, "-ZStarTrackerDoorMotor", 8.0));

	psubsystems->AddSubsystem(pACBusSystem = new eps::ACBusSystem(psubsystems));
	psubsystems->AddSubsystem(pInverter[0] = new eps::Inverter(psubsystems, "INVERTER1"));
	psubsystems->AddSubsystem(pInverter[1] = new eps::Inverter(psubsystems, "INVERTER2"));
	psubsystems->AddSubsystem(pInverter[2] = new eps::Inverter(psubsystems, "INVERTER3"));

	psubsystems->AddSubsystem( new comm::GCIL( psubsystems ) );

	psubsystems->AddSubsystem(pATVC = new gnc::ATVC(psubsystems, "ATVC", 1));// HACK should be 4 of this

	psubsystems->AddSubsystem(pPayloadBay = new PayloadBay(psubsystems));

	//psubsystems->AddSubsystem(new dps::AerojetDAP(psubsystems));

	pDeployedAssembly = NULL;

	pRMS = NULL; //don't create RMS unless it is used on the shuttle
	pMPMs = NULL;

	pTAA = NULL;

	RealizeSubsystemConnections();


#ifdef INCLUDE_OMS_CODE
	psubsystems->AddSubsystem(pOMS = new OMSSubsystem(psubsystems));
#else
	pOMS = NULL;
#endif

	//connect CRT MDUs to IDPs
	mdus[vc::MDUID_CDR1]->SetPrimaryIDP(pIDP[2]);
	mdus[vc::MDUID_CDR2]->SetPrimaryIDP(pIDP[0]);
	mdus[vc::MDUID_CRT1]->SetPrimaryIDP(pIDP[0]);
	mdus[vc::MDUID_CRT2]->SetPrimaryIDP(pIDP[1]);
	mdus[vc::MDUID_CRT3]->SetPrimaryIDP(pIDP[2]);
	mdus[vc::MDUID_CRT4]->SetPrimaryIDP(pIDP[3]);
	mdus[vc::MDUID_MFD1]->SetPrimaryIDP(pIDP[1]);
	mdus[vc::MDUID_MFD2]->SetPrimaryIDP(pIDP[0]);
	mdus[vc::MDUID_PLT1]->SetPrimaryIDP(pIDP[1]);
	mdus[vc::MDUID_PLT2]->SetPrimaryIDP(pIDP[2]);
	mdus[vc::MDUID_AFD]->SetPrimaryIDP(pIDP[3]);
	/*for (i=0;i<3;i++) {
		//pIDP[i]->ConnectToMDU(mdus[i+vc::MDUID_CRT1]);
		if(mdus[i+vc::MDUID_CRT1]) mdus[i+vc::MDUID_CRT1]->SetPrimaryIDP(pIDP[i]);
		}*/

	met = 0.0;

	status = STATE_ORBITER;
	ldoor_drag = rdoor_drag = 0.0;
	spdb_status = AnimState::CLOSED;
	spdb_proc = 0.0;
	spdb_tgt = 0.0;
	mesh_orbiter = MESH_UNDEFINED;
	mesh_cockpit = MESH_UNDEFINED;
	mesh_vc = MESH_UNDEFINED;
	mesh_middeck = MESH_UNDEFINED;
	mesh_kuband = MESH_UNDEFINED;
	mesh_cargo_static = MESH_UNDEFINED;
	mesh_dragchute = MESH_UNDEFINED;
	mesh_heatshield = MESH_UNDEFINED;

	vis = NULL;

	reset_mmu = false;
	jettison_time = 0.0;
	render_cockpit = false;
	bLiftOff = false;
	bHasKUBand = true;
	bUseRealRCS = true;
	bEnableMCADebug = false;

	vcDeckMode = VCM_FLIGHTDECK;

	___iCurrentManifold = 0;

	//SRB slag effects
	slag1 = 0.0;
	slag2 = 0.0;
	slag3 = 0.0;

	pshSlag1[0] = pshSlag1[1] = NULL;
	pshSlag2[0] = pshSlag2[1] = NULL;
	pshSlag3[0] = pshSlag3[1] = NULL;

	for (i = 0; i < 11; i++) {
		mfds[i] = -1;
	}
	for (i = 0; i < 11; i++)
		mfdbright[i] = 1.0;
	huds.ngroup = GRP_VIRTUALHUD_VC;
	huds.size = 0.176558;

	scnVCMode = 0;

	// propellant resources
	ph_oms = NULL;
	ph_frcs = NULL;
	ph_lrcs = NULL;
	ph_rrcs = NULL;
	ph_controller = NULL;
	ph_mps = NULL;
	ph_srb = NULL;
	thg_main = NULL;
	thg_retro = NULL;
	thg_srb = NULL;

	LOXmass = 0;
	LH2mass = 0;


	for (i = 0; i < 16; i++)
	{
		thFRCS[i] = NULL;
	}

	for (i = 0; i < 14; i++)
	{
		thLRCS[i] = NULL;
	}

	for (i = 0; i < 14; i++)
	{
		thRRCS[i] = NULL;
	}

	th_oms[0] = th_oms[1] = NULL;

	for (i = 0; i < 3; i++)
	{
		th_main[i] = NULL;
		th_ssme_gox[i] = NULL;
	}
	thMPSDump[0] = NULL;
	thMPSDump[1] = NULL;
	thMPSDump[2] = NULL;
	thMPSDump[3] = NULL;
	thMPSDump[4] = NULL;
	thMPSDump[5] = NULL;
	thMPSDump[6] = NULL;
	thMPSDump[7] = NULL;
	th_srb[0] = th_srb[1] = NULL;

	thg_pitchup = thg_pitchdown = NULL;
	thg_yawleft = thg_yawright = NULL;
	thg_rollleft = thg_rollright = NULL;
	thg_transfwd = thg_transaft = NULL;
	thg_transleft = thg_transright = NULL;
	thg_transup = thg_transdown = NULL;

	bSSMEGOXVent = true;
	bSSMEsDefined = false;
	bOMSDefined = false;
	bRCSDefined = false;
	bControllerThrustersDefined = false;

	phLOXdump = NULL;
	phLH2dump = NULL;

	oms_helium_tank[0] = NULL;
	oms_helium_tank[1] = NULL;
	for (i = 0; i < 3; i++) {
		ex_main[i] = NULL;
	}

	hStackAirfoil = NULL;
	//Control Surfaces
	hrudder = NULL;
	hbodyflap = NULL;
	helevator = NULL;
	hlaileron = NULL;
	hraileron = NULL;

	orbiter_ofs = _V(0, 0, 0);
	currentCoG = _V(0.0, 0.0, 0.0);

	payloadMass = 0.0;
	payloadCoG = _V(0, 0, 0);

	// preload meshes
	hOrbiterMesh = oapiLoadMeshGlobal(DEFAULT_MESHNAME_ORBITER);
	hOrbiterCockpitMesh = oapiLoadMeshGlobal(DEFAULT_MESHNAME_COCKPIT);
	hOrbiterVCMesh = oapiLoadMeshGlobal(DEFAULT_MESHNAME_VC);
	hMidDeckMesh = oapiLoadMeshGlobal(DEFAULT_MESHNAME_MIDDECK);
	hKUBandMesh = oapiLoadMeshGlobal(DEFAULT_MESHNAME_KU);
	hDragChuteMesh = oapiLoadMeshGlobal(DEFAULT_MESHNAME_CHUTE);
	hHeatShieldMesh = oapiLoadMeshGlobal("SSU/SSU_entry");
	hDevOrbiterMesh = NULL;
	hDevHeatShieldMesh = NULL;


	ControlSurfacesEnabled = false;

	//DefineAnimations();
	do_eva = false;
	do_plat = false;
	do_cargostatic = false;
	vis = NULL;
	ahHDP = NULL;
	ahTow = NULL;
	ahMMU[0] = NULL;
	ahMMU[1] = NULL;
	ahExtAL[0] = NULL;
	ahExtAL[1] = NULL;

	/*for (int i = 0; i < 3; i++)
	{
		ahCenterActive[i] = NULL;
	}*/
	for (int i = 0; i < 4; i++)
	{
		ahCenterPassive[i] = NULL;
		ahPortPL[i] = NULL;
		ahStbdPL[i] = NULL;
	}

	cargo_static_ofs = _V(0, 0, 0);

	// default arm status: deployed
	STBDMPM = false;
	RMS = false;

	gear_status.Set(AnimState::CLOSED, 0.0);
	gear_armed = false;
	DragChuteDeploying = false;
	DragChuteState = STOWED;
	DragChuteSize = 0.0;
	DragChuteSpin.Set(AnimState::OPENING, 0.0);

	// default camera positions
	for (int i = 0; i < 4; i++) {
		camYaw[i] = 0;
		camPitch[i] = 0;
	}

	cameraLowRate = false;
	cameraMoved = false;
	cameraControl = 0;

	bLastCamInternal = false;

	pl_mass = 0.0;

	newmfd = NULL;

	// gpc
	firstStep = true;


	RHCInput = _V(0, 0, 0);
	THCInput = _V(0, 0, 0);
	AltKybdInput = _V(0, 0, 0);

	for (i = 0; i < 3; i++) {
		lastRotCommand[i] = 0;
		lastTransCommand[i] = 0;
		RotationCommand.data[i] = 0.0;
		TranslationCommand.data[i] = 0.0;
		TransForce[0].data[i] = TransForce[1].data[i] = 0.0001; //small number to avoid divide by zero
	}

	aerosurfaces.leftElevon = aerosurfaces.rightElevon = 0.0;
	aerosurfaces.speedbrake = 0.0;
	aerosurfaces.bodyFlap = 0.0;
	aerosurfaces.rudder = 0.0;

	ControlRMS = false;
	lastRMSSJCommand = 0;

	SERCstop = true;

	LO2LowLevelSensor[0] = Sensor(65, 80);
	LO2LowLevelSensor[1] = Sensor(65, 80);
	LO2LowLevelSensor[2] = Sensor(65, 80);
	LO2LowLevelSensor[3] = Sensor(65, 80);

	pCommModeHandler->DefineAnnotations();

	fTimeCameraLabel = 0.0;
	nhCameraLabel = oapiCreateAnnotation(true, 1.0, _V(1.0, 1.0, 1.0));
	oapiAnnotationSetPos(nhCameraLabel, 0.4, 0.05, 0.6, 0.15);

	SoundID = -1;
	fSSMEHandleErrorFlag = false;

	psubsystems->AddSubsystem(pAPU[0] = new APU(psubsystems, "APU1", 1));
	psubsystems->AddSubsystem(pAPU[1] = new APU(psubsystems, "APU2", 2));
	psubsystems->AddSubsystem(pAPU[2] = new APU(psubsystems, "APU3", 3));

	psubsystems->AddSubsystem(new WSB(psubsystems, "WSB1", 1));
	psubsystems->AddSubsystem(new WSB(psubsystems, "WSB2", 2));
	psubsystems->AddSubsystem(new WSB(psubsystems, "WSB3", 3));

	// latch instances need to be created before scenario is loaded
	// latch positions are set in DefineAttachments() function
	VECTOR3 vPayloadPos = _V(0.0, PL_ATTACH_CENTER_Y, 0.0);
	psubsystems->AddSubsystem(pActiveLatches[0] = new ActiveLatchGroup(psubsystems, "LATCH0", vPayloadPos, DIR_CENTERPL, ROT_CENTERPL));
	psubsystems->AddSubsystem(pActiveLatches[1] = new ActiveLatchGroup(psubsystems, "LATCH1", vPayloadPos, DIR_CENTERPL, ROT_CENTERPL));
	psubsystems->AddSubsystem(pActiveLatches[2] = new ActiveLatchGroup(psubsystems, "LATCH2", vPayloadPos, DIR_CENTERPL, ROT_CENTERPL));

	for (unsigned short i = 0; i < 5; i++) {
		bPLBDCamPanLeft[i] = false;
		bPLBDCamPanRight[i] = false;
		bPLBDCamTiltUp[i] = false;
		bPLBDCamTiltDown[i] = false;
	}

	curOMSPitch[0] = curOMSPitch[RIGHT] = 0.0;
	curOMSYaw[0] = curOMSYaw[RIGHT] = 0.0;

	SSMEInstalledNullPos[0] = SSMET_INSTALLED_NULL_POS;
	SSMEInstalledNullPos[1] = SSMEL_INSTALLED_NULL_POS;
	SSMEInstalledNullPos[2] = SSMER_INSTALLED_NULL_POS;
	SSMECurrentPos[0] = SSMET_INSTALLED_NULL_POS;
	SSMECurrentPos[1] = SSMEL_INSTALLED_NULL_POS;
	SSMECurrentPos[2] = SSMER_INSTALLED_NULL_POS;

	for (int i = 0; i < 2; i++) SRBNullDirection[i] = _V(0.0, 0.0, 1.0);


	plbdCamPos[0] = CAM_A_POS;
	plbdCamPos[1] = CAM_B_POS;
	plbdCamPos[2] = CAM_C_POS;
	plbdCamPos[3] = CAM_D_POS;

	//PLB LIGHTS
	PLBLightPosition[0] = _V(1.6380, -2.1207, 5.5686); //forward stbd
	PLBLightPosition[1] = _V(-1.5549, -2.1732, 5.1957); //forward port
	PLBLightPosition[2] = _V(1.6380, -2.1207, -0.2724);//mid stbd
	PLBLightPosition[3] = _V(-1.6380, -2.1207, -0.2724);//mid port
	PLBLightPosition[4] = _V(1.6380, -2.1207, -4.3755);//aft stbd
	PLBLightPosition[5] = _V(-1.6380, -2.1207, -4.3755);//aft port
	FwdBulkheadLightPos = _V(0.0, 1.7550, 9.6065);//fwd bulkhead
	DockingLightPos = _V(0.0, 2.0162, 9.6057);//docking light

	//CREATE LIGHTS
	for (int i = 0; i < 6; ++i)
	{
		VECTOR3 dir = _V(-sign(PLBLightPosition[i].x), 0, 0);
		PLBLight[i] = AddPayloadBayLight(PLBLightPosition[i], dir, 135.0, PLB_bspec[i]);
	}
	FwdBulkheadLight = AddPayloadBayLight(FwdBulkheadLightPos, _V(0, 0, -1), 120.0, FwdBulkhead_bspec);
	for (int i = 0; i < 2; i++) {
		DockingLight[i] = AddPayloadBayLight(DockingLightPos, _V(0, 1, 0), 120.0, Docking_bspec[i]); // create two copies of docking light to simulate DIM and BRIGHT settings
	}

	// light for engines during launch
	//SSME light
	COLOUR4 col_diff_SSME = { 1.0, 1.0, 1.0, 0 };
	COLOUR4 col_zero_SSME = { 0, 0, 0, 0 };
	COLOUR4 col_ambient_SSME = { 0.5, 0.5, 0.5, 0 };
	//SRB light
	COLOUR4 col_diff_SRB = { 1.0, 0.733f, 0.196f, 0 };
	COLOUR4 col_zero_SRB = { 0, 0, 0, 0 };
	COLOUR4 col_ambient_SRB = { 0.5, 0.5, 0.5, 0 };
	SRBLight[0] = AddPointLight(_V(LSRB_OFFSET.x, LSRB_OFFSET.y, LSRB_OFFSET.z - 25.8), 300, 1e-3, 0, 0.0025, col_diff_SRB, col_zero_SRB, col_ambient_SRB);
	SRBLight[1] = AddPointLight(_V(RSRB_OFFSET.x, RSRB_OFFSET.y, RSRB_OFFSET.z - 25.8), 300, 1e-3, 0, 0.0025, col_diff_SRB, col_zero_SRB, col_ambient_SRB);
	SSMELight = AddPointLight(_V(0, (SSMET_REF.y + SSMEL_REF.y) / 2, SSMEL_REF.z - 2.9348), 300, 5e-3, 0, 5e-2, col_diff_SSME, col_zero_SSME, col_ambient_SSME);

	// RCS exhaust
	RCS_Exhaust_tex = oapiRegisterExhaustTexture("SSU\\Exhaust_atrcs");
	SURFHANDLE RCS_tex = oapiRegisterParticleTexture("SSU\\ps-rcs2");
	RCS_PSSpec.srcsize = 0.1;//0.1
	RCS_PSSpec.srcrate = 3000;//3000
	RCS_PSSpec.v0 = 50;//10
	RCS_PSSpec.srcspread = 0;//0
	RCS_PSSpec.lifetime = 0.2;//0.2
	RCS_PSSpec.growthrate = 25;//3.5
	RCS_PSSpec.atmslowdown = 5.0;//0.5
	RCS_PSSpec.ltype = PARTICLESTREAMSPEC::DIFFUSE;//EMSSIVE
	RCS_PSSpec.levelmap = PARTICLESTREAMSPEC::LVL_PLIN;//LVL_FLAT
	RCS_PSSpec.lmin = 0.1;//0.25
	RCS_PSSpec.lmax = 0.5;//0.25
	RCS_PSSpec.atmsmap = PARTICLESTREAMSPEC::ATM_FLAT;//ATM_FLAT
	RCS_PSSpec.amin = 1;//1
	RCS_PSSpec.tex = RCS_tex;

	reentry_flames = NULL;


	SSMEGH2burn[0] = NULL;
	SSMEGH2burn[1] = NULL;
	SSMEGH2burn[2] = NULL;

	hasCISS = false;
}

// --------------------------------------------------------------
// Destructor
// --------------------------------------------------------------
Atlantis::~Atlantis() {
	delete psubsystems;
	delete pCommModeHandler;

	for (unsigned int i = 0; i < vpAnimations.size(); i++) delete vpAnimations.at(i);

	delete busManager;
	delete bundleManager;
}

DiscreteBundleManager* Atlantis::BundleManager() const
{
	return bundleManager;
}

dps::ShuttleBusManager* Atlantis::BusManager() const {
	return busManager;
}

mission::Mission* Atlantis::GetMissionData() const
{
	return pMission;
}

const VECTOR3& Atlantis::GetOrbiterCoGOffset() const
{
	return orbiter_ofs;
}

// --------------------------------------------------------------
// launch configuration setup (orbiter + tank + SRB)
// --------------------------------------------------------------
void Atlantis::SetLaunchConfiguration(void)
{
	int i;
	extern PARTICLESTREAMSPEC srb_contrail, srb_exhaust, srb_slag1, srb_slag2, srb_slag3;

	// *********************** physical parameters *********************************

	SetSize(30.0);
	//SetEmptyMass (ORBITER_EMPTY_MASS + TANK_EMPTY_MASS + 2*SRB_EMPTY_MASS + pl_mass);
	SetCW(0.2, 0.5, 1.5, 1.5);
	SetCrossSections(_V(668.94, 676.94, 258.97));
	SetRotDrag(_V(0.7, 0.1, 0.3));
	SetPMI(_V(249.62, 239.97, 67.43));
	SetTrimScale(0.05);
	SetLiftCoeffFunc(0); // simplification: we assume no lift during launch phase
	SetTouchdownPoints(_V(0, -10, -55.8), _V(-7, 7, -55.8), _V(7, 7, -55.8));
	SetGravityGradientDamping(0.05);
	//SetEmptyMass(GetEmptyMass()+ 2*SRB_EMPTY_MASS);

	// ************************* propellant specs **********************************


	if (!ph_mps) ph_mps = CreatePropellantResource(MPS_MANIFOLD_MASS_TOTAL);    // mps manifold
	LOXmass = MPS_MANIFOLD_MASS_LOX;
	LH2mass = MPS_MANIFOLD_MASS_LH2;
	if (!ph_srb)  ph_srb = CreatePropellantResource(SRB_MAX_PROPELLANT_MASS*2.0); // SRB's

	CreateOrbiterTanks();

	// *********************** thruster definitions ********************************

	// The main and SRB thrusters are defined so as to minimise the angular momentum
	// when engaged at maximum thrust. Dynamic gimbaling is not required until the SRBs
	// start to lose power since the centre of gravity is assumed static.
	// However the resulting linear force vector has a component in +y ("up") direction

	ClearThrusterDefinitions();

	CreateSSMEs(OFS_ZERO);

	// SRBs
	th_srb[0] = CreateThruster(LSRB_OFFSET + _V(0.0, 0.0, -21.8), SRB_THRUST_DIR, SRB_THRUST, ph_srb, SRB_ISP0, SRB_ISP1);
	th_srb[1] = CreateThruster(RSRB_OFFSET + _V(0.0, 0.0, -21.8), SRB_THRUST_DIR, SRB_THRUST, ph_srb, SRB_ISP0, SRB_ISP1);
	thg_srb = CreateThrusterGroup(th_srb, 2, THGROUP_USER);
	SURFHANDLE tex = oapiRegisterExhaustTexture("Exhaust2");
	srb_exhaust.tex = oapiRegisterParticleTexture("SSU\\SRB_exhaust");
	srb_contrail.tex = oapiRegisterParticleTexture("SSU\\SRB_contrail");
	AddExhaustStream(th_srb[0], RSRB_OFFSET + _V(0, 0, -135), &srb_contrail);
	AddExhaustStream(th_srb[1], LSRB_OFFSET + _V(0, 0, -135), &srb_contrail);
	for (i = 0; i < 2; i++) AddExhaust(th_srb[i], 16.0, 2.0, tex);
	AddExhaustStream(th_srb[0], &srb_exhaust);
	AddExhaustStream(th_srb[1], &srb_exhaust);

	//Add slag effect streams
	for (i = 0; i < 2; i++) {
		pshSlag1[i] = AddExhaustStream(th_srb[i], &srb_slag1);
		pshSlag2[i] = AddExhaustStream(th_srb[i], &srb_slag2);
		pshSlag3[i] = AddExhaustStream(th_srb[i], &srb_slag3);
	}

	//OMS
	// DaveS edit: Fixed OMS position to line up with OMS nozzles on the scaled down orbiter mesh
	if (!bOMSDefined) {
		th_oms[0] = CreateThruster(L_OMS_REF, L_OMS_DIR, ORBITER_OMS_THRUST, ph_oms, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);
		th_oms[1] = CreateThruster(R_OMS_REF, R_OMS_DIR, ORBITER_OMS_THRUST, ph_oms, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);
		bOMSDefined = true;
		SURFHANDLE tex_oms = oapiRegisterExhaustTexture("SSU\\OMSExhaust");
		for (i = 0; i < 2; i++) {
			AddExhaust(th_oms[i], 0, 1.5, 1.25, tex_oms);
			OMSEngControl(i);
		}
	}

	// attitude
	CreateAttControls_RCS(OFS_ZERO);
	CreateDummyThrusters();

	// ************************* aerodynamics **************************************

	hStackAirfoil = CreateAirfoil2(LIFT_VERTICAL, _V(0, 0, 0), AscentLiftCoeff, ORBITER_CHORD_LENGTH, ORBITER_WING_AREA, ORBITER_WING_ASPECT_RATIO);
	ClearVariableDragElements();
	CreateVariableDragElement(&spdb_proc, 5, _V(0, 7.5, -14)); // speedbrake drag
	CreateVariableDragElement(&(gear_status.pos), 2, _V(0, -3, 0));      // landing gear drag
	CreateVariableDragElement(&rdoor_drag, 7, _V(2.9, 0, 10));   // right cargo door drag
	CreateVariableDragElement(&ldoor_drag, 7, _V(-2.9, 0, 10));  // right cargo door drag

	// ************************ visual parameters **********************************

	AddOrbiterVisual();

	phLOXdump = ph_mps;
	phLH2dump = ph_mps;
	CreateMPSDumpVents();// must be after the AddOrbiterVisual call as it uses orbiter_ofs and it not initialized before the 1º run, feel free to "fix" if needed

	status = STATE_PRELAUNCH;

}

// --------------------------------------------------------------
// Configuration after launch, before SRB separation
// --------------------------------------------------------------
void Atlantis::SetPostLaunchConfiguration(double met)
{
	SetLaunchConfiguration();
	t0 = -met; // reference time (liftoff)
	status = STATE_STAGE1;
}

// --------------------------------------------------------------
// Configuration after booster separation (orbiter + tank)
// --------------------------------------------------------------
void Atlantis::SetOrbiterTankConfiguration(void)
{
	// *********************** physical parameters *********************************

	SetSize(28.8);
	//SetEmptyMass (ORBITER_EMPTY_MASS + TANK_EMPTY_MASS + pl_mass);
	SetCW(0.2, 0.5, 1.5, 1.5);
	SetCrossSections(_V(303.91, 422.33, 153.51));
	SetRotDrag(_V(0.7, 0.1, 0.3));
	SetPMI(_V(131.82, 131.49, 59.28));
	SetTrimScale(0.05);
	SetLiftCoeffFunc(0); // simplification: we assume no lift during launch phase
	SetTouchdownPoints(_V(0, -5, 30), _V(-10, -10, -30), _V(10, 0, -30));

	// ************************* propellant specs **********************************

	if (!ph_mps) ph_mps = CreatePropellantResource(MPS_MANIFOLD_MASS_TOTAL);    // mps manifold
	LOXmass = MPS_MANIFOLD_MASS_LOX;
	LH2mass = MPS_MANIFOLD_MASS_LH2;

	CreateOrbiterTanks();

	// *********************** thruster definitions ********************************

	// Orbiter main engines
	CreateSSMEs(OFS_ZERO);

	phLOXdump = ph_mps;
	phLH2dump = ph_mps;
	CreateMPSDumpVents();

	// DaveS edit: Fixed OMS position to line up with OMS nozzles on the scaled down orbiter mesh
	if (!bOMSDefined) {
		th_oms[0] = CreateThruster(L_OMS_REF, L_OMS_DIR, ORBITER_OMS_THRUST, ph_oms, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);
		th_oms[1] = CreateThruster(R_OMS_REF, R_OMS_DIR, ORBITER_OMS_THRUST, ph_oms, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);
		bOMSDefined = true;
		SURFHANDLE tex_oms = oapiRegisterExhaustTexture("SSU\\OMSExhaust");
		for (int i = 0; i < 2; i++) {
			AddExhaust(th_oms[i], 0, 1.5, 1.25, tex_oms);
			OMSEngControl(i);
		}
	}

	CreateAttControls_RCS(OFS_ZERO);
	CreateDummyThrusters();

	// ************************* aerodynamics **************************************

	ClearAirfoilDefinitions();
	hStackAirfoil = NULL;
	ClearVariableDragElements();
	CreateVariableDragElement(&spdb_proc, 5, _V(0, 7.5, -14)); // speedbrake drag
	CreateVariableDragElement(&(gear_status.pos), 2, _V(0, -3, 0));      // landing gear drag
	CreateVariableDragElement(&rdoor_drag, 7, _V(2.9, 0, 10));   // right cargo door drag
	CreateVariableDragElement(&ldoor_drag, 7, _V(-2.9, 0, 10));  // right cargo door drag

	// ************************ visual parameters **********************************

	// status has to be updated before AddTankVisual is called to update ET texture
	status = STATE_STAGE2;

	AddOrbiterVisual();
}

// --------------------------------------------------------------
// Configuration after tank separation (orbiter only)
// --------------------------------------------------------------
void Atlantis::SetOrbiterConfiguration(void)
{
	int i;

	// *********************** physical parameters *********************************

	SetSize(19.6);
	SetPMI(_V(120.2, 108.8, 13.497));
	SetGravityGradientDamping(20.0);
	SetTrimScale(0.05);
	SetCW(ORBITER_CW[0], ORBITER_CW[1], ORBITER_CW[2], ORBITER_CW[3]);
	SetCrossSections(ORBITER_CS);
	DefineTouchdownPoints();
	SetMaxWheelbrakeForce(250000 / 2);

	// ************************* aerodynamics **************************************

	ClearAirfoilDefinitions();
	hStackAirfoil = NULL;
	SetRotDrag(_V(0.43, 0.43, 0.29)); // angular drag
	CreateAirfoil3(LIFT_VERTICAL, _V(0.0, 0.0, 0.0), VLiftCoeff, &aerosurfaces, ORBITER_CHORD_LENGTH, ORBITER_WING_AREA, ORBITER_WING_ASPECT_RATIO);
	CreateAirfoil3(LIFT_HORIZONTAL, _V(0.0, 0.0, 0.0), HLiftCoeff, &aerosurfaces, 20, 50, 1.5);

	/*helevator = CreateControlSurface2 (AIRCTRL_ELEVATOR, 5, 1.5, _V( 0, 0,  -15), AIRCTRL_AXIS_XPOS, anim_elev);
	hbodyflap = CreateControlSurface2 (AIRCTRL_ELEVATORTRIM, 5, 1.75, _V( 0, 0,  -17), AIRCTRL_AXIS_XPOS, anim_bf);
	hrudder = CreateControlSurface2 (AIRCTRL_RUDDER,   2, 1.5, _V( 0, 3,  -16), AIRCTRL_AXIS_YPOS, anim_rudder);
	hraileron = CreateControlSurface2 (AIRCTRL_AILERON,  3, 1.5, _V( 7,-0.5,-15), AIRCTRL_AXIS_XPOS, anim_raileron);
	hlaileron = CreateControlSurface2 (AIRCTRL_AILERON,  3, 1.5, _V(-7,-0.5,-15), AIRCTRL_AXIS_XNEG, anim_laileron);
	ControlSurfacesEnabled=true;*/

	ClearVariableDragElements();
	//CreateVariableDragElement (&spdb_proc, 9, _V(0, 7.5, -14)); // speedbrake drag
	CreateVariableDragElement(&(gear_status.pos), 2, _V(0, -3, 0));      // landing gear drag
	CreateVariableDragElement(&rdoor_drag, 7, _V(2.9, 0, 10));   // right cargo door drag
	CreateVariableDragElement(&ldoor_drag, 7, _V(-2.9, 0, 10));  // right cargo door drag
	CreateVariableDragElement(&DragChuteSize, 22.5, _V(0, 4.6, -12.03));

	SetADCtrlMode(7);

	// ************************* particle streams **********************************

	PARTICLESTREAMSPEC rps = {
		0, 20, 20, 0, 0.03, 0.5, 100, 3, PARTICLESTREAMSPEC::DIFFUSE,
		PARTICLESTREAMSPEC::LVL_FLAT, 1, 1, PARTICLESTREAMSPEC::ATM_PLIN, 6e7, 12e7
	};
	AddReentryStream(&rps);

	// ************************* propellant specs **********************************

	CreateOrbiterTanks();

	// *********************** thruster definitions ********************************
	// OMS (Orbital Manouevering System)
	// DaveS edit: Fixed OMS position to line up with OMS nozzles on the scaled down orbiter mesh
	//VECTOR3 OMS_POS=_V(0,3.55,-13.04);
	if (!bOMSDefined) {
		//th_oms[0] = CreateThruster (OMS_POS-_V(2.313,0,0), -OMS_POS/length(OMS_POS), ORBITER_OMS_THRUST, ph_oms, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);
		//th_oms[1] = CreateThruster (OMS_POS+_V(2.313,0,0), -OMS_POS/length(OMS_POS), ORBITER_OMS_THRUST, ph_oms, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);
		th_oms[0] = CreateThruster(L_OMS_REF, L_OMS_DIR, ORBITER_OMS_THRUST, ph_oms, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);
		th_oms[1] = CreateThruster(R_OMS_REF, R_OMS_DIR, ORBITER_OMS_THRUST, ph_oms, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);
		bOMSDefined = true;
		//thg_main = CreateThrusterGroup (th_oms, 2, THGROUP_MAIN);
		SURFHANDLE tex_oms = oapiRegisterExhaustTexture("SSU\\OMSExhaust");
		for (i = 0; i < 2; i++) {
			AddExhaust(th_oms[i], 0, 1.5, 1.25, tex_oms);
			OMSEngControl(i);
		}
	}

	CreateMPSGOXVents(_V(0.0, 0.0, 0.0));

	// RCS (Reaction Control System)
	CreateAttControls_RCS(orbiter_ofs);
	CreateDummyThrusters();

	// ************************ visual parameters **********************************
	AddOrbiterVisual();

	status = 3;
}


void assertValidHandles(const char* context, THRUSTER_HANDLE *thx, int count)
{
	static char buf[200];
	for (int i = 0; i < count; i++) {
		if (thx[i] == NULL) {
			sprintf_s(buf, 200, "[SpaceShuttleUltra] ERROR %s : Thruster handle is NULL (at index %d)", context, i);
			oapiWriteLog(buf);
		}
	}
}

// --------------------------------------------------------------
// Attitude controls (RCS) during orbital phase
// --------------------------------------------------------------
void Atlantis::CreateAttControls_RCS(VECTOR3 center) {
	//if(bRCSDefined) return;

	THRUSTER_HANDLE thTmp[16];

	// delete existing exhaust definitions and update positions
	// we only need to define the thruster positions once; after that ShiftCG should set up positions
	while (!vExRCS.empty()) {
		DelExhaust(vExRCS.back());
		vExRCS.pop_back();
	}
	while (!vExStreamRCS.empty()) {
		DelExhaustStream(vExStreamRCS.back());
		vExStreamRCS.pop_back();
	}

	if (bUseRealRCS)
	{
		CreateFRCS(center);
		CreateLeftARCS(center);
		CreateRightARCS(center);

		/* RCS GROUPS:
		G-1 : F2F, F3F, F1F : -x
		G-2 : F1L, F3L : + yaw, +y
		G-3 : F2R, F4R : - yaw, -y
		G-4 : F2U, F3U, F1U : +z
		G-5 : F5R, F2D, F4D : -roll, +pitch, -z
		G-6 : F5L, F1D, F3D : +roll, +pitch, -z
		G-7 : R3A, R1A : +x
		G-8 : L1A, L3A : +x
		G-9 : L4L, L2L, L3L, L1L : -yaw, +y
		G-10: R1R, R3R, R2R, R4R : +yaw, -y
		G-11: L1U, L2U, L4U : -roll, +pitch, +z
		G-12: R1U, R2U, R4U : +roll, +pitch, +z
		G-13: L3D, L2D, L4D : +roll, -pitch, -z
		G-14: R3D, R2D, R4D : -roll, -pitch, -z

		+Pitch : G-5, G-6, G-11, G-12
		-Pitch : G-13, G14
		-yaw : G-3, G-9

		*/



		thTmp[0] = thFRCS[RCS_F2D];
		thTmp[1] = thFRCS[RCS_F4D];
		thTmp[2] = thFRCS[RCS_F1D];
		thTmp[3] = thFRCS[RCS_F3D];
		thTmp[4] = thLRCS[RCS_L1U];
		thTmp[5] = thLRCS[RCS_L2U];
		thTmp[6] = thLRCS[RCS_L4U];
		thTmp[7] = thRRCS[RCS_R1U];
		thTmp[8] = thRRCS[RCS_R2U];
		thTmp[9] = thRRCS[RCS_R4U];

		assertValidHandles("Create pitch up group", thTmp, 10);
		thg_pitchup = CreateThrusterGroup(thTmp, 10, THGROUP_USER);

		thTmp[0] = thLRCS[RCS_L2D];
		thTmp[1] = thLRCS[RCS_L3D];
		thTmp[2] = thLRCS[RCS_L4D];
		thTmp[3] = thRRCS[RCS_R2D];
		thTmp[4] = thRRCS[RCS_R3D];
		thTmp[5] = thRRCS[RCS_R4D];
		thTmp[6] = thFRCS[RCS_F2U];
		thTmp[7] = thFRCS[RCS_F3U];
		thTmp[8] = thFRCS[RCS_F1U];

		assertValidHandles("Create pitch down group", thTmp, 9);
		thg_pitchdown = CreateThrusterGroup(thTmp, 9, THGROUP_USER);

		thTmp[0] = thLRCS[RCS_L1L];
		thTmp[1] = thLRCS[RCS_L2L];
		thTmp[2] = thLRCS[RCS_L3L];
		thTmp[3] = thLRCS[RCS_L4L];
		thTmp[4] = thFRCS[RCS_F2R];
		thTmp[5] = thFRCS[RCS_F4R];

		assertValidHandles("Create yaw left group", thTmp, 6);
		thg_yawleft = CreateThrusterGroup(thTmp, 6, THGROUP_USER);

		thTmp[0] = thRRCS[RCS_R1R];
		thTmp[1] = thRRCS[RCS_R2R];
		thTmp[2] = thRRCS[RCS_R3R];
		thTmp[3] = thRRCS[RCS_R4R];
		thTmp[4] = thFRCS[RCS_F1L];
		thTmp[5] = thFRCS[RCS_F3L];

		assertValidHandles("Create yaw right group", thTmp, 6);
		thg_yawright = CreateThrusterGroup(thTmp, 6, THGROUP_USER);

		//G-11: L1U, L2U, L4U : -roll, +pitch, +z
		thTmp[0] = thLRCS[RCS_L1U];
		thTmp[1] = thLRCS[RCS_L2U];
		thTmp[2] = thLRCS[RCS_L4U];
		thTmp[3] = thRRCS[RCS_R2D];
		thTmp[4] = thRRCS[RCS_R3D];
		thTmp[5] = thRRCS[RCS_R4D];
		thTmp[6] = thFRCS[RCS_F2D];
		thTmp[7] = thFRCS[RCS_F4D];

		assertValidHandles("Create bank left group", thTmp, 8);
		thg_rollleft = CreateThrusterGroup(thTmp, 8, THGROUP_USER);

		thTmp[0] = thRRCS[RCS_R1U];
		thTmp[1] = thRRCS[RCS_R2U];
		thTmp[2] = thRRCS[RCS_R4U];
		thTmp[3] = thLRCS[RCS_L2D];
		thTmp[4] = thLRCS[RCS_L3D];
		thTmp[5] = thLRCS[RCS_L4D];
		thTmp[6] = thFRCS[RCS_F3D];
		thTmp[7] = thFRCS[RCS_F1D];

		assertValidHandles("Create bank right group", thTmp, 8);
		thg_rollright = CreateThrusterGroup(thTmp, 8, THGROUP_USER);

		thTmp[0] = thRRCS[RCS_R1A];
		thTmp[1] = thRRCS[RCS_R3A];
		thTmp[2] = thLRCS[RCS_L1A];
		thTmp[3] = thLRCS[RCS_L3A];

		assertValidHandles("Create translate +X group", thTmp, 4);
		thg_transfwd = CreateThrusterGroup(thTmp, 4, THGROUP_USER);

		thTmp[0] = thFRCS[RCS_F1F];
		thTmp[1] = thFRCS[RCS_F3F];
		thTmp[2] = thFRCS[RCS_F2F];


		assertValidHandles("Create translate -X group", thTmp, 3);
		thg_transaft = CreateThrusterGroup(thTmp, 3, THGROUP_USER);


		thTmp[0] = thLRCS[RCS_L1L];
		thTmp[1] = thLRCS[RCS_L2L];
		thTmp[2] = thLRCS[RCS_L3L];
		thTmp[3] = thLRCS[RCS_L4L];
		thTmp[4] = thFRCS[RCS_F1L];
		thTmp[5] = thFRCS[RCS_F3L];

		assertValidHandles("Create translate +Y group", thTmp, 6);
		thg_transright = CreateThrusterGroup(thTmp, 6, THGROUP_USER);

		thTmp[0] = thRRCS[RCS_R1R];
		thTmp[1] = thRRCS[RCS_R2R];
		thTmp[2] = thRRCS[RCS_R3R];
		thTmp[3] = thRRCS[RCS_R4R];
		thTmp[4] = thFRCS[RCS_F2R];
		thTmp[5] = thFRCS[RCS_F4R];

		assertValidHandles("Create translate -Y group", thTmp, 6);
		thg_transleft = CreateThrusterGroup(thTmp, 6, THGROUP_USER);

		thTmp[0] = thRRCS[RCS_R1U];
		thTmp[1] = thRRCS[RCS_R2U];
		thTmp[2] = thRRCS[RCS_R4U];
		thTmp[3] = thLRCS[RCS_L1U];
		thTmp[4] = thLRCS[RCS_L2U];
		thTmp[5] = thLRCS[RCS_L4U];
		thTmp[6] = thFRCS[RCS_F1U];
		thTmp[7] = thFRCS[RCS_F2U];
		thTmp[8] = thFRCS[RCS_F3U];

		assertValidHandles("Create translate +Z group", thTmp, 9);
		thg_transdown = CreateThrusterGroup(thTmp, 9, THGROUP_USER);

		thTmp[0] = thRRCS[RCS_R3D];
		thTmp[1] = thRRCS[RCS_R2D];
		thTmp[2] = thRRCS[RCS_R4D];
		thTmp[3] = thLRCS[RCS_L3D];
		thTmp[4] = thLRCS[RCS_L2D];
		thTmp[5] = thLRCS[RCS_L4D];
		thTmp[6] = thFRCS[RCS_F1D];
		thTmp[7] = thFRCS[RCS_F2D];
		thTmp[8] = thFRCS[RCS_F3D];
		thTmp[9] = thFRCS[RCS_F4D];

		assertValidHandles("Create translate -Z group", thTmp, 10);
		thg_transup = CreateThrusterGroup(thTmp, 10, THGROUP_USER);

		bRCSDefined = true;

		return;
	}

	// set of attitude thrusters (idealised). The arrangement is such that no angular
	// momentum is created in linear mode, and no linear momentum is created in rotational mode.
	if (!bRCSDefined) {
		th_att_rcs[0] = CreateThruster(_V(0, 0, 15.5), _V(0, 1, 0), ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
		th_att_rcs[1] = CreateThruster(_V(0, 0, -15.5), _V(0, -1, 0), ORBITER_RCS_THRUST, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
		th_att_rcs[2] = CreateThruster(_V(0, 0, 15.5), _V(0, -1, 0), ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
		th_att_rcs[3] = CreateThruster(_V(0, 0, -15.5), _V(0, 1, 0), ORBITER_RCS_THRUST, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);

		th_att_lin[0] = CreateThruster(_V(0, 0, 15.5), _V(0, 1, 0), ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
		th_att_lin[3] = CreateThruster(_V(0, 0, -15.5), _V(0, -1, 0), ORBITER_RCS_THRUST, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
		th_att_lin[2] = CreateThruster(_V(0, 0, 15.5), _V(0, -1, 0), ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
		th_att_lin[1] = CreateThruster(_V(0, 0, -15.5), _V(0, 1, 0), ORBITER_RCS_THRUST, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
		thg_transup = CreateThrusterGroup(th_att_lin, 2, THGROUP_USER);
		thg_transdown = CreateThrusterGroup(th_att_lin + 2, 2, THGROUP_USER);
	}
	else { // update thruster positions
		SetThrusterRef(th_att_rcs[0], _V(0, 0, 15.5));
		SetThrusterRef(th_att_rcs[1], _V(0, 0, -15.5));
		SetThrusterRef(th_att_rcs[2], _V(0, 0, 15.5));
		SetThrusterRef(th_att_rcs[3], _V(0, 0, -15.5));

		SetThrusterRef(th_att_lin[0], _V(0, 0, 15.5));
		SetThrusterRef(th_att_lin[3], _V(0, 0, -15.5));
		SetThrusterRef(th_att_lin[2], _V(0, 0, 15.5));
		SetThrusterRef(th_att_lin[1], _V(0, 0, -15.5));
	}

	AddRCSExhaust(th_att_rcs[0], center + _V(1.561, -1.249, 15.992), _V(0.4339, -0.8830, -0.1793));//F2D
	AddRCSExhaust(th_att_rcs[0], center + _V(1.647, -1.228, 15.647), _V(0.4339, -0.8830, -0.1793));//F4D
	AddRCSExhaust(th_att_rcs[0], center + _V(-1.561, -1.249, 15.992), _V(-0.4339, -0.8830, -0.1793));//F1D
	AddRCSExhaust(th_att_rcs[0], center + _V(-1.647, -1.228, 15.647), _V(-0.4339, -0.8830, -0.1793));//F3D

	AddRCSExhaust(th_att_rcs[1], center + _V(-3.395, 1.893, -14.167), _V(0, 1, 0));//L4U
	AddRCSExhaust(th_att_rcs[1], center + _V(-3.395, 1.893, -14.487), _V(0, 1, 0));//L2U
	AddRCSExhaust(th_att_rcs[1], center + _V(-3.395, 1.893, -14.806), _V(0, 1, 0));//L1U

	AddRCSExhaust(th_att_rcs[1], center + _V(3.395, 1.893, -14.167), _V(0, 1, 0));//R4U
	AddRCSExhaust(th_att_rcs[1], center + _V(3.395, 1.893, -14.487), _V(0, 1, 0));//R2U
	AddRCSExhaust(th_att_rcs[1], center + _V(3.395, 1.893, -14.806), _V(0, 1, 0));//R1U

	AddRCSExhaust(th_att_rcs[2], center + _V(-0.367, -0.006, 15.51), _V(0, 1, 0));//F1U
	AddRCSExhaust(th_att_rcs[2], center + _V(0.0, -0.006, 15.51), _V(0, 1, 0));//F3U
	AddRCSExhaust(th_att_rcs[2], center + _V(0.367, -0.006, 15.51), _V(0, 1, 0));//F2U

	AddRCSExhaust(th_att_rcs[3], center + _V(-3.112, 0.236, -14.244), _V(-0.2844, -0.9481, -0.1422));//L4D
	AddRCSExhaust(th_att_rcs[3], center + _V(-3.097, 0.303, -14.573), _V(-0.2844, -0.9481, -0.1422));//L2D
	AddRCSExhaust(th_att_rcs[3], center + _V(-3.081, 0.369, -14.909), _V(-0.2844, -0.9481, -0.1422));//L3D

	AddRCSExhaust(th_att_rcs[3], center + _V(2.839, 0.236, -14.244), _V(0.2844, -0.9481, -0.1422));//R4D
	AddRCSExhaust(th_att_rcs[3], center + _V(2.839, 0.303, -14.573), _V(0.2844, -0.9481, -0.1422));//R2D
	AddRCSExhaust(th_att_rcs[3], center + _V(2.839, 0.369, -14.909), _V(0.2844, -0.9481, -0.1422));//R3D

	AddRCSExhaust(th_att_lin[0], center + _V(1.561, -1.249, 15.992), _V(0.4339, -0.8830, -0.1793));//F2D
	AddRCSExhaust(th_att_lin[0], center + _V(1.647, -1.228, 15.647), _V(0.4339, -0.8830, -0.1793));//F4D
	AddRCSExhaust(th_att_lin[0], center + _V(-1.561, -1.249, 15.992), _V(-0.4339, -0.8830, -0.1793));//F1D
	AddRCSExhaust(th_att_lin[0], center + _V(-1.647, -1.228, 15.647), _V(-0.4339, -0.8830, -0.1793));//F3D

	AddRCSExhaust(th_att_lin[3], center + _V(-3.395, 1.893, -14.167), _V(0, 1, 0));//L4U
	AddRCSExhaust(th_att_lin[3], center + _V(-3.395, 1.893, -14.487), _V(0, 1, 0));//L2U
	AddRCSExhaust(th_att_lin[3], center + _V(-3.395, 1.893, -14.806), _V(0, 1, 0));//L1U

	AddRCSExhaust(th_att_lin[3], center + _V(3.395, 1.893, -14.167), _V(0, 1, 0));//R4U
	AddRCSExhaust(th_att_lin[3], center + _V(3.395, 1.893, -14.487), _V(0, 1, 0));//R2U
	AddRCSExhaust(th_att_lin[3], center + _V(3.395, 1.893, -14.806), _V(0, 1, 0));//R1U

	AddRCSExhaust(th_att_lin[2], center + _V(-0.367, -0.006, 15.51), _V(0, 1, 0));//F1U
	AddRCSExhaust(th_att_lin[2], center + _V(0.0, -0.006, 15.51), _V(0, 1, 0));//F3U
	AddRCSExhaust(th_att_lin[2], center + _V(0.367, -0.006, 15.51), _V(0, 1, 0));//F2U

	AddRCSExhaust(th_att_lin[1], center + _V(-2.839, 0.236, -14.244), _V(-0.2844, -0.9481, -0.1422));//L4D
	AddRCSExhaust(th_att_lin[1], center + _V(-2.839, 0.303, -14.573), _V(-0.2844, -0.9481, -0.1422));//L2D
	AddRCSExhaust(th_att_lin[1], center + _V(-2.839, 0.369, -14.909), _V(-0.2844, -0.9481, -0.1422));//L3D

	AddRCSExhaust(th_att_lin[1], center + _V(2.839, 0.236, -14.244), _V(0.2844, -0.9481, -0.1422));//R4D
	AddRCSExhaust(th_att_lin[1], center + _V(2.839, 0.303, -14.573), _V(0.2844, -0.9481, -0.1422));//R2D
	AddRCSExhaust(th_att_lin[1], center + _V(2.839, 0.369, -14.909), _V(0.2844, -0.9481, -0.1422));//R3D

	if (!bRCSDefined) {
		th_att_rcs[4] = CreateThruster(_V(0, 0, 15.5), _V(-1, 0, 0), ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
		th_att_rcs[5] = CreateThruster(_V(0, 0, -15.5), _V(1, 0, 0), ORBITER_RCS_THRUST, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
		th_att_rcs[6] = CreateThruster(_V(0, 0, 15.5), _V(1, 0, 0), ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
		th_att_rcs[7] = CreateThruster(_V(0, 0, -15.5), _V(-1, 0, 0), ORBITER_RCS_THRUST, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);

		th_att_lin[4] = CreateThruster(_V(0, 0, 15.5), _V(-1, 0, 0), ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
		th_att_lin[7] = CreateThruster(_V(0, 0, -15.5), _V(1, 0, 0), ORBITER_RCS_THRUST, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
		th_att_lin[6] = CreateThruster(_V(0, 0, 15.5), _V(1, 0, 0), ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
		th_att_lin[5] = CreateThruster(_V(0, 0, -15.5), _V(-1, 0, 0), ORBITER_RCS_THRUST, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
		thg_transleft = CreateThrusterGroup(th_att_lin + 4, 2, THGROUP_USER);
		thg_transright = CreateThrusterGroup(th_att_lin + 6, 2, THGROUP_USER);
	}
	else { // update thruster positions
		SetThrusterRef(th_att_rcs[4], _V(0, 0, 15.5));
		SetThrusterRef(th_att_rcs[5], _V(0, 0, -15.5));
		SetThrusterRef(th_att_rcs[6], _V(0, 0, 15.5));
		SetThrusterRef(th_att_rcs[7], _V(0, 0, -15.5));

		SetThrusterRef(th_att_lin[4], _V(0, 0, 15.5));
		SetThrusterRef(th_att_lin[7], _V(0, 0, -15.5));
		SetThrusterRef(th_att_lin[6], _V(0, 0, 15.5));
		SetThrusterRef(th_att_lin[5], _V(0, 0, -15.5));
	}

	AddRCSExhaust(th_att_rcs[4], center + _V(1.733, -1.084, 15.266), _V(1, 0, 0));//F2R
	AddRCSExhaust(th_att_rcs[4], center + _V(1.740, -1.451, 15.222), _V(1, 0, 0));//F4R
	AddRCSExhaust(th_att_rcs[6], center + _V(-1.733, -1.084, 15.266), _V(-1, 0, 0));//F1L
	AddRCSExhaust(th_att_rcs[6], center + _V(-1.740, -1.451, 15.222), _V(-1, 0, 0));//F3L

	AddRCSExhaust(th_att_rcs[5], center + _V(-3.688, 1.051, -14.254), _V(-1, 0, 0));//L4L
	AddRCSExhaust(th_att_rcs[5], center + _V(-3.698, 1.051, -14.563), _V(-1, 0, 0));//L2L
	AddRCSExhaust(th_att_rcs[5], center + _V(-3.704, 1.051, -14.876), _V(-1, 0, 0));//L3L
	AddRCSExhaust(th_att_rcs[5], center + _V(-3.711, 1.051, -15.179), _V(-1, 0, 0));//L1L

	AddRCSExhaust(th_att_rcs[7], center + _V(3.688, 1.051, -14.254), _V(1, 0, 0));//R4R
	AddRCSExhaust(th_att_rcs[7], center + _V(3.698, 1.051, -14.563), _V(1, 0, 0));//R2R
	AddRCSExhaust(th_att_rcs[7], center + _V(3.704, 1.051, -14.876), _V(1, 0, 0));//R3R
	AddRCSExhaust(th_att_rcs[7], center + _V(3.711, 1.051, -15.179), _V(1, 0, 0));//R1R

	AddRCSExhaust(th_att_lin[4], center + _V(1.733, -1.084, 15.266), _V(1, 0, 0));//F2R
	AddRCSExhaust(th_att_lin[4], center + _V(1.740, -1.451, 15.222), _V(1, 0, 0));//F4R
	AddRCSExhaust(th_att_lin[6], center + _V(-1.733, -1.084, 15.266), _V(-1, 0, 0));//F1L
	AddRCSExhaust(th_att_lin[6], center + _V(-1.740, -1.451, 15.222), _V(-1, 0, 0));//F3L

	AddRCSExhaust(th_att_lin[7], center + _V(-3.688, 1.051, -14.254), _V(-1, 0, 0));//L4L
	AddRCSExhaust(th_att_lin[7], center + _V(-3.698, 1.051, -14.563), _V(-1, 0, 0));//L2L
	AddRCSExhaust(th_att_lin[7], center + _V(-3.704, 1.051, -14.876), _V(-1, 0, 0));//L3L
	AddRCSExhaust(th_att_lin[7], center + _V(-3.711, 1.051, -15.179), _V(-1, 0, 0));//L1L

	AddRCSExhaust(th_att_lin[5], center + _V(3.688, 1.051, -14.254), _V(1, 0, 0));//R4R
	AddRCSExhaust(th_att_lin[5], center + _V(3.698, 1.051, -14.563), _V(1, 0, 0));//R2R
	AddRCSExhaust(th_att_lin[5], center + _V(3.704, 1.051, -14.876), _V(1, 0, 0));//R3R
	AddRCSExhaust(th_att_lin[5], center + _V(3.711, 1.051, -15.179), _V(1, 0, 0));//R1R

	if (!bRCSDefined) {
		th_att_rcs[8] = CreateThruster(_V(2.7, 0, 0), _V(0, 1, 0), ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
		th_att_rcs[9] = CreateThruster(_V(-2.7, 0, 0), _V(0, -1, 0), ORBITER_RCS_THRUST, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
		th_att_rcs[10] = CreateThruster(_V(-2.7, 0, 0), _V(0, 1, 0), ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
		th_att_rcs[11] = CreateThruster(_V(2.7, 0, 0), _V(0, -1, 0), ORBITER_RCS_THRUST, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
		//The virtual thrusters
		th_att_rcs[12] = CreateThruster(_V(0, 0, 0), _V(0, -1, 0), 0, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
		th_att_rcs[13] = CreateThruster(_V(0, 0, 0), _V(0, -1, 0), 0, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
		th_att_rcs[14] = CreateThruster(_V(0, 0, 0), _V(0, -1, 0), 0, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
		th_att_rcs[15] = CreateThruster(_V(0, 0, 0), _V(0, -1, 0), 0, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
		th_att_rcs[16] = CreateThruster(_V(0, 0, 0), _V(0, -1, 0), 0, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
		th_att_rcs[17] = CreateThruster(_V(0, 0, 0), _V(0, -1, 0), 0, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	}
	else { // update thruster positions
		SetThrusterRef(th_att_rcs[8], _V(2.7, 0, 0));
		SetThrusterRef(th_att_rcs[9], _V(-2.7, 0, 0));
		SetThrusterRef(th_att_rcs[10], _V(-2.7, 0, 0));
		SetThrusterRef(th_att_rcs[11], _V(2.7, 0, 0));
	}

	AddRCSExhaust(th_att_rcs[8], center + _V(1.561, -1.249, 15.992), _V(0.4339, -0.8830, -0.1793));//F2D
	AddRCSExhaust(th_att_rcs[8], center + _V(1.647, -1.228, 15.647), _V(0.4339, -0.8830, -0.1793));//F4D
	AddRCSExhaust(th_att_rcs[10], center + _V(-1.561, -1.249, 15.992), _V(-0.4339, -0.8830, -0.1793));//F1D
	AddRCSExhaust(th_att_rcs[10], center + _V(-1.647, -1.228, 15.647), _V(-0.4339, -0.8830, -0.1793));//F3D

	AddRCSExhaust(th_att_rcs[9], center + _V(-3.395, 1.893, -14.167), _V(0, 1, 0));//L4U
	AddRCSExhaust(th_att_rcs[9], center + _V(-3.395, 1.893, -14.487), _V(0, 1, 0));//L2U
	AddRCSExhaust(th_att_rcs[9], center + _V(-3.395, 1.893, -14.806), _V(0, 1, 0));//L1U

	AddRCSExhaust(th_att_rcs[11], center + _V(3.395, 1.893, -14.167), _V(0, 1, 0));//R4U
	AddRCSExhaust(th_att_rcs[11], center + _V(3.395, 1.893, -14.487), _V(0, 1, 0));//R2U
	AddRCSExhaust(th_att_rcs[11], center + _V(3.395, 1.893, -14.806), _V(0, 1, 0));//R1U

	AddRCSExhaust(th_att_rcs[10], center + _V(-2.839, 0.236, -14.244), _V(-0.2844, -0.9481, -0.1422));//L4D
	AddRCSExhaust(th_att_rcs[10], center + _V(-2.839, 0.303, -14.573), _V(-0.2844, -0.9481, -0.1422));//L2D
	AddRCSExhaust(th_att_rcs[10], center + _V(-2.839, 0.369, -14.909), _V(-0.2844, -0.9481, -0.1422));//L3D

	AddRCSExhaust(th_att_rcs[8], center + _V(2.839, 0.236, -14.244), _V(0.2844, -0.9481, -0.1422));//R4D
	AddRCSExhaust(th_att_rcs[8], center + _V(2.839, 0.303, -14.573), _V(0.2844, -0.9481, -0.1422));//R2D
	AddRCSExhaust(th_att_rcs[8], center + _V(2.839, 0.369, -14.909), _V(0.2844, -0.9481, -0.1422));//R3D

	if (!bRCSDefined) {
		th_att_lin[8] = CreateThruster(_V(0, 0, -16), _V(0, 0, 1), ORBITER_RCS_THRUST, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
		th_att_lin[9] = CreateThruster(_V(0, 0, 16), _V(0, 0, -1), ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
		thg_transfwd = CreateThrusterGroup(th_att_lin + 8, 1, THGROUP_USER);
		thg_transaft = CreateThrusterGroup(th_att_lin + 9, 1, THGROUP_USER);
	}
	else { // update thruster positions
		SetThrusterRef(th_att_lin[8], _V(0, 0, -16));
		SetThrusterRef(th_att_lin[9], _V(0, 0, 16));
	}

	AddRCSExhaust(th_att_lin[8], center + _V(-3.212, 1.501, -15.475), _V(0, 0, -1));//L1A
	AddRCSExhaust(th_att_lin[8], center + _V(-3.505, 1.501, -15.475), _V(0, 0, -1));//L3A
	AddRCSExhaust(th_att_lin[8], center + _V(3.212, 1.501, -15.475), _V(0, 0, -1));//R1A
	AddRCSExhaust(th_att_lin[8], center + _V(3.505, 1.501, -15.475), _V(0, 0, -1));//R3A

	AddRCSExhaust(th_att_lin[9], center + _V(0.0, -0.487, 16.286), _V(0, 0.0499, 0.9988));//F3F
	AddRCSExhaust(th_att_lin[9], center + _V(-0.446, -0.487, 16.286), _V(0, 0.0499, 0.9988));//F1F
	AddRCSExhaust(th_att_lin[9], center + _V(0.446, -0.487, 16.286), _V(0, 0.0499, 0.9988));//F2F

	if (!bRCSDefined) {
		thg_pitchup = CreateThrusterGroup(th_att_rcs, 2, THGROUP_USER);
		thg_pitchdown = CreateThrusterGroup(th_att_rcs + 2, 2, THGROUP_USER);
		thg_yawleft = CreateThrusterGroup(th_att_rcs + 4, 2, THGROUP_USER);
		thg_yawright = CreateThrusterGroup(th_att_rcs + 6, 2, THGROUP_USER);
		thg_rollleft = CreateThrusterGroup(th_att_rcs + 8, 2, THGROUP_USER);
		thg_rollright = CreateThrusterGroup(th_att_rcs + 10, 2, THGROUP_USER);
	}

	UpdateTranslationForces();

	bRCSDefined = true;
}

void Atlantis::AddRCSExhaust(THRUSTER_HANDLE thX, const VECTOR3& pos, const VECTOR3& dir)
{
	const double eh = 12.0;             // exhaust length scale
	const double ew1 = 0.8; // exhaust width scales
	vExRCS.push_back(AddExhaust(thX, eh, ew1, pos, dir, RCS_Exhaust_tex));
	vExStreamRCS.push_back(AddExhaustStream(thX, pos, &RCS_PSSpec));
}

void Atlantis::AddVRCSExhaust(THRUSTER_HANDLE thX, const VECTOR3& pos, const VECTOR3& dir)
{
	const double eh = 0.5754;             // exhaust length scale
	const double ew1 = 0.1; // exhaust width scales
	vExRCS.push_back(AddExhaust(thX, eh, ew1, pos, dir, RCS_Exhaust_tex));
	vExStreamRCS.push_back(AddExhaustStream(thX, pos, &RCS_PSSpec));
}

void Atlantis::CreateDummyThrusters()
{
	if (bControllerThrustersDefined) return;
	THRUSTER_HANDLE thTmp;
	//rotation
	thTmp = CreateThruster(_V(0, 0, 0), _V(1, 0, 0), 0, ph_controller);
	CreateThrusterGroup(&thTmp, 1, THGROUP_ATT_PITCHUP);
	thTmp = CreateThruster(_V(0, 0, 0), _V(1, 0, 0), 0, ph_controller);
	CreateThrusterGroup(&thTmp, 1, THGROUP_ATT_PITCHDOWN);
	thTmp = CreateThruster(_V(0, 0, 0), _V(1, 0, 0), 0, ph_controller);
	CreateThrusterGroup(&thTmp, 1, THGROUP_ATT_YAWLEFT);
	thTmp = CreateThruster(_V(0, 0, 0), _V(1, 0, 0), 0, ph_controller);
	CreateThrusterGroup(&thTmp, 1, THGROUP_ATT_YAWRIGHT);
	thTmp = CreateThruster(_V(0, 0, 0), _V(1, 0, 0), 0, ph_controller);
	CreateThrusterGroup(&thTmp, 1, THGROUP_ATT_BANKLEFT);
	thTmp = CreateThruster(_V(0, 0, 0), _V(1, 0, 0), 0, ph_controller);
	CreateThrusterGroup(&thTmp, 1, THGROUP_ATT_BANKRIGHT);
	//translation
	thTmp = CreateThruster(_V(0, 0, 0), _V(1, 0, 0), 0, ph_controller);
	CreateThrusterGroup(&thTmp, 1, THGROUP_ATT_FORWARD);
	thTmp = CreateThruster(_V(0, 0, 0), _V(1, 0, 0), 0, ph_controller);
	CreateThrusterGroup(&thTmp, 1, THGROUP_ATT_BACK);
	thTmp = CreateThruster(_V(0, 0, 0), _V(1, 0, 0), 0, ph_controller);
	CreateThrusterGroup(&thTmp, 1, THGROUP_ATT_UP);
	thTmp = CreateThruster(_V(0, 0, 0), _V(1, 0, 0), 0, ph_controller);
	CreateThrusterGroup(&thTmp, 1, THGROUP_ATT_DOWN);
	thTmp = CreateThruster(_V(0, 0, 0), _V(1, 0, 0), 0, ph_controller);
	CreateThrusterGroup(&thTmp, 1, THGROUP_ATT_LEFT);
	thTmp = CreateThruster(_V(0, 0, 0), _V(1, 0, 0), 0, ph_controller);
	CreateThrusterGroup(&thTmp, 1, THGROUP_ATT_RIGHT);
	// SPDBK/THROT
	thTmp = CreateThruster(_V(0, 0, 0), _V(1, 0, 0), 0, ph_controller);
	CreateThrusterGroup(&thTmp, 1, THGROUP_MAIN);

	bControllerThrustersDefined = true;
}

void Atlantis::DisableControlSurfaces()
{
	if (!ControlSurfacesEnabled) return;
	ClearControlSurfaceDefinitions();
	hrudder = NULL;
	hbodyflap = NULL;
	helevator = NULL;
	hlaileron = NULL;
	hraileron = NULL;
	ControlSurfacesEnabled = false;
}

void Atlantis::EnableControlSurfaces()
{
	if (ControlSurfacesEnabled) return;
	helevator = CreateControlSurface3(AIRCTRL_ELEVATOR, 0.0, 0.0, _V(0, 0, -15), AIRCTRL_AXIS_XPOS, 5.0);
	hbodyflap = CreateControlSurface2(AIRCTRL_ELEVATORTRIM, 0.0, 0.0, _V(0, 0, -17), AIRCTRL_AXIS_XPOS, anim_bf);
	//hrudder = CreateControlSurface2 (AIRCTRL_RUDDER,   2, 1.5, _V( 0, 3,  -16), AIRCTRL_AXIS_YPOS, anim_rudder);
	hrudder = CreateControlSurface2(AIRCTRL_RUDDERTRIM, 2, 1.5, _V(0, 3, -16), AIRCTRL_AXIS_YPOS, anim_rudder);
	//hraileron = CreateControlSurface2 (AIRCTRL_AILERON,  3, 1.5, _V( 7,-0.5,-15), AIRCTRL_AXIS_XPOS, anim_raileron);
	//hlaileron = CreateControlSurface2 (AIRCTRL_AILERON,  3, 1.5, _V(-7,-0.5,-15), AIRCTRL_AXIS_XNEG, anim_laileron);
	//hraileron = CreateControlSurface2 (AIRCTRL_AILERON, 0.0, 0.0, _V( 7,-0.5,-15), AIRCTRL_AXIS_XPOS, anim_raileron);
	//hlaileron = CreateControlSurface2 (AIRCTRL_AILERON, 0.0, 0.0, _V(-7,-0.5,-15), AIRCTRL_AXIS_XNEG, anim_laileron);
	hraileron = CreateControlSurface3(AIRCTRL_AILERON, 0.0, 0.0, _V(7, -0.5, -15), AIRCTRL_AXIS_XPOS, 1.0);
	hlaileron = CreateControlSurface3(AIRCTRL_AILERON, 0.0, 0.0, _V(-7, -0.5, -15), AIRCTRL_AXIS_XNEG, 1.0);
	ControlSurfacesEnabled = true;
}

// --------------------------------------------------------------
// Define animation sequences for moving parts
// --------------------------------------------------------------
void Atlantis::DefineAnimations(void)
{
	UINT midx = mesh_orbiter; // mesh index for all external animations

	ANIMATIONCOMPONENT_HANDLE parent;
	BeginLoggingAnims();

	// ***** 1. Cargo door and radiator animations *****
	// DaveS edit: Updated animations to work with the new scaled down orbiter mesh

	static UINT RCargoDoorGrp[4] = { GRP_RIGHT_PLBD_EXT, GRP_STBD_PLBD_INTERIOR, GRP_STBD_PLB_RADIATOR_3, GRP_STBD_PLB_RADIATOR_4, };
	MGROUP_ROTATE* pRCargoDoor = new MGROUP_ROTATE(midx, RCargoDoorGrp, 4,
		_V(2.6942, 0.2449, 0.0), _V(0, 0, 1), (float)(-175.5*RAD));
	static UINT LCargoDoorGrp[4] = { GRP_LEFT_PLBD_EXT, GRP_PORT_PLBD_INTERIOR, GRP_PORT_PLB_RADIATOR_3, GRP_PORT_PLB_RADIATOR_4 };
	MGROUP_ROTATE* pLCargoDoor = new MGROUP_ROTATE(midx, LCargoDoorGrp, 4,
		_V(-2.6942, 0.2449, 0.0), _V(0, 0, 1), (float)(175.5*RAD));

	static UINT RRadiatorGrp[3] = { GRP_STBD_PLB_RADIATOR_1, GRP_STBD_PLB_RADIATOR_2, GRP_STBD_FWD_RADIATOR_COOLANT_LINES };
	MGROUP_ROTATE* pRRadiator = new MGROUP_ROTATE(midx, RRadiatorGrp, 3,
		_V(2.587, 0.348, 0.0), _V(0, 0, 1), (float)(31.6*RAD));
	static UINT LRadiatorGrp[3] = { GRP_PORT_PLB_RADIATOR_1, GRP_PORT_PLB_RADIATOR_2, GRP_PORT_FWD_RADIATOR_COOLANT_LINES };
	MGROUP_ROTATE* pLRadiator = new MGROUP_ROTATE(midx, LRadiatorGrp, 3,
		_V(-2.587, 0.348, 0.0), _V(0, 0, 1), (float)(-31.6*RAD));

	static UINT PORTPUSH_RODGrp[1] = { GRP_PORTPUSH_ROD };
	MGROUP_ROTATE* pPORTPushRod = new MGROUP_ROTATE(midx, PORTPUSH_RODGrp, 1,
		_V(-2.567, 0.059, 0.0), _V(0, 0, 1), static_cast<float>(-90.0*RAD));

	static UINT PORT_CLAMPGrp[1] = { GRP_PORT_CLAMP };
	MGROUP_ROTATE* pPORT_CLAMP = new MGROUP_ROTATE(midx, PORT_CLAMPGrp, 1,
		_V(-2.6289, 0.1321, 0.0), _V(0, 0, 1), (float)(90 * RAD));

	static UINT PORT_PullRodGrp[1] = { GRP_PORTDOOR_PULL };
	MGROUP_ROTATE* pPORTPullRod = new MGROUP_ROTATE(midx, PORT_PullRodGrp, 1,
		_V(-2.427, 0.294, 0.0), _V(0, 0, 1), (float)(-10 * RAD));

	static UINT STBDPUSH_RODGrp[1] = { GRP_STBDPUSH_ROD };
	MGROUP_ROTATE* pSTBDPushRod = new MGROUP_ROTATE(midx, STBDPUSH_RODGrp, 1,
		_V(2.567, 0.059, 0.0), _V(0, 0, 1), static_cast<float>(90.0*RAD));

	static UINT STBD_CLAMPGrp[1] = { GRP_STBD_CLAMP };
	MGROUP_ROTATE* pSTBD_CLAMP = new MGROUP_ROTATE(midx, STBD_CLAMPGrp, 1,
		_V(2.6289, 0.1321, 0.0), _V(0, 0, 1), (float)(-90 * RAD));

	static UINT STBD_PullRodGrp[1] = { GRP_STBDDOOR_PULL };
	MGROUP_ROTATE* pSTBDPullRod = new MGROUP_ROTATE(midx, STBD_PullRodGrp, 1,
		_V(2.427, 0.294, 0.0), _V(0, 0, 1), (float)(10.0*RAD));

	//latches
	static UINT CLatch1_4Grp[1] = { GRP_FWD_HOOKS };
	static MGROUP_ROTATE CLatch1_4(midx, CLatch1_4Grp, 1,
		_V(0.0234, 2.1905, 0.0), _V(0, 0, 1), (float)(90 * RAD));

	static UINT CLatch5_8Grp[1] = { GRP_MID_FWD_HOOKS };
	static MGROUP_ROTATE CLatch5_8(midx, CLatch5_8Grp, 1,
		_V(0.0234, 2.1905, 0.0), _V(0, 0, 1), (float)(90 * RAD));

	static UINT CLatch9_12Grp[1] = { GRP_MID_AFT_HOOKS };
	static MGROUP_ROTATE CLatch9_12(midx, CLatch9_12Grp, 1,
		_V(0.0234, 2.1905, 0.0), _V(0, 0, 1), (float)(90 * RAD));

	static UINT CLatch13_16Grp[1] = { GRP_AFT_HOOKS };
	static MGROUP_ROTATE CLatch13_16(midx, CLatch13_16Grp, 1,
		_V(0.0234, 2.1905, 0.0), _V(0, 0, 1), (float)(90 * RAD));

	anim_door = CreateAnimation(0);
	LogAnim("anim_door", anim_door);
	anim_rad[0] = CreateAnimation(0);
	LogAnim("anim_rad[0]", anim_rad[0]);
	anim_rad[1] = CreateAnimation(0);
	LogAnim("anim_rad[1]", anim_rad[1]);
	anim_clatch[0] = CreateAnimation(0);
	anim_clatch[1] = CreateAnimation(0);
	anim_clatch[2] = CreateAnimation(0);
	anim_clatch[3] = CreateAnimation(0);
	// **************************************************************************************
	//right (starboard) side
	// **************************************************************************************
	parent = AddManagedAnimationComponent(anim_door, 0.161290, 0.5, pRCargoDoor);
	AddManagedAnimationComponent(anim_rad[1], 0, 1, pRRadiator, parent);
	//latches
	AddAnimationComponent(anim_clatch[0], 0, 1, &CLatch1_4, parent);
	AddAnimationComponent(anim_clatch[1], 0, 1, &CLatch5_8, parent);
	AddAnimationComponent(anim_clatch[2], 0, 1, &CLatch9_12, parent);
	AddAnimationComponent(anim_clatch[3], 0, 1, &CLatch13_16, parent);
	//right push/pull rods
	parent = AddManagedAnimationComponent(anim_door, 0.161290, 0.5, pSTBD_CLAMP);
	AddManagedAnimationComponent(anim_door, 0, 0.4632, pSTBDPullRod, parent);
	AddManagedAnimationComponent(anim_door, 0, 0.4632, pSTBDPushRod, parent);
	// **************************************************************************************
	//left(port) side
	// **************************************************************************************
	parent = AddManagedAnimationComponent(anim_door, 0.661290, 1.0, pLCargoDoor);
	AddManagedAnimationComponent(anim_rad[0], 0, 1, pLRadiator, parent);
	//left push/pull rods
	parent = AddManagedAnimationComponent(anim_door, 0.661290, 1.0, pPORT_CLAMP);
	AddManagedAnimationComponent(anim_door, 0.5368, 1.0, pPORTPullRod, parent);
	AddManagedAnimationComponent(anim_door, 0.5368, 1.0, pPORTPushRod, parent);

	// ***** 2. Landing gear animation *****
	static UINT LNosewheelDoorGrp[1] = { GRP_LEFT_NLG_DOOR };
	static MGROUP_ROTATE LNosewheelDoor(midx, LNosewheelDoorGrp, 1,
		_V(-0.712, -2.905, 16.082), _V(0, -0.209789, -0.977747), (float)(65.3*RAD));
	static UINT RNosewheelDoorGrp[1] = { GRP_RIGHT_NLG_DOOR };
	static MGROUP_ROTATE RNosewheelDoor(midx, RNosewheelDoorGrp, 1,
		_V(0.712, -2.905, 16.082), _V(0, -0.209789, -0.977747), (float)(-65.3*RAD));
	static UINT NosewheelGrp[5] = { GRP_NOSE_WHEELS, GRP_NLG_STRUT, GRP_NLG_UPPER_TORQUE_ARM, GRP_NLG_LOWER_TORQUE_ARM, GRP_NLG_SHOCK_STRUT };
	static MGROUP_ROTATE Nosewheel(midx, NosewheelGrp, 5,
		_V(0, -3.038, 14.888), _V(1, 0, 0), (float)(114.5*RAD));
	static UINT RGearDoorGrp[1] = { GRP_RIGHT_MLG_DOOR };
	static MGROUP_ROTATE RGearDoor(midx, RGearDoorGrp, 1,
		_V(4.507, -3.504, -3.991), _V(0, -0.0261815, -0.999657), (float)(-84.2*RAD));
	static UINT LGearDoorGrp[1] = { GRP_LEFT_MLG_DOOR };
	static MGROUP_ROTATE LGearDoor(midx, LGearDoorGrp, 1,
		_V(-4.507, -3.504, -3.991), _V(0, -0.0261815, -0.999657), (float)(84.2*RAD));
	static UINT MainGearGrp[6] = { GRP_RIGHT_MN_WHEELS, GRP_RIGHT_MLG_STRUT, GRP_RIGHT_MLG_SHOCK_STRUT, GRP_LEFT_MN_WHEELS, GRP_LEFT_MLG_STRUT, GRP_LEFT_MLG_SHOCK_STRUT };
	static MGROUP_ROTATE MainGear(midx, MainGearGrp, 6,
		_V(0, -3.24, -5.546), _V(1, 0, 0), (float)(94.5*RAD));

	anim_gear = CreateAnimation(0);
	LogAnim("anim_gear", anim_gear);
	AddAnimationComponent(anim_gear, 0, 0.5, &LNosewheelDoor);
	AddAnimationComponent(anim_gear, 0, 0.5, &RNosewheelDoor);
	AddAnimationComponent(anim_gear, 0.4, 1.0, &Nosewheel);
	AddAnimationComponent(anim_gear, 0, 0.5, &RGearDoor);
	AddAnimationComponent(anim_gear, 0, 0.5, &LGearDoor);
	AddAnimationComponent(anim_gear, 0.4, 1.0, &MainGear);

	if (bHasKUBand)
		DefineKUBandAnimations();

	//drag chute
	static UINT ChuteCords[1] = { 0 };
	static MGROUP_SCALE ScaleCords(mesh_dragchute, ChuteCords, 1, _V(0.0, 0.0, -11.53), _V(0.5, 0.5, 1.1));
	static UINT Chute[3] = { 1, 2, 3 };
	static MGROUP_SCALE ScaleChute(mesh_dragchute, Chute, 3, _V(0.0, 0.0, -29.03), _V(0.5, 0.5, 1.2));
	static UINT DragChute[4] = { 0, 1, 2, 3 };
	static MGROUP_SCALE ScaleAll(mesh_dragchute, DragChute, 4, _V(0.0, 0.0, 0.0), _V(0.01, 0.01, 0.01));
	anim_chute_deploy = CreateAnimation(0.0);
	LogAnim("anim_chute_deploy", anim_chute_deploy);
	AddAnimationComponent(anim_chute_deploy, 0.0, 0.6, &ScaleCords);
	AddAnimationComponent(anim_chute_deploy, 0.0, 0.6, &ScaleChute);
	parent = AddAnimationComponent(anim_chute_deploy, 0.6, 0.9, &ScaleAll);
	anim_chute_spin = CreateAnimation(0.0);
	LogAnim("anim_chute_spin", anim_chute_spin);
	static MGROUP_ROTATE SpinChute1(mesh_dragchute, DragChute, 4,
		_V(0, 0, 0), _V(0.00609621, -0.0146035, -0.999875), (float)(400.0*RAD));
	AddAnimationComponent(anim_chute_spin, 0.0, 0.5, &SpinChute1, parent);
	static MGROUP_ROTATE SpinChute2(mesh_dragchute, DragChute, 4,
		_V(0, 0, 0), _V(-0.00965286, -0.00698363, -0.999929), (float)(400.0*RAD));
	AddAnimationComponent(anim_chute_spin, 0.5, 1.0, &SpinChute2, parent);

	// ***** 4. Elevator animation of elevons *****

	// ***** 4A. Upward animation of elevons *****
	static UINT LElevGrp[2] = { GRP_LEFT_INBOARD_ELEVON, GRP_LEFT_OUTBOARD_ELEVON };
	static UINT RElevGrp[2] = { GRP_RIGHT_INBOARD_ELEVON, GRP_RIGHT_OUTBOARD_ELEVON };
	static UINT LElevDoorGrp[2] = { GRP_LEFT_INBOARD_ELEVON_SEAL_PANEL, GRP_LEFT_OUTBOARD_ELEVON_SEAL_PANEL };
	static UINT RElevDoorGrp[2] = { GRP_RIGHT_INBOARD_ELEVON_SEAL_PANEL, GRP_RIGHT_OUTBOARD_ELEVON_SEAL_PANEL };
	static MGROUP_ROTATE LElevator_up(midx, LElevGrp, 2,
		_V(-7.83304, -3.37684, -10.70273), _V(-0.994826, -0.101587, -0.00088665), (float)(34.0*RAD));
	static MGROUP_ROTATE RElevator_up(midx, RElevGrp, 2,
		_V(7.83304, -3.37684, -10.70273), _V(0.994826, 0.101587, -0.00088665), (float)(-34.0*RAD));
	static MGROUP_ROTATE LElevatorDoor_up(midx, LElevDoorGrp, 2,
		_V(-7.837, -2.697, -10.329), _V(-0.999228, 0.0392895, 0.000550051), (float)(22.0*RAD));
	static MGROUP_ROTATE RElevatorDoor_up(midx, RElevDoorGrp, 2,
		_V(7.837, -2.697, -10.329), _V(0.999228, 0.0392895, 0.000550051), (float)(-22.0*RAD));

	// ***** 4B. Downward animation of elevons *****
	static MGROUP_ROTATE LElevator_down(midx, LElevGrp, 2,
		_V(-7.83304, -3.37684, -10.70273), _V(-0.994826, 0.101587, -0.00088665), (float)(18.0*RAD));
	static MGROUP_ROTATE RElevator_down(midx, RElevGrp, 2,
		_V(7.83304, -3.37684, -10.70273), _V(0.994826, 0.101587, -0.00088665), (float)(-18.0*RAD));
	static MGROUP_ROTATE LElevatorDoor_down(midx, LElevDoorGrp, 2,
		_V(-7.837, -2.697, -10.329), _V(-0.999228, 0.0392895, -0.000550051), (float)(5 * RAD));
	static MGROUP_ROTATE RElevatorDoor_down(midx, RElevDoorGrp, 2,
		_V(7.837, -2.697, -10.329), _V(0.999228, 0.0392895, -0.000550051), (float)(-5 * RAD));
	anim_lelevon = CreateAnimation(0.5);
	anim_relevon = CreateAnimation(0.5);
	LogAnim("anim_lelevon", anim_lelevon);
	LogAnim("anim_relevon", anim_relevon);
	AddAnimationComponent(anim_lelevon, 0.5, 1, &LElevator_down);
	AddAnimationComponent(anim_lelevon, 0, 0.5, &LElevator_up);
	AddAnimationComponent(anim_lelevon, 0.5, 1, &LElevatorDoor_down);
	AddAnimationComponent(anim_lelevon, 0, 0.5, &LElevatorDoor_up);
	AddAnimationComponent(anim_relevon, 0.5, 1, &RElevator_down);
	AddAnimationComponent(anim_relevon, 0, 0.5, &RElevator_up);
	AddAnimationComponent(anim_relevon, 0.5, 1, &RElevatorDoor_down);
	AddAnimationComponent(anim_relevon, 0, 0.5, &RElevatorDoor_up);

	// ***** 5. Bodyflap animation *****
	static UINT bfGrp[1] = { GRP_BODYFLAP };
	static MGROUP_ROTATE BodyFlap(midx, bfGrp, 1,
		_V(0, -3.3191, -14.5522), _V(1, 0, 0), (float)(22.5*RAD));
	anim_bf = CreateAnimation(0.5);
	LogAnim("anim_bf", anim_bf);
	AddAnimationComponent(anim_bf, 0, 1, &BodyFlap);

	// ***** 6. Rudder animation *****

	static UINT RudderGrp[2] = { GRP_RIGHT_RSB_PANEL, GRP_LEFT_RSB_PANEL };
	static MGROUP_ROTATE Rudder(midx, RudderGrp, 2,
		_V(0, 6.8866, -15.8689), _V(0, -0.825413, 0.56453), (float)(54.2*RAD));
	anim_rudder = CreateAnimation(0.5);
	LogAnim("anim_rudder", anim_rudder);
	AddAnimationComponent(anim_rudder, 0, 1, &Rudder);

	// ***** 7. Speedbrake animation *****

	static UINT SB1Grp[1] = { GRP_RIGHT_RSB_PANEL };
	static MGROUP_ROTATE SB1(midx, SB1Grp, 1,
		_V(0,  6.8866, -15.8689), _V(0, -0.825413, 0.56453), (float)(49.3*RAD));
	static UINT SB2Grp[1] = { GRP_LEFT_RSB_PANEL };
	static MGROUP_ROTATE SB2(midx, SB2Grp, 1,
		_V(0,  6.8866, -15.8689), _V(0, -0.825413, 0.56453), (float)(-49.3*RAD));

	anim_spdb = CreateAnimation(0);
	LogAnim("anim_spdb", anim_spdb);
	AddAnimationComponent(anim_spdb, 0, 1, &SB1);
	AddAnimationComponent(anim_spdb, 0, 1, &SB2);

	// ***** 8. RMS arm animation *****
	// Note that the animation components can't be declared static here, since
	// their rotation parameters are modified by the respective parent transforms


	// ***** 9 Payload bay cameras animation *****
	// DaveS edit: realigned with the scaled down orbiter mesh
	// FRONT LEFT
	static UINT camFLYawGrp[1] = { GRP_PANTILT_CAM_A };
	MGROUP_ROTATE* CameraFLYaw = new MGROUP_ROTATE(midx, camFLYawGrp, 1,
		_V(-1.816, 0.909, 9.464), _V(0, 1, 0), (float)(340 * RAD));
	anim_camFLyaw = CreateAnimation(0.5);
	LogAnim("anim_camFLyaw", anim_camFLyaw);
	parent = AddManagedAnimationComponent(anim_camFLyaw, 0, 1, CameraFLYaw);

	static UINT camFLPitchGrp[1] = { GRP_CAM_A };
	MGROUP_ROTATE* CameraFLPitch = new MGROUP_ROTATE(midx, camFLPitchGrp, 1,
		_V(-1.816, 0.909, 9.464), _V(1, 0, 0), (float)(340 * RAD));
	anim_camFLpitch = CreateAnimation(0.5);
	//anim_camFLpitch = CreateAnimation (0.7647);
	LogAnim("anim_camFLpitch", anim_camFLpitch);
	parent = AddManagedAnimationComponent(anim_camFLpitch, 0, 1, CameraFLPitch, parent);

	MGROUP_TRANSFORM* CameraFLPos = new MGROUP_TRANSFORM(LOCALVERTEXLIST, MAKEGROUPARRAY(&plbdCamPos[0]), 1);
	AddAnimationComponent(anim_camFLpitch, 0, 1, CameraFLPos, parent);

	// FRONT RIGHT
	static UINT camFRYawGrp[1] = { GRP_PANTILT_CAM_D };
	MGROUP_ROTATE* CameraFRYaw = new MGROUP_ROTATE(midx, camFRYawGrp, 1,
		_V(1.816, 0.909, 9.464), _V(0, 1, 0), (float)(340 * RAD));
	anim_camFRyaw = CreateAnimation(0.5);
	LogAnim("anim_camFRyaw", anim_camFRyaw);
	parent = AddManagedAnimationComponent(anim_camFRyaw, 0, 1, CameraFRYaw);

	static UINT camFRPitchGrp[1] = { GRP_CAM_D };
	MGROUP_ROTATE* CameraFRPitch = new MGROUP_ROTATE(midx, camFRPitchGrp, 1,
		_V(1.816, 0.909, 9.464), _V(1, 0, 0), (float)(340 * RAD));
	anim_camFRpitch = CreateAnimation(0.5);
	LogAnim("anim_camFRpitch", anim_camFRpitch);
	parent = AddManagedAnimationComponent(anim_camFRpitch, 0, 1, CameraFRPitch, parent);

	MGROUP_TRANSFORM* CameraFRPos = new MGROUP_TRANSFORM(LOCALVERTEXLIST, MAKEGROUPARRAY(&plbdCamPos[3]), 1);
	AddManagedAnimationComponent(anim_camFRpitch, 0, 1, CameraFRPos, parent);

	// BACK LEFT
	static UINT camBLYawGrp[1] = { GRP_PANTILT_CAM_B };
	MGROUP_ROTATE* CameraBLYaw = new MGROUP_ROTATE(midx, camBLYawGrp, 1,
		_V(-2.21, 0.909, -8.438), _V(0, 1, 0), (float)(340 * RAD));
	anim_camBLyaw = CreateAnimation(0.5);
	LogAnim("anim_camBLyaw", anim_camBLyaw);
	parent = AddManagedAnimationComponent(anim_camBLyaw, 0, 1, CameraBLYaw);

	static UINT camBLPitchGrp[1] = { GRP_CAM_B };
	MGROUP_ROTATE* CameraBLPitch = new MGROUP_ROTATE(midx, camBLPitchGrp, 1,
		_V(-2.21, 0.909, -8.438), _V(-1, 0, 0), (float)(340 * RAD));
	anim_camBLpitch = CreateAnimation(0.5);
	LogAnim("anim_camBLpitch", anim_camBLpitch);
	parent = AddManagedAnimationComponent(anim_camBLpitch, 0, 1, CameraBLPitch, parent);

	MGROUP_TRANSFORM* CameraBLPos = new MGROUP_TRANSFORM(LOCALVERTEXLIST, MAKEGROUPARRAY(&plbdCamPos[1]), 1);
	AddAnimationComponent(anim_camBLpitch, 0, 1, CameraBLPos, parent);

	// BACK RIGHT
	static UINT camBRYawGrp[1] = { GRP_PANTILT_CAM_C };
	MGROUP_ROTATE* CameraBRYaw = new MGROUP_ROTATE(midx, camBRYawGrp, 1,
		_V(2.21, 0.909, -8.438), _V(0, 1, 0), (float)(340 * RAD));
	anim_camBRyaw = CreateAnimation(0.5);
	LogAnim("anim_camBRyaw", anim_camBRyaw);
	parent = AddManagedAnimationComponent(anim_camBRyaw, 0, 1, CameraBRYaw);

	static UINT camBRPitchGrp[1] = { GRP_CAM_C };
	MGROUP_ROTATE* CameraBRPitch = new MGROUP_ROTATE(midx, camBRPitchGrp, 1,
		_V(2.21, 0.909, -8.438), _V(-1, 0, 0), (float)(340 * RAD));
	anim_camBRpitch = CreateAnimation(0.5);
	LogAnim("anim_camBRpitch", anim_camBRpitch);
	parent = AddManagedAnimationComponent(anim_camBRpitch, 0, 1, CameraBRPitch, parent);

	MGROUP_TRANSFORM* CameraBRPos = new MGROUP_TRANSFORM(LOCALVERTEXLIST, MAKEGROUPARRAY(&plbdCamPos[2]), 1);
	AddManagedAnimationComponent(anim_camBRpitch, 0, 1, CameraBRPos, parent);

	// ***** 11 ET Umb Door animation *****
	static UINT ETUmbLGrp[1] = { GRP_ET_DOOR_LEFT };
	static UINT ETUmbRGrp[1] = { GRP_ET_DOOR_RIGHT };
	static MGROUP_ROTATE EtumbdoorL(midx, ETUmbLGrp, 1,
		_V(-1.323, -3.961, -9.49), _V(0.0, -0.0700974159956, 0.997540150706), (float)(+180.0*RAD));
	static MGROUP_ROTATE EtumbdoorR(midx, ETUmbRGrp, 1,
		_V(1.323, -3.961, -9.49), _V(0.0, -0.0700974159956, 0.997540150706), (float)(-180.0*RAD));

	static VECTOR3 etumbdoor_dummyvec[8];
	static UINT ETUmbLDDMAftGrp[1] = { GRP_ET_DOOR_LEFT_DDM_ARM_AFT };
	static UINT ETUmbLDDMAftRodGrp[1] = { GRP_ET_DOOR_LEFT_DDM_LINK_ROD_AFT };
	static MGROUP_ROTATE ETUmbLDDMAft1(LOCALVERTEXLIST, MAKEGROUPARRAY(etumbdoor_dummyvec), 1, _V(-1.242, -3.821, -10.024), _V(0, 0.060757, -0.998152), static_cast<float>(-65.0*RAD));
	static MGROUP_ROTATE ETUmbLDDMAft2(midx, ETUmbLDDMAftGrp, 1, _V(-1.242, -3.821, -10.024), _V(0, 0.060757, -0.998152), static_cast<float>(-55.0*RAD));
	static MGROUP_ROTATE ETUmbLDDMAftRod1(LOCALVERTEXLIST, MAKEGROUPARRAY(etumbdoor_dummyvec + 1), 1, _V(-1.404, -3.583, -10.013), _V(0, 0, -1), static_cast<float>(19.0*RAD));
	static MGROUP_ROTATE ETUmbLDDMAftRod2(midx, ETUmbLDDMAftRodGrp, 1, _V(-1.404, -3.583, -10.013), _V(0, 0, -1), static_cast<float>(-26 * RAD));
	static UINT ETUmbLDDMFwdGrp[1] = { GRP_ET_DOOR_LEFT_DDM_ARM_FWD };
	static UINT ETUmbLDDMFwdRodGrp[1] = { GRP_ET_DOOR_LEFT_DDM_LINK_ROD_FWD };
	static MGROUP_ROTATE ETUmbLDDMFwd1(LOCALVERTEXLIST, MAKEGROUPARRAY(etumbdoor_dummyvec + 2), 1, _V(-1.242, -3.888, -8.967), _V(0, 0.060757, -0.998152), static_cast<float>(-65.0*RAD));
	static MGROUP_ROTATE ETUmbLDDMFwd2(midx, ETUmbLDDMFwdGrp, 1, _V(-1.242, -3.888, -8.967), _V(0, 0.060757, -0.998152), static_cast<float>(-55.0*RAD));
	static MGROUP_ROTATE ETUmbLDDMFwdRod1(LOCALVERTEXLIST, MAKEGROUPARRAY(etumbdoor_dummyvec + 3), 1, _V(-1.404, -3.662, -8.953), _V(0, 0, -1), static_cast<float>(19.0*RAD));
	static MGROUP_ROTATE ETUmbLDDMFwdRod2(midx, ETUmbLDDMFwdRodGrp, 1, _V(-1.404, -3.662, -8.953), _V(0, 0, -1), static_cast<float>(-26.0*RAD));
	anim_letumbdoor = CreateAnimation(0);
	LogAnim("anim_letumbdoor", anim_letumbdoor);
	parent = AddAnimationComponent(anim_letumbdoor, 0, 1, &EtumbdoorL);
	parent = AddAnimationComponent(anim_letumbdoor, 0, 0.5, &ETUmbLDDMFwd1);
	parent = AddAnimationComponent(anim_letumbdoor, 0.5, 1, &ETUmbLDDMFwd2, parent);
	parent = AddAnimationComponent(anim_letumbdoor, 0, 0.5, &ETUmbLDDMFwdRod1, parent);
	AddAnimationComponent(anim_letumbdoor, 0.5, 1, &ETUmbLDDMFwdRod2, parent);
	parent = AddAnimationComponent(anim_letumbdoor, 0, 0.5, &ETUmbLDDMAft1);
	parent = AddAnimationComponent(anim_letumbdoor, 0.5, 1, &ETUmbLDDMAft2, parent);
	parent = AddAnimationComponent(anim_letumbdoor, 0, 0.5, &ETUmbLDDMAftRod1, parent);
	AddAnimationComponent(anim_letumbdoor, 0.5, 1, &ETUmbLDDMAftRod2, parent);

	static UINT ETUmbRDDMAftGrp[1] = { GRP_ET_DOOR_RIGHT_DDM_ARM_AFT };
	static UINT ETUmbRDDMAftRodGrp[1] = { GRP_ET_DOOR_RIGHT_DDM_LINK_ROD_AFT };
	static MGROUP_ROTATE ETUmbRDDMAft1(LOCALVERTEXLIST, MAKEGROUPARRAY(etumbdoor_dummyvec + 4), 1, _V(1.242, -3.821, -10.024), _V(0, 0.060757, -0.998152), static_cast<float>(65.0*RAD));
	static MGROUP_ROTATE ETUmbRDDMAft2(midx, ETUmbRDDMAftGrp, 1, _V(1.242, -3.821, -10.024), _V(0, 0.060757, -0.998152), static_cast<float>(55.0*RAD));
	static MGROUP_ROTATE ETUmbRDDMAftRod1(LOCALVERTEXLIST, MAKEGROUPARRAY(etumbdoor_dummyvec + 5), 1, _V(1.404, -3.583, -10.013), _V(0, 0, -1), static_cast<float>(-19.0*RAD));
	static MGROUP_ROTATE ETUmbRDDMAftRod2(midx, ETUmbRDDMAftRodGrp, 1, _V(1.404, -3.583, -10.013), _V(0, 0, -1), static_cast<float>(29 * RAD));
	static UINT ETUmbRDDMFwdGrp[1] = { GRP_ET_DOOR_RIGHT_DDM_ARM_FWD };
	static UINT ETUmbRDDMFwdRodGrp[1] = { GRP_ET_DOOR_RIGHT_DDM_LINK_ROD_FWD };
	static MGROUP_ROTATE ETUmbRDDMFwd1(LOCALVERTEXLIST, MAKEGROUPARRAY(etumbdoor_dummyvec + 6), 1, _V(1.242, -3.888, -8.967), _V(0, 0.060757, -0.998152), static_cast<float>(65.0*RAD));
	static MGROUP_ROTATE ETUmbRDDMFwd2(midx, ETUmbRDDMFwdGrp, 1, _V(1.242, -3.888, -8.967), _V(0, 0.060757, -0.998152), static_cast<float>(55.0*RAD));
	static MGROUP_ROTATE ETUmbRDDMFwdRod1(LOCALVERTEXLIST, MAKEGROUPARRAY(etumbdoor_dummyvec + 7), 1, _V(1.404, -3.662, -8.953), _V(0, 0, -1), static_cast<float>(-19.0*RAD));
	static MGROUP_ROTATE ETUmbRDDMFwdRod2(midx, ETUmbRDDMFwdRodGrp, 1, _V(1.404, -3.662, -8.953), _V(0, 0, -1), static_cast<float>(29.0*RAD));
	anim_retumbdoor = CreateAnimation(0);
	LogAnim("anim_retumbdoor", anim_retumbdoor);
	parent = AddAnimationComponent(anim_retumbdoor, 0, 1, &EtumbdoorR);
	parent = AddAnimationComponent(anim_retumbdoor, 0, 0.5, &ETUmbRDDMFwd1);
	parent = AddAnimationComponent(anim_retumbdoor, 0.5, 1, &ETUmbRDDMFwd2, parent);
	parent = AddAnimationComponent(anim_retumbdoor, 0, 0.5, &ETUmbRDDMFwdRod1, parent);
	AddAnimationComponent(anim_retumbdoor, 0.5, 1, &ETUmbRDDMFwdRod2, parent);
	parent = AddAnimationComponent(anim_retumbdoor, 0, 0.5, &ETUmbRDDMAft1);
	parent = AddAnimationComponent(anim_retumbdoor, 0.5, 1, &ETUmbRDDMAft2, parent);
	parent = AddAnimationComponent(anim_retumbdoor, 0, 0.5, &ETUmbRDDMAftRod1, parent);
	AddAnimationComponent(anim_retumbdoor, 0.5, 1, &ETUmbRDDMAftRod2, parent);

	// ***** SSME Gimbal Animations *****


	static UINT SSMET_Grp[1] = { GRP_CENTER_SSME };
	static UINT SSMEL_Grp[1] = { GRP_LEFT_SSME };
	static UINT SSMER_Grp[1] = { GRP_RIGHT_SSME };
	static VECTOR3 ssmet_dummy_vec;
	static VECTOR3 ssmer_dummy_vec;
	static VECTOR3 ssmel_dummy_vec;

	SSMET_GOX_REF1 = SSMET_GOX_REF;
	SSMEL_GOX_REF1 = SSMEL_GOX_REF;
	SSMER_GOX_REF1 = SSMER_GOX_REF;

	static MGROUP_ROTATE SSMETYaw(LOCALVERTEXLIST, MAKEGROUPARRAY(&ssmet_dummy_vec), 1, SSMET_REF, _V(0.0, 1.0, 0.0), (float)(21.0 * RAD));
	static MGROUP_ROTATE SSMETPitch(midx, SSMET_Grp, 1, SSMET_REF, _V(1.0, 0.0, 0.0), (float)(17.0 * RAD));
	static MGROUP_ROTATE SSMETPitchV(LOCALVERTEXLIST, MAKEGROUPARRAY(&SSMET_GOX_REF1), 1, SSMET_REF, _V(1.0, 0.0, 0.0), (float)(17.0 * RAD));

	anim_ssmeTyaw = CreateAnimation(0.5);
	LogAnim("anim_ssmeTyaw", anim_ssmeTyaw);
	anim_ssmeTpitch = CreateAnimation(0.5);
	LogAnim("anim_ssmeTpitch", anim_ssmeTpitch);

	parent = AddAnimationComponent(anim_ssmeTyaw, 0.0, 1.0, &SSMETYaw, NULL);
	AddAnimationComponent(anim_ssmeTpitch, 0.0, 1.0, &SSMETPitch, parent);
	AddAnimationComponent(anim_ssmeTpitch, 0.0, 1.0, &SSMETPitchV, parent);


	static MGROUP_ROTATE SSMELYaw(LOCALVERTEXLIST, MAKEGROUPARRAY(&ssmel_dummy_vec), 1, SSMEL_REF, _V(0.0, 1.0, 0.0), (float)(21.0 * RAD));
	static MGROUP_ROTATE SSMELPitch(midx, SSMEL_Grp, 1, SSMEL_REF, _V(1.0, 0.0, 0.0), (float)(17.0 * RAD));
	//Virtual animations
	static MGROUP_ROTATE SSMELPitchV(LOCALVERTEXLIST, MAKEGROUPARRAY(&SSMEL_GOX_REF1), 1, SSMEL_REF, _V(1.0, 0.0, 0.0), (float)(17.0 * RAD));

	anim_ssmeLyaw = CreateAnimation(0.5);
	LogAnim("anim_ssmeLyaw", anim_ssmeLyaw);
	anim_ssmeLpitch = CreateAnimation(0.5);
	LogAnim("anim_ssmeLpitch", anim_ssmeLpitch);

	parent = AddAnimationComponent(anim_ssmeLyaw, 0.0, 1.0, &SSMELYaw, NULL);
	AddAnimationComponent(anim_ssmeLpitch, 0.0, 1.0, &SSMELPitch, parent);
	AddAnimationComponent(anim_ssmeLpitch, 0.0, 1.0, &SSMELPitchV, parent);


	static MGROUP_ROTATE SSMERYaw(LOCALVERTEXLIST, MAKEGROUPARRAY(&ssmer_dummy_vec), 1, SSMER_REF, _V(0.0, 1.0, 0.0), (float)(21.0 * RAD));
	static MGROUP_ROTATE SSMERPitch(midx, SSMER_Grp, 1, SSMER_REF, _V(1.0, 0.0, 0.0), (float)(17.0 * RAD));
	static MGROUP_ROTATE SSMERPitchV(LOCALVERTEXLIST, MAKEGROUPARRAY(&SSMER_GOX_REF1), 1, SSMER_REF, _V(1.0, 0.0, 0.0), (float)(17.0 * RAD));

	anim_ssmeRyaw = CreateAnimation(0.5);
	LogAnim("anim_ssmeRyaw", anim_ssmeRyaw);
	anim_ssmeRpitch = CreateAnimation(0.5);
	LogAnim("anim_ssmeRpitch", anim_ssmeRpitch);

	parent = AddAnimationComponent(anim_ssmeRyaw, 0.0, 1.0, &SSMERYaw, NULL);
	AddAnimationComponent(anim_ssmeRpitch, 0.0, 1.0, &SSMERPitch, parent);
	AddAnimationComponent(anim_ssmeRpitch, 0.0, 1.0, &SSMERPitchV, parent);

	// ======================================================
	// Air Data Probe Assembly Animations
	// ======================================================



	static UINT ADPL_Grp[1] = { GRP_PROBEL };
	static UINT ADPR_Grp[1] = { GRP_PROBER };

	static MGROUP_ROTATE ADPL_Deploy(midx, ADPL_Grp, 1, PROBEL_REF, PROBE_AXIS, (float)(180.0 * RAD));
	static MGROUP_ROTATE ADPR_Deploy(midx, ADPR_Grp, 1, PROBER_REF, PROBE_AXIS, (float)(-180.0 * RAD));

	anim_adpl = CreateAnimation(1.0);
	LogAnim("anim_adpl", anim_adpl);
	AddAnimationComponent(anim_adpl, 0.0, 1.0, &ADPL_Deploy);

	anim_adpr = CreateAnimation(1.0);
	LogAnim("anim_adpr", anim_adpr);
	AddAnimationComponent(anim_adpr, 0.0, 1.0, &ADPR_Deploy);

	// ======================================================
	// Star tracker door animation
	// ======================================================

	static UINT STZD_Grp[1] = { GRP_ZSTARTRACKER_DOOR };
	static UINT STYD_Grp[1] = { GRP_XSTARTRACKER_DOOR };

	static MGROUP_ROTATE STZD_Open(midx, STZD_Grp, 1, STZD_REF, STZD_AXIS, STAR_TRACKER_DOOR_ANIMATION_ANGLE);
	static MGROUP_ROTATE STYD_Open(midx, STYD_Grp, 1, STYD_REF, STYD_AXIS, STAR_TRACKER_DOOR_ANIMATION_ANGLE);

	anim_stzd = CreateAnimation(0.0);
	LogAnim("anim_stzd", anim_stzd);
	AddAnimationComponent(anim_stzd, 0.0, 1.0, &STZD_Open);

	pSTZDoorMotor->SetObjectAnim(anim_stzd);

	anim_styd = CreateAnimation(0.0);
	LogAnim("anim_styd", anim_styd);
	AddAnimationComponent(anim_styd, 0.0, 1.0, &STYD_Open);

	pSTYDoorMotor->SetObjectAnim(anim_styd);


	EndLoggingAnims();
}

void Atlantis::DefineAttachments(const VECTOR3& ofs0)
{

	if (ahHDP)
	{
		//when without ET, turn this into a ferry attachment "XFERRY"
		SetAttachmentParams(ahHDP, POS_HDP, _V(0.0, 0.0, -1.0),
			_V(0.0, 1.0, 0.0));
	}
	else
	{
		ahHDP = CreateAttachment(true, POS_HDP, _V(0.0, 0.0, -1.0),
			_V(0.0, 1.0, 0.0), "XHDP");
	}
	if (ahTow)
	{
		//Update position
		SetAttachmentParams(ahTow, POS_TOW, _V(0, 0.0749788, 0.997185),
			_V(0, 0.997185, -0.0749787));
	}
	else {
		//create new attachment
		ahTow = CreateAttachment(true, POS_TOW, _V(0, 0.0749788, 0.997185),
			_V(0, 0.997185, -0.0749787), "T");
	}

	if (pRMS) pRMS->CreateAttachment();
	// if RMS does not exist, create a placeholder attachment
	else CreateAttachment(false, _V(0, 0, 0), _V(1, 0, 0), _V(0, 1, 0), "INVALID");

	if (pMPMs) pMPMs->CreateAttachment();
	// if MPMs are not used, create a placeholder attachment
	else CreateAttachment(false, _V(0, 0, 0), _V(1, 0, 0), _V(0, 1, 0), "INVALID");


	//Without MMU, make this port airlock payload 
	if (ahMMU[0])
	{

	}
	else
	{
		ahMMU[0] = CreateAttachment(false, ofs0 + OFS_PORTMMU, _V(1, 0, 0), _V(0, 0, 1), "XS");
	}

	//Without MMU, make this stbd airlock payload 
	if (ahMMU[1])
	{
	}
	else
	{
		ahMMU[1] = CreateAttachment(false, ofs0 + OFS_STBDMMU, _V(-1, 0, 0), _V(0, 0, 1), "XS");
	}


	/*eva_docking::ODS* pODS = dynamic_cast<eva_docking::ODS*>(pExtAirlock);
	if (pODS) pODS->UpdateODSAttachment( ofs0 + _V( 0, 0, pMission->GetExternalAirlockZPos() ) );
	else */CreateAttachment( false, _V( 0, 0, 0 ), _V( 1, 0, 0 ), _V( 0, 1, 0 ), "INVALID" );
	

	/*
	dynamic centerline payloads, controlled by the payload 1-3 interfaces

	5. Payload 1
	6. Payload 2
	7. Payload 3.

	*/

	VECTOR3 vPayloadPos = _V(0.0, PL_ATTACH_CENTER_Y, 0.0);
	for (unsigned int i = 0; i < 3; i++)
	{
		vPayloadPos.z = pMission->GetPayloadZPos(i);

		pActiveLatches[i]->SetAttachmentParams(vPayloadPos, DIR_CENTERPL, ROT_CENTERPL);
		pActiveLatches[i]->CreateAttachment();
	}

	/*
	Static centerline Payloads:

	8. Static C/L payload 1
	9.	Static C/L payload 2
	10. Static C/L payload 3.
	11. Static C/L payload 4.
	*/
	vPayloadPos = _V(0.0, PL_ATTACH_CENTER_Y, 0.0);

	for (int i = 0; i < 4; i++)
	{
		vPayloadPos.z = pMission->GetPayloadZPos(i + 3);
		if (ahCenterPassive[i])
		{
			//update
			SetAttachmentParams(ahCenterPassive[i], ofs0 + vPayloadPos, DIR_CENTERPL,
				ROT_CENTERPL);
		}
		else
		{
			//create
			ahCenterPassive[i] = CreateAttachment(false, ofs0 + vPayloadPos, DIR_CENTERPL,
				ROT_CENTERPL, "XS");
		}
	}

	/*

	Pseudo static Port sill payloads (Are static inside the Shuttle, but can later get separated by EVA)

	12. Port static 1
	13. Port Static 2
	14. Port Static 3
	15. Port Static 4

	The same starboard

	16. Starboard Static 1
	17. Starboard Static 2
	18. Starboard Static 3
	19. Starboard Static 4
	*/

	vPayloadPos = _V(-PL_ATTACH_SIDE_X, PL_ATTACH_SIDE_Y, 0.0);

	for (int i = 0; i < 4; i++)
	{
		vPayloadPos.z = pMission->GetPayloadZPos(i + 7);
		if (ahPortPL[i])
		{
			//update
			SetAttachmentParams(ahPortPL[i], ofs0 + vPayloadPos, DIR_PORTPL,
				ROT_PORTPL);
		}
		else
		{
			//create
			ahPortPL[i] = CreateAttachment(false, ofs0 + vPayloadPos, DIR_PORTPL,
				ROT_PORTPL, "XS");
		}
	}

	vPayloadPos = _V(PL_ATTACH_SIDE_X, PL_ATTACH_SIDE_Y, 0.0);

	for (int i = 0; i < 4; i++)
	{
		vPayloadPos.z = pMission->GetPayloadZPos(i + 11);
		if (ahStbdPL[i])
		{
			//update
			SetAttachmentParams(ahStbdPL[i], ofs0 + vPayloadPos, DIR_STBDPL,
				ROT_STBDPL);
		}
		else
		{
			//create
			ahStbdPL[i] = CreateAttachment(false, ofs0 + vPayloadPos, DIR_STBDPL,
				ROT_STBDPL, "XS");
		}
	}

	/*
	20. ET
	21. LSRB
	22. RSRB
	*/
	CreateETAndSRBAttachments(ofs0);

	// only one will be created
	if (pASE_IUS) pASE_IUS->CreateAttachment();// 23
	else if (pCISS) pCISS->CreateAttachment();// 23
}

void Atlantis::CreateETAndSRBAttachments(const VECTOR3 &ofs)
{
	ahET = CreateAttachment(false, ET_OFFSET + ofs, _V(0, 1, 0), _V(0, 0, 1), "SSU_ET");
	ahLeftSRB = CreateAttachment(false, LSRB_OFFSET + ofs, _V(-1, 0, 0), _V(0, 0, 1), "SSU_SRB");
	ahRightSRB = CreateAttachment(false, RSRB_OFFSET + ofs, _V(-1, 0, 0), _V(0, 0, 1), "SSU_SRB");
	char pszBuf[255];
	sprintf_s(pszBuf, 255, "Attachment count: %d", AttachmentCount(false));
	oapiWriteLog(pszBuf);
}

void Atlantis::AddOrbiterVisual()
{
	huds.hudcnt = _V(-0.671257, 2.523535, 14.969);

	bHasODS = pMission->HasODS();
	bHasExtAL = pMission->HasExtAL();

	if (mesh_orbiter == MESH_UNDEFINED) {

		// ***** Load meshes
		if (pCISS) pCISS->AddMesh();
		if (pASE_IUS) pASE_IUS->AddMesh();

		mesh_cockpit = AddMesh(hOrbiterCockpitMesh, &VC_OFFSET);
		SetMeshVisibilityMode(mesh_cockpit, MESHVIS_EXTERNAL);

		mesh_orbiter = AddMesh(hOrbiterMesh, &OFS_ZERO);
		SetMeshVisibilityMode(mesh_orbiter, MESHVIS_EXTERNAL | MESHVIS_VC | MESHVIS_EXTPASS);

		if (pMission->UseSILTS() == true)
		{
			hSILTSMesh = oapiLoadMeshGlobal(SILTS_MESHNAME);
			mesh_SILTS = AddMesh(hSILTSMesh, &SILTS_OFFSET);
			SetMeshVisibilityMode(mesh_SILTS, MESHVIS_EXTERNAL | MESHVIS_VC | MESHVIS_EXTPASS);
			oapiWriteLog("SILTS pod mesh added");
		}

		//ADD REENTRY MESH
		oapiWriteLog("OFFSET REENTRY MESH SET");
		mesh_heatshield = AddMesh(hHeatShieldMesh, &ENTRY_OFFSET);
		oapiWriteLog("REENTRY MESH ADDED");

		mesh_vc = AddMesh(hOrbiterVCMesh, &VC_OFFSET);
		SetMeshVisibilityMode(mesh_vc, MESHVIS_VC);

		AddKUBandVisual(KU_OFFSET);

		if (pExtAirlock)
		{
			pExtAirlock->AddMeshes( _V( 0, 0, pMission->GetExternalAirlockZPos() ) );
			
			eva_docking::ODS* pODS = dynamic_cast<eva_docking::ODS*>(pExtAirlock);
			if (pODS) pODS->SetDockParams( pMission->GetExternalAirlockZPos() );
			
			pExtAirlock->DefineAnimations( _V( 0, 0, pMission->GetExternalAirlockZPos() ) );
		}

		if (pTAA)
		{
			pTAA->AddMeshes( _V( 0, 0, pMission->GetTunnelAdapterAssemblyZPos() ) );
			//pTAA->DefineAnimations( _V( 0, 0, pMission->GetTunnelAdapterAssemblyZPos() ) );
		}

		mesh_middeck = AddMesh(hMidDeckMesh, &VC_OFFSET);
		//Only make visible when actually inside the mid deck
		bMidDeckVisible = false;
		SetMeshVisibilityMode(mesh_middeck, MESHVIS_NEVER);

		VECTOR3 chute_ofs = CHUTE_ATTACH_POINT;
		mesh_dragchute = AddMesh(hDragChuteMesh, &chute_ofs);
		SetMeshVisibilityMode(mesh_dragchute, MESHVIS_NEVER);

		/* Add optional A7A3/A8A3 panel meshes
		*/
		if (pA7A8Panel) pA7A8Panel->AddMeshes(VC_OFFSET);
		if (pPanelA8) pPanelA8->AddMeshes(VC_OFFSET);
		if (pPanelL10) pPanelL10->AddMeshes( VC_OFFSET );
		if (pPanelL12U) pPanelL12U->AddMeshes( VC_OFFSET );

		pgForward.DefineVC();
		pgForward.DefineVCAnimations(mesh_vc);

		pgLeft.DefineVC();
		pgLeft.DefineVCAnimations(mesh_vc);

		pgCenter.DefineVC();
		pgCenter.DefineVCAnimations(mesh_vc);

		pgOverhead.DefineVC();
		pgOverhead.DefineVCAnimations(mesh_vc);

		pgOverheadAft.DefineVC();
		pgOverheadAft.DefineVCAnimations(mesh_vc);

		pgRight.DefineVC();
		pgRight.DefineVCAnimations(mesh_vc);

		pgAft.DefineVC();
		pgAft.DefineVCAnimations(mesh_vc);

		pgAftStbd.DefineVC();
		pgAftStbd.DefineVCAnimations(mesh_vc);

		pgAftPort.DefineVC();
		pgAftPort.DefineVCAnimations(mesh_vc);

		huds.nmesh = mesh_vc;

		if (do_cargostatic) {
			VECTOR3 cofs = cargo_static_ofs;
			if (mesh_cargo_static == MESH_UNDEFINED)
			{
				mesh_cargo_static = AddMesh(cargo_static_mesh_name, &cofs);
			}
			else {
				InsertMesh(cargo_static_mesh_name, mesh_cargo_static, &cofs);
			}
			SetMeshVisibilityMode(mesh_cargo_static, MESHVIS_EXTERNAL | MESHVIS_VC | MESHVIS_EXTPASS);
		}
		if (do_plat) {
			VECTOR3 plat_ofs = _V(-2.59805, 1.69209, -5.15524);
			AddMesh("shuttle_eva_plat", &plat_ofs);
		}

		// ***** Attachment definitions
		DefineAttachments(OFS_ZERO);

		// ***** Cockpit camera definition

		SetCameraOffset(_V(-0.67, 2.55, 14.4));
		oapiVCRegisterHUD(&huds); // register changes in HUD parameters

		DefineAnimations();
	}
}

void Atlantis::SeparateBoosters(double met)
{
	char buffer[120];

	double thrust_level, prop_level;
	GetSRB_State(met, thrust_level, prop_level);
	prop_level = GetPropellantMass(ph_srb) / GetPropellantMaxMass(ph_srb);

	DetachSRB(LEFT, thrust_level, prop_level);
	DetachSRB(RIGHT, thrust_level, prop_level);

	sprintf(buffer, "MG_Atlantis: Residual SRB propellant mass is %f kg\n", GetPropellantMass(ph_srb));
	oapiWriteLog(buffer);

	// Remove SRB's from Shuttle instance
	DelPropellantResource(ph_srb);
	DelThrusterGroup(thg_srb, THGROUP_USER, true);
	for (int i = 0; i < 2; i++)
	{
		DelExhaustStream(pshSlag1[i]);
		DelExhaustStream(pshSlag2[i]);
		DelExhaustStream(pshSlag3[i]);
		pshSlag1[i] = NULL;
		pshSlag2[i] = NULL;
		pshSlag3[i] = NULL;
	}

	// reconfigure
	SetOrbiterTankConfiguration();
	char cbuf[255];
	sprintf(cbuf, "Boosters separated");
	oapiWriteLog(cbuf);

	//stop playing sound
	StopVesselWave(SoundID, SSME_RUNNING);

	// change ET texture (TODO: uncomment this code once ET burn texture is fixed)
	/*OBJHANDLE hTank = GetAttachmentStatus(ahET);
	if(hTank) {
	Atlantis_Tank* pTank = static_cast<Atlantis_Tank*>(oapiGetVesselInterface(hTank));
	pTank->UseBurntETTexture();
	}*/
}

void Atlantis::DetachSRB(SIDE side, double thrust, double prop)
{
	Atlantis_SRB* pSRB = GetSRBInterface(side);
	if (side == LEFT) DetachChildAndUpdateMass(ahLeftSRB);
	else DetachChildAndUpdateMass(ahRightSRB);
	pSRB->SetPostSeparationState(t0, thrust, prop);
}

void Atlantis::SeparateTank(void)
{
	DetachChildAndUpdateMass(ahET, 0.0);

	// create separate tanks for MPS dumps
	// using remaining mass in manifold to estimate LO2 & LH2 masses
	phLOXdump = CreatePropellantResource(LOXmass);
	phLH2dump = CreatePropellantResource(LH2mass);
	CreateMPSDumpVents();

	// delete joint mps manifold
	DelPropellantResource(ph_mps);

	// reconfigure
	SetOrbiterConfiguration();
}

bool Atlantis::HasTank() const
{
	return (status <= STATE_STAGE2);
}

Atlantis_Tank* Atlantis::GetTankInterface() const
{
	OBJHANDLE hTank = GetAttachmentStatus(ahET);
	if (hTank)
		return static_cast<Atlantis_Tank*>(oapiGetVesselInterface(hTank));
	return NULL;
}

Atlantis_SRB* Atlantis::GetSRBInterface(SIDE side) const
{
	OBJHANDLE hSRB = NULL;
	if (side == LEFT) hSRB = GetAttachmentStatus(ahLeftSRB);
	else hSRB = GetAttachmentStatus(ahRightSRB);
	if (hSRB)
		return static_cast<Atlantis_SRB*>(oapiGetVesselInterface(hSRB));
	return NULL;
}

ISSUMLP* Atlantis::GetMLPInterface() const
{
	if (ahHDP)
	{
		OBJHANDLE hMLP = GetAttachmentStatus(ahHDP);
		if (hMLP)
		{
			VESSEL* pV = oapiGetVesselInterface(hMLP);
			if (pV)
			{
				if (!_stricmp(pV->GetClassName(), "SSU_MLP")) return static_cast<MLP*>(pV);
				else if (!_stricmp(pV->GetClassName(), "SSU_SLC6")) return static_cast<SLC6*>(pV);
			}
		}
	}
	return NULL;
}

void Atlantis::ToggleGrapple(void)
{
	if (!RMS) return; //no arm
	if (pRMS->Grappled()) {  // release satellite
		pRMS->Ungrapple();
	}
	else {             // grapple satellite
		ATTACHMENTHANDLE hV = pRMS->Grapple();
		if (hV) {
			//increase mass of shuttle
			/*pl_mass+=oapiGetMass(hV);
			//oapiWriteLog("pl_mass increased");
			SetEmptyMass(ORBITER_EMPTY_MASS+pl_mass);*/
		}
	}
}

void Atlantis::ToggleVCMode()
{
	switch (vcDeckMode)
	{
	case VCM_FLIGHTDECK:
		oapiSetPanel(VC_MIDDECK);
		vcDeckMode = VCM_MIDDECK;
		break;
	case VCM_MIDDECK:
		oapiSetPanel(VC_PORTSTATION);
		vcDeckMode = VCM_FLIGHTDECK;
		break;
	}
}

void Atlantis::SeparateMMU(void)
{
	// Create MMU as individual object
	VESSELSTATUS vs;
	GetStatus(vs);
	char name[256];
	strcpy(name, GetName()); strcat(name, "-MMU");
	hMMU = oapiCreateVessel(name, "Nasa_MMU", vs);
	jettison_time = oapiGetSimTime();
	reset_mmu = true;
	// Remove MMU from shuttle instance
	SetOrbiterConfiguration();
}

bool Atlantis::HydraulicsOK() {
	return panelr2->HydraulicPressure();
}

void Atlantis::GimbalOMS(int engine, double pitch, double yaw)
{
	if (abs(pitch) < 6.0 && abs(yaw) < 7.0) {
		//OMSGimbal[engine][0]=pitch;
		//OMSGimbal[engine][1]=yaw;

		/*VECTOR3 dir;
		if(engine==0) dir=L_OMS_DIR;
		else dir=R_OMS_DIR;

		dir=RotateVectorX(dir, -pitch); //positive OMS gimbal directs thrust downwards
		dir=RotateVectorY(dir, -yaw); //positive yaw gimbal directs thrust to right*/
		VECTOR3 dir = CalcOMSThrustDir(engine, pitch, yaw);

		SetThrusterDir(th_oms[engine], dir);

		curOMSPitch[engine] = pitch;
		curOMSYaw[engine] = yaw;

		char cbuf[255];
		sprintf_s(cbuf, 255, "OMS DIR: %d %f %f %f", engine, dir.x, dir.y, dir.z);
		oapiWriteLog(cbuf);
	}
}

double Atlantis::CalcNetSSMEThrust() const
{
	VECTOR3 N = _V(0, 0, 0), F, M;
	for (int i = 0; i < 3; i++) {
		GetThrusterMoment(th_main[i], F, M);
		N += F;
	}
	return length(N);
}

double Atlantis::GetSSMEISP() const
{
	return GetThrusterIsp(th_main[0]);
}

void Atlantis::CalcSSMEThrustAngles(int eng, double& degAngleP, double& degAngleY) const
{
	VECTOR3 N = _V(0, 0, 0);
	VECTOR3 dir = _V(0, 0, 0);
	if (eng == 0)
	{
		for (int i = 0; i < 3; i++) {
			GetThrusterRef(th_main[i], dir);
			dir = Normalize(-dir);
			N += dir*GetThrusterLevel(th_main[i]);
		}
	}
	else
	{
		GetThrusterRef(th_main[eng - 1], dir);
		N = -dir;
	}
	degAngleP = DEG*atan2(N.y, N.z);
	degAngleY = -DEG*atan2(cos(RAD*degAngleP) * N.x, N.z);
}

void Atlantis::LaunchClamps()
{
	VECTOR3 F, T, r = _V(0, 0, 0), Fc = _V(0, 0, 0), Tc = _V(0, 0, 0);
	GetThrusterMoment(th_srb[0], F, T);
	Fc.z = -2 * F.z;
	Tc.x = 2 * T.x;
	GetThrusterMoment(th_main[0], F, T);
	Fc.z -= 2 * F.z;
	Fc.y -= 2 * F.y;
	Tc.x += 2 * T.x;
	GetThrusterMoment(th_main[2], F, T);
	Fc.z -= F.z;
	Fc.y -= F.y;
	Tc.x += T.x;
	r.z = (Fc.y ? Tc.x / Fc.y : 0);
	AddForce(Fc, r);
}


// Update moving parts of the orbiter's visual: payload bay doors and gear
// This should only be called when the visual exists, e.g. from within
// clbkVisualCreated or clbkAnimate

void Atlantis::UpdateMesh()
{
	// update animation states
	SetAnimation(anim_spdb, spdb_proc);
	SetAnimation(anim_door, pPayloadBay->BayDoorStatus.pos);
	for (int i = 0; i < 4; i++) SetAnimation(anim_clatch[i], pPayloadBay->CLBayDoorLatch[i].pos);
	SetAnimation(anim_rad[0], pPayloadBay->RadiatorStatus[0].pos);
	SetAnimation(anim_rad[1], pPayloadBay->RadiatorStatus[1].pos);
	SetAnimation( anim_kubd, pPayloadBay->KuAntennaStatus.pos );
	if (pDeployedAssembly)
	{
		SetAnimation( anim_kualpha, pDeployedAssembly->GetAlphaAnimation() );
		SetAnimation( anim_kubeta, pDeployedAssembly->GetBetaAnimation() );
	}
	SetAnimation(anim_gear, gear_status.pos);
	SetAnimation(anim_chute_deploy, 1 - DragChuteSize);

	SetAnimationCameras(); // update camera positions


	// update MFD brightness
	/*if (vis) {
		int i;
		MESHHANDLE hMesh = GetMesh(vis, mesh_vc);
		for (i = 0; i < 10; i++) {
			MATERIAL *mat = oapiMeshMaterial(hMesh, 10 + i);
			mat->emissive.r = mat->emissive.g = mat->emissive.b = (float)mfdbright[i];
		}
	}*/
}

void Atlantis::ShowMidDeck()
{
	if (!bMidDeckVisible)
	{
		SetMeshVisibilityMode(mesh_middeck, MESHVIS_VC);
		bMidDeckVisible = true;
	}
}
void Atlantis::HideMidDeck()
{
	if (bMidDeckVisible)
	{
		SetMeshVisibilityMode(mesh_middeck, MESHVIS_NEVER);
		bMidDeckVisible = false;
	}
}

void Atlantis::DeployLandingGear()
{
	if (status == STATE_ORBITER && GearArmed() && gear_status.action != AnimState::OPEN) {
		gear_status.action = AnimState::OPENING;
		LandingGearArmDeployLT[1].SetLine();
		RecordEvent("GEAR", "DOWN");
	}
}

void Atlantis::RetractLandingGear()
{
	if (status == STATE_ORBITER && GearArmed()) {
		gear_status.action = AnimState::CLOSING;
		RecordEvent("GEAR", "UP");
	}
}

void Atlantis::ArmGear()
{
	gear_armed = true;
	LandingGearArmDeployLT[0].SetLine();
}

bool Atlantis::GearArmed() const
{
	return gear_armed;
}

void Atlantis::DeployDragChute()
{
	SetAnimation(anim_chute_deploy, 1.0);
	SetMeshVisibilityMode(mesh_dragchute, MESHVIS_EXTERNAL);

	DragChuteState = DEPLOYING;
	DragChuteSize = 0.0;
}

void Atlantis::JettisonDragChute()
{
	SetMeshVisibilityMode(mesh_dragchute, MESHVIS_NEVER);

	DragChuteState = JETTISONED;
	DragChuteSize = 0.0;

	//add chute vessel
	VESSELSTATUS2 vs;
	memset(&vs, 0, sizeof(vs));
	vs.version = 2;
	GetStatusEx(&vs);
	vs.status = 0;
	VECTOR3 chute_ofs;
	Local2Rel(CHUTE_ATTACH_POINT, chute_ofs);
	vs.rpos = chute_ofs;
	char name[255];
	strcpy_s(name, GetName());
	strcat_s(name, "-Chute");
	oapiCreateVesselEx(name, "SSU_Chute", &vs);

	DragChuteARMDPYJETTLT[2].SetLine();
}

void Atlantis::DefineTouchdownPoints()
{
	if (gear_status.action == AnimState::OPEN) { // gear fully deployed
		SetTouchdownPoints(_V(0, -5.142, 14.897) + orbiter_ofs, _V(-4.027, -6.607, -5.3) + orbiter_ofs, _V(4.027, -6.607, -5.3) + orbiter_ofs); // gear wheel tips
		SetSurfaceFrictionCoeff(0.035 / 2, 0.5);
	}
	else {
		SetTouchdownPoints(_V(0, -2.5, 14) + orbiter_ofs, _V(-8, -2.8, -9) + orbiter_ofs, _V(8, -2.8, -9) + orbiter_ofs); // belly landing
		SetSurfaceFrictionCoeff(0.4, 0.4);
	}
}

void Atlantis::ClearMeshes()
{
	VESSEL::ClearMeshes();
	mesh_orbiter = MESH_UNDEFINED;
	mesh_kuband = MESH_UNDEFINED;
	mesh_cockpit = MESH_UNDEFINED;
	mesh_vc = MESH_UNDEFINED;
}

void Atlantis::SetBayDoorPosition(double pos)
{
	SetAnimation(anim_door, pos);
	rdoor_drag = sqrt(min(1.0, pos*3.0));
	ldoor_drag = sqrt(min(1.0, max(0.0, pos - 0.3656)*3.0));
	//SetAnimation (anim_portTS, max(0.0, (pos - 0.5)/0.5));
}

void Atlantis::SetBayDoorLatchPosition(int gang, double pos)
{
	SetAnimation(anim_clatch[gang], pos);
}

void Atlantis::SetRadiatorPosition(double pos, int side)
{
	SetAnimation(anim_rad[side], pos);
}

void Atlantis::SetKuAntennaDAPosition(double pos)
{
	SetAnimation(anim_kubd, pos);
}

void Atlantis::SetETUmbDoorPosition(double pos, int door)
{
	if (door == 0) SetAnimation(anim_letumbdoor, pos);
	else SetAnimation(anim_retumbdoor, pos);
}

void Atlantis::OperateSpeedbrake(AnimState::Action action)
{
	spdb_status = action;
	RecordEvent("SPEEDBRAKE", action == AnimState::CLOSING ? "CLOSE" : "OPEN");
}

void Atlantis::SetSpeedbrake(double tgt)
{
	spdb_tgt = tgt;
	if (spdb_tgt<spdb_proc) OperateSpeedbrake(AnimState::CLOSING);
	else if (spdb_tgt>spdb_proc) OperateSpeedbrake(AnimState::OPENING);
}

void Atlantis::SetAnimationCameras() {
	// FRONT LEFT
	double anim_yaw = linterp(-170, 0, 170, 1, camYaw[CAM_A]);
	SetAnimation(anim_camFLyaw, anim_yaw);

	double anim_pitch = linterp(-170, 0, 170, 1, camPitch[CAM_A]);
	SetAnimation(anim_camFLpitch, anim_pitch);

	// FRONT RIGHT
	anim_yaw = linterp(-170, 0, 170, 1, camYaw[CAM_D]);
	SetAnimation(anim_camFRyaw, anim_yaw);

	anim_pitch = linterp(-170, 0, 170, 1, camPitch[CAM_D]);
	SetAnimation(anim_camFRpitch, anim_pitch);

	// BACK LEFT
	anim_yaw = linterp(-170, 0, 170, 1, camYaw[CAM_B]);
	SetAnimation(anim_camBLyaw, anim_yaw);

	anim_pitch = linterp(-170, 0, 170, 1, camPitch[CAM_B]);
	SetAnimation(anim_camBLpitch, anim_pitch);

	// BACK RIGHT
	anim_yaw = linterp(-170, 0, 170, 1, camYaw[CAM_C]);
	SetAnimation(anim_camBRyaw, anim_yaw);

	anim_pitch = linterp(-170, 0, 170, 1, camPitch[CAM_C]);
	SetAnimation(anim_camBRpitch, anim_pitch);

	if (oapiCameraInternal() && VCMode >= VC_PLBCAMFL && VCMode <= VC_PLBCAMFR) {
		double a = 0;
		double b = 0;

		switch (VCMode) {
		case VC_PLBCAMFL:
			a = ((-camYaw[CAM_A] + 90)*RAD);
			b = ((camPitch[CAM_A] - 90)*RAD);
			break;
		case VC_PLBCAMFR:
			a = ((-camYaw[CAM_D] + 90)*RAD);
			b = ((camPitch[CAM_D] - 90)*RAD);
			break;
		case VC_PLBCAMBL:
			a = ((-camYaw[CAM_B] - 90)*RAD);
			b = ((camPitch[CAM_B] - 90)*RAD);
			break;
		case VC_PLBCAMBR:
			a = ((-camYaw[CAM_C] - 90)*RAD);
			b = ((camPitch[CAM_C] - 90)*RAD);
			break;
		}
		SetCameraOffset(orbiter_ofs + plbdCamPos[VCMode - VC_PLBCAMFL]);
		SetCameraDefaultDirection(_V(cos(a)*sin(b), cos(b), sin(a)*sin(b)));
		oapiCameraSetCockpitDir(0.0, 0.0);
	}
}

LightEmitter* Atlantis::AddPayloadBayLight(VECTOR3& pos, VECTOR3& dir, double degWidth, BEACONLIGHTSPEC& bspec)
{
	static VECTOR3 color = _V(0.75, 0.75, 0.75);
	//const COLOUR4 diff = {0.949f, 0.988f, 1.0f, 0.0f}; //RGB for metal halide but it doesn't quite match up with actual photos
	const COLOUR4 diff = { 0.847f, 0.968f, 1.0f, 0.0f }; //RGB for mercury vapor, this better matches photos
	const COLOUR4 amb = { 0.0, 0.0, 0 };
	const COLOUR4 spec = { 0.0f, 0.0f, 0.0f, 0 };

	bspec.active = false;
	bspec.col = &color;
	bspec.duration = 0;
	bspec.falloff = 0.4;
	bspec.period = 0;
	bspec.pos = &pos;
	bspec.shape = BEACONSHAPE_DIFFUSE;
	bspec.size = 0.25;
	bspec.tofs = 0;
	AddBeacon(&bspec);
	return AddSpotLight(pos, dir, 20, 0.5, 0.0, 0.05, degWidth*RAD, degWidth*1.1*RAD,
		diff, spec, amb);
}

void Atlantis::EnableThrusters(const int Thrusters[], int nThrusters)
{
	if (bUseRealRCS)
	{
		//th_att_rcs[0] = CreateThruster(_V(0, 0, 15.5), _V(0, 1, 0), 
		//th_att_rcs[1] = CreateThruster(_V(0, 0, -15.5), _V(0, -1, 0),
		//th_att_rcs[2] = CreateThruster(_V(0, 0, 15.5), _V(0, -1, 0), 
		//th_att_rcs[3] = CreateThruster(_V(0, 0, -15.5), _V(0, 1, 0), 
		/*th_att_rcs[4] = CreateThruster(_V(0, 0, 15.5), _V(-1, 0, 0),
		th_att_rcs[5] = CreateThruster(_V(0, 0, -15.5), _V(1, 0, 0),
		th_att_rcs[6] = CreateThruster(_V(0, 0, 15.5), _V(1, 0, 0),
		th_att_rcs[7] = CreateThruster(_V(0, 0, -15.5), _V(-1, 0, 0),
		th_att_rcs[8] = CreateThruster(_V(2.7, 0, 0), _V(0, 1, 0), ORBITER_RCS_THRUST, ph_frcs,
		th_att_rcs[9] = CreateThruster(_V(-2.7, 0, 0), _V(0, -1, 0), ORBITER_RCS_THRUST, ph_oms,
		th_att_rcs[10] = CreateThruster(_V(-2.7, 0, 0), _V(0, 1, 0), ORBITER_RCS_THRUST, ph_frcs,
		th_att_rcs[11] = CreateThruster(_V(2.7, 0, 0), _V(0, -1, 0), ORBITER_RCS_THRUST, ph_oms,

		*/
		for (int i = 0; i < nThrusters; i++) {
			switch (Thrusters[i]) {
			case 0:
				//Enable FxD
				SetThrusterResource(thFRCS[RCS_F1D], ph_frcs);
				SetThrusterResource(thFRCS[RCS_F2D], ph_frcs);
				SetThrusterResource(thFRCS[RCS_F3D], ph_frcs);
				SetThrusterResource(thFRCS[RCS_F4D], ph_frcs);
				break;
			case 1:
				//Enable L(R)xU
				SetThrusterResource(thLRCS[RCS_L1U], ph_lrcs);
				SetThrusterResource(thLRCS[RCS_L2U], ph_lrcs);
				SetThrusterResource(thLRCS[RCS_L4U], ph_lrcs);
				SetThrusterResource(thRRCS[RCS_R1U], ph_rrcs);
				SetThrusterResource(thRRCS[RCS_R2U], ph_rrcs);
				SetThrusterResource(thRRCS[RCS_R4U], ph_rrcs);
				break;
			case 2:
				//Enable FxU
				SetThrusterResource(thFRCS[RCS_F1U], ph_frcs);
				SetThrusterResource(thFRCS[RCS_F2U], ph_frcs);
				SetThrusterResource(thFRCS[RCS_F3U], ph_frcs);
				break;
			case 3:
				//Enable L(R)xD
				SetThrusterResource(thLRCS[RCS_L3D], ph_lrcs);
				SetThrusterResource(thLRCS[RCS_L2D], ph_lrcs);
				SetThrusterResource(thLRCS[RCS_L4D], ph_lrcs);
				SetThrusterResource(thRRCS[RCS_R3D], ph_rrcs);
				SetThrusterResource(thRRCS[RCS_R2D], ph_rrcs);
				SetThrusterResource(thRRCS[RCS_R4D], ph_rrcs);
				break;
			case 4:
				//Enable FxL
				SetThrusterResource(thFRCS[RCS_F1L], ph_frcs);
				SetThrusterResource(thFRCS[RCS_F3L], ph_frcs);
				break;
			case 5:
				//Enable RxR

				SetThrusterResource(thRRCS[RCS_R3R], ph_rrcs);
				SetThrusterResource(thRRCS[RCS_R1R], ph_rrcs);
				SetThrusterResource(thRRCS[RCS_R2R], ph_rrcs);
				SetThrusterResource(thRRCS[RCS_R4R], ph_rrcs);
				break;
			case 6:
				//Enable FxR
				SetThrusterResource(thFRCS[RCS_F2R], ph_frcs);
				SetThrusterResource(thFRCS[RCS_F4R], ph_frcs);
				break;
			case 7:
				//Enable LxL
				SetThrusterResource(thLRCS[RCS_L3L], ph_lrcs);
				SetThrusterResource(thLRCS[RCS_L1L], ph_lrcs);
				SetThrusterResource(thLRCS[RCS_L2L], ph_lrcs);
				SetThrusterResource(thLRCS[RCS_L4L], ph_lrcs);
				break;
			case 8:
				//Enable front bank left
				SetThrusterResource(thFRCS[RCS_F2D], ph_frcs);
				SetThrusterResource(thFRCS[RCS_F4D], ph_frcs);
				break;
			case 9:
				//Enable aft bank left
				SetThrusterResource(thRRCS[RCS_R3D], ph_rrcs);
				SetThrusterResource(thRRCS[RCS_R4D], ph_rrcs);
				SetThrusterResource(thRRCS[RCS_R2D], ph_rrcs);
				SetThrusterResource(thLRCS[RCS_L4U], ph_lrcs);
				SetThrusterResource(thLRCS[RCS_L2U], ph_lrcs);
				SetThrusterResource(thLRCS[RCS_L1U], ph_lrcs);
				break;
			case 10:
				//Enable front bank right
				SetThrusterResource(thFRCS[RCS_F2R], ph_frcs);
				SetThrusterResource(thFRCS[RCS_F4R], ph_frcs);
				break;
			case 11:
				//Enable aft bank right
				SetThrusterResource(thRRCS[RCS_R1U], ph_rrcs);
				SetThrusterResource(thRRCS[RCS_R4U], ph_rrcs);
				SetThrusterResource(thRRCS[RCS_R2U], ph_rrcs);
				SetThrusterResource(thLRCS[RCS_L4D], ph_lrcs);
				SetThrusterResource(thLRCS[RCS_L2D], ph_lrcs);
				SetThrusterResource(thLRCS[RCS_L3D], ph_lrcs);
				break;
			}
		}
	}
	else
	{
		//TODO: Possible Crash
		for (int i = 0; i < nThrusters; i++) {
			if (Thrusters[i] % 2 == 1)
				SetThrusterResource(th_att_rcs[Thrusters[i]], ph_oms);
			else
				SetThrusterResource(th_att_rcs[Thrusters[i]], ph_frcs);
		}
	}

}

void Atlantis::DisableThrusters(const int Thrusters[], int nThrusters)
{
	if (bUseRealRCS) {
		for (int i = 0; i < nThrusters; i++) {
			switch (Thrusters[i]) {
			case 0:
				//Disable FxD
				SetThrusterResource(thFRCS[RCS_F1D], NULL);
				SetThrusterResource(thFRCS[RCS_F2D], NULL);
				SetThrusterResource(thFRCS[RCS_F3D], NULL);
				SetThrusterResource(thFRCS[RCS_F4D], NULL);
				break;
			case 1:
				//Enable L(R)xU
				SetThrusterResource(thLRCS[RCS_L1U], NULL);
				SetThrusterResource(thLRCS[RCS_L2U], NULL);
				SetThrusterResource(thLRCS[RCS_L4U], NULL);
				SetThrusterResource(thRRCS[RCS_R1U], NULL);
				SetThrusterResource(thRRCS[RCS_R2U], NULL);
				SetThrusterResource(thRRCS[RCS_R4U], NULL);
				break;
			case 2:
				//Enable FxU
				SetThrusterResource(thFRCS[RCS_F1U], NULL);
				SetThrusterResource(thFRCS[RCS_F2U], NULL);
				SetThrusterResource(thFRCS[RCS_F3U], NULL);
				break;
			case 3:
				//Enable L(R)xD
				SetThrusterResource(thLRCS[RCS_L3D], NULL);
				SetThrusterResource(thLRCS[RCS_L2D], NULL);
				SetThrusterResource(thLRCS[RCS_L4D], NULL);
				SetThrusterResource(thRRCS[RCS_R3D], NULL);
				SetThrusterResource(thRRCS[RCS_R2D], NULL);
				SetThrusterResource(thRRCS[RCS_R4D], NULL);
				break;
			case 4:
				//Enable FxL
				SetThrusterResource(thFRCS[RCS_F1L], NULL);
				SetThrusterResource(thFRCS[RCS_F3L], NULL);
				break;
			case 5:
				//Enable RxR

				SetThrusterResource(thRRCS[RCS_R3R], NULL);
				SetThrusterResource(thRRCS[RCS_R1R], NULL);
				SetThrusterResource(thRRCS[RCS_R2R], NULL);
				SetThrusterResource(thRRCS[RCS_R4R], NULL);
				break;
			case 6:
				//Enable FxR
				SetThrusterResource(thFRCS[RCS_F2R], NULL);
				SetThrusterResource(thFRCS[RCS_F4R], NULL);
				break;
			case 7:
				//Enable LxL
				SetThrusterResource(thLRCS[RCS_L3L], NULL);
				SetThrusterResource(thLRCS[RCS_L1L], NULL);
				SetThrusterResource(thLRCS[RCS_L2L], NULL);
				SetThrusterResource(thLRCS[RCS_L4L], NULL);
				break;
			case 8:
				//Enable front bank left
				SetThrusterResource(thFRCS[RCS_F2D], NULL);
				SetThrusterResource(thFRCS[RCS_F4D], NULL);
				break;
			case 9:
				//Enable aft bank left
				SetThrusterResource(thRRCS[RCS_R3D], NULL);
				SetThrusterResource(thRRCS[RCS_R4D], NULL);
				SetThrusterResource(thRRCS[RCS_R2D], NULL);
				SetThrusterResource(thLRCS[RCS_L4U], NULL);
				SetThrusterResource(thLRCS[RCS_L2U], NULL);
				SetThrusterResource(thLRCS[RCS_L1U], NULL);
				break;
			case 10:
				//Enable front bank right
				SetThrusterResource(thFRCS[RCS_F2R], NULL);
				SetThrusterResource(thFRCS[RCS_F4R], NULL);
				break;
			case 11:
				//Enable aft bank right
				SetThrusterResource(thRRCS[RCS_R1U], NULL);
				SetThrusterResource(thRRCS[RCS_R4U], NULL);
				SetThrusterResource(thRRCS[RCS_R2U], NULL);
				SetThrusterResource(thLRCS[RCS_L4D], NULL);
				SetThrusterResource(thLRCS[RCS_L2D], NULL);
				SetThrusterResource(thLRCS[RCS_L3D], NULL);
				break;
			}
		}
	}
	else
	{
		//TODO: Possible Crash!
		for (int i = 0; i < nThrusters; i++) {
			SetThrusterResource(th_att_rcs[Thrusters[i]], NULL);
		}
	}
}

void Atlantis::UpdateTranslationForces()
{
	TransForce[0].x = GetThrusterGroupMaxThrust(thg_transfwd);
	TransForce[1].x = GetThrusterGroupMaxThrust(thg_transaft);
	TransForce[0].y = GetThrusterGroupMaxThrust(thg_transright);
	TransForce[1].y = GetThrusterGroupMaxThrust(thg_transleft);
	TransForce[0].z = GetThrusterGroupMaxThrust(thg_transdown);
	TransForce[1].z = GetThrusterGroupMaxThrust(thg_transup);
}

double Atlantis::GetThrusterGroupMaxThrust(THGROUP_HANDLE thg) const
{
	VECTOR3 Total = _V(0.0, 0.0, 0.0), Dir;
	for (DWORD i = 0; i < GetGroupThrusterCount(thg); i++) {
		THRUSTER_HANDLE th = GetGroupThruster(thg, i);
		GetThrusterDir(th, Dir);
		Total += Dir*GetThrusterMax0(th);
	}
	return length(Total);
}

double Atlantis::GetPropellantLevel(PROPELLANT_HANDLE ph) const
{
	return 100.0*(GetPropellantMass(ph) / GetPropellantMaxMass(ph));
}

void Atlantis::UpdateHandControllerSignals()
{
	//get THC and RHC input
	if (ControlRMS) { // use RHC/THC input to control RMS
		RMS_RHCInput[PITCH].SetLine(5.0f*(float)(GetThrusterGroupLevel(THGROUP_ATT_PITCHUP) - GetThrusterGroupLevel(THGROUP_ATT_PITCHDOWN)));
		RMS_RHCInput[YAW].SetLine(5.0f*(float)(GetThrusterGroupLevel(THGROUP_ATT_YAWRIGHT) - GetThrusterGroupLevel(THGROUP_ATT_YAWLEFT)));
		RMS_RHCInput[ROLL].SetLine(5.0f*(float)(GetThrusterGroupLevel(THGROUP_ATT_BANKRIGHT) - GetThrusterGroupLevel(THGROUP_ATT_BANKLEFT)));
		if (!ControlSurfacesEnabled && GetAttitudeMode() == RCS_ROT) { // use arrow, Ins/Del keys for translation input
			RMS_THCInput[0].SetLine(5.0f*(float)(AltKybdInput.x));
			RMS_THCInput[1].SetLine(5.0f*(float)(AltKybdInput.y));
			RMS_THCInput[2].SetLine(5.0f*(float)(AltKybdInput.z));
		}
		else {
			RMS_THCInput[0].SetLine(5.0f*(float)(GetThrusterGroupLevel(THGROUP_ATT_FORWARD) - GetThrusterGroupLevel(THGROUP_ATT_BACK)));
			RMS_THCInput[1].SetLine(5.0f*(float)(GetThrusterGroupLevel(THGROUP_ATT_RIGHT) - GetThrusterGroupLevel(THGROUP_ATT_LEFT)));
			RMS_THCInput[2].SetLine(5.0f*(float)(GetThrusterGroupLevel(THGROUP_ATT_DOWN) - GetThrusterGroupLevel(THGROUP_ATT_UP)));
		}
		for (int i = 0; i<3; i++) {
			RHCInput.data[i] = 0.0;
			THCInput.data[i] = 0.0;
		}

		// use RHC pitch commands to drive single joint input
		// in theory, these discrete lines should only be set if RMS is in SINGLE or DIRECT mode
		// in practice, RMS code only looks at these discrete lines if in appropriate mode
		if ((GetThrusterGroupLevel(THGROUP_ATT_PITCHUP) - GetThrusterGroupLevel(THGROUP_ATT_PITCHDOWN)) > 0.5) {
			RMSDrivePlus.SetLine();
			RMSDriveMinus.ResetLine();
			lastRMSSJCommand = 1;
		}
		else if ((GetThrusterGroupLevel(THGROUP_ATT_PITCHUP) - GetThrusterGroupLevel(THGROUP_ATT_PITCHDOWN)) < -0.5) {
			RMSDrivePlus.ResetLine();
			RMSDriveMinus.SetLine();
			lastRMSSJCommand = -1;
		}
		else {
			if (lastRMSSJCommand != 0) { // only reset lines once, so Panel A8 joint drive switch works
				RMSDrivePlus.ResetLine();
				RMSDriveMinus.ResetLine();
				lastRMSSJCommand = 0;
			}
		}
	}
	else { // use RHC/THC input to control RCS
		if (status == STATE_ORBITER && GetAltitude() < 100000.0) { // use Orbiter aerosurfaces, which will smoothly ramp between values, for entry
			if ((VCMode == VC_CDR && CdrFltCntlrPwr) || (VCMode == VC_PLT && PltFltCntlrPwr)) { //forward RHC/THC
				RHCInput.data[PITCH] = GetControlSurfaceLevel(AIRCTRL_ELEVATOR);
				RHCInput.data[YAW] = GetControlSurfaceLevel(AIRCTRL_RUDDER);
				RHCInput.data[ROLL] = GetControlSurfaceLevel(AIRCTRL_AILERON);
			}
			else {
				RHCInput = _V(0, 0, 0);
			}
			THCInput = _V(0, 0, 0);
		}
		else { // launch or in orbit - use Orbiter thrusters
			//if(VCMode==VC_CDR || VCMode==VC_PLT || VCMode==VC_MS1 || VCMode==VC_MS2) { //forward RHC/THC
			if ((VCMode == VC_CDR && CdrFltCntlrPwr) || (VCMode == VC_PLT && PltFltCntlrPwr)) { //forward RHC/THC
				RHCInput.data[PITCH] = GetThrusterGroupLevel(THGROUP_ATT_PITCHUP) - GetThrusterGroupLevel(THGROUP_ATT_PITCHDOWN);
				RHCInput.data[YAW] = GetThrusterGroupLevel(THGROUP_ATT_YAWRIGHT) - GetThrusterGroupLevel(THGROUP_ATT_YAWLEFT);
				RHCInput.data[ROLL] = GetThrusterGroupLevel(THGROUP_ATT_BANKRIGHT) - GetThrusterGroupLevel(THGROUP_ATT_BANKLEFT);
				if (!ControlSurfacesEnabled && GetAttitudeMode() == RCS_ROT) { // use arrow, Ins/Del keys for translation input
					for (int i = 0; i < 3; i++) THCInput.data[i] = AltKybdInput.data[i];
				}
				else {
					THCInput.x = GetThrusterGroupLevel(THGROUP_ATT_FORWARD) - GetThrusterGroupLevel(THGROUP_ATT_BACK);
					THCInput.y = GetThrusterGroupLevel(THGROUP_ATT_RIGHT) - GetThrusterGroupLevel(THGROUP_ATT_LEFT);
					THCInput.z = GetThrusterGroupLevel(THGROUP_ATT_DOWN) - GetThrusterGroupLevel(THGROUP_ATT_UP);
				}
			}
			else if ((VCMode != VC_MS1 && VCMode != VC_MS2) && AftFltCntlrPwr) { //aft RHC/THC
				if (AftSense) { //-Z
					RHCInput.data[PITCH] = GetThrusterGroupLevel(THGROUP_ATT_PITCHDOWN) - GetThrusterGroupLevel(THGROUP_ATT_PITCHUP);
					RHCInput.data[YAW] = GetThrusterGroupLevel(THGROUP_ATT_BANKLEFT) - GetThrusterGroupLevel(THGROUP_ATT_BANKRIGHT);
					RHCInput.data[ROLL] = GetThrusterGroupLevel(THGROUP_ATT_YAWLEFT) - GetThrusterGroupLevel(THGROUP_ATT_YAWRIGHT);
					if (!ControlSurfacesEnabled && GetAttitudeMode() == RCS_ROT) { // use arrow, Ins/Del keys for translation input
						THCInput.z = -AltKybdInput.x;
						THCInput.y = -AltKybdInput.y;
						THCInput.x = -AltKybdInput.z;
					}
					else {
						THCInput.z = GetThrusterGroupLevel(THGROUP_ATT_BACK) - GetThrusterGroupLevel(THGROUP_ATT_FORWARD);
						THCInput.y = GetThrusterGroupLevel(THGROUP_ATT_LEFT) - GetThrusterGroupLevel(THGROUP_ATT_RIGHT);
						THCInput.x = GetThrusterGroupLevel(THGROUP_ATT_UP) - GetThrusterGroupLevel(THGROUP_ATT_DOWN);
					}
				}
				else { //-X
					RHCInput.data[PITCH] = GetThrusterGroupLevel(THGROUP_ATT_PITCHDOWN) - GetThrusterGroupLevel(THGROUP_ATT_PITCHUP);
					RHCInput.data[YAW] = GetThrusterGroupLevel(THGROUP_ATT_YAWRIGHT) - GetThrusterGroupLevel(THGROUP_ATT_YAWLEFT);
					RHCInput.data[ROLL] = GetThrusterGroupLevel(THGROUP_ATT_BANKLEFT) - GetThrusterGroupLevel(THGROUP_ATT_BANKRIGHT);
					if (!ControlSurfacesEnabled && GetAttitudeMode() == RCS_ROT) { // use arrow, Ins/Del keys for translation input
						THCInput.x = -AltKybdInput.x;
						THCInput.y = -AltKybdInput.y;
						THCInput.z = AltKybdInput.z;
					}
					else {
						THCInput.x = GetThrusterGroupLevel(THGROUP_ATT_BACK) - GetThrusterGroupLevel(THGROUP_ATT_FORWARD);
						THCInput.y = GetThrusterGroupLevel(THGROUP_ATT_LEFT) - GetThrusterGroupLevel(THGROUP_ATT_RIGHT);
						THCInput.z = GetThrusterGroupLevel(THGROUP_ATT_DOWN) - GetThrusterGroupLevel(THGROUP_ATT_UP);
					}
				}
			}
			else {
				RHCInput = _V(0, 0, 0);
				THCInput = _V(0, 0, 0);
			}
		}

		for (unsigned short i = 0; i < 3; i++) {
			RMS_RHCInput[i].SetLine(0.0);
			RMS_THCInput[i].SetLine(0.0);

			RHCInputPort[i].SetLine(static_cast<float>(RHCInput.data[i]));
			THCInputPort[i].SetLine(static_cast<float>(THCInput.data[i]));
		}
	}
	// get SPDBK/THROT level
	if (pSimpleGPC->GetMajorMode() == 102 || pSimpleGPC->GetMajorMode() == 103
		|| pSimpleGPC->GetMajorMode() == 304 || pSimpleGPC->GetMajorMode() == 305)
	{
		SpdbkThrotPort.SetLine(static_cast<float>(GetThrusterGroupLevel(THGROUP_MAIN)));
	}
	else {
		SpdbkThrotPort.ResetLine();
	}
}

void Atlantis::clbkSetClassCaps(FILEHANDLE cfg)
{
	try
	{
		options->Parse(cfg);
		if (!oapiReadItem_bool(cfg, "RenderCockpit", render_cockpit))
			render_cockpit = false;

		if (!oapiReadItem_bool(cfg, "UseRealRCS", bUseRealRCS))
			bUseRealRCS = false;

		if (!oapiReadItem_bool(cfg, "EnableMCADebug", bEnableMCADebug))
			bEnableMCADebug = false;



		psubsystems->SetClassCaps(cfg);
		loadMDMConfiguration();
	}
	catch (std::exception &e)
	{
		char buffer[400];
		sprintf_s(buffer, 400, "(SpaceShuttleUltra) [ERROR] Exception in clbkSetClassCaps: %s", e.what());
		oapiWriteLog(buffer);
		exit(999);
	}
}

// --------------------------------------------------------------
// Set status from a VESSELSTATUS2 structure
// --------------------------------------------------------------
void Atlantis::clbkSetStateEx(const void *status)
{
	// default parameter initialisation
	DefSetStateEx(status);

	oapiWriteLog("(ssu)Set Orbiter configuration.");
	// reset vessel-specific parameters to defaults
	SetOrbiterConfiguration();

}

// --------------------------------------------------------------
// Read status from scenario file
// --------------------------------------------------------------
void Atlantis::clbkLoadStateEx(FILEHANDLE scn, void *vs)
{
	int action;
	char *line;
	char pszBuffer[256];
	char pszLogBuffer[256];

	spdb_status = AnimState::CLOSED; spdb_proc = 0.0;

	while (oapiReadScenario_nextline(scn, line))
	{
		if (!_strnicmp(line, "CONFIGURATION", 13))
		{
			sscanf(line + 13, "%d", &status);
		}
		else if (!_strnicmp(line, "VC_POS", 6))
		{
			sscanf(line + 6, "%d", &scnVCMode);
		}
		else if (!_strnicmp(line, "MISSION", 7))
		{
			strncpy(pszBuffer, line + 8, 255);

			sprintf_s(pszLogBuffer, 255, "(SpaceShuttleUltra) Loading mission %s", pszBuffer);
			oapiWriteLog(pszLogBuffer);

			pMission = ssuGetMission(pszBuffer);

			// add additional components defined in Mission file
			RMS = pMission->HasRMS();
			STBDMPM = pMission->HasSTBDMPMs();

			if (RMS)
			{
				psubsystems->AddSubsystem(pRMS = new RMSSystem(psubsystems));
				if (!pPanelA8) pgAft.AddPanel(pPanelA8 = new vc::PanelA8(this));
			}
			if (STBDMPM)
			{
				psubsystems->AddSubsystem(pMPMs = new StbdMPMSystem(psubsystems));
				if (!pPanelA8) pgAft.AddPanel(pPanelA8 = new vc::PanelA8(this));
			}
			if (pMission->HasODS())
			{
				psubsystems->AddSubsystem(pExtAirlock = new eva_docking::ODS(psubsystems, "ODS"));
				pgAft.AddPanel(pA7A8Panel = new vc::PanelA7A8ODS(this));
			}
			else if (pMission->HasExtAL()) psubsystems->AddSubsystem( pExtAirlock = new eva_docking::ExtAirlock( psubsystems, "ExternalAirlock" ) );

			if (pMission->HasTAA()) psubsystems->AddSubsystem( pTAA = new eva_docking::TunnelAdapterAssembly( psubsystems, pMission->AftTAA() ) );

			if (pMission->UseASE_IUS())
			{
				psubsystems->AddSubsystem( pASE_IUS = new ASE_IUS( psubsystems, pMission->IsASELocationAft() ) );
				//pgAftPort.AddPanel( pPanelL10 = new vc::PanelL10( this ) );
				pgAft.AddPanel( pPanelL10 = new vc::PanelL10( this ) );
			}

			if (pMission->UseCISS())
			{
				psubsystems->AddSubsystem( pCISS = new CISS( psubsystems, pMission->IsCISSGPrime() ) );
				//pgAftPort.AddPanel( pPanelL12U = new vc::PanelL12U( this ) );
				pgAft.AddPanel( pPanelL12U = new vc::PanelL12U( this ) );
				hasCISS = true;
			}

			bHasKUBand = pMission->HasKUBand();

			if (bHasKUBand)
			{
				psubsystems->AddSubsystem( pDeployedAssembly = new comm::DeployedAssembly( psubsystems ) );
				psubsystems->AddSubsystem( new comm::ElectronicsAssembly1( psubsystems, pDeployedAssembly ) );
				psubsystems->AddSubsystem( new comm::ElectronicsAssembly2( psubsystems, pDeployedAssembly ) );
			}
		}
		else if (!_strnicmp(line, "SPEEDBRAKE", 10))
		{
			sscanf(line + 10, "%d%lf%lf", &action, &spdb_proc, &spdb_tgt);
			spdb_status = (AnimState::Action)(action + 1);
		}
		else if (!_strnicmp(line, "GEAR", 4))
		{
			sscan_state(line + 4, gear_status);
			if (gear_status.action == AnimState::STOPPED) gear_status.action = AnimState::CLOSED;
		}
		else if (!_strnicmp(line, "PLB_CAM", 7))
		{
			sscanf(line + 8, "%lf%lf%lf%lf%lf%lf%lf%lf", &camPitch[CAM_A], &camYaw[CAM_A], &camPitch[CAM_B], &camYaw[CAM_B],
				&camPitch[CAM_C], &camYaw[CAM_C], &camPitch[CAM_D], &camYaw[CAM_D]);
			cameraMoved = true;
		}
		else if (!_strnicmp(line, "PAYLOAD_MASS", 12))
		{
			sscanf(line + 12, "%lf", &pl_mass);
		}
		else if (!_strnicmp(line, "CARGO_STATIC_MESH", 17))
		{
			sscanf(line + 17, "%s", cargo_static_mesh_name);
			do_cargostatic = true;
		}
		else if (!_strnicmp(line, "CARGO_STATIC_OFS", 16))
		{
			sscanf(line + 16, "%lf%lf%lf", &cargo_static_ofs.x, &cargo_static_ofs.y, &cargo_static_ofs.z);
		}
		else if (!_strnicmp(line, "OPS", 3))
		{
			unsigned int ops;
			sscanf(line + 3, "%u", &ops);
			pSimpleGPC->SetMajorMode(ops);
		}
		else if (_strnicmp(line, "GOXVENTSOFF", 11) == 0)
		{
			if (status == STATE_PRELAUNCH) bSSMEGOXVent = false;
		}
		else if (!_strnicmp(line, "@PANEL", 6))
		{
			char pszPanelName[30];
			sscanf_s(line + 6, "%s", pszPanelName, sizeof(pszPanelName));
			sprintf_s(pszBuffer, 255, "\tLook up panel \"%s\"... \t\t(%s)", pszPanelName, line);
			oapiWriteLog(pszBuffer);

			if (pgLeft.HasPanel(pszPanelName))
				pgLeft.ParsePanelBlock(pszPanelName, scn);

			if (pgForward.HasPanel(pszPanelName))
				pgForward.ParsePanelBlock(pszPanelName, scn);

			if (pgRight.HasPanel(pszPanelName))
				pgRight.ParsePanelBlock(pszPanelName, scn);

			if (pgCenter.HasPanel(pszPanelName))
				pgCenter.ParsePanelBlock(pszPanelName, scn);

			if (pgOverhead.HasPanel(pszPanelName))
				pgOverhead.ParsePanelBlock(pszPanelName, scn);

			if (pgOverheadAft.HasPanel(pszPanelName))
				pgOverheadAft.ParsePanelBlock(pszPanelName, scn);

			if (pgAftStbd.HasPanel(pszPanelName))
				pgAftStbd.ParsePanelBlock(pszPanelName, scn);

			if (pgAft.HasPanel(pszPanelName))
				pgAft.ParsePanelBlock(pszPanelName, scn);

			if (pgAftPort.HasPanel(pszPanelName))
				pgAftPort.ParsePanelBlock(pszPanelName, scn);

			oapiWriteLog("\tLeave @PANEL block.");
		}
		else
		{
			if (psubsystems->ParseScenarioLine(scn, line)) continue; // offer line to subsystem simulation
			ParseScenarioLineEx(line, vs);// unrecognised option - pass to Orbiter's generic parser
		}
	}

	ClearMeshes();
	switch (status)
	{
	case 0:
		SetLaunchConfiguration();
		break;
	case 1:
		SetPostLaunchConfiguration(met);
		break;
	case 2:
		SetOrbiterTankConfiguration();
		break;
	case 3:
		SetOrbiterConfiguration();
		break;
	}
	if (status >= STATE_STAGE1) pMTU->StartMET(); // make sure timer is running

	UpdateMesh();
}

// --------------------------------------------------------------
// Write status to scenario file
// --------------------------------------------------------------
void Atlantis::clbkSaveState(FILEHANDLE scn)
{
	char cbuf[256];

	// save default vessel parameters
	// set CoG to center of mesh before saving scenario; otherwise, shuttle position will change slightly when saved scenario is loaded
	ShiftCG(-currentCoG);
	VESSEL3::clbkSaveState(scn);
	ShiftCG(currentCoG); // reset CoG to correct position

	if (!pMission->GetMissionFileName().empty())
	{
		strcpy(cbuf, pMission->GetMissionFileName().c_str());
		oapiWriteScenario_string(scn, "MISSION", cbuf);
	}

	// custom parameters
	oapiWriteScenario_int(scn, "CONFIGURATION", status);
	oapiWriteScenario_int(scn, "VC_POS", VCMode);

	if (spdb_status != AnimState::CLOSED)
	{
		sprintf(cbuf, "%d %0.4f %0.4f", spdb_status - 1, spdb_proc, spdb_tgt);
		oapiWriteScenario_string(scn, "SPEEDBRAKE", cbuf);
	}
	WriteScenario_state(scn, "GEAR", gear_status);

	if (do_cargostatic)
	{
		oapiWriteScenario_string(scn, "CARGO_STATIC_MESH", cargo_static_mesh_name);
		if (cargo_static_ofs.x || cargo_static_ofs.y || cargo_static_ofs.z)
			oapiWriteScenario_vec(scn, "CARGO_STATIC_OFS", cargo_static_ofs);
	}
	if (pl_mass != 0.0) oapiWriteScenario_float(scn, "PAYLOAD_MASS", pl_mass);

	//GPC
	oapiWriteScenario_int(scn, "OPS", pSimpleGPC->GetMajorMode());

	sprintf_s(cbuf, 255, "%0.4f %0.4f %0.4f %0.4f %0.4f %0.4f %0.4f %0.4f", camPitch[CAM_A], camYaw[CAM_A], camPitch[CAM_B], camYaw[CAM_B],
		camPitch[CAM_C], camYaw[CAM_C], camPitch[CAM_D], camYaw[CAM_D]);
	oapiWriteScenario_string(scn, "PLB_CAM", cbuf);

	oapiWriteLog("SpaceShuttleUltra:\tSave subsystem states...");
	psubsystems->SaveState(scn);

	oapiWriteLog("SpaceShuttleUltra:\tSave panel states...");
	oapiWriteLog("\tForward flight deck");
	pgLeft.OnSaveState(scn);
	pgForward.OnSaveState(scn);
	pgRight.OnSaveState(scn);
	pgCenter.OnSaveState(scn);
	pgOverhead.OnSaveState(scn);
	pgOverheadAft.OnSaveState(scn);
	oapiWriteLog("\tAft flight deck");
	pgAftStbd.OnSaveState(scn);
	pgAft.OnSaveState(scn);
	pgAftPort.OnSaveState(scn);

	oapiWriteLog("SpaceShuttleUltra:\tSaving state done.");
}

// --------------------------------------------------------------
// Vessel gains or loses input focus
// --------------------------------------------------------------
void Atlantis::clbkFocusChanged(bool getfocus, OBJHANDLE newv, OBJHANDLE oldv)
{
	if (getfocus) {
		oapiDisableMFDMode(MFD_LANDING);
		// no VTOL MFD mode for Atlantis
		//Enable communication overlays
	}
	else
	{
		//Disable all Communication overlays
	}
}

int Atlantis::clbkGeneric(int msgid, int prm, void *context)
{
	try {
		switch (msgid)
		{
		case VMSG_LUAINSTANCE:
			return Lua_InitInstance(context);
		case VMSG_LUAINTERPRETER:
			return Lua_InitInterpreter(context);
		default:
			return 0;
		}
	}
	catch (std::exception &e)
	{
		char buffer[400];
		sprintf_s(buffer, 400, "(SpaceShuttleUltra) [ERROR] Exception in clbkGeneric: %s", e.what());
		oapiWriteLog(buffer);
		return 0;
	}
}

// --------------------------------------------------------------
// Before first timestep
// --------------------------------------------------------------
void Atlantis::clbkPostCreation()
{
	try
	{
		//oapiWriteLog("In clbkPostCreation");
		VESSEL3::clbkPostCreation(); //may not be necessary

		SoundID = ConnectToOrbiterSoundDLL(GetHandle());
		if (SoundID != -1) {
			//NOTE: (char*) casts in OrbiterSound calls should be safe; I think function just stores the file names (SiameseCat)
			SetMyDefaultWaveDirectory(const_cast<char*>(SOUND_DIRECTORY));

			SoundOptionOnOff(SoundID, PLAYATTITUDETHRUST, FALSE);
			RequestLoadVesselWave(SoundID, RCS_SOUND, (char*)RCS_SOUND_FILE, INTERNAL_ONLY);

			ReplaceStockSound(SoundID, const_cast<char*>(AIR_CONDITIONING_SOUND_FILE), REPLACE_AIR_CONDITIONNING);

			//SSME sounds
			SoundOptionOnOff(SoundID, PLAYMAINTHRUST, FALSE);
			SoundOptionOnOff(SoundID, PLAYUSERTHRUST, FALSE);
			RequestLoadVesselWave(SoundID, SSME_START, (char*)SSME_START_FILE, BOTHVIEW_FADED_MEDIUM);
			RequestLoadVesselWave(SoundID, SSME_RUNNING, (char*)SSME_RUNNING_FILE, BOTHVIEW_FADED_MEDIUM);
			RequestLoadVesselWave(SoundID, SSME_SHUTDOWN, (char*)SSME_SHUTDOWN_FILE, BOTHVIEW_FADED_MEDIUM);

			//APU sounds
			RequestLoadVesselWave(SoundID, APU_START, (char*)APU_START_FILE, EXTERNAL_ONLY_FADED_MEDIUM);
			RequestLoadVesselWave(SoundID, APU_RUNNING, (char*)APU_RUNNING_FILE, EXTERNAL_ONLY_FADED_MEDIUM);
			RequestLoadVesselWave(SoundID, APU_SHUTDOWN, (char*)APU_SHUTDOWN_FILE, EXTERNAL_ONLY_FADED_MEDIUM);

			RequestLoadVesselWave(SoundID, SWITCH_GUARD_SOUND, const_cast<char*>(SWITCH_GUARD_FILE), INTERNAL_ONLY);
			RequestLoadVesselWave(SoundID, SWITCH_THROW_SOUND, const_cast<char*>(SWITCH_THROW_FILE), INTERNAL_ONLY);
			RequestLoadVesselWave(SoundID, KEY_PRESS_SOUND, const_cast<char*>(KEY_PRESS_FILE), INTERNAL_ONLY);
		}


		//oapiWriteLog("(ssu)Realize all subsystems");
		psubsystems->RealizeAll();
		pgForward.Realize();
		pgLeft.Realize();
		pgRight.Realize();
		pgCenter.Realize();
		pgOverhead.Realize();
		pgOverheadAft.Realize();
		pgAftPort.Realize();
		pgAft.Realize();
		pgAftStbd.Realize();

		DiscreteBundle* pBundle = BundleManager()->CreateBundle("BODYFLAP_CONTROLS", 16);
		BodyFlapAutoIn.Connect(pBundle, 0);
		BodyFlapAutoOut.Connect(pBundle, 0);
		BodyFlapManOut.Connect(pBundle, 1);

		pBundle = BundleManager()->CreateBundle("SPDBKTHROT_CONTROLS", 16);
		SpdbkThrotAutoIn.Connect(pBundle, 0);
		SpdbkThrotCDROut.Connect(pBundle, 1);
		SpdbkThrotPLTOut.Connect(pBundle, 2);
		// if neither MAN port is set, set AUTO port
		DiscInPort SpdbkThrotCDRIn, SpdbkThrotPLTIn;
		SpdbkThrotCDRIn.Connect(pBundle, 1);
		SpdbkThrotPLTIn.Connect(pBundle, 2);
		if (!SpdbkThrotCDRIn && !SpdbkThrotPLTIn) {
			DiscOutPort SpdbkThrotAutoOut;
			SpdbkThrotAutoOut.Connect(pBundle, 0);
			SpdbkThrotAutoOut.SetLine();
		}

		pBundle = BundleManager()->CreateBundle("Controllers", 16);
		AftSense.Connect(pBundle, 0);
		CdrFltCntlrPwr.Connect(pBundle, 1);
		PltFltCntlrPwr.Connect(pBundle, 2);
		AftFltCntlrPwr.Connect(pBundle, 3);

		pBundle = bundleManager->CreateBundle("HC_INPUT", 16);
		for (int i = 0; i < 3; i++) {
			RHCInputPort[i].Connect(pBundle, i);
			THCInputPort[i].Connect(pBundle, i + 3);

			RHCInputPort[i].SetLine(0.0f);
			THCInputPort[i].SetLine(0.0f);
		}
		SpdbkThrotPort.Connect(pBundle, 6);

		pBundle = bundleManager->CreateBundle("AEROSURFACE_CMD", 16);
		//LeftElevonCommand.Connect(pBundle, 0);
		//RightElevonCommand.Connect(pBundle, 1);
		ElevonCommand.Connect(pBundle, 0);
		AileronCommand.Connect(pBundle, 1);
		RudderCommand.Connect(pBundle, 2);

		pBundle = bundleManager->CreateBundle("THRUSTER_CMD", 16);
		for (unsigned int i = 0; i < 3; i++) {
			RotThrusterCommands[i].Connect(pBundle, i);
			TransThrusterCommands[i].Connect(pBundle, i + 3);

			// at start, make sure lines are set to 0;
			DiscOutPort temp;
			temp.Connect(pBundle, i);
			temp.ResetLine();
			temp.Connect(pBundle, i + 3);
			temp.ResetLine();
		}
		RotThrusterCommands[3].Connect(pBundle, 6);// SERC RCS thrusters

		pBundle = bundleManager->CreateBundle("RMS_EE", 16);
		RMSGrapple.Connect(pBundle, 0);
		RMSRelease.Connect(pBundle, 1);

		pBundle = bundleManager->CreateBundle("RMS_HC_INPUT", 16);
		for (int i = 0; i < 3; i++) {
			RMS_RHCInput[i].Connect(pBundle, i);
			RMS_THCInput[i].Connect(pBundle, i + 3);
		}

		pBundle = bundleManager->CreateBundle("RMS_MODE", 16);
		RMSSpeedIn.Connect(pBundle, 12);
		RMSSpeedOut.Connect(pBundle, 12);

		pBundle = bundleManager->CreateBundle("RMS_SINGLE_JOINT", 16);
		RMSDrivePlus.Connect(pBundle, 8);
		RMSDriveMinus.Connect(pBundle, 9);

		pBundle = bundleManager->CreateBundle("LOMS", 5);
		OMSArm[LEFT].Connect(pBundle, 0);
		OMSArmPress[LEFT].Connect(pBundle, 1);
		OMSFire[LEFT].Connect(pBundle, 2);
		OMSPitch[LEFT].Connect(pBundle, 3);
		OMSYaw[LEFT].Connect(pBundle, 4);
		pBundle = bundleManager->CreateBundle("ROMS", 5);
		OMSArm[RIGHT].Connect(pBundle, 0);
		OMSArmPress[RIGHT].Connect(pBundle, 1);
		OMSFire[RIGHT].Connect(pBundle, 2);
		OMSPitch[RIGHT].Connect(pBundle, 3);
		OMSYaw[RIGHT].Connect(pBundle, 4);

		pBundle = bundleManager->CreateBundle("C3_LIMITS_SSMEPB", 5);
		for (int i = 0; i < 3; i++) SSMEPBAnalog[i].Connect(pBundle, i + 2);

		pBundle = bundleManager->CreateBundle("ET_LOX_SENSORS", 16);
		LO2LowLevelSensor[0].Connect(pBundle, 0);
		LO2LowLevelSensor[1].Connect(pBundle, 1);
		LO2LowLevelSensor[2].Connect(pBundle, 2);
		LO2LowLevelSensor[3].Connect(pBundle, 3);

		// ports for pan/tilt and cam settings
		DiscreteBundle* pCamBundles[5];
		pCamBundles[0] = bundleManager->CreateBundle("PLB_CAM_A", 16);
		pCamBundles[1] = bundleManager->CreateBundle("PLB_CAM_B", 16);
		pCamBundles[2] = bundleManager->CreateBundle("PLB_CAM_C", 16);
		pCamBundles[3] = bundleManager->CreateBundle("PLB_CAM_D", 16);
		pCamBundles[4] = bundleManager->CreateBundle("RMS_ELBOW_CAM", 16);
		for (unsigned short i = 0; i < 5; i++) {
			PLBDCamPanLeft[i].Connect(pCamBundles[i], 0);
			PLBDCamPanLeft_Out[i].Connect(pCamBundles[i], 0);
			PLBDCamPanRight[i].Connect(pCamBundles[i], 1);
			PLBDCamPanRight_Out[i].Connect(pCamBundles[i], 1);

			PLBDCamTiltUp[i].Connect(pCamBundles[i], 2);
			PLBDCamTiltUp_Out[i].Connect(pCamBundles[i], 2);
			PLBDCamTiltDown[i].Connect(pCamBundles[i], 3);
			PLBDCamTiltDown_Out[i].Connect(pCamBundles[i], 3);

			PTULowSpeed[i].Connect(pCamBundles[i], 4);
		}

		pBundle = bundleManager->CreateBundle("PLBD_LIGHTS", 16);
		for (int i = 0; i < 6; i++) PLBDLightPower[i].Connect(pBundle, i);
		if (pMission->HasBulkheadFloodlights()) {
			FwdBulkheadLightPower.Connect(pBundle, 6);
			DockingLightDim.Connect(pBundle, 7);
			DockingLightBright.Connect(pBundle, 8);
		}

		pBundle = bundleManager->CreateBundle("LANDING_GEAR", 16);
		LandingGearPosition[0].Connect(pBundle, 0);
		LandingGearPosition[1].Connect(pBundle, 1);
		LandingGearPosition[2].Connect(pBundle, 2);
		LandingGearPosition[3].Connect(pBundle, 3);
		LandingGearPosition[4].Connect(pBundle, 4);
		LandingGearPosition[5].Connect(pBundle, 5);
		LandingGearArmDeployLT[0].Connect(pBundle, 6);
		LandingGearArmDeployLT[1].Connect(pBundle, 7);
		LandingGearArmDeployPB[0].Connect(pBundle, 8);
		LandingGearArmDeployPB[1].Connect(pBundle, 9);
		LandingGearArmDeployPB[2].Connect(pBundle, 10);
		LandingGearArmDeployPB[3].Connect(pBundle, 11);

		pBundle = bundleManager->CreateBundle("DRAG_CHUTE", 16);
		DragChuteARMDPYJETTLT[0].Connect(pBundle, 0);
		DragChuteARMDPYJETTLT[1].Connect(pBundle, 1);
		DragChuteARMDPYJETTLT[2].Connect(pBundle, 2);
		DragChuteARM[0].Connect(pBundle, 3);
		DragChuteARM[1].Connect(pBundle, 4);
		DragChuteDPY[0].Connect(pBundle, 5);
		DragChuteDPY[1].Connect(pBundle, 6);
		DragChuteJETT[0].Connect(pBundle, 7);
		DragChuteJETT[1].Connect(pBundle, 8);

		pgCenter.LogPanels("Center");
		pgForward.LogPanels("Forward");
		pgOverhead.LogPanels("Overhead");
		pgLeft.LogPanels("Left");
		pgRight.LogPanels("Right");
		pgAft.LogPanels("Aft");
		pgAftPort.LogPanels("Aft Port");
		pgAftStbd.LogPanels("Aft Starboard");
		pgOverheadAft.LogPanels("Aft Overhead");

	}
	catch (std::exception &e)
	{
		char buffer[400];
		sprintf_s(buffer, 400, "(SpaceShuttleUltra) [ERROR] Exception in clbkPostCreation: %s", e.what());
		oapiWriteLog(buffer);
	}
} //Atlantis::clbkPostCreation

// --------------------------------------------------------------
// Simulation time step
// --------------------------------------------------------------

void Atlantis::clbkPreStep(double simT, double simDT, double mjd)
{
	static bool ___PreStep_flag = false;

	try
	{
		if (firstStep) {
			UpdateMassAndCoG(); // update visual before simulation starts

			if (status <= STATE_STAGE1) {
				// update SRB thrusters to match values from SRB vessel
				VESSEL* pLeftSRB = oapiGetVesselInterface(GetAttachmentStatus(ahLeftSRB));
				THRUSTER_HANDLE th_ref = pLeftSRB->GetGroupThruster(THGROUP_MAIN, 0);
				CopyThrusterSettings(th_srb[0], pLeftSRB, th_ref);
				VESSEL* pRightSRB = oapiGetVesselInterface(GetAttachmentStatus(ahRightSRB));
				th_ref = pRightSRB->GetGroupThruster(THGROUP_MAIN, 0);
				CopyThrusterSettings(th_srb[1], pRightSRB, th_ref);

				PROPELLANT_HANDLE ph_ref = pLeftSRB->GetThrusterResource(th_ref);
				double phMass = pLeftSRB->GetPropellantMaxMass(ph_ref);
				SetPropellantMaxMass(ph_srb, phMass*2.0);

				// update touchdown points to match height of attachment to pad
				// otherwise, stack will either fall through pad or bounce upwards at T-0
				OBJHANDLE hPad = GetAttachmentStatus(ahHDP);
				if (hPad) {
					VESSEL* v = oapiGetVesselInterface(hPad);
					DWORD count = v->AttachmentCount(false);
					//ATTACHMENTHANDLE ahParent = NULL;
					VECTOR3 parentAttachPos, dir, rot;
					for (DWORD i = 0; i < count; i++) {
						ATTACHMENTHANDLE ah = v->GetAttachmentHandle(false, i);
						if (v->GetAttachmentStatus(ah) == GetHandle()) {
							v->GetAttachmentParams(ah, parentAttachPos, dir, rot);
							break;
						}
					}
					VECTOR3 pt1, pt2, pt3;
					v->GetTouchdownPoints(pt1, pt2, pt3);
					// assume y-axis of pad is in vertical direction and pt1.y==pt2.y==pt3.y
					VECTOR3 pos = POS_HDP - currentCoG;
					double touchdownZ = pt1.y - parentAttachPos.y + pos.z;
					SetTouchdownPoints(_V(0, -10, touchdownZ), _V(-7, 7, touchdownZ), _V(7, 7, touchdownZ));
				}
			}
		}

		if (!___PreStep_flag)
		{
			oapiWriteLog("In clbkPreStep");
			___PreStep_flag = true;
		}

		// update MET; needs to be done as early as possible, before subsytem PreStep functions are called
		if (status == STATE_PRELAUNCH) {
			met = 0.0;
		}
		else {
			// calculate MET (in seconds) from MTU
			met = pMTU->GetMETDay(0)*86400.0 + pMTU->GetMETHour(0)*3600.0 + pMTU->GetMETMin(0)*60.0 + pMTU->GetMETSec(0) + pMTU->GetMETMilli(0) / 1000.0;
		}
		//Stopwatch st, stSub;
		//st.Start();

		//stSub.Start();
		psubsystems->PreStep(simT, simDT, mjd);
		pgLeft.OnPreStep(simT, simDT, mjd);
		pgForward.OnPreStep(simT, simDT, mjd);
		pgRight.OnPreStep(simT, simDT, mjd);
		pgCenter.OnPreStep(simT, simDT, mjd);
		pgOverhead.OnPreStep(simT, simDT, mjd);
		pgOverheadAft.OnPreStep(simT, simDT, mjd);
		pgAftStbd.OnPreStep(simT, simDT, mjd);
		pgAft.OnPreStep(simT, simDT, mjd);
		pgAftPort.OnPreStep(simT, simDT, mjd);
		//double subTime = stSub.Stop();

		OMSEngControl(LEFT);
		OMSEngControl(RIGHT);

		// disable all Orbitersim autopilots
		for (int i = NAVMODE_KILLROT; i <= NAVMODE_HOLDALT; i++) DeactivateNavmode(i);

		UpdateHandControllerSignals();



		// check inputs from GPC and set thrusters
		//PITCH Commands to Thrusters
		double pitchcmd = RotThrusterCommands[PITCH].GetVoltage();
		if (pitchcmd > 0.0001)
		{
			SetThrusterGroupLevel(thg_pitchup, pitchcmd);
			SetThrusterGroupLevel(thg_pitchdown, 0.0);

			if (lastRotCommand[PITCH] != 1) {
				lastRotCommand[PITCH] = 1;
				PlayVesselWave(SoundID, RCS_SOUND);
			}
		}
		else if (pitchcmd < -0.0001)
		{
			SetThrusterGroupLevel(thg_pitchdown, -pitchcmd);
			SetThrusterGroupLevel(thg_pitchup, 0.0);

			if (lastRotCommand[PITCH] != -1) {
				lastRotCommand[PITCH] = -1;
				PlayVesselWave(SoundID, RCS_SOUND);
			}
		}
		else
		{
			SetThrusterGroupLevel(thg_pitchup, 0.0);
			SetThrusterGroupLevel(thg_pitchdown, 0.0);
			lastRotCommand[PITCH] = 0;

		}


		//YAW Commands to thrusters
		if (RotThrusterCommands[YAW].GetVoltage() > 0.0001) {
			SetThrusterGroupLevel(thg_yawright, RotThrusterCommands[YAW].GetVoltage());
			SetThrusterGroupLevel(thg_yawleft, 0.0);

			if (lastRotCommand[YAW] != 1) {
				lastRotCommand[YAW] = 1;
				PlayVesselWave(SoundID, RCS_SOUND);
			}
		}
		else if (RotThrusterCommands[YAW].GetVoltage() < -0.0001) {
			SetThrusterGroupLevel(thg_yawleft, -RotThrusterCommands[YAW].GetVoltage());
			SetThrusterGroupLevel(thg_yawright, 0.0);

			if (lastRotCommand[YAW] != -1) {
				lastRotCommand[YAW] = -1;
				PlayVesselWave(SoundID, RCS_SOUND);
			}
		}
		else {
			SetThrusterGroupLevel(thg_yawright, 0.0);
			SetThrusterGroupLevel(thg_yawleft, 0.0);
			lastRotCommand[YAW] = 0;
		}

		//ROLL Commands to Thruster
		if (RotThrusterCommands[ROLL].GetVoltage() > 0.0001) {
			SetThrusterGroupLevel(thg_rollright, RotThrusterCommands[ROLL].GetVoltage());
			SetThrusterGroupLevel(thg_rollleft, 0.0);

			if (lastRotCommand[ROLL] != 1) {
				lastRotCommand[ROLL] = 1;
				PlayVesselWave(SoundID, RCS_SOUND);
			}
		}
		else if (RotThrusterCommands[ROLL].GetVoltage() < -0.0001) {
			SetThrusterGroupLevel(thg_rollleft, -RotThrusterCommands[ROLL].GetVoltage());
			SetThrusterGroupLevel(thg_rollright, 0.0);

			if (lastRotCommand[ROLL] != -1) {
				lastRotCommand[ROLL] = -1;
				PlayVesselWave(SoundID, RCS_SOUND);
			}
		}
		else {
			SetThrusterGroupLevel(thg_rollright, 0.0);
			SetThrusterGroupLevel(thg_rollleft, 0.0);
			lastRotCommand[ROLL] = 0;
		}

		// SERC
		if (RotThrusterCommands[3].GetVoltage() > 0.0001)
		{
			// roll left
			//Possible Crash
			if (!bUseRealRCS)
			{
				SetThrusterLevel(th_att_rcs[4], RotThrusterCommands[3].GetVoltage());// F2R, F4R
				SetThrusterLevel(th_att_rcs[9], RotThrusterCommands[3].GetVoltage());// L1U, L2U, L4U
				SetThrusterLevel(th_att_rcs[8], RotThrusterCommands[3].GetVoltage());// R2D, R3D, R4D
				SetThrusterLevel(th_att_rcs[7], RotThrusterCommands[3].GetVoltage());// R1R, R2R, R3R, R4R

				SetThrusterLevel(th_att_rcs[6], 0);// F1L, F3L
				SetThrusterLevel(th_att_rcs[10], 0);// L2D, L3D, L4D
				SetThrusterLevel(th_att_rcs[5], 0);// L1L, L2L, L3L, L4L
				SetThrusterLevel(th_att_rcs[11], 0);// R1U, R2U, R4U
			}

			SERCstop = false;
		}
		else if (RotThrusterCommands[3].GetVoltage() < -0.0001)
		{
			// roll right
			//Possible Crash
			if (!bUseRealRCS)
			{
				SetThrusterLevel(th_att_rcs[6], -RotThrusterCommands[3].GetVoltage());// F1L, F3L
				SetThrusterLevel(th_att_rcs[10], -RotThrusterCommands[3].GetVoltage());// L2D, L3D, L4D
				SetThrusterLevel(th_att_rcs[5], -RotThrusterCommands[3].GetVoltage());// L1L, L2L, L3L, L4L
				SetThrusterLevel(th_att_rcs[11], -RotThrusterCommands[3].GetVoltage());// R1U, R2U, R4U

				SetThrusterLevel(th_att_rcs[4], 0);// F2R, F4R
				SetThrusterLevel(th_att_rcs[9], 0);// L1U, L2U, L4U
				SetThrusterLevel(th_att_rcs[8], 0);// R2D, R3D, R4D
				SetThrusterLevel(th_att_rcs[7], 0);// R1R, R2R, R3R, R4R
			}
			SERCstop = false;
		}
		else
		{
			if (SERCstop == false)
			{
				if (!bUseRealRCS)
				{
					SetThrusterLevel(th_att_rcs[4], 0);// F2R, F4R
					SetThrusterLevel(th_att_rcs[9], 0);// L1U, L2U, L4U
					SetThrusterLevel(th_att_rcs[8], 0);// R2D, R3D, R4D
					SetThrusterLevel(th_att_rcs[7], 0);// R1R, R2R, R3R, R4R

					SetThrusterLevel(th_att_rcs[6], 0);// F1L, F3L
					SetThrusterLevel(th_att_rcs[10], 0);// L2D, L3D, L4D
					SetThrusterLevel(th_att_rcs[5], 0);// L1L, L2L, L3L, L4L
					SetThrusterLevel(th_att_rcs[11], 0);// R1U, R2U, R4U
				}

				SERCstop = true;
			}
		}


		if (!bUseRealRCS) {


			if (TransThrusterCommands[0].GetVoltage() > 0.0001) {
				SetThrusterGroupLevel(thg_transfwd, 1.0);
				SetThrusterGroupLevel(thg_transaft, 0.0);

				if (lastTransCommand[0] != 1) {
					lastTransCommand[0] = 1;
					PlayVesselWave(SoundID, RCS_SOUND);
				}
			}
			else if (TransThrusterCommands[0].GetVoltage() < -0.0001) {
				SetThrusterGroupLevel(thg_transaft, 1.0);
				SetThrusterGroupLevel(thg_transfwd, 0.0);

				if (lastTransCommand[0] != -1) {
					lastTransCommand[0] = -1;
					PlayVesselWave(SoundID, RCS_SOUND);
				}
			}
			else {
				SetThrusterGroupLevel(thg_transfwd, 0.0);
				SetThrusterGroupLevel(thg_transaft, 0.0);
				lastTransCommand[0] = 0;
			}
			if (TransThrusterCommands[1].GetVoltage() > 0.0001) {
				SetThrusterGroupLevel(thg_transright, 1.0);
				SetThrusterGroupLevel(thg_transleft, 0.0);

				if (lastTransCommand[1] != 1) {
					lastTransCommand[1] = 1;
					PlayVesselWave(SoundID, RCS_SOUND);
				}
			}
			else if (TransThrusterCommands[1].GetVoltage() < -0.0001) {
				SetThrusterGroupLevel(thg_transleft, 1.0);
				SetThrusterGroupLevel(thg_transright, 0.0);

				if (lastTransCommand[1] != -1) {
					lastTransCommand[1] = -1;
					PlayVesselWave(SoundID, RCS_SOUND);
				}
			}
			else {
				SetThrusterGroupLevel(thg_transright, 0.0);
				SetThrusterGroupLevel(thg_transleft, 0.0);
				lastTransCommand[1] = 0;
			}
			if (TransThrusterCommands[2].GetVoltage() > 0.0001) {
				SetThrusterGroupLevel(thg_transdown, 1.0);
				SetThrusterGroupLevel(thg_transup, 0.0);

				if (lastTransCommand[2] != 1) {
					lastTransCommand[2] = 1;
					PlayVesselWave(SoundID, RCS_SOUND);
				}
			}
			else if (TransThrusterCommands[2].GetVoltage() < -0.0001) {
				SetThrusterGroupLevel(thg_transup, 1.0);
				SetThrusterGroupLevel(thg_transdown, 0.0);

				if (lastTransCommand[2] != -1) {
					lastTransCommand[2] = -1;
					PlayVesselWave(SoundID, RCS_SOUND);
				}
			}
			else {
				SetThrusterGroupLevel(thg_transdown, 0.0);
				SetThrusterGroupLevel(thg_transup, 0.0);
				lastTransCommand[2] = 0;
			}
		}


		if (pSimpleGPC->GetMajorMode() == 304 || pSimpleGPC->GetMajorMode() == 305 || pSimpleGPC->GetMajorMode() == 801) {
			double elevonPos = 0.0;
			double aileronPos = 0.0;
			if (HydraulicsOK()) {
				elevonPos = range(-33.0, ElevonCommand.GetVoltage()*33.0, 18.0);
				aileronPos = range(-10.0, AileronCommand.GetVoltage()*10.0, 10.0);
				//aerosurfaces.leftElevon = range(-33.0, LeftElevonCommand.GetVoltage()*-33.0, 18.0);
				//aerosurfaces.rightElevon = range(-33.0, RightElevonCommand.GetVoltage()*-33.0, 18.0);
				aerosurfaces.leftElevon = range(-33.0, elevonPos - aileronPos, 18.0);
				aerosurfaces.rightElevon = range(-33.0, elevonPos + aileronPos, 18.0);
				aerosurfaces.bodyFlap = 0.0;
				aerosurfaces.speedbrake = spdb_proc*100.0;
				//if(pSimpleGPC->GetMajorMode() == 801)
				//aerosurfaces.bodyFlap = (ElevonCommand.GetVoltage() + 1.0)/2.0 * 100.0;

				// TODO: limit combined rudder and speedbrake deflection
				aerosurfaces.rudder = range(-27.1, RudderCommand.GetVoltage()*27.1, 27.1);
				SetControlSurfaceLevel(AIRCTRL_RUDDERTRIM, RudderCommand.GetVoltage());
			}
			else {
				aerosurfaces.leftElevon = aerosurfaces.rightElevon = 0.0;
				aerosurfaces.bodyFlap = 0.0;
				aerosurfaces.rudder = 0.0;
				aerosurfaces.speedbrake = 0.0;
			}

			// set animations corresponding to aerosurface positions
			//double elevonPos = (LeftElevonCommand.GetVoltage()+RightElevonCommand.GetVoltage())/2.0; // position in range [-1.0, 1.0]
			//SetAnimation(anim_elev, (1.0-ElevonCommand.GetVoltage())/2.0);
			if (aerosurfaces.leftElevon < 0.0) SetAnimation(anim_lelevon, (aerosurfaces.leftElevon + 34.0) / (34.0 * 2));
			else SetAnimation(anim_lelevon, (18.0 + aerosurfaces.leftElevon) / (18.0 * 2));
			if (aerosurfaces.rightElevon < 0.0) SetAnimation(anim_relevon, (aerosurfaces.rightElevon + 34.0) / (34.0 * 2));
			else SetAnimation(anim_relevon, (18.0 + aerosurfaces.rightElevon) / (18.0 * 2));
		}


		// if we reenter PLBD cam view from external view, update camera direction
		if (!bLastCamInternal && oapiCameraInternal()) {
			if (VCMode >= VC_PLBCAMFL && VCMode <= VC_RMSCAM) SetAnimationCameras();
		}
		bLastCamInternal = oapiCameraInternal();

		// turn PLBD lights on/off
		if (pMission->HasBulkheadFloodlights()) {
			bool state = FwdBulkheadLightPower;
			FwdBulkheadLight->Activate(state);
			FwdBulkhead_bspec.active = state;

			// check docking light (which has DIM and BRIGHT positions)
			if (DockingLightDim) {
				DockingLight[0]->Activate(true);
				Docking_bspec[0].active = true;
				DockingLight[1]->Activate(false);
				Docking_bspec[1].active = false;
			}
			else if (DockingLightBright) {
				DockingLight[0]->Activate(true);
				Docking_bspec[0].active = true;
				DockingLight[1]->Activate(true);
				Docking_bspec[1].active = true;
			}
			else { // off
				for (int i = 0; i < 2; i++) {
					DockingLight[i]->Activate(false);
					Docking_bspec[i].active = false;
				}
			}
		}
		else { // turn off FWD bulkhead & docking lights
			FwdBulkheadLight->Activate(false);
			FwdBulkhead_bspec.active = false;
			for (int i = 0; i < 2; i++) {
				DockingLight[i]->Activate(false);
				Docking_bspec[i].active = false;
			}
		}
		for (int i = 0; i < 6; i++) {
			bool state = PLBDLightPower[i].IsSet();
			PLBLight[i]->Activate(state);
			PLB_bspec[i].active = state;
		}

		// during launch, turn engine light source on
		if (status <= STATE_STAGE2 && GetSSMEThrustLevel(0) > 1.0) {
			SSMELight->Activate(true);
			SSMELight->SetIntensity(GetSSMEThrustLevel(0) / SSME_MAX_POWER_LEVEL);
		}
		else {
			SSMELight->Activate(false);
		}
		if (status == STATE_STAGE1 && GetLiftOffFlag()) {
			SRBLight[0]->Activate(true);
			SRBLight[1]->Activate(true);
		}
		else {
			SRBLight[0]->Activate(false);
			SRBLight[1]->Activate(false);
		}

		//double time=st.Stop();
		//sprintf_s(oapiDebugString(), 255, "PreStep time: %f Subsystem time: %f", time, subTime);
		//oapiWriteLog(oapiDebugString());

		if (status > STATE_PRELAUNCH) UpdateMassAndCoG(); // TODO: refine

	}
	catch (std::exception &e)
	{
		char buffer[400];
		sprintf_s(buffer, 400, "(SpaceShuttleUltra) [ERROR] Exception in clbkPreStep: %s", e.what());
		oapiWriteLog(buffer);
		exit(1);
	}
}	//Atlantis::clbkPreStep

void Atlantis::clbkPostStep(double simt, double simdt, double mjd)
{
	static bool ___PostStep_flag = false;
	
	double airspeed;
	
	OBJHANDLE hvessel;
	try
	{
		//Stopwatch st, stSub;
		//st.Start();

		if (!___PostStep_flag)
		{
			oapiWriteLog("(Atlantis::clbkPostStep) Entering.");
		}

		if (firstStep) {
			firstStep = false;
			UpdateMassAndCoG(true);
		}

		if (!___PostStep_flag)
		{
			oapiWriteLog("(Atlantis::clbkPostStep) Processing subsystems.");
		}
		//stSub.Start();
		psubsystems->PostStep(simt, simdt, mjd);
		//double subTime = stSub.Stop();

		if (!___PostStep_flag)
		{
			oapiWriteLog("(Atlantis::clbkPostStep) Processing DAP.");
		}

		if (!___PostStep_flag)
		{
			oapiWriteLog("(Atlantis::clbkPostStep) Processing panels.");
		}

		//Panel groups
		pgLeft.OnPostStep(simt, simdt, mjd);
		pgForward.OnPostStep(simt, simdt, mjd);
		pgRight.OnPostStep(simt, simdt, mjd);
		pgCenter.OnPostStep(simt, simdt, mjd);
		pgOverhead.OnPostStep(simt, simdt, mjd);
		pgOverheadAft.OnPostStep(simt, simdt, mjd);
		pgAftStbd.OnPostStep(simt, simdt, mjd);
		pgAft.OnPostStep(simt, simdt, mjd);
		pgAftPort.OnPostStep(simt, simdt, mjd);
		//stSub.Stop();


		if (!___PostStep_flag)
		{
			oapiWriteLog("(Atlantis::clbkPostStep) Executing state depending behavior.");
		}

		switch (status) {
		case STATE_PRELAUNCH: // launch configuration
			// check SSME state and trigger liftoff when required
			if (Eq(GetSSMEThrustLevel(0), 0.0, 0.0001))
			{
				if ((GetPropellantLevel(ph_mps) > 0.5) && (bSSMEGOXVent == true)) // TODO improve this venting with engine status
				{
					for (unsigned short i = 0; i < 3; i++)
					{
						if (th_ssme_gox[i] != NULL) {
							SetThrusterLevel(th_ssme_gox[i], 1.0);
						}
					}
				}
				else
				{
					for (unsigned short i = 0; i < 3; i++)
					{
						if (th_ssme_gox[i] != NULL) {
							SetThrusterLevel(th_ssme_gox[i], 0.0);
						}
					}
				}
			}
			else
			{
				status = STATE_STAGE1; // launch
				t0 = simt + SRB_STABILISATION_TIME;   // store designated liftoff time
				RecordEvent("STATUS", "SSME_IGNITION");
				//play sounds
				PlayVesselWave(SoundID, SSME_START, NOLOOP);

				for (unsigned short i = 0; i < 3; i++)
				{
					if (th_ssme_gox[i] != NULL) {
						SetThrusterLevel(th_ssme_gox[i], 0.0);
					}
				}
			}
			break;
		case STATE_STAGE1: // SRB's ignited
			//play sounds
			if (!IsPlaying(SoundID, SSME_START) && !pRSLS->GetRSLSAbortFlag())
				PlayVesselWave(SoundID, SSME_RUNNING, LOOP);
			for (unsigned short i = 0; i < 3; i++)
			{
				if (th_ssme_gox[i] != NULL) {
					SetThrusterLevel(th_ssme_gox[i], 0.0);
				}
			}

			if (!GetLiftOffFlag()) Twang(t0 - simt);

			if (met > 0.0) {
				// extract current thrust level and propellant level as a function of time
				double thrust_level, prop_level;
				GetSRB_State(met, thrust_level, prop_level);
				for (unsigned short i = 0; i < 2; i++) {
					SetThrusterLevel(th_srb[i], thrust_level);
					SetPropellantMass(ph_srb, prop_level*GetPropellantMaxMass(ph_srb));
				}

				if (met > 15.0)
				{
					slag1 = pow(1.0 - thrust_level, 3);
					slag2 = pow(1.0 - thrust_level, 2);
					slag3 = 1.0 - thrust_level;
				}
				if (pSimpleGPC->GetMajorMode() == 101) pSimpleGPC->SetMajorMode(102);
			}
			else {
				LaunchClamps();

				if (pRSLS->GetRSLSAbortFlag())// handle pad abort (sound wise)
				{
					if (IsPlaying( SoundID, SSME_RUNNING )) PlayVesselWave( SoundID, SSME_SHUTDOWN, NOLOOP );
					StopVesselWave( SoundID, SSME_RUNNING );
				}
			}

			break;

		case STATE_STAGE2: // post SRB separation
			break;
		case STATE_ORBITER: // post tank separation
			//Check if Control Surfaces are usable
			if (ControlSurfacesEnabled && !panelr2->HydraulicPressure())
			{
				DisableControlSurfaces();
			}
			else if (!ControlSurfacesEnabled && panelr2->HydraulicPressure())
			{
				EnableControlSurfaces();
			}

			//deploy gear
			airspeed = GetAirspeed();
			if (GetAltitude() < 92.44 && gear_status.action == AnimState::CLOSED) {
				DeployLandingGear();
			}
			else if (GetAltitude() < 609.6) ArmGear();

			if (GroundContact())
				if (GetAirspeed() > 1.0) SetThrusterGroupLevel(THGROUP_MAIN, 0.0); // if main thrust is nonzero, shuttle will never come to a complete stop

			//drag chute
			if (pMission->HasDragChute()) {
				if (!DragChuteDeploying &&
					(((GetAirspeed() <= CHUTE_DEPLOY_SPEED) && (GetAirspeed() > CHUTE_JETTISON_SPEED) && GroundContact()) ||
					DragChuteARM[0].IsSet() || DragChuteARM[1].IsSet() || DragChuteDPY[0].IsSet() || DragChuteDPY[1].IsSet())) {
					DragChuteDeploying = true;
					DragChuteDeployTime = met;

					DragChuteARMDPYJETTLT[0].SetLine();
					DragChuteARMDPYJETTLT[1].SetLine();
				}
				else if (DragChuteState == STOWED && DragChuteDeploying && (met - DragChuteDeployTime) > CHUTE_DEPLOY_TIME)
					DeployDragChute();
				else if (DragChuteState == DEPLOYING) { // go from initial deployment to reefed state
					DragChuteSize = min(0.4, DragChuteSize + CHUTE_DEPLOY_RATE*simdt);
					SetAnimation(anim_chute_deploy, 1 - DragChuteSize);
					if (Eq(DragChuteSize, 0.4, 0.001)) DragChuteState = REEFED;
				}
				else if (DragChuteState == REEFED) { // maintain chute in reefed state until time to fully inflate chute
					if ((met - DragChuteDeployTime) > CHUTE_INFLATE_TIME) {
						DragChuteState = INFLATED;
					}
				}
				else if (DragChuteState == INFLATED) { // fully inflate chute, then jettison it once airspeed is low enough
					// also jettison if airspeed is too high (it breaks away) or if user jettisons it
					if ((GetAirspeed() < CHUTE_JETTISON_SPEED) || (GetAirspeed() > CHUTE_FAIL_SPEED) || DragChuteJETT[0].IsSet() || DragChuteJETT[1].IsSet()) JettisonDragChute();
					else if (DragChuteSize < 1.0) {
						DragChuteSize = min(1.0, DragChuteSize + CHUTE_INFLATE_RATE*simdt);
						SetAnimation(anim_chute_deploy, 1 - DragChuteSize);
					}
				}

				//spin chute
				if (DragChuteState >= DEPLOYING && DragChuteState < JETTISONED) {
					if (DragChuteSpin.Opening()) {
						DragChuteSpin.pos = min(1.0, DragChuteSpin.pos + CHUTE_SPIN_RATE*simdt);
						if (Eq(DragChuteSpin.pos, 1.0, 0.01)) DragChuteSpin.action = AnimState::CLOSING;
					}
					else {
						DragChuteSpin.pos = max(0.0, DragChuteSpin.pos - CHUTE_SPIN_RATE*simdt);
						if (Eq(DragChuteSpin.pos, 0.0, 0.01)) DragChuteSpin.action = AnimState::OPENING;
					}
					SetAnimation(anim_chute_spin, DragChuteSpin.pos);
				}
			}

			if (do_eva) {
				char name[256];
				strcpy(name, GetName()); strcat(name, "-MMU");
				hvessel = oapiGetVesselByName(name);
				if (hvessel == 0)
				{
					SeparateMMU();
					hvessel = oapiGetVesselByName(name);
					if (hvessel != 0)
						oapiSetFocusObject(hvessel);
				}
				else
				{
					hvessel = oapiGetVesselByName(name);
					if (hvessel != 0)
						oapiSetFocusObject(hvessel);
				};
				do_eva = false;
			};

			//handle body flap and speedbrake PBIs
			if ((int)(pSimpleGPC->GetMajorMode() / 100) == 3) //Entry
			{
				//if flap is in AUTO mode, reset MAN line; otherwise set MAN line
				if (BodyFlapAutoIn) BodyFlapManOut.ResetLine();
				else BodyFlapManOut.SetLine();

				if (!SpdbkThrotAutoIn) {
					if (VCMode == VC_PLT) {
						SpdbkThrotPLTOut.SetLine();
						SpdbkThrotCDROut.ResetLine();
					}
					else {
						SpdbkThrotCDROut.SetLine();
						SpdbkThrotPLTOut.ResetLine();
					}
				}
				else {
					SpdbkThrotCDROut.ResetLine();
					SpdbkThrotPLTOut.ResetLine();
				}
			}
			//else if(pSimpleGPC->GetMajorMode() < 200) //LAUNCH
			//{
			//	BodyFlapAutoOut.ResetLine();
			//	BodyFlapManOut.ResetLine();
			//}

			break;
		}

		VESSEL *aVessel;
		VESSELSTATUS vs;

		if (reset_mmu && simt - jettison_time > .01)
		{
			GetStatus(vs);
			vs.eng_main = vs.eng_hovr = 0.0;
			VECTOR3 ofs = OFS_ZERO;
			ofs.x += OFS_MMU.x;
			ofs.y += OFS_MMU.y;
			ofs.z += OFS_MMU.z;
			VECTOR3 rofs, rvel = { vs.rvel.x, vs.rvel.y, vs.rvel.z };
			VECTOR3 vel = { 0, 0, 0 };
			Local2Rel(ofs, vs.rpos);
			GlobalRot(vel, rofs);
			vs.rvel.x = rvel.x + rofs.x;
			vs.rvel.y = rvel.y + rofs.y;
			vs.rvel.z = rvel.z + rofs.z;
			aVessel = oapiGetVesselInterface(hMMU);
			aVessel->DefSetState(&vs);
			reset_mmu = false;
		}

		// ***** Animate speedbrake *****

		if (spdb_status >= AnimState::CLOSING && panelr2->HydraulicPressure()) {
			double da = simdt * SPEEDBRAKE_OPERATING_SPEED;
			double tgt = spdb_tgt; // once speedbrake has been opened, limit position to >15%
			if (GetMachNumber() < 10.0) tgt = max(spdb_tgt, 0.15); // once speedbrake has been opened, limit position to >15%
			if (spdb_status == AnimState::CLOSING) { // retract brake
				if (spdb_proc > tgt) spdb_proc = max(tgt, spdb_proc - da);
				else                 spdb_status = AnimState::CLOSED;
			}
			else {                           // deploy antenna
				if (spdb_proc < tgt) spdb_proc = min(tgt, spdb_proc + da);
				else                 spdb_status = AnimState::OPEN;
			}
			SetAnimation(anim_spdb, spdb_proc);
		}

		// ***** Animate landing gear *****
		if (gear_status.action >= AnimState::CLOSING) {
			double da = simdt * GEAR_OPERATING_SPEED;
			if (gear_status.action == AnimState::CLOSING) { // retract gear
				if (gear_status.pos > 0.0) gear_status.pos = max(0.0, gear_status.pos - da);
				else {
					gear_status.action = AnimState::CLOSED;
					DefineTouchdownPoints();
					//UpdateVC();
				}
			}
			else {                           // deploy gear
				if (gear_status.pos < 1.0) gear_status.pos = min(1.0, gear_status.pos + da);
				else {
					gear_status.action = AnimState::OPEN;
					DefineTouchdownPoints();
					//UpdateVC();
				}
			}
			SetAnimation(anim_gear, gear_status.pos);
		}

		// ----------------------------------------------------------
		// Animate payload bay cameras.
		// ----------------------------------------------------------
		if (VCMode >= VC_PLBCAMFL && VCMode <= VC_RMSCAM) {
			if (bPLBDCamPanLeft[VCMode - VC_PLBCAMFL]) {
				PLBDCamPanLeft_Out[VCMode - VC_PLBCAMFL].SetLine();
				PLBDCamPanRight_Out[VCMode - VC_PLBCAMFL].ResetLine();
			}
			else if (bPLBDCamPanRight[VCMode - VC_PLBCAMFL]) {
				PLBDCamPanLeft_Out[VCMode - VC_PLBCAMFL].ResetLine();
				PLBDCamPanRight_Out[VCMode - VC_PLBCAMFL].SetLine();
			}
			else {
				PLBDCamPanLeft_Out[VCMode - VC_PLBCAMFL].ResetLine();
				PLBDCamPanRight_Out[VCMode - VC_PLBCAMFL].ResetLine();
			}

			if (bPLBDCamTiltUp[VCMode - VC_PLBCAMFL]) {
				PLBDCamTiltUp_Out[VCMode - VC_PLBCAMFL].SetLine();
				PLBDCamTiltDown_Out[VCMode - VC_PLBCAMFL].ResetLine();
			}
			else if (bPLBDCamTiltDown[VCMode - VC_PLBCAMFL]) {
				PLBDCamTiltUp_Out[VCMode - VC_PLBCAMFL].ResetLine();
				PLBDCamTiltDown_Out[VCMode - VC_PLBCAMFL].SetLine();
			}
			else {
				PLBDCamTiltUp_Out[VCMode - VC_PLBCAMFL].ResetLine();
				PLBDCamTiltDown_Out[VCMode - VC_PLBCAMFL].ResetLine();
			}
		}

		double camRate;
		for (int i = 0; i < 4; i++) {
			if (PTULowSpeed[i]) camRate = PTU_LOWRATE_SPEED;
			else camRate = PTU_HIGHRATE_SPEED;

			if (PLBDCamPanLeft[i])  {
				camYaw[i] = max(-MAX_PLB_CAM_TILT, camYaw[i] - camRate*simdt);
				cameraMoved = true;
			}
			else if (PLBDCamPanRight[i]) {
				camYaw[i] = min(MAX_PLB_CAM_TILT, camYaw[i] + camRate*simdt);
				cameraMoved = true;
			}

			if (PLBDCamTiltDown[i]) {
				camPitch[i] = max(-MAX_PLB_CAM_TILT, camPitch[i] - camRate*simdt);
				cameraMoved = true;
			}
			else if (PLBDCamTiltUp[i]) {
				camPitch[i] = min(MAX_PLB_CAM_TILT, camPitch[i] + camRate*simdt);
				cameraMoved = true;
			}
		}
		if (cameraMoved) {
			SetAnimationCameras();
			cameraMoved = false;
		}

		// ----------------------------------------------------------
		// Communication mode handler
		// ----------------------------------------------------------

		pCommModeHandler->PostStep(simt, simdt);

		// ----------------------------------------------------------
		// VC position label display
		// ----------------------------------------------------------
		if (fTimeCameraLabel > 0)
		{
			fTimeCameraLabel -= simdt;
			if (fTimeCameraLabel < 0)
				fTimeCameraLabel = 0;
			if (0 == fTimeCameraLabel)
			{
				oapiAnnotationSetText(nhCameraLabel, NULL);
			}
		}

		//double time=st.Stop();
		//sprintf_s(oapiDebugString(), 255, "PostStep time: %f", time);
		//oapiWriteLog(oapiDebugString());

		if (!___PostStep_flag)
		{
			oapiWriteLog("(Atlantis::clbkPostStep) Leaving.");
			___PostStep_flag = true;
		}


		/*if(simt - tt >= 5)
		{
		tt = simt;
		FILEHANDLE f1 = oapiOpenFile("SSUDensity.txt",FILE_APP);
		oapiWriteItem_float(f1,"",GetAtmDensity());
		oapiCloseFile(f1, FILE_APP);

		FILEHANDLE f2 = oapiOpenFile("SSUVelocity.txt",FILE_APP);
		oapiWriteItem_float(f2,"",GetAirspeed());
		oapiCloseFile(f2,FILE_APP);

		FILEHANDLE f3 = oapiOpenFile("SSUAltitude.txt",FILE_APP);
		oapiWriteItem_float(f3,"",GetAltitude());
		oapiCloseFile(f3,FILE_APP);
		}*/

		//Calculations used to modulate the alpha level (AKA visibility) of the entry plasma mesh
		double dens = GetAtmDensity();
		double speed = GetAirspeed();
		double flux = (dens*pow(speed, 3)) / 3 / 1000000;
		double heating_factor = flux / 4 - 3.5;
		double heating_scalar = 0;
		if (heating_factor >= 1)
			heating_scalar = 1;
		else if (heating_factor <= 0)
			heating_scalar = 0;
		else
			heating_scalar = heating_factor;

		//sprintf(oapiDebugString(),"%lf",heating_scalar);



		if (heating_scalar == 0)
			SetMeshVisibilityMode(mesh_heatshield, MESHVIS_NEVER);
		else
			SetMeshVisibilityMode(mesh_heatshield, MESHVIS_ALWAYS);



		//REENTRY HEAT SHIELD
		if (hDevHeatShieldMesh)
		{
			oapiSetMeshProperty(hDevHeatShieldMesh, MESHPROPERTY_MODULATEMATALPHA, (DWORD)1);
			oapiSetMeshProperty(hHeatShieldMesh, MESHPROPERTY_MODULATEMATALPHA, (DWORD)1);
			DWORD i, num = oapiMeshMaterialCount(hHeatShieldMesh);
			for (i = 0; i < num; i++)
			{
				MATERIAL *mat1 = oapiMeshMaterial(hHeatShieldMesh, i);
				MATERIAL mat2;
				memcpy(&mat2, mat1, sizeof(MATERIAL));
				mat2.ambient.a = static_cast<float>(heating_scalar);
				mat2.ambient.b = 255;
				mat2.ambient.r = 255;
				mat2.ambient.g = 255;
				mat2.diffuse.a = static_cast<float>(heating_scalar);
				mat2.emissive.a = static_cast<float>(heating_scalar);
				mat2.specular.a = static_cast<float>(heating_scalar);
				oapiSetMaterial(hDevHeatShieldMesh, i, &mat2);
				ZeroMemory(&mat2, sizeof(MATERIAL));
				mat1 = NULL;
			}

		}

		// landing gear PB
		if ((LandingGearArmDeployPB[0].IsSet() == true) || (LandingGearArmDeployPB[2].IsSet() == true)) ArmGear();
		if ((GearArmed() == true) && ((LandingGearArmDeployPB[1].IsSet() == true) || (LandingGearArmDeployPB[3].IsSet() == true))) DeployLandingGear();

		// landing gear position switches
		if (gear_status.action == AnimState::CLOSED)
		{
			// uplock on / downlock off
			LandingGearPosition[0].SetLine();
			LandingGearPosition[1].ResetLine();

			LandingGearPosition[2].SetLine();
			LandingGearPosition[3].ResetLine();

			LandingGearPosition[4].SetLine();
			LandingGearPosition[5].ResetLine();
		}
		else if (gear_status.action == AnimState::OPEN)
		{
			// uplock off / downlock on
			LandingGearPosition[0].ResetLine();
			LandingGearPosition[1].SetLine();

			LandingGearPosition[2].ResetLine();
			LandingGearPosition[3].SetLine();

			LandingGearPosition[4].ResetLine();
			LandingGearPosition[5].SetLine();
		}
		else
		{
			// uplock off / downlock off
			LandingGearPosition[0].ResetLine();
			LandingGearPosition[1].ResetLine();

			LandingGearPosition[2].ResetLine();
			LandingGearPosition[3].ResetLine();

			LandingGearPosition[4].ResetLine();
			LandingGearPosition[5].ResetLine();
		}

		//double time = st.Stop();
		//sprintf_s(oapiDebugString(), 255, "PostStep time: %f Subsystem time: %f", time, subTime);
		//sprintf(oapiDebugString(),"Heating scalar %lf",heating_scalar);

	}
	catch (std::exception &e)
	{
		char buffer[400];
		sprintf_s(buffer, 400, "(SpaceShuttleUltra) [ERROR] Exception in clbkPostStep: %s", e.what());
		oapiWriteLog(buffer);
		exit(1);
	}
}   //Atlantis::clbkPostStep

// --------------------------------------------------------------
// Respond to playback event
// --------------------------------------------------------------
bool Atlantis::clbkPlaybackEvent(double simt, double event_t, const char *event_type, const char *event)
{
	if (!_stricmp(event_type, "STATUS")) {
		if (!_stricmp(event, "SRB_IGNITION")) {
			status = 1;
			t0 = event_t + SRB_STABILISATION_TIME;
			return true;
		}
	}/* else if (!_stricmp (event_type, "CARGODOOR")) {
	  plop->SetDoorAction (!_stricmp (event, "CLOSE") ? AnimState::CLOSING : AnimState::OPENING);
	  return true;
	  }*/ else if (!_stricmp(event_type, "GEAR")) {
		if (!_stricmp(event, "UP")) DeployLandingGear();
		else RetractLandingGear();
		return true;
	}
	  else if (!_stricmp(event_type, "SPEEDBRAKE")) {
		  OperateSpeedbrake(!_stricmp(event, "CLOSE") ? AnimState::CLOSING : AnimState::OPENING);
		  return true;
	  }/* else if (!_stricmp (event_type, "KUBAND")) {
		plop->SetKuAntennaAction (!_stricmp (event, "CLOSE") ? AnimState::CLOSING : AnimState::OPENING);
		return true;
		}*/ else if (psubsystems->PlaybackEvent(simt, event_t, event_type, event)) {
		  return true;
	  }

	  return false;
}

bool Atlantis::clbkDrawHUD(int mode, const HUDPAINTSPEC *hps, oapi::Sketchpad *skp)
{
	pSimpleGPC->OnDrawHUD(hps, skp);
	return true;
}

// --------------------------------------------------------------
// Atlantis mesh loaded
// --------------------------------------------------------------
void Atlantis::clbkVisualCreated(VISHANDLE _vis, int refcount)
{
	oapiWriteLog("(Atlantis::clbkVisualCreated) Entered.");
	if (refcount > 1) return; // we don't support more than one visual per object
	vis = _vis;

	// get device-specific mesh handles
	hDevOrbiterMesh = GetDevMesh(vis, mesh_orbiter);
	oapiWriteLog("GETTING DEVMESH");
	hDevHeatShieldMesh = GetDevMesh(vis, mesh_heatshield);

#ifdef UNDEF
	// note: orbiter re-applies the animations to the mesh before calling
	// clbkVisualCreated, so updating the mesh here is not necessary

	// reset grappling point
	//wrist_yaw_joint[0] = _V(-2.87, 2.03, -4.88);
	//wrist_yaw_joint[1] = _V(-2.87, 2.03, -4.88)+RotateVectorZ(_V(0.0, 1.0, 0.0), 18.435);
	arm_tip[0] = _V(-2.87, 2.03, -6.27);
	arm_tip[1] = _V(-2.87, 2.03, -7.27);
	arm_tip[2] = _V(-2.87, 3.03, -6.27);

	UpdateMesh ();
#endif
	if (!pMission->GetOrbiterTextureName().empty())
		UpdateOrbiterTexture(pMission->GetOrbiterTextureName());

	// hide tail which is not used on this flight
	GROUPEDITSPEC grpSpec;
	grpSpec.flags = GRPEDIT_SETUSERFLAG;
	grpSpec.UsrFlag = 0x00000002; // hide group
	if (pMission->HasDragChute())
		oapiEditMeshGroup(hDevOrbiterMesh, GRP_TAIL_NO_CHUTE, &grpSpec);
	else
		oapiEditMeshGroup(hDevOrbiterMesh, GRP_TAIL_CHUTE, &grpSpec);

	// hide bridgerails
	for (unsigned int i = 0; i < 13; i++) {
		if (!pMission->HasBridgerail(i)) oapiEditMeshGroup(hDevOrbiterMesh, GRP_BAY1_LONGERON + i, &grpSpec);
	}
	
	// hide bay 13 covers
	if (hasCISS) oapiEditMeshGroup( hDevOrbiterMesh, GRP_PLB_BAY13_COVERS, &grpSpec );

	if (pExtAirlock) dynamic_cast<eva_docking::ExtAirlock*>(pExtAirlock)->VisualCreated( vis );

	// update UVs for talkbacks and lights
	oapiWriteLog( "Started Panels UpdateUVState" );
	pgForward.UpdateUVState();
	pgLeft.UpdateUVState();
	pgCenter.UpdateUVState();
	pgRight.UpdateUVState();
	pgOverhead.UpdateUVState();
	pgOverheadAft.UpdateUVState();
	pgAftStbd.UpdateUVState();
	pgAft.UpdateUVState();
	pgAftPort.UpdateUVState();
	oapiWriteLog( "Ended Panels UpdateUVState" );

	oapiWriteLog("(Atlantis::clbkVisualCreated) Leaving.");
}

// --------------------------------------------------------------
// Atlantis mesh discarded
// --------------------------------------------------------------
void Atlantis::clbkVisualDestroyed(VISHANDLE _vis, int refcount)
{
	if (vis == _vis) vis = NULL;
	hDevHeatShieldMesh = NULL;
}

// --------------------------------------------------------------
// Update mesh animation state
// --------------------------------------------------------------
void Atlantis::clbkAnimate(double simt)
{
	UpdateMesh();
}

// --------------------------------------------------------------
// Respond to MFD mode change
// --------------------------------------------------------------
void Atlantis::clbkMFDMode(int mfd, int mode)
{
	oapiVCTriggerRedrawArea(-1, AID_CDR1_LABEL + mfds[mfd]);

	//get pointer to CRT MFD as required
	if (newmfd != NULL && mode != MFD_REFRESHBUTTONS) {
		newmfd->MDUID = mfds[mfd]; //index of MDU associated with MFD
		newmfd->UpdateStatus = true;
		newmfd = NULL; //reset newmfd so it can be used by next new instance of CRT MFD
	}
}

// --------------------------------------------------------------
// Load generic glass cockpit mode
// --------------------------------------------------------------
bool Atlantis::clbkLoadGenericCockpit()
{
	SetCameraOffset(_V(orbiter_ofs.x - 0.67, orbiter_ofs.y + 2.55, orbiter_ofs.z + 14.4));
	SetCameraDefaultDirection(_V(0, 0, 1));
	return true;
}

// --------------------------------------------------------------
// Load virtual cockpit mode
// --------------------------------------------------------------
bool Atlantis::clbkLoadVC(int id)
{
	bool ok = false;
	bool bUpdateVC = false;
	std::set<int> InactiveMDUs;

	// when loading scenario, use cockpit position loaded from scenario instead of default position
	if (firstStep) id = scnVCMode;

	// Get the VC Mode.
	VCMode = id;
	//Reset Clip Radius settings
	SetClipRadius(0.0);

	// VC Cockpit not visible from Payload cameras or RMS camera.
	// 080415, DaveS edit: Fixed VC being seen from the BR PLB camera. Original line below in comment
	// Original line: if ((id > VC_PLBCAMFL && id < VC_PLBCAMBR) || id == VC_LEECAM || id == VC_DOCKCAM) {
	//if ((id == VC_PLBCAMBL || id == VC_PLBCAMBR) || id == VC_LEECAM || id == VC_DOCKCAM) {
	if (id >= VC_DOCKCAM && id <= VC_LEECAM) {
		// for PLBD/RMS camera views, show hid internal VC mesh and show cockpit mesh meant to be seen in external views
		SetMeshVisibilityMode(mesh_vc, MESHVIS_NEVER);
		SetMeshVisibilityMode(mesh_cockpit, MESHVIS_EXTERNAL | MESHVIS_VC | MESHVIS_EXTPASS);
	}
	else {
		SetMeshVisibilityMode(mesh_vc, MESHVIS_VC);
		SetMeshVisibilityMode(mesh_cockpit, MESHVIS_EXTERNAL);
	}

	if (pRMS) {
		if (id != VC_LEECAM) pRMS->SetEECameraView(false);
		if (id != VC_RMSCAM) pRMS->SetElbowCamView(false);
	}

	switch (id) {
	case VC_CDR: // commander position
		DisplayCameraLabel(VC_LBL_CDR);
		SetCameraOffset(orbiter_ofs + VC_OFFSET + VC_POS_CDR);
		SetCameraDefaultDirection(_V(0, 0, 1));
		SetCameraMovement(_V(0, 0, 0.3), 0, 0, _V(-0.3, 0, 0), 75 * RAD, -5 * RAD, _V(0.3, 0, 0), -20 * RAD, -27 * RAD);
		huds.hudcnt = orbiter_ofs + VC_OFFSET + VC_HUDPOS_CDR;

		if (bHasODS) oapiVCSetNeighbours(VC_PANELL4, VC_PLT, VC_DOCKCAM, VC_MS1);
		else oapiVCSetNeighbours(VC_PANELL4, VC_PLT, VC_PLBCAMFL, VC_MS1);

		InactiveMDUs.insert(vc::MDUID_AFD);
		InactiveMDUs.insert(vc::MDUID_CRT4);

		// Default camera rotarion
		SetCameraRotationRange(144 * RAD, 144 * RAD, 100 * RAD, 50 * RAD);

		//HideMidDeck();

		pgCenter.RegisterVC();
		pgForward.RegisterVC();
		pgLeft.RegisterVC();
		pgOverhead.RegisterVC();

		ok = true;
		bUpdateVC = true;
		break;
	case VC_PLT: // pilot position
		DisplayCameraLabel(VC_LBL_PLT);
		SetCameraOffset(orbiter_ofs + VC_OFFSET + VC_POS_PLT);
		SetCameraDefaultDirection(_V(0, 0, 1));
		SetCameraMovement(_V(0, 0, 0.3), 0, 0,		//Upwards/forward
			_V(-0.3, 0, 0), 20 * RAD, -27 * RAD,			//To the left
			_V(0.2, -0.1, 0.25), -90 * RAD, -72 * RAD);	//To the right
		huds.hudcnt = orbiter_ofs + VC_OFFSET + VC_HUDPOS_PLT;

		if (bHasODS) oapiVCSetNeighbours(VC_CDR, VC_PANELR4, VC_DOCKCAM, VC_MS2);
		else oapiVCSetNeighbours(VC_CDR, VC_PANELR4, VC_PLBCAMFR, VC_MS2);

		InactiveMDUs.insert(vc::MDUID_AFD);
		InactiveMDUs.insert(vc::MDUID_CRT4);

		// Default camera rotarion
		SetCameraRotationRange(144 * RAD, 144 * RAD, 100 * RAD, 75 * RAD);

		//HideMidDeck();

		pgCenter.RegisterVC();
		pgForward.RegisterVC();
		pgRight.RegisterVC();
		pgOverhead.RegisterVC();

		ok = true;
		bUpdateVC = true;
		break;
	case VC_PANELR4:
		DisplayCameraLabel(VC_LBL_PANELR4);
		SetCameraOffset(orbiter_ofs + VC_OFFSET + VC_POS_PANELR4);
		SetCameraDefaultDirection(_V(0, -sin(15 * RAD), -cos(15 * RAD)), 90.0*RAD);
		SetCameraMovement(_V(0, 0, 0.3), 0, 0,		//Upwards/forward
			_V(-0.3, 0, 0), 20 * RAD, -27 * RAD,			//To the left
			_V(0.2, -0.1, 0.25), -90 * RAD, -72 * RAD);	//To the right

		if (bHasODS) oapiVCSetNeighbours(VC_PLT, VC_STBDSTATION, VC_DOCKCAM, VC_MS2);
		else oapiVCSetNeighbours(VC_PLT, VC_STBDSTATION, VC_PLBCAMFR, VC_MS2);

		InactiveMDUs.insert(vc::MDUID_AFD);
		InactiveMDUs.insert(vc::MDUID_CRT4);


		// Default camera rotarion
		SetCameraRotationRange(30 * RAD, 30 * RAD, 20 * RAD, 40 * RAD);

		//HideMidDeck();

		pgCenter.RegisterVC();
		pgForward.RegisterVC();
		pgRight.RegisterVC();
		pgOverhead.RegisterVC();

		ok = true;
		bUpdateVC = true;
		break;
	case VC_PANELL4:
		DisplayCameraLabel(VC_LBL_PANELL4);
		SetCameraOffset(orbiter_ofs + VC_OFFSET + VC_POS_PANELL4);
		SetCameraDefaultDirection(_V(0, 0, -1), -90.0*RAD);
		SetCameraMovement(_V(0, 0, 0.3), 0, 0,		//Upwards/forward
			_V(-0.3, 0, 0), 20 * RAD, -27 * RAD,			//To the left
			_V(0.2, -0.1, 0.25), -90 * RAD, -72 * RAD);	//To the right

		if (bHasODS) oapiVCSetNeighbours(VC_PORTSTATION, VC_CDR, VC_DOCKCAM, VC_MS1);
		else oapiVCSetNeighbours(VC_PORTSTATION, VC_CDR, VC_PLBCAMFR, VC_MS1);

		InactiveMDUs.insert(vc::MDUID_AFD);
		InactiveMDUs.insert(vc::MDUID_CRT4);


		// Default camera rotarion
		SetCameraRotationRange(30 * RAD, 30 * RAD, 20 * RAD, 40 * RAD);

		//HideMidDeck();

		pgCenter.RegisterVC();
		pgForward.RegisterVC();
		pgLeft.RegisterVC();
		pgOverhead.RegisterVC();

		ok = true;
		bUpdateVC = true;
		break;
	case VC_STBDSTATION:
		DisplayCameraLabel(VC_LBL_STBDSTATION);
		SetCameraOffset(VC_OFFSET + VC_POS_STBDSTATION + orbiter_ofs);
		SetCameraDefaultDirection(VC_DIR_STBDSTATION);
		//SetCameraMovement (_V(0,0.20,0.20), 0, 40.0*RAD, _V(0.3,-0.3,0.15), 60.0*RAD, -50.0*RAD, _V(-0.8,0,0), 0, 0);
		InactiveMDUs.insert(vc::MDUID_CDR1);
		InactiveMDUs.insert(vc::MDUID_CDR2);

		// Outside cameras neighbours
		if (bHasODS) oapiVCSetNeighbours(VC_PLT, VC_AFTPILOT, VC_DOCKCAM, VC_AFTWORKSTATION);
		else oapiVCSetNeighbours(VC_PLT, VC_AFTPILOT, VC_PLBCAMFR, VC_AFTWORKSTATION);

		// Default camera rotarion
		SetCameraRotationRange(144 * RAD, 144 * RAD, 72 * RAD, 72 * RAD);

		//HideMidDeck();

		//pgOverhead.RegisterVC();
		pgOverheadAft.RegisterVC();
		pgAftStbd.RegisterVC();
		pgAft.RegisterVC();

		ok = true;
		bUpdateVC = true;
		break;
	case VC_LEECAM: //RMS End Effector Camera
		if (pRMS) {
			DisplayCameraLabel(VC_LBL_LEECAM);
			
			pRMS->SetEECameraView(true);
			oapiVCSetNeighbours(VC_RMSCAM, -1, -1, VC_RMSSTATION);

			//HideMidDeck();

			ok = true;
		}
		else ok = false;
		break;
	case VC_RMSCAM:
		if (pRMS) {
			DisplayCameraLabel(VC_LBL_ELBOWCAM);
			pRMS->SetElbowCamView(true);

			oapiVCSetNeighbours(-1, VC_LEECAM, -1, VC_RMSSTATION);
			//HideMidDeck();
			ok = true;
		}
		else ok = false;
		break;
	case VC_PLBCAMFL: //FL Payload Bay Camera
		DisplayCameraLabel(VC_LBL_PLBCAMFL);
		if (bHasODS) oapiVCSetNeighbours(VC_PLBCAMFR, VC_PLBCAMBL, VC_LEECAM, VC_DOCKCAM);
		else if (pRMS) oapiVCSetNeighbours(VC_PLBCAMFR, VC_PLBCAMBL, VC_LEECAM, VC_RMSSTATION);
		else oapiVCSetNeighbours(VC_PLBCAMFR, VC_PLBCAMBL, -1, VC_RMSSTATION);

		//HideMidDeck();

		ok = true;

		break;
	case VC_PLBCAMFR: //FR Payload Bay Camera
		DisplayCameraLabel(VC_LBL_PLBCAMFR);
		if (bHasODS) oapiVCSetNeighbours(VC_PLBCAMBR, VC_PLBCAMFL, VC_LEECAM, VC_DOCKCAM);
		else if (pRMS) oapiVCSetNeighbours(VC_PLBCAMBR, VC_PLBCAMFL, VC_LEECAM, VC_AFTPILOT);
		else oapiVCSetNeighbours(VC_PLBCAMBR, VC_PLBCAMFL, -1, VC_AFTPILOT);

		//HideMidDeck();

		ok = true;
		break;
	case VC_PLBCAMBL: //BL Payload Bay Camera
		DisplayCameraLabel(VC_LBL_PLBCAMBL);
		if (bHasODS) oapiVCSetNeighbours(VC_PLBCAMFL, VC_PLBCAMBR, VC_LEECAM, VC_DOCKCAM);
		else if (pRMS) oapiVCSetNeighbours(VC_PLBCAMFL, VC_PLBCAMBR, VC_LEECAM, VC_RMSSTATION);
		else oapiVCSetNeighbours(VC_PLBCAMFL, VC_PLBCAMBR, -1, VC_RMSSTATION);

		//HideMidDeck();

		ok = true;
		break;
	case VC_PLBCAMBR: //BR Payload Bay Camera
		DisplayCameraLabel(VC_LBL_PLBCAMBR);
		if (bHasODS) oapiVCSetNeighbours(VC_PLBCAMBL, VC_PLBCAMFR, VC_LEECAM, VC_DOCKCAM);
		else if (pRMS) oapiVCSetNeighbours(VC_PLBCAMBL, VC_PLBCAMFR, VC_LEECAM, VC_AFTPILOT);
		else oapiVCSetNeighbours(VC_PLBCAMBL, VC_PLBCAMFR, -1, VC_AFTPILOT);

		//HideMidDeck();

		ok = true;
		break;
	case VC_DOCKCAM: //Docking camera
		DisplayCameraLabel(VC_LBL_DOCKCAM);
		SetCameraOffset(_V(orbiter_ofs.x + 0.00175, orbiter_ofs.y - 1.03/*EXTERNAL_AIRLOCK_POS.y*/ + 1.15, orbiter_ofs.z + pMission->GetExternalAirlockZPos() - 0.3275));
		SetCameraDefaultDirection(_V(0.0, 1.0, 0.0), PI);
		SetCameraRotationRange(0, 0, 0, 0);
		oapiVCSetNeighbours(-1, -1, VC_PLBCAMFL, VC_AFTPILOT);

		//HideMidDeck();

		ok = true;
		break;
	case VC_AFTPILOT: //Aft Flight Deck
		DisplayCameraLabel(VC_LBL_AFTPILOT);
		SetCameraOffset(VC_OFFSET + VC_POS_AFTPILOT + orbiter_ofs);
		SetCameraDefaultDirection(VC_DIR_AFTPILOT);
		InactiveMDUs.insert(vc::MDUID_CDR1);
		InactiveMDUs.insert(vc::MDUID_CDR2);

		// Default camera rotarion
		SetCameraRotationRange(144 * RAD, 144 * RAD, 95 * RAD, 72 * RAD);
		SetCameraMovement(VC_OFSFWD_AFTPILOT, 0, 90.0*RAD,
			_V( 0.5, -0.5, 0.0 ), 20.0 * RAD, -10.0 * RAD,
			_V(-0.4, 0.0, 0.0), 0, 0);
		// Outside cameras neighbours
		if (bHasODS) oapiVCSetNeighbours(VC_STBDSTATION, VC_RMSSTATION, VC_DOCKCAM, VC_AFTWORKSTATION);
		oapiVCSetNeighbours(VC_STBDSTATION, VC_RMSSTATION, VC_PLBCAMFR, VC_AFTWORKSTATION);

		//HideMidDeck();

		//pgOverhead.RegisterVC();
		pgAftStbd.RegisterVC();
		pgAft.RegisterVC();
		pgAftPort.RegisterVC();

		ok = true;
		bUpdateVC = true;
		break;
	case VC_RMSSTATION:
		DisplayCameraLabel(VC_LBL_RMSSTATION);
		SetCameraOffset(orbiter_ofs + VC_OFFSET + VC_POS_RMSSTATION);
		SetCameraDefaultDirection(VC_DIR_RMSSTATION);
		//SetCameraMovement (_V(0,0,0.3), 0, 0, _V(-0.3,0,0), 20*RAD, -27*RAD, _V(0.3,0,0), -75*RAD, -5*RAD);

		if (bHasODS) oapiVCSetNeighbours(VC_AFTPILOT, VC_PORTSTATION, VC_DOCKCAM, VC_AFTWORKSTATION);
		else oapiVCSetNeighbours(VC_AFTPILOT, VC_PORTSTATION, VC_PLBCAMFL, VC_AFTWORKSTATION);

		InactiveMDUs.insert(vc::MDUID_PLT1);
		InactiveMDUs.insert(vc::MDUID_PLT2);

		// Default camera rotation
		SetCameraRotationRange(144 * RAD, 144 * RAD, 72 * RAD, 72 * RAD);
		SetCameraMovement(VC_OFSFWD_AFTPILOT, 0, 90.0*RAD,
			_V(0.4, 0, 0), 0, 0,
			_V(0, -0.3, 0.15), 0, 0);

		//ShowMidDeck();

		//pgOverhead.RegisterVC();
		pgAft.RegisterVC();
		pgAftStbd.RegisterVC();
		pgAftPort.RegisterVC();

		ok = true;
		bUpdateVC = true;
		break;
	case VC_PORTSTATION:
		DisplayCameraLabel(VC_LBL_PORTSTATION);
		SetCameraOffset(orbiter_ofs + VC_OFFSET + VC_POS_PORTSTATION);
		SetCameraDefaultDirection(VC_DIR_PORTSTATION);
		//SetCameraMovement (_V(0,0,0.3), 0, 0, _V(-0.3,0,0), 20*RAD, -27*RAD, _V(0.3,0,0), -75*RAD, -5*RAD);

		if (bHasODS) oapiVCSetNeighbours(VC_RMSSTATION, VC_CDR, VC_DOCKCAM, VC_MIDDECK);
		else oapiVCSetNeighbours(VC_RMSSTATION, VC_CDR, VC_PLBCAMFL, VC_MIDDECK);

		InactiveMDUs.insert(vc::MDUID_PLT1);
		InactiveMDUs.insert(vc::MDUID_PLT2);

		// Default camera rotation
		SetCameraRotationRange(144 * RAD, 144 * RAD, 72 * RAD, 72 * RAD);

		//ShowMidDeck();

		//pgOverhead.RegisterVC();
		pgOverheadAft.RegisterVC();
		pgAft.RegisterVC();
		pgAftPort.RegisterVC();

		ok = true;
		bUpdateVC = true;
		break;
	case VC_AFTWORKSTATION:
		DisplayCameraLabel(VC_LBL_AFTWORKSTATION);
		SetCameraOffset(orbiter_ofs + VC_OFFSET + VC_POS_AFTWORKSTATION);
		SetCameraDefaultDirection(VC_DIR_AFTWORKSTATION);
		//SetCameraMovement (_V(0,0,0.3), 0, 0, _V(-0.3,0,0), 20*RAD, -27*RAD, _V(0.3,0,0), -75*RAD, -5*RAD);
		oapiVCSetNeighbours(VC_STBDSTATION, VC_PORTSTATION, VC_RMSSTATION, VC_MS1);
		InactiveMDUs.insert(vc::MDUID_CDR1);
		InactiveMDUs.insert(vc::MDUID_CDR2);

		// Default camera rotation
		SetCameraRotationRange(144 * RAD, 144 * RAD, 72 * RAD, 72 * RAD);

		//ShowMidDeck();

		pgOverhead.RegisterVC();
		pgOverheadAft.RegisterVC();
		pgAft.RegisterVC();
		pgAftStbd.RegisterVC();
		pgAftPort.RegisterVC();

		ok = true;
		bUpdateVC = true;
		break;

	case VC_MS1:
		DisplayCameraLabel(VC_LBL_MS1);
		SetCameraOffset(orbiter_ofs + VC_OFFSET + VC_POS_MS1);
		SetCameraDefaultDirection(VC_DIR_MS1);
		//SetCameraMovement (_V(0,0,0.3), 0, 0, _V(-0.3,0,0), 20*RAD, -27*RAD, _V(0.3,0,0), -75*RAD, -5*RAD);

		if (bHasODS) oapiVCSetNeighbours(VC_PORTSTATION, VC_MS2, VC_CDR, VC_DOCKCAM);
		else oapiVCSetNeighbours(VC_PORTSTATION, VC_MS2, VC_CDR, VC_PLBCAMFL);

		InactiveMDUs.insert(vc::MDUID_AFD);

		// Default camera rotation
		SetCameraRotationRange(144 * RAD, 144 * RAD, 72 * RAD, 72 * RAD);

		//HideMidDeck();

		pgForward.RegisterVC();
		pgCenter.RegisterVC();
		pgOverhead.RegisterVC();
		pgOverheadAft.RegisterVC();
		pgAft.RegisterVC();

		ok = true;
		bUpdateVC = true;
		break;
	case VC_MS2:
		DisplayCameraLabel(VC_LBL_MS2);
		SetCameraOffset(orbiter_ofs + VC_OFFSET + VC_POS_MS2);
		SetCameraDefaultDirection(VC_DIR_MS2);
		//SetCameraMovement (_V(0,0,0.3), 0, 0, _V(-0.3,0,0), 20*RAD, -27*RAD, _V(0.3,0,0), -75*RAD, -5*RAD);

		if (bHasODS) oapiVCSetNeighbours(VC_MS1, VC_STBDSTATION, VC_PLT, VC_DOCKCAM);
		else oapiVCSetNeighbours(VC_MS1, VC_STBDSTATION, VC_PLT, VC_PLBCAMFL);

		InactiveMDUs.insert(vc::MDUID_AFD);

		// Default camera rotation
		SetCameraRotationRange(144 * RAD, 144 * RAD, 72 * RAD, 72 * RAD);

		//HideMidDeck();

		pgCenter.RegisterVC();
		pgOverhead.RegisterVC();
		pgOverheadAft.RegisterVC();
		pgAftStbd.RegisterVC();

		ok = true;
		bUpdateVC = true;
		break;
	case VC_MIDDECK:

		DisplayCameraLabel(VC_LBL_MIDDECK);
		SetCameraOffset(orbiter_ofs + VC_OFFSET + VC_POS_MIDDECK);
		SetCameraDefaultDirection(VC_DIR_MIDDECK);
		//SetCameraMovement (_V(0,0,0.3), 0, 0, _V(-0.3,0,0), 20*RAD, -27*RAD, _V(0.3,0,0), -75*RAD, -5*RAD);

		//ShowMidDeck();
		// Default camera rotation
		if (pMission->HasExtAL())
		{
			oapiVCSetNeighbours(-1, -1, VC_PORTSTATION, VC_EXT_AL);
		}
		else {
			oapiVCSetNeighbours(-1, -1, VC_PORTSTATION, -1);
		}

		SetCameraRotationRange(144 * RAD, 144 * RAD, 72 * RAD, 72 * RAD);

		ok = true;
		break;
	case VC_EXT_AL:
		DisplayCameraLabel(VC_LBL_EXT_AL);
		SetCameraOffset(orbiter_ofs + VC_OFFSET + VC_POS_EXT_AL + _V( 0, 0, pMission->GetExternalAirlockZPos() ));
		SetCameraDefaultDirection(VC_DIR_EXT_AL);

		SetCameraRotationRange(144 * RAD, 144 * RAD, 72 * RAD, 72 * RAD);

		if (bHasODS) oapiVCSetNeighbours(-1, -1, VC_MIDDECK, VC_DOCKCAM);
		else oapiVCSetNeighbours(-1, -1, VC_MIDDECK, -1);

		//ShowMidDeck();

		ok = true;
		break;

	}

	// Common action for external payload cameras
	if (id >= VC_DOCKCAM && id <= VC_LEECAM) {
		// Pan and tilt from camera control not from alt + arrow but from the dialog
		SetCameraRotationRange(0, 0, 0, 0);
		// No lean for payload camera
		SetCameraMovement(_V(0, 0, 0), 0, 0, _V(0, 0, 0), 0, 0, _V(0, 0, 0), 0, 0);

		// Refresh camera meshes and view positions
		SetAnimationCameras();

		// hide panels
		HideMidDeck();
		pgAft.HidePanels();
	}
	else {
		ShowMidDeck();
		pgAft.ShowPanels();
	}

	if (bUpdateVC) {
		// register the HUDs (synced)
		oapiVCRegisterHUD(&huds);
		// register all MFD displays
		for (int i = 0, counter = 0; i < 11; i++)
		{
			//oapiRegisterMFD (MFD_LEFT+i, mfds+i);
			mfds[i] = -1;
			if (mdus[i])
			{
				if (InactiveMDUs.find(i) == InactiveMDUs.end()) {
					mdus[i]->RealizeMFD(counter);
					//mfds[i]=counter;
					mfds[counter] = i;
					counter++;
				}
				else mdus[i]->RealizeMFD(-1);
			}
			oapiVCTriggerRedrawArea(-1, AID_CDR1_LABEL + i);
		}
	}
	oapiCameraSetCockpitDir(0, 0);
	return ok;
}

// --------------------------------------------------------------
// Respond to virtual cockpit mouse event
// --------------------------------------------------------------
bool Atlantis::clbkVCMouseEvent(int id, int _event, VECTOR3 &p)
{
	if (pgForward.OnVCMouseEvent( id, _event, p ))
		return true;
	if (pgLeft.OnVCMouseEvent( id, _event, p ))
		return true;
	if (pgRight.OnVCMouseEvent( id, _event, p ))
		return true;
	if (pgCenter.OnVCMouseEvent( id, _event, p ))
		return true;
	if (pgOverhead.OnVCMouseEvent( id, _event, p ))
		return true;
	if (pgOverheadAft.OnVCMouseEvent( id, _event, p ))
		return true;
	if (pgAft.OnVCMouseEvent( id, _event, p ))
		return true;
	if (pgAftStbd.OnVCMouseEvent( id, _event, p ))
		return true;
	return false;
}

// --------------------------------------------------------------
// Respond to virtual cockpit area redraw request
// --------------------------------------------------------------
bool Atlantis::clbkVCRedrawEvent(int id, int _event, SURFHANDLE surf)
{
	if (pgForward.OnVCRedrawEvent(id, _event, surf))
		return true;
	if (pgCenter.OnVCRedrawEvent(id, _event, surf))
		return true;
	if (pgOverhead.OnVCRedrawEvent(id, _event, surf))
		return true;
	if (pgOverheadAft.OnVCRedrawEvent(id, _event, surf))
		return true;
	if (pgLeft.OnVCRedrawEvent(id, _event, surf))
		return true;
	if (pgRight.OnVCRedrawEvent(id, _event, surf))
		return true;
	if (pgAft.OnVCRedrawEvent(id, _event, surf))
		return true;
	if (pgAftStbd.OnVCRedrawEvent(id, _event, surf))
		return true;
	if (pgAftPort.OnVCRedrawEvent(id, _event, surf))
		return true;
	return false;
}

bool Atlantis::RegisterMDU(unsigned short usMDUID, vc::MDU* pMDU)
{
	if (usMDUID < 11)
	{
		if (mdus[usMDUID] != NULL)
		{
			return false;
		}
		else {
			mdus[usMDUID] = pMDU;
			return true;
		}
	}
	else {
		return false;
	}
}

// --------------------------------------------------------------
// Keyboard interface handler (buffered key events)
// --------------------------------------------------------------
int Atlantis::clbkConsumeBufferedKey(DWORD key, bool down, char *kstate)
{
	if (!down) {
		if (KEYMOD_ALT(kstate)) {
			if (VCMode >= VC_PLBCAMFL && VCMode <= VC_RMSCAM) {
				switch (key) {
				case OAPI_KEY_LEFT:
					//PLBDCamPanLeft_Out[VCMode-VC_PLBCAMFL].ResetLine();
					bPLBDCamPanLeft[VCMode - VC_PLBCAMFL] = false;
					return 1;
				case OAPI_KEY_RIGHT:
					//PLBDCamPanRight_Out[VCMode-VC_PLBCAMFL].ResetLine();
					bPLBDCamPanRight[VCMode - VC_PLBCAMFL] = false;
					return 1;
				case OAPI_KEY_UP:
					//PLBDCamTiltUp_Out[VCMode-VC_PLBCAMFL].ResetLine();
					bPLBDCamTiltUp[VCMode - VC_PLBCAMFL] = false;
					return 1;
				case OAPI_KEY_DOWN:
					//PLBDCamTiltDown_Out[VCMode-VC_PLBCAMFL].ResetLine();
					bPLBDCamTiltDown[VCMode - VC_PLBCAMFL] = false;
					return 1;
				default:
					return 0;
				}
			}
		}
		else if (!KEYMOD_ALT(kstate) && !KEYMOD_SHIFT(kstate)) { // Ctrl or unmodified
			switch (key) {
			case OAPI_KEY_LEFT:
			case OAPI_KEY_RIGHT:
				AltKybdInput.y = 0.0;
				return 1;
			case OAPI_KEY_INSERT:
			case OAPI_KEY_DELETE:
				AltKybdInput.x = 0.0;
				return 1;
			case OAPI_KEY_UP:
			case OAPI_KEY_DOWN:
				AltKybdInput.z = 0.0;
				return 1;
			case OAPI_KEY_RETURN:
			case OAPI_KEY_BACK:
				RMSGrapple.ResetLine();
				RMSRelease.ResetLine();
				return 1;
			default:
				return 0;
			}
		}
	}

	if (pCommModeHandler->IsInCommMode())
	{
		return pCommModeHandler->ConsumeBufferedKey(key, down, kstate);
	}

	if (KEYMOD_CONTROL(kstate)) {
		switch (key) {
		case OAPI_KEY_G:
			ArmGear();
			return 1;
		case OAPI_KEY_A:
			ControlRMS = !ControlRMS;
			if (ControlRMS) DisplayCameraLabel(TEXT_RMSCONTROL);
			else DisplayCameraLabel(TEXT_RCSCONTROL);
			return 1;
		case OAPI_KEY_O:
			if (RMSSpeedIn) RMSSpeedOut.ResetLine();
			else RMSSpeedOut.SetLine();
			return 1;
		case OAPI_KEY_RETURN:
			RMSGrapple.SetLine();
			RMSRelease.ResetLine();
			return 1;
		case OAPI_KEY_BACK:
			RMSGrapple.ResetLine();
			RMSRelease.SetLine();
			return 1;
		case OAPI_KEY_X: //temporary
			if (status == STATE_PRELAUNCH)
			{
				bSSMEGOXVent = true;//!bSSMEGOXVent;
			}
			return 1;
		case OAPI_KEY_1: //temporary
			if (pRMS) pRMS->ToggleJointAngleDisplay();
			return 1;
		case OAPI_KEY_2:
			FireAllNextManifold();
			return 1;
		case OAPI_KEY_3:
			pgForward.ToggleCoordinateDisplayMode();
			pgCenter.ToggleCoordinateDisplayMode();
			pgRight.ToggleCoordinateDisplayMode();
			pgLeft.ToggleCoordinateDisplayMode();
			pgOverhead.ToggleCoordinateDisplayMode();
			pgOverheadAft.ToggleCoordinateDisplayMode();
			pgAft.ToggleCoordinateDisplayMode();
			pgAftStbd.ToggleCoordinateDisplayMode();
			pgAftPort.ToggleCoordinateDisplayMode();
			sprintf_s(oapiDebugString(), 255, "COORDINATE DISPLAY MODE");
			return 1;
		}
	}
	else if (KEYMOD_ALT(kstate)) {
		if (VCMode >= VC_PLBCAMFL && VCMode <= VC_RMSCAM) {
			switch (key) {
			case OAPI_KEY_LEFT:
				//PLBDCamPanLeft_Out[VCMode-VC_PLBCAMFL].SetLine();
				bPLBDCamPanLeft[VCMode - VC_PLBCAMFL] = true;
				return 1;
			case OAPI_KEY_RIGHT:
				//PLBDCamPanRight_Out[VCMode-VC_PLBCAMFL].SetLine();
				bPLBDCamPanRight[VCMode - VC_PLBCAMFL] = true;
				return 1;
			case OAPI_KEY_UP:
				//PLBDCamTiltUp_Out[VCMode-VC_PLBCAMFL].SetLine();
				bPLBDCamTiltUp[VCMode - VC_PLBCAMFL] = true;
				return 1;
			case OAPI_KEY_DOWN:
				//PLBDCamTiltDown_Out[VCMode-VC_PLBCAMFL].SetLine();
				bPLBDCamTiltDown[VCMode - VC_PLBCAMFL] = true;
				return 1;
			default:
				return 0;
			}
		}
	}
	else if (!KEYMOD_SHIFT(kstate) && !KEYMOD_ALT(kstate)) { // unmodified keys
		switch (key) {
			/*case OAPI_KEY_TAB:
				pCommModeHandler->EnterCommMode();
				break;*/
		case OAPI_KEY_8:
			ToggleGrapple();
			return 1;
		/*case OAPI_KEY_E:
			  do_eva = true;
			  return 1;*/
		case OAPI_KEY_COMMA:
			// speedbrake is tied to throttle setting, so close sppedbrake by decrementing Orbiter main engine throttle
			if (!Playback() && !GroundContact() && pSimpleGPC->GetMajorMode() >= 304) IncThrusterGroupLevel(THGROUP_MAIN, -0.05);
			return 1;
		case OAPI_KEY_PERIOD:
			// speedbrake is tied to throttle setting, so close sppedbrake by decrementing Orbiter main engine throttle
			if (!Playback() && !GroundContact() && pSimpleGPC->GetMajorMode() >= 304) IncThrusterGroupLevel(THGROUP_MAIN, 0.05);
			return 1;
		case OAPI_KEY_G:
			DeployLandingGear();
			return 1;
		case OAPI_KEY_LEFT:
			AltKybdInput.y = -1.0;
			return 1;
		case OAPI_KEY_RIGHT:
			AltKybdInput.y = 1.0;
			return 1;
		case OAPI_KEY_INSERT:
			AltKybdInput.x = -1.0;
			return 1;
		case OAPI_KEY_DELETE:
			AltKybdInput.x = 1.0;
			return 1;
		case OAPI_KEY_UP:
			AltKybdInput.z = 1.0;
			return 1;
		case OAPI_KEY_DOWN:
			AltKybdInput.z = -1.0;
			return 1;
		case OAPI_KEY_MULTIPLY: // NUMPAD *
			for (int i = 0; i < 3; i++) SSMEPBAnalog[i].SetLine();
			return 0; // this key is used by Orbitersim, so make sure Orbitersim processes it as well
		}
	}
	return 0;
}

bool Atlantis::SetSSMEParams(unsigned short usMPSNo, double fThrust0, double fISP0, double fISP1)
{
	if (usMPSNo == 0) {
		//Set all
		return SetSSMEParams(1, fThrust0, fISP0, fISP1) &&
			SetSSMEParams(2, fThrust0, fISP0, fISP1) &&
			SetSSMEParams(3, fThrust0, fISP0, fISP1);
	}
	else if (usMPSNo > 3) {
		//error
		return false;
	}
	else {
		if (th_main[usMPSNo - 1] == NULL) {
			if (!fSSMEHandleErrorFlag) {
				oapiWriteLog("Atlantis::SetSSMEParams : No SSME handle ready");
				fSSMEHandleErrorFlag = true;
			}
			return false;
		}

		SetThrusterMax0(th_main[usMPSNo - 1], fThrust0);
		SetThrusterIsp(th_main[usMPSNo - 1], fISP0, fISP1);
		fSSMEHandleErrorFlag = false;
		return true;
	}
}

bool Atlantis::SetSSMEDir(unsigned short usMPSNo, const VECTOR3& dir)
{
	if (usMPSNo == 0) {
		//Set all
		return SetSSMEDir(1, dir) &&
			SetSSMEDir(2, dir) &&
			SetSSMEDir(3, dir);
	}
	else if (usMPSNo > 3) {
		//error
		return false;
	}
	else {
		if (th_main[usMPSNo - 1] == NULL) {
			if (!fSSMEHandleErrorFlag) {
				oapiWriteLog("Atlantis::SetSSMEDir : No SSME handle ready");
				fSSMEHandleErrorFlag = true;
			}
			return false;
		}
		SetThrusterDir(th_main[usMPSNo - 1], dir);
	}
	fSSMEHandleErrorFlag = false;
	return true;
}

bool Atlantis::SetSSMEGimbalAngles(unsigned usMPSNo, double degPitch, double degYaw)
{
	if (usMPSNo == 0) {
		return SetSSMEGimbalAngles(1, degPitch, degYaw) &&
			SetSSMEGimbalAngles(2, degPitch, degYaw) &&
			SetSSMEGimbalAngles(3, degPitch, degYaw);
	}
	else if (usMPSNo > 3) {
		return false; // error
	}
	else {
		VECTOR3 dir = RotateVectorX(SSMEInstalledNullPos[usMPSNo - 1], range(-10.5, degPitch, 10.5));
		SSMECurrentPos[usMPSNo - 1] = RotateVectorY(dir, range(-8.5, degYaw, 8.5));
		UpdateSSMEGimbalAnimations();
		return SetSSMEDir(usMPSNo, SSMECurrentPos[usMPSNo - 1]);
	}
}

bool Atlantis::SetSSMEThrustLevel(unsigned short usMPSNo, double fThrustLevel) {
	if (usMPSNo == 0) {
		//Set all
		return SetSSMEThrustLevel(1, fThrustLevel) &&
			SetSSMEThrustLevel(2, fThrustLevel) &&
			SetSSMEThrustLevel(3, fThrustLevel);
	}
	else if (usMPSNo > 3) {
		//error
		oapiWriteLog("Atlantis::SetSSMEThrustLevel : Wrong MPS ID");
		return false;
	}
	else {
		if (th_main[usMPSNo - 1] == NULL) {
			if (!fSSMEHandleErrorFlag) {
				oapiWriteLog("Atlantis::SetSSMEThrustLevel : No SSME handle ready");
				fSSMEHandleErrorFlag = true;
			}
			return false;
		}
		SetThrusterLevel(th_main[usMPSNo - 1], fThrustLevel / SSME_MAX_POWER_LEVEL);
	}
	fSSMEHandleErrorFlag = false;
	return true;
}

double Atlantis::GetSSMEThrustLevel(unsigned short usMPSNo)
{
	if (usMPSNo == 0) return (GetSSMEThrustLevel(1) + GetSSMEThrustLevel(2) + GetSSMEThrustLevel(3)) / 3.0; // return average thrust
	if (th_main[usMPSNo - 1] == NULL)
	{
		if (!fSSMEHandleErrorFlag) {
			oapiWriteLog("Atlantis::GetSSMEThrustLevel: No SSME handle ready");
			fSSMEHandleErrorFlag = true;
		}
		return -1;
	}
	fSSMEHandleErrorFlag = false;
	return GetThrusterLevel(th_main[usMPSNo - 1])*SSME_MAX_POWER_LEVEL;
}

void Atlantis::SetSRBGimbalAngles(SIDE SRB, double degPitch, double degYaw)
{
	VECTOR3 dir = RotateVectorX(SRBNullDirection[SRB], range(-5.0, degPitch, 5.0));
	dir = RotateVectorY(dir, range(-5.0, degYaw, 5.0));
	SetThrusterDir(th_srb[SRB], dir);
}

void Atlantis::IgniteSRBs()
{
	double thrust_level, prop_level;
	GetSRB_State(0.0, thrust_level, prop_level);
	for (int i = 0; i < 2; i++)
		SetThrusterLevel(th_srb[i], thrust_level);
}

void Atlantis::SetMPSDumpLevel(int vent, double level)
{
	assert((vent >= 0) && (vent <= 7) && " Atlantis::SetMPSDumpLevel.vent");
	assert((level >= 0) && (level <= 1) && " Atlantis::SetMPSDumpLevel.level");
	if (thMPSDump[vent] != NULL) SetThrusterLevel(thMPSDump[vent], level);
	return;
}

void Atlantis::SetSSMEGH2burn(int eng, bool burn)
{
	assert((eng >= 1) && (eng <= 3) && " Atlantis::SetSSMEGH2burn.eng");
	if (SSMEGH2burn[eng - 1] != NULL) DelExhaustStream(SSMEGH2burn[eng - 1]);

	if (burn == false)
	{
		SSMEGH2burn[eng - 1] = NULL;
		return;
	}

	static PARTICLESTREAMSPEC psSSMEGH2burn = {
		0,
		0.2,
		40,
		35,
		0.4,
		0.4,
		5,
		5,
		PARTICLESTREAMSPEC::EMISSIVE,
		PARTICLESTREAMSPEC::LVL_FLAT,
		1, 1,
		PARTICLESTREAMSPEC::ATM_FLAT,
		1, 1,
		0
	};
	if (GetAtmPressure() > 25000)
	{
		//psSSMEGH2burn.srcrate = 40;
		//psSSMEGH2burn.growthrate = 5;
		//psSSMEGH2burn.levelmap = PARTICLESTREAMSPEC::LVL_FLAT;
		psSSMEGH2burn.tex = oapiRegisterParticleTexture("SSU\\SSMEstream");
	}
	else
	{
		psSSMEGH2burn.srcrate = 80;
		psSSMEGH2burn.growthrate = 15;
		psSSMEGH2burn.levelmap = PARTICLESTREAMSPEC::LVL_LIN;
		psSSMEGH2burn.tex = 0;
	}

	SSMEGH2burn[eng - 1] = AddExhaustStream(th_main[eng - 1], &psSSMEGH2burn);
	return;
}


// ==============================================================
// API callback interface
// ==============================================================

// --------------------------------------------------------------
// Module initialisation
// --------------------------------------------------------------
DLLCLBK void InitModule(HINSTANCE hModule)
{
	g_Param.hDLL = hModule;
	oapiRegisterCustomControls(hModule);

	// initialize aerodynamic lookup tables
	elevonVerticalLookup.Init("Config/SSU_Elevon.csv");
	verticalLookup.Init("Config/SSU_Aero.csv");
	bodyFlapVerticalLookup.Init("Config/SSU_BodyFlap.csv");
	groundEffectLookup.Init("Config/SSU_GroundEffect.csv");
	groundEffectBodyFlapLookup.Init("Config/SSU_GroundEffectBodyFlap.csv");
	horizontalLookup.Init("Config/SSU_HorizontalAero.csv", true);
	aileronHorizontalLookup.Init("Config/SSU_Aileron.csv", true);

	g_Param.tkbk_label = oapiCreateSurface(LOADBMP(IDB_TKBKLABEL));
	g_Param.pbi_lights = oapiCreateSurface(LOADBMP(IDB_PBILIGHTS));
	if (g_Param.pbi_lights == NULL) {
		oapiWriteLog("Loading bitmap \"PBI_LIGHTS\" failed.");
	}
	g_Param.clock_digits = oapiCreateSurface(LOADBMP(IDB_CLOCKDIGITS));
	if (g_Param.clock_digits == NULL) {
		oapiWriteLog("Loading bitmap \"CLOCK_DIGITS\" failed.");
	}
	g_Param.digits_7seg = oapiCreateSurface(LOADBMP(IDB_7SEGDIGITS));
	if (g_Param.digits_7seg == NULL) {
		oapiWriteLog("Loading bitmap \"DIGITS_7SEG\" failed.");
	}

	g_Param.odslights = oapiCreateSurface(LOADBMP(IDB_ODSBUTTONS));
	if (g_Param.odslights == NULL) {
		oapiWriteLog("Loading bitmap \"ODSBUTTONS\" failed.");
	}

	g_Param.deu_characters = LOADBMP(IDB_DEUCHARACTERS);

	g_Param.deu_characters_overbright = LOADBMP(IDB_DEUCHARACTERSOVERBRIGHT);

	g_Param.ssme_lights = oapiCreateSurface(LOADBMP(IDB_SSMELIGHTS));
	if (g_Param.ssme_lights == NULL) {
		oapiWriteLog("Loading bitmap \"SSME_LIGHTS\" failed.");
	}

	g_Param.a8_lights = oapiCreateSurface(LOADBMP(IDB_A8LIGHTS));
	if (g_Param.a8_lights == NULL)
	{
		oapiWriteLog("Loading bitmap \"A8_LIGHTS\" failed.");
	}

	HDC Temp1DC = CreateDC("DISPLAY", NULL, NULL, NULL);
	g_Param.DeuCharBitmapDC = CreateCompatibleDC(Temp1DC);
	SelectObject(g_Param.DeuCharBitmapDC, g_Param.deu_characters);
	SetStretchBltMode(g_Param.DeuCharBitmapDC, HALFTONE);
	StretchBlt(g_Param.DeuCharBitmapDC, 0, 0, 80, 144, g_Param.DeuCharBitmapDC, 0, 0, 288, 528, SRCCOPY);
	DeleteDC(Temp1DC);

	Temp1DC = CreateDC("DISPLAY", NULL, NULL, NULL);
	g_Param.DeuCharOvrBrgtBitmapDC = CreateCompatibleDC(Temp1DC);
	SelectObject(g_Param.DeuCharOvrBrgtBitmapDC, g_Param.deu_characters_overbright);
	SetStretchBltMode(g_Param.DeuCharOvrBrgtBitmapDC, HALFTONE);
	StretchBlt(g_Param.DeuCharOvrBrgtBitmapDC, 0, 0, 80, 144, g_Param.DeuCharOvrBrgtBitmapDC, 0, 0, 288, 528, SRCCOPY);
	DeleteDC(Temp1DC);

	// allocate GDI resources
	g_Param.font[0] = CreateFont(-11, -3, 0, 0, 400, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");
}

DLLCLBK void ExitModule(HINSTANCE hModule)
{
	oapiUnregisterCustomControls(hModule);
	DeleteDC(g_Param.DeuCharBitmapDC);
	DeleteDC(g_Param.DeuCharOvrBrgtBitmapDC);
	if (g_Param.tkbk_label)
	{
		oapiDestroySurface(g_Param.tkbk_label);
	}
	if (g_Param.pbi_lights)
	{
		oapiDestroySurface(g_Param.pbi_lights);
	}
	if (g_Param.clock_digits)
	{
		oapiDestroySurface(g_Param.clock_digits);
	}
	if (g_Param.digits_7seg)
	{
		oapiDestroySurface(g_Param.digits_7seg);
	}
	if (g_Param.odslights)
	{
		oapiDestroySurface(g_Param.odslights);
	}

	if (g_Param.ssme_lights)
	{
		oapiDestroySurface(g_Param.ssme_lights);
	}

	if (g_Param.a8_lights)
	{
		oapiDestroySurface(g_Param.a8_lights);
	}

	if (g_Param.deu_characters)
	{
		DeleteObject(g_Param.deu_characters);
	}

	if (g_Param.deu_characters_overbright)
	{
		DeleteObject(g_Param.deu_characters_overbright);
	}

	// deallocate GDI resources
	DeleteObject(g_Param.font[0]);
}

// --------------------------------------------------------------
// Vessel initialisation
// --------------------------------------------------------------
DLLCLBK VESSEL *ovcInit(OBJHANDLE hvessel, int flightmodel)
{
	return new Atlantis(hvessel, flightmodel);
}

// --------------------------------------------------------------
// Vessel cleanup
// --------------------------------------------------------------
DLLCLBK void ovcExit(VESSEL *vessel)
{
	if (vessel) delete (Atlantis*)vessel;
}

DLLCLBK bool gpcReadValue(VESSEL* pVessel, UINT gpc, UINT val_index, DWORD* value)
{
	return false;
}

DLLCLBK bool gpcSetValue(VESSEL* pVessel, UINT gpc, UINT val_index, const DWORD value)
{
	return false;
}

bool Atlantis::GetLiftOffFlag() const
{
	return bLiftOff;
}

void Atlantis::TriggerLiftOff()
{
	bLiftOff = true;
	t0 = oapiGetSimTime(); //update t0 value to actual (instead of planned) liftoff time
	pMTU->StartMET();
}

short Atlantis::GetSRBChamberPressure(unsigned short which_srb)
{
	if (which_srb < 2 && status < 2)
	{
		return (short)(530 * GetThrusterLevel(th_srb[which_srb]));
	}
	else
		return 0;
}

unsigned short Atlantis::GetGPCMET(unsigned short usGPCID, unsigned short &usDay, unsigned short &usHour, unsigned short &usMin, unsigned short &usSec)
{
	usDay = pMTU->GetMETDay(0);
	usHour = pMTU->GetMETHour(0);
	usMin = pMTU->GetMETMin(0);
	usSec = pMTU->GetMETSec(0);
	return 0;
}

short Atlantis::GetETPropellant() const
{
	Atlantis_Tank* et = GetTankInterface();

	if (et != NULL) return min((short)et->GetPropellantLevel(), 99);
	else return -1;
}

double Atlantis::GetETPropellant_B(void) const
{
	Atlantis_Tank* et = GetTankInterface();

	if (et != NULL) return et->GetPropellantLevel();
	else return -1;
}

double Atlantis::GetETLOXUllagePressure(void) const
{
	Atlantis_Tank* et = GetTankInterface();

	if (et != NULL) return et->GetLOXUllagePressure();
	else return 0;
}

double Atlantis::GetETLH2UllagePressure(void) const
{
	Atlantis_Tank* et = GetTankInterface();

	if (et != NULL) return et->GetLH2UllagePressure();
	else return 0;
}

dps::IDP* Atlantis::GetIDP(unsigned short usIDPNumber) const
{
	if (usIDPNumber == 0 || usIDPNumber > 4)
		return NULL;
	return pIDP[usIDPNumber - 1];
}

unsigned short Atlantis::GetGPCLVLHVel(unsigned short usGPCID, VECTOR3 &vel)
{
	VECTOR3 tmpVel;
	this->GetRelativeVel(GetSurfaceRef(), tmpVel);
	this->GetRelativePos(GetSurfaceRef(), LVLH_Z);

	double magVel = length(tmpVel);

	if (magVel != 0.0)
	{
		LVLH_X = tmpVel / length(tmpVel);
		LVLH_Z = -LVLH_Z / length(LVLH_Z);

		LVLH_Y = crossp(LVLH_X, LVLH_Z);

		vel.x = dotp(LVLH_X, tmpVel) * MPS2FPS;
		vel.y = dotp(LVLH_Y, tmpVel)* MPS2FPS;
		vel.z = dotp(LVLH_Z, tmpVel) * MPS2FPS;

		return 0;
	}
	else
	{
		return 1;
	}
}

short Atlantis::GetGPCRefHDot(unsigned short usGPCID, double &fRefHDot)
{
	switch (pSimpleGPC->GetMajorMode())
	{
	case 102:
		if (met < 30.0)
		{
			fRefHDot = met * 21.33;
		}
		else if (met < 50.0)
		{
			fRefHDot = 640 + (met - 30.0) * 16.7;
		}
		else if (met < 70.0)
		{
			fRefHDot = 974 + (met - 50.0) * 21.65;
		}
		else if (met < 90)
		{
			fRefHDot = 1407 + (met - 70.0) * 23.4;
		}
		else if (met < 110.0)
		{
			fRefHDot = 1875 + (met - 90.0) * 15.6;
		}
		else {
			fRefHDot = 2187 + (met - 110.0) * 15.6;
		}
		//TODO: Generate VSpeed Table.
		return VARSTATE_OK;
	}
	return VARSTATE_MISSING;
}

vc::MDU* Atlantis::GetMDU(unsigned short usMDUID) const
{
	if (usMDUID >= 0 && usMDUID < 11)
	{
		return mdus[usMDUID];
	}
	else
		return NULL;
}

inline double angle(const VECTOR3 dir, const VECTOR3 dir0)
{
	return acos(dotp(dir, dir0) /
		(length(dir) * length(dir0)));
}

void Atlantis::UpdateSSMEGimbalAnimations()
{

	const double YAWS = 2 * sin(8.5 * RAD);
	const double PITCHS = 2 * sin(10 * RAD);

	double fDeflYaw, fDeflPitch;

	// center engine
	fDeflPitch = asin(-SSMECurrentPos[0].y);
	fDeflYaw = asin(SSMECurrentPos[0].x / cos(fDeflPitch));

	SetAnimation(anim_ssmeTyaw, fDeflYaw / YAWS + 0.5);
	SetAnimation(anim_ssmeTpitch, (fDeflPitch - 16.0 *RAD) / PITCHS + 0.5);

	if (th_ssme_gox[0] != NULL) {
		SetThrusterDir(th_ssme_gox[0], SSMECurrentPos[0]);
		SetThrusterRef(th_ssme_gox[0], orbiter_ofs + SSMET_GOX_REF1);
	}

	if (thMPSDump[0] != NULL)
	{
		SetThrusterDir(thMPSDump[0], SSMECurrentPos[0]);
		SetThrusterRef(thMPSDump[0], orbiter_ofs + SSMET_GOX_REF1 + _V(-1.15, 0, -1));
	}

	// left engine
	fDeflPitch = asin(-SSMECurrentPos[1].y);
	fDeflYaw = asin(SSMECurrentPos[1].x / cos(fDeflPitch));

	SetAnimation(anim_ssmeLyaw, (fDeflYaw - 3.5 * RAD) / YAWS + 0.5);
	SetAnimation(anim_ssmeLpitch, (fDeflPitch - 10 * RAD) / PITCHS + 0.5);

	if (th_ssme_gox[1] != NULL) {
		SetThrusterDir(th_ssme_gox[1], SSMECurrentPos[1]);
		SetThrusterRef(th_ssme_gox[1], orbiter_ofs + SSMEL_GOX_REF1);
	}

	if (thMPSDump[1] != NULL)
	{
		SetThrusterDir(thMPSDump[1], SSMECurrentPos[1]);
		SetThrusterRef(thMPSDump[1], orbiter_ofs + SSMEL_GOX_REF1 + _V(0, -1.15, -1));
	}

	// right engine
	fDeflPitch = asin(-SSMECurrentPos[2].y);
	fDeflYaw = asin(SSMECurrentPos[2].x / cos(fDeflPitch));

	SetAnimation(anim_ssmeRyaw, (fDeflYaw + 3.5 * RAD) / YAWS + 0.5);
	SetAnimation(anim_ssmeRpitch, (fDeflPitch - 10 * RAD) / PITCHS + 0.5);

	if (th_ssme_gox[2] != NULL) {
		SetThrusterDir(th_ssme_gox[2], SSMECurrentPos[2]);
		SetThrusterRef(th_ssme_gox[2], orbiter_ofs + SSMER_GOX_REF1);
	}

	if (thMPSDump[2] != NULL)
	{
		SetThrusterDir(thMPSDump[2], SSMECurrentPos[2]);
		SetThrusterRef(thMPSDump[2], orbiter_ofs + SSMER_GOX_REF1 + _V(-1.15, 0, -1));
	}
}

void Atlantis::AddKUBandVisual(const VECTOR3 ofs)
{
	if (mesh_kuband == MESH_UNDEFINED && bHasKUBand)
	{
		mesh_kuband = AddMesh(hKUBandMesh, &ofs);

		SetMeshVisibilityMode(mesh_kuband, MESHVIS_EXTERNAL | MESHVIS_VC | MESHVIS_EXTPASS);
	}
}

void Atlantis::DefineKUBandAnimations()
{
	UINT kidx = mesh_kuband;
	// ***** 3. Ku-band antenna animation *****

	static UINT KuBand1Grp[3] = { GRP_DEA_KU, GRP_DEA_MOUNT_KU, GRP_DEA_EQUIPMENT_KU };
	static MGROUP_ROTATE KuBand1(kidx, KuBand1Grp, 3,
		_V(2.5299, 0.3521, 9.4444), _V(0, 1, 0), (float)(-138 * RAD)); // This angle puts the Alpha/Beta gimbal boom center point at the correct orbiter relative coordinates (Xo566, Yo135)

	static UINT KuBand2Grp[1] = { GRP_ALPHA_GIMBAL_KU };
	static MGROUP_ROTATE KuBand2(kidx, KuBand2Grp, 1,
		_V(2.2113, 0.81235, 8.53828), _V(0.503871025524, 0.0, 0.863778900898), (float)(360*RAD)); //Data from the Ku band System Workbook

	static UINT KuBand3Grp[4] = { GRP_BETA_GIMBAL_BOOM_KU, GRP_GIMBAL_LOCK_STRUCTURE_KU, GRP_DISH_KU, GRP_DISH_RECEIVER_SUPPORTS_KU };
	static MGROUP_ROTATE KuBand3(kidx, KuBand3Grp, 4,
		_V(2.1733, 0.852241, 8.42364), _V(-0.468631, 0.841773, 0.267962), (float)(160*RAD));//Data from the Ku band System Workbook

	anim_kubd = CreateAnimation(0);
	LogAnim("anim_kubd", anim_kubd);
	ANIMATIONCOMPONENT_HANDLE parent = AddAnimationComponent(anim_kubd, 0, 1, &KuBand1);

	anim_kualpha = CreateAnimation( 0.773611 );// 124.3º
	LogAnim("anim_kualpha", anim_kualpha);
	parent = AddAnimationComponent(anim_kualpha, 0, 1, &KuBand2, parent);

	anim_kubeta = CreateAnimation( 0.296875 );// -27.5º
	LogAnim("anim_kubeta", anim_kubeta);
	AddAnimationComponent(anim_kubeta, 0, 1, &KuBand3, parent);
}

void Atlantis::GLSAutoSeqStart()
{
	ISSUMLP* pMLP = GetMLPInterface();
	pMLP->GLSAutoSeqStart();
}

void Atlantis::SignalGSEBreakHDP()
{
	ISSUMLP* pMLP = GetMLPInterface();
	if (pMLP) pMLP->OnT0();
}

void Atlantis::StartROFIs()
{
	ISSUMLP* pMLP = GetMLPInterface();
	pMLP->TriggerROFIs();
}

void Atlantis::CreateSSMEs(const VECTOR3 &ofs)
{
	if (!bSSMEsDefined) {
		th_main[0] = CreateThruster(ofs + SSMET_REF, SSMECurrentPos[0], ORBITER_MAIN_THRUST, ph_mps, ORBITER_MAIN_ISP0, ORBITER_MAIN_ISP1);
		th_main[1] = CreateThruster(ofs + SSMEL_REF, SSMECurrentPos[1], ORBITER_MAIN_THRUST, ph_mps, ORBITER_MAIN_ISP0, ORBITER_MAIN_ISP1);
		th_main[2] = CreateThruster(ofs + SSMER_REF, SSMECurrentPos[2], ORBITER_MAIN_THRUST, ph_mps, ORBITER_MAIN_ISP0, ORBITER_MAIN_ISP1);
		bSSMEsDefined = true;
	}

	CreateMPSGOXVents(ofs);
	DefineSSMEExhaust();

	// calculate null direction for each engine 
	UpdateNullDirections();
}

void Atlantis::DefineSSMEExhaust()
{
	int i;

	SURFHANDLE tex_main = oapiRegisterExhaustTexture("SSU\\SSME_exhaust");
	for (i = 0; i < 3; i++)
	{
		if (ex_main[i])
		{
			DelExhaust(ex_main[i]);
		}
		ex_main[i] = AddExhaust(th_main[i], 55.0, 3.0, 3.5, tex_main);
	}
}

void Atlantis::UpdateNullDirections()
{
	// calculate null direction for each engine
	if (status <= STATE_STAGE1) {
		for (unsigned short i = 0; i < 2; i++) {
			if (th_srb[i]) {
				GetThrusterRef(th_srb[i], SRBNullDirection[i]);
				SRBNullDirection[i].x = 0.0; // keep both SRBs pointing up without any gimballing left/right (otherwise, we get some losses fomr both SRBs thrusting in opposing directions)
				SRBNullDirection[i] = Normalize(-SRBNullDirection[i]);
				SetThrusterDir(th_srb[i], SRBNullDirection[i]);
			}
		}
	}
}

void Atlantis::SetKuGimbalAngles( double fAlpha, double fBeta )
{
	SetAnimation( anim_kualpha, fAlpha );
	SetAnimation( anim_kubeta, fBeta );
}

bool Atlantis::IsValidSPEC(int gpc, int spec) const
{
	switch (pSimpleGPC->GetMajorMode() / 100)
	{
	case 0:
		switch (spec)
		{
		case 1:
		case 2:
		case 6:
		case 99:
			return true;
		}
		break;
	case 1:
		switch (spec)
		{
		case 0:
		case 1:
		case 6:
		case 18:
		case 19:// HACK BFS only
		case 23:
		case 50:
		case 51:
		case 53:
		case 55:
		case 99:
			return true;
		default:
			return false;
		}
		break;
	case 2:
		switch (spec)
		{
		case 0:
		case 1:
		case 2:
		case 6:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
		case 25:
		case 33:
		case 34:
		case 55:
		case 99:
			return true;
		default:
			return false;
		}
		break;
	case 3:
		switch (spec)
		{
		case 0:
		case 1:
		case 6:
		case 18:
		case 19:// HACK BFS only
		case 21:
		case 22:
		case 23:
		case 50:
		case 51:
		case 53:
		case 55:
		case 99:
			return true;
		default:
			return false;
		}
		break;
	case 6:
		switch (spec)
		{
		case 0:
		case 1:
		case 6:
		case 18:
		case 19:// HACK BFS only
		case 23:
		case 50:
		case 51:
		case 53:
		case 55:
		case 99:
			return true;
		default:
			return false;
		}
		break;
	case 8:
		switch (spec)
		{
		case 0:
		case 1:
		case 2:
		case 6:
		case 18:
		case 19:
		case 23:
		case 40:
		case 41:
		case 42:
		case 43:
		case 44:
		case 45:
		case 55:
		case 99:
			return true;
		default:
			return false;
		}
	case 9:
		switch (spec)
		{
		case 0:
		case 1:
		case 2:
		case 6:
		case 55:
		case 62:
		case 99:
		case 100:
		case 101:
		case 102:
		case 104:
		case 105:
		case 106:
		case 112:
		case 113:
			return true;
		default:
			return false;
		}
		break;
	}
	return false;
}

unsigned int Atlantis::GetGPCMajorMode() const
{
	return pSimpleGPC->GetMajorMode();
}

double Atlantis::GetMET() const
{
	return met;
}

double Atlantis::GetTgtSpeedbrakePosition() const
{
	return spdb_tgt;
}

double Atlantis::GetActSpeedbrakePosition() const
{
	return spdb_proc;
}

double Atlantis::GetKEAS() const
{
	return 661.47 * GetMachNumber() * sqrt(GetAtmPressure() / 101325.0);
}

AnimState::Action Atlantis::GetGearState() const
{
	return gear_status.action;
}

void Atlantis::CreateOrbiterTanks()
{
	int i;
	if (!ph_oms)
		ph_oms = CreatePropellantResource(ORBITER_MAX_PROPELLANT_MASS); // OMS propellant
	for (i = 0; i < 3; i++) {
		PROPELLANT_HANDLE phTank = CreatePropellantResource(APU_FUEL_TANK_MASS);
		pAPU[i]->DefineTank(phTank);
	}

	for (i = 0; i < 2; i++) {
		if (!oms_helium_tank[i])
			oms_helium_tank[i] = CreatePropellantResource(OMS_HELIUM_TANK_MASS);
	}
	if (!ph_frcs)
		ph_frcs = CreatePropellantResource(ORBITER_FRCS_PROPELLANT_MASS);
	if (!ph_lrcs)
		ph_lrcs = CreatePropellantResource(ORBITER_LEFT_ARCS_PROPELLANT_MASS);
	if (!ph_rrcs)
		ph_rrcs = CreatePropellantResource(ORBITER_RIGHT_ARCS_PROPELLANT_MASS);
	if (!ph_controller)
		ph_controller = CreatePropellantResource(0.01); //dummy tank for controller input
}

void Atlantis::CreateFRCS(const VECTOR3 &ref_pos)
{
	ssuAssert(ph_frcs != NULL, "Forward RCS tank resource was not defined");

	CreateOrRedefineRCSThruster(&thFRCS[RCS_F2F], ref_pos + RCS_F2F_OFS, RCS_F2F_DIR, RCS_F2F_THRUST0, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thFRCS[RCS_F3F], ref_pos + RCS_F3F_OFS, RCS_F3F_DIR, RCS_F3F_THRUST0, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thFRCS[RCS_F1F], ref_pos + RCS_F1F_OFS, RCS_F1F_DIR, RCS_F1F_THRUST0, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thFRCS[RCS_F1L], ref_pos + RCS_F1L_OFS, RCS_F1L_DIR, RCS_F1L_THRUST0, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thFRCS[RCS_F3L], ref_pos + RCS_F3L_OFS, RCS_F3L_DIR, RCS_F3L_THRUST0, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thFRCS[RCS_F2R], ref_pos + RCS_F2R_OFS, RCS_F2R_DIR, RCS_F2R_THRUST0, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thFRCS[RCS_F4R], ref_pos + RCS_F4R_OFS, RCS_F4R_DIR, RCS_F4R_THRUST0, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thFRCS[RCS_F2U], ref_pos + RCS_F2U_OFS, RCS_F2U_DIR, RCS_F2U_THRUST0, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thFRCS[RCS_F3U], ref_pos + RCS_F3U_OFS, RCS_F3U_DIR, RCS_F3U_THRUST0, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thFRCS[RCS_F1U], ref_pos + RCS_F1U_OFS, RCS_F1U_DIR, RCS_F1U_THRUST0, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thFRCS[RCS_F2D], ref_pos + RCS_F2D_OFS, RCS_F2D_DIR, RCS_F2D_THRUST0, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thFRCS[RCS_F1D], ref_pos + RCS_F1D_OFS, RCS_F1D_DIR, RCS_F1D_THRUST0, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thFRCS[RCS_F4D], ref_pos + RCS_F4D_OFS, RCS_F4D_DIR, RCS_F4D_THRUST0, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thFRCS[RCS_F3D], ref_pos + RCS_F3D_OFS, RCS_F3D_DIR, RCS_F3D_THRUST0, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thFRCS[RCS_F5R], ref_pos + RCS_F5R_OFS, RCS_F5R_DIR, RCS_F5R_THRUST0, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thFRCS[RCS_F5L], ref_pos + RCS_F5L_OFS, RCS_F5L_DIR, RCS_F5L_THRUST0, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
}

void Atlantis::CreateLeftARCS(const VECTOR3 &ref_pos)
{
	ssuAssert(ph_lrcs != NULL, "Left RCS tank resource was not defined");

	CreateOrRedefineRCSThruster(&thLRCS[RCS_L3A], ref_pos + RCS_L3A_OFS, RCS_L3A_DIR, RCS_L3A_THRUST0, ph_lrcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thLRCS[RCS_L1A], ref_pos + RCS_L1A_OFS, RCS_L1A_DIR, RCS_L1A_THRUST0, ph_lrcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thLRCS[RCS_L4L], ref_pos + RCS_L4L_OFS, RCS_L4L_DIR, RCS_L4L_THRUST0, ph_lrcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thLRCS[RCS_L2L], ref_pos + RCS_L2L_OFS, RCS_L2L_DIR, RCS_L2L_THRUST0, ph_lrcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thLRCS[RCS_L3L], ref_pos + RCS_L3L_OFS, RCS_L3L_DIR, RCS_L3L_THRUST0, ph_lrcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thLRCS[RCS_L1L], ref_pos + RCS_L1L_OFS, RCS_L1L_DIR, RCS_L1L_THRUST0, ph_lrcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thLRCS[RCS_L4U], ref_pos + RCS_L4U_OFS, RCS_L4U_DIR, RCS_L4U_THRUST0, ph_lrcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thLRCS[RCS_L2U], ref_pos + RCS_L2U_OFS, RCS_L2U_DIR, RCS_L2U_THRUST0, ph_lrcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thLRCS[RCS_L1U], ref_pos + RCS_L1U_OFS, RCS_L1U_DIR, RCS_L1U_THRUST0, ph_lrcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thLRCS[RCS_L4D], ref_pos + RCS_L4D_OFS, RCS_L4D_DIR, RCS_L4D_THRUST0, ph_lrcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thLRCS[RCS_L2D], ref_pos + RCS_L2D_OFS, RCS_L2D_DIR, RCS_L2D_THRUST0, ph_lrcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thLRCS[RCS_L3D], ref_pos + RCS_L2D_OFS, RCS_L3D_DIR, RCS_L3D_THRUST0, ph_lrcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thLRCS[RCS_L5D], ref_pos + RCS_L5D_OFS, RCS_L5D_DIR, RCS_L5D_THRUST0, ph_lrcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thLRCS[RCS_L5L], ref_pos + RCS_L5L_OFS, RCS_L5L_DIR, RCS_L5L_THRUST0, ph_lrcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);

}

void Atlantis::CreateRightARCS(const VECTOR3 &ref_pos)
{
	ssuAssert(ph_rrcs != NULL, "Right RCS tank resource was not defined");

	CreateOrRedefineRCSThruster(&thRRCS[RCS_R3A], ref_pos + RCS_R3A_OFS, RCS_R3A_DIR, RCS_R3A_THRUST0, ph_rrcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thRRCS[RCS_R1A], ref_pos + RCS_R1A_OFS, RCS_R1A_DIR, RCS_R1A_THRUST0, ph_rrcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thRRCS[RCS_R4R], ref_pos + RCS_R4R_OFS, RCS_R4R_DIR, RCS_R4R_THRUST0, ph_rrcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thRRCS[RCS_R2R], ref_pos + RCS_R2R_OFS, RCS_R2R_DIR, RCS_R2R_THRUST0, ph_rrcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thRRCS[RCS_R3R], ref_pos + RCS_R3R_OFS, RCS_R3R_DIR, RCS_R3R_THRUST0, ph_rrcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thRRCS[RCS_R1R], ref_pos + RCS_R1R_OFS, RCS_R1R_DIR, RCS_R1R_THRUST0, ph_rrcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thRRCS[RCS_R4U], ref_pos + RCS_R4U_OFS, RCS_R4U_DIR, RCS_R4U_THRUST0, ph_rrcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thRRCS[RCS_R2U], ref_pos + RCS_R2U_OFS, RCS_R2U_DIR, RCS_R2U_THRUST0, ph_rrcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thRRCS[RCS_R1U], ref_pos + RCS_R1U_OFS, RCS_R1U_DIR, RCS_R1U_THRUST0, ph_rrcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thRRCS[RCS_R4D], ref_pos + RCS_R4D_OFS, RCS_R4D_DIR, RCS_R4D_THRUST0, ph_rrcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thRRCS[RCS_R2D], ref_pos + RCS_R2D_OFS, RCS_R2D_DIR, RCS_R2D_THRUST0, ph_rrcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thRRCS[RCS_R3D], ref_pos + RCS_R2D_OFS, RCS_R3D_DIR, RCS_R3D_THRUST0, ph_rrcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thRRCS[RCS_R5D], ref_pos + RCS_R5D_OFS, RCS_R5D_DIR, RCS_R5D_THRUST0, ph_rrcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
	CreateOrRedefineRCSThruster(&thRRCS[RCS_R5R], ref_pos + RCS_R5R_OFS, RCS_R5R_DIR, RCS_R5R_THRUST0, ph_rrcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
}

void Atlantis::AddPrimaryRCSExhaust(THRUSTER_HANDLE thX)
{
	VECTOR3 pos, dir;
	GetThrusterRef(thX, pos);
	GetThrusterDir(thX, dir);
	AddRCSExhaust(thX, pos, -dir);
}

void Atlantis::AddVernierRCSExhaust(THRUSTER_HANDLE thX)
{
	VECTOR3 pos, dir;
	GetThrusterRef(thX, pos);
	GetThrusterDir(thX, dir);
	AddVRCSExhaust(thX, pos, -dir);
}

void Atlantis::FireAllNextManifold()
{
	StopAllManifolds();
	int i;
	switch (___iCurrentManifold)
	{
	case 0:
		//Fire none
		return;
	case 1:
		//Fire Manifold 1
		for (i = 0; i < 4; i++)
		{
			if (thFRCS[i])
			{
				SetThrusterLevel(thFRCS[i], 1.0);
			}

		}
		___iCurrentManifold++;
		return;
	case 2:
		//Fire manifold 2
		for (i = 0; i < 4; i++)
		{
			if (thFRCS[i])
			{
				SetThrusterLevel(thFRCS[i], 1.0);
			}

		}
		___iCurrentManifold++;
		return;
	case 3:
		//Fire manifold 3
		for (i = 0; i < 4; i++)
		{
			if (thFRCS[i])
			{
				SetThrusterLevel(thFRCS[i], 1.0);
			}

		}
		___iCurrentManifold++;
		return;
	case 4:
		//Fire manifold 4
		for (i = 0; i < 2; i++)
		{
			if (thFRCS[i])
			{
				SetThrusterLevel(thFRCS[i], 1.0);
			}

		}
		___iCurrentManifold++;
		return;
	case 5:
		//Fire manifold 5 (vernier)
		for (i = 0; i < 2; i++)
		{
			if (thFRCS[i])
			{
				SetThrusterLevel(thFRCS[i], 1.0);
			}

		}
		___iCurrentManifold++;
		return;
	default:
		___iCurrentManifold = 0;
		return;
	}
}

void Atlantis::StopAllManifolds()
{
	int i;
	for (i = 0; i < 4; i++)
	{
		if (thFRCS[i])
		{
			SetThrusterLevel(thFRCS[i], 0.0);
		}

		if (thFRCS[i])
		{
			SetThrusterLevel(thFRCS[i], 0.0);
		}

		if (thFRCS[i])
		{
			SetThrusterLevel(thFRCS[i], 0.0);
		}


	}
	for (i = 0; i < 2; i++)
	{
		if (thFRCS[i])
		{
			SetThrusterLevel(thFRCS[i], 0.0);
		}
		if (thFRCS[i])
		{
			SetThrusterLevel(thFRCS[i], 0.0);
		}
	}
}

void Atlantis::SetAirDataProbeDeployment(int side, double position)
{
	if (side == 1) {
		SetAnimation(anim_adpr, position);
	}
	else if (side == 0) {
		SetAnimation(anim_adpl, position);
	}
}

void Atlantis::DisplayCameraLabel(const char* pszLabel)
{
	strcpy(pszCameraLabelBuffer, pszLabel);
	oapiAnnotationSetText(nhCameraLabel, pszCameraLabelBuffer);
	fTimeCameraLabel = 5.0;
}

void Atlantis::CreateMPSGOXVents(const VECTOR3& ref_pos)
{
	int i;

	static PARTICLESTREAMSPEC gox_stream = {
		0, 0.06, 140, 10, 0, 0.8, 1.2, 1.35, PARTICLESTREAMSPEC::DIFFUSE,
		PARTICLESTREAMSPEC::LVL_FLAT, 1, 1,
		PARTICLESTREAMSPEC::ATM_PLOG, 1e-50, 1
	};

	gox_stream.tex = oapiRegisterParticleTexture("contrail1");


	th_ssme_gox[0] = CreateThruster(ref_pos + SSMET_GOX_REF, _V(0, -0.121, 0.992), 0.0, ph_mps, 250.0, 100.0);
	th_ssme_gox[1] = CreateThruster(ref_pos + SSMEL_GOX_REF, _V(0, -0.121, 0.992), 0.0, ph_mps, 250.0, 100.0);
	th_ssme_gox[2] = CreateThruster(ref_pos + SSMER_GOX_REF, _V(0, -0.121, 0.992), 0.0, ph_mps, 250.0, 100.0);

	for (i = 0; i < 3; i++)
	{
		AddExhaustStream(th_ssme_gox[i], &gox_stream);
	}

}

void Atlantis::CreateMPSDumpVents(void)
{
	static PARTICLESTREAMSPEC psLOXdump_SSME = {
		0,
		2,
		80,
		20,
		0.4,
		0.4,
		10,
		5,
		PARTICLESTREAMSPEC::DIFFUSE,
		PARTICLESTREAMSPEC::LVL_PLIN,
		0, 1,
		PARTICLESTREAMSPEC::ATM_FLAT,
		1, 1,
		0
	};

	static PARTICLESTREAMSPEC psLOXdump_FD = {
		0,
		0.2,
		80,
		20,
		0.4,
		0.4,
		5,
		5,
		PARTICLESTREAMSPEC::DIFFUSE,
		PARTICLESTREAMSPEC::LVL_PLIN,
		0, 1,
		PARTICLESTREAMSPEC::ATM_FLAT,
		1, 1,
		0
	};

	static PARTICLESTREAMSPEC psLH2dump_BU = {
		0,
		0.0381,///<     particle size at creation [m]
		200,///<     average particle creation rate [Hz]
		15,///<     emission velocity [m/s]
		0.2,///<     velocity spread during creation
		0.3,///<     average particle lifetime [s]
		6,///<     particle growth rate [m/s]
		7,///<     slowdown rate in atmosphere
		PARTICLESTREAMSPEC::DIFFUSE,
		PARTICLESTREAMSPEC::LVL_PLIN,
		0, 1,
		PARTICLESTREAMSPEC::ATM_FLAT,
		1, 1,
		0
	};

	static PARTICLESTREAMSPEC psLH2dump_FD = {
		0,
		0.2,///<     particle size at creation [m]
		200,///<     average particle creation rate [Hz]
		15,///<     emission velocity [m/s]
		0.2,///<     velocity spread during creation
		0.35,///<     average particle lifetime [s]
		7,///<     particle growth rate [m/s]
		7,///<     slowdown rate in atmosphere
		PARTICLESTREAMSPEC::DIFFUSE,
		PARTICLESTREAMSPEC::LVL_PLIN,
		0, 1,
		PARTICLESTREAMSPEC::ATM_FLAT,
		1, 1,
		0
	};

	static PARTICLESTREAMSPEC psLH2dump_FDLN = {
		0,
		0.0254,///<     particle size at creation [m]
		200,///<     average particle creation rate [Hz]
		15,///<     emission velocity [m/s]
		0.1,///<     velocity spread during creation
		0.3,///<     average particle lifetime [s]
		2,///<     particle growth rate [m/s]
		7,///<     slowdown rate in atmosphere
		PARTICLESTREAMSPEC::DIFFUSE,
		PARTICLESTREAMSPEC::LVL_PLIN,
		0, 1,
		PARTICLESTREAMSPEC::ATM_FLAT,
		1, 1,
		0
	};

	static PARTICLESTREAMSPEC psLOXdump_FDLN = {
		0,
		0.0254,///<     particle size at creation [m]
		200,///<     average particle creation rate [Hz]
		20,///<     emission velocity [m/s]
		0.1,///<     velocity spread during creation
		0.4,///<     average particle lifetime [s]
		2,///<     particle growth rate [m/s]
		5,///<     slowdown rate in atmosphere
		PARTICLESTREAMSPEC::DIFFUSE,
		PARTICLESTREAMSPEC::LVL_PLIN,
		0, 1,
		PARTICLESTREAMSPEC::ATM_FLAT,
		1, 1,
		0
	};

	// LOX dump -> dv = 9-11 fps
	// LOX dump SSME 1
	if (thMPSDump[0] != NULL) DelThruster(thMPSDump[0]);
	thMPSDump[0] = CreateThruster(orbiter_ofs + SSMET_GOX_REF1 + _V(-1.15, 0, -1), SSMECurrentPos[0], 4000, phLOXdump, 80, 80);
	AddExhaustStream(thMPSDump[0], &psLOXdump_SSME);

	// LOX dump SSME 2
	if (thMPSDump[1] != NULL) DelThruster(thMPSDump[1]);
	thMPSDump[1] = CreateThruster(orbiter_ofs + SSMEL_GOX_REF1 + _V(-1.15, 0, -1), SSMECurrentPos[1], 4000, phLOXdump, 80, 80);
	AddExhaustStream(thMPSDump[1], &psLOXdump_SSME);

	// LOX dump SSME 3
	if (thMPSDump[2] != NULL) DelThruster(thMPSDump[2]);
	thMPSDump[2] = CreateThruster(orbiter_ofs + SSMER_GOX_REF1 + _V(-1.15, 0, -1), SSMECurrentPos[2], 4000, phLOXdump, 80, 80);
	AddExhaustStream(thMPSDump[2], &psLOXdump_SSME);

	// LH2 dump B/U
	if (thMPSDump[3] != NULL) DelThruster(thMPSDump[3]);
	thMPSDump[3] = CreateThruster(orbiter_ofs + _V(-2.7432, -2.4155, -9.4365), _V(0.993373, -0.094977, -0.064729), 60, phLH2dump, 60, 60);
	AddExhaustStream(thMPSDump[3], &psLH2dump_BU);

	// LH2 dump F/D
	if (thMPSDump[4] != NULL) DelThruster(thMPSDump[4]);
	thMPSDump[4] = CreateThruster(orbiter_ofs + _V(-2.9718, -0.4596, -12.4591), _V(1, 0, 0), 90, phLH2dump, 30, 30);
	AddExhaustStream(thMPSDump[4], &psLH2dump_FD);

	// LOX dump F/D
	if (thMPSDump[5] != NULL) DelThruster(thMPSDump[5]);
	thMPSDump[5] = CreateThruster(orbiter_ofs + _V(2.9718, -0.4596, -12.4591), _V(-1, 0, 0), 600, phLOXdump, 60, 60);
	AddExhaustStream(thMPSDump[5], &psLOXdump_FD);

	// LH2 FDLN Relief vent
	if (thMPSDump[6] != NULL) DelThruster(thMPSDump[6]);
	thMPSDump[6] = CreateThruster(orbiter_ofs + _V(-0.4171, 1.5138, -11.1097), _V(0, -0.993572, -0.113203), 34, phLH2dump, 60, 60);
	AddExhaustStream(thMPSDump[6], &psLH2dump_FDLN);

	// LOX FDLN Relief vent
	if (thMPSDump[7] != NULL) DelThruster(thMPSDump[7]);
	thMPSDump[7] = CreateThruster(orbiter_ofs + _V(2.92116, -2.5624, -11.4256), _V(-0.993373, -0.094977, -0.064729), 331, phLOXdump, 60, 60);
	AddExhaustStream(thMPSDump[7], &psLOXdump_FDLN);
	return;
}

void Atlantis::RealizeSubsystemConnections() {
	dps::ShuttleBus* pBus = BusManager()->GetBus("DK1");
	pBus->ConnectTo(&(pIDP[0]->dk_channel));
	pBus->ConnectTo(&(pGPC[0]->channel[5]));
	pBus->ConnectTo(&(pGPC[1]->channel[5]));
	pBus->ConnectTo(&(pGPC[2]->channel[5]));
	pBus->ConnectTo(&(pGPC[3]->channel[5]));
	pBus->ConnectTo(&(pGPC[4]->channel[5]));

	pBus = BusManager()->GetBus("DK2");
	pBus->ConnectTo(&(pIDP[1]->dk_channel));
	pBus->ConnectTo(&(pGPC[0]->channel[6]));
	pBus->ConnectTo(&(pGPC[1]->channel[6]));
	pBus->ConnectTo(&(pGPC[2]->channel[6]));
	pBus->ConnectTo(&(pGPC[3]->channel[6]));
	pBus->ConnectTo(&(pGPC[4]->channel[6]));

	pBus = BusManager()->GetBus("DK3");
	pBus->ConnectTo(&(pIDP[2]->dk_channel));
	pBus->ConnectTo(&(pGPC[0]->channel[7]));
	pBus->ConnectTo(&(pGPC[1]->channel[7]));
	pBus->ConnectTo(&(pGPC[2]->channel[7]));
	pBus->ConnectTo(&(pGPC[3]->channel[7]));
	pBus->ConnectTo(&(pGPC[4]->channel[7]));

	pBus = BusManager()->GetBus("DK4");
	pBus->ConnectTo(&(pIDP[3]->dk_channel));
	pBus->ConnectTo(&(pGPC[0]->channel[8]));
	pBus->ConnectTo(&(pGPC[1]->channel[8]));
	pBus->ConnectTo(&(pGPC[2]->channel[8]));
	pBus->ConnectTo(&(pGPC[3]->channel[8]));
	pBus->ConnectTo(&(pGPC[4]->channel[8]));


	pBus = BusManager()->GetBus("IC1");
	pBus->ConnectTo(&(pGPC[0]->channel[0]));
	pBus->ConnectTo(&(pGPC[1]->channel[0]));
	pBus->ConnectTo(&(pGPC[2]->channel[0]));
	pBus->ConnectTo(&(pGPC[3]->channel[0]));
	pBus->ConnectTo(&(pGPC[4]->channel[0]));
	pBus = BusManager()->GetBus("IC2");
	pBus->ConnectTo(&(pGPC[0]->channel[1]));
	pBus->ConnectTo(&(pGPC[1]->channel[1]));
	pBus->ConnectTo(&(pGPC[2]->channel[1]));
	pBus->ConnectTo(&(pGPC[3]->channel[1]));
	pBus->ConnectTo(&(pGPC[4]->channel[1]));
	pBus = BusManager()->GetBus("IC3");
	pBus->ConnectTo(&(pGPC[0]->channel[2]));
	pBus->ConnectTo(&(pGPC[1]->channel[2]));
	pBus->ConnectTo(&(pGPC[2]->channel[2]));
	pBus->ConnectTo(&(pGPC[3]->channel[2]));
	pBus->ConnectTo(&(pGPC[4]->channel[2]));
	pBus = BusManager()->GetBus("IC4");
	pBus->ConnectTo(&(pGPC[0]->channel[3]));
	pBus->ConnectTo(&(pGPC[1]->channel[3]));
	pBus->ConnectTo(&(pGPC[2]->channel[3]));
	pBus->ConnectTo(&(pGPC[3]->channel[3]));
	pBus->ConnectTo(&(pGPC[4]->channel[3]));

	pBus = BusManager()->GetBus("IC5");
	pBus->ConnectTo(&(pGPC[0]->channel[4]));
	pBus->ConnectTo(&(pGPC[1]->channel[4]));
	pBus->ConnectTo(&(pGPC[2]->channel[4]));
	pBus->ConnectTo(&(pGPC[3]->channel[4]));
	pBus->ConnectTo(&(pGPC[4]->channel[4]));

	pBus = BusManager()->GetBus("FC1");
	pBus->ConnectTo(&(pGPC[0]->channel[19]));
	pBus->ConnectTo(&(pGPC[1]->channel[19]));
	pBus->ConnectTo(&(pGPC[2]->channel[19]));
	pBus->ConnectTo(&(pGPC[3]->channel[19]));
	pBus->ConnectTo(&(pGPC[4]->channel[19]));
	pBus->ConnectTo(&(pIDP[0]->fc_channel[0]));
	pBus->ConnectTo(&(pIDP[1]->fc_channel[0]));
	pBus->ConnectTo(&(pIDP[2]->fc_channel[0]));
	pBus->ConnectTo(&(pIDP[3]->fc_channel[0]));

	pBus = BusManager()->GetBus("FC2");
	pBus->ConnectTo(&(pGPC[0]->channel[20]));
	pBus->ConnectTo(&(pGPC[1]->channel[20]));
	pBus->ConnectTo(&(pGPC[2]->channel[20]));
	pBus->ConnectTo(&(pGPC[3]->channel[20]));
	pBus->ConnectTo(&(pGPC[4]->channel[20]));
	pBus->ConnectTo(&(pIDP[0]->fc_channel[1]));
	pBus->ConnectTo(&(pIDP[1]->fc_channel[1]));
	pBus->ConnectTo(&(pIDP[2]->fc_channel[1]));
	pBus->ConnectTo(&(pIDP[3]->fc_channel[1]));


	pBus = BusManager()->GetBus("FC3");
	pBus->ConnectTo(&(pGPC[0]->channel[21]));
	pBus->ConnectTo(&(pGPC[1]->channel[21]));
	pBus->ConnectTo(&(pGPC[2]->channel[21]));
	pBus->ConnectTo(&(pGPC[3]->channel[21]));
	pBus->ConnectTo(&(pGPC[4]->channel[21]));
	pBus->ConnectTo(&(pIDP[0]->fc_channel[2]));
	pBus->ConnectTo(&(pIDP[1]->fc_channel[2]));
	pBus->ConnectTo(&(pIDP[2]->fc_channel[2]));
	pBus->ConnectTo(&(pIDP[3]->fc_channel[2]));


	pBus = BusManager()->GetBus("FC4");
	pBus->ConnectTo(&(pGPC[0]->channel[22]));
	pBus->ConnectTo(&(pGPC[1]->channel[22]));
	pBus->ConnectTo(&(pGPC[2]->channel[22]));
	pBus->ConnectTo(&(pGPC[3]->channel[22]));
	pBus->ConnectTo(&(pGPC[4]->channel[22]));
	pBus->ConnectTo(&(pIDP[0]->fc_channel[3]));
	pBus->ConnectTo(&(pIDP[1]->fc_channel[3]));
	pBus->ConnectTo(&(pIDP[2]->fc_channel[3]));
	pBus->ConnectTo(&(pIDP[3]->fc_channel[3]));


	pBus = BusManager()->GetBus("FC5");
	pBus->ConnectTo(&(pGPC[0]->channel[13]));
	pBus->ConnectTo(&(pGPC[1]->channel[13]));
	pBus->ConnectTo(&(pGPC[2]->channel[13]));
	pBus->ConnectTo(&(pGPC[3]->channel[13]));
	pBus->ConnectTo(&(pGPC[4]->channel[13]));
	pBus->ConnectTo(&(pEIU[0]->mia[0]));
	pBus->ConnectTo(&(pEIU[1]->mia[0]));
	pBus->ConnectTo(&(pEIU[2]->mia[0]));


	pBus = BusManager()->GetBus("FC6");
	pBus->ConnectTo(&(pGPC[0]->channel[14]));
	pBus->ConnectTo(&(pGPC[1]->channel[14]));
	pBus->ConnectTo(&(pGPC[2]->channel[14]));
	pBus->ConnectTo(&(pGPC[3]->channel[14]));
	pBus->ConnectTo(&(pGPC[4]->channel[14]));
	pBus->ConnectTo(&(pEIU[0]->mia[1]));
	pBus->ConnectTo(&(pEIU[1]->mia[1]));
	pBus->ConnectTo(&(pEIU[2]->mia[1]));


	pBus = BusManager()->GetBus("FC7");
	pBus->ConnectTo(&(pGPC[0]->channel[15]));
	pBus->ConnectTo(&(pGPC[1]->channel[15]));
	pBus->ConnectTo(&(pGPC[2]->channel[15]));
	pBus->ConnectTo(&(pGPC[3]->channel[15]));
	pBus->ConnectTo(&(pGPC[4]->channel[15]));
	pBus->ConnectTo(&(pEIU[0]->mia[2]));
	pBus->ConnectTo(&(pEIU[1]->mia[2]));
	pBus->ConnectTo(&(pEIU[2]->mia[2]));


	pBus = BusManager()->GetBus("FC8");
	pBus->ConnectTo(&(pGPC[0]->channel[16]));
	pBus->ConnectTo(&(pGPC[1]->channel[16]));
	pBus->ConnectTo(&(pGPC[2]->channel[16]));
	pBus->ConnectTo(&(pGPC[3]->channel[16]));
	pBus->ConnectTo(&(pGPC[4]->channel[16]));
	pBus->ConnectTo(&(pEIU[0]->mia[3]));
	pBus->ConnectTo(&(pEIU[1]->mia[3]));
	pBus->ConnectTo(&(pEIU[2]->mia[3]));


	pBus = BusManager()->GetBus("PL1");
	pBus->ConnectTo(&(pGPC[0]->channel[9]));
	pBus->ConnectTo(&(pGPC[1]->channel[9]));
	pBus->ConnectTo(&(pGPC[2]->channel[9]));
	pBus->ConnectTo(&(pGPC[3]->channel[9]));
	pBus->ConnectTo(&(pGPC[4]->channel[9]));

	pBus = BusManager()->GetBus("PL2");
	pBus->ConnectTo(&(pGPC[0]->channel[10]));
	pBus->ConnectTo(&(pGPC[1]->channel[10]));
	pBus->ConnectTo(&(pGPC[2]->channel[10]));
	pBus->ConnectTo(&(pGPC[3]->channel[10]));
	pBus->ConnectTo(&(pGPC[4]->channel[10]));

	pBus = BusManager()->GetBus("LB1");
	pBus->ConnectTo(&(pGPC[0]->channel[11]));
	pBus->ConnectTo(&(pGPC[1]->channel[11]));
	pBus->ConnectTo(&(pGPC[2]->channel[11]));
	pBus->ConnectTo(&(pGPC[3]->channel[11]));
	pBus->ConnectTo(&(pGPC[4]->channel[11]));

	pBus = BusManager()->GetBus("LB2");
	pBus->ConnectTo(&(pGPC[0]->channel[12]));
	pBus->ConnectTo(&(pGPC[1]->channel[12]));
	pBus->ConnectTo(&(pGPC[2]->channel[12]));
	pBus->ConnectTo(&(pGPC[3]->channel[12]));
	pBus->ConnectTo(&(pGPC[4]->channel[12]));

	discsignals::DiscreteBundle* pBundle =
		BundleManager()->CreateBundle("FMC1_STARTRACKER", 16);

	//pSTYDoorMotor->CmdDriveFwd.Connect(pBundle, 0);
	//pSTYDoorMotor->CmdDriveRwd.Connect(pBundle, 1);
	pSTYDoorMotor->PosLimit0.Connect(pBundle, 2);
	pSTYDoorMotor->PosLimit1.Connect(pBundle, 3);

	//pSTZDoorMotor->CmdDriveFwd.Connect(pBundle, 4);
	//pSTZDoorMotor->CmdDriveRwd.Connect(pBundle, 5);
	pSTZDoorMotor->PosLimit0.Connect(pBundle, 6);
	pSTZDoorMotor->PosLimit1.Connect(pBundle, 7);

	eps::ACBus* pACBus = pACBusSystem->GetBus("AC1");
	pACBus->ConnectToSource(pInverter[0]);
	//ST Doors System 1
	pFMC1->module[0].mc.ConnectToSource(pACBus);
	pFMC1->module[1].mc.ConnectToSource(pACBus);
	pFMC3->module[0].mc.ConnectToSource(pACBus);

	pACBus = pACBusSystem->GetBus("AC2");
	pACBus->ConnectToSource(pInverter[1]);

	pACBus = pACBusSystem->GetBus("AC3");
	pACBus->ConnectToSource(pInverter[2]);

}

void Atlantis::SynchronizeCountdown(double launch_mjd)
{
	pRSLS->SychronizeCountdown(launch_mjd);
}

void Atlantis::StartRSLSSequence()
{
	pRSLS->StartRSLSSequence();
}

bool Atlantis::GetRSLSAbortFlag() const
{
	return pRSLS->GetRSLSAbortFlag();
}

void Atlantis::PSN4(void)
{
	pEIU[0]->command(0xBC00);
	pEIU[1]->command(0xBC00);
	pEIU[2]->command(0xBC00);
}

void Atlantis::SetSSMEActPos(int num, double Ppos, double Ypos)
{
	pATVC_SOP->SetSSMEActPos(num, Ppos, Ypos);
	return;
}

int Atlantis::GetSSMEPress(int eng)
{
	return Round(pSSME_SOP->GetPercentChamberPressVal(eng));
}

int Atlantis::GetHeTankPress(int sys) const
{
	assert((sys >= 0) && (sys <= 3) && "Atlantis::GetHeTankPress");
	if (sys == 0) return pHePneu->GetTankPress();
	return pHeEng[sys - 1]->GetTankPress();
}

int Atlantis::GetHeRegPress(int sys) const
{
	assert((sys >= 0) && (sys <= 3) && "Atlantis::GetHeRegPress");
	if (sys == 0) return pHePneu->GetRegPress();
	return pHeEng[sys - 1]->GetRegPress();
}

void Atlantis::HeFillTank(int sys, double mass)
{
	assert((sys >= 0) && (sys <= 3) && "Atlantis::HeFillTank");
	if (sys == 0) pHePneu->FillTank(mass);
	else pHeEng[sys - 1]->FillTank(mass);
}

PROPELLANT_HANDLE Atlantis::GetLH2Tank(void) const
{
	return phLH2dump;
}

PROPELLANT_HANDLE Atlantis::GetLOXTank(void) const
{
	return phLOXdump;
}

double Atlantis::GetLOXManifPress(void) const
{
	return pMPS->GetLOXManifPress();
}

double Atlantis::GetLH2ManifPress(void) const
{
	return pMPS->GetLH2ManifPress();
}

void Atlantis::UpdateOrbiterTexture(const std::string& strTextureName) {
	if (!hDevOrbiterMesh) return; // no mesh handle
	if (strTextureName.length() == 0) return; // no texture specified
	SURFHANDLE hTexture = oapiLoadTexture(strTextureName.c_str());
	oapiSetTexture(hDevOrbiterMesh, 1, hTexture);
}

int Atlantis::GetSoundID() const {
	return SoundID;
}

bool Atlantis::AreMCADebugMessagesEnabled() const throw()
{
	return bEnableMCADebug;
}

ANIMATIONCOMPONENT_HANDLE Atlantis::AddManagedAnimationComponent(UINT anim, double state0, double state1,
	MGROUP_TRANSFORM *trans, ANIMATIONCOMPONENT_HANDLE parent)
{
	vpAnimations.push_back(trans);
	return AddAnimationComponent(anim, state0, state1, trans, parent);
}

void Atlantis::OMSEngControl(unsigned short usEng)
{
	if (GetPropellantMass(oms_helium_tank[usEng]) > 0.0 && (OMSArm[usEng] || OMSArmPress[usEng])) {
		SetThrusterResource(th_oms[usEng], ph_oms);
	}
	else if (GetPropellantMass(oms_helium_tank[usEng]) <= 0.0 || (!OMSArm[usEng] && !OMSArmPress[usEng])) {
		SetThrusterResource(th_oms[usEng], NULL);
	}

	if (OMSFire[usEng]) {
		SetThrusterLevel(th_oms[usEng], 1.0);
	}
	else {
		SetThrusterLevel(th_oms[usEng], 0.0);
	}

	double pitch = OMSPitch[usEng].GetVoltage()*OMS_PITCH_RANGE;
	double yaw = OMSYaw[usEng].GetVoltage()*OMS_YAW_RANGE;
	if (!Eq(pitch, curOMSPitch[usEng], 0.01) || !Eq(yaw, curOMSYaw[usEng], 0.01)) {
		GimbalOMS(usEng, pitch, yaw);
	}
}

bool Atlantis::AttachChildAndUpdateMass(OBJHANDLE child, ATTACHMENTHANDLE attachment, ATTACHMENTHANDLE child_attachment)
{
	bool result = AttachChild(child, attachment, child_attachment);
	if (result) {
		UpdateMassAndCoG(true);
	}
	return result;
}

bool Atlantis::DetachChildAndUpdateMass(ATTACHMENTHANDLE attachment, double vel)
{
	OBJHANDLE hChild = GetAttachmentStatus(attachment);
	bool result = DetachChild(attachment, vel);
	if (result && hChild) {
		UpdateMassAndCoG(true);
	}
	return result;
}

double Atlantis::GetMassAndCoGOfAttachedObject(ATTACHMENTHANDLE ah, VECTOR3& CoG) const
{
	double mass = 0.0;
	CoG = _V(0, 0, 0);
	OBJHANDLE hV = GetAttachmentStatus(ah);
	if (hV) {
		VESSEL* v = oapiGetVesselInterface(hV);
		if (v) {
			VECTOR3 childCoG;
			mass = GetMassAndCoG(v, childCoG);
			// get CoG of payload in Orbiter frame
			VECTOR3 rpos;
			v->GetRelativePos(GetHandle(), rpos);
			MATRIX3 LocalToGlob, ChildToGlob;
			GetRotationMatrix(LocalToGlob);
			v->GetRotationMatrix(ChildToGlob);
			CoG = tmul(LocalToGlob, mul(ChildToGlob, childCoG) + rpos) + currentCoG;
		}
	}

	return mass;
}

void Atlantis::ETPressurization(double GOXmass, double GH2mass)
{
	Atlantis_Tank* et = GetTankInterface();

	if (et != NULL) et->PressurantFlow(GOXmass, GH2mass);
	return;
}

void Atlantis::UpdateMPSManifold(void)
{
	Atlantis_Tank* et = GetTankInterface();

	if (et == NULL) return;

	double LH2deltamass = (LOXmass + LH2mass - GetPropellantMass(ph_mps)) / 7.032;
	double LOXdeltamass = LH2deltamass * 6.032;

	LOXmass -= LOXdeltamass;
	LH2mass -= LH2deltamass;

	et->PropellantFlow(LOXdeltamass, LH2deltamass);

	LOXmass += LOXdeltamass;
	if (LOXmass < 1) LOXmass = 0;// so it doesn't last forever
	LH2mass += LH2deltamass;
	if (LH2mass < 1) LH2mass = 0;// so it doesn't last forever

	SetPropellantMass(ph_mps, LOXmass + LH2mass);

	// HACK no clue... using 65-80% LOX mass
	double lvl = 100 * LOXmass / MPS_MANIFOLD_MASS_LOX;
	LO2LowLevelSensor[0].SetValue(lvl);
	LO2LowLevelSensor[1].SetValue(lvl);
	LO2LowLevelSensor[2].SetValue(lvl);
	LO2LowLevelSensor[3].SetValue(lvl);
	return;
}

void Atlantis::UpdateMassAndCoG(bool bUpdateAttachedVessels)
{
	// for the moment, only look at shuttle, ET and SRBs
	// ignore payloads and shuttle consumables
	// TODO: include subsystem CoG in calculations (at the moment, subsystem mass is assumed to be located at ORBITER_CG)
	std::vector<double> masses;
	std::vector<VECTOR3> positions;

	//if (bUpdateAttachedVessels) {
		payloadMass = 0.0;
		payloadCoG = _V(0, 0, 0);

		DWORD attachmentCount = AttachmentCount(false);
		for (DWORD i = 0; i < attachmentCount; i++) {
			ATTACHMENTHANDLE ah = GetAttachmentHandle(false, i);
			if (ah != ahET && ah != ahLeftSRB && ah != ahRightSRB) {
				if (GetAttachmentStatus(ah)) {
					double mass;
					VECTOR3 CoG;
					mass = GetMassAndCoGOfAttachedObject(ah, CoG);
					payloadMass += mass;
					payloadCoG += CoG*mass;

					char cbuf[255];
					sprintf_s(cbuf, 255, "Payload: %s CoG: %f %f %f", oapiGetVesselInterface(GetAttachmentStatus(ah))->GetName(), CoG.x, CoG.y, CoG.z);
					oapiWriteLog(cbuf);
				}
			}
		}
		if ((payloadMass > 0.1) && (bUpdateAttachedVessels)) payloadCoG = payloadCoG / payloadMass;
		else payloadCoG = _V(0, 0, 0);

		SetEmptyMass(pMission->GetOrbiterMass() + psubsystems->GetTotalSubsystemMass() + pl_mass + payloadMass);
	//}

	if (status <= STATE_STAGE2) {
		double stackMass = 0.0; // mass of ET & SRBs (if attached)
		if (status <= STATE_STAGE1) {
			OBJHANDLE hLeftSRB = GetAttachmentStatus(ahLeftSRB);
			if (hLeftSRB) stackMass += oapiGetMass(hLeftSRB);
			OBJHANDLE hRightSRB = GetAttachmentStatus(ahRightSRB);
			if (hRightSRB) stackMass += oapiGetMass(hRightSRB);
		}
		OBJHANDLE hET = GetAttachmentStatus(ahET);
		if (hET) stackMass += oapiGetMass(hET);

		double subsystemMass = psubsystems->GetTotalSubsystemMass();
		SetEmptyMass(pMission->GetOrbiterMass() + subsystemMass + pl_mass + payloadMass + stackMass);
	}

	double shuttleMass = GetMass(); // as we add masses, subtract them from this parameter
	shuttleMass -= payloadMass;
	masses.push_back(payloadMass);
	positions.push_back(payloadCoG);
	if (status <= STATE_STAGE2) { // add ET mass
		// density in kg/m^3 (calculated from ET tank mass/volume values in SCOM)
		const double LOX_DENSITY = 1138.43342579;
		const double LH2_DENSITY = 70.8014107928;
		const double TANK_RADIUS = 4.2;

		VESSEL* pTank = GetTankInterface();
		if (pTank) {
			double ETMass = pTank->GetEmptyMass();
			shuttleMass -= ETMass;
			masses.push_back(ETMass);
			positions.push_back(ET_EMPTY_CG);

			// approximate propellant tanks as cylinders where position of bottom of cylinder is known
			double prop = GetETPropellant_B();
			double LOXMass = LOX_MAX_PROPELLANT_MASS*(prop / 100.0);
			double LH2Mass = LH2_MAX_PROPELLANT_MASS*(prop / 100.0);
			double LOXHeight = LOXMass / (LOX_DENSITY*PI*TANK_RADIUS*TANK_RADIUS); // height of LOX in cylindrical tank
			double LH2Height = LH2Mass / (LH2_DENSITY*PI*TANK_RADIUS*TANK_RADIUS); // height of LH2 in cylindrical tank
			shuttleMass -= LOXMass;
			shuttleMass -= LH2Mass;
			masses.push_back(LOXMass);
			positions.push_back(ET_LOX_BASE + _V(0.0, 0.0, LOXHeight / 2));
			masses.push_back(LH2Mass);
			positions.push_back(ET_LH2_BASE + _V(0.0, 0.0, LH2Height / 2));
		}
	}
	if (status <= STATE_STAGE1) { // add SRB mass (assume SRB CG doesn't change and SRBs are symmetric)
		VESSEL* pLSRB = GetSRBInterface(LEFT);
		if (pLSRB) {
			double SRBMass = pLSRB->GetEmptyMass() + GetPropellantMass(ph_srb) / 2.0;
			shuttleMass -= 2.0*SRBMass;
			masses.push_back(SRBMass);
			positions.push_back(LSRB_CG);
			masses.push_back(SRBMass);
			positions.push_back(RSRB_CG);
		}
	}
	masses.push_back(shuttleMass);
	positions.push_back(ORBITER_CG);

	// calculate center of gravity (relative to center of Orbiter mesh)
	double totalMass = 0.0;
	VECTOR3 CoG = _V(0.0, 0.0, 0.0);
	for (unsigned int i = 0; i < masses.size(); i++) {
		totalMass += masses[i];
		CoG += positions[i] * masses[i];
	}
	CoG = CoG / totalMass;
	if (length(CoG - currentCoG) > 0.1) { // to avoid rounding errors during launch, only shift CG when magnitude of change is large enough
		VECTOR3 CoGShift = CoG - currentCoG;
		ShiftCG(CoGShift);
		currentCoG = CoG;
		orbiter_ofs = -currentCoG;
		//sprintf_s(oapiDebugString(), 255, "New CoG: %f %f %f", CoG.x, CoG.y, CoG.z);
		//oapiWriteLog(oapiDebugString());

		DefineTouchdownPoints();

		CreateAttControls_RCS(orbiter_ofs); // update RCS thruster positions

		// update PLBD light positions
		for (int i = 0; i < 6; i++) PLBLightPosition[i] -= CoGShift;
		DockingLightPos -= CoGShift;
		FwdBulkheadLightPos -= CoGShift;

		if (hStackAirfoil) EditAirfoil(hStackAirfoil, 1, CoGShift, NULL, 0.0, 0.0, 0.0);

		if (status <= STATE_STAGE2) UpdateNullDirections();
	}
}

void Atlantis::Twang(double timeToLaunch) const
{
	double twangParam = (1.0 - timeToLaunch / 5.0);
	if (twangParam < 0 || twangParam>1) return;
	double twangAngle = (1 - cos(twangParam * 2 * PI))*2.0 / 184.0; //Approximately 2 feet of twang on top of a 184 foot stack
	double c = cos(twangAngle);
	double s = sin(twangAngle);
	SetAttachmentParams(ahHDP, POS_HDP - currentCoG, _V(0, -s, -c), _V(0.0, c, -s));
}

void Atlantis::CopyThrusterSettings(THRUSTER_HANDLE th, const VESSEL* v, THRUSTER_HANDLE th_ref)
{
	SetThrusterMax0(th, v->GetThrusterMax0(th_ref));
	SetThrusterIsp(th, v->GetThrusterIsp0(th_ref), v->GetThrusterIsp(th_ref, 101.4e3), 101.4e3);
}

void Atlantis::loadMDMConfiguration()
{
	oapiWriteLog("(SpaceShuttleUltra) [INFO] Loading MDM configuration");
	for (int i = 0; i < 4; i++)
	{
		pFF[i]->LoadMDM("FF.mdm");
		pFA[i]->LoadMDM("FA.mdm");
		pOF[i]->LoadMDM("OF.mdm");
	}

	for (int i = 0; i < 3; i++)
	{
		pOA[i]->LoadMDM("OF.mdm");
	}

	for (int i = 0; i < 2; i++)
	{
		pPL[i]->LoadMDM("PL.mdm");
		pLL[i]->LoadMDM("LL.mdm");
		pLR[i]->LoadMDM("LR.mdm");
	}

	pLF1->LoadMDM("LF1.mdm");
	pLM1->LoadMDM("LM1.mdm");
	pLA1->LoadMDM("LA1.mdm");

	oapiWriteLog("(SpaceShuttleUltra) [INFO] Finished MDM configuration");
}
