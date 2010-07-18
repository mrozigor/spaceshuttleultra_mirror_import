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
#include <OrbiterSoundSDK35.h>
#include "../resource_Crawler.h"

#include "../SSUMath.h"
#include "Crawler.h"
#include "CrawlerCenterPanel.h"
#include "CrawlerRightPanel.h"
#include "meshres_crawler.h"

//HINSTANCE g_hDLL;
GlobalHandles g_Resources;

DLLCLBK void InitModule(HINSTANCE hModule) {

	//g_hDLL = hModule;
	g_Resources.hDll = hModule;
	HBITMAP hBmp = LoadBitmap(g_Resources.hDll, MAKEINTRESOURCE(IDB_PBILABELS));
	g_Resources.pbi_lights = oapiCreateSurface(hBmp);
	hBmp = LoadBitmap(g_Resources.hDll, MAKEINTRESOURCE(IDB_7SEGDIGITS));
	g_Resources.digits_7seg = oapiCreateSurface(hBmp);
}

DLLCLBK void ExitModule (HINSTANCE hModule)
{
	if(g_Resources.pbi_lights)
		oapiDestroySurface(g_Resources.pbi_lights);
	if(g_Resources.digits_7seg)
		oapiDestroySurface(g_Resources.digits_7seg);
}

DLLCLBK VESSEL *ovcInit(OBJHANDLE hvessel, int flightmodel) {

	return new Crawler(hvessel, flightmodel);
}

DLLCLBK void ovcExit(VESSEL *vessel) {

	if (vessel) delete (Crawler*)vessel;
}

Crawler::Crawler(OBJHANDLE hObj, int fmodel)
: VESSEL2 (hObj, fmodel)
{
	pBundleManager = new DiscreteBundleManager();
	psubsystems = new SubsystemDirector<Crawler>(this);
	psubsystems->AddSubsystem(pEngine = new CrawlerEngine(psubsystems));

	pgFwdCab.AddPanel(new vc::CrawlerCenterPanel(this, "FWD_CAB_CTR", vc::FWD));
	pgFwdCab.AddPanel(new vc::CrawlerRightPanel(this, "FWD_CAB_RIGHT", vc::FWD));
	pgRearCab.AddPanel(new vc::CrawlerCenterPanel(this, "REAR_CAB", vc::REAR));
	pgRearCab.AddPanel(new vc::CrawlerRightPanel(this, "REAR_CAB_RIGHT", vc::REAR));

	//tgtVelocity = 0.0;
	//velocity = 0;
	currentSpeed = 0.0;
	velocityStop = false;
	steeringActual[0] = steeringActual[1] = 0;
	steeringCommanded[0] = steeringCommanded[1] = 0;
	viewPos = VIEWPOS_FRONTCABIN;
	firstTimestepDone = false;
	standalone = false;

	lastLatLongSet = false;
	lastLat = 0;
	lastLong = 0;

	curFrontAngle = 0.0;
	curBackAngle = 0.0;
	curFrontHeight = 0.01;
	curBackHeight = 0.01;
	jackHeight = 0.0;

	bReverseDirection=false;
	
	//keyAccelerate = false;
	//keyBrake= false;
	keyLeft= false;
	keyRight= false;
	keyCenter = false;

	hMLP = NULL;
	hMLPAttach = NULL;
	vccVis = NULL;	
	vccSpeed = 0;	
	vccSteering = 0;
	meshidxCrawler = 0;
	meshidxTruck1 = 0;
	meshidxTruck2 = 0;
	meshidxTruck3 = 0;
	meshidxTruck4 = 0;
	hFwdVCMesh = NULL;
	hRearVCMesh = NULL;

	hEarth = NULL;

	//soundlib.InitSoundLib(hObj, SOUND_DIRECTORY);
	//soundlib.LoadSound(soundEngine, "CrawlerEngine.wav", BOTHVIEW_FADED_MEDIUM);

	SoundID = ConnectToOrbiterSoundDLL3(GetHandle());
	RequestLoadVesselWave3(SoundID, ENGINE_SOUND_ID, "Sound\\ShuttleUltra\\CrawlerEngine.wav", BOTHVIEW_FADED_MEDIUM);
}

Crawler::~Crawler() {
	// delete MGROUP_TRANSFORMs
	for(unsigned short i=0;i<vpAnimations.size();i++) delete vpAnimations.at(i);

	delete psubsystems;
	delete pBundleManager;
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

	// Crawler
	//meshoffset = _V(0.767, 3.387, 2.534);
	VECTOR3 crawler_meshoffset = CRAWLER_MESH_OFFSET;
    meshidxCrawler = AddMesh(oapiLoadMeshGlobal(CRAWLER_MESHNAME), &crawler_meshoffset);
    //meshidxCrawler = AddMesh(oapiLoadMeshGlobal(CRAWLER_MESHNAME_1980), &crawler_meshoffset);
	SetMeshVisibilityMode(meshidxCrawler, MESHVIS_ALWAYS);

	// Tracks
	MESHHANDLE track = oapiLoadMeshGlobal("SSU\\Crawler_drivetrucks");
	
	//meshoffset = _V(14.539, 1.765, 15.512);
	VECTOR3 meshoffset = _V(FWD_DRIVETRACK_X_OFFSET, DRIVETRACK_Y_OFFSET, DRIVETRACK_Z_OFFSET);
    meshidxTruck1 = AddMesh(track, &meshoffset);
	SetMeshVisibilityMode(meshidxTruck1, MESHVIS_ALWAYS);

	//meshoffset = _V(-12.957, 1.765, 15.512);
	meshoffset = _V(REAR_DRIVETRACK_X_OFFSET, DRIVETRACK_Y_OFFSET, DRIVETRACK_Z_OFFSET);
    meshidxTruck2 = AddMesh(track, &meshoffset);
	SetMeshVisibilityMode(meshidxTruck2, MESHVIS_ALWAYS);

	//meshoffset = _V(14.539, 1.765, -10.359);
	meshoffset = _V(FWD_DRIVETRACK_X_OFFSET, DRIVETRACK_Y_OFFSET, -DRIVETRACK_Z_OFFSET);
    meshidxTruck3 = AddMesh(track, &meshoffset);
	SetMeshVisibilityMode(meshidxTruck3, MESHVIS_ALWAYS);

	//meshoffset = _V(-12.957, 1.765, -10.359);
	meshoffset = _V(REAR_DRIVETRACK_X_OFFSET, DRIVETRACK_Y_OFFSET, -DRIVETRACK_Z_OFFSET);
    meshidxTruck4 = AddMesh(track, &meshoffset);
	SetMeshVisibilityMode(meshidxTruck4, MESHVIS_ALWAYS);

	/*VECTOR3 crawler_vc_offset = CRAWLER_VC_OFFSET;
	meshidxVC = AddMesh(oapiLoadMeshGlobal("SSU\\Crawler_VC_panels"), &crawler_vc_offset);
	//SetMeshVisibilityMode(meshidxVC, MESHVIS_VC | MESHVIS_EXTERNAL | MESHVIS_EXTPASS);
	SetMeshVisibilityMode(meshidxVC, MESHVIS_ALWAYS);*/

	VECTOR3 crawler_vc_offset = CRAWLER_FWD_VC_OFFSET;
	hFwdVCMesh = oapiLoadMeshGlobal("SSU\\Crawler_VC_panels_reversed");
	fwdVCIdx = AddMesh(hFwdVCMesh, &crawler_vc_offset);
	SetMeshVisibilityMode(fwdVCIdx, MESHVIS_COCKPIT | MESHVIS_VC | MESHVIS_EXTERNAL);
	crawler_vc_offset = CRAWLER_REAR_VC_OFFSET;
	hRearVCMesh = oapiLoadMeshGlobal("SSU\\Crawler_VC_panels");
	rearVCIdx = AddMesh(hRearVCMesh, &crawler_vc_offset);
	SetMeshVisibilityMode(rearVCIdx, MESHVIS_COCKPIT | MESHVIS_VC | MESHVIS_EXTERNAL);

	DefineAnimations();

	//CreateAttachment(false, _V(0.0, 6.3, 0.0), _V(0, 1, 0), _V(1, 0, 0), "ML", false);
	ahMLP = CreateAttachment(false, MLP_ATTACH_POS, _V(0, -1, 0), MLP_ATTACH_ROT, "XMLP");

	//VSEnableCollisions(GetHandle(),"ProjectApollo");
	//double tph = -0.01;
	//SetTouchdownPoints(_V(  0, tph,  10), _V(-10, tph, -10), _V( 10, tph, -10));
	//VSSetTouchdownPoints(GetHandle(), _V(  0, tph,  10), _V(-10, tph, -10), _V( 10, tph, -10));
	SetTouchdownPoints(_V(  0, 0.01,  10), _V(-10, 0.01, -10), _V( 10, 0.01, -10));
	//ShiftCG(_V(0, -16, 0));

	psubsystems->SetClassCaps(cfg);
	pgFwdCab.DefineVC();
	pgFwdCab.DefineVCAnimations(fwdVCIdx);
	pgRearCab.DefineVC();
	pgRearCab.DefineVCAnimations(rearVCIdx);
}

void Crawler::DefineAnimations()
{
	const float STRUT_ROTATION_ANGLE = static_cast<float>(22.5*RAD);
	const VECTOR3 STRUT_SCALE_VECTOR = _V(1.45, 1.0, 1.0);

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
	static UINT Strut11GrpList[2] = {GRP_CORNERDC_FWD_STEERING_CYLINDER, GRP_CORNER_D_FWD_STEERING_CYLINDER_DUST_SHIELD};
	MGROUP_ROTATE* Rotate_Strut11 = new MGROUP_ROTATE(meshidxCrawler, &Strut11GrpList[0], 1, _V(8.289, 2.394, 18.126), _V(0, 0, -1), STRUT_ROTATION_ANGLE);
	parent = AddManagedAnimationComponent(anim_truck_trans[0], 0.0, 1.0, Rotate_Strut11);
	MGROUP_SCALE* Scale_Strut11 = new MGROUP_SCALE(meshidxCrawler, &Strut11GrpList[1], 1, _V(12.447, 2.905, 18.126), STRUT_SCALE_VECTOR);
	AddManagedAnimationComponent(anim_truck_trans[0], 0.0, 1.0, Scale_Strut11, parent);
	static UINT Strut12GrpList[2] = {GRP_CORNER_D_REAR_STEERING_CYLINDER, GRP_CORNER_D_REAR_STEERING_CYLINDER_DUST_SHIELD};
	MGROUP_ROTATE* Rotate_Strut12 = new MGROUP_ROTATE(meshidxCrawler, &Strut12GrpList[0], 1, _V(8.286, 2.394, 8.279), _V(0, 0, -1), STRUT_ROTATION_ANGLE);
	parent = AddManagedAnimationComponent(anim_truck_trans[0], 0.0, 1.0, Rotate_Strut12);
	MGROUP_SCALE* Scale_Strut12 = new MGROUP_SCALE(meshidxCrawler, &Strut12GrpList[1], 1, _V(12.447, 2.905, 8.279), STRUT_SCALE_VECTOR);
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
	static UINT Strut21GrpList[2] = {GRP_CORNER_C_FWD_STEERING_CYLINDER, GRP_CORNER_C_FWD_STEERING_CYLINDER_DUST_SHIELDA};
	MGROUP_ROTATE* Rotate_Strut21 = new MGROUP_ROTATE(meshidxCrawler, &Strut21GrpList[0], 1, _V(-8.307, 2.414, 18.125), _V(0, 0, 1), STRUT_ROTATION_ANGLE);
	parent = AddManagedAnimationComponent(anim_truck_trans[1], 0.0, 1.0, Rotate_Strut21);
	MGROUP_SCALE* Scale_Strut21 = new MGROUP_SCALE(meshidxCrawler, &Strut21GrpList[1], 1, _V(-12.465, 2.924, 18.125), STRUT_SCALE_VECTOR);
	AddManagedAnimationComponent(anim_truck_trans[1], 0.0, 1.0, Scale_Strut21, parent);
	static UINT Strut22GrpList[2] = {GRP_CORNER_C_REAR_STEERING_CYLINDER, GRP_CORNER_C_FWD_STEERING_CYLINDER_DUST_SHIELD};
	MGROUP_ROTATE* Rotate_Strut22 = new MGROUP_ROTATE(meshidxCrawler, &Strut22GrpList[0], 1, _V(-8.307, 2.414, 8.275), _V(0, 0, 1), STRUT_ROTATION_ANGLE);
	parent = AddManagedAnimationComponent(anim_truck_trans[1], 0.0, 1.0, Rotate_Strut22);
	MGROUP_SCALE* Scale_Strut22 = new MGROUP_SCALE(meshidxCrawler, &Strut22GrpList[1], 1, _V(-12.465, 2.924, 8.275), STRUT_SCALE_VECTOR);
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
	static UINT Strut31GrpList[2] = {GRP_CORNER_A_FWD_STEERING_CYLINDER, GRP_CORNER_A_FWD_STEERING_CYLINDER_DUST_SHIELD};
	MGROUP_ROTATE* Rotate_Strut31 = new MGROUP_ROTATE(meshidxCrawler, &Strut31GrpList[0], 1, _V(8.289, 2.396, -18.181), _V(0, 0, -1), STRUT_ROTATION_ANGLE);
	parent = AddManagedAnimationComponent(anim_truck_trans[2], 0.0, 1.0, Rotate_Strut31);
	MGROUP_SCALE* Scale_Strut31 = new MGROUP_SCALE(meshidxCrawler, &Strut31GrpList[1], 1, _V(12.449, 2.908, -18.181), STRUT_SCALE_VECTOR);
	AddManagedAnimationComponent(anim_truck_trans[2], 0.0, 1.0, Scale_Strut31, parent);
	static UINT Strut32GrpList[2] = {GRP_CORNER_A_REAR_STEERING_CYLINDER, GRP_CORNER_A_REAR_STEERING_CYLINDER_DUST_SHIELD};
	MGROUP_ROTATE* Rotate_Strut32 = new MGROUP_ROTATE(meshidxCrawler, &Strut32GrpList[0], 1, _V(8.289, 2.396, -8.333), _V(0, 0, -1), STRUT_ROTATION_ANGLE);
	parent = AddManagedAnimationComponent(anim_truck_trans[2], 0.0, 1.0, Rotate_Strut32);
	MGROUP_SCALE* Scale_Strut32 = new MGROUP_SCALE(meshidxCrawler, &Strut32GrpList[1], 1, _V(12.449, 2.908, -8.333), STRUT_SCALE_VECTOR);
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
	static UINT Strut41GrpList[2] = {GRP_CORNER_B_FWD_STEERING_CYLINDER, GRP_CORNER_B_FWD_STEERING_CYLINDER_DUST_SHIELD};
	MGROUP_ROTATE* Rotate_Strut41 = new MGROUP_ROTATE(meshidxCrawler, &Strut41GrpList[0], 1, _V(-8.307, 2.414, -18.191), _V(0, 0, 1), STRUT_ROTATION_ANGLE);
	parent = AddManagedAnimationComponent(anim_truck_trans[3], 0.0, 1.0, Rotate_Strut41);
	MGROUP_SCALE* Scale_Strut41 = new MGROUP_SCALE(meshidxCrawler, &Strut41GrpList[1], 1, _V(-12.465, 2.924, -18.191), STRUT_SCALE_VECTOR);
	AddManagedAnimationComponent(anim_truck_trans[3], 0.0, 1.0, Scale_Strut41, parent);
	static UINT Strut42GrpList[2] = {GRP_CORNER_B_REAR_STEERING_CYLINDER, GRP_CORNER_B_REAR_STEERING_CYLINDER_DUST_SHIELD};
	MGROUP_ROTATE* Rotate_Strut42 = new MGROUP_ROTATE(meshidxCrawler, &Strut42GrpList[0], 1, _V(-8.307, 2.414, -8.343), _V(0, 0, 1), STRUT_ROTATION_ANGLE);
	parent = AddManagedAnimationComponent(anim_truck_trans[3], 0.0, 1.0, Rotate_Strut42);
	MGROUP_SCALE* Scale_Strut42 = new MGROUP_SCALE(meshidxCrawler, &Strut42GrpList[1], 1, _V(-12.465, 2.924, -8.343), STRUT_SCALE_VECTOR);
	AddManagedAnimationComponent(anim_truck_trans[3], 0.0, 1.0, Scale_Strut42, parent);
}

void Crawler::clbkPostCreation()
{
	psubsystems->RealizeAll();
	pgFwdCab.Realize();
	pgRearCab.Realize();

	// connect DiscPorts
	DiscreteBundle* pBundle = pBundleManager->CreateBundle("CRAWLER_STEERING", 7);
	port_steeringCommand[0].Connect(pBundle, 0);
	port_steeringCommand[1].Connect(pBundle, 1);
	greatCircle.Connect(pBundle, 2);
	crab.Connect(pBundle, 3);
	independent.Connect(pBundle, 4);
	port_steeringActual[0].Connect(pBundle, 5);
	port_steeringActual[1].Connect(pBundle, 6);

	port_steeringCommand[0].SetLine(static_cast<float>(steeringCommanded[0]));
	port_steeringCommand[1].SetLine(static_cast<float>(steeringCommanded[1]));
	port_steeringActual[0].SetLine(static_cast<float>(steeringActual[0]));
	port_steeringActual[1].SetLine(static_cast<float>(steeringActual[1]));

	pBundle = pBundleManager->CreateBundle("CRAWLER_BRAKE", 2);
	port_Brake.Connect(pBundle, 0);
	port_BrakeSet.Connect(pBundle, 0);
	port_ParkingBrakeSet.Connect(pBundle, 1);

	port_Brake.ResetLine();

	pBundle = pBundleManager->CreateBundle("CRAWLER_SPEED", 2);
	port_currentSpeed.Connect(pBundle, 0);
	port_currentSpeed.SetLine(static_cast<float>(currentSpeed*MPS2MPH));
}

void Crawler::clbkPreStep(double simt, double simdt, double mjd) {

	//double maxVelocity = 0.894;

	if (!firstTimestepDone) DoFirstTimestep();

	psubsystems->PreStep(simt, simdt, mjd);
	pgFwdCab.OnPreStep(simt, simdt, mjd);
	pgRearCab.OnPreStep(simt, simdt, mjd);

	// control crawler speed
	if(IsAttached()) pEngine->SetMaxSpeed(MAX_LOADED_SPEED);
	else pEngine->SetMaxSpeed(MAX_UNLOADED_SPEED);
	//pEngine->OnPreStep(simt, simdt, mjd);

	//if (IsAttached()) maxVelocity = maxVelocity / 2.0;

	// update speed
	double acceleration = pEngine->GetAcceleration();
	if(!Eq(currentSpeed, 0.0, 0.01) || !Eq(acceleration, 0.0)) {
		// simulate friction and brakes
		if(!Eq(currentSpeed, 0.0)) {
			double friction = 0.0025;
			if(port_BrakeSet) friction += 0.005; // brakes
			if(port_ParkingBrakeSet) friction += 0.01;
			
			if(currentSpeed >= 0.0) acceleration = acceleration-friction;
			else acceleration = acceleration+friction;
		}
		// calculate new speed
		currentSpeed+=acceleration*simdt;
	}
	else currentSpeed = 0.0;
	port_currentSpeed.SetLine(static_cast<float>(currentSpeed));
	
	double timeW = oapiGetTimeAcceleration();

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

	if(keyRight && (viewPos==VIEWPOS_FRONTCABIN || viewPos==VIEWPOS_REARCABIN)) {
		double dAngle = 0.1 * simdt;
		if((steeringCommanded[viewPos]-dAngle-steeringActual[viewPos]) > (-1/(MAX_TURN_ANGLE*DEG))) {
			steeringCommanded[viewPos] = max(-1,steeringCommanded[viewPos] - dAngle);
			if(greatCircle) steeringCommanded[1-viewPos] = -steeringCommanded[viewPos];
			else if(crab) steeringCommanded[1-viewPos] = steeringCommanded[viewPos];
			port_steeringCommand[0].SetLine(static_cast<float>(steeringCommanded[0]));
			port_steeringCommand[1].SetLine(static_cast<float>(steeringCommanded[1]));
		}	
	} else if(keyLeft && (viewPos==VIEWPOS_FRONTCABIN || viewPos==VIEWPOS_REARCABIN)) {
		double dAngle = 0.1 * simdt;
		if((steeringActual[viewPos]-steeringCommanded[viewPos]-dAngle) > (-1/(MAX_TURN_ANGLE*DEG))) {
			steeringCommanded[viewPos] = min(1, steeringCommanded[viewPos] + dAngle);
			if(greatCircle) steeringCommanded[1-viewPos] = -steeringCommanded[viewPos];
			else if(crab) steeringCommanded[1-viewPos] = steeringCommanded[viewPos];
			port_steeringCommand[0].SetLine(static_cast<float>(steeringCommanded[0]));
			port_steeringCommand[1].SetLine(static_cast<float>(steeringCommanded[1]));
		}
	} else if (keyCenter) {
		for(unsigned short i=0;i<2;i++) {
			steeringCommanded[i] = 0;
			port_steeringCommand[i].ResetLine();
		}
	}
	keyLeft = false;
	keyRight = false;
	keyCenter = false;
	
	for(unsigned short i=0;i<2;i++) {
		double steeringError = steeringCommanded[i]-steeringActual[i];
		if(steeringError < -0.0001) {
			double dAngle = 0.05*simdt;
			steeringActual[i] = max(steeringCommanded[i], steeringActual[i]-dAngle);
			port_steeringActual[i].SetLine(static_cast<float>(steeringActual[i]));
		}
		else if(steeringError > 0.0001) {			
			double dAngle = 0.05*simdt;
			steeringActual[i] = min(steeringCommanded[i], steeringActual[i]+dAngle);
			port_steeringActual[i].SetLine(static_cast<float>(steeringActual[i]));
		}
	}

	const double r = 45; // 150 ft
	double dheading = ((steeringActual[1]-steeringActual[0]) * currentSpeed * simdt) / (2*r);
	// turn speed is only doubled when time acc is multiplied by ten:
	dheading = (pow(5.0, log10(timeW))) * dheading / timeW;
	vs.surf_hdg += dheading;
	if(vs.surf_hdg< 0) vs.surf_hdg += 2.0 * PI;
	if(vs.surf_hdg >= 2.0 * PI) vs.surf_hdg -= 2.0 * PI;

	double headingOffset = -(steeringActual[1]+steeringActual[0]) * MAX_TURN_ANGLE/2.0;
	VECTOR3 dir = _V(sin(lastHead+headingOffset), cos(lat) * cos(lastHead+headingOffset), 0);
	dir /= length(dir);
	lon += dir.x * currentSpeed * simdt / oapiGetSize(hEarth);
	lat += dir.y * currentSpeed * simdt / oapiGetSize(hEarth);
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
	if (currentSpeed != 0) {
		// velocity dependent sound disabled
		// soundEngine.play(LOOP, (int)(127.5 + 127.5 * velocity / maxVelocity));
		//soundEngine.play();
		PlayVesselWave3(SoundID, ENGINE_SOUND_ID, LOOP);
	} else {
		//soundEngine.stop();
		StopVesselWave3(SoundID, ENGINE_SOUND_ID);
	}
}

void Crawler::clbkPostStep(double simt, double simdt, double mjd) {
	psubsystems->PostStep(simt, simdt, mjd);
	pgFwdCab.OnPostStep(simt, simdt, mjd);
	pgRearCab.OnPostStep(simt, simdt, mjd);
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
		if (!_strnicmp (line, "JACK_HEIGHT", 11)) {
			sscanf (line + 11, "%lf", &jackHeight);
		} else if (!_strnicmp (line, "VELOCITY", 8)) {
			sscanf_s (line + 8, "%lf", &currentSpeed);
		} else if (!_strnicmp (line, "STEERING_ACTUAL", 15)) {
			sscanf (line + 15, "%lf%lf", &steeringActual[0], &steeringActual[1]);
		} else if (!_strnicmp (line, "STEERING_COMMANDED", 18)) {
			sscanf (line + 18, "%lf%lf", &steeringCommanded[0], &steeringCommanded[1]);
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
		} else if (!_strnicmp(line, "@PANEL", 6)) {
			char pszPanelName[30];
			sscanf_s(line+6, "%s", pszPanelName, sizeof(pszPanelName));
			oapiWriteLog(pszPanelName);
			//bool bFound = false;

			if(pgFwdCab.HasPanel(pszPanelName))
				pgFwdCab.ParsePanelBlock(pszPanelName, scn);
			else if(pgRearCab.HasPanel(pszPanelName))
				pgRearCab.ParsePanelBlock(pszPanelName, scn);

			oapiWriteLog("\tLeave @PANEL block.");
		} else {
			//if(!pEngine->ParseLine(line))
			if(!psubsystems->ParseScenarioLine(scn, line))
				ParseScenarioLineEx (line, status);
		}
	}

	UpdateTouchdownPoints();
}

void Crawler::clbkSaveState(FILEHANDLE scn)
{	
	VESSEL2::clbkSaveState(scn);

	char cbuf[255];

	//oapiWriteScenario_float(scn, "VELOCITY", velocity);
	oapiWriteScenario_float(scn, "VELOCITY", currentSpeed);
	//oapiWriteScenario_float(scn, "WHEELDEFLECT", wheeldeflect);
	sprintf_s(cbuf, 255, "%f %f", steeringActual[0], steeringActual[1]);
	oapiWriteScenario_string(scn, "STEERING_ACTUAL", cbuf);	
	sprintf_s(cbuf, 255, "%f %f", steeringCommanded[0], steeringCommanded[1]);
	oapiWriteScenario_string(scn, "STEERING_COMMANDED", cbuf);

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
	//pEngine->SaveState(scn);
	psubsystems->SaveState(scn);
	pgFwdCab.OnSaveState(scn);
	pgRearCab.OnSaveState(scn);
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
		/*if (KEYDOWN(kstate, OAPI_KEY_ADD)) {
			keyAccelerate = true;				
			RESETKEY(kstate, OAPI_KEY_ADD);
		}
		if (KEYDOWN(kstate, OAPI_KEY_SUBTRACT)) {
			keyBrake = true;				
			RESETKEY(kstate, OAPI_KEY_SUBTRACT);
		}*/
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
	return 0;
}

int Crawler::clbkConsumeBufferedKey(DWORD key, bool down, char *kstate) {

	if (!firstTimestepDone) return 0;

	if(KEYMOD_CONTROL(kstate)) {
		if(!down && key == OAPI_KEY_3) {
			pgFwdCab.ToggleCoordinateDisplayMode();
			pgRearCab.ToggleCoordinateDisplayMode();
			oapiWriteLog("Toggling coordinate display mode");
			return 1;
		}
		else return 0;
	}

	if (KEYMOD_SHIFT(kstate)) {
		return 0; 
	}

	if (key == OAPI_KEY_ADD) {
		if(viewPos == VIEWPOS_FRONTCABIN) pEngine->IncreaseTgtSpeed(down);
		else if(viewPos == VIEWPOS_REARCABIN) pEngine->DecreaseTgtSpeed(down);
		return 1;
	}
	else if (key == OAPI_KEY_SUBTRACT) {
		if(viewPos == VIEWPOS_FRONTCABIN) pEngine->DecreaseTgtSpeed(down);
		else if(viewPos == VIEWPOS_REARCABIN) pEngine->IncreaseTgtSpeed(down);
		return 1;
	}
	else if (key == OAPI_KEY_B) {
		if(down) port_Brake.SetLine();
		else port_Brake.ResetLine();
		return 1;
	}

	if(down) {
		if (key == OAPI_KEY_J) {
			if (IsAttached())
				Detach();
			else
				Attach();
			return 1;
		}

		if (key == OAPI_KEY_1) {
			SetView(VIEWPOS_FRONTCABIN);
			return 1;
		}
		if (key == OAPI_KEY_2) {
			SetView(VIEWPOS_REARCABIN);
			return 1;
		}
		if (key == OAPI_KEY_3) {
			SetView(VIEWPOS_ML);
			return 1;
		}
		if (key == OAPI_KEY_4) {
			SetView(VIEWPOS_GROUND);
			return 1;
		}
		if (key == OAPI_KEY_5) {
			SetView(VIEWPOS_FRONTGANGWAY);
			return 1;
		}
		if (key == OAPI_KEY_6) {
			SetView(VIEWPOS_REARGANGWAY);
			return 1;
		}
		if (key == OAPI_KEY_7) {
			SetView(VIEWPOS_RIGHTREARGANGWAY);
			return 1;
		}

		if(key==OAPI_KEY_G) {
			oapiSetShowGrapplePoints(true);
			return 1;
		}
	}
	return 0;
}

void Crawler::Attach() {

	if (standalone) return;
	//if (!Eq(pEngine->GetSpeed(), 0.0)) return;
	if (!Eq(currentSpeed, 0.0)) return;
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
						for(DWORD j=0;j<pParent->AttachmentCount(false);j++) {
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
	if (!Eq(currentSpeed, 0.0)) return;
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

					attach_pos = rpos-RotateVectorY(attach_pos, heading*DEG)+MLP_ATTACH_POS;
					attach_pos.y+=(jackHeight+curFrontHeight+pt1.y); //we can use any TD point; y coordinate should be the same
					sprintf_s(oapiDebugString(), 255, "Attach point: %f %f %f rpos: %f %f %f", attach_pos.x, attach_pos.y, attach_pos.z,
						rpos.x, rpos.y, rpos.z);
					oapiWriteLog(oapiDebugString());

					if(length(_V(attach_pos.x, 0.0, attach_pos.z)) < 5.0 && abs(attach_pos.y) < 0.25) { // attach MLP to VAB/LC39
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
		//SetCameraOffset(_V(-15.19, 5, -17.774000));
		SetCameraOffset(CRAWLER_REAR_VC_OFFSET + _V(0.0, 0.456, 0.576));
		SetCameraShiftRange(_V(0.0, 0.2, -0.3), _V(0, 0, 0), _V(0, 0, 0));
		SetCameraDefaultDirection(_V(0, -0.309017, -0.951057));
		SetMeshesVisibility(MESHVIS_ALWAYS);

	} else if (viewPos == VIEWPOS_FRONTCABIN) {
		//SetCameraOffset(_V(15.19, 5, 17.774000));
		SetCameraOffset(CRAWLER_FWD_VC_OFFSET + _V(0.0, 0.456, -0.576));
		SetCameraShiftRange(_V(0.0, 0.2, 0.3), _V(0, 0, 0), _V(0, 0, 0));
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

bool Crawler::clbkLoadVC (int id)
{
	switch(id) {
		case VIEWPOS_FRONTCABIN:
		case VIEWPOS_REARCABIN:
			oapiWriteLog("Showing VC");
			//cabs[id]->RegisterVC();
			if(id==VIEWPOS_FRONTCABIN) pgFwdCab.RegisterVC();
			else pgRearCab.RegisterVC();
			oapiVCSetNeighbours(1-id, 1-id, -1, -1); // left, right neighbours are other panels
			SetView(id);
			return true;
	}
	return false;
}

bool Crawler::clbkVCMouseEvent(int id, int _event, VECTOR3& p)
{
	oapiWriteLog("clbkVCMouseEvent called");
	bool bRet = false;

	bRet = pgFwdCab.OnVCMouseEvent(id, _event, p);
	bRet = pgRearCab.OnVCMouseEvent(id, _event, p);

	return bRet;
}

bool Crawler::clbkVCRedrawEvent(int id, int _event, SURFHANDLE surf)
{
	sprintf_s(oapiDebugString(), 255, "VC Redraw event: %d", id);
	if(pgFwdCab.OnVCRedrawEvent(id, _event, surf))
		return true;
	if(pgRearCab.OnVCRedrawEvent(id, _event, surf))
		return true;
	
	return false;
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

DiscreteBundleManager* Crawler::BundleManager() const
{
	return pBundleManager;
}

MESHHANDLE Crawler::GetVCMesh(vc::CRAWLER_CAB cab) const
{
	if(cab==vc::FWD) return hFwdVCMesh;
	else return hRearVCMesh;
}