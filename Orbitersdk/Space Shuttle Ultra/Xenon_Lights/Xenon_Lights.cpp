#define ORBITER_MODULE

#include "Xenon_Lights.h"

XenonLights::XenonLights(OBJHANDLE hVessel, int fmodel)
: VESSEL3(hVessel, fmodel), bLightsOn(false), bFoundTarget(false)
{
	phLights = NULL;
	thLights[0] = thLights[1] = NULL;
	pLights[0] = pLights[1] = NULL;
}

XenonLights::~XenonLights()
{
}

void XenonLights::clbkSetClassCaps(FILEHANDLE cfg)
{
	AddMesh(XENON_TRAILER_MESH_NAME);
	//DefineAnimations();
	CreateLights();
}

void XenonLights::CreateLights()
{
	phLights = CreatePropellantResource(1.0, 1.0);

	VECTOR3 LIGHT_POS[2] = { _V(2.395, 1.526, 0.488), _V(-2.395, 1.526, 0.488)};

	const COLOUR4 LIGHT_DIFFUSE = {0.925, 1, 0.925, 1};
	const COLOUR4 LIGHT_SPECULAR = {0.925, 1, 0.925, 1};
	const COLOUR4 LIGHT_AMBIENT = {0, 0, 0, 0};
	const double LIGHT_RANGE = 1000.0;
	const double LIGHT_ATT0 = 1e-3;
	const double LIGHT_ATT1 = 0;
	const double LIGHT_ATT2 = 5e-5;

	for(int i=0;i<2;i++) {
		pLights[i] = AddSpotLight(LIGHT_POS[i], _V(0, 0, 1),
			LIGHT_RANGE, LIGHT_ATT0, LIGHT_ATT1, LIGHT_ATT2, 3.0*RAD, 7.5*RAD,
			LIGHT_DIFFUSE, LIGHT_SPECULAR, LIGHT_AMBIENT);

		// create fake thruster to simulate glare from lights
		thLights[i] = CreateThruster(LIGHT_POS[i], _V(0, 0, 1), 0.0, phLights, 1.0, 1.0);
		AddExhaust(thLights[i], 0.0, 0.5);
	}
}

void XenonLights::clbkPostCreation()
{
	const double MAX_DIST = 1000.0; // max distance from light to shuttle
	const double HEIGHT_OFFSET = 75.0;

	DWORD vesselCount = oapiGetVesselCount();
	for(int i=0;i<vesselCount;i++) {
		OBJHANDLE hV = oapiGetVesselByIndex(i);
		VESSEL* v = oapiGetVesselInterface(hV);
		if(!_stricmp(v->GetClassName(), PAD_CLASS_NAME)) {
			VECTOR3 relPos;
			oapiGetRelativePos(hV, GetHandle(), &relPos);
			if(length(relPos) < MAX_DIST) {
				bFoundTarget = true;

				MATRIX3 RotMatrix;
				GetRotationMatrix(RotMatrix);
				VECTOR3 pos = tmul(RotMatrix, relPos);
				pos.y+=HEIGHT_OFFSET;
				lightDir=pos/length(pos);
				break;
			}
		}
	}

	for(int i=0;i<2;i++) {
		pLights[i]->SetDirection(lightDir);
	}
	SetLightState(true);
}

void XenonLights::SetLightState(bool on) const
{
	for(int i=0;i<2;i++) {
		if(on) SetThrusterLevel(thLights[i], 1.0);
		else SetThrusterLevel(thLights[i], 0.0);
		pLights[i]->Activate(on);
	}
}

//global functions
DLLCLBK void InitModule(HINSTANCE hDLL)
{
}

DLLCLBK void ExitModule(HINSTANCE hDLL)
{
}

DLLCLBK VESSEL* ovcInit(OBJHANDLE hVessel, int iFlightModel)
{
	return new XenonLights(hVessel, iFlightModel);
}

DLLCLBK void ovcExit(VESSEL* pVessel)
{
	delete static_cast<XenonLights*>(pVessel);
}