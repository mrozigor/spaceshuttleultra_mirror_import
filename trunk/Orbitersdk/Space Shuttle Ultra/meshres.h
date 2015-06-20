// ======================================================
// Created by ssumeshc 0.2
// Date of conversion : Sat Jun 20 13:24:37 2015

// Input file: Orbiter.msh
// ======================================================

#pragma once

const unsigned int NUMGRP = 145;

const unsigned int GRP_PLB_BAY13_COVERS	=	0;	//PLB_BAY13_COVERS
const unsigned int GRP_MIDBODY_T0_UMBILICAL_CAVITIES_CUTOUTS	=	1;	//MIDBODY_T0_UMBILICAL_CAVITIES_CUTOUTS
const unsigned int GRP_MIDBODY_T0_UMBILICAL_CAVITIES	=	2;	//MIDBODY_T0_UMBILICAL_CAVITIES
const unsigned int GRP_PLB_TCS	=	3;	//PLB_TCS
const unsigned int GRP_LEFT__OMSPOD	=	4;	//LEFT _OMSpod
const unsigned int GRP_RIGHT_PLBD_EXT	=	5;	//RIGHT_PLBD_EXT
const unsigned int GRP_STBD_PLB_RADIATOR_1	=	6;	//STBD_PLB_RADIATOR_1
const unsigned int GRP_STBD_PLB_RADIATOR_2	=	7;	//STBD_PLB_RADIATOR_2
const unsigned int GRP_STBD_PLB_RADIATOR_3	=	8;	//STBD_PLB_RADIATOR_3
const unsigned int GRP_STBD_PLB_RADIATOR_4	=	9;	//STBD_PLB_RADIATOR_4
const unsigned int GRP_STBD_FWD_RADIATOR_COOLANT_LINES	=	10;	//STBD_FWD_RADIATOR_COOLANT_LINES
const unsigned int GRP_STBD_PLBD_INTERIOR	=	11;	//STBD_PLBD_INTERIOR
const unsigned int GRP_STBDDOOR_PULL	=	12;	//STBDdoor_pull
const unsigned int GRP_STBD_CLAMP	=	13;	//STBD_clamp
const unsigned int GRP_STBDPUSH_ROD	=	14;	//STBDpush_rod
const unsigned int GRP_PORTPUSH_ROD	=	15;	//PORTpush_rod
const unsigned int GRP_PORT_CLAMP	=	16;	//PORT_clamp
const unsigned int GRP_PORTDOOR_PULL	=	17;	//PORTdoor_pull
const unsigned int GRP_RIGHT_OMSPOD	=	18;	//RIGHT_OMSpod
const unsigned int GRP_FUSELAGE	=	19;	//FUSELAGE
const unsigned int GRP_XO576_BULKHEAD_EQUIPMENT	=	20;	//Xo576_BULKHEAD_EQUIPMENT
const unsigned int GRP_XO576_BULKHEAD	=	21;	//Xo576_BULKHEAD
const unsigned int GRP_ROME	=	22;	//ROME
const unsigned int GRP_CENTER_SSME	=	23;	//CENTER_SSME
const unsigned int GRP_LEFT_OUTBOARD_ELEVON_SEAL_PANEL	=	24;	//LEFT_OUTBOARD_ELEVON_SEAL_PANEL
const unsigned int GRP_LEFT_INBOARD_ELEVON_SEAL_PANEL	=	25;	//LEFT_INBOARD_ELEVON_SEAL_PANEL
const unsigned int GRP_LEFT_INBOARD_ELEVON	=	26;	//LEFT_INBOARD_ELEVON
const unsigned int GRP_LEFT_OUTBOARD_ELEVON	=	27;	//LEFT_OUTBOARD_ELEVON
const unsigned int GRP_ELEVON_HINGES	=	28;	//ELEVON_HINGES
const unsigned int GRP_XSTARTRACKER_IMAGE	=	29;	//Xstartracker_image
const unsigned int GRP_ZSTARTRACKER_IMAGE	=	30;	//Zstartracker_image
const unsigned int GRP_STARTRACKER_INDENT	=	31;	//startracker_indent
const unsigned int GRP_ZSTARTRACKER_DOOR	=	32;	//Zstartracker_door
const unsigned int GRP_XSTARTRACKER_DOOR	=	33;	//Xstartracker_door
const unsigned int GRP_TRANSPARENT	=	34;	//transparent
const unsigned int GRP_PORT_PLBD_INTERIOR	=	35;	//PORT_PLBD_INTERIOR
const unsigned int GRP_PLB_SILL_EQUIPMENT	=	36;	//PLB_SILL_EQUIPMENT
const unsigned int GRP_BODYFLAP	=	37;	//BODYFLAP
const unsigned int GRP_AFT_EVA_WINCH_HOOK	=	38;	//AFT_EVA_WINCH_HOOK
const unsigned int GRP_RIGHT_ARCS_JETS	=	39;	//RIGHT_ARCS_JETS
const unsigned int GRP_LEFT_ARCS_JETS	=	40;	//LEFT_ARCS_JETS
const unsigned int GRP_ARCS_JETS_TRANSPARENCY	=	41;	//ARCS_JETS_TRANSPARENCY
const unsigned int GRP_RIGHT_ARCS_POD	=	42;	//RIGHT_ARCS_POD
const unsigned int GRP_OMS_BASE_TILES	=	43;	//OMS_base_tiles
const unsigned int GRP_WING_RCC	=	44;	//Wing_RCC
const unsigned int GRP_LEFT_ARCS_POD	=	45;	//LEFT_ARCS_POD
const unsigned int GRP_PLB_LIGHTS	=	46;	//PLB_LIGHTS
const unsigned int GRP_PLB_VENTS	=	47;	//PLB_VENTS
const unsigned int GRP_PLB_SILL_LONGERONS	=	48;	//PLB_SILL_LONGERONS
const unsigned int GRP_PLB_RING_FRAMES	=	49;	//PLB_RING_FRAMES
const unsigned int GRP_WINGTOPS	=	50;	//Wingtops
const unsigned int GRP_HATCH	=	51;	//hatch
const unsigned int GRP_FRCS_MODULE	=	52;	//FRCS_MODULE
const unsigned int GRP_RIGHT_RSB_PANEL	=	53;	//RIGHT_RSB_PANEL
const unsigned int GRP_LEFT_RSB_PANEL	=	54;	//LEFT_RSB_PANEL
const unsigned int GRP_TAIL_CHUTE	=	55;	//Tail_Chute
const unsigned int GRP_TAIL_NO_CHUTE	=	56;	//Tail_NO_Chute
const unsigned int GRP_PORT_PLB_RADIATOR_4	=	57;	//PORT_PLB_RADIATOR_4
const unsigned int GRP_PORT_PLB_RADIATOR_1	=	58;	//PORT_PLB_RADIATOR_1
const unsigned int GRP_PORT_PLB_RADIATOR_2	=	59;	//PORT_PLB_RADIATOR_2
const unsigned int GRP_PORT_PLB_RADIATOR_3	=	60;	//PORT_PLB_RADIATOR_3
const unsigned int GRP_PORT_FWD_RADIATOR_COOLANT_LINES	=	61;	//PORT_FWD_RADIATOR_COOLANT_LINES
const unsigned int GRP_PANTILT_CAM_A	=	62;	//PANTILT_CAM_A
const unsigned int GRP_CAM_B	=	63;	//CAM_B
const unsigned int GRP_PANTILT_CAM_B	=	64;	//PANTILT_CAM_B
const unsigned int GRP_CAM_C	=	65;	//CAM_C
const unsigned int GRP_PANTILT_CAM_C	=	66;	//PANTILT_CAM_C
const unsigned int GRP_CAM_D	=	67;	//CAM_D
const unsigned int GRP_PANTILT_CAM_D	=	68;	//PANTILT_CAM_D
const unsigned int GRP_AFT_EVA_WINCH_HANDLE	=	69;	//AFT_EVA_WINCH_HANDLE
const unsigned int GRP_FWD_EVA_WINCH_HOOK	=	70;	//FWD_EVA_WINCH_HOOK
const unsigned int GRP_FWD_EVA_WINCH_HANDLE	=	71;	//FWD_EVA_WINCH_HANDLE
const unsigned int GRP_EVA_THERMAL_COVER	=	72;	//EVA_THERMAL_COVER
const unsigned int GRP_NLG_UPPER_TORQUE_ARM	=	73;	//NLG_UPPER_TORQUE_ARM
const unsigned int GRP_NLG_LOWER_TORQUE_ARM	=	74;	//NLG_LOWER_TORQUE_ARM
const unsigned int GRP_NLG_STRUT	=	75;	//NLG_STRUT
const unsigned int GRP_NLG_SHOCK_STRUT	=	76;	//NLG_SHOCK_STRUT
const unsigned int GRP_NOSE_WHEELS	=	77;	//NOSE_WHEELS
const unsigned int GRP_LEFT_MN_WHEELS	=	78;	//LEFT_MN_WHEELS
const unsigned int GRP_LEFT_MLG_STRUT	=	79;	//LEFT_MLG_STRUT
const unsigned int GRP_LEFT_MLG_SHOCK_STRUT	=	80;	//LEFT_MLG_SHOCK_STRUT
const unsigned int GRP_RIGHT_MN_WHEELS	=	81;	//RIGHT_MN_WHEELS
const unsigned int GRP_RIGHT_MLG_STRUT	=	82;	//RIGHT_MLG_STRUT
const unsigned int GRP_RIGHT_MLG_SHOCK_STRUT	=	83;	//RIGHT_MLG_SHOCK_STRUT
const unsigned int GRP_LEFT_NLG_DOOR	=	84;	//LEFT_NLG_DOOR
const unsigned int GRP_RIGHT_NLG_DOOR	=	85;	//RIGHT_NLG_DOOR
const unsigned int GRP_LEFT_MLG_DOOR	=	86;	//LEFT_MLG_DOOR
const unsigned int GRP_RIGHT_MLG_DOOR	=	87;	//RIGHT_MLG_DOOR
const unsigned int GRP_PROBEL	=	88;	//PROBEL
const unsigned int GRP_PROBER	=	89;	//PROBER
const unsigned int GRP_SSME_HEATSHIELDS	=	90;	//SSME_HEATSHIELDS
const unsigned int GRP_ET_DOOR_LEFT	=	91;	//ET_DOOR_LEFT
const unsigned int GRP_ET_DOOR_LEFT_DDM_ARM_AFT	=	92;	//ET_DOOR_LEFT_DDM_ARM_AFT
const unsigned int GRP_ET_DOOR_LEFT_DDM_ARM_FWD	=	93;	//ET_DOOR_LEFT_DDM_ARM_FWD
const unsigned int GRP_ET_DOOR_LEFT_DDM_LINK_ROD_AFT	=	94;	//ET_DOOR_LEFT_DDM_LINK_ROD_AFT
const unsigned int GRP_ET_DOOR_LEFT_DDM_LINK_ROD_FWD	=	95;	//ET_DOOR_LEFT_DDM_LINK_ROD_FWD
const unsigned int GRP_ET_DOOR_RIGHT_DDM_LINK_ROD_FWD	=	96;	//ET_DOOR_RIGHT_DDM_LINK_ROD_FWD
const unsigned int GRP_ET_DOOR_RIGHT_DDM_LINK_ROD_AFT	=	97;	//ET_DOOR_RIGHT_DDM_LINK_ROD_AFT
const unsigned int GRP_ET_DOOR_RIGHT_DDM_ARM_FWD	=	98;	//ET_DOOR_RIGHT_DDM_ARM_FWD
const unsigned int GRP_ET_DOOR_RIGHT_DDM_ARM_AFT	=	99;	//ET_DOOR_RIGHT_DDM_ARM_AFT
const unsigned int GRP_ET_DOOR_RIGHT	=	100;	//ET_DOOR_RIGHT
const unsigned int GRP_STARBOARD_MIDBODY_AVS_DOOR_7	=	101;	//STARBOARD_MIDBODY_AVS_DOOR_7
const unsigned int GRP_STARBOARD_MIDBODY_AVS_DOOR_4	=	102;	//STARBOARD_MIDBODY_AVS_DOOR_4
const unsigned int GRP_STARBOARD_MIDBODY_AVS_DOOR_5	=	103;	//STARBOARD_MIDBODY_AVS_DOOR_5
const unsigned int GRP_STARBOARD_MIDBODY_AVS_DOOR_6	=	104;	//STARBOARD_MIDBODY_AVS_DOOR_6
const unsigned int GRP_STARBOARD_AFT_AVS_DOOR_8	=	105;	//STARBOARD_AFT_AVS_DOOR_8
const unsigned int GRP_STARBOARD_AFT_AVS_DOOR_9	=	106;	//STARBOARD_AFT_AVS_DOOR_9
const unsigned int GRP_PORT_MIDBODY_AVS_DOOR_6	=	107;	//PORT_MIDBODY_AVS_DOOR_6
const unsigned int GRP_PORT_MIDBODY_AVS_DOOR_5	=	108;	//PORT_MIDBODY_AVS_DOOR_5
const unsigned int GRP_PORT_MIDBODY_AVS_DOOR_4	=	109;	//PORT_MIDBODY_AVS_DOOR_4
const unsigned int GRP_PORT_MIDBODY_AVS_DOOR_7	=	110;	//PORT_MIDBODY_AVS_DOOR_7
const unsigned int GRP_PORT_AFT_AVS_DOOR_8	=	111;	//PORT_AFT_AVS_DOOR_8
const unsigned int GRP_PORT_AFT_AVS_DOOR_9	=	112;	//PORT_AFT_AVS_DOOR_9
const unsigned int GRP_BAYPFR	=	113;	//BayPFR
const unsigned int GRP_WINDOWS	=	114;	//WINDOWS
const unsigned int GRP_BAY1_LONGERON	=	115;	//Bay1_longeron
const unsigned int GRP_BAY2_LONGERON	=	116;	//Bay2_longeron
const unsigned int GRP_BAY3_LONGERON	=	117;	//Bay3_longeron
const unsigned int GRP_BAY4_LONGERON	=	118;	//Bay4_longeron
const unsigned int GRP_BAY5_LONGERON	=	119;	//Bay5_longeron
const unsigned int GRP_BAY6_LONGERON	=	120;	//Bay6_longeron
const unsigned int GRP_BAY7_LONGERON	=	121;	//Bay7_longeron
const unsigned int GRP_BAY8_LONGERON	=	122;	//Bay8_longeron
const unsigned int GRP_BAY9_LONGERON	=	123;	//Bay9_longeron
const unsigned int GRP_BAY10_LONGERON	=	124;	//Bay10_longeron
const unsigned int GRP_BAY11_LONGERON	=	125;	//Bay11_longeron
const unsigned int GRP_BAY12_LONGERON	=	126;	//Bay12_longeron
const unsigned int GRP_BAY13_LONGERON	=	127;	//Bay13_longeron
const unsigned int GRP_CAM_A	=	128;	//CAM_A
const unsigned int GRP_LEFT_PLBD_EXT	=	129;	//LEFT_PLBD_EXT
const unsigned int GRP_FWD_HOOKS	=	130;	//FWD_HOOKS
const unsigned int GRP_MID_FWD_HOOKS	=	131;	//MID_FWD_HOOKS
const unsigned int GRP_MID_AFT_HOOKS	=	132;	//MID_AFT_HOOKS
const unsigned int GRP_AFT_HOOKS	=	133;	//AFT_HOOKS
const unsigned int GRP_PLB_WIRE_TRAY	=	134;	//PLB_WIRE_TRAY
const unsigned int GRP_PORT_MIDBODY_AVS_DOOR_3	=	135;	//PORT_MIDBODY_AVS_DOOR_3
const unsigned int GRP_STARBOARD_MIDBODY_AVS_DOOR_3	=	136;	//STARBOARD_MIDBODY_AVS_DOOR_3
const unsigned int GRP_XO1307_BULKHEAD	=	137;	//Xo1307_BULKHEAD
const unsigned int GRP_RIGHT_OUTBOARD_ELEVON	=	138;	//RIGHT_OUTBOARD_ELEVON
const unsigned int GRP_RIGHT_INBOARD_ELEVON	=	139;	//RIGHT_INBOARD_ELEVON
const unsigned int GRP_RIGHT_INBOARD_ELEVON_SEAL_PANEL	=	140;	//RIGHT_INBOARD_ELEVON_SEAL_PANEL
const unsigned int GRP_RIGHT_OUTBOARD_ELEVON_SEAL_PANEL	=	141;	//RIGHT_OUTBOARD_ELEVON_SEAL_PANEL
const unsigned int GRP_LOME	=	142;	//LOME
const unsigned int GRP_RIGHT_SSME	=	143;	//RIGHT_SSME
const unsigned int GRP_LEFT_SSME	=	144;	//LEFT_SSME

