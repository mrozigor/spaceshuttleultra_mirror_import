#include "PanelF6.h"
#include "../Atlantis.h"
#include "../meshres_vc.h"
#include "MDU.h"
#include "../Atlantis_defs.h"

namespace vc {

	PanelF6::PanelF6(Atlantis* _sts)
		: AtlantisPanel(_sts, "F6")
	{
		Add(pCDR1 = new MDU(_sts, "CDR1", MDUID_CDR1, true));
		Add(pCDR2 = new MDU(_sts, "CDR2", MDUID_CDR2, true));
		Add(pFltCntlrPower = new StdSwitch2(_sts, "Cdr Flt Cntlr Pwr"));
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

	PanelF6::~PanelF6()
	{
	}

	void PanelF6::DefineVC()
	{
		VECTOR3 switch_rot = _V(1, 0, 0);

		AddAIDToMouseEventList(AID_F6);

		pCDR1->SetMouseRegion(0.0f, 0.0f, 0.487004f, 0.615168f);
		pCDR2->SetMouseRegion(0.508892f, 0.0f, 0.993170f, 0.613257f);
		
		pCDR1->DefineVCGroup(MFDGROUPS[MDUID_CDR1]);
		pCDR2->DefineVCGroup(MFDGROUPS[MDUID_CDR2]);

		pFltCntlrPower->DefineSwitchGroup(GRP_F6SWITCH_5_VC);
		pFltCntlrPower->SetInitialAnimState(0.5);
		pFltCntlrPower->SetReference(_V(-0.734, 1.99, 14.685), switch_rot);
		pFltCntlrPower->SetMouseRegion(0.372206f, 0.851179f, 0.428169f, 0.928952f);

		pADIAttitude->DefineSwitchGroup( GRP_F6SWITCH_6_VC );
		pADIAttitude->SetInitialAnimState( 0.5 );
		pADIAttitude->SetReference( _V( 0.633, 2.045, 14.7004 ), switch_rot );
		pADIAttitude->SetMouseRegion( 0.571952f, 0.695254f, 0.614206f, 0.759335f );

		pADIError->DefineSwitchGroup( GRP_F6SWITCH_7_VC );
		pADIError->SetInitialAnimState( 0.5 );
		pADIError->SetReference( _V( 0.5921, 2.0469, 14.7009 ), switch_rot );
		pADIError->SetMouseRegion( 0.649129f, 0.690659f, 0.693501f, 0.759623f );

		pADIRate->DefineSwitchGroup( GRP_F6SWITCH_8_VC );
		pADIRate->SetInitialAnimState( 0.5 );
		pADIRate->SetReference( _V( 0.5627, 2.0463, 14.7008 ), switch_rot );
		pADIRate->SetMouseRegion( 0.704403f, 0.693751f, 0.747005f, 0.759008f );
	}

	void PanelF6::RegisterVC()
	{
		AtlantisPanel::RegisterVC();
		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;
		
		oapiVCRegisterArea(AID_F6, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
		oapiVCSetAreaClickmode_Quadrilateral (AID_F6, 
			_V(-0.940, 2.312, 14.756)+ofs, _V(-0.413, 2.312, 14.755)+ofs,
			_V(-0.947, 1.951, 14.674)+ofs, _V(-0.413, 1.949, 14.673)+ofs);
	}

	void PanelF6::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle("Controllers", 16);
		pFltCntlrPower->ConnectPort(1, pBundle, 1);

		pBundle = STS()->BundleManager()->CreateBundle( "ADI_Switches_F6_F8", 12 );
		pADIAttitude->outputA.Connect( pBundle, 0 );// REF
		pADIAttitude->outputB.Connect( pBundle, 1 );// INRTL
		pADIError->outputA.Connect( pBundle, 2 );// LOW
		pADIError->outputB.Connect( pBundle, 3 );// HIGH
		pADIRate->outputA.Connect( pBundle, 4 );// LOW
		pADIRate->outputB.Connect( pBundle, 5 );// HIGH

		AtlantisPanel::Realize();
	}
		
};
