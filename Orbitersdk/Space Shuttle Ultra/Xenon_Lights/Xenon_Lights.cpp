#define ORBITER_MODULE

#include "Xenon_Lights.h"

XenonLights::XenonLights(OBJHANDLE hVessel, int fmodel)
: VESSEL3(hVessel, fmodel), bLightsOn(false), 
bFoundTarget(false), hTarget(NULL),
updateClock(-0.1)
{
	phLights = NULL;
	thLights[0] = thLights[1] = NULL;
	pLights[0] = pLights[1] = NULL;

	lightDir = _V(0, 0, 1);
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
	const COLOUR4 LIGHT_SPECULAR = {0.425, 0.5, 0.425, 0.5};
	const COLOUR4 LIGHT_AMBIENT = {0, 0, 0, 0};
	const double LIGHT_RANGE = 1000.0;
	const double LIGHT_ATT0 = 1e-3;
	const double LIGHT_ATT1 = 0;
	const double LIGHT_ATT2 = 1e-5;

	for(int i=0;i<2;i++) {
		pLights[i] = AddSpotLight(LIGHT_POS[i], _V(0, 0, 1),
			LIGHT_RANGE, LIGHT_ATT0, LIGHT_ATT1, LIGHT_ATT2, 3.0*RAD, 5.0*RAD,
			LIGHT_DIFFUSE, LIGHT_SPECULAR, LIGHT_AMBIENT);

		// create fake thruster to simulate glare from lights
		thLights[i] = CreateThruster(LIGHT_POS[i], _V(0, 0, 1), 0.0, phLights, 1.0, 1.0);
		AddExhaust(thLights[i], 0.0, 0.5);
	}
}

void XenonLights::clbkPreStep(double simT, double simDT, double MJD)
{
	const double HEIGHT_OFFSET = 70.0;

	updateClock -= simDT;
	if(updateClock < 0.0) {
		updateClock = 60.0;

		// if target has not been found previously, try again
		if(bFoundTarget || FindTarget()) {
			// make sure target is still within range
			VECTOR3 relPos;
			GetRelativePos(hTarget, relPos);
			if(length(relPos) > MAX_TARGET_RANGE) {
				bFoundTarget = false;
				hTarget = NULL;
			}
			else {
				MATRIX3 RotMatrix;
				GetRotationMatrix(RotMatrix);
				VECTOR3 pos = tmul(RotMatrix, -relPos);
				pos.y += HEIGHT_OFFSET;
				lightDir=pos/length(pos);
			}
		}

		for(int i=0;i<2;i++) {
			pLights[i]->SetDirection(lightDir);
		}
		SetLightState(true);
	}
}

bool XenonLights::FindTarget()
{
	DWORD vesselCount = oapiGetVesselCount();
	for(int i=0;i<vesselCount;i++) {
		OBJHANDLE hV = oapiGetVesselByIndex(i);
		VESSEL* v = oapiGetVesselInterface(hV);
		if(!_stricmp(v->GetClassName(), SHUTTLE_CLASS_NAME)) {
			OBJHANDLE hParent = GetAttachedParent(hV);
			while(hParent) {
				hV = hParent;
				hParent = GetAttachedParent(hV);
			}

			VECTOR3 relPos;
			GetRelativePos(hV, relPos);
			if(length(relPos) < MAX_TARGET_RANGE) {
				bFoundTarget = true;
				hTarget = hV;
				return true;
			}
		}
	}
	return false;
}

OBJHANDLE XenonLights::GetAttachedParent(OBJHANDLE hVessel) const
{
	VESSEL* pVessel = oapiGetVesselInterface(hVessel);
	DWORD count = pVessel->AttachmentCount(true);
	for(DWORD i=0;i<count;i++) {
		ATTACHMENTHANDLE ah = pVessel->GetAttachmentHandle(true, i);
		OBJHANDLE hV = pVessel->GetAttachmentStatus(ah);
		if(hV) return hV;
	}
	return NULL;
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