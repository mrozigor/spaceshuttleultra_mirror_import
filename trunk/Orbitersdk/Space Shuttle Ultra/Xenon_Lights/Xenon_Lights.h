#ifndef XENONLIGHTS_H_F799652E_29DD_49ac_8E1E_53727281AABE
#define XENONLIGHTS_H_F799652E_29DD_49ac_8E1E_53727281AABE
#pragma once

#include "orbitersdk.h"

static const char* XENON_TRAILER_MESH_NAME = "SSU/KSC_xenon_lights_trailer";
static const char* PAD_CLASS_NAME = "SSU_Pad";

class XenonLights : public VESSEL3
{
public:
	XenonLights(OBJHANDLE hVessel, int fmodel);
	virtual ~XenonLights();

	void clbkSetClassCaps(FILEHANDLE cfg);
	void clbkPostCreation();
private:
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
};

#endif