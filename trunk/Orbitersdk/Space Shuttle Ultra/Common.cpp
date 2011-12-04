// ==============================================================
//                 ORBITER MODULE: Atlantis
//                  Part of the ORBITER SDK
//          Copyright (C) 2001-2003 Martin Schweiger
//                   All rights reserved
//
// Common.cpp
// Utility functions common to multiple Atlantis-related modules
// ==============================================================

#include "Atlantis_SRB.h"

#ifdef _DEBUG
// D. Beachy: GROW THE STACK HERE SO WE CAN USE BOUNDSCHECKER FOR DEBUGGING
// We need this is because BoundsChecker (for this object) grows the stack more than 1 full page (4K) at once
// and then touches data beyond the initial 4K, skipping over the guard page that Windows places below the stack to grow it automatically.
// Therefore we will grow the stack manually in one-page increments here.
// This is only necessary for BoundsChecker debugging.

int GrowStack()
{
    // NOTE: this requires that orbiter.exe has its 'Size of Stack Reserve' PE header parameter set to 4 MB
    int pageCount = 256;    // 256 4K pages = reserve 1 MB of stack
    DWORD dwStackDelta = 0; // total # of stack bytes used
    for (int i=0; i < pageCount; i++)
    {
        dwStackDelta += 4096;
        __asm
        {
            sub     esp, 4092;  // 1 page - 4 bytes
            push    0xFEEDFEED  // touch the page
        }
    }

    // now pop the stack we touched
    __asm
    {
        mov     eax, [dwStackDelta] // size in bytes
        add     esp, eax
    }

    return 0;
}

// invoke GrowStack early before the next lines are called (otherwise BoundsChecker will crash)
int growStack=GrowStack();
#endif

double SRB_Seq[SRB_nt]  = {-SRB_STABILISATION_TIME,-0.001,    0,    5,   10,   15,   20,   25,   30,   35,   40,   45,   50,   55,   60,   65,   70,   75,   80,   85,   90,   95,  100,  105,  110,  115,  120,  125,  130,  160};
//double SRB_Thrust[SRB_nt] = {                0.000, 0.000,0.955,0.972,0.990,0.998,1.000,0.915,0.856,0.810,0.769,0.735,0.705,0.722,0.740,0.757,0.764,0.765,0.720,0.680,0.647,0.629,0.588,0.514,0.441,0.220,0.044,0.024,0.010,0.000};
// thrust data from JSC-19041, REV.F, section 4.3 (SRB bible)
double SRB_Thrust[SRB_nt] = {                0.000, 0.000,0.939,0.970,0.985,0.985,1.000,0.955,0.894,0.833,0.788,0.758,0.727,0.727,0.742,0.764,0.779,0.782,0.773,0.727,0.682,0.652,0.606,0.545,0.485,0.242,0.076,0.030,0.010,0.000};
double SRB_Prop[SRB_nt]   = { 1,1,1.000,0.950,0.900,0.848,0.795,0.743,0.693,0.646,0.602,0.559,0.519,0.480,0.440,0.400,
                                0.357,0.314,0.270,0.228,0.188,0.150,0.114,0.080,0.049,0.020,0.007,0.002,0.001,0.000};


//PARTICLESTREAMSPEC srb_contrail = {
//	0, 12.0, 3, 150.0, 0.4, 8.0, 4, 3.0, PARTICLESTREAMSPEC::DIFFUSE,
//	PARTICLESTREAMSPEC::LVL_PSQRT, 0, 0.5,
//	PARTICLESTREAMSPEC::ATM_PLOG, 1e-6, 0.1
//};
PARTICLESTREAMSPEC srb_contrail = {//DaveS edit: SRB contrail PSTREAM to be more realistic
	0, 19, 20, 150.0, 0.4, 10, 10, 1.5, PARTICLESTREAMSPEC::DIFFUSE,
	PARTICLESTREAMSPEC::LVL_PSQRT, 0, 0.5,
	PARTICLESTREAMSPEC::ATM_PLOG, 1e-6, 0.1
};
PARTICLESTREAMSPEC srb_exhaust = {//DaveS edit: Modified SRB exhaust PSTREAM to be more realistic
	0, 4, 5000, 100.0, 0.0, 1, 30, 0.0, PARTICLESTREAMSPEC::EMISSIVE,
	PARTICLESTREAMSPEC::LVL_FLAT, 1, 1,
	PARTICLESTREAMSPEC::ATM_FLAT, 1, 1
};

PARTICLESTREAMSPEC srb_slag1 = {
	0, 0.1, SLAG1_RATE, 50.0, SLAG_SPREAD, 4, 0, 0.1, PARTICLESTREAMSPEC::EMISSIVE,
	PARTICLESTREAMSPEC::LVL_FLAT, 1, 1,
	PARTICLESTREAMSPEC::ATM_FLAT, 1, 1
};

PARTICLESTREAMSPEC srb_slag2 = {
	0, 0.05, SLAG2_RATE, 50.0, SLAG_SPREAD, 3, 0, 0.1, PARTICLESTREAMSPEC::EMISSIVE,
	PARTICLESTREAMSPEC::LVL_FLAT, 1, 1,
	PARTICLESTREAMSPEC::ATM_FLAT, 1, 1
};

PARTICLESTREAMSPEC srb_slag3 = {
	0, 0.01, SLAG3_RATE, 50.0, SLAG_SPREAD, 3, 0, 0.1, PARTICLESTREAMSPEC::EMISSIVE,
	PARTICLESTREAMSPEC::LVL_FLAT, 1, 1,
	PARTICLESTREAMSPEC::ATM_FLAT, 1, 1
};

PARTICLESTREAMSPEC bsm_exhaust = {//DaveS edit: Modified SRB exhaust PSTREAM to be more realistic
	0, 2.4, 5000, 45, 0.1, 0.2, 30, 3.0, PARTICLESTREAMSPEC::EMISSIVE,
	PARTICLESTREAMSPEC::LVL_FLAT, 1, 1,
	PARTICLESTREAMSPEC::ATM_FLAT, 1, 1
};

PARTICLESTREAMSPEC bsm_contrail = {
	0, 1.6, 40, 70.0, 0.1, 10, 8, 3.0, PARTICLESTREAMSPEC::DIFFUSE,
	PARTICLESTREAMSPEC::LVL_PSQRT, 0, 0.5,
	PARTICLESTREAMSPEC::ATM_PLOG, 1e-6, 0.1
};


PARTICLESTREAMSPEC bsm_residual = {
	0, 0.3, 20, 5.0, 0.3, 10, 1, 1.5, PARTICLESTREAMSPEC::DIFFUSE,
	PARTICLESTREAMSPEC::LVL_PSQRT, 0, 0.5,
	PARTICLESTREAMSPEC::ATM_PLOG, 1e-6, 0.1
};

// time-dependent calculation of SRB thrust and remaining propellant
void GetSRB_State (double met, double &thrust_level, double &prop_level)
{
	int i;
	for (i = SRB_nt-2; i >= 0; i--)
		if (met > SRB_Seq[i]) break;
    double SRB_ThrSCL=(SRB_Thrust[i+1]-SRB_Thrust[i])/(SRB_Seq[i+1]-SRB_Seq[i]);
    double SRB_PrpSCL=(SRB_Prop[i+1]-SRB_Prop[i])/(SRB_Seq[i+1]-SRB_Seq[i]);
	thrust_level = SRB_ThrSCL * (met-SRB_Seq[i]) + SRB_Thrust[i];
	if(met>0.0) thrust_level+=0.01*oapiRand() - 0.01*oapiRand();
	prop_level = SRB_PrpSCL * (met-SRB_Seq[i]) + SRB_Prop[i];
}

