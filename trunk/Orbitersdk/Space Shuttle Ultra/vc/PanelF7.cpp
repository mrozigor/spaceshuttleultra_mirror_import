#include "PanelF7.h"
#include "../Atlantis.h"
#include "../meshres_vc.h"
#include "../meshres_vc_additions.h"
#include "MDU.h"
#include "../Atlantis_defs.h"

extern GDIParams g_Param;

namespace vc {

	PanelF7::PanelF7(Atlantis* _sts)
		: AtlantisPanel(_sts, "F7")
	{
		Add(pCRT1 = new MDU(_sts, "CRT1", MDUID_CRT1, true));
		Add(pCRT2 = new MDU(_sts, "CRT2", MDUID_CRT2, true));
		Add(pCRT3 = new MDU(_sts, "CRT3", MDUID_CRT3, true));
		Add(pMFD1 = new MDU(_sts, "MFD1", MDUID_MFD1, true));
		Add(pMFD2 = new MDU(_sts, "MFD2", MDUID_MFD2, true));

		Add(pMainEngStatusR[0] = new StandardLight(_sts, "SSME 1 STATUS RED"));
		Add(pMainEngStatusR[1] = new StandardLight(_sts, "SSME 2 STATUS RED"));
		Add(pMainEngStatusR[2] = new StandardLight(_sts, "SSME 3 STATUS RED"));
		Add(pMainEngStatusY[0] = new StandardLight(_sts, "SSME 1 STATUS YELLOW"));
		Add(pMainEngStatusY[1] = new StandardLight(_sts, "SSME 2 STATUS YELLOW"));
		Add(pMainEngStatusY[2] = new StandardLight(_sts, "SSME 3 STATUS YELLOW"));

		Add( pEventTime = new _7SegDisp_EventTime( _sts, "Event Time" ) );
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

		pCRT1->DefineVCGroup(MFDGROUPS[MDUID_CRT1]);
		pCRT2->DefineVCGroup(MFDGROUPS[MDUID_CRT2]);
		pCRT3->DefineVCGroup(MFDGROUPS[MDUID_CRT3]);
		pMFD1->DefineVCGroup(MFDGROUPS[MDUID_MFD1]);
		pMFD2->DefineVCGroup(MFDGROUPS[MDUID_MFD2]);

		for(int i=0;i<3;i++) {
			pMainEngStatusR[i]->AddAIDToRedrawEventList(AID_F7_SSME_C_R+2*i);
			pMainEngStatusR[i]->SetSourceImage(g_Param.ssme_lights);
			pMainEngStatusR[i]->SetDimensions(65, 30);
			pMainEngStatusR[i]->SetBase(0, 0);
			pMainEngStatusR[i]->SetSourceCoords(true, 130*i, 0);
			pMainEngStatusR[i]->SetSourceCoords(false, 390, 0);

			pMainEngStatusY[i]->AddAIDToRedrawEventList(AID_F7_SSME_C_Y+2*i);
			pMainEngStatusY[i]->SetSourceImage(g_Param.ssme_lights);
			pMainEngStatusY[i]->SetDimensions(65, 30);
			pMainEngStatusY[i]->SetBase(0, 0);
			pMainEngStatusY[i]->SetSourceCoords(true, 65+130*i, 0);
			pMainEngStatusY[i]->SetSourceCoords(false, 390, 0);
		}

		pEventTime->DefineMesh( STS()->mesh_vc );
		pEventTime->DefineComponent( GRP_F7_EVENT_S_VC, true, false, false, _7SD_STATE_NUM0 );
		pEventTime->DefineComponent( GRP_F7_EVENT_S10_VC, true, false, false, _7SD_STATE_NUM0 );
		pEventTime->DefineComponent( GRP_F7_EVENT_M_VC, true, false, false, _7SD_STATE_NUM0 );
		pEventTime->DefineComponent( GRP_F7_EVENT_M10_VC, true, false, false, _7SD_STATE_NUM0 );
		pEventTime->SetLocation( true );
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

		SURFHANDLE SSME_light_tex = oapiGetTextureHandle(STS()->hOrbiterVCMesh, TEX_TIMER_VC);
		oapiVCRegisterArea(AID_F7_SSME_L_R, _R(498, 165, 563, 195), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, SSME_light_tex);
		oapiVCRegisterArea(AID_F7_SSME_L_Y, _R(498, 195, 563, 225), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, SSME_light_tex);
		oapiVCRegisterArea(AID_F7_SSME_C_R, _R(629, 117, 694, 147), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, SSME_light_tex);
		oapiVCRegisterArea(AID_F7_SSME_C_Y, _R(629, 147, 694, 177), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, SSME_light_tex);
		oapiVCRegisterArea(AID_F7_SSME_R_R, _R(764, 166, 829, 196), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, SSME_light_tex);
		oapiVCRegisterArea(AID_F7_SSME_R_Y, _R(764, 196, 829, 226), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, SSME_light_tex);
	}

	void PanelF7::Realize()
	{
		DiscreteBundle* pBundle=STS()->BundleManager()->CreateBundle("MPS_STATUS_LIGHTS", 6);
		for(int i=0;i<3;i++) {
			pMainEngStatusR[i]->Connect(0, pBundle, i);
			pMainEngStatusY[i]->Connect(0, pBundle, i+3);
		}

		AtlantisPanel::Realize();
	}
};
