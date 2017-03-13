/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel A8 VC implementation



  Space Shuttle Ultra is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Space Shuttle Ultra is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Space Shuttle Ultra; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See http://spaceshuttleultra.sourceforge.net/license/ for more details.

  **************************************************************************/

#include "PanelA8.h"
#include "../meshres_vc_a8.h"
#include "../Atlantis_defs.h"
#include "../CommonDefs.h"
#include "..\Atlantis.h"
#include <UltraMath.h>

namespace vc
{
	PanelA8::PanelA8(Atlantis* _sts)
		: AtlantisPanel(_sts, "A8")
	{
		hPanelMesh=oapiLoadMeshGlobal(DEFAULT_MESHNAME_PANELA8);
		mesh_index=MESH_UNDEFINED;

		Add(pPortMPMCover = new StandardSwitchCover(_sts, "Port MPM Deploy Cover"));
		Add(pStbdMPMCover = new StandardSwitchCover(_sts, "Stbd MPM Deploy Cover"));

		Add(pPortMRL = new StdSwitch3(_sts, "Port RMS Latches"));
		Add(pStbdMRL = new StdSwitch3(_sts, "Stbd MPM Latches"));
		Add(pPortMPM = new StdSwitch3(_sts, "Port MPM Deploy"));
		Add(pStbdMPM = new StdSwitch3(_sts, "Stbd MPM Deploy"));
		Add(pEEMode = new StdSwitch3(_sts, "EE Mode"));
		Add(pEEManContr = new StdSwitch3(_sts, "EE Man Contr"));
		Add(pShoulderBrace = new StdSwitch3(_sts, "Shoulder Brace Release"));
		Add(pRMSSelect = new StdSwitch3(_sts, "RMS SELECT"));
		Add(pSingleDirectDrive = new StdSwitch3(_sts, "Single/Direct Drive"));

		Add( pPortMRLTb = new StandardTalkback3( _sts, "Port MRL Talkback" ) );
		Add( pStbdMRLTb = new StandardTalkback3( _sts, "Stbd MRL Talkback" ) );
		Add( pPortMPMTb = new StandardTalkback3( _sts, "Port MPM Talkback" ) );
		Add( pStbdMPMTb = new StandardTalkback3( _sts, "Stbd MPM Talkback" ) );

		Add( pPortMRL_RTL[0] = new StandardTalkback2( _sts, "Port MRL RTL FWD" ) );
		Add( pPortMRL_RTL[1] = new StandardTalkback2( _sts, "Port MRL RTL MID" ) );
		Add( pPortMRL_RTL[2] = new StandardTalkback2( _sts, "Port MRL RTL AFT" ) );
		Add( pStbdMRL_RTL[0] = new StandardTalkback2( _sts, "Stbd MRL RTL FWD" ) );
		Add( pStbdMRL_RTL[1] = new StandardTalkback2( _sts, "Stbd MRL RTL MID" ) );
		Add( pStbdMRL_RTL[2] = new StandardTalkback2( _sts, "Stbd MRL RTL AFT" ) );
		Add( pEECapture = new StandardTalkback2( _sts, "EE Capture" ) );
		Add( pEEExtend = new StandardTalkback2( _sts, "EE Extend" ) );
		Add( pEEClose = new StandardTalkback2( _sts, "EE Close" ) );
		Add( pEEOpen = new StandardTalkback2( _sts, "EE Open" ) );
		Add( pEERigid = new StandardTalkback2( _sts, "EE Rigid" ) );
		Add( pEEDerigid = new StandardTalkback2( _sts, "EE Derigid" ) );
		Add( pShoulderBraceTb = new StandardTalkback2( _sts, "Shoulder Brace" ) );
		Add( pSoftStopTB = new StandardTalkback2( _sts, "Software Stop" ) );

		Add(pLEDParameter = new RotaryDemuxSwitch(_sts, "Parameter", 8));
		Add(pLEDJoint = new RotaryDemuxSwitch(_sts, "Joint", 8));
		Add(pRMSMode = new RotaryDemuxSwitch(_sts, "RMS Mode", 12));
		
		Add( pModeLights[0] = new StandardSingleLight( _sts, "TEST" ) );
		Add( pModeLights[1] = new StandardSingleLight( _sts, "DIRECT" ) );
		Add( pModeLights[2] = new StandardSingleLight( _sts, "SINGLE" ) );
		Add( pModeLights[3] = new StandardSingleLight( _sts, "PL" ) );
		Add( pModeLights[4] = new StandardSingleLight( _sts, "ORB_LD" ) );
		Add( pModeLights[5] = new StandardSingleLight( _sts, "END_EFF" ) );
		Add( pModeLights[6] = new StandardSingleLight( _sts, "ORB_UNL" ) );
		Add( pModeLights[7] = new StandardSingleLight( _sts, "AUTO_4" ) );
		Add( pModeLights[8] = new StandardSingleLight( _sts, "AUTO_3" ) );
		Add( pModeLights[9] = new StandardSingleLight( _sts, "AUTO_2" ) );
		Add( pModeLights[10] = new StandardSingleLight( _sts, "AUTO_1" ) );
		Add( pModeLights[11] = new StandardSingleLight( _sts, "OPR_CMD" ) );

		Add( pCWLights[0] = new StandardSingleLight( _sts, "MCIU" ) );
		Add( pCWLights[1] = new StandardSingleLight( _sts, "ABE" ) );
		Add( pCWLights[2] = new StandardSingleLight( _sts, "GPC_DATA" ) );
		Add( pCWLights[3] = new StandardSingleLight( _sts, "SINGULAR" ) );
		Add( pCWLights[4] = new StandardSingleLight( _sts, "CONTR_ERR" ) );
		Add( pCWLights[5] = new StandardSingleLight( _sts, "STBD_TEMP" ) );
		Add( pCWLights[6] = new StandardSingleLight( _sts, "DERIZIDIZE" ) );
		Add( pCWLights[7] = new StandardSingleLight( _sts, "RELEASE" ) );
		Add( pCWLights[8] = new StandardSingleLight( _sts, "CHECK_CRT" ) );
		Add( pCWLights[9] = new StandardSingleLight( _sts, "REACH_LIM" ) );
		Add( pCWLights[10] = new StandardSingleLight( _sts, "PORT_TEMP" ) );

		Add( pSequenceLights[0] = new StandardSingleLight( _sts, "READY" ) );
		Add( pSequenceLights[1] = new StandardSingleLight( _sts, "IN_PROG" ) );

		Add( pMasterAlarm = new PushButtonIndicatorSingleLight( _sts, "MASTER ALARM" ) );

		Add( pRMS = new _7SegDisp_RMS( _sts, "RMS" ) );

		pPortMRL->SetLabel(2, "REL");
		pPortMRL->SetLabel(1, "OFF");
		pPortMRL->SetLabel(0, "LAT");
		pStbdMRL->SetLabel(2, "REL");
		pStbdMRL->SetLabel(1, "OFF");
		pStbdMRL->SetLabel(0, "LAT");
		pPortMPM->SetLabel(2, "DEPLOY");
		pPortMPM->SetLabel(1, "OFF");
		pPortMPM->SetLabel(0, "STOW");
		pStbdMPM->SetLabel(2, "DEPLOY");
		pStbdMPM->SetLabel(1, "OFF");
		pStbdMPM->SetLabel(0, "STOW");
		pEEMode->SetLabel(2, "AUTO");
		pEEMode->SetLabel(1, "OFF");
		pEEMode->SetLabel(0, "MAN");
		pEEManContr->SetLabel(2, "RIGID");
		pEEManContr->SetLabel(1, "OFF");
		pEEManContr->SetLabel(0, "DERIGID");
		pShoulderBrace->SetLabel(2, "PORT");
		pShoulderBrace->SetLabel(1, "OFF");
		pShoulderBrace->SetLabel(0, "STBD");
		pRMSSelect->SetLabel(2, "PORT");
		pRMSSelect->SetLabel(1, "OFF");
		pRMSSelect->SetLabel(0, "STBD");
		pSingleDirectDrive->SetLabel(2, "+");
		pSingleDirectDrive->SetLabel(1, "OFF");
		pSingleDirectDrive->SetLabel(0, "-");

		pLEDParameter->SetLabel(7, "TEST");
		pLEDParameter->SetLabel(6, "POSITION");
		pLEDParameter->SetLabel(5, "ATTITUDE");
		pLEDParameter->SetLabel(4, "JOINT_ANGLE");
		pLEDParameter->SetLabel(3, "VEL");
		pLEDParameter->SetLabel(2, "RATE");
		pLEDParameter->SetLabel(1, "PORT_TEMP");
		pLEDParameter->SetLabel(0, "STBD_TEMP");
		pLEDParameter->SetInitialPosition( 6 );
		pLEDJoint->SetLabel(7, "SHOULDER_YAW");
		pLEDJoint->SetLabel(6, "SHOULDER_PITCH");
		pLEDJoint->SetLabel(5, "ELBOW");
		pLEDJoint->SetLabel(4, "WRIST_PITCH");
		pLEDJoint->SetLabel(3, "WRIST_YAW");
		pLEDJoint->SetLabel(2, "WRIST_ROLL");
		pLEDJoint->SetLabel(1, "EE_TEMP");
		pLEDJoint->SetLabel(0, "CRIT_TEMP");
		pRMSMode->SetLabel(11, "OPR_CMD");
		pRMSMode->SetLabel(10, "AUTO_1");
		pRMSMode->SetLabel(9, "AUTO_2");
		pRMSMode->SetLabel(8, "AUTO_3");
		pRMSMode->SetLabel(7, "AUTO_4");
		pRMSMode->SetLabel(6, "ORB_UNL");
		pRMSMode->SetLabel(5, "END_EFF");
		pRMSMode->SetLabel(4, "ORB_LD");
		pRMSMode->SetLabel(3, "PL");
		pRMSMode->SetLabel(2, "SINGLE");
		pRMSMode->SetLabel(1, "DIRECT");
		pRMSMode->SetLabel(0, "TEST");
	}

	PanelA8::~PanelA8()
	{
	}

	void PanelA8::AddMeshes(const VECTOR3 &ofs)
	{
		SetHasOwnVCMesh();

		if(mesh_index==MESH_UNDEFINED) {
			mesh_index=STS()->AddMesh(hPanelMesh, &ofs);
			STS()->SetMeshVisibilityMode(mesh_index, MESHVIS_VC);
		}
	}

	void PanelA8::SetMeshVisibility(bool visible)
	{
		if(visible) STS()->SetMeshVisibilityMode(mesh_index, MESHVIS_VC);
		else STS()->SetMeshVisibilityMode(mesh_index, MESHVIS_NEVER);
	}

	UINT PanelA8::GetVCMeshIndex() const
	{
		return mesh_index;
	}

	void PanelA8::RegisterVC()
	{
		oapiWriteLog("PanelA8::RegisterVC() called");
		AtlantisPanel::RegisterVC();
		oapiWriteLog("PanelA8::AtlantisPanel::RegisterVC() called");

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;
		oapiVCRegisterArea(AID_A8, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP);
		oapiVCSetAreaClickmode_Quadrilateral (AID_A8,
			_V(-0.797, 2.893, 12.277)+ofs, _V(-0.266, 2.893, 12.277)+ofs, 
			_V(-0.797, 2.119, 12.522)+ofs, _V(-0.266, 2.119, 12.522)+ofs);

		return;
	}

	void PanelA8::DefineVC()
	{
		const VECTOR3 switch_rot_vert=_V(-1.0, 0.0, 0.0);
		const VECTOR3 switch_rot_horz=_V(0.0, -0.9499, 0.3126);
		const VECTOR3 rotary_switch_rot = _V(0.0, -0.3126, -0.9499);
		const VECTOR3 push_dir = rotary_switch_rot;

		AddAIDToMouseEventList(AID_A8);

		pPortMPMCover->SetMouseRegion(0, 0.252639f, 0.834143f, 0.305916f, 0.863428f);
		pPortMPMCover->SetMouseRegion(1, 0.235794f, 0.804317f, 0.298074f, 0.834143f);
		pPortMPMCover->SetReference(_V(-0.646, 2.257, 12.478), switch_rot_vert);
		pPortMPMCover->DefineCoverGroup(GRP_A8SCOVER5_A8_VC);

		pStbdMPMCover->SetMouseRegion(0, 0.841143f, 0.836290f, 0.886384f, 0.867186f);
		pStbdMPMCover->SetMouseRegion(1, 0.841143f, 0.806764f, 0.886384f, 0.833786f);
		pStbdMPMCover->SetReference(_V(-0.339, 2.257, 12.479), switch_rot_vert);
		pStbdMPMCover->DefineCoverGroup(GRP_A8SCOVER1_A8_VC);

		pPortMPM->SetMouseRegion(0.252639f, 0.834143f, 0.305916f, 0.863428f);
		pPortMPM->SetReference(_V(-0.647, 2.238, 12.486), switch_rot_vert);
		pPortMPM->DefineSwitchGroup(GRP_A8S17_A8_VC);
		pPortMPM->SetInitialAnimState(0.5f);

		pStbdMPM->SetMouseRegion(0.841143f, 0.836290f, 0.886384f, 0.867186f);
		pStbdMPM->SetReference(_V(-0.693, 2.233, 12.486), switch_rot_vert);
		pStbdMPM->DefineSwitchGroup(GRP_A8S12_A8_VC);
		pStbdMPM->SetInitialAnimState(0.5f);

		pPortMRL->SetMouseRegion(0.167373f, 0.841234f, 0.217567f, 0.867132f);
		pPortMRL->SetReference(_V(-0.693, 2.233, 12.486), switch_rot_vert);
		pPortMRL->DefineSwitchGroup(GRP_A8S18_A8_VC);
		pPortMRL->SetInitialAnimState(0.5f);

		pStbdMRL->SetMouseRegion(0.755850f, 0.842015f, 0.801863f, 0.880786f);
		pStbdMRL->SetReference(_V(-0.384, 2.233, 12.487), switch_rot_vert);
		pStbdMRL->DefineSwitchGroup(GRP_A8S13_A8_VC);
		pStbdMRL->SetInitialAnimState(0.5f);

		pEEMode->SetMouseRegion(0.298233f, 0.2227635f, 0.346051f, 0.250502f);
		pEEMode->SetReference(_V(-0.625, 2.709, 12.337), switch_rot_vert);
		pEEMode->DefineSwitchGroup(GRP_A8S4_A8_VC);
		pEEMode->SetInitialAnimState(0.5f);

		pEEManContr->SetMouseRegion(0.243867f, 0.22259f, 0.288102f, 0.252219f);
		pEEManContr->SetReference(_V(-0.654, 2.709, 12.337), switch_rot_vert);
		pEEManContr->DefineSwitchGroup(GRP_A8S5_A8_VC);
		pEEManContr->SetInitialAnimState(0.5f);
		pEEManContr->SetSpringLoaded(true);

		pShoulderBrace->SetMouseRegion(0.170449f, 0.618925f, 0.226497f, 0.651137f);
		pShoulderBrace->SetReference(_V(-0.687, 2.402, 12.434), switch_rot_horz);
		pShoulderBrace->DefineSwitchGroup(GRP_A8S10_A8_VC);
		pShoulderBrace->SetInitialAnimState(0.5f);
		pShoulderBrace->SetOrientation(true);
		pShoulderBrace->SetSpringLoaded(true);

		pSingleDirectDrive->SetMouseRegion(0.142212f, 0.547297f, 0.208286f, 0.581384f);
		pSingleDirectDrive->SetReference(_V(-0.698, 2.458, 12.417), switch_rot_vert);
		pSingleDirectDrive->DefineSwitchGroup(GRP_A8S9_A8_VC);
		pSingleDirectDrive->SetInitialAnimState(0.5f);
		pSingleDirectDrive->SetSpringLoaded(true);

		pRMSSelect->SetMouseRegion(0.466754f, 0.757090f, 0.510409f, 0.788049f);
		pRMSSelect->SetReference(_V(-0.537, 2.294, 12.467), switch_rot_horz);
		pRMSSelect->DefineSwitchGroup(GRP_A8S11_A8_VC);
		pRMSSelect->SetInitialAnimState(0.5f);
		pRMSSelect->SetOrientation(true);

		pPortMRLTb->DefineMeshGroup( mesh_index, GRP_A8L_DS5_A8_VC );

		pStbdMRLTb->DefineMeshGroup( mesh_index, GRP_A8L_DS2_A8_VC );

		pPortMPMTb->DefineMeshGroup( mesh_index, GRP_A8L_DS4_A8_VC );

		pStbdMPMTb->DefineMeshGroup( mesh_index, GRP_A8L_DS1_A8_VC );

		pStbdMRL_RTL[0]->DefineMeshGroups( mesh_index, GRP_A8L_DS9_U_A8_VC, GRP_A8L_DS9_L_A8_VC );
		pStbdMRL_RTL[1]->DefineMeshGroups( mesh_index, GRP_A8L_DS7_U_A8_VC, GRP_A8L_DS7_L_A8_VC );
		pStbdMRL_RTL[2]->DefineMeshGroups( mesh_index, GRP_A8L_DS3_U_A8_VC, GRP_A8L_DS3_L_A8_VC );
		
		pPortMRL_RTL[0]->DefineMeshGroups( mesh_index, GRP_A8L_DS10_U_A8_VC, GRP_A8L_DS10_L_A8_VC );
		pPortMRL_RTL[1]->DefineMeshGroups( mesh_index, GRP_A8L_DS8_U_A8_VC, GRP_A8L_DS8_L_A8_VC );
		pPortMRL_RTL[2]->DefineMeshGroups( mesh_index, GRP_A8L_DS6_U_A8_VC, GRP_A8L_DS6_L_A8_VC );

		pShoulderBraceTb->DefineMeshGroups( mesh_index, GRP_A8U_DS13_U_A8_VC, GRP_A8U_DS13_L_A8_VC );

		pEECapture->DefineMeshGroups( mesh_index, GRP_A8U_DS9_U_A8_VC, GRP_A8U_DS9_L_A8_VC );

		pEEExtend->DefineMeshGroups( mesh_index, GRP_A8U_DS12_U_A8_VC, GRP_A8U_DS12_L_A8_VC );

		pEEClose->DefineMeshGroups( mesh_index, GRP_A8U_DS8_U_A8_VC, GRP_A8U_DS8_L_A8_VC );

		pEEOpen->DefineMeshGroups( mesh_index, GRP_A8U_DS11_U_A8_VC, GRP_A8U_DS11_L_A8_VC );

		pEERigid->DefineMeshGroups( mesh_index, GRP_A8U_DS7_U_A8_VC, GRP_A8U_DS7_L_A8_VC );

		pEEDerigid->DefineMeshGroups( mesh_index, GRP_A8U_DS10_U_A8_VC, GRP_A8U_DS10_L_A8_VC );

		pSoftStopTB->DefineMeshGroups( mesh_index, GRP_A8U_DS3_U_A8_VC, GRP_A8U_DS3_L_A8_VC );

		pLEDParameter->SetMouseRegion(0.45049f, 0.440562f, 0.531691f, 0.494226f);
		pLEDParameter->DefineSwitchGroup(GRP_A8RS3_A8_VC);
		pLEDParameter->SetReference(_V(-0.5354, 2.534, 12.4013), rotary_switch_rot);
		pLEDParameter->SetInitialAnimState(1.0f);
		pLEDParameter->DefineRotationAngle(210.0f);
		pLEDParameter->SetOffset(-90.0f);

		pLEDJoint->SetMouseRegion(0.13677f, 0.439533f, 0.223088f, 0.494112f);
		pLEDJoint->DefineSwitchGroup(GRP_A8RS4_A8_VC);
		pLEDJoint->SetReference(_V(-0.6984, 2.5339, 12.4013), rotary_switch_rot);
		pLEDJoint->SetInitialAnimState(1.0f);
		pLEDJoint->DefineRotationAngle(210.0f);
		pLEDJoint->SetOffset(-150.0f);

		pRMSMode->SetMouseRegion(0.77292f, 0.177526f, 0.855731f, 0.234343f);
		pRMSMode->DefineSwitchGroup(GRP_A8RS1_A8_VC);
		pRMSMode->SetReference(_V(-0.3639, 2.7365, 12.3378), rotary_switch_rot);
		pRMSMode->SetInitialAnimState(1.0f);
		pRMSMode->SetWraparound(true);
		pRMSMode->DefineRotationAngle(330.0f);
		pRMSMode->SetOffset(-180.0f);

		// lights
		// auto 1
		pModeLights[10]->DefineMeshGroup( mesh_index, GRP_A8AUTO1_A8_VC );
		pModeLights[10]->SetStateOffset( 1, 0.0f, 0.390625f );
		// auto 2
		pModeLights[9]->DefineMeshGroup( mesh_index, GRP_A8AUTO2_A8_VC );
		pModeLights[9]->SetStateOffset( 1, 0.0f, 0.390625f );
		// auto 3
		pModeLights[8]->DefineMeshGroup( mesh_index, GRP_A8AUTO3_A8_VC );
		pModeLights[8]->SetStateOffset( 1, 0.0f, 0.390625f );
		// auto 4
		pModeLights[7]->DefineMeshGroup( mesh_index, GRP_A8AUTO4_A8_VC );
		pModeLights[7]->SetStateOffset( 1, 0.0f, 0.390625f );
		// opr cmd
		pModeLights[11]->DefineMeshGroup( mesh_index, GRP_A8OPRCMD_A8_VC );
		pModeLights[11]->SetStateOffset( 1, 0.0f, 0.390625f );
		// test
		pModeLights[0]->DefineMeshGroup( mesh_index, GRP_A8TEST_A8_VC );
		pModeLights[0]->SetStateOffset( 1, 0.0f, 0.390625f );
		// orb unl
		pModeLights[6]->DefineMeshGroup( mesh_index, GRP_A8ORBUNL_A8_VC );
		pModeLights[6]->SetStateOffset( 1, 0.0f, 0.390625f );
		// end eff
		pModeLights[5]->DefineMeshGroup( mesh_index, GRP_A8EE_A8_VC );
		pModeLights[5]->SetStateOffset( 1, 0.0f, 0.390625f );
		// orb ld
		pModeLights[4]->DefineMeshGroup( mesh_index, GRP_A8ORBLD_A8_VC );
		pModeLights[4]->SetStateOffset( 1, 0.0f, 0.390625f );
		// payload
		pModeLights[3]->DefineMeshGroup( mesh_index, GRP_A8PL_A8_VC );
		pModeLights[3]->SetStateOffset( 1, 0.0f, 0.390625f );
		// single
		pModeLights[2]->DefineMeshGroup( mesh_index, GRP_A8SINGLE_A8_VC );
		pModeLights[2]->SetStateOffset( 1, 0.0f, 0.390625f );
		// direct
		pModeLights[1]->DefineMeshGroup( mesh_index, GRP_A8DIRECT_A8_VC );
		pModeLights[1]->SetStateOffset( 1, 0.0f, 0.390625f );
		// mciu
		pCWLights[0]->DefineMeshGroup( mesh_index, GRP_A8MCIU_A8_VC );
		pCWLights[0]->SetStateOffset( 1, 0.0f, 0.390625f );
		// abe
		pCWLights[1]->DefineMeshGroup( mesh_index, GRP_A8ABE_A8_VC );
		pCWLights[1]->SetStateOffset( 1, 0.0f, 0.390625f );
		// gpc data
		pCWLights[2]->DefineMeshGroup( mesh_index, GRP_A8GPCDATA_A8_VC );
		pCWLights[2]->SetStateOffset( 1, 0.0f, 0.390625f );
		// singular
		pCWLights[3]->DefineMeshGroup( mesh_index, GRP_A8SINGULAR_A8_VC );
		pCWLights[3]->SetStateOffset( 1, 0.0f, 0.390625f );
		// contr err
		pCWLights[4]->DefineMeshGroup( mesh_index, GRP_A8CONTRERR_A8_VC );
		pCWLights[4]->SetStateOffset( 1, 0.0f, 0.390625f );
		// stbd temp
		pCWLights[5]->DefineMeshGroup( mesh_index, GRP_A8STBDTEMP_A8_VC );
		pCWLights[5]->SetStateOffset( 1, 0.0f, 0.390625f );
		// derizidize
		pCWLights[6]->DefineMeshGroup( mesh_index, GRP_A8DERIGID_A8_VC );
		pCWLights[6]->SetStateOffset( 1, 0.0f, 0.390625f );
		// release
		pCWLights[7]->DefineMeshGroup( mesh_index, GRP_A8RELEASE_A8_VC );
		pCWLights[7]->SetStateOffset( 1, 0.0f, 0.390625f );
		// check crt
		pCWLights[8]->DefineMeshGroup( mesh_index, GRP_A8CHKCRT_A8_VC );
		pCWLights[8]->SetStateOffset( 1, 0.0f, 0.390625f );
		// reach lim
		pCWLights[9]->DefineMeshGroup( mesh_index, GRP_A8REACHLIM_A8_VC );
		pCWLights[9]->SetStateOffset( 1, 0.0f, 0.390625f );
		// port temp
		pCWLights[10]->DefineMeshGroup( mesh_index, GRP_A8PORTTEMP_A8_VC );
		pCWLights[10]->SetStateOffset( 1, 0.0f, 0.390625f );
		// ready
		pSequenceLights[0]->DefineMeshGroup( mesh_index, GRP_A8READY_A8_VC );
		pSequenceLights[0]->SetStateOffset( 1, 0.0f, 0.158203f );
		// in prog
		pSequenceLights[1]->DefineMeshGroup( mesh_index, GRP_A8INPROG_A8_VC );
		pSequenceLights[1]->SetStateOffset( 1, 0.0f, 0.158203f );

		pMasterAlarm->SetStateOffset( 1, 0.0f, 0.113281f );
		pMasterAlarm->SetDirection( push_dir );
		pMasterAlarm->SetMouseRegion( 0.473079f, 0.225946f, 0.515116f, 0.246649f );
		pMasterAlarm->DefineMeshGroup( mesh_index, GRP_A8MASALARM_A8_VC );

		pRMS->DefineMesh( mesh_index );
		pRMS->DefineComponent( GRP_M2_1_A8_VC, true, true, false, _7SD_STATE_NUM0_DOTOFF );
		pRMS->DefineComponent( GRP_M2_2_A8_VC, true, true, false, _7SD_STATE_NUM0_DOTOFF );
		pRMS->DefineComponent( GRP_M2_3_A8_VC, true, true, false, _7SD_STATE_NUM0_DOTOFF );
		pRMS->DefineComponent( GRP_M2_4_A8_VC, true, true, false, _7SD_STATE_NUM0_DOTOFF );
		pRMS->DefineComponent( GRP_M2_SIGN_A8_VC, false, false, false, _7SD_STATE_SIGNOFF );
		pRMS->DefineComponent( GRP_M3_1_A8_VC, true, true, false, _7SD_STATE_NUM0_DOTOFF );
		pRMS->DefineComponent( GRP_M3_2_A8_VC, true, true, false, _7SD_STATE_NUM0_DOTOFF );
		pRMS->DefineComponent( GRP_M3_3_A8_VC, true, true, false, _7SD_STATE_NUM0_DOTOFF );
		pRMS->DefineComponent( GRP_M3_4_A8_VC, true, true, false, _7SD_STATE_NUM0_DOTOFF );
		pRMS->DefineComponent( GRP_M3_SIGN_A8_VC, false, false, false, _7SD_STATE_SIGNOFF );
		pRMS->DefineComponent( GRP_M4_1_A8_VC, true, true, false, _7SD_STATE_NUM0_DOTOFF );
		pRMS->DefineComponent( GRP_M4_2_A8_VC, true, true, false, _7SD_STATE_NUM0_DOTOFF );
		pRMS->DefineComponent( GRP_M4_3_A8_VC, true, true, false, _7SD_STATE_NUM0_DOTOFF );
		pRMS->DefineComponent( GRP_M4_4_A8_VC, true, true, false, _7SD_STATE_NUM0_DOTOFF );
		pRMS->DefineComponent( GRP_M4_SIGN_A8_VC, false, false, false, _7SD_STATE_SIGNOFF );
	}

	void PanelA8::DefineVCAnimations(UINT vcidx)
	{
		// call DefineVCAnimations using correct index
		AtlantisPanel::DefineVCAnimations(mesh_index);
	}

	void PanelA8::Realize()
	{
		DiscreteBundle* pBundle=STS()->BundleManager()->CreateBundle("RMS", 16);
		pPortMPM->outputB.Connect(pBundle, 0);
		pPortMPM->outputA.Connect(pBundle, 1);
		pPortMPMTb->SetInput(0, pBundle, 2, TB_DPY);
		pPortMPMTb->SetInput(1, pBundle, 3, TB_STO);
		// for the moment, ignore STBD connections for shoulder brace
		pShoulderBrace->outputB.Connect(pBundle, 4);
		pShoulderBraceTb->SetInput( pBundle, 5, TB_GRAY );
		pRMSSelect->outputB.Connect(pBundle, 6);
		pMasterAlarm->ConnectPushButton( pBundle, 7 );

		pBundle=STS()->BundleManager()->CreateBundle("RMS_MODE", 16);
		for(unsigned short i=0;i<12;i++) pRMSMode->ConnectOutputSignal(i, pBundle, i);

		pBundle=STS()->BundleManager()->CreateBundle("RMS_MRL", 16);
		pPortMRL->outputB.Connect(pBundle, 0);
		pPortMRL->outputA.Connect(pBundle, 1);
		for(int i=0;i<3;i++) pPortMRL_RTL[i]->SetInput( pBundle, i + 5, TB_GRAY );
		pPortMRLTb->SetInput( 0, pBundle, 11, TB_REL );
		pPortMRLTb->SetInput( 1, pBundle, 12, TB_LAT );

		pBundle=STS()->BundleManager()->CreateBundle("RMS_EE", 16);
		pEEMode->outputB.Connect(pBundle, 2);
		pEEMode->outputA.Connect(pBundle, 3);
		pEEManContr->outputB.Connect(pBundle, 4);
		pEEManContr->outputA.Connect(pBundle, 5);
		pEECapture->SetInput( pBundle, 6, TB_GRAY );
		pEEExtend->SetInput( pBundle, 7, TB_GRAY );
		pEEClose->SetInput( pBundle, 8, TB_GRAY );
		pEEOpen->SetInput( pBundle, 9, TB_GRAY );
		pEERigid->SetInput( pBundle, 10, TB_GRAY );
		pEEDerigid->SetInput( pBundle, 11, TB_GRAY );

		pBundle=STS()->BundleManager()->CreateBundle("STBD_MPM", 16);
		pStbdMPM->outputB.Connect(pBundle, 0);
		pStbdMPM->outputA.Connect(pBundle, 1);
		pStbdMPMTb->SetInput( 0, pBundle, 2, TB_DPY );
		pStbdMPMTb->SetInput( 1, pBundle, 3, TB_STO );

		pBundle=STS()->BundleManager()->CreateBundle("STBD_MPM_MRL", 16);
		pStbdMRL->outputB.Connect(pBundle, 0);
		pStbdMRL->outputA.Connect(pBundle, 1);
		for(int i=0;i<3;i++) pStbdMRL_RTL[i]->SetInput( pBundle, i + 5, TB_GRAY );
		pStbdMRLTb->SetInput( 0, pBundle, 11, TB_REL );
		pStbdMRLTb->SetInput( 1, pBundle, 12, TB_LAT );

		pBundle=STS()->BundleManager()->CreateBundle("A8_LED", 16);
		for(int i=0;i<8;i++) pLEDParameter->ConnectOutputSignal(i, pBundle, i);
		lighttest.Connect( pBundle, 7 );

		pBundle=STS()->BundleManager()->CreateBundle("RMS_SINGLE_JOINT", 16);
		for(int i=0;i<8;i++) pLEDJoint->ConnectOutputSignal(i, pBundle, 7-i);
		
		pSingleDirectDrive->outputB.Connect(pBundle, 8);
		pSingleDirectDrive->outputA.Connect(pBundle, 9);

		pBundle = STS()->BundleManager()->CreateBundle( "RMS_MODELIGHTS", 16 );
		for (int i = 0; i < 14; i++) lightcmd[i].Connect( pBundle, i );

		pBundle=STS()->BundleManager()->CreateBundle( "RMS_CWLIGHTS_TB", 16 );
		for (int i = 14; i < 26; i++) lightcmd[i].Connect( pBundle, i - 14 );
		pSoftStopTB->SetInput( pBundle, 12, TB_GRAY );

		pBundle = STS()->BundleManager()->CreateBundle( "A8LIGHTS_A", 16 );
		for (int i = 0; i < 12; i++) pModeLights[i]->ConnectLight( 0, pBundle, i );
		pSequenceLights[0]->ConnectLight( 0, pBundle, 12 );
		pSequenceLights[1]->ConnectLight( 0, pBundle, 13 );

		for (int i = 0; i < 14; i++) lightoutput[i].Connect( pBundle, i );

		pBundle = STS()->BundleManager()->CreateBundle( "A8LIGHTS_B", 16 );
		for (int i = 0; i < 11; i++) pCWLights[i]->ConnectLight( 0, pBundle, i );
		pMasterAlarm->ConnectLight( 0, pBundle, 11 );

		for (int i = 14; i < 26; i++) lightoutput[i].Connect( pBundle, i - 14 );

		AtlantisPanel::Realize();
	}

	void PanelA8::OnPostStep( double SimT, double DeltaT, double MJD )
	{
		AtlantisPanel::OnPostStep( SimT, DeltaT, MJD );

		if (lighttest.IsSet())
		{
			for (int i = 0; i < 26; i++) lightoutput[i].SetLine();
		}
		else
		{
			for (int i = 0; i < 26; i++) lightoutput[i].SetLine( (int)lightcmd[i].IsSet() * 5.0f );
		}
		return;
	}
};
