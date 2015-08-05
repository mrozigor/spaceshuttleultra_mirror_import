// ======================================================
// Created by ssumeshc 0.2
// Date of conversion : Wed Aug 05 16:38:20 2015

// Input file: Orbiter.msh
// ======================================================

#pragma once

const unsigned int NUMGRP = 144;

const unsigned int GRP_RIGHT_ARCS_JETS	=	0;	//RIGHT_ARCS_JETS
const unsigned int GRP_LEFT_ARCS_JETS	=	1;	//LEFT_ARCS_JETS
const unsigned int GRP_XSTARTRACKER_IMAGE	=	2;	//Xstartracker_image
const unsigned int GRP_ZSTARTRACKER_IMAGE	=	3;	//Zstartracker_image
const unsigned int GRP_STARTRACKER_INDENT	=	4;	//startracker_indent
const unsigned int GRP_ZSTARTRACKER_DOOR	=	5;	//Zstartracker_door
const unsigned int GRP_XSTARTRACKER_DOOR	=	6;	//Xstartracker_door
const unsigned int GRP_TRANSPARENT	=	7;	//transparent
const unsigned int GRP_STBD_PLBD_INTERIOR	=	8;	//STBD_PLBD_INTERIOR
const unsigned int GRP_PLB_TCS	=	9;	//PLB_TCS
const unsigned int GRP_RIGHT_PLBD_EXT	=	10;	//RIGHT_PLBD_EXT
const unsigned int GRP_STBD_PLB_RADIATOR_1	=	11;	//STBD_PLB_RADIATOR_1
const unsigned int GRP_STBD_PLB_RADIATOR_2	=	12;	//STBD_PLB_RADIATOR_2
const unsigned int GRP_STBD_PLB_RADIATOR_3	=	13;	//STBD_PLB_RADIATOR_3
const unsigned int GRP_STBD_PLB_RADIATOR_4	=	14;	//STBD_PLB_RADIATOR_4
const unsigned int GRP_STBD_FWD_RADIATOR_COOLANT_LINES	=	15;	//STBD_FWD_RADIATOR_COOLANT_LINES
const unsigned int GRP_STBDDOOR_PULL	=	16;	//STBDdoor_pull
const unsigned int GRP_STBD_CLAMP	=	17;	//STBD_clamp
const unsigned int GRP_STBDPUSH_ROD	=	18;	//STBDpush_rod
const unsigned int GRP_PORTPUSH_ROD	=	19;	//PORTpush_rod
const unsigned int GRP_PORT_CLAMP	=	20;	//PORT_clamp
const unsigned int GRP_PORTDOOR_PULL	=	21;	//PORTdoor_pull
const unsigned int GRP_FUSELAGE	=	22;	//FUSELAGE
const unsigned int GRP_CENTER_SSME	=	23;	//CENTER_SSME
const unsigned int GRP_LEFT_OUTBOARD_ELEVON_SEAL_PANEL	=	24;	//LEFT_OUTBOARD_ELEVON_SEAL_PANEL
const unsigned int GRP_LEFT_INBOARD_ELEVON_SEAL_PANEL	=	25;	//LEFT_INBOARD_ELEVON_SEAL_PANEL
const unsigned int GRP_LEFT_INBOARD_ELEVON	=	26;	//LEFT_INBOARD_ELEVON
const unsigned int GRP_LEFT_OUTBOARD_ELEVON	=	27;	//LEFT_OUTBOARD_ELEVON
const unsigned int GRP_ELEVON_HINGES	=	28;	//ELEVON_HINGES
const unsigned int GRP_PORT_PLBD_INTERIOR	=	29;	//PORT_PLBD_INTERIOR
const unsigned int GRP_BODYFLAP	=	30;	//BODYFLAP
const unsigned int GRP_AFT_EVA_WINCH_HOOK	=	31;	//AFT_EVA_WINCH_HOOK
const unsigned int GRP_RIGHT_ARCS_POD	=	32;	//RIGHT_ARCS_POD
const unsigned int GRP_OMS_BASE_TILES	=	33;	//OMS_base_tiles
const unsigned int GRP_LEFT_ARCS_POD	=	34;	//LEFT_ARCS_POD
const unsigned int GRP_PLB_LIGHTS	=	35;	//PLB_LIGHTS
const unsigned int GRP_PLB_VENTS	=	36;	//PLB_VENTS
const unsigned int GRP_PLB_SILL_LONGERONS	=	37;	//PLB_SILL_LONGERONS
const unsigned int GRP_PLB_RING_FRAMES	=	38;	//PLB_RING_FRAMES
const unsigned int GRP_WINGTOPS	=	39;	//Wingtops
const unsigned int GRP_HATCH	=	40;	//hatch
const unsigned int GRP_FRCS_MODULE	=	41;	//FRCS_MODULE
const unsigned int GRP_RIGHT_RSB_PANEL	=	42;	//RIGHT_RSB_PANEL
const unsigned int GRP_LEFT_RSB_PANEL	=	43;	//LEFT_RSB_PANEL
const unsigned int GRP_TAIL_CHUTE	=	44;	//Tail_Chute
const unsigned int GRP_TAIL_NO_CHUTE	=	45;	//Tail_NO_Chute
const unsigned int GRP_PORT_PLB_RADIATOR_4	=	46;	//PORT_PLB_RADIATOR_4
const unsigned int GRP_PORT_PLB_RADIATOR_1	=	47;	//PORT_PLB_RADIATOR_1
const unsigned int GRP_PORT_PLB_RADIATOR_2	=	48;	//PORT_PLB_RADIATOR_2
const unsigned int GRP_PORT_PLB_RADIATOR_3	=	49;	//PORT_PLB_RADIATOR_3
const unsigned int GRP_PORT_FWD_RADIATOR_COOLANT_LINES	=	50;	//PORT_FWD_RADIATOR_COOLANT_LINES
const unsigned int GRP_PANTILT_CAM_A	=	51;	//PANTILT_CAM_A
const unsigned int GRP_CAM_B	=	52;	//CAM_B
const unsigned int GRP_PANTILT_CAM_B	=	53;	//PANTILT_CAM_B
const unsigned int GRP_CAM_C	=	54;	//CAM_C
const unsigned int GRP_PANTILT_CAM_C	=	55;	//PANTILT_CAM_C
const unsigned int GRP_CAM_D	=	56;	//CAM_D
const unsigned int GRP_PANTILT_CAM_D	=	57;	//PANTILT_CAM_D
const unsigned int GRP_AFT_EVA_WINCH_HANDLE	=	58;	//AFT_EVA_WINCH_HANDLE
const unsigned int GRP_FWD_EVA_WINCH_HOOK	=	59;	//FWD_EVA_WINCH_HOOK
const unsigned int GRP_FWD_EVA_WINCH_HANDLE	=	60;	//FWD_EVA_WINCH_HANDLE
const unsigned int GRP_EVA_THERMAL_COVER	=	61;	//EVA_THERMAL_COVER
const unsigned int GRP_NLG_UPPER_TORQUE_ARM	=	62;	//NLG_UPPER_TORQUE_ARM
const unsigned int GRP_NLG_LOWER_TORQUE_ARM	=	63;	//NLG_LOWER_TORQUE_ARM
const unsigned int GRP_NLG_STRUT	=	64;	//NLG_STRUT
const unsigned int GRP_NLG_SHOCK_STRUT	=	65;	//NLG_SHOCK_STRUT
const unsigned int GRP_NOSE_WHEELS	=	66;	//NOSE_WHEELS
const unsigned int GRP_LEFT_MN_WHEELS	=	67;	//LEFT_MN_WHEELS
const unsigned int GRP_LEFT_MLG_STRUT	=	68;	//LEFT_MLG_STRUT
const unsigned int GRP_LEFT_MLG_SHOCK_STRUT	=	69;	//LEFT_MLG_SHOCK_STRUT
const unsigned int GRP_RIGHT_MN_WHEELS	=	70;	//RIGHT_MN_WHEELS
const unsigned int GRP_RIGHT_MLG_STRUT	=	71;	//RIGHT_MLG_STRUT
const unsigned int GRP_RIGHT_MLG_SHOCK_STRUT	=	72;	//RIGHT_MLG_SHOCK_STRUT
const unsigned int GRP_LEFT_NLG_DOOR	=	73;	//LEFT_NLG_DOOR
const unsigned int GRP_RIGHT_NLG_DOOR	=	74;	//RIGHT_NLG_DOOR
const unsigned int GRP_LEFT_MLG_DOOR	=	75;	//LEFT_MLG_DOOR
const unsigned int GRP_RIGHT_MLG_DOOR	=	76;	//RIGHT_MLG_DOOR
const unsigned int GRP_PROBEL	=	77;	//PROBEL
const unsigned int GRP_PROBER	=	78;	//PROBER
const unsigned int GRP_SSME_HEATSHIELDS	=	79;	//SSME_HEATSHIELDS
const unsigned int GRP_ET_DOOR_LEFT	=	80;	//ET_DOOR_LEFT
const unsigned int GRP_ET_DOOR_LEFT_DDM_ARM_AFT	=	81;	//ET_DOOR_LEFT_DDM_ARM_AFT
const unsigned int GRP_ET_DOOR_LEFT_DDM_ARM_FWD	=	82;	//ET_DOOR_LEFT_DDM_ARM_FWD
const unsigned int GRP_ET_DOOR_LEFT_DDM_LINK_ROD_AFT	=	83;	//ET_DOOR_LEFT_DDM_LINK_ROD_AFT
const unsigned int GRP_ET_DOOR_LEFT_DDM_LINK_ROD_FWD	=	84;	//ET_DOOR_LEFT_DDM_LINK_ROD_FWD
const unsigned int GRP_ET_DOOR_RIGHT_DDM_LINK_ROD_FWD	=	85;	//ET_DOOR_RIGHT_DDM_LINK_ROD_FWD
const unsigned int GRP_ET_DOOR_RIGHT_DDM_LINK_ROD_AFT	=	86;	//ET_DOOR_RIGHT_DDM_LINK_ROD_AFT
const unsigned int GRP_ET_DOOR_RIGHT_DDM_ARM_FWD	=	87;	//ET_DOOR_RIGHT_DDM_ARM_FWD
const unsigned int GRP_ET_DOOR_RIGHT_DDM_ARM_AFT	=	88;	//ET_DOOR_RIGHT_DDM_ARM_AFT
const unsigned int GRP_ET_DOOR_RIGHT	=	89;	//ET_DOOR_RIGHT
const unsigned int GRP_STARBOARD_MIDBODY_AVS_DOOR_7	=	90;	//STARBOARD_MIDBODY_AVS_DOOR_7
const unsigned int GRP_STARBOARD_MIDBODY_AVS_DOOR_4	=	91;	//STARBOARD_MIDBODY_AVS_DOOR_4
const unsigned int GRP_STARBOARD_MIDBODY_AVS_DOOR_5	=	92;	//STARBOARD_MIDBODY_AVS_DOOR_5
const unsigned int GRP_STARBOARD_MIDBODY_AVS_DOOR_6	=	93;	//STARBOARD_MIDBODY_AVS_DOOR_6
const unsigned int GRP_STARBOARD_AFT_AVS_DOOR_8	=	94;	//STARBOARD_AFT_AVS_DOOR_8
const unsigned int GRP_STARBOARD_AFT_AVS_DOOR_9	=	95;	//STARBOARD_AFT_AVS_DOOR_9
const unsigned int GRP_PORT_MIDBODY_AVS_DOOR_6	=	96;	//PORT_MIDBODY_AVS_DOOR_6
const unsigned int GRP_PORT_MIDBODY_AVS_DOOR_5	=	97;	//PORT_MIDBODY_AVS_DOOR_5
const unsigned int GRP_PORT_MIDBODY_AVS_DOOR_4	=	98;	//PORT_MIDBODY_AVS_DOOR_4
const unsigned int GRP_PORT_MIDBODY_AVS_DOOR_7	=	99;	//PORT_MIDBODY_AVS_DOOR_7
const unsigned int GRP_PORT_AFT_AVS_DOOR_8	=	100;	//PORT_AFT_AVS_DOOR_8
const unsigned int GRP_PORT_AFT_AVS_DOOR_9	=	101;	//PORT_AFT_AVS_DOOR_9
const unsigned int GRP_BAYPFR	=	102;	//BayPFR
const unsigned int GRP_WINDOWS	=	103;	//WINDOWS
const unsigned int GRP_BAY1_LONGERON	=	104;	//Bay1_longeron
const unsigned int GRP_BAY2_LONGERON	=	105;	//Bay2_longeron
const unsigned int GRP_BAY3_LONGERON	=	106;	//Bay3_longeron
const unsigned int GRP_BAY4_LONGERON	=	107;	//Bay4_longeron
const unsigned int GRP_BAY5_LONGERON	=	108;	//Bay5_longeron
const unsigned int GRP_BAY6_LONGERON	=	109;	//Bay6_longeron
const unsigned int GRP_BAY7_LONGERON	=	110;	//Bay7_longeron
const unsigned int GRP_BAY8_LONGERON	=	111;	//Bay8_longeron
const unsigned int GRP_BAY9_LONGERON	=	112;	//Bay9_longeron
const unsigned int GRP_BAY10_LONGERON	=	113;	//Bay10_longeron
const unsigned int GRP_BAY11_LONGERON	=	114;	//Bay11_longeron
const unsigned int GRP_BAY12_LONGERON	=	115;	//Bay12_longeron
const unsigned int GRP_BAY13_LONGERON	=	116;	//Bay13_longeron
const unsigned int GRP_CAM_A	=	117;	//CAM_A
const unsigned int GRP_FWD_HOOKS	=	118;	//FWD_HOOKS
const unsigned int GRP_MID_FWD_HOOKS	=	119;	//MID_FWD_HOOKS
const unsigned int GRP_MID_AFT_HOOKS	=	120;	//MID_AFT_HOOKS
const unsigned int GRP_AFT_HOOKS	=	121;	//AFT_HOOKS
const unsigned int GRP_PLB_WIRE_TRAY	=	122;	//PLB_WIRE_TRAY
const unsigned int GRP_PORT_MIDBODY_AVS_DOOR_3	=	123;	//PORT_MIDBODY_AVS_DOOR_3
const unsigned int GRP_STARBOARD_MIDBODY_AVS_DOOR_3	=	124;	//STARBOARD_MIDBODY_AVS_DOOR_3
const unsigned int GRP_XO1307_BULKHEAD	=	125;	//Xo1307_BULKHEAD
const unsigned int GRP_RIGHT_OUTBOARD_ELEVON	=	126;	//RIGHT_OUTBOARD_ELEVON
const unsigned int GRP_RIGHT_INBOARD_ELEVON	=	127;	//RIGHT_INBOARD_ELEVON
const unsigned int GRP_RIGHT_INBOARD_ELEVON_SEAL_PANEL	=	128;	//RIGHT_INBOARD_ELEVON_SEAL_PANEL
const unsigned int GRP_RIGHT_OUTBOARD_ELEVON_SEAL_PANEL	=	129;	//RIGHT_OUTBOARD_ELEVON_SEAL_PANEL
const unsigned int GRP_LOME	=	130;	//LOME
const unsigned int GRP_RIGHT_SSME	=	131;	//RIGHT_SSME
const unsigned int GRP_LEFT_SSME	=	132;	//LEFT_SSME
const unsigned int GRP_LEFT_OMSPOD	=	133;	//LEFT_OMSpod
const unsigned int GRP_RIGHT_OMSPOD	=	134;	//RIGHT_OMSpod
const unsigned int GRP_XO576_BULKHEAD_EQUIPMENT	=	135;	//Xo576_BULKHEAD_EQUIPMENT
const unsigned int GRP_XO576_BULKHEAD_	=	136;	//Xo576_BULKHEAD 
const unsigned int GRP_ROME	=	137;	//ROME
const unsigned int GRP_LEFT_PLBD_EXT	=	138;	//LEFT_PLBD_EXT
const unsigned int GRP_MIDBODYT0_UMBILICAL_CAVITIES	=	139;	//MIDBODYT0_UMBILICAL_CAVITIES
const unsigned int GRP_MIDBODYT0_UMBILICAL_CAVITIES_TRANSPARENCY	=	140;	//MIDBODYT0_UMBILICAL_CAVITIES_TRANSPARENCY
const unsigned int GRP_PLB_BAY13_COVERS	=	141;	//PLB_BAY13_COVERS
const unsigned int GRP_PLB_SILL_EQUIPMENT	=	142;	//PLB_SILL_EQUIPMENT
const unsigned int GRP_WING_RCC	=	143;	//Wing_RCC

