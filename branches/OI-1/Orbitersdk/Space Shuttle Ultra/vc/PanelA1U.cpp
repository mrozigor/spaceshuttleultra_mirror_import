#include "PanelA1U.h"
#include "../meshres_vc_additions.h"
#include "../Atlantis.h"

extern GDIParams g_Param;

const VECTOR3 S1_REF = _V(0.0, 3.048204, 12.22);
const VECTOR3 S3_REF = _V(0.0, 2.97289, 12.22);
const VECTOR3 S5_REF = _V(0.0, 2.91259, 12.22);

namespace vc
{
	PanelA1U::PanelA1U(Atlantis* _sts)
		: BasicPanel(_sts, "A1U")
	{
		Add(pSignalStrengthSelect = new StdSwitch3(_sts, "SIG STR"));
		pSignalStrengthSelect->SetLabel(0, "SBAND FM");
		pSignalStrengthSelect->SetLabel(1, "KU");
		pSignalStrengthSelect->SetLabel(2, "SBAND PM");
		
		/*
		Add(pCameraPan = new StdSwitch3(_sts, "PAN"));
		Add(pCameraTilt = new StdSwitch3(_sts, "TILT"));
		Add(pPanTiltRate= new StdSwitch3(_sts, "CAMERA_RATE"));

		Add(pCamApbi = new PushButtonIndicator(_sts, "CAM_A"));
		Add(pCamBpbi = new PushButtonIndicator(_sts, "CAM_B"));
		Add(pCamCpbi = new PushButtonIndicator(_sts, "CAM_C"));
		Add(pCamDpbi = new PushButtonIndicator(_sts, "CAM_D"));
		Add(pCamRMSpbi = new PushButtonIndicator(_sts, "CAM_RMS"));

		pPanTiltRate->SetLabel(0, "LOW");
		pPanTiltRate->SetLabel(1, "HIGH");
		pPanTiltRate->SetLabel(2, "RESET");
		*/
	}

	PanelA1U::~PanelA1U()
	{
	}

	void PanelA1U::OnPreStep(double SimT, double DeltaT, double MJD)
	{
		/*
		VideoInputGroup.OnPreStep();

		BasicPanel::OnPreStep(SimT, DeltaT, MJD);

		//if(CamTiltUp_In) sprintf_s(oapiDebugString(), 255, "Tilt up");
		//else if(CamTiltDown_In) sprintf_s(oapiDebugString(), 255, "Tilt down");

		// determine which camera (if any) is being operated
		unsigned short cam = 5;
		if(CamA) cam = 0;
		else if(CamB) cam = 1;
		else if(CamC) cam = 2;
		else if(CamD) cam = 3;
		else if(CamRMS) {
			cam = 4;
			sprintf_s(oapiDebugString(), 255, "RMS Selected");
		}

		if(cam != 5) // make sure a camera is selected
		{
			if(CamPanLeft_In) {
				CamPanLeft_Out[cam].SetLine();
				CamPanRight_Out[cam].ResetLine();
			}
			else if(CamPanRight_In) {
				CamPanLeft_Out[cam].ResetLine();
				CamPanRight_Out[cam].SetLine();
			}
			else {
				CamPanLeft_Out[cam].ResetLine();
				CamPanRight_Out[cam].ResetLine();
			}
			if(CamTiltUp_In) {
				CamTiltUp_Out[cam].SetLine();
				CamTiltDown_Out[cam].ResetLine();
			}
			else if(CamTiltDown_In) {
				CamTiltUp_Out[cam].ResetLine();
				CamTiltDown_Out[cam].SetLine();
			}
			else {
				CamTiltUp_Out[cam].ResetLine();
				CamTiltDown_Out[cam].ResetLine();
			}

			if(LowSpeed_In) {
				LowSpeed_Out[cam].SetLine();
			}
			else {
				LowSpeed_Out[cam].ResetLine();
			}
		}
		*/
	}

	void PanelA1U::DefineVC()
	{
		const VECTOR3 switch_rot_vert =  _V(-1, 0, 0);
		const VECTOR3 switch_rot_horz = _V(0.0, -0.9499, 0.3126);
		//VECTOR3 switch_pull = _V(0, 0.3126, 0.9499);

		AddAIDToMouseEventList(AID_A1U1);
		AddAIDToMouseEventList(AID_A1U2);

		/*
		pCameraPan->DefineSwitchGroup(GRP_A7U44_VC);
		pCameraPan->SetInitialAnimState(0.5);
		pCameraPan->SetInitialPosition(1);
		pCameraPan->SetMouseRegion(0.099787f, 0.768763f, 0.153872f, 0.873155f);
		pCameraPan->SetReference(_V(-0.2, 2.584, 12.373), switch_rot_horz);
		pCameraPan->SetOrientation(true);
		pCameraPan->SetSpringLoaded(true);

		pCameraTilt->DefineSwitchGroup(GRP_A7U43_VC);
		pCameraTilt->SetInitialAnimState(0.5);
		pCameraTilt->SetInitialPosition(1);
		pCameraTilt->SetMouseRegion(0.174297f, 0.773431f, 0.231700f, 0.856463f);
		pCameraTilt->SetReference(_V(-0.158, 2.583, 12.373), switch_rot_vert);
		pCameraTilt->SetSpringLoaded(true);

		pPanTiltRate->DefineSwitchGroup(GRP_A7U56_VC);
		pPanTiltRate->SetInitialAnimState(0.5);
		pPanTiltRate->SetMouseRegion(0.412282f, 0.769816f, 0.471316f, 0.868281f);
		pPanTiltRate->SetReference(_V(-0.043, 2.583, 12.373), switch_rot_vert);
		pPanTiltRate->SetSpringLoaded(true, 2);

		pCamApbi->AddAIDToRedrawEventList(AID_A7U_PBI1);
		pCamApbi->SetMouseRegion(0.448337f, 0.392420f, 0.497079f, 0.456108f);
		pCamApbi->SetSourceImage(g_Param.pbi_lights);
		pCamApbi->SetBase(0, 0);
		pCamApbi->SetSourceCoords(true, 0, 0);
		pCamApbi->SetSourceCoords(false, 0, 14);
		pCamApbi->SetDimensions(37, 11);
		pCamApbi->AllowReset(true);

		pCamBpbi->AddAIDToRedrawEventList(AID_A7U_PBI2);
		pCamBpbi->SetMouseRegion(0.391635f, 0.391737f, 0.440330f, 0.457819f);
		pCamBpbi->SetSourceImage(g_Param.pbi_lights);
		pCamBpbi->SetBase(0, 0);
		pCamBpbi->SetSourceCoords(true, 0, 0);
		pCamBpbi->SetSourceCoords(false, 0, 14);
		pCamBpbi->SetDimensions(37, 11);
		pCamBpbi->AllowReset(true);

		pCamCpbi->AddAIDToRedrawEventList(AID_A7U_PBI3);
		pCamCpbi->SetMouseRegion(0.335499f, 0.391366f, 0.387364f, 0.458246f);
		pCamCpbi->SetSourceImage(g_Param.pbi_lights);
		pCamCpbi->SetBase(0, 0);
		pCamCpbi->SetSourceCoords(true, 0, 0);
		pCamCpbi->SetSourceCoords(false, 0, 14);
		pCamCpbi->SetDimensions(37, 11);
		pCamCpbi->AllowReset(true);

		pCamDpbi->AddAIDToRedrawEventList(AID_A7U_PBI4);
		pCamDpbi->SetMouseRegion(0.447819f, 0.467543f, 0.495719f, 0.535743f);
		pCamDpbi->SetSourceImage(g_Param.pbi_lights);
		pCamDpbi->SetBase(0, 0);
		pCamDpbi->SetSourceCoords(true, 0, 0);
		pCamDpbi->SetSourceCoords(false, 0, 14);
		pCamDpbi->SetDimensions(37, 11);
		pCamDpbi->AllowReset(true);

		pCamRMSpbi->AddAIDToRedrawEventList(AID_A7U_PBI5);
		pCamRMSpbi->SetMouseRegion(0.390772f, 0.468728f, 0.440740f, 0.535320f);
		pCamRMSpbi->SetSourceImage(g_Param.pbi_lights);
		pCamRMSpbi->SetBase(0, 0);
		pCamRMSpbi->SetSourceCoords(true, 0, 0);
		pCamRMSpbi->SetSourceCoords(false, 0, 14);
		pCamRMSpbi->SetDimensions(37, 11);
		pCamRMSpbi->AllowReset(true);
		*/
	}

	void PanelA1U::RegisterVC()
	{
		BasicPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset();

		oapiVCRegisterArea(AID_A1U1, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
		oapiVCSetAreaClickmode_Quadrilateral(AID_A1U1,
			_V(-0.264, 2.893, 12.277)+ofs, _V(0.255, 2.893, 12.277)+ofs,
			_V(-0.264, 2.507, 12.397)+ofs, _V(0.255, 2.507, 12.397)+ofs);

		oapiVCRegisterArea(AID_A1U2, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
		oapiVCSetAreaClickmode_Quadrilateral(AID_A1U2,
			_V(-0.264, 2.893, 12.277)+ofs, _V(0.255, 2.893, 12.277)+ofs,
			_V(-0.264, 2.507, 12.397)+ofs, _V(0.255, 2.507, 12.397)+ofs);
		
		/*
		//register PBIs
		SURFHANDLE panela7_tex = oapiGetTextureHandle (STS()->hOrbiterVCMesh, TEX_PANELA6A7_VC);
		// CAM A
		oapiVCRegisterArea(AID_A7U_PBI1, _R(993, 444, 1030, 455), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela7_tex);
		// CAM B
		oapiVCRegisterArea(AID_A7U_PBI2, _R(1070, 444, 1107, 455), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela7_tex);
		// CAM C
		oapiVCRegisterArea(AID_A7U_PBI3, _R(1147, 444, 1184, 455), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela7_tex);
		// CAM D
		oapiVCRegisterArea(AID_A7U_PBI4, _R(993, 524, 1030, 535), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela7_tex);
		// RMS CAM
		oapiVCRegisterArea(AID_A7U_PBI5, _R(1070, 524, 1107, 535), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela7_tex);
		*/
	}

	void PanelA1U::Realize()
	{
		BasicPanel::Realize();

		/*
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle("A7U_CameraSelect", 16);
		pCamApbi->ConnectAll(pBundle, 0);
		CamA.Connect(pBundle, 0);
		pCamBpbi->ConnectAll(pBundle, 1);
		CamB.Connect(pBundle, 1);
		pCamCpbi->ConnectAll(pBundle, 2);
		CamC.Connect(pBundle, 2);
		pCamDpbi->ConnectAll(pBundle, 3);
		CamD.Connect(pBundle, 3);
		pCamRMSpbi->ConnectAll(pBundle, 4);
		CamRMS.Connect(pBundle, 4);
		VideoInputGroup.AddPorts(pBundle, 0, 4);
		
		pBundle = STS()->BundleManager()->CreateBundle("A7U_PanTilt", 16);
		CamPanLeft_In.Connect(pBundle, 0);
		pCameraPan->outputA.Connect(pBundle, 0);
		CamPanRight_In.Connect(pBundle, 1);
		pCameraPan->outputB.Connect(pBundle, 1);
		CamTiltUp_In.Connect(pBundle, 2);
		pCameraTilt->outputB.Connect(pBundle, 2);
		CamTiltDown_In.Connect(pBundle, 3);
		pCameraTilt->outputA.Connect(pBundle, 3);
		LowSpeed_In.Connect(pBundle, 4);
		pPanTiltRate->outputA.Connect(pBundle, 4);

		DiscreteBundle* pCamBundles[5];
		pCamBundles[0] = STS()->BundleManager()->CreateBundle("PLBD_CAM_A", 16);
		pCamBundles[1] = STS()->BundleManager()->CreateBundle("PLBD_CAM_B", 16);
		pCamBundles[2] = STS()->BundleManager()->CreateBundle("PLBD_CAM_C", 16);
		pCamBundles[3] = STS()->BundleManager()->CreateBundle("PLBD_CAM_D", 16);
		pCamBundles[4] = STS()->BundleManager()->CreateBundle("RMS_ELBOW_CAM", 16);
		for(int i=0;i<5;i++) {
			CamPanLeft_Out[i].Connect(pCamBundles[i], 0);
			CamPanRight_Out[i].Connect(pCamBundles[i], 1);
			CamTiltUp_Out[i].Connect(pCamBundles[i], 2);
			CamTiltDown_Out[i].Connect(pCamBundles[i], 3);
			LowSpeed_Out[i].Connect(pCamBundles[i], 4);
		}
		*/
	}
};