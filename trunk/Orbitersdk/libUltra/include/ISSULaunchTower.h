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
	virtual void MoveOrbiterAccessArm(AnimState::Action action) = 0;
	virtual void MoveGOXArm(AnimState::Action action) = 0;
	virtual void MoveGH2Pipe(AnimState::Action action) = 0;
	virtual void MoveVentHood(AnimState::Action action) = 0;
	virtual void MoveLOXArm(AnimState::Action action) = 0;
	//virtual AnimState::Action GetAccessArmState() const;
	//virtual AnimState::Action GetGOXArmState() const;
};

#endif //ISSULAUNCHTOWER_H_5A88719C_C362_4769_992F_89393B4F9728