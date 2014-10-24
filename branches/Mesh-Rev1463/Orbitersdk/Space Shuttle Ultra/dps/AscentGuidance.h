#ifndef ASCENTGUIDANCE_H_C2DAA79D_0FAA_4c19_BB7C_BEB3722AFC27
#define ASCENTGUIDANCE_H_C2DAA79D_0FAA_4c19_BB7C_BEB3722AFC27
#pragma once

#include "SimpleGPCSoftware.h"
#include <discsignals.h>
#include <PIDControl.h>
#include <EngConst.h>

namespace dps
{

const unsigned int STAGE1_GUIDANCE_TABLE_SIZE = 30;
const double DEFAULT_STAGE1_GUIDANCE_TABLE_VEL[STAGE1_GUIDANCE_TABLE_SIZE] = {0, 127/MPS2FPS, 341/MPS2FPS, 414/MPS2FPS, 493/MPS2FPS, 571/MPS2FPS, 678/MPS2FPS, 747/MPS2FPS, 846/MPS2FPS, 932/MPS2FPS, 1068/MPS2FPS, 1149/MPS2FPS, 1304/MPS2FPS, 1382/MPS2FPS, 1463/MPS2FPS, 1550/MPS2FPS, 1693/MPS2FPS, 1797/MPS2FPS, 2027/MPS2FPS, 2154/MPS2FPS, 2286/MPS2FPS, 2353/MPS2FPS, 2421/MPS2FPS, 2746/MPS2FPS, 3019/MPS2FPS, 3157/MPS2FPS, 3350/MPS2FPS, 3573/MPS2FPS, 3950/MPS2FPS, 5000/MPS2FPS};
const double DEFAULT_STAGE1_GUIDANCE_TABLE_PITCH[STAGE1_GUIDANCE_TABLE_SIZE] = {90.0, 70.5, 70.1, 70.0, 69.7, 69.4, 68.6, 67.8, 66.2, 64.4, 62.1, 61.1, 59.0, 58.2, 57.8, 57.1, 54.7, 52.9, 48.7, 46.4, 43.0, 42.0, 41.6, 38.6, 36.3, 35.3, 33.9, 32.4, 29.8, 22.6};

const double ROLL_TO_HEADS_UP_VELOCITY = 12200.0/MPS2FPS; // relative velocity at which RTHU begins (nominal/ATO)

const double ASCENT_MAJOR_CYCLE = 1.0000; // time (seconds) between guidance major cycles
const double PEG_STOP_TIME = 7.000; // time (seconds) before expected MECO to stop PEG calculations and hold constant attitude (in real life, PEG ends 40 s before MECO, but active attitude control continues)

// HACK not sure real values, both I-loads
const double ALIM1 = 29.3218835;// m/s^2 | 2.99 g
const double ALIM2 = 29.41995;// m/s^2 | 3.0 g

// AGT params
const double Vref_adjust = 368;// fps
const double Tref_adjust = 17.87;// MET (sec)

// VRel/SSME throttle table
const double THROT1 = 104;
const double THROT2 = 104;
const double THROT3 = 72;
const double THROT4 = 104;

const double QPOLY1 = 18.288;
const double QPOLY2 = 116.7384;
const double QPOLY3 = 188.0616;
const double QPOLY4 = 425.8056;
const double QPOLY5 = 99999;// fake

const double FINECOUNT_THROTTLE = 67;
const double FINECOUNT_THROTTLE_1EO = 91;
const double FINECOUNT_THROTTLE_2EO = 91;

const double SSME_TAILOFF_DV_67 = 90;// fps
const double SSME_TAILOFF_DV_91_1EO = 75;// fps
const double SSME_TAILOFF_DV_91_2EO = 35;// fps

// HACK the mass is just a guess, it's probably a little lower
const double LOWLEVEL_ARM_MASS = 370000;// lbs


class SSME_SOP;
class SSME_Operations;
class ATVC_SOP;
class SRBSepSequence;
class ETSepSequence;

/**
 * Controls shuttle during ascent (first and second stage).
 */
class AscentGuidance : public SimpleGPCSoftware
{
public:
	AscentGuidance(SimpleGPCSystem* _gpc);
	virtual ~AscentGuidance();

	virtual void Realize();

	virtual void OnPreStep(double SimT, double DeltaT, double MJD);

	virtual bool OnMajorModeChange(unsigned int newMajorMode);
	//virtual bool ItemInput(int spec, int item, const char* Data);
	//virtual bool OnPaint(int spec, vc::MDU* pMDU) const;

	virtual bool OnParseLine(const char* keyword, const char* value);
	virtual void OnSaveState(FILEHANDLE scn) const;

	void NullSRBNozzles( void );

	bool OnPaint( int spec, vc::MDU* pMDU ) const;

	/**
	 * Gets current state of throttle commanding.
	 * @return	true if AUTO, false if MAN
	 */
	bool GetAutoThrottleState( void ) const;

	/**
	 * Gets current vehicle attitude errors (deg).
	 * @return	attitude errors (deg) (x=pitch, y=yaw, z=roll)
	 */
	VECTOR3 GetAttitudeErrors( void ) const;
private:
	void InitializeAutopilot();

	/**
	 * Returns current heading in radians.
	 */
	double GetCurrentHeading() const;

	void GimbalSRBs(double DeltaT);
	void GimbalSSMEs(double DeltaT);

	/**
	 * Calculates target rates during first stage
	 * This phase uses fixed pitch profile
	 */
	void FirstStageRateCommand();
	/**
	 * Calculates target rates during second stage.
	 * Uses PEG guidance.
	 */
	void SecondStageRateCommand();
	/**
	 * Sets SSME throttle value.
	 */
	void Throttle(double dt);

	/**
	 * Calculates heading required to reach target inclination.
	 * \returns heading in radians
	 */
	double CalculateAzimuth();
	void MajorCycle();
	void Navigate();
	void Estimate();
	void Guide();

	void AdaptiveGuidanceThrottling( void );

	// utility functions required by PEG guidance
	inline double b0(double TT) {
		return -Isp*log(1-TT/tau);
	}
	inline double bn(double TT, int n) {
		if(n==0) return b0(TT);
		return bn(TT,n-1)*tau-Isp*pow(TT,n)/n;
	}
	inline double c0(double TT) {
		return b0(TT)*TT-bn(TT,1);
	}
	inline double cn(double TT, int n) {
		if(n==0) return c0(TT);
		return cn(TT,n-1)*tau-Isp*pow(TT,n+1)/(n*(n+1));
	}
private:
	OBJHANDLE hEarth;
	double mu;
	double EarthRadius, SidDay;

	// guidance parameters
	double TgtInc, TgtFPA, TgtAlt, TgtSpd;
	double OMSAssistDuration;
	bool PerformRTHU;

	discsignals::DiscInPort SpdbkThrotPort;
	discsignals::DiscInPort SpdbkThrotAutoIn;
	discsignals::DiscOutPort SpdbkThrotAutoOut;
	discsignals::DiscOutPort SpdbkThrotPLT;
	// ports for commanding thrusters
	discsignals::DiscOutPort OMSCommand[2];
	discsignals::DiscOutPort SERC;

	double lastSBTCCommand;

	VECTOR3 degReqdRates;

	PIDControl SRBGimbal[2][3];

	// copied from Atlantis.h
	double radTargetHeading, TAp, TPe, TTrA, TEcc, TgtRad;
	std::vector<double> stage1GuidanceVelTable, stage1GuidancePitchTable;
	bool dogleg;

	double MaxThrust; // maximum thrust that can be commanded; usually 104.5
	//bool bAutopilot, bThrottle;
	double tSRBSep; //time(MET)
	double tLastMajorCycle;

	double target_pitch; // target second stage pitch in degrees
	double CmdPDot; // commanded second stage pitch rate in deg/sec
	VECTOR3 rh0;
	double radius; // distance from centre of Earth (r)
	double relativeVelocity; // velocity relative to Earth (v)
	double r,h,theta,omega,phi;
	VECTOR3 rh,thetah,hh;
	VECTOR3 posMoon,velMoon,rmh;
	double vr,vtheta,vh;

	//double fh;
	//double pitch,yaw,roll;

	double g;
	double thrustAcceleration; // a0
	double Isp, tau, ThrAngleP, ThrAngleY;
	double timeRemaining; // timeRemaining - T
	double deltatheta,thetaT;
	double fr,fdotr;
	double d1,d2,d3,d4;
	double A,C;
	double eCurrent;

	SSME_SOP* pSSME_SOP;
	SSME_Operations* pSSME_Operations;
	ATVC_SOP* pATVC_SOP;
	SRBSepSequence* pSRBSepSequence;
	ETSepSequence* pETSepSequence;
	double throttlecmd;// SSME commaded throttle
	bool glimiting;// g limiting in progress
	double dt_thrt_glim;// timer for g limiting throttle cmds

	bool enaSERC;
	bool MEFail[3];
	int NSSME;
	bool finecount;
	double finecountthrottle[3];

	double QPOLY[5];// SSME throttle velocity
	double THROT[4];// SSME throttle command
	int J;

	bool AGT_done;

	bool bNullSRBNozzles;

	double EOVI[2];

	double SSMETailoffDV[3];
};
	
};

#endif // ASCENTGUIDANCE_H_C2DAA79D_0FAA_4c19_BB7C_BEB3722AFC27
