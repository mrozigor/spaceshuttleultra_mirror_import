/***************************************************************************
  This file is part of Project Apollo - NASSP
  Copyright 2004-2005

  Crawler Transporter vessel

  Project Apollo is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Project Apollo is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Project Apollo; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See http://nassp.sourceforge.net/license/ for more details.

  **************************** Revision History ****************************
  *	$Log: Crawler.h,v $
  *	Revision 1.1  2009/02/18 23:21:14  tschachim
  *	Moved files as proposed by Artlav.
  *	
  *	Revision 1.16  2007/12/15 19:48:26  lassombra
  *	Added functionality to allow ProjectApollo MFD to get mission time from the Crawler as well as the Saturn.  The Crawler actually extracts the mission time from the Saturn, no updates to scenario files needed.
  *	
  *	Revision 1.15  2007/08/16 16:49:53  tschachim
  *	New meshes.
  *	
  *	Revision 1.14  2007/03/03 00:14:41  tschachim
  *	Bugfix generic cockpit.
  *	
  *	Revision 1.13  2007/03/02 18:34:37  tschachim
  *	Improved crawler VC.
  *	
  *	Revision 1.12  2007/03/01 17:58:26  tschachim
  *	New VC panel
  *	
  *	Revision 1.11  2006/06/26 19:05:36  movieman523
  *	More doxygen, made Lunar EVA a VESSEL2, made SM breakup, made LRV use VESSEL2 save/load functions.
  *	
  *	Revision 1.10  2006/04/25 13:32:50  tschachim
  *	New KSC.
  *	
  *	Revision 1.9  2005/11/23 21:36:55  movieman523
  *	Allow specification of LV name in scenario file.
  *	
  *	Revision 1.8  2005/10/31 19:18:39  tschachim
  *	Bugfixes.
  *	
  *	Revision 1.7  2005/10/31 10:30:35  tschachim
  *	New VAB.
  *	
  *	Revision 1.6  2005/08/05 13:01:49  tschachim
  *	Saturn detachment handling
  *	
  *	Revision 1.5  2005/07/05 17:23:11  tschachim
  *	Scenario saving/loading
  *	
  *	Revision 1.4  2005/07/01 12:23:48  tschachim
  *	Introduced standalone flag
  *	
  *	Revision 1.3  2005/06/29 11:01:18  tschachim
  *	new dynamics, added attachment management
  *	
  *	Revision 1.2  2005/06/14 16:14:41  tschachim
  *	File header inserted.
  *	
  **************************************************************************/
#ifndef _CRAWLER_H
#define _CRAWLER_H
#pragma once

#include <vector>
#include <orbitersdk.h>
#include "Discsignals.h"
#include "../SSUMath.h"
#include "meshres_drivetruck.h"
#include "CrawlerEngine.h"
#include "PanelGroup.h"
#include "SubsystemDirector.h"
#include "Crawler_vc_defs.h"

//const double DRIVETRACK_X_OFFSET = 14.539;
//const double DRIVETRACK_Y_OFFSET = 1.765;
const double FWD_DRIVETRACK_X_OFFSET = 14.45;
const double REAR_DRIVETRACK_X_OFFSET = -12.30;
const double DRIVETRACK_Y_OFFSET = 2.85;
const double DRIVETRACK_Z_OFFSET = 13.239;
// offsets between crawler drivetracks and main body mesh

//const VECTOR3 CRAWLER_MESH_OFFSET = _V(0.0, 0.0, 2.534);
//const VECTOR3 CRAWLER_MESH_OFFSET = _V(0.0, 4.25, 0.0);
const VECTOR3 CRAWLER_MESH_OFFSET = _V(0.0, 0.0, 0.0);

const static char* CRAWLER_MESHNAME = "SSU\\Crawler_mainbody";
const static char* CRAWLER_MESHNAME_1980 = "SSU\\Crawler_mainbody_1980";

const double JACKING_MAX_HEIGHT = 1.8;
// max distance the crawler platform can be jacked up from the drivetrucks
// According to various sources maxing jacking height is 6 ft(1.8 m)

const double MAX_TURN_ANGLE = 10.0*RAD;
// max angle (in radians) each set of tracks can be turned

const VECTOR3 MLP_ATTACH_POS = _V(0.0, 6, 0.0);
//const VECTOR3 MLP_ATTACH_POS = _V(0.0, 10.869, 0.0);
const VECTOR3 MLP_ATTACH_ROT = _V(0, 0, 1);

const double MAX_UNLOADED_SPEED = 2.0/MPS2MPH;
const double MAX_LOADED_SPEED = 0.8/MPS2MPH;

const int ENGINE_SOUND_ID = 1;


// View positions
const int VIEWPOS_FRONTCABIN		= 0;
const int VIEWPOS_REARCABIN			= 1;
const int VIEWPOS_ML				= 2;
const int VIEWPOS_GROUND			= 3;
const int VIEWPOS_FRONTGANGWAY		= 4;
const int VIEWPOS_REARGANGWAY		= 5;
const int VIEWPOS_RIGHTREARGANGWAY	= 6;

const int AID_REAR_OFFSET = 50; // offset added for rear cabs
const int AID_LEFT_MIN			= 0;
const int AID_CTR_MIN			= 10;
const int AID_SPEED_VALUE		= 11;
const int AID_SPEED_BAR			= 12;
const int AID_RIGHT_MIN			= 20;
const int AID_GCIR_PBI			= 21;
const int AID_INDEP_PBI			= 22;
const int AID_CRAB_PBI			= 23;
const int AID_CAB_ACK_PBI		= 24;
const int AID_STEERING_L_VALUE	= 25;
const int AID_STEERING_L_BAR	= 26;
const int AID_STEERING_R_VALUE	= 27;
const int AID_STEERING_R_BAR	= 28;
const int AID_STEERING_DES_VALUE= 29;
const int AID_STEEEING_DES_BAR	= 30;

namespace vc
{
	class CrawlerVC;
};

typedef struct
{
	HINSTANCE hDll;
	SURFHANDLE pbi_lights;
	SURFHANDLE digits_7seg;
} GlobalHandles;


///
/// \ingroup Ground
///
class Crawler: public VESSEL2 {

public:
	Crawler(OBJHANDLE hObj, int fmodel);
	virtual ~Crawler();

	void clbkSetClassCaps(FILEHANDLE cfg);
	void clbkLoadStateEx(FILEHANDLE scn, void *status);
	void clbkSaveState(FILEHANDLE scn);
	int clbkConsumeDirectKey(char *kstate);
	int clbkConsumeBufferedKey(DWORD key, bool down, char *kstate);
	void clbkPostCreation();
	void clbkPreStep (double simt, double simdt, double mjd);
	void clbkPostStep(double simt, double simdt, double mjd);
	void clbkVisualCreated(VISHANDLE vis, int refcount);
	void clbkVisualDestroyed(VISHANDLE vis, int refcount);

	bool clbkLoadGenericCockpit();
	bool clbkLoadVC (int id);
	bool clbkVCMouseEvent(int id, int _event, VECTOR3& p);
	bool clbkVCRedrawEvent(int id, int _event, SURFHANDLE surf);

	// This will extract the mission time from the saturn in order to pass to ProjectApollo MFD
	//double GetMissionTime() {return MissionTime;};

	DiscreteBundleManager* BundleManager() const;

	MESHHANDLE GetVCMesh(vc::CRAWLER_CAB cab) const;

private:
	void DoFirstTimestep();
	/**
	 * Populates vector with all launchpads in scenario
	 **/
	void FindLaunchpads();
	bool IsAttached();
	void Attach();
	void Detach();
	void SetView();
	void SetView(int viewpos);
	void SlowIfDesired(double timeAcceleration);
	void SetMeshesVisibility(WORD mode);

	void DefineAnimations();
	//void CreateVCAnimations(UINT meshIdx, const VECTOR3& pos, const double direction);

	/**
	 * Returns crawler position in vessel-centered local horizon frame
	 * Assumes vessels are close and Earth is flat
	 * @param hVessel -handle of target vessel
	 * @param vs -struct containing crawler's current surf. position
	 */
	VECTOR3 CalcRelSurfPos(OBJHANDLE hVessel, const VESSELSTATUS2& vs) const;
	/**
	 * Sets touchdown points based on position relative to launchpad.
	 * @returns true if crawler is on pad ramp/surface
	 */
	bool UpdateTouchdownPoints(const VECTOR3 &relPos);
	/**
	 * Sets touchdown points based on current hieght/angle values
	 */
	void UpdateTouchdownPoints() const;
	/**
	 * Wrapper for AddAnimationComponent function
	 * MGROUP_TRANSFORM passed MUST be allocated with new and will be deleted by destructor
	 */
	ANIMATIONCOMPONENT_HANDLE AddManagedAnimationComponent(UINT anim, double state0, double state1,
		MGROUP_TRANSFORM *trans, ANIMATIONCOMPONENT_HANDLE parent = NULL);

	double CalcRampHeight(double dist) { return range(0.0, (395.0-dist)*(15.4 / (395.0-131.5)), 15.4); };
	
public:
	VISHANDLE vccVis;
private:
	SubsystemDirector<Crawler>* psubsystems;
	DiscreteBundleManager* pBundleManager;

	vc::PanelGroup<Crawler> pgFwdCab, pgRearCab;
	//vc::CrawlerVC* cabs[2];

	CrawlerEngine* pEngine;

	//double tgtVelocity;
	//double velocity;
	double currentSpeed;
	bool velocityStop;
	//double targetHeading;
	bool bGenericCockpitView;
	int viewPos;
	//double wheeldeflect[2];
	double steeringActual[2], steeringCommanded[2];
	int standalone;
	bool firstTimestepDone;
	// Mission Time for passing to PAMFD:
	//double MissionTime;

	//double curHeight; // height above ground (m)
	//double curAngle; // angle rel. to horizontal (rad)
	double curFrontHeight, curBackHeight; // height above ground (m)
	double curFrontAngle, curBackAngle; // angle rel. to horizontal (rad)
	double jackHeight; // height to which platform has been jacked

	bool lastLatLongSet;
	double lastLat;
	double lastLong;
	double lastHead;
	VESSELSTATUS2 vs;

	DiscOutPort port_steeringCommand[2];
	DiscOutPort port_steeringActual[2];
	DiscInPort port_BrakeSet, port_ParkingBrakeSet;
	DiscOutPort port_Brake;
	DiscInPort independent, crab, greatCircle;
	DiscOutPort port_currentSpeed;

	//bool keyAccelerate;
	//bool keyBrake;
	bool keyLeft;
	bool keyRight;
	bool keyCenter;

	OBJHANDLE hMLP;
	ATTACHMENTHANDLE hMLPAttach;
	//OBJHANDLE hLV;
	//OBJHANDLE hMSS;
	ATTACHMENTHANDLE ahMLP;

	MESHHANDLE hFwdVCMesh;
	MESHHANDLE hRearVCMesh;

	/*MESHGROUP_TRANSFORM vccSpeedGroup, vccSpeedGroupReverse;
	MESHGROUP_TRANSFORM vccSteering1Group, vccSteering2Group;
	MESHGROUP_TRANSFORM vccSteering1GroupReverse, vccSteering2GroupReverse;*/
	double vccSpeed, vccSteering;
    int meshidxCrawler;
    int meshidxTruck1;
	int meshidxTruck2;
	int meshidxTruck3;
	int meshidxTruck4;
	UINT fwdVCIdx, rearVCIdx;
    //int meshidxPanel;
	//int meshidxPanelReverse;
	UINT anim_truck_trans[4];
	UINT anim_truck_rot[4];
	UINT DrivetruckGrpList[NGRP_TRUCK]; // array used to define drivetruck animations


	std::vector<MGROUP_TRANSFORM*> vpAnimations;

	//SoundLib soundlib;
	//Sound soundEngine;
	int SoundID;

	OBJHANDLE hEarth;
	std::vector<OBJHANDLE> vhLC39;
};

#endif //_CRAWLER_H
