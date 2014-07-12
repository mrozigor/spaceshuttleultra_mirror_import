#include "PanelC3.h"
#include "../meshres_vc.h"
#include "../meshres_vc_additions.h"
#include "../Atlantis_defs.h"

extern GDIParams g_Param;

namespace vc
{
	PanelC3::PanelC3(Atlantis *_sts)
		: AtlantisPanel(_sts, "C3")
	{
		char cbuf[5];

		for(int i=0;i<24;i++) {
			sprintf_s(cbuf, 5, "%d", i+1);
			std::string name="A6_PBI";
			name+=cbuf;
			Add(pPBIs[i]=new PushButtonIndicator(_sts, name));
		}

		Add(pOMSArm[LEFT] = new LockableLever3(_sts, "LOMS Arm"));
		Add(pOMSArm[RIGHT] = new LockableLever3(_sts, "ROMS Arm"));

		Add(pAirDataProbeStowEnable[LEFT] = new StdSwitch2(_sts, "LADP Stow Enable"));
		Add(pAirDataProbeStowEnable[RIGHT] = new StdSwitch2(_sts, "RADP Stow Enable"));
		Add(pAirDataProbeDeploy[LEFT] = new LockableLever3(_sts, "LADP Deploy"));
		Add(pAirDataProbeDeploy[RIGHT] = new LockableLever3(_sts, "RADP Deploy"));

		for(int i=0;i<2;i++) {
			pOMSArm[i]->SetLabel(0, "OFF");
			pOMSArm[i]->SetLabel(1, "ARM/PRESS");
			pOMSArm[i]->SetLabel(2, "ARM");

			pAirDataProbeStowEnable[i]->SetLabel(0, "INHIBIT");
			pAirDataProbeStowEnable[i]->SetLabel(1, "ENABLE");

			pAirDataProbeDeploy[i]->SetLabel(0, "STOW");
			pAirDataProbeDeploy[i]->SetLabel(1, "DEPLOY");
			pAirDataProbeDeploy[i]->SetLabel(2, "DEPLOY/HEAT");
		}

		Add(pSSMELimitShutDn = new StdSwitch3(_sts, "MAIN ENGINE LIMIT SHUT DN"));
		pSSMELimitShutDn->SetLabel(0, "INHIBIT");
		pSSMELimitShutDn->SetLabel(1, "AUTO");
		pSSMELimitShutDn->SetLabel(2, "ENABLE");

		Add( pSSMESDPBCover[0] = new StandardSwitchCover( _sts, "SSME Left S/D PB Cover" ) );
		Add( pSSMESDPBCover[1] = new StandardSwitchCover( _sts, "SSME Ctr S/D PB Cover" ) );
		Add( pSSMESDPBCover[2] = new StandardSwitchCover( _sts, "SSME Right S/D PB Cover" ) );
		Add( pSSMESDPB[0] = new PushButton( _sts, "SSME Left S/D PB" ) );
		Add( pSSMESDPB[1] = new PushButton( _sts, "SSME Ctr S/D PB" ) );
		Add( pSSMESDPB[2] = new PushButton( _sts, "SSME Right S/D PB" ) );

		Add( pSRBSEPSW = new StdSwitch2( _sts, "SRB Separation" ) );
		pSRBSEPSW->SetLabel( 0, "AUTO" );
		pSRBSEPSW->SetLabel( 1, "MAN/AUTO" );
		Add( pSRBSEPCover = new StandardSwitchCover( _sts, "SRB Separation PB Cover" ) );
		Add( pSRBSEPPB = new PushButton( _sts, "SRB Separation PB" ) );

		Add( pETSEPSW = new LockableLever2( _sts, "ET Separation" ) );
		pETSEPSW->SetLabel( 0, "AUTO" );
		pETSEPSW->SetLabel( 1, "MAN" );
		Add( pETSEPCover = new StandardSwitchCover( _sts, "ET Separation PB Cover" ) );
		Add( pETSEPPB = new PushButton( _sts, "ET Separation PB" ) );
	}

	PanelC3::~PanelC3()
	{
	}

	void PanelC3::RegisterVC()
	{
		oapiWriteLog("Registering Panel C3");
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;
		SURFHANDLE panelc3_tex = oapiGetTextureHandle (STS()->hOrbiterVCMesh, TEX_PANELC3_VC);
		oapiVCRegisterArea (AID_C3, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);

		oapiVCSetAreaClickmode_Quadrilateral (AID_C3, _V(-0.2732771, 1.76045, 14.35651)+ofs, _V(0.2732771, 1.76045, 14.35651)+ofs, 
			_V(-0.2732771, 1.69551,  13.73551)+ofs, _V(0.2732771, 1.69551,  13.73551)+ofs);

		//register DAP PBIs
		//A
		oapiVCRegisterArea(AID_C3_PBI1, _R(155, 888, 197, 902), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//B
		oapiVCRegisterArea(AID_C3_PBI2, _R(250, 887, 292, 901), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//AUTO
		oapiVCRegisterArea(AID_C3_PBI3, _R(344, 888, 386, 902), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//INRTL
		oapiVCRegisterArea(AID_C3_PBI4, _R(434, 889, 476, 903), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//LVLH
		oapiVCRegisterArea(AID_C3_PBI5, _R(527, 889, 569, 903), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//FREE
		oapiVCRegisterArea(AID_C3_PBI6, _R(622, 889, 664, 903), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//PCT
		oapiVCRegisterArea(AID_C3_PBI7, _R(157, 1089, 199, 1103), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//LOW Z
		oapiVCRegisterArea(AID_C3_PBI8, _R(247, 1089, 289, 1103), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//HIGH Z
		oapiVCRegisterArea(AID_C3_PBI9, _R(339, 1090, 381, 1104), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//PRI
		oapiVCRegisterArea(AID_C3_PBI10, _R(440, 1089, 482, 1103), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//ALT
		oapiVCRegisterArea(AID_C3_PBI11, _R(531, 1090, 573, 1104), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//VERN
		oapiVCRegisterArea(AID_C3_PBI12, _R(623, 1090, 665, 1104), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//TRANS X NORM
		oapiVCRegisterArea(AID_C3_PBI13, _R(156, 1186, 198, 1200), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//TRANS Y NORM
		oapiVCRegisterArea(AID_C3_PBI14, _R(249, 1186, 291, 1200), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//TRANS Z NORM
		oapiVCRegisterArea(AID_C3_PBI15, _R(338, 1187, 380, 1201), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//ROT ROLL DISC RATE
		oapiVCRegisterArea(AID_C3_PBI16, _R(439, 1186, 481, 1200), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//ROT PITCH DISC RATE
		oapiVCRegisterArea(AID_C3_PBI17, _R(530, 1185, 572, 1199), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//ROT YAW DISC RATE
		oapiVCRegisterArea(AID_C3_PBI18, _R(624, 1185, 666, 1199), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//TRANS X PULSE
		oapiVCRegisterArea(AID_C3_PBI19, _R(158, 1284, 200, 1298), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//TRANS Y PULSE
		oapiVCRegisterArea(AID_C3_PBI20, _R(249, 1284, 291, 1298), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//TRANS Z PULSE
		oapiVCRegisterArea(AID_C3_PBI21, _R(339, 1285, 381, 1299), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//ROT ROLL PULSE
		oapiVCRegisterArea(AID_C3_PBI22, _R(441, 1284, 483, 1298), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//ROT PITCH PULSE
		oapiVCRegisterArea(AID_C3_PBI23, _R(531, 1285, 573, 1299), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//ROT YAW PULSE
		oapiVCRegisterArea(AID_C3_PBI24, _R(624, 1285, 666, 1299), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
	}

	void PanelC3::DefineVC()
	{
		const VECTOR3 switch_rot = _V(1, 0, 0);
		const VECTOR3 pull_dir = _V(0, 1, 0);

		oapiWriteLog("PanelC3: DefineVC called");

		AddAIDToMouseEventList(AID_C3);

		for(int i=0;i<24;i++) {
			pPBIs[i]->AddAIDToRedrawEventList(AID_C3_PBI1+i);
			pPBIs[i]->SetSourceImage(g_Param.pbi_lights);
			pPBIs[i]->SetBase(0, 0);
			pPBIs[i]->SetSourceCoords(true, 0, 0);
			pPBIs[i]->SetSourceCoords(false, 0, 14);
			pPBIs[i]->SetDimensions(42, 14);
		}
		// PBI mouse regions
		pPBIs[0]->SetMouseRegion(0.075540f, 0.421567f, 0.107636f, 0.455563f); //A
		pPBIs[1]->SetMouseRegion(0.129797f, 0.423483f, 0.160799f, 0.456580f); //B
		pPBIs[2]->SetMouseRegion(0.181586f, 0.424022f, 0.213814f, 0.455769f); //AUTO
		pPBIs[3]->SetMouseRegion(0.231808f, 0.423959f, 0.263971f, 0.456584f); //INRTL
		pPBIs[4]->SetMouseRegion(0.285346f, 0.423488f, 0.314472f, 0.455417f); //LVLH
		pPBIs[5]->SetMouseRegion(0.336988f, 0.423070f, 0.370935f, 0.457239f); //FREE
		// NOTE: PCT pbi does not work on PanelC3 (SiameseCat)
		//pPBIs[6]->SetMouseRegion(0.653568f, 0.271157f, 0.699381f, 0.331649f); //PCT
		pPBIs[9]->SetMouseRegion(0.235090f, 0.526461f, 0.268066f, 0.560655f); //PRI
		pPBIs[10]->SetMouseRegion(0.285877f, 0.527114f, 0.319531f, 0.560210f); //ALT
		pPBIs[11]->SetMouseRegion(0.337574f, 0.527336f, 0.372188f, 0.560061f); //VERN
		pPBIs[12]->SetMouseRegion(0.075623f, 0.576277f, 0.110736f, 0.611008f); //X NORM
		pPBIs[13]->SetMouseRegion(0.128061f, 0.576498f, 0.160462f, 0.609389f); //Y NORM
		pPBIs[14]->SetMouseRegion(0.179329f, 0.576932f, 0.211752f, 0.609589f); //Z NORM
		pPBIs[15]->SetMouseRegion(0.235932f, 0.575860f, 0.267935f, 0.609234f); //ROLL DISC RATE
		pPBIs[16]->SetMouseRegion(0.288161f, 0.576933f, 0.318657f, 0.608937f); //PITCH DISC RATE
		pPBIs[17]->SetMouseRegion(0.341027f, 0.575933f, 0.372842f, 0.608609f); //YAW DISC RATE
		pPBIs[18]->SetMouseRegion(0.076191f, 0.627008f, 0.109321f, 0.661322f); //X PULSE
		pPBIs[19]->SetMouseRegion(0.128366f, 0.626492f, 0.159067f, 0.660411f); //Y PULSE
		pPBIs[20]->SetMouseRegion(0.177013f, 0.626544f, 0.213859f, 0.661142f); //Z PULSE
		pPBIs[21]->SetMouseRegion(0.233781f, 0.625428f, 0.269470f, 0.661436f); //ROLL PULSE
		pPBIs[22]->SetMouseRegion(0.284747f, 0.627472f, 0.318325f, 0.661340f); //PITCH PULSE
		pPBIs[23]->SetMouseRegion(0.336868f, 0.626517f, 0.373105f, 0.660984f); //YAW PULSE

		pOMSArm[LEFT]->SetMouseRegion(0.063487f, 0.070910f, 0.117992f, 0.173581f);
		pOMSArm[LEFT]->SetReference(_V(-0.2114868, 1.728119, 14.29085), switch_rot);
		pOMSArm[LEFT]->SetPullDirection(pull_dir);
		pOMSArm[LEFT]->DefineSwitchGroup(GRP_C3B1_VC);
		pOMSArm[LEFT]->ConnectSwitchPosition(1, 1);
		pOMSArm[LEFT]->SetInitialAnimState(0.5f);

		pOMSArm[RIGHT]->SetMouseRegion(0.117992f, 0.070910f, 0.179360f, 0.173581f);
		pOMSArm[RIGHT]->SetReference(_V(-0.1716415, 1.728119, 14.29085), switch_rot);
		pOMSArm[RIGHT]->SetPullDirection(pull_dir);
		pOMSArm[RIGHT]->DefineSwitchGroup(GRP_C3B2_VC);
		pOMSArm[RIGHT]->ConnectSwitchPosition(1, 1);
		pOMSArm[RIGHT]->SetInitialAnimState(0.5f);

		pAirDataProbeStowEnable[LEFT]->SetMouseRegion(0.063720f, 0.255919f, 0.126235f, 0.321174f);
		pAirDataProbeStowEnable[LEFT]->SetReference(_V(-0.2114868,  1.715764,  14.18536), switch_rot);
		pAirDataProbeStowEnable[LEFT]->DefineSwitchGroup(GRP_C3B10_VC);
		pAirDataProbeStowEnable[LEFT]->SetInitialAnimState(0.5f);

		pAirDataProbeStowEnable[RIGHT]->SetMouseRegion(0.126235f, 0.255919f, 0.189637f, 0.321174f);		
		pAirDataProbeStowEnable[RIGHT]->SetReference(_V(-0.1716415,  1.715764,  14.18536), switch_rot);
		pAirDataProbeStowEnable[RIGHT]->DefineSwitchGroup(GRP_C3B11_VC);
		pAirDataProbeStowEnable[RIGHT]->SetInitialAnimState(0.5f);

		pAirDataProbeDeploy[LEFT]->SetMouseRegion(0.080556f, 0.753680f, 0.148883f, 0.864232f);
		pAirDataProbeDeploy[LEFT]->SetReference(_V(-0.2114868, 1.680126, 13.8549), switch_rot);
		pAirDataProbeDeploy[LEFT]->SetPullDirection(pull_dir);
		pAirDataProbeDeploy[LEFT]->DefineSwitchGroup(GRP_C3B23_VC);
		pAirDataProbeDeploy[LEFT]->ConnectSwitchPosition(1, 1);
		pAirDataProbeDeploy[LEFT]->SetInitialAnimState(0.5f);

		pAirDataProbeDeploy[RIGHT]->SetMouseRegion(0.148883f, 0.753680f, 0.208679f, 0.864232f);		
		pAirDataProbeDeploy[RIGHT]->SetReference(_V(-0.1716415, 1.680126, 13.8549), switch_rot);
		pAirDataProbeDeploy[RIGHT]->SetPullDirection(pull_dir);
		pAirDataProbeDeploy[RIGHT]->DefineSwitchGroup(GRP_C3B24_VC);
		pAirDataProbeDeploy[RIGHT]->ConnectSwitchPosition(1, 1);
		pAirDataProbeDeploy[RIGHT]->SetInitialAnimState(0.5f);

		pSSMELimitShutDn->SetMouseRegion(0.302924f, 0.241994f, 0.357174f, 0.322577f);
		pSSMELimitShutDn->SetReference(_V(-0.4785, 1.7175, 14.181), switch_rot);
		pSSMELimitShutDn->DefineSwitchGroup(GRP_C3B13_VC);
		pSSMELimitShutDn->SetInitialAnimState(0.5f);

		pSSMESDPBCover[0]->SetMouseRegion( 0, 0.374513f, 0.282742f, 0.428198f, 0.344442f );
		pSSMESDPBCover[0]->SetMouseRegion( 1, 0.368484f, 0.236416f, 0.423421f, 0.272399f );
		pSSMESDPBCover[0]->SetReference( _V( -0.0545, 1.7466, 14.1836 ), switch_rot );
		pSSMESDPBCover[0]->DefineCoverGroup( GRP_C3COVER1_VC );

		pSSMESDPBCover[1]->SetMouseRegion( 0, 0.441879f, 0.260643f, 0.496332f, 0.321328f );
		pSSMESDPBCover[1]->SetMouseRegion( 1, 0.440854f, 0.217779f, 0.493692f, 0.257521f );
		pSSMESDPBCover[1]->SetReference( _V( -0.0174, 1.7466, 14.1954 ), switch_rot );
		pSSMESDPBCover[1]->DefineCoverGroup( GRP_C3COVER2_VC );

		pSSMESDPBCover[2]->SetMouseRegion( 0, 0.511570f, 0.281802f, 0.563658f, 0.340997f );
		pSSMESDPBCover[2]->SetMouseRegion( 1, 0.514202f, 0.240377f, 0.563319f, 0.277074f );
		pSSMESDPBCover[2]->SetReference( _V( 0.0197, 1.7466, 14.1826 ), switch_rot );
		pSSMESDPBCover[2]->DefineCoverGroup( GRP_C3COVER3_VC );

		pSSMESDPB[0]->SetMouseRegion( 0.380817f, 0.295599f, 0.420386f, 0.331035f );
		pSSMESDPB[0]->SetReference( _V( -0.0545, 1.7466, 14.1623 ), _V( 0, 0, 1 ) );
		pSSMESDPB[0]->DefineGroup( GRP_LEFT_SSME_SHTDN_PB_VC );

		pSSMESDPB[1]->SetMouseRegion( 0.448899f, 0.276360f, 0.488503f, 0.310700f );
		pSSMESDPB[1]->SetReference( _V( -0.0174, 1.7466, 14.1736 ), _V( 0, 0, 1 ) );
		pSSMESDPB[1]->DefineGroup( GRP_CTR_SSME_SHTDN_PB_VC );

		pSSMESDPB[2]->SetMouseRegion( 0.515987f, 0.296511f, 0.556983f, 0.331199f );
		pSSMESDPB[2]->SetReference( _V( 0.197, 1.7466, 14.16 ), _V( 0, 0, 1 ) );
		pSSMESDPB[2]->DefineGroup( GRP_RIGHT_SSME_SHTDN_PB_VC );

		pSRBSEPSW->SetMouseRegion( 0.462571f, 0.414336f, 0.509469f, 0.491312f );
		pSRBSEPSW->SetReference( _V( 0.00608, 1.70746, 14.0791 ), switch_rot );
		pSRBSEPSW->DefineSwitchGroup( GRP_C3B16_VC );
		pSRBSEPSW->SetInitialAnimState( 0.5f );

		pSRBSEPCover->SetMouseRegion( 0, 0.533090f, 0.424105f, 0.586752f, 0.485038f );
		pSRBSEPCover->SetMouseRegion( 1, 0.532972f, 0.381586f, 0.579912f, 0.430505f );
		pSRBSEPCover->SetReference( _V( -0.031, 1.734, 14.096 ), switch_rot );
		pSRBSEPCover->DefineCoverGroup( GRP_C3COVER4_VC );
		
		pSRBSEPPB->SetMouseRegion( 0.537215f, 0.435073f, 0.574794f, 0.470203f );
		pSRBSEPPB->SetReference( _V( -0.031, 1.734, 14.096 ), _V( 0, 0, 1 ) );
		pSRBSEPPB->DefineGroup( GRP_SRB_SEP_PB_VC );

		pETSEPSW->SetMouseRegion( 0.629313f, 0.420718f, 0.675625f, 0.520487f );
		pETSEPSW->SetReference( _V( -0.08218, 1.69955, 14.06513 ), switch_rot );
		pETSEPSW->SetPullDirection( pull_dir );
		pETSEPSW->DefineSwitchGroup( GRP_C3B17_VC );
		pETSEPSW->SetInitialAnimState( 0.5f );
		
		pETSEPCover->SetMouseRegion( 0, 0.691866f, 0.422427f, 0.745989f, 0.482821f );
		pETSEPCover->SetMouseRegion( 1, 0.703331f, 0.380311f, 0.735696f, 0.423886f );
		pETSEPCover->SetReference( _V( -0.119, 1.734, 14.096 ), switch_rot );
		pETSEPCover->DefineCoverGroup( GRP_C3COVER5_VC );

		pETSEPPB->SetMouseRegion( 0.698907f, 0.432862f, 0.731796f, 0.463562f );
		pETSEPPB->SetReference( _V( -0.119, 1.734, 14.096 ), _V( 0, 0, 1 ) );
		pETSEPPB->DefineGroup( GRP_ET_SEP_PB_VC );
	}

	void PanelC3::Realize()
	{
		DiscreteBundle* pBundle=STS()->BundleManager()->CreateBundle("DAP_PBIS1", 16);
		for(int i=0;i<16;i++) {
			pPBIs[i]->input.Connect(pBundle, i);
			pPBIs[i]->output.Connect(pBundle, i);
			pPBIs[i]->test.Connect(pBundle, i);
		}
		pBundle=STS()->BundleManager()->CreateBundle("DAP_PBIS2", 16);
		for(int i=16;i<24;i++) {
			pPBIs[i]->input.Connect(pBundle, i-16);
			pPBIs[i]->output.Connect(pBundle, i-16);
			pPBIs[i]->test.Connect(pBundle, i-16);
		}

		pBundle=STS()->BundleManager()->CreateBundle("LOMS", 2);
		pOMSArm[LEFT]->ConnectPort(2, pBundle, 0); // ARM
		pOMSArm[LEFT]->ConnectPort(1, pBundle, 1); // ARM/PRESS
		pBundle=STS()->BundleManager()->CreateBundle("ROMS", 2);
		pOMSArm[RIGHT]->ConnectPort(2, pBundle, 0); // ARM
		pOMSArm[RIGHT]->ConnectPort(1, pBundle, 1); // ARM/PRESS

		pBundle=STS()->BundleManager()->CreateBundle("LADP", 3);
		pAirDataProbeStowEnable[LEFT]->output.Connect(pBundle, 0);
		pAirDataProbeDeploy[LEFT]->ConnectPort(2, pBundle, 1); // DEPLOY
		pAirDataProbeDeploy[LEFT]->ConnectPort(1, pBundle, 2); // DEPLOY/HEAT

		pBundle=STS()->BundleManager()->CreateBundle("RADP", 3);
		pAirDataProbeStowEnable[RIGHT]->output.Connect(pBundle, 0);
		pAirDataProbeDeploy[RIGHT]->ConnectPort(2, pBundle, 1); // DEPLOY
		pAirDataProbeDeploy[RIGHT]->ConnectPort(1, pBundle, 2); // DEPLOY/HEAT

		pBundle = STS()->BundleManager()->CreateBundle( "C3_LIMITS_SSMEPB", 5 );
		pSSMELimitShutDn->ConnectPort(1, pBundle, 0); // INHIBIT
		pSSMELimitShutDn->ConnectPort(2, pBundle, 1); // ENABLE
		pSSMESDPB[0]->output.Connect( pBundle, 2 );// L
		pSSMESDPB[1]->output.Connect( pBundle, 3 );// C
		pSSMESDPB[2]->output.Connect( pBundle, 4 );// R

		pBundle = STS()->BundleManager()->CreateBundle( "C3_SEP", 4 );
		pSRBSEPSW->ConnectPort( 1, pBundle, 0 );
		pSRBSEPPB->output.Connect( pBundle, 1 );
		pETSEPSW->ConnectPort( 1, pBundle, 2 );
		pETSEPPB->output.Connect( pBundle, 3 );

		// VC component DiscPorts need to be connected before Realize() is called
		AtlantisPanel::Realize();
	}
};
