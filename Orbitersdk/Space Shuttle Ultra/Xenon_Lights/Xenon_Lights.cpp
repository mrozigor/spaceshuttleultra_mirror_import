#define ORBITER_MODULE

#include "Xenon_Lights.h"
#include "meshres_xenon.h"
#include "../SSUMath.h"

XenonLights::XenonLights(OBJHANDLE hVessel, int fmodel)
: VESSEL3(hVessel, fmodel), bLightsOn(false),
bFoundTarget(false), hTarget(NULL),
updateClock(-0.1)
{
	phLights = NULL;
	thLights[0] = thLights[1] = NULL;
	pLights[0] = pLights[1] = NULL;

	//lightDir = _V(0, 0, 1);
	lightPosition[0] = LIGHT_POS[0];
	lightPosition[1] = LIGHT_POS[1];
}

XenonLights::~XenonLights()
{
	for(unsigned int i=0;i<vpAnimations.size();i++)
		delete vpAnimations[i];
}

void XenonLights::clbkSetClassCaps(FILEHANDLE cfg)
{
	meshIndex = AddMesh(XENON_TRAILER_MESH_NAME);
	DefineAnimations();
	CreateLights();
}

void XenonLights::DefineAnimations()
{
	static UINT LeftPanGrp[1] = {LINE02};
	MGROUP_ROTATE* pLeftPan = new MGROUP_ROTATE(meshIndex, LeftPanGrp, 1,
		_V(-2.397, 0.552, -0.049), _V(0, 1, 0), static_cast<float>(PAN_RANGE));
	vpAnimations.push_back(pLeftPan);
	anim_pan[0] = CreateAnimation(0.0);
	ANIMATIONCOMPONENT_HANDLE parent = AddAnimationComponent(anim_pan[0], 0.0, 1.0, pLeftPan);
	static UINT LeftTiltGrp[4] = {BOX08, BOX09, BOX10, CYLINDER04};
	MGROUP_ROTATE* pLeftTilt = new MGROUP_ROTATE(meshIndex, LeftTiltGrp, 4,
		_V(-2.397, 1.343, -0.049), _V(-1, 0, 0), static_cast<float>(TILT_RANGE));
	vpAnimations.push_back(pLeftTilt);
	anim_tilt[0] = CreateAnimation(0.0);
	parent = AddAnimationComponent(anim_tilt[0], 0.0, 1.0, pLeftTilt, parent);
	MGROUP_ROTATE* pLeftPos = new MGROUP_ROTATE(LOCALVERTEXLIST, MAKEGROUPARRAY(lightPosition), 1,
		LIGHT_POS[0], _V(0, 0, 1), 0.0f);
	vpAnimations.push_back(pLeftPos);
	UINT pos_anim = CreateAnimation(0.0);
	AddAnimationComponent(pos_anim, 0.0, 1.0, pLeftPos, parent);

	static UINT RightPanGrp[1] = {LINE01};
	MGROUP_ROTATE* pRightPan = new MGROUP_ROTATE(meshIndex, RightPanGrp, 1,
		_V(2.397, 0.552, -0.049), _V(0, 1, 0), static_cast<float>(PAN_RANGE));
	vpAnimations.push_back(pRightPan);
	anim_pan[1] = CreateAnimation(0.0);
	parent = AddAnimationComponent(anim_pan[1], 0.0, 1.0, pRightPan);
	static UINT RightTiltGrp[4] = {BOX02, BOX03, BOX04, CYLINDER02};
	MGROUP_ROTATE* pRightTilt = new MGROUP_ROTATE(meshIndex, RightTiltGrp, 4,
		_V(2.397, 1.343, -0.049), _V(-1, 0, 0), static_cast<float>(TILT_RANGE));
	vpAnimations.push_back(pRightTilt);
	anim_tilt[1] = CreateAnimation(0.0);
	parent = AddAnimationComponent(anim_tilt[1], 0.0, 1.0, pRightTilt, parent);
	MGROUP_ROTATE* pRightPos = new MGROUP_ROTATE(LOCALVERTEXLIST, MAKEGROUPARRAY(&lightPosition[1]), 1,
		LIGHT_POS[1], _V(0, 0, 1), 0.0f);
	vpAnimations.push_back(pRightPos);
	pos_anim = CreateAnimation(0.0);
	AddAnimationComponent(pos_anim, 0.0, 1.0, pRightPos, parent);
}

void XenonLights::CreateLights()
{
	phLights = CreatePropellantResource(1.0, 1.0);

	//VECTOR3 LIGHT_POS[2] = { _V(-2.395, 1.526, 0.488), _V(2.395, 1.526, 0.488)};

	const COLOUR4 LIGHT_DIFFUSE = {0.925, 1, 0.925, 1};
	const COLOUR4 LIGHT_SPECULAR = {0.425, 0.5, 0.425, 0.5};
	const COLOUR4 LIGHT_AMBIENT = {0, 0, 0, 0};
	const double LIGHT_RANGE = 1000.0;
	const double LIGHT_ATT0 = 1e-3;
	const double LIGHT_ATT1 = 0;
	const double LIGHT_ATT2 = 1e-5;

	for(int i=0;i<2;i++) {
		pLights[i] = AddSpotLight(LIGHT_POS[i], _V(0, 0, 1),
			LIGHT_RANGE, LIGHT_ATT0, LIGHT_ATT1, LIGHT_ATT2, 4.5*RAD, 8.0*RAD,
			LIGHT_DIFFUSE, LIGHT_SPECULAR, LIGHT_AMBIENT);

		// create fake thruster to simulate glare from lights
		thLights[i] = CreateThruster(LIGHT_POS[i], _V(0, 0, 1), 0.0, phLights, 1.0, 1.0);
		AddExhaust(thLights[i], 0.0, 0.5);
	}
}

void XenonLights::clbkPreStep(double simT, double simDT, double MJD)
{
	updateClock -= simDT;
	if(updateClock < 0.0) {
		updateClock = 60.0;

		VECTOR3 lightDir = _V(0, 0, 1);
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
				pos.y = heightOffset;
				lightDir=pos/length(pos);
			}
		}
		SetDirection(lightDir);

		bool day = IsDay();
		if(!day && !bLightsOn) SetLightState(true);
		else if(day && bLightsOn) SetLightState(false);
	}

	for(unsigned short i=0;i<2;i++)
		SetThrusterRef(thLights[i], lightPosition[i]);
}

bool XenonLights::IsDay() const
{	
	OBJHANDLE Sun=NULL;
	int count=(int)oapiGetGbodyCount();
	for(int i=0;i<count;i++) {
		Sun=oapiGetGbodyByIndex(i);
		if(oapiGetObjectType(Sun)==OBJTP_STAR) break;
	}
	if(Sun) {
		VECTOR3 SunPosGlobal, SunPos;
		oapiGetGlobalPos(Sun, &SunPosGlobal);
		Global2Local(SunPosGlobal, SunPos);
		double angle=acos(SunPos.y/length(SunPos))*DEG;
		if(angle>85.0)
			return false;
	}
	return true;
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

				// set height offset; this is different between rollout and on pad
				VESSEL* pTarget = oapiGetVesselInterface(hTarget);
				if(!_stricmp(pTarget->GetClassName(), CRAWLER_CLASS_NAME) ||
					!_stricmp(pTarget->GetClassName(), CRAWLER_1980_CLASS_NAME)) {
						heightOffset = CRAWLER_HEIGHT_OFFSET;
				}
				else {
					heightOffset = PAD_HEIGHT_OFFSET;
				}
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

void XenonLights::SetDirection(VECTOR3 dir)
{
	double pan = atan2(dir.x, dir.z);
	// make sure pan is from 0 to 2pi radians
	if(pan < 0.0) pan += 2.0*PI;
	else if (pan > 2.0*PI) pan -= 2.0*PI;

	double tilt = asin(dir.y);
	// make sure result is between 0 and 90 degrees
	tilt = range(0.0, tilt, TILT_RANGE);
	
	for(int i=0;i<2;i++) {
		SetAnimation(anim_pan[i], pan/PAN_RANGE);
		SetAnimation(anim_tilt[i], tilt/TILT_RANGE);

		pLights[i]->SetDirection(dir);
	}
}

void XenonLights::SetLightState(bool on)
{
	for(int i=0;i<2;i++) {
		if(on) SetThrusterLevel(thLights[i], 1.0);
		else SetThrusterLevel(thLights[i], 0.0);
		pLights[i]->Activate(on);
	}
	bLightsOn = on;
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