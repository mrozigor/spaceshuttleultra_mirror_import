#include "PanelA7U.h"
#include "../Atlantis.h"
#include "../Atlantis_defs.h"
#include "..\CommonDefs.h"
#include "..\meshres_vc_a7u.h"


namespace vc
{
	PanelA7U::PanelA7U(Atlantis* _sts)
		: AtlantisPanel(_sts, "A7U")
	{
		hPanelMesh = oapiLoadMeshGlobal( DEFAULT_MESHNAME_PANELA7U );
		mesh_index = MESH_UNDEFINED;

		Add( pCameraPowerA_TB = new StandardTalkback2( _sts, "Camera Power A Tb" ) );
		Add( pCameraPowerB_TB = new StandardTalkback2( _sts, "Camera Power B Tb" ) );
		Add( pCameraPowerC_TB = new StandardTalkback2( _sts, "Camera Power C Tb" ) );
		Add( pCameraPowerD_TB = new StandardTalkback2( _sts, "Camera Power D Tb" ) );
		Add( pCameraPowerRMS_TB = new StandardTalkback2( _sts, "Camera Power RMS Tb" ) );

		Add( pMasterAlarm = new PushButtonIndicatorSingleLight( _sts, "Master Alarm" ) );

		Add( pVideoInputA = new PushButtonIndicatorSingleLight( _sts, "Video Input A" ) );
		Add( pVideoInputB = new PushButtonIndicatorSingleLight( _sts, "Video Input B" ) );
		Add( pVideoInputC = new PushButtonIndicatorSingleLight( _sts, "Video Input C" ) );
		Add( pVideoInputD = new PushButtonIndicatorSingleLight( _sts, "Video Input D" ) );
		Add( pVideoInputRMS = new PushButtonIndicatorSingleLight( _sts, "Video Input RMS" ) );
		Add( pVideoInputFltDeck = new PushButtonIndicatorSingleLight( _sts, "Video Input Flt Deck" ) );
		Add( pVideoInputMidDeck = new PushButtonIndicatorSingleLight( _sts, "Video Input Mid Deck" ) );
		Add( pVideoInputPL1 = new PushButtonIndicatorSingleLight( _sts, "Video Input PL 1" ) );
		Add( pVideoInputPL2 = new PushButtonIndicatorSingleLight( _sts, "Video Input PL 2" ) );
		Add( pVideoInputPL3 = new PushButtonIndicatorSingleLight( _sts, "Video Input PL 3" ) );
		Add( pVideoInputMUX1 = new PushButtonIndicatorSingleLight( _sts, "Video Input MUX 1" ) );
		Add( pVideoInputMUX2 = new PushButtonIndicatorSingleLight( _sts, "Video Input MUX 2" ) );
		Add( pVideoInputTest = new PushButtonIndicatorSingleLight( _sts, "Video Input Test" ) );
		
		Add( pVideoOutputMon1 = new PushButtonIndicatorSingleLight( _sts, "Video Output Mon 1" ) );
		Add( pVideoOutputMon2 = new PushButtonIndicatorSingleLight( _sts, "Video Output Mon 2" ) );
		Add( pVideoOutputDownLink = new PushButtonIndicatorSingleLight( _sts, "Video Output Down Link" ) );
		Add( pVideoOutputDTV = new PushButtonIndicatorSingleLight( _sts, "Video Output DTV" ) );
		Add( pVideoOutputMUX1L = new PushButtonIndicatorSingleLight( _sts, "Video Output MUX 1L" ) );
		Add( pVideoOutputMUX1R = new PushButtonIndicatorSingleLight( _sts, "Video Output MUX 1R" ) );
		Add( pVideoOutputMUX2L = new PushButtonIndicatorSingleLight( _sts, "Video Output MUX 2L" ) );
		Add( pVideoOutputMUX2R = new PushButtonIndicatorSingleLight( _sts, "Video Output MUX 2R" ) );
		
		Add( pMenuALC = new PushButtonIndicatorSingleLight( _sts, "Menu ALC" ) );
		Add( pMenuManGain = new PushButtonIndicatorSingleLight( _sts, "Menu Man Gain" ) );
		Add( pMenuColorBalLTLevel = new PushButtonIndicatorSingleLight( _sts, "Menu Color Bal/LT Level" ) );
		Add( pMode1 = new PushButtonIndicatorSingleLight( _sts, "Mode 1" ) );
		Add( pMode2 = new PushButtonIndicatorSingleLight( _sts, "Mode 2" ) );
		Add( pMode3 = new PushButtonIndicatorSingleLight( _sts, "Mode 3" ) );

		Add( pCameraPowerA = new StdSwitch3( _sts, "Camera Power A" ) );
		Add( pCameraPowerB = new StdSwitch3( _sts, "Camera Power B" ) );
		Add( pCameraPowerC = new StdSwitch3( _sts, "Camera Power C" ) );
		Add( pCameraPowerD = new StdSwitch3( _sts, "Camera Power D" ) );
		Add( pCameraPowerRMS = new StdSwitch3( _sts, "Camera Power RMS" ) );

		Add(pCameraPan = new StdSwitch3(_sts, "PAN"));
		Add(pCameraTilt = new StdSwitch3(_sts, "TILT"));
		Add(pPanTiltRate= new StdSwitch3(_sts, "CAMERA_RATE"));

		Add(pPLBDLights[0] = new StdSwitch2(_sts, "PLBD FLOOD FWD STBD"));
		Add(pPLBDLights[1] = new StdSwitch2(_sts, "PLBD FLOOD FWD PORT"));
		Add(pPLBDLights[2] = new StdSwitch2(_sts, "PLBD FLOOD MID STBD"));
		Add(pPLBDLights[3] = new StdSwitch2(_sts, "PLBD FLOOD MID PORT"));
		Add(pPLBDLights[4] = new StdSwitch2(_sts, "PLBD FLOOD AFT STBD"));
		Add(pPLBDLights[5] = new StdSwitch2(_sts, "PLBD FLOOD AFT PORT"));
		Add(pPLBDLights[6] = new StdSwitch2(_sts, "PLBD FWD BHD"));
		Add(pDockingLight = new StdSwitch3(_sts, "PLBD DOCKING"));
		Add(pRMSLight = new StdSwitch2(_sts, "RMS SPOTLIGHT PORT"));

		pPanTiltRate->SetLabel(0, "LOW");
		pPanTiltRate->SetLabel(1, "HIGH");
		pPanTiltRate->SetLabel(2, "RESET");

		for(int i=0;i<7;i++) {
			pPLBDLights[i]->SetLabel(0, "OFF");
			pPLBDLights[i]->SetLabel(1, "ON");
			pPLBDLights[i]->SetInitialPosition(0);
		}
		pRMSLight->SetLabel(0, "OFF");
		pRMSLight->SetLabel(1, "ON");
		pRMSLight->SetInitialPosition(0);
		pDockingLight->SetLabel(0, "OFF");
		pDockingLight->SetLabel(1, "DIM");
		pDockingLight->SetLabel(2, "BRIGHT");
		pDockingLight->SetInitialPosition(0);
	}

	PanelA7U::~PanelA7U()
	{
	}

	void PanelA7U::AddMeshes( const VECTOR3 &ofs )
	{
		SetHasOwnVCMesh();

		if (mesh_index == MESH_UNDEFINED)
		{
			mesh_index = STS()->AddMesh( hPanelMesh, &ofs );
			STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		}
		return;
	}

	void PanelA7U::SetMeshVisibility( bool visible )
	{
		if (visible) STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		else STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_NEVER );
		return;
	}

	UINT PanelA7U::GetVCMeshIndex( void ) const
	{
		return mesh_index;
	}

	void PanelA7U::DefineVC()
	{
		const VECTOR3 switch_rot_vert =  _V(-1, 0, 0);
		const VECTOR3 switch_rot_horz = _V(0.0, -0.9499, 0.3126);
		VECTOR3 switch_pull = _V( 0.0, 0.3126, 0.9499 );
		VECTOR3 push_dir = -switch_pull;

		AddAIDToMouseEventList(AID_A7U);

		pCameraPowerA_TB->DefineMeshGroups( mesh_index, GRP_DS4_U_A7U_VC, GRP_DS4_L_A7U_VC );
		pCameraPowerA_TB->SetInactiveSegment( TB_OFF );

		pCameraPowerB_TB->DefineMeshGroups( mesh_index, GRP_DS5_U_A7U_VC, GRP_DS5_L_A7U_VC );
		pCameraPowerB_TB->SetInactiveSegment( TB_OFF );
		
		pCameraPowerC_TB->DefineMeshGroups( mesh_index, GRP_DS6_U_A7U_VC, GRP_DS6_L_A7U_VC );
		pCameraPowerC_TB->SetInactiveSegment( TB_OFF );
		
		pCameraPowerD_TB->DefineMeshGroups( mesh_index, GRP_DS7_U_A7U_VC, GRP_DS7_L_A7U_VC );
		pCameraPowerD_TB->SetInactiveSegment( TB_OFF );
		
		pCameraPowerRMS_TB->DefineMeshGroups( mesh_index, GRP_DS8_U_A7U_VC, GRP_DS8_L_A7U_VC );
		pCameraPowerRMS_TB->SetInactiveSegment( TB_OFF );

		pMasterAlarm->SetStateOffset( 1, 0.139648f, 0.0f );
		pMasterAlarm->SetDirection( push_dir );
		pMasterAlarm->SetMouseRegion( 0.860198f, 0.080638f, 0.916229f, 0.133652f );
		pMasterAlarm->DefineMeshGroup( mesh_index, GRP_S51_A7U_VC );

		pVideoInputA->SetStateOffset( 1, 0.0f, 0.488281f );
		pVideoInputA->SetDirection( push_dir );
		pVideoInputA->SetMouseRegion( 0.449159f, 0.393826f, 0.491246f, 0.448441f );
		pVideoInputA->DefineMeshGroup( mesh_index, GRP_S27_A7U_VC );

		pVideoInputB->SetStateOffset( 1, 0.0f, 0.488281f );
		pVideoInputB->SetDirection( push_dir );
		pVideoInputB->SetMouseRegion( 0.393562f, 0.393563f, 0.435272f, 0.448867f );
		pVideoInputB->DefineMeshGroup( mesh_index, GRP_S28_A7U_VC );

		pVideoInputC->SetStateOffset( 1, 0.0f, 0.488281f );
		pVideoInputC->SetDirection( push_dir );
		pVideoInputC->SetMouseRegion( 0.338530f, 0.394243f, 0.380856f, 0.448831f );
		pVideoInputC->DefineMeshGroup( mesh_index, GRP_S29_A7U_VC );

		pVideoInputD->SetStateOffset( 1, 0.0f, 0.488281f );
		pVideoInputD->SetDirection( push_dir );
		pVideoInputD->SetMouseRegion( 0.449167f, 0.472623f, 0.491354f, 0.527204f );
		pVideoInputD->DefineMeshGroup( mesh_index, GRP_S30_A7U_VC );

		pVideoInputRMS->SetStateOffset( 1, 0.0f, 0.488281f );
		pVideoInputRMS->SetDirection( push_dir );
		pVideoInputRMS->SetMouseRegion( 0.393252f, 0.472663f, 0.435020f, 0.527389f );
		pVideoInputRMS->DefineMeshGroup( mesh_index, GRP_S31_A7U_VC );

		pVideoInputFltDeck->SetStateOffset( 1, 0.0f, 0.488281f );
		pVideoInputFltDeck->SetDirection( push_dir );
		pVideoInputFltDeck->SetMouseRegion( 0.338498f, 0.473108f, 0.380514f, 0.527508f );
		pVideoInputFltDeck->DefineMeshGroup( mesh_index, GRP_S32_A7U_VC );

		pVideoInputMidDeck->SetStateOffset( 1, 0.0f, 0.488281f );
		pVideoInputMidDeck->SetDirection( push_dir );
		pVideoInputMidDeck->SetMouseRegion( 0.283088f, 0.472665f, 0.325025f, 0.526744f );
		pVideoInputMidDeck->DefineMeshGroup( mesh_index, GRP_S55_A7U_VC );

		pVideoInputPL1->SetStateOffset( 1, 0.0f, 0.488281f );
		pVideoInputPL1->SetDirection( push_dir );
		pVideoInputPL1->SetMouseRegion( 0.449259f, 0.564221f, 0.491032f, 0.618662f );
		pVideoInputPL1->DefineMeshGroup( mesh_index, GRP_S33_A7U_VC );

		pVideoInputPL2->SetStateOffset( 1, 0.0f, 0.488281f );
		pVideoInputPL2->SetDirection( push_dir );
		pVideoInputPL2->SetMouseRegion( 0.392937f, 0.563183f, 0.435327f, 0.619063f );
		pVideoInputPL2->DefineMeshGroup( mesh_index, GRP_S34_A7U_VC );

		pVideoInputPL3->SetStateOffset( 1, 0.0f, 0.488281f );
		pVideoInputPL3->SetDirection( push_dir );
		pVideoInputPL3->SetMouseRegion( 0.337886f, 0.563028f, 0.380877f, 0.618987f );
		pVideoInputPL3->DefineMeshGroup( mesh_index, GRP_S35_A7U_VC );

		pVideoInputMUX1->SetStateOffset( 1, 0.0f, 0.488281f );
		pVideoInputMUX1->SetDirection( push_dir );
		pVideoInputMUX1->SetMouseRegion( 0.448892f, 0.641357f, 0.490923f, 0.695677f );
		pVideoInputMUX1->DefineMeshGroup( mesh_index, GRP_S36_A7U_VC );

		pVideoInputMUX2->SetStateOffset( 1, 0.0f, 0.488281f );
		pVideoInputMUX2->SetDirection( push_dir );
		pVideoInputMUX2->SetMouseRegion( 0.393810f, 0.642569f, 0.434115f, 0.696355f );
		pVideoInputMUX2->DefineMeshGroup( mesh_index, GRP_S37_A7U_VC );

		pVideoInputTest->SetStateOffset( 1, 0.0f, 0.488281f );
		pVideoInputTest->SetDirection( push_dir );
		pVideoInputTest->SetMouseRegion( 0.338595f, 0.641467f, 0.379875f, 0.695877f );
		pVideoInputTest->DefineMeshGroup( mesh_index, GRP_S38_A7U_VC );

		pVideoOutputMon1->SetStateOffset( 1, 0.0f, 0.488281f );
		pVideoOutputMon1->SetDirection( push_dir );
		pVideoOutputMon1->SetMouseRegion( 0.155487f, 0.394234f, 0.197079f, 0.448819f );
		pVideoOutputMon1->DefineMeshGroup( mesh_index, GRP_S19_A7U_VC );

		pVideoOutputMon2->SetStateOffset( 1, 0.0f, 0.488281f );
		pVideoOutputMon2->SetDirection( push_dir );
		pVideoOutputMon2->SetMouseRegion( 0.155040f, 0.472985f, 0.197537f, 0.527201f );
		pVideoOutputMon2->DefineMeshGroup( mesh_index, GRP_S23_A7U_VC );

		pVideoOutputDownLink->SetStateOffset( 1, 0.0f, 0.488281f );
		pVideoOutputDownLink->SetDirection( push_dir );
		pVideoOutputDownLink->SetMouseRegion( 0.099781f, 0.394072f, 0.141733f, 0.448912f );
		pVideoOutputDownLink->DefineMeshGroup( mesh_index, GRP_S22_A7U_VC );

		pVideoOutputDTV->SetStateOffset( 1, 0.0f, 0.488281f );
		pVideoOutputDTV->SetDirection( push_dir );
		pVideoOutputDTV->SetMouseRegion( 0.099828f, 0.473221f, 0.141774f, 0.527048f );
		pVideoOutputDTV->DefineMeshGroup( mesh_index, GRP_S26_A7U_VC );

		pVideoOutputMUX1L->SetStateOffset( 1, 0.0f, 0.488281f );
		pVideoOutputMUX1L->SetDirection( push_dir );
		pVideoOutputMUX1L->SetMouseRegion( 0.155033f, 0.563973f, 0.197524f, 0.617812f );
		pVideoOutputMUX1L->DefineMeshGroup( mesh_index, GRP_S20_A7U_VC );

		pVideoOutputMUX1R->SetStateOffset( 1, 0.0f, 0.488281f );
		pVideoOutputMUX1R->SetDirection( push_dir );
		pVideoOutputMUX1R->SetMouseRegion( 0.098967f, 0.563465f, 0.141770f, 0.618104f );
		pVideoOutputMUX1R->DefineMeshGroup( mesh_index, GRP_S21_A7U_VC );

		pVideoOutputMUX2L->SetStateOffset( 1, 0.0f, 0.488281f );
		pVideoOutputMUX2L->SetDirection( push_dir );
		pVideoOutputMUX2L->SetMouseRegion( 0.155095f, 0.642404f, 0.197513f, 0.696950f );
		pVideoOutputMUX2L->DefineMeshGroup( mesh_index, GRP_S24_A7U_VC );

		pVideoOutputMUX2R->SetStateOffset( 1, 0.0f, 0.488281f );
		pVideoOutputMUX2R->SetDirection( push_dir );
		pVideoOutputMUX2R->SetMouseRegion( 0.099998f, 0.643432f, 0.141877f, 0.696410f );
		pVideoOutputMUX2R->DefineMeshGroup( mesh_index, GRP_S25_A7U_VC );

		pMenuALC->SetStateOffset( 1, 0.0f, 0.488281f );
		pMenuALC->SetDirection( push_dir );
		pMenuALC->SetMouseRegion( 0.365113f, 0.907374f, 0.407299f, 0.961727f );
		pMenuALC->DefineMeshGroup( mesh_index, GRP_S45_A7U_VC );

		pMenuManGain->SetStateOffset( 1, 0.0f, 0.488281f );
		pMenuManGain->SetDirection( push_dir );
		pMenuManGain->SetMouseRegion( 0.311151f, 0.908085f, 0.352909f, 0.962547f );
		pMenuManGain->DefineMeshGroup( mesh_index, GRP_S46_A7U_VC );

		pMenuColorBalLTLevel->SetStateOffset( 1, 0.0f, 0.488281f );
		pMenuColorBalLTLevel->SetDirection( push_dir );
		pMenuColorBalLTLevel->SetMouseRegion( 0.257292f, 0.906941f, 0.299945f, 0.962167f );
		pMenuColorBalLTLevel->DefineMeshGroup( mesh_index, GRP_S47_A7U_VC );

		pMode1->SetStateOffset( 1, 0.0f, 0.488281f );
		pMode1->SetDirection( push_dir );
		pMode1->SetMouseRegion( 0.204969f, 0.906908f, 0.274620f, 0.961676f );
		pMode1->DefineMeshGroup( mesh_index, GRP_S48_A7U_VC );

		pMode2->SetStateOffset( 1, 0.0f, 0.488281f );
		pMode2->SetDirection( push_dir );
		pMode2->SetMouseRegion( 0.150506f, 0.907584f, 0.192701f, 0.961874f );
		pMode2->DefineMeshGroup( mesh_index, GRP_S49_A7U_VC );

		pMode3->SetStateOffset( 1, 0.0f, 0.488281f );
		pMode3->SetDirection( push_dir );
		pMode3->SetMouseRegion( 0.097100f, 0.906973f, 0.139273f, 0.961415f );
		pMode3->DefineMeshGroup( mesh_index, GRP_S50_A7U_VC );

		pCameraPowerA->DefineSwitchGroup( GRP_S13_A7U_VC );
		pCameraPowerA->SetInitialAnimState( 0.5f );
		pCameraPowerA->SetMouseRegion( 0.449941f, 0.216160f, 0.489953f, 0.272383f );
		pCameraPowerA->SetReference( _V( -0.0244, 2.7983, 12.3060 ), switch_rot_vert );
		pCameraPowerA->SetSpringLoaded( true );

		pCameraPowerB->DefineSwitchGroup( GRP_S14_A7U_VC );
		pCameraPowerB->SetInitialAnimState( 0.5f );
		pCameraPowerB->SetMouseRegion( 0.393330f, 0.216516f, 0.432321f, 0.270997f );
		pCameraPowerB->SetReference( _V( -0.0244, 2.7983, 12.3060 ), switch_rot_vert );
		pCameraPowerB->SetSpringLoaded( true );

		pCameraPowerC->DefineSwitchGroup( GRP_S15_A7U_VC );
		pCameraPowerC->SetInitialAnimState( 0.5f );
		pCameraPowerC->SetMouseRegion( 0.337538f, 0.215611f, 0.380122f, 0.271418f );
		pCameraPowerC->SetReference( _V( -0.0244, 2.7983, 12.3060 ), switch_rot_vert );
		pCameraPowerC->SetSpringLoaded( true );

		pCameraPowerD->DefineSwitchGroup( GRP_S16_A7U_VC );
		pCameraPowerD->SetInitialAnimState( 0.5f );
		pCameraPowerD->SetMouseRegion( 0.285335f, 0.215972f, 0.322215f, 0.270588f );
		pCameraPowerD->SetReference( _V( -0.0244, 2.7983, 12.3060 ), switch_rot_vert );
		pCameraPowerD->SetSpringLoaded( true );

		pCameraPowerRMS->DefineSwitchGroup( GRP_S17_A7U_VC );
		pCameraPowerRMS->SetInitialAnimState( 0.5f );
		pCameraPowerRMS->SetMouseRegion( 0.226493f, 0.212685f, 0.269290f, 0.270410f );
		pCameraPowerRMS->SetReference( _V( -0.0244, 2.7983, 12.3060 ), switch_rot_vert );
		pCameraPowerRMS->SetSpringLoaded( true );

		pCameraPan->DefineSwitchGroup( GRP_S44_A7U_VC );
		pCameraPan->SetInitialAnimState(0.5);
		pCameraPan->SetMouseRegion(0.099787f, 0.768763f, 0.153872f, 0.873155f);
		pCameraPan->SetReference(_V(-0.2, 2.584, 12.373), switch_rot_horz);
		pCameraPan->SetOrientation(true);
		pCameraPan->SetSpringLoaded(true);

		pCameraTilt->DefineSwitchGroup( GRP_S43_A7U_VC );
		pCameraTilt->SetInitialAnimState(0.5);
		pCameraTilt->SetMouseRegion(0.174297f, 0.773431f, 0.231700f, 0.856463f);
		pCameraTilt->SetReference(_V(-0.158, 2.583, 12.373), switch_rot_vert);
		pCameraTilt->SetSpringLoaded(true);

		pPanTiltRate->DefineSwitchGroup( GRP_S56_A7U_VC );
		pPanTiltRate->SetInitialAnimState(0.5);
		pPanTiltRate->SetMouseRegion(0.412282f, 0.769816f, 0.471316f, 0.868281f);
		pPanTiltRate->SetReference(_V(-0.043, 2.583, 12.373), switch_rot_vert);
		pPanTiltRate->SetSpringLoaded(true, 2);

		for(int i=0;i<7;i++) {
			pPLBDLights[i]->SetInitialAnimState(0.5);
		}
		pPLBDLights[0]->DefineSwitchGroup( GRP_S5_A7U_VC );
		pPLBDLights[0]->SetMouseRegion(0.865516f, 0.559828f, 0.933777f, 0.635897f);
		pPLBDLights[0]->SetReference(_V(0.207, 2.666, 12.347), switch_rot_vert);

		pPLBDLights[1]->DefineSwitchGroup( GRP_S6_A7U_VC );
		pPLBDLights[1]->SetMouseRegion(0.777810f, 0.558671f, 0.844142f, 0.639297f);
		pPLBDLights[1]->SetReference(_V(0.1635, 2.666, 12.347), switch_rot_vert);
		pPLBDLights[1]->SetReference(_V(0.1635, 2.666, 12.347), switch_rot_vert);

		pPLBDLights[2]->DefineSwitchGroup( GRP_S3_A7U_VC );
		pPLBDLights[2]->SetMouseRegion(0.869307f, 0.401896f, 0.931245f, 0.487941f);
		pPLBDLights[2]->SetReference(_V(0.207, 2.725, 12.3285), switch_rot_vert);

		pPLBDLights[3]->DefineSwitchGroup( GRP_S4_A7U_VC );
		pPLBDLights[3]->SetMouseRegion(0.779615f, 0.407602f, 0.844617f, 0.479162f);
		pPLBDLights[3]->SetReference(_V(0.1635, 2.725, 12.3285), switch_rot_vert);

		pPLBDLights[4]->DefineSwitchGroup( GRP_S1_A7U_VC );
		pPLBDLights[4]->SetMouseRegion(0.869901f, 0.264662f, 0.929604f, 0.335401f);
		pPLBDLights[4]->SetReference(_V(0.207, 2.7795, 12.311), switch_rot_vert);

		pPLBDLights[5]->DefineSwitchGroup( GRP_S2_A7U_VC );
		pPLBDLights[5]->SetMouseRegion(0.778263f, 0.264470f, 0.848168f, 0.332787f);
		pPLBDLights[5]->SetReference(_V(0.1635, 2.7795, 12.311), switch_rot_vert);

		pPLBDLights[6]->DefineSwitchGroup( GRP_S8_A7U_VC );
		pPLBDLights[6]->SetMouseRegion(0.777841f, 0.683267f, 0.843850f, 0.776423f);
		pPLBDLights[6]->SetReference(_V(0.1635, 2.6155, 12.363), switch_rot_vert);

		pRMSLight->SetInitialAnimState(0.5);
		pRMSLight->DefineSwitchGroup( GRP_S9_A7U_VC );
		pRMSLight->SetMouseRegion(0.887903f, 0.839638f, 0.927633f, 0.892271f);
		pRMSLight->SetReference(_V(0.1635, 2.5595, 12.3805), switch_rot_vert);

		pDockingLight->SetInitialAnimState(0.5);
		pDockingLight->DefineSwitchGroup( GRP_S7_A7U_VC );
		pDockingLight->SetMouseRegion(0.870177f, 0.691057f, 0.931207f, 0.773388f);
		pDockingLight->SetReference(_V(0.207, 2.6155, 12.363), switch_rot_vert);
	}

	void PanelA7U::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea(AID_A7U, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
		oapiVCSetAreaClickmode_Quadrilateral(AID_A7U,
			_V(-0.264, 2.893, 12.277)+ofs, _V(0.255, 2.893, 12.277)+ofs,
			_V(-0.264, 2.507, 12.397)+ofs, _V(0.255, 2.507, 12.397)+ofs);
		
		return;
	}

	void PanelA7U::DefineVCAnimations( UINT vcidx )
	{
		AtlantisPanel::DefineVCAnimations( mesh_index );
		return;
	}

	void PanelA7U::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle("PLB_LIGHTS", 16);
		for(int i=0;i<7;i++) pPLBDLights[i]->ConnectPort(1, pBundle, i);
		pDockingLight->ConnectPort(1, pBundle, 7); // DIM
		pDockingLight->ConnectPort(2, pBundle, 8); // BRIGHT
		pDockingLight->ConnectSwitchPosition(1, 1);
		pDockingLight->ConnectSwitchPosition(2, 2);
		pRMSLight->ConnectPort(1, pBundle, 9);

		pBundle = STS()->BundleManager()->CreateBundle( "VCU_input_1", 16 );
		pVideoInputA->ConnectPushButton( pBundle, 0 );
		pVideoInputB->ConnectPushButton( pBundle, 1 );
		pVideoInputC->ConnectPushButton( pBundle, 2 );
		pVideoInputD->ConnectPushButton( pBundle, 3 );
		pVideoInputRMS->ConnectPushButton( pBundle, 4 );
		pVideoInputFltDeck->ConnectPushButton( pBundle, 5 );
		pVideoInputMidDeck->ConnectPushButton( pBundle, 6 );
		pVideoInputPL1->ConnectPushButton( pBundle, 7 );
		pVideoInputPL2->ConnectPushButton( pBundle, 8 );
		pVideoInputPL3->ConnectPushButton( pBundle, 9 );
		pVideoInputMUX1->ConnectPushButton( pBundle, 10 );
		pVideoInputMUX2->ConnectPushButton( pBundle, 11 );
		pVideoInputTest->ConnectPushButton( pBundle, 12 );
		pVideoOutputMon1->ConnectPushButton( pBundle, 13 );
		pVideoOutputMon2->ConnectPushButton( pBundle, 14 );
		pVideoOutputDownLink->ConnectPushButton( pBundle, 15 );

		pBundle = STS()->BundleManager()->CreateBundle( "VCU_input_2", 16 );
		pVideoOutputDTV->ConnectPushButton( pBundle, 0 );
		pVideoOutputMUX1L->ConnectPushButton( pBundle, 1 );
		pVideoOutputMUX1R->ConnectPushButton( pBundle, 2 );
		pVideoOutputMUX2L->ConnectPushButton( pBundle, 3 );
		pVideoOutputMUX2R->ConnectPushButton( pBundle, 4 );
		pMenuALC->ConnectPushButton( pBundle, 5 );
		pMenuManGain->ConnectPushButton( pBundle, 6 );
		pMenuColorBalLTLevel->ConnectPushButton( pBundle, 7 );
		pMode1->ConnectPushButton( pBundle, 8 );
		pMode2->ConnectPushButton( pBundle, 9 );
		pMode3->ConnectPushButton( pBundle, 10 );
		pCameraPowerA->ConnectPort( 2, pBundle, 11 );
		pCameraPowerA->ConnectPort( 1, pBundle, 12 );
		pCameraPowerB->ConnectPort( 2, pBundle, 13 );
		pCameraPowerB->ConnectPort( 1, pBundle, 14 );
		pCameraPowerC->ConnectPort( 2, pBundle, 15 );

		pBundle = STS()->BundleManager()->CreateBundle( "VCU_input_3", 16 );
		pCameraPowerC->ConnectPort( 1, pBundle, 0 );
		pCameraPowerD->ConnectPort( 2, pBundle, 1 );
		pCameraPowerD->ConnectPort( 1, pBundle, 2 );
		pCameraPowerRMS->ConnectPort( 2, pBundle, 3 );
		pCameraPowerRMS->ConnectPort( 1, pBundle, 4 );
		pPanTiltRate->ConnectPort( 2, pBundle, 5 );
		pPanTiltRate->ConnectPort( 1, pBundle, 6 );
		pCameraTilt->ConnectPort( 2, pBundle, 7 );
		pCameraTilt->ConnectPort( 1, pBundle, 8 );
		pCameraPan->ConnectPort( 1, pBundle, 9 );
		pCameraPan->ConnectPort( 2, pBundle, 10 );

		pBundle = STS()->BundleManager()->CreateBundle( "VCU_output_1", 16 );
		pCameraPowerA_TB->SetInput( pBundle, 0, TB_ON );
		pCameraPowerB_TB->SetInput( pBundle, 1, TB_ON );
		pCameraPowerC_TB->SetInput( pBundle, 2, TB_ON );
		pCameraPowerD_TB->SetInput( pBundle, 3, TB_ON );
		pCameraPowerRMS_TB->SetInput( pBundle, 4, TB_ON );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA4_1", 16 );
		pVideoOutputMon1->ConnectLight( 0, pBundle, 3 );
		pVideoOutputMUX1L->ConnectLight( 0, pBundle, 7 );
		pVideoOutputMUX1R->ConnectLight( 0, pBundle, 11 );
		pVideoOutputMon2->ConnectLight( 0, pBundle, 15 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA4_2", 16 );
		pVideoOutputMUX2L->ConnectLight( 0, pBundle, 3 );
		pVideoOutputMUX2R->ConnectLight( 0, pBundle, 7 );
		pVideoInputA->ConnectLight( 0, pBundle, 11 );
		pVideoInputB->ConnectLight( 0, pBundle, 15 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA4_3", 16 );
		pVideoInputC->ConnectLight( 0, pBundle, 3 );
		pVideoInputPL1->ConnectLight( 0, pBundle, 7 );
		pVideoInputPL2->ConnectLight( 0, pBundle, 11 );
		pVideoInputPL3->ConnectLight( 0, pBundle, 15 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA4_4", 16 );
		pMode1->ConnectLight( 0, pBundle, 3 );
		pMode2->ConnectLight( 0, pBundle, 7 );
		pMode3->ConnectLight( 0, pBundle, 11 );
		pMasterAlarm->ConnectLight( 0, pBundle, 15 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA4_5", 16 );
		pVideoInputMidDeck->ConnectLight( 0, pBundle, 3 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA5_1", 16 );
		pVideoOutputDownLink->ConnectLight( 0, pBundle, 11 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA5_2", 16 );
		pVideoOutputDTV->ConnectLight( 0, pBundle, 7 );
		pVideoInputD->ConnectLight( 0, pBundle, 11 );
		pVideoInputRMS->ConnectLight( 0, pBundle, 15 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA5_3", 16 );
		pVideoInputFltDeck->ConnectLight( 0, pBundle, 3 );
		pVideoInputMUX1->ConnectLight( 0, pBundle, 7 );
		pVideoInputMUX2->ConnectLight( 0, pBundle, 11 );
		pVideoInputTest->ConnectLight( 0, pBundle, 15 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA5_4", 16 );
		pMenuALC->ConnectLight( 0, pBundle, 3 );
		pMenuManGain->ConnectLight( 0, pBundle, 7 );
		pMenuColorBalLTLevel->ConnectLight( 0, pBundle, 11 );
		pMasterAlarm->ConnectLight( 1, pBundle, 15 );

		AtlantisPanel::Realize();
	}
};
