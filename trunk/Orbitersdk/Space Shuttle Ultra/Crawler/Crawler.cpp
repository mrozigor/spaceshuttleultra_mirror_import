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
  *	$Log: Crawler.cpp,v $
  *	Revision 1.2  2009/06/15 16:11:32  tschachim
  *	New CollisionSDK.
  *	
  *	Revision 1.1  2009/02/18 23:21:14  tschachim
  *	Moved files as proposed by Artlav.
  *	
  *	Revision 1.28  2008/09/04 22:01:58  tschachim
  *	Bugfix
  *	
  *	Revision 1.27  2008/04/11 11:49:09  tschachim
  *	Fixed BasicExcel for VC6, reduced VS2005 warnings, bugfixes.
  *	
  *	Revision 1.26  2007/12/15 19:48:26  lassombra
  *	Added functionality to allow ProjectApollo MFD to get mission time from the Crawler as well as the Saturn.  The Crawler actually extracts the mission time from the Saturn, no updates to scenario files needed.
  *	
  *	Revision 1.25  2007/08/16 16:49:53  tschachim
  *	New meshes.
  *	
  *	Revision 1.24  2007/08/13 16:05:33  tschachim
  *	Moved bitmaps to subdirectory.
  *	New VAGC mission time pad load handling.
  *	New telescope and sextant panels.
  *	Fixed CSM/LV separation speed.
  *	
  *	Revision 1.23  2007/06/06 15:02:07  tschachim
  *	OrbiterSound 3.5 support, various fixes and improvements.
  *	
  *	Revision 1.22  2007/03/03 19:32:29  tschachim
  *	More viewpoints.
  *	
  *	Revision 1.21  2007/03/03 00:14:40  tschachim
  *	Bugfix generic cockpit.
  *	
  *	Revision 1.20  2007/03/02 18:34:37  tschachim
  *	Improved crawler VC.
  *	
  *	Revision 1.19  2007/03/01 17:58:26  tschachim
  *	New VC panel
  *	
  *	Revision 1.18  2007/02/18 01:35:28  dseagrav
  *	MCC / LVDC++ CHECKPOINT COMMIT. No user-visible functionality added. lvimu.cpp/h and mcc.cpp/h added.
  *	
  *	Revision 1.17  2006/07/24 20:47:27  orbiter_fan
  *	Changed turn speed. Now the Crawler can turn corners just like the real one!
  *	
  *	Revision 1.16  2006/07/17 19:33:36  tschachim
  *	Small improvements of LC39.
  *	
  *	Revision 1.15  2006/06/23 11:58:07  tschachim
  *	Smaller turning diameter
  *	Slower acceleration/braking during time acceleration.
  *	
  *	Revision 1.14  2006/04/25 13:51:32  tschachim
  *	New KSC.
  *	
  *	Revision 1.13  2006/03/02 14:54:39  tschachim
  *	Velocity dependent sound disabled.
  *	
  *	Revision 1.12  2005/12/19 16:46:33  tschachim
  *	Bugfix touchdownpoints.
  *	
  *	Revision 1.11  2005/11/23 21:36:55  movieman523
  *	Allow specification of LV name in scenario file.
  *	
  *	Revision 1.10  2005/10/31 19:18:39  tschachim
  *	Bugfixes.
  *	
  *	Revision 1.9  2005/10/31 10:15:06  tschachim
  *	New VAB.
  *	
  *	Revision 1.8  2005/10/19 11:06:24  tschachim
  *	Changed log file name.
  *	
  *	Revision 1.7  2005/08/14 16:08:20  tschachim
  *	LM is now a VESSEL2
  *	Changed panel restore mechanism because the CSM mechanism
  *	caused CTDs, reason is still unknown.
  *	
  *	Revision 1.6  2005/08/05 12:59:35  tschachim
  *	Saturn detachment handling
  *	
  *	Revision 1.5  2005/07/05 17:23:11  tschachim
  *	Scenario saving/loading
  *	
  *	Revision 1.4  2005/07/01 12:23:48  tschachim
  *	Introduced standalone flag
  *	
  *	Revision 1.3  2005/06/29 11:01:17  tschachim
  *	new dynamics, added attachment management
  *	
  *	Revision 1.2  2005/06/14 16:14:41  tschachim
  *	File header inserted.
  *	
  **************************************************************************/

#define ORBITER_MODULE

// To force orbitersdk.h to use <fstream> in any compiler version
#pragma include_alias( <fstream.h>, <fstream> )
#include "orbitersdk.h"

#include <stdio.h>
#include <math.h>
#include <string>
//#include "nasspsound.h"
#include <OrbiterSoundSDK35.h>
//#include "soundlib.h"
//#include "tracer.h"

#include "../SSUMath.h"
#include "Crawler.h"
#include "meshres_crawler.h"
/*#include "nasspdefs.h"
#include "toggleswitch.h"
#include "apolloguidance.h"
#include "dsky.h"
#include "csmcomputer.h"
#include "IMU.h"
#include "lvimu.h"
#include "saturn.h"
#include "VAB.h"
#include "ML.h"
#include "MSS.h"
#include "papi.h"*/

//#include "../CollisionSDK.h"

// View positions
#define VIEWPOS_FRONTCABIN				0
#define VIEWPOS_REARCABIN				1
#define VIEWPOS_ML						2
#define VIEWPOS_GROUND					3
#define VIEWPOS_FRONTGANGWAY			4
#define VIEWPOS_REARGANGWAY				5
#define VIEWPOS_RIGHTREARGANGWAY		6

HINSTANCE g_hDLL;
//char trace_file[] = "ProjectApollo Crawler.log";

DLLCLBK void InitModule(HINSTANCE hModule) {

	g_hDLL = hModule;
	//InitCollisionSDK();
}

DLLCLBK VESSEL *ovcInit(OBJHANDLE hvessel, int flightmodel) {

	return new Crawler(hvessel, flightmodel);
}

DLLCLBK void ovcExit(VESSEL *vessel) {

	if (vessel) delete (Crawler*)vessel;
}

Crawler::Crawler(OBJHANDLE hObj, int fmodel) : VESSEL2 (hObj, fmodel)
{
	velocity = 0;
	velocityStop = false;
	//targetHeading = 0;
	wheeldeflect = 0;
	viewPos = VIEWPOS_FRONTCABIN;
	firstTimestepDone = false;
	standalone = false;
	MissionTime = 0;

	lastLatLongSet = false;
	lastLat = 0;
	lastLong = 0;

	//curHeight = 0.01;
	//curAngle = 0.0;
	curFrontAngle = 0.0;
	curBackAngle = 0.0;
	curFrontHeight = 0.01;
	curBackHeight = 0.01;
	jackHeight = 0.0;

	bReverseDirection=false;
	
	keyAccelerate = false;
	keyBrake= false;
	keyLeft= false;
	keyRight= false;
	keyCenter = false;

	hMLP = NULL;
	hMLPAttach = NULL;
	//hLV = NULL;
	//hMSS = NULL;
	vccVis = NULL;	
	vccSpeed = 0;	
	vccSteering = 0;
	meshidxCrawler = 0;
	meshidxTruck1 = 0;
	meshidxTruck2 = 0;
	meshidxTruck3 = 0;
	meshidxTruck4 = 0;
	//meshidxPanel = 0;
	//meshidxPanelReverse = 0;

	hEarth = NULL;

	//soundlib.InitSoundLib(hObj, SOUND_DIRECTORY);
	//soundlib.LoadSound(soundEngine, "CrawlerEngine.wav", BOTHVIEW_FADED_MEDIUM);

	//panelMeshoffset = _V(0,0,0);
    //panelMeshidx = 0;

	SoundID = ConnectToOrbiterSoundDLL3(GetHandle());
	RequestLoadVesselWave3(SoundID, ENGINE_SOUND_ID, "Sound\\ShuttleUltra\\CrawlerEngine.wav", BOTHVIEW_FADED_MEDIUM);
}

Crawler::~Crawler() {
	// delete MGROUP_TRANSFORMs
	for(unsigned short i=0;i<vpAnimations.size();i++) delete vpAnimations.at(i);
}

void Crawler::clbkSetClassCaps(FILEHANDLE cfg) {

	SetEmptyMass(2721000);
	SetSize(40);
	SetPMI(_V(133, 189, 89));

	SetSurfaceFrictionCoeff(0.005, 0.5);
	SetRotDrag (_V(0, 0, 0));
	SetCW(0, 0, 0, 0);
	SetPitchMomentScale(0);
	SetBankMomentScale(0);
	SetLiftCoeffFunc(0); 

    ClearMeshes();
    ClearExhaustRefs();
    ClearAttExhaustRefs();

	// Front cabin panel
	//meshoffset = _V(0.767, 3.387, 2.534);
	/*VECTOR3 meshoffset = _V(15.266, 4.675, 20.812);
    meshidxPanel = AddMesh(oapiLoadMeshGlobal("SSU\\Crawler_centerpanel"), &meshoffset);
	SetMeshVisibilityMode(meshidxPanel, MESHVIS_ALWAYS);

    // Speed indicator
	vccSpeedGroup.P.transparam.shift = _V(0, 0, 0);
	vccSpeedGroup.nmesh = meshidxPanel;
	vccSpeedGroup.ngrp = 10;
	vccSpeedGroup.transform = MESHGROUP_TRANSFORM::TRANSLATE;

    // Steering wheel rotation
	vccSteering1Group.P.rotparam.ref = _V(0.148, 0.084, 0.225);
	vccSteering1Group.P.rotparam.axis = _V(0, 0.913545, -0.406736);
	vccSteering1Group.P.rotparam.angle = 0.0;  
	vccSteering1Group.nmesh = meshidxPanel;
	vccSteering1Group.ngrp = 11;
	vccSteering1Group.transform = MESHGROUP_TRANSFORM::ROTATE;
	vccSteering2Group.P.rotparam.ref = _V(0.148, 0.084, 0.225);
	vccSteering2Group.P.rotparam.axis = _V(0, 0.913545, -0.406736);
	vccSteering2Group.P.rotparam.angle = 0.0;  
	vccSteering2Group.nmesh = meshidxPanel;
	vccSteering2Group.ngrp = 12;
	vccSteering2Group.transform = MESHGROUP_TRANSFORM::ROTATE;*/

	// Rear cabin panel
	//meshoffset = _V(-15.057, 4.675, -15.298);
	/*meshoffset = _V(-15.599, 1.288, -17.278) + CRAWLER_MESH_OFFSET;
    meshidxPanelReverse = AddMesh(oapiLoadMeshGlobal("SSU\\Crawler_centerpanel_reverse"), &meshoffset);
	SetMeshVisibilityMode(meshidxPanelReverse, MESHVIS_ALWAYS);

    // Speed indicator (reverse)
	vccSpeedGroupReverse.P.transparam.shift = _V(0, 0, 0);
	vccSpeedGroupReverse.nmesh = meshidxPanelReverse;
	vccSpeedGroupReverse.ngrp = 10;
	vccSpeedGroupReverse.transform = MESHGROUP_TRANSFORM::TRANSLATE;

    // Steering wheel rotation (reverse)
	vccSteering1GroupReverse.P.rotparam.ref = _V(0.076, 0.084, 0.33);
	vccSteering1GroupReverse.P.rotparam.axis = _V(0, 0.913545, 0.406736);
	vccSteering1GroupReverse.P.rotparam.angle = 0.0;  
	vccSteering1GroupReverse.nmesh = meshidxPanelReverse;
	vccSteering1GroupReverse.ngrp = 11;
	vccSteering1GroupReverse.transform = MESHGROUP_TRANSFORM::ROTATE;
	vccSteering2GroupReverse.P.rotparam.ref = _V(0.076, 0.084, 0.33);
	vccSteering2GroupReverse.P.rotparam.axis = _V(0, 0.913545, 0.406736);
	vccSteering2GroupReverse.P.rotparam.angle = 0.0;  
	vccSteering2GroupReverse.nmesh = meshidxPanelReverse;
	vccSteering2GroupReverse.ngrp = 12;
	vccSteering2GroupReverse.transform = MESHGROUP_TRANSFORM::ROTATE;*/

	// Crawler
	//meshoffset = _V(0.767, 3.387, 2.534);
	VECTOR3 crawler_meshoffset = CRAWLER_MESH_OFFSET;
    meshidxCrawler = AddMesh(oapiLoadMeshGlobal("SSU\\Crawler_mainbody"), &crawler_meshoffset);
	SetMeshVisibilityMode(meshidxCrawler, MESHVIS_ALWAYS);

	// Tracks
	MESHHANDLE track = oapiLoadMeshGlobal("SSU\\Crawler_drivetrucks");
	
	//meshoffset = _V(14.539, 1.765, 15.512);
	VECTOR3 meshoffset = crawler_meshoffset + _V(14.539, DRIVETRACK_Y_OFFSET, DRIVETRACK_Z_OFFSET);
    meshidxTruck1 = AddMesh(track, &meshoffset);
	SetMeshVisibilityMode(meshidxTruck1, MESHVIS_ALWAYS);

	//meshoffset = _V(-12.957, 1.765, 15.512);
	meshoffset = crawler_meshoffset + _V(-12.657, DRIVETRACK_Y_OFFSET, DRIVETRACK_Z_OFFSET);
    meshidxTruck2 = AddMesh(track, &meshoffset);
	SetMeshVisibilityMode(meshidxTruck2, MESHVIS_ALWAYS);

	//meshoffset = _V(14.539, 1.765, -10.359);
	meshoffset = crawler_meshoffset + _V(14.539, DRIVETRACK_Y_OFFSET, -DRIVETRACK_Z_OFFSET);
    meshidxTruck3 = AddMesh(track, &meshoffset);
	SetMeshVisibilityMode(meshidxTruck3, MESHVIS_ALWAYS);

	//meshoffset = _V(-12.957, 1.765, -10.359);
	meshoffset = crawler_meshoffset + _V(-12.657, DRIVETRACK_Y_OFFSET, -DRIVETRACK_Z_OFFSET);
    meshidxTruck4 = AddMesh(track, &meshoffset);
	SetMeshVisibilityMode(meshidxTruck4, MESHVIS_ALWAYS);

	// initialize array of groups needed for drivetruck translation animation
	for(int i=0, j=0 ; i<NGRP_TRUCK ; i++) {
		if(i!=GRP_JEL1_TRUCK && i!=GRP_JEL2_TRUCK) {
			DrivetruckGrpList[j]=i;
			j++;
		}
	}
	DrivetruckGrpList[NGRP_TRUCK-2]=GRP_JEL1_TRUCK;
	DrivetruckGrpList[NGRP_TRUCK-1]=GRP_JEL2_TRUCK;

	static VECTOR3 dummy_vec[4];
	MGROUP_ROTATE* Rot_Truck1 = new MGROUP_ROTATE(LOCALVERTEXLIST, MAKEGROUPARRAY(&dummy_vec[0]), 1, _V(0, 0, 0), _V(-1, 0, 0), (float)(20.0*RAD));
	anim_truck_rot[0] = CreateAnimation(0.5);
	ANIMATIONCOMPONENT_HANDLE parent = AddManagedAnimationComponent(anim_truck_rot[0], 0.0, 1.0, Rot_Truck1);
	MGROUP_TRANSLATE* Trans_Truck1 = new MGROUP_TRANSLATE(meshidxTruck1, DrivetruckGrpList, NGRP_TRUCK-2, _V(0.0, -JACKING_MAX_HEIGHT, 0.0));
	anim_truck_trans[0] = CreateAnimation(0.0);
	AddManagedAnimationComponent(anim_truck_trans[0], 0.0, 1.0, Trans_Truck1, parent);
	MGROUP_SCALE* Scale_Truck1 = new MGROUP_SCALE(meshidxTruck1, &DrivetruckGrpList[NGRP_TRUCK-2], 1, _V(0, 0.948, 0), _V(1, 1.0935*JACKING_MAX_HEIGHT, 1));
	AddManagedAnimationComponent(anim_truck_trans[0], 0.0, 1.0, Scale_Truck1, parent);
	// strut animations
	static UINT Strut11GrpList[4] = {GRP_Cylinder668, GRP_Cylinder669, GRP_Cylinder670, GRP_Cylinder682};
	MGROUP_ROTATE* Rotate_Strut11 = new MGROUP_ROTATE(meshidxCrawler, Strut11GrpList, 3, _V(8.705, 2.499, 18.125), _V(0, 0, -1), (float)(45.0*RAD));
	parent = AddManagedAnimationComponent(anim_truck_trans[0], 0.0, 1.0, Rotate_Strut11);
	MGROUP_SCALE* Scale_Strut11 = new MGROUP_SCALE(meshidxCrawler, &Strut11GrpList[3], 1, _V(12.836, 3.013, 18.125), _V(1.75, 1.0, 1.0));
	AddManagedAnimationComponent(anim_truck_trans[0], 0.0, 1.0, Scale_Strut11, parent);
	static UINT Strut12GrpList[4] = {GRP_Cylinder673, GRP_Cylinder674, GRP_Cylinder675, GRP_Cylinder683};
	MGROUP_ROTATE* Rotate_Strut12 = new MGROUP_ROTATE(meshidxCrawler, Strut12GrpList, 3, _V(8.705, 2.499, 8.277), _V(0, 0, -1), (float)(45.0*RAD));
	parent = AddManagedAnimationComponent(anim_truck_trans[0], 0.0, 1.0, Rotate_Strut12);
	MGROUP_SCALE* Scale_Strut12 = new MGROUP_SCALE(meshidxCrawler, &Strut12GrpList[3], 1, _V(12.836, 3.013, 8.277), _V(1.75, 1.0, 1.0));
	AddManagedAnimationComponent(anim_truck_trans[0], 0.0, 1.0, Scale_Strut12, parent);

	MGROUP_ROTATE* Rot_Truck2 = new MGROUP_ROTATE(LOCALVERTEXLIST, MAKEGROUPARRAY(&dummy_vec[1]), 1, _V(0, 0, 0), _V(-1, 0, 0), (float)(20.0*RAD));
	anim_truck_rot[1] = CreateAnimation(0.5);
	parent = AddManagedAnimationComponent(anim_truck_rot[1], 0.0, 1.0, Rot_Truck2);
	MGROUP_TRANSLATE* Trans_Truck2 = new MGROUP_TRANSLATE(meshidxTruck2, DrivetruckGrpList, NGRP_TRUCK-2, _V(0.0, -JACKING_MAX_HEIGHT, 0.0));
	anim_truck_trans[1] = CreateAnimation(0.0);
	AddManagedAnimationComponent(anim_truck_trans[1], 0.0, 1.0, Trans_Truck2, parent);
	MGROUP_SCALE* Scale_Truck2 = new MGROUP_SCALE(meshidxTruck2, &DrivetruckGrpList[NGRP_TRUCK-2], 1, _V(0, 1.004, 0), _V(1, 1.0935*JACKING_MAX_HEIGHT, 1));
	AddManagedAnimationComponent(anim_truck_trans[1], 0.0, 1.0, Scale_Truck2, parent);
	// strut animations
	static UINT Strut21GrpList[4] = {GRP_Cylinder496, GRP_Cylinder497, GRP_Cylinder498, GRP_Cylinder499};
	MGROUP_ROTATE* Rotate_Strut21 = new MGROUP_ROTATE(meshidxCrawler, Strut21GrpList, 3, _V(-8.698, 2.518, 18.125), _V(0, 0, 1), (float)(45.0*RAD));
	parent = AddManagedAnimationComponent(anim_truck_trans[1], 0.0, 1.0, Rotate_Strut21);
	MGROUP_SCALE* Scale_Strut21 = new MGROUP_SCALE(meshidxCrawler, &Strut21GrpList[3], 1, _V(-12.829, 3.033, 18.12), _V(1.75, 1.0, 1.0));
	AddManagedAnimationComponent(anim_truck_trans[1], 0.0, 1.0, Scale_Strut21, parent);
	static UINT Strut22GrpList[4] = {GRP_Cylinder430, GRP_Cylinder457, GRP_Cylinder458, GRP_Cylinder459};
	MGROUP_ROTATE* Rotate_Strut22 = new MGROUP_ROTATE(meshidxCrawler, Strut22GrpList, 3, _V(-8.698, 2.518, 8.274), _V(0, 0, 1), (float)(45.0*RAD));
	parent = AddManagedAnimationComponent(anim_truck_trans[1], 0.0, 1.0, Rotate_Strut22);
	MGROUP_SCALE* Scale_Strut22 = new MGROUP_SCALE(meshidxCrawler, &Strut22GrpList[3], 1, _V(-12.829, 3.033, 8.275), _V(1.75, 1.0, 1.0));
	AddManagedAnimationComponent(anim_truck_trans[1], 0.0, 1.0, Scale_Strut22, parent);

	MGROUP_ROTATE* Rot_Truck3 = new MGROUP_ROTATE(LOCALVERTEXLIST, MAKEGROUPARRAY(&dummy_vec[2]), 1, _V(0, 0, 0), _V(-1, 0, 0), (float)(20.0*RAD));
	anim_truck_rot[2] = CreateAnimation(0.5);
	parent = AddManagedAnimationComponent(anim_truck_rot[2], 0.0, 1.0, Rot_Truck3);
	MGROUP_TRANSLATE* Trans_Truck3 = new MGROUP_TRANSLATE(meshidxTruck3, DrivetruckGrpList, NGRP_TRUCK-2, _V(0.0, -JACKING_MAX_HEIGHT, 0.0));
	anim_truck_trans[2] = CreateAnimation(0.0);
	AddManagedAnimationComponent(anim_truck_trans[2], 0.0, 1.0, Trans_Truck3, parent);
	MGROUP_SCALE* Scale_Truck3 = new MGROUP_SCALE(meshidxTruck3, &DrivetruckGrpList[NGRP_TRUCK-2], 1,_V(0, 1.004, 0), _V(1, 1.0935*JACKING_MAX_HEIGHT, 1));
	AddManagedAnimationComponent(anim_truck_trans[2], 0.0, 1.0, Scale_Truck3, parent);
	// strut animations
	static UINT Strut31GrpList[4] = {GRP_Cylinder657, GRP_Cylinder658, GRP_Cylinder659, GRP_Cylinder667};
	MGROUP_ROTATE* Rotate_Strut31 = new MGROUP_ROTATE(meshidxCrawler, Strut31GrpList, 3, _V(8.705, 2.499, -18.186), _V(0, 0, -1), (float)(45.0*RAD));
	parent = AddManagedAnimationComponent(anim_truck_trans[2], 0.0, 1.0, Rotate_Strut31);
	MGROUP_SCALE* Scale_Strut31 = new MGROUP_SCALE(meshidxCrawler, &Strut31GrpList[3], 1, _V(12.836, 3.013, -18.183), _V(1.75, 1.0, 1.0));
	AddManagedAnimationComponent(anim_truck_trans[2], 0.0, 1.0, Scale_Strut31, parent);
	static UINT Strut32GrpList[4] = {GRP_Cylinder652, GRP_Cylinder653, GRP_Cylinder654, GRP_Cylinder666};
	MGROUP_ROTATE* Rotate_Strut32 = new MGROUP_ROTATE(meshidxCrawler, Strut32GrpList, 3, _V(8.705, 2.499, -8.337), _V(0, 0, -1), (float)(45.0*RAD));
	parent = AddManagedAnimationComponent(anim_truck_trans[2], 0.0, 1.0, Rotate_Strut32);
	MGROUP_SCALE* Scale_Strut32 = new MGROUP_SCALE(meshidxCrawler, &Strut32GrpList[3], 1, _V(12.836, 3.013, -8.335), _V(1.75, 1.0, 1.0));
	AddManagedAnimationComponent(anim_truck_trans[2], 0.0, 1.0, Scale_Strut32, parent);

	MGROUP_ROTATE* Rot_Truck4 = new MGROUP_ROTATE(LOCALVERTEXLIST, MAKEGROUPARRAY(&dummy_vec[3]), 1, _V(0, 0, 0), _V(-1, 0, 0), (float)(20.0*RAD));
	anim_truck_rot[3] = CreateAnimation(0.5);
	parent = AddManagedAnimationComponent(anim_truck_rot[3], 0.0, 1.0, Rot_Truck4);
	MGROUP_TRANSLATE* Trans_Truck4 = new MGROUP_TRANSLATE(meshidxTruck4, DrivetruckGrpList, NGRP_TRUCK-2, _V(0.0, -JACKING_MAX_HEIGHT, 0.0));
	anim_truck_trans[3] = CreateAnimation(0.0);
	AddManagedAnimationComponent(anim_truck_trans[3], 0.0, 1.0, Trans_Truck4, parent);
	MGROUP_SCALE* Scale_Truck4 = new MGROUP_SCALE(meshidxTruck4, &DrivetruckGrpList[NGRP_TRUCK-2], 1, _V(0, 1.004, 0), _V(1, 1.0935*JACKING_MAX_HEIGHT, 1));
	AddManagedAnimationComponent(anim_truck_trans[3], 0.0, 1.0, Scale_Truck4, parent);
	// strut animations
	static UINT Strut41GrpList[4] = {GRP_Cylinder636, GRP_Cylinder637, GRP_Cylinder638, GRP_Cylinder650};
	MGROUP_ROTATE* Rotate_Strut41 = new MGROUP_ROTATE(meshidxCrawler, Strut41GrpList, 3, _V(-8.698, 2.518, -18.186), _V(0, 0, 1), (float)(45.0*RAD));
	parent = AddManagedAnimationComponent(anim_truck_trans[3], 0.0, 1.0, Rotate_Strut41);
	MGROUP_SCALE* Scale_Strut41 = new MGROUP_SCALE(meshidxCrawler, &Strut41GrpList[3], 1, _V(-12.829, 3.033, -18.195), _V(1.75, 1.0, 1.0));
	AddManagedAnimationComponent(anim_truck_trans[3], 0.0, 1.0, Scale_Strut41, parent);
	static UINT Strut42GrpList[4] = {GRP_Cylinder641, GRP_Cylinder642, GRP_Cylinder643, GRP_Cylinder651};
	MGROUP_ROTATE* Rotate_Strut42 = new MGROUP_ROTATE(meshidxCrawler, Strut42GrpList, 3, _V(-8.698, 2.518, -8.337), _V(0, 0, 1), (float)(45.0*RAD));
	parent = AddManagedAnimationComponent(anim_truck_trans[3], 0.0, 1.0, Rotate_Strut42);
	MGROUP_SCALE* Scale_Strut42 = new MGROUP_SCALE(meshidxCrawler, &Strut42GrpList[3], 1, _V(-12.829, 3.033, -8.345), _V(1.75, 1.0, 1.0));
	AddManagedAnimationComponent(anim_truck_trans[3], 0.0, 1.0, Scale_Strut42, parent);

	// Panel position test
	// panelMeshoffset = meshoffset;
	// panelMeshidx = meshidxTruck3;

	//CreateAttachment(false, _V(0.0, 6.3, 0.0), _V(0, 1, 0), _V(1, 0, 0), "ML", false);
	ahMLP = CreateAttachment(false, MLP_ATTACH_POS, _V(0, -1, 0), MLP_ATTACH_ROT, "XMLP");

	//VSEnableCollisions(GetHandle(),"ProjectApollo");
	//double tph = -0.01;
	//SetTouchdownPoints(_V(  0, tph,  10), _V(-10, tph, -10), _V( 10, tph, -10));
	//VSSetTouchdownPoints(GetHandle(), _V(  0, tph,  10), _V(-10, tph, -10), _V( 10, tph, -10));
	SetTouchdownPoints(_V(  0, 0.01,  10), _V(-10, 0.01, -10), _V( 10, 0.01, -10));
	//ShiftCG(_V(0, -16, 0));
}

void Crawler::clbkPreStep(double simt, double simdt, double mjd) {

	double maxVelocity = 0.894;

	if (!firstTimestepDone) DoFirstTimestep(); 

	if (IsAttached()) maxVelocity = maxVelocity / 2.0;

	double dv;
	//oapiGetHeading(GetHandle(), &lastHead);
	double timeW = oapiGetTimeAcceleration();

	if (((keyAccelerate && viewPos == VIEWPOS_FRONTCABIN) || (keyBrake && viewPos == VIEWPOS_REARCABIN)) && !velocityStop) {
		dv = 0.5 * simdt * (pow(2.0, log10(timeW))) / timeW;
		if (velocity < 0 && velocity + dv >= 0) {
			velocity = 0;
			velocityStop = true;
		} else {
			velocity += dv;
		}
		if (velocity > maxVelocity) velocity = maxVelocity;

	} else if (((keyBrake && viewPos == VIEWPOS_FRONTCABIN) || (keyAccelerate && viewPos == VIEWPOS_REARCABIN)) && !velocityStop) {
		dv = -0.5 * simdt * (pow(2.0, log10(timeW))) / timeW;
		if (velocity > 0 && velocity + dv <= 0) {
			velocity = 0;
			velocityStop = true;
		} else {
			velocity += dv;
		}
		if (velocity < -maxVelocity) velocity = -maxVelocity;
	
	} else if (!keyAccelerate && !keyBrake) {
		velocityStop = false;
	}
	// Reset flags
	keyAccelerate = false;
	keyBrake = false;

	double lat, lon;
	vs.version = 2;
	GetStatusEx(&vs);

	if (vs.status == 1) {
		lon = vs.surf_lng;
		lat = vs.surf_lat;
		lastHead = vs.surf_hdg;
	} else if (lastLatLongSet) {
		lon = lastLong;
		lat = lastLat;
		vs.surf_hdg = lastHead;
	
	} else return;

	if ((keyLeft && viewPos == VIEWPOS_FRONTCABIN) || (keyRight && viewPos == VIEWPOS_REARCABIN)) {
		wheeldeflect = max(-1,wheeldeflect - (0.5 * simdt * (pow(2.0, log10(timeW))) / timeW));
	
	} else if ((keyRight && viewPos == VIEWPOS_FRONTCABIN) || (keyLeft && viewPos == VIEWPOS_REARCABIN)) {
		wheeldeflect = min(1, wheeldeflect + (0.5 * simdt * (pow(2.0, log10(timeW))) / timeW));

	} else if (keyCenter) {
		wheeldeflect = 0;
	}
	keyLeft = false;
	keyRight = false;
	keyCenter = false;

	double r = 45; // 150 ft
	double dheading = wheeldeflect * velocity * simdt / r;
	// turn speed is only doubled when time acc is multiplied by ten:
	dheading = (pow(5.0, log10(timeW))) * dheading / timeW;
	vs.surf_hdg += dheading;
	if(vs.surf_hdg< 0) vs.surf_hdg += 2.0 * PI;
	if(vs.surf_hdg >= 2.0 * PI) vs.surf_hdg -= 2.0 * PI;

	lon += sin(lastHead) * velocity * simdt / oapiGetSize(hEarth);
	lat += cos(lastHead) * velocity * simdt / oapiGetSize(hEarth);
	vs.surf_lng = lon;
	vs.surf_lat = lat;
	vs.status = 1;
	DefSetStateEx(&vs);

	lastLat = lat;
	lastLong = lon;
	lastHead = vs.surf_hdg;
	lastLatLongSet = true;
	//targetHeading = lastHead;

	// check distance from pads; adjust touchdown points to simulate going up ramp
	for(unsigned int i=0; i<vhLC39.size(); i++) {
		VESSEL* pV=oapiGetVesselInterface(vhLC39[i]);

		/*double padLat, padLong, padRad;
		oapiGetEquPos(vhLC39[i], &padLong, &padLat, &padRad);
		VECTOR3 rpos = _V(padLat*oapiGetSize(hEarth)-vs.surf_lat*oapiGetSize(hEarth), padLong*oapiGetSize(hEarth)-vs.surf_lng*oapiGetSize(hEarth), 0.0);*/
		VECTOR3 rpos = CalcRelSurfPos(vhLC39[i], vs);

		//sprintf_s(oapiDebugString(), 255, "RelPos: %f %f %f", rpos.x, rpos.y, rpos.z);
		if(UpdateTouchdownPoints(rpos)) break;
	}

	// play sounds
	if (velocity != 0) {
		// velocity dependent sound disabled
		// soundEngine.play(LOOP, (int)(127.5 + 127.5 * velocity / maxVelocity));
		//soundEngine.play();
		PlayVesselWave3(SoundID, ENGINE_SOUND_ID, LOOP);
	} else {
		//soundEngine.stop();
		StopVesselWave3(SoundID, ENGINE_SOUND_ID);
	}

	//
	// Update the VC console
	//
	/*if (vccVis) {
		// Speed indicators
		double v = (velocity * 0.066); 
		vccSpeedGroup.P.transparam.shift.x = float(v - vccSpeed);
		vccSpeedGroupReverse.P.transparam.shift.x = float(v - vccSpeed);
		MeshgroupTransform(vccVis, vccSpeedGroup);
		MeshgroupTransform(vccVis, vccSpeedGroupReverse);
		vccSpeed = v;

		// Steering wheel
		double a = -wheeldeflect * 90.0 * RAD;
		vccSteering1Group.P.rotparam.angle = float(a - vccSteering);
		vccSteering2Group.P.rotparam.angle = float(a - vccSteering);
		vccSteering1GroupReverse.P.rotparam.angle = -float(a - vccSteering);
		vccSteering2GroupReverse.P.rotparam.angle = -float(a - vccSteering);
		MeshgroupTransform(vccVis, vccSteering1Group);
		MeshgroupTransform(vccVis, vccSteering2Group);
		MeshgroupTransform(vccVis, vccSteering1GroupReverse);
		MeshgroupTransform(vccVis, vccSteering2GroupReverse);
		vccSteering = a;		
	}*/

	/* VECTOR3 pos;
	GetRelativePos(hMSS, pos);
	sprintf(oapiDebugString(), "Dist %f", length(pos)); */
}

void Crawler::clbkPostStep(double simt, double simdt, double mjd) {
	//This seems the best place to update our mission time off of the Saturn.
	/*Saturn *lv = NULL;
	if (!standalone) {
		// Updating internal mission time from Launch Vehichle.
		lv = (Saturn *)oapiGetVesselInterface(hLV);
		MissionTime = lv -> GetMissionTime();
	}*/
}

void Crawler::DoFirstTimestep() {

	SetView();

	FindLaunchpads();
	hEarth = GetGravityRef();

	//oapiGetHeading(GetHandle(), &targetHeading);
	
	// Turn off pretty much everything that Orbitersound does by default.
	/*soundlib.SoundOptionOnOff(PLAYCOUNTDOWNWHENTAKEOFF, FALSE);
	soundlib.SoundOptionOnOff(PLAYCABINAIRCONDITIONING, FALSE);
	soundlib.SoundOptionOnOff(PLAYCABINRANDOMAMBIANCE, FALSE);
	soundlib.SoundOptionOnOff(PLAYLANDINGANDGROUNDSOUND, FALSE);
	soundlib.SoundOptionOnOff(PLAYRADIOATC, FALSE);
	soundlib.SoundOptionOnOff(PLAYRADARBIP, FALSE);
	soundlib.SoundOptionOnOff(DISPLAYTIMER, FALSE);*/

	if (!standalone) {
		/*if (LVName[0])
			hLV = oapiGetVesselByName(LVName);
		hML = oapiGetVesselByName("ML");
		hMSS = oapiGetVesselByName("MSS");*/
		hMLP = GetAttachmentStatus(ahMLP);
		if(hMLP) {
			// find point on MLP we are attached to
			VESSEL* pMLP = oapiGetVesselInterface(hMLP);
			for(DWORD i=0;i<pMLP->AttachmentCount(true);i++) {
				ATTACHMENTHANDLE aH = pMLP->GetAttachmentHandle(true, i);
				if(pMLP->GetAttachmentStatus(aH)==GetHandle()) hMLPAttach = aH;
			}
		}
	}
	firstTimestepDone = true;
}

void Crawler::FindLaunchpads()
{
	for(DWORD i=0;i<oapiGetVesselCount();i++) {
		OBJHANDLE hVessel=oapiGetVesselByIndex(i);
		if(hVessel && !_strnicmp(oapiGetVesselInterface(hVessel)->GetClassName(), "SSU_Pad", 7)) {
			vhLC39.push_back(hVessel);
			//oapiWriteLog("Found pad");
		}
	}
}

void Crawler::clbkLoadStateEx(FILEHANDLE scn, void *status) {
	
	char *line;
	
	while (oapiReadScenario_nextline (scn, line)) {
		if (!_strnicmp (line, "VELOCITY", 8)) {
			sscanf (line + 8, "%lf", &velocity);
		} else if (!_strnicmp (line, "JACK_HEIGHT", 11)) {
			sscanf (line + 11, "%lf", &jackHeight);
		} else if (!_strnicmp (line, "WHEELDEFLECT", 12)) {
			sscanf (line + 12, "%lf", &wheeldeflect);
		} else if (!_strnicmp (line, "VIEWPOS", 7)) {
			sscanf (line + 7, "%i", &viewPos);
		} else if (!_strnicmp (line, "STANDALONE", 10)) {
			sscanf (line + 10, "%i", &standalone);
		} else if (!_strnicmp (line, "HEIGHT", 6)) {
			sscanf (line+6, "%lf%lf", &curFrontHeight, &curBackHeight);
			SetTouchdownPoints(_V(0, curFrontHeight,  10), _V(-10, curFrontHeight, -10), _V(10, curFrontHeight, -10));
		} else if (!_strnicmp (line, "ANGLE", 5)) {
			sscanf (line+5, "%lf%lf", &curFrontAngle, &curBackAngle);
		} else if (!_strnicmp (line, "GROUND_POS", 10)) {
			sscanf (line + 10, "%lf%lf%lf", &lastLat, &lastLong, &lastHead);
			lastLatLongSet=true;
		} else if (!_strnicmp (line, "REVERSE_ATTACH", 14)) {
			bReverseDirection=true;
			SetAttachmentParams(ahMLP, MLP_ATTACH_POS, _V(0, -1, 0), -MLP_ATTACH_ROT);
		} else {
			ParseScenarioLineEx (line, status);
		}
	}

	UpdateTouchdownPoints();
}

void Crawler::clbkSaveState(FILEHANDLE scn) {
	
	VESSEL2::clbkSaveState (scn);

	char cbuf[255];

	oapiWriteScenario_float(scn, "VELOCITY", velocity);
	oapiWriteScenario_float(scn, "WHEELDEFLECT", wheeldeflect);	
	oapiWriteScenario_float(scn, "JACK_HEIGHT", jackHeight);
	sprintf_s(cbuf, 255, "%f %f", curFrontHeight, curBackHeight);
	oapiWriteScenario_string(scn, "HEIGHT", cbuf);
	sprintf_s(cbuf, 255, "%f %f", curFrontAngle, curBackAngle);
	oapiWriteScenario_string(scn, "ANGLE", cbuf);
	oapiWriteScenario_int(scn, "VIEWPOS", viewPos);
	oapiWriteScenario_int(scn, "STANDALONE", standalone);
	sprintf_s(cbuf, 255, "%.10f %.10f %.10f", lastLat, lastLong, lastHead);
	oapiWriteScenario_string(scn, "GROUND_POS", cbuf);
	if(bReverseDirection) oapiWriteLine(scn, "  REVERSE_ATTACH");
}

int Crawler::clbkConsumeDirectKey(char *kstate) {

	if (!firstTimestepDone) return 0;

	if (KEYMOD_SHIFT(kstate) /*|| KEYMOD_CONTROL(kstate)*/) {
		return 0; 
	}

	if(KEYMOD_CONTROL(kstate)) {
		if (KEYDOWN(kstate, OAPI_KEY_J)) { // raise platform
			jackHeight=min(jackHeight+0.0001*oapiGetTimeAcceleration(), JACKING_MAX_HEIGHT);
			UpdateTouchdownPoints();
			RESETKEY(kstate, OAPI_KEY_J);
		}
		if (KEYDOWN(kstate, OAPI_KEY_K)) { // lower platform
			jackHeight=max(jackHeight-0.0001*oapiGetTimeAcceleration(), 0.0);
			UpdateTouchdownPoints();
			RESETKEY(kstate, OAPI_KEY_K);
		}
	}
	else {
		if (KEYDOWN(kstate, OAPI_KEY_ADD)) {
			keyAccelerate = true;				
			RESETKEY(kstate, OAPI_KEY_ADD);
		}
		if (KEYDOWN(kstate, OAPI_KEY_SUBTRACT)) {
			keyBrake = true;				
			RESETKEY(kstate, OAPI_KEY_SUBTRACT);
		}
		if (KEYDOWN(kstate, OAPI_KEY_NUMPAD1)) {
			keyLeft = true;				
			RESETKEY(kstate, OAPI_KEY_NUMPAD1);
		}
		if (KEYDOWN(kstate, OAPI_KEY_NUMPAD2)) {
			keyCenter = true;				
			RESETKEY(kstate, OAPI_KEY_NUMPAD2);
		}
		if (KEYDOWN(kstate, OAPI_KEY_NUMPAD3)) {
			keyRight = true;				
			RESETKEY(kstate, OAPI_KEY_NUMPAD3);
		}
		if (KEYDOWN(kstate, OAPI_KEY_NUMPAD5)) {
			keyCenter = true;				
			RESETKEY(kstate, OAPI_KEY_NUMPAD5);
		}
	}

	// touchdown point test
	/*
	VESSELSTATUS vs;
	GetStatus(vs);

	if (KEYDOWN (kstate, OAPI_KEY_A)) {
		touchdownPointHeight += 0.001;

		SetTouchdownPoints(_V(  0, touchdownPointHeight,  10), 
						   _V(-10, touchdownPointHeight, -10), 
						   _V( 10, touchdownPointHeight, -10));
		VSSetTouchdownPoints(GetHandle(), 
						   _V(  0, touchdownPointHeight,  10), 
						   _V(-10, touchdownPointHeight, -10), 
						   _V( 10, touchdownPointHeight, -10),
						   -touchdownPointHeight);

		sprintf(oapiDebugString(), "touchdownPointHeight %f", touchdownPointHeight);
		RESETKEY(kstate, OAPI_KEY_A);
	}
	if (KEYDOWN (kstate, OAPI_KEY_S)) {
		touchdownPointHeight -= 0.001;

		SetTouchdownPoints(_V(  0, touchdownPointHeight,  10), 
						   _V(-10, touchdownPointHeight, -10), 
						   _V( 10, touchdownPointHeight, -10));
		VSSetTouchdownPoints(GetHandle(), 
						   _V(  0, touchdownPointHeight,  10), 
						   _V(-10, touchdownPointHeight, -10), 
						   _V( 10, touchdownPointHeight, -10),
						   -touchdownPointHeight);

		sprintf(oapiDebugString(), "touchdownPointHeight %f", touchdownPointHeight);
		RESETKEY(kstate, OAPI_KEY_S);
	}
	*/

	// Panel test
	/*		
	double step = 0.01;
	if (KEYMOD_CONTROL(kstate))
		step = 0.001;

	if (KEYDOWN(kstate, OAPI_KEY_NUMPAD4)) {
		DelMesh(panelMeshidx);
		panelMeshoffset.x += step; 
	    panelMeshidx = AddMesh(oapiLoadMeshGlobal("ProjectApollo\\Crawler_drivetrucks"), &panelMeshoffset);
		SetMeshVisibilityMode(panelMeshidx, MESHVIS_ALWAYS);
		RESETKEY(kstate, OAPI_KEY_NUMPAD4);
	}
	if (KEYDOWN(kstate, OAPI_KEY_NUMPAD6)) {
		DelMesh(panelMeshidx);
		panelMeshoffset.x -= step; 
	    panelMeshidx = AddMesh(oapiLoadMeshGlobal("ProjectApollo\\Crawler_drivetrucks"), &panelMeshoffset);
		SetMeshVisibilityMode(panelMeshidx, MESHVIS_ALWAYS);
		RESETKEY(kstate, OAPI_KEY_NUMPAD6);
	}
	if (KEYDOWN(kstate, OAPI_KEY_NUMPAD8)) {
		DelMesh(panelMeshidx);
		panelMeshoffset.y += step; 
	    panelMeshidx = AddMesh(oapiLoadMeshGlobal("ProjectApollo\\Crawler_drivetrucks"), &panelMeshoffset);
		SetMeshVisibilityMode(panelMeshidx, MESHVIS_ALWAYS);
		RESETKEY(kstate, OAPI_KEY_NUMPAD8);
	}
	if (KEYDOWN(kstate, OAPI_KEY_NUMPAD2)) {
		DelMesh(panelMeshidx);
		panelMeshoffset.y -= step; 
	    panelMeshidx = AddMesh(oapiLoadMeshGlobal("ProjectApollo\\Crawler_drivetrucks"), &panelMeshoffset);
		SetMeshVisibilityMode(panelMeshidx, MESHVIS_ALWAYS);
		RESETKEY(kstate, OAPI_KEY_NUMPAD2);
	}
	if (KEYDOWN(kstate, OAPI_KEY_NUMPAD1)) {
		DelMesh(panelMeshidx);
		panelMeshoffset.z += step; 
	    panelMeshidx = AddMesh(oapiLoadMeshGlobal("ProjectApollo\\Crawler_drivetrucks"), &panelMeshoffset);
		SetMeshVisibilityMode(panelMeshidx, MESHVIS_ALWAYS);
		RESETKEY(kstate, OAPI_KEY_NUMPAD1);
	}
	if (KEYDOWN(kstate, OAPI_KEY_NUMPAD3)) {
		DelMesh(panelMeshidx);
		panelMeshoffset.z -= step; 
	    panelMeshidx = AddMesh(oapiLoadMeshGlobal("ProjectApollo\\Crawler_drivetrucks"), &panelMeshoffset);
		SetMeshVisibilityMode(panelMeshidx, MESHVIS_ALWAYS);
		RESETKEY(kstate, OAPI_KEY_NUMPAD3);
	}
	sprintf(oapiDebugString(), "panelMeshoffset x %f y %f z %f", panelMeshoffset.x, panelMeshoffset.y, panelMeshoffset.z);
	*/
	return 0;
}

int Crawler::clbkConsumeBufferedKey(DWORD key, bool down, char *kstate) {

	if (!firstTimestepDone) return 0;

	if (KEYMOD_SHIFT(kstate) || KEYMOD_CONTROL(kstate) || !down) {
		return 0; 
	}

	if (key == OAPI_KEY_J) {
		if (IsAttached())
			Detach();
		else
			Attach();
		return 1;
	}

	if (key == OAPI_KEY_1 && down == true) {
		SetView(VIEWPOS_FRONTCABIN);
		return 1;
	}
	if (key == OAPI_KEY_2 && down == true) {
		SetView(VIEWPOS_REARCABIN);
		return 1;
	}
	if (key == OAPI_KEY_3 && down == true) {
		SetView(VIEWPOS_ML);
		return 1;
	}
	if (key == OAPI_KEY_4 && down == true) {
		SetView(VIEWPOS_GROUND);
		return 1;
	}
	if (key == OAPI_KEY_5 && down == true) {
		SetView(VIEWPOS_FRONTGANGWAY);
		return 1;
	}
	if (key == OAPI_KEY_6 && down == true) {
		SetView(VIEWPOS_REARGANGWAY);
		return 1;
	}
	if (key == OAPI_KEY_7 && down == true) {
		SetView(VIEWPOS_RIGHTREARGANGWAY);
		return 1;
	}

	if(key==OAPI_KEY_G) {
		oapiSetShowGrapplePoints(true);
		return 1;
	}

	/*if (key == OAPI_KEY_NUMPAD7 && down == true) {
		if (!standalone) {
			OBJHANDLE hVab = oapiGetVesselByName("VAB");
			if (hVab) {
				VAB *vab = (VAB *) oapiGetVesselInterface(hVab);
				vab->ToggleHighBay1Door();
			}
		}
		return 1;
	}

	if (key == OAPI_KEY_NUMPAD8 && down == true) {
		if (!standalone) {
			OBJHANDLE hVab = oapiGetVesselByName("VAB");
			if (hVab) {
				VAB *vab = (VAB *) oapiGetVesselInterface(hVab);
				vab->ToggleHighBay3Door();
			}
		}
		return 1;
	}*/

	/*if (key == OAPI_KEY_B && down == true) {
		if (!standalone) {
			OBJHANDLE hVab = oapiGetVesselByName("VAB");
			if (hVab) {
				VAB *vab = (VAB *) oapiGetVesselInterface(hVab);
				vab->BuildSaturnStage();
			}
		}
		return 1;
	}

	if (key == OAPI_KEY_U && down == true) {
		if (!standalone) {
			OBJHANDLE hVab = oapiGetVesselByName("VAB");
			if (hVab) {
				VAB *vab = (VAB *) oapiGetVesselInterface(hVab);
				vab->UnbuildSaturnStage();
			}
		}
		return 1;
	} */
	return 0;
}

void Crawler::Attach() {

	if (standalone) return;
	if (velocity != 0) return;
	if (IsAttached()) return;

	VECTOR3 pos, dir, rot;
	VECTOR3 gpos, gattach, horizon_attach;

	Local2Global(MLP_ATTACH_POS, gattach);
	HorizonRot(MLP_ATTACH_POS, horizon_attach);

	for(DWORD i=0; i<oapiGetVesselCount(); i++) {
		OBJHANDLE hV=oapiGetVesselByIndex(i);
		if(hV == GetHandle()) continue;

		VESSEL* pVessel=oapiGetVesselInterface(hV);
		std::string className=pVessel->GetClassName();
		if(className == "Atlantis_MLP") { //found an MLP
			oapiWriteLog("Found MLP");
			ATTACHMENTHANDLE ahAttach=pVessel->GetAttachmentHandle(true, 0);

			//check attachment string
			oapiWriteLog("Testing attachment string");
			oapiWriteLog((char*)pVessel->GetAttachmentId(ahAttach));
			if(_strnicmp(pVessel->GetAttachmentId(ahAttach), "XMLP", 4)) continue;
			oapiWriteLog("Valid attach string");

			pVessel->GetAttachmentParams(ahAttach, pos, dir, rot);
			pVessel->Local2Global(pos, gpos);
			
			if(dist(gpos, gattach) < 5.0) {
				/*VECTOR3 horizon_pos;
				pVessel->HorizonRot(pos, horizon_pos);
				sprintf_s(oapiDebugString(), 255, "VDist: %f attach: %f pos: %f", horizon_attach.y-horizon_pos.y, horizon_attach.y, horizon_pos.y);
				oapiWriteLog(oapiDebugString());*/
				//if(abs(horizon_pos.y-horizon_attach.y) < 0.5) { // make sure crawler has been jacked to correct height
				VECTOR3 gdist = gpos-gattach;
				MATRIX3 RotMatrix;
				GetRotationMatrix(RotMatrix);
				VECTOR3 ldist = tmul(RotMatrix, gdist);
				if(abs(ldist.y)<0.5) { // make sure crawler has been jacked to correct height
					oapiWriteLog("Attaching MLP");
					// if MLP is attached to something else, detach it
					if(pVessel->GetAttachmentStatus(ahAttach)) {
						VESSEL* pParent = oapiGetVesselInterface(pVessel->GetAttachmentStatus(ahAttach));
						for(DWORD j=0;j<pParent->AttachmentCount(false);i++) {
							ATTACHMENTHANDLE ahParent = pParent->GetAttachmentHandle(false, j);
							if(pParent->GetAttachmentStatus(ahParent)==hV) {
								pParent->DetachChild(ahParent);
								break;
							}
						}
					}

					// make sure Crawler attach point has correct rot vector
					double CrawlerHeading, MLPHeading;
					oapiGetHeading(GetHandle(), &CrawlerHeading);
					oapiGetHeading(GetHandle(), &MLPHeading);
					VECTOR3 CrawlerRot=RotateVectorY(MLP_ATTACH_ROT, CrawlerHeading*DEG);
					VECTOR3 MLPRot=RotateVectorY(rot, MLPHeading*DEG);
					if(!bReverseDirection && dotp(MLPRot, CrawlerRot)>=0.0) {
						bReverseDirection=true;
						SetAttachmentParams(ahMLP, MLP_ATTACH_POS, _V(0, -1, 0), -MLP_ATTACH_ROT);
					}
					else if(bReverseDirection && dotp(MLPRot, CrawlerRot)<0.0) {
						bReverseDirection=false;
						SetAttachmentParams(ahMLP, MLP_ATTACH_POS, _V(0, -1, 0), MLP_ATTACH_ROT);
					}

					AttachChild(hV, ahMLP, ahAttach);
					hMLP = hV;
					hMLPAttach = ahAttach;
					break; // get out of for_loop
				}
			}
		}
	}

	//ATTACHMENTHANDLE ah = GetAttachmentHandle(false, 0);
	/*if (hML != NULL) {
		ML *ml = (ML *)oapiGetVesselInterface(hML);

		// Is the crawler close enough to the ML?
		VECTOR3 pos;
		GetRelativePos(hML, pos);
		if (length(pos) < 73) {
			if (ml->Attach()) {
				AttachChild(hML, ah, ml->GetAttachmentHandle(true, 0));
			}
		}
	}*/

	/*if (hMSS != NULL) {
		MSS *mss = (MSS *)oapiGetVesselInterface(hMSS);

		// Is the crawler close enough to the MSS?
		VECTOR3 pos;
		GetRelativePos(hMSS, pos);
		if (length(pos) < 67) {
			if (mss->Attach()) {
				AttachChild(hMSS, ah, mss->GetAttachmentHandle(true, 0));
			}
		}
	}*/
}

void Crawler::Detach() {

	if (standalone) return;
	if (velocity != 0) return;
	if (!IsAttached()) return;

	// loop thorugh all landed vessels and find one within range with MLP attach point
	// release MLP and attach it to vessel
	for(DWORD i=0;i<oapiGetVesselCount();i++) {
		OBJHANDLE hV = oapiGetVesselByIndex(i);
		if(hV == GetHandle()) continue;
		VESSEL* pV = oapiGetVesselInterface(hV);

		// check if vessel is landed
		if((pV->GetFlightStatus() & 1)) {
			//oapiWriteLog(pV->GetName());
			VECTOR3 rpos = CalcRelSurfPos(hV, vs);

			for(DWORD j=0;j<pV->AttachmentCount(false);j++) {
				ATTACHMENTHANDLE aH = pV->GetAttachmentHandle(false, j);
				if(!_strnicmp("XMLP", pV->GetAttachmentId(aH), 4) && !pV->GetAttachmentStatus(aH)) {
					VECTOR3 attach_pos, attach_dir, attach_rot;
					VECTOR3 pt1, pt2, pt3;
					double heading;
					pV->GetAttachmentParams(aH, attach_pos, attach_dir, attach_rot);
					pV->GetTouchdownPoints(pt1, pt2, pt3);
					oapiGetHeading(hV, &heading);

					attach_pos = rpos+RotateVectorY(attach_pos, heading*DEG)-MLP_ATTACH_POS;
					attach_pos.y-=(jackHeight+curFrontHeight+pt1.y); //we can use any TD point; y coordinate should be the same
					sprintf_s(oapiDebugString(), 255, "Attach point: %f %f %f rpos: %f %f %f", attach_pos.x, attach_pos.y, attach_pos.z,
						rpos.x, rpos.y, rpos.z);
					oapiWriteLog(oapiDebugString());

					if(length(_V(attach_pos.x, 0.0, attach_pos.z)) < 4.0 && abs(attach_pos.y) < 0.5) { // attach MLP to VAB/LC39
						DetachChild(ahMLP);
						bool success = pV->AttachChild(hMLP, aH, hMLPAttach);
						hMLP = NULL;
						hMLPAttach = NULL;

						if(!success) oapiWriteLog("ERROR: Could not attach to pad");
					}
				}
			}
		}
	}
}

bool Crawler::IsAttached() {

	if (standalone) return false;

	return (GetAttachmentStatus(ahMLP) != NULL);
}

void Crawler::SetView() {
	SetView(viewPos);
}

void Crawler::SetView(int viewpos) {

	viewPos = viewpos;
	if (viewPos == VIEWPOS_REARCABIN) {
		SetCameraOffset(_V(-15.77, 5.3, -16.934000));
		SetCameraDefaultDirection(_V(0, -0.309017, -0.951057));
		SetMeshesVisibility(MESHVIS_ALWAYS);

	} else if (viewPos == VIEWPOS_FRONTCABIN) {
		SetCameraOffset(_V(15.59, 5.3, 17.094000));
		SetCameraDefaultDirection(_V(0, -0.309017, 0.951057));
		SetMeshesVisibility(MESHVIS_ALWAYS);

	} else if (viewPos == VIEWPOS_ML) {
		SetCameraOffset(_V(19.9, 15.4, -25.6));
		SetCameraDefaultDirection(_V(-0.630037, 0.453991, 0.630037));
		SetMeshesVisibility(MESHVIS_ALWAYS | MESHVIS_EXTPASS);

	} else if (viewPos == VIEWPOS_GROUND) {
		SetCameraOffset(_V(21.5, 1.75, 1));
		SetCameraDefaultDirection(_V(0, 0, 1));
		SetMeshesVisibility(MESHVIS_ALWAYS | MESHVIS_EXTPASS);

	} else if (viewPos == VIEWPOS_FRONTGANGWAY) {
		SetCameraOffset(_V(0, 4.1, 21.3));
		SetCameraDefaultDirection(_V(0, 0, 1));
		SetMeshesVisibility(MESHVIS_ALWAYS);

	} else if (viewPos == VIEWPOS_REARGANGWAY) {
		SetCameraOffset(_V(0, 4.1, -16.234));
		SetCameraDefaultDirection(_V(0, 0, -1));
		SetMeshesVisibility(MESHVIS_ALWAYS);

	} else if (viewPos == VIEWPOS_RIGHTREARGANGWAY) {
		SetCameraOffset(_V(16.4, 5.7, -11.434));
		SetCameraDefaultDirection(_V(0, 0, -1));
		SetMeshesVisibility(MESHVIS_ALWAYS);
	}	
}

void Crawler::SetMeshesVisibility(WORD mode) {

	SetMeshVisibilityMode(meshidxCrawler, mode);
	SetMeshVisibilityMode(meshidxTruck1, mode);
	SetMeshVisibilityMode(meshidxTruck2, mode);
	SetMeshVisibilityMode(meshidxTruck3, mode);
	SetMeshVisibilityMode(meshidxTruck4, mode);
	//SetMeshVisibilityMode(meshidxPanel, mode);
	//SetMeshVisibilityMode(meshidxPanelReverse, mode);

}

void Crawler::SlowIfDesired(double timeAcceleration) {

	if (oapiGetTimeAcceleration() > timeAcceleration) {
		oapiSetTimeAcceleration(timeAcceleration);
	}
}

void Crawler::clbkVisualCreated(VISHANDLE vis, int refcount) {

	vccVis = vis;
}

void Crawler::clbkVisualDestroyed(VISHANDLE vis, int refcount) {
	
	vccVis = NULL;
	// reset the variables keeping track of console mesh animation
	vccSpeed = 0;
	vccSteering = 0;
}

bool Crawler::clbkLoadGenericCockpit() {

	SetView();
	return true;
}

bool Crawler::UpdateTouchdownPoints(const VECTOR3 &relPos)
{
	double front_dist, back_dist;
	double dist=length(relPos);
	double dCos = cos(lastHead);
	//unsigned short usAftIndex; // indicates which tracks are at 'aft' of crawler
	
	front_dist = dist-20.0*abs(dCos);
	back_dist = dist+20.0*abs(dCos);
	/*if(dCos >= 0.0) {
		usAftIndex = 2;
	}
	else {
		usAftIndex = 0;
	}*/

	// ramp to LC39 starts 395m from pad and ends 131.5 m from pad
	if(front_dist < 395.0 && abs(relPos.x)<10.0)
	{
		double front_dist2 = dist-(DRIVETRACK_Z_OFFSET-5.723)*abs(dCos);
		double back_dist2 = dist+(DRIVETRACK_Z_OFFSET-5.723)*abs(dCos);

		double front_height = CalcRampHeight(front_dist);
		double back_height = CalcRampHeight(back_dist);
		double front_height2 = CalcRampHeight(front_dist2);
		double back_height2 = CalcRampHeight(back_dist2);

		double fwdAngle=atan((front_height-front_height2)/(front_dist2-front_dist));
		double backAngle=atan((back_height2-back_height)/(back_dist-back_dist2));
		//double rot_anim_pos = curAngle/(20*RAD); // animation has range of +/10 degress; divide by 20 to get result in correct range
		double fwd_rot_anim_pos = fwdAngle/(20*RAD);
		double back_rot_anim_pos = backAngle/(20*RAD);
		if(dCos<0.0) {
			fwd_rot_anim_pos = -fwd_rot_anim_pos;
			back_rot_anim_pos = -back_rot_anim_pos;
		}

		// needed to prevent treads from appearing to sink into lover segment of ramp
		front_height += range(0.0, (front_dist-131.5)*(0.4/(395.0-131.5)), 0.5);
		back_height += range(0.0, (back_dist-131.5)*(0.4/(395.0-131.5)), 0.5);
		
		//curHeight=front_height;
		//curAngle=atan((front_height-back_height)/40.0);
		curFrontHeight=front_height;
		curBackHeight=back_height2;
		curFrontAngle=fwdAngle;
		curBackAngle=backAngle;

		UpdateTouchdownPoints();
		/*SetTouchdownPoints(_V(0, front_height, 20.0), _V(-10, front_height, -20.0), _V(10, front_height, -20.0));

		for(int i=0;i<2;i++) {
			SetAnimation(anim_truck_rot[i+2-usAftIndex], 0.5+fwd_rot_anim_pos);
			SetAnimation(anim_truck_rot[i+usAftIndex], 0.5+back_rot_anim_pos);
		}
		SetAnimation(anim_truck_trans[usAftIndex], min(JACKING_MAX_HEIGHT, front_height-back_height2)/JACKING_MAX_HEIGHT);
		SetAnimation(anim_truck_trans[usAftIndex+1], min(JACKING_MAX_HEIGHT, front_height-back_height2)/JACKING_MAX_HEIGHT);*/

		//sprintf_s(oapiDebugString(), 255, "dists: %f %f Calc Heights %f Angle: %f %f", front_dist, back_dist, curHeight, curAngle*DEG,  0.5 + curAngle/(20.0*RAD));
		//sprintf_s(oapiDebugString(), 255, "Angles: %f %f", fwdAngle*DEG, backAngle*DEG);
		//sprintf_s(oapiDebugString(), 255, "Heights: Fwd %f %f %f Back %f %f %f Jack: %f", front_height, front_height2, front_height3, back_height, back_height2, back_height3, front_height3-back_height3);
		return true;
	}
	else {
		curFrontHeight=curBackHeight=0.01;
		curFrontAngle=curBackAngle=0.0;
		UpdateTouchdownPoints();
	}
	return false;
}

void Crawler::UpdateTouchdownPoints() const
{
	double dCos = cos(lastHead);
	unsigned short usAftIndex, usFwdIndex; // indicates which tracks are at 'aft' of crawler
	if(dCos >= 0.0) {
		usAftIndex = 2;
		usFwdIndex = 0;
	}
	else {
		usAftIndex = 0;
		usFwdIndex = 2;
	}

	double fwd_rot_anim_pos = curFrontAngle/(20*RAD);
	double back_rot_anim_pos = curBackAngle/(20*RAD);
	if(dCos<0.0) {
		fwd_rot_anim_pos = -fwd_rot_anim_pos;
		back_rot_anim_pos = -back_rot_anim_pos;
	}

	SetTouchdownPoints(_V(0, jackHeight+curFrontHeight, 20.0), _V(-10, jackHeight+curFrontHeight, -20.0), _V(10, jackHeight+curFrontHeight, -20.0));
	for(int i=0;i<2;i++) {
		SetAnimation(anim_truck_rot[i+usFwdIndex], 0.5+fwd_rot_anim_pos);
		SetAnimation(anim_truck_rot[i+usAftIndex], 0.5+back_rot_anim_pos);

		SetAnimation(anim_truck_trans[usAftIndex+i], min(JACKING_MAX_HEIGHT, jackHeight+curFrontHeight-curBackHeight)/JACKING_MAX_HEIGHT);
		SetAnimation(anim_truck_trans[usFwdIndex+i], min(JACKING_MAX_HEIGHT, jackHeight)/JACKING_MAX_HEIGHT);
	}
}

ANIMATIONCOMPONENT_HANDLE Crawler::AddManagedAnimationComponent(UINT anim, double state0, double state1,
		MGROUP_TRANSFORM *trans, ANIMATIONCOMPONENT_HANDLE parent)
{
	/*for(unsigned short i=0;i<vpAnimations.size();i++) {
		if(vpAnimations.at(i) == trans) {
			oapiWriteLog("ERROR: adding animation twice");
			return NULL;
		}
	}*/
	vpAnimations.push_back(trans);
	return AddAnimationComponent(anim, state0, state1, trans, parent);
}

VECTOR3 Crawler::CalcRelSurfPos(OBJHANDLE hVessel, const VESSELSTATUS2& vs) const
{
	double padLat, padLong, padRad;
	oapiGetEquPos(hVessel, &padLong, &padLat, &padRad);
	return _V(padLong*oapiGetSize(hEarth)-vs.surf_lng*oapiGetSize(hEarth), 0.0, padLat*oapiGetSize(hEarth)-vs.surf_lat*oapiGetSize(hEarth));
}