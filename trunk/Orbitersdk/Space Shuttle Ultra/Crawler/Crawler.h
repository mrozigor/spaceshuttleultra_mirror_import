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
#include "meshres_drivetruck.h"

//const double DRIVETRACK_X_OFFSET = 14.539;
//const double DRIVETRACK_Y_OFFSET = 1.765;
const double DRIVETRACK_Y_OFFSET = 2.85;
const double DRIVETRACK_Z_OFFSET = 13.439;
// offsets between crawler drivetracks and main body mesh

//const VECTOR3 CRAWLER_MESH_OFFSET = _V(0.0, 0.0, 2.534);
const VECTOR3 CRAWLER_MESH_OFFSET = _V(0.0, 0.0, 0.0);

const double JACKING_MAX_HEIGHT = 4.0;
// max distance the crawler platform can be jacked up from the drivetrucks

const VECTOR3 MLP_ATTACH_POS = _V(0.0, 5.869, 0.0);
//const VECTOR3 MLP_ATTACH_POS = _V(0.0, 10.869, 0.0);
const VECTOR3 MLP_ATTACH_ROT = _V(0, 0, 1);

const int ENGINE_SOUND_ID = 1;

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
	void clbkPreStep (double simt, double simdt, double mjd);
	void clbkPostStep(double simt, double simdt, double mjd);
	void clbkVisualCreated(VISHANDLE vis, int refcount);
	void clbkVisualDestroyed(VISHANDLE vis, int refcount);
	bool clbkLoadGenericCockpit();
	// This will extract the mission time from the saturn in order to pass to ProjectApollo MFD
	double GetMissionTime() {return MissionTime;};

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

	double velocity;
	bool velocityStop;
	//double targetHeading;
	int viewPos;
	double wheeldeflect;
	int standalone;
	bool firstTimestepDone;
	// Mission Time for passing to PAMFD:
	double MissionTime;

	//double curHeight; // height above ground (m)
	//double curAngle; // angle rel. to horizontal (rad)
	double curFrontHeight, curBackHeight; // height above ground (m)
	double curFrontAngle, curBackAngle; // angle rel. to horizontal (rad)
	double jackHeight; // height to which platform has been jacked

	bool bReverseDirection; // true if attachment point direction needs to be reversed

	bool lastLatLongSet;
	double lastLat;
	double lastLong;
	double lastHead;
	
	VESSELSTATUS2 vs;

	bool keyAccelerate;
	bool keyBrake;
	bool keyLeft;
	bool keyRight;
	bool keyCenter;

	OBJHANDLE hMLP;
	ATTACHMENTHANDLE hMLPAttach;
	//OBJHANDLE hLV;
	//OBJHANDLE hMSS;
	ATTACHMENTHANDLE ahMLP;

	VISHANDLE vccVis;	
	/*MESHGROUP_TRANSFORM vccSpeedGroup, vccSpeedGroupReverse;
	MESHGROUP_TRANSFORM vccSteering1Group, vccSteering2Group;
	MESHGROUP_TRANSFORM vccSteering1GroupReverse, vccSteering2GroupReverse;*/
	double vccSpeed, vccSteering;
    int meshidxCrawler;
    int meshidxTruck1;
	int meshidxTruck2;
	int meshidxTruck3;
	int meshidxTruck4;
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
