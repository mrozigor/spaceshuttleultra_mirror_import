#ifndef XENONLIGHTS_H_F799652E_29DD_49ac_8E1E_53727281AABE
#define XENONLIGHTS_H_F799652E_29DD_49ac_8E1E_53727281AABE
#pragma once

#include "orbitersdk.h"

static const char* XENON_TRAILER_MESH_NAME = "SSU/KSC_xenon_lights_trailer";
static const char* SHUTTLE_CLASS_NAME = "SpaceShuttleUltra";
static const char* CRAWLER_CLASS_NAME = "SSU_CT";
static const char* CRAWLER_1980_CLASS_NAME = "SSU_CT_1980";

const double CRAWLER_HEIGHT_OFFSET = 50.0;
const double PAD_HEIGHT_OFFSET = 60.0;

const double MAX_TARGET_RANGE = 1000.0; // max distance from light to shuttle

class XenonLights : public VESSEL3
{
public:
	XenonLights(OBJHANDLE hVessel, int fmodel);
	virtual ~XenonLights();

	void clbkSetClassCaps(FILEHANDLE cfg);

	void clbkPreStep(double simT, double simDT, double MJD);
private:
	bool FindTarget();
	/**
	 * Gets parent vessel to which hVessel is attached
	 * @param hVessel vessel to find parent attachment
	 * @returns NULL if no parent, otherwise handle of parent vessel
	 */
	OBJHANDLE GetAttachedParent(OBJHANDLE hVessel) const;

	void CreateLights();
	void DefineAnimations();
	void SetLightState(bool on) const;

	UINT anim_pan[2], anim_tilt[2];
	VECTOR3 lightDir;

	PROPELLANT_HANDLE phLights;
	THRUSTER_HANDLE thLights[2];
	LightEmitter* pLights[2];

	bool bLightsOn;
	bool bFoundTarget;
	OBJHANDLE hTarget; // vessel lights are pointed at
	double heightOffset;

	double updateClock; // if <0, update position of target
};

#endif