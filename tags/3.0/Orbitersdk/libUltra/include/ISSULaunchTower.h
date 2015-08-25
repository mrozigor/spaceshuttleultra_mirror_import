#ifndef ISSULAUNCHTOWER_H_5A88719C_C362_4769_992F_89393B4F9728
#define ISSULAUNCHTOWER_H_5A88719C_C362_4769_992F_89393B4F9728
#pragma once

/**
 * Interface for launch pad (does not include MLP).
 * Used for both LC-39 (KSC) and SLC-6 (Vandenberg)
 */
class ISSULaunchTower
{
public:
	virtual void OnT0() = 0;

	virtual void ExtendOrbiterAccessArm( int mode ) = 0;
	virtual void RetractOrbiterAccessArm() = 0;
	virtual void HaltOrbiterAccessArm() = 0;

	virtual void ExtendGOXArm() = 0;
	virtual void RetractGOXArm() = 0;
	virtual void HaltGOXArm() = 0;

	virtual void AttachGH2Ventline() = 0;
	virtual void DetachGH2Ventline() = 0;

	virtual void RaiseVentHood() = 0;
	virtual void LowerVentHood() = 0;
	virtual void HaltVentHood() = 0;

	virtual void ExtendGOXArmAndHood() = 0;
	virtual void RetractGOXArmAndHood() = 0;
	virtual void HaltGOXArmAndHood() = 0;

	virtual void DeployIAA() = 0;
	virtual void HaltIAA() = 0;
	virtual void RetractIAA() = 0;
};

#endif //ISSULAUNCHTOWER_H_5A88719C_C362_4769_992F_89393B4F9728