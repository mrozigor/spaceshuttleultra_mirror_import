#include "PanelO17.h"
#include "../Atlantis.h"
#include "../Atlantis_defs.h"
#include "..\CommonDefs.h"
#include "..\meshres_vc_o17.h"


namespace vc
{

	PanelO17::PanelO17(Atlantis* psts):AtlantisPanel(psts, "O17")
	{
		hPanelMesh = oapiLoadMeshGlobal( DEFAULT_MESHNAME_PANELO17 );
		mesh_index = MESH_UNDEFINED;

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

		Add( pMECPower[0] = new StdSwitch2( psts, "MEC 1 Power" ) );
		pMECPower[0]->SetLabel( 1, "OFF" );
		pMECPower[0]->SetLabel( 0, "ON" );
		pMECPower[0]->SetOnPosition( 0 );
		Add( pMECPower[1] = new StdSwitch2( psts, "MEC 2 Power" ) );
		pMECPower[1]->SetLabel( 1, "OFF" );
		pMECPower[1]->SetLabel( 0, "ON" );
		pMECPower[1]->SetOnPosition( 0 );
	}

	PanelO17::~PanelO17()
	{
	}

	void PanelO17::AddMeshes( const VECTOR3 &ofs )
	{
		SetHasOwnVCMesh();

		if (mesh_index == MESH_UNDEFINED)
		{
			mesh_index = STS()->AddMesh( hPanelMesh, &ofs );
			STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		}
		return;
	}

	void PanelO17::SetMeshVisibility( bool visible )
	{
		if (visible) STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		else STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_NEVER );
		return;
	}

	UINT PanelO17::GetVCMeshIndex( void ) const
	{
		return mesh_index;
	}

	void PanelO17::DefineVC()
	{
		const VECTOR3 SWITCH_ROT = _V(-0.788502306017, 0.615031798693, 0.0);
		AddAIDToMouseEventList(AID_O17);

		pEIUPowerLC->SetInitialAnimState(0.5f);
		pEIUPowerLC->DefineSwitchGroup(GRP_S8_O17_VC);
		pEIUPowerLC->SetReference(_V(0.7605, 3.1545, 13.336), SWITCH_ROT);
		pEIUPowerLC->SetMouseRegion(0.073f, 0.625f, 0.159f, 0.7f);

		pEIUPowerCR->SetInitialAnimState(0.5f);
		pEIUPowerCR->DefineSwitchGroup(GRP_S7_O17_VC);
		pEIUPowerCR->SetReference(_V(0.78925, 3.1325, 13.336), SWITCH_ROT);
		pEIUPowerCR->SetMouseRegion(0.171f, 0.625f, 0.265f, 0.7f);

		pEIUPowerRL->SetInitialAnimState(0.5f);
		pEIUPowerRL->DefineSwitchGroup(GRP_S9_O17_VC);
		pEIUPowerRL->SetReference(_V(0.81725, 3.1105, 13.3365), SWITCH_ROT);
		pEIUPowerRL->SetMouseRegion(0.270f, 0.625f, 0.362f, 0.7f);

		pMECPower[0]->SetInitialAnimState( 0.5f );
		pMECPower[0]->DefineSwitchGroup( GRP_S5_O17_VC );
		pMECPower[0]->SetReference( _V( 0.8507, 3.0852, 13.4520 ), SWITCH_ROT );
		pMECPower[0]->SetMouseRegion( 0.296503f, 0.365015f, 0.346141f, 0.413497f );

		pMECPower[1]->SetInitialAnimState( 0.5f );
		pMECPower[1]->DefineSwitchGroup( GRP_S6_O17_VC );
		pMECPower[1]->SetReference( _V( 0.8507, 3.0852, 13.4520 ), SWITCH_ROT );
		pMECPower[1]->SetMouseRegion( 0.390501f, 0.365165f, 0.439871f, 0.412690f );
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

	void PanelO17::DefineVCAnimations( UINT vcidx )
	{
		AtlantisPanel::DefineVCAnimations( mesh_index );
		return;
	}

	void PanelO17::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "O17_to_EIU_AC", 3 );
		pEIUPowerLC->output.Connect( pBundle, 0 );// AC2
		pEIUPowerCR->output.Connect( pBundle, 1 );// AC1
		pEIUPowerRL->output.Connect( pBundle, 2 );// AC3

		pBundle = STS()->BundleManager()->CreateBundle( "O17_MEC", 2 );
		pMECPower[0]->output.Connect( pBundle, 0 );
		pMECPower[1]->output.Connect( pBundle, 1 );
		AtlantisPanel::Realize();
	}
};
