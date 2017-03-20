#include "PanelA2.h"
#include "..\Atlantis.h"
#include "..\Atlantis_defs.h"


namespace vc
{
	PanelA2::PanelA2( Atlantis* _sts ):AtlantisPanel( _sts, "A2" )
	{
		power = true;

		Add( pDIGI_DIS_SELECT = new StdSwitch3( _sts, "DIGI DIS SELECT" ) );
		pDIGI_DIS_SELECT->SetLabel( 1, "EL/AZ" );
		pDIGI_DIS_SELECT->SetLabel( 2, "R/Rbar" );

		Add( pXPNTR_SCALE = new StdSwitch2( _sts, "X-PNTR SCALE" ) );
		pXPNTR_SCALE->SetLabel( 0, "X10" );
		pXPNTR_SCALE->SetLabel( 1, "X1" );

		Add( pRE_RRA = new _7SegDisp_RE_RRA( _sts, "Range/Elevation / Range Rate/Azimuth" ) );

		Add( pKey = new StandardSingleLight( _sts, "KEY" ) );
	}

	PanelA2::~PanelA2()
	{
	}

	void PanelA2::DefineVC()
	{
		const VECTOR3 switch_rot = _V( -1.0, 0.0, 0.0 );

		AddAIDToMouseEventList( AID_A2 );

		pDIGI_DIS_SELECT->SetInitialAnimState( 0.5f );
		pDIGI_DIS_SELECT->DefineSwitchGroup( GRP_A2U1_VC );
		pDIGI_DIS_SELECT->SetMouseRegion( 0.136026f, 0.298255f, 0.726990f, 0.441468f );
		pDIGI_DIS_SELECT->SetReference( _V( 0.0022, 3.2618, 12.3100 ), switch_rot );
		pDIGI_DIS_SELECT->SetSpringLoaded( true, 0 );

		pXPNTR_SCALE->SetInitialAnimState( 0.5f );
		pXPNTR_SCALE->DefineSwitchGroup( GRP_A2U2_VC );
		pXPNTR_SCALE->SetMouseRegion( 0.174571f, 0.653784f, 0.766816f, 0.789220f );
		pXPNTR_SCALE->SetReference( _V( 0.0037, 3.2001, 12.3100 ), switch_rot );

		pRE_RRA->DefineMesh( STS()->mesh_vc );
		pRE_RRA->DefineComponent( GRP_A2_RANGE_ELEVATION_1_VC, true, true, false, _7SD_STATE_NUM0_DOTOFF );
		pRE_RRA->DefineComponent( GRP_A2_RANGE_ELEVATION_2_VC, true, true, false, _7SD_STATE_NUM0_DOTOFF );
		pRE_RRA->DefineComponent( GRP_A2_RANGE_ELEVATION_3_VC, true, true, false, _7SD_STATE_NUM0_DOTOFF );
		pRE_RRA->DefineComponent( GRP_A2_RANGE_ELEVATION_4_VC, true, true, false, _7SD_STATE_NUM0_DOTOFF );
		pRE_RRA->DefineComponent( GRP_A2_RANGE_ELEVATION_SIGN_VC, false, false, true, _7SD_STATE_SIGN2OFF );
		pRE_RRA->DefineComponent( GRP_A2_RANGERATE_AZIMUTH_1_VC, true, true, false, _7SD_STATE_NUM0_DOTOFF );
		pRE_RRA->DefineComponent( GRP_A2_RANGERATE_AZIMUTH_2_VC, true, true, false, _7SD_STATE_NUM0_DOTOFF );
		pRE_RRA->DefineComponent( GRP_A2_RANGERATE_AZIMUTH_3_VC, true, true, false, _7SD_STATE_NUM0_DOTOFF );
		pRE_RRA->DefineComponent( GRP_A2_RANGERATE_AZIMUTH_4_VC, true, true, false, _7SD_STATE_NUM0_DOTOFF );
		pRE_RRA->DefineComponent( GRP_A2_RANGERATE_AZIMUTH_SIGN_VC, false, false, true, _7SD_STATE_SIGN2OFF );

		pKey->DefineMeshGroup( STS()->mesh_vc, GRP_A2_KEY_VC );
		pKey->SetStateOffset( 1, 0.0f, 0.044434f );
		return;
	}

	void PanelA2::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea( AID_A2, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_A2, 
			_V( 0.0178, 3.3287, 12.3071 ) + ofs, _V( -0.0173, 3.3287, 12.3071 ) + ofs, 
			_V( 0.0178, 3.1499, 12.3071 ) + ofs, _V( -0.0173, 3.1499, 12.3071 ) + ofs );
	}

	void PanelA2::DefineVCAnimations( UINT vcidx )
	{
		static UINT M1_AZ_Grp[1] = {GRP_A2ELV_VC};
		static MGROUP_TRANSLATE M1_AZ_Translate = MGROUP_TRANSLATE( vcidx, M1_AZ_Grp, 1, _V( -0.0725, 0, 0 ) );
		anim_M1_AZ = STS()->CreateAnimation( 0.5 );
		STS()->AddAnimationComponent( anim_M1_AZ, 0, 1, &M1_AZ_Translate );

		static UINT M1_EL_Grp[1] = {GRP_A2ELH_VC};
		static MGROUP_TRANSLATE M1_EL_Translate = MGROUP_TRANSLATE( vcidx, M1_EL_Grp, 1, _V( 0, 0.072, 0 ) );
		anim_M1_EL = STS()->CreateAnimation( 0.5 );
		STS()->AddAnimationComponent( anim_M1_EL, 0, 1, &M1_EL_Translate );

		AtlantisPanel::DefineVCAnimations( vcidx );
		return;
	}

	void PanelA2::OnPreStep( double SimT, double DeltaT, double MJD )
	{
		AtlantisPanel::OnPreStep( SimT, DeltaT, MJD );

		if (pPower_On.IsSet() == false)
		{
			if (power == true)
			{
				power = false;
				STS()->SetAnimation( anim_M1_AZ, 0.5 );
				STS()->SetAnimation( anim_M1_EL, 0.5 );
			}
			return;
		}

		power = true;

		double az_pos = ((pXPNTR_SCALE_X1 * pAZrate0_2.GetVoltage()) + (!pXPNTR_SCALE_X1 * pAZrate0_20.GetVoltage())) * 0.1 + 0.5;
		double el_pos = ((pXPNTR_SCALE_X1 * pELrate0_2.GetVoltage()) + (!pXPNTR_SCALE_X1 * pELrate0_20.GetVoltage())) * 0.1 + 0.5;
		STS()->SetAnimation( anim_M1_AZ, az_pos );
		STS()->SetAnimation( anim_M1_EL, el_pos );
		return;
	}

	void PanelA2::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "A2_SWITCHES_DATA", 16 );
		pDIGI_DIS_SELECT->ConnectPort( 1, pBundle, 0 );
		pDIGI_DIS_SELECT->ConnectPort( 2, pBundle, 1 );
		pXPNTR_SCALE->ConnectPort( 1, pBundle, 2 );

		pXPNTR_SCALE_X1.Connect( pBundle, 2 );

		pELrate0_20.Connect( pBundle, 7 );
		pELrate0_2.Connect( pBundle, 8 );
		pAZrate0_20.Connect( pBundle, 9 );
		pAZrate0_2.Connect( pBundle, 10 );

		pBundle = STS()->BundleManager()->CreateBundle( "A1U_SWITCHES_B", 16 );
		pPower_On.Connect( pBundle, 6 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA5_5", 16 );
		pKey->ConnectLight( 0, pBundle, 9 );

		AtlantisPanel::Realize();
	}
};