#include "BaseSSUPad.h"
#include "UltraMath.h"

BaseSSUPad::BaseSSUPad(OBJHANDLE hVessel, int flightmodel)
: VESSEL3(hVessel, flightmodel)
{
	bLightsOn = false;

	fNextLightUpdate = -20.0;
}

BaseSSUPad::~BaseSSUPad()
{
	for(unsigned int i=0;i<vpAnimations.size();i++) delete vpAnimations.at(i);
}

void BaseSSUPad::CreateLights(VECTOR3* positions, unsigned int count)
{	
	static VECTOR3& light_color = _V(1, 1, 1);

	lights.resize(count); // set size of vector
	for(unsigned int i = 0; i<count; i++) {
		lights[i].duration = 0.5;
		lights[i].period = 2;
		lights[i].pos = &positions[i];
		lights[i].col = &light_color;
		lights[i].size = 1;
		lights[i].shape = BEACONSHAPE_STAR;
		lights[i].falloff = 0.4;
		lights[i].active = false;

		AddBeacon(&lights[i]);
	}
	ToggleLights(bLightsOn); // make sure lights initially have correct state
}

void BaseSSUPad::CreateStadiumLights(const VECTOR3* positions, const VECTOR3* dir, unsigned int count)
{
	const COLOUR4 STADIUM_LIGHT_DIFFUSE = {0.95f, 1.0f, 0.95f, 1.0f};//{0.95f, 1.0f, 0.95f, 1.0f};
	const COLOUR4 STADIUM_LIGHT_SPECULAR = {0,0,0,0};
	const COLOUR4 STADIUM_LIGHT_AMBIENT = {0.1f, 0.125f, 0.1f, 0.0f};
	const double STADIUM_LIGHT_RANGE = 100.0;
	const double STADIUM_LIGHT_ATT0 = 1e-3;
	const double STADIUM_LIGHT_ATT1 = 0;
	const double STADIUM_LIGHT_ATT2 = 1e-3;
	static VECTOR3& light_color = _V(1, 1, 1);

	phStadiumLights = CreatePropellantResource(1.0, 1.0);

	// set size of vectors
	//stadium_lights.resize(count);
	thStadiumLights.resize(count);
	pStadiumLights.resize(count);

	for(unsigned int i=0;i<count;i++) {
		pStadiumLights[i] = AddSpotLight(positions[i], dir[i],
			STADIUM_LIGHT_RANGE, STADIUM_LIGHT_ATT0, STADIUM_LIGHT_ATT1, STADIUM_LIGHT_ATT2, 45*RAD, 180*RAD,
			STADIUM_LIGHT_DIFFUSE, STADIUM_LIGHT_SPECULAR, STADIUM_LIGHT_AMBIENT);
		
		// create fake thruster to simulate glare from lights
		thStadiumLights[i] = CreateThruster(positions[i], dir[i], 0.0, phStadiumLights,
			1.0, 1.0);
		AddExhaust(thStadiumLights[i], 0.0, 1.5);

		// add beacon to show where lights are (debugging)
		/*stadium_lights[i].duration = 0.5;
		stadium_lights[i].period = 2;
		stadium_lights[i].pos = &positions[i];
		stadium_lights[i].col = &light_color;
		stadium_lights[i].size = 1;
		stadium_lights[i].shape = BEACONSHAPE_STAR;
		stadium_lights[i].falloff = 0.4;
		stadium_lights[i].active = false;
		AddBeacon(&stadium_lights[i]);*/
	}
	ToggleLights(bLightsOn); // make sure lights initially have correct state
}

void BaseSSUPad::ToggleLights(bool enable)
{
	bLightsOn = enable;

	for(unsigned int i = 0; i<lights.size(); i++) {
		lights[i].active = enable;
	}

	for(unsigned int i=0;i<pStadiumLights.size();i++) {
		pStadiumLights[i]->Activate(enable);
		SetThrusterLevel(thStadiumLights[i], 1.0);
		//stadium_lights[i].active = enable;
	}
}

bool BaseSSUPad::IsNight() const
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
		if(angle>80.0)
			return true;
	}
	return false;
}

void BaseSSUPad::clbkPreStep(double simt, double simdt, double mjd)
{
	VESSEL3::clbkPreStep(simt, simdt, mjd);

	if(simt > fNextLightUpdate) {
		fNextLightUpdate = simt + 300.0;

		if(bLightsOn && !IsNight()) {
			ToggleLights(false);
		} 
		else if(!bLightsOn && IsNight()) {
			ToggleLights(true);
		}
	}

	if(AccessArmState.Moving()) {
		AccessArmState.Move(simdt*orbiter_access_arm_rate);
		SetAnimation(anim_AccessArm, AccessArmState.pos);
	}

	// only allow vent hood to move when vent arm is fully extended or stopped
	// only allow arm to move when hood is closed (retracted)
	if(VentHoodState.Moving() && (Eq(VentArmState.pos, 1.0, 0.001) || VentArmState.Static())) {
		VentHoodState.Move(simdt*vent_hood_rate);
		SetAnimation(anim_VentHood, VentHoodState.pos);
		UpdateGOXVents();
	}
	else if(VentArmState.Moving() && Eq(VentHoodState.pos, 0.0, 0.001)) {
		VentArmState.Move(simdt*vent_arm_rate);
		SetAnimation(anim_VentArm, VentArmState.pos);
		UpdateGOXVents();
	}

	//sprintf_s(oapiDebugString(), 255, "VentArm: %d %f VentHood: %d %f", VentArmState.action, VentArmState.pos, VentHoodState.action, VentHoodState.pos);

	if(GH2VentlineState.Moving()) {
		GH2VentlineState.Move(simdt*gh2_arm_rate);
		SetAnimation(anim_GH2Ventline, GH2VentlineState.pos);
	}

	if(IAAState.Moving()) {
		IAAState.Move(simdt*iaa_rate);
		SetAnimation(anim_IAA, IAAState.pos);
	}
}

int BaseSSUPad::clbkConsumeBufferedKey(DWORD key, bool down, char* keystate)
{
	if(!down) return 0; //only handle keydown events

	switch(key) {
		case OAPI_KEY_K:
			if(AccessArmState.Open() || AccessArmState.Opening()) RetractOrbiterAccessArm();
			else ExtendOrbiterAccessArm();
			return 1;
		case OAPI_KEY_G:
			if(VentHoodState.Closed()) ExtendGOXArmAndHood();
			else RetractGOXArmAndHood();
			return 1;
		case OAPI_KEY_V:
			DetachGH2Ventline();
			return 1;
		case OAPI_KEY_A:
			if(IAAState.Open()) RetractIAA();
			else DeployIAA();
			return 1;
	}
	return 0;
}

MGROUP_ROTATE* BaseSSUPad::DefineRotation(UINT mesh, UINT* grp, UINT ngrp, const VECTOR3& ref, const VECTOR3& axis, float angle)
{
	MGROUP_ROTATE* mgrp = new MGROUP_ROTATE(mesh, grp, ngrp, ref, axis, angle);
	vpAnimations.push_back(mgrp);
	return mgrp;
}

MGROUP_TRANSLATE* BaseSSUPad::DefineTranslation(UINT mesh, UINT* grp, UINT ngrp, const VECTOR3& shift)
{
	MGROUP_TRANSLATE* mgrp = new MGROUP_TRANSLATE(mesh, grp, ngrp, shift);
	vpAnimations.push_back(mgrp);
	return mgrp;
}

MGROUP_SCALE* BaseSSUPad::DefineScale(UINT mesh, UINT* grp, UINT ngrp, const VECTOR3& ref, const VECTOR3& scale)
{
	MGROUP_SCALE* mgrp = new MGROUP_SCALE(mesh, grp, ngrp, ref, scale);
	vpAnimations.push_back(mgrp);
	return mgrp;
}
void BaseSSUPad::SetOrbiterAccessArmRate(double rate)
{
	orbiter_access_arm_rate = rate;
}

void BaseSSUPad::SetGOXVentArmRate(double rate)
{
	vent_arm_rate = rate;
}

void BaseSSUPad::SetGOXVentHoodRate(double rate)
{
	vent_hood_rate = rate;
}

void BaseSSUPad::SetGH2VentlineRate(double rate)
{
	gh2_arm_rate = rate;
}

void BaseSSUPad::SetIntertankAccessArmRate(double rate)
{
	iaa_rate = rate;
}

void BaseSSUPad::ExtendOrbiterAccessArm()
{
	AccessArmState.action=AnimState::OPENING;
}

void BaseSSUPad::RetractOrbiterAccessArm()
{
	AccessArmState.action=AnimState::CLOSING;
}

void BaseSSUPad::HaltOrbiterAccessArm()
{
	AccessArmState.action=AnimState::STOPPED;
}

void BaseSSUPad::ExtendGOXArm()
{
	VentArmState.action=AnimState::OPENING;
}

void BaseSSUPad::RetractGOXArm()
{
	VentArmState.action=AnimState::CLOSING;
}

void BaseSSUPad::HaltGOXArm()
{
	VentArmState.action=AnimState::STOPPED;
}

void BaseSSUPad::AttachGH2Ventline()
{
	GH2VentlineState.action=AnimState::CLOSING;
}

void BaseSSUPad::DetachGH2Ventline()
{
	GH2VentlineState.action=AnimState::OPENING;
}

void BaseSSUPad::RaiseVentHood()
{
	VentHoodState.action=AnimState::CLOSING;
}

void BaseSSUPad::LowerVentHood()
{
	VentHoodState.action=AnimState::OPENING;
}

void BaseSSUPad::HaltVentHood()
{
	VentHoodState.action=AnimState::STOPPED;
}

void BaseSSUPad::ExtendGOXArmAndHood()
{
	ExtendGOXArm();
	LowerVentHood();
}

void BaseSSUPad::RetractGOXArmAndHood()
{
	RetractGOXArm();
	RaiseVentHood();
}

void BaseSSUPad::HaltGOXArmAndHood()
{
	HaltGOXArm();
	HaltVentHood();
}

void BaseSSUPad::DeployIAA()
{
	IAAState.action=AnimState::OPENING;
}

void BaseSSUPad::HaltIAA()
{
	IAAState.action=AnimState::STOPPED;
}

void BaseSSUPad::RetractIAA()
{
	IAAState.action=AnimState::CLOSING;
}
