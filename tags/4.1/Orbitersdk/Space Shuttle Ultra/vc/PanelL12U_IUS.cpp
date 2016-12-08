/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel L12U VC implementation



  Space Shuttle Ultra is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Space Shuttle Ultra is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Space Shuttle Ultra; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See http://spaceshuttleultra.sourceforge.net/license/ for more details.

  **************************************************************************/

#include "PanelL12U_IUS.h"
#include "../meshres_vc_l12u_ius.h"
#include "../Atlantis.h"
#include "../Atlantis_defs.h"
#include "../CommonDefs.h"


namespace vc
{
	PanelL12U_IUS::PanelL12U_IUS( Atlantis* _sts ):AtlantisPanel( _sts, "L12U_IUS" )
	{
		hPanelMesh = oapiLoadMeshGlobal( DEFAULT_MESHNAME_PANELL12U_IUS );
		mesh_index = MESH_UNDEFINED;

		Add( pACT_ORD_ARM = new StdSwitch2( _sts, "ACT ORD ARM" ) );
		pACT_ORD_ARM->SetLabel( 0, "SAFE" );
		pACT_ORD_ARM->SetLabel( 1, "ARM" );

		Add( pACT_1_DISENG = new StdSwitch2( _sts, " ACT 1 DISENG" ) );
		pACT_1_DISENG->SetLabel( 0, "OFF" );
		pACT_1_DISENG->SetLabel( 1, "DISENG" );

		Add( pIUS_RF_ANT_ED = new StdSwitch2( _sts, "IUS RF ANT E/D" ) );
		pIUS_RF_ANT_ED->SetLabel( 0, "OFF" );
		pIUS_RF_ANT_ED->SetLabel( 1, "ON" );

		Add( pACT_2_DISENG = new StdSwitch2( _sts, " ACT 2 DISENG" ) );
		pACT_2_DISENG->SetLabel( 0, "OFF" );
		pACT_2_DISENG->SetLabel( 1, "DISENG" );

		Add( pACT_ORD_PREARM = new StdSwitch2( _sts, "ACT ORD PREARM" ) );
		pACT_ORD_PREARM->SetLabel( 0, "OFF" );
		pACT_ORD_PREARM->SetLabel( 1, "ARM" );

		Add( pACT_ORD_ARM_TB = new StandardTalkback_2( _sts, "ACT ORD ARM TB", 1 ) );
		Add( pIUS_RF_ANT_ENA_TB = new StandardTalkback_2( _sts, "IUS RF ANT ENA TB", 1 ) );
		Add( pIUS_RF_ANT_DSBL_TB = new StandardTalkback_2( _sts, "IUS RF ANT DSBL TB", 1 ) );
		return;
	}

	PanelL12U_IUS::~PanelL12U_IUS()
	{
		return;
	}

	void PanelL12U_IUS::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;
		oapiVCRegisterArea( AID_L12U_IUS, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBPRESSED | PANEL_MOUSE_LBUP );
		oapiVCSetAreaClickmode_Quadrilateral( AID_L12U_IUS,
			_V( -1.5330, 2.4258, 12.2205 ) + ofs, _V( -1.5330, 2.4258, 12.6776 ) + ofs, 
			_V( -1.3427, 2.3011, 12.2205 ) + ofs, _V( -1.3427, 2.3011, 12.6776 ) + ofs );
		return;
	}

	void PanelL12U_IUS::DefineVC()
	{
		const VECTOR3 SWITCH_ROT = _V( 0, 0, 1 );

		AddAIDToMouseEventList( AID_L12U_IUS );

		pACT_ORD_ARM->SetInitialAnimState( 0.5f );
		pACT_ORD_ARM->DefineSwitchGroup( GRP_L12US3_IUS_VC );
		pACT_ORD_ARM->SetReference( _V( -1.4561, 2.3776, 12.3233 ), SWITCH_ROT );
		pACT_ORD_ARM->SetMouseRegion( 0.194471f, 0.355979f, 0.233811f, 0.441061f );

		pACT_1_DISENG->SetInitialAnimState( 0.5f );
		pACT_1_DISENG->DefineSwitchGroup( GRP_L12US4_IUS_VC );
		pACT_1_DISENG->SetReference( _V( -1.4561, 2.3776, 12.3233 ), SWITCH_ROT );
		pACT_1_DISENG->SetMouseRegion( 0.247067f, 0.354133f, 0.300532f, 0.438349f );

		pIUS_RF_ANT_ED->SetInitialAnimState( 0.5f );
		pIUS_RF_ANT_ED->DefineSwitchGroup( GRP_L12US5_IUS_VC );
		pIUS_RF_ANT_ED->SetReference( _V( -1.4561, 2.3776, 12.3233 ), SWITCH_ROT );
		pIUS_RF_ANT_ED->SetMouseRegion( 0.317950f, 0.355818f, 0.359740f, 0.441822f );

		pACT_2_DISENG->SetInitialAnimState( 0.5f );
		pACT_2_DISENG->DefineSwitchGroup( GRP_L12US6_IUS_VC );
		pACT_2_DISENG->SetReference( _V( -1.4561, 2.3776, 12.2933 ), SWITCH_ROT );
		pACT_2_DISENG->SetMouseRegion( 0.372846f, 0.353155f, 0.426563f, 0.437927f );

		pACT_ORD_PREARM->SetInitialAnimState( 0.5f );
		pACT_ORD_PREARM->DefineSwitchGroup( GRP_L12US10_IUS_VC );
		pACT_ORD_PREARM->SetReference( _V( -1.3684, 2.3204, 12.2933 ), SWITCH_ROT );
		pACT_ORD_PREARM->SetMouseRegion( 0.260196f, 0.815091f, 0.303861f, 0.904874f );

		pACT_ORD_ARM_TB->DefineMeshGroup( mesh_index, GRP_TB_ACT_ORD_ARM_IUS_VC );
		pIUS_RF_ANT_ENA_TB->DefineMeshGroup( mesh_index, GRP_TB_IUS_RF_ANT_ENA_IUS_VC );
		pIUS_RF_ANT_DSBL_TB->DefineMeshGroup( mesh_index, GRP_TB_IUS_RF_ANT_DSBL_IUS_VC );
		return;
	}

	void PanelL12U_IUS::AddMeshes( const VECTOR3 &ofs )
	{
		SetHasOwnVCMesh();

		if (mesh_index == MESH_UNDEFINED)
		{
			mesh_index = STS()->AddMesh( hPanelMesh, &ofs );
			STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		}
		return;
	}

	void PanelL12U_IUS::SetMeshVisibility( bool visible )
	{
		if (visible) STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		else STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_NEVER );
		return;
	}

	UINT PanelL12U_IUS::GetVCMeshIndex() const
	{
		return mesh_index;
	}

	void PanelL12U_IUS::DefineVCAnimations( UINT vcidx )
	{
		AtlantisPanel::DefineVCAnimations( mesh_index );
		return;
	}

	void PanelL12U_IUS::Realize()
	{
		/*DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "L12UtoASEIUS", 16 );
		pACT_ORD_ARM->ConnectPort( 1, pBundle, 0 );
		pACT_ORD_ARM_TB->SetInput( 0, pBundle, 1, TB_GRAY );
		pACT_1_DISENG->ConnectPort( 1, pBundle, 2 );
		pIUS_RF_ANT_ED->ConnectPort( 1, pBundle, 3 );
		pIUS_RF_ANT_ENA_TB->SetInput( 0, pBundle, 4, TB_GRAY );
		pIUS_RF_ANT_DSBL_TB->SetInput( 0, pBundle, 5, TB_GRAY );
		pACT_2_DISENG->ConnectPort( 1, pBundle, 6 );
		pACT_ORD_PREARM->ConnectPort( 1, pBundle, 7 );*/
		
		AtlantisPanel::Realize();
		return;
	}
};
