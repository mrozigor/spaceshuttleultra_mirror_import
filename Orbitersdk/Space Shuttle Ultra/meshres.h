// ======================================================
// Created by ssumeshc 0.2
// Date of conversion : Fri Oct 07 02:45:36 2016

// Input file: Orbiter.msh
// ======================================================

#pragma once

const unsigned int NUMGRP = 148;

const unsigned int GRP_XO576_BULKHEAD_	=	0;	//Xo576_BULKHEAD 
const unsigned int GRP_STBDOMS_BASE_TILES	=	1;	//STBDOMS_base_tiles
const unsigned int GRP_XO576_BULKHEAD_DOCKING_LIGHT	=	2;	//Xo576_BULKHEAD_DOCKING_LIGHT
const unsigned int GRP_XO576_BULKHEAD_LIGHT	=	3;	//Xo576_BULKHEAD_LIGHT
const unsigned int GRP_RIGHT_OMSPOD	=	4;	//RIGHT_OMSpod
const unsigned int GRP_LEFT_OMSPOD	=	5;	//LEFT_OMSpod
const unsigned int GRP_TAIL_CHUTE	=	6;	//TAIL_CHUTE
const unsigned int GRP_TAIL_NO_CHUTE	=	7;	//TAIL_NO_CHUTE
const unsigned int GRP_XSTARTRACKER_IMAGE	=	8;	//Xstartracker_image
const unsigned int GRP_ZSTARTRACKER_IMAGE	=	9;	//Zstartracker_image
const unsigned int GRP_STARTRACKER_INDENT	=	10;	//startracker_indent
const unsigned int GRP_ZSTARTRACKER_DOOR	=	11;	//Zstartracker_door
const unsigned int GRP_XSTARTRACKER_DOOR	=	12;	//Xstartracker_door
const unsigned int GRP_RIGHT_ARCS_JETS	=	13;	//RIGHT_ARCS_JETS
const unsigned int GRP_LEFT_ARCS_JETS	=	14;	//LEFT_ARCS_JETS
const unsigned int GRP_TRANSPARENT	=	15;	//transparent
const unsigned int GRP_MIDBODYT0_UMBILICAL_CAVITIES	=	16;	//MIDBODYT0_UMBILICAL_CAVITIES
const unsigned int GRP_BAY13_TRANSPARENCY	=	17;	//BAY13_TRANSPARENCY
const unsigned int GRP_WINDOWS	=	18;	//WINDOWS
const unsigned int GRP_XO576_BULKHEAD_EQUIPMENT	=	19;	//Xo576_BULKHEAD_EQUIPMENT
const unsigned int GRP_PLB_VENTS	=	20;	//PLB_VENTS
const unsigned int GRP_STBD_FWD_RADIATOR_COOLANT_LINES	=	21;	//STBD_FWD_RADIATOR_COOLANT_LINES
const unsigned int GRP_CENTER_SSME	=	22;	//CENTER_SSME
const unsigned int GRP_PLB_TCS	=	23;	//PLB_TCS
const unsigned int GRP_STBDDOOR_PULL	=	24;	//STBDdoor_pull
const unsigned int GRP_STBD_CLAMP	=	25;	//STBD_clamp
const unsigned int GRP_STBDPUSH_ROD	=	26;	//STBDpush_rod
const unsigned int GRP_PORTPUSH_ROD	=	27;	//PORTpush_rod
const unsigned int GRP_PORT_CLAMP	=	28;	//PORT_clamp
const unsigned int GRP_PORTDOOR_PULL	=	29;	//PORTdoor_pull
const unsigned int GRP_AFT_EVA_WINCH_HOOK	=	30;	//AFT_EVA_WINCH_HOOK
const unsigned int GRP_RIGHT_ARCS_POD	=	31;	//RIGHT_ARCS_POD
const unsigned int GRP_LEFT_ARCS_POD	=	32;	//LEFT_ARCS_POD
const unsigned int GRP_PLB_LIGHTS	=	33;	//PLB_LIGHTS
const unsigned int GRP_PLB_SILL_LONGERONS	=	34;	//PLB_SILL_LONGERONS
const unsigned int GRP_PLB_RING_FRAMES	=	35;	//PLB_RING_FRAMES
const unsigned int GRP_PORT_FWD_RADIATOR_COOLANT_LINES	=	36;	//PORT_FWD_RADIATOR_COOLANT_LINES
const unsigned int GRP_PANTILT_CAM_A	=	37;	//PANTILT_CAM_A
const unsigned int GRP_CAM_B	=	38;	//CAM_B
const unsigned int GRP_PANTILT_CAM_B	=	39;	//PANTILT_CAM_B
const unsigned int GRP_CAM_C	=	40;	//CAM_C
const unsigned int GRP_PANTILT_CAM_C	=	41;	//PANTILT_CAM_C
const unsigned int GRP_CAM_D	=	42;	//CAM_D
const unsigned int GRP_PANTILT_CAM_D	=	43;	//PANTILT_CAM_D
const unsigned int GRP_AFT_EVA_WINCH_HANDLE	=	44;	//AFT_EVA_WINCH_HANDLE
const unsigned int GRP_FWD_EVA_WINCH_HOOK	=	45;	//FWD_EVA_WINCH_HOOK
const unsigned int GRP_FWD_EVA_WINCH_HANDLE	=	46;	//FWD_EVA_WINCH_HANDLE
const unsigned int GRP_EVA_THERMAL_COVER	=	47;	//EVA_THERMAL_COVER
const unsigned int GRP_NLG_UPPER_TORQUE_ARM	=	48;	//NLG_UPPER_TORQUE_ARM
const unsigned int GRP_NLG_LOWER_TORQUE_ARM	=	49;	//NLG_LOWER_TORQUE_ARM
const unsigned int GRP_NLG_STRUT	=	50;	//NLG_STRUT
const unsigned int GRP_NLG_SHOCK_STRUT	=	51;	//NLG_SHOCK_STRUT
const unsigned int GRP_NOSE_WHEELS	=	52;	//NOSE_WHEELS
const unsigned int GRP_LEFT_MN_WHEELS	=	53;	//LEFT_MN_WHEELS
const unsigned int GRP_LEFT_MLG_STRUT	=	54;	//LEFT_MLG_STRUT
const unsigned int GRP_LEFT_MLG_SHOCK_STRUT	=	55;	//LEFT_MLG_SHOCK_STRUT
const unsigned int GRP_RIGHT_MN_WHEELS	=	56;	//RIGHT_MN_WHEELS
const unsigned int GRP_RIGHT_MLG_STRUT	=	57;	//RIGHT_MLG_STRUT
const unsigned int GRP_RIGHT_MLG_SHOCK_STRUT	=	58;	//RIGHT_MLG_SHOCK_STRUT
const unsigned int GRP_PROBEL	=	59;	//PROBEL
const unsigned int GRP_PROBER	=	60;	//PROBER
const unsigned int GRP_SSME_HEATSHIELDS	=	61;	//SSME_HEATSHIELDS
const unsigned int GRP_ET_DOOR_LEFT	=	62;	//ET_DOOR_LEFT
const unsigned int GRP_ET_DOOR_LEFT_DDM_ARM_AFT	=	63;	//ET_DOOR_LEFT_DDM_ARM_AFT
const unsigned int GRP_ET_DOOR_LEFT_DDM_ARM_FWD	=	64;	//ET_DOOR_LEFT_DDM_ARM_FWD
const unsigned int GRP_ET_DOOR_LEFT_DDM_LINK_ROD_AFT	=	65;	//ET_DOOR_LEFT_DDM_LINK_ROD_AFT
const unsigned int GRP_ET_DOOR_LEFT_DDM_LINK_ROD_FWD	=	66;	//ET_DOOR_LEFT_DDM_LINK_ROD_FWD
const unsigned int GRP_ET_DOOR_RIGHT_DDM_LINK_ROD_FWD	=	67;	//ET_DOOR_RIGHT_DDM_LINK_ROD_FWD
const unsigned int GRP_ET_DOOR_RIGHT_DDM_LINK_ROD_AFT	=	68;	//ET_DOOR_RIGHT_DDM_LINK_ROD_AFT
const unsigned int GRP_ET_DOOR_RIGHT_DDM_ARM_FWD	=	69;	//ET_DOOR_RIGHT_DDM_ARM_FWD
const unsigned int GRP_ET_DOOR_RIGHT_DDM_ARM_AFT	=	70;	//ET_DOOR_RIGHT_DDM_ARM_AFT
const unsigned int GRP_ET_DOOR_RIGHT	=	71;	//ET_DOOR_RIGHT
const unsigned int GRP_BAYPFR	=	72;	//BayPFR
const unsigned int GRP_BAY1_LONGERON	=	73;	//Bay1_longeron
const unsigned int GRP_BAY2_LONGERON	=	74;	//Bay2_longeron
const unsigned int GRP_BAY3_LONGERON	=	75;	//Bay3_longeron
const unsigned int GRP_BAY4_LONGERON	=	76;	//Bay4_longeron
const unsigned int GRP_BAY5_LONGERON	=	77;	//Bay5_longeron
const unsigned int GRP_BAY6_LONGERON	=	78;	//Bay6_longeron
const unsigned int GRP_BAY7_LONGERON	=	79;	//Bay7_longeron
const unsigned int GRP_BAY8_LONGERON	=	80;	//Bay8_longeron
const unsigned int GRP_BAY9_LONGERON	=	81;	//Bay9_longeron
const unsigned int GRP_BAY10_LONGERON	=	82;	//Bay10_longeron
const unsigned int GRP_BAY11_LONGERON	=	83;	//Bay11_longeron
const unsigned int GRP_BAY12_LONGERON	=	84;	//Bay12_longeron
const unsigned int GRP_BAY13_LONGERON	=	85;	//Bay13_longeron
const unsigned int GRP_CAM_A	=	86;	//CAM_A
const unsigned int GRP_PLB_WIRE_TRAY	=	87;	//PLB_WIRE_TRAY
const unsigned int GRP_XO1307_BULKHEAD	=	88;	//Xo1307_BULKHEAD
const unsigned int GRP_LOME	=	89;	//LOME
const unsigned int GRP_RIGHT_SSME	=	90;	//RIGHT_SSME
const unsigned int GRP_LEFT_SSME	=	91;	//LEFT_SSME
const unsigned int GRP_ROME	=	92;	//ROME
const unsigned int GRP_PLB_BAY13_COVERS	=	93;	//PLB_BAY13_COVERS
const unsigned int GRP_AFT_HOOKS	=	94;	//AFT_HOOKS
const unsigned int GRP_MID_AFT_HOOKS	=	95;	//MID_AFT_HOOKS
const unsigned int GRP_MID_FWD_HOOKS	=	96;	//MID_FWD_HOOKS
const unsigned int GRP_FWD_HOOKS	=	97;	//FWD_HOOKS
const unsigned int GRP_PORT_PLB_RADIATOR_3	=	98;	//PORT_PLB_RADIATOR_3
const unsigned int GRP_PORT_PLB_RADIATOR_2	=	99;	//PORT_PLB_RADIATOR_2
const unsigned int GRP_PORT_PLB_RADIATOR_1	=	100;	//PORT_PLB_RADIATOR_1
const unsigned int GRP_PORT_PLB_RADIATOR_4	=	101;	//PORT_PLB_RADIATOR_4
const unsigned int GRP_STBD_PLB_RADIATOR_4	=	102;	//STBD_PLB_RADIATOR_4
const unsigned int GRP_STBD_PLB_RADIATOR_3	=	103;	//STBD_PLB_RADIATOR_3
const unsigned int GRP_STBD_PLB_RADIATOR_2	=	104;	//STBD_PLB_RADIATOR_2
const unsigned int GRP_STBD_PLB_RADIATOR_1	=	105;	//STBD_PLB_RADIATOR_1
const unsigned int GRP_PORT_PLBD_INTERIOR	=	106;	//PORT_PLBD_INTERIOR
const unsigned int GRP_STBD_PLBD_INTERIOR	=	107;	//STBD_PLBD_INTERIOR
const unsigned int GRP_FUSELAGE	=	108;	//FUSELAGE
const unsigned int GRP_FRCS_MODULE	=	109;	//FRCS_MODULE
const unsigned int GRP_WING_RCC	=	110;	//Wing_RCC
const unsigned int GRP_HATCH	=	111;	//hatch
const unsigned int GRP_LEFT_NLG_DOOR	=	112;	//LEFT_NLG_DOOR
const unsigned int GRP_RIGHT_NLG_DOOR	=	113;	//RIGHT_NLG_DOOR
const unsigned int GRP_LEFT_MLG_DOOR	=	114;	//LEFT_MLG_DOOR
const unsigned int GRP_RIGHT_MLG_DOOR	=	115;	//RIGHT_MLG_DOOR
const unsigned int GRP_WINGTOPS	=	116;	//Wingtops
const unsigned int GRP_T0_UMBILICAL_PANELS	=	117;	//T0_UMBILICAL_PANELS
const unsigned int GRP_BODYFLAP	=	118;	//BODYFLAP
const unsigned int GRP_RIGHT_PLBD_EXT	=	119;	//RIGHT_PLBD_EXT
const unsigned int GRP_LEFT_PLBD_EXT	=	120;	//LEFT_PLBD_EXT
const unsigned int GRP_RIGHT_RSB_PANEL	=	121;	//RIGHT_RSB_PANEL
const unsigned int GRP_LEFT_RSB_PANEL	=	122;	//LEFT_RSB_PANEL
const unsigned int GRP_LEFT_INBOARD_ELEVON	=	123;	//LEFT_INBOARD_ELEVON
const unsigned int GRP_LEFT_OUTBOARD_ELEVON	=	124;	//LEFT_OUTBOARD_ELEVON
const unsigned int GRP_ELEVON_HINGES	=	125;	//ELEVON_HINGES
const unsigned int GRP_RIGHT_INBOARD_ELEVON	=	126;	//RIGHT_INBOARD_ELEVON
const unsigned int GRP_RIGHT_OUTBOARD_ELEVON	=	127;	//RIGHT_OUTBOARD_ELEVON
const unsigned int GRP_RIGHT_INBOARD_ELEVON_SEAL_PANEL	=	128;	//RIGHT_INBOARD_ELEVON_SEAL_PANEL
const unsigned int GRP_RIGHT_OUTBOARD_ELEVON_SEAL_PANEL	=	129;	//RIGHT_OUTBOARD_ELEVON_SEAL_PANEL
const unsigned int GRP_LEFT_INBOARD_ELEVON_SEAL_PANEL	=	130;	//LEFT_INBOARD_ELEVON_SEAL_PANEL
const unsigned int GRP_LEFT_OUTBOARD_ELEVON_SEAL_PANEL	=	131;	//LEFT_OUTBOARD_ELEVON_SEAL_PANEL
const unsigned int GRP_PORTOMS_BASE_TILES	=	132;	//PORTOMS_base_tiles
const unsigned int GRP_STARBOARD_AFT_AVS_DOOR_9	=	133;	//STARBOARD_AFT_AVS_DOOR_9
const unsigned int GRP_STARBOARD_AFT_AVS_DOOR_8	=	134;	//STARBOARD_AFT_AVS_DOOR_8
const unsigned int GRP_PORT_AFT_AVS_DOOR_8	=	135;	//PORT_AFT_AVS_DOOR_8
const unsigned int GRP_PORT_AFT_AVS_DOOR_9	=	136;	//PORT_AFT_AVS_DOOR_9
const unsigned int GRP_PORT_MIDBODY_AVS_DOOR_6	=	137;	//PORT_MIDBODY_AVS_DOOR_6
const unsigned int GRP_PORT_MIDBODY_AVS_DOOR_3	=	138;	//PORT_MIDBODY_AVS_DOOR_3
const unsigned int GRP_PORT_MIDBODY_AVS_DOOR_7	=	139;	//PORT_MIDBODY_AVS_DOOR_7
const unsigned int GRP_PORT_MIDBODY_AVS_DOOR_4	=	140;	//PORT_MIDBODY_AVS_DOOR_4
const unsigned int GRP_PORT_MIDBODY_AVS_DOOR_5	=	141;	//PORT_MIDBODY_AVS_DOOR_5
const unsigned int GRP_STARBOARD_MIDBODY_AVS_DOOR_5	=	142;	//STARBOARD_MIDBODY_AVS_DOOR_5
const unsigned int GRP_STARBOARD_MIDBODY_AVS_DOOR_4	=	143;	//STARBOARD_MIDBODY_AVS_DOOR_4
const unsigned int GRP_STARBOARD_MIDBODY_AVS_DOOR_7	=	144;	//STARBOARD_MIDBODY_AVS_DOOR_7
const unsigned int GRP_STARBOARD_MIDBODY_AVS_DOOR_3	=	145;	//STARBOARD_MIDBODY_AVS_DOOR_3
const unsigned int GRP_STARBOARD_MIDBODY_AVS_DOOR_6	=	146;	//STARBOARD_MIDBODY_AVS_DOOR_6
const unsigned int GRP_PLB_SILL_EQUIPMENT	=	147;	//PLB_SILL_EQUIPMENT

