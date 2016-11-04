#include "Crawler.h"
#include "meshres_crawler.h"


void Crawler::DefineStrutAnimations()
{	
	const float STRUT_ROTATION_ANGLE = static_cast<float>(22.5*RAD);
	const VECTOR3 STRUT_SCALE_VECTOR = _V(1.45, 1.0, 1.0);

	static UINT Strut11GrpList[2] = {GRP_CORNER_D_FWD_STEERING_CYLINDER, GRP_CORNER_D_FWD_STEERING_CYLINDER_DUST_SHIELD};
	MGROUP_ROTATE* Rotate_Strut11 = new MGROUP_ROTATE(meshidxCrawler, &Strut11GrpList[0], 1, _V(8.289, 2.394, 18.126), _V(0, 0, -1), STRUT_ROTATION_ANGLE);
	ANIMATIONCOMPONENT_HANDLE parent = AddManagedAnimationComponent(anim_truck_trans[0], 0.0, 1.0, Rotate_Strut11);
	MGROUP_SCALE* Scale_Strut11 = new MGROUP_SCALE(meshidxCrawler, &Strut11GrpList[1], 1, _V(12.447, 2.905, 18.126), STRUT_SCALE_VECTOR);
	AddManagedAnimationComponent(anim_truck_trans[0], 0.0, 1.0, Scale_Strut11, parent);
	static UINT Strut12GrpList[2] = {GRP_CORNER_D_REAR_STEERING_CYLINDER, GRP_CORNER_D_FWD_STEERING_CYLINDER_DUST_SHIELD};
	MGROUP_ROTATE* Rotate_Strut12 = new MGROUP_ROTATE(meshidxCrawler, &Strut12GrpList[0], 1, _V(8.286, 2.394, 8.279), _V(0, 0, -1), STRUT_ROTATION_ANGLE);
	parent = AddManagedAnimationComponent(anim_truck_trans[0], 0.0, 1.0, Rotate_Strut12);
	MGROUP_SCALE* Scale_Strut12 = new MGROUP_SCALE(meshidxCrawler, &Strut12GrpList[1], 1, _V(12.447, 2.905, 8.279), STRUT_SCALE_VECTOR);
	AddManagedAnimationComponent(anim_truck_trans[0], 0.0, 1.0, Scale_Strut12, parent);

	static UINT Strut21GrpList[2] = {GRP_CORNER_C_FWD_STEERING_CYLINDER, GRP_CORNER_C_FWD_STEERING_CYLINDER_DUST_SHIELD};
	MGROUP_ROTATE* Rotate_Strut21 = new MGROUP_ROTATE(meshidxCrawler, &Strut21GrpList[0], 1, _V(-8.307, 2.414, -18.191), _V(0, 0, 1), STRUT_ROTATION_ANGLE);
	parent = AddManagedAnimationComponent(anim_truck_trans[1], 0.0, 1.0, Rotate_Strut21);
	MGROUP_SCALE* Scale_Strut21 = new MGROUP_SCALE(meshidxCrawler, &Strut21GrpList[1], 1, _V(-12.465, 2.924, -18.191), STRUT_SCALE_VECTOR);
	AddManagedAnimationComponent(anim_truck_trans[1], 0.0, 1.0, Scale_Strut21, parent);
	static UINT Strut22GrpList[2] = {GRP_CORNER_C_REAR_STEERING_CYLINDER, GRP_CORNER_C_REAR_STEERING_CYLINDER_DUST_SHIELD};
	MGROUP_ROTATE* Rotate_Strut22 = new MGROUP_ROTATE(meshidxCrawler, &Strut22GrpList[0], 1, _V(-8.307, 2.414, -8.343), _V(0, 0, 1), STRUT_ROTATION_ANGLE);
	parent = AddManagedAnimationComponent(anim_truck_trans[1], 0.0, 1.0, Rotate_Strut22);
	MGROUP_SCALE* Scale_Strut22 = new MGROUP_SCALE(meshidxCrawler, &Strut22GrpList[1], 1, _V(-12.465, 2.924, -8.343), STRUT_SCALE_VECTOR);
	AddManagedAnimationComponent(anim_truck_trans[1], 0.0, 1.0, Scale_Strut22, parent);

	static UINT Strut31GrpList[2] = {GRP_CORNER_A_FWD_STEERING_CYLINDER, GRP_CORNER_A_FWD_STEERING_CYLINDER_DUST_SHIELD};
	MGROUP_ROTATE* Rotate_Strut31 = new MGROUP_ROTATE(meshidxCrawler, &Strut31GrpList[0], 1, _V(8.289, 2.396, -18.181), _V(0, 0, -1), STRUT_ROTATION_ANGLE);
	parent = AddManagedAnimationComponent(anim_truck_trans[2], 0.0, 1.0, Rotate_Strut31);
	MGROUP_SCALE* Scale_Strut31 = new MGROUP_SCALE(meshidxCrawler, &Strut31GrpList[1], 1, _V(12.449, 2.908, -18.181), STRUT_SCALE_VECTOR);
	AddManagedAnimationComponent(anim_truck_trans[2], 0.0, 1.0, Scale_Strut31, parent);
	static UINT Strut32GrpList[2] = {GRP_CORNER_A_REAR_STEERING_CYLINDER, GRP_CORNER_D_REAR_STEERING_CYLINDER_DUST_SHIELD};
	MGROUP_ROTATE* Rotate_Strut32 = new MGROUP_ROTATE(meshidxCrawler, &Strut32GrpList[0], 1, _V(8.289, 2.396, -8.333), _V(0, 0, -1), STRUT_ROTATION_ANGLE);
	parent = AddManagedAnimationComponent(anim_truck_trans[2], 0.0, 1.0, Rotate_Strut32);
	MGROUP_SCALE* Scale_Strut32 = new MGROUP_SCALE(meshidxCrawler, &Strut32GrpList[1], 1, _V(12.449, 2.908, -8.333), STRUT_SCALE_VECTOR);
	AddManagedAnimationComponent(anim_truck_trans[2], 0.0, 1.0, Scale_Strut32, parent);

	static UINT Strut41GrpList[2] = {GRP_CORNER_B_FWD_STEERING_CYLINDER, GRP_CORNER_B_FWD_STEERING_CYLINDER_DUST_SHIELD};
	MGROUP_ROTATE* Rotate_Strut41 = new MGROUP_ROTATE(meshidxCrawler, &Strut41GrpList[0], 1, _V(-8.307, 2.414, -18.191), _V(0, 0, 1), STRUT_ROTATION_ANGLE);
	parent = AddManagedAnimationComponent(anim_truck_trans[3], 0.0, 1.0, Rotate_Strut41);
	MGROUP_SCALE* Scale_Strut41 = new MGROUP_SCALE(meshidxCrawler, &Strut41GrpList[1], 1, _V(-12.465, 2.924, -18.191), STRUT_SCALE_VECTOR);
	AddManagedAnimationComponent(anim_truck_trans[3], 0.0, 1.0, Scale_Strut41, parent);
	static UINT Strut42GrpList[2] = {GRP_CORNER_B_FWD_STEERING_CYLINDER, GRP_CORNER_B_REAR_STEERING_CYLINDER_DUST_SHIELD};
	MGROUP_ROTATE* Rotate_Strut42 = new MGROUP_ROTATE(meshidxCrawler, &Strut42GrpList[0], 1, _V(-8.307, 2.414, -8.343), _V(0, 0, 1), STRUT_ROTATION_ANGLE);
	parent = AddManagedAnimationComponent(anim_truck_trans[3], 0.0, 1.0, Rotate_Strut42);
	MGROUP_SCALE* Scale_Strut42 = new MGROUP_SCALE(meshidxCrawler, &Strut42GrpList[1], 1, _V(-12.465, 2.924, -8.343), STRUT_SCALE_VECTOR);
	AddManagedAnimationComponent(anim_truck_trans[3], 0.0, 1.0, Scale_Strut42, parent);
}
