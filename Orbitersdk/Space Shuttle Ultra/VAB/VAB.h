#pragma once

#include <OrbiterAPI.h>
#include <VesselAPI.h>
#include <queue>

class VAB;

const char MESHNAME_EXTERIOR_1980[] = "SSU\\VAB_exterior_1980";
const char MESHNAME_INTERIOR[] = "SSU\\VAB_interior_structure_low-res";
const double BUILDING_COG_BASE = -79.85;

const unsigned int VAB_HIGHBAY1 = 0;
const unsigned int VAB_HIGHBAY2 = 1;
const unsigned int VAB_HIGHBAY3 = 2;
const unsigned int VAB_HIGHBAY4 = 3;


enum VAB_VLD_INDEX {
	VAB_VLD0 = 0,
	VAB_VLD1 = 1,
	VAB_VLD2 = 2,
	VAB_VLD3 = 3,
	VAB_VLD4 = 4,
	VAB_VLD5 = 5,
	VAB_VLD6 = 6,
	VAB_VLD7 = 7
};

const double VAB_VLD0_REF = 58.80;
const double VAB_VLD1_REF = 43.92;
const double VAB_VLD2_REF = 29.04;
const double VAB_VLD3_REF = 14.16;
const double VAB_VLD4_REF = -0.54;
const double VAB_VLD5_REF = -15.48;
const double VAB_VLD6_REF = -30.26;
const double VAB_VLD7_REF = -45.17;

const double VAB_VLD_REF[8] = {
	VAB_VLD0_REF, 
	VAB_VLD1_REF,
	VAB_VLD2_REF,
	VAB_VLD3_REF,
	VAB_VLD4_REF,
	VAB_VLD5_REF,
	VAB_VLD6_REF,
	VAB_VLD7_REF
};



/**
 * Each horizontal door need about a minute to completely open/close. 
 * Each vertical door need about 2 minutes to move its own height.
 */

/**
 * Speed of the VLD panel in meters per second
 */
const double VAB_VLD_SPEED = 0.124;		
/**
 *	The time the VLD panel needs for accelerating to full speed or back to zero.
 *  In seconds.
 */ 
const double VAB_VLD_T_ACC = 15.0;

/**
 * Distance the VLD travels during acceleration in meters
 */
const double VAB_VLD_D_ACC = 0.5 * VAB_VLD_SPEED * VAB_VLD_T_ACC;

class VerticalDoorAction
{
public:
	enum DOOR_ACTION_TYPE {
		STOP = 0,
		MOVE
	};
protected:
	VAB_VLD_INDEX uiSegment;
	DOOR_ACTION_TYPE curAction;
	VAB_VLD_INDEX uiTargetPos;
public:
	VerticalDoorAction();
	VerticalDoorAction(VAB_VLD_INDEX door_segment, DOOR_ACTION_TYPE action, 
		VAB_VLD_INDEX target_pos = VAB_VLD0);
	VerticalDoorAction(const VerticalDoorAction& copy);

	VAB_VLD_INDEX getTargetPosition() const throw();
	VAB_VLD_INDEX getSegment() const throw();
	

	VerticalDoorAction& operator =(const VerticalDoorAction& o);
};

/**
 * This class encapsules the creation and mathematics of 
 * the VLD animations
 */
class VLDAnimation {
public:
	enum STATE {
		MOVING_UP,
		STOPPED,
		MOVING_DOWN
	};
private:
	VAB* pVAB;
	/**
	 * VLD Index of the panel
	 */
	VAB_VLD_INDEX index;
	/**
	 * Current position from the 0 position in meters
	 */
	double fPosZ;
	/**
	 *  current velocity of the panel in meters/second
	 */
	double fVelZ;

	/**
	 * Length of the travel from lowest position to fully retracted
	 */
	double fDistance;
	UINT uiGroup;
	/**
	 *  Animation component for this animation
	 */
	MGROUP_TRANSLATE* pAnimVLD;
	/**
	 * Animation handle for Orbiter
	 */
	UINT animVLD;
	/**
	 *  Current animation position
	 */
	double fCurPos;
	/**
	 *  Current animation target position
	 */
	double fTargetPos;

	double fVelMax;
	/**
	 *  Acceleration distance in animation coordinates
	 */
	double fAccDistance;
	/**
	 * Current state of the drive motor
	 */
	STATE state;
public:
	/**
	 *	@param vld_idx Index of the VLD Door of this high bay.
	 */
	VLDAnimation(VAB_VLD_INDEX vld_idx);
	~VLDAnimation();
	/**
	 * Create the animation and initialize animation data
	 */
	void create(VAB* vab, UINT mshidx, UINT group);
	double getSpeed() const throw();
	double getPosition() const throw();
	bool getSegmentPosition(VAB_VLD_INDEX &current) const throw();
	/**
	 * Move the panel (or not)
	 */
	void move(double dt);

	void setPosition(double apos);
	void setPosition(VAB_VLD_INDEX spos);
};

class HighBay {
protected:
	/**
	 * Pointer to vab vessel, set by HighBay::init
	 */
	VAB* pVAB;
	/**
	 * Group numbers of the left horizontal door of the high bay.
	 * Set by HighBay::init
	 */
	UINT uiLeftHorDoor[2];
	/**
	 * Group numbers of the right horizontal door of the high bay.
	 * Set by HighBay::init
	 */
	UINT uiRightHorDoor[2];
	
	/**
	 * Queue with the current and the future actions for the vertical doors
	 */
	std::queue<VerticalDoorAction> vld_actions;

	AnimState state_horDoor;
	
	VLDAnimation vld1;
	VLDAnimation vld2;
	VLDAnimation vld3;
	VLDAnimation vld4;
	VLDAnimation vld5;
	VLDAnimation vld6;
	VLDAnimation vld7;
	

public:
	HighBay();
	~HighBay();
	
	/**
	 *
	 */
	void init(VAB* vab, UINT mshidx, const VECTOR3& horDoorDir, const UINT groups[]);

	void execute(double dt);
	/**
	 * Open the highbay vertical doors up to and including  
	 * this segment.
	 */
	bool openTo(unsigned int segment);


	

	
};

class VAB: public VESSEL3
{
	UINT midxExterior;
	UINT midxInterior;

	HighBay highbay[4];

	void DefineDoorAnimations(void);
	void DefineLightBeacons(void);
	void DefineLights(void);

	int Lua_InitInterpreter (void *context);
	int Lua_InitInstance (void *context);
public:
	VAB(OBJHANDLE hVessel, int iFlightModel);
	virtual ~VAB();
	virtual int clbkGeneric(int msgid = 0, int prm = 0, void* context = 0);
	virtual void clbkLoadStateEx(FILEHANDLE scn, void* status);
	virtual void clbkPostStep(double fSimT, double fDeltaT, double fMJD);
	virtual void clbkPreStep(double fSimT, double fDeltaT, double fMJD);
	virtual void clbkSaveState(FILEHANDLE scn);
	virtual void clbkSetClassCaps(FILEHANDLE cfg);
	virtual bool openTo(unsigned int hb, unsigned int segpos);
};