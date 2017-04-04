#include "PanelL1.h"
#include "../Atlantis.h"
#include "../Atlantis_defs.h"
#include "..\CommonDefs.h"
#include "..\meshres_vc_l1.h"


namespace vc
{
	PanelL1::PanelL1( Atlantis *_sts ):AtlantisPanel( _sts, "L1" )
	{
		hPanelMesh = oapiLoadMeshGlobal( DEFAULT_MESHNAME_PANELL1 );
		mesh_index = MESH_UNDEFINED;

		Add( pFireSuppressionAvBayDischAgentCover[0] = new StandardSwitchCover( _sts, "Fire Suppression Av Bay 1 Disch Agent Cover" ) );
		Add( pFireSuppressionAvBayDischAgentCover[1] = new StandardSwitchCover( _sts, "Fire Suppression Av Bay 2 Disch Agent Cover" ) );
		Add( pFireSuppressionAvBayDischAgentCover[2] = new StandardSwitchCover( _sts, "Fire Suppression Av Bay 3 Disch Agent Cover" ) );

		Add( pFireSuppressionAvBayDischAgent[0] = new PushButtonIndicatorSingleLight( _sts, "Fire Suppression Av Bay 1 Disch Agent" ) );
		Add( pFireSuppressionAvBayDischAgent[1] = new PushButtonIndicatorSingleLight( _sts, "Fire Suppression Av Bay 2 Disch Agent" ) );
		Add( pFireSuppressionAvBayDischAgent[2] = new PushButtonIndicatorSingleLight( _sts, "Fire Suppression Av Bay 3 Disch Agent" ) );

		Add( pSmokeDetectionA_Cabin = new StandardSingleLight( _sts, "Smoke Detection A Cabin" ) );
		Add( pSmokeDetectionA_LFltDeck = new StandardSingleLight( _sts, "Smoke Detection A L Flt Deck" ) );
		Add( pSmokeDetectionA_AvBay1 = new StandardSingleLight( _sts, "Smoke Detection A Av Bay 1" ) );
		Add( pSmokeDetectionA_AvBay2 = new StandardSingleLight( _sts, "Smoke Detection A Av Bay 2" ) );
		Add( pSmokeDetectionA_AvBay3 = new StandardSingleLight( _sts, "Smoke Detection A Av Bay 3" ) );

		Add( pSmokeDetectionB_Payload = new StandardSingleLight( _sts, "Smoke Detection B Payload" ) );
		Add( pSmokeDetectionB_RFltDeck = new StandardSingleLight( _sts, "Smoke Detection B R Flt Deck" ) );
		Add( pSmokeDetectionB_AvBay1 = new StandardSingleLight( _sts, "Smoke Detection B Av Bay 1" ) );
		Add( pSmokeDetectionB_AvBay2 = new StandardSingleLight( _sts, "Smoke Detection B Av Bay 2" ) );
		Add( pSmokeDetectionB_AvBay3 = new StandardSingleLight( _sts, "Smoke Detection B Av Bay 3" ) );
	}

	PanelL1::~PanelL1()
	{
	}

	void PanelL1::AddMeshes( const VECTOR3 &ofs )
	{
		SetHasOwnVCMesh();

		if (mesh_index == MESH_UNDEFINED)
		{
			mesh_index = STS()->AddMesh( hPanelMesh, &ofs );
			STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		}
		return;
	}

	void PanelL1::SetMeshVisibility( bool visible )
	{
		if (visible) STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		else STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_NEVER );
		return;
	}

	UINT PanelL1::GetVCMeshIndex( void ) const
	{
		return mesh_index;
	}

	void PanelL1::DefineVC( void )
	{
		VECTOR3 switch_rot = _V( 0.338809, 0.0217555, 0.940604 );
		VECTOR3 push_dir = _V( -0.916229, -0.227543, 0.329772 );

		AddAIDToMouseEventList( AID_L1 );

		pFireSuppressionAvBayDischAgentCover[0]->SetMouseRegion( 0, 0.816760f, 0.231591f, 0.845892f, 0.349546f );
		pFireSuppressionAvBayDischAgentCover[0]->SetMouseRegion( 1, 0.816760f, 0.231591f, 0.845892f, 0.250382f );
		pFireSuppressionAvBayDischAgentCover[0]->DefineCoverGroup( GRP_A1_COVER4_L1_VC );
		pFireSuppressionAvBayDischAgentCover[0]->SetReference( _V( -1.2237, 2.1413, 14.5724 ), switch_rot );

		pFireSuppressionAvBayDischAgentCover[1]->SetMouseRegion( 0, 0.853003f, 0.231591f, 0.882802f, 0.349546f );
		pFireSuppressionAvBayDischAgentCover[1]->SetMouseRegion( 1, 0.853003f, 0.231591f, 0.882802f, 0.250382f );
		pFireSuppressionAvBayDischAgentCover[1]->DefineCoverGroup( GRP_A1_COVER5_L1_VC );
		pFireSuppressionAvBayDischAgentCover[1]->SetReference( _V( -1.2237, 2.1413, 14.5724 ), switch_rot );

		pFireSuppressionAvBayDischAgentCover[2]->SetMouseRegion( 0, 0.889269f, 0.231591f, 0.920107f, 0.349546f );
		pFireSuppressionAvBayDischAgentCover[2]->SetMouseRegion( 1, 0.889269f, 0.231591f, 0.920107f, 0.250382f );
		pFireSuppressionAvBayDischAgentCover[2]->DefineCoverGroup( GRP_A1_COVER6_L1_VC );
		pFireSuppressionAvBayDischAgentCover[2]->SetReference( _V( -1.2237, 2.1413, 14.5724 ), switch_rot );

		pFireSuppressionAvBayDischAgent[0]->SetStateOffset( 1, 0.0f, 0.488281f );
		pFireSuppressionAvBayDischAgent[0]->SetDirection( push_dir );
		pFireSuppressionAvBayDischAgent[0]->SetMouseRegion( 0.820565f, 0.259112f, 0.841511f, 0.325636f );
		pFireSuppressionAvBayDischAgent[0]->DefineMeshGroup( mesh_index, GRP_A1_S4_L1_VC );

		pFireSuppressionAvBayDischAgent[1]->SetStateOffset( 1, 0.0f, 0.488281f );
		pFireSuppressionAvBayDischAgent[1]->SetDirection( push_dir );
		pFireSuppressionAvBayDischAgent[1]->SetMouseRegion( 0.856432f, 0.259112f, 0.878352f, 0.325636f );
		pFireSuppressionAvBayDischAgent[1]->DefineMeshGroup( mesh_index, GRP_A1_S5_L1_VC );

		pFireSuppressionAvBayDischAgent[2]->SetStateOffset( 1, 0.0f, 0.488281f );
		pFireSuppressionAvBayDischAgent[2]->SetDirection( push_dir );
		pFireSuppressionAvBayDischAgent[2]->SetMouseRegion( 0.893686f, 0.259112f, 0.916010f, 0.325636f );
		pFireSuppressionAvBayDischAgent[2]->DefineMeshGroup( mesh_index, GRP_A1_S6_L1_VC );

		pSmokeDetectionA_Cabin->DefineMeshGroup( mesh_index, GRP_A1_XDS1_A_CABIN_L1_VC );
		pSmokeDetectionA_Cabin->SetStateOffset( 1, 0.0f, -0.232910f );

		pSmokeDetectionA_LFltDeck->DefineMeshGroup( mesh_index, GRP_A1_XDS1_A_LFLTDECK_L1_VC );
		pSmokeDetectionA_LFltDeck->SetStateOffset( 1, 0.0f, -0.232910f );

		pSmokeDetectionA_AvBay1->DefineMeshGroup( mesh_index, GRP_A1_XDS1_A_AVBAY1_L1_VC );
		pSmokeDetectionA_AvBay1->SetStateOffset( 1, 0.0f, -0.232910f );

		pSmokeDetectionA_AvBay2->DefineMeshGroup( mesh_index, GRP_A1_XDS1_A_AVBAY2_L1_VC );
		pSmokeDetectionA_AvBay2->SetStateOffset( 1, 0.0f, -0.232910f );

		pSmokeDetectionA_AvBay3->DefineMeshGroup( mesh_index, GRP_A1_XDS1_A_AVBAY3_L1_VC );
		pSmokeDetectionA_AvBay3->SetStateOffset( 1, 0.0f, -0.232910f );

		pSmokeDetectionB_Payload->DefineMeshGroup( mesh_index, GRP_A1_XDS1_B_PAYLOAD_L1_VC );
		pSmokeDetectionB_Payload->SetStateOffset( 1, 0.0f, -0.232910f );

		pSmokeDetectionB_RFltDeck->DefineMeshGroup( mesh_index, GRP_A1_XDS1_B_RFLTDECK_L1_VC );
		pSmokeDetectionB_RFltDeck->SetStateOffset( 1, 0.0f, -0.232910f );

		pSmokeDetectionB_AvBay1->DefineMeshGroup( mesh_index, GRP_A1_XDS1_B_AVBAY1_L1_VC );
		pSmokeDetectionB_AvBay1->SetStateOffset( 1, 0.0f, -0.232910f );

		pSmokeDetectionB_AvBay2->DefineMeshGroup( mesh_index, GRP_A1_XDS1_B_AVBAY2_L1_VC );
		pSmokeDetectionB_AvBay2->SetStateOffset( 1, 0.0f, -0.232910f );

		pSmokeDetectionB_AvBay3->DefineMeshGroup( mesh_index, GRP_A1_XDS1_B_AVBAY3_L1_VC );
		pSmokeDetectionB_AvBay3->SetStateOffset( 1, 0.0f, -0.232910f );
		return;
	}

	void PanelL1::RegisterVC( void )
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea( AID_L1, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_L1, 
			_V( -1.5142, 2.20883, 13.8049 ) + ofs, _V( -1.18219, 2.21861, 14.7453 ) + ofs, 
			_V( -1.43522, 1.87544, 13.8055 ) + ofs, _V( -1.12764, 1.88993, 14.66 ) + ofs );
		return;
	}

	void PanelL1::DefineVCAnimations( UINT vcidx )
	{
		AtlantisPanel::DefineVCAnimations( mesh_index );
		return;
	}

	void PanelL1::Realize( void )
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "ACA1_1", 16 );
		pSmokeDetectionA_Cabin->ConnectLight( 0, pBundle, 1 );
		pSmokeDetectionA_LFltDeck->ConnectLight( 0, pBundle, 5 );
		pSmokeDetectionA_AvBay1->ConnectLight( 0, pBundle, 9 );
		pSmokeDetectionA_AvBay2->ConnectLight( 0, pBundle, 13 );
		
		pBundle = STS()->BundleManager()->CreateBundle( "ACA1_2", 16 );
		pSmokeDetectionA_AvBay3->ConnectLight( 0, pBundle, 1 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA2_1", 16 );
		pSmokeDetectionB_Payload->ConnectLight( 0, pBundle, 1 );
		pSmokeDetectionB_RFltDeck->ConnectLight( 0, pBundle, 5 );
		pSmokeDetectionB_AvBay1->ConnectLight( 0, pBundle, 9 );
		pSmokeDetectionB_AvBay2->ConnectLight( 0, pBundle, 13 );
		
		pBundle = STS()->BundleManager()->CreateBundle( "ACA2_2", 16 );
		pSmokeDetectionB_AvBay3->ConnectLight( 0, pBundle, 1 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA3_2", 16 );
		pFireSuppressionAvBayDischAgent[0]->ConnectLight( 0, pBundle, 5 );
		pFireSuppressionAvBayDischAgent[1]->ConnectLight( 0, pBundle, 9 );
		pFireSuppressionAvBayDischAgent[2]->ConnectLight( 0, pBundle, 13 );
		
		AtlantisPanel::Realize();
		return;
	}
}
