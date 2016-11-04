// ======================================================
// Created by ssumeshc 0.2
// Date of conversion : Fri Nov 04 18:08:38 2016

// Input file: Crawler_mainbody.msh
// ======================================================

#pragma once

const unsigned int NUMGRP = 56;

const unsigned int GRP_CORNER_D_FWD_BOTTOM_STEERING_CYLINDER	=	0;	//Corner_D_FWD_bottom_steering_cylinder
const unsigned int GRP_CORNER_C_FWD_BOTTOM_STEERING_CYLINDER	=	1;	//Corner_C_FWD_bottom_steering_cylinder
const unsigned int GRP_CORNER_D_FWD_BOTTOM_STEERING_CYLINDER_DUST_SHIELD	=	2;	//Corner_D_FWD_bottom_steering_cylinder_dust_shield
const unsigned int GRP_CORNER_C_FWD_BOTTOM_STEERING_CYLINDER_DUST_SHIELD	=	3;	//Corner_C_FWD_bottom_steering_cylinder_dust_shield
const unsigned int GRP_CABLE_TRAY	=	4;	//Cable_tray
const unsigned int GRP_HAND_RAILS	=	5;	//Hand_rails
const unsigned int GRP_INTERIOR_ELEMENTS	=	6;	//Interior_elements
const unsigned int GRP_MAIN_STRUCTURE	=	7;	//Main_Structure
const unsigned int GRP_CAB_1	=	8;	//Cab_1
const unsigned int GRP_CAB_1_WINDOW_FRAMES_1	=	9;	//Cab_1_window_frames_1
const unsigned int GRP_CAB_1_WINDOW_FRAMES_2	=	10;	//Cab_1_window_frames_2
const unsigned int GRP_CAB_3_WINDOW_FRAMES_1	=	11;	//Cab_3_window_frames_1
const unsigned int GRP_CAB_3_WINDOW_FRAMES_2	=	12;	//Cab_3_window_frames_2
const unsigned int GRP_CRANE_BRACKETS	=	13;	//Crane_brackets
const unsigned int GRP_CYLINDER639	=	14;	//Cylinder639
const unsigned int GRP_CYLINDER641	=	15;	//Cylinder641
const unsigned int GRP_DECALS	=	16;	//Decals
const unsigned int GRP_FORWARD_CRANE_HOOK	=	17;	//Forward crane hook
const unsigned int GRP_MUFFLER_BOXES1	=	18;	//Muffler_boxes1
const unsigned int GRP_MUFFLER_BOXES2	=	19;	//Muffler_Boxes2
const unsigned int GRP_MUFFLER_BRACKETS	=	20;	//Muffler_brackets
const unsigned int GRP_MUFFLER_PIPES1	=	21;	//Muffler_pipes1
const unsigned int GRP_MUFFLER_PIPES2	=	22;	//Muffler_pipes2
const unsigned int GRP_MUFFLER_PIPES3	=	23;	//Muffler_pipes3
const unsigned int GRP_CORNER_A_REAR_STEERING_CYLINDER	=	24;	//Corner_A_Rear_steering_cylinder
const unsigned int GRP_CORNER_D_REAR_STEERING_CYLINDER	=	25;	//Corner_D_Rear_steering_cylinder
const unsigned int GRP_CORNER_D_FWD_STEERING_CYLINDER	=	26;	//Corner_D_FWD_steering_cylinder
const unsigned int GRP_CORNER_A_FWD_STEERING_CYLINDER	=	27;	//Corner_A_FWD_steering_cylinder
const unsigned int GRP_CORNER_A_REAR_STEERING_CYLINDER_DUST_SHIELD	=	28;	//Corner_A_Rear_steering_cylinder_dust_shield
const unsigned int GRP_CORNER_D_REAR_STEERING_CYLINDER_DUST_SHIELD	=	29;	//Corner_D_Rear_steering_cylinder_dust_shield
const unsigned int GRP_CORNER_D_FWD_STEERING_CYLINDER_DUST_SHIELD	=	30;	//Corner_D_FWD_steering_cylinder_dust_shield
const unsigned int GRP_CORNER_A_FWD_STEERING_CYLINDER_DUST_SHIELD	=	31;	//Corner_A_FWD_steering_cylinder_dust_shield
const unsigned int GRP_CORNER_B_FWD_BOTTOM_STEERING_CYLINDER01	=	32;	//Corner_B_FWD_bottom_steering_cylinder01
const unsigned int GRP_CORNER_A_FWD_BOTTOM_STEERING_CYLINDER	=	33;	//Corner_A_FWD_bottom_steering_cylinder
const unsigned int GRP_CORNER_B_FWD_BOTTOM_STEERING_CYLINDER_DUST_SHIELD	=	34;	//Corner_B_FWD_bottom_steering_cylinder_dust_shield
const unsigned int GRP_CORNER_A_FWD_BOTTOM_STEERING_CYLINDER_DUST_SHIELD01	=	35;	//Corner_A_FWD_bottom_steering_cylinder_dust_shield01
const unsigned int GRP_CORNER_B_REAR_BOTTOM_STEERING_CYLINDER	=	36;	//Corner_B_Rear_bottom_steering_cylinder
const unsigned int GRP_CORNER_A_REAR_BOTTOM_STEERING_CYLINDER	=	37;	//Corner_A_Rear_bottom_steering_cylinder
const unsigned int GRP_CORNER_B_REAR_BOTTOM_STEERING_CYLINDER_DUST_SHIELD01	=	38;	//Corner_B_Rear_bottom_steering_cylinder_dust_shield01
const unsigned int GRP_CORNER_A_REAR_BOTTOM_STEERING_CYLINDER_DUST_SHIELD01	=	39;	//Corner_A_Rear_bottom_steering_cylinder_dust_shield01
const unsigned int GRP_CORNER_C_REAR_BOTTOM_STEERING_CYLINDER_DUST_SHIELD	=	40;	//Corner_C_Rear_bottom_steering_cylinder_dust_shield
const unsigned int GRP_CORNER_D_REAR_BOTTOM_STEERING_CYLINDER	=	41;	//Corner_D_Rear_bottom_steering_cylinder
const unsigned int GRP_CORNER_D_REAR_BOTTOM_STEERING_CYLINDER_DUST_SHIELD	=	42;	//Corner_D_Rear_bottom_steering_cylinder_dust_shield
const unsigned int GRP_CORNER_C_REAR_BOTTOM_STEERING_CYLINDER	=	43;	//Corner_C_Rear_bottom_steering_cylinder
const unsigned int GRP_CORNER_B_REAR_STEERING_CYLINDER	=	44;	//Corner_B_Rear_steering_cylinder
const unsigned int GRP_CORNER_B_FWD_STEERING_CYLINDER	=	45;	//Corner_B_FWD_steering_cylinder
const unsigned int GRP_CORNER_C_FWD_STEERING_CYLINDER	=	46;	//Corner_C_FWD_steering_cylinder
const unsigned int GRP_CORNER_C_REAR_STEERING_CYLINDER	=	47;	//Corner_C_Rear_steering_cylinder
const unsigned int GRP_CORNER_B_REAR_STEERING_CYLINDER_DUST_SHIELD	=	48;	//Corner_B_Rear_steering_cylinder_dust_shield
const unsigned int GRP_CORNER_B_FWD_STEERING_CYLINDER_DUST_SHIELD	=	49;	//Corner_B_FWD_steering_cylinder_dust_shield
const unsigned int GRP_CORNER_C_FWD_STEERING_CYLINDER_DUST_SHIELD	=	50;	//Corner_C_FWD_steering_cylinder_dust_shield
const unsigned int GRP_CORNER_C_REAR_STEERING_CYLINDER_DUST_SHIELD	=	51;	//Corner_C_Rear_steering_cylinder_dust_shield
const unsigned int GRP_CAB_3	=	52;	//Cab_3
const unsigned int GRP_CAB_3_WINDOWS	=	53;	//Cab_3_windows
const unsigned int GRP_CAB_1_WINDOWS	=	54;	//Cab_1_windows
const unsigned int GRP_CATWALK_GRATING	=	55;	//Catwalk_grating

