#include "PanelC3.h"
#include "../meshres_vc.h"
#include "../Atlantis_defs.h"


namespace vc
{
	PanelC3::PanelC3(Atlantis *_sts)
		: AtlantisPanel(_sts, "C3")
	{
		char cbuf[5];

		for(int i=0;i<24;i++) {
			sprintf_s(cbuf, 5, "%d", i+1);
			std::string name="C3_PBI";
			name+=cbuf;
			Add(pPBIs[i]=new PushButtonIndicatorSingleLight(_sts, name));
		}

		Add(pOMSArm[LEFT] = new LockableLever3(_sts, "LOMS Arm"));
		Add(pOMSArm[RIGHT] = new LockableLever3(_sts, "ROMS Arm"));

		Add( pBFCCRTDisplay = new StdSwitch2( _sts, "BFC CRT Display" ) );
		pBFCCRTDisplay->SetLabel( 0, "OFF" );
		pBFCCRTDisplay->SetLabel( 1, "ON" );

		Add( pBFCCRTSelect = new StdSwitch3( _sts, "BFC CRT Select" ) );
		pBFCCRTSelect->SetLabel( 0, "3+1" );
		pBFCCRTSelect->SetLabel( 1, "2+3" );
		pBFCCRTSelect->SetLabel( 2, "1+2" );

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
		oapiVCRegisterArea (AID_C3, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);

		oapiVCSetAreaClickmode_Quadrilateral (AID_C3, _V(-0.2732771, 1.76045, 14.35651)+ofs, _V(0.2732771, 1.76045, 14.35651)+ofs, 
			_V(-0.2732771, 1.69551,  13.73551)+ofs, _V(0.2732771, 1.69551,  13.73551)+ofs);

		return;
	}

	void PanelC3::DefineVC()
	{
		const VECTOR3 switch_rot = _V(1, 0, 0);
		const VECTOR3 pull_dir = _V( 0, 0.994522, -0.104528 );
		const VECTOR3 push_dir = -pull_dir;

		oapiWriteLog("PanelC3: DefineVC called");

		AddAIDToMouseEventList(AID_C3);

		for (int i = 0; i < 24; i++)
		{
			pPBIs[i]->SetStateOffset( 1, 0.0f, 0.488281f );
			pPBIs[i]->SetDirection( push_dir );
		}
		
		// switch number: original name (current name)
		pPBIs[0]->SetMouseRegion( 0.076792f, 0.420048f, 0.115283f, 0.455242f );// S1: SELECT - A (SELECT - A)
		pPBIs[0]->DefineMeshGroup( STS()->mesh_vc, GRP_C3_S1_VC );

		pPBIs[1]->SetMouseRegion( 0.126949f, 0.421518f, 0.166340f, 0.455293f );// S2: SELECT - B (SELECT - B)
		pPBIs[1]->DefineMeshGroup( STS()->mesh_vc, GRP_C3_S2_VC );

		pPBIs[2]->SetMouseRegion( 0.181124f, 0.421175f, 0.219444f, 0.455331f );// S3: CONTROL - AUTO (CONTROL - AUTO)
		pPBIs[2]->DefineMeshGroup( STS()->mesh_vc, GRP_C3_S3_VC );

		pPBIs[3]->SetMouseRegion( 0.231705f, 0.420583f, 0.271740f, 0.454928f );// S4: CONTROL - MAN (CONTROL - INRTL)
		pPBIs[3]->DefineMeshGroup( STS()->mesh_vc, GRP_C3_S4_VC );

		pPBIs[4]->SetMouseRegion( 0.284360f, 0.420678f, 0.325547f, 0.455212f );// S5: RCS JETS - NORM (CONTROL - LVLH)
		pPBIs[4]->DefineMeshGroup( STS()->mesh_vc, GRP_C3_S5_VC );

		pPBIs[5]->SetMouseRegion( 0.335767f, 0.420633f, 0.374571f, 0.455531f );// S6: RCS JETS - VERN (CONTROL - FREE)
		pPBIs[5]->DefineMeshGroup( STS()->mesh_vc, GRP_C3_S6_VC );
		
		pPBIs[6]->SetMouseRegion( 0.076832f, 0.524369f, 0.115813f, 0.558626f );// S16: TRANSLATION X - HIGH ()
		pPBIs[6]->DefineMeshGroup( STS()->mesh_vc, GRP_C3_S16_VC );

		pPBIs[7]->SetMouseRegion( 0.126628f, 0.523217f, 0.166078f, 0.558785f );// S17: TRANSLATION Y - HIGH (TRANSLATION Y - LOW Z)
		pPBIs[7]->DefineMeshGroup( STS()->mesh_vc, GRP_C3_S17_VC );

		pPBIs[8]->SetMouseRegion( 0.178143f, 0.523321f, 0.218689f, 0.558025f );// S18: TRANSLATION Z - HIGH (TRANSLATION Z - HIGH Z)
		pPBIs[8]->DefineMeshGroup( STS()->mesh_vc, GRP_C3_S18_VC );

		pPBIs[9]->SetMouseRegion( 0.233175f, 0.523569f, 0.273972f, 0.558653f );// S7: ROTATION ROLL - DISC RATE (ROTATION ROLL - PRI)
		pPBIs[9]->DefineMeshGroup( STS()->mesh_vc, GRP_C3_S7_VC );
		
		pPBIs[10]->SetMouseRegion( 0.285445f, 0.523146f, 0.325593f, 0.558556f );// S8: ROTATION PITCH - DISC RATE (ROTATION PITCH - ALT)
		pPBIs[10]->DefineMeshGroup( STS()->mesh_vc, GRP_C3_S8_VC );
		
		pPBIs[11]->SetMouseRegion( 0.336362f, 0.524081f, 0.376589f, 0.558426f );// S9: ROTATION YAW - DISC RATE (ROTATION YAW - VERN)
		pPBIs[11]->DefineMeshGroup( STS()->mesh_vc, GRP_C3_S9_VC );
		
		pPBIs[12]->SetMouseRegion( 0.074573f, 0.576707f, 0.115387f, 0.612468f );// S19: TRANSLATION X - NORM (TRANSLATION X - NORM)
		pPBIs[12]->DefineMeshGroup( STS()->mesh_vc, GRP_C3_S19_VC );
		
		pPBIs[13]->SetMouseRegion( 0.126687f, 0.577016f, 0.165797f, 0.611745f );// S20: TRANSLATION Y - NORM (TRANSLATION Y - NORM)
		pPBIs[13]->DefineMeshGroup( STS()->mesh_vc, GRP_C3_S20_VC );
		
		pPBIs[14]->SetMouseRegion( 0.178360f, 0.576320f, 0.218115f, 0.611317f );// S21: TRANSLATION Z - NORM (TRANSLATION Z - NORM)
		pPBIs[14]->DefineMeshGroup( STS()->mesh_vc, GRP_C3_S21_VC );
		
		pPBIs[15]->SetMouseRegion( 0.234408f, 0.576546f, 0.273775f, 0.611331f );// S10: ROTATION ROLL - ACCEL (ROTATION ROLL - DISC RATE)
		pPBIs[15]->DefineMeshGroup( STS()->mesh_vc, GRP_C3_S10_VC );
		
		pPBIs[16]->SetMouseRegion( 0.284538f, 0.575901f, 0.324818f, 0.611630f );// S11: ROTATION PITCH - ACCEL (ROTATION PITCH - DISC RATE)
		pPBIs[16]->DefineMeshGroup( STS()->mesh_vc, GRP_C3_S11_VC );
		
		pPBIs[17]->SetMouseRegion( 0.335612f, 0.576493f, 0.377545f, 0.611941f );// S12: ROTATION YAW - ACCEL (ROTATION YAW - DISC RATE)
		pPBIs[17]->DefineMeshGroup( STS()->mesh_vc, GRP_C3_S12_VC );
		
		pPBIs[18]->SetMouseRegion( 0.074080f, 0.625474f, 0.115904f, 0.660760f );// S22: TRANSLATION X - PULSE (TRANSLATION X - PULSE)
		pPBIs[18]->DefineMeshGroup( STS()->mesh_vc, GRP_C3_S22_VC );
		
		pPBIs[19]->SetMouseRegion( 0.127879f, 0.625166f, 0.166948f, 0.661009f );// S23: TRANSLATION Y - PULSE (TRANSLATION Y - PULSE)
		pPBIs[19]->DefineMeshGroup( STS()->mesh_vc, GRP_C3_S23_VC );
		
		pPBIs[20]->SetMouseRegion( 0.177976f, 0.626219f, 0.218171f, 0.660640f );// S24: TRANSLATION Z - PULSE (TRANSLATION Z - PULSE)
		pPBIs[20]->DefineMeshGroup( STS()->mesh_vc, GRP_C3_S24_VC );
		
		pPBIs[21]->SetMouseRegion( 0.233390f, 0.625540f, 0.274357f, 0.660435f );// S13: ROTATION ROLL - PULSE (ROTATION ROLL - PULSE)
		pPBIs[21]->DefineMeshGroup( STS()->mesh_vc, GRP_C3_S13_VC );
		
		pPBIs[22]->SetMouseRegion( 0.284311f, 0.625695f, 0.324971f, 0.660364f );// S14: ROTATION PITCH - PULSE (ROTATION PITCH - PULSE)
		pPBIs[22]->DefineMeshGroup( STS()->mesh_vc, GRP_C3_S14_VC );

		pPBIs[23]->SetMouseRegion( 0.336204f, 0.625340f, 0.376533f, 0.660289f );// S15: ROTATION YAW - PULSE (ROTATION YAW - PULSE)
		pPBIs[23]->DefineMeshGroup( STS()->mesh_vc, GRP_C3_S15_VC );

		pOMSArm[LEFT]->SetMouseRegion(0.063487f, 0.070910f, 0.117992f, 0.173581f);
		pOMSArm[LEFT]->SetReference(_V(-0.2114868, 1.7215, 14.2841), switch_rot);
		pOMSArm[LEFT]->SetPullDirection(pull_dir);
		pOMSArm[LEFT]->DefineSwitchGroup(GRP_C3B1_VC);
		pOMSArm[LEFT]->ConnectSwitchPosition(1, 1);
		pOMSArm[LEFT]->SetInitialAnimState(0.5f);

		pOMSArm[RIGHT]->SetMouseRegion(0.117992f, 0.070910f, 0.179360f, 0.173581f);
		pOMSArm[RIGHT]->SetReference(_V(-0.1716415, 1.7215, 14.2841), switch_rot);
		pOMSArm[RIGHT]->SetPullDirection(pull_dir);
		pOMSArm[RIGHT]->DefineSwitchGroup(GRP_C3B2_VC);
		pOMSArm[RIGHT]->ConnectSwitchPosition(1, 1);
		pOMSArm[RIGHT]->SetInitialAnimState(0.5f);

		pBFCCRTDisplay->SetMouseRegion( 0.204959f, 0.071767f, 0.231104f, 0.138565f );
		pBFCCRTDisplay->SetReference( _V( -0.153415, 1.7302, 14.2963 ), switch_rot );
		pBFCCRTDisplay->DefineSwitchGroup( GRP_C3B3_VC );
		pBFCCRTDisplay->SetInitialAnimState( 0.5f );

		pBFCCRTSelect->SetMouseRegion( 0.250595f, 0.071845f, 0.286626f, 0.133207f );
		pBFCCRTSelect->SetReference( _V( -0.124915, 1.7302, 14.2954 ), switch_rot );
		pBFCCRTSelect->DefineSwitchGroup( GRP_C3B4_VC );
		pBFCCRTSelect->SetInitialAnimState( 0.5f );

		pAirDataProbeStowEnable[LEFT]->SetMouseRegion(0.063720f, 0.255919f, 0.126235f, 0.321174f);
		pAirDataProbeStowEnable[LEFT]->SetReference(_V(-0.2114868, 1.7178, 14.1801), switch_rot);
		pAirDataProbeStowEnable[LEFT]->DefineSwitchGroup(GRP_C3B10_VC);
		pAirDataProbeStowEnable[LEFT]->SetInitialAnimState(0.5f);

		pAirDataProbeStowEnable[RIGHT]->SetMouseRegion(0.126235f, 0.255919f, 0.189637f, 0.321174f);		
		pAirDataProbeStowEnable[RIGHT]->SetReference(_V(-0.1716415, 1.7178, 14.1801), switch_rot);
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
		pSSMESDPB[0]->SetDirection( push_dir );
		pSSMESDPB[0]->DefineGroup( GRP_LEFT_SSME_SHTDN_PB_VC );

		pSSMESDPB[1]->SetMouseRegion( 0.448899f, 0.276360f, 0.488503f, 0.310700f );
		pSSMESDPB[1]->SetDirection( push_dir );
		pSSMESDPB[1]->DefineGroup( GRP_CTR_SSME_SHTDN_PB_VC );

		pSSMESDPB[2]->SetMouseRegion( 0.515987f, 0.296511f, 0.556983f, 0.331199f );
		pSSMESDPB[2]->SetDirection( push_dir );
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
		pSRBSEPPB->SetDirection( push_dir );
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
		pETSEPPB->SetDirection( push_dir );
		pETSEPPB->DefineGroup( GRP_ET_SEP_PB_VC );
	}

	void PanelC3::Realize()
	{
		DiscreteBundle* pBundle=STS()->BundleManager()->CreateBundle("DAP_PBIS1", 16);
		for (int i = 0; i < 16; i++)
		{
			if ((i == 6) || (i == 7) || (i == 8)) continue;// HACK don't connect "blank", low z and high z
			pPBIs[i]->ConnectPushButton( pBundle, i );
		}
		pBundle=STS()->BundleManager()->CreateBundle("DAP_PBIS2", 16);
		for (int i = 16; i < 24; i++) pPBIs[i]->ConnectPushButton( pBundle, i - 16 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA1_1", 16 );
		pPBIs[3]->ConnectLight( 0, pBundle, 3 );
		pPBIs[4]->ConnectLight( 0, pBundle, 7 );
		pPBIs[5]->ConnectLight( 0, pBundle, 11 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA1_2", 16 );
		pPBIs[9]->ConnectLight( 0, pBundle, 3 );
		pPBIs[10]->ConnectLight( 0, pBundle, 7 );
		pPBIs[11]->ConnectLight( 0, pBundle, 11 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA1_3", 16 );
		pPBIs[15]->ConnectLight( 0, pBundle, 3 );
		pPBIs[16]->ConnectLight( 0, pBundle, 7 );
		pPBIs[17]->ConnectLight( 0, pBundle, 11 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA1_4", 16 );
		pPBIs[21]->ConnectLight( 0, pBundle, 3 );
		pPBIs[22]->ConnectLight( 0, pBundle, 7 );
		pPBIs[23]->ConnectLight( 0, pBundle, 11 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA3_1", 16 );
		pPBIs[0]->ConnectLight( 0, pBundle, 3 );
		pPBIs[1]->ConnectLight( 0, pBundle, 7 );
		pPBIs[2]->ConnectLight( 0, pBundle, 11 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA3_2", 16 );
		pPBIs[6]->ConnectLight( 0, pBundle, 3 );
		pPBIs[7]->ConnectLight( 0, pBundle, 7 );
		pPBIs[8]->ConnectLight( 0, pBundle, 11 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA3_3", 16 );
		pPBIs[12]->ConnectLight( 0, pBundle, 3 );
		pPBIs[13]->ConnectLight( 0, pBundle, 7 );
		pPBIs[14]->ConnectLight( 0, pBundle, 11 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA3_4", 16 );
		pPBIs[18]->ConnectLight( 0, pBundle, 3 );
		pPBIs[19]->ConnectLight( 0, pBundle, 7 );
		pPBIs[20]->ConnectLight( 0, pBundle, 11 );

		pBundle=STS()->BundleManager()->CreateBundle("LOMS", 2);
		pOMSArm[LEFT]->ConnectPort(2, pBundle, 0); // ARM
		pOMSArm[LEFT]->ConnectPort(1, pBundle, 1); // ARM/PRESS
		pBundle=STS()->BundleManager()->CreateBundle("ROMS", 2);
		pOMSArm[RIGHT]->ConnectPort(2, pBundle, 0); // ARM
		pOMSArm[RIGHT]->ConnectPort(1, pBundle, 1); // ARM/PRESS

		pBundle = STS()->BundleManager()->CreateBundle( "BFCCRT", 3 );
		pBFCCRTDisplay->output.Connect( pBundle, 0 );// ON
		pBFCCRTSelect->ConnectPort( 1, pBundle, 1 ); // 3+1
		pBFCCRTSelect->ConnectPort( 2, pBundle, 2 ); // 1+2

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
		pSRBSEPSW->ConnectPort( 1, pBundle, 0 );// MAN/AUTO
		pSRBSEPPB->output.Connect( pBundle, 1 );
		pETSEPSW->ConnectPort( 1, pBundle, 2 );// MAN
		pETSEPPB->output.Connect( pBundle, 3 );

		// VC component DiscPorts need to be connected before Realize() is called
		AtlantisPanel::Realize();
	}
};
