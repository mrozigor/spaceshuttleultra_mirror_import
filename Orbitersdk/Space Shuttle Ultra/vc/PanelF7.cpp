#include "PanelF7.h"
#include "../Atlantis.h"
#include "../meshres_vc.h"
#include "MDU.h"
#include "../Atlantis_defs.h"


namespace vc {

	PanelF7::PanelF7(Atlantis* _sts)
		: AtlantisPanel(_sts, "F7")
	{
		Add(pCRT1 = new MDU(_sts, "CRT1", MDUID_CRT1, true));
		Add(pCRT2 = new MDU(_sts, "CRT2", MDUID_CRT2, true));
		Add(pCRT3 = new MDU(_sts, "CRT3", MDUID_CRT3, true));
		Add(pMFD1 = new MDU(_sts, "MFD1", MDUID_MFD1, true));
		Add(pMFD2 = new MDU(_sts, "MFD2", MDUID_MFD2, true));

		Add( pEventTime = new _7SegDisp_EventTime( _sts, "Event Time" ) );

		Add( pCWLight[0] = new StandardSingleLight( _sts, "CW O2 PRESS" ) );
		Add( pCWLight[1] = new StandardSingleLight( _sts, "CW H2 PRESS" ) );
		Add( pCWLight[2] = new StandardSingleLight( _sts, "CW FUEL CELL REAC" ) );
		Add( pCWLight[3] = new StandardSingleLight( _sts, "CW FUEL CELL STACK TEMP" ) );
		Add( pCWLight[4] = new StandardSingleLight( _sts, "CW FUEL CELL PUMP" ) );

		Add( pCWLight[5] = new StandardSingleLight( _sts, "CW CABIN ATM" ) );
		Add( pCWLight[6] = new StandardSingleLight( _sts, "CW O2 HEATER TEMP" ) );
		Add( pCWLight[7] = new StandardSingleLight( _sts, "CW MAIN BUS UNDERVOLT" ) );
		Add( pCWLight[8] = new StandardSingleLight( _sts, "CW AC VOLTAGE" ) );
		Add( pCWLight[9] = new StandardSingleLight( _sts, "CW AC OVERLOAD" ) );

		Add( pCWLight[10] = new StandardSingleLight( _sts, "CW FREON LOOP" ) );
		Add( pCWLight[11] = new StandardSingleLight( _sts, "CW AV BAY/CABIN AIR" ) );
		Add( pCWLight[12] = new StandardSingleLight( _sts, "CW IMU" ) );
		Add( pCWLight[13] = new StandardSingleLight( _sts, "CW FWD RCS" ) );
		Add( pCWLight[14] = new StandardSingleLight( _sts, "CW RCS JET" ) );

		Add( pCWLight[15] = new StandardSingleLight( _sts, "CW H2O LOOP" ) );
		Add( pCWLight[16] = new StandardSingleLight( _sts, "CW RGA/ACCEL" ) );
		Add( pCWLight[17] = new StandardSingleLight( _sts, "CW AIR DATA" ) );
		Add( pCWLight[18] = new StandardSingleLight( _sts, "CW LEFT RCS" ) );
		Add( pCWLight[19] = new StandardSingleLight( _sts, "CW RIGHT RCS" ) );

		Add( pCWLight[20] = new StandardSingleLight( _sts, "CW LEFT RHC" ) );
		Add( pCWLight[21] = new StandardSingleLight( _sts, "CW RIGHT/AFT RHC" ) );
		Add( pCWLight[22] = new StandardSingleLight( _sts, "CW LEFT OMS" ) );
		Add( pCWLight[23] = new StandardSingleLight( _sts, "CW RIGHT OMS" ) );

		Add( pCWLight[24] = new StandardSingleLight( _sts, "CW PAYLOAD WARNING" ) );
		Add( pCWLight[25] = new StandardSingleLight( _sts, "CW GPC" ) );
		Add( pCWLight[26] = new StandardSingleLight( _sts, "CW FCS SATURATION" ) );
		Add( pCWLight[27] = new StandardSingleLight( _sts, "CW OMS KIT" ) );
		Add( pCWLight[28] = new StandardSingleLight( _sts, "CW OMS TVC" ) );

		Add( pCWLight[29] = new StandardSingleLight( _sts, "CW PAYLOAD CAUTION" ) );
		Add( pCWLight[30] = new StandardSingleLight( _sts, "CW PRIMARY C/W" ) );
		Add( pCWLight[31] = new StandardSingleLight( _sts, "CW FCS CHANNEL" ) );
		Add( pCWLight[32] = new StandardSingleLight( _sts, "CW MPS" ) );
		
		Add( pCWLight[33] = new StandardSingleLight( _sts, "CW BACKUP C/W ALARM" ) );
		Add( pCWLight[34] = new StandardSingleLight( _sts, "CW APU TEMP" ) );
		Add( pCWLight[35] = new StandardSingleLight( _sts, "CW APU OVERSPEED" ) );
		Add( pCWLight[36] = new StandardSingleLight( _sts, "CW APU UNDERSPEED" ) );
		Add( pCWLight[37] = new StandardSingleLight( _sts, "CW HYD PRESS" ) );

		Add( pMainEngStatus[0] = new StandardDoubleLight( _sts, "MAIN ENGINE STATUS CTR" ) );
		Add( pMainEngStatus[1] = new StandardDoubleLight( _sts, "MAIN ENGINE STATUS LEFT" ) );
		Add( pMainEngStatus[2] = new StandardDoubleLight( _sts, "MAIN ENGINE STATUS RIGHT" ) );
		
		Add( pSMAlert = new StandardSingleLight( _sts, "SM ALERT" ) );
	}

	PanelF7::~PanelF7()
	{
	}

	void PanelF7::DefineVC()
	{
		AddAIDToMouseEventList(AID_F7);

		pCRT1->SetMouseRegion(0.0f, 0.0f, 0.330f, 0.5136f);
		pCRT1->SetPowerButtonArea(0.044091f, 0.806369f, 0.129668f, 0.901916f);
		pCRT1->SetEdgekeyArea(0.226685f, 0.900011f, 0.784375f, 0.948678f);

		pCRT2->SetMouseRegion(0.6715f, 0.0f, 1.0f, 0.5136f);
		pCRT2->SetPowerButtonArea(0.0f, 0.829584f, 0.091723f, 0.898716f);
		pCRT2->SetEdgekeyArea(0.206909f, 0.910128f, 0.776398f, 0.954403f);

		pCRT3->SetMouseRegion(0.340f, 0.26f, 0.6715f, 0.75f);
		pCRT3->SetPowerButtonArea(0.0f, 0.860329f, 0.145023f, 0.953701f);
		pCRT3->SetEdgekeyArea(0.196294f, 0.951293f, 0.787676f, 0.995252f);

		pMFD1->SetMouseRegion(0.0f, 0.5284f, 0.330f, 1.0f);
		pMFD1->SetPowerButtonArea(0.044176f, 0.841141f, 0.125769f, 0.962449f);
		pMFD1->SetEdgekeyArea(0.224502f, 0.937218f, 0.784168f, 0.984580f);

		pMFD2->SetMouseRegion(0.6715f, 0.5284f, 1.0f, 1.0f);
		pMFD2->SetPowerButtonArea(0.0f, 0.859715f, 0.099139f, 0.965387f);
		pMFD2->SetEdgekeyArea(0.211796f, 0.942848f, 0.779738f, 0.992954f);

		pCRT1->DefineVCGroup( GRP_CRT1_VC );
		pCRT2->DefineVCGroup( GRP_CRT2_VC );
		pCRT3->DefineVCGroup( GRP_CRT3_VC );
		pMFD1->DefineVCGroup( GRP_MFD1_VC );
		pMFD2->DefineVCGroup( GRP_MFD2_VC );

		pEventTime->DefineMesh( STS()->mesh_vc );
		pEventTime->DefineComponent( GRP_F7_EVENT_S_VC, true, false, false, _7SD_STATE_NUM0 );
		pEventTime->DefineComponent( GRP_F7_EVENT_S10_VC, true, false, false, _7SD_STATE_NUM0 );
		pEventTime->DefineComponent( GRP_F7_EVENT_M_VC, true, false, false, _7SD_STATE_NUM0 );
		pEventTime->DefineComponent( GRP_F7_EVENT_M10_VC, true, false, false, _7SD_STATE_NUM0 );
		pEventTime->SetLocation( true );

		pCWLight[0]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_O2_PRESS_VC );
		pCWLight[0]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[1]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_H2_PRESS_VC );
		pCWLight[1]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[2]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_FUEL_CELL_REAC_VC );
		pCWLight[2]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[3]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_FUEL_CELL_STACK_TEMP_VC );
		pCWLight[3]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[4]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_FUEL_CELL_PUMP_VC );
		pCWLight[4]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[5]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_CABIN_ATM_VC );
		pCWLight[5]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[6]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_O2_HEATER_TEMP_VC );
		pCWLight[6]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[7]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_MAIN_BUS_UNDERVOLT_VC );
		pCWLight[7]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[8]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_AC_VOLTAGE_VC );
		pCWLight[8]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[9]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_AC_OVERLOAD_VC );
		pCWLight[9]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[10]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_FREON_LOOP_VC );
		pCWLight[10]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[11]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_AV_BAY_CABIN_AIR_VC );
		pCWLight[11]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[12]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_IMU_VC );
		pCWLight[12]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[13]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_FWD_RCS_VC );
		pCWLight[13]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[14]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_RCS_JET_VC );
		pCWLight[14]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[15]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_H2O_LOOP_VC );
		pCWLight[15]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[16]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_RGA_ACCEL_VC );
		pCWLight[16]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[17]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_AIR_DATA_VC );
		pCWLight[17]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[18]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_LEFT_RCS_VC );
		pCWLight[18]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[19]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_RIGHT_RCS_VC );
		pCWLight[19]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[20]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_LEFT_RHC_VC );
		pCWLight[20]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[21]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_RIGHT_AFT_RHC_VC );
		pCWLight[21]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[22]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_LEFT_OMS_VC );
		pCWLight[22]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[23]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_RIGHT_OMS_VC );
		pCWLight[23]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[24]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_PAYLOAD_WARNING_VC );
		pCWLight[24]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[25]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_GPC_VC );
		pCWLight[25]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[26]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_FCS_SATURATION_VC );
		pCWLight[26]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[27]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_OMS_KIT_VC );
		pCWLight[27]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[28]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_OMS_TVC_VC );
		pCWLight[28]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[29]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_PAYLOAD_CAUTION_VC );
		pCWLight[29]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[30]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_PRIMARY_CW_VC );
		pCWLight[30]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[31]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_FCS_CHANNEL_VC );
		pCWLight[31]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[32]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_MPS_VC );
		pCWLight[32]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[33]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_BACKUP_CW_ALARM_VC );
		pCWLight[33]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[34]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_APU_TEMP_VC );
		pCWLight[34]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[35]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_APU_OVERSPEED_VC );
		pCWLight[35]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[36]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_APU_UNDERSPEED_VC );
		pCWLight[36]->SetStateOffset( 1, 0.0f, 0.177246f );

		pCWLight[37]->DefineMeshGroup( STS()->mesh_vc, GRP_CW_HYD_PRESS_VC );
		pCWLight[37]->SetStateOffset( 1, 0.0f, 0.177246f );

		pMainEngStatus[0]->DefineMeshGroup( STS()->mesh_vc, GRP_F7_XDS1_VC );
		pMainEngStatus[0]->SetStateOffset( 1, 0.120117f, 0.344727f );// R
		pMainEngStatus[0]->SetStateOffset( 2, 0.120117f, 0.478516f );// Y
		pMainEngStatus[0]->SetStateOffset( 3, 0.120117f, 0.619141f );// RY
		
		pMainEngStatus[1]->DefineMeshGroup( STS()->mesh_vc, GRP_F7_XDS2_VC );
		pMainEngStatus[1]->SetStateOffset( 1, 0.120117f, 0.344727f );// R
		pMainEngStatus[1]->SetStateOffset( 2, 0.120117f, 0.478516f );// Y
		pMainEngStatus[1]->SetStateOffset( 3, 0.120117f, 0.619141f );// RY
		
		pMainEngStatus[2]->DefineMeshGroup( STS()->mesh_vc, GRP_F7_XDS3_VC );
		pMainEngStatus[2]->SetStateOffset( 1, 0.120117f, 0.344727f );// R
		pMainEngStatus[2]->SetStateOffset( 2, 0.120117f, 0.478516f );// Y
		pMainEngStatus[2]->SetStateOffset( 3, 0.120117f, 0.619141f );// RY

		pSMAlert->DefineMeshGroup( STS()->mesh_vc, GRP_F7_XDS4_VC );
		pSMAlert->SetStateOffset( 1, 0.120117f, 0.619141f );
		return;
	}

	void PanelF7::RegisterVC()
	{
		AtlantisPanel::RegisterVC();
		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;
		oapiVCRegisterArea(AID_F7, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
		
		oapiVCSetAreaClickmode_Quadrilateral (AID_F7, 
			_V(- 0.389, 2.4112365, 14.790174)+ ofs, 
			_V(0.385, 2.4112365, 14.791174) + ofs,
			_V(- 0.389, 1.9662365, 14.664174)+ ofs, 
			_V(0.385, 1.9662365, 14.664174) + ofs);

		return;
	}

	void PanelF7::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "CW_ANNUNCIATOR_A", 16 );
		for (int i = 0; i < 16; i++) lightcmd[i].Connect( pBundle, i );

		pBundle = STS()->BundleManager()->CreateBundle( "CW_ANNUNCIATOR_B", 16 );
		for (int i = 16; i < 32; i++) lightcmd[i].Connect( pBundle, i - 16 );

		pBundle = STS()->BundleManager()->CreateBundle( "CW_ANNUNCIATOR_C", 16 );
		for (int i = 32; i < 38; i++) lightcmd[i].Connect( pBundle, i - 32 );

		pBundle = STS()->BundleManager()->CreateBundle( "F7LIGHTS_A", 16 );
		for (int i = 0; i < 16; i++)
		{
			pCWLight[i]->ConnectLight( 0, pBundle, i );
			lightoutput[i].Connect( pBundle, i );
		}

		pBundle = STS()->BundleManager()->CreateBundle( "F7LIGHTS_B", 16 );
		for (int i = 16; i < 32; i++)
		{
			pCWLight[i]->ConnectLight( 0, pBundle, i - 16 );
			lightoutput[i].Connect( pBundle, i - 16 );
		}

		pBundle = STS()->BundleManager()->CreateBundle( "F7LIGHTS_C", 16 );
		for (int i = 32; i < 38; i++)
		{
			pCWLight[i]->ConnectLight( 0, pBundle, i - 32 );
			lightoutput[i].Connect( pBundle, i - 32 );
		}

		pBundle = STS()->BundleManager()->CreateBundle( "ACA", 16 );
		lighttestleft[0].Connect( pBundle, 6 );
		lighttestright[0].Connect( pBundle, 7 );
		lighttestleft[1].Connect( pBundle, 8 );
		lighttestright[1].Connect( pBundle, 9 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA1_1", 16 );
		pMainEngStatus[1]->ConnectLight( 0, pBundle, 15 );
		pBundle = STS()->BundleManager()->CreateBundle( "ACA1_2", 16 );
		pMainEngStatus[1]->ConnectLight( 2, pBundle, 15 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA2_4", 16 );
		pSMAlert->ConnectLight( 0, pBundle, 9 );
		pMainEngStatus[2]->ConnectLight( 0, pBundle, 15 );
		pBundle = STS()->BundleManager()->CreateBundle( "ACA2_5", 16 );
		pMainEngStatus[2]->ConnectLight( 2, pBundle, 11 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA3_1", 16 );
		pMainEngStatus[0]->ConnectLight( 0, pBundle, 15 );
		pBundle = STS()->BundleManager()->CreateBundle( "ACA3_2", 16 );
		pMainEngStatus[0]->ConnectLight( 2, pBundle, 15 );
		
		pBundle = STS()->BundleManager()->CreateBundle( "ACA3_4", 16 );
		pSMAlert->ConnectLight( 1, pBundle, 9 );

		AtlantisPanel::Realize();
	}

	void PanelF7::OnPostStep( double SimT, double DeltaT, double MJD )
	{
		AtlantisPanel::OnPostStep( SimT, DeltaT, MJD );

		if (lighttestleft[0].IsSet() || lighttestleft[1].IsSet())
		{
			lightoutput[0].SetLine();
			lightoutput[1].SetLine();
			lightoutput[2].SetLine();
			lightoutput[5].SetLine();
			lightoutput[6].SetLine();
			lightoutput[7].SetLine();
			lightoutput[10].SetLine();
			lightoutput[11].SetLine();
			lightoutput[12].SetLine();
			lightoutput[15].SetLine();
			lightoutput[16].SetLine();
			lightoutput[17].SetLine();
			lightoutput[20].SetLine();
			lightoutput[24].SetLine();
			lightoutput[25].SetLine();
			lightoutput[29].SetLine();
			lightoutput[30].SetLine();
			lightoutput[33].SetLine();
			lightoutput[34].SetLine();
		}
		else
		{
			lightoutput[0].SetLine( (int)lightcmd[0].IsSet() * 5.0f );
			lightoutput[1].SetLine( (int)lightcmd[1].IsSet() * 5.0f );
			lightoutput[2].SetLine( (int)lightcmd[2].IsSet() * 5.0f );
			lightoutput[5].SetLine( (int)lightcmd[5].IsSet() * 5.0f );
			lightoutput[6].SetLine( (int)lightcmd[6].IsSet() * 5.0f );
			lightoutput[7].SetLine( (int)lightcmd[7].IsSet() * 5.0f );
			lightoutput[10].SetLine( (int)lightcmd[10].IsSet() * 5.0f );
			lightoutput[11].SetLine( (int)lightcmd[11].IsSet() * 5.0f );
			lightoutput[12].SetLine( (int)lightcmd[12].IsSet() * 5.0f );
			lightoutput[15].SetLine( (int)lightcmd[15].IsSet() * 5.0f );
			lightoutput[16].SetLine( (int)lightcmd[16].IsSet() * 5.0f );
			lightoutput[17].SetLine( (int)lightcmd[17].IsSet() * 5.0f );
			lightoutput[20].SetLine( (int)lightcmd[20].IsSet() * 5.0f );
			lightoutput[24].SetLine( (int)lightcmd[24].IsSet() * 5.0f );
			lightoutput[25].SetLine( (int)lightcmd[25].IsSet() * 5.0f );
			lightoutput[29].SetLine( (int)lightcmd[29].IsSet() * 5.0f );
			lightoutput[30].SetLine( (int)lightcmd[30].IsSet() * 5.0f );
			lightoutput[33].SetLine( (int)lightcmd[33].IsSet() * 5.0f );
			lightoutput[34].SetLine( (int)lightcmd[34].IsSet() * 5.0f );
		}
		
		if (lighttestright[0].IsSet() || lighttestright[1].IsSet())
		{
			lightoutput[3].SetLine();
			lightoutput[4].SetLine();
			lightoutput[8].SetLine();
			lightoutput[9].SetLine();
			lightoutput[13].SetLine();
			lightoutput[14].SetLine();
			lightoutput[18].SetLine();
			lightoutput[19].SetLine();
			lightoutput[21].SetLine();
			lightoutput[22].SetLine();
			lightoutput[23].SetLine();
			lightoutput[26].SetLine();
			lightoutput[27].SetLine();
			lightoutput[28].SetLine();
			lightoutput[31].SetLine();
			lightoutput[32].SetLine();
			lightoutput[35].SetLine();
			lightoutput[36].SetLine();
			lightoutput[37].SetLine();
		}
		else
		{
			lightoutput[3].SetLine( (int)lightcmd[3].IsSet() * 5.0f );
			lightoutput[4].SetLine( (int)lightcmd[4].IsSet() * 5.0f );
			lightoutput[8].SetLine( (int)lightcmd[8].IsSet() * 5.0f );
			lightoutput[9].SetLine( (int)lightcmd[9].IsSet() * 5.0f );
			lightoutput[13].SetLine( (int)lightcmd[13].IsSet() * 5.0f );
			lightoutput[14].SetLine( (int)lightcmd[14].IsSet() * 5.0f );
			lightoutput[18].SetLine( (int)lightcmd[18].IsSet() * 5.0f );
			lightoutput[19].SetLine( (int)lightcmd[19].IsSet() * 5.0f );
			lightoutput[21].SetLine( (int)lightcmd[21].IsSet() * 5.0f );
			lightoutput[22].SetLine( (int)lightcmd[22].IsSet() * 5.0f );
			lightoutput[23].SetLine( (int)lightcmd[23].IsSet() * 5.0f );
			lightoutput[26].SetLine( (int)lightcmd[26].IsSet() * 5.0f );
			lightoutput[27].SetLine( (int)lightcmd[27].IsSet() * 5.0f );
			lightoutput[28].SetLine( (int)lightcmd[28].IsSet() * 5.0f );
			lightoutput[31].SetLine( (int)lightcmd[31].IsSet() * 5.0f );
			lightoutput[32].SetLine( (int)lightcmd[32].IsSet() * 5.0f );
			lightoutput[35].SetLine( (int)lightcmd[33].IsSet() * 5.0f );
			lightoutput[36].SetLine( (int)lightcmd[36].IsSet() * 5.0f );
			lightoutput[37].SetLine( (int)lightcmd[37].IsSet() * 5.0f );
		}
		return;
	}
};
