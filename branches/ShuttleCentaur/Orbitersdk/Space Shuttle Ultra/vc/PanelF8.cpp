#include "PanelF8.h"
#include "../Atlantis.h"
#include "../meshres_vc.h"
#include "MDU.h"
#include "../Atlantis_defs.h"

namespace vc {

	PanelF8::PanelF8(Atlantis* _sts)
		: AtlantisPanel(_sts, "F8")
	{
		Add(pPLT1 = new MDU(_sts, "PLT1", MDUID_PLT1, true));
		Add(pPLT2 = new MDU(_sts, "PLT2", MDUID_PLT2, true));
		Add(pFltCntlrPower = new StdSwitch2(_sts, "Plt Flt Cntlr Pwr"));
		Add( pADIAttitude = new StdSwitch3( _sts, "ADI Attitude" ) );
		Add( pADIError = new StdSwitch3( _sts, "ADI Error" ) );
		Add( pADIRate = new StdSwitch3( _sts, "ADI Rate" ) );

		pFltCntlrPower->SetLabel(0, "OFF");
		pFltCntlrPower->SetLabel(1, "ON");
		pADIAttitude->SetLabel( 0, "REF" );
		pADIAttitude->SetLabel( 1, "LVLH" );
		pADIAttitude->SetLabel( 2, "INRTL" );
		pADIError->SetLabel( 0, "LOW" );
		pADIError->SetLabel( 1, "MED" );
		pADIError->SetLabel( 2, "HIGH" );
		pADIRate->SetLabel( 0, "LOW" );
		pADIRate->SetLabel( 1, "MED" );
		pADIRate->SetLabel( 2, "HIGH" );
	}

	PanelF8::~PanelF8()
	{
	}

	void PanelF8::DefineVC()
	{
		VECTOR3 switch_rot = _V(1, 0, 0);

		AddAIDToMouseEventList(AID_F8);

		pPLT1->SetMouseRegion(0.0f, 0.0f, 0.491582f, 0.609860f);
		pPLT1->SetEdgekeyArea(0.210163f, 0.915644f, 0.783473f, 0.973985f);
		pPLT2->SetMouseRegion(0.511129f, 0.0f, 1.0f, 0.611175f);
		pPLT2->SetEdgekeyArea(0.210163f, 0.915644f, 0.783473f, 0.973985f);
		
		pPLT1->DefineVCGroup(MFDGROUPS[MDUID_PLT1]);
		pPLT2->DefineVCGroup(MFDGROUPS[MDUID_PLT2]);

		pFltCntlrPower->DefineSwitchGroup(GRP_F8SWITCH5_VC);
		pFltCntlrPower->SetInitialAnimState(0.5);
		pFltCntlrPower->SetReference(_V(-0.734, 1.99, 14.685), switch_rot);
		pFltCntlrPower->SetMouseRegion(0.372206f, 0.851179f, 0.428169f, 0.928952f);

		pADIAttitude->DefineSwitchGroup( GRP_F8SWITCH6_VC );
		pADIAttitude->SetInitialAnimState( 0.5 );
		pADIAttitude->SetReference( _V( -0.633, 2.045, 14.7004 ), switch_rot );
		pADIAttitude->SetMouseRegion( 0.576194f, 0.689506f, 0.618179f, 0.755844f );

		pADIError->DefineSwitchGroup( GRP_F8SWITCH7_VC );
		pADIError->SetInitialAnimState( 0.5 );
		pADIError->SetReference( _V( -0.5921, 2.0469, 14.7009 ), switch_rot );
		pADIError->SetMouseRegion( 0.655636f, 0.686714f, 0.698151f, 0.754200f );

		pADIRate->DefineSwitchGroup( GRP_F8SWITCH8_VC );
		pADIRate->SetInitialAnimState( 0.5 );
		pADIRate->SetReference( _V( -0.5627, 2.0463, 14.7008 ), switch_rot );
		pADIRate->SetMouseRegion( 0.710745f, 0.687697f, 0.753016f, 0.750151f );
	}

	void PanelF8::RegisterVC()
	{
		AtlantisPanel::RegisterVC();
		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;
		oapiVCRegisterArea(AID_F8, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
		oapiVCSetAreaClickmode_Quadrilateral (AID_F8, 
			_V(0.416, 2.312, 14.755)+ofs, _V(0.942, 2.312, 14.755)+ofs,
			_V(0.416, 1.949, 14.673)+ofs, _V(0.942, 1.949, 14.673) + ofs);
	}

	void PanelF8::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle("Controllers", 16);
		pFltCntlrPower->ConnectPort(1, pBundle, 2);

		pBundle = STS()->BundleManager()->CreateBundle( "ADI_Switches_F6_F8", 12 );
		pADIAttitude->outputA.Connect( pBundle, 6 );// REF
		pADIAttitude->outputB.Connect( pBundle, 7 );// INRTL
		pADIError->outputA.Connect( pBundle, 8 );// LOW
		pADIError->outputB.Connect( pBundle, 9 );// HIGH
		pADIRate->outputA.Connect( pBundle, 10 );// LOW
		pADIRate->outputB.Connect( pBundle, 11 );// HIGH
		
		AtlantisPanel::Realize();
	}
		
};
