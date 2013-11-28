#include "Crawler.h"
#include "meshres_crawler_1980.h"

void Crawler::Define1980StrutAnimations()
{
	const float STRUT_ROTATION_ANGLE = static_cast<float>(22.5*RAD);
	const VECTOR3 STRUT_SCALE_VECTOR = _V(1.45, 1.0, 1.0);

	static UINT GRP_Brake_Discs[2] = {GRP_DC_traction_motor_brake_discs_inner_TRUCK, GRP_DC_traction_motor_brake_discs_outer_TRUCK};
	anim_brake_discs = CreateAnimation(0.0);

	static VECTOR3 dummy_vec[4];
	MGROUP_ROTATE* Rot_Truck1 = new MGROUP_ROTATE(LOCALVERTEXLIST, MAKEGROUPARRAY(&dummy_vec[0]), 1, _V(0, 0, 0), _V(-1, 0, 0), (float)(20.0*RAD));
	anim_truck_rot[0] = CreateAnimation(0.5);
	ANIMATIONCOMPONENT_HANDLE parent = AddManagedAnimationComponent(anim_truck_rot[0], 0.0, 1.0, Rot_Truck1);
	MGROUP_TRANSLATE* Trans_Truck1 = new MGROUP_TRANSLATE(meshidxTruck1, DrivetruckGrpList, NGRP_TRUCK-2, _V(0.0, -JACKING_MAX_HEIGHT, 0.0));
	anim_truck_trans[0] = CreateAnimation(0.0);
	AddManagedAnimationComponent(anim_truck_trans[0], 0.0, 1.0, Trans_Truck1, parent);
	MGROUP_SCALE* Scale_Truck1 = new MGROUP_SCALE(meshidxTruck1, &DrivetruckGrpList[NGRP_TRUCK-1], 1, _V(0, 2.548, 0), _V(1, 0.8*JACKING_MAX_HEIGHT, 1));
	AddManagedAnimationComponent(anim_truck_trans[0], 0.0, 1.0, Scale_Truck1, parent);
	//brake discs inner truck 1
	InnerBrakeDiscs[0] = new MGROUP_ROTATE(meshidxTruck1,&GRP_Brake_Discs[0],1,_V(INNER_BRAKE_DISK_REF_PT.x,INNER_BRAKE_DISK_REF_PT.y-jackHeight,INNER_BRAKE_DISK_REF_PT.z),_V(0,0,1),static_cast<float>(360*RAD));
	AddManagedAnimationComponent(anim_brake_discs,0,1,InnerBrakeDiscs[0],parent);
	//brake discs outter
	OutterBrakeDiscs[0] = new MGROUP_ROTATE(meshidxTruck1,&GRP_Brake_Discs[1],1,_V(OUTTER_BRAKE_DISK_REF_PT.x,OUTTER_BRAKE_DISK_REF_PT.y-jackHeight,OUTTER_BRAKE_DISK_REF_PT.z),_V(0,0,1),-static_cast<float>(360*RAD));
	AddAnimationComponent(anim_brake_discs,0,1,OutterBrakeDiscs[0]);
	// strut animations
	static UINT Strut11GrpList[2] = {GRP_Corner_D_FWD_steering_cylinder, GRP_Corner_D_FWD_steering_cylinder_dust_shield};
	MGROUP_ROTATE* Rotate_Strut11 = new MGROUP_ROTATE(meshidxCrawler, &Strut11GrpList[0], 1, _V(8.289, 2.394, 18.126), _V(0, 0, -1), STRUT_ROTATION_ANGLE);
	parent = AddManagedAnimationComponent(anim_truck_trans[0], 0.0, 1.0, Rotate_Strut11);
	MGROUP_SCALE* Scale_Strut11 = new MGROUP_SCALE(meshidxCrawler, &Strut11GrpList[1], 1, _V(12.447, 2.905, 18.126), STRUT_SCALE_VECTOR);
	AddManagedAnimationComponent(anim_truck_trans[0], 0.0, 1.0, Scale_Strut11, parent);
	static UINT Strut12GrpList[2] = {GRP_Corner_D_Rear_steering_cylinder, GRP_Corner_D_Rear_steering_cylinder_dust_shield};
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
	MGROUP_SCALE* Scale_Truck2 = new MGROUP_SCALE(meshidxTruck2, &DrivetruckGrpList[NGRP_TRUCK-1], 1, _V(0, 2.548, 0), _V(1, 0.8*JACKING_MAX_HEIGHT, 1));
	AddManagedAnimationComponent(anim_truck_trans[1], 0.0, 1.0, Scale_Truck2, parent);
	//brake discs inner truck 2
	InnerBrakeDiscs[1] = new MGROUP_ROTATE(meshidxTruck2,&GRP_Brake_Discs[0],1,_V(-INNER_BRAKE_DISK_REF_PT.x,INNER_BRAKE_DISK_REF_PT.y-jackHeight,INNER_BRAKE_DISK_REF_PT.z),_V(0,0,1),-static_cast<float>(360*RAD));
	AddAnimationComponent(anim_brake_discs,0,1,InnerBrakeDiscs[1]);
	//brake discs outter
	OutterBrakeDiscs[1] = new MGROUP_ROTATE(meshidxTruck2,&GRP_Brake_Discs[1],1,_V(-OUTTER_BRAKE_DISK_REF_PT.x,OUTTER_BRAKE_DISK_REF_PT.y-jackHeight,OUTTER_BRAKE_DISK_REF_PT.z),_V(0,0,1),static_cast<float>(360*RAD));
	AddAnimationComponent(anim_brake_discs,0,1,OutterBrakeDiscs[1]);
	// strut animations
	static UINT Strut21GrpList[2] = {GRP_Corner_C_FWD_steering_cylinder, GRP_Corner_C_FWD_steering_cylinder_dust_shield};
	MGROUP_ROTATE* Rotate_Strut21 = new MGROUP_ROTATE(meshidxCrawler, &Strut21GrpList[0], 1, _V(-8.307, 2.414, -18.191), _V(0, 0, 1), STRUT_ROTATION_ANGLE);
	parent = AddManagedAnimationComponent(anim_truck_trans[1], 0.0, 1.0, Rotate_Strut21);
	MGROUP_SCALE* Scale_Strut21 = new MGROUP_SCALE(meshidxCrawler, &Strut21GrpList[1], 1, _V(-12.465, 2.924, -18.191), STRUT_SCALE_VECTOR);
	AddManagedAnimationComponent(anim_truck_trans[1], 0.0, 1.0, Scale_Strut21, parent);
	static UINT Strut22GrpList[2] = {GRP_Corner_C_Rear_steering_cylinder, GRP_Corner_C_Rear_steering_cylinder_dust_shield};
	MGROUP_ROTATE* Rotate_Strut22 = new MGROUP_ROTATE(meshidxCrawler, &Strut22GrpList[0], 1, _V(-8.307, 2.414, -8.343), _V(0, 0, 1), STRUT_ROTATION_ANGLE);
	parent = AddManagedAnimationComponent(anim_truck_trans[1], 0.0, 1.0, Rotate_Strut22);
	MGROUP_SCALE* Scale_Strut22 = new MGROUP_SCALE(meshidxCrawler, &Strut22GrpList[1], 1, _V(-12.465, 2.924, -8.343), STRUT_SCALE_VECTOR);
	AddManagedAnimationComponent(anim_truck_trans[1], 0.0, 1.0, Scale_Strut22, parent);

	MGROUP_ROTATE* Rot_Truck3 = new MGROUP_ROTATE(LOCALVERTEXLIST, MAKEGROUPARRAY(&dummy_vec[2]), 1, _V(0, 0, 0), _V(-1, 0, 0), (float)(20.0*RAD));
	anim_truck_rot[2] = CreateAnimation(0.5);
	parent = AddManagedAnimationComponent(anim_truck_rot[2], 0.0, 1.0, Rot_Truck3);
	MGROUP_TRANSLATE* Trans_Truck3 = new MGROUP_TRANSLATE(meshidxTruck3, DrivetruckGrpList, NGRP_TRUCK-2, _V(0.0, -JACKING_MAX_HEIGHT, 0.0));
	anim_truck_trans[2] = CreateAnimation(0.0);
	AddManagedAnimationComponent(anim_truck_trans[2], 0.0, 1.0, Trans_Truck3, parent);
	MGROUP_SCALE* Scale_Truck3 = new MGROUP_SCALE(meshidxTruck3, &DrivetruckGrpList[NGRP_TRUCK-1], 1,_V(0, 2.548, 0), _V(1, 0.8*JACKING_MAX_HEIGHT, 1));
	AddManagedAnimationComponent(anim_truck_trans[2], 0.0, 1.0, Scale_Truck3, parent);
	//brake discs inner truck 3
	InnerBrakeDiscs[2] = new MGROUP_ROTATE(meshidxTruck3,&GRP_Brake_Discs[0],1,_V(INNER_BRAKE_DISK_REF_PT.x,INNER_BRAKE_DISK_REF_PT.y-jackHeight,INNER_BRAKE_DISK_REF_PT.z),_V(0,0,1),static_cast<float>(360*RAD));
	AddAnimationComponent(anim_brake_discs,0,1,InnerBrakeDiscs[2]);
	//brake discs outter
	OutterBrakeDiscs[2] = new MGROUP_ROTATE(meshidxTruck3,&GRP_Brake_Discs[1],1,_V(OUTTER_BRAKE_DISK_REF_PT.x,OUTTER_BRAKE_DISK_REF_PT.y-jackHeight,OUTTER_BRAKE_DISK_REF_PT.z),_V(0,0,1),-static_cast<float>(360*RAD));
	AddAnimationComponent(anim_brake_discs,0,1,OutterBrakeDiscs[2]);
	// strut animations
	static UINT Strut31GrpList[2] = {GRP_Corner_A_FWD_steering_cylinder, GRP_Corner_A_FWD_steering_cylinder_dust_shield};
	MGROUP_ROTATE* Rotate_Strut31 = new MGROUP_ROTATE(meshidxCrawler, &Strut31GrpList[0], 1, _V(8.289, 2.396, -18.181), _V(0, 0, -1), STRUT_ROTATION_ANGLE);
	parent = AddManagedAnimationComponent(anim_truck_trans[2], 0.0, 1.0, Rotate_Strut31);
	MGROUP_SCALE* Scale_Strut31 = new MGROUP_SCALE(meshidxCrawler, &Strut31GrpList[1], 1, _V(12.449, 2.908, -18.181), STRUT_SCALE_VECTOR);
	AddManagedAnimationComponent(anim_truck_trans[2], 0.0, 1.0, Scale_Strut31, parent);
	static UINT Strut32GrpList[2] = {GRP_Corner_A_Rear_steering_cylinder, GRP_Corner_A_Rear_steering_cylinder_dust_shield};
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
	MGROUP_SCALE* Scale_Truck4 = new MGROUP_SCALE(meshidxTruck4, &DrivetruckGrpList[NGRP_TRUCK-1], 1, _V(0, 2.548, 0), _V(1, 0.8*JACKING_MAX_HEIGHT, 1));
	AddManagedAnimationComponent(anim_truck_trans[3], 0.0, 1.0, Scale_Truck4, parent);
	//brake discs inner truck 4
	InnerBrakeDiscs[3] = new MGROUP_ROTATE(meshidxTruck4,&GRP_Brake_Discs[0],1,_V(-INNER_BRAKE_DISK_REF_PT.x,INNER_BRAKE_DISK_REF_PT.y-jackHeight,INNER_BRAKE_DISK_REF_PT.z),_V(0,0,1),-static_cast<float>(360*RAD));
	AddAnimationComponent(anim_brake_discs,0,1,InnerBrakeDiscs[3]);
	//brake discs outter
	OutterBrakeDiscs[3] = new MGROUP_ROTATE(meshidxTruck4,&GRP_Brake_Discs[1],1,_V(-OUTTER_BRAKE_DISK_REF_PT.x,OUTTER_BRAKE_DISK_REF_PT.y-jackHeight,OUTTER_BRAKE_DISK_REF_PT.z),_V(0,0,1),static_cast<float>(360*RAD));
	AddAnimationComponent(anim_brake_discs,0,1,OutterBrakeDiscs[3]);
	// strut animations
	static UINT Strut41GrpList[2] = {GRP_Corner_B_FWD_steering_cylinder, GRP_Corner_B_FWD_steering_cylinder_dust_shield};
	MGROUP_ROTATE* Rotate_Strut41 = new MGROUP_ROTATE(meshidxCrawler, &Strut41GrpList[0], 1, _V(-8.307, 2.414, -18.191), _V(0, 0, 1), STRUT_ROTATION_ANGLE);
	parent = AddManagedAnimationComponent(anim_truck_trans[3], 0.0, 1.0, Rotate_Strut41);
	MGROUP_SCALE* Scale_Strut41 = new MGROUP_SCALE(meshidxCrawler, &Strut41GrpList[1], 1, _V(-12.465, 2.924, -18.191), STRUT_SCALE_VECTOR);
	AddManagedAnimationComponent(anim_truck_trans[3], 0.0, 1.0, Scale_Strut41, parent);
	static UINT Strut42GrpList[2] = {GRP_Corner_B_Rear_steering_cylinder, GRP_Corner_B_Rear_steering_cylinder_dust_shield};
	MGROUP_ROTATE* Rotate_Strut42 = new MGROUP_ROTATE(meshidxCrawler, &Strut42GrpList[0], 1, _V(-8.307, 2.414, -8.343), _V(0, 0, 1), STRUT_ROTATION_ANGLE);
	parent = AddManagedAnimationComponent(anim_truck_trans[3], 0.0, 1.0, Rotate_Strut42);
	MGROUP_SCALE* Scale_Strut42 = new MGROUP_SCALE(meshidxCrawler, &Strut42GrpList[1], 1, _V(-12.465, 2.924, -8.343), STRUT_SCALE_VECTOR);
	AddManagedAnimationComponent(anim_truck_trans[3], 0.0, 1.0, Scale_Strut42, parent);
}