#include "PanelO17.h"
#include "../Atlantis.h"
#include "../Atlantis_defs.h"

namespace vc
{

PanelO17::PanelO17(Atlantis* psts)
: AtlantisPanel(psts, "O17")
{
	Add(pEIUPowerLC = new StdSwitch2(psts, "EIU L-C"));
	Add(pEIUPowerCR = new StdSwitch2(psts, "EIU C-R"));
	Add(pEIUPowerRL = new StdSwitch2(psts, "EIU R-L"));

	pEIUPowerLC->SetLabel(1, "OFF");
	pEIUPowerLC->SetLabel(0, "ON");
	pEIUPowerLC->SetOnPosition(0);
	pEIUPowerCR->SetLabel(1, "OFF");
	pEIUPowerCR->SetLabel(0, "ON");
	pEIUPowerCR->SetOnPosition(0);
	pEIUPowerRL->SetLabel(1, "OFF");
	pEIUPowerRL->SetLabel(0, "ON");
	pEIUPowerRL->SetOnPosition(0);

	Add( pATVC1 = new StdSwitch2( psts, "ATVC 1" ) );
	pATVC1->SetLabel( 0, "ON" );
	pATVC1->SetLabel( 1, "OFF" );

	Add( pATVC2 = new StdSwitch2( psts, "ATVC 2" ) );
	pATVC2->SetLabel( 0, "ON" );
	pATVC2->SetLabel( 1, "OFF" );

	Add( pATVC3 = new StdSwitch2( psts, "ATVC 3" ) );
	pATVC3->SetLabel( 0, "ON" );
	pATVC3->SetLabel( 1, "OFF" );
	
	Add( pATVC4 = new StdSwitch2( psts, "ATVC 4" ) );
	pATVC4->SetLabel( 0, "ON" );
	pATVC4->SetLabel( 1, "OFF" );
}

PanelO17::~PanelO17()
{
}

void PanelO17::DefineVC()
{
	const VECTOR3 SWITCH_ROT = _V(-0.788502306017, 0.615031798693, 0.0);
	AddAIDToMouseEventList(AID_O17);

	pEIUPowerLC->SetInitialAnimState(0.5f);
	pEIUPowerLC->DefineSwitchGroup(GRP_O17S8_VC);
	pEIUPowerLC->SetReference(_V(0.7605, 3.1545, 13.336), SWITCH_ROT);
	pEIUPowerLC->SetMouseRegion(0.073f, 0.625f, 0.159f, 0.7f);

	pEIUPowerCR->SetInitialAnimState(0.5f);
	pEIUPowerCR->DefineSwitchGroup(GRP_O17S7_VC);
	pEIUPowerCR->SetReference(_V(0.78925, 3.1325, 13.336), SWITCH_ROT);
	pEIUPowerCR->SetMouseRegion(0.171f, 0.625f, 0.265f, 0.7f);

	pEIUPowerRL->SetInitialAnimState(0.5f);
	pEIUPowerRL->DefineSwitchGroup(GRP_O17S9_VC);
	pEIUPowerRL->SetReference(_V(0.81725, 3.1105, 13.3365), SWITCH_ROT);
	pEIUPowerRL->SetMouseRegion(0.270f, 0.625f, 0.362f, 0.7f);

	pATVC1->SetInitialAnimState( 0.5f );
	pATVC1->DefineSwitchGroup( GRP_O17S1_VC );
	pATVC1->SetReference( _V( 0.7567, 3.1587, 13.2782 ), SWITCH_ROT );
	pATVC1->SetMouseRegion( 0.103174f, 0.790438f, 0.166405f, 0.843446f );

	pATVC2->SetInitialAnimState( 0.5f );
	pATVC2->DefineSwitchGroup( GRP_O17S2_VC );
	pATVC2->SetReference( _V( 0.7567, 3.1587, 13.2782 ), SWITCH_ROT );
	pATVC2->SetMouseRegion( 0.197668f, 0.790624f, 0.259918f, 0.845494f );

	pATVC3->SetInitialAnimState( 0.5f );
	pATVC3->DefineSwitchGroup( GRP_O17S3_VC );
	pATVC3->SetReference( _V( 0.7567, 3.1587, 13.2782 ), SWITCH_ROT );
	pATVC3->SetMouseRegion( 0.297184f, 0.790073f, 0.359735f, 0.842862f );

	pATVC4->SetInitialAnimState( 0.5f );
	pATVC4->DefineSwitchGroup( GRP_O17S4_VC );
	pATVC4->SetReference( _V( 0.7567, 3.1587, 13.2782 ), SWITCH_ROT );
	pATVC4->SetMouseRegion( 0.392647f, 0.789922f, 0.453516f, 0.844299f );
}

void PanelO17::RegisterVC()
{
	AtlantisPanel::RegisterVC();
	VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

	oapiVCRegisterArea(AID_O17, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP);
	oapiVCSetAreaClickmode_Quadrilateral(AID_O17, 
										 _V(0.721, 3.186, 13.609)+ofs, _V(1.021, 2.949, 13.609)+ofs, 
										 _V(0.721, 3.186, 13.205) + ofs, _V(1.021, 2.952, 13.242)+ofs);
}

void PanelO17::Realize()
{
	DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "O17_to_EIU_AC", 3 );

	pEIUPowerLC->output.Connect( pBundle, 0 );// AC2
	pEIUPowerCR->output.Connect( pBundle, 1 );// AC1
	pEIUPowerRL->output.Connect( pBundle, 2 );// AC3

	pBundle = STS()->BundleManager()->CreateBundle( "O17_to_ATVC", 3 );
	pATVC1->output.Connect( pBundle, 0 );
	pATVC2->output.Connect( pBundle, 1 );
	pATVC3->output.Connect( pBundle, 2 );
	pATVC4->output.Connect( pBundle, 3 );

	AtlantisPanel::Realize();
}

};
