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
char trace_file[] = "ProjectApollo Crawler.log";

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
	
	keyAccelerate = false;
	keyBrake= false;
	keyLeft= false;
	keyRight= false;
	keyCenter = false;

	LVName[0] = 0;
	hMLP = NULL;
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

	VECTOR3 dummy_vec[4];
	MGROUP_ROTATE* Rot_Truck1 = new MGROUP_ROTATE(LOCALVERTEXLIST, MAKEGROUPARRAY(&dummy_vec[0]), 1, _V(0, 0, 0), _V(-1, 0, 0), (float)(20.0*RAD));
	anim_truck_rot[0] = CreateAnimation(0.5);
	ANIMATIONCOMPONENT_HANDLE parent = AddManagedAnimationComponent(anim_truck_rot[0], 0.0, 1.0, Rot_Truck1);
	MGROUP_TRANSLATE* Trans_Truck1 = new MGROUP_TRANSLATE(meshidxTruck1, NULL, 0, _V(0.0, -JACKING_MAX_HEIGHT, 0.0));
	anim_truck_trans[0] = CreateAnimation(0.0);
	AddManagedAnimationComponent(anim_truck_trans[0], 0.0, 1.0, Trans_Truck1, parent);

	MGROUP_ROTATE* Rot_Truck2 = new MGROUP_ROTATE(LOCALVERTEXLIST, MAKEGROUPARRAY(&dummy_vec[1]), 1, _V(0, 0, 0), _V(-1, 0, 0), (float)(20.0*RAD));
	anim_truck_rot[1] = CreateAnimation(0.5);
	AddManagedAnimationComponent(anim_truck_rot[1], 0.0, 1.0, Rot_Truck2);
	MGROUP_TRANSLATE* Trans_Truck2 = new MGROUP_TRANSLATE(meshidxTruck2, NULL, 0, _V(0.0, -JACKING_MAX_HEIGHT, 0.0));
	anim_truck_trans[1] = CreateAnimation(0.0);
	AddManagedAnimationComponent(anim_truck_trans[1], 0.0, 1.0, Trans_Truck2, parent);

	MGROUP_ROTATE* Rot_Truck3 = new MGROUP_ROTATE(LOCALVERTEXLIST, MAKEGROUPARRAY(&dummy_vec[2]), 1, _V(0, 0, 0), _V(-1, 0, 0), (float)(20.0*RAD));
	anim_truck_rot[2] = CreateAnimation(0.5);
	parent = AddManagedAnimationComponent(anim_truck_rot[2], 0.0, 1.0, Rot_Truck3);
	MGROUP_TRANSLATE* Trans_Truck3 = new MGROUP_TRANSLATE(meshidxTruck3, NULL, 0, _V(0.0, -JACKING_MAX_HEIGHT, 0.0));
	anim_truck_trans[2] = CreateAnimation(0.0);
	AddManagedAnimationComponent(anim_truck_trans[2], 0.0, 1.0, Trans_Truck3, parent);

	MGROUP_ROTATE* Rot_Truck4 = new MGROUP_ROTATE(LOCALVERTEXLIST, MAKEGROUPARRAY(&dummy_vec[3]), 1, _V(0, 0, 0), _V(-1, 0, 0), (float)(20.0*RAD));
	anim_truck_rot[3] = CreateAnimation(0.5);
	AddManagedAnimationComponent(anim_truck_rot[3], 0.0, 1.0, Rot_Truck4);
	MGROUP_TRANSLATE* Trans_Truck4 = new MGROUP_TRANSLATE(meshidxTruck4, NULL, 0, _V(0.0, -JACKING_MAX_HEIGHT, 0.0));
	anim_truck_trans[3] = CreateAnimation(0.0);
	AddManagedAnimationComponent(anim_truck_trans[3], 0.0, 1.0, Trans_Truck4, parent);

	// Panel position test
	// panelMeshoffset = meshoffset;
	// panelMeshidx = meshidxTruck3;

	//CreateAttachment(false, _V(0.0, 6.3, 0.0), _V(0, 1, 0), _V(1, 0, 0), "ML", false);
	ahMLP = CreateAttachment(false, MLP_ATTACH_POS, _V(0, -1, 0), _V(1, 0, 0), "XMLP");

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
	VESSELSTATUS2 vs;
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

		double padLat, padLong, padRad;
		oapiGetEquPos(vhLC39[i], &padLong, &padLat, &padRad);
		VECTOR3 rpos = _V(padLat*oapiGetSize(hEarth)-vs.surf_lat*oapiGetSize(hEarth), padLong*oapiGetSize(hEarth)-vs.surf_lng*oapiGetSize(hEarth), 0.0);

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
		} /*else if (!_strnicmp (line, "TARGETHEADING", 13)) {
			sscanf (line + 13, "%lf", &targetHeading);
		}*/ else if (!_strnicmp (line, "WHEELDEFLECT", 12)) {
			sscanf (line + 12, "%lf", &wheeldeflect);
		} else if (!_strnicmp (line, "VIEWPOS", 7)) {
			sscanf (line + 7, "%i", &viewPos);
		} else if (!_strnicmp (line, "STANDALONE", 10)) {
			sscanf (line + 10, "%i", &standalone);
		} else if (!_strnicmp (line, "HEIGHT", 6)) {
			sscanf (line+6, "%lf", &curHeight);
			SetTouchdownPoints(_V(0, curHeight,  10), _V(-10, curHeight, -10), _V(10, curHeight, -10));
		} else if (!_strnicmp (line, "ANGLE", 5)) {
			sscanf (line+5, "%lf", &curAngle);
			curAngle*=RAD; // angle is saved as degrees, so convert back to radians
		} else if (!_strnicmp (line, "GROUND_POS", 10)) {
			sscanf (line + 10, "%lf%lf%lf", &lastLat, &lastLong, &lastHead);
			lastLatLongSet=true;
		} else if (!_strnicmp (line, "LVNAME", 6)) {
			strncpy (LVName, line + 7, 64);
		} else {
			ParseScenarioLineEx (line, status);
		}
	}
}

void Crawler::clbkSaveState(FILEHANDLE scn) {
	
	VESSEL2::clbkSaveState (scn);

	char cbuf[255];

	oapiWriteScenario_float(scn, "VELOCITY", velocity);
	//oapiWriteScenario_float(scn, "TARGETHEADING", targetHeading);
	oapiWriteScenario_float(scn, "WHEELDEFLECT", wheeldeflect);	
	oapiWriteScenario_float(scn, "HEIGHT", curHeight);
	oapiWriteScenario_float(scn, "ANGLE", DEG*curAngle);
	oapiWriteScenario_int(scn, "VIEWPOS", viewPos);
	oapiWriteScenario_int(scn, "STANDALONE", standalone);
	if (LVName[0])
		oapiWriteScenario_string(scn, "LVNAME", LVName);
	sprintf_s(cbuf, 255, "%.10f %.10f %.10f", lastLat, lastLong, lastHead);
	oapiWriteScenario_string(scn, "GROUND_POS", cbuf);
}

int Crawler::clbkConsumeDirectKey(char *kstate) {

	if (!firstTimestepDone) return 0;

	if (KEYMOD_SHIFT(kstate) || KEYMOD_CONTROL(kstate)) {
		return 0; 
	}

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

	if (KEYMOD_SHIFT(kstate) || KEYMOD_CONTROL(kstate)) {
		return 0; 
	}

	if (key == OAPI_KEY_J && down == true) {
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
	VECTOR3 gpos, gattach;

	Local2Global(MLP_ATTACH_POS, gattach);

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
			if(dist(gpos, gattach) < 2.0 || true) {
				oapiWriteLog("Attaching MLP");
				AttachChild(hV, ahMLP, ahAttach);
				hMLP = hV;
				break;
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

	// Is ML attached?
	/*if (hML == GetAttachmentStatus(ahMLP)) {
		ML *ml = (ML *)oapiGetVesselInterface(hML);
		if (ml->Detach()) {
			SlowIfDesired(1.0);
			DetachChild(ah);
		}
	}*/
	
	// Is MSS attached?
	/*if (hMSS == GetAttachmentStatus(ahMLP)) {
		MSS *mss = (MSS *)oapiGetVesselInterface(hMSS);
		if (mss->Detach()) {
			SlowIfDesired(1.0);
			DetachChild(ah);
		}
	}*/
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
		SetCameraOffset(_V(-14.97, 5.3, -14.534));
		SetCameraDefaultDirection(_V(0, -0.309017, -0.951057));
		SetMeshesVisibility(MESHVIS_ALWAYS);

	} else if (viewPos == VIEWPOS_FRONTCABIN) {
		SetCameraOffset(_V(16.5, 5.3, 19.6));
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
	unsigned short usFwdIndex; // indicates which tracks are at 'front' of crawler

	if(dCos >= 0.0) {
		front_dist = dist-20.0*abs(dCos);
		back_dist = dist+20.0*abs(dCos);
		usFwdIndex = 2;
	}
	else {
		front_dist = dist-20.0*abs(dCos);
		back_dist = dist+20.0*abs(dCos);
		usFwdIndex = 0;
	}

	// ramp to LC39 starts 395m from pad and ends 131.5 m from pad
	if(front_dist < 395.0 && abs(relPos.y)<10.0)
	{
		static const double ramp_slope = 15.4 / (395.0-131.5);

		//double height = range(0.0, (395.0-dist)*ramp_slope, 15.4);
		//double height = range(0.0, (395.0-front_dist)*ramp_slope, 15.4);
		double front_height = range(0.0, (395.0-front_dist)*ramp_slope, 15.4);
		double back_height = range(0.0, (395.0-back_dist)*ramp_slope, 15.4);

		// needed to prevent treads from appearing to sink into lover segment of ramp
		front_height += range(0.0, (front_dist-131.5)*(0.4/(395.0-131.5)), 0.5);
		back_height += range(0.0, (back_dist-131.5)*(0.4/(395.0-131.5)), 0.5);
		
		curHeight=front_height;
		curAngle=atan((front_height-back_height)/40.0);

		//SetTouchdownPoints(_V(0, back_height, 20.0), _V(-10, front_height, -20.0), _V(10, front_height, -20.0));
		SetTouchdownPoints(_V(0, front_height, 20.0), _V(-10, front_height, -20.0), _V(10, front_height, -20.0));

		double rot_anim_pos = curAngle/(20*RAD); // animation has range of +/10 degress; divide by 20 to get result in correct range
		if(usFwdIndex==0) rot_anim_pos = -rot_anim_pos;
		for(int i=0;i<4;i++) SetAnimation(anim_truck_rot[i], 0.5 + rot_anim_pos);
		SetAnimation(anim_truck_trans[usFwdIndex], min(JACKING_MAX_HEIGHT, front_height-back_height));
		SetAnimation(anim_truck_trans[usFwdIndex+1], min(JACKING_MAX_HEIGHT, front_height-back_height));

		//sprintf_s(oapiDebugString(), 255, "dists: %f %f Calc Heights %f Angle: %f %f", front_dist, back_dist, curHeight, curAngle*DEG,  0.5 + curAngle/(20.0*RAD));
		return true;
	}
	else {
		SetTouchdownPoints(_V(0, 0.01, 20.0), _V(-10, 0.01, -20.0), _V(10, 0.01, -20.0));
		for(int i=0;i<4;i++) {
			SetAnimation(anim_truck_rot[i], 0.5);
			SetAnimation(anim_truck_trans[i], 0.0);
		}
		curHeight=0.01;
	}
	return false;
}

ANIMATIONCOMPONENT_HANDLE Crawler::AddManagedAnimationComponent(UINT anim, double state0, double state1,
		MGROUP_TRANSFORM *trans, ANIMATIONCOMPONENT_HANDLE parent)
{
	vpAnimations.push_back(trans);
	return AddAnimationComponent(anim, state0, state1, trans, parent);
}