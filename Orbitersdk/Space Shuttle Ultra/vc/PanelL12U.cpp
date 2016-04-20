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

#include "PanelL12U.h"
#include "../meshres_vc_l12u.h"
#include "../meshres_vc_additions.h"
#include "../Atlantis.h"
#include "../Atlantis_defs.h"
#include "../CommonDefs.h"


namespace vc
{
	PanelL12U::PanelL12U( Atlantis* _sts ):AtlantisPanel( _sts, "L12U" )
	{
		hPanelMesh = oapiLoadMeshGlobal( DEFAULT_MESHNAME_PANELL12U );
		mesh_index = MESH_UNDEFINED;

		Add( pSUPER_ZIP_PRI_FIRE = new StdSwitch3( _sts, "SUPER*ZIP PRI FIRE" ) );
		pSUPER_ZIP_PRI_FIRE->SetLabel( 2, "FIRE" );

		Add( pSUPER_ZIP_PRI_ARM = new StdSwitch2( _sts, "SUPER*ZIP PRI ARM" ) );
		pSUPER_ZIP_PRI_ARM->SetLabel( 0, "SAFE" );
		pSUPER_ZIP_PRI_ARM->SetLabel( 1, "ARM" );

		Add( pDA_PRI_ROT = new StdSwitch3( _sts, "DA PRI ROT" ) );
		pDA_PRI_ROT->SetLabel( 0, "DN" );
		pDA_PRI_ROT->SetLabel( 2, "UP" );

		Add( pLOGIC_PRI_PWR = new StdSwitch2( _sts, "LOGIC PRI PWR" ) );
		pLOGIC_PRI_PWR->SetLabel( 0, "OFF" );
		pLOGIC_PRI_PWR->SetLabel( 1, "ON" );

		Add( pSSP_PRI_PWR = new StdSwitch2( _sts, "SSP PRI PWR" ) );
		pSSP_PRI_PWR->SetLabel( 0, "OFF" );
		pSSP_PRI_PWR->SetLabel( 1, "ON" );

		Add( pMECH_PRI_PWR = new StdSwitch3( _sts, "MECH PRI PWR" ) );
		pMECH_PRI_PWR->SetLabel( 0, "OFF" );
		pMECH_PRI_PWR->SetLabel( 2, "ON" );


		Add( pSUPER_ZIP_BKUP_FIRE = new StdSwitch3( _sts, "SUPER*ZIP BKUP FIRE" ) );
		pSUPER_ZIP_BKUP_FIRE->SetLabel( 2, "FIRE" );

		Add( pSUPER_ZIP_BKUP_ARM = new StdSwitch2( _sts, "SUPER*ZIP BKUP ARM" ) );
		pSUPER_ZIP_BKUP_ARM->SetLabel( 0, "SAFE" );
		pSUPER_ZIP_BKUP_ARM->SetLabel( 1, "ARM" );

		Add( pDA_BKUP_ROT = new StdSwitch3( _sts, "DA BKUP ROT" ) );
		pDA_BKUP_ROT->SetLabel( 0, "DN" );
		pDA_BKUP_ROT->SetLabel( 2, "UP" );

		Add( pLOGIC_BKUP_PWR = new StdSwitch2( _sts, "LOGIC BKUP PWR" ) );
		pLOGIC_BKUP_PWR->SetLabel( 0, "OFF" );
		pLOGIC_BKUP_PWR->SetLabel( 1, "ON" );

		Add( pSSP_BKUP_PWR = new StdSwitch2( _sts, "SSP BKUP PWR" ) );
		pSSP_BKUP_PWR->SetLabel( 0, "OFF" );
		pSSP_BKUP_PWR->SetLabel( 1, "ON" );

		Add( pMECH_BKUP_PWR = new StdSwitch3( _sts, "MECH BKUP PWR" ) );
		pMECH_BKUP_PWR->SetLabel( 0, "OFF" );
		pMECH_BKUP_PWR->SetLabel( 2, "ON" );

		Add( pMECH_PRI_PWR_TB = new Std2SegTalkback( _sts, "MECH PRI PWR TB", 1 ) );
		Add( pPosition_PRI_TB = new Std2SegTalkback( _sts, "Position PRI TB", 1 ) );
		Add( pSUPER_ZIP_PRI_ARM_TB = new Std2SegTalkback( _sts, "SUPER*ZIP PRI ARM TB", 1 ) );
		Add( pMECH_BKUP_PWR_TB = new Std2SegTalkback( _sts, "MECH BKUP PWR TB", 1 ) );
		Add( pPosition_BKUP_TB = new Std2SegTalkback( _sts, "Position BKUP TB", 1 ) );
		Add( pSUPER_ZIP_BKUP_ARM_TB = new Std2SegTalkback( _sts, "SUPER*ZIP BKUP ARM TB", 1 ) );
		return;
	}

	PanelL12U::~PanelL12U()
	{
		return;
	}

	void PanelL12U::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;
		oapiVCRegisterArea( AID_L12U, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBPRESSED | PANEL_MOUSE_LBUP );
		oapiVCSetAreaClickmode_Quadrilateral( AID_L12U,
			_V( -1.5330, 2.4258, 12.2205 ) + ofs, _V( -1.5330, 2.4258, 12.6776 ) + ofs, 
			_V( -1.3427, 2.3011, 12.2205 ) + ofs, _V( -1.3427, 2.3011, 12.6776 ) + ofs );
		
		SURFHANDLE panel_tex = oapiGetTextureHandle( hPanelMesh, 1 );
		oapiVCRegisterArea( AID_L12U_TKBK1, _R( 408, 182, 469, 233 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panel_tex );
		oapiVCRegisterArea( AID_L12U_TKBK2, _R( 659, 182, 720, 233 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panel_tex );
		oapiVCRegisterArea( AID_L12U_TKBK3, _R( 419, 645, 480, 696 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panel_tex );
		oapiVCRegisterArea( AID_L12U_TKBK4, _R( 1380, 181, 1441, 232 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panel_tex );
		oapiVCRegisterArea( AID_L12U_TKBK5, _R( 1630, 182, 1691, 233 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panel_tex );
		oapiVCRegisterArea( AID_L12U_TKBK6, _R( 1418, 645, 1479, 696 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panel_tex );
		return;
	}

	void PanelL12U::DefineVC()
	{
		const VECTOR3 SWITCH_ROT = _V( 0, 0, 1 );

		AddAIDToMouseEventList( AID_L12U );

		pSUPER_ZIP_PRI_FIRE->SetInitialAnimState( 0.5f );
		pSUPER_ZIP_PRI_FIRE->DefineSwitchGroup( GRP_L12US2_VC );
		pSUPER_ZIP_PRI_FIRE->SetReference( _V( -1.4561, 2.3776, 12.3233 ), SWITCH_ROT );
		pSUPER_ZIP_PRI_FIRE->SetMouseRegion( 0.116959f, 0.351245f, 0.159764f, 0.441283f );
		pSUPER_ZIP_PRI_FIRE->SetSpringLoaded( true );

		pSUPER_ZIP_PRI_ARM->SetInitialAnimState( 0.5f );
		pSUPER_ZIP_PRI_ARM->DefineSwitchGroup( GRP_L12US3_VC );
		pSUPER_ZIP_PRI_ARM->SetReference( _V( -1.4561, 2.3776, 12.3233 ), SWITCH_ROT );
		pSUPER_ZIP_PRI_ARM->SetMouseRegion( 0.191898f, 0.352835f, 0.236525f, 0.441156f );

		pDA_PRI_ROT->SetInitialAnimState( 0.5f );
		pDA_PRI_ROT->DefineSwitchGroup( GRP_L12US5_VC );
		pDA_PRI_ROT->SetReference( _V( -1.4561, 2.3776, 12.3233 ), SWITCH_ROT );
		pDA_PRI_ROT->SetMouseRegion( 0.317172f, 0.354949f, 0.358382f, 0.442820f );
		pDA_PRI_ROT->SetSpringLoaded( true );

		pLOGIC_PRI_PWR->SetInitialAnimState( 0.5f );
		pLOGIC_PRI_PWR->DefineSwitchGroup( GRP_L12US6_VC );
		pLOGIC_PRI_PWR->SetReference( _V( -1.4561, 2.3776, 12.3233 ), SWITCH_ROT );
		pLOGIC_PRI_PWR->SetMouseRegion( 0.375559f, 0.353634f, 0.422636f, 0.441116f );

		pSSP_PRI_PWR->SetInitialAnimState( 0.5f );
		pSSP_PRI_PWR->DefineSwitchGroup( GRP_L12US8_VC );
		pSSP_PRI_PWR->SetReference( _V( -1.3684, 2.3204, 12.2933 ), SWITCH_ROT );
		pSSP_PRI_PWR->SetMouseRegion( 0.123397f, 0.813643f, 0.167096f, 0.903715f );

		pMECH_PRI_PWR->SetInitialAnimState( 0.5f );
		pMECH_PRI_PWR->DefineSwitchGroup( GRP_L12US9_VC );
		pMECH_PRI_PWR->SetReference( _V( -1.3684, 2.3204, 12.2933 ), SWITCH_ROT );
		pMECH_PRI_PWR->SetMouseRegion( 0.199524f, 0.813388f, 0.244427f, 0.904525f );
		pMECH_PRI_PWR->SetSpringLoaded( true );


		pSUPER_ZIP_BKUP_FIRE->SetInitialAnimState( 0.5f );
		pSUPER_ZIP_BKUP_FIRE->DefineSwitchGroup( GRP_L12US14_VC );
		pSUPER_ZIP_BKUP_FIRE->SetReference( _V( -1.4561, 2.3776, 12.3233 ), SWITCH_ROT );
		pSUPER_ZIP_BKUP_FIRE->SetMouseRegion( 0.591131f, 0.352536f, 0.636050f, 0.443414f );
		pSUPER_ZIP_BKUP_FIRE->SetSpringLoaded( true );

		pSUPER_ZIP_BKUP_ARM->SetInitialAnimState( 0.5f );
		pSUPER_ZIP_BKUP_ARM->DefineSwitchGroup( GRP_L12US15_VC );
		pSUPER_ZIP_BKUP_ARM->SetReference( _V( -1.4561, 2.3776, 12.3233 ), SWITCH_ROT );
		pSUPER_ZIP_BKUP_ARM->SetMouseRegion( 0.670413f, 0.351977f, 0.715270f, 0.443036f );

		pDA_BKUP_ROT->SetInitialAnimState( 0.5f );
		pDA_BKUP_ROT->DefineSwitchGroup( GRP_L12US17_VC );
		pDA_BKUP_ROT->SetReference( _V( -1.4561, 2.3776, 12.3233 ), SWITCH_ROT );
		pDA_BKUP_ROT->SetMouseRegion( 0.792740f, 0.353993f, 0.837321f, 0.444548f );
		pDA_BKUP_ROT->SetSpringLoaded( true );

		pLOGIC_BKUP_PWR->SetInitialAnimState( 0.5f );
		pLOGIC_BKUP_PWR->DefineSwitchGroup( GRP_L12US18_VC );
		pLOGIC_BKUP_PWR->SetReference( _V( -1.4561, 2.3776, 12.3233 ), SWITCH_ROT );
		pLOGIC_BKUP_PWR->SetMouseRegion( 0.852032f, 0.356422f, 0.896194f, 0.440456f );

		pSSP_BKUP_PWR->SetInitialAnimState( 0.5f );
		pSSP_BKUP_PWR->DefineSwitchGroup( GRP_L12US20_VC );
		pSSP_BKUP_PWR->SetReference( _V( -1.3684, 2.3204, 12.2933 ), SWITCH_ROT );
		pSSP_BKUP_PWR->SetMouseRegion( 0.617838f, 0.812859f, 0.660512f, 0.903650f );

		pMECH_BKUP_PWR->SetInitialAnimState( 0.5f );
		pMECH_BKUP_PWR->DefineSwitchGroup( GRP_L12US21_VC );
		pMECH_BKUP_PWR->SetReference( _V( -1.3684, 2.3204, 12.2933 ), SWITCH_ROT );
		pMECH_BKUP_PWR->SetMouseRegion( 0.692812f, 0.812956f, 0.735333f, 0.902322f );
		pMECH_BKUP_PWR->SetSpringLoaded( true );

		pSUPER_ZIP_PRI_ARM_TB->AddAIDToRedrawEventList( AID_L12U_TKBK1 );
		pSUPER_ZIP_PRI_ARM_TB->SetDimensions( 61, 51 );
		pSUPER_ZIP_PRI_ARM_TB->SetTalkbackLocation( 0, 0 );
		pSUPER_ZIP_PRI_ARM_TB->SetInactiveSegment( 0, TB_GRAY );

		pPosition_PRI_TB->AddAIDToRedrawEventList( AID_L12U_TKBK2 );
		pPosition_PRI_TB->SetDimensions( 61, 51 );
		pPosition_PRI_TB->SetTalkbackLocation( 0, 0 );
		pPosition_PRI_TB->SetInactiveSegment( 0, TB_GRAY );

		pMECH_PRI_PWR_TB->AddAIDToRedrawEventList( AID_L12U_TKBK3 );
		pMECH_PRI_PWR_TB->SetDimensions( 61, 51 );
		pMECH_PRI_PWR_TB->SetTalkbackLocation( 0, 0 );
		pMECH_PRI_PWR_TB->SetInactiveSegment( 0, TB_GRAY );

		pSUPER_ZIP_BKUP_ARM_TB->AddAIDToRedrawEventList( AID_L12U_TKBK4 );
		pSUPER_ZIP_BKUP_ARM_TB->SetDimensions( 61, 51 );
		pSUPER_ZIP_BKUP_ARM_TB->SetTalkbackLocation( 0, 0 );
		pSUPER_ZIP_BKUP_ARM_TB->SetInactiveSegment( 0, TB_GRAY );

		pPosition_BKUP_TB->AddAIDToRedrawEventList( AID_L12U_TKBK5 );
		pPosition_BKUP_TB->SetDimensions( 61, 51 );
		pPosition_BKUP_TB->SetTalkbackLocation( 0, 0 );
		pPosition_BKUP_TB->SetInactiveSegment( 0, TB_GRAY );

		pMECH_BKUP_PWR_TB->AddAIDToRedrawEventList( AID_L12U_TKBK6 );
		pMECH_BKUP_PWR_TB->SetDimensions( 61, 51 );
		pMECH_BKUP_PWR_TB->SetTalkbackLocation( 0, 0 );
		pMECH_BKUP_PWR_TB->SetInactiveSegment( 0, TB_GRAY );
		return;
	}

	void PanelL12U::AddMeshes( const VECTOR3 &ofs )
	{
		SetHasOwnVCMesh();

		if (mesh_index == MESH_UNDEFINED)
		{
			mesh_index = STS()->AddMesh( hPanelMesh, &ofs );
			STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		}
		return;
	}

	void PanelL12U::SetMeshVisibility( bool visible )
	{
		if (visible) STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		else STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_NEVER );
		return;
	}

	UINT PanelL12U::GetVCMeshIndex() const
	{
		return mesh_index;
	}

	void PanelL12U::DefineVCAnimations( UINT vcidx )
	{
		AtlantisPanel::DefineVCAnimations( mesh_index );
		return;
	}

	void PanelL12U::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "L12UtoCISS_PRI", 16 );
		pSUPER_ZIP_PRI_FIRE->ConnectPort( 2, pBundle, 0 );
		pSUPER_ZIP_PRI_ARM->ConnectPort( 1, pBundle, 1 );
		pSUPER_ZIP_PRI_ARM_TB->SetInput( 0, pBundle, 2, TB_GRAY );
		pDA_PRI_ROT->ConnectPort( 1, pBundle, 3 );
		pDA_PRI_ROT->ConnectPort( 2, pBundle, 4 );
		pPosition_PRI_TB->SetInput( 0, pBundle, 5, TB_GRAY );
		pLOGIC_PRI_PWR->ConnectPort( 1, pBundle, 6 );
		pSSP_PRI_PWR->ConnectPort( 1, pBundle, 7 );
		pMECH_PRI_PWR->ConnectPort( 1, pBundle, 8 );
		pMECH_PRI_PWR->ConnectPort( 2, pBundle, 9 );
		pMECH_PRI_PWR_TB->SetInput( 0, pBundle, 10, TB_GRAY );

		pBundle = STS()->BundleManager()->CreateBundle( "L12UtoCISS_BKUP", 16 );
		pSUPER_ZIP_BKUP_FIRE->ConnectPort( 2, pBundle, 0 );
		pSUPER_ZIP_BKUP_ARM->ConnectPort( 1, pBundle, 1 );
		pSUPER_ZIP_BKUP_ARM_TB->SetInput( 0, pBundle, 2, TB_GRAY );
		pDA_BKUP_ROT->ConnectPort( 1, pBundle, 3 );
		pDA_BKUP_ROT->ConnectPort( 2, pBundle, 4 );
		pPosition_BKUP_TB->SetInput( 0, pBundle, 5, TB_GRAY );
		pLOGIC_BKUP_PWR->ConnectPort( 1, pBundle, 6 );
		pSSP_BKUP_PWR->ConnectPort( 1, pBundle, 7 );
		pMECH_BKUP_PWR->ConnectPort( 1, pBundle, 8 );
		pMECH_BKUP_PWR->ConnectPort( 2, pBundle, 9 );
		pMECH_BKUP_PWR_TB->SetInput( 0, pBundle, 10, TB_GRAY );
		
		AtlantisPanel::Realize();
		return;
	}
};
