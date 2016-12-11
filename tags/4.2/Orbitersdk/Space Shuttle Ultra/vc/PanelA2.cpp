#include "PanelA2.h"
#include "..\Atlantis.h"
#include "..\Atlantis_defs.h"
#include "..\meshres_vc_additions.h"


extern GDIParams g_Param;


namespace vc
{
	PanelA2::PanelA2( Atlantis* _sts ):AtlantisPanel( _sts, "A2" )
	{
		range_elevation = 0;
		rangerate_azimuth = 0;
		power = true;

		Add( pDIGI_DIS_SELECT = new StdSwitch3( _sts, "DIGI DIS SELECT" ) );
		pDIGI_DIS_SELECT->SetLabel( 1, "EL/AZ" );
		pDIGI_DIS_SELECT->SetLabel( 2, "R/Rbar" );

		Add( pXPNTR_SCALE = new StdSwitch2( _sts, "X-PNTR SCALE" ) );
		pXPNTR_SCALE->SetLabel( 0, "X10" );
		pXPNTR_SCALE->SetLabel( 1, "X1" );
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
	}

	void PanelA2::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea( AID_A2, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_A2, 
			_V( 0.0178, 3.3287, 12.3071 ) + ofs, _V( -0.0173, 3.3287, 12.3071 ) + ofs, 
			_V( 0.0178, 3.1499, 12.3071 ) + ofs, _V( -0.0173, 3.1499, 12.3071 ) + ofs );

		SURFHANDLE paneltex = oapiGetTextureHandle( STS()->hOrbiterVCMesh, TEX_PANELA2_VC );
		oapiVCRegisterArea( AID_A2_LED1, _R( 384, 597, 569, 644 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, paneltex );
		oapiVCRegisterArea( AID_A2_LED2, _R( 384, 805, 569, 852 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, paneltex );
	}

	bool PanelA2::OnVCRedrawEvent( int id, int _event, SURFHANDLE surf )
	{
		static const int NUMX[10] = {64, 0, 64, 128, 192, 256, 320, 384, 448, 0};
		static const int NUMY[10] = {384, 448, 448, 448, 448, 448, 448, 448, 448, 384};

		if (id == AID_A2_LED1)// RANGE/ELEVATION
		{
			if (power == false)
			{
				RECT rect_tgt;
				RECT rect_src = _R( 192, 0, 256, 64 );

				for (int i = 0; i < 5; i++)
				{
					rect_tgt = _R( i * 37, 0, (i + 1) * 37, 47 );
					oapiBlt( surf, g_Param.digits_7seg, &rect_tgt, &rect_src );
				}
			}
			else if (pDIGI_DIS_SELECT_LAMPTEST.IsSet())
			{
				// test
				RECT rect_tgt = _R( 0, 0, 37, 47 );
				RECT rect_src = _R( 0, 0, 64, 64 );// +
				oapiBlt( surf, g_Param.digits_7seg, &rect_tgt, &rect_src );

				for (int i = 1; i < 5; i++)
				{
					rect_src = _R( 448, 448, 512, 512 );// 8
					rect_tgt = _R( i * 37, 0, (i + 1) * 37, 47 );
					oapiBlt( surf, g_Param.digits_7seg, &rect_tgt, &rect_src );

					rect_src = _R( 184, 56, 192, 64 );// .
					rect_tgt = _R( ((i + 1) * 37) - 8, 37, (i + 1) * 37, 45 );
					oapiBlt( surf, g_Param.digits_7seg, &rect_tgt, &rect_src );
				}
			}
			else if (pDIGI_DIS_SELECT_RRbar.IsSet())
			{
				// range
				RECT rect_tgt = _R( 0, 0, 37, 47 );
				RECT rect_src = _R( 192, 0, 256, 64 );
				oapiBlt( surf, g_Param.digits_7seg, &rect_tgt, &rect_src );

				char cbuf[8];
				int decimalpoint = 4;
				if (range_elevation < 9.99)
				{
					// X.XX0
					sprintf_s( cbuf, 8, "%04.2f0", range_elevation );
					decimalpoint = 1;
				}
				else if (range_elevation < 99.99)
				{
					// XX.XX
					sprintf_s( cbuf, 8, "%05.2f", range_elevation );
					decimalpoint = 2;
				}
				else if (range_elevation < 999.9)
				{
					// XXX.X
					sprintf_s( cbuf, 8, "%05.1f", range_elevation );
					decimalpoint = 3;
				}
				else if (range_elevation < 9999) sprintf_s( cbuf, 8, "%04.0f", range_elevation );// XXXX.
				else sprintf_s( cbuf, 8, "9999" );

				int num;
				for (int i = 1, j = 0; i < 5; i++, j++)
				{
					if (cbuf[j] == '.') j++;

					num = cbuf[j] - 48;
					rect_src = _R( NUMX[num], NUMY[num], NUMX[num] + 64, NUMY[num] + 64 );
					rect_tgt = _R( i * 37, 0, (i + 1) * 37, 47 );
					oapiBlt( surf, g_Param.digits_7seg, &rect_tgt, &rect_src );
				}

				rect_src = _R( 184, 56, 192, 64 );// .
				rect_tgt = _R( ((decimalpoint + 1) * 37) - 8, 37, (decimalpoint + 1) * 37, 45 );
				oapiBlt( surf, g_Param.digits_7seg, &rect_tgt, &rect_src );
			}
			else
			{
				// elevation
				RECT rect_tgt = _R( 0, 0, 37, 47 );
				RECT rect_src = _R( 192, 0, 256, 64 );
				if (range_elevation < 0) rect_src = _R( 64, 0, 128, 64 );// -
				else if (range_elevation > 0) rect_src = _R( 0, 0, 64, 64 );// +
				oapiBlt( surf, g_Param.digits_7seg, &rect_tgt, &rect_src );

				char cbuf[8];
				sprintf_s( cbuf, 8, "%05.1f", fabs( range_elevation ) );

				int num;
				for (int i = 1, j = 0; i < 5; i++, j++)
				{
					if (cbuf[j] == '.') j++;

					num = cbuf[j] - 48;
					rect_src = _R( NUMX[num], NUMY[num], NUMX[num] + 64, NUMY[num] + 64 );
					rect_tgt = _R( i * 37, 0, (i + 1) * 37, 47 );
					oapiBlt( surf, g_Param.digits_7seg, &rect_tgt, &rect_src );
				}

				rect_src = _R( 184, 56, 192, 64 );// .
				rect_tgt = _R( 140, 37, 148, 45 );
				oapiBlt( surf, g_Param.digits_7seg, &rect_tgt, &rect_src );
			}

			return true;
		}
		else if (id == AID_A2_LED2)// RANGE RATE/AZIMUTH
		{
			if (power == false)
			{
				RECT rect_tgt;
				RECT rect_src = _R( 192, 0, 256, 64 );

				for (int i = 0; i < 5; i++)
				{
					rect_tgt = _R( i * 37, 0, (i + 1) * 37, 47 );
					oapiBlt( surf, g_Param.digits_7seg, &rect_tgt, &rect_src );
				}
			}
			else if (pDIGI_DIS_SELECT_LAMPTEST.IsSet())
			{
				// test
				RECT rect_tgt = _R( 0, 0, 37, 47 );
				RECT rect_src = _R( 0, 0, 64, 64 );// +
				oapiBlt( surf, g_Param.digits_7seg, &rect_tgt, &rect_src );

				for (int i = 1; i < 5; i++)
				{
					rect_src = _R( 448, 448, 512, 512 );// 8
					rect_tgt = _R( i * 37, 0, (i + 1) * 37, 47 );
					oapiBlt( surf, g_Param.digits_7seg, &rect_tgt, &rect_src );

					rect_src = _R( 184, 56, 192, 64 );// .
					rect_tgt = _R( ((i + 1) * 37) - 8, 37, (i + 1) * 37, 45 );
					oapiBlt( surf, g_Param.digits_7seg, &rect_tgt, &rect_src );
				}
			}
			else if (pDIGI_DIS_SELECT_RRbar.IsSet())
			{
				// range rate
				RECT rect_tgt = _R( 0, 0, 37, 47 );
				RECT rect_src = _R( 192, 0, 256, 64 );
				if (rangerate_azimuth < 0) rect_src = _R( 64, 0, 128, 64 );// -
				else if (rangerate_azimuth > 0) rect_src = _R( 0, 0, 64, 64 );// +
				oapiBlt( surf, g_Param.digits_7seg, &rect_tgt, &rect_src );

				char cbuf[8];
				int decimalpoint = 4;
				if (fabs( range_elevation ) <= 999.9)
				{
					sprintf_s( cbuf, 8, "%05.1f", fabs( rangerate_azimuth ) );
					decimalpoint = 3;
				}
				else
				{
					sprintf_s( cbuf, 8, "%04.0f", fabs( rangerate_azimuth ) );
				}

				int num;
				for (int i = 1, j = 0; i < 5; i++, j++)
				{
					if (cbuf[j] == '.') j++;

					num = cbuf[j] - 48;
					rect_src = _R( NUMX[num], NUMY[num], NUMX[num] + 64, NUMY[num] + 64 );
					rect_tgt = _R( i * 37, 0, (i + 1) * 37, 47 );
					oapiBlt( surf, g_Param.digits_7seg, &rect_tgt, &rect_src );
				}

				rect_src = _R( 184, 56, 192, 64 );// .
				rect_tgt = _R( ((decimalpoint + 1) * 37) - 8, 37, (decimalpoint + 1) * 37, 45 );
				oapiBlt( surf, g_Param.digits_7seg, &rect_tgt, &rect_src );
			}
			else
			{
				// azimuth
				RECT rect_tgt = _R( 0, 0, 37, 47 );
				RECT rect_src = _R( 192, 0, 256, 64 );
				if (rangerate_azimuth < 0) rect_src = _R( 64, 0, 128, 64 );// -
				else if (rangerate_azimuth > 0) rect_src = _R( 0, 0, 64, 64 );// +
				oapiBlt( surf, g_Param.digits_7seg, &rect_tgt, &rect_src );

				char cbuf[8];
				sprintf_s( cbuf, 8, "%05.1f", fabs( rangerate_azimuth ) );

				int num;
				for (int i = 1, j = 0; i < 5; i++, j++)
				{
					if (cbuf[j] == '.') j++;

					num = cbuf[j] - 48;
					rect_src = _R( NUMX[num], NUMY[num], NUMX[num] + 64, NUMY[num] + 64 );
					rect_tgt = _R( i * 37, 0, (i + 1) * 37, 47 );
					oapiBlt( surf, g_Param.digits_7seg, &rect_tgt, &rect_src );
				}

				rect_src = _R( 184, 56, 192, 64 );// .
				rect_tgt = _R( 140, 37, 148, 45 );
				oapiBlt( surf, g_Param.digits_7seg, &rect_tgt, &rect_src );
			}

			return true;
		}

		return AtlantisPanel::OnVCRedrawEvent( id, _event, surf );
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
		if (pPower_On.IsSet() == false)
		{
			if (power == true)
			{
				power = false;
				STS()->SetAnimation( anim_M1_AZ, 0.5 );
				STS()->SetAnimation( anim_M1_EL, 0.5 );
				oapiVCTriggerRedrawArea( -1, AID_A2_LED1 );
				oapiVCTriggerRedrawArea( -1, AID_A2_LED2 );
				// force repaint next time it's powered up
				range_elevation = 1.23456789;
				rangerate_azimuth = 1.23456789;
			}
			return;
		}

		power = true;

		double az_pos = ((pXPNTR_SCALE_X1 * pAZrate0_2.GetVoltage()) + (!pXPNTR_SCALE_X1 * pAZrate0_20.GetVoltage())) * 0.1 + 0.5;
		double el_pos = ((pXPNTR_SCALE_X1 * pELrate0_2.GetVoltage()) + (!pXPNTR_SCALE_X1 * pELrate0_20.GetVoltage())) * 0.1 + 0.5;
		STS()->SetAnimation( anim_M1_AZ, az_pos );
		STS()->SetAnimation( anim_M1_EL, el_pos );

		if (pDIGI_DIS_SELECT_LAMPTEST.IsSet())
		{
			// test
			if (range_elevation != 8888)
			{
				range_elevation = 8888;
				oapiVCTriggerRedrawArea( -1, AID_A2_LED1 );
			}
			if (rangerate_azimuth != 8888)
			{
				rangerate_azimuth = 8888;
				oapiVCTriggerRedrawArea( -1, AID_A2_LED2 );
			}
		}
		else if (pDIGI_DIS_SELECT_RRbar.IsSet())
		{
			// range
			if (range_elevation != (pRange.GetVoltage() * 2000))
			{
				range_elevation = pRange.GetVoltage() * 2000;
				oapiVCTriggerRedrawArea( -1, AID_A2_LED1 );
			}
			// range rate
			if (rangerate_azimuth != (pRangeRate.GetVoltage() * 2000))
			{
				rangerate_azimuth = pRangeRate.GetVoltage() * 2000;
				oapiVCTriggerRedrawArea( -1, AID_A2_LED2 );
			}
		}
		else
		{
			// elevation
			if (range_elevation != (pElevation.GetVoltage() * 100))
			{
				range_elevation = pElevation.GetVoltage() * 100;
				oapiVCTriggerRedrawArea( -1, AID_A2_LED1 );
			}
			// azimuth
			if (rangerate_azimuth != (pAzimuth.GetVoltage() * 100))
			{
				rangerate_azimuth = pAzimuth.GetVoltage() * 100;
				oapiVCTriggerRedrawArea( -1, AID_A2_LED2 );
			}
		}
		return;
	}

	void PanelA2::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "A2_SWITCHES_DATA", 16 );
		pDIGI_DIS_SELECT->ConnectPort( 1, pBundle, 0 );
		pDIGI_DIS_SELECT->ConnectPort( 2, pBundle, 1 );
		pXPNTR_SCALE->ConnectPort( 1, pBundle, 2 );

		pDIGI_DIS_SELECT_LAMPTEST.Connect( pBundle, 0 );
		pDIGI_DIS_SELECT_RRbar.Connect( pBundle, 1 );
		pXPNTR_SCALE_X1.Connect( pBundle, 2 );
		pRange.Connect( pBundle, 3 );
		pRangeRate.Connect( pBundle, 4 );
		pElevation.Connect( pBundle, 5 );
		pAzimuth.Connect( pBundle, 6 );
		pELrate0_20.Connect( pBundle, 7 );
		pELrate0_2.Connect( pBundle, 8 );
		pAZrate0_20.Connect( pBundle, 9 );
		pAZrate0_2.Connect( pBundle, 10 );

		pBundle = STS()->BundleManager()->CreateBundle( "A1U_SWITCHES_B", 16 );
		pPower_On.Connect( pBundle, 6 );

		AtlantisPanel::Realize();
	}
};