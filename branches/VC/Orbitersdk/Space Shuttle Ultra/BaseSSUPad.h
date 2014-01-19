#ifndef BASESSUPAD_H_32815393_9D69_4a58_978C_29F4D3C04278
#define BASESSUPAD_H_32815393_9D69_4a58_978C_29F4D3C04278
#pragma once

#include "orbitersdk.h"
#include "ISSULaunchTower.h"
#include <vector>

/**
 * Base class for SSU pads (SLC-6 and LC-39)
 * Controls lights and animations common to both pads (OAA, GVA/Hood, IAA and GH2 ventline)
 */
class BaseSSUPad : public VESSEL3, public ISSULaunchTower
{
	bool bLightsOn;
	double fNextLightUpdate;

	PROPELLANT_HANDLE phStadiumLights; // fake tank for lights
	std::vector<BEACONLIGHTSPEC> lights;
	//std::vector<BEACONLIGHTSPEC> stadium_lights; // TODO: debugging only
	std::vector<THRUSTER_HANDLE> thStadiumLights;
	std::vector<LightEmitter*> pStadiumLights;

	std::vector<MGROUP_TRANSFORM*> vpAnimations;

	double orbiter_access_arm_rate;
	double vent_hood_rate;
	double vent_arm_rate;
	double gh2_arm_rate;
	double iaa_rate;
	
	//bool bFirstStep;
protected:
	// 0.0, CLOSED corresponds to state at T0
	AnimState AccessArmState;
	AnimState VentHoodState, VentArmState;
	AnimState GH2VentlineState;
	AnimState IAAState;

	UINT anim_AccessArm;
	UINT anim_VentHood, anim_VentArm;
	UINT anim_GH2Ventline;
	UINT anim_IAA;
protected:
	BaseSSUPad(OBJHANDLE hVessel, int flightmodel);
	virtual ~BaseSSUPad();

	/**
	 * Creates lights (beacons only) at specified positions
	 * \param positions array of positions at which to create lights
	 * \param count number of lights (size of positions array)
	 */
	void CreateLights(VECTOR3* positions, unsigned int count);
	/**
	 * Creates stadium lights (beacons and spotlights) at specified positions
	 * \param positions array of positions at which to create lights
	 * \param count number of lights (size of positions array)
	 */
	void CreateStadiumLights(const VECTOR3* positions, const VECTOR3* dir, unsigned int count);
	void ToggleLights(bool enable);
	bool IsNight() const;
	
	void SetOrbiterAccessArmRate(double rate);
	void SetGOXVentArmRate(double rate);
	void SetGOXVentHoodRate(double rate);
	void SetGH2VentlineRate(double rate);
	void SetIntertankAccessArmRate(double rate);
	
	// ISSULaunchTower
	virtual void ExtendOrbiterAccessArm();
	virtual void RetractOrbiterAccessArm();
	virtual void HaltOrbiterAccessArm();

	virtual void ExtendGOXArm();
	virtual void RetractGOXArm();
	virtual void HaltGOXArm();

	virtual void AttachGH2Ventline();
	virtual void DetachGH2Ventline();

	virtual void RaiseVentHood();
	virtual void LowerVentHood();
	virtual void HaltVentHood();

	virtual void ExtendGOXArmAndHood();
	virtual void RetractGOXArmAndHood();
	virtual void HaltGOXArmAndHood();

	virtual void DeployIAA();
	virtual void HaltIAA();
	virtual void RetractIAA();
	
	/**
	 * Abstract function to update position/direction of GOX vent thrusters
	 */
	virtual void UpdateGOXVents() = 0;

	/**
	 * Performs actions common to both SLC-6 and LC-39
	 * Updates animations defined in ISSULaunchTower (OAA, GOX Vent Arm, GH2 vent line, Intertank Access Arm)
	 * Toggles GOX venting as appropriate
	 */
	virtual void clbkPreStep(double simt, double simdt, double mjd);
	virtual int clbkConsumeBufferedKey(DWORD key, bool down, char* keystate);

	/**
	 * Creates MGROUP_ROTATE struct, adds it to animation list, and returns pointer to struct
	 */
	MGROUP_ROTATE* DefineRotation(UINT mesh, UINT *grp, UINT ngrp, const VECTOR3 &ref, const VECTOR3 &axis, float angle);
	/**
	 * Creates MGROUP_TRANSLATE struct, adds it to animation list, and returns pointer to struct
	 */
	MGROUP_TRANSLATE* DefineTranslation(UINT mesh, UINT* grp, UINT ngrp, const VECTOR3& shift);
	/**
	 * Creates MGROUP_SCALE struct, adds it to animation list, and returns pointer to struct
	 */
	MGROUP_SCALE* DefineScale(UINT mesh, UINT* grp, UINT ngrp, const VECTOR3& ref, const VECTOR3& scale);
};

#endif // BASESSUPAD_H_32815393_9D69_4a58_978C_29F4D3C04278
